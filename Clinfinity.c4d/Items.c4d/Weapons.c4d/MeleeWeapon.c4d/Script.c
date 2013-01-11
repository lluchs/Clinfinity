/*	Script: MeleeWeapon
	Function library for common melee weapon functionality. */

#strict 2

/*	Function: GetDamageType
	Predefined damage type for all melee weapons.

	Returns:
	DamageType_Melee */
public func GetDamageType() { return DamageType_Melee; }

/*	Function: MeleeHit
	Hit living beings in a rectangular area vertically centered in front of the user of the weapon.

	Parameters:
	rectangleWidth	- Width of the area.
	rectangleHeight	- Height of the area.
	damage			- Damage to deal. */
public func MeleeHit(int rectangleWidth, int rectangleHeight, int damage) {
	for(var target in FindTargets(rectangleWidth, rectangleHeight)) {
		if(PathFree(Contained()->GetX(), Contained()->GetY(), target->GetX(), target->GetY())) {
			MeleeHitObject(target, damage);
		}
	}
}

/*	Function: MeleeHitObject
	Hit a single object.

	Parameters:
	target	- Object to hit.
	damage	- Damage to deal to the target. */
public func MeleeHitObject(object target, int damage) {
	target->InflictDamage(damage, this);
	target->CastParticles("PxSpark", RandomX(3, 5), 16, 0, 0, 15, 30, RGB(83, 41, 25), RGB(193, 95, 60));
}


/*	Section: Throw back
	"Throw back" is worse than a simple knockback: Instead of just pushing targets away from the user of the weapon, this sends them *flying* away.
	Usually, targets get thrown further away than knocked back, which poses an additional threat, e.g. to fall off a cliff.
	Throw back can either make the targets jump, so for example Clonks can still use weapons or their wing suit, or even make them tumble. */

/*	Function: ThrowBack
	Throw back living beings in a rectangular area vertically centered in front of the user of the weapon.

	Parameters:
	rectangleWidth	- Width of the area.
	rectangleHeight	- Height of the area.
	throwSpeed		- Flying speed for hit objects
	tumble			- Determines whether hit objects just fly or also tumble while flying. */
public func ThrowBack(int rectangleWidth, int rectangleHeight, int throwSpeed, bool tumble) {
	for(var target in FindTargets(rectangleWidth, rectangleHeight)) {
		if(PathFree(Contained()->GetX(), Contained()->GetY(), target->GetX(), target->GetY())) {
			ThrowBackObject(target, throwSpeed, tumble);
		}
	}
}

/*	Function: ThrowBackObject
	Throw back a single object.

	Parameters:
	target		- Object to throw back.
	throwSpeed	- Flying speed for the target.
	tumble		- Determines whether the target just flies or also tumbles while flying. */
public func ThrowBackObject(object target, int throwSpeed, bool tumble) {
	var away = -1;
	if(target->GetX() > GetX()) away = 1;
	Fling(target, away * throwSpeed, -throwSpeed);
	if(!tumble) {
		target->SetAction("Jump");
	}
}

private func FindTargets(int rectangleWidth, int rectangleHeight) {
	var rectangleX = 0;
	if(Contained()->GetDir() == DIR_Left) rectangleX = -rectangleWidth;
	return FindObjects(Find_Exclude(Contained()), Find_InRect(rectangleX, -rectangleHeight / 2, rectangleWidth, rectangleHeight), Find_NoContainer(), Find_OCF(OCF_Alive));
}
