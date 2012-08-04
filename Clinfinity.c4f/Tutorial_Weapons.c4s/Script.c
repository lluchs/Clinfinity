/*-- Weapon Tutorial --*/

#strict 2

// Positions for Relaunch
static iPlrX, iPlrY, iCounter, tank;

func Initialize() {
	// Decoration
  CreateObject(WTFL, 350, 290); //Waterfall

  CreateObject(GUNR,230,289); //Gun Range
  CreateObject(_CST, 252,227); //Chest
  
  
  var Bird=CreateObject(BIRD, 50, 50, 1); //B÷RD!
  var AIClonk=CreateObject(CLNK,50,50);
  MakeCrewMember(AIClonk,1);
  Enter(Bird, AIClonk);
  
  CreateObject(MUSK,50,50,0);
  
  PLTF->CreatePlatform(1000,400,0);
	CreateConstruction(STMT, 1000, 150, 0, 100);
  
  
	ScriptGo(true);
}

func InitializePlayer(int plr) {
	GetCrew(plr)->SetPosition(0, 320);
	// Message positioning
	//SetPlrShowControlPos(plr, SHOWCTRLPOS_TopLeft);
	SetTutorialMessagePos(MSG_Top | MSG_Left | MSG_WidthRel | MSG_XRel, 50, 50, 30);

	SavePosition();
	iCounter = 0;
	
	  CreateMatSys(plr);
    // fill with material
    var msys = GetMatSys(plr);
    msys->DoFill(10, METL);
}



func Script5() {
	TutorialMessage(Format("$Willkommen zur¸ck %d!$", GetPlayerName()));
	var tank = FindObject(STMT);
	tank->DoFill(500);
}

func Script10() {
  TutorialMessage("Begebe dich nun auf den Schieﬂstand. Denk dran: Wenn du irgendwo dranh‰ngst, kannst du mittels Doppel [Hoch] dich nach oben ziehen!");
  SetArrow(239, 210);
}

func Script40() {
	if(!FindObject(AVTR, 160, 50 , 30, 20)) {
		goto(39);
		return;
	}
	SavePosition();
	TutorialMessage("$TutNice$");
	Sound("Applause");
	RemoveArrow();
}

