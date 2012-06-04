/*-- Neues Objekt --*/

#strict

func Initialize() {
  SetCategory(GetCategory() | C4D_Parallax() | C4D_Background());
  Local()=90; Local(1)=100;
}
