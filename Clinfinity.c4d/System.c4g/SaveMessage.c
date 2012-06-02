#strict 2

#appendto CLNK

local name, deathMessage;

protected func Recruitment() {
	name = GetName();
	deathMessage = GetObjCoreDeathMessage();
	return inherited(...);
}
