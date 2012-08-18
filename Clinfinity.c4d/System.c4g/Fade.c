/*	Script: Fade.c
	Provides convenience functions for fading objects in and out. */

#strict 2

static const Fade_CurrentTransparency = 0;
static const Fade_TargetTransparency = 1;
static const Fade_Change = 2;

/*	Function: FadeIn
	Fades an object from completely transparent to completely visible.

	Returns:
	_true_ if fading successfully started, _false_ otherwise. */
global func FadeIn() {
	return FadeFromTo(255, 0);
}

/*	Function: FadeOut
	Fades an object from completely visible to completely transparent.

	Returns:
	_true_ if fading successfully started, _false_ otherwise. */
global func FadeOut() {
	return FadeFromTo(0, 255);
}

/*	Function: FadeFromTo
	Fades an object between specified transparency values.

	Parameters:
	startTransparency	- Initial transparency for the object.
	targetTransparency	- Transparency to fade to.

	Returns:
	_true_ if fading successfully started, _false_ otherwise. */
global func FadeFromTo(int startTransparency, int targetTransparency) {
	return 0 != AddEffect("Fade", this, 101, 1, this, 0, startTransparency, targetTransparency)
}

global func FxFadeStart(object target, int effectNumber, int temporary, int startTransparency, int targetTransparency) {
	if(temporary == 0) {
		EffectVar(Fade_CurrentTransparency, target, effectNumber) = startTransparency;
		EffectVar(Fade_TargetTransparency, target, effectNumber) = targetTransparency;
		if(startTransparency < targetTransparency) {
			EffectVar(Fade_Change, target, effectNumber) = 1;
		} else {
			EffectVar(Fade_Change, target, effectNumber) = -1;
		}
		target->SetClrModulation(RGBa(255, 255, 255, startTransparency));
	}
}

global func FxFadeTimer(object target, int effectNumber, int effectTime) {
	var current = EffectVar(Fade_CurrentTransparency, target, effectNumber);
	if(current == EffectVar(Fade_TargetTransparency, target, effectNumber)) {
		this->~OnFadeFinish(current);
		return FX_Execute_Kill;
	} else {
		current += EffectVar(Fade_Change, target, effectNumber);
		target->SetClrModulation(RGBa(255, 255, 255, current));
		EffectVar(Fade_CurrentTransparency, target, effectNumber) = current;
		return FX_OK;
	}
}

global func FxFadeEffect(string newEffectName, object target, int effectNumber, int newEffectNumber, var1, var2, var3, var4) {
	if(newEffectName == "Fade") {
		return FX_Effect_Annul;
	}
}

global func FxFadeAdd(object target, int effectNumber, string newEffectName, int newEffectTimer, int startTransparency, int targetTransparency) {
	EffectVar(Fade_TargetTransparency, target, effectNumber) = targetTransparency;
	var current = EffectVar(Fade_CurrentTransparency, target, effectNumber);
	if(current < targetTransparency) {
		EffectVar(Fade_Change, target, effectNumber) = 1;
	} else {
		EffectVar(Fade_Change, target, effectNumber) = -1;
	}
}