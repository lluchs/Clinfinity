/*-- BirdTweet --*/

#strict 2

protected func Initialize()
{
  SetAction("Sing");
  SetPhase(Random(20));
  return(1);
}

private func Singing(){
  // is night? Sleep!
  if (IsNight())
    return(0);
    
  // check if there are some trees left and is day
    if(Random(8)){
      if(ObjectCount2(Find_Func("IsTree")) >= 10)
      Sound("BirdSong*",1);
      }
    else{
      Sound("Crow*",1);
      }
}  
