/* Tumble mode hacks */
#strict 2

#appendto AVTR

protected func MakeDoubleJumpPossible() {
	if(tumbleMode)
		// No normal jumping
		SetAction("Tumble");
	return inherited(...);
}

protected func ControlUp() {
	if(tumbleMode)
		// Always possible \o/
		doubleJumpPossible = true;
	return inherited(...);
}

protected func ControlUpDouble() {
	if(tumbleMode)
		// Faster double jump
		return ControlUp(...);
}
