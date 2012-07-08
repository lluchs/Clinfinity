/*-- Crumbling Island --*/

#strict 2

func Initialize() {
	Local(0) = RandomX(95, 98);
	Local(1) = 92;
}

protected func Crumble(){
  SetAction("Crumble");
  }

protected func RemoveObj(obj){
  RemoveObject(obj);
  }
