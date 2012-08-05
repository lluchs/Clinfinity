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
  /*var AIClonk=CreateObject(CLNK,50,50);
  MakeCrewMember(AIClonk,1);
  Enter(Bird, AIClonk);*/
  
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
    msys->DoFill(100, METL);
    MatSysDoFill(2000,0,STEM);
}

func Script1() {
  CreateObject(WEED, 610, 367); //Workaround for remove all obj in this area
}

func Script5() {
	TutorialMessage("Willkommen zurück!");
}

func Script15() {
  TutorialMessage("Begebe dich nun auf den Schießstand und nimm die Muskete aus der Kiste! Denk dran: Wenn du an etwas dünnem hangelst, kannst du dich mittels Doppel [Hoch] nach oben ziehen!");
  CreateContents(MUSK, FindObject(_CST));
  Sound("Ding");
  SetArrow(245, 210);
}

func Script30() {
	if(!FindContents(MUSK, FindObject(AVTR))) {
		goto(29);
		return;
	}
	SavePosition();
	TutorialMessage("Gut. Mit Werfen kannst du die Muskete anlegen und schießen. Dabei verbraucht sie etwas Dampf und pro Munitionspaket ein Metall!");
	RemoveArrow();
}

func Script50() {
  TutorialMessage("Der Balken über dir zeigt die Stärke des Schusses an, der rote Pfeil zeigt an, dass getroffene Lebewesen bei einem Treffer geschleudert werden. Schieß nun den Vogel ab, er ist böse!");
}

func Script52() {
  if(!FindObject(DBRD)) {
    goto(51);
    return;
  }
  TutorialMessage("Der Vogel ist tot!");
  Sound("Applause");
}

func Script60(){
  TutorialMessage("Lege nun die Muskete wieder in die Kiste und Schnapp dir den Dampfgranatwerfer!");
}

func Script70(){
  TutorialMessage("Bonk.");
  GameOver(1);
}
