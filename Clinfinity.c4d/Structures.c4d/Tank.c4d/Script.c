/*-- Steam Tank --*/

#strict 2

#include L_SS

// number of animation phases
static const STMT_Phases = 10;

public func MaxFill() { return 1500; }

protected func Initialize() {
	SetAction("Tank");
	// create dummy effect if this is the first Akku
	// this ensures that when the last Akku gets destroyed, the display is reset to 0
	if(!GetEffect("MatSysSTEM"))
		AddEffect("MatSysSTEM", 0, 1, 0);
	AddEffect("MatSysSTEM", 0, 1, 0, this);
}

// manages delay for steam generation
local generate;

protected func Steam() {
	CreateParticle("Smoke", 30, -18, 0, 0, 50, RGBa(255, 255, 255, 0));
	// generation
	if(!generate--) {
		DoFill(1);
		generate = 5;
	}
}

private func OnFillChange() {
	UpdateDisplay();
}

private func UpdateDisplay() {
	SetPhase(ChangeRange(GetFill(), 0, MaxFill() - 1, STMT_Phases - 1, 0));
	return 1;
}

// Steuerung
  
protected func ControlUp() {
	Message("$TxtStored$", this, GetFill());
}

/* Material System fill level */
public func FxMatSysSTEMUpdate(object target, int effectNum, int plr) {
	if(!this || GetOwner() != plr)
		return 0;
	return GetFill();
}

public func FxMatSysSTEMChange(object target, int effectNum, int plr, int change) {
	if(!this || GetOwner() != plr)
		return 0;
	return DoFill(change);
}

