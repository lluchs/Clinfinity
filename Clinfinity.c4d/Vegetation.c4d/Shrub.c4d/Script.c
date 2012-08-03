/*-- Plants --*/

#strict 2

static const Actions = 2;

protected func Initialize(obj) 
{ 
  SetAction(Format("Shrub%d", Random(Actions) + 1));
  DoCon(-30,obj);
  
  /*if (Random(5))
    DoCon(-Random(20) - 40);
  else
    DoCon(-Random(50) - 20);
    
  SetPhase(Random(2));
  if (!Random(20)) 
    SetPhase(2);
  // Zufällige Richtung
  if (Random(2))
   SetDir(DIR_Right)
*/
  // Drehung nach Erdoberfläche
  var x_off = 18 * GetCon() / 100;
  var y_off = 15 * GetCon() / 100;
  var slope = GetSolidOffset(-x_off, y_off) - GetSolidOffset(x_off, y_off);
  SetR(slope);
  // Höhe anpassen
  while (!GBackSolid(0, 5))
    SetPosition(GetX(), GetY() + 1);
}

private func GetSolidOffset(int x, int y)
{
  var i;
  for (i = -10; GBackSolid(x, y - i) && (i < 10); i++);
  return(i);
}

// Bei nahen Explosionen
public func OnShockwaveHit(iLevel,iX,iY)
{
 var con=(40*GetCon())/100;
 iLevel=40+iLevel/2;
 for(var cnt=0;cnt<10+Random(10);cnt++)
  CreateParticle("GrassBlade",RandomX(-con/2,con/2),-1,RandomX(-iLevel/3 ,iLevel/3),RandomX(-2*iLevel/3,-iLevel/3),30+Random(30),RGB(255,255,255),0,0);
 return(RemoveObject());
}

// Kann immer von Schockwellen getroffen werden
public func CanBeHitByShockwaves(){return(true);}
public func BlastObjectsShockwaveCheck(){return(true);}
