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
	SetPlrShowControlPos(plr, SHOWCTRLPOS_TopLeft);
	SetTutorialMessagePos(MSG_Top | MSG_Left | MSG_WidthRel | MSG_XRel, 50, 50, 30);

	// Positions for Relaunch
	iPlrX = FindObject(AVTR)->GetX();
	iPlrY = FindObject(AVTR)->GetY();
	iCounter = 0;
}

func Script5() {
	TutorialMessage(Format("Joho %s! Willkommen in Clinfinity!", GetPlayerName()));
}

func Script15() {
	SetPlrShowControl(0, "____4_678_          __________");
	Sound("Ding");
	TutorialMessage("Dein Clonk kann ein paar Sachen mehr als der Standardclonk. Schauen wir uns den Doppelsprung an.");
}

func Script35() {
	TutorialMessage("Wenn du springst, kannst du einfach noch mal nach oben drücken, dann springt dein Clonk noch höher.|Du kannst dich im Sprung auch drehen und einen weiteren Sprung ausführen! Erreiche die Insel mit dem Pfeil.");
}

func Script40() {
	if(!FindObject(AVTR, 160, 50 , 30, 20)) {
		goto(39);
		return;
	}
	TutorialMessage("Sehr gut!");
	Sound("Applause");
}

func Script50() {
	TutorialMessage("Drücke im freien Flug nach unten, um deinen Fluganzug auszuklappen. Mit Links und Rechts kannst du die Richtung steuern, mit Springen ebenfalls einen weiteren Sprung im Flug ausführen. Erreiche die zweite Insel!");
}

func Script60() {
	if(!FindObject(AVTR, 610, 340 , 60, 50)) {
		goto(59);
		return;
	}
	TutorialMessage("Nicht schlecht!");
	Sound("Applause");
}

func Script70() {
	TutorialMessage("Das hier ist ein Aufwind! Du kannst ihn im Flug benutzen um dort hoch zu kommen!");
	var Draft = CreateObject(DRFT, 700, 330, -1);
	Draft->SetPermanent();
}

func Script80() {
	if(!FindObject(AVTR, 750, 80 , 50, 25)) {
		goto(79);
		return;
	}
	TutorialMessage("Gut gemacht. Du solltest wissen: Aufwinde treten im Normalfall zufällig auf und verschwinden auch wieder.");
	Sound("Applause");
}

func Script105() {
	TutorialMessage("Versuche nun mit deinem Fluganzug auf die nächste Insel zu kommen. Berühre dabei nicht den Fels!");
}

func Script110() {
	if(!FindObject(AVTR, 1000, 390 , 60, 30)) {
		goto(109);
		return;
	}
	TutorialMessage("Wunderbar. Damit hast du die Grundlagen erlernt!");
	Sound("Applause");
	FindObject(SCRG)->Fulfill();
}

func RelaunchPlayer(iPlr) {
	// Comment
	Sound("Oops");
	// new Clonk
	if (!FindObject(AVTR)->GetAlive()) {
		var pClonk = CreateObject(AVTR, iPlrX, iPlrY, iPlr);
		MakeCrewMember(pClonk, iPlr);
		SetCursor(iPlr, pClonk);
	}
}
