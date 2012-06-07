#strict 2

#appendto CLNK

private func Chopping() {
	if(_inherited()) {
		var tree = GetActionTarget();
		tree->Shrink();

		var matSys = GetMatSys(GetOwner(), true);
		if(matSys != 0 && InArray(WOOD, GetMatSysIDs())) {
			matSys->DoFill(1, WOOD);
		} else {
			CreateContents(WOOD);
		}
	}
	return true;
}
