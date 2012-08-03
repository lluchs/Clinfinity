/*-- Weapon Tutorial --*/

#strict 2

// Positions for Relaunch
static iPlrX, iPlrY, iCounter;

func Initialize() {
	// Decoration
  CreateObject(WTFL, 350, 290); //Waterfall

  CreateObject(GUNR,230,289);
	ScriptGo(true);
}
