/*--- Kampfgeschütz ---*/

#strict

protected func ConnectID() {
	return(CTW2);
}
protected func GetAlternativeType() {
	return(ConnectID());
}

func ControlDigDouble(object pObj) {
	[$TxtConnectcannon$ | Image = CTW3]
	var pTower = FindObject2(Find_ID(CTW0), Find_Not(Find_Hostile(GetOwner(pObj))), Find_Distance(30));
	if(pTower)
		pTower->ConnectCannon(this);
	else {
		Sound("Error");
		PlayerMessage(GetOwner(pObj), "$TxtNoTower$", this());
	}
	return(1);
}

func Grabbed(object pByObject, bool fGrab) {
	SetOwner(GetOwner(pByObject));
	_inherited(pByObject, fGrab);
}

/* Wird vom Turm als Kanone erkannt */

public func IsCannon() {
	return(1);
}

/* An Turm anschrauben */

public func Connect(object pTower) {
	pTower->ConnectCannonDef(this(), ConnectID() );
}

/* Aufschlaggeräusch */

protected func Hit() {
	Sound("RockHit*");
}

/* Forschung */

public func GetResearchBase() {
	return(CTW0);
}
