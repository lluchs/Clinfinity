/*-- fliegende Patrone --*/

#strict 2

#include ARRW
#include MWEP

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
    // No check if bullet didn't travel yet
    if(!oldX && !oldY) return;

    var newX = GetX();
    var newY = GetY();
    var xDist = newX - oldX;
    var yDist = newY - oldY;

    // Go back to old position so objects can correctly determine from which direction the bullet came
    SetPosition(oldX, oldY);

    var steps = Abs(xDist / 4);
    if(Abs(GetXDir()) < Abs(GetYDir())) steps = Abs(yDist / 4);
    var x, y, obj;
    for(var i = 0; i < steps; ++i) {
        x = xDist * i / steps;
        y = yDist * i / steps;
        if(y == 0) y = 1;
        if(obj = FindObject(0, x, y, 0, 0, OCF_Alive, 0, 0, NoContainer())) {
            HitLiving(obj);
            break;
        }
    }

    SetPosition(newX, newY);
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
    if(knockback) ThrowBackObject(living, 2, true);
    living->InflictDamage(dam, this);
    return RemoveObject();
}

public func GetDamageType() { return DamageType_Bullet; }

public func Entrance(clonk) {
    if(clonk->~IsWeapon() || GetID(clonk) == WCHR) return;
    return _inherited(clonk, ...);
}
