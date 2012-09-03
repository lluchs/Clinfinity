/*-- Zikaden --*/

#strict 2

protected func Initialize() {
    var time = FindObject2(Find_ID(TIME));
    if(time == 0) {
    	StartCicadas();
    	return;
    } else if(IsNight()) {
    	StartCicadas();
    }
    time->AddEventListener(this, ["OnNight", "StartCicadas"]);
    time->AddEventListener(this, ["OnDaybreak", "StopCicadas"]);
}

public func StartCicadas() {
	SetAction("Cicadas");
	SetPhase(Random(20));
}

public func StopCicadas() {
	SetAction("Idle");
}

private func Cicadas() { if(!Random(20)) Sound("Cicada*", true); }