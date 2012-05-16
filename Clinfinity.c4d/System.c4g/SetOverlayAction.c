/* Aktion mit Animation als Overlay setzen */

/* IntOverlayAction Vars
 * 0: szAction
 * 1: iOverlay
 * 2: iLength
 * 3: iFrame (or just i)
 * 4: fReverse
 * 5: szPhaseCall
 * 6: szEndCall
 * 7: fNoCalls
 * 8: fNoRotationChecks
 */

#strict 2

global func OverlayShiftX(int iWidth) {
	return 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 0) + iWidth/2);
}

global func OverlayShiftY(int iHeight) {
	return 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 1) + iHeight/2);
}

/* Methode 1: alle Einzelframes in der ActMap.txt */

global func OverlayActionInit(string szAction, id ID) {
	if(!ID && !(ID = GetID()))
		return;
	var iLength = GetActMapVal("Length", szAction, ID);
	if(!iLength)
		return;
	var iFacetX = GetActMapVal("Facet", szAction, ID, 0),
		iFacetY = GetActMapVal("Facet", szAction, ID, 1),
		iFacetWidth = GetActMapVal("Facet", szAction, ID, 2),
		iFacetHeight = GetActMapVal("Facet", szAction, ID, 3),
		iFacetPX = GetActMapVal("Facet", szAction, ID, 4),
		iFacetPY = GetActMapVal("Facet", szAction, ID, 5);
	for(var i = 0; i < iLength; i++) {
		Log("[Action]");
		Log("Name=%s%d", szAction, i);
		Log("Length=1");
		Log("Facet=%d,%d,%d,%d,%d,%d", iFacetX + iFacetWidth * i, iFacetY, iFacetWidth, iFacetHeight, iFacetPX, iFacetPY);
	}
	return iLength;
}

// Grafik einer nicht animierten Aktion setzen
global func SetStillOverlayAction(string szAction, int iOverlay) {
	if(!iOverlay)
		iOverlay = GetUnusedOverlayID(1);
	SetGraphics(0, this, GetID(), iOverlay, GFXOV_MODE_Action, szAction);
	if(GetDir() == DIR_Left)
		SetObjDrawTransform(1000, 0, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 0) + GetActMapVal("Facet", szAction, GetID(), 2)/2 + GetActMapVal("Facet", szAction, GetID(), 4)), 0, 1000, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 1) + GetActMapVal("Facet", szAction, GetID(), 3)/2 + GetActMapVal("Facet", szAction, GetID(), 5)), this, iOverlay);
	else if(GetDir() == DIR_Right) {
		SetObjDrawTransform(-1000, 0, 1000*(-GetDefCoreVal("Offset", "DefCore", GetID(), 0) - GetActMapVal("Facet", szAction, GetID(), 2)/2 - GetActMapVal("Facet", szAction, GetID(), 4)), 0, 1000, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 1) + GetActMapVal("Facet", szAction, GetID(), 3)/2 + GetActMapVal("Facet", szAction, GetID(), 5)), this, iOverlay);
	}
}

global func SetOverlayAction(string szAction, int iOverlay, bool fReverse, bool fNoCalls, string szPhaseCall, string szEndCall, bool fNoRotation) {
	if(!this) return;
	if(!iOverlay)
		iOverlay = GetUnusedOverlayID(1);
	if(!GetDefRotate(GetID()) || GetDefRotate(GetID()) == "") fNoRotation = true;
	var iDelay;
	if(fNoRotation) iDelay = GetActMapVal("Delay", szAction, GetID());
	else iDelay = 1;
	if(iDelay == 1) fNoRotation = true; //This might look hacky, But the rotation check is not needed if the gfx is updated every frame
	AddEffect("IntOverlayAction", this, 100, iDelay, this, 0, szAction, iOverlay, fReverse, [fNoCalls, szPhaseCall, szEndCall, fNoRotation]);
	return iOverlay;
}

global func FxIntOverlayActionStart(object pTarget, int iEffectNumber, int iTemp, string szAction, int iOverlay, bool fReverse, array aPars) {
	if(iTemp)
		return;
	var iLength = GetActMapVal("Length", szAction, GetID());
	EffectVar(0, pTarget, iEffectNumber) = szAction;
	EffectVar(1, pTarget, iEffectNumber) = iOverlay;
	EffectVar(2, pTarget, iEffectNumber) = iLength;
	// 3: i
	if(fReverse) {
		EffectVar(3, pTarget, iEffectNumber) = iLength - 1;
		EffectVar(4, pTarget, iEffectNumber) = true;
	} else {
		EffectVar(3, pTarget, iEffectNumber) = 0; //Calling callbacks from script is hack.
	}
	if(!aPars[0]) {
		var szStartCall = GetActMapVal("StartCall", szAction, GetID()),
				szPhaseCall = GetActMapVal("PhaseCall", szAction, GetID()),
				szEndCall = GetActMapVal("EndCall", szAction, GetID());
		if(szStartCall)
			Call(szStartCall);
		EffectVar(5, pTarget, iEffectNumber) = szPhaseCall;
		EffectVar(6, pTarget, iEffectNumber) = szEndCall;
	}
	else {
		EffectVar(5, pTarget, iEffectNumber) = aPars[1];
		EffectVar(6, pTarget, iEffectNumber) = aPars[2];
		EffectVar(7, pTarget, iEffectNumber) = true;
	}
	EffectVar(7, pTarget, iEffectNumber) = aPars[3];
	Sound(GetActMapVal("Sound", szAction, GetID(pTarget)), 0, pTarget, 0, 0, +1);
	UpdateActionOverlayTransform(pTarget, iOverlay, szAction, EffectVar(3, pTarget, iEffectNumber));
}

global func FxIntOverlayActionTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	var szAction = EffectVar(0, pTarget, iEffectNumber),
		i = EffectVar(3, pTarget, iEffectNumber),
		iOverlay = EffectVar(1, pTarget, iEffectNumber),
		fNoRotationChecks = EffectVar(7, pTarget, iEffectNumber),
		iDelay = GetActMapVal("Delay", szAction, GetID(pTarget));
	if(!iDelay || iDelay == "") iDelay = 1;
	if(fNoRotationChecks || !(iEffectTime%GetActMapVal("Delay", szAction, GetID(pTarget)))) { //a new frame, just a gfx update
		var iLength = EffectVar(2, pTarget, iEffectNumber),
			fReverse = EffectVar(4, pTarget, iEffectNumber),
			szPhaseCall = EffectVar(5, pTarget, iEffectNumber);
		UpdateActionOverlayTransform(pTarget, iOverlay, szAction, i);
		if(szPhaseCall)
			Call(szPhaseCall, szAction, iOverlay, iLength, i, fReverse);
		if(fReverse) {
			i--;
			if(i < 0) {
				var szNextAction = GetActMapVal("NextAction", szAction, GetID(pTarget));
				if(szNextAction && szNextAction != "") {
					Sound(GetActMapVal("Sound", szAction, GetID(pTarget)), 0, pTarget, 0, 0, +1); //Preserve the sound if
					FxIntOverlayActionStop(pTarget, iEffectNumber);
					SetOverlayAction(szNextAction, iOverlay, GetActMapVal("Reverse", szNextAction, GetID()));
					Sound(GetActMapVal("Sound", szAction, GetID(pTarget)), 0, pTarget, 0, 0, -1);
					i = EffectVar(3, pTarget, iEffectNumber);
				}
				else return -1;
			}
		}
		else {
			i++;
			if(i == iLength) {
				var szNextAction = GetActMapVal("NextAction", szAction, GetID(pTarget));
				if(szNextAction && szNextAction != "") {
					Sound(GetActMapVal("Sound", szAction, GetID(pTarget)), 0, pTarget, 0, 0, +1); //Preserve the sound if
					FxIntOverlayActionStop(pTarget, iEffectNumber);
					SetOverlayAction(szNextAction, iOverlay, GetActMapVal("Reverse", szNextAction, GetID()));
					Sound(GetActMapVal("Sound", szAction, GetID(pTarget)), 0, pTarget, 0, 0, -1);
					i = EffectVar(3, pTarget, iEffectNumber);
				}
				else return -1;
			}/* else if(i > iLength) {
				FatalError("An Action did not break when it should.");
			}*/
		}
		EffectVar(3, pTarget, iEffectNumber) = i;
	} else {
		//UpdateActionOverlayTransform(pTarget, iOverlay, szAction, i);
	}
}

global func FxIntOverlayActionStop(object pTarget, int iEffectNumber) {
	if(	EffectVar(4, pTarget, iEffectNumber) && EffectVar(3, pTarget, iEffectNumber) != 0 ||
		!EffectVar(4, pTarget, iEffectNumber) && EffectVar(3, pTarget, iEffectNumber) < EffectVar(2, pTarget, iEffectNumber)-1) 
	{
		var szAbortCall = EffectVar(6, pTarget, iEffectNumber);
		if(szAbortCall) Call(szAbortCall, EffectVar(0, pTarget, iEffectNumber), EffectVar(1, pTarget, iEffectNumber));		
	} else {
		var szEndCall = EffectVar(6, pTarget, iEffectNumber);
		if(szEndCall) Call(szEndCall, EffectVar(0, pTarget, iEffectNumber), EffectVar(1, pTarget, iEffectNumber));
	}
	Sound(GetActMapVal("Sound", EffectVar(0, pTarget, iEffectNumber), GetID(pTarget)), 0, pTarget, 0, 0, -1);
}

//I must guess that this function does not work correctly.
global func FxIntOverlayActionEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, string szAction, int iOverlay, bool fReverse, array aPars) {
	if(szNewEffectName != "IntOverlayAction") return 0;
	if(iOverlay == EffectVar(1, pTarget, iEffectNumber)) {
		return -2;
	}
}

global func FxIntOverlayActionAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, string szAction, int iOverlay, bool fReverse, array aPars) {
	FxIntOverlayActionStop(pTarget, iEffectNumber);
	FxIntOverlayActionStart(pTarget, iEffectNumber, false, szAction, iOverlay, fReverse, aPars);
}

global func GetOverlayAction(int iOverlay, object pTarget) {
	if(!pTarget && !(pTarget=this)) return false;
	var iEffect;
	for(var i; iEffect=GetEffect("IntOverlayAction", this, i); i++) {
		if(EffectVar(1, pTarget, iEffect) == iOverlay) return EffectVar(0, pTarget, iEffect);
	}
	return "Idle";
}

global func UpdateActionOverlayTransform(object pTarget, iOverlay, szAction, i) {
	SetGraphics(0, this, GetID(), iOverlay, GFXOV_MODE_Action, Format("%s%d", szAction, i));
	var fsin=Sin(-GetR(0), 1000), fcos=Cos(-GetR(), 1000);
	if(GetDir() == DIR_Left)
		SetObjDrawTransform(fcos, fsin, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 0) + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 2)/2 + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 4)), -fsin, fcos, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 1) + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 3)/2 + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 5)), this, iOverlay);
	else if(GetDir() == DIR_Right) {
		SetObjDrawTransform(-fcos, fsin, 1000*(-GetDefCoreVal("Offset", "DefCore", GetID(), 0) - GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 2)/2 - GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 4)), -fsin, fcos, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 1) + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 3)/2 + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 5)), this, iOverlay);
	}
}
