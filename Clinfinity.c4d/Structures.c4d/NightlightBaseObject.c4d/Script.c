/*	Script: Night light base object
	Inheriting from this object automatically activates...
	follow these rules:
	- Provide an alternative GraphicsNight.png */

#strict 2

protected func Initialize() {
	var clock = FindObject2(Find_ID(TIME));
	if(clock != 0) {
		clock->AddAlarmListener(this, GetNightfallTime());
		clock->AddAlarmListener(this, GetDaybreakTime());
	}
	return _inherited();
}

protected func OnAlarm(object clock, int time) {
	if(IsNightfall()) {
		ScheduleCall(this, "SetDayNightGraphics", RandomX(TIME_TwilightLength * 3 / 4 / TIME_SecondsPerFrame, TIME_TwilightLength / TIME_SecondsPerFrame), 0, "Night");
	} else {
		ScheduleCall(this, "SetDayNightGraphics", RandomX(1, TIME_TwilightLength / 4 / TIME_SecondsPerFrame), 0, "");
	}
}

protected func SetDayNightGraphics(string graphics) {
	SetGraphics(graphics);
}
