/*-- Helpers --*/

#strict 2

//Fügt ein Item am Ende eines Array ein
global func PushBack(value,&aArray)
{
 return(aArray[GetLength(aArray)]=value);
}

global func DeleteLast(&aArray)
{
  return SetLength(aArray, GetLength(aArray) - 1);
}

//Aus Doku geklaut
global func SetObjectPicture(idSrcDef, pObj)
  {
  SetPicture(GetDefCoreVal("Picture", "DefCore", idSrcDef, 0),
             GetDefCoreVal("Picture", "DefCore", idSrcDef, 1),
             GetDefCoreVal("Picture", "DefCore", idSrcDef, 2),
             GetDefCoreVal("Picture", "DefCore", idSrcDef, 3), pObj);
  SetGraphics(0, pObj, idSrcDef);
  return(1);
  }

global func GreyString (string sz)
{
  return Format("<c 888888>%s</c>", sz);
}
