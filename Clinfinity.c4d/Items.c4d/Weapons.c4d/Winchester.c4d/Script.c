/*-- Winchester --*/

#strict 2

/* Steuerung */

public func ControlThrow (pClonk) {          // Bedienung: Werfen (benutzen)
    if(!(pClonk->~FireRifle())) {
        if(!GetPlrDownDouble(pClonk->GetOwner()))
            return 1;
        else
            return 0;
    }
    SetPhase(6, pClonk);
    // Fadenkreuz zum besseren Zielen erzeugen
    var pCross = CreateObject(WCHR, 0, 0, GetOwner(pClonk));
    pCross->SetAction("Crosshair", pClonk);
    Local(0, GetCrosshair(pClonk)) = 84;
    WINC->ActualizePhase(pClonk);
    // dem Clonk übermitteln, wie viel Ammo da ist
    LocalN("iRifleAmmo", pClonk) = ContentsCount();
    while(Contents()) Enter(pCross, Contents());
    // eigene ID speichern
    LocalN("idWeapon", pClonk) = GetID();
    // Schnell noch schauen, ob der Clonk auch Munition hat!
    if(!LocalN("iRifleAmmo", pClonk)) DefinitionCall(GetID(), "CheckAmmo", pClonk);
    // ...und selbst löschen
    RemoveObject();
    return 1;
}

public func Activate (pClonk) {
    // Schon/Noch am Laden?
    if(WildcardMatch(GetAction(pClonk), "*Load*")) return 1;
    // Noch genug Muni vorhanden?
    if(ContentsCount() == 6) return Sound("RevolverNoAmmo", 0, pClonk);
    // Clonk hat keine Muni mehr?
    if(!pClonk->~GetCartridgeCount()) return Sound("RevolverNoAmmo", 0, pClonk);
    // Inventar wieder auf Waffe verschieben
    ShiftContents(pClonk, 1, GetID());
    // Nachladen
    pClonk->~ControlThrow(pClonk);
    pClonk->~LoadRifle();
    // Kein Zielen nach dem Laden
    AddEffect("NoAim", pClonk, 1, 1, 0, GetID());
    // Dabei kein Fadenkreuz anzeigen
    var obj;
    if(obj = FindObject(WCHR, 0, 0, 0, 0, 0, "Crosshair", pClonk)) SetClrModulation(RGBa(255, 255, 255, 255), obj);
    //RemoveObject(FindObject(WCHR, 0, 0, 0, 0, 0, "Crosshair", pClonk));
    return 1;
}

public func ControlDig(pClonk) {
    if(WildcardMatch(GetAction(pClonk), "*Aim*")) {
        // Noch genug Muni vorhanden?
        if(LocalN("iRifleAmmo", pClonk) == 6) return Sound("RevolverNoAmmo", 0, pClonk);
        // Clonk hat keine Muni mehr?
        if(!pClonk->~GetCartridgeCount()) return Sound("RevolverNoAmmo", 0, pClonk);
        LocalN("iAimPhase", pClonk) = GetPhase(pClonk);
        pClonk->~LoadRifle();
        return 1;
    }
}

func FxNoAimTimer(pTarget) {
    // Wenn der Clonk nicht mehr läd
    if(!WildcardMatch(GetAction(pTarget), "*Load*")) {
        // Wenn er zielt abbrechen, sonst nichts machen (z.B. bei Tumble)
        if(WildcardMatch(GetAction(pTarget), "*Aim*"))  ObjectSetAction(pTarget, "Walk");
        return -1;
    }
}

public func ControlUp (pClonk) {             // hoch zielen
    AimUp(pClonk, 1, "ControlConf", WINC);
}

public func ControlDown(pClonk) {           // runter zielen
    AimDown(pClonk, 1, "ControlConf", WINC);
}

public func ControlUpdate(object clonk, int comdir, bool dig, bool throw) {
    AimUpdate(clonk, comdir, 1, "ControlConf", WINC);
}

public func ControlConf(int conf, object clonk) {
    if(clonk->GetAction() != "AimRifle" && clonk->GetAction() != "RideAimRifle") {
        AimCancel(clonk);
    } else {
        // Feinere Abstufung mit JnR möglich
        var angle_change;
        if(GetPlrCoreJumpAndRunControl(clonk->GetController()))
            angle_change = conf * 2;
        else
            angle_change = conf * 6;

        Local(0, GetCrosshair(clonk)) = BoundBy(Local(0, GetCrosshair(clonk)) + angle_change, 0, 140);
        WINC->ActualizePhase(clonk);
    }
}

public func ActualizePhase(pClonk) {
    var iAngle = Local(0, GetCrosshair(pClonk));
//  SetPhase(BoundBy(Local(0,pClonk)/14,0,9),pClonk);
    if(iAngle < 6)        SetPhase(0, pClonk);
    else if(iAngle < 24)  SetPhase(1, pClonk);
    else if(iAngle < 35)  SetPhase(2, pClonk);
    else if(iAngle < 48)  SetPhase(3, pClonk);
    else if(iAngle < 54)  SetPhase(4, pClonk);
    else if(iAngle < 72)  SetPhase(5, pClonk);
    else if(iAngle < 90)  SetPhase(6, pClonk);
    else if(iAngle < 108) SetPhase(7, pClonk);
    else if(iAngle < 128) SetPhase(8, pClonk);
    else		      SetPhase(9, pClonk);
    var iDir = GetDir(pClonk);
    if(!iDir) iDir = -1;
    var obj = FindObject(WCHR, 0, 0, 0, 0, 0, "Crosshair", pClonk);
    SetVertexXY(0, -Sin(Local(0, GetCrosshair(pClonk)), 40)*iDir, Cos(Local(0, GetCrosshair(pClonk)), 40), obj);
}

public func ControlLeft(pClonk) {           // nach links drehen
    if(!WildcardMatch(GetAction(pClonk), "*Rifle*")) return;
    if(pClonk->~IsRiding()) {
        GetActionTarget(0, pClonk)->~ControlLeft(pClonk);
        return 1;
    }
    if(GetDir(pClonk))
        SetDir(0, pClonk);
    WINC->ActualizePhase(pClonk);
    return 1;
}

public func ControlRight(pClonk) {          // nach rechts drehen
    if(!WildcardMatch(GetAction(pClonk), "*Rifle*")) return;
    if(pClonk->~IsRiding()) {
        GetActionTarget(0, pClonk)->~ControlRight(pClonk);
        return 1;
    }
    if(!GetDir(pClonk))
        SetDir(1, pClonk);
    WINC->ActualizePhase(pClonk);
    return 1;
}

public func ControlCommand(pClonk, iX, iY, iDevitation) {
    if(!WildcardMatch(GetAction(pClonk), "*Rifle*")) return;
    // lädt noch nach
    if(GetAction(pClonk) != "AimRifle" && GetAction(pClonk) != "RideAimRifle") return 1;

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
    if(iDevitation) iAngle += RandomX(-iDevitation / 2, iDevitation / 2);
    if(iAngle > 180) iAngle = -iAngle + 360;
    Local(0, GetCrosshair(pClonk)) = iAngle;
    WINC->ActualizePhase(pClonk);
    RifleShot(pClonk, iAngle);


    return 1;
}


public func Fire(pClonk) {              // Bedienung: Feuern!
    // verschießen
    if(GetAction(pClonk) == "AimRifle" || GetAction(pClonk) == "RideAimRifle") RifleShot(pClonk);
    return 1;
}

protected func RifleShot(pClonk, iAngle) {       // Schuss mit dem Gewehr
    var pObj, pObj2, iX, iY, iR, iXDir, iYDir, iRDir, iDir, iPhase;

    // Noch Ammo da?
    if(!LocalN("iRifleAmmo", pClonk)) return DefinitionCall(LocalN("idWeapon", pClonk), "CheckAmmo", pClonk);
    // Clonk lädt keine neue Kugel rein?
    var i = GetEffectCount(0, pClonk), iEffect;
    while(i--)
        if(iEffect = GetEffect("*ReloadRifle*", pClonk, i))
            return 0;
    // Ammo ist noch vorhanden, also Patrone her
    var ammo = Contents(0, GetCrosshair(pClonk)); //CreateContents(CART, pClonk);
    LocalN("iRifleAmmo", pClonk)--;

    // Austrittsparameter
    iDir = GetDir(pClonk) * 2 - 1;
    iPhase = GetPhase(pClonk);
    if(!iAngle) iAngle = Local(0, GetCrosshair(pClonk)); //iPhase*14;//+RandomX(-7,7); // Winkel mit Abweichung
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
    // Ein Captain trifft schmerzhafter
    if(pClonk->~IsCaptain()) ammo->Launch(-1, 14 + Random(10));
    else ammo->Launch(-1, 10 + Random(10));

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
    AddEffect("ReloadRifle", pClonk, 101, 1);
    return 1;
}

/* Neue Kugel einladen */
// Dies hier wird gebraucht, damit der Clonk kein Dauerfeuer schießen kann

global func FxReloadRifleTimer(objTarget, iNumber, iTime) {
    if(iTime > 30) return -1;
}


/* TimerCall (um Gottes Willen...!): Bei Bedarf Träger nach passender Munition durchsuchen */
// Anders, ganz ganz anders!

protected func CheckAmmo(pClonk) {
    // Clonk hat gar kein Gewehr?!
    if((GetAction(pClonk) != "AimRifle") && (GetAction(pClonk) != "RideAimRifle")) return 0;
    // Clonk hat keine Muni mehr?
    if(!pClonk->~GetCartridgeCount()) return Sound("RevolverNoAmmo", 0, pClonk);

    // Clonk hat das Gewehr und noch Munition. Gut, dann soll er nachladen!
    LocalN("iAimPhase", pClonk) = GetPhase(pClonk);
    pClonk->~LoadRifle();
}

/* Fadenkreuz finden */
public func GetCrosshair(pClonk) {
    return FindObject(WCHR, 0, 0, 0, 0, 0, "Crosshair", pClonk);
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

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() {
    return 1;
}
