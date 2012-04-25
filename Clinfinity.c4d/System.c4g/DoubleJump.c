#strict 2

#appendto CLNK

local doubleJumpPossible;
local jumpParticleColour;

local minDoubleJumpStartSpeed, maxDoubleJumpStartSpeed;

protected func Initialize() {
	jumpParticleColour = RGBa(255, 255, 255, 150);
	minDoubleJumpStartSpeed = -10;
	maxDoubleJumpStartSpeed = 40;
	return _inherited();
}

protected func ControlLeft() {
	if( _inherited() == 0 ) {
		if( GetAction() == "Jump" ) {
			SetDir( DIR_Left );
		}
	}
	return 0;
}

protected func ControlRight() {
	if( _inherited() == 0 ) {
		if( GetAction() == "Jump" ) {
			SetDir( DIR_Right );
		}
	}
	return 0;
}

protected func ControlUp() {
	var result = _inherited();
	if( result == 0 ) {
		if( GetAction() == "Jump" ) {
			if( doubleJumpPossible && Inside( GetYDir(), minDoubleJumpStartSpeed, maxDoubleJumpStartSpeed ) ) {
				doubleJumpPossible = false;
				if( GetDir() == DIR_Left ) {
					SetXDir( -1 * Abs( GetXDir() ) );
				} else {
					SetXDir( Abs( GetXDir() ) );
				}
				SetYDir( -34 );
				for( var i = 0; i < 7; i++ ) {
					CreateParticle("MSpark", -3 + i, 9 + Random(3), -3 + i, 8 - Random(3), 40, jumpParticleColour);
				}
				return true;
			}
		}
	} else {
		return result;
	}
	return 0;
}

protected func CheckStuck() {
	if( GetAction() == "Jump" && GetPhase() == 1 ) {
		doubleJumpPossible = true;
	}
	return _inherited();
}