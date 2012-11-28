/*-- Fading Fog --*/

#strict 2

protected func Initialize() {
	var time = FindObject2(Find_ID(TIME));
	if(time != 0)
		time->AddAlarmListener(this, GetNightfallTime());
}

protected func OnAlarm() {
		FadeFromTo(RGBa(255, 255, 255, 0), RGBa(255, 255, 255, 120));
}

