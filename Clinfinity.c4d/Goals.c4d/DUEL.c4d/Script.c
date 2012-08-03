/*-- DUEL GOAL --*/

#strict 2

#include GOAL

local fulfilled;

protected func Completion() {
	CreateObject(RVLR, 0, 0, NO_OWNER);
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
