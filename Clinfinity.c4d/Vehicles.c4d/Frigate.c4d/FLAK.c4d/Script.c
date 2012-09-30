#strict 2

#include FRCN

private func GetBarrelLength() { return 10; }

private func GetAmmo() {
	return CreateContents(FLNT);
}

/* Fixes the shooting angle. */
private func GetPhase(bool real) {
	if(real)
		return inherited();
	else
		return phases - 1 - inherited();
}

private func TurnLeft() {
	SetPhase(Min(GetPhase(true) + 1, phases - 1));
}

private func TurnRight() {
	SetPhase(Max(GetPhase(true) - 1, 0));
}

