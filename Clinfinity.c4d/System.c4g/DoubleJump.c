#strict 2

#appendto CLNK

local doubleJumpPossible;

protected func ControlUp() {
	if( _inherited() == 0 ) {
		if( GetAction() == "Jump" ) {
			if( doubleJumpPossible && GetYDir() <= 10 && GetYDir() >= -10 ) {
				doubleJumpPossible = false;
				SetYDir( -34 );
			}
		}
	}
	return 0;
}

protected func CheckStuck() {
	if( GetAction() == "Jump" && GetPhase() == 1 ) {
		Log("Phase %d, ydir %d", GetPhase(), GetYDir());
		doubleJumpPossible = true;
	}
	return _inherited();
}