/*-- MOLOCH --*/

#strict 2

func Initialize(obj) {
	ScheduleCall(obj, "OpenHeart", 500);
  return(1);
}

protected func OpenHeart(){
	SetAction("Open");
	}

protected func Breathe(){
	Log("Test");
	CreateParticle("Smoke", 30, -18, 0, 0, 50, RGBa(255, 255, 255, 0));
  }
