/*	Script: Control mediator
	Encapsulates the interactions between levers and platforms.*/

#strict 2

local controlLever, controlledPlatform;
local masterMediator, slaveMediators;

protected func Initialize() {
	slaveMediators = [];
}

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
		// Event vom Master: An Plattform geben
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
		// Event vom master: An Hebel geben
		// Nicht vom Master: An Master geben
}