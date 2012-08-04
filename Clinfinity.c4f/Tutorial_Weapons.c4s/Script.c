/*-- Weapon Tutorial --*/

#strict 2

// Positions for Relaunch
static iPlrX, iPlrY, iCounter, tank;

func Initialize() {
  //Trees
  for(var i; i<21; ++i){
  PlaceVegetation(TRE1, 24, 0, 410, 140, -1); 
  PlaceVegetation(TRE2, 24, 0, 410, 140, -1);
  PlaceVegetation(TRE3, 24, 0, 410, 140, -1);
  }

	//Decoration
  CreateObject(WTFL, 350, 290); //Waterfall
  CreateObject(WMLL, 770, 269); //Windmill
  CreateObject(MINE, 250, 523); //Mine decoration

  CreateObject(GUNR,230,289); //Gun Range
  CreateObject(_CST, 252,227); //Chest
  
  var Bird=CreateObject(BIRD, 50, 50, 1); //BÖRD!
  var AIClonk=CreateObject(CLNK,50,50);
  MakeCrewMember(AIClonk,1);
  Enter(Bird, AIClonk);
  
  CreateObject(MUSK,50,50,0);
  
  //remove all objects for wheat field
  for(var obj in FindObjects(Find_InRect(424, 288, 388, 83))) obj->RemoveObject();


  
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
    MatSysDoFill(1000,0,STEM);
}

func Script1() {
  CreateObject(WEED, 610, 367); //Workaround for remove all obj in this area
}

func Script5() {
	TutorialMessage("Willkommen zurück!");
}

func Script10() {
  TutorialMessage("Begebe dich nun auf den Schießstand. Denk dran: Wenn du irgendwo dranhängst, kannst du mittels Doppel [Hoch] dich nach oben ziehen!");
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

