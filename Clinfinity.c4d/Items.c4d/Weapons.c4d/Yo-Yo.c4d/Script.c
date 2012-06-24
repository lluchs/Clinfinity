#strict 2

/*	TODO:
	This yo-yo has a state automaton.
	What are the events that cause state transitions?
	*/
static const YOYO_StateInactive = 0;
static const YOYO_StateThrown = 1;
static const YOYO_StateReturning = 2;

local thrower;
local line;
local currentState;

/* Engine events */

protected func Initialize() {
	YoyoInactive();
}

protected func Departure(object from) {
	// If the yo-yo was thrown by a Clonk, act as weapon.
	if((from->GetOCF() & OCF_CrewMember) != 0 && from->GetAction() == "Throw") {
		// Set speed to fly lower, because it's supposed to hit enemies.
		SetXDir(GetXDir() * 3);
		SetYDir(-5);
		YoyoThrown(from);
	}
	// TODO: What if used while gliding? Should probably work similarly in that case, just with a different throwing angle.
}

protected func Hit(int xSpeed, int ySpeed) {
	// TODO: Yo-yo hit sound (different sounds for hitting material and hitting Clonks?)
	if(currentState == YOYO_StateThrown) {
		YoyoReturn();
	}
}

protected func RejectEntrance(object into) {
	return (currentState != YOYO_StateInactive && into != thrower);
}

protected func Entrance(object into) {
	YoyoInactive();
}


/* Yo-yo functionality */

/*	TODO:
	In these cases, returning must be aborted and the yo-yo must be collectible to anyone again:
	* Thrower dies
	* Thrower is removed (either by script (RemoveObject()) or by falling out of the landscape bottom)
	*/

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
}

protected func YoyoThrown(object by) {
	currentState = YOYO_StateThrown;
	thrower = by;
	line = CreateObject(YOLN, 0, 0, GetOwner());
	ObjectSetAction(line, "Connect", this, by);
	ScheduleCall(0, "YoyoReturn", 12);
	// TODO: Yo-yo whirring sound? Different sounds for flying and returning?

}

protected func YoyoReturn() {
	currentState = YOYO_StateReturning;
	// TODO: Perhaps play a short "whoosh" to tell the player acoustically that the yo-yo returns now.
	AddEffect("YoyoReturning", this, 150, 1, this);
	// TODO: If working as a weapon: Remove vertex (? or do something else to deactivate collision with material), return to sender
	ClearScheduleCall(this, "YoyoReturn");
	// TODO: While returning, perhaps change the category to vehicle so we don't hit the thrower. OTOH we still want to hit enemy Clonks, right?

}


/* Yo-yo returning effect */

protected func FxYoyoReturningStart(object target, int effectNumber, int temporary) {
	if(temporary == 0) {
		// TODO: Yoyo pull back sound
		//Sound("SailDown", false, target, 50);
	}
}

protected func FxYoyoReturningTimer(object target, int effectNumber, int effectTime) {
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