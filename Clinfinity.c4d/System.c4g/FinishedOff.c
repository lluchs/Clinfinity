/*  Script: FinishedOff.c
	Provides a finished off mechanic to prevent players from suiciding. */
#strict 2

static const FINISHED_OFF_TIMEOUT = 288; // 8*36

/*  Function: AddFinishedOffListener
	Adds an effect monitoring incoming damage and death. */
global func AddFinishedOffListener() {
	return AddEffect("FinishedOffListener", this, 1);
}

/*  Function: FinishedOff
	Returns the player who finished off this clonk, or NO_OWNER.
	
	"Finished off" means that somebody has just dealt damage to the clonk. */
global func FinishedOff() {
	var effect = GetEffect("FinishedOffListener", this);
	if(effect && FrameCounter() - EffectVar(1, this, effect) < FINISHED_OFF_TIMEOUT)
		return EffectVar(0, this, effect);
	else
		return NO_OWNER;
}

global func FxFinishedOffListenerDamage(object target, int effectNum, int amount, int cause) {
	var killer = target->GetKiller();
	if(killer != NO_OWNER && killer != GetOwner()) {
		EffectVar(0, target, effectNum) = killer;
		EffectVar(1, target, effectNum) = FrameCounter();
	}
	return amount;
}

global func FxFinishedOffListenerStop(object target, int effectNum, int reason, bool temp) {
	// Don't delete on death.
	return -1;
}
