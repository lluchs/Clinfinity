/*  Script: Pointer
	Provides a pointer highlighting an object for a player. */

#strict 2

static const PT0D_Distance = 200;

func Initialize() {
    SetVisibility(VIS_None, this);
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
    if(!pTarget || !szMessage) return false;
    if(!iColor) iColor = GetPlrColorDw(iPlr);

    var pPointer = CreateObject(PT0D, 0, 0, iPlr);
    AddEffect("Pointing", pPointer, 100, 1, 0, PT0D, iPlr, pTarget, szMessage);
    SetClrModulation(iColor, pPointer);
    return pPointer;
}

func FxPointingStart(object pTarget, int iIndex, int iTemp, int iPlr, object pObj, string szMsg) {
    SetVisibility(VIS_Owner, pTarget);
    EffectVar(0, pTarget, iIndex) = iPlr;
    EffectVar(1, pTarget, iIndex) = pObj;
    EffectVar(2, pTarget, iIndex) = szMsg;
    return true;
}

func FxPointingTimer(object pTarget, int iIndex, int iTime) {
    if(iTime > 510) {
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

    if(iTime % 2 == 0)
        SetClrModulation(RGBa(r, g, b, Min(255, a + 1)), pTarget);
    Message("<c %x>%s</c>", pTarget, RGBa(r, g, b, Max(0, 255 - a)), szMsg);

    return true;
}
