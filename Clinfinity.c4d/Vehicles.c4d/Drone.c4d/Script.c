
#strict 2

static const DRNE_MovementAngleDeviation = 20;
static const DRNE_MovementSpeed = 15;
static const DRNE_PreciseMovementDistance = 30;
static const DRNE_StopDistance = 2;
static const DRNE_DrillTime = 80;
static const DRNE_DrillRadius = 10;
static const DRNE_MaxRockCollection = 20;

local myQuarry;
local drilledMaterial, collectedMaterial;
local targetX, targetY;

public func CreateDrone(int x, int y, int owner, object forQuarry) {
	var drone = CreateObject(DRNE, x, y, owner);
	drone->LocalN("myQuarry") = forQuarry;
	//drone->FadeIn();
	return drone;
}

protected func Initialize() {
	Stop();
	drilledMaterial = "Rock";
	collectedMaterial = ROCK;
}

protected func RejectCollect(id collectedObjectId, object collectedObject) {
	return collectedObjectId != collectedMaterial;
}



/* Actions/Commands */

public func MoveTo(int x, int y) {
	targetX = x;
	targetY = y;
	if(GetAction() != "Fly") {
		SetAction("Fly");
	}
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
	if(GetAction() != "Drill") {
		SetAction("Drill");
	}
}

protected func DecideAction() {
	var drillX, drillY;
	if(myQuarry == 0) {
		// TODO: What should happen if the quarry is missing?
		/* Ideas:
			- Incinerate
			- Explode
			- Fall out of the landscape
			- Fade out and get removed
		*/
	} else if(ContentsCount(collectedMaterial) >= DRNE_MaxRockCollection) {
		// TODO: Empty contents when arrived at quarry
		MoveTo(myQuarry->GetX(), myQuarry->GetY());
	} else if(IsOtherDroneDrillingHere() || !IsRockHere()) {
		if(FindDrillingPosition(drillX, drillY)) {
			MoveTo(drillX, drillY);
		} else {
			MoveTo(myQuarry->GetX(), myQuarry->GetY());
		}
	} else {
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

private func IsOtherDroneDrillingHere() {
	return FindObject2(Find_ID(GetID()), Find_AtPoint(0, 0), Find_Exclude(this), Find_Action("Drill")) != 0;
}

private func IsRockHere() {
	return GetMaterial(0, 0) == Material(drilledMaterial);
}

// 
private func FindDrillingPosition(&x, &y) {
	for(var radius = DRNE_DrillRadius * 2; radius < DRNE_DrillRadius * 10; radius += DRNE_DrillRadius) {
		var searchAngle = Random(360);
		for(var j = 0; j < 360; j += 10) {
			var searchX = Sin(searchAngle + j, radius);
			var searchY = -Cos(searchAngle + j, radius);
			if(GetMaterial(searchX, searchY) == Material(drilledMaterial)) {
				x = searchX + GetX();
				y = searchY + GetY();
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
