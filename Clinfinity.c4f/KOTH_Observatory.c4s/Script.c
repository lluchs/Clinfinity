/*-- Planetary --*/

#strict 2

func Initialize() {
	/* create goal
	var cp = CreateObject(SHIP, LandscapeWidth() / 2, LandscapeHeight() / 2, NO_OWNER);
	var koth = CreateObject(KOTH, 0, 0, NO_OWNER);
	koth->SetCP(cp);
	
	//var mast1 = CreateObject(TEL1,168,417,-1);
  //var mast2 = CreateObject(TEL1,259,419,-1);
  //CreateObject(WIRE)->Connect(mast1,mast2);
  }
	
	// steampunky flair
	SetGamma(RGB(15,15,20),RGB(118,118,118),RGB(210,215,255));
	
  // place fog
	for(var i;i<160;++i) 
    CreateParticle("Cloud",Random(LandscapeWidth()),Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500),RGBa(116,131,145,0));
	
	ScriptGo(1);
	*/
	
  CreateObject(PLAN,1200,1412);
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
