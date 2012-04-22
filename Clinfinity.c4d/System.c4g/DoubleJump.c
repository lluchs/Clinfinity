#strict 2

#appendto CLNK

local doubleJumpPossible;
local jumpParticleColour;

protected func Initialize() {
	jumpParticleColour = RGBa(255, 255, 255, 150);
	return _inherited();
}

protected func ControlUp() {
	if( _inherited() == 0 ) {
		if( GetAction() == "Jump" ) {
			if( doubleJumpPossible && GetYDir() <= 10 && GetYDir() >= -10 ) {
				doubleJumpPossible = false;
				SetYDir( -34 );
				for( var i = 0; i < 7; i++ ) {
					CreateParticle("MSpark", -3 + i, 9 + Random(3), -3 + i, 8 - Random(3), 40, jumpParticleColour);
				}
			}
		}
	}
	return 0;
}

protected func CheckStuck() {
	if( GetAction() == "Jump" && GetPhase() == 1 ) {
		doubleJumpPossible = true;
	}
	return _inherited();
}