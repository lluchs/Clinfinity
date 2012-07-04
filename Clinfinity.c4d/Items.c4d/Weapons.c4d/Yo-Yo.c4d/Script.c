/*	Script: Yo-yo
	A short-range throwing weapon.

	Technical details:
	Internally, the yo-yo object is in one of three discrete states:
	* Inactive
	* Thrown
	* Returning

	When inactive, the yo-yo exhibits no special behaviour. This is the default state.

	In the "Thrown" state, activated when a Clonk (or any other type of crew member) throws the yo-yo, the following rules apply
	* The yo-yo flies in a regular flight path, determined by the Clonk engine.
	* Its starting speed is different from that of other objects, to be able to hit Clonks standing in front of the thrower.
	* A line is drawn between the thrower and the yo-yo.
	* After a set timespan or when hitting a living being, the yo-yo automatically switches do the "Returning" state.
	* Nobody but its thrower can collect the yo-yo.

	In the returning state, the yo-yo moves back to the original thrower, straight into its current direction.
	Initially, it moves at a set maximum speed, but slows down to not injure the thrower.
	When close enough, the following happens
	* The yo-yo gets moved into the thrower's inventory.
	* The drawn line is removed.
	* The yo-yo switches to the "Inactive" state.

	Two events, namely death and removal of the thrower, make the yo-yo switch to the "Inactive" state immediately. */

#strict 2

/*	Constants: Internal yo-yo states
	YOYO_StateInactive	- Denotes the inactive state without special behaviour.
	YOYO_StateThrown	- Denotes the state: Yo-yo has been thrown.
	YOYO_StateReturning	- Denotes the state: Yo-yo moves back towards the original thrower. */
static const YOYO_StateInactive = 0;
static const YOYO_StateThrown = 1;
static const YOYO_StateReturning = 2;

/*	Constants: Yo-yo flight
	YOYO_ThrowFlightTime		- The time between getting thrown and switching to the "Returning" state.
	YOYO_ReturnMaxSpeed			- Maximum speed the yo-yo moves while returning to the thrower.
	YOYO_ReturnSlowDownDistance	- Distance where the yo-yo slows down when approaching the thrower.
	YOYO_MaxCollectionDistance	- Maximum distance in which the yo-yo returns itself to the thrower's inventory. */
static const YOYO_ThrowFlightTime = 12;
static const YOYO_ReturnMaxSpeed = 50;
static const YOYO_ReturnSlowDownDistance = 5;
static const YOYO_MaxCollectionDistance = 10;

/*	Constants: Yo-yo hits
	YOYO_Damage				- Damage one single yo-yo hit does.
	YOYO_MaxHitMoveDistance	- Maximum distance a target that was hit gets knocked away from the thrower.
	YOYO_FlingTargetChance	- Determines the chance for a "lucky strike" that flings a target. The chance is calculated as 1/YOYO_FlingTargetChance.
	YOYO_FlingSpeed			- Fling speed for the "lucky strike". */
static const YOYO_Damage = 2;
static const YOYO_MaxHitMoveDistance = 5; // 
static const YOYO_FlingTargetChance = 6; // Chance: 1 of YOYO_FlingTargetChance is a lucky strike/critical hit
static const YOYO_FlingSpeed = 2;

local thrower;
local line;
local currentState;

/*	Section: Events */

protected func Initialize() {
	YoyoInactive();
}

public func ContainerThrow() {
	var container = Contained();
	// Thrown in flight by a Clonk: Act as weapon.
	if((container->GetOCF() & OCF_CrewMember) != 0 && container->GetAction() == "Jump" && !GetPlrDownDouble(container->GetOwner())) {
		var exitX = -9 + container->GetDir() * 18;
		var exitY = 0;
		var exitXSpeed = -2 + container->GetDir() * 4;
		var exitYSpeed = 2;
		if(container->~IsGliding()) {
			exitX += container->GetXDir() / 10;
			exitY = 10;
			exitYSpeed = Max(2, container->GetYDir() * 2 / 10);
		}
		Exit(0, exitX, exitY, 0, exitXSpeed, exitYSpeed, 0);
		YoyoThrown(container);
		return true;
	}
	return false;
}

protected func Departure(object from) {
	// If the yo-yo was thrown by a Clonk, act as weapon.
	if((from->GetOCF() & OCF_CrewMember) != 0 && from->GetAction() == "Throw") {
		// Set speed to fly lower, because it's supposed to hit enemies.
		SetXDir(GetXDir() * 3);
		SetYDir(-5);
		YoyoThrown(from);
	}
}

protected func Hit(int xSpeed, int ySpeed) {
	HitEffect();
	if(currentState == YOYO_StateThrown) {
		YoyoReturn(true);
	}
}

/*	Function: QueryStrikeBlow
	Event call, called before the yo-yo hits another object.

	When it was thrown and therefore used as a weapon,
	the yo-yo will recoil from living beings instead of hitting them normally.

	Parameters:
	target - The object the yo-yo is about to hit.

	Returns:
	_true_ when recoiling from living beings, _false_ otherwise. */
protected func QueryStrikeBlow(object target) {
	// Recoil from living beings when used as weapon.
	if((target->GetOCF() & OCF_Living) != 0) {
		if(currentState == YOYO_StateThrown || currentState == YOYO_StateReturning) {
			HitEffect();

			target->DoEnergy(-YOYO_Damage);
			var awayFromThrower = -1;
			if(GetX() > thrower->GetX()) awayFromThrower = 1;

			if(target->GetProcedure() != "FLIGHT") {
				target->SetAction("KneelUp");
				// Move target away from thrower, but don't make it stuck in solid materials.
				for(var i = 0; i < YOYO_MaxHitMoveDistance; i++) {
					target->SetPosition(target->GetX() + awayFromThrower, target->GetY());
					if(target->Stuck()) {
						target->SetPosition(target->GetX() - awayFromThrower, target->GetY());
						break;
					}
				}
			}
			if(!Random(YOYO_FlingTargetChance)) {
				Fling(target, awayFromThrower * YOYO_FlingSpeed, -YOYO_FlingSpeed);
			}

			if(currentState == YOYO_StateThrown)
				YoyoReturn(true);
			return true;
		}
	}
}

private func HitEffect() {
	Sound("Yo-yo hit");
	CastParticles("PxSpark", RandomX(3, 5), 16, 0, 0, 15, 30, RGB(83, 41, 25), RGB(193, 95, 60));
}

protected func RejectEntrance(object into) {
	return (currentState != YOYO_StateInactive && into != thrower);
}

protected func Entrance(object into) {
	YoyoInactive();
}


/*	Section: Yo-yo functionality */

public func GetThrower() {
	return thrower;
}

/* Sets the yo-yo into inactive state. Clears scheduled call of YoyoReturn and removes returning effects, so it _stays_ inactive. */
protected func YoyoInactive() {
	currentState = YOYO_StateInactive;
	ClearScheduleCall(this, "YoyoReturn");
	RemoveEffect("YoyoReturning", this);
	if(line != 0) {
		line->RemoveObject();
	}
	SetVertex(0, 2, CNAT_Center);
	Sound("Yo-yo spin", false, this, 100, 0, -1);
}

/* Sets the yo-yo to thrown state. The yo-yo schedules a call of YoyoReturn so it only flies for a short time. */
protected func YoyoThrown(object by) {
	currentState = YOYO_StateThrown;
	thrower = by;
	line = CreateObject(YOLN, 0, 0, GetOwner());
	ObjectSetAction(line, "Connect", this, by);
	ScheduleCall(0, "YoyoReturn", YOYO_ThrowFlightTime);
	Sound("Yo-yo spin", false, this, 100, 0, 1);

}

/* Makes the yo-yo return to its original thrower.
	it distinguishes between returning after colliding with solid material or hitting a target,
	and returning because it reached the end of the string.

	Parameters:
	byHit - _true_ if the yo-yo returns because it collided with solid material */
protected func YoyoReturn(bool byHit) {
	currentState = YOYO_StateReturning;
	ClearScheduleCall(this, "YoyoReturn");
	AddEffect("YoyoReturning", this, 150, 1, this, 0, byHit);
	SetVertex(0, 2, CNAT_NoCollision);
}


/* Yo-yo returning effect */

protected func FxYoyoReturningStart(object target, int effectNumber, int temporary, bool byHit) {
	if(temporary == 0 && !byHit) {
		Sound("Yo-yo whoosh", false, target, 25);
	}
}

protected func FxYoyoReturningTimer(object target, int effectNumber, int effectTime) {
	// Problem handling
	if(target->GetThrower() == 0) return FX_Execute_Kill;
	if(!target->GetThrower()->GetAlive()) return FX_Execute_Kill;

	// Approach until close enough for the Clonk to catch the yo-yo
	if(target->ObjectDistance(target->GetThrower()) > YOYO_MaxCollectionDistance) {
		var xDistance = target->GetThrower()->GetX() - target->GetX();
		var yDistance = target->GetThrower()->GetY() - target->GetY();
		// Speed calculation: Approach fast when far away but slow when very close.
		// Otherwise, if too fast the yo-yo hits the thrower and it oscillates strangely around the Clonk.
		if(Abs(xDistance) > YOYO_ReturnSlowDownDistance) {
			var sign = xDistance / Abs(xDistance);
			target->SetXDir(sign * YOYO_ReturnMaxSpeed);
		} else {
			target->SetXDir(xDistance * 4);
		}
		target->SetYDir(BoundBy(target->GetThrower()->GetY() - target->GetY(), -50, 50));
		return FX_OK;
	} else {
		target->Enter(target->GetThrower());
		return FX_Execute_Kill;
	}
}

protected func FxYoyoReturningStop(object target, int effectNumber, int reason, bool temporary) {
	if(!temporary) {
		YoyoInactive();
	}
}