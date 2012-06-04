/*-- Lagerfeuer --*/

#strict

protected func ControlThrow(caller){
	// get player
	var plr = caller->GetOwner();

	if(MatSysGetTeamFill(plr, WOOD) >= 1) {
		MatSysDoTeamFill(-1, plr, WOOD);
		var campfire = FindObject(CPFR); //ziemlicher hardcode :(
		CreateContents(WOOD, campfire, 1);
	} else {
		Sound("Error");
		Message("$TxtNoWood$", caller);
	}
}
  

private func Smoking() 
{
  Smoke(Random(3), Random(3), Random(8) + 5);
}

protected func CheckContents() 
{
  if(GetActTime()>5000)
    SetAction("Idle");
  if(ActIdle())
    if(ContentsCount(WOOD))
      return(BurnWood());
}

private func BurnWood() 
{
  var pWood;
  if(pWood=FindContents(WOOD)) RemoveObject(pWood);
  Sound("Inflame");
  SetAction("Burn");
}

// auﬂer Holz nichts aufnehmen
protected func RejectCollect(def,pObj) { if(def!=WOOD) return(1); }
