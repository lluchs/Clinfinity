/* Conkit - Appendto for MatSys compatibility */

#strict 2

#appendto CNKT

local aMenuItems;

public func Activate(pClonk) { 
	[$TxtCreateconstructionsi$]
	// Clonk anhalten
	SetComDir(COMD_Stop, pClonk);
	// Kann der Clonk überhaupt bauen?
	if (pClonk && !GetPhysical("CanConstruct", PHYS_Current, pClonk)) {
		PlayerMessage(GetController(pClonk), "$TxtCantConstruct$", this, GetName(pClonk));
		return false;
	}
	// Menü erzeugen und mit Bauplänen des Spielers füllen
	CreateMenu(CXCN, pClonk, this, 1, "$TxtNoconstructionplansa$");
	var idType, i = 0;
	aMenuItems = CreateArray();
	while (idType = GetPlrKnowledge(GetOwner(pClonk), 0, i++, C4D_Structure))
		// Keine Trapper/Indianer Sachen mit Bausatz bauen. (Außer für Wachtürme/Palisaden, die es ausdrücklich zulassen)
		if(idType->~IsConkitBuilding() || (!idType->~IsIndianHandcraft() && !idType->~IsTrapperHandcraft())) {
			AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk);
			aMenuItems[i - 1] = idType;
		}
	CheckMenu();
	return 1;
}

protected func CheckMenu() {
	var pMatSys = GetMatSys(GetOwner(Contained()));
	if(GetMenu(Contained()) != CXCN || !pMatSys)
		return;
	var idType = aMenuItems[GetMenuSelection(Contained())];
	if(!idType)
		return;
	pMatSys -> MaterialCheck(idType);
	ScheduleCall(this, "CheckMenu", 5);
}

protected func MenuQueryCancel() {
	GetMatSys(GetOwner(Contained())) -> LocalN("fNoStatusMessage") = 0;
	ClearScheduleCall(this, "CheckMenu");
}

protected func CreateConstructionSite(id idType) {
	MenuQueryCancel();
	// Nur wenn der Clonk steht und sich im Freien befindet
	if (GetAction(Contained()) != "Walk") return;
	if (Contained(Contained())) return;
	// Pruefen, ob das Gebaeude hier gebaut werden kann
	if (idType->~RejectConstruction(0, 10, Contained()) ) return;
	var fNeedMaterial;
	if(fNeedMaterial = FindObject(CNMT)) {
		var hNeeded = CreateHash(), iNeeded, ID;
		for(ID in GetMatSysIDs()) {
			if(iNeeded = GetComponent(ID, 0, 0, idType)) {
				if(MatSysGetTeamFill(GetOwner(Contained()), ID) < iNeeded) {
					PlayerMessage(GetOwner(Contained()), "<c ff0000>$TxtNotEnoughMaterial$</c>", this);
					return;
				}
				else
					HashPut(hNeeded, ID, iNeeded);
			}
		}
	}
	// Besitzer setzen für CreateConstruction
	SetOwner(GetOwner(Contained()));
	// Baustelle erschaffen
	var pSite;
	if (!(pSite = CreateConstruction(idType, 0, 10, GetOwner(Contained()), 100, true, true))) return;

	if(fNeedMaterial) {
		var iter = HashIter(hNeeded), node;
		while(node = HashIterNext(iter)) {
			MatSysDoTeamFill(-node[1], GetOwner(), node[0]);
			pSite -> SetComponent(node[0], node[1]);
		}
	}
	// Meldung
	Message("$TxtConstructions$", Contained(), GetName(pSite));
	// Bausatz verbrauchen
	RemoveObject();
	return 1;
}
