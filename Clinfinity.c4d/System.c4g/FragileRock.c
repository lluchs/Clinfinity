#strict 2

#appendto ROCK

protected func Hit() {
	_inherited(...);
	if(GetOCF() & OCF_HitSpeed4) {
		var colourIndex = Random(3);
		var red =	GetMaterialColor(Material("Rock"), colourIndex, 0);
		var green =	GetMaterialColor(Material("Rock"), colourIndex, 1);
		var blue =	GetMaterialColor(Material("Rock"), colourIndex, 2);
		CastParticles("MatSpark", RandomX(3, 6), RandomX(20, 30), 0, 0, 40, 60, RGBa(red, green, blue, 50), RGBa(red, green, blue, 50));
		Sound("Rock_burst");
		RemoveObject();
	}
}
