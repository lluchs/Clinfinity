
#strict 2

protected func Initialize() {
	ScheduleCall(this, "Initializing", 1);
}

private func Initializing() {
	var ruleTypeCount = ObjectCount2(Find_ID(GetID()));
	var others = FindObjects(Find_ID(GetID()), Find_Exclude(this));
	for(var other in others) {
		other->RemoveObject();
	}
	Initialized(ruleTypeCount);
}

private func Initialized(int ruleTypeCount) {}