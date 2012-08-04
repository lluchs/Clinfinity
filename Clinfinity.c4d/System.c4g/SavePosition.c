/*-- SavePosition --*/

#strict 2

global func SavePosition() {
	// Position for Relaunch (currently Tutorial only)
	var clonk = GetCrew();
	iPlrX = clonk->GetX();
	iPlrY = clonk->GetY();
}
