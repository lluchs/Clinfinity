/*-- Neues Objekt --*/

#strict 2

func Initialize() {
  SetAction("Letsroll");
  return(1);
}

protected func DoSmoke(){
  CreateParticle("Smoke", 30, -18, 0, 0, 50, RGBa(255, 255, 255, 0));
}
