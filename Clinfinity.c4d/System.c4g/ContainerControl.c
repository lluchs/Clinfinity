/*	Script: ContainerControl.c
	Clonk appendto. Defines control events called in the first carried object, analogous to the other control functions. */

#strict 2

#appendto CLNK

protected func ControlLeft() {
	if(Control2Contents("Left")) return true;
	return inherited(...);
}

protected func ControlLeftDouble() {
	if(Control2Contents("LeftDouble")) return true;
	return inherited(...);
}

protected func ControlLeftReleased() {
	if(Control2Contents("LeftReleased")) return true;
	return inherited(...);
}


protected func ControlRight() {
	if(Control2Contents("Right")) return true;
	return inherited(...);
}

protected func ControlRightDouble() {
	if(Control2Contents("RightDouble")) return true;
	return inherited(...);
}

protected func ControlRightReleased() {
	if(Control2Contents("RightReleased")) return true;
	return inherited(...);
}


protected func ControlUp() {
	if(Control2Contents("Up")) return true;
	return inherited(...);
}

protected func ControlUpDouble() {
	if(Control2Contents("UpDouble")) return true;
	return inherited(...);
}

protected func ControlUpReleased() {
	if(Control2Contents("UpReleased")) return true;
	return inherited(...);
}


protected func ControlDown() {
	if(Control2Contents("Down")) return true;
	return inherited(...);
}

protected func ControlDownSingle() {
	if(Control2Contents("DownSingle")) return true;
	return inherited(...);
}

protected func ControlDownDouble() {
	if(Control2Contents("DownDouble")) return true;
	return inherited(...);
}

protected func ControlDownReleased() {
	if(Control2Contents("DownReleased")) return true;
	return inherited(...);
}




protected func ControlDig() {
	if(Control2Contents("Dig")) return true;
	return inherited(...);
}

protected func ControlDigSingle() {
	if(Control2Contents("DigSingle")) return true;
	return inherited(...);
}

protected func ControlDigDouble() {
	if(Control2Contents("DigDouble")) return true;
	return inherited(...);
}

protected func ControlDigReleased() {
	if(Control2Contents("DigReleased")) return true;
	return inherited(...);
}



protected func ControlThrow() {
	if(Control2Contents("Throw")) return true;
	return inherited(...);
}


protected func ControlUpdate(object self, int commandDirection, bool dig, bool throw) {
	if(Control2Contents("Update", commandDirection, dig, throw)) return true;
	return inherited(self, commandDirection, dig, throw);
}

protected func ControlCommand(string commandName, object target, int targetX, int targetY, object target2, data) {
	if(Control2Contents("Command", commandName, target, targetX, targetY, target2, data)) return true;
	return inherited(...);
}


private func Control2Contents(string controlName, a, b, c, d, e, f) {
	return Contents(0) != 0 && ObjectCall(Contents(0), Format("Container%s", controlName), a, b, c, d, e, f);
}
