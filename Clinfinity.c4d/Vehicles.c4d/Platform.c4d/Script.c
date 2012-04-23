/*-- Plattform --*/

#strict 2

local pLeft, pRight, pMaster;
local iX, iY, iR;

public func IsPlatform() { return true; }
public func GetLeft() { return pLeft; }
public func GetRight() { return pRight; }
public func IsMaster() { return pMaster == this; }
public func GetMaster() { return pMaster; }

protected func Initialize()
{  
  pMaster = 0;
  SetAction("Fly");
}

protected func Flying()
{
  SetSolidMask(0, 11, 72, 3);
  SoundLevel("Propel", 100, this);
  AddEffect("IntFly", this, 10, 1, this, 0);
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
  
  pMaster = FindMaster(this);
}

public func ConnectRight(object pPlatform, bool fSlave) {
  if(!pPlatform->~IsPlatform()) return false;
  if(pPlatform == this) return false;

  pRight = pPlatform;
  if(!fSlave) {
    Sound("Connect");
    pPlatform->ConnectLeft(this, true);
  }

  pMaster = FindMaster(this);
}

public func FindMaster(object pBy) {
  if(pMaster) return pMaster;
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
  pMaster = this;
  SetMaster(pMaster);
  return this;
}

public func SetMaster(object pNewMaster, object pBy) {
  if(!pBy) pBy = this;
  
  pMaster = pNewMaster;
  
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
  SetXDir(0); SetYDir(0);
  SetPosition(GetX(), GetY());
  
 // SetPosition(GetX(), GetY()+Sin(iTime%360, 360, 1000));
  
  if(pMaster) {
		if(iX != GetX() || iY != GetY()) {
			pMaster->CalculatePositions(pTarget);
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
