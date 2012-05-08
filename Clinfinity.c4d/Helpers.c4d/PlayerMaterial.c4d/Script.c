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

private func OnFillChange(Key, int iChange) {
	DoScore(GetOwner(), iChange * GetValue(0, Key));
	// highlight change
	HashGet(hIcons, Key)->Flash(iChange);
	return 1;
}

local fNoStatusMessage;
public func Timer() {
	if(fNoStatusMessage)
		return;
	var iter = HashIter(hIcons), node;
	while(node = HashIterNext(iter))
		node[1] -> SetStatusMessage(Format("@%d", GetFill(node[0])));
}

public func MaterialCheck(id idType) {
	fNoStatusMessage = 1;
	var iter = HashIter(hIcons), node;
	while(node = HashIterNext(iter))
		node[1] -> BuildMessage(GetComponent(node[0], 0, 0, idType), GetFill(node[0]));
}
