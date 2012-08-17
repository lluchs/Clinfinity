/*  Script: Firefly
*/

#strict 2


private func Flying() {
	var xdir, ydir;

	if(Random(4)) return;

	xdir = Random(13) - 6;
	ydir = Random(9) - 4;
	if(GBackLiquid(xdir, ydir)) {
		SetSpeed(0, 0);
	} else {
		SetSpeed(xdir,ydir);
	}
}

protected func Check() {
	// Buried or in water: Instant death
	if(GBackSemiSolid()) {
		Death();
		return;
	}
}

protected func Initialize() {
	SetAction("Fly");
	SetPhase(Random(6));
	AddLight(60, RGB(255, 255, 200), this);
}

public func CatchBlow()	{ RemoveObject(); }
public func Damage()	{ RemoveObject(); }
protected func Death()	{ RemoveObject(); }
