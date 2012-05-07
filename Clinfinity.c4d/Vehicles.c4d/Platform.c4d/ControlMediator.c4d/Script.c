/*	Script: Control mediator
	Encapsulates the interactions between levers and platforms.*/

#strict 2

local controlLever, controlledPlatform;
local masterMediator, leftSlaveMediator, rightSlaveMediator;

/*	Function: ControlEvent
	Called by the source when it wants to broadcast a control event.
	This can be, for example, a lever that has been switched into its "up" position.

	Parameters:
	direction	- Direction of control. Must be one of the COMD_* constants.
	source		- Source of the event. */
public func ControlEvent(int direction, object source) {
	if(masterMediator == 0) {
		controlledPlatform->ControlEvent(direction, this);
		ControlEventToSlaves(direction);
	} else {
		if(source == masterMediator) {
			controlledPlatform->ControlEvent(direction, this);
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
		controlLever->MovementEvent(direction, this);
		MovementEventToSlaves(direction);
	} else {
		if(source == masterMediator) {
			controlLever->MovementEvent(direction, this);
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
	}
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
	}
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