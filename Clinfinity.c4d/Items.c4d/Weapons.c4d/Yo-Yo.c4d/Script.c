/*	Script: Yo-yo
	A short-range throwing weapon. */

#strict 2

/*	Constants: Internal yo-yo states. */
static const YOYO_StateInactive = 0;
static const YOYO_StateThrown = 1;
static const YOYO_StateReturning = 2;

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
	if((container->GetOCF() & OCF_CrewMember) != 0 && container->GetAction() == "Jump") {
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
	}
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
		YoyoReturn();
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
		if(currentState == YOYO_StateThrown) {
			HitEffect();
			// TODO: Inflict damage on target and perhaps fling it a bit
			YoyoReturn();
			return true;
		} else if(currentState == YOYO_StateReturning) {
			HitEffect();
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

protected func YoyoThrown(object by) {
	currentState = YOYO_StateThrown;
	thrower = by;
	line = CreateObject(YOLN, 0, 0, GetOwner());
	ObjectSetAction(line, "Connect", this, by);
	ScheduleCall(0, "YoyoReturn", 12);
	Sound("Yo-yo spin", false, this, 100, 0, 1);

}

protected func YoyoReturn() {
	currentState = YOYO_StateReturning;
	// TODO: Perhaps play a short "whoosh" to tell the player acoustically that the yo-yo returns now.
	ClearScheduleCall(this, "YoyoReturn");
	AddEffect("YoyoReturning", this, 150, 1, this);
	SetVertex(0, 2, CNAT_NoCollision);
}


/* Yo-yo returning effect */

protected func FxYoyoReturningStart(object target, int effectNumber, int temporary) {
	if(temporary == 0) {
		// TODO: Yoyo pull back sound
		//Sound("SailDown", false, target, 50);
	}
}

protected func FxYoyoReturningTimer(object target, int effectNumber, int effectTime) {
	// Problem handling
	if(target->GetThrower() == 0) return FX_Execute_Kill;
	if(!target->GetThrower()->GetAlive()) return FX_Execute_Kill;

	// Approach until close enough for the Clonk to catch the yo-yo
	if(target->ObjectDistance(target->GetThrower()) > 10) {
		var xDistance = target->GetThrower()->GetX() - target->GetX();
		var yDistance = target->GetThrower()->GetY() - target->GetY();
		if(Abs(xDistance) > 5) {
			var sign = xDistance / Abs(xDistance);
			target->SetXDir(sign * 50);
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
		// TODO: Do something on stopping the effect?
		YoyoInactive();
	}
}