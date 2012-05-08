#strict 2

global func DrawResource(int x, int y, int materialNumber, string materialTexture) {
	AddEffect("DrawResource", 0, 1, 1, 0, 0, x, y, materialNumber, materialTexture);
}

static const DrawResourceX = 0;
static const DrawResourceY = 1;
static const DrawResourceMaterialNumber = 2;
static const DrawResourceMaterialTexture = 3;
static const DrawResourceOldX = 4;
static const DrawResourceOldY = 5;
static const DrawResourceSize = 6;
static const DrawResourceNewX = 7;
static const DrawResourceNewY = 8;

global func FxDrawResourceStart(object target, int effectNumber, int temporary, x, y, materialNumber, materialTexture) {
	if(temporary == 0) {
		EffectVar(DrawResourceX, target, effectNumber) = x;
		EffectVar(DrawResourceY, target, effectNumber) = y;
		EffectVar(DrawResourceMaterialNumber, target, effectNumber) = materialNumber;
		EffectVar(DrawResourceMaterialTexture, target, effectNumber) = materialTexture;
	}
}

global func FxDrawResourceTimer(object target, int effectNumber, int effectTime) {
	var iX = EffectVar(DrawResourceX, target, effectNumber);
	var iY = EffectVar(DrawResourceY, target, effectNumber);
	var iMat = EffectVar(DrawResourceMaterialNumber, target, effectNumber);
	var materialTexture = EffectVar(DrawResourceMaterialTexture, target, effectNumber);
	var sMat = Format("%s-%s", MaterialName(iMat), materialTexture);
	var xOld = EffectVar(DrawResourceOldX, target, effectNumber);
	var yOld = EffectVar(DrawResourceOldY, target, effectNumber);

	var iSize = EffectVar(DrawResourceSize, target, effectNumber);
	if(!iSize)
		iSize = RandomX(20, 25);
	else
		iSize = BoundBy(iSize + RandomX(-6, 6), 10, 65);

	var xNew = Cos(effectTime * 5, iSize);
	var yNew = Sin(effectTime * 5, iSize) / 2;

	if(effectTime <= 2) {
		EffectVar(DrawResourceNewX, target, effectNumber) = xNew;
		EffectVar(DrawResourceNewY, target, effectNumber) = yNew;
	} else if(effectTime > 72 * 2) {
		xNew = EffectVar(DrawResourceNewX, target, effectNumber);
		yNew = EffectVar(DrawResourceNewY, target, effectNumber);
	}
	EffectVar(DrawResourceOldX, target, effectNumber) = xNew;
	EffectVar(DrawResourceOldY, target, effectNumber) = yNew;
	EffectVar(DrawResourceSize, target, effectNumber) = iSize;

	DrawMaterialQuad(sMat, iX, iY, iX + xOld, iY + yOld, iX + xNew, iY + yNew, iX, iY, true);

	CreateParticle("PSpark", iX + xNew / 2, iY + yNew / 2, 0, 0, iSize * 25, RGBa(GetMaterialColor(iMat, 0, 0), GetMaterialColor(iMat, 0, 1), GetMaterialColor(iMat, 0, 2), 100));

	if(effectTime > 72 * 2)
		return -1;
	else
		return 1;
}