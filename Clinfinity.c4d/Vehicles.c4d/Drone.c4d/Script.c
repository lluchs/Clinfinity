
#strict 2

static const DRNE_MovementAngleDeviation = 20;
static const DRNE_MovementSpeed = 15;
static const DRNE_PreciseMovementDistance = 30;
static const DRNE_StopDistance = 2;
static const DRNE_DrillTime = 80;
static const DRNE_DrillRadius = 10;

local targetX, targetY;

protected func Initialize() {
	Stop();
}

public func MoveTo(int x, int y) {
	targetX = x;
	targetY = y;
	SetAction("Fly");
}

public func Stop() {
	targetX = GetX();
	targetY = GetY();
	SetSpeed(0);
	if(GetAction() != "FlyIdle") {
		SetAction("FlyIdle");
	}
}

public func Drill() {
	SetAction("Drill");
}

protected func DecideAction() {
	if(FindObject2(Find_ID(GetID()), Find_AtPoint(0, 0), Find_Exclude(this)) != 0) {
		if(FindDrillingPosition()) {
			SetAction("Fly");
		}
	} else if(GetMaterial(0, 0) != Material("Rock")) {
		if(FindDrillingPosition()) {
			SetAction("Fly");
		}
	} else if(GetMaterial(0, 0) == Material("Rock")) {
		Drill();
	}
	/*

	Drones should:
		- Get a destination from the quarry and fly there
		- Be able to find new rock to drill nearby
		- Detect: If at the destination there already is another drone drilling, find some other place
		- If full: Fly back to the quarry and deposit collected rock
	*/
}

// 
private func FindDrillingPosition() {
	for(var radius = DRNE_DrillRadius * 2; radius < DRNE_DrillRadius * 10; radius += DRNE_DrillRadius) {
		var searchAngle = Random(360);
		for(var j = 0; j < 360; j += 10) {
			var searchX = Sin(searchAngle + j, radius);
			var searchY = -Cos(searchAngle + j, radius);
			if(GetMaterial(searchX, searchY) == Material("Rock")) {
				targetX = searchX + GetX();
				targetY = searchY + GetY();
				return true;
			}
		}
	}
	return false;
}



/* Action-Calls */

protected func Flying() {
	if(Distance(GetX(), GetY(), targetX, targetY) < DRNE_StopDistance) {
		Stop();
		return;
	}
	var angle = Angle(GetX(), GetY(), targetX, targetY);
	if(Distance(GetX(), GetY(), targetX, targetY) > DRNE_PreciseMovementDistance) {
		var angle = RandomX(angle - DRNE_MovementAngleDeviation, angle + DRNE_MovementAngleDeviation);
	}
	var speedX = Sin(angle, DRNE_MovementSpeed);
	var speedY = -Cos(angle, DRNE_MovementSpeed);
	SetSpeed(speedX, speedY);
}

protected func Drilling() {
	SetPosition(GetX(), GetY() + Sin(GetActTime() * 360 / GetActMapVal("Length", GetAction()), 1));
	if(GetActTime() > DRNE_DrillTime) {
		SetAction("Fly");
		BlastFree(0, 0, DRNE_DrillRadius);
	}
}
