#strict 2

public func IsMeleeWeapon()		{ return true; }
public func GetWieldDirection()	{ return AVMW_WieldDown; }

public func HandX() { return 0; }
public func HandY() { return 1; }

public func GetStartAngle(int direction) {
	if(direction == DIR_Left) return -45;
	else return 45;
}
public func GetEndAngle(int direction) {
	if(direction == DIR_Left) return -135;
	else return 135;
}

public func WieldStart(int wieldDuration) {
/*	Log("Sword WieldStart %d", wieldDuration);
	var clonk = Contained();
	
	RemoveVertex(1);
	RemoveVertex(0);

	SetAction("Wield", clonk);
	var startAngle = 45;
	var endAngle = 45 + 90;
	var rotateSpeed = (endAngle - startAngle) / wieldDuration;
	if(clonk->GetDir() == DIR_Left) {
		startAngle = -startAngle;
		rotateSpeed = -rotateSpeed;
	}*/
	SetAction("On");
	Log("Sword rotation");
	//SetR(startAngle);
	//SetRDir(rotateSpeed);
}

public func WieldAbort() {
	SetAction("Off");
	Log("Sword WieldAbort");
	// RemoveEffect(); remove defence effect
}

public func WieldEnd() {
	/*for(var clonk in FindObjects(...)) {
		clonk->DoEnergy(-9000);
	}*/
	SetAction("Off");
	Log("Sword WieldEnd");
}

protected func Wielding() {
	//...
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

protected func RejectEntrance(object into) {
	return GetAction() == "Wield";
}
