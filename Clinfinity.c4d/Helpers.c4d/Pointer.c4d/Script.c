/*  Script: Pointer
	Provides a pointer highlighting an object for a player. */

#strict 2

static const PT0D_Distance = 200;
static const PT0D_Duration = 255;

local alphaEase;

func Initialize() {
    SetVisibility(VIS_None, this);
	alphaEase = CreateEaseFunction("circle-out", PT0D_Duration);
    return true;
}

/*  Constructor: CreatePointer
	Creates a pointer.

	Parameters:
	iPlr      - The player who will see the pointer.
	pTarget   - The object the pointer will point at.
	iColor    - Pointer's color modulation.
	szMessage - A message shown above the pointer.

	Returns:
	The created pointer. */
global func CreatePointer(int iPlr, object pTarget, int iColor, string szMessage) {
    if(!pTarget) return false;
    if(!iColor) iColor = GetPlrColorDw(iPlr);
	if(!szMessage) szMessage = "";

    var pPointer = CreateObject(PT0D, 0, 0, iPlr);
    pPointer->SetClrModulation(iColor);
    AddEffect("Pointing", pPointer, 100, 1, pPointer, 0, iPlr, pTarget, szMessage);
    return pPointer;
}

/*  Function: PointOut
	Creates a pointer for every player pointing to the calling object.
	
	Parameters:
	color   - Pointer's color modulation.
	message - A message shown above the pointer. */
global func PointOut(int color, string message) {
	for(var count = GetPlayerCount(), i = 0; i < count; i++) {
		var p = GetPlayerByIndex(i);
		CreatePointer(p, this, color, message);
	}
	return count;
}

func FxPointingStart(object pTarget, int iIndex, int iTemp, int iPlr, object pObj, string szMsg) {
    SetVisibility(VIS_Owner, pTarget);
	CreateParticle("PSpark", 0, 0, 0, 0, 500, GetClrModulation(), this, true);
    EffectVar(0, pTarget, iIndex) = iPlr;
    EffectVar(1, pTarget, iIndex) = pObj;
    EffectVar(2, pTarget, iIndex) = szMsg;
    return true;
}

func FxPointingTimer(object pTarget, int iIndex, int iTime) {
    if(iTime > PT0D_Duration) {
        RemoveObject(pTarget);
        return -1;
    }

    var pClonk = GetCursor(EffectVar(0, pTarget, iIndex));
    var pObj = EffectVar(1, pTarget, iIndex);
    var szMsg = EffectVar(2, pTarget, iIndex);

    // Zeiger hat kein Ziel mehr? Löschen.
    if(!pObj) {
        RemoveObject(pTarget);
        return -1;
    }

    var iAng = Angle(GetX(pClonk), GetY(pClonk), GetX(pObj), GetY(pObj)) - 90;
    var iDst = Min(PT0D_Distance, Distance(GetX(pClonk), GetY(pClonk), GetX(pObj), GetY(pObj)) / 2);

    SetR(iAng + 90, pTarget);
    SetPosition(GetX(pClonk) + Cos(iAng, iDst), GetY(pClonk) + Sin(iAng, iDst), pTarget);

    var r, g, b, a;
    SplitRGBaValue(GetClrModulation(pTarget), r, g, b, a);

	a = EvalEase(alphaEase, Min(255, iTime));
	SetClrModulation(RGBa(r, g, b, a), pTarget);
    Message("<c %x>%s</c>", pTarget, RGBa(r, g, b, 255 - a), szMsg);

    return true;
}
