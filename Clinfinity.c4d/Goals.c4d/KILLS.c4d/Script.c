#strict 2

#include GOAL
#include RULE

local winMargin, winTotalKills;
local playerScores, totalKills;
local isFulfilled, winner;

protected func Initialize() {
	CreateObject(RVLR, 0, 0, NO_OWNER);
	playerScores = CreateHash();
	totalKills = 0;
	isFulfilled = false;
	winner = NO_OWNER;
	return _inherited();
}

private func Initialized(int ruleTypeCount) {
	winMargin = ruleTypeCount;
	winTotalKills = ruleTypeCount * 4;
}

public func IsFulfilled() {
	return isFulfilled;
}

public func IsFulfilledforPlr(int player) {
	return winner == player || (isFulfilled && winner == NO_OWNER);
}

protected func InitializePlayer(int playerNumber) {
	HashPut(playerScores, playerNumber, 0);
}

public func OnClonkDeath(object oldClonk, int killingPlayerNumber) {
	// No kills awarded for suicide or for the game engine
	if(oldClonk->GetOwner() == killingPlayerNumber || killingPlayerNumber == NO_OWNER) return;

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
	} else if(totalKills >= winTotalKills) {
		// TODO: Instead of having everyone win under this condition, make the best players (there may be more than one with the same score!) the winners
		isFulfilled = true;
	}
}
