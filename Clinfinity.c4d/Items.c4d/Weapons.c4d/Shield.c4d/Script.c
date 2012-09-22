#strict 2

protected func Hit() {
	Sound("MetalHit*");
}

public func IsMeleeWeapon()		{ return true; }
public func GetWieldDirection()	{ return AVMW_WieldUp; }

public func HandX() { return 0; }
public func HandY() { return 0; }


public func GetStartAngle(int direction) {
	if(direction == DIR_Left) return -140;
	else return 140;
}
public func GetCentralAngle(int direction) {
	if(direction == DIR_Left) {
		return 50;
	} else {
		return -50;
	}
}

public func WieldStart(int direction) {
	SetR(0);
	SetAction("Shield");
	SetDir(direction);
}

public func WieldAbort() {
	SetAction("Idle");
}

public func WieldEnd() {
	SetAction("Idle");
}
