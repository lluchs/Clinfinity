/*--- Geschützturm ---*/

#strict

local cannon;

#include CXEC
//#include BAS2
//#include DOOR

#include STBO
#include L_DC
#include L_SS

public func MaxDamage() { return(50); }

/* Fahrzeug suchen und Inhalt nehmen */
private func CloseEntrance()
{
  var pCont=FindObject2(Find_Category(C4D_Vehicle),Find_Container(this()),
  Find_Not(Find_ID(CTW3),Find_ID(CTW5),Find_ID(CTW7),Find_ID(ACT1),Find_ID(ACT3),Find_ID(LICN)));
  if(pCont) {
   GrabContents(pCont);
   Exit(pCont,0,21);
   }
  return(_inherited() );
}

//Hilft der Kanone, loescht die Zielhilfe
func Grabbed(object pBy,bool fGrab) {
 if(!fGrab)
  if(HasCannon())
   HasCannon()->Trajectory(pBy,1);
 _inherited(pBy,fGrab);
 }

/* Kanonensteuerung */


protected func Progress() {
 SetAction("Attach");
	/*
	if(GetActTime() >= 60) {
		MatSysDoTeamFill(1, GetOwner(), METL);
		SetAction("Attach");
	} */
 }

protected func ControlLeft(object pObj)
{
  [$TxtRotatecannontothelef$|Image=CT01:0]
  if(!cannon) return(0);
  var plr = pObj->GetOwner();
  // wrong team?
  if(Hostile(plr, GetOwner())) {
		Sound("CommandFailure1");
		return;
	}
  SetOwner(plr);

  Sound("Click");
  return(cannon->ComLeft(pObj) );
}

protected func ControlLeftReleased(object pObj)
{
  if(!cannon) return(0);
  Sound("CannonStop");
  return(cannon->ComStop(pObj));
}

protected func ControlRight(object pObj)
{
  [$TxtRotatecannontotherig$|Image=CT01:2]
  if(!cannon) return(0);
  Sound("Click");
  return(cannon->ComRight(pObj) );
}

protected func ControlRightReleased(object pObj)
{
  if(!cannon) return(0);
  Sound("CannonStop");
  return(cannon->ComStop(pObj));
}

protected func ControlUp(object pObj)
{
  [$TxtStopcannonrotation$|Method=Classic]
  if(!cannon) return(0);

  if(!GetPlrCoreJumpAndRunControl(pObj->GetController()))
  {
    Sound("Click");
    Sound("CannonStop");
    return(cannon->ComStop(pObj) );
  }
}

protected func ControlUpDouble(object pObj)
{
  [$TxtDisconnectcannon$|Image=CT01:3]
  if(!cannon) return(0);
  return(ReleaseCannon());
}

protected func ControlDig(object pObj)
{
  [$TxtFire$|Image=CT01:1]
  if (!cannon) return(0);
  return (cannon->ComFire(pObj));
}

protected func ControlDown(object pObj)
{
  [Method=Classic]
  /* Kanone anhalten und den Clonk noch rauslassen */
  ControlUp(pObj);
  return(0);
}

/* Kontext */

public func ContextReleaseCannon(pCaller)
{
  [$TxtDisconnectcannon$|Image=CT01:3|Condition=HasCannon]
  // Der Clonk soll erst in Geschützturm laufen
  /*
  if (pCaller->Contained() != this())
  {
    SetCommand(pCaller, "None");
    AddCommand(pCaller, "Call", this(), 0,0,0,0, "ReleaseCannon");
    AddCommand(pCaller, "Enter", this());
    return(0);
  } */
  // Der Clonk kann die Kanone direkt abschrauben
  return(ReleaseCannon());  
}

/* Kanone anschrauben */

protected func Collection2(object pObj)
{
  if(pObj->~IsCannon() )
  {
    if(!cannon)
      ConnectCannon(pObj);
  }
  else
  {
    if(cannon)
      cannon->~ComEnter(pObj);
  }
}

protected func Ejection(object pObj)
{
  if(cannon)
    cannon->~ComLeave(pObj);
}

public func ConnectCannon(object pCannon)
{
  // Wir rufen die Connect-Funktion der Kanone auf. Somit kann die Kanone
  // eigene Dinge tun um sich anzubauen.
  pCannon->Connect(this() );
  SetR(45, pCannon);
  if(GetX() > LandscapeWidth() / 2) SetR(-45, pCannon);
  
  // neue Kategorie für die Kanone um sie in den Hintergrund zu kriegen
  SetCategory(2,pCannon);
  SetObjectOrder(pCannon);

  // Die neue Kanone über enthaltene Objekte informieren
  var i = ContentsCount() - 1, obj;
  while(obj = Contents(--i) )
    if(!obj->~IsCannon() )
      pCannon->~ComEnter(obj);
  
  cannon = pCannon;
  Sound("Connect");
}

// Diese Funktion können die Kanonen aufrufen, um sich an den Geschützturm anzuschrauben,
// wenn es darum geht, ihre Definition zu ändern.
public func ConnectCannonDef(object pCannon, id defChg, string szAction)
{
  if(!szAction) szAction = "Attaching";
  ChangeDef(defChg, pCannon);
  ObjectSetAction(pCannon, szAction, this() ); 
}

// Löst die Kanone vom Geschützturm
public func ReleaseCannon()
{
  if(!cannon) return(0);
  Sound("Connect");
  cannon->ComStopDouble();
  // Kategorie für die Kanone wiederherstellen
  SetCategory(GetDefCategory(GetID(cannon)),cannon);
  cannon = 0;
  
  // Neue Kanonen suchen
  var i, obj;
  while(obj = Contents(i++) )
  {
    if(obj->~IsCannon() )
    {
      ConnectCannon(obj);
      break;
    }
  }
  return(1);
}

/* Wird von den Kanonen als Turm erkannt */

public func IsCannonTower()
{
  return(1);
}

public func HasCannon()
{
  //return (cannon != 0); 
  return (cannon); 
}

/* Zerstörung */

protected func Destruction()
{
  if(cannon) ReleaseCannon();
  return(_inherited() );
}

