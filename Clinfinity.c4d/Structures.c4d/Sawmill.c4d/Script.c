/*-- Sawmill --*/

#strict 2

#include STBO
#include L_DC
#include L_SS

public func MaxDamage() { return 40; }

protected func Initialize() {
  SetAction("GrabWood");
}
  
protected func Finish() {
  MatSysDoFill(1, 0, WOOD);
}
