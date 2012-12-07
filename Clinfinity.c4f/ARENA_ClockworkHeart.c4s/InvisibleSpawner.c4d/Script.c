
#strict 2

public func LightningStrike() {
	var randomId = SelectRandomId();
	var randomObject = CreateObject(randomId, 0, 0, NO_OWNER);
	if(randomId == XBOW) {
		CreateContents(ARWP, randomObject);
	}
}

private func SelectRandomId() {
	var r = Random(7);
	if(!r)   return FLNT;
	if(!--r) return SFLN;
	if(!--r) return EFLN;
	if(!--r) return BOMB;
	if(!--r) return MUSK;
	if(!--r) return SGLR;
	if(!--r) return XBOW;
}
