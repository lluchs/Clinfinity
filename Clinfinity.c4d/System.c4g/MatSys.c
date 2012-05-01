/* Globale Materialsystemfunktionen */

#strict 2

// Creates a material system for the given player or for every player if iPlr == NO_OWNER
global func CreateMatSys(int iPlr) {
	if(iPlr == NO_OWNER)
		for(var i = 0; i < GetPlayerCount(); i++)
			CreateMatSys(GetPlayerByIndex(i));
	else
		CreateObject(MSYS, 0, 0, iPlr);
}

// Returns the material system for the given player, creating one if there is none
global func GetMatSys(int iPlr, bool bDoNotCreateMatSys) {
	if(GetType(aMaterialSystem) != C4V_Array || !aMaterialSystem[iPlr]) {
		if(!bDoNotCreateMatSys)
			CreateMatSys(iPlr);
		else
			return;
	}
	return aMaterialSystem[iPlr];
}
global func MatSysGetFill(int iPlr, id Key) {
	return GetMatSys(iPlr) -> GetFill(Key);
}

global func MatSysDoFill(int iChange, int iPlr, id Key) {
	return GetMatSys(iPlr) -> DoFill(iChange, Key);
}

global func GetMatSysIDs() {
	var aIDs = [WOOD, ROCK, METL];
	ConcatArrays(aIDs, GameCall("SpecialMatSysIDs")); // vertauschte Reihenfolge, da von rechts nach links
	return aIDs;
}
