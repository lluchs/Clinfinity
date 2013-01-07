/*	Script: ThrowProjectile.c
	Provides a helper function for throwable objects. */
#strict 2

/*	Function: ThrowProjectile
	Lets the calling Clonk throw the given object further than usual.

	Should be called from the object's _Departure_ function.
	Automatically figures out whether the Clonk is capable of throwing the
	object with his current action.

	Adapted from the snowball.

	Parameters:
	obj - The object to throw. */
global func ThrowProjectile(object obj) {
	var proc = GetProcedure();
	var comd = GetComDir();
	// Ablegen -> Abbruch
	if (GetPlrDownDouble(GetOwner()))
		// Nur Ablegen in der Luft mit Befehlsrichtung? Luftwurf erlauben!
		if (proc != "FLIGHT" || GetComDir() == COMD_None)
			return;

	// Ablegen im stehenden Hangeln -> Abbruch
	if (proc == "HANGLE" && comd == COMD_None)
		return;

	// Ablegen im Schwimmen / Klettern -> Abbruch
	if (proc == "SWIM" || proc == "SCALE")
		return;

	// Wurfrichtung rausfinden
	var dir;
	// Nach Befehlsrichtung, wenn sinnvolle Werte vorhanden
	if (comd == COMD_Left || comd == COMD_Right) {
		if (comd == COMD_Left)  dir = -1;
		if (comd == COMD_Right) dir = +1;
	}
	// sonst nach Blickrichtung
	else {
		if (GetDir() == DIR_Left) dir = -1;
		else dir = +1;
	}

	// Wurfgeschwindigkeiten berechnen
	var x_dir = dir * GetPhysical("Throw")/1000 + GetXDir() / 3;
	var y_dir = -30;

	if (!x_dir) return;

	// When throwing in direction of travel: Prevent hitting yourself
	if(GetXDir() * x_dir > 0) {
		obj->SetPosition(GetX() + x_dir / 5, obj->GetY());
	}

	// Geschwindigkeit setzen
	obj->SetXDir(x_dir);
	obj->SetYDir(y_dir);
}
