#strict 2

protected func Initialize() {
	var result = _inherited();
	var platform = FindObject2(Find_ID(PLTF), Find_AtPoint(0, GetObjHeight() / 2 + 1));
	if(platform != 0) {
		AttachTo(platform);
		platform->AttachEvent(this, platform, false, this);
	}
	return result;
}
