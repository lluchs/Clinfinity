/*	Script: Control lever
	Floating platforms are controlled with this lever. */

#strict 2

local controlMediator;
local gearStop, gearUp, gearDown;

/*	Constructor: CreateLever
	Factory method for levers.
	The coordinates are relative to the calling object in local calls, otherwise global.
	*Note:*	You should always create a lever using this method.

	Parameters:
	x			- Horizontal coordinate.
	y			- Vertical coordinate.
	forMediator	- The control mediator this lever sends its events to.

	Returns:
	The created lever. */
public func CreateLever(int x, int y, object forMediator) {
	var lever = CreateObject(COLV, x, y, forMediator->GetOwner());
	lever->LocalN("controlMediator") = forMediator;
	lever->SetAction("Control");
	return lever;
}

protected func Initialize() {
	gearStop = 0;
	gearUp = 1;
	gearDown = 2;
}

protected func MouseSelection(int player) {
	Sound("Ding");
}

private func HostileCheck(object clonk) {
	if(Hostile(GetOwner(), clonk->GetOwner())) {
		SetCommand(clonk, "UnGrab");
		return true;
	}
}

protected func Grabbed(object controller, bool grab) {
	if(grab && Hostile(GetOwner(), controller->GetOwner())) {
		Sound("CommandFailure1");
		SetCommand(controller, "UnGrab");
	}
}

protected func ControlUp(object controller) {
	if(HostileCheck(controller))
		return false;
	if(GetDir() == gearDown) {
		controlMediator->ControlEvent(COMD_Stop, this);
	} else if(GetDir() == gearStop) {
		controlMediator->ControlEvent(COMD_Up, this);
	} else {
		return false;
	}
	return true;
}

protected func ControlDownSingle(object controller) {
	if(HostileCheck(controller))
		return false;
	if(GetDir() == gearUp) {
		controlMediator->ControlEvent(COMD_Stop, this);
	} else if(GetDir() == gearStop) {
		controlMediator->ControlEvent(COMD_Down, this);
	} else {
		return false;
	}
	return true;
}

protected func ControlLeft(object controller) {
	return ControlUp(controller);
}

protected func ControlRight(object controller) {
	return ControlDownSingle(controller);
}

public func MovementEvent(int direction, object source) {
	var oldDirection = GetDir();
	if(direction == COMD_Stop) {
		SetDir(gearStop);
	} else if(direction == COMD_Up) {
		SetDir(gearUp);
	} else if(direction == COMD_Down) {
		SetDir(gearDown);
	}
	if(GetDir() != oldDirection) {
		Sound("lever", false, this, 15);
	}
}

/* -- Platform Connection Control -- */
protected func ControlDigDouble(object controller) {
	if(HostileCheck(controller))
		return false;
	CreateMenu(CXCN, controller, this, C4MN_Extra_Components);
	var left  = controlMediator->GetMaster() || PlatformMediator(FindPlatform(false));
	var right = controlMediator->GetSlave()  || PlatformMediator(FindPlatform(true));
	ConnectionMenuItem(controller, left, controlMediator);
	ConnectionMenuItem(controller, controlMediator, right);
}

// Finds platforms to the left/right
private func FindPlatform(bool right) {
	right = right * 2 - 1;
	var platform = controlMediator->GetControlledPlatform();
	return FindObject2(platform->Find_AtPoint(right * GetDefWidth(PLTF)), Find_Allied(GetOwner()), Find_Func("IsPlatform"));
}

private func PlatformMediator(object platform) {
	return platform && platform->GetControlMediator();
}

private func ConnectionMenuItem(object clonk, object master, object slave) {
	var mn, sn;
	if(master)
		mn = ObjectNumber(master);
	if(slave)
		sn = ObjectNumber(slave);
	var command;
	if(master && slave) {
		if(master->IsMasterOf(slave)) {
			command = Format("DisconnectPlatforms(Object(%d), Object(%d))", mn, sn);
			AddMaterialMenuItem("$Disconnect$", command, MS4C, clonk, 0, 0, "", 2, 4);
		} else {
			command = Format("ConnectPlatforms(Object(%d), Object(%d))", mn, sn);
			AddMaterialMenuItem("$Connect$", command, MS4C, clonk, 0, 0, "", 2, 1);
		}
	} else {
		command = Format("AddPlatform(Object(%d), Object(%d))", mn, sn);
		AddMaterialMenuItem("$NewPlatform$", command, PLTF, clonk, 0, 0, "", 2, !slave);
	}
}

protected func AddPlatform(object master, object slave) {
	// subtract components
	var components = [];
	for(var i = 0, comp, num; (comp = GetComponent(0, i, 0, PLTF)) && (num = GetComponent(comp, i, 0, PLTF)); i++) {
		if(MatSysGetTeamFill(GetOwner(), comp) < num) {
			Sound("Error");
			return;
		}
		PushBack([comp, num], components);
	}
	for(var c in components)
		MatSysDoTeamFill(-c[1], GetOwner(), c[0]);

	Sound("Connect");
	var new = controlMediator->GetControlledPlatform()->CreatePlatform(-GetDefWidth(PLTF), GetDefHeight(PLTF) / 2, GetOwner())->GetControlMediator();
	if(master)
		slave = new;
	else
		master = new;
	COMD->Connect(master, slave);
}

protected func ConnectPlatforms(object master, object slave) {
	Sound("Connect");
	COMD->Connect(master, slave);
}

protected func DisconnectPlatforms(object master, object slave) {
	var masterPlatform = master->GetControlledPlatform(), slavePlatform = slave->GetControlledPlatform();
	if(masterPlatform->HasSharedBuildingsWith(slavePlatform) || slavePlatform->HasSharedBuildingsWith(masterPlatform)) {
		Message("$DisconnectNotPossible$", this);
		Sound("Error");
		return;
	}
	Sound("Connect");
	COMD->Disconnect(master, slave);
	// move slave a bit to the right so the platforms aren't stuck
	var platform = slave->GetControlledPlatform();
	platform->SetPosition(platform->GetX() + 2, platform->GetY());
}
