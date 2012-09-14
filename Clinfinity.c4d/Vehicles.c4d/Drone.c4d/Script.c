
#strict 2

static const DRNE_MovementAngleDeviation = 20;
static const DRNE_MovementSpeed = 15;
static const DRNE_PreciseMovementDistance = 30;
static const DRNE_StopDistance = 2;
static const DRNE_DrillTime = 80;
static const DRNE_DrillRadius = 10;
static const DRNE_MaxRockCollection = 20;

local creationFrame;
local myQuarry;
local drilledMaterial, collectedMaterial;
local targetX, targetY;

public func CreateDrone(int x, int y, int owner, object forQuarry) {
	var drone = CreateObject(DRNE, x, y, owner);
	drone->LocalN("myQuarry") = forQuarry;
	return drone;
}

protected func Initialize() {
	creationFrame = FrameCounter();
	Stop();
	drilledMaterial = "Metalearth";
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
		SetPhase(Random(GetActMapVal("Length", GetAction())));
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
		if(FrameCounter() > creationFrame) {
			if(GetAction() != "Idle") {
				SetAction("Idle");
				FadeOut();
			}
		}
	} else if(IsAtQuarry()) {
		MoveRockToQuarry();
		if(myQuarry->~FindDrillingPosition(drillX, drillY)) {
			MoveTo(drillX, drillY);
		}
	} else if(ContentsCount(collectedMaterial) >= DRNE_MaxRockCollection) {
		MoveToQuarry();
	} else if(IsOtherDroneDrillingHere(0, 0) || !IsRockHere(0, 0)) {
		if(FindDrillingPosition(drillX, drillY)) {
			MoveTo(drillX, drillY);
		} else {
			MoveToQuarry();
		}
	} else {
		Drill();
	}
}

private func MoveToQuarry() {
	var width = GetDefWidth(GetID(myQuarry));
	var height = GetDefHeight(GetID(myQuarry));
	var x = myQuarry->GetX() + RandomX(-width / 4, width / 4);
	var y = myQuarry->GetY() + RandomX(-height / 4, height / 4);
	MoveTo(x, y);
}

private func IsAtQuarry() {
	return InArray(myQuarry, FindObjects(Find_AtPoint(0, 0)));
}

private func MoveRockToQuarry() {
	var rocks = FindObjects(Find_ID(collectedMaterial), Find_Container(this));
	for(var rock in rocks) {
		rock->Enter(myQuarry);
	}
}

private func IsOtherDroneDrillingHere(int x, int y) {
	return FindObject2(Find_ID(GetID()), Find_AtPoint(x, y), Find_Exclude(this), Find_Action("Drill")) != 0;
}

private func IsRockHere(int x, int y) {
	return GetMaterial(x, y) == Material(drilledMaterial);
}

private func FindDrillingPosition(&x, &y) {
	for(var radius = DRNE_DrillRadius * 2; radius < DRNE_DrillRadius * 10; radius += DRNE_DrillRadius) {
		var searchAngle = Random(360);
		for(var j = 0; j < 360; j += 10) {
			var searchX = Sin(searchAngle + j, radius);
			var searchY = -Cos(searchAngle + j, radius);
			if(!IsOtherDroneDrillingHere(searchX, searchY) && IsRockHere(searchX, searchY)) {
				x = searchX + GetX();
				y = searchY + GetY();
				return true;
			}
		}
	}
	return false;
}


/* Action-Calls */

protected func Idling() {
	SetPosition(targetX, targetY + Sin(GetPhase() * 360 / GetActMapVal("Length", GetAction()), 1));
}

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
	SetPosition(GetX(), GetY() + Sin(GetActTime() * 2 * 360 / GetActMapVal("Length", GetAction()), 1));
	if(GetActTime() > DRNE_DrillTime) {
		SetAction("Fly");
		BlastFree(0, 0, DRNE_DrillRadius);
	}
}
