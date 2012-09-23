#strict 2

protected func Hit() {
	Sound("MetalHit*");
}

public func IsMeleeWeapon()			{ return true; }
public func GetWieldDirection()		{ return AVMW_WieldUp; }
public func GetCoolDownDirection()	{ return AVMW_WieldHold; }

public func HandX() { return 0; }
public func HandY() { return -3; }


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
	SetAction("Shield");
	SetDir(direction);
}

public func WieldAbort() {
	Log("WieldAbort"); // TODO: Oh no, this is called even though the action isn't 'really' aborted!
	// This is because the wielding actions continue (NextAction is themselves) and SetAction() is
	// called to start the cool down, I believe.
	// The wielding actions continue because we need GetActTime() to still count the wielding action on finishing wielding.
	// Perhaps we can hack this/calculate the action time ourselves, so this won'n be necessary.
	SetAction("Idle");
}

public func WieldEnd() {
	var handX, handY, weaponAngle;
	Contained()->~GetCurrentWieldData(handX, handY, weaponAngle);
	for(var i = 0; i < 20; ++i) {
		var xSpeed = RandomX(12, 15);
		if(GetDir() == DIR_Left) xSpeed = -xSpeed;
		var y = handY - 6 + Random(13);
		CreateParticle("ThrustSpark", handX, y, xSpeed, y * 2 / 3, 50, RGBa(255,255,255, 200));
	}
	// CreateParticle("ThrustSpark", 10, 0, 15, 0, 50, RGBa(255,255,255, 150))
	//CreateObject(SHID, handX -4 + GetDir() * 8, handY + 6, Contained()->GetOwner());
}

public func CoolDownEnd() {
	SetAction("Idle");
}

public func CoolDownAbort() {
	SetAction("Idle");	
}
