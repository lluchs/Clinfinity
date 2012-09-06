/*-- Quarry --*/

#strict 2

#include STBO

protected func Initialize() {
	for(var i = 0; i < 5; ++i) {
		DRNE->CreateDrone(GetX(), GetY(), GetOwner(), this);
	}
	return _inherited(...);
}

protected func Quarry() {
	Sound("Quarry");
}
