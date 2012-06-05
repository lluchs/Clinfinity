/*-- Drain --*/

#strict 2

local amount;
local drain;
local iMaterial;
local fNoFountain;
protected func Initialize()
{
  SetAction("Drain");
  amount = 2;
  iMaterial = Material("Water");
  return 1;
}

private func Draining()
{
  if(GBackSolid()) SetPosition(GetX(), GetY()-1);   
  var i;
  var mat;
  if(!drain)
  {
    i=amount;
    if(!fNoFountain)
      while(i--) InsertMaterial(iMaterial,0,-1,RandomX(-6,6),-40);
    else
      while(i--) InsertMaterial(iMaterial);
    return;
  }
  mat = iMaterial;
  //mat = GetMaterial(GetX(drain)-GetX(), GetX(drain)-GetX());
  i = drain->ExtractMaterialAmount(0, 0, mat, amount);
  Message("%d %s", this, i, MaterialName(mat));
  while(i--) InsertMaterial(mat);
  return 1;
}
