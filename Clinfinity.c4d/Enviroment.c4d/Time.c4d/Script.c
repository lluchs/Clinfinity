/*	Script: Time
	Controls the passage of time and the day/night cycle.
	See <Day/night cycle> for details about the phases of the day/night cycle.

	Several events are sent by the Time object.
	- OnClockStrike, sent every full hour. The current time is passed as parameter to the event handler function.
	- OnDay, sent after daybreak is over and the day begins.
	- OnNight, sent after nightfall, when the night begins.
	- OnDaybreak, sent when daybreak begins.
	- OnNightfall, sent when nightfall begins. */

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
static const TIME_DarkSkyBlue = 100;
static const TIME_BrightSkyBlue = 255;

/*	Variables: Characteristical points in time
	These can be set in _Initialize()_. Changing the values afterwards has no effect.

	*Important*: If you set custom values, you must make sure that nightfall and daybreak don't overlap.

	daybreakHour	- Hour of daybreak.
	daybreakMinute	- Precise minute of daybreak.
	nightfallHour	- Hour of nightfall.
	nightfallMinute	- Precise minute of nightfall. */
local daybreakHour, daybreakMinute, nightfallHour, nightfallMinute;

// These get calculated automatically. Internal use only, don't touch!
local daybreak, day, nightfall, night;
local dayLength, nightLength;
local currentSeconds;


protected func Initialize() {
	daybreakHour = 5;
	daybreakMinute = 0;
	nightfallHour = 21;
	nightfallMinute = 30;

	CalculateDaytimes();
	CalculateDurations();

	var hours = 12;
	var minutes = 0;
	var seconds = 0;
	currentSeconds = hours * 3600 + minutes * 60 + seconds;
	ResumeClock();
}

private func CalculateDaytimes() {
	daybreak = daybreakHour * 3600 + daybreakMinute * 60;
	day = daybreak + TIME_TwilightLength;
	nightfall = nightfallHour * 3600 + nightfallMinute * 60;
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
	if((currentSeconds % 3600) == 0) {
		Emit("OnClockStrike", currentSeconds);
	}
	SetSkyColour();
	ResumeClock();
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

	Parameters:
	time	- Time measured in seconds. */
public func SetTime(int time) {
	time %= TIME_TotalDayLength;
	if(time < 0) {
		time += TIME_TotalDayLength;
	}
	time = time / TIME_SecondsPerFrame * TIME_SecondsPerFrame; // Don't set 'odd' seconds.
	currentSeconds = time;
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
	ClearScheduleCall(0, "AdvanceClock"); // TODO: Doesn't work! Must use "this" instead of 0 (man this sucks, the docu is lying)
}

/*	Function: ResumeClock
	Starts or resumes the advancement of the clock. */
public func ResumeClock() {
	ClearScheduleCall(0, "AdvanceClock");
	ScheduleCall(0, "AdvanceClock", 1);
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
	var result;
	if(currentSeconds >= time) {
		result = currentSeconds - time;
	} else {
		result = 24 * 60 * 60 - time + currentSeconds;
	}
	return result;
}

public func SecondsToTime(int seconds) {
	var hours = seconds / 3600 * 100;
	var minutes = seconds / 60 % 60;
	return hours + minutes;
}

public func GetNightfallTime() {
	return nightfall;
}

public func IsDay() {
	return Inside(currentSeconds, day, nightfall - 1);
}

public func IsNight() {
	return currentSeconds >= night || currentSeconds < daybreak;
}

public func IsDaybreak() {
	return Inside(currentSeconds, daybreak, day - 1);
}

public func IsNightfall() {
	return Inside(currentSeconds, nightfall, night - 1);
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

/*	Function: IsDay

	Returns:
	*true* if it is daytime currently, *false* otherwise. */
global func IsDay() {
	var time = FindObject2(Find_ID(TIME));
	return time == 0 || time->IsDay();
}

/*	Function: IsNight

	Returns:
	*true* if it is nighttime currently, *false* otherwise. */
global func IsNight() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->IsNight();
}

/*	Function: IsDaybreak

	Returns:
	*true* if it is daybreak currently, *false* otherwise. */
global func IsDaybreak() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->IsDaybreak();
}

/*	Function: IsNightfall

	Returns:
	*true* if it is nightfall currently, *false* otherwise. */
global func IsNightfall() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->IsNightfall();
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

global func GetNightfallTime() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->GetNightfallTime();
}
