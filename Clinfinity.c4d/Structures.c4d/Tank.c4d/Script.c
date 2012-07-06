/*-- Steam Tank --*/

#strict 2

#include STBO
#include L_DC
#include L_SS

static steamTrendWarnEffects;

// number of animation phases
static const STMT_Phases = 10;
// amount needed to respawn one clonk
static const STMT_RespawnAmount = 100;

public func MaxFill() { return 1500; }

public func MaxDamage() { return 50; }

public func DamageGraphics() { return 2; }

// all previous changes in fill level
local changes;

protected func Initialize() {
	changes = [];
	SetAction("Tank");

	// steam trend warning
	if(!steamTrendWarnEffects)
		steamTrendWarnEffects = [];
	var team = GetPlayerTeam(GetOwner());
	if(!steamTrendWarnEffects[team]) {
		steamTrendWarnEffects[team] = AddEffect("SteamTrendWarning", 0, 1, 100);
		EffectVar(0, 0, steamTrendWarnEffects[team]) = team;
	}
	SetAlarmLamp(false);
	SetObjDrawTransform(1000, 0, 3500, 0, 1000, -5000, this, 1);

	// create dummy effect if this is the first Akku
	// this ensures that when the last Akku gets destroyed, the display is reset to 0
	if(!GetEffect("MatSysSTEM"))
		AddEffect("MatSysSTEM", 0, 1, 0);
	AddEffect("MatSysSTEM", 0, 1, 0, this);
	_inherited();
}

// manages delay for steam generation
local generate;
// bubbling?
local bubbling;

protected func Steam() {
	if(GetFill() > MaxFill() * 2 / 3) {
		CreateParticle("Smoke", 30, -18, 0, 0, 50, RGBa(255, 255, 255, 0));
		if(!bubbling) {
			Sound("steam_exhaust", 0, 0, 0, 0, 1);
			bubbling = true;
		}
	} else if(bubbling) {
		Sound("steam_exhaust", 0, 0, 0, 0, -1);
		bubbling = false;
	}
	// generation
	if(!generate--) {
		var change = DoFill(125 + RandomX(-10, 10));
		MatSysMessage(change, STEM);
		generate = RandomX(350, 450);
	}
	// respawn
	var clonk, i = 0;
	while(clonk = Contents(i++))
		StartRespawn(clonk);
}

private func StartRespawn(object clonk) {
	if(GetEffect("Respawn", clonk))
		return;
	var effectNum = AddEffect("Respawn", clonk, 1, KOTH_FPS, this);
	EffectVar(0, clonk, effectNum) = GetRespawnTime(GetPlayerTeam(clonk->GetOwner()));
}

protected func FxRespawnTimer(object target, int effectNum, int effectTime) {
	var timeToWait = EffectVar(0, target, effectNum);
	Message("@Respawn in %d...", target, Max(0, timeToWait - effectTime) / KOTH_FPS);
	if(effectTime >= timeToWait && MatSysGetTeamFill(GetOwner(), STEM) >= STMT_RespawnAmount) {
		MatSysDoTeamFill(-STMT_RespawnAmount, GetOwner(), STEM);
		target->Exit();
		var plr = target->GetOwner();
		if(!GetCursor(plr))
			SetCursor(plr, target);
		Message("", target);
		return -1;
	}
}

private func OnFillChange(key, int change) {
	UpdateDisplay();
	PushBack([FrameCounter(), change], changes);
}

private func UpdateDisplay() {
	SetPhase(ChangeRange(GetFill(), 0, MaxFill() - 1, STMT_Phases - 1, 0));
	return 1;
}

public func GetChanges(int frames) {
	var min = FrameCounter() - frames, result = [];
	for(var i = GetLength(changes) - 1; i && changes[i][0] >= frames; i--)
		PushBack(changes[i][1], result);
	return result;
}

public func GlobalSteamChanges(int frames, bool average) {
	var result = 0, num = 0;
	for(var tank in FindObjects(Find_ID(GetID()), Find_Allied(GetOwner()))) {
		for(var change in tank->GetChanges(frames)) {
			result += change;
			num++;
		}
	}
	if(average)
		return result / num;
	else
		return result;
}

public func ShowChanges() {
	Message("@Trend: %d", 0, GlobalSteamChanges(750));
	ScheduleCall(this, "ShowChanges", 25);
}

// Steuerung
  
protected func ControlUp() {
	Message("$TxtStored$", this, GetFill());
}

protected func ControlDig(object clonk) {
	if(Hostile(GetOwner(), clonk->GetOwner())) {
		clonk->Sound("CommandFailure1");
		return;
	}
	var physical = clonk->GetPhysical("Energy");
	var health = 100 - clonk->GetEnergy() * 100000 / physical;
	clonk->DoEnergy(-MatSysDoTeamFill(-health, clonk->GetOwner(), STEM) * physical / 100000);
}

/* Material System fill level */
public func FxMatSysSTEMUpdate(object target, int effectNum, int plr) {
	if(!this || GetOwner() != plr)
		return 0;
	return GetFill();
}

public func FxMatSysSTEMChange(object target, int effectNum, int plr, int change) {
	if(!this || GetOwner() != plr)
		return 0;
	return DoFill(change);
}

/* Steam trend warning */

public func SetAlarmLamp(bool on) {
	var action;
	if(on)
		action = "On";
	else
		action = "Off";
	SetGraphics(0, this, ALRM, 1, GFXOV_MODE_Action, action);
	return true;
}

global func FxSteamTrendWarningTimer(object target, int effectNum, int effectTime) {
	var team = EffectVar(0, target, effectNum), players = GetPlayersByTeam(team);
	var tank = FindObject2(Find_ID(STMT), Find_Allied(players[0]));
	var warning = EffectVar(1, target, effectNum), next;
	if(tank && tank->GlobalSteamChanges(750, true) < 0)
		next = true;
	else
		next = false;
	if(!warning && next || warning && !next) {
		for(var plr in players)
			Sound("Warning_blowup", true, 0, 75, plr + 1, next*2 - 1);
		for(var t in FindObjects(Find_ID(STMT), Find_Allied(players[0])))
			t->SetAlarmLamp(next);
	}
	EffectVar(1, target, effectNum) = next;
}

