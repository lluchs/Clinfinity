#strict 2

/*	TODO:
	This yo-yo has a state automaton.
	What are the events that cause state transitions?
	*/
static const YOYO_StateInactive = 0;
static const YOYO_StateThrown = 1;
static const YOYO_StateReturning = 2;

local thrower;
local currentState;

/* Engine events */

protected func Initialize() {
	YoyoInactive();
}

protected func Departure(object from) {
	// If the yo-yo was thrown by a Clonk, act as weapon.
	if((from->GetOCF() & OCF_CrewMember) != 0) {
		// Set speed to fly lower, because it's supposed to hit enemies.
		SetXDir(GetXDir() * 3);
		SetYDir(-5);
		YoyoThrown();
	}
	// TODO: What if used while gliding? Should probably work similarly in that case, just with a different throwing angle.
}

protected func Hit(int xSpeed, int ySpeed) {
	// TODO: Yo-yo hit sound (different sounds for hitting material and hitting Clonks?)
	if(currentState == YOYO_StateThrown) {
		YoyoReturn();
	}
}

// TODO: Reject entrance to anyone but the throwing clonk (while acting as weapon).
protected func RejectEntrance(object into) {
	return (currentState != YOYO_StateInactive && into != thrower);
}

protected func Entrance(object into) {
	// TODO: Stop returning, remove effect(s)
}


/* Yo-yo functionality */

// TODO: Get slower on x axis over time, and pick up speed again when returning to thrower?

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
}

protected func YoyoThrown(object by) {
	currentState = YOYO_StateThrown;
	thrower = by;
	// TODO: Attach yo-yo line to Clonk.
	// TODO: Start YoyoReturn effect after some frames. (-> ScheduleCall)
	// TODO: Yo-yo whirring sound? Different sounds for flying and returning?

}

protected func YoyoReturn() {
	currentState = YOYO_StateReturning;
	// TODO: Start YoyoReturn effect, perhaps play a short "whoosh" to tell the player acoustically that the yo-yo returns now.
	// TODO: If working as a weapon: Remove vertex (? or do something else to deactivate collision with material), return to sender
	// TODO: Start YoyoReturn effect immediately, stop scheduled call
	// TODO: While returning, perhaps change the category to vehicle so we don't hit the thrower. OTOH we still want to hit enemy Clonks, right?

}


/* Yo-yo returning effect */

protected func FxYoyoReturnStart(object target, int effectNumber, int temporary) {
	if(temporary == 0) {
		// TODO: Yoyo pull back sound
		//Sound("SailDown", false, target, 50);
	}
}

protected func FxYoyoReturnTimer(object target, int effectNumber, int effectTime) {
	if(target->ObjectDistance(target->GetThrower()) > 5) {
		target->SetXDir(BoundBy( target->GetThrower()->GetY() - target->GetX() ));
		return 0;
	} else {
		target->Enter(target->GetThrower());
		return -1;
	}
}

protected func FxYoyoReturnStop(object target, int effectNumber, int reason, bool temporary) {
	if(!temporary) {
		// TODO: Do something on stopping the effect?
	}
}