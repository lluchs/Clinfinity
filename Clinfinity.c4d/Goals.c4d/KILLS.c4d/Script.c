#strict 2

#include GOAL
#include RULE

local winMargin;
local playerScores, totalKills;
local isFulfilled, winner, winningScore;

protected func Initialize() {
	CreateObject(RVLR, 0, 0, NO_OWNER);
	playerScores = CreateHash();
	totalKills = 0;
	isFulfilled = false;
	winner = NO_OWNER;
	InitializeScoreboard();
	return _inherited();
}

private func Initialized(int ruleTypeCount) {
	winMargin = ruleTypeCount;
}

public func GetTotalKills() {
	return totalKills;
}

public func GetWinMargin() {
	return winMargin;
}

public func GetWinTotalKills() {
	return GetPlayerCount() * winMargin * 4;
}

public func IsFulfilled() {
	return isFulfilled;
}

public func IsFulfilledforPlr(int player) {
	var pid = GetPlayerID(player);
	return winner == pid || (isFulfilled && HashGet(playerScores, pid) >= winningScore);
}

protected func InitializePlayer(int playerNumber) {
	HashPut(playerScores, GetPlayerID(playerNumber), 0);
}

public func OnClonkDeath(object oldClonk, int killingPlayerNumber) {
	// No kills awarded for suicide or for the game engine
	if(oldClonk->GetOwner() == killingPlayerNumber || killingPlayerNumber == NO_OWNER) {
		// But maybe somebody has just dealt damage?
		var finishedOff = oldClonk->FinishedOff();
		if(finishedOff == NO_OWNER)
			return;
		else
			killingPlayerNumber = finishedOff;
	}

	killingPlayerNumber = GetPlayerID(killingPlayerNumber);

	var currentScore = HashGet(playerScores, killingPlayerNumber);
	HashPut(playerScores, killingPlayerNumber, currentScore + 1);
	totalKills++;

	var bestPlayer, bestScore, secondBestScore;
	var iterator = HashIter(playerScores);
	while(HashIterHasNext(iterator)) {
		var entry = HashIterNext(iterator);
		var player = entry[0];
		var score = entry[1];

		if(score > bestScore) {
			bestScore = score;
			bestPlayer = player;
		} else if(score > secondBestScore) {
			secondBestScore = score;
		}
	}

	if((bestScore - secondBestScore) >= winMargin) {
		winner = bestPlayer;
		isFulfilled = true;
	} else if(totalKills >= GetWinTotalKills()) {
		winningScore = bestScore;
		isFulfilled = true;
	}

	UpdateScoreboard(bestScore, secondBestScore);
	// Emit an event for external handling.
	Emit("ScoreUpdate", bestScore, secondBestScore);
}

/* Scoreboard */

private func InitializeScoreboard() {
	SetScoreboardData(SBRD_Caption, SBRD_Caption, "KILLS");
	SetScoreboardData(SBRD_Caption, 0, "{{AVTR}}");
	SetScoreboardData(SBRD_Caption, 1, "{{MUSK}}");
	SetScoreboardData(SBRD_Caption, 2, "{{RVLR}}");
}

private func UpdateScoreboard(int bestScore, int secondBestScore) {
	SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("KILLS: %d (%d to go)", totalKills, Max(GetWinTotalKills() - totalKills, 0)));
	for(var count = GetPlayerCount(), i = 0; i < count; i++) {
		var p = GetPlayerByIndex(i), name = GetPlayerName(p), pid = GetPlayerID(p);
		var score = HashGet(playerScores, pid), needed;
		if(score == bestScore)
			needed = winMargin - (bestScore - secondBestScore);
		else
			needed = winMargin + bestScore - score;
		SetScoreboardData(pid, 0, name);
		SetScoreboardData(pid, 1, Format("%d", score));
		SetScoreboardData(pid, 2, Format("%d", needed));
	}
	SortScoreboard(1, true);
}
