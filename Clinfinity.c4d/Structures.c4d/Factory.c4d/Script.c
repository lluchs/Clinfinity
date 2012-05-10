/*-- Neues Script --*/

#strict

  
public func ControlDigDouble(){
  SetAction("ProduceTest");
  };
  
public func ControlUp(){
  SetAction("None");
  Sound("finish*");
  };
  
protected func Smoketest(){
  CreateParticle("Smoke",-18,-18,0,0,150,RGBa(0,0,0,0));
  CreateParticle("Smoke",-40,-18,0,0,150,RGBa(0,0,0,0));
  };