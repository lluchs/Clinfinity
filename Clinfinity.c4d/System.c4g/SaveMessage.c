#strict 2

#appendto CLNK

local name, deathMessage;

protected func Initialize() {
	name = GetName();
	deathMessage = GetObjCoreDeathMessage();
	return inherited(...);
}
