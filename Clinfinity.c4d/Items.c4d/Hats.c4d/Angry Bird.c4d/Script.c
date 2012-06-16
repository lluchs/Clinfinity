/*-- Angry Bird --*/

#strict 2

#include L_HT

protected func Cackle() {
	if(!Random(5)) {
		Sound(Format("Chicken%d", RandomX(1, 6)));
	}
}