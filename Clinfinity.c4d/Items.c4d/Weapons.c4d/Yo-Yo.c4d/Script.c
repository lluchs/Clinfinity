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
static const YOYO_ReturnMaxSpeed = 40;
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

/*	Function: Initialize
	Called when the yo-yo is created.
	The yo-yo is initialised as inactive. */
protected func Initialize() {
	YoyoInactive();
}

/*	Function: ContainerThrow
	If a Clonk throws the yo-yo in flight (and does not want to drop it),
	activate the yo-yo and make it fly downwards.

	Returns:
	_true_ if the yo-yo was activated as weapon, _false_ otherwise. */
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

/*	Function: Departure
	If a Clonk throws the yo-yo while standing, activate the yo-yo.

	Parameters:
	from	- The object the yo-yo departed from. */
protected func Departure(object from) {
	// If the yo-yo was thrown by a Clonk, act as weapon.
	if((from->GetOCF() & OCF_CrewMember) != 0 && from->GetAction() == "Throw" && !OtherYoyoThrownBy(from)) {
		// Set speed to fly lower, because it's supposed to hit enemies.
		SetXDir(GetXDir() * 3);
		SetYDir(-5);
		YoyoThrown(from);
	}
}

private func OtherYoyoThrownBy(object clonk) {
	return FindObject2(Find_ID(YOLN), Find_ActionTarget(clonk)) != 0;
}

/*	Function: Hit
	Event call when hitting solid materials.
	If the yo-yo was thrown, it returns to the thrower.

	Parameters:
	xSpeed	- Horizontal speed.
	ySpeed	- Vertical speed. */
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

/*	Function: RejectEntrance
	Reject entrance into other objects than the original thrower when active or returning.

	Parameters:
	into	- The object that the yo-yo is about to enter.

	Returns:
	_true_ if the yo-yo rejects entrance into the object, _false_ if entrance is allowed. */
protected func RejectEntrance(object into) {
	return (currentState != YOYO_StateInactive && into != thrower);
}

/*	Function: Entrance
	Event call when entering an object.
	The yo-yo is set to the inactive state.

	Parameters:
	into	- The object that the yo-yo entered. */
protected func Entrance(object into) {
	YoyoInactive();
}


/*	Section: Yo-yo functionality */

/*	Function: GetThrower
	Returns the object that threw the yo-yo.

	Returns:
	The original thrower. */
public func GetThrower() {
	return thrower;
}

/*	Function: YoyoInactive
	Sets the yo-yo to inactive state.
	Clears the scheduled call of "YoyoReturn" and removes returning effects, so it stays inactive.
	Removes the string between the thrower and the yo-yo and resets the yo-yo's vertex. */
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

/*	Function: YoyoThrown
	Sets the yo-yo to thrown state.
	The yo-yo schedules a call of "YoyoReturn" so it only flies for a short time.
	Additionally, a string attached to the thrower and to the yo-yo is created.

	_Note_: The flight speed of the yo-yo must be set by the calling code.

	Parameters:
	by	- The yo-yo was thrown by this object. */
protected func YoyoThrown(object by) {
	currentState = YOYO_StateThrown;
	thrower = by;
	line = CreateObject(YOLN, 0, 0, GetOwner());
	ObjectSetAction(line, "Connect", by, this);
	ScheduleCall(0, "YoyoReturn", YOYO_ThrowFlightTime);
	Sound("Yo-yo spin", false, this, 100, 0, 1);

}

/*	Function: YoyoReturn
 	Makes the yo-yo return to its original thrower.
	The cause for returning is distinguished between two cases:
	* Hitting a solid material or a target.
	* Reaching the maximum flight timespan (reaching the "end of the string", so to say).

	While returning, the yo-yo's vertex is set to not collide with solid materials,
	so the yo-yo can return to the thrower in any case.

	Parameters:
	byHit - _true_ if the yo-yo returns because of a hit. */
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