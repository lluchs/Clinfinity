/*	Script: Control lever
	Floating platforms are controlled with this lever. */

#strict 2

local controlMediator;
local gearStop, gearUp, gearDown;

/*	Constructor: CreateLever
	Factory method for levers.
	The coordinates are relative to the calling object in local calls, otherwise global.
	*Note:*	You should always create a lever using this method.

	Parameters:
	x			- Horizontal coordinate.
	y			- Vertical coordinate.
	forMediator	- The control mediator this lever sends its events to.

	Returns:
	The created lever. */
public func CreateLever(int x, int y, object forMediator) {
	var lever = CreateObject(COLV, x, y, forMediator->GetOwner());
	lever->LocalN("controlMediator") = forMediator;
	lever->SetAction("Control");
	return lever;
}

protected func Initialize() {
	gearStop = 0;
	gearUp = 1;
	gearDown = 2;
}

protected func MouseSelection(int player) {
	Sound("Ding");
}

protected func ControlUp(object controller) {
	if(GetDir() == gearDown) {
		controlMediator->ControlEvent(COMD_Stop, this);
	} else if(GetDir() == gearStop) {
		controlMediator->ControlEvent(COMD_Up, this);
	} else {
		return false;
	}
	return true;
}

protected func ControlDownSingle(object controller) {
	if(GetDir() == gearUp) {
		controlMediator->ControlEvent(COMD_Stop, this);
	} else if(GetDir() == gearStop) {
		controlMediator->ControlEvent(COMD_Down, this);
	} else {
		return false;
	}
	return true;
}

public func MovementEvent(int direction, object source) {
	var oldDirection = GetDir();
	if(direction == COMD_Stop) {
		SetDir(gearStop);
	} else if(direction == COMD_Up) {
		SetDir(gearUp);
	} else if(direction == COMD_Down) {
		SetDir(gearDown);
	}
	if(GetDir() != oldDirection) {
		Sound("lever", false, this, 15);
	}
}