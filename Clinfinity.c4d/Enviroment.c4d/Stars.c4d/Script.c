/*-- Sternenhimmel --*/

#strict 2

protected func Initialize() {
	SetPosition(0, 0);
	ScheduleCall(0, "Initialized", 1);
}

private func Initialized() {
	var time = FindObject2(Find_ID(TIME));
	if(time == 0) {
		RemoveObject();
		return;
	}
	time->AddEventListener(this, "OnNightfall");
	time->AddEventListener(this, "OnDaybreak");

	var others = FindObjects(Find_ID(GetID()), Find_Exclude(this));
	for(var other in others) {
		other->RemoveObject();
	}

	var maxStarsCount = (LandscapeWidth() * LandscapeHeight() * ObjectCount2(Find_ID(GetID()))) / 20000;
	for(var i = 0; i < maxStarsCount; ++i) {
		CreateObject(STAR, Random(LandscapeWidth()), Random(LandscapeHeight()), NO_OWNER);
	}
}

public func OnNightfall() {
	for(var star in FindObjects(Find_ID(STAR))) {
		ScheduleCall(0, "FadeStarIn", RandomX(TIME_TwilightLength / 2 / TIME_SecondsPerFrame, TIME_TwilightLength * 3 / 4 / TIME_SecondsPerFrame), 0, star);
	}
}

public func FadeStarIn(object star) {
	star->FadeIn();
}

public func OnDaybreak() {
	var stars = FindObjects(Find_ID(STAR));
	for(var star in stars) {
		star->FadeOut();
	}
}
