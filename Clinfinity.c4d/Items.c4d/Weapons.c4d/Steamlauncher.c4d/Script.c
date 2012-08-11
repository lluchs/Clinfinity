/*	Script: Steam launcher
	A long-range explosive weapon. */

#strict 2

#include L_SS

static const SGLR_AmmoMaterial = METL;
static const SGLR_ShootSteamUsage = 10;
static const SGLR_GrenadeExitSpeed = 30;

static const SGLR_MinDamage = 5;
static const SGLR_MaxDamage = 20;
static const SGLR_DamageDeviation = 3;

/*	Function: MaxFill
	Returns the clip size of the steam launcher. */
public func MaxFill() { return 1; }
private func FillPicture() { return 0; }

/*	Section: Events */

/*	Function: Initialize
	Called when the steam launcher is created.
	It is created with a full magazine. */
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
	if(MatSysDoTeamFill(-1, Contained()->GetOwner(), SGLR_AmmoMaterial)) {
		DoFill(MaxFill());
	}
}

public func StartLoading() {
	Sound("SteamlauncherReload");
}

public func StartAiming() {
	Sound("MusketDeploy");
}

public func GetTargets() { return []; }

public func Abort() {/* :'( */}

/*	Section: Controls */

public func CanLoad() {
	return !IsFull() && MatSysGetTeamFill(Contained()->GetOwner(), SGLR_AmmoMaterial) >= 1;
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
	// Enough steam?
	var owner = clonk->GetOwner();
	if(MatSysGetTeamFill(owner, STEM) < SGLR_ShootSteamUsage)
		return;
	MatSysDoTeamFill(-SGLR_ShootSteamUsage, owner, STEM);
	DoFill(-1);
	
	var grenade = CreateContents(METL);

    var direction = clonk->GetDir() * 2 - 1;
    var phase = clonk->GetPhase();
    var xSpeed = Sin(angle, SGLR_GrenadeExitSpeed) * direction;
    var ySpeed = -Cos(angle, SGLR_GrenadeExitSpeed);


    grenade->SetOwner(clonk->GetOwner());

    Exit(grenade, xSpeed, ySpeed, 0, xSpeed, ySpeed, 0);
    //ammo->Launch(-1, CalcDamage(), ChargeKnockback());

    // Muzzle flash
    // hax, weil die Animation nicht genauen Winkeln entspricht und der Partikel seltsam verdreht wird
    /*
    if(iDir == DIR_Left) {
        if(Inside(iPhase, 0, 1)) iX = +Sin(iPhase * 15, 15) * iDir + 5;
        else    iX = +Sin(iPhase * 15, 15) * iDir;
    } else  iX = +Sin(iPhase * 14, 15) * iDir;
    if(Inside(iPhase, 0, 7)) iY = -Cos(iPhase * 15, 14) - 6;
    else  iY = -Cos(iPhase * 15, 14) - 3;
    CreateParticle("MuzzleFlash", AbsX(iX + GetX(clonk)), AbsY(iY + GetY(clonk)), iXDir, iYDir, 35, RGBa(255, 255, 255, 0), clonk);
    */

    Sound("SteamlauncherShoot*", 0, clonk);
    //Smoke(iX, iY, 2);
    //Smoke(iX, iY + Random(2), 3);
    //CreateParticle("Casing", AbsX(iX / 2 + GetX(clonk)), AbsY(iY / 2 + GetY(clonk)), -iDir * RandomX(1, 5), -RandomX(3, 7), 15, RGBa(250, 140, 80, 0));
    AddEffect("ReloadRifle", this, 101, 30);

    return 1;
}


public func IsWeapon()	{ return 1; }
public func IsRifle()	{ return 1; }
public func Selection()	{ Sound("RevolverDraw"); }
