/*-- Neues Szenario --*/

#strict

func Initialize() {
  //Ressource Buildings
  CreateConstruction(RFLN,810,550,-1,100,1,0); //Flintfactory

  //Crumbling Islands
  var CIsleL = CreateObject(ISLE,760,810);
  CIsleL -> SetClrModulation(RGBa(200,215,255,50));
  
  SetSkyParallax(1,20,0,1,0);
}

func InitializePlayer(int plr) {
	CreateMatSys(plr);
}
