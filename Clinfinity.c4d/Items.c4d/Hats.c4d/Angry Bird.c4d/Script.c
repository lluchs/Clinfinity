/*-- Angry Bird --*/

#strict 2

#include L_HT

protected func Cackle() {
	if(!Random(3)) {
		Sound(Format("Chicken%d", RandomX(1, 7)));
	}
}