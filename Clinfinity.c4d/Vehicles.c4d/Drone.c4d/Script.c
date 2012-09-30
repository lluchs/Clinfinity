/*	Script: Drone
	Flying robot for drilling solid material.

	A drone needs a "home quarry" where it asks for coordinates of material veins and where it delivers the drilled products.
	After receiving a set of coordinates, the drone will move there and search a place to drill as nearby as possible.
	It will drill the material and collect the resulting objects.
	When a configurable number of these objects has been collected,
	the drone will fly back to its quarry and deliver the objects there. */

#strict 2

/*	Constants: Movement
	DRNE_MovementAngleDeviation		- When flying towards a target, this is the maximum deviation from flying straight.
	DRNE_MovementSpeed				- Drone speed.
	DRNE_PreciseMovementDistance	- When reaching this distance from its target, the drone will fly straight.
	DRNE_StopDistance				- When reaching this distance from its target, the drone will stop. */
static const DRNE_MovementAngleDeviation = 20;
static const DRNE_MovementSpeed = 15;
static const DRNE_PreciseMovementDistance = 30;
static const DRNE_StopDistance = 2;

/*	Constants: Drilling
	DRNE_DrillTime			- Time (in frames) that the drilling animation is played.
	DRNE_DrillRadius		- Radius of the drilled hole.
	DRNE_MaxRockCollection	- Number of objects to collect before returning to the quarry. */
static const DRNE_DrillTime = 80;
static const DRNE_DrillRadius = 10;
static const DRNE_MaxRockCollection = 20;

local creationFrame;
local myQuarry;
local drilledMaterial, collectedMaterial;
local targetX, targetY;

/*	Function: CreateDrone
	Factory method for drones.
	The coordinates are relative to the calling object in local calls, otherwise global.
	*Note:* You should always create a drone using this method.

	Parameters:
	x					- Horizontal coordinate.
	y					- Vertical coordinate.
	owner				- Owner of the created drone: Player index. Use NO_OWNER for ownerless drones.
	forQuarry			- Home quarry of the drone.
	drilledMaterial		- [optional] Name of the material to drill.
	collectedMaterial	- [optional] ID of the objects to collect.

	Returns:
	The created drone. */
public func CreateDrone(int x, int y, int owner, object forQuarry, string drilledMaterial, id collectedMaterial) {
	var drone = CreateObject(DRNE, x, y, owner);
	drone->LocalN("myQuarry") = forQuarry;
	if(drilledMaterial != 0) {
		drone->LocalN("drilledMaterial") = drilledMaterial;
	}
	if(collectedMaterial != 0) {
		drone->LocalN("collectedMaterial") = collectedMaterial;
	}
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


/*	Section: Actions/Commands */

/*	Function: MoveTo
	Tells the drone to move to a position.

	Parameters:
	x	- Horizontal coordinate.
	y	- Vertical coordinate. */
public func MoveTo(int x, int y) {
	targetX = x;
	targetY = y;
	if(GetAction() != "Fly") {
		SetAction("Fly");
	}
}

/*	Function: Stop
	Tells the drone to stop. */
public func Stop() {
	targetX = GetX();
	targetY = GetY();
	SetSpeed(0);
	if(GetAction() != "FlyIdle") {
		SetAction("FlyIdle");
		SetPhase(Random(GetActMapVal("Length", GetAction())));
	}
}

/*	Function: Drill
	Makes the drone play the drilling animation. */
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
  Sound("drill*");
	SetPosition(GetX(), GetY() + Sin(GetActTime() * 2 * 360 / GetActMapVal("Length", GetAction()), 1));
	var colourIndex = Random(3);
	var red =	GetMaterialColor(Material(drilledMaterial), colourIndex, 0);
	var green =	GetMaterialColor(Material(drilledMaterial), colourIndex, 1);
	var blue =	GetMaterialColor(Material(drilledMaterial), colourIndex, 2);
	CastParticles("MatSpark", RandomX(1, 3), RandomX(10, 20), 0, 4, 20, 40, RGBa(red, green, blue, 50), RGBa(red, green, blue, 50));
	if(GetActTime() > DRNE_DrillTime) {
		SetAction("Fly");
		BlastFree(0, 0, DRNE_DrillRadius);
	}
}
