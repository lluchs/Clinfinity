/*-- Explosionsteuerung --*/

#strict 2

/* Dieses Objekt stellt die Grundfunktionalität für Gebäudeexplosionen
   zur Verfügung. */
   
static StructComp;

local RepairComp;

protected func Construction() {
	if(GetType(StructComp) != C4V_Array)
		StructComp = CreateHash();
	if(!HashContains(StructComp, GetID())) {
		var ID = GetID();
		var aIDs, aNum, iNum = 0;
		// IDs finden, aus denen das Gebäude zusammengesetzt ist
		aIDs = CreateArray();
		for (var i = 0, component; component = GetComponent(0, i, 0, ID); ++i) {
			aIDs[i] = component; 
		}
		aNum = CreateArray(GetLength(aIDs));
		// Anzahl
		for (var i = 0; i < GetLength(aIDs); ++i) {
			aNum[i] = GetComponent(aIDs[i], 0, 0, ID);
			iNum += aNum[i];
		}
		
		HashPut(StructComp, ID, [aIDs, aNum, iNum]);
	}
	
	RepairComp = CreateHash();
	
	return _inherited(...);
}

public func Damage(int iChange) {
	if(iChange <= 0)
		return;
	
	if (GetDamage() > MaxDamage())
		AddEffect("MaxDamageExplosion", this, 1, 20, this, L_DC);
	else {
		var type = "BuildingAttack";
		if(GetID() == CPSL)
			type = "CapsuleAttack";
		UpdateHUD(-GetOwner() - 2, HUD_EventLog, type);
	}
	var ox, oy, wdt, hgt;
	GetRect(GetID(), ox, oy, wdt, hgt);
	var glascount = iChange + Random(4);
	for (var i = 0; i < glascount; ++i) {
		CastParticles("Glas", 1, RandomX(30,50), ox+Random(wdt), oy+Random(hgt), 20, 20);
	}
	var frazzlecount = GetDamage() * 5 / MaxDamage();
	for (var i = 0; i < frazzlecount; ++i) {
		CastParticles("Fragment1", 1, RandomX(30,50), ox+Random(wdt), oy+Random(hgt), 20, 20);		
	}
	
	/* Components verlieren */
	
	var aComponents = GetComponents();
	var aIDs = aComponents[0], aNum = aComponents[1], iNum = aComponents[2];
	
	var aNormal = HashGet(StructComp, GetID());
	
	// wie viele Components müssen entfernt werden?
	var iCompDiff = aNormal[2] - iNum;
	var iDamDiff = ChangeRange(GetDamage(), 0, MaxDamage(), 0, aNormal[2]);
	//Log("CompDiff: %d; DamDiff: %d; MaxDamage: %d; Normal: %d", iCompDiff, iDamDiff, MaxDamage(), aNormal[2]);
	iDamDiff -= iCompDiff;
	
	// Components zufällig entfernen
	for(var i = 0; iNum && i < iDamDiff; i++) {
		var c = Random(GetLength(aIDs));
		if(aNum[c]) {
			SetComponent(aIDs[c], --aNum[c]);
			iNum--;
			//Log("Removed: %i", aIDs[c]);
		}
		else
			i--; // nochmal versuchen
	}
}

public func FxMaxDamageExplosionStart(object pTarget, int iEffectNumber, bool fTemp) {
	if(!fTemp) {
		Sound("Warning_blowup", 0, this, 0, 0, 1);
	}
}

public func FxMaxDamageExplosionTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (!pTarget) return -1; //Wir sind nicht Global!
	var ox, oy, wdt, hgt;
	GetRect(pTarget->GetID(), ox, oy, wdt, hgt);
	for (var i = 0; i < 10; ++i) {
		pTarget->CastParticles("Fragment1", 1, RandomX(50,150), ox+Random(wdt), oy+Random(hgt), 20, 30);		
	}
	if (iEffectTime > 1 * 35) {
		for (var i = 0; i < 10; ++i) {
			pTarget->CastParticles("Fragment2", 1, RandomX(50,150), ox+Random(wdt), oy+Random(hgt), 20, 30);
			pTarget->CastParticles("Fragment3", 1, RandomX(50,150), ox+Random(wdt), oy+Random(hgt), 20, 30);	
		}		
	}
	if (iEffectTime > 3 * 35) {
		DestroyBlast(pTarget);
		return -1;
	}
}

private func DestroyBlast(object pTarget) {
	if(!pTarget) if(!(pTarget=this)) return;
	
	var type = "BuildingExplode";
	if(pTarget -> GetID() == CPSL)
		type = "CapsuleExplode";
	UpdateHUD(-(pTarget -> GetOwner()) - 2, HUD_EventLog, type);
	
	var ox, oy, wdt, hgt;
	GetRect(pTarget->GetID(), ox, oy, wdt, hgt);
	var count = GetComponent(METL, 0, 0, pTarget -> GetID()) + Random(GetComponent(PSTC, 0, 0, pTarget -> GetID()));
	var power = Sqrt(wdt ** 2 + hgt ** 2);
	pTarget->CastObjects(ORE1,count/2,power);
	for(var pObj in FindObjects(Find_Container(pTarget), Find_Not(Find_Func("IsLight"))))
		pObj -> Exit(0, Random(wdt) - wdt / 2, Random(hgt) - hgt / 2);
	pTarget -> Explode(power / 2);
	
	return true;
}

public func MaxDamage () { return 1; } //Überlade mich!


/* Gebäudereparatur */

public func GetMissingComponents() {
	// IDs + Anzahl finden, aus denen das Gebäude momentan zusammengesetzt ist
	var aComponents = GetComponents();
	var aIDs = aComponents[0], aNum = aComponents[1], iNum = aComponents[2];
	
	var aNormal = HashGet(StructComp, GetID());
	var aNormalIDs = aNormal[0], aNormalNum = aNormal[1];
	
	// Unterschiede feststellen
	var aDiffIDs = CreateArray(), aDiffNum = CreateArray();
	for(var i = 0; i < GetLength(aNormalIDs); i++) {
		var ID = aNormalIDs[i];
		var index = GetIndexOf(ID, aIDs);
		var iDiff = 0;
		if(index == -1)
			iDiff = aNormalNum[i];
		else
			iDiff = aNormalNum[i] - aNum[index];
		
		iDiff -= HashGet(RepairComp, ID);
		
		if(iDiff) {
			PushBack(ID, aDiffIDs);
			PushBack(iDiff, aDiffNum);
		}
	}
	
	// Unterschiede (IDs), (Menge), Anzahl der fehlenden Components (insgesamt)
	return [aDiffIDs, aDiffNum, aNormal[2] - iNum];
}

public func DoRepairComponent(id ID) {
	return HashPut(RepairComp, ID, HashGet(RepairComp, ID) + 1);
}

public func Repair(int percent) {
	var iChange = Max(ChangeRange(percent, 0, 100, 0, MaxDamage()), 1);
	if(GetDamage() - iChange < 0)
		iChange = GetDamage();
	
	// Werden zusätzlich Components gebraucht?
	
	// Nur, wenn Baumaterial aktiviert
	if(ObjectCount2(Find_ID(CNMT))) {
		
		// aktuelle Components
		var aComponents = GetComponents();
		var aIDs = aComponents[0], aNum = aComponents[1], iNum = aComponents[2];
		
		// ursprünglich
		var aNormal = HashGet(StructComp, GetID());
		
		var iCompDiff = aNormal[2] - iNum;
		var iDamDiff = ChangeRange(GetDamage() - iChange, 0, MaxDamage(), 0, aNormal[2]);
		//Log("CompDiff: %d; DamDiff: %d; MaxDamage: %d; Normal: %d", iCompDiff, iDamDiff, MaxDamage(), aNormal[2]);
		iDamDiff -= iCompDiff;
		
		// fehlen Components für die Änderung?
		while(iDamDiff++ < 0) {
			// mal sehen was wir so alles da haben!
			var iter = HashIter(RepairComp);
			var node = HashIterNext(iter);
			// nichts da? Dann keine Reparatur.
			if(!node)
				return;
			
			if(!--node[1])
				HashErase(RepairComp, node[0]);
			else
				HashPut(RepairComp, node[0], node[1]);
			
			SetComponent(node[0], GetComponent(node[0]) + 1);
		}
	}
	
	// success!
	DoDamage(-iChange);
	return 1;
}
