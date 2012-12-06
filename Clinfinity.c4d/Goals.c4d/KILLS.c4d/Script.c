#strict 2

#include RULE
#include GOAL

local winMargin, winTotalKills;
local playerScores, totalKills;
local isFulfilled;

private func Initialized(int ruleTypeCount) {
	CreateObject(RVLR, 0, 0, NO_OWNER);
	winMargin = ruleTypeCount;
	winTotalKills = ruleTypeCount * 4;
	playerScores = CreateHash();
	totalKills = 0;
}

public func IsFulfilled() {
	return isFulfilled;
}

public func IsFulfilledforPlr(int plr) {
	// TODO
	return false;
}

protected func InitializePlayer(int playerNumber) {
	HashPut(playerScores, playerNumber, 0);
}

public func OnClonkDeath(object oldClonk, int killingPlayerNumber) {
	var currentScore = HashGet(playerScores, killingPlayerNumber);
	HashPut(playerScores, killingPlayerNumber, currentScore + 1);
	totalKills++;

	// TODO: Determine if round is over
	var bestPlayer, bestScore, marginToSecondBest;
	var iterator = HashIter(playerScores);
	while(HashIterHasNext(iterator)) {
		var entry = HashIterNext(iterator);
		var player = entry[0];
		var score = entry[1];

		if(score > bestScore) {
			marginToSecondBest = score - bestScore;
			bestScore = score;
			bestPlayer = player;
		}
	}

	if((marginToSecondBest >= winMargin) || (totalKills >= winTotalKills)) {
		// TODO: GAME OUVEURE
	}
}
