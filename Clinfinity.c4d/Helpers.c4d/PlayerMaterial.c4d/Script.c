/*-- Spielermaterialien --*/

#strict 2

#include L_SS

private func MaxFill() { return 9999; }

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

private func OnFillChange(Key, &iChange) {
	DoScore(GetOwner(), iChange * GetValue(0, Key));
	iChange = UpdateFillEffects(Key, iChange);
	return 1;
}

// checks for global effects defining the fill level
// these effects must define a function FxMatSys<ID>Update
private func UpdateFill(id ID) {
	var effectName = Format("MatSys%i", ID), i = GetEffectCount(effectName);
	if(i > 0) {
		var fill = 0;
		while(i--)
			fill += EffectCall(0, GetEffect(effectName, 0, i), "Update", GetOwner());
		DoFill(fill - GetFill(ID), ID);
		return true;
	}
	return false;
}

// changes the actual fill if defined by effects
private func UpdateFillEffects(id ID, int change) {
	var effectName = Format("MatSys%i", ID), i = GetEffectCount(effectName);
	if(i > 0) {
		var original = change;
		while(i-- && change != 0)
			change -= EffectCall(0, GetEffect(effectName, 0, i), "Change", GetOwner(), change);
		return original - change;
	}
	return change;
}

local fNoStatusMessage;
public func Timer() {
	if(fNoStatusMessage)
		return;
	var iter = HashIter(hIcons), node;
	while(node = HashIterNext(iter)) {
		UpdateFill(node[0]);
		node[1] -> SetStatusMessage(Format("@%d", GetFill(node[0])));
	}
}

public func MaterialCheck(id idType) {
	fNoStatusMessage = 1;
	var iter = HashIter(hIcons), node;
	while(node = HashIterNext(iter))
		node[1] -> BuildMessage(GetComponent(node[0], 0, 0, idType), GetFill(node[0]));
}
