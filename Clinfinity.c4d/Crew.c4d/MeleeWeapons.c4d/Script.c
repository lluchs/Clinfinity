/*	Script: Melee weapons
	Functionality for using melee weapons.

	Melee weapons are short range weapons wielded with both hands.
	Therefore, only one melee weapon can be used at a time.
	Additionally, some other actions, such as throwing, using other types of weapons, etc, are also disabled while wielding.
	
	Wielding:
	Wielding a melee weapon consists of two phases:
	- The actual wielding
	- A cool down, which is optional

	For both phases, several weapon movement directions are available.
	See <Wield directions> and <Cool down directions> for details.

	Animation:
	The aviator provides the appropriate actions for the wielding and cool down phases,
	with the weapon automatically rendered into the aviator's hands.
	A weapon can determine its appearance in the aviator's hands by setting an action while being wielded.
	Otherwise, the weapon's default graphics is used.

	Events:
	To make animation, weapon effects, etc. as easy as possible,
	various functions are invoked on the weapon object at different times.
	See <Calls to weapons> for details.

	Melee weapon objects:
	Melee weapon objects must implement the following functions: 
	- IsMeleeWeapon()		- Return _true_ here.
	- GetWieldDirection()	- Specifies the wield direction.
							  Return one of the defined <Wield directions> here.
	- GetStartAngle()		- Angle in which the weapon starts when wielding.
	- GetCentralAngle()		- Angle and direction to wield the weapon.

	Beyond minimal functionality,
	the following functions may be implemented:
	- GetCoolDownDirection()	- Specifies the movement during the cool down phase.
								  Implement this function to provide cool down for a weapon.
								  Return one of the defined <Cool down directions> here. */

#strict 2

/*	Constants: Wield directions
	AVMW_WieldUp	- Direction: Wield weapon upwards.
	AVMW_WieldDown	- Direction: Wield weapon downwards. */
static const AVMW_WieldUp	= 1;
static const AVMW_WieldDown	= 2;

/*	Constants: Cool down directions
	AVMW_WieldReverse	- Direction: Wield weapon back to the initial angle.
	AVMW_WieldHold		- Direction: Hold weapon in its final position for a short time span. */
static const AVMW_WieldReverse	= 3;
static const AVMW_WieldHold		= 4;

/* Constants for internal use only, therefore not publicly documented. */
static const AVMW_WieldDownHandX = 0;
static const AVMW_WieldDownHandY = -5;
static const AVMW_WieldUpHandX = 0;
static const AVMW_WieldUpHandY = -3;

static const AVMW_WieldDownShoulderY	= -1;
static const AVMW_WieldUpShoulderY		= 1;

local activeMeleeWeapon, startAngle, angularSpeed, endAngle;
local isRegularActionSwitch;

/*	Function: ReadyToWield
	Returns if the Clonk is currently ready to start wielding a melee weapon.

	Returns:
	_true_ if ready, _false_ otherwise. */
public func ReadyToWield() {
	// Can't use more than one melee weapon
	if(YOYO->IsYoyoThrownBy(this)) return false;
	if(IsGliding()) return false;
	return GetAction() == "Walk" || GetAction() == "Jump";
}

private func IsGliding() {
	return _inherited();
}

/*	Function: IsWielding
	Returns if the Clonk is currently wielding a melee weapon.
	If a weapon is passed as parameter, _IsWielding()_ checks if this weapon is wielded.
	On the other hand, if no weapon is passed, the function checks if any weapon is wielded.

	Parameters:
	weapon	- [optional] Weapon to check if it is wielded. May be omitted for checking for any weapon.

	Returns:
	_true_ if a weapon is wielded, _false_ otherwise. */
public func IsWielding(object weapon) {
	if(weapon == 0) return activeMeleeWeapon != 0;
	else return activeMeleeWeapon == weapon;
}

/*	Function: GetCurrentWieldData
	Function to access the current position and angle of the hands while wielding a weapon.

	Parameters:
	handX		- Horizontal hand coordinate.
	handY		- Vertical hand coordinate.
	weaponAngle	- Hand angle. */
public func GetCurrentWieldData(&handX, &handY, &weaponAngle) {
	if(activeMeleeWeapon != 0) {
		var shoulderY;
		if(WildcardMatch(GetAction(), "WieldDown*") || WildcardMatch(GetAction(), "WieldBackUp*")) {
			shoulderY = AVMW_WieldDownShoulderY;
			handX = AVMW_WieldDownHandX;
			handY = AVMW_WieldDownHandY;
		} else {
			shoulderY = AVMW_WieldUpShoulderY;
			handX = AVMW_WieldUpHandX;
			handY = AVMW_WieldUpHandY;
		}

		var frames = GetActMapVal("Length", GetAction());
		var delay = GetActMapVal("Delay", GetAction());
		var duration = frames * delay;
		if(WildcardMatch(GetAction(), "Wield*Hold*")) {
			weaponAngle = startAngle;
		} else if(GetActTime() >= duration) {
			weaponAngle = endAngle;
		} else {
			weaponAngle = startAngle + GetActTime() * angularSpeed;
		}

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

private func NormaliseAngle(int angle) {
	angle %= 360;
	if(angle < 0) {
		angle += 360;
	}
	return angle;
}

private func Wielding() {
	if(activeMeleeWeapon == 0) EndWielding();
	var handX, handY, angle;
	GetCurrentWieldData(handX, handY, angle);
	var addHandX = activeMeleeWeapon->~HandX();
	var addHandY = activeMeleeWeapon->~HandY();
	Rotate(angle, addHandX, addHandY);
	DrawMeleeWeaponOverlay(angle, handX - addHandX, handY - addHandY);
	ScheduleCall(this, "Wielding", 1);
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

// Action calls: Initial wielding phase

private func WieldDownEnd() {
	CallToWeapon("WieldEnd");
	ChooseCoolDownActionFrom("Walk", "WieldBackUp", "WieldDownHold");
}

private func WieldUpEnd() {
	CallToWeapon("WieldEnd");
	ChooseCoolDownActionFrom("Walk", "WieldBackDown", "WieldUpHold");
}

private func WieldDownJumpEnd() {
	CallToWeapon("WieldEnd");
	ChooseCoolDownActionFrom("Jump", "WieldBackUpJump", "WieldDownHoldJump");
}

private func WieldUpJumpEnd() {
	CallToWeapon("WieldEnd");
	ChooseCoolDownActionFrom("Jump", "WieldBackDownJump", "WieldUpHoldJump");
}

private func ChooseCoolDownActionFrom(string standardAction, string reverseAction, string holdAction) {
	if(activeMeleeWeapon != 0) {
		isRegularActionSwitch = true;
		var coolDown = activeMeleeWeapon->~GetCoolDownDirection();
		if(coolDown == AVMW_WieldReverse) SetAction(reverseAction);
		else if(coolDown == AVMW_WieldHold) SetAction(holdAction);
		else {
			EndWielding();
			SetAction(standardAction);
			return;
		}
		StartCoolDown();
	} else {
		EndWielding();
		SetAction(standardAction);
	}
}

private func WieldAnyAbort() {
	if(!isRegularActionSwitch) {
		CallToWeapon("WieldAbort");
		EndWielding();
	} else {
		isRegularActionSwitch = false;
	}
}

private func EndWielding() {
	ClearScheduleCall(this, "Wielding");
	RemoveMeleeWeaponOverlay();
	activeMeleeWeapon = 0;
}

private func StartCoolDown() {
	CallToWeapon("CoolDownStart");
	// For cool down animations, the direction is reversed.
	var oldEndAngle = endAngle;
	endAngle = startAngle;
	startAngle = oldEndAngle;
	angularSpeed *= -1;
}

// Action calls: Cool down phase

private func CoolDownAnyEnd() {
	EndCoolDown("Walk");
}

private func CoolDownAnyJumpEnd() {
	EndCoolDown("Jump");
}

private func CoolDownAnyAbort() {
	if(!isRegularActionSwitch) {
		CallToWeapon("CoolDownAbort");
		EndWielding();
	} else {
		isRegularActionSwitch = false;
	}
}

private func EndCoolDown(string nextAction) {
	CallToWeapon("CoolDownEnd");
	EndWielding();
	isRegularActionSwitch = true;
	SetAction(nextAction);	
}

// Utility functions for action calls

private func CallToWeapon(string callName, a) {
	if(activeMeleeWeapon != 0) {
		ObjectCall(activeMeleeWeapon, callName, a);
	}
}

/*	Section: Calls to weapons
	The functions described in this section are invoked on the weapon object at the appropriate times. */

/*	Function: WieldStart
	Called when a Clonk starts wielding the weapon.

	Parameters:
	direction	- The direction the Clonk looks in. */
public func WieldStart(int direction) {}

/*	Function: WieldEnd
	Called when a Clonk successfully finished wielding the weapon.
	This function is invoked regardless of a cool down following or not. */
public func WieldEnd() {}

/*	Function: WieldAbort
	Called when wielding was aborted before finished. */
public func WieldAbort() {}

/*	Function: CoolDownStart
	Called when the cool down phase starts. */
public func CoolDownStart() {}

/*	Function: CoolDownEnd
	Called when the cool down phase was successfully finished. */
public func CoolDownEnd() {}

/*	Function: CoolDownAbort
	Called when the cool down phase was aborted before finished. */
public func CoolDownAbort() {}
