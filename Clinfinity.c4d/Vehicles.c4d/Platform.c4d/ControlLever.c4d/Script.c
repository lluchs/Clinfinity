/*	Script: Control lever
	Floating platforms are controlled with this lever. */

#strict 2

local controlMediator;
local gearStop, gearUp, gearDown;
local vertical, startPos;

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

	// save position
	Schedule("startPos = GetX()", 1);
	AddEffect("HorizontalBoundsCheck", this, 1, 0, this);
}

/*  Function: GetPlatform
	Returns the platform controlled by this lever. */
public func GetPlatform() {
	return controlMediator->GetControlledPlatform();
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
	if(vertical || GetDir() == gearDown) {
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
	if(vertical || GetDir() == gearUp) {
		controlMediator->ControlEvent(COMD_Stop, this);
	} else if(GetDir() == gearStop) {
		controlMediator->ControlEvent(COMD_Down, this);
	} else {
		return false;
	}
	return true;
}

protected func ControlLeft(object controller) {
	if(HostileCheck(controller))
		return false;
	// No horizontal movement when there are connected platforms.
	if(controlMediator->GetMaster() || controlMediator->GetSlave())
		return ControlUp(controller);
	// We need to stop if the platform isn't already moving to the left.
	if(!vertical && GetDir() || GetDir() == gearDown)
		controlMediator->ControlEvent(COMD_Stop, this);
	else if(GetDir() == gearStop) {
		// Are we allowed to go further left?
		if(GetX() - startPos < -PLTF_HorizontalMovement) {
			Sound("CommandFailure1");
			return;
		}
		controlMediator->GetControlledPlatform()->SetComDir(COMD_Left);
		SetDir(gearUp);
		Sound("lever", false, this, 15);
		vertical = true;
		StartHorizontalBoundsCheck();
	}
}

protected func ControlRight(object controller) {
	if(HostileCheck(controller))
		return false;
	// No horizontal movement when there are connected platforms.
	if(controlMediator->GetMaster() || controlMediator->GetSlave())
		return ControlDownSingle(controller);
	// We need to stop if the platform isn't already moving to the right.
	if(!vertical && GetDir() || GetDir() == gearUp)
		controlMediator->ControlEvent(COMD_Stop, this);
	else if(GetDir() == gearStop) {
		// Are we allowed to go further right?
		if(GetX() - startPos > PLTF_HorizontalMovement) {
			Sound("CommandFailure1");
			return;
		}
		controlMediator->GetControlledPlatform()->SetComDir(COMD_Right);
		SetDir(gearDown);
		Sound("lever", false, this, 15);
		vertical = true;
		StartHorizontalBoundsCheck();
	}
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
	vertical = false;
	StopHorizontalBoundsCheck();
}

private func StartHorizontalBoundsCheck() {
	ChangeEffect("HorizontalBoundsCheck", this, 0, "HorizontalBoundsCheck", 10);
}

private func StopHorizontalBoundsCheck() {
	ChangeEffect("HorizontalBoundsCheck", this, 0, "HorizontalBoundsCheck", 0);
}

protected func FxHorizontalBoundsCheckTimer(object target, int effectNum, int effectTime) {
	if(Abs(GetX() - startPos) > PLTF_HorizontalMovement)
		controlMediator->ControlEvent(COMD_Stop, this);
}

/* -- Platform Connection Control -- */
protected func ControlDigDouble(object controller) {
	if(HostileCheck(controller))
		return false;
	CreateMenu(CXCN, controller, this, C4MN_Extra_Components);

	// find leftest platform
	var leftest, prev = controlMediator;
	while(leftest = prev->GetMaster())
		prev = leftest;
	AddPlatformMenuItem(controller, 0, ObjectNumber(prev));

	// dis/connect
	var left  = controlMediator->GetMaster() || PlatformMediator(FindPlatform(false));
	var right = controlMediator->GetSlave()  || PlatformMediator(FindPlatform(true));
	ConnectionMenuItem(controller, left, controlMediator);
	ConnectionMenuItem(controller, controlMediator, right);

	// find rightest platform
	var rightest, prev = controlMediator;
	while(rightest = prev->GetSlave())
		prev = rightest;
	AddPlatformMenuItem(controller, ObjectNumber(prev), 0);
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
	}
}

private func AddPlatformMenuItem(object clonk, int mn, int sn) {
	var command = Format("AddPlatform(Object(%d), Object(%d))", mn, sn);
	AddMaterialMenuItem("$NewPlatform$", command, PLTF, clonk, 0, 0, "", 2, !sn);
}

protected func AddPlatform(object master, object slave) {
	var platform = (master || slave)->GetControlledPlatform(), width = GetDefWidth(PLTF);
	var dir = !!master * 2 - 1;
	// inner x: left/right edge of the existing platform
	var ix = platform->GetX() + dir * (width / 2 + 1);
	// outer x: edge of the new platform
	var ox = platform->GetX() + dir * width * 3 / 2;
	var y = platform->GetY();
	if(ox < 0 || ox > LandscapeWidth() || !PathFree(ix, y, ox, y)) {
		Sound("Error");
		Message("$PathNotFree$", this);
		return;
	}
	if(!MatSysSubtractComponents(PLTF, GetOwner())) {
		Sound("Error");
		return;
	}

	Sound("Connect");
	var new = platform->CreatePlatform(-87, GetDefHeight(PLTF) / 2, GetOwner())->GetControlMediator();
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
