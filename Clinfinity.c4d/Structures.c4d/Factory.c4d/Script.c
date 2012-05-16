/*-- Factory --*/

#strict 2

local requestedItem;
local remainingTime;

public func ControlDigDouble(object caller) {
	ShowProductionMenu(caller);
}

public func ShowProductionMenu(object caller) {
	CreateMenu(CXCN,caller,this,1,"");
	var knowledge = ROCK;
	AddMenuItem("", "StartProduction", knowledge, caller, 0, caller);
}

public func StartProduction(id item) {
	requestedItem = item;
	remainingTime = 10;
	SetAction("Produce");
}

protected func Produce() {
	if(!remainingTime) return CompleteProduction();
	remainingTime--;
}

public func CompleteProduction() {
	SetAction("Idle");
	Sound("finish*");
	remainingTime = 0;
	var producedItem = CreateObject(requestedItem, 0, 0, GetOwner());
}

public func IsProducing() {
	return remainingTime > 0;
}

private func Smoking() {
	if(!IsProducing()) return;
	CreateParticle("Smoke",-18,-18,0,0,150,RGBa(0,0,0,0));
	CreateParticle("Smoke",-40,-18,0,0,150,RGBa(0,0,0,0));
}
