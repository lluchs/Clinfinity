#strict 2

public func IsMeleeWeapon()		{ return true; }
public func GetWieldDirection()	{ return AVMW_WieldDown; }

public func WieldStart() {
	Message("Sword WieldStart");
	// Add defence effect
}

public func WieldAbort() {
	Message("Sword WieldAbort");
	// RemoveEffect(); remove defence effect
}

public func WieldEnd() {
	/*for(var clonk in FindObjects(...)) {
		clonk->DoEnergy(-9000);
	}*/
	Message("Sword WieldEnd");
}

/*
public func FxWhatever() {
	for(var incoming in FindObjects(...)) {
		if(correctAngleTo(incoming)) {
			FlingAway(incoming);
		}
	}
}
*/
