#strict 2

local controlledPlatform;

/** Factory method for levers */
public func CreateLever(int x, int y, object forPlatform) {
	var lever = CreateObject(_CLV, x, y, forPlatform->GetOwner());
	lever->	LocalN("controlledPlatform") = forPlatform;
}

protected func MouseSelection(int player) {
	Sound("Ding");
}

protected func ControlUp(object controller) {
	if(controlledPlatform->GetComDir() == COMD_Down) {
		controlledPlatform->FloatStop();
	} else {
		controlledPlatform->FloatUp();
	}
	return true;
}

protected func ControlDownSingle(object controller) {
	if(controlledPlatform->GetComDir() == COMD_Up) {
		controlledPlatform->FloatStop();
	} else {
		controlledPlatform->FloatDown();
	}
	return true;
}