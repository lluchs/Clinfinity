/*-- Clonk-Appendto --*/

#strict 2
#appendto CLNK


local aCHEC_CheckPoints;
local iCHEC_SavePlrViewRange;










/*--------------------------------- Engine-definierte Funktionen ---------------------------------*/



protected func ContextCheckpointSuicide()
{
	[$Suicide$|Image=CHEC|Condition=HasCheckpoints]

	//Menüs!
	OpenCheckPointRespawnMenu();
}




protected func Death()
{
	CheckpointDoSuicide(true);

	//Wir wollen doch keine Funktionen überladen!
	return _inherited();
}














/*--------------------------------- Publike Funktionen ---------------------------------*/



func SaveCheckpoint(object pCheckPoint)
{
	//Kein Checkpoint angegeben? Dann ist das Weitermachen unmöglich!
	if(!pCheckPoint)	return false;

	//Sind wir da schon einmal vorbei gegangen?
	if(PartOf(pCheckPoint , aCHEC_CheckPoints))	return false;

	//Noch kein Array? Dann zum Array machen!
	if(GetType(aCHEC_CheckPoints) != C4V_Array)	aCHEC_CheckPoints = [];

	//Und den Checkpoint hinzufügen
	aCHEC_CheckPoints[GetLength(aCHEC_CheckPoints)] = pCheckPoint;

	//Müssen wir alle Checkpoints passieren und haben wir das schon gemacht?
	if(FindObject(RCHE) && GetLength(aCHEC_CheckPoints) == ObjectCount(CHEC))
	{
		//Alle anderen Spieler eliminieren!
		for(var i = 0; i < GetPlayerCount(); i ++)
		{
			//Sind das nicht wir?
			if(GetPlayerByIndex(i) != GetOwner())	EliminatePlayer(GetPlayerByIndex(i));
		}

		//Und GameOver aufrufen, aber verzögert, zur Sicherheit, weil niemand genau weiß, wie man das machen muss...
		Schedule("GameOver()" , 2);
	}

	//Erfolgreich!
	return true;
}



















/*--------------------------------- Private Funktionen ---------------------------------*/




private func OpenCheckPointRespawnMenu()
{
	//Menü öffnen...
	CreateMenu(CHEC , this , this , C4MN_Extra_None , "$Suicide$" , 0 , C4MN_Style_Context);

	//Und die Einträge
	for(var i = 0; i < GetLength(aCHEC_CheckPoints); i ++)
	{
		//Eintrag machen...
		AddMenuItem(Format("$SuicideMenuDesc1$ %d" , i) , "CheckpointDoSuicide" , CHEC , this , 0 , aCHEC_CheckPoints[i] , Format("$SuicideMenuDesc2$" , i));
	}
}



private func HasCheckpoints()
{
	//Dürfen wir das?
	if(!fCHEC_Respawn)	return false;

	//Ist das überhaupt nötig?
	if(!fCHEC_RespawnMenu)	return false;

	//Haben wir Checkpoints gespeichert?
	if(GetType(aCHEC_CheckPoints) == C4V_Array && GetLength(aCHEC_CheckPoints))
		return true;

	//Offenbar nicht...
	return false;
}





private func CheckpointDoSuicide(id idItem , object pCheckPoint , bool fDeath)
{
	//Sind wir noch nicht gestorben? Dann machen wir das mal!
	if(!fDeath)	SetAction("Dead");

	//Neuen Clonk erstellen...
	var pNewClonk = CreateObject(GetID() , 0 , 0 , GetOwner());

	//Zur Crew hinzufügen...
	MakeCrewMember(pNewClonk , GetOwner(pNewClonk));

	//Und den Cursor auf den Clonk setzen...
	SetCursor(GetOwner(pNewClonk) , pNewClonk);

	//Das Inventar durchgehen
	for(var i = 0; i < ContentsCount(); i ++)
	{
		//Sollen wir dem Clonk die Sachen zurückgeben?
		if(fCHEC_RespawnInventory)	Enter(pNewClonk , Contents(i));

		//Nein, nein, so lieb sind wir nicht!
		else if(fCHEC_RemoveInv)	RemoveObject(Contents(i));
	}

	//Volle Lebensenergie geben
	DoEnergy(100000 , pNewClonk);

	//Sind wir gerade gestorben?
	if(fDeath)
	{
		//Können wir auswählen?
		if(fCHEC_RespawnMenu)
		{
			//Mal in ein unsichtbares Objekt setzen, damit er keinen Schaden nimmt...
			var pTemp = CreateObject(TEMP);

			//Reinsetzen!
			Enter(pTemp , pNewClonk);

			//Sichtweite speichern
			iCHEC_SavePlrViewRange = GetObjPlrViewRange(this);

			//Sichweite auf null setzen...
			SetPlrViewRange(0 , pNewClonk);

			//Und die Menüs öffnen...
			OpenCheckPointRespawnMenu();
		}
	}

	//Der Name fehlt noch...
	SetName(GetName(this) , pNewClonk , 0 , 0 , true);

	//...und die Position! Aber nur, wenn wir gerade nicht sterben!
	if(!fDeath)	SetPosition(GetX(pCheckPoint) , GetY(pCheckPoint) + (GetDefHeight(GetID()) / 2) , pNewClonk);

	//Die Sichtweite muss auch noch wiederhergestellt werden!
	if(fCHEC_RespawnMenu && !fDeath)	SetPlrViewRange(iCHEC_SavePlrViewRange , pNewClonk);

	//Wir wollen doch keine Funktionen überladen!
	return _inherited();
}


private func PartOf(xPart, array aArray, bool fExact)
{
	for(var i; i < GetLength(aArray); i++)
	{
		if(xPart == aArray[i])	return true;

		if(fExact && GetType(aArray[i]) == C4V_Array)
		{
			if(PartOf(xPart,aArray[i],true))	return true;
		}

		return false;
	}
}