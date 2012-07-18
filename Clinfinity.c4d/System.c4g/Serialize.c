/* Serialisierung der Objekte */

#strict 2

global func GetObjects()
{
	// Objekte serialisieren
	var data = SerializeObjects(0,0,LandscapeWidth(), LandscapeHeight(), Find_And(Find_Not(Find_Func("IsHUD")),Find_Not(Find_Func("IsLight"))));
	
	// Aufbereiten
	return data;
}

global func LogObjects() {
	var objects = GetObjects();
	
	var reference = [[],[]];
	var obj, data;
	
	var i = 0;
	// Alle Objekte
	for (data in objects)
	{
		// Daten in data.
		// [0] ID
		// [1] X-Pos
		// [2] Y-Pos
		// [3] Besitzer
		// [4] Extra (weitere Aufrufe)
		// [5] Noch mehr Extra (???)
	
		// Sonderbehandlung (z.B. PlaceSpawnpoint)
		if (data[0] == -1)
		{
			Log("%s;",Format(data[4], data[1], data[2]));
		}
		// Ansonsten normal
		else
		{
			// CreateObject-String formatieren.
			var str = Format("CreateObject(%i,%d,%d,%d);",data[0], data[1], data[2], data[3]);

			var v = false;
			
			// falls noch was mit dem Objekt geschieht muss dieses Objekt gemerkt werden
			for (var extra in data[4])
			{
			
				// vor den Logs also ausgeben...
				if(!v) {
					v = true;
					Log("var obj%d = %s",i,str);
				}
				
				if (GetType(extra) == C4V_Array) {
					reference[0][GetLength(reference[0])] = [i, extra];
				} else {
					Log("obj%d->%s;", i, extra);
				}
			}
			for (var j in data[5])
			{
				// auch hier... aber nicht doppelt
				if(!v) {
					v = true;
					Log("var obj%d = %s",i,str);
				}
				reference[1][j] = i;
			}
			
			// Variablenname benutzt, hochzählen
			if(v) {
				++i;
			// ansonsten das CreateObject ausgeben
			} else {
				Log(str);
			}

		}
	}
	for (data in reference[0]) {
		// Hier kommt es normalerweise zu einem BUG!!! :(
		Log("obj%d->%s;", data[0], Format(data[1][0], reference[1][data[1][1]], reference[1][data[1][2]], reference[1][data[1][3]], reference[1][data[1][4]], reference[1][data[1][5]]));
	}
}

global func SerializeObjects(iX, int iY, int iW, int iH, exclusions)
{
	// Arraykompabilität
	if (!iW || !iH) {
		iY = iX[1]; iW = iX[2]; iH = iX[3];
		iX = iX[0];
	}
	
	var exp = Find_And(
		Find_InRect(iX, iY, iW, iH),
		Find_Not(Find_Func("NoSerialize")),
		Find_Not(Find_Category(C4D_Goal)),
		Find_Not(Find_Category(C4D_Rule)),
		Find_Not(Find_Category(C4D_Environment))
		);
	if (exclusions)
		exp = Find_And(exp, exclusions);
	
	var search = FindObjects(exp);
	var objects = CreateArray(2);
	objects[0] = CreateArray(GetLength(search)); // Vorallokierung
	objects[1] = CreateArray(GetLength(search));
	var extra, extray, i = 0;
	
	for(obj in search)
	{
		// wenn das Objekt von seinem Container gelöscht werden soll
		if (obj->Contained() && obj->Contained()->~RejectContainedSerialize(obj)) {
			// Allokierung kürzen
			SetLength(objects[0], GetLength(objects[0])-1);
			SetLength(objects[1], GetLength(objects[1])-1);
			continue;
		}
		
		// Aufräumen
		extra = CreateArray();
		extray = 0;
		
		// Objektzeiger temporär aufbewahren
		objects[1][i] = obj;
		
		// Rotation
		if (obj->GetR() != 0) extra[GetLength(extra)] = Format("SetR(%d)", obj->GetR());
		// Größe
		if (obj->GetCon() != 100) {
			extra[GetLength(extra)] = Format("SetCon(%d)", obj->GetCon());
			extray = -GetDefCoreVal("Offset", "DefCore", GetID(obj), 1) * (100-obj->GetCon()) / 100;
		}
		if (obj->Contained()) {
			extra[GetLength(extra)] = Format("Enter(Object(%d))", obj->Contained());
		}
		// Sonderbehandlung
		if (obj->~Serialize(extra, extray)) {
			// Kein CreateObject(ID, relative Position, Besitzer)->Extracalls Verfahren.
			// Hier nur eval(Format(extra,x,y))
			objects[0][i++] = [-1, -iX, -iY, 0, extra];
			continue;
		}
		
		// Aufzeichnen
		objects[0][i++] = [
			GetID(obj),
			-iX+GetX(obj),
			-iY+GetY(obj)-GetDefCoreVal("Offset", "DefCore", GetID(obj), 1)-extray,
			GetOwner(obj),
			extra,
			[]];
	}
	
	// Objektzeiger in den extras aufbereiten
	i = 0; // Index der Objektzeigerliste
	for (var index_object=0; index_object<GetLength(objects[0]); ++index_object) {
		// eigene eval formatierte Aufrufe brauchen keine Objektzeiger
		if (objects[0][index_object][0] == -1)
			continue;
		for (var index_extra=0; index_extra<GetLength(objects[0][index_object][4]); ++index_extra) {
			// keine Strings, nur [String, obj, ...] Inhalte
			if (GetType(objects[0][index_object][4][index_extra]) != C4V_Array)
				continue;
			for (var index_pointer=1; index_pointer<GetLength(objects[0][index_object][4][index_extra]); ++index_pointer) {
				// den Zeiger in der temporären Liste object[1] suchen
				for (var j=0; j<GetLength(objects[1]); ++j) {
					if (objects[0][index_object][4][index_extra][index_pointer] == objects[1][j]) {
						// das Zielobjekt markieren, damit es sich in die Verfügbarkeitsliste einträgt
						objects[0][j][5][GetLength(objects[0][j][5])] = i;
						// und die Referenz auf das Objekt in der Liste setzen
						objects[0][index_object][4][index_extra][index_pointer] = i;
						++i;
						break;
					}
				}
			}
		}
	}
	
	return objects[0];
}

global func DeserializeObjects(array objects, int x, int y)
{
	var reference = [[],[]];
	var obj, data;
	
	for (data in objects) {
		if (data[0] == -1) {
			// Sonderbehandlung
			eval(data[4], data[1]+x, data[2]+y);
		} else {
			obj = CreateObject(data[0], data[1]+x, data[2]+y, data[3]);
			for (var extra in data[4]) {
				if (GetType(extra) == C4V_Array) {
					reference[0][GetLength(reference[0])] = [obj, extra];
				} else {
					eval(Format("Object(%d)->%s", ObjectNumber(obj), extra));
				}
			}
			for (var i in data[5]) {
				reference[1][i] = obj;
			}
		}
	}
	for (data in reference[0]) {
		eval(Format("Object(%d)->%s", ObjectNumber(data[0]), Format(data[1][0], ObjectNumber(reference[1][data[1][1]]), ObjectNumber(reference[1][data[1][2]]), ObjectNumber(reference[1][data[1][3]]), ObjectNumber(reference[1][data[1][4]]), ObjectNumber(reference[1][data[1][5]]))));
	}
}

/* TODO!!! */
// Hazardclonk (KI Commands, ...)

/* UNWICHTIG :D */
// Leitern (auf Arrays umstellen und bla)
// Wegpunkte (arrive command data auch objekt-referenzieren)
