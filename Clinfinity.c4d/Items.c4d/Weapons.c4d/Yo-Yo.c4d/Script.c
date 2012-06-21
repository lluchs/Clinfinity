#strict 2

protected func Departure(object container) {
	// If the yo-yo was thrown by a Clonk, act as weapon.
	if((container->GetOCF() & OCF_CrewMember) != 0) {
		// Set speed to fly lower, because it's supposed to hit enemies.
		SetXDir(GetXDir() * 3);
		SetYDir(-5);
		// Attach yo-yo line to Clonk.
	}
}

protected func Hit(/* TODO: Parameters? */) {
	// TODO
	// If working as a weapon: Remove vertex, return to sender
	// TODO: While returning, perhaps change the category to vehicle so we don't hit the thrower
}

// TODO: Reject entrance to anyone but the throwing clonk.

// TODO: Get slower on x axis over time, and pick up speed again when returning to thrower?