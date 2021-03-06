/*-- Flint Factory --*/

#strict 2

#include L_CP	//ControlPoint
#include B100 //Basement
#include NLBO //Nightlight

static const RFLN_MaxFlints = 4;

public func CaptureZone() {
	// complete building
	return Find_InRect(-50, -40, 100, 80);
}

public func CaptureTime() { return 200; }

public func NoDemolition() { return true; }

protected func Initialize() {
	UpdateDisplay();
	// position the display correctly
	SetObjDrawTransform(1000, 0, -25000, 0, 1000, 34000, this, 1);
	
	return inherited(...);
}

private func NumberOfFlints() {
	return ObjectCount2(Find_Container(this));
}

public func UpdateDisplay() {
	var n = NumberOfFlints();
	SetGraphics(0, this, FFFD, 1, GFXOV_MODE_Action, Format("Fill%d", n));
}

private func Captured() {
	for(var obj in FindObjects(Find_Container(this)))
		obj->RemoveObject();
	UpdateDisplay();
	StartProduction();
	return inherited(...);
}

protected func DoSmoke(){
	CreateParticle("Smoke", -13, -18, 0, 0, 70, RGBa(255, 255, 255, 0));
}

private func StartProduction() {
	if(GetAction() == "Idle" && NumberOfFlints() < RFLN_MaxFlints)
		SetAction("Producing");
}

protected func ProduceFlint() {
	var flintID, n = Random(8);
	if(!n)         flintID = EFLN;
	else if(n < 4) flintID = SFLN;
	else           flintID = FLNT;
	CreateContents(flintID);
	UpdateDisplay();
	StartProduction();
}

protected func ControlDig(object clonk) {
	if(!Hostile(clonk->GetOwner(), GetOwner())) {
		var flint = FindObject2(Find_Container(this), Sort_Random());
		if(flint) {
			flint->Exit(0, AbsX(clonk->GetX()), AbsY(clonk->GetY()));
			UpdateDisplay();
			StartProduction();
		}
	}
	return true;
}
