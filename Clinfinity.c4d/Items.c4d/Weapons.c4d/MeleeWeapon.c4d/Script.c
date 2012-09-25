#strict 2

// Hit living beings, vertically centered in front of the user of the weapon
public func MeleeHit(int rectangleWidth, int rectangleHeight, int damage) {
	for(var target in FindTargets(rectangleWidth, rectangleHeight)) {
		if(PathFree(Contained()->GetX(), Contained()->GetY(), target->GetX(), target->GetY())) {
			SetKiller(GetController(), target);
			target->DoEnergy(-damage);
		}
	}
}

// Throw back living beings (this is worse than a simple knockback)
public func ThrowBack(int rectangleWidth, int rectangleHeight, int throwSpeed) {
	for(var target in FindTargets(rectangleWidth, rectangleHeight)) {
		if(PathFree(Contained()->GetX(), Contained()->GetY(), target->GetX(), target->GetY())) {
			var away = -1;
			if(target->GetX() > Contained()->GetX()) away = 1;
			Fling(target, away * throwSpeed, -throwSpeed);
			target->SetAction("Jump");
		}
	}
}

private func FindTargets(int rectangleWidth, int rectangleHeight) {
	var rectangleX = 0;
	if(Contained()->GetDir() == DIR_Left) rectangleX = -rectangleWidth;
	return FindObjects(Find_Exclude(Contained()), Find_InRect(rectangleX, -rectangleHeight / 2, rectangleWidth, rectangleHeight), Find_NoContainer(), Find_OCF(OCF_Alive));
}
