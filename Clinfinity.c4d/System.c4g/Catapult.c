/*-- Catapult Trajectory --*/

#strict 2

#appendto CATA

protected func Grabbed(object clonk, bool grab) {
	SetOwner(clonk->GetOwner());
	ShowTrajectory(grab);
}

protected func GrabLost() {
	ShowTrajectory(false);
}

private func AimUp() {
	inherited(...);
	ShowTrajectory(true);
}

private func AimDown() {
	inherited(...);
	ShowTrajectory(true);
}

private func AimUpdate() {
	inherited(...);
	ShowTrajectory(true);
}

private func ShowTrajectory(bool show) {
	var phase = GetPhase();
	if(show && phase) {
		var x = (GetDir()*2-1)*12;
		var y = -10;
		var xDir =  +8 * (GetDir() * 2 - 1) * phase / 6;
		var yDir = -12 * phase / 6;
		AddTrajectory(this, GetX() + x, GetY() + y, xDir*10, yDir*10);
		if(!GetEffect("UpdateTrajectory", this))
			AddEffect("UpdateTrajectory", this, 1, 5, this);
	} else {
		RemoveTrajectory(this);
		RemoveEffect("UpdateTrajectory", this);
	}
}

local oldDir;

protected func FxUpdateTrajectoryTimer(object target, int effectNum, int effectTime) {
	if(GetDir() != oldDir) {
		ShowTrajectory(true);
		oldDir = GetDir();
	}
	if(effectTime % 10 == 0 && !FindObject2(Find_OCF(OCF_CrewMember), Find_Action("Push"), Find_ActionTarget(this))) {
		ShowTrajectory(false);
	}
}
