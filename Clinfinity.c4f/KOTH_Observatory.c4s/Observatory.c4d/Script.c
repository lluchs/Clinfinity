/*-- Observatory --*/

#strict 2

public func SetupTime() { return 800; }

// this is a control point
#include L_CP

local flag;

public func Initialize() {
	SetAction("MakeSound");

	flag = CreateObject(FLAG, 0, 0, NO_OWNER);
	flag->SetAction("FlyBase", this);
}

public func EnablePoint() {
	if(inherited(...))
		SetAction("OpenCupola");
}

protected func LogMsg() {
	Log("$PointOpen$");
	Sound("Status");
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

