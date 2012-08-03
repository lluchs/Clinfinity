/*--- Kampfgeschütz (angeschraubt) ---*/

#strict 2

// Steam a single shot consumes this amount of Steam
static const CTW2_BasicSteamUsage = 5; //SteamUsage*ObjectMass

/* Überladbar für andere Kanonen */

protected func RotationSpeed() { return 5; }
protected func CannonMobileID() { return CTW3; }
protected func CannonAmmo(object obj) { return GetOCF(obj & OCF_Collectible ); }
protected func CannonPower(object obj) { return 12; }
protected func CannonSound(object obj) { return "Blast3"; }
protected func CannonSmoke(object obj) { return 1; }
protected func CannonShootMenuID() { return GetID(); }
protected func CannonShootMenuName(object potentialProjectile) { return Format("$TxtShoots$",GetName(potentialProjectile)); }


protected func Destruction() {
	CloseMenus();
}

private func CloseMenus() {
	var tower = GetActionTarget();

	for(var obj in FindObjects(Find_Container(tower))) {
		if(!obj->~IsClonk()) continue;
		if(obj->GetMenu() != CannonShootMenuID()) continue;
		obj->CloseMenu();
	}
}

/* Turm weg? */

protected func AttachTargetLost() {
	ComStopDouble();
}

/* Kommandos aus dem Turm */

public func ComLeft(object clonk) {
	SetAction("Rotating", GetActionTarget());
	SetRDir(-RotationSpeed() );
	Trajectory(clonk);
	return 1;
}

public func ComRight(object clonk) {
	SetAction("Rotating", GetActionTarget());
	SetRDir(RotationSpeed() );
	Trajectory(clonk);
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
	CloseMenus();

	var r = (GetR() + 270) % 360;
	SetR(r);
	ChangeDef(CannonMobileID() );
	return 1;
}

public func ComEnter() {
	ComLeave();
}

public func ComLeave() {
	var tower = GetActionTarget();
	for(var obj in FindObjects(Find_Container(tower))) {
		if(!obj->~IsClonk()) continue;
		if(obj->GetMenu() != CannonShootMenuID()) continue;

		var sel = GetMenuSelection(obj);
		obj->CloseMenu();
		DoMenu(obj, sel);
	}
}

public func ComFire(object clonk) {
	SetAction("Attaching", GetActionTarget());
	SetRDir(0);
	Trajectory(clonk, 1);
	DoMenu(clonk, 0);
}

private func DoMenu(object clonk, int selection) {
	CreateMenu(GetID(GetActionTarget()), clonk, this(), 0, "$TxtNoammo$", 0, 0, 0, CannonShootMenuID());

	var i, obj;
	while(obj = Contents(i++, GetActionTarget()) )
		if(GetID(obj) != GUNP)
			if(CannonAmmo(obj) )
				AddMenuItem(CannonShootMenuName(obj), Format("Shoot(%i, Object(%d), Object(%d))", obj->GetID(), ObjectNumber(obj), ObjectNumber(clonk)), 0, clonk, 0, 0, 0, 4, obj);
	SelectMenuItem(selection, clonk);
	Trajectory(clonk);
	return 1;
}

func OnMenuSelection(int itemIndex, object menuObject) {
	Trajectory(menuObject);
	return 1;
}

func Trajectory(object clonk, bool fClose) {
	//Log("!");
	var selection = GetMenuSelection(clonk);
	if(selection == -1) selection = 0;
	var projectile = Contents(selection, GetActionTarget());
	//Log("MenuSelection: %d; Contents: %s", GetMenuSelection(clonk),GetName(projectile));

	if(!fClose) AddTrajectory(this(), Sin(GetR(), 13) + GetX(this()), -Cos(GetR(), 13) + GetY(this()), Sin(GetR(), CannonPower(projectile)) * 10, -Cos(GetR(), CannonPower(projectile)) * 10);
	else {
		//Log("Remove");
		RemoveTrajectory(this());
	}
	return 1;
}

private func Shoot(id defFoo, object projectile, object shooter) {
	if (!projectile) return 0;
	if (projectile->Contained() != GetActionTarget()) return 0;


	//enough Steam?
	var plr = GetOwner();
	var msys = GetMatSys(plr);
	var steam = CTW2_BasicSteamUsage * GetMass(projectile);

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
		Shoot();

		//AddTrajectory(this(),Sin(GetR(), 13), -Cos(GetR(), 13),Sin(GetR(), CannonPower(projectile)), -Cos(GetR(), CannonPower(projectile)));
		Exit(projectile, Sin(GetR(), 13), -Cos(GetR(), 13), GetR(), Sin(GetR(), CannonPower(projectile)), -Cos(GetR(), CannonPower(projectile)), 20);
		if(GetOCF(projectile) & OCF_CrewMember ) ObjectSetAction(projectile, "Tumble");
		Sound(CannonSound(projectile) );
		SetPlrView(shooter->GetOwner(), projectile);
	}
}
