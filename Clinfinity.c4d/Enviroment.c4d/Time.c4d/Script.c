/*	Script: Time
	Controls the passage of time and the day-night cycle. */

#strict 2

static const TIME_TotalDayLength = 86400;
static const TIME_TwilightLength = 3000; // = 200 Minutes (which is about double the time as it is in reality, but looks better)
static const TIME_SecondsPerFrame = 4;
static const TIME_MaxSkyTransparency = 250;

static const TIME_DarkSkyBlue = 20;
static const TIME_BrightSkyBlue = 225;

// You can set these
local daybreakHour, daybreakMinute, nightfallHour, nightfallMinute;

// These get calculated automatically
local daybreak, day, nightfall, night;
local dayLength, nightLength;
local currentSeconds;

local fullHourListeners; // TODO: Implement adding and removing listeners.

protected func Initialize() {
	daybreakHour = 5;
	daybreakMinute = 0;
	nightfallHour = 21;
	nightfallMinute = 30;

	CalculateDaytimes();
	CalculateDurations();

	var hours = 17;
	var minutes = 0;
	var seconds = 0;
	currentSeconds = hours * 3600 + minutes * 60 + seconds;
	ResumeClock();
}

private func CalculateDaytimes() {
	daybreak = daybreakHour * 3600 + daybreakMinute * 60;
	day = daybreak + TIME_TwilightLength;
	nightfall = nightfallHour * 3600 + nightfallMinute * 60;
	night = nightfall + TIME_TwilightLength;
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
		Log("Current hour: %d (%d seconds)", currentSeconds / 3600, currentSeconds);
		// TODO: Notify listeners here.
	}
	SetSkyColour();
	ResumeClock();
}

private func SetSkyColour() {
	if(IsDay()) {
		SetSkyColourModulation(CalculateDayBrightness(SecondsSince(day)), true, 4);
	} else if(IsNight()) {
		SetSkyColourModulation(CalculateNightBlue(SecondsSince(night)), true, 4);
	} else if(IsDaybreak()) {
		var progress = SecondsSince(daybreak);
		SetSkyColourModulation(CalculateDaybreakBlue(progress), true, 4);
		SetSkyColourModulation(CalculateDaybreakRed(progress), false, 5);
	} else if(IsNightfall()) {
		SetSkyColourModulation(CalculateNightfallBrightness(SecondsSince(nightfall)), true, 4);
	}
}

private func CalculateNightBlue(int progress) {
	if(progress < nightLength / 4) {
		var blue = 4 * TIME_DarkSkyBlue * (nightLength / 4 - progress) / nightLength;
		return RGB(0, 0, blue);
	} else if(progress > nightLength * 3 / 4) {
		var blue = TIME_DarkSkyBlue * 4 * progress / nightLength - 3 * TIME_DarkSkyBlue;
		return RGB(0, 0, blue);
	}
	return RGB(0, 0, 0);
}

private func CalculateDaybreakBlue(int progress) {
	if(progress < TIME_TwilightLength / 2) {
		var maxBrightness = TIME_DarkSkyBlue;
		var brightness = 2 * maxBrightness * progress / TIME_TwilightLength;
		return RGB(brightness, brightness, TIME_DarkSkyBlue);
	} else {
		var minBrightness = TIME_DarkSkyBlue;
		var maxBrightness = 225;
		var brightness = 410 * progress / TIME_TwilightLength + (2 * minBrightness - maxBrightness);
		return RGB(brightness, brightness, brightness);
	}
}

private func CalculateDaybreakRed(int progress) {
	var maxRed = 200;
	progress -= TIME_TwilightLength / 2;
	var red = maxRed - (maxRed * 2 * Abs(progress) / TIME_TwilightLength);
	var green = red * 3 / 8;
	var blue = red * 1 / 4;
	return RGB(red, green, blue);
}

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
	var brightness = -TIME_BrightSkyBlue * progress / TIME_TwilightLength + TIME_BrightSkyBlue;
	var blue = (TIME_DarkSkyBlue - TIME_BrightSkyBlue) * progress / TIME_TwilightLength + TIME_BrightSkyBlue;
	return RGB(brightness, brightness, blue);
}


/* Passage of time */

public func SetTime(int hours, int minutes) {
	// TODO
	//CalculateSkyColour();
	SetSkyColour();
}

/*	Function: PauseClock
	Stops the advancement of time. */
public func PauseClock() {
	ClearScheduleCall(0, "AdvanceClock");
}

/*	Function: ResumeClock
	Starts or resumes the advancement of time. */
public func ResumeClock() {
	ClearScheduleCall(0, "AdvanceClock");
	ScheduleCall(0, "AdvanceClock", 1);
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

public func SecondsSince(int time) {
	var result;
	if(currentSeconds >= time) {
		result = currentSeconds - time;
	} else {
		result = 24 * 60 * 60 - time + currentSeconds;
	}
	return result;
}


/* Re-routed global functions */

global func IsDay() {
	var time = FindObject2(Find_ID(TIME));
	return time == 0 || time->IsDay();
}
  
global func IsNight() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->IsNight();
}

global func IsDaybreak() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->IsDaybreak();
}

global func IsNightfall() {
	var time = FindObject2(Find_ID(TIME));
	return time != 0 && time->IsNightfall();
}
