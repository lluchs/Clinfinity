/*-- Pipe --*/

#strict 2

local leaking;

protected func Damage() {
	if(!leaking) {
		leaking = true;
		Sound("steam_exhaust", 0, 0, 30, 0, 1);
	}
}

protected func Leak() {
	if(leaking)
		CreateParticle("Smoke", 50, -140, 0, 50, 190, RGBa(255, 255, 255, 0));
}
