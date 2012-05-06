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
	controlledPlatform->ControlEvent(direction, source);
	// Kein master: Event an plattform leiten
	// Master vorhanden
		// Event vom Master: An Plattform und Slaves geben
		// Nicht vom Master: An Master geben
}

/*	Function: MovementEvent
	Called by the source when it wants to broadcast its movement status.
	Platforms use this when they start or stop moving to notify levers, for example.

	Parameters:
	direction	- Direction of movement. Must be one of the COMD_* constants.
	source		- Source of the event. */
public func MovementEvent(int direction, object source) {
	controlLever->MovementEvent(direction, source);
	// Kein master: Event an Hebel leiten
	// Master vorhanden
		// Event vom master: An Hebel und Slaves geben
		// Nicht vom Master: An Master geben
}

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