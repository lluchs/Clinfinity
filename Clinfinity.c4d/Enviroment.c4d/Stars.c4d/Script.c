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

	var maxStarsCount = ObjectCount2(Find_ID(GetID())) * 100;
	for(var i = 0; i < maxStarsCount; ++i) {
		CreateStar();
		//ScheduleCall(0, "CreateStar", RandomX(TIME_TwilightLength * 3 / 4 / TIME_SecondsPerFrame, TIME_TwilightLength / TIME_SecondsPerFrame));
	}
}

public func OnNightfall() {
	// for(var i = 0; i < maxStarsCount; ++i) {
	// 	ScheduleCall(0, "CreateStar", RandomX(TIME_TwilightLength * 3 / 4 / TIME_SecondsPerFrame, TIME_TwilightLength / TIME_SecondsPerFrame));
	// }
	for(var star in FindObjects(Find_ID(STAR))) {
		ScheduleCall(0, "FadeStarIn", RandomX(TIME_TwilightLength * 3 / 4 / TIME_SecondsPerFrame, TIME_TwilightLength / TIME_SecondsPerFrame), 0, star);
	}
}

public func CreateStar() {
	CreateObject(STAR, Random(LandscapeWidth()), Random(LandscapeHeight()), NO_OWNER);
}

public func FadeStarIn(object star) {
	star->FadeIn();
}

public func OnDaybreak() {
	var stars = FindObjects(Find_ID(STAR));
	for(var star in stars) {
		ScheduleCall(0, "RemoveStar", 1 /*RandomX(1, TIME_TwilightLength / 10 / TIME_SecondsPerFrame)*/, 0, star);
	}
}

public func RemoveStar(object star) {
	//star->RemoveObject();
	star->FadeOut();
}
