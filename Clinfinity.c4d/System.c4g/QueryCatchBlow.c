/*	Script: QueryCatchBlow.c
	Clonk appendto for QueryCatchBlow.
	Defines a new event: QueryStrikeBlow,
	which is the counterpart for QueryCatchBlow called in the striking object. */

#strict 2

#appendto CLNK

protected func QueryCatchBlow(object obj) {
	if(obj->~QueryStrikeBlow(this))
		return true;
	return _inherited(obj, ...);
}

/*	Function: QueryStrikeBlow
	Event call, called before the object hits another object.
	By returning _true_, QueryStrikeBlow can reject physical blows.

	Parameters:
	target - The object about to be hit.

	Returns:
	_true_ to reject physical blows, _false_ otherwise. */
public func QueryStrikeBlow(object target) {
	// Do nothing, this function is just here for documentation purposes.
	return _inherited(...);
}
