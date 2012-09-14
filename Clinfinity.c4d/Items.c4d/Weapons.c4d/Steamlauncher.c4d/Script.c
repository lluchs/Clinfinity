/*	Script: Steam grenade launcher
	A long-range explosive weapon.
	Shoots ballistically moving steam grenades.

	See <Weapons> for a general reference about weapons. */

#strict 2

#include L_SS

/*	Constants: Material consumption when filling magazine
	SGLR_AmmoMaterial		- Material to use.
	SGLR_AmmoSteamUsage	- Amount of steam to use. */
static const SGLR_AmmoMaterial = METL;
static const SGLR_AmmoSteamUsage = 20;

/*	Constants: Grenade launching
	SGLR_GrenadeExitDistance	- Distance of the muzzle, relative to the offset.
	SGLR_GrenadeExitSpeed		- The grenade's initial speed after being launched.
	SGLR_SteamExitDistance		- Distance where steam exits the muzzle.
	SGLR_SteamExitMinSpeed		- Minimum speed of steam blast effect.
	SGLR_SteamExitMaxSpeed		- Maximum speed of steam blast effect. */
static const SGLR_GrenadeExitDistance = 12;
static const SGLR_GrenadeExitSpeed = 10;
static const SGLR_SteamExitDistance = 20;
static const SGLR_SteamExitMinSpeed = 15;
static const SGLR_SteamExitMaxSpeed = 40;

// overlay position
public func HandX() { return 7000; }
public func HandY() { return 2000; }

public func MaxFill() { return 1; }
private func FillPicture() { return 0; }

/*	Section: Events */

/*	Function: Initialize
	Called when the steam grenade launcher is created.
	It is initialised with a full magazine. */
protected func Initialize() {
	DoFill(MaxFill());
}

/*	Function: Entrance
	Event call when entering an object.
	If the steam grenade launcher is collected by a crew member who is already carrying another steam grenade launcher,
	their ammunition is combined and the collected launcher is removed.

	Parameters:
	into	- The object that the steam grenade launcher entered. */
protected func Entrance(object into) {
	if((into->GetOCF() & OCF_CrewMember) == 0) return;
	var steamlauncher = FindObject2(Find_Container(into), Find_ID(GetID()), Find_Exclude(this));
	if(steamlauncher) {
		steamlauncher->DoFill(GetFill());
		RemoveObject();
	}
}

/*	Function: Load
	Called by a crew member after its loading animation has finished. See <Weapons> for reference.
	If all the required materials and enough steam are available in the material system,
	the launcher's magazine gets filled completely. */
public func Load() {
	if(CanLoad()) {
		MatSysDoTeamFill(-1, Contained()->GetOwner(), SGLR_AmmoMaterial);
		MatSysDoTeamFill(-SGLR_AmmoSteamUsage, Contained()->GetOwner(), STEM);
		DoFill(MaxFill());
	}
}

public func StartLoading() {
	Sound("SteamlauncherReload");
}

public func StartAiming() {
	Sound("MusketDeploy");
}

/*	Function: GetTargets
	Called by a crew member to get a list of possible targets for auto-aiming. See <Weapons> for reference.
	Since auto-aiming is disabled for the steam grenade launcher, this always returns no targets.

	Returns:
	An empty array. */
public func GetTargets() { return []; }

public func Abort() { /* Nothing to do */ }

public func CanLoad() {
	return !IsFull() && MatSysGetTeamFill(Contained()->GetOwner(), SGLR_AmmoMaterial) >= 1 && MatSysGetTeamFill(Contained()->GetOwner(), STEM) >= SGLR_AmmoSteamUsage;
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
	var steamAmount = RandomX(5, 10);
	var steamX = Sin(angle, SGLR_SteamExitDistance * direction);
	var steamY = -Cos(angle, SGLR_SteamExitDistance);
	for(var i = 0; i < steamAmount; i++) {
		BSTE->LaunchSteam(steamX + GetX(), steamY + GetY(), RandomX(SGLR_SteamExitMinSpeed, SGLR_SteamExitMaxSpeed), angle * direction);
	}
	AddEffect("ReloadRifle", this, 101, 30);

	return 1;
}

public func IsWeapon()	{ return 1; }
public func Selection()	{ Sound("RevolverDraw"); }
