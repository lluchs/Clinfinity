
#strict 2

static const DRNE_MovementAngleDeviation = 20;
static const DRNE_MovementSpeed = 15;
static const DRNE_PreciseMovementDistance = 30;
static const DRNE_StopDistance = 2;
static const DRNE_DrillTime = 80;
static const DRNE_DrillRadius = 10;

local targetX, targetY;

protected func Initialize() {
	targetX = GetX();
	targetY = GetY();
	SetAction("Fly");
}

public func MoveTo(int x, int y) {
	targetX = x;
	targetY = y;
}

protected func Flying() {
	if(Distance(GetX(), GetY(), targetX, targetY) < DRNE_StopDistance) {
		SetSpeed(0);
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
