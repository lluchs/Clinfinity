#strict 2

local thrower;

protected func Departure(object from) {
	// If the yo-yo was thrown by a Clonk, act as weapon.
	if((from->GetOCF() & OCF_CrewMember) != 0) {
		thrower = from;
		// Set speed to fly lower, because it's supposed to hit enemies.
		SetXDir(GetXDir() * 3);
		SetYDir(-5);
		// Attach yo-yo line to Clonk.
		// TODO
	}
}

protected func Hit(int xSpeed, int ySpeed) {
	// TODO
	// If working as a weapon: Remove vertex, return to sender
	// TODO: While returning, perhaps change the category to vehicle so we don't hit the thrower
}

// TODO: Reject entrance to anyone but the throwing clonk (while acting as weapon).
protected func RejectEntrance(object into) {
	if(into != thrower) return true;
}

// TODO: Get slower on x axis over time, and pick up speed again when returning to thrower?

// TODO: Remove effect on collection

public func GetThrower() {
	return thrower;
}

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
		Sound("SailUp", false, target, 50);
		SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, 0);
	}
}