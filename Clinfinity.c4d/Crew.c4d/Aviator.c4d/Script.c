/*-- Pilot --*/

#strict 2
#include CLNK

/* Itemlimit */
public func MaxContentsCount() { return 3; }

/* Inhalt durchwechseln */
protected func ControlSpecial() { ShiftContents(); }

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
			clonk->AddHat(C4Id(Format("HAT%d", RandomX(1, 5))));
	}
	return _inherited(killedBy, ...);
}

/* Weapons */

// generic functions

// Rifle

// aim radius in degrees
static const AVTR_MinAimAngle = 0;
static const AVTR_MaxAimAngle = 140;
static const AVTR_InitialAimAngle = 84;
static const AVTR_AimStep = 6;

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
	if(rifle->IsRifle()) {
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
			UpdateAimPhase();
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

	// update crosshair placement
	var dir = GetDir() || -1;
	crosshair->SetVertexXY(0, -Sin(aimAngle, 40)*dir, Cos(aimAngle, 40));
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

/*  Function: LoadRifle
	Starts the appropriate rifle loading action when aming and returns to the aim action again.
	
	Calls the function _Load()_ in the active weapon after loading finishes. */
public func LoadRifle() {
	var action = GetAction(), loadingAction;
	if(action == "AimRifle")
		loadingAction = "LoadRifle";
	else if(action == "RideAimRifle")
		loadingAction = "RideLoadRifle";
	if(loadingAction) {
		Sound("RifleLoad");
		SetAction(loadingAction);
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
		activeRifle->Fire(this, aimAngle);
		return 1;
	}
	var obj = Contents();
	if(obj && obj->~IsRifle()) {
		StartAiming(obj);
		return 1;
	}
	return inherited(...);
}

public func ControlCommand(string command, object pClonk, int iX, int iY) {
    if(!IsAiming())
		return inherited(command, pClonk, iX, iY, ...);
    // lädt noch nach
    if(GetAction(pClonk) != "AimRifle" && GetAction(pClonk) != "RideAimRifle")
		return 1;

    // Zielwinkel

    var iAngle = Angle(GetX(pClonk), GetY(pClonk), iX, iY);

    // Bei größerer Distanz höher zielen

    if (Inside(iX - GetX(pClonk), +1, +300))

        iAngle -= Abs(iX - GetX(pClonk)) / 50;

    if (Inside(iX - GetX(pClonk), -300, -1))

        iAngle += Abs(iX - GetX(pClonk)) / 50;

    // zur richtigen Seite hin schießen
    if(iAngle < 180) {
        SetDir(1, pClonk);
        SetPhase(BoundBy(iAngle, 0, 135) / 15, pClonk);
        if(GetAction(pClonk) == "RideAimRifle" && GetDir(GetActionTarget(0, pClonk)) == DIR_Left) GetActionTarget(0, pClonk)->~TurnRight();
    } else {
        SetDir(0, pClonk);
        SetPhase((360 - BoundBy(iAngle, 225, 360)) / 15, pClonk);
        if(GetAction(pClonk) == "RideAimRifle" && GetDir(GetActionTarget(0, pClonk)) == DIR_Right) GetActionTarget(0, pClonk)->~TurnLeft();
    }
    //if(iDevitation) iAngle += RandomX(-iDevitation / 2, iDevitation / 2);
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

protected func ControlDown() {
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

// vim: fdm=marker
