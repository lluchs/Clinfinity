/*  Script: Control Point
    Library: Provides functionality for TF2-style control points.

	 - Initially, the point doesn't have an owner.
	 - The first clonk entering its capture zone (as defined by _CaptureZone()_) will start a capture for his team.
	 - Enemy clonks entering the zone will block the capture progress.
	 - If the enemy clonks defeat the initial capturers, they will revert the capture, then starting to capture for
	   their team.
	 - A successful capture will change the owner to a player of the capturing team.
	 - Reverting capture time isn't possible for the owning team.
	 - Capture time will decrease slowly while there is no capturing clonk on point. */

#strict 2

/*  Function: SetupTime
    Should be overwritten.

	Returns:
	The time in frames a before the control point is available for capture. */
public func SetupTime() { return 1; }

/*  Function: CaptureTime
    Should be overwritten.

	Returns:
	The time in frames a single clonk needs to capture the point. */
public func CaptureTime() { return 200; }

/*  Function: CaptureZone
	Should be overwritten.

	Returns:
	FindObject2-criteria which define the capture zone. */
public func CaptureZone() { return Find_Distance(300); }

// the timer interval
static const CP_Interval = 5;

// a player of the capturing team
local capturingPlayer;
// the current capture time
local captureTime;
// the wait time before the capture time starts decreasing
local wait;
// are we in overtime?
local overtime;

protected func Completion() {
	ScheduleCall(this, "EnablePoint", SetupTime());
	capturingPlayer = NO_OWNER;
	captureTime = 0;
	overtime = false;
	return _inherited(...);
}

/*  Function: EnablePoint
	Enables the control point, making it available for capture.

	This function will automatically be called after the point is created and <SetupTime>() frames have passed. */
public func EnablePoint() {
	if(!GetEffect("ControlPoint", this)) {
		AddEffect("ControlPoint", this, 1, CP_Interval, this);
		return true;
	}
}

/*  Function: DisablePoint
	Disables the control point, freezing all current capture operations.

	A disabled point can be re-enabled using <EnablePoint>. */
public func DisablePoint() {
	RemoveEffect("ControlPoint", this);
	return true;
}

protected func FxControlPointTimer(object target, int effectNum, int effectTime) {
	var clonks = FindObjects(Find_OCF(OCF_CrewMember), CaptureZone());
	var owner = GetOwner();
	var capturingClonks = 0;
	var defendingClonks = 0;
	for(var clonk in clonks) {
		var plr = clonk->GetOwner();
		if(capturingPlayer == NO_OWNER) {
			if(owner == NO_OWNER || Hostile(plr, owner)) {
				capturingPlayer = plr;
				capturingClonks++;
			}
		} else if(Hostile(capturingPlayer, plr)) {
			defendingClonks++;
		} else {
			capturingClonks++;
		}
	}
	UpdateCaptureTime(capturingClonks, defendingClonks);
	CheckCapture();
}

/*  Function: UpdateCaptureTime
	Updates the capture time according to the number of capturing clonks and the number of defending clonks.

	Parameters:
	capturing - the number of capturing clonks
	defending - the number of defending clonks */
public func UpdateCaptureTime(int capturing, int defending) {
	var prev = captureTime;
	if(capturing) {
		if(defending) {
			// don't do anything
			return;
		}
		// callback when capture is just starting
		if(captureTime == 0)
			CaptureStarting();
		// increase capture time
		captureTime += CP_Interval * capturing;
		wait = 20;
	} else if(defending && GetOwner() == NO_OWNER) {
		// decrease capture time
		captureTime -= CP_Interval * defending;
	} else if(overtime) {
		// overtime! Decrease faster: 150 frames to finish
		captureTime -= CaptureTime() * CP_Interval / 150;
	} else {
		// nobody there
		if(wait-- < 0)
			captureTime -= CP_Interval / 2;
	}
	if(captureTime <= 0) {
		captureTime = 0;
		if(prev)
			CaptureEnding();
	}
}

/*  Function: CheckCapture
	Checks whether the point has been captures and updates its owner and the capture time accordingly.

	Returns:
	_true_ if the point has been captured. */
public func CheckCapture() {
	if(captureTime > CaptureTime()) {
		SetOwner(capturingPlayer);
		capturingPlayer = NO_OWNER;
		captureTime = 0;
		overtime = false;
		Captured();
		return true;
	} else {
		if(captureTime == 0)
			capturingPlayer = NO_OWNER;
		return false;
	}
}

/*  Function: SetOvertime
	Enables or disables overtime mode.

	In overtime, capture progress decreases much faster if nobody is on point.

	Parameters:
	val - _true_ to enable overtime mode */
public func SetOvertime(bool val) {
	if(val && !overtime)
		OvertimeStarting();
	overtime = val;
}

/*  Function: IsControlPoint
	This is a control point.

	Returns:
	_true_ */
public func IsControlPoint() { return true; }

/*  Function: GetCapturingPlayer
	
	Returns:
	The capturing player or NO_OWNER if nobody is capturing. */
public func GetCapturingPlayer() {
	return capturingPlayer;
}

/*  Function: GetCapturePercentage
	
	Returns:
	The current capture progress in percent. */
public func GetCapturePercentage() {
	return Min(100, 100 * captureTime / CaptureTime());
}

/*  Function: CaptureStarting
	Called when someone starts capping the point. */
private func CaptureStarting() {
	Sound("koth_startcapture");
	Sound("koth_capturing", 0, 0, 0, 0, 1);
}

/*  Function: Captured
	Called when the point is captured, will output a message to the log and play a sound. */
private func Captured() {
	var team = GetPlayerTeam(GetOwner());
	Log("<c %x>$Capture$</c>", GetTeamColor(team), GetTeamName(team), GetName());
	Sound("koth_captured");
	Sound("koth_sign", true);
	Sound("koth_capturing", 0, 0, 0, 0, -1);
}

/*  Function: CaptureEnding
	Called when the point's capture time runs out. */
private func CaptureEnding() {
	Sound("koth_captured");
	Sound("koth_capturing", 0, 0, 0, 0, -1);
}

/*  Function: OvertimeStarting
	Called when the point switches to overtime, will output a message to the log. */
private func OvertimeStarting() {
	var team = GetPlayerTeam(GetOwner());
	Log("<c %x>$Overtime$</c>", GetTeamColor(team));
	Sound("koth_overtime");
}

