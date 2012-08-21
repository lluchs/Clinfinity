/*	Script: SkyColourModulation.c
	Provides individual colour modulation layers for the sky background.
	The modulations are combined additively.
	If the transparency of the foreground colour of any layer is not 0, the combined background colours will be used as background colour behind the sky.

	With this mechanism, you can apply several different modulations without worrying about their combination yourself.
	For example, a magic spell can simply set the colours of the appropriate layer, without having to consider other effects on the sky, such as night and day, lightning, etc.

	For this, ten layers are available.
	Generally, lower layer indices mean longer colour changess, while higher indices mean shorter effects.
	Their exact purpose is summarised by the following table.
	You should generally abide by these conventions, it is not mandatory, though.
	(start table)
	Layer	| Purpose
	--------+----------------------
	0		| Scenario global value
	1		| Climate
	2		| Season
	3		| Free
	4		| Day/night cycle
	5		| Twilight effects
	6		| Free
	7		| Lightning
	8		| Magic effects
	9		| Free
	(end) */

#strict 2

/* Internal variables */
static const SkyColourLayerCount = 10;

static SkyColourForegrounds;
static SkyColourBackgrounds;

/*	Function: SetSkyColourModulation
	Sets the colour modulation of the specified layer.

	Parameters:
	foregroundColour	- Primary colour modulation.
	backgroundColour	- Background colour.
	layerIndex			- Index of the layer to be set. */
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

/*	Function: GetSkyColourModulation
	Returns the current colour modulation of the specified layer.

	Parameters:
	getBackground	- If _true_, the function returns the background colour, otherwise the foreground colour.
	layerIndex		- Index of the layer. */
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

/*	Function: ResetSkyColourModulation
	Resets all layers to default values, so the sky is completely without modulation.
	*/
global func ResetSkyColourModulation() {
	CheckSkyColourArrayInitialization();
	for(var i = 1; i < SkyColourLayerCount; i++) {
		SkyColourForegrounds[i] = RGBa(0, 0, 0, 0);
		SkyColourBackgrounds[i] = RGB(0, 0, 0);
	}
	SetSkyColourModulation(RGBa(255, 255, 255, 0), RGB(0, 0, 0), 0);
}

/* Internal helper */
global func CheckSkyColourArrayInitialization() {
	if(SkyColourForegrounds == 0) {
		SkyColourForegrounds = CreateArray(SkyColourLayerCount);
		SkyColourForegrounds[0] = RGBa(255, 255, 255, 0);
		SkyColourBackgrounds = CreateArray(SkyColourLayerCount);
	}
}
