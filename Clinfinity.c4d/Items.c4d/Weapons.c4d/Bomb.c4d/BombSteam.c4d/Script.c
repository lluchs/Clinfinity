/*	Script: BombSteam
	Uses a geometric series to calculate the expansion! */

#strict 2

static const BSTE_ExpansionNumerator = 1023;
static const BSTE_ExpansionDenominator = 512;
static const BSTE_ExpansionFrames = 10;

local maxXDistance, maxYDistance, transparency, targetYSpeed;

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
	transparency = RandomX(100, 200);
	SetClrModulation(RGBa(255, 255, 255, transparency));
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
	targetYSpeed = RandomX(-10, -22);
	SetYDir(-10);
}

protected func Fume() {
	if(GetYDir() > targetYSpeed) {
		SetYDir(GetYDir() - 1);
	}
	transparency += 2;
	if(transparency > 255) {
		RemoveObject();
	} else {
		SetClrModulation(RGBa(255, 255, 255, transparency));
	}
}