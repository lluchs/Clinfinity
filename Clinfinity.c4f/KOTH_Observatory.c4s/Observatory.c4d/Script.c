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

// insert Point-Opening functionality here
protected func OpenPoint(){
  SetAction("OpenCupola");
  }
  
protected func LogMsg(){
  Log("Punkt ist voll offen.");
  Sound("cupola_open");
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

