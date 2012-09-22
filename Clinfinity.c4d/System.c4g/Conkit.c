/* Conkit ability for aviator */

#strict 2

#appendto AVTR

public func ContextConkit(object caller) {
	[$CtxConkit$|Image=CCNT]
	SetComDir(COMD_Stop);
	if(!GetPhysical("CanConstruct", PHYS_Current)) {
		PlayerMessage(GetController(), "$TxtCantConstruct$", this, GetName());
		return;
	}

	CreateMenu(CXCN, this, this, C4MN_Extra_Components, "$TxtNoconstructionplansa$");
	var type; var i = 0;
	while(type = GetPlrKnowledge(GetOwner(), 0, i++, C4D_Structure)) {
		if(type->~IsConkitBuilding() || (!type->~IsIndianHandcraft() && !type->~IsTrapperHandcraft())) {
			AddMaterialMenuItem("$TxtConstructions$", "CreateConstructionSite", type, this);
		}
	}

}

protected func CreateConstructionSite(id type) {
	if(GetAction() != "Walk") return;
	if(Contained()) return;
	if(type->~RejectConstruction(0, 10, this)) return;

	var fNeedMaterial;
	if(fNeedMaterial = FindObject(CNMT)) {
		var hNeeded = CreateHash(), iNeeded, ID;
		for(ID in GetMatSysIDs()) {
			if(iNeeded = GetComponent(ID, 0, 0, type)) {
				if(MatSysGetTeamFill(GetOwner(), ID) < iNeeded) {
					PlayerMessage(GetOwner(), "<c ff0000>$TxtNotEnoughMaterial$</c>", this);
					return;
				}
				else
					HashPut(hNeeded, ID, iNeeded);
			}
		}
	}

	var site;
	if(!(site = CreateConstruction(type, 0, 10, GetOwner(), 100, true, true))) return;

	if(fNeedMaterial) {
		var iter = HashIter(hNeeded), node;
		while(node = HashIterNext(iter)) {
			MatSysDoTeamFill(-node[1], GetOwner(), node[0]);
			site->SetComponent(node[0], node[1]);
		}
	}

	Message("$TxtConstructions$", this, GetName(site));
}
