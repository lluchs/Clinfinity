/*--Panel--*/

#strict 2

local iPhases, iEnergy;

protected func Initialize() {
	iPhases = GetActMapVal("Length", "Panel", GetID());
	SetAction("Panel");
}

protected func CheckDay() { // TimerCall
	// might add some time check
	ProduceSteam();
}

private func ProduceSteam() {
	//regulates Power-producing by Sunstrength (powered by Luchs)
	iEnergy = 15 * PhaseFree() / 3;
	var water = MatSysGetFill(GetOwner(), WBRL);
	var change = Min(iEnergy, water);
	var actualChange = MatSysDoFill(-change, GetOwner(), WBRL);
	MatSysDoFill(-actualChange, GetOwner(), STEM);
}

private func PhaseFree() {
	var iPhase = GetPhase();
	var iAngle = 270;
	if(iPhase < 18)
		iAngle -= iPhase * 2;
	else if(iPhase < 21)
		iAngle -= iPhase * 3;
	else if(iPhase < 33)
		iAngle -= iPhase * 11 / 3;
	else if(iPhase < 43)
		iAngle -= iPhase * 4;
	else
		iAngle = 90 + 50 - iPhase;
	
	return AngleFree(iAngle) + AngleFree(iAngle - 15) + AngleFree(iAngle + 15);
}

/* Steuerung */

// aktuell erzeugte Energie anzeigen
protected func ControlUp() {
	[$TxtShowEnergy$|Image=CXEC]
	Message("$TxtShowEnergyMsg$", this, iEnergy);
}

protected func ContextShowEnergy() {
	[$TxtShowEnergy$|Image=CXEC]
	ControlUp();
}

protected func ControlLeft() {
	var phase = GetPhase() - 1;
	if(phase >= 0)
		SetPhase(phase);
	else
		Sound("Click");
}

protected func ControlRight() {
	var phase = GetPhase() + 1;
	if(phase < iPhases)
		SetPhase(phase);
	else
		Sound("Click");
}

public func MaxDamage() { return 21; } //Maximaler Schaden
