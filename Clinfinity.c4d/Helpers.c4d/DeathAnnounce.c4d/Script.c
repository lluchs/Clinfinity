/*  Script: DeathAnnounce
	Handles improved death messages. */

#strict 2

static const DANN_Duration = 350;

local message, posEase, alphaEase, messageColor;

/*  Function: Announce
	Announces the death of the given Clonk.

	Parameters:
	clonk - the clonk to announce death for */
public func Announce(object clonk) {
	var messages = ["$DeathMsg1$", "$DeathMsg2$", "$DeathMsg3$", "$DeathMsg4$", "$DeathMsg5$", "$DeathMsg6$", "$DeathMsg7$"];
	message = clonk->LocalN("deathMessage");
	if(!GetLength(message))
		message = Format(messages[Random(7)], clonk->LocalN("name"));
	posEase = CreateEaseFunction("cubic-in-out", DANN_Duration);
	alphaEase = CreateEaseFunction("quad-in-out", DANN_Duration);
	messageColor = Desaturate(clonk->GetColorDw());
	AddEffect("Fade", this, 1, 1, this);

	// Point to the death
	PointOut(messageColor, Format("† %s", GetPlayerName(clonk->GetOwner())));
}

/*  Function: DeathAnnounce
	Overwrites the global engine-defined function to provide the custom message. */
global func DeathAnnounce() {
	CreateObject(DANN)->Announce(this);
	return true;
}

private func Desaturate(int color) {
	var hsl = RGB2HSL(color);
	var hue, sat, light, alpha;
	SplitRGBaValue(hsl, hue, sat, light, alpha);
	return HSL(hue, sat / 4, light);
}

func FxFadeTimer(object target, int effectNum, int effectTime) {
	var color = SetRGBaValue(messageColor, ChangeRange(EvalEase(alphaEase, effectTime), 0, DANN_Duration, 0, 255));
	CustomMessage(message, this, NO_OWNER, 0, -EvalEase(posEase, effectTime), color);
	if(effectTime > DANN_Duration)
		RemoveObject();
}
