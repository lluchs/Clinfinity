/*	Script: Fade.c
	Provides convenience functions for fading objects in and out.
	After fading is finished, the function _OnFadeFinish_ is called in the target object,
	with the final modulation as first parameter. */

#strict 2

static const Fade_CurrentRed = 0;
static const Fade_CurrentGreen = 1;
static const Fade_CurrentBlue = 2;
static const Fade_CurrentAlpha = 3;

static const Fade_TargetRed = 4;
static const Fade_TargetGreen = 5;
static const Fade_TargetBlue = 6;
static const Fade_TargetAlpha = 7;

static const Fade_ChangeRed = 8;
static const Fade_ChangeGreen = 9;
static const Fade_ChangeBlue = 10;
static const Fade_ChangeAlpha = 11;

/*	Function: FadeIn
	Fades an object from completely transparent to a specified modulation.
	If the modulation is not specified, the object will be faded to complete visibility, retaining its current colour modulation.

	Parameters:
	targetColourModulation	- [optional] Colour modulation to fade to. Current modulation by default.

	Returns:
	_true_ if fading successfully started, _false_ otherwise. */
global func FadeIn(int targetColourModulation) {
	if(targetColourModulation == 0) {
		if(GetClrModulation() == 0) {
			targetColourModulation = RGB(255, 255, 255);
		} else {
			targetColourModulation = SetRGBaValue(GetClrModulation(), 0, 0);
		}
	}
	return FadeFromTo(SetRGBaValue(targetColourModulation, 255, 0), targetColourModulation);
}

/*	Function: FadeOut
	Fades an object from a specified modulation to completely transparent.
	If the modulation is not specified, the object's current modulation will be used as starting point.

	Parameters:
	initialColourModulation	- [optional] Colour modulation to fade from. Current modulation by default.

	Returns:
	_true_ if fading successfully started, _false_ otherwise. */
global func FadeOut(int initialColourModulation) {
	if(initialColourModulation == 0) {
		initialColourModulation = GetClrModulation();
	}
	return FadeFromTo(initialColourModulation, SetRGBaValue(initialColourModulation, 255, 0));
}

/*	Function: FadeFromTo
	Fades an object between specified colour modulations.

	Parameters:
	initialColourModulation	- Initial modulation for the object.
	targetColourModulation	- Modulation to fade to.

	Returns:
	_true_ if fading successfully started, _false_ otherwise. */
global func FadeFromTo(int initialColourModulation, int targetColourModulation) {
	return 0 != AddEffect("Fade", this, 101, 1, this, 0, initialColourModulation, targetColourModulation);
}

global func FxFadeStart(object target, int effectNumber, int temporary, int initialColourModulation, int targetColourModulation) {
	if(temporary == 0) {
		if(initialColourModulation == 0 || targetColourModulation == 0) {
			return FX_Start_Deny;
		}

		var initRed, initGreen, initBlue, initAlpha;
		SplitRGBaValue(initialColourModulation, initRed, initGreen, initBlue, initAlpha);
		EffectVar(Fade_CurrentRed, target, effectNumber) = initRed;
		EffectVar(Fade_CurrentGreen, target, effectNumber) = initGreen;
		EffectVar(Fade_CurrentBlue, target, effectNumber) = initBlue;
		EffectVar(Fade_CurrentAlpha, target, effectNumber) = initAlpha;

		var targetRed, targetGreen, targetBlue, targetAlpha;
		SplitRGBaValue(targetColourModulation, targetRed, targetGreen, targetBlue, targetAlpha);
		EffectVar(Fade_TargetRed, target, effectNumber) = targetRed;
		EffectVar(Fade_TargetGreen, target, effectNumber) = targetGreen;
		EffectVar(Fade_TargetBlue, target, effectNumber) = targetBlue;
		EffectVar(Fade_TargetAlpha, target, effectNumber) = targetAlpha;

		EffectVar(Fade_ChangeRed, target, effectNumber) = CalculateChange(initRed, targetRed);
		EffectVar(Fade_ChangeGreen, target, effectNumber) = CalculateChange(initGreen, targetGreen);
		EffectVar(Fade_ChangeBlue, target, effectNumber) = CalculateChange(initBlue, targetBlue);
		EffectVar(Fade_ChangeAlpha, target, effectNumber) = CalculateChange(initAlpha, targetAlpha);

		target->SetClrModulation(RGBa(initRed, initGreen, initBlue, initAlpha));
	}
}

global func FxFadeTimer(object target, int effectNumber, int effectTime) {
	var currentRed		= EffectVar(Fade_CurrentRed, target, effectNumber);
	var currentGreen	= EffectVar(Fade_CurrentGreen, target, effectNumber);
	var currentBlue		= EffectVar(Fade_CurrentBlue, target, effectNumber);
	var currentAlpha	= EffectVar(Fade_CurrentAlpha, target, effectNumber);

	var targetRed		= EffectVar(Fade_TargetRed, target, effectNumber);
	var targetGreen		= EffectVar(Fade_TargetGreen, target, effectNumber);
	var targetBlue		= EffectVar(Fade_TargetBlue, target, effectNumber);
	var targetAlpha		= EffectVar(Fade_TargetAlpha, target, effectNumber);

	var matches = 0;
	if(currentRed == targetRed) {
		EffectVar(Fade_ChangeRed, target, effectNumber) = 0;
		matches++;
	}
	if(currentGreen == targetGreen) {
		EffectVar(Fade_ChangeGreen, target, effectNumber) = 0;
		matches++;
	}
	if(currentBlue == targetBlue) {
		EffectVar(Fade_ChangeBlue, target, effectNumber) = 0;
		matches++;
	}
	if(currentAlpha == targetAlpha) {
		EffectVar(Fade_ChangeAlpha, target, effectNumber) = 0;
		matches++;
	}
	if(matches == 4) {
		target->~OnFadeFinish(RGBa(targetRed, targetGreen, targetBlue, targetAlpha));
		return FX_Execute_Kill;
	}

	currentRed 		+= EffectVar(Fade_ChangeRed, target, effectNumber);
	currentGreen 	+= EffectVar(Fade_ChangeGreen, target, effectNumber);
	currentBlue 	+= EffectVar(Fade_ChangeBlue, target, effectNumber);
	currentAlpha 	+= EffectVar(Fade_ChangeAlpha, target, effectNumber);
	
	target->SetClrModulation(RGBa(currentRed, currentGreen, currentBlue, currentAlpha));
	
	EffectVar(Fade_CurrentRed, target, effectNumber)	= currentRed;
	EffectVar(Fade_CurrentGreen, target, effectNumber)	= currentGreen;
	EffectVar(Fade_CurrentBlue, target, effectNumber)	= currentBlue;
	EffectVar(Fade_CurrentAlpha, target, effectNumber)	= currentAlpha;

	return FX_OK;
}

global func FxFadeEffect(string newEffectName, object target, int effectNumber, int newEffectNumber, var1, var2, var3, var4) {
	if(newEffectName == "Fade") {
		if(var2 == 0) {
			return FX_Effect_Deny;
		}
		return FX_Effect_Annul;
	}
}

global func FxFadeAdd(object target, int effectNumber, string newEffectName, int newEffectTimer, int initialColourModulation, int targetColourModulation) {
	var currentRed		= EffectVar(Fade_CurrentRed, target, effectNumber);
	var currentGreen	= EffectVar(Fade_CurrentGreen, target, effectNumber);
	var currentBlue		= EffectVar(Fade_CurrentBlue, target, effectNumber);
	var currentAlpha	= EffectVar(Fade_CurrentAlpha, target, effectNumber);

	var targetRed, targetGreen, targetBlue, targetAlpha;
	SplitRGBaValue(targetColourModulation, targetRed, targetGreen, targetBlue, targetAlpha);
	EffectVar(Fade_TargetRed, target, effectNumber) = targetRed;
	EffectVar(Fade_TargetGreen, target, effectNumber) = targetGreen;
	EffectVar(Fade_TargetBlue, target, effectNumber) = targetBlue;
	EffectVar(Fade_TargetAlpha, target, effectNumber) = targetAlpha;

	EffectVar(Fade_ChangeRed, target, effectNumber) = CalculateChange(currentRed, targetRed);
	EffectVar(Fade_ChangeGreen, target, effectNumber) = CalculateChange(currentGreen, targetGreen);
	EffectVar(Fade_ChangeBlue, target, effectNumber) = CalculateChange(currentBlue, targetBlue);
	EffectVar(Fade_ChangeAlpha, target, effectNumber) = CalculateChange(currentAlpha, targetAlpha);
}

global func CalculateChange(int start, int target) {
	if(start < target) {
		return 1;
	} else if(start == target) {
		return 0;
	} else {
		return -1;
	}
}
