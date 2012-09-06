/*-- Quarry --*/

#strict 2

#include STBO

static const QRRY_MaxSearchDistance = 500;

local lastRockX, lastRockY;

protected func Initialize() {
	for(var i = 0; i < 5; ++i) {
		DRNE->CreateDrone(GetX(), GetY(), GetOwner(), this);
	}
	return _inherited(...);
}

protected func Quarry() {
	Sound("Quarry");
}

public func FindDrillingPosition(&x, &y) {
	if(GetMaterial(lastRockX, lastRockY) == Material("Metalearth")) {
		x = lastRockX + GetX();
		y = lastRockY + GetY();
		return true;
	}

	var searchX = RandomX(-QRRY_MaxSearchDistance, QRRY_MaxSearchDistance);
	var searchY = RandomX(-QRRY_MaxSearchDistance, QRRY_MaxSearchDistance);

	for(var i = 0; i < 100; ++i) {
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
