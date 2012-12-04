#strict 2

#include GOAL

local playerScore;
local fulfilled;

protected func Completion() {
	CreateObject(RVLR, 0, 0, NO_OWNER);
	playerScore = CreateHash();
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
	// TODO: Set player score to 0
}

public func OnClonkDeath(object oldClonk) {
	// TODO: Award kill to killer
	// TODO: Determine if round is over
}
