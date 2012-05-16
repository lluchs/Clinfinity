/*-- Pilot --*/

#strict 2
#include CLNK

/* Itemlimit */
public func MaxContentsCount() { return 3; }

/* Inhalt durchwechseln */
protected func ControlSpecial() { ShiftContents(); }

protected func Death(int killedBy) {
	/* Respawn */
	var tank = FindObject2(Find_ID(STMT), Find_Allied(GetOwner()));
	if(tank) {
		var new = CreateObject(GetID(), 0, 0, GetOwner());
		new->GrabObjectInfo(this);
		new->Enter(tank);
	}

	// try to award a hat
	if(Hostile(GetController(), killedBy)) {
		var clonk = GetCursor(killedBy);
		if(clonk)
			clonk->AddHat(C4Id(Format("HAT%d", RandomX(1, 5))));
	}
	return _inherited(killedBy, ...);
}
