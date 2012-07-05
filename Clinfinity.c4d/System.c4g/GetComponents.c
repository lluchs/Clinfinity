/*-- GetComponents --*/

#strict 2

global func GetComponents() {
	// alles durchz�hlen
	var aIDs, aNum, iNum = 0;
	// IDs finden, aus denen das Geb�ude zusammengesetzt ist
	aIDs = CreateArray();
	for (var i = 0, component; component = GetComponent(0, i); ++i) {
		aIDs[i] = component; 
	}
	aNum = CreateArray(GetLength(aIDs));
	// Dazugeh�rige Anzahl
	for (var i = 0; i < GetLength(aIDs); ++i) {
		aNum[i] = GetComponent(aIDs[i]);
		iNum += aNum[i];
	}
	
	// IDs der Components, dazugeh�rige Anzahl, Anzahl insgesamt
	return [aIDs, aNum, iNum];
}
