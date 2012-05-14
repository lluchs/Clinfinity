/*-- Frigate --*/

#strict 2

// this is a control point
#include L_CP

local flag;

public func Initialize() {
	SetAction("MakeSound");
}

private func CaptureMsg() {
	if(!flag) {
		flag = CreateObject(FLAG);
		flag->SetAction("FlyBase", this);
	}
	flag->SetOwner(GetOwner());
}

public func CaptureTime() { return 1000; }

public func CaptureZone() {
	// on top of the platform, between the cannons
	return Find_InRect(-76, -34, 168, 57);
}

