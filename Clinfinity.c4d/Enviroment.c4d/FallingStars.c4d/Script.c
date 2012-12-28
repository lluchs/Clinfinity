/*-- Sternenschnuppenhimmel --*/

#strict 2

#include RULE

private func Initialized(int ruleTypeCount) {
	var time = FindObject2(Find_ID(TIME));
	if(time == 0) {
		RemoveObject();
		return;
	}
	SetPosition(0, 0);
	time->AddEventListener(this, "OnNightfall");
	time->AddAlarmListener(this, time->GetDaybreakTime() - 255 * TIME_SecondsPerFrame);

	var maxStarsCount = (LandscapeWidth() * LandscapeHeight() * ruleTypeCount) / 200000;
	for(var i = 0; i < maxStarsCount; ++i) {
		CreateObject(FLST, Random(LandscapeWidth()), Random(LandscapeHeight()), NO_OWNER);
	}
}

public func OnNightfall() {
	for(var star in FindObjects(Find_ID(FLST))) {
		ScheduleCall(0, "FadeStarIn", RandomX(TIME_TwilightLength * 9 / 10 / TIME_SecondsPerFrame, TIME_TwilightLength / TIME_SecondsPerFrame), 0, star);
	}
}

public func OnAlarm(object clock, int time) {
	for(var star in FindObjects(Find_ID(FLST))) {
		ScheduleCall(0, "FadeStarOut", RandomX(1, TIME_TwilightLength / 10 / TIME_SecondsPerFrame), 0, star);
	}
}

public func FadeStarIn(object star) {
	star->FadeIn();
}

public func FadeStarOut(object star) {
	star->FadeOut();
}
