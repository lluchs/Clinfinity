#strict 2

#appendto CLNK

private func Chopping() {
	if(_inherited()) {
		var tree = GetActionTarget();
		var rest = Max(tree->GetComponent(WOOD), 1);
		var shrinkage = tree->GetCon() / rest;
		var oldHeight = tree->GetObjHeight();

		var matSys = GetMatSys(GetOwner(), true);
		if(matSys != 0 && InArray(WOOD, GetMatSysIDs())) {
			matSys->DoFill(1, WOOD);
		} else {
			CreateContents(WOOD);
		}
		if(rest == 1) {
			tree->RemoveObject();
			FinishCommand(this, true);
		} else {
			tree->DoCon(-shrinkage);
			// Unrotated trees shrink towards their offset instead their bottom. Thus, adjust their position so they stay on the ground.
			if(tree->GetR() != 0) {
				var verticalAdjustment = (oldHeight - tree->GetObjHeight()) / 3;
				tree->SetPosition(tree->GetX(), tree->GetY() + verticalAdjustment);
			}
		}
	}
	return true;
}
