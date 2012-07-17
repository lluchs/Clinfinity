/*--- Flammenwerfer (angeschraubt) ---*/

#strict

#include CTW2

local FillLevel;

protected func RotationSpeed() { return(2); }
protected func CannonMobileID() { return(ACT1); }
protected func CannonPower(object obj) { return(5); }

static const CTW2_SteamUsage = 40; 

/* Turm weg und Munitionscheck */

public func ComEnter(object pAmmo)
{
  // Ölfasser in Munition umwandeln
  if(GetID(pAmmo) == OBRL)
  {
    Sound("Click");
    FillLevel += pAmmo->OBRL::GetAmount();
    pAmmo->OBRL::BarrelDoFill(-pAmmo->OBRL::GetAmount());
    ChangeDef(pAmmo->EmptyBarrelID(),pAmmo);
  }
}

public func ComFire(object pClonk)
{
  // Rotation stoppen
  Sound("Click");
  SetAction("Attaching", GetActionTarget());
  SetRDir(0);
  
  // Öl suchen
  var ammo = FindContents(OBRL,GetActionTarget());
  if(ammo) ComEnter(ammo);
  
  // Muni aus Öl erzeugen
  if(FillLevel < 50) return(Sound("Error") );
  FillLevel -= 50;
  ammo = CreateContents(ADM2);
  
  // Controller setzen (Killverfolgung)
  ammo->SetController(pClonk->GetController());
  
  // Muni ausspucken
  Exit(ammo, Sin(GetR(), 18), -Cos(GetR(), 18), GetR(), Sin(GetR(), 5), -Cos(GetR(), 5), 35);
  ammo->Launch();
  Sound("FlameThrower");
}

private func Shoot(id defFoo, object pProjectile, object pShooter)
{
  if (!pProjectile) return(0);
  if (pProjectile->Contained() != GetActionTarget()) return(0);

  
  //enough Steam?
  var plr=GetOwner();
  var msys = GetMatSys(plr);
  var powder = CannonPowderNeeded(pProjectile);
  
  if(MatSysGetTeamFill(plr, STEM) < CTW2_SteamUsage) {
    Sound("Error");
    Message("$TxtNotenoughgunpowder1r$", GetActionTarget(), powder);   
    }
  else
  {
	MatSysDoTeamFill(-CTW2_SteamUsage,plr,STEM);
	
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