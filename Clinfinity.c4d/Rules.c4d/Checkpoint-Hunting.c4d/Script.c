/*-- Checkpoint-Hunting --*/

#strict 2



func Activate(pClonk)
{
	//Beschreibung anzeigen
	MessageWindow(GetDesc() , pClonk);

	//Schon fertig...
	return true;
}