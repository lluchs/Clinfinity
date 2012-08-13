/*	Script: Musket
	A long-range projectile weapon. */

#strict 2

#include L_SS

// overlay position
public func HandX() { return 9000; }
public func HandY() { return 2000; }

// clip size
public func MaxFill() { return 6; }
private func FillPicture() { return 0; }

// minimum and maximum damage
static const MUSK_MinDamage = 5;
static const MUSK_MaxDamage = 20;
static const MUSK_DamageDeviation = 3;

// charging duration in frames
static const MUSK_ChargeDuration = 150;

// minimum charge for knockback
static const MUSK_KnockbackCharge = 50;

// timer interval for charging effect
static const MUSK_ChargeRefreshRate = 5;

// charge progress in percent
local charge, chargeIndicator;

protected func Initialize() {
	DoFill(MaxFill());
}

// collect clip
protected func Entrance(object container) {
	var musket = FindObject2(Find_Container(container), Find_ID(GetID()), Find_Exclude(this));
	if(musket) {
		musket->DoFill(GetFill());
		RemoveObject();
	}
}

/* Charging */
private func StartCharging() {
	StopCharging();
	AddEffect("Charging", this, 1, MUSK_ChargeRefreshRate, this);
	chargeIndicator = Contained()->CreateObject(CRGE, 0, -20, Contained()->GetOwner());
	chargeIndicator->AttachTo(Contained());
}

private func StopCharging() {
	RemoveEffect("Charging", this);
	charge = 0;
	if(chargeIndicator)
		chargeIndicator->RemoveObject();
}

private func CalcDamage() {
	var deviation = RandomX(-MUSK_DamageDeviation, MUSK_DamageDeviation);
	return ChangeRange(charge, 0, 100, MUSK_MinDamage, MUSK_MaxDamage) + deviation;
}

private func ChargeKnockback() {
	return charge >= MUSK_KnockbackCharge;
}

protected func FxChargingTimer(object target, int effectNum, int effectTime) {
	charge += 100 * MUSK_ChargeRefreshRate / MUSK_ChargeDuration;
	if(charge >= 100) {
		charge = 100;
		UpdateChargeIndicator();
		return FX_Execute_Kill;
	}
	UpdateChargeIndicator();
}

private func UpdateChargeIndicator() {
	chargeIndicator->SetCharge(charge);
	if(ChargeKnockback())
		chargeIndicator->On();
}

/* Steuerung */
public func GetTargets() {
	return FindObjects(Find_Distance(1000), Find_Hostile(Contained()->GetOwner()), Find_OCF(OCF_Alive), Find_NoContainer(), Sort_Distance());
}

public func CanLoad() {
	return !IsFull() && MatSysGetTeamFill(Contained()->GetOwner(), METL) >= 1;
}

public func StartLoading() {
	Sound("MusketReload");
	StopCharging();
}

public func Load() {
	if(MatSysDoTeamFill(-1, Contained()->GetOwner(), METL)) {
		DoFill(MaxFill());
	}
	StartCharging();
}

public func StartAiming() {
	Sound("MusketDeploy");
	StartCharging();
}

public func Abort() {
	StopCharging();
}

public func Fire(object clonk, int angle) {
	// cooldown
	if(GetEffect("ReloadRifle", this))
		return;
	
	// enough ammo in clip?
	if(!GetFill()) {
		Sound("MusketEmpty");
		// try to reload
		clonk->LoadRifle();
		return;
	}
	// enough steam?
	var owner = clonk->GetOwner();
	if(MatSysGetTeamFill(owner, STEM) < 5)
		return;
	MatSysDoTeamFill(-5, owner, STEM);
	DoFill(-1);
	
    var obj, obj2, x, y, r, xDir, yDir, rDir, dir, phase;

	var ammo = CreateContents(CSHO);

	// Launch parameter calculation
    dir = clonk->GetDir() * 2 - 1;
    phase = clonk->GetPhase();
    r = angle * dir + 90;
    xDir = (Sin(angle, 22) + 1) * dir;
    yDir = -Cos(angle, 22);
    rDir = 0;

	// See AVTR::WeaponAt
	var dst = 10 + GetDefWidth() - HandX() / 1000;
	x = (Sin(angle, dst)) * dir;
	y = -Cos(angle, dst) - HandY() / 1000;

    // Owner is important for awarding kills
    SetOwner(clonk->GetOwner(), ammo);

    Exit(ammo, AbsX(x + clonk->GetX()), AbsY(y + clonk->GetY()), r, xDir, yDir, rDir);
    ammo->Launch(-1, CalcDamage(), ChargeKnockback());

    // Muzzle flash particle: same position as ammo launch
    CreateParticle("MuzzleFlash", AbsX(x + clonk->GetX()), AbsY(y + clonk->GetY()), xDir, yDir, 35, RGBa(255, 255, 255, 0), clonk);

    Sound("MusketShoot*", 0, clonk);

    Smoke(x, y, 2);
    Smoke(x, y + Random(2), 3);

    CreateParticle("Casing", AbsX(x / 2 + GetX(clonk)), AbsY(y / 2 + GetY(clonk)), -dir * RandomX(1, 5), -RandomX(3, 7), 15, RGBa(250, 140, 80, 0));

	// Cooldown before the next shot can be fired
    AddEffect("ReloadRifle", this, 101, 30);

	// Restart charging.
	StartCharging();
    return 1;
}

/* Objekt ist eine Waffe */
public func IsWeapon () {
    return 1;
}

/* Objekt ist keine Handfeuerwaffe */
public func IsGun () {
    return 0;
}

/* Objekt ist ein Gewehr */
public func IsRifle () {
    return 1;
}

/* Objekt wurde ausgewählt */
public func Selection () {
    Sound("RevolverDraw");
}
