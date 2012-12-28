/*-- Sawmill --*/

#strict 2

#include STBO	//StructureBaseObject
#include NLBO	//Nightlight
#include L_DC	//Damagecontrol
#include L_SS	//StorageSystem

static const SAWM_RADIUS = 350; // Radius in px
static const SAWM_DELAY = 300;

public func MaxDamage() { return 20; }

local chopping;

protected func Initialize(sawmill) {
	chopping = true;
	StartChopping();
	inherited(...);
}

protected func ControlUp() {
	[$TxtShowSearchRadius$]
	//show Radius
	for(var i; i < 360; i++)
		CreateParticle("PSpark", Cos(i, SAWM_RADIUS), Sin(i, SAWM_RADIUS), 0, 0, 70, RGBa(255, 255, 255, 128));
}

protected func ControlDig(object clonk) {
	if(Hostile(GetOwner(), clonk->GetOwner())) {
		Sound("CommandFailure1");
		return;
	}
	
	if(chopping) {
		chopping = false;
		ClearScheduleCall(this, "Chop");
		Sound("Command");
	} else {
		chopping = true;
		StartChopping();
		Sound("Ding");
	}
}

public func StartChopping() {
	ScheduleCall(this, "Chop", SAWM_DELAY);
}

protected func Chop() {
	// Find the biggest tree
	var tree = FindObject2(Find_Distance(SAWM_RADIUS), Find_Func("IsTree"), Sort_Reverse(Sort_Func("GetCon")));
	if(tree) {
		if(tree->Shrink())
			SetAction("GrabWood");
	} else {
		StartChopping();
	}
}

protected func Finish() {
	MatSysDoTeamFill(2, GetOwner(), WOOD);
	StartChopping();
}
