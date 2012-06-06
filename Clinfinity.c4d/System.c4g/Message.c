/*  Script: Message.c
	Provides functions related to messages. */
#strict 2

/*  Function: FadingMessage
	Creates a moving and fading message.

	These messages will be queued if multiple are created at the same time.

	Parameters:
	message - The content of the message.
	x - Start position (relative to normal message position)
	y - Start position
	tx - End position
	ty - End position
	duration - Duration the message is shown.
	color - Color of the message text.
	ease - Easing function, see <CreateEaseFunction>. */
global func FadingMessage(string message, int x, int y, int tx, int ty, int duration, int color, string ease) {
	var timer = 1;
	// wait if there's another message
	var i = 0, existing;
	// find running effect
	while((existing = GetEffect("FadingMessage", this, i++)) && !GetEffect(0, this, existing, 3));
	if(existing)
		timer = 0;
	var effectNum = AddEffect("FadingMessage", this, 10, timer, this, 0);
	EffectVar(0, this, effectNum) = message;
	EffectVar(1, this, effectNum) = x;
	EffectVar(2, this, effectNum) = y;
	EffectVar(3, this, effectNum) = tx;
	EffectVar(4, this, effectNum) = ty;
	EffectVar(5, this, effectNum) = duration;
	EffectVar(6, this, effectNum) = color;
	EffectVar(7, this, effectNum) = CreateEaseFunction(ease || "cubic-out", duration);
	if(existing) {
		var waiting = EffectVar(8, this, existing) || [];
		PushBack(effectNum, waiting);
		EffectVar(8, this, existing) = waiting;
	}
}

global func FxFadingMessageTimer(object target, int effectNum, int effectTime) {
	var duration = EffectVar(5, target, effectNum);
	var ease = EffectVar(7, target, effectNum);
	var easedVal = EvalEase(ease, effectTime);
	var color = SetRGBaValue(EffectVar(6, target, effectNum), ChangeRange(easedVal, 0, duration, 0, 255));
	var x = EffectVar(1, target, effectNum), y = EffectVar(2, target, effectNum),
	    tx = EffectVar(3, target, effectNum), ty = EffectVar(4, target, effectNum);
	if(x != tx)
		x = ChangeRange(easedVal, 0, duration, x, tx);
	if(y != ty)
		y = ChangeRange(easedVal, 0, duration, y, ty);
	CustomMessage(EffectVar(0, target, effectNum), this, NO_OWNER, x, y, color);
	if(effectTime > duration)
		return -1;
}

global func FxFadingMessageStop(object target, int effectNum, int reason, bool temp) {
	if(temp) return;
	var waiting = EffectVar(8, target, effectNum);
	if(waiting) {
		var effect = PopElement(waiting);
		ChangeEffect(0, this, effect, "FadingMessage", 1);
		if(GetLength(waiting))
			EffectVar(8, target, effect) = waiting;
	}
}

/*  Function: MatSysMessage
	Creates a fading message showing a change in MatSys storage.

	This function is automatically called by <MatSysDoFill> and <MatSysDoTeamFill>.
	Objects that don't want these messages to show can define a function NoMatSysMessages(id mat)
	that returns _true_ when no message should be displayed.

	The position of the message can be altered by defining a function MatSysMessagePosition(id mat)
	that returns an array [x, y].

	Parameters:
	change - The amount by which the storage changed.
	mat    - The id of the material whose storage changed. */
global func MatSysMessage(int change, id mat) {
	if(change == 0 || !this || this->~NoMatSysMessages(mat))
		return;
	var pos = this->~MatSysMessagePosition(mat) || [0, 0];
	var msg = Format("{{%i}}%s", mat, IntToStr(change, true));
	var color;
	if(change < 0)
		color = RGB(255, 0, 0);
	else
		color = RGB(0, 255, 0);
	FadingMessage(msg, pos[0], pos[1], pos[0], pos[1] - 50, 75, color, "circle");
}
