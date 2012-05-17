/*	Script: Structure base object
	Automatically handles attaching to and detaching from platforms. */

#strict 2

protected func Initialize() {
	var result = _inherited();
	var platform = FindObject2(Find_ID(PLTF), Find_AtPoint(0, GetObjHeight() / 2 + 1));
	if(platform != 0) {
		AttachTo(platform);
	}
	return result;
}

protected func Destruction() {
	var result = _inherited();
	if(GetProcedure() == "ATTACH") {
		GetActionTarget()->~AttachEvent(this, GetActionTarget(), true, this);
	}
	return result;
}

/*	Function: AttachEvent
	Event handler, called after an object was attached to a new parent object or detached from it.
	Buildings by default hand the event to their action target, if they are attached to something.

	Parameters:
	attached	- The attached object.
	attachedTo	- The new parent object.
	isDetaching	- *true* if the object was detached. *false* if it was attached.
	source		- Source of the event. */
public func AttachEvent(object attached, object attachedTo, bool isDetaching, object source) {
	if(GetProcedure() == "ATTACH") {
		GetActionTarget()->~AttachEvent(attached, attachedTo, isDetaching, this);
	}
}
