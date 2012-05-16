/* Stop grass from growing in the air. */

#strict 2

#appendto GRAS

protected func Initialize() {
	AddEffect("CheckAir", this, 1, 30, this);
	return inherited(...);
}

protected func FxCheckAirTimer(object target, int effectNum, int effectTime) {
	if(GetMaterial(0,10) != Material("Earth"))
		DoCon(-10);
}
