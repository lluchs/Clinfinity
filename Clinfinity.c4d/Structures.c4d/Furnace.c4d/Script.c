/*-- Schmelze --*/

#strict 2

#include L_SS

static plr;

protected func ControlUp(pCaller){
  //GetPlayer
  var plr = pCaller->GetOwner();
  
  if(MatSysGetFill(plr, ROCK) >= 1){
    SetAction("Start");
    MatSysDoFill(-1, plr, ROCK);
    }
  else{ 
    Sound("Error"); 
    Message("$TxtNotEnoughInput$", pCaller); 
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
