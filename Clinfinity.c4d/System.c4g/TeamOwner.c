// Manages owner transfership when a player is eliminated.

#strict 2

// hijack goals
#appendto GOAL

protected func RemovePlayer(int plr) {
	var team = GetPlayerTeam(plr);
	if(team) {
		var players = GetPlayersByTeam(team);
		var newOwner = NO_OWNER;
		for(var p in players) {
			if(p != plr) {
				newOwner = p;
				break;
			}
		}
		if(newOwner != NO_OWNER) {
			for(var obj in FindObjects(Find_Owner(plr)))
				obj->SetOwner(newOwner);
		}
	}
	return _inherited(plr, ...);
}
