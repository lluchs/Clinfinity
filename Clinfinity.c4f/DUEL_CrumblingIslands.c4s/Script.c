/*-- Neues Szenario --*/

#strict

func Initialize() {
  //Crumbling Islands
  var CIsleL = CreateObject(ISLE,760,810);
  CIsleL -> SetClrModulation(RGBa(200,215,255,50));
  
  SetSkyParallax(1,20,0,1,0);
}
