#strict
#include TREE

private func ReproductionAreaSize() { return(80); }
private func ReproductionRate()     { return(23); }
private func MaxTreeCount()         { return(4); }

protected func Damage() {}

protected func Incineration() { SetCategory(16); }

public func Construction() {
  SetAction("Exist");		// zuf�llige Animationsphase

  SetDir(Random(4));
  SetPosition(GetX(),GetY()-5);	// baum-syndrom

  var dwRGB = HSL(RandomX(0,38),RandomX(127,255),RandomX(64,160));
  SetColorDw(dwRGB);
}

public func GetMyColor(dwColor) {

  var dwRGB = dwColor;
  var h,s,l,a;
  dwRGB = RGB2HSL(dwRGB); 
  SplitRGBaValue(dwRGB,h,s,l,a);
  h = h+RandomX(-10,+10);  // jede Farbe ist z�chtbar!
  s = BoundBy(s+RandomX(-15,+15),127,255);
  l = BoundBy(l+RandomX(-10,+10),64,227);
  dwRGB = HSL(h,s,l);

  SetColorDw(dwRGB);
}

public func Reproduction()
{
  // Ist noch Platz f�r einen Baum?
  var iSize = ReproductionAreaSize();
  var iOffset = iSize / -2;
  if (ObjectCount(GetID(), iOffset, iOffset, iSize, iSize)<MaxTreeCount()) {
    // OK, hin damit
    var pMush = PlaceVegetation(GetID(), iOffset, iOffset, iSize, iSize, 10);
    if(pMush) pMush->GetMyColor(GetColorDw());
    return(1);
  }
  // Kein Platz ;'(
  return(0);
}


public func ContextChop(pClonk)		// per Kontextmen� pfl�cken
{
  [$TxtPick$|Image=MUSH|Condition=IsStanding]
  Pick();
  return(1);
}

protected func RejectEntrance() {
  if(GetCon()<100) return(1);
}

public func Entrance() {		// per Einsammeln pfl�cken
  [$TxtPick$]
  if(IsStanding()) Pick();
  return(1);
}

public func Pick() {			// pfl�cken
  Sound("Grab");
  var iDir = GetDir();
  SetAction("Idle");
  SetAction("Exist");
  SetDir(iDir);
  SetCategory(16);
}


public func Activate(object pClonk)	// essen
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

protected func Eat(object pClonk)
{
    pClonk->~Feed(20);
    DoMagicEnergy(4,pClonk);
    RemoveObject();
    return(1);
}

protected func Existing() {
  if(IsStanding()) return();
  // re-seed
  if(!Contained())
    if(!GetYDir())
      if(!GetXDir())
        SetCategory(C4D_StaticBack);
}

public func IsStanding() { return(GetCategory() & C4D_StaticBack); }	// steht noch

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(80); }
func IsTree() { return false; }	// kann nicht zur Holzproduktion verwendet werden

protected func Hit() { Sound("WoodHit*"); }
