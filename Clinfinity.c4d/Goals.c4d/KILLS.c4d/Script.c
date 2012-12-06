#strict 2

#include RULE
#include GOAL

local playerScore;
local totalKills;
local isFulfilled;

protected func Completion() {
	CreateObject(RVLR, 0, 0, NO_OWNER);
	playerScore = CreateHash();
	totalKills = 0;
}

public func IsFulfilled() {
	return isFulfilled;
}

public func IsFulfilledforPlr(int plr) {
	var win = !FindObject2(Find_ID(STMT), Find_Hostile(plr));
	if(win)
		isFulfilled = true;
	return win;
}

protected func InitializePlayer(int playerNumber) {
	HashPut(playerScore, playerNumber, 0);
}

public func OnClonkDeath(object oldClonk, int killingPlayerNumber) {
	var currentScore = HashGet(playerScore, killingPlayerNumber) + 1;
	HashPut(playerScore, killingPlayerNumber, currentScore);
	totalKills++;
	// TODO: Determine if round is over
}
