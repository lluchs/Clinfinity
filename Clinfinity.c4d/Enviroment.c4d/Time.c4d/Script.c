/*	Script: Time
	Controls the passage of time and the day/night cycle.
	See <Day/night cycle> for details about the phases of the day/night cycle.

	Several events are sent by the Time object.
	- OnClockStrike, sent every full hour. The current time is passed as parameter to the event handler function.
	- OnDay, sent after daybreak is over and the day begins.
	- OnNight, sent after nightfall, when the night begins.
	- OnDaybreak, sent when daybreak begins.
	- OnNightfall, sent when nightfall begins.

	Additionally, settable <Alarms> are supported. */

#strict 2

/*	Constants: Passage of time
	TIME_TotalDayLength		- Total day length in seconds.
	TIME_TwilightLength		- Length of twilight (daybreak and nightfall) in seconds.
	TIME_SecondsPerFrame	- Number of seconds represented by one frame in game. */
static const TIME_TotalDayLength = 86400;
static const TIME_TwilightLength = 6000; // = 100 Minutes (which is about the time it takes in reality)
static const TIME_SecondsPerFrame = 4;

/*	Constants: Sky brightness and colours
	TIME_DarkSkyBlue	- Value for brightness/blue during the blue hour.
	TIME_BrightSkyBlue	- Value for brightness/blue at the beginning and end of daytime. */
static const TIME_DarkSkyBlue = 30;
static const TIME_BrightSkyBlue = 255;


// These are set in Initialize(). Don't change them afterwards!
local daybreak, nightfall;

// These get calculated automatically. Internal use only, don't touch!
local day, night;
local dayLength, nightLength;
local currentSeconds;

local alarms;


protected func Initialize() {
	alarms = CreateHash();

	// Important: If you set custom values, you absolutely must make sure that nightfall and daybreak don't overlap.
	daybreak = Time(5, 00);
	nightfall = Time(21, 30);

	CalculateDaytimes();
	CalculateDurations();

	var hours = 12;
	var minutes = 0;
	var seconds = 0;
	currentSeconds = hours * 3600 + minutes * 60 + seconds;
	ResumeClock();
}

private func CalculateDaytimes() {
	day = daybreak + TIME_TwilightLength;
	night = (nightfall + TIME_TwilightLength) % TIME_TotalDayLength;
}

private func CalculateDurations() {
	dayLength = nightfall - day;
	nightLength = TIME_TotalDayLength - night + daybreak;
}

// Advances the clock by TIME_SecondsPerFrame
protected func AdvanceClock() {
	currentSeconds += TIME_SecondsPerFrame;
	currentSeconds %= 86400;
	var alarmListeners = HashGet(alarms, currentSeconds, false);
	if(alarmListeners != 0) {
		for(var listener in alarmListeners) {
			listener->~OnAlarm(this, currentSeconds);
		}
	}
	if((currentSeconds % 3600) == 0) {
		Emit("OnClockStrike", currentSeconds);
	}
	SetSkyColour();
	ScheduleCall(this, "AdvanceClock", 1);
}

private func SetSkyColour() {
	if(IsDay()) {
		if(SecondsSince(day) < TIME_SecondsPerFrame) {
			Emit("OnDay");
		}
		SetSkyColourModulation(CalculateDayBrightness(SecondsSince(day)), true, 4);
	} else if(IsNight()) {
		if(SecondsSince(night) < TIME_SecondsPerFrame) {
			Emit("OnNight");
		}
		SetSkyColourModulation(CalculateNightBlue(SecondsSince(night)), true, 4);
	} else if(IsDaybreak()) {
		var progress = SecondsSince(daybreak);
		if(progress < TIME_SecondsPerFrame) {
			Emit("OnDaybreak");
		}
		SetSkyColourModulation(CalculateDaybreakBlue(progress), true, 4);
		//SetSkyColourModulation(CalculateDaybreakRed(progress), false, 5);
	} else if(IsNightfall()) {
		if(SecondsSince(nightfall) < TIME_SecondsPerFrame) {
			Emit("OnNightfall");
		}
		SetSkyColourModulation(CalculateNightfallBrightness(SecondsSince(nightfall)), true, 4);
		SetSkyColourModulation(CalculateNightfallRed(SecondsSince(nightfall)), false, 5);
	}
}

private func CalculateNightBlue(int progress) {
	/*if(progress < nightLength / 4) {
		var blue = 4 * TIME_DarkSkyBlue * (nightLength / 4 - progress) / nightLength;
		return RGB(0, 0, blue);
	} else if(progress > nightLength * 3 / 4) {
		var blue = TIME_DarkSkyBlue * 4 * progress / nightLength - 3 * TIME_DarkSkyBlue;
		return RGB(0, 0, blue);
	}*/
	progress -= nightLength / 2;
	var blue = 3 * TIME_DarkSkyBlue * Abs(progress) / nightLength - TIME_DarkSkyBlue / 2;
	return RGB(0, 0, Max(0, blue));
}

private func CalculateDaybreakBlue(int progress) {
	/*if(progress < TIME_TwilightLength / 2) {
		var maxBrightness = TIME_DarkSkyBlue;
		var brightness = 2 * maxBrightness * progress / TIME_TwilightLength;
		return RGB(brightness, brightness, TIME_DarkSkyBlue);
	} else {
		var minBrightness = TIME_DarkSkyBlue;
		var maxBrightness = TIME_BrightSkyBlue;
		var brightness = 2 * (maxBrightness - minBrightness) * progress / TIME_TwilightLength + (2 * minBrightness - maxBrightness);
		return RGB(brightness, brightness, brightness);
	}*/
	var brightness = TIME_BrightSkyBlue * progress / TIME_TwilightLength;
	var blue = (TIME_BrightSkyBlue - TIME_DarkSkyBlue) * progress / TIME_TwilightLength + TIME_DarkSkyBlue;
	return RGB(brightness, brightness, blue);
}

/*private func CalculateDaybreakRed(int progress) {
	var maxRed = 200;
	progress -= TIME_TwilightLength / 2;
	var red = maxRed - (maxRed * 2 * Abs(progress) / TIME_TwilightLength);
	var green = red * 3 / 8;
	var blue = red * 1 / 4;
	return RGB(red, green, blue);
}*/

private func CalculateDayBrightness(int progress) {
	if(progress < dayLength / 4) {
		var brightness = 4 * (255 - TIME_BrightSkyBlue) * progress / dayLength + TIME_BrightSkyBlue;
		return RGB(brightness, brightness, brightness);
	} else if(progress > dayLength * 3 / 4) {
		var brightness = 4 * (TIME_BrightSkyBlue - 255) * progress / dayLength + 4 * 255 - 3 * TIME_BrightSkyBlue;
		return RGB(brightness, brightness, brightness);
	} else {
		return RGB(255, 255, 255);
	}
}

private func CalculateNightfallBrightness(int progress) {
	/*var brightness = -TIME_BrightSkyBlue * progress / TIME_TwilightLength + TIME_BrightSkyBlue;
	var blue = (TIME_DarkSkyBlue - TIME_BrightSkyBlue) * progress / TIME_TwilightLength + TIME_BrightSkyBlue;
	return RGB(brightness, brightness, blue);*/
	if(progress < TIME_TwilightLength / 2) {
		var brightness = 2 * (TIME_DarkSkyBlue - TIME_BrightSkyBlue) * progress / TIME_TwilightLength + TIME_BrightSkyBlue;
		return RGB(brightness, brightness, brightness);
	} else {
		progress -= TIME_TwilightLength / 2;
		var brightness = 2 * -TIME_DarkSkyBlue * progress / TIME_TwilightLength + TIME_DarkSkyBlue;
		return RGB(brightness, brightness, TIME_DarkSkyBlue);
	}
}

private func CalculateNightfallRed(int progress) {
	var maxRed = 200;
	progress -= TIME_TwilightLength / 2;
	var red = maxRed - (maxRed * 2 * Abs(progress) / TIME_TwilightLength);
	var green = red * 3 / 8;
	var blue = red * 1 / 4;
	return RGB(red, green, blue);
}


/* Passage of time */

private func NormaliseTime(int time) {
	time %= TIME_TotalDayLength;
	if(time < 0) {
		time += TIME_TotalDayLength;
	}
	time = time / TIME_SecondsPerFrame * TIME_SecondsPerFrame; // Don't allow 'odd' seconds.
	return time;
}

/*	Function: Time
	Composes a time value from the given parameters _hours_, _minutes_ and _seconds_,
	with the latter being optional.

	Parameters:
	hours	- Hours part.
	minutes	- Minutes part.
	seconds	- [optional] Seconds part. */
global func Time(int hours, int minutes, int seconds) {
	hours	= BoundBy(hours, 0, 23);
	minutes	= BoundBy(minutes, 0, 59);
	seconds	= BoundBy(seconds, 0, 59);
	return hours * 3600 + minutes * 60 + seconds;
}

/*	Function: SetTime
	Sets the clock to the specified time.

	You should use the function _Time()_ for convenience.
	A typical call that sets the time to 19:30 (7:30 PM) looks like this:
	> SetTime(Time(19, 30))

	Parameters:
	time	- Time measured in seconds. */
public func SetTime(int time) {
	currentSeconds = NormaliseTime(time);
	SetSkyColour();
}

/*	Function: GetTime
	Returns the current time measured in seconds.

	Returns:
	The current time. */
public func GetTime() {
	return currentSeconds;
}

/*	Function: PauseClock
	Stops the advancement of the clock. */
public func PauseClock() {
	ClearScheduleCall(this, "AdvanceClock");
}

/*	Function: ResumeClock
	Starts or resumes the advancement of the clock. */
public func ResumeClock() {
	if(GetEffectCount("IntScheduleCall", this) == 0) {
		ScheduleCall(this, "AdvanceClock", 1);
	}
}

/*	Function: SecondsSince
	Returns how many seconds have passed since the specified point in time.
	The maximum returned value is 86399 (23 hours 59 minutes 59 seconds).
	If more time than that passes, the return value restarts at zero.

	Parameters:
	time	- Point in time, measured in seconds.

	Returns:
	Seconds since _time_, measured in seconds. */
public func SecondsSince(int time) {
	time = NormaliseTime(time);
	var result;
	if(currentSeconds >= time) {
		result = currentSeconds - time;
	} else {
		result = 24 * 60 * 60 - time + currentSeconds;
	}
	return result;
}

/*public func SecondsToTime(int seconds) {
	var hours = seconds / 3600 * 100;
	var minutes = seconds / 60 % 60;
	return hours + minutes;
}*/

public func GetDaybreakTime() {
	return daybreak;
}

public func GetDayTime() {
	return day;
}

public func GetNightfallTime() {
	return nightfall;
}

public func GetNightTime() {
	return night;
}

public func IsDaybreak() {
	return Inside(currentSeconds, daybreak, day - 1);
}

public func IsDay() {
	return Inside(currentSeconds, day, nightfall - 1);
}

public func IsNightfall() {
	return Inside(currentSeconds, nightfall, night - 1);
}

public func IsNight() {
	if(night < daybreak) {
		// Night starts after midnight
		return currentSeconds >= night && currentSeconds < daybreak;
	} else {
		// Night starts before midnight
		return currentSeconds >= night || currentSeconds < daybreak;
	}
}


/*	Section: Alarms
	With alarms, listeners can receive a call (the "alarm") at a specified point in time.
	The listeners should implement a function called _OnAlarm()_,
	which receives the calling object and the current time, measured in seconds, as parameters. */

/*	Function: AddAlarmListener
	Adds a listener, which will receive the alarm at the specified time.

	Notes:
	- The same listener can receive several _OnAlarm()_ calls at different points in time.
	However, it is not possible to add the same listener more than once for the same time.
	- The specified time is rounded to the next lower multiple of TIME_SecondsPerFrame.

	Parameters:
	listener	- Listener that will receive the call.
	time		- Alarm time. */
public func AddAlarmListener(object listener, int time) {
	time = NormaliseTime(time);
	var listeners = HashGet(alarms, time, false);
	if(listeners == 0) {
		listeners = [listener];
	} else if(!InArray(listener, listeners)) {
		PushBack(listener, listeners);
	}
	HashPut(alarms, time, listeners);
}

/*	Function: RemoveAlarmListener
	Removes a listener from the alarms list for the specified time.
	After calling this, the listener will no longer receive alarm calls for the specified point in time,
	but it will still receive the calls for other alarm times, if it was added to any.
	If the listener was not added for the specified alarm time before calling this function,
	the function call has no effect.

	Parameters:
	listener	- Listener to remove.
	time		- Alarm time. */
public func RemoveAlarmListener(object listener, int time) {
	time = NormaliseTime(time);
	var listeners = HashGet(alarms, time, false);
	if(listeners != 0 && InArray(listener, listeners)) {
		RemoveElement(listener, listeners);
		HashPut(alarms, time, listeners);
	}
}


/*	Section: Day/night cycle
	The cycle consists of the four following phases
	- Day: The sky is brightest during this phase.
	- Nightfall: The sky darkens.
	- Night: Very dark or black sky.
	- Daybreak: The sky brightens again.

	In each phase, specifics things can occur.
	For example, nocturnal animals may wake up during nightfall and go to sleep when daybreak begins.
	It should be noted that the functions in this section only return *true* during the respective phase.
	For example, IsDay() does not return *true* during daybreak, even though the sky may already be bright. */

/*	Function: IsDaybreak

	Returns:
	*true* if it is daybreak currently, *false* otherwise. */
global func IsDaybreak() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->IsDaybreak();
}

/*	Function: IsDay

	Returns:
	*true* if it is daytime currently, *false* otherwise. */
global func IsDay() {
	var time = FindObject2(Find_ID(TIME));
	return time == 0 || time->IsDay();
}

/*	Function: IsNightfall

	Returns:
	*true* if it is nightfall currently, *false* otherwise. */
global func IsNightfall() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->IsNightfall();
}

/*	Function: IsNight

	Returns:
	*true* if it is nighttime currently, *false* otherwise. */
global func IsNight() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->IsNight();
}

/*	Re-routed global functions */

global func SetTime(int time) {
	var timeObject = FindObject2(Find_ID(TIME));
	return timeObject != 0 && timeObject->SetTime(time);
}

global func GetTime() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->GetTime();
}

global func PauseClock() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->PauseClock();	
}

global func ResumeClock() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->ResumeClock();
}

global func GetDaybreakTime() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->GetDaybreakTime();
}

global func GetDayTime() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->GetDayTime();
}

global func GetNightfallTime() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->GetNightfallTime();
}

global func GetNightTime() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->GetNightTime();
}
