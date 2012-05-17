/*-- Functionality for material system menus --*/

#strict 2

/*  Function: AddMaterialMenuItem
    Adds a menu item to a menu as documented, but also shows required materials
    in the players material system.  */
global func AddMaterialMenuItem(string caption, string command, id item, object menuObject, int iCount, int parameter, string infoCaption, int extra, par1, par2) {
	var symbol = GetMenu(menuObject);
	if(!symbol) return;
	if(command) {
		var effect = GetEffect("MaterialMenuCheck", menuObject);
		if(!effect) effect = AddEffect("MaterialMenuCheck", menuObject, 300, 1, menuObject, symbol, symbol);
		EffectCall(menuObject, effect, "AddItem", item);
	}
	AddMenuItem(caption, command, item, menuObject, iCount, parameter, infoCaption, extra, par1, par2);
}

global func FxMaterialMenuCheckStart(object target, int effectNum, int temp, id menuId) {
	EffectVar(0, target, effectNum) = menuId; //menuId
	EffectVar(1, target, effectNum) = []; //menuItems
}

global func FxMaterialMenuCheckAddItem(object target, int effectNum, id menuId) {
	var menuItems = EffectVar(1, target, effectNum);
	menuItems[GetLength(menuItems)] = menuId;
	EffectVar(1, target, effectNum) = menuItems;
}

global func FxMaterialMenuCheckTimer(object target, int effectNum) {
	var menuId = EffectVar(0, target, effectNum);
	var matSys = GetMatSys(GetOwner(target));
	if(GetMenu(target) != menuId || !matSys)
		return -1;
		
	var menuItems = EffectVar(1, target, effectNum);
	if(!GetLength(menuItems))
		return;
	var type = menuItems[GetMenuSelection(target)];
	if(!type)
		return GetMatSys(GetOwner(target))->ClearMaterialCheck();
	matSys->MaterialCheck(type);
}

global func FxMaterialMenuCheckStop(object target, int effectNum) {
	var menuId = EffectVar(0, target, effectNum);
	if(GetMenu(target) == menuId) CloseMenu(target);
	GetMatSys(GetOwner(target))->ClearMaterialCheck();
}
