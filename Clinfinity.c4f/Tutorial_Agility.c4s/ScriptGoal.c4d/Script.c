/*-- Scriptdefiniertes Spielziel --*/

#strict
#include GOAL

local goalFulfilled;
                
public func IsFulfilled()
{
  return(goalFulfilled);
}

public func Fulfill()
{
  goalFulfilled = 1;
}

protected func Activate(iPlayer)
{
	return(MessageWindow(GetDesc(), iPlayer));
}
