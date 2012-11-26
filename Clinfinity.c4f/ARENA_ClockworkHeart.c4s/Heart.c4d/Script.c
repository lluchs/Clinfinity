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

protected func Breathe(obj){
  ScheduleCall(obj, "MakeNoise", 55);
	CastParticles("Smoke", 500, 200, 80, 95);
	CreateObject(WIPF, 80, 95);
}

protected func MakeNoise(){
	Sound("Heart_Valve");
}
