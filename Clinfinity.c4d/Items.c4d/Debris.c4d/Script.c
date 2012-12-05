/*-- Fragment --*/

#strict

func Initialize() {
  SetAction(Format("g%d",Random(4)+1));
  //SetGraphics (0, 0, 0, 0, GFXOV_MODE_Action, Format("g%d",Random(4)+1));
  return(1);
}

protected func Activate(){
  var i = Random(5);
  while(i--) {
  	CastObject([METL] [Random(2)]);
  }
  Sound("Connect");
  RemoveObject();
}

private func CastObject(id ID) {
	var pObj = CreateObject(ID, 0, 0, GetOwner());
	pObj -> SetSpeed(RandomX(-10, 10), RandomX(-10, 10));
	return 1;
}

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}
