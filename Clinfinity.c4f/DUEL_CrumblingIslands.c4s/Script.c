/*-- Crumbling Islands --*/

#strict 2

func Initialize() {
    //Ressource Buildings
    CreateConstruction(RFLN, 1040, 740, -1, 100, 1, 0); //Flintfactory Middle
    CreateConstruction(RSMG, 810, 540, -1, 100, 1, 0); //Steamgenerator Left
    CreateConstruction(RSMG, 1315, 545, -1, 100, 1, 0); //Steamgenerator Left
    
    //Pipe to Steamgenerator
    CreateObject(PIPE, 1349, 1008);

    //Crumbling Islands (deco)
    var CIsleL = CreateObject(ISLE, 760, 810);
    CIsleL -> SetClrModulation(RGBa(200, 215, 255, 50));
    
    var CIsleM = CreateObject(ISLE,1100,1000);
    CIsleL -> SetClrModulation(RGBa(200, 215, 255, 80));
    
    var CIsleR = CreateObject(ISLE,1335,800);
    CIsleR -> SetClrModulation(RGBa(200, 215, 255, 30));
    

    CreateObject(WTFL, 814, 698); //Waterfall

    SetSkyParallax(1, 20, 0, 1, 0); //Sky move with Wind

    //Fog
    CreateObject(FOG_, 400, 1050, -1);
    CreateObject(FOG_, 1200, 1050, -1);
    CreateObject(FOG_, 2000, 1050, -1);
    
    //static drafts
    var DraftWaterfall = CreateObject(DRFT, 760, 780);
	  DraftWaterfall -> SetPermanent();
	  DraftWaterfall -> SetR(-10);
	  
	  var MIsleL = CreateObject(DRFT, 992, 749);
	  MIsleL -> SetPermanent();
	  MIsleL -> SetR(-12);
	  
	  var MIsleR = CreateObject(DRFT, 1095, 780);
	  MIsleR -> SetPermanent();
	  MIsleR -> SetR(12);
	  
	  var IsleR = CreateObject(DRFT, 1310, 764);
	  IsleR -> SetPermanent();
	  IsleR -> SetR(-12);
	  
	  var IsleM = CreateObject(DRFT, 1060, 670);
	  IsleM -> SetPermanent();
	  
	  //thousands of vines!
	  PlaceVines();
	  
	  //mass epic wood
	  PlaceWood();
	  
	  // Island Respawn
  	PeriodicIslandRespawn(1993, 140, 750, 330, 250); // left 'home' island
  	PeriodicIslandRespawn(2007, 1620, 730, 310, 250); // right 'home' island
}

func InitializePlayer(int plr) {
    CreateMatSys(plr);
    // fill with material
    var msys = GetMatSys(plr);
    msys->DoFill(4, WOOD);
    msys->DoFill(10, METL);
    msys->DoFill(7, ROCK);
    
    //Flints
    CreateContents(FLNT, GetHiRank(plr), 2);

    var team = GetPlayerTeam(plr);
    var pos = GetStartPosition(team);
    if(GetLength(GetPlayersByTeam(team)) == 1) {
        CreateStartMaterial(pos[0], pos[1], plr);
    }

    var tank = FindObject2(Find_ID(STMT), Find_Allied(plr));
    tank->DoFill(100);
    var i = 0, clonk;
    while(clonk = GetCrew(plr, i++))
        clonk->Enter(tank);

}

func CreateStartMaterial(int x, int y, int plr) {
    var pltf1 = PLTF->CreatePlatform(x, y, plr);
    var pltf2 = PLTF->CreatePlatform(x + 20, y, plr);
    PLTF->Connect(pltf1, pltf2);
	var team = GetPlayerTeam(plr), ox, tankX = x;
	if(team == 1) {
		tankX += 10;
		ox = 1;
	} else {
		tankX += 95;
		ox = -1;
	}
    CreateConstruction(STMT, tankX, y - 5, plr, 100);
	var artillery = CreateConstruction(CTW0, tankX + ox * 90, y - 5, plr, 100);
	artillery->ConnectCannon(CreateObject(CTW3, 0, 0, plr));
}

func GetStartPosition(int team) {
    if(team == 1)
        return [300, 730];
    if(team == 2)
        return [1800, 730];
}


protected func PlaceVines() {
    var obj0 = CreateObject(VINE, 755, 557, -1);
    obj0->SetCon(60);
    CreateObject(VINE, 796, 588, -1);
    var obj1 = CreateObject(VINE, 786, 571, -1);
    obj1->SetCon(80);
    var obj2 = CreateObject(VINE, 861, 584, -1);
    obj2->SetCon(70);
    var obj3 = CreateObject(VINE, 868, 570, -1);
    obj3->SetCon(60);
    var obj4 = CreateObject(VINE, 1289, 571, -1);
    obj4->SetCon(80);
    var obj5 = CreateObject(VINE, 1275, 565, -1);
    obj5->SetCon(50);
    var obj6 = CreateObject(VINE, 1281, 568, -1);
    obj6->SetCon(70);
    var obj7 = CreateObject(VINE, 1270, 562, -1);
    obj7->SetCon(40);
    CreateObject(VINE, 1330, 589, -1);
    CreateObject(VINE, 1316, 592, -1);
    var obj8 = CreateObject(VINE, 851, 602, -1);
    obj8->SetCon(90);
    var obj9 = CreateObject(VINE, 1368, 683, -1);
    obj9->SetCon(70);
    var obj10 = CreateObject(VINE, 1359, 720, -1);
    obj10->SetCon(70);
    var obj11 = CreateObject(VINE, 1042, 787, -1);
    obj11->SetCon(70);
    var obj12 = CreateObject(VINE, 182, 825, -1);
    obj12->SetCon(70);
    var obj13 = CreateObject(VINE, 177, 825, -1);
    obj13->SetCon(60);
    var obj14 = CreateObject(VINE, 178, 819, -1);
    obj14->SetCon(60);
    var obj15 = CreateObject(VINE, 447, 816, -1);
    obj15->SetCon(70);
    var obj16 = CreateObject(VINE, 451, 819, -1);
    obj16->SetCon(60);
    var obj17 = CreateObject(VINE, 454, 817, -1);
    obj17->SetCon(40);
    var obj18 = CreateObject(VINE, 792, 835, -1);
    obj18->SetCon(80);
    var obj19 = CreateObject(VINE, 825, 812, -1);
    obj19->SetCon(50);
    var obj20 = CreateObject(VINE, 820, 810, -1);
    obj20->SetCon(70);
    var obj21 = CreateObject(VINE, 803, 829, -1);
    obj21->SetCon(80);
    var obj22 = CreateObject(VINE, 1676, 810, -1);
    obj22->SetCon(70);
    var obj23 = CreateObject(VINE, 1683, 810, -1);
    obj23->SetCon(60);
    var obj24 = CreateObject(VINE, 1690, 834, -1);
    obj24->SetCon(70);
    var obj25 = CreateObject(VINE, 1668, 808, -1);
    obj25->SetCon(40);
    var obj26 = CreateObject(VINE, 1936, 824, -1);
    obj26->SetCon(70);
    var obj27 = CreateObject(VINE, 1944, 809, -1);
    obj27->SetCon(40);
    var obj28 = CreateObject(VINE, 1932, 825, -1);
    obj28->SetCon(70);
    var obj29 = CreateObject(VINE, 1914, 846, -1);
    obj29->SetCon(80);
    var obj30 = CreateObject(VINE, 194, 867, -1);
    obj30->SetCon(70);
    var obj31 = CreateObject(VINE, 201, 869, -1);
    obj31->SetCon(60);
    var obj32 = CreateObject(VINE, 212, 886, -1);
    obj32->SetCon(80);
    var obj33 = CreateObject(VINE, 382, 879, -1);
    obj33->SetCon(70);
    CreateObject(VINE, 380, 877, -1);
    CreateObject(VINE, 393, 879, -1);
    CreateObject(VINE, 400, 889, -1);
    var obj34 = CreateObject(VINE, 421, 889, -1);
    obj34->SetCon(70);
    var obj35 = CreateObject(VINE, 412, 888, -1);
    obj35->SetCon(70);
    CreateObject(VINE, 738, 893, -1);
    CreateObject(VINE, 738, 873, -1);
    var obj36 = CreateObject(VINE, 1731, 886, -1);
    obj36->SetCon(60);
    var obj37 = CreateObject(VINE, 1739, 886, -1);
    obj37->SetCon(60);
    CreateObject(VINE, 1756, 896, -1);
    var obj38 = CreateObject(VINE, 1899, 875, -1);
    obj38->SetCon(70);
    var obj39 = CreateObject(VINE, 1889, 878, -1);
    obj39->SetCon(70);
    CreateObject(VINE, 739, 924, -1);
    var obj40 = CreateObject(VINE, 989, 917, -1);
    obj40->SetCon(60);
    var obj41 = CreateObject(VINE, 981, 917, -1);
    obj41->SetCon(50);
    CreateObject(VINE, 740, 955, -1);
}

protected func PlaceWood() {
    var ids = [TRE1, TRE2, TRE3];
		for(var i = 0; i <= 12; i++) {
				PlaceTree(ids[Random(GetLength(ids))], 0, 0, LandscapeWidth()/2, LandscapeHeight());
		}
		for(var i = 0; i <= 12; i++) {
				PlaceTree(ids[Random(GetLength(ids))], LandscapeWidth()/2, 0, LandscapeWidth()/2, LandscapeHeight());
		}
}

protected func PlaceTree(id tree, x, y, wdt, hgt) {
		PlaceVegetation(tree, x, y, wdt, hgt, 100000);
}
