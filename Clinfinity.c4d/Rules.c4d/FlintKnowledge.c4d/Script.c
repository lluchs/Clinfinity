/* Flint Knowledge */

#strict 2

protected func Initialize()	{
	ScheduleCall(0, "ActivateSuperFlints", 17280);
	ScheduleCall(0, "ActivateTeraFlints", 25920);
}

protected func ActivateSuperFlints() {
	Log("$TxtKnowledgeSFLN$");
	Sound("Status");
	//Count the number of actual players
	for(var playernumber = 0; playernumber < GetPlayerCount(); playernumber++)
		SetPlrKnowledge(GetPlayerByIndex(playernumber), EFLN);
}

protected func ActivateTeraFlints() {
	Log("$TxtKnowledgeEFLN$");
	Sound("Status");
	for(var playernumber = 0; playernumber < GetPlayerCount(); playernumber++)
  	SetPlrKnowledge(GetPlayerByIndex(playernumber), EFLN);
}
