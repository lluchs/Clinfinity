/*  Script: GetPlayersByTeam */
#strict 2

/*  Function: GetPlayersByTeam
	
	Parameters:
	iTeam - the team whose players will be returned

	Returns:
	All players in the given team. */
global func GetPlayersByTeam(int iTeam) {
	if(!GetTeamName(iTeam))
		return [];
	var aPlayers = CreateArray();
	for(var i = 0; i < GetPlayerCount(); i++)
		if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
			aPlayers[GetLength(aPlayers)] = GetPlayerByIndex(i);
	return aPlayers;
}
