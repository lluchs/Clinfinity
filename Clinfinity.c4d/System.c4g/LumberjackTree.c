#strict 2

#appendto TREE

protected func Construction() {
	AddEffect("NoChopDown", this, 50, 0, this);
	return _inherited();
}

protected func FxNoChopDownDamage(object target, int effectNumber, int damage, int cause) {
	if(cause == FX_Call_DmgChop) {
		return 0;
	}
}