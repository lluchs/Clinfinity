/*-- Wreckage --*/

#strict 2

protected func SteamExhaust(){
	CastParticles("Smoke", 5, 5, -20, -20, 50, 70, RGBa(255, 255, 255, 200), RGBa(255, 255, 255, 0));
	CastParticles("Smoke", 5, 5, 20, -15, 50, 70, RGBa(255, 255, 255, 200), RGBa(255, 255, 255, 0));
}
