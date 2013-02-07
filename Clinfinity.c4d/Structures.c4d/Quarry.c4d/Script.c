/*-- Quarry --*/

#strict 2

#include STBO	////StructureBaseObject
#include NLBO	//Nightlight

static const QRRY_DroneCount = 3;
static const QRRY_MaxSearchDistance = 350;
static const QRRY_MaxSearchIterations = 100;

local lastRockX, lastRockY;

protected func Initialize() {
	for(var i = 0; i < QRRY_DroneCount; ++i) {
		var x = GetX() + RandomX(-GetDefWidth(GetID()) / 4, GetDefWidth(GetID()) / 4);
		var y = GetY() + RandomX(-GetDefHeight(GetID()) / 4, GetDefHeight(GetID()) / 4) + GetDefHeight(DRNE) / 2;
		DRNE->CreateDrone(x, y, GetOwner(), this);
	}
	return inherited(...);
}

protected func Collection2(object collected) {
	if(GetAction() != "Stomp") {
		SetAction("Stomp");
	}
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
			MatSysDoTeamFill(1, GetOwner(), id);
		}
		rock->RemoveObject();
	}
	// Stop when all contents have been stomped.
	if(Contents() == 0) {
		SetAction("Attach");
	}
}

public func FindDrillingPosition(&x, &y) {
	if(GetAction() == "Stomp") {
		return false;
	}

	if(GetMaterial(lastRockX, lastRockY) == Material("Metalearth")) {
		x = lastRockX + GetX();
		y = lastRockY + GetY();
		return true;
	}

	var searchX = RandomX(-QRRY_MaxSearchDistance, QRRY_MaxSearchDistance);
	// Search inside a circle: Limit randomly chosen points to the chord for the given x.
	var h = QRRY_MaxSearchDistance - Abs(searchX);
	var maxSearchY = Sqrt(2 * QRRY_MaxSearchDistance * h - h ** 2);
	var searchY = RandomX(-maxSearchY, maxSearchY);

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

/* Controls */

protected func ControlUp() {
	[$TxtShowSearchRadius$]
	// Show search radius
	for(var i; i < 360; i++)
		CreateParticle("PSpark", Cos(i, QRRY_MaxSearchDistance), Sin(i, QRRY_MaxSearchDistance), 0, 0, 70, RGBa(255, 255, 255, 128));
}
