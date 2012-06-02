/*-- Leberwurst --*/

#strict 2

static joinedTeams;

func Initialize() {
	// create goal
	var cp = CreateObject(SHIP, LandscapeWidth() / 2, LandscapeHeight() / 2, NO_OWNER);
	var koth = CreateObject(KOTH, 0, 0, NO_OWNER);
	koth->SetCP(cp);

	joinedTeams = [];
	
	// steampunky flair
	SetGamma(RGB(15, 15, 20), RGB(118, 118, 118), RGB(210, 215, 255));
	
	// place fog
	for(var i; i<160; ++i) 
		CreateParticle("Cloud", Random(LandscapeWidth()), Random(LandscapeHeight()*2/3), RandomX(3, 9), 0, RandomX(1000, 1500), RGBa(116, 131, 145, 0));
	
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
	msys->DoFill(10, METL);
	msys->DoFill(10, ROCK);

	var team = GetPlayerTeam(plr);
	var pos = GetStartPosition(team);
	if(GetLength(GetPlayersByTeam(team)) == 1) {
		CreateStartMaterial(pos[0], pos[1], plr);
	}

	var tank = FindObject2(Find_ID(STMT), Find_Allied(plr));
	tank->DoFill(200);
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

func Script4() {
	return goto(0);
}
