/*-- Schmelze --*/

#strict 2

#include L_SS


//nur als Animationstest

public func ControlDigDouble() {
  SetAction("Start");
  Message("produziert!");
  }

public func ControlUp() {
  SetAction("Stop");
  Message("fertig");
  Sound("Pshshsh");
  }