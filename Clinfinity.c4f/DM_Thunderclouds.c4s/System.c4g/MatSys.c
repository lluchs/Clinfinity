
#strict 2

// Overwritten to provide unlimited steam for ownerless platforms in the arena.
global func MatSysDoTeamFill(int change, int plr, id Key) {
	return change;
}
