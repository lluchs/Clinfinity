/*--- geklaut von Himmelsrennen, aber psst ---*/

#strict

/* Initialisierung 

func Initialize()
{
  var cnt=Random(5); while (++cnt<12) PlaceBuilding();
  ProtectedCall(CreateObject(SGNL,LandscapeWidth()-20,LandscapeHeight()/2,-1),"ControlRight");
  Log("Los geht's!");
  return(1);
}

func PlaceBuilding()
{
  var obj;
  if (!(obj=PlaceVegetation(ROCK,0,0,LandscapeWidth(),LandscapeHeight()))) return();
  var x = GetX(obj); var y = GetY(obj);
  RemoveObject(obj);
  obj = CreateConstruction(RndBldID(), x, y, -1, 100, 1);
  if (GetID(obj)==ELEV) return(obj);
  if (GetID(obj)==CTW0) { CreateContents(CTW5,obj); CreateContents(GUNP,obj); CreateContents(GUNP,obj); }
  var cnt; while(++cnt<Random(30)) CreateContents(RndCntID(obj),obj);
  return(obj);
}

private func RndBldID()
{
  var num;
  if (!(num=Random(8))) return(HUT1);
  if (!--num)           return(HUT2);
  if (!--num)           return(HUT3);
  if (!--num)           return(LORY);
  if (!--num)           return(WAGN);
  if (!--num)           return(WRKS);
  if (!--num)           return(FNDR);
  if (!--num)           return(CTW0);
                        return(ELEV);
}

private func RndCntID()
{
  var num=13; if (GetCategory(Par())&C4D_Vehicle()) num=10;
  if (!(num=Random(num))) return(SFLN);
  if (!--num)             return(EFLN);
  if (!--num)             return(LOAM);
  if (!--num)             return(METL);
  if (!--num)             return(GUNP);
  if (!--num)             return(WOOD);
  if (!--num)             return(FLNT);
  if (!--num)             return(STFN);
  if (!--num)             return(CNKT);
  if (!--num)             return(FBMP);
                          return(BRDG);
}*/

// -- Callbacks des Rennen-Spielziels --
// wenn diese Funktionen nicht im Szenarioscript sind
// oder 0 zurück geben, wird der Default-Wert verwendet

// Richtung:
// 1: links -> rechts
// 2: rechts -> links
// 3: untern -> oben
// 4: oben -> unten
// default: links -> rechts
func GetRACEDirection() { return(1); }

// Start: Anzahl an Pixeln, ab dem Rand, von dort beginnt die Zählung
// default: 50 px
func GetRACEStartOffset() { return(180); }

// Ende: Anzahl an Pixeln, ab dem Rand, bis dorthin geht die Zählung
// default: 50 px
func GetRACEEndOffset() { return(100); }

/* Spielerinitialisierung */

protected func InitializePlayer(int iPlr){ return(JoinPlayer(iPlr));}

private func JoinPlayer(int iPlr)
{
  var obj=GetCrew(iPlr);
  DoEnergy(100000,obj);
  SetPosition(10+Random(100), LandscapeHeight()/2-15, obj);
  CreateContents(LOAM,obj);
  return(1);
}

/* Neubeitritt */

public func RelaunchPlayer(iPlr)
{
  var clnk;
  MakeCrewMember(clnk=CreateObject(CLNK,0,0,iPlr),iPlr);
  SelectCrew(iPlr, clnk, 1);
  Log(RndRelaunchMsg(), GetPlayerName(iPlr));
  return(JoinPlayer(iPlr));
}

private func RndRelaunchMsg()
{
  if (!(Var()=Random(11))) return("$MsgDeath1$");
  if (!--Var())            return("$MsgDeath2$");
  if (!--Var())            return("$MsgDeath3$");
  if (!--Var())            return("$MsgDeath4$");
  if (!--Var())            return("$MsgDeath5$");
  if (!--Var())            return("$MsgDeath6$");
  if (!--Var())            return("$MsgDeath7$");
  if (!--Var())            return("$MsgDeath8$");
  if (!--Var())            return("$MsgDeath9$");
  if (!--Var())            return("$MsgDeath10$");
                           return("$MsgDeath11$");
}                           

public func IsUpdraftPermanent() { return true; }