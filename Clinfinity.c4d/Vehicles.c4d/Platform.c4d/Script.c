/*	Script: Platform
	A controllable, floating platform, powered by steam. */

#strict 2

local pLeft, pRight, master;
local iX, iY, iR;
local controlMediator;

public func IsPlatform() { return true; }
public func GetLeft() { return pLeft; }
public func GetRight() { return pRight; }
public func IsMaster() { return master == this; }
public func GetMaster() { return master; }

/*	Constructor: CreatePlatform
	Factory method for platforms.
	The coordinates are relative to the calling object in local calls, otherwise global.
	*Note:* You should always create a platform using this method.

	Parameters:
	x		- Horizontal coordinate.
	y		- Vertical coordinate.
	owner	- Owner of the created platform: Player index. Use NO_OWNER for ownerless platforms.

	Returns:
	The created platform. */
public func CreatePlatform(int x, int y, int owner) {
	var mediator = CreateObject(COMD, AbsX(3), AbsY(3), owner);
	var platform = CreateObject(PLTF, x, y, owner);
	platform->LocalN("controlMediator") = mediator;
	platform->LocalN("master") = 0;
	platform->SetAction("Fly");
	var lever = COLV->CreateLever(platform->GetX() - 35, platform->GetY() - 3, mediator);
	mediator->LocalN("controlledPlatform") = platform;
	mediator->LocalN("controlLever") = lever;
	return platform;
}

protected func Flying()
{
  SetSolidMask(0, 4, 90, 4);
  AddEffect("IntFly", this, 10, 1, this, 0);
}

protected func ContactLeft() {
	FloatStop();
}

protected func ContactRight() {
	FloatStop();
}

protected func ContactTop() {
	FloatStop();
}

protected func ContactBottom() {
	FloatStop();
}

/*	Section: Control */

/*	Function: ControlEvent
	Event handler for control events.

	Parameters:
	direction	- Direction of control, specified by one of the COMD_* constants.
	source		- Source of the event.

	See Also:
	<Control mediator> */
public func ControlEvent(int direction, object source) {
	if(direction == COMD_Stop) {
		FloatStop();
	} else if(direction == COMD_Up) {
		FloatUp();
	} else if(direction == COMD_Down) {
		FloatDown();
	}
}

private func FloatStop() {
	SetComDir(COMD_None);
	SetYDir(0);
	controlMediator->MovementEvent(COMD_Stop, this);
}

private func FloatUp() {
	SetComDir(COMD_Up);
	controlMediator->MovementEvent(COMD_Up, this);
}

private func FloatDown() {
	SetComDir(COMD_Down);
	controlMediator->MovementEvent(COMD_Down, this);
}

/* Master/Slave-System */

public func ConnectLeft(object pPlatform, bool fSlave) {
  if(!pPlatform->~IsPlatform()) return false;
  if(pPlatform == this) return false;

  pLeft = pPlatform;
  if(!fSlave) {
    Sound("Connect");
    pPlatform->ConnectRight(this, true);
  }

  master = FindMaster(this);
}

public func ConnectRight(object pPlatform, bool fSlave) {
  if(!pPlatform->~IsPlatform()) return false;
  if(pPlatform == this) return false;

  pRight = pPlatform;
  if(!fSlave) {
    Sound("Connect");
    pPlatform->ConnectLeft(this, true);
  }

  master = FindMaster(this);
}

public func FindMaster(object pBy) {
  if(master) return master;
  if(!pBy) pBy = this;

  var pResult = 0;

  //Von links kommend müssen wir nach rechts
  if(pRight && (this == pBy || pLeft == pBy)) {
    pResult = pRight->FindMaster(this);
  }
  //Von rechts kommend müssen wir nach links
  if(pLeft && (this == pBy || pRight == pBy)) {
    pResult = pLeft->FindMaster(this);
  }

  if(pResult) return pResult;

  //Keiner? Dann sind wir der neue
  master = this;
  SetMaster(master);
  return this;
}

public func SetMaster(object pNewMaster, object pBy) {
  if(!pBy) pBy = this;

  master = pNewMaster;

  //Von links kommend müssen wir nach rechts
  if(pRight && (this == pBy || pLeft == pBy)) {
    pRight->SetMaster(pNewMaster, this);
  }
  //Von rechts kommend müssen wir nach links
  if(pLeft && (this == pBy || pRight == pBy)) {
    pLeft->SetMaster(pNewMaster, this);
  }

  return true;
}

public func ConnectedTo(object pPlatform, object pBy) {
  if(pPlatform == this) return true;
  if(!pBy) pBy = this;

  var fResult = false;

  //Von links kommend müssen wir nach rechts
  if(pRight && (this == pBy || pLeft == pBy)) {
    fResult = pRight->ConnectedTo(pPlatform, this);
  }
  //Von rechts kommend müssen wir nach links
  if(pLeft && (this == pBy || pRight == pBy)) {
    fResult = pLeft->ConnectedTo(pPlatform, this);
  }

  return fResult;
}

/* Physics */

public func CalculatePositions() {
  var i, pPlatform;
  pPlatform = this;
  i = 1;
  while(pPlatform) {
    pPlatform = pPlatform->GetLeft();
    if(!pPlatform) break;
    SetPosition(GetX()-i*GetDefWidth(), GetY(), pPlatform);
    pPlatform->SavePosition();
    i++;
  }
  pPlatform = this;
  i = 1;
  while(pPlatform) {
    pPlatform = pPlatform->GetRight();
    if(!pPlatform) break;
    SetPosition(GetX()+i*GetDefWidth(), GetY(), pPlatform);
    pPlatform->SavePosition();
    i++;
  }
}

public func SavePosition() {
  iX = GetX();
  iY = GetY();
  iR = GetR();
}

public func FxIntFlyTimer(object pTarget, int iNr, int iTime)
{
  //SetXDir(0); SetYDir(0);
  //SetPosition(GetX(), GetY());

 // SetPosition(GetX(), GetY()+Sin(iTime%360, 360, 1000));

  if(master) {
		if(iX != GetX() || iY != GetY()) {
			master->CalculatePositions(pTarget);
		}
	}
	//SavePosition();

  var pWeights = FindObjects(Find_Not(Find_Func("IsPlatform")), Find_OnLine(-GetDefWidth()/2, -GetDefHeight()/2-5, GetDefWidth()/2, -GetDefHeight()/2-5));
  var iMass = 0;
  for(var pWeight in pWeights) {
    iMass += GetMass(pWeight);
    Message("%d", pWeight, GetMass(pWeight));
  }
  Message("%d", this, iMass);

  return true;
}
