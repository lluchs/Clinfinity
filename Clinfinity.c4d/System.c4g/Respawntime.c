/*  Script: Respawntime.c
	Manages the respawn timer for each team. */
#strict 2

static respawnTime;

/*  Function: SetRespawnTime
	Sets the respawn time for the given team.

	Parameters:
	team - the team number
	time - the new respawn time in frames */
global func SetRespawnTime(int team, int time) {
	if(!respawnTime)
		respawnTime = CreateArray(2);
	respawnTime[team - 1] = time;
}

/*  Function: GetRespawnTime
	Returns the respawn time for the given team.

	Parameters:
	team - the team number

	Returns:
	The respawn time in frames. */
global func GetRespawnTime(int team) {
	return respawnTime && respawnTime[team - 1];
}
