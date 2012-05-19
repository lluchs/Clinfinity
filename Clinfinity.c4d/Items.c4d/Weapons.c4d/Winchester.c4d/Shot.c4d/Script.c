/*-- fliegende Patrone --*/

#strict

#include ARRW

local xdir, ydir, dam;

/* Eigenschaften */
// Wird zur korrekten Inventarverwaltung gebraucht
public func PackTo() {
    return(AMBO);
}
public func IsBullet() {
    return(1);
}
public func IsArrow() {
    return 0;
}

/* Wird abgefeuert */
public func Launch(iDir, iDam) {
    ChangeDef(CSHO);
    SetCategory(C4D_Vehicle());
    if(iDir == DIR_Left()) xdir = -100;
    if(iDir == DIR_Right()) xdir = +100;
	ydir = GetYDir();
    dam = iDam;   // Schaden
    SetAction("Travel");
}

local iOldX, iOldY;

/* Fliegt */
private func Travel() {
    // Bewegung
    if(xdir != 0) { // Revolverschu�
        SetXDir(xdir);
        ydir = 0;
        if(GetActTime() > 20)
            ydir = 5;
    }
	// fixed ydir
	SetYDir(ydir);
    // Auf Treffer pr�fen
    if(GetActTime() > 2) CheckHit();
    iOldX = GetX();
    iOldY = GetY();

	// trace flight
	CreateParticle("PSpark", 0, 0, 0, 0, 20, RGB(255, 255, 255));
    return();
}

private func CheckHit() {
    // Nur wenn schon passende Werte da sind
    if(!iOldX && !iOldY) return();
    // Distance zur letzten Position berechnen
    var iXDist = GetX() - iOldX;
    var iYDist = GetY() - iOldY;
    // Treffer-�berpr�fung
    var iSteps = Abs(iXDist / 4);
    if(Abs(GetXDir()) < Abs(GetYDir())) iSteps = Abs(iYDist / 4);
    var iX, iY, pObj;
    // Mit Schleife alle Zwischenpunkte abklappern
    for(var i = iSteps; i; i--) {
        iX = -iXDist * i / iSteps;
        iY = -iYDist * i / iSteps;
        if(!iY) iY = 1;
        //CreateParticle("NoGravSpark", iX, iY, 0, 0, 30, RGB(i*255/iSteps, (iSteps-i)*255/iSteps));
        if(pObj = FindObject(0, iX, iY, 0, 0, OCF_Alive(), 0, 0, NoContainer())) return(HitLiving(pObj));
    }
}

/* Treffer */
protected func Hit() {
    // Vielleicht stand jedoch zwischen Wand und letzter Position noch jemand
    // -> also nochmal pr�fen
    if(CheckHit()) return();

    Sound("Ricochet*");
    RemoveObject();
}

private func HitLiving(living) {
    Sound("Punch*");
    // Schaden machen
    if(Random(5))
        DoEnergy(-dam, living);
    else
        // Manchmal Punch benutzen
        Punch(living, dam);
    return(RemoveObject());
}

public func Entrance(pClonk) {
    if(pClonk->~IsWeapon() || GetID(pClonk) == WCHR) return;
    return _inherited(pClonk, ...);
}
