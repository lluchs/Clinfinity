/*-- KOTH_Observatory --*/

#strict 2

func Initialize() {
	var cp = CreateObject(OBSV, 1210, 1170, NO_OWNER);
	cp -> SetClrModulation(RGBa(235,230,255,50));
	var koth = CreateObject(KOTH, 0, 0, NO_OWNER);
	koth->SetCP(cp);
	
	//Mountains
	var MtL = CreateObject(MONT,900,1350, NO_OWNER);
	var MtR = CreateObject(MONT,1400,1350, NO_OWNER);
	
  //Observatory Parts
	var tw1 = CreateObject(TW_1,1087,825, NO_OWNER);
	tw1 -> SetClrModulation(RGBa(230,220,255,80));
	var tw2 = CreateObject(TW_2,1019,1050, NO_OWNER);
	tw2 -> SetClrModulation(RGBa(230,220,255,5));

	
	//Moon
	var moon = CreateObject(MOON,1337,490, NO_OWNER);
	moon -> SetClrModulation(RGBa(255,255,255,80));
  
  //Trees
  CreateObject(TRE4,1076,905,NO_OWNER);
  CreateObject(TRE4,1067,926,NO_OWNER);
  CreateObject(TRE4,1184,917,NO_OWNER);
  CreateObject(TRE4,1182,904,NO_OWNER);
  CreateObject(TRE4,1190,918,NO_OWNER);
  CreateObject(TRE4,1175,922,NO_OWNER);
  CreateObject(TRE4,1231,926,NO_OWNER);
  CreateObject(TRE4,1206,925,NO_OWNER);
  CreateObject(TRE4,1211,905,NO_OWNER);
  CreateObject(TRE4,1251,921,NO_OWNER);
  CreateObject(TRE4,1341,922,NO_OWNER);
  CreateObject(TRE4,1317,920,NO_OWNER);;
  CreateObject(TRE4,1377,925,NO_OWNER);
  CreateObject(TRE4,1083,931,NO_OWNER);
  CreateObject(TRE4,1163,964,NO_OWNER);
  CreateObject(TRE4,1190,961,NO_OWNER);
  CreateObject(TRE4,1180,963,NO_OWNER);
  CreateObject(TRE4,1227,946,NO_OWNER);
  CreateObject(TRE4,1293,961,NO_OWNER);;
  CreateObject(TRE4,1264,936,NO_OWNER);
  CreateObject(TRE4,1291,941,NO_OWNER);
  CreateObject(TRE4,1340,968,NO_OWNER);
  CreateObject(TRE4,1345,945,NO_OWNER);
  CreateObject(TRE4,1313,972,NO_OWNER);
  CreateObject(TRE4,1372,961,NO_OWNER);
  CreateObject(TRE4,1392,1051,NO_OWNER);
  CreateObject(TRE4,1404,1076,NO_OWNER);
  
  //static Drafts
  var staticDraftObservatoryL=CreateObject(DRFT, 970, 1025, NO_OWNER); 
	staticDraftObservatoryL -> SetPermanent();
	
  var staticDraftObservatoryR=CreateObject(DRFT, 1430, 940, NO_OWNER); 
	staticDraftObservatoryR -> SetPermanent();
	
	// Mountain Respawn
  PeriodicIslandRespawn(3550, 0, 430, 570, 750); // left 'home' island
 	PeriodicIslandRespawn(3650, 2440, 400, 630, 770); // right 'home' island
 	
 	ScriptGo(true);
}

protected func Script10(){
	Message("@$Woodmessage$");
	Sound("Ding");
}

protected func Script60(){
  Message("");
}

func GetStartPosition(int team) {
	if(team == 1)
		return [200, 400];
	if(team == 2)
		return [2240, 400];
}

func InitializePlayer(int plr) {
	CreateMatSys(plr);
	// fill with material
	var msys = GetMatSys(plr);
	msys->DoFill(25, WOOD);
	msys->DoFill(25, METL);
	msys->DoFill(10, ROCK);
	
	CreateContents(FLNT, GetHiRank(plr), 3);

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
}

global func IsDay() { return false; }
global func IsNight() { return true; }
