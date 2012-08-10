#strict 2

local columns;

protected func Initialize() {
	SetVisibility(VIS_Owner);

	SetCharge(0);
	Off();

	columns = GetActMapVal("Length", "Charge");
}

protected func SetCharge(int charge) {
	SetAction("Charge");

	var phase = charge * (columns - 1) / 100;
	SetPhase(phase);
}

protected func On() {
	SetStillOverlayAction("On", 1);
}

protected func Off() {
	SetStillOverlayAction("Off", 1);
}
