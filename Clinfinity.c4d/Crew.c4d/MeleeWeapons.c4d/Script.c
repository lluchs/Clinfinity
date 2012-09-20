/*	Script: Melee weapons
	Functionality for using melee weapons. */

#strict 2

static const AVMW_WieldUp	= 1;
static const AVMW_WieldDown	= 2;

local activeMeleeWeapon, startAngle, angularSpeed;

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
		activeMeleeWeapon = weapon;

		var frames = GetActMapVal("Length", GetAction());
		var delay = GetActMapVal("Delay", GetAction());
		var duration = frames * delay;

		startAngle = NormaliseAngle(weapon->GetStartAngle(GetDir()));
		var endAngle = NormaliseAngle(weapon->GetEndAngle(GetDir()));
		angularSpeed = (endAngle - startAngle) / duration;

		Wielding();

		weapon->WieldStart(duration);
	}
}

private func Wielding() {
	DrawMeleeWeaponOverlay();

	var angle = startAngle + GetActTime() * angularSpeed;
	var x = 0 + activeMeleeWeapon->~HandX();
	if(GetDir() == DIR_Left) x = -x;
	var y = -5 - activeMeleeWeapon->~HandY();

	DrawRotated(angle, x, y, 0, -1);

	ScheduleCall(this, "Wielding", 1);
}

private func NormaliseAngle(int angle) {
	angle %= 360;
	if(angle < 0) {
		angle += 360;
	}
	return angle;
}

private func DrawMeleeWeaponOverlay() {
	SetGraphics(0, this, activeMeleeWeapon->GetID(), AVTR_WeaponOverlay, GFXOV_MODE_Object, 0, 0, activeMeleeWeapon);
}

private func DrawRotated(int angle, int x, int y, int xOffset, int yOffset) {
	var sin1 = Sin(360 - angle, 1000);
	var cos1 = Cos(360 - angle, 1000);
	var sin2 = Sin(angle, 1000);
	var cos2 = Cos(angle, 1000);
	var x2 = x * cos2 - y * sin2;
	var y2 = x * sin2 + y * cos2;
	SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, this, AVTR_WeaponOverlay);
	activeMeleeWeapon->SetObjDrawTransform(cos1, sin1, x2 + xOffset * 1000, -sin1, cos1, y2 + yOffset * 1000);
}

private func RemoveMeleeWeaponOverlay() {
	SetGraphics(0, this, 0, AVTR_WeaponOverlay);
}

/*	Section: Calls to weapons
	The functions described in this section are called in the weapon object at the appropriate times. */

public func WieldStart(int wieldDuration) {}

public func WieldEnd() {
	ClearScheduleCall(this, "Wielding");
	RemoveMeleeWeaponOverlay();
	CallToWeapon("WieldEnd");
}

public func WieldAbort() {
	ClearScheduleCall(this, "Wielding");
	RemoveMeleeWeaponOverlay();
	CallToWeapon("WieldAbort");
}

private func CallToWeapon(string callName, a) {
	var weapon = Contents(0);
	if(weapon != 0 && weapon->~IsMeleeWeapon()) {
		ObjectCall(weapon, callName, a);
	}
}
