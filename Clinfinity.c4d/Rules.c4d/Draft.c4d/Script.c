#strict 2

local draftWidth, draftHeight, draftDistance, draftParticleColour;
local minDraftDuration, maxDraftDuration;
local maxGliderSpeedUpwards, gliderAcceleration; // 1/10 pixels per frame

protected func Activate(byPlayer) {
	MessageWindow(GetDesc(), byPlayer);
	return 1;
}

protected func Initialize() {
	draftWidth = 50;
	draftHeight = 150;
	draftDistance = Sqrt(Pow(draftWidth, 2) + Pow(draftHeight, 2)) / 2;
	draftParticleColour = RGBa(255, 255, 255, 210);
	minDraftDuration = 1050;
	maxDraftDuration = 2100;
	maxGliderSpeedUpwards = 60;
	gliderAcceleration = 5;
	SetRandomPosition();
	Draft();
}

protected func Draft() {
	var draftDirection = GetR();
	if( !Random(2) ) {
		var x = RandomX(-draftWidth / 2, draftWidth / 2),
		    y = RandomX(-3, 3),
		    speed = RandomX(-draftHeight / 6,-draftHeight / 4);
		// rotation
		Rotate(draftDirection, x, y);
		CreateParticle("WindSpark", x, y, -Sin(draftDirection, speed), Cos(draftDirection, speed), 40, draftParticleColour);
	}
	var x = 0, y = -draftHeight / 2;
	Rotate(draftDirection, x, y);
	var gliders = FindObjects( Find_And( Find_NoContainer(), Find_Distance(draftDistance, x, y), Find_Category(C4D_Living), Find_Func("IsGliding") ) );
	for( var glider in gliders ) {
		var gx = glider->GetX() - GetX(), gy = glider->GetY() - GetY();
		Rotate(-draftDirection, gx, gy);
		if(Inside(gx, -draftWidth / 2, draftWidth / 2) && Inside(gy, -draftHeight, 0)) {
			var xDir = glider->GetXDir(0, 100), yDir = glider->GetYDir(0, 100);
			glider->SetXDir(xDir + Sin(draftDirection, gliderAcceleration * 10), 0, 100);
			//Message("X: %d, Y: %d", this, Sin(draftDirection, gliderAcceleration * 10), -Cos(draftDirection, gliderAcceleration * 10));
			if(yDir > -maxGliderSpeedUpwards * 10) {
				glider->SetYDir(yDir - Cos(draftDirection, gliderAcceleration * 10), 0, 100);
			}
		}
	}
	ScheduleCall(this, "Draft", 1);
}

protected func SetRandomPosition() {
	var x = Random(LandscapeWidth());
	var y = Random(LandscapeHeight());
	var otherDraft = FindObject2( Find_And( Find_ID(GetID()), Find_InRect(AbsX(x) - draftWidth / 2, AbsY(y) - draftHeight, draftWidth, draftHeight) ) );
	if(otherDraft != 0) {
		// Anderes Aufwind-Objekt schon an der Stelle: Nächstes Frame neue Position suchen.
		ScheduleCall(this, "SetRandomPosition", 1);
	} else {
		if( !GameCallEx("IsDraftPermanent") ) {
			// Kein anderes Aufwind-Objekt: Neue Position erst in einer Weile suchen.
			ScheduleCall(this, "SetRandomPosition", RandomX(minDraftDuration, maxDraftDuration));
		}
		SetPosition(x, y);
	}
}
