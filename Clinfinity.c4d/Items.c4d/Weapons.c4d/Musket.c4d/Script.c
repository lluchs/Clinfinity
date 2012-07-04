/*-- Winchester --*/

#strict 2

#include L_SS

// clip size
public func MaxFill() { return 6; }
private func FillPicture() { return 0; }

// collect clip
protected func Entrance(object container) {
	var musket = FindObject2(Find_Container(container), Find_ID(GetID()), Find_Exclude(this));
	if(musket) {
		musket->DoFill(GetFill());
		RemoveObject();
	}
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
}

public func Load() {
	if(MatSysDoTeamFill(-1, Contained()->GetOwner(), METL)) {
		DoFill(6);
	}
}

public func StartAiming() {
	Sound("MusketDeploy");
}

public func Abort() {}

public func Fire(object pClonk, int iAngle) {
	// cooldown
	if(GetEffect("ReloadRifle", this))
		return;
	
	// enough ammo in clip?
	if(!GetFill()) {
		Sound("MusketEmpty");
		// try to reload
		pClonk->LoadRifle();
		return;
	}
	// enough steam?
	var owner = pClonk->GetOwner();
	if(MatSysGetTeamFill(owner, STEM) < 5)
		return;
	MatSysDoTeamFill(-5, owner, STEM);
	DoFill(-1);
	
    var pObj, pObj2, iX, iY, iR, iXDir, iYDir, iRDir, iDir, iPhase;

	var ammo = CreateContents(CSHO);

    // Austrittsparameter
    iDir = GetDir(pClonk) * 2 - 1;
    iPhase = GetPhase(pClonk);
    //iX = +Sin(iPhase * 19, 14) * iDir;
    //iY = -Cos(iPhase * 19, 14) - 2;
    iR = iAngle * iDir + 90;
    iXDir = (Sin(iAngle, 22) + 1) * iDir;
    iYDir = -Cos(iAngle, 22);
    iRDir = 0;

    // Besitzer des Projektils setzen
    SetOwner( GetOwner(pClonk), ammo );

    // Abfeuern
    Exit(ammo, AbsX(iX + GetX(pClonk)), AbsY(iY + GetY(pClonk)), iR, iXDir, iYDir, iRDir);
    ammo->Launch(-1, 10 + Random(10));

    // Mündungsfeuer
    // hax, weil die Animation nicht genauen Winkeln entspricht und der Partikel seltsam verdreht wird
    if(iDir == DIR_Left) {
        if(Inside(iPhase, 0, 1)) iX = +Sin(iPhase * 15, 15) * iDir + 5;
        else    iX = +Sin(iPhase * 15, 15) * iDir;
    } else  iX = +Sin(iPhase * 14, 15) * iDir;
    if(Inside(iPhase, 0, 7)) iY = -Cos(iPhase * 15, 14) - 6;
    else  iY = -Cos(iPhase * 15, 14) - 3;
    CreateParticle("MuzzleFlash", AbsX(iX + GetX(pClonk)), AbsY(iY + GetY(pClonk)), iXDir, iYDir, 35, RGBa(255, 255, 255, 0), pClonk);
    // Sound
    Sound("MusketShoot*", 0, pClonk);
    // Rauch
    Smoke(iX, iY, 2);
    Smoke(iX, iY + Random(2), 3);
    // Patronenhülse fliegt raus
    CreateParticle("Casing", AbsX(iX / 2 + GetX(pClonk)), AbsY(iY / 2 + GetY(pClonk)), -iDir * RandomX(1, 5), -RandomX(3, 7), 15, RGBa(250, 140, 80, 0));
    // Der Clonk muss eine Kugel einladen
    AddEffect("ReloadRifle", this, 101, 30);
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
