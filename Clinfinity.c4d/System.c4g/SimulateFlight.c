/*  Script: SimulateFlight.c
	Provides helper functions for flight simulation, akin to SimFlight(). */
#strict 2

/*  Function: SimulateFlight
	Simulates the flight of an object for the specified time.

	Parameters:
	time - the time in frames

	Returns:
	An array [x, y] of the expected new coordinates. */
global func SimulateFlight(int time) {
	var x = GetX() * 10, y = GetY() * 10,
	    xdir = GetXDir(), ydir = GetYDir(),
		gravity = GetGravity() / 50;
	for(var i = 0; i < time; i++) {
		ydir += gravity;
		x += xdir;
		y += ydir;

		// Assuming the object stops on solid surfaces.
		if(GBackSolid(AbsX(x/10), AbsY(y/10)))
			break;
	}
	return [x / 10, y / 10];
}

/*  Function: ShowFlightCurve
	Shows a flight curve as calculated by SimulateFlight.

	Parameters:
	enable - true to enable, false to disable */
global func ShowFlightCurve(bool enable) {
	if(enable) {
		AddEffect("FlightCurve", this, 10, 10);
	} else {
		RemoveEffect("FlightCurve", this);
	}
}

global func FxFlightCurveTimer(object target, int effectNum, int effectTime) {
	for(var i = 0; i < 10; i++) {
		var xy = target->SimulateFlight(i * 5);
		CreateParticle("PSpark", xy[0], xy[1], 0, 0, 50, RGB(255, 0, 0));
	}
}
