/*	Script: Control mediator
	Encapsulates the interactions between platforms and the event flow for control events and movement events between platforms, levers, and possibly other objects.

	See Also:
	General information on the <mediator pattern at http://en.wikipedia.org/wiki/Mediator_pattern>. */

#strict 2

local controlledPlatform;
local controlEventListeners, movementEventListeners;
local masterMediator, slaveMediator;

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
	} else {
		if(source == masterMediator) {
			ControlEventToListeners(direction, this);
		} else {
			masterMediator->ControlEvent(direction, this);
		}
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
	} else {
		if(source == masterMediator) {
			MovementEventToListeners(direction, this);
		} else {
			masterMediator->MovementEvent(direction, this);
		}
	}
}

/*	Function: AttachEvent
	Event handler, called after an object was attached to a new parent object or detached from it.

	Parameters:
	attached	- The attached object.
	attachedTo	- The new parent object.
	isDetaching	- *true* if the object was detached. *false* if it was attached.
	source		- Source of the event. */
public func AttachEvent(object attached, object attachedTo, bool isDetaching, object source) {
	if(masterMediator == 0) {
		if(isDetaching) {
			GetControlledPlatform()->RemoveCopiedChildrenVertices(attached);
		} else {
			GetControlledPlatform()->CopyChildrenVertices(attached);
		}
	} else {
		if(source == masterMediator) {
			if(isDetaching) {
				GetControlledPlatform()->RemoveCopiedChildrenVertices(attached);
			} else {
				GetControlledPlatform()->CopyChildrenVertices(attached);
			}
		} else {
			masterMediator->AttachEvent(attached, attachedTo, isDetaching, source);
		}
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
	Methods that concern the list-like structure of connected control mediators.
	If the mediators of two adjacent platforms are connected, the mediator of the left platform acts as the master of the right platform's mediator.
	An arbitrary number of mediators can be connected this way, forming a doubly linked list.

	Each event that is fired in a slave gets handed to its master.
	This is repeated until the event reaches the "absolute" master at the left of the connected mediators.
	From there, the event gets routed back to the right, and additionally, to all listeners for the respective event.
	This way, an event fired anywhere in the list can be received by any listener in the list. */

/*	Function: Connect
	Connects the control mediators of two adjacent platforms.
	The left mediator becomes the master of the right mediator.

	Parameters:
	leftMediator	- Mediator of the left platform.
	rightMediator	- Mediator of the right platform.

	Returns:
	*true* if the mediators could successfully be connected, *false* otherwise. */
public func Connect(object leftMediator, object rightMediator) {
	if(leftMediator->HasSlave() || rightMediator->HasMaster()) {
		return false;
	}
	leftMediator->SetSlave(rightMediator);
	rightMediator->SetMaster(leftMediator);

	var leftPlatform = leftMediator->GetControlledPlatform();
	var rightPlatform = rightMediator->GetControlledPlatform();
	rightPlatform->SetAction("FlySlave", leftPlatform);
	rightPlatform->AttachTo(leftPlatform, 1, 2);
	rightPlatform->RemoveCopiedChildrenVertices();

	return true;
}

public func Disconnect(object leftMediator, object rightMediator) {
	if(!leftMediator->IsMasterOf(rightMediator) || !rightMediator->IsSlaveOf(leftMediator)) {
		return false;
	}
	leftMediator->RemoveSlave();
	rightMediator->RemoveMaster();

	var leftPlatform = leftMediator->GetControlledPlatform();
	var rightPlatform = rightMediator->GetControlledPlatform();

	rightPlatform->SetAction("Fly");

	leftMediator->AttachEvent(rightPlatform, leftPlatform, true, this);
	return true;
}

private func HasMaster() {
	return masterMediator != 0;
}

private func HasSlave() {
	return slaveMediator != 0;
}

private func IsMasterOf(object mediator) {
	return slaveMediator == mediator;
}

private func IsSlaveOf(object mediator) {
	return masterMediator == mediator;
}

private func SetMaster(object newMaster) {
	masterMediator = newMaster;
}

private func RemoveMaster() {
	masterMediator = 0;
}

private func SetSlave(object newSlave) {
	AddControlEventListener(newSlave);
	AddMovementEventListener(newSlave);
	slaveMediator = newSlave;
}

private func RemoveSlave() {
	if(slaveMediator != 0) {
		RemoveControlEventListener(slaveMediator);
		RemoveMovementEventListener(slaveMediator);
		slaveMediator = 0;
	}
}

private func GetControlledPlatform() {
	return controlledPlatform;
}