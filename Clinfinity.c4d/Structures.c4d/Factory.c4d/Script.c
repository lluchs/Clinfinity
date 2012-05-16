/*-- Factory --*/

#strict 2

local requestedItem;
local remainingTime;
local remainingAmount;
local steamWhite;

public func ControlDigDouble(object caller) {
	if(IsProducing()) return;
	ShowProductionMenu(caller);
}

public func ShowProductionMenu(object caller) {
	CreateMenu(CXCN,caller,this,1,"$NoPlrKnowledge$");
	var knowledge = ROCK;
	AddMenuItem("$Production$: %s", "StartProduction", knowledge, caller, 0, caller);
}

public func StartProduction(id item) {
	requestedItem = item;
	remainingAmount = 3;
	ContinueProduction();
	SetAction("Produce");
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
	var producedItem = CreateObject(requestedItem, 0, 0, GetOwner());
}

public func CompletedProduction() {
	SetAction("None");
	Sound("finish*");
	remainingTime = 0;
	steamWhite = 20;
}

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
