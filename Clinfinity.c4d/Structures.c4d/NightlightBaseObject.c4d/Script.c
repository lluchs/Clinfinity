/*	Script: Night light base object
	By inheriting from this object, a structure's graphics is automatically switched,
	according to the current state of the day/night cycle.
	With this, a structure can for example switch on spotlights at night.

	A structure using alternate graphics for day and night must follow the following rules
	- Provide GraphicsNight.png
	- Inherit from NLBO
	- Call inherited() in the structure's Initialize() and Destruction() functions

	The following events are then handled automatically
	- Switching to the according graphics on daybreak and nightfall.
	- Setting the according graphics when building the structure at nightfall or at night. */

#strict 2

protected func Initialize() {
	var clock = FindObject2(Find_ID(TIME));
	if(clock != 0) {
		clock->AddAlarmListener(this, GetNightfallTime());
		clock->AddAlarmListener(this, GetDaybreakTime());
	}
	if(IsNightfall() || IsNight()) {
		SetDayNightGraphics("Night");
	}
	return _inherited();
}

protected func Destruction() {
	var clock = FindObject2(Find_ID(TIME));
	if(clock != 0) {
		clock->RemoveAlarmListener(this, GetNightfallTime());
		clock->RemoveAlarmListener(this, GetDaybreakTime());
	}
	return _inherited();
}

protected func OnAlarm(object clock, int time) {
	if(IsNightfall()) {
		ScheduleCall(this, "SetDayNightGraphics", RandomX(TIME_TwilightLength * 1 / 4 / TIME_SecondsPerFrame, TIME_TwilightLength / TIME_SecondsPerFrame), 0, "Night");
	} else {
		ScheduleCall(this, "SetDayNightGraphics", RandomX(1, TIME_TwilightLength / 4 / TIME_SecondsPerFrame), 0, "");
	}
}

protected func SetDayNightGraphics(string graphics) {
	SetGraphics(graphics);
}
