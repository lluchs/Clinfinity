#strict 2

protected func Hit() {
	Sound("MetalHit*");
}

public func IsMeleeWeapon()		{ return true; }
public func GetWieldDirection()	{ return AVMW_WieldUp; }

public func WieldStart() {
	Message("SHIT WieldStart");
}

public func WieldAbort() {
	Message("SHIT WieldAbort");
}

public func WieldEnd() {
	Message("SHIT WieldEnd");
}
