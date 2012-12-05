#strict 2

#include MWEP

static const SWOR_DamageRectWidth = 25;
static const SWOR_DamageRectHeight = 38;
static const SWOR_MinDamage = 10;
static const SWOR_MaxDamage = 15;
static const SWOR_FlingSpeed = 3;
static const SWOR_KnockbackChance = 3; // Chance is calculated as 1/SWOR_KnockbackChance
static const SWOR_MaxKnockbackSpeed = 2;

public func IsMeleeWeapon()		{ return true; }
public func GetWieldDirection()	{ return AVMW_WieldDown; }

public func HandX() { return 0; }
public func HandY() { return 1; }

public func GetStartAngle(int direction) {
	if(direction == DIR_Left) return -45;
	else return 45;
}
public func GetCentralAngle(int direction) {
	if(direction == DIR_Left) return -90;
	else return 90;
}

protected func Hit() {
	Sound("MetalHit*");
}

public func WieldStart(int direction) {
	SetAction("On");
}

public func WieldAbort() {
	SetAction("Off");
}

public func WieldEnd() {
	MeleeHit(SWOR_DamageRectWidth, SWOR_DamageRectHeight, RandomX(SWOR_MinDamage, SWOR_MaxDamage));
	if(!Random(SWOR_KnockbackChance)) {
		ThrowBack(SWOR_DamageRectWidth, SWOR_DamageRectHeight, RandomX(1, Max(1, SWOR_MaxKnockbackSpeed)), false);
	}
	DeflectObjects();
	SetAction("Off");
}

private func Wielding() {
	DeflectObjects();
}

private func DeflectObjects() {
	var handX, handY, swordAngle;
	Contained()->~GetCurrentWieldData(handX, handY, swordAngle);

	var bladeCentreX = 0;
	var bladeCentreY = -8;
	Rotate(swordAngle, bladeCentreX, bladeCentreY);

	var objects = FindObjects(Find_NoContainer(), Find_Distance(8, bladeCentreX + handX, bladeCentreY + handY), Find_Category(C4D_Object), Find_OCF(OCF_InFree | OCF_HitSpeed1));
	for(var object in objects) {
		if(GetEffect("SwordDeflection", object) != 0) {
			return;
		}
		var dx = object->GetX() - Contained()->GetX();
		var dy = object->GetY() - Contained()->GetY();
		Rotate(-swordAngle, dx, dy, handX, handY);
		if(Inside(dx, -5, 5)) {
			if(object->GetXDir() != 0) {
				object->SetSpeed(-object->GetXDir(), -object->GetYDir());
			} else {
				var xSpeed = 0;
				var ySpeed = -SWOR_FlingSpeed;
				Rotate(swordAngle, xSpeed, ySpeed);
				Fling(object, xSpeed, ySpeed);
			}
			AddEffect("SwordDeflection", object, 1, 5);
			object->DoDamage(RandomX(SWOR_MinDamage, SWOR_MaxDamage));
		}
	}
}
