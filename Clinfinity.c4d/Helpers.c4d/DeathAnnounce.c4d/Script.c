/*  Script: DeathAnnounce
	Handles improved death messages. */

#strict 2

static const DANN_Duration = 350;

local message;

/*  Function: Announce
	Announces the death of the given Clonk.

	Parameters:
	clonk - the clonk to announce death for */
public func Announce(object clonk) {
	var messages = ["$DeathMsg1$", "$DeathMsg2$", "$DeathMsg3$", "$DeathMsg4$", "$DeathMsg5$", "$DeathMsg6$", "$DeathMsg7$"];
	message = clonk->LocalN("deathMessage");
	if(!GetLength(message))
		message = Format(messages[Random(7)], clonk->LocalN("name"));
	AddEffect("Fade", this, 1, 1, this);
}

/*  Function: DeathAnnounce
	Overwrites the global engine-defined function to provide the custom message. */
global func DeathAnnounce() {
	CreateObject(DANN)->Announce(this);
	return true;
}

func FxFadeTimer(object target, int effectNum, int effectTime) {
	var color = RGBa(200, 200, 200, ChangeRange(Ease("quad-in-out", effectTime, DANN_Duration), 0, DANN_Duration, 0, 255));
	CustomMessage(message, this, NO_OWNER, 0, -Ease("cubic-in-out", effectTime, DANN_Duration), color);
	if(effectTime > DANN_Duration)
		RemoveObject();
}
