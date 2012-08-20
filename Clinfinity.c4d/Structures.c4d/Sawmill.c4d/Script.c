/*-- Sawmill --*/

#strict 2

#include STBO
#include L_DC
#include L_SS

public func MaxDamage() { return 40; }

protected func Initialize() {
	inherited(...);
	SetAction("GrabWood");
}

protected func Finish() {
	MatSysDoTeamFill(1, GetOwner(), WOOD);
}
