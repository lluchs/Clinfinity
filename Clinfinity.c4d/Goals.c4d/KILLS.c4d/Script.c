#strict 2

#include GOAL

local playerScore;
local totalKills;
local fulfilled;

protected func Completion() {
	CreateObject(RVLR, 0, 0, NO_OWNER);
	playerScore = CreateHash();
	totalKills = 0;
}

public func IsFulfilled() {
	return fulfilled;
}

public func IsFulfilledforPlr(int plr) {
	var win = !FindObject2(Find_ID(STMT), Find_Hostile(plr));
	if(win)
		fulfilled = true;
	return win;
}

protected func InitializePlayer(int playerNumber) {
	HashPut(playerScore, playerNumber, 0);
}

public func OnClonkDeath(object oldClonk, int killingPlayerNumber) {
	// TODO: Award kill to killer
	totalKills++;
	// TODO: Determine if round is over
}
