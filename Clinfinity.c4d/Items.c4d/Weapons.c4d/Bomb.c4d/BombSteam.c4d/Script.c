/*	Script: BombSteam
	Uses a geometric series to calculate the expansion! */

#strict 2

static const BSTE_ExpansionNumerator = 1023;
static const BSTE_ExpansionDenominator = 512;
static const BSTE_ExpansionFrames = 10;
static const BSTE_FumeFrames = 20;

local maxXDistance, maxYDistance;

public func LaunchSteam(int x, int y, int maxRadius) {
	var steam = CreateObject(BSTE, x, y + 32, NO_OWNER);
	var direction = Random(360);
	steam->LocalN("maxXDistance") = Sin(direction, maxRadius);
	steam->LocalN("maxYDistance") = -Cos(direction, maxRadius);
}

protected func Initialize() {
	SetAction("Expanding");
	SetCon(RandomX(10, 50));
	SetR(Random(360));
	SetRDir(RandomX(-10, 10));
	SetClrModulation(RGBa(255, 255, 255, 128));
}

protected func Expand() {
	if(GetActTime() >= BSTE_ExpansionFrames) {
		StartFume();
		return;
	}
	var deltaX = (BSTE_ExpansionDenominator * maxXDistance) / (BSTE_ExpansionNumerator * (GetActTime() + 1));
	var deltaY = (BSTE_ExpansionDenominator * maxYDistance) / (BSTE_ExpansionNumerator * (GetActTime() + 1));
	SetPosition(GetX() + deltaX, GetY() + deltaY);
}

protected func StartFume() {
	SetAction("Fuming");
	// Up!
}

protected func Fume() {
	SetPosition(GetX(), GetY()-2);
	// TODO: Become more and more transparent
	if(GetActTime() >= BSTE_FumeFrames) {
		RemoveObject();
	}
}