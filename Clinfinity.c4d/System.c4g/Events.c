/*  Script: Events.c
	Provides functions for dispatching effect-based events which can be handled by listeners. */

#strict 2

/*  Function: AddEventListener
	Adds an event listener to the calling object.

	The first parameter specifies the object which will receive calls when an event is emitted.
	All other parameters specify the events which will be handled, optionally as an array which contains the handling
	function as a second parameter.

	Parameters:
	listener - the object receiving event calls
	event... - the event name or an array [event name, function name to call]
	
	Returns:
	The number of listeners which were added. */
global func AddEventListener(object listener/*, event...*/) {
	var i = 1, event, function;
	while(event = Par(i++)) {
		if(GetType(event) == C4V_Array) {
			function = event[1];
			event = event[0];
		} else
			function = event;

		var effectNum = AddEffect("EventListener", this, 1, 0, listener);
		EffectVar(0, this, effectNum) = event;
		EffectVar(1, this, effectNum) = function;
	}
	return i - 2;
}

/*  Function: GetEventListener
	Finds the effect numbers of one or all event listeners (in case on of the parameters is 0).
	
	Parameters:
	listener - the listening object
	event    - the event name

	Returns:
	The effect number or an array of effect numbers. */
global func GetEventListener(object listener, string event) {
	var i = GetEffectCount("EventListener", this), results = [];
	while(i--) {
		var effectNum = GetEffect("EventListener", this, i);
		if(event == 0 || EffectVar(0, this, effectNum) == event) {
			if(listener == 0 || GetEffect(0, this, effectNum, 4) == listener) {
				PushBack(effectNum, results);
			}
		}
	}
	if(GetLength(results) > 1)
		return results;
	else
		return results[0];
}

global func FxEventListenerCall(object target, int effectNum) {
	Call(EffectVar(1, target, effectNum), target, ...);
}

/*  Function: Emit
	Emits the given event.

	The handling function will be passed the calling object and any additional parameters.

	Parameters:
	event - the event name
	... - all other parameters are passed to the handler

	Returns:
	The number of listeners that handled this event. */
global func Emit(string event) {
	var i = GetEffectCount("EventListener", this), listeners = 0;
	while(i--) {
		var effectNum = GetEffect("EventListener", this, i);
		if(EffectVar(0, this, effectNum) == event) {
			EffectCall(this, effectNum, "Call", ...);
			listeners++;
		}
	}
	return listeners;
}
