/*-- Frigate --*/

#strict 2

// this is a control point
#include L_CP

local flag;
local t;

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
	// on top of the platform, between the cannons
	return Find_InRect(-76, -34, 168, 57);
}

// pendle around, no solidmask though (yet)
public func Pendle() {
  t++;
  var r = Cos(t, 100);
  var fsin=Sin(r, 1000, 100);
  var fcos=Cos(r, 1000, 100);
  SetObjDrawTransform (
    +fcos, +fsin, (1000-fcos) - fsin,
    -fsin, +fcos, (1000-fcos) + fsin
  );
}