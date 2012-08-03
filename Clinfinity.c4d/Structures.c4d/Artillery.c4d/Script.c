/*--- Geschützturm ---*/

#strict 2

local cannon;

#include CXEC

#include STBO
#include L_DC
#include L_SS

public func MaxDamage() {
	return 50;
}

//Hilft der Kanone, loescht die Zielhilfe
func Grabbed(object by, bool grab) {
	if(!grab)
		if(HasCannon())
			HasCannon()->Trajectory(by, 1);
	_inherited(by, grab);
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

protected func ControlLeft(object obj) {
	[$TxtRotatecannontothelef$ | Image = CT01: 0]
	if(!cannon) return 0;
	var plr = obj->GetOwner();
	// wrong team?
	if(Hostile(plr, GetOwner())) {
		Sound("CommandFailure1");
		return;
	}
	SetOwner(plr);

	Sound("Click");
	return cannon->ComLeft(obj) ;
}

protected func ControlLeftReleased(object obj) {
	if(!cannon) return 0;
	Sound("CannonStop");
	return cannon->ComStop(obj);
}

protected func ControlRight(object obj) {
	[$TxtRotatecannontotherig$ | Image = CT01: 2]
	if(!cannon) return 0;
	Sound("Click");
	return(cannon->ComRight(obj) );
}

protected func ControlRightReleased(object obj) {
	if(!cannon) return 0;
	Sound("CannonStop");
	return(cannon->ComStop(obj));
}

protected func ControlDig(object obj) {
	[$TxtStopcannonrotation$ | Method = Classic]
	if(!cannon) return 0;

	if(!GetPlrCoreJumpAndRunControl(obj->GetController())) {
		Sound("Click");
		Sound("CannonStop");
		return(cannon->ComStop(obj) );
	}
}

protected func ControlThrow(object obj) {
	[$TxtFire$ | Image = CT01: 1]
	if (!cannon) return 0;
	return cannon->ComFire(obj);
}

protected func ControlUp(object obj) {
	[$TxtLessPower$ | Image = CT01: 4]
	if(!cannon) return 0;
	return cannon->ComPowerUp(obj);
}

protected func ControlDown(object obj) {
	[$TxtLessPower$ | Image = CT01: 3]
	if(!cannon) return 0;
	return cannon->ComPowerDown(obj);
}

/* Cannon connection functionality */

public func ConnectCannon(object newCannon) {
	cannon = newCannon;
	// Wir rufen die Connect-Funktion der Kanone auf. Somit kann die Kanone
	// eigene Dinge tun um sich anzubauen.
	cannon->Connect(this() );
	SetR(45, cannon);
	if(GetX() > LandscapeWidth() / 2) SetR(-45, cannon);

	// neue Kategorie für die Kanone um sie in den Hintergrund zu kriegen
	SetCategory(2, cannon);
	SetObjectOrder(cannon);

	Sound("Connect");
}

// Diese Funktion können die Kanonen aufrufen, um sich an den Geschützturm anzuschrauben,
// wenn es darum geht, ihre Definition zu ändern.
public func ConnectCannonDef(object cannon, id defChg, string action) {
	if(!action) action = "Attaching";
	ChangeDef(defChg, cannon);
	ObjectSetAction(cannon, action, this() );
}

// Löst die Kanone vom Geschützturm
public func ReleaseCannon() {
	if(!cannon) return 0;
	Sound("Connect");
	cannon->ComStopDouble();
	// Kategorie für die Kanone wiederherstellen
	SetCategory(GetDefCategory(GetID(cannon)), cannon);
	cannon = 0;

	// Neue Kanonen suchen
	var i, obj;
	while(obj = Contents(i++) ) {
		if(obj->~IsCannon() ) {
			ConnectCannon(obj);
			break;
		}
	}
	return 1;
}

/* Wird von den Kanonen als Turm erkannt */

public func IsCannonTower() {
	return 1;
}

public func HasCannon() {
	return cannon;
}

/* Zerstörung */

protected func Destruction() {
	if(cannon)
		cannon->RemoveObject();
	return _inherited(...);
}

