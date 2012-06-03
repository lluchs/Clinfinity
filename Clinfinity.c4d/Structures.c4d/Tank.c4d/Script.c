/*-- Steam Tank --*/

#strict 2

#include L_SS

// number of animation phases
static const STMT_Phases = 10;
// amount needed to respawn one clonk
static const STMT_RespawnAmount = 100;

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
// bubbling?
local bubbling;

protected func Steam() {
	if(GetFill() > MaxFill() * 2 / 3) {
		CreateParticle("Smoke", 30, -18, 0, 0, 50, RGBa(255, 255, 255, 0));
		if(!bubbling) {
			Sound("steam_exhaust", 0, 0, 0, 0, 1);
			bubbling = true;
		}
	} else if(bubbling) {
		Sound("steam_exhaust", 0, 0, 0, 0, -1);
		bubbling = false;
	}
	// generation
	if(!generate--) {
		DoFill(100 + RandomX(-10, 10));
		generate = 500;
	}
	// respawn
	var clonk = Contents();
	if(clonk && MatSysGetTeamFill(GetOwner(), STEM) >= STMT_RespawnAmount) {
		MatSysDoTeamFill(-STMT_RespawnAmount, GetOwner(), STEM);
		clonk->Exit();
		var plr = clonk->GetOwner();
		if(!GetCursor(plr))
			SetCursor(plr, clonk);
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

