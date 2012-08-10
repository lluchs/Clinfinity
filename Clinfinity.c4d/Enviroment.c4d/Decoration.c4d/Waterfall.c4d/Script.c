/*-- Wasserfall --*/

#strict

protected func Initialize(){
  DigFree(0,0,2);
  }

protected func CastPXS2()
{
   CastPXS("Water",5,20,0);
  return(1);
}
