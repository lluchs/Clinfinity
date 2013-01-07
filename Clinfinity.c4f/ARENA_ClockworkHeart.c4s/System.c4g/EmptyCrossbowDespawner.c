
#strict 2

#appendto XBOW

public func Fire(bool fAuto, object pClonk) {
	var result = _inherited(...);
	if(!Contents()) {
		FadeOut();
	}
	return result;
}

public func OnFadeFinish() {
	RemoveObject();
}
