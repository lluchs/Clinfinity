/*-- Frigate --*/

#strict 2

// this is a control point
#include L_CP

public func Initialize() {
	SetAction("MakeSound");
}

public func CaptureTime() { return 1000; }

public func CaptureZone() {
	// on top of the platform, between the cannons
	return Find_InRect(-76, -34, 168, 57);
}

