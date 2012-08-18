/*-- Checkpoint --*/

#strict 2


static const iCHEC_ConstShowCheckpoint1_Radius = 30;
static const iCHEC_ConstShowCheckpoint1_CircleCreatingTime = 90;
static const iCHEC_ConstShowCheckpoint1_Color = -1;
static const iCHEC_ConstShowCheckpoint2_Width = 60;
static const iCHEC_ConstShowCheckpoint2_Height = 60;
static const iCHEC_ConstShowCheckpoint2_Size = 30;
static const iCHEC_ConstShowCheckpoint2_CreatingTime = 5;
static const iCHEC_ConstShowCheckpoint2_Color = -1;
static const iCHEC_ConstShowCheckpoint3_MaxRadius = 30;
static const iCHEC_ConstShowCheckpoint3_MinRadius = 10;
static const iCHEC_ConstShowCheckpoint3_CircleCreatingTime = 60;
static const iCHEC_ConstShowCheckpoint3_MoveInTime = 100;
static const iCHEC_ConstShowCheckpoint3_Color = -1;
static const iCHEC_ConstShowCheckpoint4_MaxRadius = 30;
static const iCHEC_ConstShowCheckpoint4_MinRadius = 10;
static const iCHEC_ConstShowCheckpoint4_MoveInTime = 50;
static const iCHEC_ConstShowCheckpoint4_Color = -1;
static const iCHEC_ConstGrab = 1;


static const iCHEC_ConstShowCheckpoint1_AntiLag = 1;
static const iCHEC_ConstShowCheckpoint2_AntiLag = 1;
static const iCHEC_ConstShowCheckpoint3_AntiLag = 1;
static const iCHEC_ConstShowCheckpoint4_AntiLag = 1;

static const iCHEC_ConstRadius = 30;



static fCHEC_Respawn;
static fCHEC_RespawnMenu;
static fCHEC_RespawnInventory;
static fCHEC_RemoveInv;
static iCHEC_GlobalShowCheckpoint;
static iCHEC_GlobalRadius;
static sCHEC_GlobalShowMessage;
static iCHEC_GlobalGrab;

static iCHEC_GlobalShowCheckpoint1_Radius;
static iCHEC_GlobalShowCheckpoint1_CircleCreatingTime;
static iCHEC_GlobalShowCheckpoint1_Color;
static iCHEC_GlobalShowCheckpoint2_Width;
static iCHEC_GlobalShowCheckpoint2_Height;
static iCHEC_GlobalShowCheckpoint2_Size;
static iCHEC_GlobalShowCheckpoint2_Color;
static iCHEC_GlobalShowCheckpoint2_CreatingTime;
static iCHEC_GlobalShowCheckpoint3_MaxRadius;
static iCHEC_GlobalShowCheckpoint3_MinRadius;
static iCHEC_GlobalShowCheckpoint3_CircleCreatingTime;
static iCHEC_GlobalShowCheckpoint3_MoveInTime;
static iCHEC_GlobalShowCheckpoint3_Color;
static iCHEC_GlobalShowCheckpoint4_MaxRadius;
static iCHEC_GlobalShowCheckpoint4_MinRadius;
static iCHEC_GlobalShowCheckpoint4_MoveInTime;
static iCHEC_GlobalShowCheckpoint4_Color;


static iCHEC_GlobalShowCheckpoint1_AntiLag;
static iCHEC_GlobalShowCheckpoint2_AntiLag;
static iCHEC_GlobalShowCheckpoint3_AntiLag;
static iCHEC_GlobalShowCheckpoint4_AntiLag;

static fCHEC_AutomaticLag;







//Hierin wird der aktuelle Winkel gespeichert!
local iCHEC_Angle;

//Hierin wird der temporäre Radius für die Darstellungsweise 3 gespeichert
local iCHEC_TempRadius;

//Hierin wird gespeichert, ob die Partikel-Spirale (Darstellungsweise 3) sich gerade nach innen oder außen bewegt
local sCHEC_MoveInOut;

//Hierin werden die Clonk gespeichert, die den Checkpoint passiert haben...
local aCHEC_Clonks;

//Hierin wird die Darstellungsweise gespeichert!
local iCHEC_ShowCheckpoint;

//Hierin wird die Nachricht gespeichert, die ausgegeben wird, wenn der Clonk den Checkpoint erreicht
local sCHEC_ShowMessage;

//Hierin wird der Radius des Kreises gespeichert, in den der Clonk kommen muss, um den Checkpoint passiert zu haben
//Anmerkung: Ist die Darstellungsweise ein Partikel-Kreis, so wird der Clonk im Radius des Partikel-Kreises gesucht!
//	     Ist die Darstellungsweise ein Partikel-Quadrat, so wird der Clonk im Bereich des Quadrates gesucht!
local iCHEC_Radius;

//Hierin wird gespeichert, ob man das Objekt anfassen muss, um den Checkpoint zu passieren
//0 = Keine Einstellung
//1 = Nicht anfassen
//2 = Anfassen
local iCHEC_Grab;

//Hierin wird das Grab-Objekt gespeichert
local pCHEC_Grab;



//In den folgenden Variablen werden alle Informationen zu den Darstellungsweisen gespeichert
local iCHEC_ShowCheckpoint1_Radius;
local iCHEC_ShowCheckpoint1_CircleCreatingTime;
local iCHEC_ShowCheckpoint1_Color;
local iCHEC_ShowCheckpoint2_Width;
local iCHEC_ShowCheckpoint2_Height;
local iCHEC_ShowCheckpoint2_Size;
local iCHEC_ShowCheckpoint2_CreatingTime;
local iCHEC_ShowCheckpoint2_Color;
local iCHEC_ShowCheckpoint3_MaxRadius;
local iCHEC_ShowCheckpoint3_MinRadius;
local iCHEC_ShowCheckpoint3_CircleCreatingTime;
local iCHEC_ShowCheckpoint3_MoveInTime;
local iCHEC_ShowCheckpoint3_Color;
local iCHEC_ShowCheckpoint4_MaxRadius;
local iCHEC_ShowCheckpoint4_MinRadius;
local iCHEC_ShowCheckpoint4_MoveInTime;
local iCHEC_ShowCheckpoint4_Color;

//Anti-Lag-Variablen...
local iCHEC_ShowCheckpoint1_AntiLag;
local iCHEC_ShowCheckpoint2_AntiLag;
local iCHEC_ShowCheckpoint3_AntiLag;
local iCHEC_ShowCheckpoint4_AntiLag;



protected func Initialize()
{
	//Nur mal so kleine Voreinstellungen...
	fCHEC_Respawn = true;
	fCHEC_RespawnMenu = true;
	fCHEC_RespawnInventory = true;

	//Und die Farben der Partikel setzen wir auch die konstanten Dingens
	iCHEC_ShowCheckpoint1_Color = iCHEC_ConstShowCheckpoint1_Color;
	iCHEC_ShowCheckpoint2_Color = iCHEC_ConstShowCheckpoint2_Color;
	iCHEC_ShowCheckpoint3_Color = iCHEC_ConstShowCheckpoint3_Color;
	iCHEC_ShowCheckpoint4_Color = iCHEC_ConstShowCheckpoint4_Color;



	//Wir sind unsichtbar...
	SetVisibility(VIS_None);

	//Effekt für den die Darstellungsweise erstellen
	AddEffect("Checkpoint" , this , 20 , 1 , this);

	//Und den Effekt zum Suchen des Clonks erstellen!
	AddEffect("CheckpointSearchClonks" , this , 20 , 1 , this);

	//Einen neuen Array machen!
	aCHEC_Clonks = [];

	//Das war bereits alles!
	return true;
}



private func FxCheckpointTimer(pTarget , iNumber , iEffectTime)
{
	//Ist eine allgemein gültige Darstellungsweise für alle angegeben?
	if(iCHEC_GlobalShowCheckpoint)	iCHEC_ShowCheckpoint = iCHEC_GlobalShowCheckpoint;

	//Müssen wir unsere Nachricht an die globale Nachricht anpassen?
	if(GetType(sCHEC_GlobalShowMessage) == C4V_String)	sCHEC_ShowMessage = sCHEC_GlobalShowMessage;

	//Ist ein globaler Radius angegeben? Dann diesen speichern! Ansonsten einen vorgegebenen nehmen...
	if(iCHEC_GlobalRadius)	iCHEC_Radius = iCHEC_GlobalRadius;
	if(!iCHEC_Radius)	iCHEC_Radius = iCHEC_ConstRadius;

	//Auch das mit dem Anfassen von den globalen Variablen übernehmen
	if(iCHEC_GlobalGrab)	iCHEC_Grab = iCHEC_GlobalGrab;
	if(!iCHEC_Grab)		iCHEC_Grab = iCHEC_ConstGrab;



	//Im folgenden Script werden die Such-Kriterien auf globale Informationen überprüft!
	//Gibt es keine und sind auch noch keine lokalen Infos angegeben, so werden die Standard-Informationen benutzt!

	if(iCHEC_GlobalShowCheckpoint1_Radius)			iCHEC_ShowCheckpoint1_Radius = iCHEC_GlobalShowCheckpoint1_Radius;
	else if(!iCHEC_ShowCheckpoint1_Radius)			iCHEC_ShowCheckpoint1_Radius = iCHEC_ConstShowCheckpoint1_Radius;
	if(iCHEC_GlobalShowCheckpoint1_CircleCreatingTime)	iCHEC_ShowCheckpoint1_CircleCreatingTime = iCHEC_GlobalShowCheckpoint1_CircleCreatingTime;
	else if(!iCHEC_ShowCheckpoint1_CircleCreatingTime)	iCHEC_ShowCheckpoint1_CircleCreatingTime = iCHEC_ConstShowCheckpoint1_CircleCreatingTime;
	if(!iCHEC_ShowCheckpoint1_Color)			iCHEC_ShowCheckpoint1_Color = iCHEC_GlobalShowCheckpoint1_Color;
	if(!iCHEC_ShowCheckpoint1_Color)			iCHEC_ShowCheckpoint1_Color = iCHEC_ConstShowCheckpoint1_Color;
	if(iCHEC_GlobalShowCheckpoint2_Width)			iCHEC_ShowCheckpoint2_Width = iCHEC_GlobalShowCheckpoint2_Width;
	else if(!iCHEC_ShowCheckpoint2_Width)			iCHEC_ShowCheckpoint2_Width = iCHEC_ConstShowCheckpoint2_Width;
	if(iCHEC_GlobalShowCheckpoint2_Height)			iCHEC_ShowCheckpoint2_Height = iCHEC_GlobalShowCheckpoint2_Height;
	else if(!iCHEC_ShowCheckpoint2_Height)			iCHEC_ShowCheckpoint2_Height = iCHEC_ConstShowCheckpoint2_Height;
	if(iCHEC_GlobalShowCheckpoint2_Size)			iCHEC_ShowCheckpoint2_Size = iCHEC_GlobalShowCheckpoint2_Size;
	else if(!iCHEC_ShowCheckpoint2_Size)			iCHEC_ShowCheckpoint2_Size = iCHEC_ConstShowCheckpoint2_Size;
	if(iCHEC_GlobalShowCheckpoint2_CreatingTime)		iCHEC_ShowCheckpoint2_CreatingTime = iCHEC_GlobalShowCheckpoint2_CreatingTime;
	else if(!iCHEC_ShowCheckpoint2_CreatingTime)		iCHEC_ShowCheckpoint2_CreatingTime = iCHEC_ConstShowCheckpoint2_CreatingTime;
	if(!iCHEC_ShowCheckpoint2_Color)			iCHEC_ShowCheckpoint2_Color = iCHEC_GlobalShowCheckpoint2_Color;
	if(!iCHEC_ShowCheckpoint2_Color)			iCHEC_ShowCheckpoint2_Color = iCHEC_ConstShowCheckpoint2_Color;
	if(iCHEC_GlobalShowCheckpoint3_MaxRadius)		iCHEC_ShowCheckpoint3_MaxRadius = iCHEC_GlobalShowCheckpoint3_MaxRadius;
	else if(!iCHEC_ShowCheckpoint3_MaxRadius)		iCHEC_ShowCheckpoint3_MaxRadius = iCHEC_ConstShowCheckpoint3_MaxRadius;
	if(iCHEC_GlobalShowCheckpoint3_MinRadius)		iCHEC_ShowCheckpoint3_MinRadius = iCHEC_GlobalShowCheckpoint3_MinRadius;
	else if(!iCHEC_ShowCheckpoint3_MinRadius)		iCHEC_ShowCheckpoint3_MinRadius = iCHEC_ConstShowCheckpoint3_MinRadius;
	if(iCHEC_GlobalShowCheckpoint3_CircleCreatingTime)	iCHEC_ShowCheckpoint3_CircleCreatingTime = iCHEC_GlobalShowCheckpoint3_CircleCreatingTime;
	else if(!iCHEC_ShowCheckpoint3_CircleCreatingTime)	iCHEC_ShowCheckpoint3_CircleCreatingTime = iCHEC_ConstShowCheckpoint3_CircleCreatingTime;
	if(iCHEC_GlobalShowCheckpoint3_MoveInTime)		iCHEC_ShowCheckpoint3_MoveInTime = iCHEC_GlobalShowCheckpoint3_MoveInTime;
	else if(!iCHEC_ShowCheckpoint3_MoveInTime)		iCHEC_ShowCheckpoint3_MoveInTime = iCHEC_ConstShowCheckpoint3_MoveInTime;
	if(!iCHEC_ShowCheckpoint3_Color)			iCHEC_ShowCheckpoint3_Color = iCHEC_GlobalShowCheckpoint3_Color;
	if(!iCHEC_ShowCheckpoint3_Color)			iCHEC_ShowCheckpoint3_Color = iCHEC_ConstShowCheckpoint3_Color;
	if(iCHEC_GlobalShowCheckpoint4_MaxRadius)		iCHEC_ShowCheckpoint4_MaxRadius = iCHEC_GlobalShowCheckpoint4_MaxRadius;
	else if(!iCHEC_ShowCheckpoint4_MaxRadius)		iCHEC_ShowCheckpoint4_MaxRadius = iCHEC_ConstShowCheckpoint4_MaxRadius;
	if(iCHEC_GlobalShowCheckpoint4_MinRadius)		iCHEC_ShowCheckpoint4_MinRadius = iCHEC_GlobalShowCheckpoint4_MinRadius;
	else if(!iCHEC_ShowCheckpoint4_MinRadius)		iCHEC_ShowCheckpoint4_MinRadius = iCHEC_ConstShowCheckpoint4_MinRadius;
	if(iCHEC_GlobalShowCheckpoint4_MoveInTime)		iCHEC_ShowCheckpoint4_MoveInTime = iCHEC_GlobalShowCheckpoint4_MoveInTime;
	else if(!iCHEC_ShowCheckpoint4_MoveInTime)		iCHEC_ShowCheckpoint4_MoveInTime = iCHEC_ConstShowCheckpoint4_MoveInTime;
	if(!iCHEC_ShowCheckpoint4_Color)			iCHEC_ShowCheckpoint4_Color = iCHEC_GlobalShowCheckpoint4_Color;
	if(!iCHEC_ShowCheckpoint4_Color)			iCHEC_ShowCheckpoint4_Color = iCHEC_ConstShowCheckpoint4_Color;

	if(iCHEC_GlobalShowCheckpoint1_AntiLag)			iCHEC_ShowCheckpoint1_AntiLag = iCHEC_GlobalShowCheckpoint1_AntiLag;
	else if(!iCHEC_ShowCheckpoint1_AntiLag)			iCHEC_ShowCheckpoint1_AntiLag = iCHEC_ConstShowCheckpoint1_AntiLag;
	if(iCHEC_GlobalShowCheckpoint2_AntiLag)			iCHEC_ShowCheckpoint2_AntiLag = iCHEC_GlobalShowCheckpoint2_AntiLag;
	else if(!iCHEC_ShowCheckpoint2_AntiLag)			iCHEC_ShowCheckpoint2_AntiLag = iCHEC_ConstShowCheckpoint2_AntiLag;
	if(iCHEC_GlobalShowCheckpoint3_AntiLag)			iCHEC_ShowCheckpoint3_AntiLag = iCHEC_GlobalShowCheckpoint3_AntiLag;
	else if(!iCHEC_ShowCheckpoint3_AntiLag)			iCHEC_ShowCheckpoint3_AntiLag = iCHEC_ConstShowCheckpoint3_AntiLag;
	if(iCHEC_GlobalShowCheckpoint4_AntiLag)			iCHEC_ShowCheckpoint4_AntiLag = iCHEC_GlobalShowCheckpoint4_AntiLag;
	else if(!iCHEC_ShowCheckpoint4_AntiLag)			iCHEC_ShowCheckpoint4_AntiLag = iCHEC_ConstShowCheckpoint4_AntiLag;



	//Automatisch Anti-Lag-Korrektur? Dann einfach die Anzahl aller Checkpoints mit Partikel = Anti-Lag!
	if(fCHEC_AutomaticLag)
	{
		//Haben wir eine Partikel-Darstell-Weise?
		if(GetAction() != "Signpost")
		{
			//Damit man es nicht immer neu berechnen muss...
			var iCheckpointCount = ObjectCount2(Find_ID(CHEC) , Find_Not(Find_Action("Signpost")));

			//Alle Variablen setzen
			iCHEC_ShowCheckpoint1_AntiLag = BoundBy(360 - iCheckpointCount , 50 , 360);
			iCHEC_ShowCheckpoint2_AntiLag = 1;
			iCHEC_ShowCheckpoint3_AntiLag = iCheckpointCount / 2;
			iCHEC_ShowCheckpoint4_AntiLag = BoundBy(360 - iCheckpointCount * 30 , 50 , 360);
		}
	}















	//Sind wir kein Weg-Weiser?
	if(GetAction() != "Signpost")
	{
		//Dann unsichtbar machen
		SetVisibility(VIS_None);

		//Und die Aktion zurücksetzen
		SetAction("Idle");

		//Ist noch ein Anfass-Objekt da? Dann dieses löschen!
		if(pCHEC_Grab)	RemoveObject(pCHEC_Grab);
	}



	//Darstellungsweise 1: Partikel-Kreis!
	if(iCHEC_ShowCheckpoint == 1)
	{
		//Den Partikelkreis erzeugen!
		for(var i = 0; i < (360 / iCHEC_ShowCheckpoint1_CircleCreatingTime); i ++)
		{
			//Anti-Lag-Variablen berücksichtigen
			if(!(i % iCHEC_ShowCheckpoint1_AntiLag))
			{
				var iRandom = RandomX(-2 , 2);
				if(iCHEC_ShowCheckpoint1_Color == -1)	CreateParticle("PSpark" , Sin(iCHEC_Angle , iCHEC_ShowCheckpoint1_Radius + iRandom) , Cos(iCHEC_Angle , iCHEC_ShowCheckpoint1_Radius + iRandom) , 0 , 0 , 20 , HSL(iCHEC_Angle , 255 , 100));
				else					CreateParticle("PSpark" , Sin(iCHEC_Angle , iCHEC_ShowCheckpoint1_Radius + iRandom) , Cos(iCHEC_Angle , iCHEC_ShowCheckpoint1_Radius + iRandom) , 0 , 0 , 20 , iCHEC_ShowCheckpoint1_Color);

				//Und einen Schritt weiter gehen!
				iCHEC_Angle ++;
			}
		}

		//Weiterzählen...
		iCHEC_Angle += 360 / iCHEC_ShowCheckpoint1_CircleCreatingTime;

		//Bei 360° angelangt? Dann wieder von vorne...
		if(iCHEC_Angle >= 360)	iCHEC_Angle = iCHEC_Angle - 360;
	}

	//Darstellungsweise 2: Zufällige Partikel in einem Quadrat
	else if(iCHEC_ShowCheckpoint == 2)
	{
		//Dürfen wir sie auch erstellen?
		if(!(iEffectTime % iCHEC_ShowCheckpoint2_CreatingTime) && !(iEffectTime % iCHEC_ShowCheckpoint2_AntiLag))
		{
			if(iCHEC_ShowCheckpoint2_Color == -1)
			{
				CreateParticle("PSpark" ,
					       RandomX(-(iCHEC_ShowCheckpoint2_Width / 2) , iCHEC_ShowCheckpoint2_Width / 2) ,
					       RandomX(-(iCHEC_ShowCheckpoint2_Height / 2) , iCHEC_ShowCheckpoint2_Height / 2) ,
					       0,
					       0,
					       iCHEC_ShowCheckpoint2_Size,
					       RGB(Random(256) , Random(256) , Random(256))
					      );
			}

			else
			{
				CreateParticle("PSpark" ,
					       RandomX(-(iCHEC_ShowCheckpoint2_Width / 2) , iCHEC_ShowCheckpoint2_Width / 2) ,
					       RandomX(-(iCHEC_ShowCheckpoint2_Height / 2) , iCHEC_ShowCheckpoint2_Height / 2) ,
					       0,
					       0,
					       iCHEC_ShowCheckpoint2_Size,
					       iCHEC_ShowCheckpoint2_Color
					      );
			}
		}
	}

	//Darstellungsweise 3: Partikel-Spirale, die sich nach innen und dann nach außen bewegt
	else if(iCHEC_ShowCheckpoint == 3)
	{
		//Noch keine Richtung angegeben? Dann setzen wir einfach mal eine zufällige
		if(sCHEC_MoveInOut != "MoveIn" && sCHEC_MoveInOut != "MoveOut")
			sCHEC_MoveInOut = ["MoveIn" , "MoveOut"][Random(2)];

		//Fangen wir gerade erst an?
		if(!iCHEC_TempRadius && iCHEC_ShowCheckpoint3_MinRadius)
			iCHEC_TempRadius = iCHEC_ShowCheckpoint3_MaxRadius;

		//Antilag-Variablen berücksichtigen
		if(!(iEffectTime % iCHEC_ShowCheckpoint3_AntiLag))
		{
			//Den Partikelkreis erzeugen!
			for(var i = 0; i < (360 / iCHEC_ShowCheckpoint3_CircleCreatingTime); i ++)
			{
				if(iCHEC_ShowCheckpoint2_Color == -1)	CreateParticle("PSpark" , Sin(iCHEC_Angle , iCHEC_TempRadius) , Cos(iCHEC_Angle , iCHEC_TempRadius) , 0 , 0 , 20 , HSL(iCHEC_Angle , 255 , 100));
				else					CreateParticle("PSpark" , Sin(iCHEC_Angle , iCHEC_TempRadius) , Cos(iCHEC_Angle , iCHEC_TempRadius) , 0 , 0 , 20 , iCHEC_ShowCheckpoint2_Color);

				//Und einen Schritt weiter gehen!
				iCHEC_Angle ++;
			}
		}

		//Trotzdem weiterzählen...
		else	iCHEC_Angle += 360 / iCHEC_ShowCheckpoint3_CircleCreatingTime;

		//Bei 360° angelangt? Dann wieder von vorne...
		if(iCHEC_Angle >= 360)	iCHEC_Angle = iCHEC_Angle - 360;


		//Zahl berechnen, die vom Radius sub- bzw. addiert wird
		var anyTempCount = (iCHEC_ShowCheckpoint3_MaxRadius - iCHEC_ShowCheckpoint3_MinRadius) / iCHEC_ShowCheckpoint3_MoveInTime;

		//0 Frames = Impossible, tja... Da müssen wir eben anders präzisieren...
		if(!anyTempCount)	anyTempCount = [1 , iCHEC_ShowCheckpoint3_MoveInTime / (iCHEC_ShowCheckpoint3_MaxRadius - iCHEC_ShowCheckpoint3_MinRadius)];

		//Und zählen
		if(sCHEC_MoveInOut == "MoveIn")
		{
			//Nur ein normaler Integer? Dann den Wert subtrahieren!
			if(GetType(anyTempCount) == C4V_Int)	iCHEC_TempRadius -= anyTempCount;

			//Oder ein Array?
			else if(GetType(anyTempCount) == C4V_Array)
			{
				//Dann subtrahieren, was im Array steht, aber auch die Zeit usw. berücksichtigen...
				if(!(iEffectTime % anyTempCount[1]))	iCHEC_TempRadius -= anyTempCount[0];
			}

			//Sind wir schon beim minimalsten Radius angelangt?
			if(iCHEC_TempRadius == iCHEC_ShowCheckpoint3_MinRadius)
			{
				//Dann wieder in die andere Richtung!
				sCHEC_MoveInOut = "MoveOut";
			}

			//Sind wir schon darunter?
			else if(iCHEC_TempRadius < iCHEC_ShowCheckpoint3_MinRadius)
			{
				//Auch hier in die andere Richtung gehen
				sCHEC_MoveInOut = "MoveOut";

				//Dann einfach die Präzision einhalten, und auf den minimalsten Radius setzen...
				iCHEC_TempRadius = iCHEC_ShowCheckpoint3_MinRadius;
			}
		}

		else if(sCHEC_MoveInOut == "MoveOut")
		{
			//Nur ein normaler Integer? Dann den Wert addieren!
			if(GetType(anyTempCount) == C4V_Int)	iCHEC_TempRadius += anyTempCount;

			//Oder ein Array?
			else if(GetType(anyTempCount) == C4V_Array)
			{
				//Dann addieren, was im Array steht, aber auch die Zeit usw. berücksichtigen...
				if(!(iEffectTime % anyTempCount[1]))	iCHEC_TempRadius += anyTempCount[0];
			}

			//Sind wir schon beim maximalsten Radius angelangt?
			if(iCHEC_TempRadius == iCHEC_ShowCheckpoint3_MaxRadius)
			{
				//Dann wieder in die andere Richtung!
				sCHEC_MoveInOut = "MoveIn";
			}

			//Sind wir schon darunter?
			else if(iCHEC_TempRadius > iCHEC_ShowCheckpoint3_MaxRadius)
			{
				//Auch hier in die andere Richtung gehen
				sCHEC_MoveInOut = "MoveIn";

				//Dann einfach die Präzision einhalten, und auf den minimalsten Radius setzen...
				iCHEC_TempRadius = iCHEC_ShowCheckpoint3_MaxRadius;
			}
		}
	}



	//Darstellungsweise 4: Partikelkreis, der sich nach innen und dann nach außen bewegt
	//Anmerkung: Kommentare sind die gleichen wie bei Darstellungsweise 3
	else if(iCHEC_ShowCheckpoint == 4)
	{
		//Noch keine Richtung angegeben? Dann setzen wir einfach mal eine zufällige
		if(sCHEC_MoveInOut != "MoveIn" && sCHEC_MoveInOut != "MoveOut")
			sCHEC_MoveInOut = ["MoveIn" , "MoveOut"][Random(2)];

		//Fangen wir gerade erst an?
		if(!iCHEC_TempRadius && iCHEC_ShowCheckpoint4_MinRadius)
			iCHEC_TempRadius = iCHEC_ShowCheckpoint4_MaxRadius;

		//Den Partikelkreis erzeugen!
		for(var i = 0; i < 360; i ++)
		{
			//Anti-Lag-Variablen berücksichtigen
			if(i % (360 / (360 - iCHEC_ShowCheckpoint4_AntiLag)))
			{
				if(iCHEC_ShowCheckpoint2_Color == -1)	CreateParticle("PSpark" , Sin(i , iCHEC_TempRadius) , Cos(i , iCHEC_TempRadius) , 0 , 0 , 20 , HSL(i , 255 , 100));
				else					CreateParticle("PSpark" , Sin(i , iCHEC_TempRadius) , Cos(i , iCHEC_TempRadius) , 0 , 0 , 20 , iCHEC_ShowCheckpoint2_Color);
			}
		}



		//Zahl berechnen, die vom Radius sub- bzw. addiert wird
		var anyTempCount = (iCHEC_ShowCheckpoint4_MaxRadius - iCHEC_ShowCheckpoint4_MinRadius) / iCHEC_ShowCheckpoint4_MoveInTime;

		//0 Frames = Impossible, tja... Da müssen wir eben anders präzisieren...
		if(!anyTempCount)	anyTempCount = [1 , iCHEC_ShowCheckpoint4_MoveInTime / (iCHEC_ShowCheckpoint4_MaxRadius - iCHEC_ShowCheckpoint4_MinRadius)];

		//Und zählen
		if(sCHEC_MoveInOut == "MoveIn")
		{
			//Nur ein normaler Integer? Dann den Wert subtrahieren!
			if(GetType(anyTempCount) == C4V_Int)	iCHEC_TempRadius -= anyTempCount;

			//Oder ein Array?
			else if(GetType(anyTempCount) == C4V_Array)
			{
				//Dann subtrahieren, was im Array steht, aber auch die Zeit usw. berücksichtigen...
				if(!(iEffectTime % anyTempCount[1]))	iCHEC_TempRadius -= anyTempCount[0];
			}

			//Sind wir schon beim minimalsten Radius angelangt?
			if(iCHEC_TempRadius == iCHEC_ShowCheckpoint4_MinRadius)
			{
				//Dann wieder in die andere Richtung!
				sCHEC_MoveInOut = "MoveOut";
			}

			//Sind wir schon darunter?
			else if(iCHEC_TempRadius < iCHEC_ShowCheckpoint4_MinRadius)
			{
				//Auch hier in die andere Richtung gehen
				sCHEC_MoveInOut = "MoveOut";

				//Dann einfach die Präzision einhalten, und auf den minimalsten Radius setzen...
				iCHEC_TempRadius = iCHEC_ShowCheckpoint4_MinRadius;
			}
		}

		else if(sCHEC_MoveInOut == "MoveOut")
		{
			//Nur ein normaler Integer? Dann den Wert addieren!
			if(GetType(anyTempCount) == C4V_Int)	iCHEC_TempRadius += anyTempCount;

			//Oder ein Array?
			else if(GetType(anyTempCount) == C4V_Array)
			{
				//Dann addieren, was im Array steht, aber auch die Zeit usw. berücksichtigen...
				if(!(iEffectTime % anyTempCount[1]))	iCHEC_TempRadius += anyTempCount[0];
			}

			//Sind wir schon beim maximalsten Radius angelangt?
			if(iCHEC_TempRadius == iCHEC_ShowCheckpoint4_MaxRadius)
			{
				//Dann wieder in die andere Richtung!
				sCHEC_MoveInOut = "MoveIn";
			}

			//Sind wir schon darunter?
			else if(iCHEC_TempRadius > iCHEC_ShowCheckpoint4_MaxRadius)
			{
				//Auch hier in die andere Richtung gehen
				sCHEC_MoveInOut = "MoveIn";

				//Dann einfach die Präzision einhalten, und auf den minimalsten Radius setzen...
				iCHEC_TempRadius = iCHEC_ShowCheckpoint4_MaxRadius;
			}
		}
	}

	//Darstellungsweise 10: Wegweiser
	else if(iCHEC_ShowCheckpoint == 10)
	{
		//Die Aktion setzen
		SetAction("Signpost");

		//Und sichtbar machen
		SetVisibility(VIS_All);
	}
}




private func FxCheckpointSearchClonksTimer()
{
	//Müssen wir anfassbar sein?
	if(iCHEC_Grab == 2)
	{
		//Noch kein Objekt?
		if(!pCHEC_Grab)
		{
			//Dann eines erstellen
			pCHEC_Grab = CreateObject(GRAB , 0 , 0 , GetOwner());

			//Aktion setzen
			pCHEC_Grab -> SetAction("Attaching");

			//Wo attachen? An den Checkpoint!
			pCHEC_Grab -> SetActionTargets(this);
		}

		//Auf jeden Fall die anderen Sachen nicht mehr aufrufen...
		return true;
	}

	//Sind wir nicht mehr anfassbar, aber wir haben noch den Grabber? Dann den Grabber löschen
	else if(pCHEC_Grab)	RemoveObject(pCHEC_Grab);

	//Machen wir einen Partikel-Kreis? Dann in dessen Bereich suchen!
	if(iCHEC_ShowCheckpoint == 1)
	{
		//Clonks in der Nähe?
		for(var pClonk in FindObjects(Find_Distance(iCHEC_ShowCheckpoint1_Radius) , Find_Func("IsClonk") , Find_OCF(OCF_Alive)))
		{
			if(pClonk && !PartOf(pClonk , aCHEC_Clonks))
			{
				//Zum Array hinzufügen und eine Nachricht über dem Kopf des Clonkes ausgeben lassen!
				aCHEC_Clonks[GetLength(aCHEC_Clonks)] = pClonk;

				//Die gespeicherte Nachricht ausgeben, ansonsten eine vordefinierte Nachricht ausgeben
				if(sCHEC_ShowMessage)	CustomMessage(sCHEC_ShowMessage , pClonk , GetOwner(pClonk));
				else			CustomMessage("$PassCheckpoint$" , pClonk , GetOwner(pClonk));

				//Und natürlich den Clonk informieren...
				pClonk ->~ SaveCheckpoint(this);
			}
		}
	}


	//Machen wir ein Partikel-Quadrat? Dann in diesem Quadrat suchen!
	else if(iCHEC_ShowCheckpoint == 2)
	{
		//Clonks in der Nähe?
		for(var pClonk in FindObjects(Find_InRect(-(iCHEC_ShowCheckpoint2_Width / 2) , -(iCHEC_ShowCheckpoint2_Height / 2) , iCHEC_ShowCheckpoint2_Width , iCHEC_ShowCheckpoint2_Height) , Find_Func("IsClonk") , Find_OCF(OCF_Alive)))
		{
			if(pClonk && !PartOf(pClonk , aCHEC_Clonks))
			{
				//Zum Array hinzufügen und eine Nachricht über dem Kopf des Clonkes ausgeben lassen!
				aCHEC_Clonks[GetLength(aCHEC_Clonks)] = pClonk;

				//Die gespeicherte Nachricht ausgeben, ansonsten eine vordefinierte Nachricht ausgeben
				if(sCHEC_ShowMessage)	CustomMessage(sCHEC_ShowMessage , pClonk , GetOwner(pClonk));
				else			CustomMessage("$PassCheckpoint$" , pClonk , GetOwner(pClonk));

				//Und natürlich den Clonk informieren...
				pClonk ->~ SaveCheckpoint(this);
			}
		}
	}

	//Partikel-Spirale? Dann in einem Kreis mit dem maximalen Radius der Spirale suchen!
	else if(iCHEC_ShowCheckpoint == 3)
	{
		//Clonks in der Nähe?
		for(var pClonk in FindObjects(Find_Distance(iCHEC_ShowCheckpoint3_MaxRadius) , Find_Func("IsClonk") , Find_OCF(OCF_Alive)))
		{
			if(pClonk && !PartOf(pClonk , aCHEC_Clonks))
			{
				//Zum Array hinzufügen und eine Nachricht über dem Kopf des Clonkes ausgeben lassen!
				aCHEC_Clonks[GetLength(aCHEC_Clonks)] = pClonk;

				//Die gespeicherte Nachricht ausgeben, ansonsten eine vordefinierte Nachricht ausgeben
				if(sCHEC_ShowMessage)	CustomMessage(sCHEC_ShowMessage , pClonk , GetOwner(pClonk));
				else			CustomMessage("$PassCheckpoint$" , pClonk , GetOwner(pClonk));

				//Und natürlich den Clonk informieren...
				pClonk ->~ SaveCheckpoint(this);
			}
		}
	}

	//Partikel-Kreis 2? Dann in einem Kreis mit dem maximalen Radius des Kreises suchen...
	else if(iCHEC_ShowCheckpoint == 4)
	{
		//Clonks in der Nähe?
		for(var pClonk in FindObjects(Find_Distance(iCHEC_ShowCheckpoint4_MaxRadius) , Find_Func("IsClonk") , Find_OCF(OCF_Alive)))
		{
			if(pClonk && !PartOf(pClonk , aCHEC_Clonks))
			{
				//Zum Array hinzufügen und eine Nachricht über dem Kopf des Clonkes ausgeben lassen!
				aCHEC_Clonks[GetLength(aCHEC_Clonks)] = pClonk;

				//Die gespeicherte Nachricht ausgeben, ansonsten eine vordefinierte Nachricht ausgeben
				if(sCHEC_ShowMessage)	CustomMessage(sCHEC_ShowMessage , pClonk , GetOwner(pClonk));
				else			CustomMessage("$PassCheckpoint$" , pClonk , GetOwner(pClonk));

				//Und natürlich den Clonk informieren...
				pClonk ->~ SaveCheckpoint(this);
			}
		}
	}

	

	//Müssen wir normal suchen?
	else
	{
		//Clonks in der Nähe?
		for(var pClonk in FindObjects(Find_Distance(iCHEC_Radius) , Find_Func("IsClonk") , Find_OCF(OCF_Alive)))
		{
			if(pClonk && !PartOf(pClonk , aCHEC_Clonks))
			{
				//Zum Array hinzufügen und eine Nachricht über dem Kopf des Clonkes ausgeben lassen!
				aCHEC_Clonks[GetLength(aCHEC_Clonks)] = pClonk;

				//Und die Nachrichten ausgeben?
				if(sCHEC_ShowMessage)	CustomMessage(sCHEC_ShowMessage , pClonk , GetOwner(pClonk));
				else			CustomMessage("$PassCheckpoint$" , pClonk , GetOwner(pClonk));

				//Und natürlich den Clonk informieren...
				pClonk ->~ SaveCheckpoint(this);
			}
		}
	}
}


private func PartOf(xPart, array aArray, bool fExact)
{
	for(var i; i < GetLength(aArray); i++)
	{
		if(xPart == aArray[i])	return true;

		if(fExact && GetType(aArray[i]) == C4V_Array)
		{
			if(PartOf(xPart,aArray[i],true))	return true;
		}

		return false;
	}
}





func CheckpointGrabbed(object pClonk)
{
	//Kein Clonk angegeben? Fail!
	if(!pClonk)	return false;

	//Zum Array hinzufügen und eine Nachricht über dem Kopf des Clonkes ausgeben lassen!
	aCHEC_Clonks[GetLength(aCHEC_Clonks)] = pClonk;

	//Und die Nachrichten ausgeben?
	if(sCHEC_ShowMessage)	CustomMessage(sCHEC_ShowMessage , pClonk , GetOwner(pClonk));
	else			CustomMessage("$PassCheckpoint$" , pClonk , GetOwner(pClonk));

	//Und natürlich den Clonk informieren...
	pClonk ->~ SaveCheckpoint(this);

	//Fertig!
	return true;
}

















/*--------------------------------- Hilfs-Funktionen ---------------------------------*/



func SetShow(int iShow)
{
	//Falscher Wert angegeben? Geht nicht!
	if(iShow < 0)	return false;

	//Den Wert setzen und zurückgeben, ob es geklappt hat
	return (iCHEC_ShowCheckpoint = iShow);
}



func SetShowOptions(int iOption , int iValue , bool fGlobal)
{
	//Darstellungsweise 1?
	if(iCHEC_ShowCheckpoint == 1)
	{
		if(iOption == 0)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint1_Radius = iValue;		else  iCHEC_ShowCheckpoint1_Radius = iValue; }
		if(iOption == 1)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint1_CircleCreatingTime = iValue;	else  iCHEC_ShowCheckpoint1_CircleCreatingTime = iValue; }
		if(iOption == 2)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint1_Color = iValue;		else  iCHEC_ShowCheckpoint1_Color = iValue; }

		//Fertig!
		return true;
	}

	//Darstellungsweise 2?
	if(iCHEC_ShowCheckpoint == 2)
	{
		if(iOption == 0)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint2_Width = iValue;		else  iCHEC_ShowCheckpoint2_Width = iValue; }
		if(iOption == 1)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint2_Height = iValue;		else  iCHEC_ShowCheckpoint2_Height = iValue; }
		if(iOption == 2)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint2_Size = iValue;		else  iCHEC_ShowCheckpoint2_Size = iValue; }
		if(iOption == 3)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint2_CreatingTime = iValue;	else  iCHEC_ShowCheckpoint2_CreatingTime = iValue; }
		if(iOption == 4)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint2_Color = iValue;		else  iCHEC_ShowCheckpoint2_Color = iValue; }

		//Fertig!
		return true;
	}

	//Darstellungsweise 3?
	if(iCHEC_ShowCheckpoint == 3)
	{
		if(iOption == 0)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint3_MaxRadius = iValue;		else  iCHEC_ShowCheckpoint3_MaxRadius = iValue; }
		if(iOption == 1)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint3_MinRadius = iValue;		else  iCHEC_ShowCheckpoint3_MinRadius = iValue; }
		if(iOption == 2)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint3_CircleCreatingTime = iValue;	else  iCHEC_ShowCheckpoint3_CircleCreatingTime = iValue; }
		if(iOption == 3)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint3_MoveInTime = iValue;		else  iCHEC_ShowCheckpoint3_MoveInTime = iValue; }
		if(iOption == 4)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint3_Color = iValue;		else  iCHEC_ShowCheckpoint3_Color = iValue; }

		//Fertig!
		return true;
	}

	//Darstellungsweise 4?
	if(iCHEC_ShowCheckpoint == 4)
	{
		if(iOption == 0)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint4_MaxRadius = iValue;		else  iCHEC_ShowCheckpoint4_MaxRadius = iValue; }
		if(iOption == 1)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint4_MinRadius = iValue;		else  iCHEC_ShowCheckpoint4_MinRadius = iValue; }
		if(iOption == 2)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint4_MoveInTime = iValue;		else  iCHEC_ShowCheckpoint4_MoveInTime = iValue; }
		if(iOption == 3)	{ if(fGlobal)  iCHEC_GlobalShowCheckpoint4_Color = iValue;		else  iCHEC_ShowCheckpoint4_Color = iValue; }

		//Fertig!
		return true;
	}

	//Irgendwas ging da nicht...
	return false;
}