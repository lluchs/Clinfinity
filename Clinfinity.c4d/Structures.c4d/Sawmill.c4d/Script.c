/*-- Sawmill --*/

#strict 2

#include STBO
#include L_DC
#include L_SS

public func MaxDamage() { return 20; }

protected func Initialize() {
	inherited(...);
	SetAction("GrabWood");
}

protected func ControlUp() {
  Message("Platzhalter für Baum-Radius Anzeige");
}

protected func Finish(obj) {
	MatSysDoTeamFill(1, GetOwner(), WOOD);
	ScheduleCall(obj, "Initialize", 300);
}
