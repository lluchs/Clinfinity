/*-- Factory --*/

#strict 2

#include STBO
#include L_DC

static const FTRY_ProductionActions = 3;

public func DamageGraphics() { return 3; }

public func MaxDamage() { return 60; }

// Array of items that will be produced next, in format [id, amount].
local queue;
// Id of the object that is currently in production.
local requestedId;
// Remaining time on the current production in 20f.
// Will always be set to 5 when starting production.
local remainingTime;
// Batch production: number of items of requestedId which will be produced automatically.
local remainingAmount;
// Controls the amount of white steam at the end of a production.
local steamWhite;

protected func Initialize() {
	queue = [];
	SetStillOverlayAction("Door18", 1);
	return inherited(...);
}

/* Interaktion */

public func ControlDigDouble(object caller) {
	if(Hostile(GetOwner(), caller->GetOwner())) {
		Sound("CommandFailure1");
		return;
	}
	ProductionMenu(caller);
}

public func ProductionMenu(object caller) {
	CreateMenu(CXCN, caller, this, 0, "$TxtNoPlrKnowledge$");

	var plr = caller->GetOwner(), i = 0, knowledge;
	while(knowledge = GetPlrKnowledge(plr, 0, i++, C4D_Object)) {
		AddMaterialMenuItem("$TxtProduction$: %s", "RequestProduction", knowledge, caller, 0, caller);
	}
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
	if(!right) {
		StartProduction(item, GetOwner(caller), 1);
	}
	else {
		ProductionAmountMenu(caller, item);
	}
}

protected func RequestAmountProduction(id item, object caller, int amount) {
	if(GetMenu(caller)) CloseMenu(caller);
	StartProduction(item, GetOwner(caller), amount);
}

/* Produktion */

public func StartProduction(id item, int player, int amount) {
	if(!amount) amount = 1;
	if(IsProducing()) {
		// Add the new item to the queue.
		PushBack([item, amount], queue);
		return;
	}
	remainingAmount = amount;
	requestedId = item;
	SetAction(Format("Produce%d", Random(FTRY_ProductionActions) + 1));
	ContinueProduction();
}

protected func ContinueProduction() {
	remainingTime = 5;
	steamWhite = 0;
	var steamUsage = requestedId->~FactorySteamUsage() || 50;
	if(MatSysGetTeamFill(GetOwner(), STEM) >= steamUsage && MatSysSubtractComponents(requestedId, GetOwner())) {
		MatSysDoTeamFill(-steamUsage, GetOwner(), STEM);
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
	// Create the produced item.
	var matSys = GetMatSys(GetOwner(), true);
	if(matSys != 0 && InArray(requestedId, GetMatSysIDs())) {
		matSys->DoFill(1, requestedId);
	}
	else {
		var producedItem = CreateObject(requestedId, 49, 74, GetOwner());
		OpenDoor();
	}
	// Continue production if requested.
	remainingAmount--;
	if(remainingAmount) {
		ContinueProduction();
	}
	else {
		var next = PopElement(queue);
		if(next) {
			requestedId = next[0];
			remainingAmount = next[1];
			ContinueProduction();
		}
		else {
			CompletedProduction();
		}
	}
}

public func AbortProduction() {
	SetAction("Attach");
	remainingTime = 0;
}

private func CompletedProduction() {
	AbortProduction();
	Sound("finish*");
	steamWhite = 23;
}

private func OpenDoor() {
	SetOverlayAction("Door", 1, true);
	ScheduleCall(this, "CloseDoor", 60);
}

protected func CloseDoor() {
	SetOverlayAction("Door", 1);
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
