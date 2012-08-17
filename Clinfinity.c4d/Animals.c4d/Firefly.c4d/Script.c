/*  Script: Firefly
*/

#strict 2

static const IRRL_MaxSpawnDistance = 5;
static const IRRL_MaxDistance = 40;

local attractedTo;

public func SpawnSwarm(int x, int y, int size, object attractedTo) {
	for(var i = 0; i < size; i++) {
		var firefly = CreateObject(IRRL, RandomX(x - IRRL_MaxSpawnDistance, x + IRRL_MaxSpawnDistance), RandomX(y - IRRL_MaxSpawnDistance, y + IRRL_MaxSpawnDistance), NO_OWNER);
		firefly->LocalN("attractedTo") = attractedTo;
		// TODO: Let fireflies fade in
	}
}

private func Flying() {
	var xdir, ydir;

	if(Random(4)) return;

	if(attractedTo != 0 && ObjectDistance(attractedTo) > IRRL_MaxDistance) {
		xdir = BoundBy(attractedTo->GetX() - GetX(), -1, 1);
		ydir = BoundBy(attractedTo->GetY() - GetY(), -1, 1);
		xdir = RandomX(xdir, 6 * xdir);
		ydir = RandomX(ydir, 6 * ydir);
	} else {
		xdir = Random(13) - 6;
		ydir = Random(9) - 4;
	}

	if(GBackLiquid(xdir, ydir)) {
		SetSpeed(0, 0);
	} else {
		SetSpeed(xdir, ydir);
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
	AddLight(40, RGB(220, 255, 200), this);
}

public func CatchBlow()	{ RemoveObject(); }
public func Damage()	{ RemoveObject(); }
protected func Death()	{ RemoveObject(); }
