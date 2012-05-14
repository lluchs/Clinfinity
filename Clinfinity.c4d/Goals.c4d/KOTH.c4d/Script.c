/*  Script: King of the Hill
	Goal: Teams fight for a control point.

	They try to hold the control point until their timer runs out. */

#strict 2

#include GOAL

// assumed frames per second
static const KOTH_FPS = 30;
// default win time in 'seconds'
static const KOTH_WinTime = 180;
// number of animation faces for capturing
static const KOTH_CapturePhases = 20;

// the capture point
local cp;
// the timer array which tracks progress
local timer;
// last update frame
local lastUpdate;
// the winner or -1
local winningTeam;

protected func Completion() {
	// Rivalry!
	CreateObject(RVLR);
	// HUD position
	SetPosition(150, 32);
	// timer initialization
	timer = CreateArray(GetTeamCount());
	var winTime = GameCall("WinningTime") || KOTH_WinTime * KOTH_FPS;
	for(var i = 0, len = GetLength(timer); i < len; i++)
		timer[i] = winTime;
	
	// no winner initially
	winningTeam = -1;
}

/*  Function: SetCP
	Sets the capture point this goal tracks.
	
	Paramters:
	point - the capture point */
public func SetCP(object point) {
	if(point->IsControlPoint())
		cp = point;
}

/*  Function: IsFulfilled
	Callback for GOAL.

	Returns:
	_true_ if one team has their timer at zero. */
public func IsFulfilled() {
	// shortcut
	if(winningTeam != -1)
		return true;
	// which player holds the control point?
	var owner = cp->GetOwner();
	if(owner != NO_OWNER) {
		var team = GetPlayerTeam(owner) - 1;
		// subtract time
		timer[team] -= FrameCounter() - lastUpdate;
		// has this team won?
		if(timer[team] <= 0) {
			winningTeam = team + 1;
			return true;
		}
	}
	lastUpdate = FrameCounter();
	return false;
}

/*  Function: IsFulfilledforPlr
	Callback for GOAL.

	Parameters:
	plr - the player to check

	Return:
	_true_ if the given player is in the winning team. */
public func IsFulfilledforPlr(int plr) {
	return IsFulfilled() && GetPlayerTeam(plr) == winningTeam;
}

/*  Function: UpdateHUD
	Updates the KOTH-HUD. */
public func UpdateHUD() {
	// base color: holding team
	var team = GetPlayerTeam(cp->GetOwner());
	if(team) {
		SetClrModulation(GetTeamColor(team));
	}
	// overlay: capturing team
	team = GetPlayerTeam(cp->GetCapturingPlayer());
	if(team) {
		var action = Format("Capturing%d", ChangeRange(cp->GetCapturePercentage(), 0, 100, 0, KOTH_CapturePhases));
		SetStillOverlayAction(action, 1);
		SetClrModulation(GetTeamColor(team), 0, 1);
	} else {
		// remove overlay
		SetGraphics(0, 0, 0, 1);
	}
}

protected func Timer() {
	IsFulfilled();
	UpdateHUD();
}

