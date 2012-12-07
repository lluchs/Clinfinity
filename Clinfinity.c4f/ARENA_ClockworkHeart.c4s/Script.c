
#strict 2

protected func Initialize() {
  //heart
  CreateObject(HART, 270, 710, NO_OWNER);
  
  //Foreground
  CreateObject(GRD1, 400, 805, NO_OWNER);
  
  CreateObject(FOG_,400,850);
	
	var platform;
	platform = PLTF->CreatePlatform(460, 255, NO_OWNER);
	platform->SetColorDw(RGBa(55, 43, 41, 0));
	
	platform = PLTF->CreatePlatform(560, 290, NO_OWNER);
	platform->SetColorDw(RGBa(55, 43, 41, 0));
	
	platform = PLTF->CreatePlatform(555, 390, NO_OWNER);
	platform->SetColorDw(RGBa(55, 43, 41, 0));

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
	
	// Remove the levers
	for(var platform in FindObjects(Find_ID(PLTF))) {
		var lever = FindObject2(Find_ID(COLV), Find_ActionTarget(platform));
		var mediator = platform->LocalN("controlMediator");
		mediator->RemoveMovementEventListener(lever);
		lever->RemoveObject();
	}

	// Spawners
	CreateObject(_ISP, 245, 210, NO_OWNER);
	CreateObject(_ISP, 307, 210, NO_OWNER);
	CreateObject(_ISP, 370, 210, NO_OWNER);
	CreateObject(_ISP, 460, 248, NO_OWNER);
	CreateObject(_ISP, 560, 282, NO_OWNER);
	CreateObject(_ISP, 191, 368, NO_OWNER);
	CreateObject(_ISP, 82, 408, NO_OWNER);
	
	CreateObject(VINE,626,392,-1);
	CreateObject(VINE,687,396,-1);
	CreateObject(VINE,757,386,-1);
	CreateObject(VINE,638,433,-1);
	CreateObject(VINE,670,440,-1);
	CreateObject(VINE,705,420,-1);
	CreateObject(VINE,719,445,-1);
	CreateObject(VINE,741,413,-1);
	CreateObject(VINE,758,400,-1);
	CreateObject(VINE,753,408,-1);
	CreateObject(VINE,635,450,-1);
	CreateObject(VINE,646,459,-1);
	CreateObject(VINE,673,461,-1);
	CreateObject(VINE,695,460,-1);
	CreateObject(VINE,676,471,-1);
	CreateObject(VINE,726,486,-1);
	CreateObject(VINE,733,462,-1);
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
	var spawnArea = Random(5);
	if(spawnArea == 0) {
		SetPosition(RandomX(260, 460), 399, clonk);
	} else if(spawnArea == 1) {
		SetPosition(RandomX(540, 580), 274, clonk);
	} else if(spawnArea == 2) {
		SetPosition(RandomX(250, 360), 201, clonk);
	} else if(spawnArea == 3) {
		SetPosition(RandomX(535, 575), 374, clonk);
	} else {
		SetPosition(RandomX(170, 212), 360, clonk);
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
