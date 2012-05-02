#strict 2

local controlledPlatform;

/** Factory method for levers */
public func CreateLever(int x, int y, object forPlatform) {
	var lever = CreateObject(COLV, x, y, forPlatform->GetOwner());
	lever->	LocalN("controlledPlatform") = forPlatform;
	lever->SetAction("Control");
}

protected func MouseSelection(int player) {
	Sound("Ding");
}

protected func ControlUp(object controller) {
	if(controlledPlatform->GetComDir() == COMD_Down) {
		controlledPlatform->FloatStop();
		SetDir(0);
	} else if(controlledPlatform->GetComDir() == COMD_Stop) {
		controlledPlatform->FloatUp();
		SetDir(1);
	} else {
		return false;
	}
	Sound("SignalClick", false, this, 15);
	return true;
}

protected func ControlDownSingle(object controller) {
	if(controlledPlatform->GetComDir() == COMD_Up) {
		controlledPlatform->FloatStop();
		SetDir(0);
	} else if(controlledPlatform->GetComDir() == COMD_Stop) {
		controlledPlatform->FloatDown();
		SetDir(2);
	} else {
		return false;
	}
	Sound("SignalClick", false, this, 15);
	return true;
}