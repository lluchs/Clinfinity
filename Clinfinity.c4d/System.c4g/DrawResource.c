#strict 2

global func FxDrawResourceStart(object target, int effectNumber, int temporary, x, y, materialNumber) {
	if(temporary == 0) {
		EffectVar(0, target, effectNumber) = x;
		EffectVar(1, target, effectNumber) = y;
		EffectVar(2, target, effectNumber) = materialNumber;
	}
}

global func FxDrawResourceTimer(object target, int effectNumber, int effectTime) {
	var iX = EffectVar(0, target, effectNumber);
	var iY = EffectVar(1, target, effectNumber);
	var iMat = EffectVar(2, target, effectNumber);
	var sMat = Format("%s-Flat", MaterialName(iMat));
	var xOld = EffectVar(3, target, effectNumber);
	var yOld = EffectVar(4, target, effectNumber);

	var iSize = EffectVar(5, target, effectNumber);
	if(!iSize)
		iSize = RandomX(20, 25);
	else
		iSize = BoundBy(iSize + RandomX(-6, 6), 10, 65);

	var xNew = Cos(effectTime * 5, iSize);
	var yNew = Sin(effectTime * 5, iSize) / 2;

	if(effectTime <= 2) {
		EffectVar(8, target, effectNumber) = xNew;
		EffectVar(9, target, effectNumber) = yNew;
	} else if(effectTime > 72 * 2) {
		xNew = EffectVar(8, target, effectNumber);
		yNew = EffectVar(9, target, effectNumber);
	}
	EffectVar(3, target, effectNumber) = xNew;
	EffectVar(4, target, effectNumber) = yNew;
	EffectVar(5, target, effectNumber) = iSize;

	DrawMaterialQuad(sMat, iX, iY, iX + xOld, iY + yOld, iX + xNew, iY + yNew, iX, iY, true);

	CreateParticle("PSpark", iX + xNew / 2, iY + yNew / 2, 0, 0, iSize * 25, RGBa(GetMaterialColor(iMat, 0, 0), GetMaterialColor(iMat, 0, 1), GetMaterialColor(iMat, 0, 2), 100));

	if(effectTime > 72 * 2)
		return -1;
	else
		return 1;
}