/*-- Pilot --*/

#strict 2
#include CLNK

/* Itemlimit */
public func MaxContentsCount() { return 3; }

/* Inhalt durchwechseln */
protected func ControlSpecial() { ShiftContents(); }

/* Respawn */
protected func Death() {
	var tank = FindObject2(Find_ID(STMT), Find_Allied(GetOwner()));
	if(tank) {
		var new = CreateObject(GetID(), 0, 0, GetOwner());
		new->GrabObjectInfo(this);
		new->Enter(tank);
	}
	return _inherited(...);
}
