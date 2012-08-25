/*	Script: SkyColourModulation.c
	Provides individual colour modulation layers for the sky background.
	Layers can be drawn additively or subtractively.

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

static SkyColours;
static SkyColoursSubtractive;

/*	Function: SetSkyColourModulation
	Sets the colour modulation of the specified layer.

	Parameters:
	colour		- Colour modulation.
	subtractive	- If _true_, this layer will be drawn subtractively, additively otherwise.
	layerIndex	- Index of the layer to be set. */
global func SetSkyColourModulation(int colour, bool subtractive, int layerIndex) {
	if(!Inside(layerIndex, 0, SkyColourLayerCount - 1))
		return;
	CheckSkyColourArrayInitialization();
	SkyColours[layerIndex]				= colour;
	SkyColoursSubtractive[layerIndex]	= subtractive;

	var red = 255;
	var green = 255;
	var blue = 255;
	var alpha, backRed, backGreen, backBlue;
	for(var layer = 0; layer < SkyColourLayerCount; ++layer) {
		var layerColour = GetSkyColourModulation(layer);
		if(IsSkyColourModulationSubtractive(layer)) {
			red		-= GetRGBValue(layerColour, 1);
			green	-= GetRGBValue(layerColour, 2);
			blue	-= GetRGBValue(layerColour, 3);
		} else {
			red		+= GetRGBValue(layerColour, 1);
			green	+= GetRGBValue(layerColour, 2);
			blue	+= GetRGBValue(layerColour, 3);
		}
	}

	backRed		= BoundBy(red - 255, 0, 255);
	backGreen	= BoundBy(green - 255, 0, 255);
	backBlue	= BoundBy(blue - 255, 0, 255);

	red		= BoundBy(red, 0, 255);
	green	= BoundBy(green, 0, 255);
	blue	= BoundBy(blue, 0, 255);

	alpha = Max(backRed, Max(backGreen, backBlue));

	SetSkyAdjust(RGBa(red, green, blue, alpha), RGB(backRed, backGreen, backBlue));
}

/*	Function: GetSkyColourModulation
	Returns the current colour modulation of the specified layer.

	Parameters:
	layerIndex		- Index of the layer. */
global func GetSkyColourModulation(int layerIndex) {
	if(!Inside(layerIndex, 0, SkyColourLayerCount - 1))
		return;
	CheckSkyColourArrayInitialization();
	return SkyColours[layerIndex];
}

/*	Function: IsSkyColourModulationSubtractive
	Returns whether the specified layer is drawn additively or subtractively.

	Returns:
	_true_ if subtractive, _false_ if additive. */
global func IsSkyColourModulationSubtractive(int layerIndex) {
	if(!Inside(layerIndex, 0, SkyColourLayerCount - 1))
		return;
	CheckSkyColourArrayInitialization();
	return SkyColoursSubtractive[layerIndex];
}

/*	Function: ResetSkyColourModulation
	Resets all layers to default values, so the sky is completely without modulation. */
global func ResetSkyColourModulation() {
	CheckSkyColourArrayInitialization();
	for(var i = 0; i < SkyColourLayerCount; i++) {
		SkyColours[i] = RGBa(0, 0, 0, 0);
		SkyColoursSubtractive[i] = false;
	}
	SetSkyAdjust(RGBa(255, 255, 255, 0), RGB(0, 0, 0));
}

/* Internal helper */
global func CheckSkyColourArrayInitialization() {
	if(SkyColours == 0) {
		SkyColours = CreateArray(SkyColourLayerCount);
		SkyColoursSubtractive = CreateArray(SkyColourLayerCount);
	}
}
