#strict 2

#include MWEP

static const SHIT_KnockbackRectWidth = 16;
static const SHIT_KnockbackRectHeight = 38;
static const SHIT_MinDamage = 2;
static const SHIT_MaxDamage = 3;
static const SHIT_FlingSpeed = 2;

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
	MeleeHit(SHIT_KnockbackRectWidth, SHIT_KnockbackRectHeight, RandomX(SHIT_MinDamage, SHIT_MaxDamage));
	ThrowBack(SHIT_KnockbackRectWidth, SHIT_KnockbackRectHeight, SHIT_FlingSpeed);
}

public func CoolDownEnd() {
	SetAction("Idle");
}

public func CoolDownAbort() {
	SetAction("Idle");	
}
