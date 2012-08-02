/*  Script: Pointer
	Provides a pointer highlighting an object for a player. */

#strict 2

static const PT0D_Distance = 200;
static const PT0D_Duration = 255;

local alphaEase;

func Initialize() {
	SetVisibility(VIS_None, this);
	alphaEase = CreateEaseFunction("circle-out", PT0D_Duration);
	return true;
}

/*  Constructor: CreatePointer
	Creates a pointer.

	Parameters:
	plr      - The player who will see the pointer.
	target   - The object the pointer will point at.
	color    - Pointer's color modulation.
	message  - A message shown above the pointer.

	Returns:
	The created pointer. */
global func CreatePointer(int plr, object target, int color, string message) {
	if(!target) return false;
	if(!color) color = GetPlrColorDw(plr);
	if(!message) message = "";

	var pointer = CreateObject(PT0D, 0, 0, plr);
	pointer->SetClrModulation(color);
	AddEffect("Pointing", pointer, 100, 1, pointer, 0, plr, target, message);
	return pointer;
}

/*  Function: PointOut
	Creates a pointer for every player pointing to the calling object.

	Parameters:
	color   - Pointer's color modulation.
	message - A message shown above the pointer. */
global func PointOut(int color, string message) {
	for(var count = GetPlayerCount(), i = 0; i < count; i++) {
		var p = GetPlayerByIndex(i);
		CreatePointer(p, this, color, message);
	}
	return count;
}

func FxPointingStart(object target, int index, int temp, int plr, object obj, string msg) {
	SetVisibility(VIS_Owner, target);
	CreateParticle("PSpark", 0, 0, 0, 0, 500, GetClrModulation(), this, true);
	EffectVar(0, target, index) = plr;
	EffectVar(1, target, index) = obj;
	EffectVar(2, target, index) = msg;
	return true;
}

func FxPointingTimer(object target, int index, int time) {
	if(time > PT0D_Duration) {
		RemoveObject(target);
		return -1;
	}

	var clonk = GetCursor(EffectVar(0, target, index));
	var obj = EffectVar(1, target, index);
	var msg = EffectVar(2, target, index);

	// Zeiger hat kein Ziel mehr? Löschen.
	if(!obj) {
		RemoveObject(target);
		return -1;
	}

	var ang = Angle(GetX(clonk), GetY(clonk), GetX(obj), GetY(obj)) - 90;
	var dst = Min(PT0D_Distance, Distance(GetX(clonk), GetY(clonk), GetX(obj), GetY(obj)) / 2);

	SetR(ang + 90, target);
	SetPosition(GetX(clonk) + Cos(ang, dst), GetY(clonk) + Sin(ang, dst), target);

	var r, g, b, a;
	SplitRGBaValue(GetClrModulation(target), r, g, b, a);

	a = EvalEase(alphaEase, Min(254, time));
	SetClrModulation(RGBa(r, g, b, a), target);
	Message("<c %x>%s</c>", target, RGBa(r, g, b, 255 - a), msg);

	return true;
}
