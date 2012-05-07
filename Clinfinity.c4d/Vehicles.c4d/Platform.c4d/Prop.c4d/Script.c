/*	Script: Prop
	Attached to the underside of platforms.
	Generates enough force to make platforms fly! */

#strict 2

local controlMediator;

/*	Constructor: CreateProp
	Factory method for propellers.
	The coordinates are relative to the calling object in local calls, otherwise global.
	*Note:*	You should always create a prop using this method.

	Parameters:
	x			- Horizontal coordinate.
	y			- Vertical coordinate.
	forPlatform	- The platform this prop attaches to.
	mediator	- The control mediator this prop sends its events to.

	Returns:
	The created prop. */
public func CreateProp(int x, int y, object forPlatform, object mediator) {
	var prop = CreateObject(PROP, x, y, forPlatform->GetOwner());
	prop->LocalN("controlMediator") = mediator;
	prop->AttachTo(forPlatform);
	return prop;
}

protected func CheckCollision() {
	if(Stuck()) {
		controlMediator->ControlEvent(COMD_Stop, this);
	}
}