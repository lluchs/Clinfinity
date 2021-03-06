#strict 2

#appendto CLNK

local doubleJumpPossible;
local jumpParticleColour;
local maxDoubleJumpStartSpeed, doubleJumpAcceleration;

protected func Initialize() {
	jumpParticleColour = RGBa(255, 255, 255, 150);
	maxDoubleJumpStartSpeed = 50;
	doubleJumpAcceleration = 34;
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
			if( doubleJumpPossible ) {
				doubleJumpPossible = false;
				if( GetDir() == DIR_Left ) {
					SetXDir( -1 * Abs( GetXDir() ) );
				} else {
					SetXDir( Abs( GetXDir() ) );
				}
				// To make double jumping useful when flying upwards, add more speed
				if(GetYDir() < 0) {
					SetYDir(-Sqrt(GetYDir() ** 2 + doubleJumpAcceleration ** 2));
				} else {
					SetYDir(-doubleJumpAcceleration);
				}
				for( var i = 0; i < 7; i++ ) {
					CreateParticle("MSpark", -3 + i, 9 + Random(3), -3 + i, 8 - Random(3), 40, jumpParticleColour);
				}
				Sound("DoubleJump");
				return true;
			}
		}
	} else {
		return result;
	}
	return 0;
}

protected func MakeDoubleJumpPossible() {
	doubleJumpPossible = true;
}

protected func CheckStuck() {
	if(GetAction() == "Jump") {
		// Visual clue that double jump is not available any more: Tumble if falling speed is too high.
		if(GetYDir() > maxDoubleJumpStartSpeed && !(this->~IsGliding())) {
			SetAction("Tumble");
		}
	}
	return _inherited();
}
