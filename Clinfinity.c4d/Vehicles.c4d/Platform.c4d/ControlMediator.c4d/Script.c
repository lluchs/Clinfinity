/*	Script: Control mediator
	Encapsulates the interactions between platforms and the event flow for control events and movement events between platforms, levers, and possibly other objects.

	See Also:
	General information on the <mediator pattern at http://en.wikipedia.org/wiki/Mediator_pattern>. */

#strict 2

local controlledPlatform;
local controlEventListeners, movementEventListeners;
local masterMediator, leftSlaveMediator, rightSlaveMediator;

protected func Initialize() {
	controlEventListeners = [];
	movementEventListeners = [];
}

/*	Function: ControlEvent
	Called by the source when it wants to broadcast a control event.
	This can be, for example, a lever that has been switched into its "up" position.

	Parameters:
	direction	- Direction of control. Must be one of the COMD_* constants.
	source		- Source of the event. */
public func ControlEvent(int direction, object source) {
	if(masterMediator == 0) {
		ControlEventToListeners(direction, this);
		ControlEventToSlaves(direction);
	} else {
		if(source == masterMediator) {
			ControlEventToListeners(direction, this);
			ControlEventToSlaves(direction);
		} else {
			masterMediator->ControlEvent(direction, this);
		}
	}
}

private func ControlEventToSlaves(int direction) {
	if(leftSlaveMediator != 0) {
		leftSlaveMediator->ControlEvent(direction, this);
	}
	if(rightSlaveMediator != 0) {
		rightSlaveMediator->ControlEvent(direction, this);
	}
}

/*	Function: MovementEvent
	Called by the source when it wants to broadcast its movement status.
	Platforms use this when they start or stop moving to notify levers, for example.

	Parameters:
	direction	- Direction of movement. Must be one of the COMD_* constants.
	source		- Source of the event. */
public func MovementEvent(int direction, object source) {
	if(masterMediator == 0) {
		MovementEventToListeners(direction, this);
		MovementEventToSlaves(direction);
	} else {
		if(source == masterMediator) {
			MovementEventToListeners(direction, this);
			MovementEventToSlaves(direction);
		} else {
			masterMediator->MovementEvent(direction, this);
		}
	}
}

private func MovementEventToSlaves(int direction) {
	if(leftSlaveMediator != 0) {
		leftSlaveMediator->MovementEvent(direction, this);
	}
	if(rightSlaveMediator != 0) {
		rightSlaveMediator->MovementEvent(direction, this);
	}
}

/*	Function: AddControlEventListener
	Adds a listener for control events.
	The control mediator will call the method ControlEvent() in each listener when a control event arrives.

	Parameters:
	listener	- The listener to add. */
public func AddControlEventListener(object listener) {
	if(listener != 0 && !InArray(listener, controlEventListeners)) {
		PushBack(listener, controlEventListeners);
	}
}

/*	Function: AddMovementEventListener
	Adds a listener for movement events.
	The control mediator will call the method MovementEvent() in each listener when a movement event arrives.

	Parameters:
	listener	- The listener to add. */
public func AddMovementEventListener(object listener) {
	if(listener != 0 && !InArray(listener, movementEventListeners)) {
		PushBack(listener, movementEventListeners);
	}
}

/*	Function: RemoveControlEventListener
	Removes a listener for control events.

	Parameters:
	listener	- The listener to remove. */
public func RemoveControlEventListener(object listener) {
	RemoveElement(listener, controlEventListeners);
}

/*	Function: RemoveMovementEventListener
	Removes a listener for movement events.

	Parameters:
	listener	- The listener to remove. */
public func RemoveMovementEventListener(object listener) {
	RemoveElement(listener, movementEventListeners);
}

private func ControlEventToListeners(int direction, object source) {
	for(var listener in controlEventListeners) {
		listener->ControlEvent(direction, source);
	}
}

private func MovementEventToListeners(int direction, object source) {
	for(var listener in movementEventListeners) {
		listener->MovementEvent(direction, source);
	}
}

/*	Section: Master/slave system
	Methods that concern the tree-like structure of connected control mediators.
	Each mediator can have a left and a right slave mediator, it then acts as the master to these slaves.
	Likewise, the slave mediators can have slaves themselves.
	This way, an entire tree of mediators can be built.

	Each control event and movement event that is fired in a slave gets routed to its master.
	This is repeated until the event reaches the "absolute" master at the top of the tree.
	From there, the event gets routed back down in the tree, and additionally, to all listeners for the respective event.
	This way, any event fired anywhere in the tree can be received by any listener in the tree. */

/*	Function: SetLeftSlave
	Sets the left slave mediator.

	Parameters:
	mediator	- The new slave.

	Returns:
	*true* if the mediator could be made a slave, *false* otherwise. */
public func SetLeftSlave(object mediator) {
	if(HasLeftSlave() || mediator->HasRightSlave()) {
		return false;
	}
	if(!mediator->SetMasterFromRight(this)) {
		return false;
	}
	leftSlaveMediator = mediator;
	return true;
}

/*	Function: SetRightSlave
	Sets the right slave mediator. See <SetLeftSlave> for details. */
public func SetRightSlave(object mediator) {
	if(HasRightSlave() || mediator->HasLeftSlave()) {
		return false;
	}
	if(!mediator->SetMasterFromLeft(this)) {
		return false;
	}
	rightSlaveMediator = mediator;
	return true;
}

private func SetMasterFromRight(object newMasterMediator) {
	// TODO: Sanity checks
	SetObjectOrder(newMasterMediator->GetControlledPlatform(), controlledPlatform);
	// Already slave to another mediator to the left: Restructure.
	if(masterMediator != 0) {
		masterMediator->SetMasterFromRight(this);
		leftSlaveMediator = masterMediator;
	}
	rightSlaveMediator = 0;
	masterMediator = newMasterMediator;
	controlledPlatform->SetAction("FlySlave", masterMediator->GetControlledPlatform());
	controlledPlatform->SetActionData(256 * 1 + 0);
	return true;
}

private func SetMasterFromLeft(object newMasterMediator) {
	// TODO: Sanity checks
	SetObjectOrder(newMasterMediator->GetControlledPlatform(), controlledPlatform);
	// Already slave to another mediator to the left: Restructure.
	if(masterMediator != 0) {
		masterMediator->SetMasterFromLeft(this);
		rightSlaveMediator = masterMediator;
	}
	leftSlaveMediator = 0;
	masterMediator = newMasterMediator;
	controlledPlatform->SetAction("FlySlave", masterMediator->GetControlledPlatform());
	controlledPlatform->SetActionData(256 * 0 + 1);
	return true;
}

private func HasLeftSlave() {
	return leftSlaveMediator != 0;
}

private func HasRightSlave() {
	return rightSlaveMediator != 0;
}

private func GetControlledPlatform() {
	return controlledPlatform;
}