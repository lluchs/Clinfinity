/* Frigate-Cannon */
#strict 2

local phases;

protected func Initialize() {
	SetAction("Turn");
	phases = GetActMapVal("Length", "Turn");
}

private func TurnLeft() {
	SetPhase(Max(GetPhase() - 1, 0));
}

private func TurnRight() {
	SetPhase(Min(GetPhase() + 1, phases - 1));
}

protected func ControlLeft(object clonk) {
	if(Hostile(GetOwner(), clonk->GetOwner()))
		return;
	TurnLeft();
	return true;
}

protected func ControlRight(object clonk) {
	if(Hostile(GetOwner(), clonk->GetOwner()))
		return;
	TurnRight();
	return true;
}

protected func ControlThrow(object clonk) {
	if(Hostile(GetOwner(), clonk->GetOwner()))
		return;
	Shoot();
	return true;
}

private func GetAngle() {
	var angles = [75, 71, 70, 64, 37, 32, 18, 8, 4, 1];
	var phase = GetPhase();
	var f = -1;
	if(phase >= phases / 2) {
		phase = phases - phase - 1;
		f = 1;
	}
	return f*angles[phase];
}

private func GetAmmo() {
	return CreateContents(FLNT);
}

private func GetPower() { return 20; }
private func GetBarrelLength() { return 25; }
private func GetCooldown() { return 50; }
private func CannonSound() { return "Blast3"; }

local cooldown;
private func StartCooldown() {
	cooldown = true;
	ScheduleCall(this, "FinishCooldown", GetCooldown());
}

protected func FinishCooldown() {
	cooldown = false;
}

private func Shoot() {
	if(cooldown)
		return;
	var ammo = GetAmmo();
	if(!ammo)
		return;
	var angle = GetAngle();
	// Randomize the angle a little bit.
	angle += RandomX(-5, 5);
	var power = GetPower();
	
	var x = Sin(angle, GetBarrelLength()),
		y = -Cos(angle, GetBarrelLength()),
		xdir = Sin(angle, power),
		ydir = -Cos(angle, power);
	
	Exit(ammo, x, y, 0, xdir, ydir);
	StartCooldown();
	return ammo;
}
