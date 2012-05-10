/*	Script: Platform
	A controllable, floating platform, powered by steam. */

#strict 2

local controlMediator;

/*	Constructor: CreatePlatform
	Factory method for platforms.
	The coordinates are relative to the calling object in local calls, otherwise global.
	*Note:* You should always create a platform using this method.

	Parameters:
	x		- Horizontal coordinate.
	y		- Vertical coordinate.
	owner	- Owner of the created platform: Player index. Use NO_OWNER for ownerless platforms.

	Returns:
	The created platform. */
public func CreatePlatform(int x, int y, int owner) {
	var mediator = CreateObject(COMD, AbsX(3), AbsY(3), owner);
	var platform = CreateObject(PLTF, x, y, owner);
	platform->LocalN("controlMediator") = mediator;
	platform->SetAction("Fly");
	var lever = COLV->CreateLever(platform->GetX() - 35, platform->GetY() - 3, mediator);
	lever->AttachTo(platform);
	mediator->LocalN("controlledPlatform") = platform;
	var prop = PROP->CreateProp(platform->GetX(), platform->GetY() + 12, platform);
	platform->CopyVertices(prop);
	mediator->AddControlEventListener(platform);
	mediator->AddMovementEventListener(lever);
	mediator->AddMovementEventListener(prop);
	return platform;
}

/*	Function: IsPlatform
	Marks objects to have platform-like capabilities. */
public func IsPlatform() { return true; }

protected func Flying() {
  SetSolidMask(0, 4, 90, 4);
}

protected func ContactLeft() {
	FloatStop();
}

protected func ContactRight() {
	FloatStop();
}

protected func ContactTop() {
	FloatStop();
}

protected func ContactBottom() {
	FloatStop();
}

/*	Section: Control */

private func GetControlMediator() { return controlMediator; }

/*	Function: ControlEvent
	Event handler for control events.

	Parameters:
	direction	- Direction of control, specified by one of the COMD_* constants.
	source		- Source of the event.

	See Also:
	<Control mediator> */
public func ControlEvent(int direction, object source) {
	if(direction == COMD_Stop) {
		FloatStop();
	} else if(direction == COMD_Up) {
		FloatUp();
	} else if(direction == COMD_Down) {
		FloatDown();
	}
}

private func FloatStop() {
	SetComDir(COMD_None);
	SetYDir(0);
	controlMediator->MovementEvent(COMD_Stop, this);
}

private func FloatUp() {
	SetComDir(COMD_Up);
	controlMediator->MovementEvent(COMD_Up, this);
}

private func FloatDown() {
	SetComDir(COMD_Down);
	controlMediator->MovementEvent(COMD_Down, this);
}

/*	Section: Master/Slave system */

/*	Function: SetLeftSlave
	Connects a platform to the left of this platform.
	When connected, both platforms will move in sync and can be controlled from either control lever.

	Parameters:
	platform	- The platform to connect.
	
	Returns:
	*true* if connecting the platforms was successful, *false* otherwise. */
public func SetLeftSlave(object platform) {
	if(platform == 0 || platform == this || !platform->~IsPlatform()) {
		return false;
	}
	return controlMediator->SetLeftSlave(platform->GetControlMediator());
}

/*	Function: SetRightSlave
	Connects a platform to the right of this platform.
	See <SetLeftSlave> for details. */
public func SetRightSlave(object platform) {
	if(platform == 0 || platform == this || !platform->~IsPlatform()) {
		return false;
	}
	return controlMediator->SetRightSlave(platform->GetControlMediator());
}