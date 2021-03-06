/*  Script: Firefly
*/

#strict 2

static const IRRL_MaxSpawnDistance = 15;
static const IRRL_MaxDistance = 40;
static const IRRL_ShynessDistance = 40;

local attractedTo;
local light;

public func SpawnSwarm(int x, int y, int size, object attractedTo) {
	for(var i = 0; i < size; i++) {
		var firefly = CreateObject(IRRL, RandomX(x - IRRL_MaxSpawnDistance, x + IRRL_MaxSpawnDistance), RandomX(y - IRRL_MaxSpawnDistance, y + IRRL_MaxSpawnDistance), NO_OWNER);
		firefly->LocalN("attractedTo") = attractedTo;
	}
}

private func Flying() {
	var xdir, ydir;

	var awayFrom = FindObject2(Find_Distance(IRRL_ShynessDistance), Find_Category(C4D_Object), Find_OCF(OCF_HitSpeed1), Find_NoContainer());
	if(awayFrom != 0) {
		xdir = BoundBy(GetX() - awayFrom->GetX(), -1, 1);
		ydir = BoundBy(GetY() - awayFrom->GetY(), -1, 1);
		if(xdir == 0) xdir = Random(2) * 2 - 1;
		if(ydir == 0) ydir = Random(2) * 2 - 1;
		xdir = RandomX(5 * xdir, 10 * xdir);
		ydir = RandomX(5 * ydir, 10 * ydir);
		// No check for liquids here, you can scare fireflies into those ;)
		SetSpeed(xdir, ydir);
	} else {
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
}

protected func Check() {
	// Buried or in water: Instant death
	if(GBackSemiSolid()) {
		Death();
	}
}

protected func Initialize() {
	SetAction("Fly");
	SetPhase(Random(6));
	var lightColour = RGBa(220, 255, 200, 0);
	light = AddLight(40, lightColour, this);

	FadeIn();
	var alphamod, sizemod;
	CalcLight(alphamod, sizemod);
	light->FadeIn(SetRGBaValue(lightColour, Min(60 + alphamod, 255), 0));

	var time = FindObject2(Find_ID(TIME));
	if(time != 0) {
		time->AddEventListener(this, "OnDaybreak");
	}
}

public func OnDaybreak() {
	ScheduleCall(this, "StartFadeOut", RandomX(1, 150));
}

public func OnFadeFinish(int targetColour) {
	if(GetRGBaValue(targetColour, 0) == 255) {
		Death();
	}
}

private func StartFadeOut() {
	FadeOut();
	light->FadeOut();
}

public func CatchBlow()	{ RemoveObject(); }
public func Damage()	{ RemoveObject(); }
protected func Death()	{ RemoveObject(); }
