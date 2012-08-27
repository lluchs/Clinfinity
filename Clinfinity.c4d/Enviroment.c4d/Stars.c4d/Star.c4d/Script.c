/*-- Star --*/

#strict 2

local brightness;

// Local(0) and Local(1) are used for parallax movement

protected func Initialize() {
	brightness = Random(10) + 1;
	SetAction("Shine");
	SetPhase(brightness - 1);

	if(Random(3)) {
		SetClrModulation(RGBa(Random(100) + 156, Random(100) + 156, Random(100) + 156, 255));
	} else {
		SetClrModulation(RGBa(255, 255, 255, 255));
	}

	SetCategory(GetCategory() | C4D_Parallax | C4D_Background);
	Local(0) = Local(1) = Random(20) + brightness * 5 - 5;
}
