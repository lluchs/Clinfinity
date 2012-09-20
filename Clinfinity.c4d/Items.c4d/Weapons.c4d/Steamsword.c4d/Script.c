#strict 2

static const SWOR_DamageRectWidth = 25;
static const SWOR_DamageRectHeight = 38;
static const SWOR_MinDamage = 10;
static const SWOR_MaxDamage = 15;
static const SWOR_FlingSpeed = 3;

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
	var searchX = 0;
	if(Contained()->GetDir() == DIR_Left) searchX = -SWOR_DamageRectWidth;
	for(var clonk in FindObjects(Find_Exclude(Contained()), Find_InRect(searchX, -SWOR_DamageRectHeight / 2, SWOR_DamageRectWidth, SWOR_DamageRectHeight), Find_NoContainer(), Find_OCF(OCF_Living))) {
		if(PathFree(Contained()->GetX(), Contained()->GetY(), clonk->GetX(), clonk->GetY())) {
			clonk->DoEnergy(RandomX(-SWOR_MinDamage, -SWOR_MaxDamage));
		}
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
		var dx = object->GetX() - Contained()->GetX();
		var dy = object->GetY() - Contained()->GetY();
		Rotate(-swordAngle, dx, dy, handX, handY);
		if(Inside(dx, -2, 2)) {
			dx = 0;
			dy = -SWOR_FlingSpeed;
			Rotate(swordAngle, dx, dy);
			Fling(object, dx, dy);
		}
	}
}
