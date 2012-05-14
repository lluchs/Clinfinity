/*-- Frigate --*/

#strict 2

// this is a control point
#include L_CP

local flag;

public func Initialize() {
	SetAction("MakeSound");

	flag = CreateObject(FLAG);
	flag->SetAction("FlyBase", this);
}

private func CaptureMsg() {
	flag->SetOwner(GetOwner());
	return inherited(...);
}

public func CaptureTime() { return 1000; }

public func CaptureZone() {
	// on top of the platform, between the cannons
	return Find_InRect(-76, -34, 168, 57);
}

