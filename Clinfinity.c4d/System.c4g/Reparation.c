/* Manages building reparation. */
#strict 2

#appendto CLNK

protected func ContextRepair(object clonk) {
	[$CtxRepair$|Image=CXCN|Condition=AtBuildingToRepair]
	var struct = AtBuildingToRepair();
	if(!struct) {
		Message("$NothingtoRepair$", this);
		return false;
	}
	
	// Materialien einsammeln
	var needed = struct->GetMissingComponents();
	for(var i = 0; i < GetLength(needed[0]); i++) {
		var ID = needed[0][i], num = needed[1][i];
		struct->DoRepairComponent(ID, -struct->MatSysDoTeamFill(-num, GetOwner(), ID));
	}
	
	// Repair as much as possible!
	while(struct->Repair(1) && struct->GetDamage());
	if(struct->GetDamage()) {
		// Print missing material
		var needed = struct->GetMissingComponents();
		var output = "$NeededMaterials$";
		for(var i = 0; i < GetLength(needed[0]); i++) {
			var ID = needed[0][i], num = needed[1][i];
			output = Format("%s|%dx %s", output, num, GetName(0, ID));
		}
		Message(output, this);
	}
}

protected func AtBuildingToRepair() {
	return GetPhysical("CanConstruct") && FindObject2(Find_AtObject(), Find_Category(C4D_Structure), Find_Allied(GetOwner()), Find_Func("GetDamage"), Find_Func("MaxDamage"));
}

