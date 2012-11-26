/*-- MOLOCH --*/

#strict 2

func Initialize(obj) {
	//ScheduleCall(obj, "OpenHeart", 500);
	SetAction("Work");
  return(1);
}

protected func OpenHeart() {
	SetAction("Open");
	}

protected func Breathe(){
	CastParticles("Smoke", 500, 200, 80, 95);
	CreateObject(WIPF, 80, 95);
}
