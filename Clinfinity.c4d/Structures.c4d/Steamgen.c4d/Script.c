/*-- Steamgen --*/

#strict 2

#include L_CP
#include B_70

static const RSMG_ProductionTime = 200;
static const RSMG_ProductionRate = 50;

public func NoDemolition() { return true; }

public func CaptureTime() { return 100; }

private func Captured() {
	StartProduction();
	return inherited(...);
}

private func StartProduction() {
	ClearScheduleCall(this, "ProduceSteam");
	ScheduleCall(this, "ProduceSteam", RSMG_ProductionTime);
}

protected func ProduceSteam() {
	MatSysDoTeamFill(RSMG_ProductionRate + RandomX(-10, 10), GetOwner(), STEM);
	StartProduction();
}
