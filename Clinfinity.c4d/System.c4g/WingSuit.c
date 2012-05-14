#strict 2

#appendto CLNK

local maxGlideSpeedY, glideSpeedXFactor;
local glideParticleColour;
local DEBUG_controlDownPressedBefore; // TODO: Remove this after testing is done!

protected func Initialize() {
	maxGlideSpeedY = 30;
	glideSpeedXFactor = 2;
	glideParticleColour = RGBa(255, 255, 255, 210);
	DEBUG_controlDownPressedBefore = false;
	return _inherited();
}

protected func ControlDownSingle() {
	DEBUG_controlDownPressedBefore = false;
	var result = _inherited();
	if( result == 0 ) {
		if(GetAction() == "Jump") {
			DEBUG_controlDownPressedBefore = true;
			Log("OK: %s pressed [Down]", GetPlayerName(GetOwner()));
			if(IsGliding()) {
				RemoveEffect("WingSuit", this);
			} else if(GetPhase() > 3) {
				AddEffect("WingSuit", this, 150, 1, this);
			} else {
				return 0;
			}
			return true;
		}
	} else {
		return result;
	}
	return 0;
}

public func IsGliding() {
	return GetEffect("WingSuit", this) != 0;
}

protected func FxWingSuitStart(object target, int effectNumber, int temporary) {
	if(temporary == 0) {
		Sound("SailDown", false, target, 50);
		if(!target->LocalN("DEBUG_controlDownPressedBefore"))
			Log("WARNING: [Down] was not pressed before wing suit activates!");
		else
			Log("OK: WingSuit opened normally after pressing [Down]");
		target->LocalN("DEBUG_controlDownPressedBefore") = false;
	}
}

protected func FxWingSuitTimer(object target, int effectNumber, int effectTime) {
	if(target->GetAction() == "Jump") {
		// Maximale Sinkgeschwindigkeit
		if( target->GetYDir() > maxGlideSpeedY ) {
			target->SetYDir( target->GetYDir() - 2 );
		}
		// Horizontale Geschwindigkeit berechnen und anpassen
		var targetXDir = Max( Abs(target->GetYDir()), 5 ) * (-glideSpeedXFactor + 2 * glideSpeedXFactor * target->GetDir());
		var currentXDir = target->GetXDir();
		if(currentXDir < targetXDir) {
			target->SetXDir( target->GetXDir() + 1 );
		} else if(currentXDir > targetXDir) {
			target->SetXDir( target->GetXDir() - 1 );
		}
		// Rotiert die Jump-Animation, sodass es aussieht, als würde der Clonk einen Wingsuit tragen.
		SetObjDrawTransform(0, 1000 - 2000 * target->GetDir(), 0, -1000, 0, 0);
		if(!Random(9)) {
			CreateParticle("NoGravSpark", RandomX(-7, 7), RandomX(-3, 3), target->GetXDir() / 2, target->GetYDir() / 2, 40, glideParticleColour);
		}
		return 0;
	} else {
		return -1;
	}
}

protected func FxWingSuitStop(object target, int effectNumber, int reason, bool temporary) {
	if(!temporary) {
		Sound("SailUp", false, target, 50);
		SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, 0);
		target->LocalN("DEBUG_controlDownPressedBefore") = false;
	}
}

/* Prevents the Clonk from tumbling after hitting something while gliding. */
protected func Hit(int xdir, int ydir) {
	if(IsGliding() && GetAction() == "Tumble") {
		SetXDir(BoundBy(xdir, -10, 10));
		SetYDir(BoundBy(ydir, -10, 10));
		SetAction("Jump");
	}
	return _inherited(xdir, ydir, ...);
}

