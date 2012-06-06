/*	Script: Platform
	A controllable, floating platform, powered by steam. */

#strict 2

// Interval in frames which times steam transactions
static const PLTF_SteamPayTimer = 250;
// Timer interval as defined in the DefCore
static const PLTF_SteamTimer = 10;
// Steam a single platform uses on its own every PLTF_SteamPayTimer frames
static const PLTF_SteamUsage = 10;
// Helper 'direction' for ControlMediator
static const PLTF_Explode = -1;

local controlMediator;

/*	Constructor: CreatePlatform
	Factory method for platforms.
	The coordinates are relative to the calling object in local calls, otherwise global.
	*Note:* You should always create a platform using this method.

	Parameters:
	x		- Horizontal coordinate.
	y		- Vertical coordinate.
	owner	- Owner of the created platform: Player index. Use NO_OWNER for ownerless platforms.

	Returns:
	The created platform. */
public func CreatePlatform(int x, int y, int owner) {
	var platform = CreateObject(PLTF, x, y, owner);
	CreateAdditionalObjectsFor(platform);
	return platform;
}

private func CreateAdditionalObjectsFor(object platform) {
	var mediator = CreateObject(COMD, AbsX(3), AbsY(3), platform->GetOwner());
	mediator->LocalN("controlledPlatform") = platform;
	platform->LocalN("controlMediator") = mediator;
	platform->SetAction("Fly");

	var lever = COLV->CreateLever(platform->GetX() - 35, platform->GetY() - 3, mediator);
	lever->AttachTo(platform);

	var prop = PROP->CreateProp(platform->GetX(), platform->GetY() + 12, platform);

	mediator->AddControlEventListener(platform);
	mediator->AddMovementEventListener(lever);
	mediator->AddMovementEventListener(prop);
}

protected func Construction() {
	ScheduleCall(this, "CheckAfterConstruction", 1);
}

protected func Destruction() {
	// remove everything that's connected
	controlMediator->RemoveObject();
	for(var obj in FindObjects(Find_ActionTarget(this), Find_Not(Find_ID(PLTF)), Find_Procedure("ATTACH"))) {
		obj->Schedule("RemoveObject()", 1);
	}
}

protected func CheckAfterConstruction() {
	if(controlMediator == 0) {
		Log("Platform: You created a platform without using the factory method. If changing the owner, don't forget to set the correct one of lever and prop, too!");
		CreateAdditionalObjectsFor(this);
	}
}

/*	Function: IsPlatform
	Marks objects to have platform-like capabilities. */
public func IsPlatform() { return true; }

protected func Flying() {
  SetSolidMask(0, 4, 90, 4);
}

protected func ContactLeft() {
	FloatStop();
}

protected func ContactRight() {
	FloatStop();
}

protected func ContactTop() {
	FloatStop();
}

protected func ContactBottom() {
	FloatStop();
}

/*	Function: AttachEvent
	Event handler, called after an object was attached to a new parent object or detached from it.
	A platform hands the event to its control mediator.

	Parameters:
	attached	- The attached object.
	attachedTo	- The new parent object.
	isDetaching	- *true* if the object was detached. *false* if it was attached.
	source		- Source of the event. */
public func AttachEvent(object attached, object attachedTo, bool isDetaching, object source) {
	return GetControlMediator()->AttachEvent(attached, attachedTo, isDetaching, source);
}

/*	Section: Control */

private func GetControlMediator() { return controlMediator; }

/*	Function: ControlEvent
	Event handler for control events.

	Parameters:
	direction	- Direction of control, specified by one of the COMD_* constants.
	source		- Source of the event.

	See Also:
	<Control mediator> */
public func ControlEvent(int direction, object source) {
	if(direction == COMD_Stop) {
		FloatStop();
	} else if(direction == COMD_Up) {
		FloatUp();
	} else if(direction == COMD_Down) {
		FloatDown();
	} else if(direction == PLTF_Explode) {
		Explode(GetDefWidth() / 2);
	}
}

private func FloatStop() {
	if(missingSteam) {
		controlMediator->ControlEvent(PLTF_Explode, this);
		return;
	}
	SetComDir(COMD_None);
	SetYDir(0);
	controlMediator->MovementEvent(COMD_Stop, this);
}

private func FloatUp() {
	SetComDir(COMD_Up);
	controlMediator->MovementEvent(COMD_Up, this);
}

private func FloatDown() {
	SetComDir(COMD_Down);
	controlMediator->MovementEvent(COMD_Down, this);
}

/*	Section: Master/Slave system */

/*	Function: Connect
	Connects two platforms so they move in unison.
	When connected, both platforms can be controlled from either control lever.
	By calling this method, the left platform becomes the master of the right platform.

	Parameters:
	leftPlatform	- The left platform.
	rightPlatform	- The right platform.

	Returns:
	*true* if the platforms could successfully be connected, *false* otherwise. */
public func Connect(object leftPlatform, object rightPlatform) {
	if(IsPlatformOkay(leftPlatform) && IsPlatformOkay(rightPlatform) && leftPlatform != rightPlatform) {
		var leftMediator = leftPlatform->GetControlMediator();
		var rightMediator = rightPlatform->GetControlMediator();
		return COMD->Connect(leftMediator, rightMediator);
	}
	return false;
}

public func Disconnect(object leftPlatform, object rightPlatform) {
	if(IsPlatformOkay(leftPlatform) && IsPlatformOkay(rightPlatform) && leftPlatform != rightPlatform) {
		var leftMediator = leftPlatform->GetControlMediator();
		var rightMediator = rightPlatform->GetControlMediator();
		return COMD->Disconnect(leftMediator, rightMediator);
	}
	return false;
}

private func IsPlatformOkay(object platform) {
	return platform != 0 && platform->~IsPlatform();
}

public func CopyChildrenVertices(object child) {
	return inherited(child);
}

/*  Function: HasSharedBuildingsWith
	Checks whether there are any buildings standing on this platform that are
	shared with another platform.

	This function will only check for buildings that aren't attached to this
	platform but seem to be visually standing on this platform.

	Note: Control levers (COLV) are excluded from search.

	Parameters:
	otherPlatform - The other platform.

	Returns:
	true when there is a shared building. */
public func HasSharedBuildingsWith(object otherPlatform) {
	return !!FindObject2(Find_Not(Find_ID(COLV)), Find_ActionTarget(otherPlatform), Find_Procedure("ATTACH"), Find_OnPlatform());
}

/* -- Steam Usage -- */
local missingSteam, steamUsage;

protected func CheckSteam() {
	if(missingSteam) {
			missingSteam += MatSysDoTeamFill(-missingSteam, GetOwner(), STEM);
			if(missingSteam)
				ScheduleCall(this, "CheckSteam", 1);
			else
				StopFall();
			return; // nothing else to do
	}
	// only if this is the master and it's pay time
	else if(GetActTime() % PLTF_SteamPayTimer < PLTF_SteamTimer && !GetControlMediator()->HasMaster()) {
		var usage = GetControlMediator()->AccumulateSteamUsage();
		usage /= PLTF_SteamPayTimer / PLTF_SteamTimer;
		Message("Need %d{{STEM}}", this, usage);
		missingSteam = usage + MatSysDoTeamFill(-usage, GetOwner(), STEM);
		if(missingSteam) {
			FreeFall();
			ScheduleCall(this, "CheckSteam", 1);
		}
	}

	// every platform: calculate steam usage
	if(GetControlMediator()->HasMaster())
		// connected platforms use less steam
		steamUsage += PLTF_SteamUsage / 2;
	else
		steamUsage += PLTF_SteamUsage;
	steamUsage += CalculateWeight() / 50;
}

/*  Function: ResetSteamUsage
	Resets the steam usage variable for the current period.

	Returns:
	The counter before the reset. */
public func ResetSteamUsage() {
	var temp = steamUsage;
	steamUsage = 0;
	return temp;
}

private func FreeFall() {
	SetAction("Idle");
	SetYDir(1);
}

private func StopFall() {
	SetAction("Fly");
	FloatStop();
}

/*  Function: Find_OnPlatform
	FindObject2/FindObjects search criteria: Find all objects standing on this platform

	Note: This function might also find objects that aren't actually standing
	on the platform, but are on a platform further down and overlapping with
	this platform, for example. */
public func Find_OnPlatform() {
	return Find_And(Find_OnLine(-GetDefWidth()/2, -GetDefHeight()/2-2, GetDefWidth()/2, -GetDefHeight()/2-2),
	                Find_Not(Find_Or(Find_Func("IsPlatform"), Find_Category(C4D_StaticBack))));
	                          
}

/*  Function: CalculateWeight
	Adds the mass of everything on top of the platform.

	Returns:
	The corresponding sum of masses. */
public func CalculateWeight() {
	var weights = FindObjects(Find_OnPlatform(),
							  Find_Not(Find_And(Find_Not(Find_ActionTarget(this)), Find_Procedure("ATTACH"))));
	var mass = 0;
	for(var weight in weights) {
		mass += GetMass(weight);
	}
	return mass;
}
