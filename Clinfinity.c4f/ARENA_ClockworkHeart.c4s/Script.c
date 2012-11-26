
#strict 2

protected func Initialize() {
  //heart
  CreateObject(HART,270,710, NO_OWNER);
  
  CreateObject(FOG_,400,850);

	PLTF->CreatePlatform(400, 280, NO_OWNER);
	PLTF->CreatePlatform(535, 335, NO_OWNER);

	var draft;
	draft = CreateObject(DRFT, 120, 380, NO_OWNER);
	draft->SetPermanent();
	draft->SetR(-5);

	draft = CreateObject(DRFT, 60, 180, NO_OWNER);
	draft->SetPermanent();
	draft->SetR(60);

	draft = CreateObject(DRFT, 750, 350, NO_OWNER);
	draft->SetPermanent();
	draft->SetR(-10);

	draft = CreateObject(DRFT, 750, 180, NO_OWNER);
	draft->SetPermanent();
	draft->SetR(-60);
	
	//Remove the levers -> ControlLever.c in System.c4g still needed?
	RemoveAll(COLV);
}

protected func InitializePlayer(int plr) { return JoinPlayer(plr); }

private func JoinPlayer(int plr) {
	var clonk = GetCrew(plr);
	DoEnergy(100000, clonk);
	SetRandomPosition(clonk);
	CreateContents(YOYO, clonk);
	CreateContents(SHIT, clonk);
	CreateContents(SWOR, clonk);
	return true;
}

private func SetRandomPosition(object clonk) {
	var spawnArea = Random(3);
	if(spawnArea == 0) {
		SetPosition(RandomX(150, 495), 389, clonk);
	} else if(spawnArea == 1) {
		SetPosition(RandomX(645, 740), 359, clonk);
	} else {
		SetPosition(RandomX(230, 315), 165, clonk);
	}
}

/* Neubeitritt */

public func OnClonkDeath(object oldClonk) {
	var plr = oldClonk -> GetOwner();
	if(GetPlayerType(plr)) {
		var clnk = CreateObject(AVTR, 0, 0, plr);
		clnk -> GrabObjectInfo(oldClonk);
		SelectCrew(plr, clnk, 1);
		Log(RndRelaunchMsg(), GetPlayerName(plr));
		return JoinPlayer(plr);
	}
}

private func RndRelaunchMsg() {
	var n = Random(11);
	if (!n  ) return "$MsgDeath1$";
	if (!--n) return "$MsgDeath2$";
	if (!--n) return "$MsgDeath3$";
	if (!--n) return "$MsgDeath4$";
	if (!--n) return "$MsgDeath5$";
	if (!--n) return "$MsgDeath6$";
	if (!--n) return "$MsgDeath7$";
	if (!--n) return "$MsgDeath8$";
	if (!--n) return "$MsgDeath9$";
	if (!--n) return "$MsgDeath10$";
	return "$MsgDeath11$";
}

public func IsDraftPermanent() { return true; }

global func FxSkyLightningTimer(object target, int effectNumber, int effectTime) {
	var brightness = 255 - 15 * effectTime;
	SetSkyColourModulation(RGB(brightness, brightness, brightness), false, 7);
	if(brightness == 0) {
		return FX_Execute_Kill;
	} else {
		return FX_OK;
	}
}

global func FxSkyLightningEffect(string newEffectName, object target, int effectNumber, int newEffectNumber, var1, var2, var3, var4) {
	if(newEffectName == "SkyLightning") {
		return FX_Effect_Deny;
	}
}
