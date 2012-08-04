/*--- Kampfgeschütz (angeschraubt) ---*/

#strict 2

/* Überladbar für andere Kanonen */

protected func RotationSpeed() { return 5; }
protected func CannonMobileID() { return CTW3; }
protected func CannonAmmo(object obj) { return obj && obj->GetOCF() & OCF_Collectible; }
protected func CannonPowerArray(object obj) { return [6, 8, 12, 14, 18]; }
protected func CannonPower(object obj) { return CannonPowerArray(obj)[power]; }
protected func CannonSteamUsage(object obj) { return [2, 3, 5, 7, 10][power]; } // SteamUsage*ObjectMass
protected func CannonSound(object obj) { return "Blast3"; }
protected func CannonSmoke(object obj) { return 1; }

// Legacy stuff
protected func CannonShootMenuID() {}

local power;

protected func Initialize() {
	power = GetLength(CannonPowerArray());
}

/* Turm weg? */
protected func AttachTargetLost() {
	RemoveObject();
}

/* Kommandos aus dem Turm */

public func ComLeft(object clonk) {
	SetAction("Rotating", GetActionTarget());
	if(GetRDir() == -RotationSpeed())
		ComStop(clonk);
	else {
		SetRDir(-RotationSpeed());
		Trajectory(clonk);
	}
	return 1;
}

public func ComRight(object clonk) {
	SetAction("Rotating", GetActionTarget());
	if(GetRDir() == RotationSpeed())
		ComStop(clonk);
	else {
		SetRDir(RotationSpeed());
		Trajectory(clonk);
	}
	return 1;
}

public func ComStop(object clonk) {
	SetAction("Attaching", GetActionTarget());
	SetRDir(0);
	Trajectory(clonk);
	return 1;
}

//Wird vom Grabber nicht uebertragen, nur vom Container, Achtung, Aufruf stattdessen im Cannontower
public func ComStopDouble(object clonk) {
	//Log("StopDouble");
	Trajectory(clonk, 1);

	var r = (GetR() + 270) % 360;
	SetR(r);
	ChangeDef(CannonMobileID() );
	return 1;
}

public func ComPowerUp(object clonk) {
	power++;
	if(power < GetLength(CannonPowerArray()))
		Sound("Click");
	else {
		power--;
		Sound("CommandFailure1");
	}
	Trajectory(clonk);
}

public func ComPowerDown(object clonk) {
	power--;
	if(power >= 0)
		Sound("Click");
	else {
		power++;
		Sound("CommandFailure1");
	}
	Trajectory(clonk);
}

public func ComFire(object clonk) {
	SetAction("Attaching", GetActionTarget());
	SetRDir(0);
	Trajectory(clonk, 1);
	var ammo = clonk->Contents();
	if(CannonAmmo(ammo))
		Shoot(ammo, clonk);
	else
		clonk->Sound("CommandFailure1");
	Trajectory(clonk);
	return 1;
}

func Trajectory(object clonk, bool fClose) {
	var projectile = clonk->Contents();
	//Log("MenuSelection: %d; Contents: %s", GetMenuSelection(clonk),GetName(projectile));

	if(!fClose) AddTrajectory(this, Sin(GetR(), 13) + GetX(this), -Cos(GetR(), 13) + GetY(this), Sin(GetR(), CannonPower(projectile)) * 10, -Cos(GetR(), CannonPower(projectile)) * 10);
	else {
		//Log("Remove");
		RemoveTrajectory(this);
	}
	return 1;
}

private func Shoot(object projectile, object shooter) {
	if (!projectile) return 0;

	//enough Steam?
	var plr = GetOwner();
	var msys = GetMatSys(plr);
	var steam = CannonSteamUsage(projectile) * GetMass(projectile);

	if(MatSysGetTeamFill(plr, STEM) < steam) {
		Sound("Error");
		Message("$TxtNotenoughgunpowder1r$", GetActionTarget(), steam);
	} else {
		MatSysDoTeamFill(-steam, plr, STEM);

		if(CannonSmoke(projectile) ) {
			Smoke(Sin(GetR(), 13), -Cos(GetR(), 13), 20);
			Smoke(Sin(GetR(), 18), -Cos(GetR(), 23), 17);
			Smoke(Sin(GetR(), 22), -Cos(GetR(), 32), 14);
			Smoke(Sin(GetR(), 25), -Cos(GetR(), 40), 11);
		}

		// Controller setzen (Killverfolgung)
		projectile->SetController(shooter->GetOwner());

		//AddTrajectory(this(),Sin(GetR(), 13), -Cos(GetR(), 13),Sin(GetR(), CannonPower(projectile)), -Cos(GetR(), CannonPower(projectile)));
		Exit(projectile, Sin(GetR(), 13), -Cos(GetR(), 13), GetR(), Sin(GetR(), CannonPower(projectile)), -Cos(GetR(), CannonPower(projectile)), 20);
		if(GetOCF(projectile) & OCF_CrewMember ) ObjectSetAction(projectile, "Tumble");
		Sound(CannonSound(projectile) );
		SetPlrView(shooter->GetOwner(), projectile);
	}
}
