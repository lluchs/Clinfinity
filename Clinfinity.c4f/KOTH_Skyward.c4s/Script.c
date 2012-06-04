/*-- Leberwurst --*/

#strict 2

func Initialize() {
	// create goal
	var cp = CreateObject(SHIP, LandscapeWidth() / 2, LandscapeHeight() / 2, NO_OWNER);
	var koth = CreateObject(KOTH, 0, 0, NO_OWNER);
	koth->SetCP(cp);
	
	// steampunky flair
	SetGamma(RGB(15,15,20),RGB(118,118,118),RGB(210,215,255));
	
  // place fog
	for(var i;i<160;++i) 
    CreateParticle("Cloud",Random(LandscapeWidth()),Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500),RGBa(116,131,145,0));
	
	// place decoration
	CreateObject(BEAM, 318, 441, -1) -> SetAction("Right"); //beam big-Left
	CreateObject(BEAM, 1610, 472, -1) -> SetAction("Left"); //beam big-right
	
	CreateObject(RUIN,863,613,-1); // ruin in the middle
	
	//place skylands
	CreateObject(SKYL, 293, 500, -1) -> SetClrModulation(RGBa(150, 180, 255, 150));
	
	ScriptGo(1);
}

func InitializePlayer(int plr) {
	CreateMatSys(plr);
	// fill with material
	var msys = GetMatSys(plr);
	msys->DoFill(5, WOOD);
	msys->DoFill(10, METL);
	msys->DoFill(10, ROCK);

}

func Script0()
{
  CreateParticle("Cloud",0,Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500), RGBa(116,131,145,0)); 
}

func Script4()
{
  return(goto(0));
}
