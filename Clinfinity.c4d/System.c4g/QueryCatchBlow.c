/* Clonk appendto for QueryCatchBlow */

#strict 2

#appendto CLNK

protected func QueryCatchBlow(object obj) {
	/* Counterpart to QueryCatchBlow */
	if(obj->~QueryStrikeBlow(this))
		return true;
	return _inherited(obj, ...);
}
