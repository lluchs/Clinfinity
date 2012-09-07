/*-- Grass --*/

#strict

protected func Initialize() 
{ 
  // Zuf�llige Gr��e
  if (Random(5))
    DoCon(-Random(20) - 40);
  else
    DoCon(-Random(50) - 20);
  // Zuf�llige Form  
  SetAction("Grass");
  SetPhase(Random(2));
  if (!Random(20)) SetPhase(2);
  // Zuf�llige Richtung
  if (Random(2)) SetDir(DIR_Right);
  // Drehung nach Erdoberfl�che
  var x_off = 18 * GetCon() / 100;
  var y_off = 15 * GetCon() / 100;
  var slope = GetSolidOffset(-x_off, y_off) - GetSolidOffset(x_off, y_off);
  SetR(slope);
  // H�he anpassen
  while (!GBackSolid(0, 5))
    SetPosition(GetX(), GetY() + 1);
  // Gras bleibt hinter B�umen
  MoveBehindTrees();
  ScheduleCall(0, "ListenToTime", 1);
}

protected func ListenToTime() {
  var time = FindObject2(Find_ID(TIME));
  if(time != 0) {
    time->AddEventListener(this, "OnNight");
  }
}

private func GetSolidOffset(int x, int y)
{
  var i;
  for (i = -15; GBackSolid(x, y - i) && (i < 15); i++);
  return(i);
}

private func MoveBehindTrees()
{
  var obj;
  while (obj = FindObject(0, 1,1, 0,0, OCF_Chop(), 0,0, NoContainer(), obj))
    if (obj->~IsTree() && (obj->GetCategory() & C4D_StaticBack))
      SetObjectOrder(obj, this(), 1);
}

// Bei nahen Explosionen
public func OnShockwaveHit(iLevel,iX,iY)
{
 var con=(40*GetCon())/100;
 iLevel=40+iLevel/2;
 for(var cnt=0;cnt<15+Random(10);cnt++)
  CreateParticle("GrassBlade",RandomX(-con/2,con/2),-1,RandomX(-iLevel/3 ,iLevel/3),RandomX(-2*iLevel/3,-iLevel/3),30+Random(30),RGB(255,255,255),0,0);
 return(RemoveObject());
}

public func OnNight(object source) {
  if(!Random(10)) {
    IRRL->SpawnSwarm(GetX(), GetY(), RandomX(5, 10), this);
  }
}

// Kann immer von Schockwellen getroffen werden
public func CanBeHitByShockwaves(){return(true);}
public func BlastObjectsShockwaveCheck(){return(true);}
