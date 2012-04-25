#strict 2

local draftWidth, draftHeight, draftParticleColour;
local maxGliderSpeedUpwards, gliderAcceleration;

protected func Activate(byPlayer) {
	MessageWindow(GetDesc(), byPlayer);
	return 1;
}

protected func Initialize() {
	draftWidth = 50;
	draftHeight = 150;
	draftParticleColour = RGBa(255, 255, 255, 210);
	maxGliderSpeedUpwards = -60;
	gliderAcceleration = -5;
	SetRandomPosition();
	Updraft();
}

protected func Updraft() {
	CreateParticle("NoGravSpark", RandomX(-draftWidth / 2, draftWidth / 2), RandomX(-3, 3), 0, RandomX(-draftHeight / 6,-draftHeight / 4), 40, draftParticleColour);
	var gliders = FindObjects( Find_And( Find_NoContainer(), Find_InRect(-draftWidth / 2, -draftHeight, draftWidth, draftHeight), Find_Category(C4D_Living), Find_Func("IsGliding") ) );
	for( var glider in gliders ) {
		var yDir = glider->GetYDir();
		if( yDir > maxGliderSpeedUpwards ) {
			glider->SetYDir( yDir + gliderAcceleration );
		}
	}
	ScheduleCall(this, "Updraft", 1);
}

protected func SetRandomPosition() {
	var x = Random(LandscapeWidth());
	var y = Random(LandscapeHeight());
	var otherUpdraft = FindObject2( Find_And( Find_ID(GetID()), Find_InRect(AbsX(x) - draftWidth / 2, AbsY(y) - draftHeight, draftWidth, draftHeight) ) );
	if(otherUpdraft != 0) {
		// Anderes Aufwind-Objekt schon an der Stelle: Nächstes Frame neue Position suchen.
		ScheduleCall(this, "SetRandomPosition", 1);
	} else {
		// Kein anderes Aufwind-Objekt: Neue Position erst in einer Weile suchen.
		ScheduleCall(this, "SetRandomPosition", RandomX(350, 700));
		SetPosition(x, y);
	}
}