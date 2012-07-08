/*-- Liane --*/

#strict

protected func Construction()
{
  // Zufälliges Aussehen
  SetAction("Vine");
  SetPhase(Random(10));
  // In Kürze Position anpassen
  AddEffect("AdjustGrowthPos", this(), 1, 1, this());
}

public func GetVineLength()
{
  return (GetCon() * GetDefCoreVal("Height") / 100);
}

public func FxAdjustGrowthPosTimer()
{
  // Effekt entfernen
  RemoveEffect("AdjustGrowthPos", this());
  // Y-Position anpassen (Wachstumsverschiebung der Engine ausgleichen)
  SetPosition(GetX(), GetY() + GetVineLength());   
}
