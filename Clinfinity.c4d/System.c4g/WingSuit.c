#strict 2

#appendto CLNK

local maxGlideSpeedY, glideSpeedXFactor;
local glideParticleColour;

protected func Initialize() {
	maxGlideSpeedY = 30;
	glideSpeedXFactor = 2;
	glideParticleColour = RGBa(255, 255, 255, 210);
	return _inherited();
}

protected func ControlDownDouble() {
	if( _inherited() == 0 ) {
		if( GetAction() == "Jump" ) {
			if(GetEffect("WingSuit", this) == 0) {
				AddEffect("WingSuit", this, 150, 1, this);
			} else {
				RemoveEffect("WingSuit", this);
			}
		}
	}
	return 0;
}

protected func FxWingSuitStart(object target, int effectNumber, int temporary) {
	Sound("SailDown", false, target, 50);
}

protected func FxWingSuitTimer(object target, int effectNumber, int effectTime) {
	if(target->GetAction() == "Jump") {
		// Maximale Sinkgeschwindigkeit
		if( target->GetYDir() > maxGlideSpeedY ) {
			target->SetYDir( target->GetYDir() - 2 );
		}
		// Horizontale Geschwindigkeit berechnen und anpassen
		var targetXDir = Abs(target->GetYDir()) * (-glideSpeedXFactor + 2 * glideSpeedXFactor * target->GetDir());
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
	}
}