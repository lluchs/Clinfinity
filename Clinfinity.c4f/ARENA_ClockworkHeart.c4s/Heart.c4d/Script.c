/*-- MOLOCH --*/

#strict 2

func Initialize() {
}

/*	Event Handler: ScoreUpdate
	Handles events from the KILS goal.

	Will open the heart if the round seems to end soon. */
public func ScoreUpdate(object kills, int bestScore, int secondBestScore) {
	if(GetAction() == "Idle" && (
			bestScore - secondBestScore >= (kills->GetWinMargin() - 1) ||
			kills->GetTotalKills() >= (kills->GetWinTotalKills() - 5))) {
		OpenHeart();
	}
}

protected func OpenHeart() {
	Sound("Heart_Open");
	SetAction("Open");
	CreateObject(WIPF, 0, 0);
}

protected func Breathe(obj){
	ScheduleCall(obj, "MakeNoise", 55);
	ScheduleCall(obj, "MakeSmoke", 50);
}

protected func MakeNoise() {
	Sound("Heart_Valve");
}

protected func MakeSmoke() {
	CastParticles("Smoke", RandomX(10, 20), 195, 60, RandomX(65, 75), 50, 70, RGBa(200, 200, 200, 10), RGBa(255, 255, 255, 200));
	CastParticles("Smoke", RandomX(30, 60), 195, 70, 105, 70, 200, RGBa(200, 200, 200, 10), RGBa(255, 255, 255, 200));
	CastParticles("Smoke", RandomX(20, 30), 195, 75, 120, 400, 600, RGBa(200, 200, 200, 10), RGBa(255, 255, 255, 240));
}

protected func MakeSmoke2() {	//(TimerCall)
	Sound("Steam*.wav");
	CastParticles("Smoke", RandomX(20, 30), RandomX(30, 60), -114, 82, 50, 70, RGBa(200, 200, 200, 10), RGBa(200, 200, 200, 190));
	CastParticles("Smoke", RandomX(20, 30), RandomX(30, 60), -103, 71, 50, 70, RGBa(200, 200, 200, 10), RGBa(200, 200, 200, 190));
	CastParticles("Smoke", RandomX(20, 30), RandomX(30, 60), -89, 60, 50, 70, RGBa(200, 200, 200, 10), RGBa(200, 200, 200, 190));
}
