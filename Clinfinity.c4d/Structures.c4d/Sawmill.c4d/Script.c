/*-- Sawmill --*/

#strict 2

#include STBO
#include L_DC
#include L_SS

static const TRFM_RADIUS = 350; // Radius in px

public func MaxDamage() { return 20; }

protected func Initialize(sawmill) {
	inherited(...);

	var tree = FindObject2(Find_Distance(350, 0, 0), Find_ID(TRE1));
	  
	if(tree){
	  tree -> RemoveObject();
	  SetAction("GrabWood");
	  } else {
	  ScheduleCall(sawmill, "Initialize", 300);
	}
}

protected func ControlUp() {
  //show Radius
  for(var i; i < 360; i++)
	CreateParticle("PSpark", Cos(i, TRFM_RADIUS), Sin(i, TRFM_RADIUS), 0, 0, 70, RGBa(255, 255, 255, 128));
}

protected func Finish(obj) {
	MatSysDoTeamFill(1, GetOwner(), WOOD);
	ScheduleCall(obj, "Initialize", 300);
}
