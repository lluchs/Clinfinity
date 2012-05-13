/*-- Vogelgezwitscher --*/

#strict

protected func Initialize()
{
  SetAction("Sing");
  SetPhase(Random(20));
  return(1);
}

private func Singing()
{
  // Bei Nacht schlafen die Vögelchen
  if (IsNight())
    return(0);
  // Bei Tag singen sie
  if (!Random(8))
    Sound("BirdSong*",1);
}  