/*-- Spielermaterialien --*/

#strict 2

#include LHSS

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
		var iWidth = 566, iHeight = 566, iXAdjust = 28000, iYAdjust = 13000;
		if(idObj != LHBK && idObj != LHTL) {
			iXAdjust *= 2;
			iXAdjust += 3000;
			iYAdjust *= 2;
			iWidth += 150;
			iHeight += 34;
		}
		pIcon -> SetObjDrawTransform(iWidth, 0, iXAdjust, 0, iHeight, iYAdjust, 0, GFX_Overlay);
		HashPut(hIcons, idObj, pIcon);
		iX -= 40;
	}
}

private func OnFillChange(Key, int iChange) {
	DoScore(GetOwner(), iChange * GetValue(0, Key));
	return 1;
}

local fNoStatusMessage;
public func Timer() {
	if(fNoStatusMessage)
		return;
	var iter = HashIter(hIcons), node;
	while(node = HashIterNext(iter))
		node[1] -> SetStatusMessage(Format("@%d", GetAmount(node[0])));
}

public func MaterialCheck(id idType) {
	fNoStatusMessage = 1;
	var iter = HashIter(hIcons), node;
	while(node = HashIterNext(iter))
		node[1] -> BuildMessage(GetComponent(node[0], 0, 0, idType), GetAmount(node[0]));
}
