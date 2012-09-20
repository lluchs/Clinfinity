#strict 2

public func IsMeleeWeapon()		{ return true; }
public func GetWieldDirection()	{ return AVMW_WieldDown; }

public func HandX() { return 0; }
public func HandY() { return 1; }

public func GetStartAngle(int direction) {
	if(direction == DIR_Left) return -45;
	else return 45;
}
public func GetEndAngle(int direction) {
	if(direction == DIR_Left) return -135;
	else return 135;
}

public func WieldStart(int wieldDuration) {
	SetAction("On");
}

public func WieldAbort() {
	SetAction("Off");
}

public func WieldEnd() {
	/*for(var clonk in FindObjects(...)) {
		clonk->DoEnergy(-9000);
	}*/
	SetAction("Off");
}

private func Wielding() {
	// Check for object defence, depending on current angle and stuff
}