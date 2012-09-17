/*-- KOTH_Observatory --*/

#strict 2

func Initialize() {
	var cp = CreateObject(OBSV,1210,1170,-1);
	cp -> SetClrModulation(RGBa(235,230,255,50));
	var koth = CreateObject(KOTH, 0, 0, NO_OWNER);
	koth->SetCP(cp);
	
	//Mountains
	var MtL = CreateObject(MONT,900,1350);
	var MtR = CreateObject(MONT,1400,1350);
	
  //Observatory Parts
	var tw1 = CreateObject(TW_1,1087,825);
	tw1 -> SetClrModulation(RGBa(230,220,255,80));
	var tw2 = CreateObject(TW_2,1019,1050);
	tw2 -> SetClrModulation(RGBa(230,220,255,5));

	
	//Moon
	var moon = CreateObject(MOON,1337,490);
	moon -> SetClrModulation(RGBa(255,255,255,80));
  
  //Trees
  CreateObject(TRE4,1076,905,-1);
  CreateObject(TRE4,1067,926,-1);
  CreateObject(TRE4,1184,917,-1);
  CreateObject(TRE4,1182,904,-1);
  CreateObject(TRE4,1190,918,-1);
  CreateObject(TRE4,1175,922,-1);
  CreateObject(TRE4,1231,926,-1);
  CreateObject(TRE4,1206,925,-1);
  CreateObject(TRE4,1211,905,-1);
  CreateObject(TRE4,1251,921,-1);
  CreateObject(TRE4,1341,922,-1);
  CreateObject(TRE4,1317,920,-1);;
  CreateObject(TRE4,1377,925,-1);
  CreateObject(TRE4,1083,931,-1);
  CreateObject(TRE4,1163,964,-1);
  CreateObject(TRE4,1190,961,-1);
  CreateObject(TRE4,1180,963,-1);
  CreateObject(TRE4,1227,946,-1);
  CreateObject(TRE4,1293,961,-1);;
  CreateObject(TRE4,1264,936,-1);
  CreateObject(TRE4,1291,941,-1);
  CreateObject(TRE4,1340,968,-1);
  CreateObject(TRE4,1345,945,-1);
  CreateObject(TRE4,1313,972,-1);
  CreateObject(TRE4,1372,961,-1);
  CreateObject(TRE4,1392,1051,-1);
  CreateObject(TRE4,1404,1076,-1);
  
  //static Drafts
  var staticDraftObservatoryL=CreateObject(DRFT, 970, 1025); 
	staticDraftObservatoryL -> SetPermanent();
	
  var staticDraftObservatoryR=CreateObject(DRFT, 1430, 940); 
	staticDraftObservatoryR -> SetPermanent();
	
	// Mountain Respawn
  PeriodicIslandRespawn(3550, 0, 430, 570, 750); // left 'home' island
 	PeriodicIslandRespawn(3650, 2440, 400, 630, 770); // right 'home' island
 	
 	ScriptGo(true);
}

protected func Script10(){
	TutorialMessage("$Woodmessage$");
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
}
