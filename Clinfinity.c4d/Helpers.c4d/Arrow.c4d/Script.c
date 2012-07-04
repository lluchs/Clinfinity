/*-- Pfeil --*/

#strict

protected func Initialize() { SetAction("Turn"); }

static arrow_target;

global func SetArrow(iX,iY) 
{
  if(FindObject2(Find_ID(_AR1),Find_AtPoint(iX, iY - 16))) return;
  RemoveObject(FindObject(_AR1));
  CreateObject(_AR1,iX,iY,-1);
  arrow_target = 0;
}
  
global func RemoveArrow() 
{
  RemoveObject(FindObject(_AR1));
  arrow_target = 0;
}

global func SetArrowToObj(object obj)
{
	if (arrow_target == obj) return();
  RemoveObject(FindObject(_AR1));
  CreateObject(_AR1, obj->GetX() + obj->GetDefCoreVal("Width") / 4, obj->GetY() - obj->GetDefCoreVal("Height") / 2 - 10, -1);
  arrow_target = obj;
}
 
