/*-- Drain --*/

#strict 2

protected func Initialize()
{
  SetAction("Drain");
  return 1;
}

private func Draining()
{
  if (GBackLiquid())
    ExtractMaterialAmount(0, 0, GetMaterial(), 20);
  return 1;
}
