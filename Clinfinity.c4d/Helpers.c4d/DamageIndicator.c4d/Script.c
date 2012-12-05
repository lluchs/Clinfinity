#strict 2

public func IndicateDamage(object damagedObject, int amount) {
	if(damagedObject == 0) return;
	var x = damagedObject->GetX() - GetX();
	var y = damagedObject->GetY() - GetY();
	var indicator = CreateObject(DIND, x, y, NO_OWNER);
	indicator->CustomMessage(Format("@-%d", amount), indicator, NO_OWNER, 0, 0, RGB(255, 0, 0));
	Fling(indicator, RandomX(-2, 2), RandomX(-2, 1));
}
