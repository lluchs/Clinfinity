/*-- Verbrannter Geschützturm --*/

#strict

local cannon;

#include BAS2

public func ReleaseCannon()
{
  if(!cannon) return(0);
  cannon->ComStopDouble();
  cannon = 0;
}

/* Destruction */

protected func Destruction()
{
  if(cannon) ReleaseCannon();
  return(_inherited() );
}

protected func Incineration()
{
  if(cannon) ReleaseCannon();
  return(_inherited() );
}

protected func IncinerationEx()
{
  if(cannon) ReleaseCannon();
  return(_inherited() );
}
