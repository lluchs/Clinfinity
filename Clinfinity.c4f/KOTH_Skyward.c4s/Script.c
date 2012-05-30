/*-- Leberwurst --*/

#strict 2

func Initialize() {
	// create goal
	var cp = CreateObject(SHIP, LandscapeWidth() / 2, LandscapeHeight() / 2, NO_OWNER);
	var koth = CreateObject(KOTH, 0, 0, NO_OWNER);
	koth->SetCP(cp);
	
	// steampunky flair
	SetGamma(RGB(15,15,20),RGB(118,118,118),RGB(210,215,255));
}

func InitializePlayer(int plr) {
	CreateMatSys(plr);
	// fill with material
	var msys = GetMatSys(plr);
	msys->DoFill(5, WOOD);
	msys->DoFill(10, METL);
	msys->DoFill(10, ROCK);
	
	// place fog
	for(var i;i<160;++i) 
    CreateParticle("Cloud",Random(LandscapeWidth()),Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500));
	
	ScriptGo(1);
}

func Script0()
{
  CreateParticle("Cloud",0,Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500)); 
}

func Script4()
{
  return(goto(0));
}
