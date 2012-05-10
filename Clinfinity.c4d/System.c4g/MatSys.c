/*  Script: MatSys.c
    Contains some global helper function for using the Material System from arbitrary objects. */

#strict 2

/*  Function: CreateMatSys
    Creates a material system for the given player or for every player if iPlr == NO_OWNER.

	Parameters:
	iPlr - The player for whom the material system should be created or NO_OWNER. */
global func CreateMatSys(int iPlr) {
	if(iPlr == NO_OWNER)
		for(var i = 0; i < GetPlayerCount(); i++)
			CreateMatSys(GetPlayerByIndex(i));
	else
		CreateObject(MSYS, 0, 0, iPlr);
}

/*  Function: GetMatSys
    Returns the material system for the given player, creating one if there is none.

	Parameters:
	iPlr               - The player whose material system should be returned.
	bDoNotCreateMatSys - When true, the function won't create a material system for that player.

	Returns:
	The material system for the given player or 0 if there is none. */
global func GetMatSys(int iPlr, bool bDoNotCreateMatSys) {
	if(GetType(aMaterialSystem) != C4V_Array || !aMaterialSystem[iPlr]) {
		if(!bDoNotCreateMatSys)
			CreateMatSys(iPlr);
		else
			return;
	}
	return aMaterialSystem[iPlr];
}

/*  Function: MatSysGetFill
    
	Parameters:
	iPlr - The player whose fill level should be returned.
	Key  - The material id.

	Returns:
	The fill level. */
global func MatSysGetFill(int iPlr, id Key) {
	return GetMatSys(iPlr) -> GetFill(Key);
}

/*  Function: MatSysGetTeamFill
    
	Parameters:
	plr - A player of the team whose fill level should be returned.
	Key - The material id.

	Returns:
	The combined fill level of all team members. */
global func MatSysGetTeamFill(int plr, id Key) {
	var fill = 0;
	for(var count = GetPlayerCount(), i = 0; i < count; i++) {
		var p = GetPlayerByIndex(i);
		if(!Hostile(plr, p))
			fill += MatSysGetFill(p, Key);
	}
	return fill;
}

/*  Function: MatSysDoFill
    Changes the fill level for a given player/id.
    
	Parameters:
	iChange - The amount of change.
	iPlr    - The player whose fill level should be changed.
	Key     - The material id.

	Returns:
	The actual change. */
global func MatSysDoFill(int iChange, int iPlr, id Key) {
	return GetMatSys(iPlr) -> DoFill(iChange, Key);
}


/*  Function: MatSysDoTeamFill
    Changes the fill level for a given team/id.
    
	Parameters:
	change - The amount of change.
	plr    - A player of the team whose fill level should be changed.
	Key    - The material id.

	Returns:
	The actual change. */
global func MatSysDoTeamFill(int change, int plr, id Key) {
	var orig = change;
	// first, try the given player
	change -= MatSysDoFill(change, plr, Key);
	// then, loop through the other players
	for(var count = GetPlayerCount(), i = 0; i < count && change != 0; i++) {
		var p = GetPlayerByIndex(i);
		if(!Hostile(plr, p)) {
			change -= MatSysDoFill(change, p, Key);
		}
	}
	return orig - change;
}

/*  Function: GetMatSysIDs
    Defines the material system ids.

	This function will call the scenario's function SpecialMatSysIDs, which will be prepended to the list. This allows a
	scenario to have special materials.
    
	Returns:
	An array containing the material system ids. */
global func GetMatSysIDs() {
	var aIDs = [WOOD, ROCK, METL];
	ConcatArrays(aIDs, GameCall("SpecialMatSysIDs")); // vertauschte Reihenfolge, da von rechts nach links
	return aIDs;
}
