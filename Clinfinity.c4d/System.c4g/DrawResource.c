#strict 2

global func DrawResource(int x, int y, int materialIndex, string materialTexture) {
	AddEffect("DrawResource", 0, 1, 1, 0, 0, x, y, materialIndex, materialTexture);
}

static const DrawResourceCentreX = 0;
static const DrawResourceCentreY = 1;
static const DrawResourceMaterialIndex = 2;
static const DrawResourceMaterialTexture = 3;
static const DrawResourceOldEdgeX = 4;
static const DrawResourceOldEdgeY = 5;
static const DrawResourceSize = 6;
static const DrawResourceNewEdgeX = 7;
static const DrawResourceNewEdgeY = 8;

static const DrawResourceMinSize = 10;
static const DrawMaterialMaxSize = 65;
static const DrawResourceInitialMinSize = 20;
static const DrawResourceInitialMaxSize = 25;
static const DrawResourceSizeChange = 6;
static const DrawResourceTunnelBackground = true;
static const DrawResourceMaxEffectTime = 145;

global func FxDrawResourceStart(object target, int effectNumber, int temporary, x, y, materialIndex, materialTexture) {
	if(temporary == 0) {
		EffectVar(DrawResourceCentreX, target, effectNumber) = x;
		EffectVar(DrawResourceCentreY, target, effectNumber) = y;
		EffectVar(DrawResourceMaterialIndex, target, effectNumber) = materialIndex;
		EffectVar(DrawResourceMaterialTexture, target, effectNumber) = materialTexture;
	}
}

global func FxDrawResourceTimer(object target, int effectNumber, int effectTime) {
	var centreX = EffectVar(DrawResourceCentreX, target, effectNumber);
	var centreY = EffectVar(DrawResourceCentreY, target, effectNumber);
	var materialIndex = EffectVar(DrawResourceMaterialIndex, target, effectNumber);
	var materialTexture = EffectVar(DrawResourceMaterialTexture, target, effectNumber);
	var material = Format("%s-%s", MaterialName(materialIndex), materialTexture);
	var xOld = EffectVar(DrawResourceOldEdgeX, target, effectNumber);
	var yOld = EffectVar(DrawResourceOldEdgeY, target, effectNumber);

	var size = EffectVar(DrawResourceSize, target, effectNumber);
	if(!size)
		size = RandomX(DrawResourceInitialMinSize, DrawResourceInitialMaxSize);
	else
		size = BoundBy(size + RandomX(-DrawResourceSizeChange, DrawResourceSizeChange), DrawResourceMinSize, DrawMaterialMaxSize);

	var xNew = Cos(effectTime * 5, size);
	var yNew = Sin(effectTime * 5, size) / 2;

	if(effectTime <= 2) {
		EffectVar(DrawResourceNewEdgeX, target, effectNumber) = xNew;
		EffectVar(DrawResourceNewEdgeY, target, effectNumber) = yNew;
	} else if(effectTime >= DrawResourceMaxEffectTime) {
		xNew = EffectVar(DrawResourceNewEdgeX, target, effectNumber);
		yNew = EffectVar(DrawResourceNewEdgeY, target, effectNumber);
	}
	EffectVar(DrawResourceOldEdgeX, target, effectNumber) = xNew;
	EffectVar(DrawResourceOldEdgeY, target, effectNumber) = yNew;
	EffectVar(DrawResourceSize, target, effectNumber) = size;

	DrawMaterialQuad(material, centreX, centreY, centreX + xOld, centreY + yOld, centreX + xNew, centreY + yNew, centreX, centreY, DrawResourceTunnelBackground);

	CreateParticle("PSpark", centreX + xNew / 2, centreY + yNew / 2, 0, 0, size * 25, RGBa(GetMaterialColor(materialIndex, 0, 0), GetMaterialColor(materialIndex, 0, 1), GetMaterialColor(materialIndex, 0, 2), 100));

	if(effectTime >= DrawResourceMaxEffectTime)
		return -1;
	else
		return 1;
}