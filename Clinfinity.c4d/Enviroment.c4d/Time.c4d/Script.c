/*	Script: Time
	Controls the passage of time and the day-night cycle. */

#strict 2

static const TIME_TwilightLength = 3000; // = 100 Minutes
static const TIME_SecondsPerFrame = 4;
static const TIME_MaxSkyTransparency = 250;

// You can set these
local daybreakHour, daybreakMinute, nightfallHour, nightfallMinute;

// These get calculated automatically
local daybreak, day, nightfall, night;
local currentSeconds;
local originalSkyAdjust;
local blueHourBackground;
local redSkyBackground;

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
/*	var otherTimeObjects = FindObjects(Find_ID(GetID()), Find_Exclude(this));
	for(var timeObject in otherTimeObjects) {
		timeObject->RemoveObject();
	}*/
	blueHourBackground = RGB(0, 0, 50);
	redSkyBackground = RGB(50, 0, 0);

	originalSkyAdjust = GetSkyAdjust();

	daybreakHour = 5;
	daybreakMinute = 0;
	nightfallHour = 21;
	nightfallMinute = 30;

	CalculateDaytimes();

/*	hours = 12;
	minutes = 0;
	seconds = 0;*/
	currentSeconds = 4 * 3600 + 55 * 60;
	ResumeClock();
}

private func CalculateDaytimes() {
	daybreak = daybreakHour * 3600 + daybreakMinute * 60;
	day = daybreak + TIME_TwilightLength;
	nightfall = nightfallHour * 3600 + nightfallMinute * 60;
	night = nightfall + TIME_TwilightLength;
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
	//SetSkyAdjust(RGBa(255, 255, 255, 255 - brightness), RGB(0, 0, 0));
	if(IsDay()) {
		SetSkyAdjust(RGBa(255, 255, 255, 0), RGB(0, 0, 0));
	} else if(IsNight()) {
		SetSkyAdjust(RGBa(255, 255, 255, TIME_MaxSkyTransparency), RGB(0, 0, 0));
	} else if(IsDaybreak()) {
		var background;
		var progress = currentSeconds - daybreak;
		// if(progress < TIME_TwilightLength / 3) {
		// 	background = RGB(0, 0, 50 * 3 * progress / TIME_TwilightLength);
		// } else if(Inside(progress, TIME_TwilightLength / 3, TIME_TwilightLength * 2 / 3) {

		// }

		// var progress = TIME_TwilightLength - (currentSeconds - daybreak);
		// var background = RGB(0, 0, 50);
		// if(progress < TIME_TwilightLength / 2) {
		// 	background = RGB(100, 50, 0);
		// }
		background = RGB(CalculateDaybreakRed(progress), 0, CalculateDaybreakBlue(progress));
		//var transparency = TIME_MaxSkyTransparency * progress / TIME_TwilightLength;
		//Log("transparency is %d", transparency);
		//Log("transparency is %d", transparency);
		SetSkyAdjust(RGBa(255, 255, 255, CalculateDaybreakTransparency(progress)), background);
	}
}

private func CalculateDaybreakBlue(int progress) {
	var maxBlue = 50;
	progress -= TIME_TwilightLength / 3;
	var result = Max(0, maxBlue - (maxBlue * 3 * Abs(progress) / TIME_TwilightLength));
	//Log("progress/result = %d/%d", progress, result);
	return result;
}

private func CalculateDaybreakRed(int progress) {
	var maxRed = 255;
	progress -= TIME_TwilightLength * 2 / 3;
	var result = Max(0, maxRed - (maxRed * 3 * Abs(progress) / TIME_TwilightLength));
	Log("progress/result = %d/%d", progress, result);
	return result;	
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
