/*	Script: Melee weapons
	Functionality for using melee weapons. */

#strict 2

static const AVMW_WieldUp	= 1;
static const AVMW_WieldDown	= 2;

public func ReadyToWield() {
	return GetAction() == "Walk" || GetAction() == "Jump";
}

private func WieldMeleeWeapon() {
	var weapon = Contents(0);
	if(weapon != 0 && weapon->~IsMeleeWeapon()) {
		var wieldDirection = weapon->GetWieldDirection();
		if(wieldDirection == 0) return;

		if(wieldDirection == AVMW_WieldUp) {
			SetAction("WieldUp");
		} else if(wieldDirection == AVMW_WieldDown) {
			SetAction("WieldDown");
		}
		//weapon->~WieldStart();
	}
}

/*	Section: Calls to weapons
	The functions described in this section are called in the weapon object at the appropriate times. */

public func WieldStart() { CallToWeapon("WieldStart"); }

public func WieldEnd() { CallToWeapon("WieldEnd"); }

public func WieldAbort() { CallToWeapon("WieldAbort"); }

private func CallToWeapon(string callName) {
	var weapon = Contents(0);
	if(weapon != 0 && weapon->~IsMeleeWeapon()) {
		ObjectCall(weapon, callName);
	}
}
