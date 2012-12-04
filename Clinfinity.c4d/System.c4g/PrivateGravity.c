/*  Script: PrivateGravity.c
    Provides object-private gravity that overrides global gravity. */

#strict 2

/*	Function: SetPrivateGravity
	Sets the gravity specific to the object.
	Global gravity is unaffected.

	Parameters:
	gravity	- New private gravity value. */
global func SetPrivateGravity(int gravity) {
	if(GetEffect("PrivateGravity", this) != 0) {
		RemovePrivateGravity();
	}
	AddEffect("PrivateGravity", this, 101, 1, this, 0, gravity);
}

/*	Function: RemovePrivateGravity
	Removes the object-specific gravity, restoring the effect of the global gravity. */
global func RemovePrivateGravity() {
	RemoveEffect("PrivateGravity", this);
}

global func FxPrivateGravityStart(object target, int effectNumber, int temporary, int gravity) {
	EffectVar(0, target, effectNumber) = gravity;
}

global func FxPrivateGravityTimer(object target, int effectNumber, int effectTime) {
	var antiGravity = -GetGravity() / 50;
	var antiGravityRoundingCorrection = 0;
	if(GetGravity() % 50 != 0) {
		antiGravityRoundingCorrection = -1;
	}

	var privateGravity = EffectVar(0, target, effectNumber) / 50;

	SetYDir(GetYDir() + antiGravity + antiGravityRoundingCorrection + privateGravity);

	return FX_OK;
}

global func FxPrivateGravityEffect(string newEffectName, object target, int effectNumber, int newEffectNumber, var1, var2, var3, var4) {
	if(newEffectName == "PrivateGravity") return FX_Effect_Deny;
}
