/*-- Raincatcher --*/

#strict 2
#include L_SS

// Anzahl Materialpixel, die in einen Fänger passen
private func MaxFill() { return 1000; }

protected func LiquidCheck() {
	if(GetFill() == MaxFill())
		return;
	var iX = 0, iY = -16;
	var iAmount 	= ExtractMaterialAmount(iX, iY, Material("Water"), 10),
			iChanged 	= DoFill(iAmount),
			iBack 		= iAmount - iChanged;
	//Log("iAmount: %d iChanged: %d iBack: %d", iAmount, iChanged, iBack);
	while(iBack--)
		InsertMaterial(Material("Water"), iX, iY);
	return 1;
}

private func FillBarrel(object pFass) {
	var iBarrelChange = pFass -> BarrelDoFill(pFass -> BarrelMaxFill(), Material("Water"));
	pFass -> ChangeDef(WBRL);
	var iChange = DoFill(-iBarrelChange);
	var iChange2 = iChange + iBarrelChange;
	//Log("iBarrelChange: %d iChange: %d iChange2: %d", iBarrelChange, iChange, iChange2);
	pFass -> BarrelDoFill(-iChange2, Material("Water"));
	return 1;
}

/* Steuerung */

protected func ControlDig(object pClonk) {
	[$TxtFillBarrel$, Image=WBRL]
	var pFass = FindObject2(Find_Container(pClonk), Find_Or(Find_ID(EmptyBarrelID()), Find_ID(WBRL)), Find_Not(Find_Func("BarrelIsFull")));
	if(pFass && GetFill())
		return FillBarrel(pFass);
	Sound("Error");
	return;
}

protected func ContextFillBarrel() {
	[$TxtFillBarrel$, Image=WBRL]
	return ControlDig(...);
}
