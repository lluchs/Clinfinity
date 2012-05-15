/*  Script: Hats
    Library: Provides functionality for hats. */

#strict 2

/*  Function: IsHat
	Yes, it is!

	Returns:
	_true_ */
public func IsHat() { return true; }

/*  Function: AttachTo
	Attaches the hat to a clonk.

	Parameters:
	clonk - the clonk */
public func AttachTo(object clonk) {
	SetAction("Attach", clonk);
	SetActionData(1);

	AddEffect("Hat", clonk, 1, 1, this);
}

/*  Function: AddHat
	Adds a hat to the calling clonk.

	Parameters:
	hatID - the hat's id
	
	Returns:
	The hat. */
global func AddHat(id hatID) {
	var hat = CreateObject(hatID, 0, 0, GetOwner());
	hat->AttachTo(this);
	return hat;
}

protected func FxHatTimer(object target, int effectNumber) {
	// Richtung an Clonk anpassen
	var rot = 5;
	var cx, cy;
	var action = target->GetAction();
	var phase = target->GetPhase();
	var dir = target->GetDir();
	if (action == "Swim") {
		rot = 0;
		cx = -5;
		cy = 2;
	}
	else if (action == "Dig" || action == "Bridge") {
		if (phase > 7)  phase = 15-phase;
		rot = 15-phase*3;
		cx = -1+phase/3;
		cy = 6-phase*6/7;
	}
	else if (action == "Dive") {
		rot = BoundBy(phase*15, 5, 40+phase*5);
		cx = Min(0+phase*2, -3);
		cy = phase*13/7;
	}
	else if (action == "Tumble") {
		rot = phase * 19;

		cx = 23 * phase**3 - 680 * phase**2 + 4833 * phase;
		cx /= 1000;

		cy = 24 * phase**3 - 340 * phase**2 - 733 * phase;
		cy /= 1000;
	}
	else if (action == "Build") {
		rot = 35;
		cy += 5;
	}
	else if (action == "FlatUp") {
		rot = (8 - phase) * 10;
		cy += 10 - phase;
	}
	else if(action == "Jump" && target->IsGliding()) {
		rot = -90;
		if(dir == DIR_Right)
			cx = -8;
		else
			cx = -5;
		cy = -8;
	}

	if(dir == DIR_Right) {
		cx = -cx;
		rot = -rot;
	}

	// update
	SetR(rot);
	MoveTo(cx, cy);

	// direction
	SetDir(dir);
}

private func MoveTo(int cx, int cy) {
	SetObjDrawTransform(1000, 0, 1000 * cx, 0, 1000, 1000 * cy);
}

