/*-- Crumbling Island --*/

#strict 2

func Initialize(obj) {
	Local(0) = RandomX(95, 98);
	Local(1) = 92;
}


protected func Crumble(){
  if(!Random(20))
    SetAction("Crumble");
  }

protected func RemoveObj(obj){
  RemoveObject(obj);
  }
