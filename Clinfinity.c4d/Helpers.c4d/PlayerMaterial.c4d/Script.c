/*-- Spielermaterialien --*/

#strict 2

#include L_SS

private func MaxFill() { return 1000; }

static aMaterialSystem;

local hIcons; // HashGet(GetMatSys()->LocalN("hIcons"), WOOD)

public func Initialize() {
	if(GetType(aMaterialSystem) != C4V_Array)
		aMaterialSystem = CreateArray(GetPlayerCount());
	aMaterialSystem[GetOwner()] = this;
	var aIDs = GetMatSysIDs();
	var iX = -166, pIcon;
	hIcons = CreateHash();
	for(var idObj in aIDs) {
		pIcon = CreateObject(LPMI, 0, 0, GetOwner());
		pIcon -> SetPosition(iX);
		pIcon -> Set(idObj);
		HashPut(hIcons, idObj, pIcon);
		iX -= 40;
	}
}

public func OnFillChange(Key, int iChange, bool dontNotify) {
	// notify other team member's MatSys
	var owner = GetOwner();
	if(!dontNotify) {
		for(var count = GetPlayerCount(), i = 0; i < count; i++) {
			var p = GetPlayerByIndex(i);
			if(p != owner && !Hostile(p, owner))
				GetMatSys(p)->OnFillChange(Key, iChange, true);
		}
	}

	// add to the score
	DoScore(owner, iChange * GetValue(0, Key));
	// highlight change
	HashGet(hIcons, Key)->Flash(iChange);
	return 1;
}

local fNoStatusMessage;
public func Timer() {
	if(fNoStatusMessage)
		return;
	var owner = GetOwner();
	var iter = HashIter(hIcons), node;
	while(node = HashIterNext(iter)) {
		var fill = MatSysGetTeamFill(owner, node[0]);
		node[1] -> SetStatusMessage(Format("@%d", fill));
	}
}

public func MaterialCheck(id idType) {
	fNoStatusMessage = 1;
	var owner = GetOwner();
	var iter = HashIter(hIcons), node;
	while(node = HashIterNext(iter)) {
		var fill = MatSysGetTeamFill(owner, node[0]);
		node[1] -> BuildMessage(GetComponent(node[0], 0, 0, idType), fill);
	}
}
