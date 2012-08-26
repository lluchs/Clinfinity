/*	Script: Time
	Controls the passage of time and the day-night cycle. */

#strict 2

static const TIME_TotalDayLength = 86400;
static const TIME_TwilightLength = 3000; // = 100 Minutes
static const TIME_SecondsPerFrame = 4;
static const TIME_MaxSkyTransparency = 250;

// You can set these
local daybreakHour, daybreakMinute, nightfallHour, nightfallMinute;

// These get calculated automatically
local daybreak, day, nightfall, night;
local nightLength;
local currentSeconds;

local fullHourListeners; // TODO: Implement adding and removing listeners.


public func SetTime(int hours, int minutes) {
	// TODO
	//CalculateSkyColour();
	SetSkyColour();
}

// Advances the clock by secondsPerFrame
protected func AdvanceClock() {
	currentSeconds += TIME_SecondsPerFrame;
	currentSeconds %= 86400;//(24 * 60 * 60) / TIME_SecondsPerFrame;
	if((currentSeconds % 3600) == 0) {
		Log("Current hour: %d (%d seconds)", currentSeconds / 3600, currentSeconds);
		// TODO: Notify listeners here.
	}
/*	seconds += TIME_SecondsPerFrame;
	if(seconds >= 60) {
		seconds %= 60;
		minutes++;
		if(minutes >= 60) {
			minutes = 0;
			hours++;
			if(hours >= 24) {
				hours = 0;
			}
			Log("Current hour: %d", hours);
			// TODO: Notify listeners here.
		}
	}
*/
	//CalculateSkyColour();
	SetSkyColour();
	ResumeClock();
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

protected func Initialize() {
	ScheduleCall(0, "Initialized", 1);
}

protected func Initialized() {
	daybreakHour = 5;
	daybreakMinute = 0;
	nightfallHour = 21;
	nightfallMinute = 30;

	CalculateDaytimes();
	CalculateDurations();

/*	hours = 12;
	minutes = 0;
	seconds = 0;*/
	currentSeconds = 4 * 3600 + 55 * 60;
	//currentSeconds = 11000;
	ResumeClock();
}

private func CalculateDaytimes() {
	daybreak = daybreakHour * 3600 + daybreakMinute * 60;
	day = daybreak + TIME_TwilightLength;
	nightfall = nightfallHour * 3600 + nightfallMinute * 60;
	night = nightfall + TIME_TwilightLength;
}

private func CalculateDurations() {
	nightLength = TIME_TotalDayLength - night + daybreak;
}
/*
private func CalculateSkyColour() {
	if(Inside(hours, 6, 11) || Inside(hours, 18, 21)) {
		brightness = 127;
	} else if(Inside(hours, 12, 17)) {
		brightness = 255;
	} else {
		brightness = 0;
	}
}
*/







private func SetSkyColour() {
	if(IsDay()) {
		SetSkyColourModulation(RGBa(255, 255, 255), true, 4);
	} else if(IsNight()) {
		SetSkyColourModulation(CalculateNightBlue(SecondsSince(night), nightLength), true, 4);
	} else if(IsDaybreak()) {
		var progress = SecondsSince(daybreak);
		SetSkyColourModulation(CalculateDaybreakBlue(progress), true, 4);
		SetSkyColourModulation(CalculateDaybreakRed(progress), false, 5);
	}
}

private func CalculateNightBlue(int progress, int nightLength) {
	var maxBlue = 20;
	if(progress < nightLength / 4) {
		var brightness = 4 * maxBlue * (nightLength / 4 - progress) / nightLength;
		return RGB(brightness, brightness, brightness);
	} else if(progress > nightLength * 3 / 4) {
		var blue = maxBlue * 4 * progress / nightLength - 3 * maxBlue;
		return RGB(0, 0, blue);
	}
	return RGB(0, 0, 0);
}

private func CalculateDaybreakBlue(int progress) {
	if(progress < TIME_TwilightLength / 2) {
		var maxBrightness = 20;
		var brightness = 2 * maxBrightness * progress / TIME_TwilightLength;
		return RGB(brightness, brightness, 20);
	} else {
		var minBrightness = 20;
		var maxBrightness = 225;
		var brightness = 410 * progress / TIME_TwilightLength + (2 * minBrightness - maxBrightness);
		return RGB(brightness, brightness, brightness);
	}
}

/*private func CalculateDaybreakBlue(int progress) {
	var maxBlue = 50;
	progress -= TIME_TwilightLength / 3;
	var result = Max(0, maxBlue - (maxBlue * 3 * Abs(progress) / TIME_TwilightLength));
	//Log("progress/result = %d/%d", progress, result);
	return result;
}*/

private func CalculateDaybreakRed(int progress) {
	var maxRed = 200;
	progress -= TIME_TwilightLength / 2;
	var red = maxRed - (maxRed * 2 * Abs(progress) / TIME_TwilightLength);
	var green = red * 3 / 8;
	var blue = red * 1 / 4;
	return RGB(red, green, blue);
}

private func CalculateDaybreakTransparency(int progress) {
	progress = TIME_TwilightLength -  progress;
	return TIME_MaxSkyTransparency * progress / TIME_TwilightLength;
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


/* Status */

global func IsDay()
{
  var pTime; 
  if (!(pTime = FindObject(TIME))) return(1);
  if (Local(2, pTime)) return(1);
  return(0);
}
  
global func IsNight()
{
  return(!IsDay());
}
    
/* Himmelsfarbe */

local SkyAdjustOrig;

private func RestoreSkyColors(iPercent)
{
  // Alte Speicherung? Übertragen
  if (Local (4)) GetOldSkyColors();
  if (Local (6)) 
  {
    var i;
    // ehemaliges OldGfx: Normales SetSkyColor
    while(i < 20) RestoreSkyColor(i++, 100);
  }
  // NewGfx: Einfach SetSkyAdjust
  // Minimale Gammakontrolle (Rampe 3)
  var lt = iPercent / 2 + 78;
  SetGamma(0, RGB(lt, lt, 128), 16777215, 3);
  SetSkyAdjust(RGBa(
    iPercent * GetRGBValue(SkyAdjustOrig,1) / 100,
    iPercent * GetRGBValue(SkyAdjustOrig,2) / 100,
    iPercent * GetRGBValue(SkyAdjustOrig,3) / 100,
    iPercent * GetRGBValue(SkyAdjustOrig,0) / 100  ), GetSkyAdjust(1));
  return(1);
}
  
private func RestoreSkyColor(int iColor, int iPercent)
{
  SetSkyColor(iColor,
              ((Local(iColor+6)>>16 & 255) * iPercent)/100,
              ((Local(iColor+6)>> 8 & 255) * iPercent)/100,
              ((Local(iColor+6)     & 255) * iPercent)/100);
  Local(iColor + 6) = 0;
  return;
}

private func GetOldSkyColors()
{
  var i;
  i=-1; while (++i<11) Local(i+ 6)=Local(i,Local(4));
  i=-1; while (++i<11) Local(i+16)=Local(i,Local(5));
  // Alte Hilfsobjekte entfernen
  RemoveObject(Local(4));
  RemoveObject(Local(5));
  return(1);
}
