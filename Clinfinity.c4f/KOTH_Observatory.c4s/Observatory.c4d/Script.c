/*-- Observatory --*/

#strict 2

// this is a control point
#include L_CP

local flag;

public func Initialize() {
	SetAction("MakeSound");

	flag = CreateObject(FLAG, 0, 0, NO_OWNER);
	flag->SetAction("FlyBase", this);
}

private func Captured() {
	flag->SetOwner(GetOwner());
	return inherited(...);
}

public func CaptureTime() { return 400; }

public func CaptureZone() {
	// on top of the platform
	return Find_InRect(-65, -150, 121, 30);
}

