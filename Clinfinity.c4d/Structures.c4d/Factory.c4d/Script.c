/*-- Factory --*/

#strict 2

local requestedId;
local remainingTime;
local remainingAmount;
local steamWhite;
local player;

/* Interaktion */

public func ControlDigDouble(object caller) {
	if(IsProducing()) return AlreadyProducing(caller);
	ProductionMenu(caller);
}

public func ProductionMenu(object caller) {
	if(IsProducing()) return AlreadyProducing(caller);
	CreateMenu(CXCN, caller, this, 0, "$TxtNoPlrKnowledge$");
	//todo: Verfügbare Produktion anzeigen und Kosten im HUD einblenden
	var knowledge = ROCK;
	AddMenuItem("$TxtProduction$: %s", "RequestProduction", knowledge, caller, 0, caller);
}

public func ProductionAmountMenu(object caller, id item, int amount) {
	if(!amount) amount = 1;
	
	var index = 0;
	if(GetMenu(caller)) {
		index = GetMenuSelection(caller);
		CloseMenu(caller);
	}
	
	Sound("Click", 0, 0, 100, GetOwner(caller)+1);

	CreateMenu(CXCN, caller, this, 1, 0, 0, 0, 1);

	AddMenuItem("$TxtIncrementProduction$", Format("ProductionAmountMenu(Object(%d), %i, %d, 0)", ObjectNumber(caller), item, amount+1), MS4C, caller, 0, 0, "$TxtIncrementProductionDesc$",  2, 1);
	AddMenuItem("$TxtDecrementProduction$", Format("ProductionAmountMenu(Object(%d), %i, %d, 1)", ObjectNumber(caller), item, Max(amount-1, 1)), MS4C, caller, 0, 0, "$TxtDecrementProductionDesc$",  2, 2);
	AddMenuItem("$TxtLaunchProduction$", Format("RequestAmountProduction(%i, Object(%d), %d)", item, ObjectNumber(caller), amount), MS4C, caller, 0, 0, "OK", 2, 3);
	
	SelectMenuItem(index, caller);

	PlayerMessage(GetOwner(caller), "%dx {{%i}}", caller, amount, item);
}

protected func RequestProduction(id item, object caller, bool right) {
	if(IsProducing()) return AlreadyProducing(caller);
	if(!right) {
		StartProduction(item, GetOwner(caller), 1);
	}
	else {
		ProductionAmountMenu(caller, item);
	}
}

protected func RequestAmountProduction(id item, object caller, int amount) {
	if(IsProducing()) return AlreadyProducing(caller);
	if(GetMenu(caller)) CloseMenu(caller);
	StartProduction(item, GetOwner(caller), amount);
}

private func AlreadyProducing(object clonk) {
	Sound("Error", 0, 0, 100, GetOwner(clonk)+1);
	PlayerMessage(GetOwner(clonk), "$TxtAlreadyProducing$", clonk);
}

/* Produktion */

public func StartProduction(id item, int player, int amount) {
	if(IsProducing()) return;
	if(!amount) amount = 1;
	remainingAmount = amount;
	requestedId = item;
	SetAction("Produce");
	ContinueProduction();
}

protected func ContinueProduction() {
	remainingTime = 5;
	steamWhite = 0;
	//todo: Ressourcen benötigen
	if(true) {
		//todo: Ressourcen entfernen
	}
	else {
		CompletedProduction();
	}
}

protected func Produce() {
	remainingTime--;
	if(!remainingTime) return CompleteProduction();
}

protected func CompleteProduction() {
	remainingAmount--;
	if(remainingAmount) {
		ContinueProduction();
	}
	else {
		CompletedProduction();
	}
	var matSys = GetMatSys(GetOwner(), true);
	if(matSys != 0 && InArray(requestedId, GetMatSysIDs())) {
		matSys->DoFill(1, requestedId);
	}
	else {
		var producedItem = CreateObject(requestedId, 0, 0, GetOwner());
	}
}

public func AbortProduction() {
	SetAction("None");
	remainingTime = 0;
}

private func CompletedProduction() {
	AbortProduction();
	Sound("finish*");
	steamWhite = 23;
}

/* Callbacks und Effekte */

public func IsProducing() {
	return remainingTime > 0;
}

private func Smoking() {
	if(!IsProducing() && !steamWhite) return;
	var steamColor = 0;
	if(steamWhite) {
		steamWhite--;
		CreateParticle("Smoke", 25, -10, 0, 0, 70, RGB(255, 255, 255));
		CreateParticle("Smoke", 20, -10, 0, 0, 70, RGB(255, 255, 255));
	}
	else {
		CreateParticle("Smoke", -18, -18, 0, 0, 150, RGBa(0, 0, 0, 0));
		CreateParticle("Smoke", -40, -18, 0, 0, 150, RGBa(0, 0, 0, 0));
	}
	
}
