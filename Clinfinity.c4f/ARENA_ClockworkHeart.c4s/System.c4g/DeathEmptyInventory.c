
#strict 2

#appendto CLNK

protected func Death(int killedBy) {
	var contents = FindObjects(Find_Owner(GetOwner()), Find_Exclude(this));
	for(var item in contents) {
		// The item might actually already be gone.
		if(item)
			item->RemoveObject();
	}
	return _inherited(killedBy);
}
