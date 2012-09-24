/*	Script: Melee weapons
	Functionality for using melee weapons. */

#strict 2

static const AVMW_WieldUp	= 1;
static const AVMW_WieldDown	= 2;
static const AVMW_WieldHold	= 3;

static const AVMW_StandardHandX = 0;
static const AVMW_StandardHandY = -5;

static const AVMW_WieldDownShoulderY	= -1;
static const AVMW_WieldUpShoulderY		= 0;

local activeMeleeWeapon, startAngle, angularSpeed, endAngle;
local isRegularActionSwitch;

public func ReadyToWield() {
	// Can't use more than one melee weapon
	if(YOYO->IsYoyoThrownBy(this)) return false;
	return GetAction() == "Walk" || GetAction() == "Jump";
}

public func GetCurrentWieldData(&handX, &handY, &weaponAngle) {
	if(activeMeleeWeapon != 0) {
		var shoulderY;
		if(GetAction() == "WieldDown" || GetAction() == "WieldBackUp" || GetAction() == "WieldDownHold") {
			shoulderY = AVMW_WieldDownShoulderY;
		} else {
			shoulderY = AVMW_WieldUpShoulderY;
		}

		var frames = GetActMapVal("Length", GetAction());
		var delay = GetActMapVal("Delay", GetAction());
		var duration = frames * delay;
		if(GetAction() == "WieldDownHold" || GetAction() == "WieldUpHold") {
			weaponAngle = startAngle;
		} else if(GetActTime() >= duration) {
			weaponAngle = endAngle;
		} else {
			weaponAngle = startAngle + GetActTime() * angularSpeed;
		}

		handX = AVMW_StandardHandX;
		handY = AVMW_StandardHandY;
		Rotate(weaponAngle, handX, handY, 0, shoulderY);
	}
}

private func WieldMeleeWeapon() {
	var weapon = Contents(0);
	if(weapon != 0 && weapon->~IsMeleeWeapon()) {
		var wieldDirection = weapon->GetWieldDirection();
		var wieldDirectionName;
		var movementName = "";

		if(wieldDirection == AVMW_WieldUp) wieldDirectionName = "WieldUp";
		else if(wieldDirection == AVMW_WieldDown) wieldDirectionName = "WieldDown";
		else return;

		if(GetAction() == "Jump") movementName = "Jump";

		SetAction(Format("%s%s", wieldDirectionName, movementName));

		activeMeleeWeapon = weapon;

		var frames = GetActMapVal("Length", GetAction());
		var delay = GetActMapVal("Delay", GetAction());
		var duration = frames * delay;

		startAngle = NormaliseAngle(weapon->GetStartAngle(GetDir()));
		angularSpeed = weapon->GetCentralAngle(GetDir()) / duration;
		endAngle = NormaliseAngle(weapon->GetStartAngle(GetDir())) + weapon->GetCentralAngle(GetDir());

		weapon->SetR(0);
		weapon->WieldStart(GetDir());
		Wielding();
	}
}

private func Wielding() {
	var handX, handY, angle;
	GetCurrentWieldData(handX, handY, angle);
	var addHandX = activeMeleeWeapon->~HandX();
	var addHandY = activeMeleeWeapon->~HandY();
	Rotate(angle, addHandX, addHandY);
	DrawMeleeWeaponOverlay(angle, handX - addHandX, handY - addHandY);
	ScheduleCall(this, "Wielding", 1);
}

private func EndWielding() {
	ClearScheduleCall(this, "Wielding");
	RemoveMeleeWeaponOverlay();
	activeMeleeWeapon = 0;
}

private func NormaliseAngle(int angle) {
	angle %= 360;
	if(angle < 0) {
		angle += 360;
	}
	return angle;
}

private func DrawMeleeWeaponOverlay(int angle, int x, int y) {
	SetGraphics(0, this, activeMeleeWeapon->GetID(), AVTR_WeaponOverlay, GFXOV_MODE_Object, 0, 0, activeMeleeWeapon);
	SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, this, AVTR_WeaponOverlay);
	var sin1 = Sin(360 - angle, 1000);
	var cos1 = Cos(360 - angle, 1000);
	activeMeleeWeapon->SetObjDrawTransform(cos1, sin1, x * 1000, -sin1, cos1, y * 1000);
}

private func RemoveMeleeWeaponOverlay() {
	SetGraphics(0, this, 0, AVTR_WeaponOverlay);
	SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, this, AVTR_WeaponOverlay);
	activeMeleeWeapon->SetObjDrawTransform(1000, 0, 0, 0, 1000, 0);
}

/*	Section: Calls to weapons
	The functions described in this section are called in the weapon object at the appropriate times. */

public func WieldStart(int direction) {}

public func WieldEnd() {
	CallToWeapon("WieldEnd");
	if(activeMeleeWeapon != 0) {
		isRegularActionSwitch = true;

		var coolDown = activeMeleeWeapon->~GetCoolDownDirection();
		if(coolDown == AVMW_WieldUp) SetAction("WieldBackUp");
		else if(coolDown == AVMW_WieldDown) SetAction("WieldBackDown");
		else if(coolDown == AVMW_WieldHold) {
			if(GetAction() == "WieldUp") SetAction("WieldUpHold");
			else if(GetAction() == "WieldDown") SetAction("WieldDownHold");
		} else {
			EndWielding();
			SetAction("Walk"); // TODO: Or set "Jump" when in flight.
			return;
		}
		CoolDownStart();
	}
}

public func WieldAbort() {
	if(!isRegularActionSwitch) {
		CallToWeapon("WieldAbort");
		EndWielding();
	} else {
		isRegularActionSwitch = false;
	}
}

public func CoolDownStart() {
	CallToWeapon("CoolDownStart");
	// For cool down animations, the direction is reversed.
	var oldEndAngle = endAngle;
	endAngle = startAngle;
	startAngle = oldEndAngle;
	angularSpeed *= -1;
}

public func CoolDownEnd() {
	Wielding();
	CallToWeapon("CoolDownEnd");
	EndWielding();
	isRegularActionSwitch = true;
	SetAction("Walk");
}

public func CoolDownAbort() {
	if(!isRegularActionSwitch) {
		CallToWeapon("CoolDownAbort");
		EndWielding();
	} else {
		isRegularActionSwitch = false;
	}
}

private func CallToWeapon(string callName, a) {
	if(activeMeleeWeapon != 0) {
		ObjectCall(activeMeleeWeapon, callName, a);
	}
}
