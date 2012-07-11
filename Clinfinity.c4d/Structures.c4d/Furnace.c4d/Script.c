/*-- Schmelze --*/

#strict 2

#include STBO
#include L_DC
#include L_SS

static const FNCE_SteamUsage = 25;

public func MaxDamage() { return 35; }

public func DamageGraphics() { return 1; }

protected func ControlUp(object caller) {
	var plr = caller->GetOwner();
	// wrong team?
	if(Hostile(plr, GetOwner())) {
		Sound("CommandFailure1");
		return;
	}
	SetOwner(plr);

	if(MatSysGetTeamFill(plr, ROCK) >= 1 && MatSysGetTeamFill(plr, STEM) >= FNCE_SteamUsage) {
		if(GetAction() != "Start") {
			SetAction("Start");
			MatSysDoTeamFill(-1, plr, ROCK);
			MatSysDoTeamFill(-FNCE_SteamUsage, plr, STEM);
		}
	} else {
		Sound("Error");
		Message("$TxtNotEnoughInput$", this, FNCE_SteamUsage);
	}
}

protected func Progress() {
	if(GetActTime() >= 60) {
		MatSysDoTeamFill(1, GetOwner(), METL);
		SetAction("Attach");
	}
}

