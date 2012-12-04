#strict 2

// Hit living beings, vertically centered in front of the user of the weapon
public func MeleeHit(int rectangleWidth, int rectangleHeight, int damage) {
	for(var target in FindTargets(rectangleWidth, rectangleHeight)) {
		if(PathFree(Contained()->GetX(), Contained()->GetY(), target->GetX(), target->GetY())) {
			MeleeHitObject(target, damage);
		}
	}
}

public func MeleeHitObject(object target, int damage) {
	target->InflictDamage(damage, this);
	target->CastParticles("PxSpark", RandomX(3, 5), 16, 0, 0, 15, 30, RGB(83, 41, 25), RGB(193, 95, 60));
}

// Throw back living beings (this is worse than a simple knockback)
public func ThrowBack(int rectangleWidth, int rectangleHeight, int throwSpeed, bool tumble) {
	for(var target in FindTargets(rectangleWidth, rectangleHeight)) {
		if(PathFree(Contained()->GetX(), Contained()->GetY(), target->GetX(), target->GetY())) {
			ThrowBackObject(target, throwSpeed, tumble);
		}
	}
}

public func ThrowBackObject(object target, int throwSpeed, bool tumble) {
	var away = -1;
	if(target->GetX() > Contained()->GetX()) away = 1;
	Fling(target, away * throwSpeed, -throwSpeed);
	if(!tumble) {
		target->SetAction("Jump");
	}
}

private func FindTargets(int rectangleWidth, int rectangleHeight) {
	var rectangleX = 0;
	if(Contained()->GetDir() == DIR_Left) rectangleX = -rectangleWidth;
	return FindObjects(Find_Exclude(Contained()), Find_InRect(rectangleX, -rectangleHeight / 2, rectangleWidth, rectangleHeight), Find_NoContainer(), Find_OCF(OCF_Alive));
}

public func GetDamageType() { return DamageType_Melee; }
