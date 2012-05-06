/*-- Pilot --*/

#strict 2
#include CLNK

/* Itemlimit */
public func MaxContentsCount() { return 3; }

/* Inhalt durchwechseln */
protected func ControlSpecial() { ShiftContents(); }
