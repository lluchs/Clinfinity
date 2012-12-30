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
	if(GameCall("UnlimitedMatSys"))
		return INT_MAX;

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
	noMsg   - Don't output a message. Note: You should probably define *NoMatSysMessages(id mat) instead, see <MatSysMessage>.

	Returns:
	The actual change. */
global func MatSysDoFill(int iChange, int iPlr, id Key, bool noMsg) {
	var actual = GetMatSys(iPlr) -> DoFill(iChange, Key);
	if(!noMsg)
		MatSysMessage(actual, Key);
	return actual;
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
	change -= MatSysDoFill(change, plr, Key, true);
	// then, loop through the other players
	for(var count = GetPlayerCount(), i = 0; i < count && change != 0; i++) {
		var p = GetPlayerByIndex(i);
		if(!Hostile(plr, p)) {
			change -= MatSysDoFill(change, p, Key);
		}
	}
	var actual = orig - change;
	MatSysMessage(actual, Key);
	return actual;
}

/*  Function: MatSysSubtractComponents
	Tries to subtract the components of the specified object.

	*Important:* This function ignores materials not managed by the material system,
	so the return values only relate to those actually in the system.
	This removes the ambiguity whether _false_ means that something is managed but just not in storage currently, or not managed at all.

	Any other materials should be handled by the calling code appropriately.

	Parameters:
	definition - The definition whose components should be subtracted.
	player     - A player of the team whose MatSys should be used.

	Returns:
	_true_ if all materials were in storage, _false_ otherwise. */
global func MatSysSubtractComponents(id definition, int player) {
	var components = [];
	for(var i = 0, comp, num; (comp = GetComponent(0, i, 0, definition)) && (num = GetComponent(comp, i, 0, definition)); i++) {
		if(!InArray(comp, GetMatSysIDs()))
			continue;
		if(MatSysGetTeamFill(player, comp) < num) {
			return false;
		}
		PushBack([comp, num], components);
	}
	for(var c in components)
		MatSysDoTeamFill(-c[1], player, c[0]);
	return true;
}

/*  Function: GetMatSysIDs
    Defines the material system ids.

	This function will call the scenario's function SpecialMatSysIDs, which will be prepended to the list. This allows a
	scenario to have special materials.
    
	Returns:
	An array containing the material system ids. */
global func GetMatSysIDs() {
	var aIDs = [STEM, WOOD, ROCK, METL];
	ConcatArrays(aIDs, GameCall("SpecialMatSysIDs")); // vertauschte Reihenfolge, da von rechts nach links
	return aIDs;
}
