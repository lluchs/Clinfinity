/* -- Plank -- */
#strict 2

protected func Left() {
	SetSolidMask(0, 22, 41, 22, 0, 0);
}

protected func Right() {
	SetSolidMask(41, 22, 41, 22, 0, 0);
}

protected func CheckClonk() {
	// any clonk on me?
	if(FindObject2(Find_InRect(-20, -30, 40, 25),
	               Find_Or(Find_OCF(OCF_Alive), Find_Category(C4D_Object)),
	               Find_Or(Find_Func("GetXDir"), Find_Func("GetYDir")))) {
		Sound("squeak*");
	}
}
