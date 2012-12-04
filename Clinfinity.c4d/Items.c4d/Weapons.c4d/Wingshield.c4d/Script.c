#strict 2

#include MWEP

static const SHIT_KnockbackRectWidth = 20;
static const SHIT_KnockbackRectHeight = 38;
static const SHIT_MinDamage = 2;
static const SHIT_MaxDamage = 3;
static const SHIT_FlingSpeed = 2;

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

protected func Hit() {
	Sound("MetalHit*");
}

public func WieldStart(int direction) {
	Sound("WingShield");
	SetAction("Shield");
	SetDir(direction);
}

public func WieldAbort() {
	SetAction("Idle");
}

public func WieldEnd() {
	SetAction("Block");
	var handX, handY, weaponAngle;
	Contained()->~GetCurrentWieldData(handX, handY, weaponAngle);
	for(var i = 0; i < 20; ++i) {
		var xSpeed = RandomX(12, 15);
		if(GetDir() == DIR_Left) xSpeed = -xSpeed;
		var y = handY - 7 + Random(15);
		CreateParticle("ThrustSpark", handX, y, xSpeed + Contained()->GetXDir(), y * 2 / 3, 50, RGBa(255,255,255, 200));
	}
	MeleeHit(SHIT_KnockbackRectWidth, SHIT_KnockbackRectHeight, RandomX(SHIT_MinDamage, SHIT_MaxDamage));
	ThrowBack(SHIT_KnockbackRectWidth, SHIT_KnockbackRectHeight, SHIT_FlingSpeed, false);
}

public func CoolDownEnd() {
	SetAction("Idle");
}

public func CoolDownAbort() {
	SetAction("Idle");	
}

public func GetResistance(int amount, object weapon) {
	// While holding up the shield, block sword and bullet hits from the direction the shield is held in
	if(GetAction() == "Block") {
		if(weapon->~GetDamageType() == DamageType_Bullet || weapon->GetID() == SWOR) {
			if((Contained()->GetDir() == DIR_Left && weapon->GetX() < Contained()->GetX()) ||
				(Contained()->GetDir() == DIR_Right && weapon->GetX() > Contained()->GetX())) {
				return 100;
			}
		}
	}
}
