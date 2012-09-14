#strict 2

// Local(0) and Local(1) are used for parallax movement

protected func Initialize() {
	SetClrModulation(RGBa(255, 255, 255, 255));
	Local(0) = Local(1) = Random(5);
}
