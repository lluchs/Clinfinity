/*-- DUEL GOAL --*/

#strict 2

#include GOAL

protected func Completion() {
	CreateObject(RVLR, 0, 0, NO_OWNER);
}

public func IsFulfilledforPlr(int plr) {
	return !FindObject2(Find_ID(STMT), Find_Hostile(plr));
}
