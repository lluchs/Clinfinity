/*	Script: Steam grenade
	A grenade that bounces once and detonates on second impact.

	Bouncing:
	The steam grenade only bounces off from ceilings with full speed.
	For walls, the vertical speed is kept but the horizontal speed is reversed and reduced.
	Similarly for floors, the vertical speed is reversed and reduced while the horizontal speed is kept.
	The reduced speed is calculated using the following formula:
	:new speed = -1 * original speed / BOMB_BounceDenominator
	To ensure bouncing once, the grenade delays the detection of the second impact.
	This prevents immediate detonations when two vertices touch the landscate at the same time. */

#strict 2

/*	Constants: Bouncing
	BOMB_BounceDenominator	- Determines how much the grenade bounces off a surface.
	BOMB_SecondContactDelay	- Determines after how many frames contacs are again evaluated. */
static const BOMB_BounceDenominator = 3;
static const BOMB_SecondContactDelay = 5;

/*	Constants: Detonation
	BOMB_DetonationRadius	- Determines the radius of the detonation effects.
	BOMB_DamageToStructures	- Damage done to structures.
	BOMB_CrewFlingSpeedY	- Vertical speed when the detonation flings crew members.
	BOMB_CrewFlingSpeedX	- Maximum horizontal speed when the detonation flings crew members.
	BOMB_OtherFlingSpeedY	- Vertical speed when the detonation flings objects and vehicles.
	BOMB_OtherFlingSpeedX	- Maximum horizontal speed when the detonation flings objects and vehicles. */
static const BOMB_DetonationRadius = 40;
static const BOMB_DamageToStructures = 20;
static const BOMB_CrewFlingSpeedY = 3;
static const BOMB_CrewFlingSpeedX = 4;
static const BOMB_OtherFlingSpeedY = 3;
static const BOMB_OtherFlingSpeedX = 5;

local isBounced;

public func Launch() {
	SetAction("Active");
	Sound("steam_exhaust", false, this, 20, 0, 1);
}

private func Active() {
	LeakSteam();
	if(FindObject2(Find_AtPoint(0, 0), Find_Category(C4D_Structure)) != 0) {
		Detonate();
	}
}

private func LeakSteam() {
	Smoke(0, -3, 5);
}

private func Detonate() {
	// Fling crew members, objects and vehicles. Crew members aren't flung as far horizontally to keep it fair.
	var crew = FindObjects(Find_Category(C4D_Living), Find_OCF(OCF_CrewMember | OCF_InFree), Find_Distance(BOMB_DetonationRadius), Find_NoContainer());
	for(var member in crew) {
		var xDistance = member->GetX() - GetX();
		if(Abs(xDistance) < 10 && GetXDir() != 0) {
			xDistance = 10 * GetXDir() / Abs(GetXDir());
		}
		var xFlingSpeed = BOMB_CrewFlingSpeedX * xDistance / BOMB_DetonationRadius;
		Fling(member, xFlingSpeed, -BOMB_CrewFlingSpeedY);
	}
	var stuffs = FindObjects(Find_Or(Find_Category(C4D_Object), Find_Category(C4D_Vehicle)), Find_OCF(OCF_InFree), Find_Distance(BOMB_DetonationRadius), Find_NoContainer(), Find_Not(Find_Procedure("FLOAT")));
	for(var stuff in stuffs) {
		var xDistance = stuff->GetX() - GetX();
		var xFlingSpeed = BOMB_OtherFlingSpeedX * xDistance / BOMB_DetonationRadius;
		Fling(stuff, xFlingSpeed, -BOMB_OtherFlingSpeedY);
	}

	// Damage structures
	var structures = FindObjects(Find_Category(C4D_Structure), Find_Distance(BOMB_DetonationRadius));
	for(var structure in structures) {
		structure->DoDamage(BOMB_DamageToStructures);
	}

	Sound("SteamGrenadeDetonate*");
	var steamAmount = RandomX(30, 50);
	for(var i = 0; i < steamAmount; i++) {
		BSTE->LaunchSteam(GetX(), GetY(), RandomX(BOMB_DetonationRadius / 2, BOMB_DetonationRadius * 6 / 5), Random(360));
	}
	RemoveObject();
}

/*	Section: Events */

protected func Initialize() {
	isBounced = false;
}

/*	Function: Departure
	If a crew member throws the grenade or drops it in various situations, it activates.

	Parameters:
	from	- The object the grenade departed from. */
protected func Departure(object from) {
	if((from->GetOCF() & OCF_CrewMember) != 0 && (from->GetAction() == "Throw" || from->GetProcedure() == "FLIGHT" || from->GetProcedure() == "SCALE" || from->GetProcedure() == "HANGLE")) {
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
			// Don't detonate right now when two vertices have contact at the same time.
			AddEffect("DelaySecondContact", this, 101, BOMB_SecondContactDelay);
		} else if(!GetEffect("DelaySecondContact", this)) {
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