/*-- Firefly --*/

#strict


private func Flying() { // wird alle 3 Frames aufgerufen
  var xdir,ydir;

  // nichts tun
  if(Random(4)) return(1);
  // Geschwindigkeit zufällig bestimmen
  xdir=Random(13)-6;
  ydir=Random(9)-4;
  if(GBackLiquid(xdir,ydir)) return(SetSpeed());
  SetSpeed(xdir,ydir);
}

protected func Check() { // wird alle 18Frames aufgerufen

  // Verschüttet oder im Wasser?: Sterben
  if(GBackSemiSolid()) return(Death());
	
	// too high?
	if(!GBackSemiSolid(0, 30))
	{
		if(!Random(2))
			SetSpeed(RandomX(-10, 10), RandomX(5, 10));
	}

}

protected func Initialize() {
  SetAction("Fly");
  SetPhase(Random(6));
  
  AddLight(60, RGB(255,255,200), this);
}

public func CatchBlow()	{ RemoveObject(); }
public func Damage()	{ RemoveObject(); }
protected func Death()	{ RemoveObject(); }
