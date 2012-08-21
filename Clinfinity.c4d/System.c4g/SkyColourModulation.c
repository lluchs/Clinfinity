/*	Script: SkyColourModulation.c
	Provides individual colour modulation layers for the sky background.
	The modulations are combined additively. */

#strict 2

static const SkyColourLayerCount = 10;

static SkyColourForegrounds;
static SkyColourBackgrounds;

global func SetSkyColourModulation(int foregroundColour, int backgroundColour, int layerIndex) {
	if(!Inside(layerIndex, 0, SkyColourLayerCount - 1))
		return;
	CheckSkyColourArrayInitialization();
	SkyColourForegrounds[layerIndex] = foregroundColour;
	SkyColourBackgrounds[layerIndex] = backgroundColour;

	var red, green, blue, alpha, backRed, backGreen, backBlue;
	for(var i = 0; i < SkyColourLayerCount; i++) {
		red		+= GetRGBValue(SkyColourForegrounds[i], 1);
		green	+= GetRGBValue(SkyColourForegrounds[i], 2);
		blue	+= GetRGBValue(SkyColourForegrounds[i], 3);
		alpha	+= GetRGBValue(SkyColourForegrounds[i], 0);

		backRed		+= GetRGBValue(SkyColourBackgrounds[i], 1);
		backGreen	+= GetRGBValue(SkyColourBackgrounds[i], 2);
		backBlue	+= GetRGBValue(SkyColourBackgrounds[i], 3);
	}
	red		= Min(255, red);
	green	= Min(255, green);
	blue	= Min(255, blue);
	alpha	= Min(255, alpha);

	backRed		= Min(255, backRed);
	backGreen	= Min(255, backGreen);
	backBlue	= Min(255, backBlue);

	SetSkyAdjust(RGBa(red, green, blue, alpha), RGB(backRed, backGreen, backBlue));
}

global func GetSkyColourModulation(bool getBackground, int layerIndex) {
	if(!Inside(layerIndex, 0, SkyColourLayerCount - 1))
		return;
	CheckSkyColourArrayInitialization();
	if(getBackground) {
		return SkyColourBackgrounds[layerIndex];
	} else {
		return SkyColourForegrounds[layerIndex];
	}
}

global func ResetSkyColourModulation() {
	CheckSkyColourArrayInitialization();
	for(var i = 1; i < SkyColourLayerCount; i++) {
		SkyColourForegrounds[i] = RGBa(0, 0, 0, 0);
		SkyColourBackgrounds[i] = RGB(0, 0, 0);
	}
	SetSkyColourModulation(RGBa(255, 255, 255, 0), RGB(0, 0, 0), 0);
}

global func CheckSkyColourArrayInitialization() {
	if(SkyColourForegrounds == 0) {
		SkyColourForegrounds = CreateArray(SkyColourLayerCount);
		SkyColourForegrounds[0] = RGBa(255, 255, 255, 0);
		SkyColourBackgrounds = CreateArray(SkyColourLayerCount);
	}
}
