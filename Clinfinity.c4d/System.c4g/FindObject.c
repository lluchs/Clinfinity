/* Helpers! */

#strict 2

global func GetRect(id idDef, &ox, &oy, &wdt, &hgt) {
	ox = GetDefCoreVal("Offset", "DefCore", idDef, 0);
	oy = GetDefCoreVal("Offset", "DefCore", idDef, 1);
	wdt = GetDefCoreVal("Width", "DefCore", idDef);
	hgt = GetDefCoreVal("Height", "DefCore", idDef);
} 

global func Find_AtObject(object pObj) {
	if (!pObj) pObj = this;
	if (!pObj) return;
	var ox, oy, wdt, hgt;
	GetRect(GetID(pObj), ox, oy, wdt, hgt);
	var xpos = GetX(pObj);
	var ypos = GetY(pObj);
	return([C4FO_AtRect, xpos + ox, ypos + oy, wdt, hgt]);
}

global func Find_ActionTarget2(object pObj) {
	return Find_Func("IsActionTarget2", pObj);
}

global func Find_ActionTargets(object pTarget) {
	return Find_Or(Find_ActionTarget(pTarget), Find_ActionTarget2(pTarget));
}

global func IsActionTarget2 (object pObj) {
	return GetActionTarget(1) == pObj;
}

global func Find_Angle() {
	return Find_Func("IsAtAngle", this, ...);
}

global func IsAtAngle(object pObj, int iAngle1, int iAngle2, int iX, int iY) {
	iX += pObj -> GetX();
	iY += pObj -> GetY();
	
	var iAngle = Angle(iX, iY, GetX(), GetY());
	
	//Log("%v - %v: w1 %d; w2 %d; w %d", pObj, this, iAngle1, iAngle2, iAngle);
	
	if(iAngle <= iAngle1 && iAngle >= iAngle2)
		return 1;
	
	// Spezialfall: Winkel 1 kleiner als Winkel 2, z.B. 10; 350
	if(iAngle1 < iAngle2 && (iAngle <= iAngle1 || iAngle >= iAngle2))
		return 1;
}
