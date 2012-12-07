
#strict 2

public func LightningStrike() {
	if(GetEffect("DelaySpawning", this)) return;

	var randomId = SelectRandomId();
	var randomObject = CreateObject(randomId, 0, 0, NO_OWNER);
	if(randomId == XBOW) {
		CreateContents(ARWP, randomObject);
	}
	// Don't spawn more than one object when multiple lighting strikes occur almost at the same time
	AddEffect("DelaySpawning", this, 101, 5);
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
