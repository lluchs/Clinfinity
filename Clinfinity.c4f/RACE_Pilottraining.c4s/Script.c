/*--- Agility Race ---*/

#strict 2

protected func Initialize() {
	// ozone
	SetGamma(RGB(0,0,8), RGB(115,125,125), RGB(255,255,255));

	// trailsign
	CreateObject(SIGN,150,300, NO_OWNER);

	// Rails
	CreateObject(RAIL, 30,300, NO_OWNER);
	CreateObject(RAIL, 90,300, NO_OWNER);
	CreateObject(RAIL, 120, 300, NO_OWNER);

	// rotate drafts
	for(var draft in FindObjects(Find_ID(DRFT))) {
		draft->SetR(Random(360));
	}

	// Windmill
	var windmill = CreateObject(WMIL, 850, 50, NO_OWNER);
	var windmill2 = CreateObject(WMIL, 2000, 10, NO_OWNER);
	var pump = CreateObject(PUMP, 1260, 10, NO_OWNER);
	MoveToGround(windmill);
	MoveToGround(windmill2);
	MoveToGround(pump);
}

private func MoveToGround(object obj) {
	var basement = obj->LocalN("basement");
	var bx, by;
	if(basement) {
		bx = basement->GetX() - obj->GetX();
		by = basement->GetY() - obj->GetY();
		// temporarily move basement
		basement->SetPosition(0, 0);
	}

	while(!obj->Stuck()) {
		obj->SetPosition(obj->GetX(), obj->GetY() + 1);
		if(obj->GetY() > LandscapeHeight()) {
			obj->RemoveObject();
			return;
		}
	}

	// move basement back
	if(basement)
		basement->SetPosition(obj->GetX() + bx, obj->GetY() + by);
}

// -- Callbacks des Rennen-Spielziels --
// wenn diese Funktionen nicht im Szenarioscript sind
// oder 0 zur�ck geben, wird der Default-Wert verwendet

// Race left -> right; additional condition: on top of the golden platform

func CheckRACEGoal(int plr) {
	var cursor = GetCursor(plr);
	if(cursor && cursor->GetX() > LandscapeWidth() - GetRACEEndOffset() && cursor->GetY() < LandscapeHeight() / 2)
		return 1;
	else
		return -1;
}

// Richtung:
// 1: links -> rechts
// 2: rechts -> links
// 3: untern -> oben
// 4: oben -> unten
// default: links -> rechts
func GetRACEDirection() { return 1; }

// Start: Anzahl an Pixeln, ab dem Rand, von dort beginnt die Z�hlung
// default: 50 px
func GetRACEStartOffset() { return 180; }

// Ende: Anzahl an Pixeln, ab dem Rand, bis dorthin geht die Z�hlung
// default: 50 px
func GetRACEEndOffset() { return 100; }

/* Spielerinitialisierung */

protected func InitializePlayer(int plr) { return JoinPlayer(plr); }

private func JoinPlayer(int plr) {
	var clonk = GetCrew(plr);
	DoEnergy(100000, clonk);
	SetPosition(10+Random(100), LandscapeHeight()/2-15, clonk);
	CreateContents(LOAM, clonk);
	return 1;
}

/* Neubeitritt */

public func OnClonkDeath(object oldClonk) {
	var plr = oldClonk -> GetOwner();
	if(GetPlayerType(plr)) {
		var clnk = CreateObject(AVTR, 0, 0, plr);
		clnk -> GrabObjectInfo(oldClonk);
		SelectCrew(plr, clnk, 1);
		Log(RndRelaunchMsg(), GetPlayerName(plr));
		return JoinPlayer(plr);
	}
}

private func RndRelaunchMsg() {
	var n = Random(11);
	if (!n  ) return "$MsgDeath1$";
	if (!--n) return "$MsgDeath2$";
	if (!--n) return "$MsgDeath3$";
	if (!--n) return "$MsgDeath4$";
	if (!--n) return "$MsgDeath5$";
	if (!--n) return "$MsgDeath6$";
	if (!--n) return "$MsgDeath7$";
	if (!--n) return "$MsgDeath8$";
	if (!--n) return "$MsgDeath9$";
	if (!--n) return "$MsgDeath10$";
	return "$MsgDeath11$";
}                           

public func IsDraftPermanent() { return true; }
