#strict 2

#appendto CLNK

local maxPullUpHeight;

protected func Initialize() {
	maxPullUpHeight = 10;
	return _inherited();
}

protected func ControlUp() {
	var result = _inherited();
	if( result == 0 ) {
		if( GetAction() == "Hangle" && GetComDir() == COMD_None ) {
			for(var i = -11; i > -11 - maxPullUpHeight; i--) {
				if(!GBackSemiSolid(0, i)) {
					SetPosition(GetX(), GetY() + i - (GetDefHeight(GetID()) / 2) + 1);
					SetAction("FlatUp");
					break;
				}
			}
			return true;
		}
	} else {
		return result;
	}
	return 0;
}