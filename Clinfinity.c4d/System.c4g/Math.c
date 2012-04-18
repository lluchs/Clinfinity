/* Mathematische Funktionen */

#strict 2

static const RoundUp = 1, RoundDown = 2;

global func Round(int iValue, int iNum, int iDir) {
	var iMod = iValue % iNum;
	if(iDir == RoundDown || (!iDir && Abs(iMod) < iNum / 2))
		iValue -= iMod;
	else if(iDir == RoundUp || (!iDir && Abs(iMod) >= iNum / 2))
		iValue += iNum - iMod;
	return iValue;
}

global func ChangeRange(int iValue, int iOldMin, int iOldMax, int iMin, int iMax) {
	return (iValue - iOldMin) * (iMax - iMin) / (iOldMax - iOldMin) + iMin;
}
