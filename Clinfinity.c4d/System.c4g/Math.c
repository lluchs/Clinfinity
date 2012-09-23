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

/* Rotates the point (x, y) around (ox, oy) by angle degrees. */
global func Rotate(int angle, int &x, int &y, int ox, int oy, int prec) {
	var xr = Cos(angle, x, prec) - Sin(angle, y, prec),
	    yr = Sin(angle, x, prec) + Cos(angle, y, prec);
	x = xr + ox;
	y = yr + oy;
}
