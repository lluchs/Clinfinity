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
	AddEffect("ControlPoint", this, 1, CP_Interval, this);
	capturingPlayer = NO_OWNER;
	captureTime = 0;
	overtime = false;
	return _inherited(...);
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
	if(capturing) {
		if(defending) {
			// don't do anything
			return;
		}
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
	captureTime = Max(captureTime, 0);
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
		CaptureMsg();
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
		OvertimeMsg();
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

/*  Function: CaptureMsg
	Called when the point is captured, will output a message to the log. */
private func CaptureMsg() {
	var team = GetPlayerTeam(GetOwner());
	Log("<c %x>$Capture$</c>", GetTeamColor(team), GetTeamName(team), GetName());
}

/*  Function: OvertimeMsg
	Called when the point switches to overtime, will output a message to the log. */
private func OvertimeMsg() {
	var team = GetPlayerTeam(GetOwner());
	Log("<c %x>$Overtime$</c>", GetTeamColor(team));
}
