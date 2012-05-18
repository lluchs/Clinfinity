/*-- Winchester --*/

#strict 2

/* Steuerung */

public func Load() {}
public func Abort() {}


public func Fire(object pClonk, int iAngle) {
	// cooldown
	if(GetEffect("ReloadRifle", this))
		return;
	
    var pObj, pObj2, iX, iY, iR, iXDir, iYDir, iRDir, iDir, iPhase;

	var ammo = CreateContents(CSHO);

    // Austrittsparameter
    iDir = GetDir(pClonk) * 2 - 1;
    iPhase = GetPhase(pClonk);
    iX = +Sin(iPhase * 19, 14) * iDir;
    iY = -Cos(iPhase * 19, 14) - 2;
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
    Sound("RifleShot", 0, pClonk);
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
