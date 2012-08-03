/*-- Projektilvorschau --*/

#strict 2

static const g_CrosshairID = TRTY;

protected func Initialize() {
	SetVisibility(VIS_Owner);
}

global func RemoveTrajectory(object obj) {
	// Finden und vernichten
	var trajectory = FindObject2(Find_ID(TRTY), Find_ActionTarget(obj));
	if(trajectory) RemoveObject(trajectory);
}

global func AddTrajectory(object obj, int x, int y, int xDir, int yDir, int color) {
	if(!color) color = RGB(0, 255);
	//Log("Object: %s, x: %d, y: %d, xdir: %d, ydir: %d",GetName(obj),x,y,xDir,yDir);
	// Alte Vorschau löschen
	RemoveTrajectory(obj);
	// Neues Hilfsobjekt erzeugen
	var trajectory = CreateObject(TRTY, GetX(obj) - GetX(), GetY(obj) - GetY(), GetOwner(obj));
	//Log("Trajectory: %d %d",GetX(trajectory),GetY(trajectory));
	trajectory->SetAction("Attach", obj);
	// Startwerte setzen
	var i = -1, xOld, yOld;
	var faktor = 100;
	x *= faktor;
	y *= faktor;
	yDir *= 5;
	xDir *= 5;
	y -= 4 * faktor;
	xOld = x;
	yOld = y;
	// Flugbahn simulieren
	while(++i < 500) {
		// Geschwindigkeit und Gravitation aufrechnen
		x += xDir;
		y += yDir + GetGravity() * i / 20;
		// Wenn wir weit genug weg sind für einen neuen Punkt diesen einfügen
		if(Distance((xOld - x) / faktor, (yOld - y) / faktor) >= 10) {
			CreateParticle("Aimer", x / faktor - GetX(trajectory), y / faktor - GetY(trajectory),
					xDir / 500, yDir / 500, 10, color, trajectory);
			xOld = x;
			yOld = y;
		}
		// Oder ist es hier schon aus?
		if(GBackSolid(x / faktor - GetX(), y / faktor - GetY())) break;
	}
	// So, fertig
	return trajectory;
}

public func AttachTargetLost() {
	RemoveObject();
}
