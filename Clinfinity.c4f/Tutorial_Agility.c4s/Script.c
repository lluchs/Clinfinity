/*-- Agility Tutorial --*/

#strict 2

// Positions for Relaunch
static iPlrX, iPlrY, iCounter;

func Initialize() {
	// Decoration
	CreateObject(HUT6, 36, 521, NO_OWNER);
	CreateObject(MUSH, 50, 235, NO_OWNER);
	CreateObject(MUSH, 80, 235, NO_OWNER);
	CreateObject(MUSH, 83, 235, NO_OWNER);
	CreateObject(TRE1, 44, 233, NO_OWNER);

	ScriptGo(true);
}

func InitializePlayer(int plr) {
	GetCrew(plr)->SetPosition(30, 220);
	// Message positioning
	//SetPlrShowControlPos(plr, SHOWCTRLPOS_TopLeft);
	SetTutorialMessagePos(MSG_Top | MSG_Left | MSG_WidthRel | MSG_XRel, 50, 50, 30);

	SavePosition();
	iCounter = 0;
}

func Script5() {
	TutorialMessage(Format("$TutHelloWorld$", GetPlayerName()));
}

func Script15() {
	//SetPlrShowControl(0, "____4_678_          __________");
	Sound("Ding");
	TutorialMessage("$TutDoubleJump$");
}

func Script35() {
	TutorialMessage("$TutDoubleJumpAdvise$");
  SetArrow(180, 60);
  Sound("Ding");
}

func Script40() {
	if(!FindObject(AVTR, 160, 50 , 30, 20)) {
		goto(39);
		return;
	}
	SavePosition();
	TutorialMessage("$TutNice$");
	Sound("Applause");
	RemoveArrow();
}

func Script50() {
	TutorialMessage("$TutFlyAdvise$");
  SetArrow(650, 350);
  Sound("Ding");
}

func Script60() {
	if(!FindObject(AVTR, 610, 340 , 60, 50)) {
		goto(59);
		return;
	}
	SavePosition();
	TutorialMessage("$TutNotBad$");
	RemoveArrow();
}

func Script70() {
	TutorialMessage("$TutIntroduceDraft$");
	var Draft = CreateObject(DRFT, 700, 330, -1);
	Draft->SetPermanent();
	Sound("Wind2");
  SetArrow(790, 90);
}

func Script80() {
	if(!FindObject(AVTR, 750, 80 , 50, 25)) {
		goto(79);
		return;
	}
	SavePosition();
	TutorialMessage("$TutDraftNotice$");
	AddEffect("GraniteCheck", 0, 100, 5);
	RemoveArrow();
}

global func FxGraniteCheckTimer(object target, int effectNum, int effectTime) {
	if(!FindObject(AVTR, 1000, 390 , 60, 30)) {
		var clonk = FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(825, 0, 500, 400), Find_Not(Find_Or(Find_Action("Jump"), Find_Action("Tumble"))));
		if(clonk)
			clonk->Kill();
	} else {
		ScriptGo(1);
		return -1;
	}
}

func Script109() {
	TutorialMessage("$TutGraniteIsAcid$");
	ScriptGo(0);
}

func Script110() {
	SavePosition();
	TutorialMessage("$TutDone$");
	Sound("Applause");
	FindObject(SCRG)->Fulfill();
}

func SavePosition() {
	// Position for Relaunch
	var clonk = GetCrew();
	iPlrX = clonk->GetX();
	iPlrY = clonk->GetY();
}

func RelaunchPlayer(iPlr) {
	// Comment
	Sound("Oops");
	// new Clonk
	var old = FindObject(AVTR);
	if (!old->GetAlive()) {
		var pClonk = CreateObject(AVTR, iPlrX, iPlrY, iPlr);
		pClonk->GrabObjectInfo(old);
		SetCursor(iPlr, pClonk);
	}
}
