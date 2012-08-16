/*	Script: Steam launcher
	A long-range explosive weapon. */

#strict 2

#include L_SS

static const SGLR_AmmoMaterial = METL;
static const SGLR_ShootSteamUsage = 20;
static const SGLR_GrenadeExitDistance = 12;
static const SGLR_GrenadeExitSpeed = 10;

static const SGLR_MinDamage = 5;
static const SGLR_MaxDamage = 20;
static const SGLR_DamageDeviation = 3;

// overlay position
public func HandX() { return 7000; }
public func HandY() { return 2000; }

/*	Function: MaxFill
	Returns the clip size of the steam launcher. */
public func MaxFill() { return 1; }
private func FillPicture() { return 0; }

/*	Section: Events */

/*	Function: Initialize
	Called when the steam launcher is created.
	It is initialised with a full magazine. */
protected func Initialize() {
	DoFill(MaxFill());
}

/*	Function: Entrance
	Event call when entering an object.
	If the steam launcher is collected by a crew member who is already carrying another steam launcher,
	their ammunition is combined and the collected steam launcher is removed.

	Parameters:
	into	- The object that the steam launcher entered. */
protected func Entrance(object into) {
	if((into->GetOCF() & OCF_CrewMember) == 0) return;
	var steamlauncher = FindObject2(Find_Container(into), Find_ID(GetID()), Find_Exclude(this));
	if(steamlauncher) {
		steamlauncher->DoFill(GetFill());
		RemoveObject();
	}
}

/*	Function: Load
	Called by a crew member after its loading animation has finished. */
public func Load() {
	if(CanLoad()) {
		MatSysDoTeamFill(-1, Contained()->GetOwner(), SGLR_AmmoMaterial);
		MatSysDoTeamFill(-SGLR_ShootSteamUsage, Contained()->GetOwner(), STEM);
		DoFill(MaxFill());
	}
}

public func StartLoading() {
	Sound("SteamlauncherReload");
}

public func StartAiming() {
	Sound("MusketDeploy");
}

public func GetTargets() { /* No auto-aiming */ return []; }

public func Abort() { /* Nothing to do */ }

/*	Section: Controls */

public func CanLoad() {
	return !IsFull() && MatSysGetTeamFill(Contained()->GetOwner(), SGLR_AmmoMaterial) >= 1 && MatSysGetTeamFill(Contained()->GetOwner(), STEM) >= SGLR_ShootSteamUsage;
}

public func Fire(object clonk, int angle) {
	// Cool down
	if(GetEffect("ReloadRifle", this))
		return;
	
	// Ammo in clip?
	if(!GetFill()) {
		Sound("MusketEmpty");
		// Try to reload
		clonk->LoadRifle();
		return;
	}
	DoFill(-1);

	var direction = clonk->GetDir() * 2 - 1;
	var phase = clonk->GetPhase();
	var x = Sin(angle, SGLR_GrenadeExitDistance * direction);
	var y = -Cos(angle, SGLR_GrenadeExitDistance) + HandY() / 1000;
	var xSpeed = Sin(angle, SGLR_GrenadeExitSpeed * direction);
	var ySpeed = -Cos(angle, SGLR_GrenadeExitSpeed);

	var grenade = CreateContents(BOMB);
	grenade->SetOwner(clonk->GetOwner());
	Exit(grenade, x, y, 0, xSpeed, ySpeed, 0);
	grenade->Launch();

	Sound("SteamlauncherShoot*", 0, clonk);

	AddEffect("ReloadRifle", this, 101, 30);

	return 1;
}

public func IsWeapon()	{ return 1; }
public func Selection()	{ Sound("RevolverDraw"); }
