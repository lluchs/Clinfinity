/* Manages building demolition. */

#strict 2

#appendto CLNK
#appendto COLV

protected func ContextDemolition(object clonk) {
	[$CtxDemolitionDesc$|Image=CRYC|Condition=FindBuildingToDemolish]
	CreateMenu(CXCN, clonk, this);
	AddMenuItem("$CtxDemolitionDesc$!", "StartDemolition", MS4C, clonk, 0, 0, "OK", 2, 3);
}

protected func FindBuildingToDemolish() {
	if(GetID() == COLV) {
		var platform = this->~GetPlatform();
		return !FindObject2(platform->Find_BuildingsOnPlatform()) && platform;
	}
	return FindObject2(Find_AtObject(), Find_Category(C4D_Structure), Find_Allied(GetOwner()));
}

protected func StartDemolition() {
	var building = FindBuildingToDemolish();
	if(building) {
		var owner = GetOwner();
		for(var i = 0, comp, num; (comp = GetComponent(0, i, building)) && (num = GetComponent(comp, i, building)); i++) {
			MatSysDoFill(RandomX(1, num / 2), owner, comp);
		}
		Sound("DePressurize");
		building->CastParticles("PSpark", 10, 70, RandomX(-10, 10), RandomX(-10, 10), 50, 200);
		building->CastParticles("PxSpark", 10, 70, RandomX(-10, 10), RandomX(-10, 10), 50, 200);
		building->RemoveObject();
	}
}
