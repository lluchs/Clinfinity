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
	CreateMenu(CXCN, caller, this, 1, "$TxtNoPlrKnowledge$");
	var knowledge = ROCK;
	AddMenuItem("$TxtProduction$: %s", "RequestProduction", knowledge, caller, 0, caller);
}

public func ProductionAmountMenu(object caller, id item) {
	var amount = 3;

	var dummy = CreateObject(TIM1);
	SetGraphics("Chosen", dummy, MS4C, 1, GFXOV_MODE_Picture);
	CreateMenu(CXCN, caller, this, 1);
	AddMenuItem("$TxtLaunchProduction$", Format("RequestAmountProduction(%i, Object(%d), %d)", item, ObjectNumber(caller), amount), 0, caller, 0, 0, "OK", 4, dummy);
	dummy->RemoveObject();
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
	StartProduction(item, GetOwner(caller), 3);
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

public func CompleteProduction() {
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

public func CompletedProduction() {
	SetAction("None");
	Sound("finish*");
	remainingTime = 0;
	steamWhite = 20;
}

/* Nochwas */

public func IsProducing() {
	return remainingTime > 0;
}

private func Smoking() {
	if(!IsProducing() && !steamWhite) return;
	var steamColor = 0;
	if(steamWhite) {
		steamWhite--;
		steamColor = RGB(255,255,255);
	}
	CreateParticle("Smoke",-18,-18,0,0,150,steamColor);
	CreateParticle("Smoke",-40,-18,0,0,150,steamColor);
}
