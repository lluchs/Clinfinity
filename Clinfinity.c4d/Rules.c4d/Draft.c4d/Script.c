/*	Script: Drafts
	Rule: When activated, local drafts are placed in the landscape.
	These drafts accelerate Clonks in the air, although those using the wing suit get a much greater boost.
	By riding the drafts, gliding Clonks can reach higher or more distant places which are normally inaccessible.

	Drafts have limited bounds, inside which they accelerate Clonks in the draft's direction.
	The number of drafts is determined by the number of activated drafts rules: For each rule, one draft is placed.

	A draft's position is chosen randomly, where it will stay for a configurable duration.
	You can specify a minimum and maximum value, the actual duration a draft stays is then randomised within these bounds.
	*Note:* This behaviour can be overridden by implementing a function called "IsDraftPermanent" that returns true.
	This function may be defined in the scenario script or any rule, goal or environment object.
	If such a function exists, drafts will only set their position once and then stay. */

#strict 2

/*	Variables: Draft size and appearance
	draftWidth			- Width of the draft's bounds, without rotation applied.
	draftHeight			- Height of the draft's bounds, without rotation applied.
	draftDistance		- Distance in which Clonks are searched for, automatically calculated from draftWidth and draftHeight.
	draftParticleColour	- Colour and opacity of emitted draft particles. */
local draftWidth, draftHeight, draftDistance, draftParticleColour;
/*	Variables: Random placement
	These variables determine the duration that a draft keeps its position, before repositioning itself randomly.

	permanentDraft      - _true_ if this draft is permanent.
	minDraftDuration	- Minimum duration.
	maxDraftDuration	- Maximum duration. */
local permanentDraft, minDraftDuration, maxDraftDuration;
/*	 Variables: Clonk acceleration
	maxGliderSpeedUpwards	- Maximum speed a glider is accelerated to.
	gliderAcceleration		- Acceleration rate for gliders. Unit is 1/10 pixels per frame. */
local maxGliderSpeedUpwards, gliderAcceleration;

local active;

protected func Activate(byPlayer) {
	MessageWindow(GetDesc(), byPlayer);
	return 1;
}

protected func Initialize() {
	SetSize(50, 150);
	draftParticleColour = RGBa(255, 255, 255, 210);
	minDraftDuration = 1050;
	maxDraftDuration = 2100;
	maxGliderSpeedUpwards = 60;
	gliderAcceleration = 5;
	// allow for setting it permanent
	ScheduleCall(this, "SetRandomPosition", 1);
	Enable();
}

/*	Function: SetSize
	Sets the draft's bounds inside which it accelerates Clonks.
	The bounds are specified for unrotated drafts.
	If a draft is rotated, the searching area is rotated automatically as well.

	Parameters:
	width 	- Bounds width.
	height	- Bounds height. */
public func SetSize(int width, int height) {
	draftWidth = width;
	draftHeight = height;
	draftDistance = Sqrt(Pow(draftWidth, 2) + Pow(draftHeight, 2)) / 2;
}

/*  Function: SetPermanent
	Makes this draft permanent. */
public func SetPermanent() {
	permanentDraft = true;
}

/*  Function: Enable
	Enables this draft. */
public func Enable() {
	if(!GetEffect("Draft", this)) {
		AddEffect("Draft", this, 1, 10, this);
		AddEffect("DraftParticle", this, 1, 1, this);
		return true;
	}
}

/*  Function: Disable
	Disables this draft.

	A disabled draft will continue to exist and will keep its position. Repositioning will still occur.

	A disabled draft can be re-enabled with <Enable>. */
public func Disable() {
	RemoveEffect("Draft", this);
	RemoveEffect("DraftParticle", this);
	return true;
}

protected func FxDraftParticleTimer(object target, int effectNum, int effectTime) {
	var draftDirection = GetR();
	if( !Random(2) ) {
		var x = RandomX(-draftWidth / 2, draftWidth / 2),
		    y = RandomX(-3, 3),
		    speed = RandomX(-draftHeight / 6,-draftHeight / 4);
		// rotation
		Rotate(draftDirection, x, y);
		CreateParticle("WindSpark", x, y, -Sin(draftDirection, speed), Cos(draftDirection, speed), 40, draftParticleColour);
	}
}

protected func FxDraftTimer(object target, int effectNum, int effectTime) {
	var draftDirection = GetR();
	var x = 0, y = -draftHeight / 2;
	Rotate(draftDirection, x, y);
	var gliders = FindObjects(Find_NoContainer(), Find_Distance(draftDistance, x, y), Find_Category(C4D_Living));
	if(GetLength(gliders)) {
		if(!active) {
			active = true;
			ChangeEffect(0, target, effectNum, "Draft", 1);
		}
		for( var glider in gliders ) {
			var gx = glider->GetX() - GetX(), gy = glider->GetY() - GetY();
			Rotate(-draftDirection, gx, gy);
			if(Inside(gx, -draftWidth / 2, draftWidth / 2) && Inside(gy, -draftHeight, 0)) {
				var xDir = glider->GetXDir(0, 100), yDir = glider->GetYDir(0, 100);
				var xAcc = Sin(draftDirection, gliderAcceleration * 10), yAcc = -Cos(draftDirection, gliderAcceleration * 10);
				// Not actually gliding? Less acceleration!
				if(!glider->~IsGliding()) {
					xAcc /= 3;
					yAcc /= 3;
				}
				glider->SetXDir(xDir + xAcc, 0, 100);
				//Message("X: %d, Y: %d", this, xAcc, yAcc);
				if(yDir > -maxGliderSpeedUpwards * 10) {
					glider->SetYDir(yDir + yAcc, 0, 100);
				}
			}
		}
	} else if(active) {
		active = false;
		ChangeEffect(0, target, effectNum, "Draft", 10);
	}
}

protected func SetRandomPosition() {
	if(permanentDraft)
		return;
	var x = Random(LandscapeWidth());
	var y = Random(LandscapeHeight());
	var otherDraft = FindObject2( Find_And( Find_ID(GetID()), Find_InRect(AbsX(x) - draftWidth / 2, AbsY(y) - draftHeight, draftWidth, draftHeight) ) );
	if(otherDraft != 0) {
		// Anderes Aufwind-Objekt schon an der Stelle: Nächstes Frame neue Position suchen.
		ScheduleCall(this, "SetRandomPosition", 1);
	} else {
		if( !GameCallEx("IsDraftPermanent") ) {
			// Kein anderes Aufwind-Objekt: Neue Position erst in einer Weile suchen.
			ScheduleCall(this, "SetRandomPosition", RandomX(minDraftDuration, maxDraftDuration));
		}
		SetPosition(x, y);
	}
}
