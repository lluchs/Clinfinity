
#strict 2

#appendto XBOW

public func Fire(bool fAuto, object pClonk) {
	_inherited(...);
	if(!Contents()) {
		FadeOut();
	}
}

public func OnFadeFinish() {
	RemoveObject();
}
