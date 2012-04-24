#strict 2

#appendto CLNK

local maxGlideSpeedY, glideSpeedXFactor;

protected func Initialize() {
	maxGlideSpeedY = 30;
	glideSpeedXFactor = 2;
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
	// WingSuitFlight-Action setzen
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
		return 0;
	} else {
		return -1;
	}
}

protected func FxWingSuitStop(object target, int effectNumber, int reason, bool temporary) {
	if(!temporary) {
		// Wieder normale Jump-Action setzen, Geschwindigkeit eventuell auch setzen.
	}
}