/*--- Flüssigkeitsgeschütz (angeschraubt) ---*/

#strict

#include CTW2

static const CTW2_SteamUsage = 15;

protected func RotationSpeed() { return(2); }
protected func CannonMobileID() { return(LICN); }
protected func CannonPower(object obj) { return(16); }

public func ComFire(object clonk)
{
  Sound("Click");

  inherited(clonk, ...);

  return(1);
}

protected func DoMenu(object clonk, int iSelection)
{
  var tower = GetActionTarget();

  CreateMenu(tower->GetID(), clonk, this(), 0, "$TxtNoammo$",0,0,0,CannonShootMenuID());
  
  // Fässer suchen
  var i, obj, last_id, barrel_cnt, type_cnt;
  while (obj = tower->Contents(i++)) {
    var obj_id = obj->GetID();
    if (IsFilledBarrel(obj)) {
      barrel_cnt++;
      // Erstes Objekt des Typs?
      if (obj_id != last_id) {
        type_cnt++;
        last_id = obj_id;
        AddMenuItem("$TxtShoots$", "Shoot", obj_id, clonk, tower->ContentsCount(obj_id), 0, 0, 4, obj);
      }
    }
  }

  if (type_cnt > 1)
    AddMenuItem("$TxtShootsAll$", "Shoot", BARL, clonk, barrel_cnt);

  SelectMenuItem(iSelection, clonk);
  return(1);
}

private func IsFilledBarrel(object obj)
{
  return(obj->~GetAmount() && obj->~BarrelMaterialName());
}

private func Shoot(id barrel_type)
{
  var tower = GetActionTarget();
  var r = GetR();
  
  if (barrel_type == BARL)
    barrel_type = 0;

  //enough Steam?
  var plr=GetOwner();
  var msys = GetMatSys(plr);
  
  if(MatSysGetTeamFill(plr, STEM) < CTW2_SteamUsage) {
    Sound("Error");
    return(Message("$TxtNotenoughgunpowder1r$", GetActionTarget()));   
    }
  MatSysDoTeamFill(-CTW2_SteamUsage,plr,STEM);
  	
	
  var i, obj, n;
  for(var obj in FindObjects(Find_Container(GetActionTarget()))) {
    if(!IsFilledBarrel(obj)) continue;
    if (barrel_type && barrel_type != obj->GetID()) continue;

    var mat = obj->BarrelMaterial();
    var amt = obj->GetAmount();
    obj->BarrelDoFill(-amt);
    obj->ChangeDef(obj->EmptyBarrelID());

    for (var j = 0; j < amt; j++) {
      n++;
      var f = n / 50;
      var dist = 20 + f;
      var angle = r + RandomX(f / -3, f / 3);
      InsertMaterial(mat, Sin(angle, dist), -Cos(angle, dist), Sin(r, 200), -Cos(r, 200));
    }
  }

  Sound("Splash*");
}

//Andere Flugbahn
func Trajectory(object pClonk, bool fClose) {
 //Log("!");
 var iSelection=GetMenuSelection(pClonk);
 if(iSelection== -1) iSelection=0;
 var pProjectile=Contents(iSelection,GetActionTarget());
 //Log("MenuSelection: %d; Contents: %s", GetMenuSelection(pClonk),GetName(pProjectile));

 if(!fClose) AddTrajectory(this(),Sin(GetR(), 13)+GetX(this()), -Cos(GetR(), 13)+GetY(this()),Sin(GetR(), CannonPower(pProjectile))*10,-Cos(GetR(), CannonPower(pProjectile))*10);
 else { 
  //Log("Remove");
  RemoveTrajectory(this());
  }
 return(1);
 }