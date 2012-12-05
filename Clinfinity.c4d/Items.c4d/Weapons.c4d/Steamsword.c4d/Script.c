/*	Script: Steam sword
	A short-range melee weapon.

	Deflection and hits:
	- Objects are deflected every frame, while the sword is wielded
	- If a deflected object has no horizontal speed, it is flung in the direction of the sword
	- Otherwise, the object is "returned to sender" by inverting its movement
	- Living beings are hit at the end of the wielding animation

	Critical hits:
	- When dealing a critical hit, the damage value is increased by multiples of the damage increase factor
	- Having a vertical speed of at least SWOR_MinCriticalHitSpeed is awarded with 1x the damage increase factor
	- For every further 10 speed units above this limit, another 1x damage increase factor is awarded */

#strict 2

#include MWEP

/*	Constants: Damage
	SWOR_DamageRectWidth	- Width of the sword's hit zone.
	SWOR_DamageRectHeight	- Height of the sword's hit zone.
	SWOR_MinDamage			- Minimum standard damage dealt for each hit.
	SWOR_MaxDamage			- Maximum standard damage dealt for each hit. */
static const SWOR_DamageRectWidth = 25;
static const SWOR_DamageRectHeight = 38;
static const SWOR_MinDamage = 10;
static const SWOR_MaxDamage = 15;

/*	Constants: Deflection and knockback
	SWOR_FlingSpeed					- Speed to set for deflected objects that have no horizontal speed.
	SWOR_KnockbackChance			- Chance to knock back a hit living being.
	SWOR_MaxKnockbackSpeed			- Maximum speed to knock back a hit living being.
	SWOR_MinCriticalHitSpeed		- Vertical speed necessary to deal a critical hit.
	SWOR_CriticalHitDamageIncrease	- Damage increase factor for critical hits. */
static const SWOR_FlingSpeed = 3;
static const SWOR_KnockbackChance = 3; // Chance is calculated as 1/SWOR_KnockbackChance
static const SWOR_MaxKnockbackSpeed = 2;
static const SWOR_MinCriticalHitSpeed = 30;
static const SWOR_CriticalHitDamageIncrease = 5;

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
	var dealtDamage = RandomX(SWOR_MinDamage, SWOR_MaxDamage);
	if(GetYDir() >= SWOR_MinCriticalHitSpeed) {
		var speedFactor = GetYDir() - SWOR_MinCriticalHitSpeed;
		speedFactor = speedFactor / 10 + 1;
		dealtDamage += speedFactor * SWOR_CriticalHitDamageIncrease;
	}
	MeleeHit(SWOR_DamageRectWidth, SWOR_DamageRectHeight, dealtDamage);
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
