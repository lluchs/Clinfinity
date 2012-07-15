/*-- Leberwurst --*/

#strict 2

func Initialize() {
	// create goal
	var cp = CreateObject(SHIP, LandscapeWidth() / 2, LandscapeHeight() / 2, NO_OWNER);
	var koth = CreateObject(KOTH, 0, 0, NO_OWNER);
	koth->SetCP(cp);
	
	//static drafts
	
	var DraftL = CreateObject(DRFT,320, 590);
	DraftL -> SetPermanent();
	DraftL -> SetR(20);
	
	var DraftR = CreateObject(DRFT,1500, 690);
	DraftR -> SetPermanent();
	DraftR -> SetR(8);
	 
	var DraftL2=CreateObject(DRFT, 799, 600); 
	DraftL2 -> SetPermanent();
	DraftL2 -> SetR(-15);
	
	var DraftR2=CreateObject(DRFT, 1122, 600);
	DraftR2 -> SetPermanent();
	DraftR2 -> SetR(20);

	// steampunky flair
	SetGamma(RGB(15, 15, 20), RGB(118, 118, 118), RGB(210, 215, 255));
	
	// place fog
	for(var i; i<160; ++i) 
		CreateParticle("Cloud", Random(LandscapeWidth()), Random(LandscapeHeight()*2/3), RandomX(3, 9), 0, RandomX(1000, 1500), RGBa(116, 131, 145, 0));
	
	// place decoration
	CreateObject(BEAM, 318, 441, -1) -> SetAction("Right"); //beam big-Left
	CreateObject(BEAM, 1610, 472, -1) -> SetAction("Left"); //beam big-right
	
	CreateObject(RUIN,863,613,-1); // ruin in the middle
	
	//place skylands
	CreateObject(SKYL, 293, 500, -1) -> SetClrModulation(RGBa(150, 180, 255, 150));
	var isle2=CreateObject(SKYL, 1400, 545, -1);
	isle2 -> SetClrModulation(RGBa(150, 180, 255, 155));
	isle2 -> SetAction("2");

	// Island Respawn
	PeriodicIslandRespawn(3500, 100, 400, 220, 120); // left 'home' island
	PeriodicIslandRespawn(3503, 1625, 440, 190, 120); // right 'home' island
	
	ScriptGo(1);
}

func GetStartPosition(int team) {
	if(team == 1)
		return [200, 400];
	if(team == 2)
		return [1710, 430];
}

func InitializePlayer(int plr) {
	CreateMatSys(plr);
	// fill with material
	var msys = GetMatSys(plr);
	msys->DoFill(5, WOOD);
	msys->DoFill(15, METL);
	msys->DoFill(10, ROCK);

	var team = GetPlayerTeam(plr);
	var pos = GetStartPosition(team);
	if(GetLength(GetPlayersByTeam(team)) == 1) {
		CreateStartMaterial(pos[0], pos[1], plr);
	}

	var tank = FindObject2(Find_ID(STMT), Find_Allied(plr));
	tank->DoFill(300);
	var i = 0, clonk;
	while(clonk = GetCrew(plr, i++))
		clonk->Enter(tank);

}

func CreateStartMaterial(int x, int y, int plr) {
	PLTF->CreatePlatform(x, y, plr);
	CreateConstruction(STMT, x + 10, y - 5, plr, 100);
	CreateObject(CATA, x + Random(20), y - 5, plr);
}

func Script0() {
	CreateParticle("Cloud", 0, Random(LandscapeHeight()*2/3), RandomX(3, 9), 0, RandomX(1000, 1500),  RGBa(116, 131, 145, 0)); 
}


func Script20() {
	return goto(0);
}
