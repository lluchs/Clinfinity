/*	Script: Melee weapons
	Functionality for using melee weapons. */

#strict 2

static const AVMW_WieldUp	= 1;
static const AVMW_WieldDown	= 2;
static const AVMW_WieldHold	= 3;

static const AVMW_StandardHandX = 0;
static const AVMW_StandardHandY = -5;

local activeMeleeWeapon, startAngle, angularSpeed, endAngle;
local isRegularActionSwitch;

public func ReadyToWield() {
	// Can't use more than one melee weapon
	if(YOYO->IsYoyoThrownBy(this)) return false;
	return GetAction() == "Walk" /*|| GetAction() == "Jump"*/;
}

public func GetCurrentWieldData(&handX, &handY, &weaponAngle) {
	if(activeMeleeWeapon != 0) {
		var frames = GetActMapVal("Length", GetAction());
		var delay = GetActMapVal("Delay", GetAction());
		var duration = frames * delay;
		if(GetActTime() >= duration)
			weaponAngle = endAngle;
		else
			weaponAngle = startAngle + GetActTime() * angularSpeed;
		handX = AVMW_StandardHandX;
		handY = AVMW_StandardHandY;
		Rotate(weaponAngle, handX, handY);
	}
}

private func WieldMeleeWeapon() {
	var weapon = Contents(0);
	if(weapon != 0 && weapon->~IsMeleeWeapon()) {
		var wieldDirection = weapon->GetWieldDirection();
		if(wieldDirection == 0) return;

		if(wieldDirection == AVMW_WieldUp) {
			if(GetAction() == "Walk") {
				SetAction("WieldUp");
			} else {
				// TODO: Jump+wield up animation
				return;
			}
		} else if(wieldDirection == AVMW_WieldDown) {
			if(GetAction() == "Walk") {
				SetAction("WieldDown");
			} else {
				// TODO: Jump+wield down animation
				return;
			}
		}
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
	var x, y, angle;
	GetCurrentWieldData(x, y, angle);

	var x = AVMW_StandardHandX + activeMeleeWeapon->~HandX();
	if(GetDir() == DIR_Left) x = -x;
	var y = AVMW_StandardHandY - activeMeleeWeapon->~HandY();

	DrawMeleeWeaponOverlay(angle, x, y, 0, -1);

	ScheduleCall(this, "Wielding", 1);
}

private func NormaliseAngle(int angle) {
	angle %= 360;
	if(angle < 0) {
		angle += 360;
	}
	return angle;
}

private func DrawMeleeWeaponOverlay(int angle, int x, int y, int xOffset, int yOffset) {
	SetGraphics(0, this, activeMeleeWeapon->GetID(), AVTR_WeaponOverlay, GFXOV_MODE_Object, 0, 0, activeMeleeWeapon);
	SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, this, AVTR_WeaponOverlay);
	Rotate(angle, x, y, xOffset, yOffset);
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
	Wielding();
	ClearScheduleCall(this, "Wielding");
	//DrawRotated(0, 0, 0, 0, 0);
	//RemoveMeleeWeaponOverlay();
	var weapon = Contents(0);
	if(weapon != 0 && weapon->~IsMeleeWeapon()) {
		isRegularActionSwitch = true;

		var coolDown = weapon->~GetCoolDownDirection();
		if(coolDown == AVMW_WieldUp) {
			SetAction("WieldBackUp");
		} else if(coolDown == AVMW_WieldDown) {
			SetAction("WieldBackDown");
		} else if(coolDown == AVMW_WieldHold) {
			if(GetAction() == "WieldUp") SetAction("WieldUpHold");
			if(GetAction() == "WieldDown") SetAction("WieldDownHold");
		} else {
			RemoveMeleeWeaponOverlay();
			SetAction("Walk");
		}
	}
}

public func WieldAbort() {
	if(!isRegularActionSwitch) {
		CallToWeapon("WieldAbort");
		ClearScheduleCall(this, "Wielding");
		RemoveMeleeWeaponOverlay();
	} else {
		isRegularActionSwitch = false;
	}
}

public func CoolDownEnd() {
	CallToWeapon("CoolDownEnd");
	RemoveMeleeWeaponOverlay();
	isRegularActionSwitch = true;
	SetAction("Walk");
}

public func CoolDownAbort() {
	if(!isRegularActionSwitch) {
		CallToWeapon("CoolDownAbort");
		RemoveMeleeWeaponOverlay();
	} else {
		isRegularActionSwitch = false;
	}
}

private func CallToWeapon(string callName, a) {
	var weapon = Contents(0);
	if(weapon != 0 && weapon->~IsMeleeWeapon()) {
		ObjectCall(weapon, callName, a);
	}
}
