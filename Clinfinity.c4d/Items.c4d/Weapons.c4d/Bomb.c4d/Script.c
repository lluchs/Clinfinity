/*	Script: Steam grenade
*/

#strict 2

local isBounced;

public func Launch() {
	SetAction("Active");
	Sound("steam_exhaust", false, this, 20, 0, 1);
}

protected func LeakSteam() {
	Smoke(0, -3, 5);
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

protected func ContactTop() {
	Bounce(GetXDir(), -GetYDir());
}

protected func ContactRight() {
	Bounce(-GetXDir() / 3, GetYDir());
}

protected func ContactBottom() {
	Bounce(GetXDir(), -GetYDir() / 3);
}

protected func ContactLeft() {
	Bounce(-GetXDir() / 3, GetYDir());
}

private func Bounce(int xSpeed, int ySpeed) {
	if(GetAction() == "Active") {
		if(!isBounced) {
			isBounced = true;
			ScheduleCall(this, "DoBounce", 1, 0, xSpeed, ySpeed);
		} else {
			// BOOM!
		}
	}
}

public func DoBounce(int xSpeed, int ySpeed) {
	SetXDir(xSpeed);
	SetYDir(ySpeed);
}