/*	Script: DrawResource.c
	Draws a resource vein.
	The algorithm draws a vein in consecutive triangles, clockwise, around a central point. */

#strict 2

/*	Function: DrawResource
	Draws a resource vein around a central point.
	
	Parameters:
	materialIndex	- Material the vein will be made of.
	materialTexture	- The material's texture.
	x				- Horizontal coordinate of the centre.
	y				- Vertical coordinate of the centre. */
global func DrawResource(int materialIndex, string materialTexture, int x, int y) {
	AddEffect("DrawResource", 0, 1, 1, 0, 0, materialIndex, materialTexture, x, y);
}

static const DrawResourceCentreX = 0;
static const DrawResourceCentreY = 1;
static const DrawResourceMaterialIndex = 2;
static const DrawResourceMaterialTexture = 3;
static const DrawResourcePreviousEdgeX = 4;
static const DrawResourcePreviousEdgeY = 5;
static const DrawResourceSize = 6;
static const DrawResourceStartEdgeX = 7;
static const DrawResourceStartEdgeY = 8;

static const DrawResourceMinSize = 10;
static const DrawMaterialMaxSize = 65;
static const DrawResourceInitialMinSize = 20;
static const DrawResourceInitialMaxSize = 25;
static const DrawResourceSizeChange = 6;
static const DrawResourceTunnelBackground = true;
static const DrawResourceAngularRate = 5;
static const DrawResourceDrawingCycles = 2;

global func FxDrawResourceStart(object target, int effectNumber, int temporary, materialIndex, materialTexture, x, y) {
	if(temporary == 0) {
		EffectVar(DrawResourceCentreX, target, effectNumber) = x;
		EffectVar(DrawResourceCentreY, target, effectNumber) = y;
		EffectVar(DrawResourceMaterialIndex, target, effectNumber) = materialIndex;
		EffectVar(DrawResourceMaterialTexture, target, effectNumber) = materialTexture;

		var size = RandomX(DrawResourceInitialMinSize, DrawResourceInitialMaxSize);
		EffectVar(DrawResourceSize, target, effectNumber) = size;
		EffectVar(DrawResourceStartEdgeX, target, effectNumber) = size;
		EffectVar(DrawResourceStartEdgeY, target, effectNumber) = 0;
		EffectVar(DrawResourcePreviousEdgeX, target, effectNumber) = size;
		EffectVar(DrawResourcePreviousEdgeY, target, effectNumber) = 0;
	}
}

global func FxDrawResourceTimer(object target, int effectNumber, int effectTime) {
	var centreX = EffectVar(DrawResourceCentreX, target, effectNumber);
	var centreY = EffectVar(DrawResourceCentreY, target, effectNumber);
	var materialIndex = EffectVar(DrawResourceMaterialIndex, target, effectNumber);
	var materialTexture = EffectVar(DrawResourceMaterialTexture, target, effectNumber);
	var material = Format("%s-%s", MaterialName(materialIndex), materialTexture);
	var previousX = EffectVar(DrawResourcePreviousEdgeX, target, effectNumber);
	var previousY = EffectVar(DrawResourcePreviousEdgeY, target, effectNumber);
	var result = FX_OK;

	var size, currentX, currentY;
	if(effectTime < DrawResourceDrawingCycles * 360 / DrawResourceAngularRate) {
		size = EffectVar(DrawResourceSize, target, effectNumber);
		size = BoundBy(size + RandomX(-DrawResourceSizeChange, DrawResourceSizeChange), DrawResourceMinSize, DrawMaterialMaxSize);

		currentX = Cos(effectTime * DrawResourceAngularRate, size);
		currentY = Sin(effectTime * DrawResourceAngularRate, size) / 2;
	} else {
		// Make start and end fit together
		size = EffectVar(DrawResourceStartEdgeX, target, effectNumber);
		currentX = EffectVar(DrawResourceStartEdgeX, target, effectNumber);
		currentY = EffectVar(DrawResourceStartEdgeY, target, effectNumber);
		result = FX_Execute_Kill;
	}

	EffectVar(DrawResourcePreviousEdgeX, target, effectNumber) = currentX;
	EffectVar(DrawResourcePreviousEdgeY, target, effectNumber) = currentY;
	EffectVar(DrawResourceSize, target, effectNumber) = size;

	DrawMaterialQuad(material, centreX, centreY, centreX + previousX, centreY + previousY, centreX + currentX, centreY + currentY, centreX, centreY, DrawResourceTunnelBackground);

	var particleMinSize = 300;
	var particleMaxSize = 2000;
	var particleMinSpeed = 30;
	var particleMaxSpeed = 50;
	
	var particleSize = RandomX(particleMinSize, particleMaxSize);
	var speedFactor = (particleMaxSize - particleSize) / (particleMaxSize - particleMinSize);
	var particleSpeed = particleMinSpeed + (particleMaxSize - particleSize) * (particleMaxSpeed - particleMinSpeed) / (particleMaxSize - particleMinSize);
	var particleMoveAngle = Random(360);
	var particleSpeedX = Cos(particleMoveAngle, particleSpeed);
	var particleSpeedY = Sin(particleMoveAngle, particleSpeed);

	//CreateParticle("PSpark", centreX + currentX / 2, centreY + currentY / 2, 0, 0, size * 25, RGBa(GetMaterialColor(materialIndex, 0, 0), GetMaterialColor(materialIndex, 0, 1), GetMaterialColor(materialIndex, 0, 2), 100));
	CreateParticle("PSpark", centreX, centreY, particleSpeedX, particleSpeedY, particleSize, RGBa(GetMaterialColor(materialIndex, 0, 0), GetMaterialColor(materialIndex, 0, 1), GetMaterialColor(materialIndex, 0, 2), 100));

	return result;
}