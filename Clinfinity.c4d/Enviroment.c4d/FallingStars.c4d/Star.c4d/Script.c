/*-- Sternschnuppe --*/

#strict 2

local falling;

public func FadeIn() {
	falling = true;
	PeriodicFalling();
}

public func FadeOut() {
	falling = false;
}

protected func PeriodicFalling() {
	if(!falling)
		return;
	if(Random(2))
		Fall();
	ScheduleCall(this, "PeriodicFalling", RandomX(100, 250));
}

public func Fall() {
	var angle = Random(360), range = RandomX(100, 200);
	AddEffect("FallingStar", this, 130, 1, 0, GetID(), angle, GetX(), GetY(), range);
	SetPosition(Random(LandscapeWidth()), Random(LandscapeHeight()));
	return 1;
}

// EffectVars: 0 - Angle
//             1 - Distance from start (0...MaxRange)
//             2 - Start:X
//             3 - Start:Y
//             4 - Maximum range

protected func FxFallingStarStart(object target, int number, int temp, int angle, int x, int y, int range) {
	if(temp) return;

	// Direction
	EffectVar(0, target, number) = angle;
	EffectVar(1, target, number) = 20;

	// Position
	EffectVar(2, target, number) = x;
	EffectVar(3, target, number) = y;

	// Range
	EffectVar(4, target, number) = range;
}

protected func FxFallingStarTimer(object target, int number, int time) {
	var x, y, angle, size, step;
	var pos = EffectVar(1, target, number);
	var range = EffectVar(4, target, number);

	step = -pos/15+range/20;

	if(pos >= range || step == 0)
		return -1;

	x = EffectVar(2, target, number);
	y = EffectVar(3, target, number);

	angle = EffectVar(0, target, number);
	size = step+pos/10;

	x += Sin(angle, pos);
	y -= Cos(angle, pos);

	var clr = pos * 255 / range;
	CreateParticle("PSpark", x, y, 0, 0, size*2, RGBa(200, 200, 255, clr*127/255), target, true);

	EffectVar(1, target, number) += step;
}

