/*-- Battery --*/

#strict 2
#include B_40
#include PWRG // Ist Energie"produzent"
#include PWRC // ...und Energie"verbraucher"
#include DACT // Damagecontrol
#include L_CA // Baugrafik

// Es mangelt uns nicht an Energie
private func FxEnergyNeedStart() {
	return -1;
}

public func GetGeneratorPriority() {
	return 1;
}

public func IsBattery() {
	return true;
}

local Capacity;

func Initialize() {
  SetCapacity(1500); 
  return(1);
}

public func Deconstruction() {
	//Overlay löschen
	SetGraphics(0, this, 0, 1);
	
	return _inherited(...);
}

public func SetPower() {
	_inherited(...);
	UpdateDisplay();
	return true;
}

public func DoPower() {
	_inherited(...);
	UpdateDisplay();
	return true;
}

private func UpdateDisplay() {
	SetGraphics(0, this, _BAR, 1, GFXOV_MODE_Action, Format("%d/5", GetPower() * 5 / GetCapacity()));
	SetObjDrawTransform(1000, 0, -13500, 0, 1000, 1000, this, 1);
	return 1;
}

// Get- Funktionen
public func GetCapacity() {
  return Capacity;
}

// Set- Funktionen
public func SetCapacity(int NewCapacity) {
  Capacity = NewCapacity;
  UpdateDisplay();
  return true;
}

func Act() { // Timercall
	if(!(GetOCF() & OCF_Fullcon))
		return;
  // Energie saugen
  var iEnergy = BoundBy(GetCapacity() - GetPower(), 0, 10);
  if(iEnergy && CheckPower(iEnergy)) {
  	DoPower(iEnergy);
  }
  return 1;
}

//Steuerung
  
protected func ControlUp() {
	Message("$TxtStored$", this, GetPower());
}

protected func ControlDig(object pClonk) {
	var pObj = FindObject2(Find_Container(pClonk), Find_ID(CELL));
	if(pObj) {
		if(GetPower() >= 200) {
			DoPower(-200);
			pObj -> ChangeDef(FCLL);
			Message("$TxtCharged$", this);
		}
		else
			Message("$TxtNotEnoughPower$", this);
	} else {
		Message("$TxtNoCell$", this);
	}
	return 1;
}

protected func ControlThrow(object pClonk) {
	var pObj = FindObject2(Find_Container(pClonk), Find_ID(FCLL));
	if(pObj) {
		if(GetPower() < GetCapacity()) {
			DoPower(200);
			pObj -> ChangeDef(CELL);
			pObj -> Enter(pClonk);
			Message("$TxtAssign$", this);
		}
	} else {
		Message("$TxtNoChargedCell$", this);
	}
	return 1;
}

/* Erforschbar */

public func MarsResearch() {
	return true;
}

public func MaxDamage() { return 25; } //Maximaler Schaden

/* Anzeige-fix nach Spielstand laden */

func UpdateTransferZone()
{
	UpdateDisplay();
}
