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
  if(FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-20,-30,40,25))){
  Sound("squeak*");
  }
}
