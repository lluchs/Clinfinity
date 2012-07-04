/*	Script: Structure base object
	Automatically handles attaching to and detaching from platforms.
	All structures created for Clonk Infinity should adhere to the following rules:
	- Provide an action with the Procedure "ATTACH".
	  It can either be named "Attach", it will then get set automatically by AttachTo(),
	  or it can have any other name, in which case you must set the action yourself in your structure's Initialize() function.
	- Inherit from this base object.
	- Have no bottom vertices the structure stands on.
	  Otherwise, when moving, these vertices get stuck in the platform's solid masks.
	  Other vertices, e.g. at the top, are allowed. Set the proper CNAT values for those. */

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

protected func RejectConstruction(int x, int y, object clonk) {
	// only allow building on platform
	if(!FindObject2(Find_ID(PLTF), clonk->Find_AtPoint(x, y + 1))) {
		Sound("Error");
		Message("$OnlyOnPlatform$", clonk);
		return true;
	}
}

/*	Function: AttachEvent
	Event handler, called after an object was attached to a new parent object or detached from it.
	Buildings by default hand the event to their action target, if they are attached to something.
	This makes sure platforms (and, in turn, the master platform) get notified of attached/detached objects and can react appropriately.

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
