/*	Script: Grenade steam
	Visual steam shockwave effect for the <Steam grenade>.
	Its animation is divided into two phases: Expansion and fuming.

	Expansion:
	* A steam cloud moves from the point where it is created to an end point.
	* Initially, it moves fast but slows down continuously, until its speed is zero at the end point.
	* Its speed is calculated using a <geometric series at http://en.wikipedia.org/wiki/Geometric_series>.

	When creating a shockwave with several steam clouds,
	the movement in this phase leads to the visual impression that the steam is stopped by air drag.

	Fuming:
	* The steam cloud starts moving upwards.
	* It accelerates until it reaches a randomly chosen maximum speed.
	* At the same time, it quickly fades out until it disappears completely. */

#strict 2

/*	Constants: Expansion phase
	BSTE_ExpansionNumerator		- Numerator used in the geometric series.
	BSTE_ExpansionDenominator	- Denominator used in the geometric series.
	BSTE_ExpansionFrames		- Length of the expansion phase in frames. */
static const BSTE_ExpansionNumerator = 1023;
static const BSTE_ExpansionDenominator = 512;
static const BSTE_ExpansionFrames = 10;

/*	Constants: Fuming phase
	BSTE_MinTargetYSpeed	- Minimum value for target vertical speed.
	BSTE_MaxTargetYSpeed	- Maximum value for target vertical speed. */
static const BSTE_MinTargetYSpeed = -10;
static const BSTE_MaxTargetYSpeed = -22;

local maxXDistance, maxYDistance, transparency, targetYSpeed;

/*	Function: LaunchSteam
	Factory method for launching single steam clouds.
	The coordinates are relative to the calling object in local calls, otherwise global.
	*Note:* For best results, launch steam using this method. This is not mandatory, though.

	Parameters:
	x			- Horizontal coordinate.
	y			- Vertical coordinate.
	maxRadius	- Distance between start and end point for the expansion phase.
	direction	- Direction to move in the expansion phase. */
public func LaunchSteam(int x, int y, int maxRadius, direction) {
	var steam = CreateObject(BSTE, x, y + 16, NO_OWNER);
	steam->LocalN("maxXDistance") = Sin(direction, maxRadius);
	steam->LocalN("maxYDistance") = -Cos(direction, maxRadius);
}

protected func Initialize() {
	SetAction("Expanding");
	SetDir(Random(15));
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
	targetYSpeed = RandomX(BSTE_MinTargetYSpeed, BSTE_MaxTargetYSpeed);
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