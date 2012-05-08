/* Manages collection of MatSys materials. */

#strict 2

#appendto CLNK

protected func Collection(object obj) {
	var ID = obj->GetID();
	if(InArray(ID, GetMatSysIDs())) {
		var matSys = GetMatSys(GetOwner(), true);
		if(matSys) {
			matSys->DoFill(1, ID);
			obj->RemoveObject();
			return;
		}
	}
	return _inherited(obj, ...);
}

