/*--- Kampfgeschütz (angeschraubt) ---*/

#strict

// Steam a single shot consumes this amount of Steam
static const CTW2_BasicSteamUsage = 5; //SteamUsage*ObjectMass

/* Überladbar für andere Kanonen */

protected func RotationSpeed() { return(5); }
protected func CannonMobileID() { return(CTW3); }
protected func CannonAmmo(object obj) { return(GetOCF(obj) & OCF_Collectible() ); }
protected func CannonPowderNeeded(object obj) { return(1); }
protected func CannonPower(object obj) { return(12); }
protected func CannonSound(object obj) { return("Blast3"); }
protected func CannonSmoke(object obj) { return(1); }
protected func CannonShootMenuID() { return(GetID()); }
protected func CannonShootMenuName(object pPotentialProjectile) { return Format("$TxtShoots$",GetName(pPotentialProjectile)); }


protected func Destruction() {
  CloseMenus();
}

private func CloseMenus() {
  var tower = GetActionTarget();

  for(var obj in FindObjects(Find_Container(tower)))
  {
    if(!obj->~IsClonk()) continue;
    if(obj->GetMenu() != CannonShootMenuID()) continue;
    obj->CloseMenu();
  }
}

/* Turm weg? */

protected func AttachTargetLost()
{
  ComStopDouble();
}

/* Kommandos aus dem Turm */

public func ComLeft(object pClonk)
{
  SetAction("Rotating",GetActionTarget());
  SetRDir(-RotationSpeed() );
  Trajectory(pClonk); 
  return(1);
}

public func ComRight(object pClonk)
{
  SetAction("Rotating",GetActionTarget());
  SetRDir(RotationSpeed() );
  Trajectory(pClonk);   
  return(1);
}

public func ComStop(object pClonk)
{
  SetAction("Attaching",GetActionTarget());
  SetRDir(0);
  Trajectory(pClonk);   
  return(1);
}

//Wird vom Grabber nicht uebertragen, nur vom Container, Achtung, Aufruf stattdessen im Cannontower
public func ComStopDouble(object pClonk)
{
  //Log("StopDouble");
  Trajectory(pClonk,1);  
  CloseMenus();

  var r = (GetR() + 270) % 360;
  SetR(r);
  ChangeDef(CannonMobileID() );
  return(1);
}

public func ComEnter()
{
  ComLeave();
}

public func ComLeave()
{
  var tower = GetActionTarget();
  for(var obj in FindObjects(Find_Container(tower)))
  {
    if(!obj->~IsClonk()) continue;
    if(obj->GetMenu() != CannonShootMenuID()) continue;

    var sel = GetMenuSelection(obj);
    obj->CloseMenu();
    DoMenu(obj, sel);	
  }
}

public func ComFire(object pClonk)
{
  SetAction("Attaching",GetActionTarget());
  SetRDir(0);
  Trajectory(pClonk,1);
  DoMenu(pClonk, 0);
}

private func DoMenu(object pClonk, int iSelection)
{
  CreateMenu(GetID(GetActionTarget()), pClonk, this(), 0, "$TxtNoammo$", 0, 0, 0, CannonShootMenuID());

  var i, obj;
  while(obj = Contents(i++, GetActionTarget()) )
    if(GetID(obj) != GUNP)
      if(CannonAmmo(obj) )
        AddMenuItem(CannonShootMenuName(obj), Format("Shoot(%i, Object(%d), Object(%d))", obj->GetID(), ObjectNumber(obj), ObjectNumber(pClonk)), 0, pClonk, 0, 0, 0, 4, obj);
  SelectMenuItem(iSelection, pClonk);
  Trajectory(pClonk);  
  return(1);
}

func OnMenuSelection(int iItemIndex, object pMenuObject) {
 Trajectory(pMenuObject);
 return(1);
 }

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

private func Shoot(id defFoo, object pProjectile, object pShooter)
{
  if (!pProjectile) return(0);
  if (pProjectile->Contained() != GetActionTarget()) return(0);

  
  //enough Steam?
  var plr=GetOwner();
  var msys = GetMatSys(plr);
  var powder = CannonPowderNeeded(pProjectile);
  
  if(MatSysGetTeamFill(plr, STEM) < CTW2_BasicSteamUsage*GetMass(pProjectile)) {
    Sound("Error");
    Message("$TxtNotenoughgunpowder1r$", GetActionTarget(), powder);   
    }
  else
  {
	MatSysDoTeamFill(-CTW2_BasicSteamUsage*GetMass(pProjectile),plr,STEM);
	
    if(CannonSmoke(pProjectile) )
    {
      Smoke(Sin(GetR(), 13), -Cos(GetR(), 13), 20);
      Smoke(Sin(GetR(), 18), -Cos(GetR(), 23), 17);
      Smoke(Sin(GetR(), 22), -Cos(GetR(), 32), 14);
      Smoke(Sin(GetR(), 25), -Cos(GetR(), 40), 11);
    }
    
    // Controller setzen (Killverfolgung)
    pProjectile->SetController(pShooter->GetOwner());
    Shoot();

	//AddTrajectory(this(),Sin(GetR(), 13), -Cos(GetR(), 13),Sin(GetR(), CannonPower(pProjectile)), -Cos(GetR(), CannonPower(pProjectile)));
    Exit(pProjectile, Sin(GetR(), 13), -Cos(GetR(), 13), GetR(), Sin(GetR(), CannonPower(pProjectile)), -Cos(GetR(), CannonPower(pProjectile)), 20);
    if(GetOCF(pProjectile) & OCF_CrewMember() ) ObjectSetAction(pProjectile, "Tumble");
    Sound(CannonSound(pProjectile) );
    SetPlrView(pShooter->GetOwner(), pProjectile);
  }
}