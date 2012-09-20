#strict 2

static const SWOR_DamageRectWidth = 25;
static const SWOR_DamageRectHeight = 38;
static const SWOR_Damage = 10;

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
			clonk->DoEnergy(-SWOR_Damage);
		}
	}
	SetAction("Off");
}

private func Wielding() {
	// Check for object defence, depending on current angle and stuff
}