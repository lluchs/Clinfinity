/*-- Library for centralised production menus --*/

#strict 2

global func CreateProductionMenu(id symbol, object menuObject, object commandObject, int extra, string caption, int extraData, int style, bool permanent, id menuId) {
	if(!menuObject) return;
	if(GetMenu(menuObject) == symbol) return;
	AddEffect("ProductionMenuCheck", menuObject, 300, 1, commandObject, symbol, symbol);
	return CreateMenu(symbol, menuObject, commandObject, extra, caption, extraData, style, permanent, menuId);
}
global func AddProductionMenuItem(string caption, string command, id item, object menuObject, int iCount, int parameter, string infoCaption, int extra, par1, par2) {
	if(command) EffectCall(menuObject, GetEffect("ProductionMenuCheck", menuObject), "AddItem", item);
	AddMenuItem(caption, command, item, menuObject, iCount, parameter, infoCaption, extra, par1, par2);
}

global func FxProductionMenuCheckStart(object target, int effectNum, int temp, id menuId) {
	EffectVar(0, target, effectNum) = menuId;
	EffectVar(1, target, effectNum) = [];
}

global func FxProductionMenuCheckAddItem(object target, int effectNum, id menuId) {
	var menuItems = EffectVar(1, target, effectNum);
	menuItems[GetLength(menuItems)] = menuId;
	EffectVar(1, target, effectNum) = menuItems;
}

global func FxProductionMenuCheckTimer(object target, int effectNum) {
	var menuId = EffectVar(0, target, effectNum);
	var matSys = GetMatSys(GetOwner(target));
	if(GetMenu(target) != menuId || !matSys)
		return -1;
	var menuItems = EffectVar(1, target, effectNum);
	if(!GetLength(menuItems))
		return;
	var type = menuItems[GetMenuSelection(target)];
	if(!type)
		return;
	matSys->MaterialCheck(type);
}

global func FxProductionMenuCheckStop(object target, int effectNum) {
	var menuId = EffectVar(0, target, effectNum);
	if(GetMenu(target) == menuId) CloseMenu(target);
	GetMatSys(GetOwner(target))->ClearMaterialCheck();
}
