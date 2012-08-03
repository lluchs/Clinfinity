/*-- Truhe --*/

#strict

local fLocked; // Wenn true, lässt sich die Truhe nicht ohne weiteres öffnen

protected func Initialize()
  {
  SetAction("Closed");
  return(1);
  }
  
protected func ChestSound() { Sound("ChestOpening"); }

protected func RejectCollect(id idObj, object pObj)
  {
  // Gegenstände nur annehmen, wenn die Truhe offen ist
  return (GetAction() ne "Open");
  }
  
protected func ControlUp(object pClonk)
  {
  [$DescOpen$]
  if (GetAction() ne "Closed") return();
  if (!fLocked) return(SetAction("Opening"));
  Message("$MsgLocked$", this());
  Sound("Click");
  }
  
protected func ControlThrow(object pClonk)
  {
  [$DescTake$]
  if (GetAction() eq "Closed") { return(true, ControlUp(pClonk)); }
  // Keine Überladung: Normales Nehmen ausführen
  }

protected func ControlLeft(object pClonk)
  {
  [$DescTake$]
  if (GetAction() eq "Closed") { return(true, ControlUp(pClonk)); }
  // Objekte herausnehmen
  return(SetCommand(pClonk, "Get", this(), 0,0, 0, 1));
  }

protected func ControlRight(object pClonk)
  {
  [$DescTake$]
  if (GetAction() eq "Closed") { return(true, ControlUp(pClonk)); }
  // Objekte herausnehmen
  return(SetCommand(pClonk, "Get", this(), 0,0, 0, 1));
  }
  
protected func ControlDownSingle(object pClonk)
  {
  [$DescClose$]
  if (GetAction() eq "Open") return(SetAction("Closing"));
  }
  
protected func ControlDig(object pClonk)
  {
  [$DescTake$]
  // Kiste noch zu: Öffnen
  if (GetAction() eq "Closed") { return(true, ControlUp(pClonk)); }
  // Objekte herausnehmen
  return(SetCommand(pClonk, "Get", this(), 0,0, 0, 1));
  }
  
