/*--- Agility Race ---*/

#strict 2

protected func Initialize() {
	// rotate drafts
	for(var draft in FindObjects(Find_ID(DRFT))) {
		draft->SetR(Random(360));
	}
}

// -- Callbacks des Rennen-Spielziels --
// wenn diese Funktionen nicht im Szenarioscript sind
// oder 0 zurück geben, wird der Default-Wert verwendet

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

// Start: Anzahl an Pixeln, ab dem Rand, von dort beginnt die Zählung
// default: 50 px
func GetRACEStartOffset() { return 180; }

// Ende: Anzahl an Pixeln, ab dem Rand, bis dorthin geht die Zählung
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
