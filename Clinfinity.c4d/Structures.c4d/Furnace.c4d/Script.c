/*-- Schmelze --*/

#strict 2

#include L_SS

static plr;

protected func ControlUp(pCaller){
  //GetPlayer
  var plr = pCaller->GetOwner();
  
  if(MatSysGetFill(plr, ROCK) >= 1){
    SetAction("Start");
    Message("produziert!");
    MatSysDoFill(-1, plr, ROCK);
    }
  else{ 
    Sound("Error"); 
    Message("Nicht gen�gend Baumaterial"); 
    }
}

protected func Finish(){
  //GetPlayer
  MatSysDoFill(1, plr, METL);
  }

/*nur als Animationstest

public func ControlDigDouble() {
  }

public func ControlUp() {
  SetAction("Stop");
  Message("fertig");
  Sound("Pshshsh");
  }*/
