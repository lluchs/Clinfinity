/*-- Battery --*/

#strict 2

#include L_SS

public func MaxFill() { return 1500; }

protected func Initialize() {
	// create dummy effect if this is the first Akku
	// this ensures that when the last Akku gets destroyed, the display is reset to 0
	if(!GetEffect("MatSysSTEM"))
		AddEffect("MatSysSTEM", 0, 1, 0);
	AddEffect("MatSysSTEM", 0, 1, 0, this);
}

public func Deconstruction() {
	//Overlay löschen
	SetGraphics(0, this, 0, 1);
	
	return _inherited(...);
}

private func OnFillChange() {
	UpdateDisplay();
}

private func UpdateDisplay() {
	SetGraphics(0, this, _BAR, 1, GFXOV_MODE_Action, Format("%d/5", GetFill() * 5 / MaxFill()));
	SetObjDrawTransform(1000, 0, -13500, 0, 1000, 1000, this, 1);
	return 1;
}

// Steuerung
  
protected func ControlUp() {
	Message("$TxtStored$", this, GetFill());
}

public func MaxDamage() { return 25; } //Maximaler Schaden

/* Anzeige-fix nach Spielstand laden */

protected func UpdateTransferZone() {
	UpdateDisplay();
}

/* Material System fill level */
public func FxMatSysSTEMUpdate(object target, int effectNum, int plr) {
	if(!this || Hostile(GetOwner(), plr))
		return 0;
	return GetFill();
}

