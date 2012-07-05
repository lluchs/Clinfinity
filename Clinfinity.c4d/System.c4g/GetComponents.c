/*-- GetComponents --*/

#strict 2

global func GetComponents() {
	// alles durchzählen
	var aIDs, aNum, iNum = 0;
	// IDs finden, aus denen das Gebäude zusammengesetzt ist
	aIDs = CreateArray();
	for (var i = 0, component; component = GetComponent(0, i); ++i) {
		aIDs[i] = component; 
	}
	aNum = CreateArray(GetLength(aIDs));
	// Dazugehörige Anzahl
	for (var i = 0; i < GetLength(aIDs); ++i) {
		aNum[i] = GetComponent(aIDs[i]);
		iNum += aNum[i];
	}
	
	// IDs der Components, dazugehörige Anzahl, Anzahl insgesamt
	return [aIDs, aNum, iNum];
}
