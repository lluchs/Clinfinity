/*-- Anfass-Objekt --*/

#strict 2





protected func Initialize()
{
	//Unsichtbar
	SetVisibility(VIS_None);

	//Das war's auch schon
	return true;
}



protected func Grabbed(object pClonk)
{
	//Werden wir angefasst und sind wir am Attachen?
	if(GetCommand(pClonk) == "Grab" && GetAction() == "Attaching" && GetActionTarget(0 , this))
	{
		//Dann den Checkpoint darüber informieren!
		GetActionTarget(0 , this) ->~ CheckpointGrabbed(pClonk);
	}
}