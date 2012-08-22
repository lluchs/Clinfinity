#strict 2

#appendto TREE

protected func Construction() {
	AddEffect("NoChopDown", this, 50, 0, this);
	return _inherited();
}

protected func FxNoChopDownDamage(object target, int effectNumber, int damage, int cause) {
	if(cause != FX_Call_DmgChop) {
		Shrink();
	}
	return 0;
}

public func Shrink() {
	var rest = Max(GetComponent(WOOD), 1);
	var shrinkage = GetCon() / rest;
	var oldHeight = GetObjHeight();


	if(rest == 1) {
		RemoveObject();
	} else {
		DoCon(-shrinkage);
		// Unrotated trees shrink towards their offset instead their bottom. Thus, adjust their position so they stay on the ground.
		if(GetR() != 0) {
			var verticalAdjustment = (oldHeight - GetObjHeight()) / 3;
			SetPosition(GetX(), GetY() + verticalAdjustment);
		}
	}
}

protected func Destruction() {
	// Replace the tree with a tiny one.
	PlaceVegetation(GetID(), 0, 0, LandscapeWidth(), LandscapeHeight(), 1);
	_inherited(...);
}
