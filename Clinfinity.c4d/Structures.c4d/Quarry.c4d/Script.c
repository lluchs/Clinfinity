/*-- Quarry --*/

#strict 2

#include STBO
#include NLBO

static const QRRY_MaxSearchDistance = 500;
static const QRRY_MaxSearchIterations = 100;

local lastRockX, lastRockY;

protected func Initialize() {
	for(var i = 0; i < 5; ++i) {
		var x = GetX() + RandomX(-GetDefWidth(GetID()) / 4, GetDefWidth(GetID()) / 4);
		var y = GetY() + RandomX(-GetDefHeight(GetID()) / 4, GetDefHeight(GetID()) / 4) + GetDefHeight(DRNE) / 2;
		DRNE->CreateDrone(x, y, GetOwner(), this);
	}
	return _inherited(...);
}

protected func Collection2(object collected) {
	SetAction("Stomp");
}

protected func Quarry() {
	Sound("Quarry");
}

protected func Stomping() {
	var rock = Contents();
	if(rock != 0) {
		var matSys = GetMatSys(GetOwner(), true);
		var id = rock->GetID();
		if(matSys != 0 && InArray(id, GetMatSysIDs())) {
			matSys->DoFill(1, id);
		}
		rock->RemoveObject();
	}
	// Stop when all contents have been stomped.
	if(Contents() == 0) {
		SetAction("Attach");
	}
}

public func FindDrillingPosition(&x, &y) {
	if(GetMaterial(lastRockX, lastRockY) == Material("Metalearth")) {
		x = lastRockX + GetX();
		y = lastRockY + GetY();
		return true;
	}

	var searchX = RandomX(-QRRY_MaxSearchDistance, QRRY_MaxSearchDistance);
	var searchY = RandomX(-QRRY_MaxSearchDistance, QRRY_MaxSearchDistance);

	for(var i = 0; i < QRRY_MaxSearchIterations; ++i) {
		if(GetMaterial(searchX, searchY) == Material("Metalearth")) {
			x = searchX + GetX();
			y = searchY + GetY();
			lastRockX = searchX;
			lastRockY = searchY;
			return true;
		}
	}
	return false;
}
