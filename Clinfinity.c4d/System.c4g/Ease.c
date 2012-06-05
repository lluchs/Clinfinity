/*  Script: Ease.c
	Provides some universal easing functions. */
#strict 2

/*  Function: CreateEaseFunction
	Creates an easing function for later use with <EvalEase>.

	Possible easing functions are:
	 - poly(k): raises t to the specified power k (last parameter)
	 - quad: equivalent to poly(2)
	 - cubic: equivalent to poly(3)
	 - sin: applies the trigonometric function *sin*
	 - circle: a quarter circle

	Possible modes are:
	 - in: the identity function
	 - out: reverses the easing direction
	 - in-out: copies and mirrors the easing function from [0, max/2] and [max/2, max]

	Parameters:
	function - The easing function (see above), and an optional mode (see above) separated by a minus. Example: "cubic-in-out"
	max      - The maximum for t (see <EvalEase>).

	Returns:
	An easing function. */
global func CreateEaseFunction(string function, int max) {
	var i = IndexOf(function, "-");
	var f, mode;
	if(i >= 0) {
		f = Substring(function, 0, i);
		mode = Substring(function, i + 1);
	} else {
		f = function;
		mode = "in";
	}

	// determine function
	var exe;
	if(f == "quad")
		exe = "EasePoly(2, %d, %d)";
	else if(f == "cubic")
		exe = "EasePoly(3, %d, %d)";
	else if(f == "poly")
		exe = Format("EasePoly(%d, %%d, %%d)", Par(3));
	else if(f == "sin")
		exe = "EaseSin(%d, %d)";
	else if(f == "circle")
		exe = "EaseCircle(%d, %d)";
	else
		FatalError(Format("CreateEaseFunction: Invalid easing function %s!", f));
	
	// check if mode is valid
	if(mode != "in" && mode != "out" && mode != "in-out")
		FatalError(Format("CreateEaseFunction: Invalid easing mode %s!", mode));

	return [exe, mode, max];
}

/*  Function: EvalEase
	Evaluates an easing function.

	Parameters:
	f - The easing function, created by <CreateEaseFunction>.
	t - A number 0 <= t <= max

	Returns:
	The new value. */
global func EvalEase(array f, int t) {
	var exe = f[0], mode = f[1], max = f[2];
	t = BoundBy(t, 0, max);

	// execute depending on mode
	if(mode == "in")
		exe = Format(exe, t, max);
	else if(mode == "out")
		exe = Format("%d - (%s)", max, Format(exe, max - t, max));
	else if(mode == "in-out") {
		if(t < max / 2)
			exe = Format(exe, 2 * t, max);
		else
			exe = Format("%d - (%s)", 2 * max, Format(exe, 2 * max - 2 * t, max));
		exe = Format("(%s) / 2", exe);
	}

	return eval(exe);
}

/*  Function: Ease
	Interpolates a number 0 <= t <= max according to the given function.

	For performance reasons, you should probably use <CreateEaseFunction> and
	<EvalEase>, caching the function.

	Parameters:
	function - The easing function, consisting of a function and a modifier.
	t        - A number 0 <= t <= max
	max      - The maximum

	Returns:
	The new value. */
global func Ease(string function, int t, int max) {
	var exe = CreateEaseFunction(function, max);
	return EvalEase(exe, t);
}

global func EasePoly(int e, int t, int max) {
	return t ** e / max ** (e - 1);
}

global func EaseSin(int t, int max) {
	return max - Cos(t * 90 / max, max);
}

global func EaseCircle(int t, int max) {
	return max - Sqrt(max*max - t*t);
}

global func TestEasing(string f) {
	var max = 500;
	var easeFunction = CreateEaseFunction(f, max);
	for(var x = 0; x < max; x++) {
		var y = EvalEase(easeFunction, x);
		CreateParticle("PSpark", x, y, 0, 0, 30, RGB(255, 255, 255));
	}
}
