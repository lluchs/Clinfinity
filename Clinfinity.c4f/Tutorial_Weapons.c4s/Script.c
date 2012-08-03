/*-- Weapon Tutorial --*/

#strict 2

// Positions for Relaunch
static iPlrX, iPlrY, iCounter;

func Initialize() {
	// Decoration
  CreateObject(WTFL, 350, 290); //Waterfall

  CreateObject(GUNR,230,289); //Gun Range
  CreateObject(_CST, 252,227); //Chest
  
	ScriptGo(true);
}
