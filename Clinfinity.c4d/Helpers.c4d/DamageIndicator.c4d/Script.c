#strict 2

static const DIND_Duration = 35;

local amount, alphaEase;

public func IndicateDamage(object damagedObject, int amount) {
	if(damagedObject == 0) return;

	var x = damagedObject->GetX() - GetX();
	var y = damagedObject->GetY() - GetY();
	var indicator = CreateObject(DIND, x, y, NO_OWNER);
	Fling(indicator, RandomX(-2, 2), RandomX(-2, 1));

	indicator->LocalN("alphaEase") = CreateEaseFunction("quad-in-out", DIND_Duration);
	indicator->LocalN("amount") = amount;
	AddEffect("AlphaFade", indicator, 1, 1, indicator);
}

protected func FxAlphaFadeTimer(object target, int effectNumber, int effectTime) {
	if(effectTime > DIND_Duration) {
		RemoveObject();
		return FX_Execute_Kill;
	}
	var alpha = ChangeRange(EvalEase(alphaEase, effectTime), 0, DIND_Duration, 0, 255);
	CustomMessage(Format("@-%d", amount), this, NO_OWNER, 0, 0, RGBa(255, 0, 0, alpha));
	return FX_OK;
}
