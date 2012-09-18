/*-- Schmelze --*/

#strict 2

#include STBO
#include L_DC
#include L_SS

static const FNCE_SteamUsage = 25;

public func MaxDamage() { return 35; }

public func DamageGraphics() { return 1; }

local productionNum;

private func Authorized(object clonk) {
	var plr = clonk->GetOwner();
	// wrong team?
	if(Hostile(plr, GetOwner())) {
		Sound("CommandFailure1");
		return;
	}
	SetOwner(plr);
	return true;
}

protected func ControlUp(object clonk) {
	if(Authorized(clonk))
		Produce();
}

protected func ControlUpDouble(object clonk) {
	if(!Authorized(clonk))
		return;
	var menu = CreateMenuTemplate(GetID());
	AddRangeChoice(menu, 0, "n", 0, "$Production$", METL, 2, 100, 1, 5);
	CreateMenuByTemplate(clonk, this, "StartMassProduction", menu);
}

public func StartMassProduction(array selection, extraData, abort) {
	if(!abort) {
		productionNum = HashGet(selection, "n") - 1;
		Produce();
	}
}

private func Produce() {
	var plr = GetOwner();
	if(MatSysGetTeamFill(plr, ROCK) >= 1 && MatSysGetTeamFill(plr, STEM) >= FNCE_SteamUsage) {
		if(GetAction() != "Start") {
			SetAction("Start");
			MatSysDoTeamFill(-1, plr, ROCK);
			MatSysDoTeamFill(-FNCE_SteamUsage, plr, STEM);
			return true;
		}
	} else {
		Sound("Error");
		Message("$TxtNotEnoughInput$", this, FNCE_SteamUsage);
		productionNum = 0;
	}
}

protected func Progress() {
	if(GetActTime() >= 60) {
		MatSysDoTeamFill(1, GetOwner(), METL);
		SetAction("Attach");
		if(productionNum > 0) {
			productionNum--;
			Produce();
		}
	}
}

