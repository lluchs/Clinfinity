/* Clonk appendto for QueryCatchBlow */

#strict 2

#appendto CLNK

protected func QueryCatchBlow(object obj) {
	/* Don't collide with hats. */
	if(obj->~IsHat())
		return true;
	return _inherited(obj, ...);
}
