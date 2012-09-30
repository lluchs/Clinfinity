#strict 2

#appendto TREE

protected func Construction() {
	AddEffect("NoChopDown", this, 50, 0, this);
	return _inherited();
}

protected func FxNoChopDownDamage(object target, int effectNumber, int damage, int cause) {
	if(cause != FX_Call_DmgChop && cause != FX_Call_EngFire) {
		Shrink();
	}
	return 0;
}

// Catch calls by BlastObjects()
public func CanBeHitByShockwaves() { return true; }
public func BlastObjectsShockwaveCheck() { return true; }

public func OnShockwaveHit(int level, int x, int y) {
	var wood = CreateObject(WOOD, 0, 0, GetOwner());
	var xdir = GetX() - x,
	    ydir = GetY() - y;
	wood->SetXDir(xdir * level, 0, 100);
	wood->SetYDir(ydir * level, 0, 100);
	return true;
}

public func Shrink() {
	var rest = Max(GetComponent(WOOD), 1);
	var shrinkage = GetCon() / rest;
	var oldHeight = GetObjHeight();


	DoCon(-shrinkage);

	// Unrotated trees shrink towards their offset instead their bottom. Thus, adjust their position so they stay on the ground.
	if(GetR() != 0) {
		var verticalAdjustment = (oldHeight - GetObjHeight()) / 3;
		SetPosition(GetX(), GetY() + verticalAdjustment);
	}

	return GetCon();
}

protected func Destruction() {
	// Replace the tree with a tiny one.
	PlaceVegetation(GetID(), 0, 0, LandscapeWidth(), LandscapeHeight(), 1);
	_inherited(...);
}
