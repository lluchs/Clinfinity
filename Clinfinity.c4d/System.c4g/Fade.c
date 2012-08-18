/*	Script: Fade.c
	Provides convenience functions for fading objects in and out. */

#strict 2

static const Fade_CurrentTransparency = 0;
static const Fade_TargetTransparency = 1;
static const Fade_Change = 2;
static const Fade_Red = 3;
static const Fade_Green = 4;
static const Fade_Blue = 5;

/*	Function: FadeIn
	Fades an object from completely transparent to completely visible.

	Parameters:
	colourModulation	- [optional] Colour modulation to use. White by default.

	Returns:
	_true_ if fading successfully started, _false_ otherwise. */
global func FadeIn(int colourModulation) {
	return FadeFromTo(255, 0, colourModulation);
}

/*	Function: FadeOut
	Fades an object from completely visible to completely transparent.

	Parameters:
	colourModulation	- [optional] Colour modulation to use. White by default.

	Returns:
	_true_ if fading successfully started, _false_ otherwise. */
global func FadeOut(int colourModulation) {
	return FadeFromTo(0, 255, colourModulation);
}

/*	Function: FadeFromTo
	Fades an object between specified transparency values.

	Parameters:
	startTransparency	- Initial transparency for the object.
	targetTransparency	- Transparency to fade to.
	colourModulation	- [optional] Colour modulation to use. White by default.

	Returns:
	_true_ if fading successfully started, _false_ otherwise. */
global func FadeFromTo(int startTransparency, int targetTransparency, int colourModulation) {
	return 0 != AddEffect("Fade", this, 101, 1, this, 0, startTransparency, targetTransparency, colourModulation);
}

global func FxFadeStart(object target, int effectNumber, int temporary, int startTransparency, int targetTransparency, int colourModulation) {
	if(temporary == 0) {
		EffectVar(Fade_CurrentTransparency, target, effectNumber) = startTransparency;
		EffectVar(Fade_TargetTransparency, target, effectNumber) = targetTransparency;
		if(startTransparency < targetTransparency) {
			EffectVar(Fade_Change, target, effectNumber) = 1;
		} else {
			EffectVar(Fade_Change, target, effectNumber) = -1;
		}
		if(colourModulation == 0) {
			EffectVar(Fade_Red, target, effectNumber) = 255;
			EffectVar(Fade_Green, target, effectNumber) = 255;
			EffectVar(Fade_Blue, target, effectNumber) = 255;
			target->SetClrModulation(RGBa(255, 255, 255, startTransparency));
		} else {
			var red, green, blue, alpha;
			SplitRGBaValue(colourModulation, red, green, blue, alpha);
			EffectVar(Fade_Red, target, effectNumber) = red;
			EffectVar(Fade_Green, target, effectNumber) = green;
			EffectVar(Fade_Blue, target, effectNumber) = blue;
			target->SetClrModulation(RGBa(red, green, blue, startTransparency));
		}
	}
}

global func FxFadeTimer(object target, int effectNumber, int effectTime) {
	var current = EffectVar(Fade_CurrentTransparency, target, effectNumber);
	if(current == EffectVar(Fade_TargetTransparency, target, effectNumber)) {
		this->~OnFadeFinish(current);
		return FX_Execute_Kill;
	} else {
		current += EffectVar(Fade_Change, target, effectNumber);
		var red = EffectVar(Fade_Red, target, effectNumber);
		var green = EffectVar(Fade_Green, target, effectNumber);
		var blue = EffectVar(Fade_Blue, target, effectNumber);
		target->SetClrModulation(RGBa(red, green, blue, current));
		EffectVar(Fade_CurrentTransparency, target, effectNumber) = current;
		return FX_OK;
	}
}

global func FxFadeEffect(string newEffectName, object target, int effectNumber, int newEffectNumber, var1, var2, var3, var4) {
	if(newEffectName == "Fade") {
		return FX_Effect_Annul;
	}
}

global func FxFadeAdd(object target, int effectNumber, string newEffectName, int newEffectTimer, int startTransparency, int targetTransparency, int colourModulation) {
	EffectVar(Fade_TargetTransparency, target, effectNumber) = targetTransparency;
	var current = EffectVar(Fade_CurrentTransparency, target, effectNumber);
	if(current < targetTransparency) {
		EffectVar(Fade_Change, target, effectNumber) = 1;
	} else {
		EffectVar(Fade_Change, target, effectNumber) = -1;
	}
	if(colourModulation == 0) {
		EffectVar(Fade_Red, target, effectNumber) = 255;
		EffectVar(Fade_Green, target, effectNumber) = 255;
		EffectVar(Fade_Blue, target, effectNumber) = 255;
	} else {
		var red, green, blue, alpha;
		SplitRGBaValue(colourModulation, red, green, blue, alpha);
		EffectVar(Fade_Red, target, effectNumber) = red;
		EffectVar(Fade_Green, target, effectNumber) = green;
		EffectVar(Fade_Blue, target, effectNumber) = blue;
	}
}