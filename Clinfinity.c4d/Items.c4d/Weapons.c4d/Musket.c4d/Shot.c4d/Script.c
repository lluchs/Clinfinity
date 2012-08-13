/*-- fliegende Patrone --*/

#strict 2

#include ARRW

local xdir, ydir, dam, knockback;

/* Eigenschaften */
// Wird zur korrekten Inventarverwaltung gebraucht
public func PackTo() {
    return AMBO;
}
public func IsBullet() {
    return 1;
}
public func IsArrow() {
    return 0;
}

/* Wird abgefeuert */
public func Launch(int dir, int damage, bool knock) {
    ChangeDef(CSHO);
    SetCategory(C4D_Vehicle);
    if(dir == DIR_Left) xdir = -100;
    if(dir == DIR_Right) xdir = +100;
	ydir = GetYDir();
    dam = damage;   // Schaden
	knockback = knock;
    SetAction("Travel");
}

local oldX, oldY;

/* Fliegt */
private func Travel() {
	var actTime = GetActTime();
	// trace flight
	if(oldX && actTime > 1) {
		var color = RGB(255, 255, 255);
		DrawParticleLine("PSpark", 0, 0, AbsX(oldX), AbsY(oldY), 3, 20, color, color);
	}

    // Bewegung
    if(xdir != 0) { // Revolverschuß
        SetXDir(xdir);
        ydir = 0;
        if(actTime > 20)
            ydir = 5;
    }
	// fixed ydir
	SetYDir(ydir);
    // Auf Treffer prüfen
    if(actTime > 2) CheckHit();
    oldX = GetX();
    oldY = GetY();

    return;
}

private func CheckHit() {
    // Nur wenn schon passende Werte da sind
    if(!oldX && !oldY) return;
    // Distance zur letzten Position berechnen
    var xDist = GetX() - oldX;
    var yDist = GetY() - oldY;
    // Treffer-Überprüfung
    var steps = Abs(xDist / 4);
    if(Abs(GetXDir()) < Abs(GetYDir())) steps = Abs(yDist / 4);
    var x, y, obj;
    // Mit Schleife alle Zwischenpunkte abklappern
    for(var i = steps; i; i--) {
        x = -xDist * i / steps;
        y = -yDist * i / steps;
        if(!y) y = 1;
        //CreateParticle("NoGravSpark", x, y, 0, 0, 30, RGB(i*255/steps, (steps-i)*255/steps));
        if(obj = FindObject(0, x, y, 0, 0, OCF_Alive, 0, 0, NoContainer())) return HitLiving(obj);
    }
}

/* Treffer */
protected func Hit() {
    // Vielleicht stand jedoch zwischen Wand und letzter Position noch jemand
    // -> also nochmal prüfen
    if(CheckHit()) return;

    Sound("Ricochet*");
    RemoveObject();
}

private func HitLiving(living) {
    Sound("Punch*");
    // Schaden machen
    if(knockback)
        // Damage with knockback.
        Punch(living, dam);
    else
        DoEnergy(-dam, living);
    return RemoveObject();
}

public func Entrance(clonk) {
    if(clonk->~IsWeapon() || GetID(clonk) == WCHR) return;
    return _inherited(clonk, ...);
}
