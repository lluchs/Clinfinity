/*-- Schmelze --*/

#strict 2

#include L_SS

protected func ControlUp(object caller) {
	var plr = caller->GetOwner();
	// wrong team?
	if(Hostile(plr, GetOwner())) {
		Sound("CommandFailure1");
		return;
	}
	SetOwner(plr);

	if(MatSysGetTeamFill(plr, ROCK) >= 1) {
		if(GetAction() != "Start") {
			SetAction("Start");
			MatSysDoTeamFill(-1, plr, ROCK);
		}
	} else {
		Sound("Error");
		Message("$TxtNotEnoughInput$", this);
	}
}

protected func Finish() {
	MatSysDoTeamFill(1, GetOwner(), METL);
}

