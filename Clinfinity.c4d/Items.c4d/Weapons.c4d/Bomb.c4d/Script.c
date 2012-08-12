/*	Script: Steam grenade
*/

#strict 2

static const BOMB_BounceDenominator = 3;
static const BOMB_DetonationRadius = 40;
static const BOMB_DamageToStructures = 10;
static const BOMB_CrewFlingSpeedY = 5;
static const BOMB_CrewFlingSpeedX = 3;

local isBounced;

public func Launch() {
	SetAction("Active");
	Sound("steam_exhaust", false, this, 20, 0, 1);
}

protected func LeakSteam() {
	Smoke(0, -3, 5);
}

private func Detonate() {
	// Fling crew members
	var crew = FindObjects(Find_Category(C4D_Living), Find_OCF(OCF_CrewMember), Find_Distance(BOMB_DetonationRadius));
	for(var member in crew) {
		var xDistance = member->GetX() - GetX();
		var xFlingSpeed = BOMB_CrewFlingSpeedX * xDistance / BOMB_DetonationRadius;
		Fling(member, xFlingSpeed, -BOMB_CrewFlingSpeedY);
	}

	// Damage structures
	var structures = FindObjects(Find_Category(C4D_Structure), Find_Distance(BOMB_DetonationRadius));
	for(var structure in structures) {
		structure->DoDamage(BOMB_DamageToStructures);
	}

	Sound("SteamGrenadeDetonate*");
	var steamAmount = RandomX(30, 50);
	for(var i = 0; i < steamAmount; i++) {
		BSTE->LaunchSteam(GetX(), GetY(), RandomX(BOMB_DetonationRadius / 2, BOMB_DetonationRadius * 6 / 5));
	}
	RemoveObject();
}

/*	Section: Events */

protected func Initialize() {
	isBounced = false;
}

/*	Function: Departure
	If a crew member throws the grenade while standing, it activates.

	Parameters:
	from	- The object the grenade departed from. */
protected func Departure(object from) {
	if((from->GetOCF() & OCF_CrewMember) != 0 && from->GetAction() == "Throw") {
		Launch();
	}
}

protected func Hit() {
	Sound("MetalHit*");
	CastParticles("PxSpark", RandomX(3, 5), 16, 0, 0, 15, 30, RGB(25, 25, 25), RGB(100, 100, 100));
}

protected func ContactTop() {
	Bounce(GetXDir(), -GetYDir());
}

protected func ContactRight() {
	Bounce(-GetXDir() / BOMB_BounceDenominator, GetYDir());
}

protected func ContactBottom() {
	Bounce(GetXDir(), -GetYDir() / BOMB_BounceDenominator);
}

protected func ContactLeft() {
	Bounce(-GetXDir() / BOMB_BounceDenominator, GetYDir());
}

private func Bounce(int xSpeed, int ySpeed) {
	if(GetAction() == "Active") {
		if(!isBounced) {
			isBounced = true;
			// Wait one frame until setting the new speed. Otherwise the current speed is kept unchanged.
			ScheduleCall(this, "DoBounce", 1, 0, xSpeed, ySpeed);
		} else {
			Detonate();
		}
	}
}

public func DoBounce(int xSpeed, int ySpeed) {
	SetXDir(xSpeed);
	SetYDir(ySpeed);
}

protected func QueryStrikeBlow(object target) {
	Detonate();
}