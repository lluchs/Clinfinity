#strict 2

#appendto CLNK

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
	if(target->GetAction() == "WingSuitFlight") {
		// Geschwindigkeit berechnen und setzen
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