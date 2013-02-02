/*	Script: Aviator
*/

#strict 2
#include CLNK

static const AVTR_WeaponOverlay = 2;

/* Itemlimit */
public func MaxContentsCount() { return 3; }

protected func RejectCollect(id ID, object obj) {
	// Only two flints at any time.
	return obj->~IsFlint() && ObjectCount2(Find_Container(this), Find_Func("IsFlint")) >= 2;
}

/* Inhalt durchwechseln */
protected func ControlSpecial() { ShiftContents(); }

protected func Initialize() {
	AddFinishedOffListener();
	return inherited(...);
}

protected func Death(int killedBy) {
	/* Respawn */
	var tank = FindObject2(Find_ID(STMT), Find_Allied(GetOwner()));
	if(tank) {
		var new = CreateObject(GetID(), 0, 0, GetOwner());
		new->GrabObjectInfo(this);
		new->Enter(tank);
	}

	// try to award a hat
	if(Hostile(GetController(), killedBy)) {
		var clonk = GetCursor(killedBy);
		if(clonk)
			clonk->AddHat(C4Id(Format("HAT%d", RandomX(1, 9))));
	}
	return _inherited(killedBy, ...);
}

/*  Section: Weapons
	Provides functionality for aiming and shooting weapons (previously only rifles).

	Compatible weapons must define the following functions:

	 - IsWeapon
	 - HandX/HandY: Overlay position in 1/1000px
	 - GetTargets: Possible targets for auto aim
	 - CanLoad: Whether the weapon must be loaded
	 - StartLoading: Called when a reload is starting
	 - Load: Called after the loading animation finished
	 - Abort: Called when aiming is aborted
	 - Fire(clonk, angle): Called when the user wants to fire */

// aim radius in degrees
static const AVTR_MinAimAngle = 0;
static const AVTR_MaxAimAngle = 140;
static const AVTR_InitialAimAngle = 84;
static const AVTR_AimStep = 20;

local activeRifle, crosshair, aimAngle;

/*  Function: CanUseRifle
	The Aviator can use rifles.

	Returns:
	*true* */
public func CanUseRifle() { return true; }

/*  Function: StartAiming
	Sets the appropriate aiming action, allowing the player to aim and fire the weapon.

	Parameters:
	rifle - the rifle which will receive the _Load()_, _Fire()_ and _Abort()_ events */
public func StartAiming(object rifle) {
	var action = GetAction();
	if(rifle->IsWeapon()) {
		var aimAction;
		if(action == "Walk")
			aimAction = "AimRifle";
		else if(action == "Ride" || action == "RideStil")
			aimAction = "RideAimRifle";
		if(aimAction) {
			activeRifle = rifle;
			aimAngle = AVTR_InitialAimAngle;
			SetAction(aimAction);
			CreateCrosshair();
			DrawWeaponOverlay();
			UpdateAimPhase();
			rifle->~StartAiming(this);
			return true;
		}
	}
}

// creates a crosshair for better aiming
private func CreateCrosshair() {
	crosshair = CreateObject(WCHR, 0, 0, GetOwner());
	crosshair->SetAction("Crosshair", this);
}

// Updates the Aviator's aim phase to the current aimAngle
private func UpdateAimPhase() {
    if(aimAngle < 6)        SetPhase(0);
    else if(aimAngle < 24)  SetPhase(1);
    else if(aimAngle < 35)  SetPhase(2);
    else if(aimAngle < 48)  SetPhase(3);
    else if(aimAngle < 54)  SetPhase(4);
    else if(aimAngle < 72)  SetPhase(5);
    else if(aimAngle < 90)  SetPhase(6);
    else if(aimAngle < 108) SetPhase(7);
    else if(aimAngle < 128) SetPhase(8);
    else                    SetPhase(9);

	AdjustWeaponOverlay();

	// update crosshair placement
	var dir = GetDir() || -1;
	crosshair->SetVertexXY(0, -Sin(aimAngle, 40)*dir, Cos(aimAngle, 40) + activeRifle->~HandY() / 1000);
}

// Draws the weapon overlay on top of the aiming aviator.
// Adjustments are automatically done when the aim angle changes.
private func DrawWeaponOverlay() {
	SetGraphics(0, this, activeRifle->GetID(), AVTR_WeaponOverlay, GFXOV_MODE_Object, 0, 0, activeRifle);
	//SetGraphics(0, this, activeRifle->GetID(), AVTR_WeaponOverlay, GFXOV_MODE_Base);
}

// Calculates weapon position and angle.
private func WeaponAt(&x, &y, &r) {
	if(IsReloading())
		r = 45;
	else
		r = aimAngle - 90;
	x = -Sin(aimAngle, 10);
	y = Cos(aimAngle, 10);
}

// Adjusts the weapon overlay on top of the aviator depending on the current aiming angle.
// Stolen from Hazard (Items.c4d/Weapons.c4d/Weapon.c4d/Script.c)
private func AdjustWeaponOverlay() {
	// Variablen für die Transformation

	var width, height;  // Breiten- und Höhenverzerrung der Waffe
	var xskew, yskew;   // Zerrung der Waffe, wird zur Rotation gebraucht
	var size;           // Größe der Waffe in der Hand: 1000 = 100%
	// Variablen für die Position
	var xaim, yaim;     // Offset, dass sich durch zielen ergibt
	var dir;            // Richtung in die das Objekt schaut

	var xoff, yoff, r;
	WeaponAt(xoff, yoff, r);

	// Variablen mit Werten versehen
	width = height = xskew = yskew = 1;
	size = 1000;
	dir  = GetDir()*2-1;
	if(r > 180 || r < -180)
		dir *= -1;
	r *= dir;

	var xfact = size * activeRifle->~HandX();
	var yfact = size * activeRifle->~HandY();

	xoff += Cos(r,xfact)/1000 + dir*Sin(r,yfact)/1000;
	yoff -= Cos(r,yfact)/1000 - dir*Sin(r,xfact)/1000;

	if(dir == 1) {
		height = -1;
		xskew = -1;
		yskew = -1;
	}

	r = -90*dir-r-90;
	height *= width *= Cos(r, size);
	xskew *= Sin(r, size);
	yskew *= -xskew;
	xoff *= dir;

	SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, 0, AVTR_WeaponOverlay); //position
	activeRifle->SetObjDrawTransform(width,xskew,0,yskew,height); //Größe und Rotation

}

// Removes the weapon overlay created by DrawWeaponOverlay().
private func RemoveWeaponOverlay() {
	SetGraphics(0, this, 0, AVTR_WeaponOverlay);
}

/*  Function: IsAiming
	
	Returns:
	Whether the Aviator is currently aiming his rifle. */
public func IsAiming() {
	var action = GetAction();
	return action == "AimRifle" || action == "RideAimRifle";
}

/*  Function: IsReloading
	
	Returns:
	Whether the Aviator is currently reloading his gun. */
public func IsReloading() {
	var action = GetAction();
	return action == "LoadRifle" || action == "RideLoadRifle";
}

/*  Function: DoAim
	Changes the aim by *angle* degrees.

	Parameters:
	angle - How much the aim should be changed. Negative -> up, positive -> down. */
public func DoAim(int angle) {
	aimAngle = BoundBy(aimAngle + angle, AVTR_MinAimAngle, AVTR_MaxAimAngle);
	UpdateAimPhase();
}

/*  Function: AutoAim
	Adjusts the current aim by searching for potential targets.
	
	Calls _GetTargets()_ in the gun to get a selection of potential targets. */
public func AutoAim() {
	if(!IsAiming())
		return;
	var x = GetX(), y = GetY(), tx, ty;
	var targets = activeRifle->GetTargets(), shotSpeed = activeRifle->~ShotSpeed();
	for(var target in targets) {
		tx = target->GetX();
		ty = target->GetY();
		// improve aiming for moving targets
		if(shotSpeed && !GetContact(target, -1)) {
			var distance = Distance(x, y, tx, ty);
			var time = distance * 10 / shotSpeed;
			var xy = target->SimulateFlight(time);
			tx = xy[0];
			ty = xy[1];
		}
		// check angle
		var angle = Angle(x, y, tx, ty);
		if(GetDir() == DIR_Left)
			angle = 360 - angle;
		// roughly in direction of current aim and nothing solid inbetween?
		if(Inside(angle, AVTR_MinAimAngle, AVTR_MaxAimAngle)
				&& Inside(angle, aimAngle - AVTR_AimStep, aimAngle + AVTR_AimStep)
				&& PathFree(x, y, tx, ty)) {
			// target found!
			// aim at target
			aimAngle = angle;
			UpdateAimPhase();
			return true;
		}
	}
}

/*  Function: LoadRifle
	Starts the appropriate rifle loading action when aming and returns to the aim action again.
	
	Calls the function _Load()_ in the active weapon after loading finishes. */
public func LoadRifle() {
	var action = GetAction(), loadingAction;
	if(action == "AimRifle")
		loadingAction = "LoadRifle";
	else if(action == "RideAimRifle")
		loadingAction = "RideLoadRifle";
	if(loadingAction && activeRifle->CanLoad()) {
		activeRifle->~StartLoading();
		SetAction(loadingAction);
		AdjustWeaponOverlay();
	} else {
		Sound("CommandFailure1");
	}
}

/*  Function: AbortAiming
	Stops aiming, returning to the 'Walk' action if possible.

	This function is also automatically called when the Aviator is hit by something while aiming. */
public func AbortAiming() {
	// don't do anything if just for reloading
	if(IsReloading())
		return;

	if(IsAiming())
		SetAction("Walk");
	
	RemoveWeaponOverlay();

	if(crosshair) {
		crosshair->RemoveObject();
		activeRifle->Abort();
		activeRifle = 0;
	}
}

/* Control and Action calls {{{ */

protected func AimAgain() {
	activeRifle->Load();
	SetAction("AimRifle");
	UpdateAimPhase();
}

protected func AimAgainRide() {
	activeRifle->Load();
	SetAction("RideAimRifle");
	UpdateAimPhase();
}

protected func ControlThrow() {
	if(IsAiming()) {
		AutoAim();
		activeRifle->Fire(this, aimAngle);
		return 1;
	}
	var obj = Contents();
	if(obj != 0) {
		if(obj->~IsWeapon()) {
			if(GetPlrDownDouble(GetOwner()))
				return 0;
			else {
				StartAiming(obj);
				return 1;
			}
		}
		if(obj->~IsMeleeWeapon()) {
			if(GetPlrDownDouble(GetOwner())) {
				if(!IsWielding(Contents(0))) return 0;
				else return 1;
			}
			if(ReadyToWield()) {
				WieldMeleeWeapon();
				return 1;
			}
		}
		if(YOYO->IsYoyoThrownBy(this)) {
			return 1;
		}
	}
	return inherited(...);
}

public func ControlCommand(string command, object pClonk, int iX, int iY) {
	if(!IsAiming())
		return inherited(command, pClonk, iX, iY, ...);
	if(GetAction(pClonk) != "AimRifle" && GetAction(pClonk) != "RideAimRifle")
		return 1;

	var iAngle = Angle(GetX(pClonk), GetY(pClonk), iX, iY);

	// Shoot into correct direction
	if(iAngle < 180) {
		SetDir(1, pClonk);
		SetPhase(BoundBy(iAngle, 0, 135) / 15, pClonk);
		if(GetAction(pClonk) == "RideAimRifle" && GetDir(GetActionTarget(0, pClonk)) == DIR_Left) GetActionTarget(0, pClonk)->~TurnRight();
	} else {
		SetDir(0, pClonk);
		SetPhase((360 - BoundBy(iAngle, 225, 360)) / 15, pClonk);
		if(GetAction(pClonk) == "RideAimRifle" && GetDir(GetActionTarget(0, pClonk)) == DIR_Right) GetActionTarget(0, pClonk)->~TurnLeft();
	}
	if(iAngle > 180) iAngle = -iAngle + 360;

	aimAngle = iAngle;
	UpdateAimPhase();

	activeRifle->Fire(this, iAngle);

	return 1;
}

protected func ControlDig() {
	if(IsAiming()) {
		LoadRifle();
		return 1;
	}
	return inherited(...);
}

protected func ControlUp() {
	if(IsAiming()) {
		DoAim(-AVTR_AimStep);
		return 1;
	}
	return inherited(...);
}

protected func ControlUpDouble() {
	if(IsAiming()) {
		DoAim(-AVTR_AimStep);
		return 1;
	}
	return inherited(...);
}

protected func ControlDown() {
	if(IsAiming()) {
		DoAim(AVTR_AimStep);
		return 1;
	}
	return inherited(...);
}

protected func ControlDownDouble() {
	if(IsAiming()) {
		DoAim(AVTR_AimStep);
		return 1;
	}
	return inherited(...);
}

protected func ControlLeft() {
	if(IsAiming()) {
		SetDir(DIR_Left);
		UpdateAimPhase();
		return 1;
	}
	return inherited(...);
}

protected func ControlRight() {
	if(IsAiming()) {
		SetDir(DIR_Right);
		UpdateAimPhase();
		return 1;
	}
	return inherited(...);
}

protected func ControlLeftDouble() {
	if(IsAiming()) {
		AbortAiming();
		return 1;
	}
	return inherited(...);
}

protected func ControlRightDouble() {
	if(IsAiming()) {
		AbortAiming();
		return 1;
	}
	return inherited(...);
}


// stubs for Gun Action calls (not implemented yet)
protected func DrawingGun() {}
protected func FiringGun() {}
protected func AbortJumpDrawGun() {}
protected func AbortJumpReplaceGun() {}
protected func LoadGunEnd() {}

/* Control functions end }}} */

public func GetResistance(int amount, object weapon) {
	var total = 0;
	var contents = FindObjects(Find_Container(this));
	for(var item in contents) {
		total += item->~GetResistance(amount, weapon);
	}
	return total;
}

// vim: fdm=marker
