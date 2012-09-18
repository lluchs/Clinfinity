/* Menüsystem */

#strict 2

static const MS4C_Typ_Bool    = 0;
static const MS4C_Typ_Enum    = 1;
static const MS4C_Typ_Range   = 2;
static const MS4C_Typ_Submenu = 3;

//Conditions
static const MS4C_Cond_Not            = 0;
static const MS4C_Cond_And            = 1;
static const MS4C_Cond_Or             = 2;
static const MS4C_Cond_Activated      = 3;
static const MS4C_Cond_Chosen         = 4;
static const MS4C_Cond_Less           = 5;
static const MS4C_Cond_Equal          = 6;
static const MS4C_Cond_Greater        = 7;

static const MS4C_Verbose_ObjectMessage = 1;
static const MS4C_Verbose_GlobalMessage = 2;
static const MS4C_Verbose_Log           = 4;

//Array indices
static const MS4C_Symbol_Index        = 0;
static const MS4C_Caption_Index       = 1;
static const MS4C_Hash_Index          = 2;
static const MS4C_Sequence_Index      = 3;

static const MS4C_Type_Index          = 0;
static const MS4C_Cond_Index          = 1;
static const MS4C_Name_Index          = 2;
static const MS4C_Id_Index            = 3;
static const MS4C_Data_Index          = 4;

global func CreateMenuTemplate(id Symbol, string Caption)
{
  var aMenu=CreateArray(4);
  aMenu[MS4C_Symbol_Index]=Symbol;
  aMenu[MS4C_Caption_Index]=Caption;
  aMenu[MS4C_Hash_Index]=CreateHash();
  aMenu[MS4C_Sequence_Index]=CreateArray();
  return aMenu;
}

//Fügt dem Menü eine Boolauswahlmöglichkeit hinzu, Key wird zur Identifizierung benutzt, sollte eindeutig sein
global func AddBoolChoice(&menu, array aPath, Key, array aCond, string szName, id idItem, bool fDefault)
{
  if (MenuContains(menu, aPath, Key)) return false;
  MenuPut(menu, aPath, Key, [MS4C_Typ_Bool, aCond, szName, idItem, fDefault]);
  return true;
}

//Fügt dem Menü eine Enumauswahl hinzu
global func AddEnumChoice(&menu, array aPath, Key, array aCond, string szName, id idItem)
{
  if (MenuContains(menu, aPath, Key)) return false;
  MenuPut(menu, aPath, Key, [MS4C_Typ_Enum, aCond, szName, idItem, [CreateHash(),0,0,[]]]);
  return true;
}

//Fügt der Enumauswahl mit dem Key Enumkey eine weitere Möglichkeit hinzu
global func AddEnumChoiceItem(&menu, array aPath, Enumkey, Itemkey, string szName, id idItem, Itemvalue, bool fDefault)
{
  if (!MenuContains(menu, aPath, Enumkey)) return false;
  if (MenuEnumContains(menu, aPath, Enumkey, Itemkey)) return false;
  MenuEnumPut(menu,aPath,Enumkey,Itemkey,[szName, idItem, Itemvalue]);
  if (!MenuGet(menu, aPath, Enumkey)[MS4C_Data_Index][1] || fDefault)
  {
    MenuGet(menu,aPath,Enumkey)[MS4C_Data_Index][1] = true;
    MenuGet(menu,aPath,Enumkey)[MS4C_Data_Index][2] = Itemkey;
  }
  return true;
}

//Fügt dem Menü eine Rangeauswahl hinzu
global func AddRangeChoice(&menu, array aPath, Key, array aCond, string szName, id idItem, int iMin, int iMax, int iStep, int iDefault)
{
  if (MenuContains(menu, aPath, Key)) return false;
  MenuPut(menu,aPath,Key,[MS4C_Typ_Range,aCond,szName,idItem,[iMin,iMax,iStep,iDefault]]);
  return true;
}

global func AddSubmenu(&menu, array aPath, Key, array aCond, string szName, id idItem, array submenu)
{
  if (MenuContains(menu, aPath, Key)) return false;
  if (!submenu) submenu = CreateMenuTemplate(idItem, szName);
  MenuPut(menu,aPath,Key,[MS4C_Typ_Submenu,aCond,szName,idItem,submenu]);
  return true;
}

//Erstellt ein Menü ausgehend von dem Template und ein paar unwichtigen Parametern
global func CreateMenuByTemplate(object pMenuObject, object pCommandObject, string szCallback, array aTemplate, ExtraData, int iVerbose, bool ForcedValues)
{
  var pMenu=CreateObject(MS4C, 0, 0, GetOwner());
  return pMenu->Init(pMenuObject, pCommandObject, szCallback, aTemplate, ExtraData, iVerbose, ForcedValues);
}

global func GetMenuValues(array aMenu, array aMainMenu, bool ForcedValues)
{
  if (!aMainMenu) aMainMenu = aMenu;
  var hValues=CreateHash();
  var iter = HashIter(aMenu[MS4C_Hash_Index]); // Iterator erzeugen
  var node;
  while(node = HashIterNext(iter)) { // Solange es weitere Elemente gibt
  if(!Evaluate_MenuCond(aMainMenu, node[1][MS4C_Cond_Index])
    && !ForcedValues)
    continue;
  if(node[1][MS4C_Type_Index]==MS4C_Typ_Bool)
    HashPut(hValues,node[0],node[1][MS4C_Data_Index]);
  else if (node[1][MS4C_Type_Index]==MS4C_Typ_Enum)
    HashPut(hValues,node[0],HashGet(node[1][MS4C_Data_Index][0],node[1][MS4C_Data_Index][2])[2]);
  else if (node[1][MS4C_Type_Index]==MS4C_Typ_Range)
    HashPut(hValues,node[0],node[1][MS4C_Data_Index][3]);
  else if (node[1][MS4C_Type_Index]==MS4C_Typ_Submenu)
    HashPut(hValues,node[0],GetMenuValues(node[1][MS4C_Data_Index], aMainMenu));
  else
    DebugLog("Unknown value typ");
  }
  return hValues;
}

global func CreateMenuMessage(array aMenu, array aMainMenu, bool ForcedValues)
{
  if (!aMainMenu) aMainMenu = aMenu;
  var szTotalMsg=aMenu[MS4C_Caption_Index];
  var fFirst=true;
  for (var Key in aMenu[MS4C_Sequence_Index])
  {
    var value=MenuGet(aMenu,0,Key);
    if(!Evaluate_MenuCond(aMainMenu, value[MS4C_Cond_Index])
      && !ForcedValues)
      continue;
    var szMsg=0;
    if(value[MS4C_Type_Index]==MS4C_Typ_Bool)
    {
      if(value[MS4C_Data_Index])
        szMsg=Format(value[MS4C_Name_Index]);
      else
        continue;
    }
    else if(value[MS4C_Type_Index]==MS4C_Typ_Enum)
    {
      szMsg=Format("%s: %s",value[MS4C_Name_Index],HashGet(value[MS4C_Data_Index][0],value[MS4C_Data_Index][2])[0]);
    }
    else if(value[MS4C_Type_Index]==MS4C_Typ_Range)
    {
      szMsg=Format("%s: %d",value[MS4C_Name_Index],value[MS4C_Data_Index][3]);
    }
    else if(value[MS4C_Type_Index]==MS4C_Typ_Submenu)
    {
      szMsg=Format("(%s)",CreateMenuMessage(value[MS4C_Data_Index], aMainMenu));
    }
    else
    {
      szMsg=Format("%s: Unknown value typ",value[MS4C_Name_Index]);
    }

    if (fFirst) //Hier hinten, damit nicht angewählte bools nicht alles kapput machen!
    {
      szTotalMsg=Format("%s: %s", szTotalMsg, szMsg);
      fFirst=false;
    }
    else
      szTotalMsg=Format("%s, %s", szTotalMsg, szMsg);
  }
  return szTotalMsg;
}

//Mit funktionslokalen Referenzen wäre das nicht passiert ..! OMGOMG INEFFIZIENT
global func &GetSubmenu(&menu, array aPath, int i)
{
  if(!MenuContains(menu,0,aPath[i])) return 0;
  if (GetLength(aPath)==i+1) return MenuGet(menu,0,aPath[i])[MS4C_Data_Index];
  else return GetSubmenu(MenuGet(menu,0,aPath[i])[MS4C_Data_Index],aPath,i+1);
}

global func MenuContains(&menu, array aPath, Key)
{
  if (!menu) return false;
  if (!aPath || aPath == [])
    return HashContains(menu[MS4C_Hash_Index], Key);
  else
    return HashContains(GetSubmenu(menu, aPath)[MS4C_Hash_Index], 0, Key);
}

global func &MenuGet(&menu, array aPath, Key)
{
  if (!aPath || aPath == [])
    return HashGet(menu[MS4C_Hash_Index], Key);
  else
    return HashGet(GetSubmenu(menu, aPath)[MS4C_Hash_Index], Key);
}

global func MenuPut(&menu, array aPath, Key, value)
{
  if (!aPath || aPath == [])
  {
    HashPut(menu[MS4C_Hash_Index], Key, value);
    PushBack(Key,menu[MS4C_Sequence_Index]);
  }
  else
  {
    HashPut(GetSubmenu(menu, aPath)[MS4C_Hash_Index], Key, value);
    PushBack(Key,GetSubmenu(menu, aPath)[MS4C_Sequence_Index]);
  }
  return true;
}

global func MenuEnumContains(&menu, array aPath, Enumkey, Itemkey)
{
  return HashContains(MenuGet(menu, aPath, Enumkey)[MS4C_Data_Index][0], Itemkey);
}

global func &MenuEnumGet(&menu, array aPath, Enumkey, Itemkey)
{
  return HashGet(MenuGet(menu, aPath, Enumkey)[MS4C_Data_Index][0], Itemkey);
}

global func MenuEnumPut(&menu, array aPath, Enumkey, Itemkey, value)
{
  HashPut(MenuGet(menu, aPath, Enumkey)[MS4C_Data_Index][0], Itemkey, value);
  PushBack(Itemkey, MenuGet(menu, aPath, Enumkey)[MS4C_Data_Index][3]);
  return true;
}

//Conds
global func MenuCond_Not(Cond) {
  return([MS4C_Cond_Not, Cond]);
}

global func MenuCond_And() {
  var result = [MS4C_Cond_And];
  for(var i = 0; Par(i); i++) {
    result[i+1] = Par(i);
  }
  return(result);
}

global func MenuCond_Or() {
  var result = [MS4C_Cond_Or];
  for(var i = 0; Par(i); i++)
    result[i+1] = Par(i);
  return(result);
}

global func MenuCond_Activated(array Path, Key) {
  return([MS4C_Cond_Activated, Path, Key]);
}

global func MenuCond_Chosen(array Path, Enumkey, Itemkey) {
  return([MS4C_Cond_Chosen, Path, Enumkey, Itemkey]);
}

global func MenuCond_Less(array Path, Key, value)
{
  return([MS4C_Cond_Less, Path, Key, value]);
}

global func MenuCond_Equal(array Path, Key, value)
{
  return([MS4C_Cond_Equal, Path, Key, value]);
}

global func MenuCond_Greater(array Path, Key, value)
{
  return([MS4C_Cond_Greater, Path, Key, value]);
}

global func Evaluate_MenuCond(array aMenu, array aCond)
{
  if (!aCond || aCond == []) return true;
  if (aCond[0] == MS4C_Cond_Not)
  {
    return !Evaluate_MenuCond(aMenu, aCond[1]);
  }
  else if (aCond[0] == MS4C_Cond_And)
  {
    for (var i = 1; i < GetLength(aCond); i++)
      if (!Evaluate_MenuCond(aMenu, aCond[i]))
        return false;
    return true;
  }
  else if (aCond[0] == MS4C_Cond_Or)
  {
    for (var i = 1; i < GetLength(aCond); i++)
      if (Evaluate_MenuCond(aMenu, aCond[i]))
        return true;
    return false;
  }
  else if (aCond[0] == MS4C_Cond_Activated)
  {
    if (!MenuContains(aMenu, aCond[1], aCond[2])) return false;
    if (MenuGet(aMenu, aCond[1], aCond[2])[MS4C_Type_Index] != MS4C_Typ_Bool)
      return false;
    return MenuGet(aMenu, aCond[1], aCond[2])[MS4C_Data_Index];
  }
  else if (aCond[0] == MS4C_Cond_Chosen)
  {
    if (!MenuContains(aMenu, aCond[1], aCond[2])) return false;
    if (MenuGet(aMenu, aCond[1], aCond[2])[MS4C_Type_Index] != MS4C_Typ_Enum)
      return false;
    return MenuGet(aMenu, aCond[1], aCond[2])[MS4C_Data_Index][2] == aCond[3];
  }
  else if (aCond[0] == MS4C_Cond_Less)
  {
    if (!MenuContains(aMenu, aCond[1], aCond[2])) return false;
    if (MenuGet(aMenu, aCond[1], aCond[2])[MS4C_Type_Index] != MS4C_Typ_Range)
      return false;
    return MenuGet(aMenu, aCond[1], aCond[2])[MS4C_Data_Index][3] < aCond[3];
  }
  else if (aCond[0] == MS4C_Cond_Equal)
  {
    if (!MenuContains(aMenu, aCond[1], aCond[2])) return false;
    if (MenuGet(aMenu, aCond[1], aCond[2])[MS4C_Type_Index] != MS4C_Typ_Range)
      return false;
    return MenuGet(aMenu, aCond[1], aCond[2])[MS4C_Data_Index][3] == aCond[3];
  }
  else if (aCond[0] == MS4C_Cond_Greater)
  {
    if (!MenuContains(aMenu, aCond[1], aCond[2])) return false;
    if (MenuGet(aMenu, aCond[1], aCond[2])[MS4C_Type_Index] != MS4C_Typ_Range)
      return false;
    return MenuGet(aMenu, aCond[1], aCond[2])[MS4C_Data_Index][3] > aCond[3];
  }  
}
