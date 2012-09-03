/*-- BirdTweet --*/

#strict 2

#include RULE

local tweetCount;

private func Initialized(int ruleTypeCount) {
    tweetCount = ruleTypeCount;

    if(IsDay()) ScheduleTweets();

    var time = FindObject2(Find_ID(TIME));
    if(time == 0) return;

    time->AddEventListener(this, ["OnDay", "ScheduleTweets"]);
    time->AddEventListener(this, "OnNightfall");
    time->AddAlarmListener(this, time->GetDaybreakTime() + TIME_TwilightLength / 4);
    time->AddAlarmListener(this, time->GetNightfallTime() + TIME_TwilightLength * 3 / 4);
}

protected func ScheduleTweets() {
    for(var i = 0; i < tweetCount; ++i) {
        // randomize start
        ScheduleCall(0, "Singing", 25 - Random(20));
    }
}

public func OnAlarm(object clock, int time) {
    if(IsDaybreak()) Sound("Rooster*");
    else Sound("Owl");
}

public func OnNightfall(object clock) {
    ClearScheduleCall(this, "Singing");
}

private func Singing() {
    if(!Random(8)) {
		// check if there are some trees left
        if(ObjectCount2(Find_Category(C4D_StaticBack), Find_Func("IsTree")) >= 10) {
            //Sound("BirdSong*", 1);
            var sound = Random(15)+1;
            Sound(Format("BirdSong%d", sound), true);
            //Log("%d", sound);
        } else
			Sound("Crow*", 1);
    }
    ScheduleCall(this, "Singing", 25);
}
