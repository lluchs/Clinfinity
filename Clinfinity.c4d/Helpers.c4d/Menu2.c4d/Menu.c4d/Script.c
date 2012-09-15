/* Menuobjekt, verwaltet das Menü, nachdem es erstellt wurde. */

#strict 2

local pMenuObject;     //Objekt, in dem das Menu gezeigt wird
local pCommandObject;  //Objekt für den Callback (wenn Null, GameCall)
local szCallback;      //Wird in pCommandObject, wenn mit Auswahl fertig
local aMenu;           //Das Hauptmenu
local ExtraData;       //ExtraData, wird auch dem Callback übergeben, sinnvoll, um viele Optionsmenüs auf ein mal zu erstellen
local iVerbose;
local fForcedValues;
local aCurrentPath;

public func Init(object MenuObject, object CommandObject, string Callback, array Template, Data, int Verbose, bool ForcedValues)
{
  pMenuObject     = MenuObject;
  pCommandObject  = CommandObject;
  szCallback      = Callback;
  aMenu           = Template;
  ExtraData       = Data;
  iVerbose        = Verbose;
  fForcedValues   = ForcedValues;
  aCurrentPath    = CreateArray();
  ShowMenu();
}

private func ShowMenu(int iSelection)
{
  var currentMenu;
  if (aCurrentPath && aCurrentPath != [])
    currentMenu = GetSubmenu(aMenu, aCurrentPath);
  else
    currentMenu = aMenu;
  CreateMenu(currentMenu[MS4C_Symbol_Index], pMenuObject, this, 0,
    currentMenu[MS4C_Caption_Index], 0, 1);
  var i;
  for (var Key in currentMenu[MS4C_Sequence_Index])
  {
    var value = MenuGet(currentMenu, 0, Key);
    var type = value[MS4C_Type_Index];
    var cond = value[MS4C_Cond_Index];
    var szName = value[MS4C_Name_Index];
    var idItem = value[MS4C_Id_Index];
    var data = value[MS4C_Data_Index];
    if (type == MS4C_Typ_Bool)
      ShowBool(Key, cond, szName, idItem, data, i);
    else if (type == MS4C_Typ_Enum)
      ShowEnum(Key, cond, szName, idItem, data, i);
    else if (type == MS4C_Typ_Range)
      ShowRange(Key, cond, szName, idItem, data, i);
    else if (type == MS4C_Typ_Submenu)
      ShowSubmenu(Key, cond, szName, idItem, data, i);
  }
  AddMenuItem("$Finished$","Finished()",MS4C,pMenuObject,0,0,"$Finished$",2,3);
  SelectMenuItem(iSelection,pMenuObject);
}

private func ShowBool(Key, array aCond, string szName, id idItem, data, &i)
{
  var selectable = Evaluate_MenuCond(aMenu, aCond);
  var def = CreateDummy(idItem, data, true, selectable);
  var szCaption;
  if (data)
    szCaption = Format("$Deactivate$", szName);
  else
    szCaption = Format("$Activate$", szName);
  if (selectable)
  {
    AddMenuItem(szCaption, Format("Toggle(%v,%d)", Key, i++), 0, pMenuObject, 0,
      0, szCaption, 4, def);
  }
  else
  {
    szCaption = GreyString(szCaption);
    AddMenuItem(szCaption, Format("ShowMenu(%d)", i++), 0, pMenuObject, 0,
      0, szCaption, 4, def);
  }
  RemoveObject(def);
}

private func ShowEnum(Key, array aCond, string szName, id idItem, data, &i)
{
  var selectable = Evaluate_MenuCond(aMenu, aCond);
  for (var itemkey in data[3])
  {
    var itemvalue = HashGet(data[0], itemkey);
    var def = CreateDummy(itemvalue[1], itemkey == data[2], false, selectable);
    var szCaption = Format("$Choose$", itemvalue[0], szName);
    if (selectable)
    {
      AddMenuItem(szCaption, Format("Choose(%v,%v,%d)", Key, itemkey, i++), 0,
        pMenuObject, 0, 0, szCaption, 4, def);
    }
    else
    {
      szCaption = GreyString(szCaption);
      AddMenuItem(szCaption, Format("ShowMenu(%d)", i++), 0,
        pMenuObject, 0, 0, szCaption, 4, def);        
    }
    RemoveObject(def);
  }
}

private func ShowRange(Key, array aCond, string szName, id idItem, data, &i)
{
  var selectable = Evaluate_MenuCond(aMenu, aCond);
  var szCaption = szName;
  var szIncCaption = Format("$Increase$", szName, data[2]);
  var szDeCaption = Format("$Decrease$", szName, data[2]);  
  if (selectable)
  {
    AddMenuItem(szCaption, Format("ShowMenu(%d)",i++), idItem, pMenuObject, data[3],
      0, szCaption);
    AddMenuItem(szIncCaption, Format("Increase(%v,%d)", Key, i++), MS4C,
      pMenuObject, 0, 0, szIncCaption, 2, 1);
    AddMenuItem(szDeCaption ,Format("Decrease(%v,%d)", Key, i++), MS4C,
      pMenuObject, 0, 0, szDeCaption, 2, 2);
  }
  else
  {
    szCaption = GreyString(szCaption);
    szIncCaption = GreyString(szIncCaption);
    szDeCaption = GreyString(szDeCaption);
    var def = CreateDummy(idItem, false, false, selectable);
    AddMenuItem(szCaption, Format("ShowMenu(%d)",i++), 0, pMenuObject, data[3],
      0, szCaption, 4, def);
    RemoveObject(def);
    def = CreatePlusMinus(true, selectable);
    AddMenuItem(szIncCaption, Format("ShowMenu(%d)", i++), MS4C,
      pMenuObject, 0, 0, szIncCaption, 4, def);
    RemoveObject(def);
    def = CreatePlusMinus(false, selectable);
    AddMenuItem(szDeCaption ,Format("ShowMenu(%d)", i++), MS4C,
      pMenuObject, 0, 0, szDeCaption, 4, def);
    RemoveObject(def); 
  }
}

private func ShowSubmenu(Key, array aCond, string szName, id idItem, data, &i)
{
  var selectable = Evaluate_MenuCond(aMenu, aCond);
  var szCaption = Format("$OpenMenu$", szName);
  if (selectable)
  {
    AddMenuItem(szCaption, Format("OpenMenu(%v,%d)", Key, i++), idItem,
    	pMenuObject, 0, 0, szCaption);
  }
  else
  {
    var def = CreateDummy(idItem, false, false, selectable);
    szCaption = GreyString(szCaption);
    AddMenuItem(szCaption, Format("ShowMenu(%d)", i++), idItem, pMenuObject, 0,
    	0, szCaption, 4, def);
    RemoveObject(def);    
  }
}

protected func Toggle(Key, int iSelection)
{
  ToggleBool(MenuGet(aMenu, aCurrentPath, Key)[MS4C_Data_Index]);
  ShowMenu(iSelection);
}

private func ToggleBool(&f) { return f = !f; }

protected func Choose(Key,itemkey, int iSelection)
{
  MenuGet(aMenu, aCurrentPath, Key)[MS4C_Data_Index][2]=itemkey;
  ShowMenu(iSelection);
}

protected func Increase(Key, int iSelection)
{
  IncreaseRange(MenuGet(aMenu, aCurrentPath, Key)[MS4C_Data_Index]);
  ShowMenu(iSelection);
}

private func IncreaseRange(&range)
{
  return range[3]=BoundBy(range[3]+range[2],range[0],range[1]);
}

protected func Decrease(Key, int iSelection)
{
  DecreaseRange(MenuGet(aMenu, aCurrentPath, Key)[MS4C_Data_Index]);
  ShowMenu(iSelection);  
}

private func DecreaseRange(&range)
{
  return range[3]=BoundBy(range[3]-range[2],range[0],range[1]);
}

protected func OpenMenu(Key, int iSelection)
{
  PushBack(Key, aCurrentPath);
  ShowMenu();
}

protected func Finished(bool fAbort)
{
  if (aCurrentPath && aCurrentPath != [])
  {
    DeleteLast(aCurrentPath);
    ShowMenu();
  }
  else
  {
    var szMsg=CreateMenuMessage(aMenu,0,fForcedValues);
    if (iVerbose&MS4C_Verbose_ObjectMessage)
      Message(szMsg, pMenuObject);
    if (iVerbose&MS4C_Verbose_GlobalMessage)
      Message(szMsg);
    if (iVerbose&MS4C_Verbose_Log)
      Log(szMsg);
    if (pCommandObject)
      pCommandObject->Call(szCallback,GetMenuValues(aMenu,0,fForcedValues),ExtraData,fAbort);
    else
      GameCall(szCallback,GetMenuValues(aMenu,0,fForcedValues),ExtraData,fAbort);
    RemoveObject();
  }
}

public func MenuQueryCancel()
{
  CloseMenu(pMenuObject);
  Finished(true);
  return 1;
}

private func CreateDummy(id idItem, bool fChecked, bool fOverlayForced, bool fSelectable)
{
  var pDummy=CreateObject(MS4C,0,0,-1);
  if(idItem && FindDefinition(idItem))
  {
    SetObjectPicture(idItem, pDummy);
    if (fChecked)
    {
      SetGraphics("Chosen", pDummy, MS4C, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(650,0,5000,0,650,5000, pDummy, 1);      
    }
    else if(fOverlayForced)
    {
      SetGraphics("NotChosen", pDummy, MS4C, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(650,0,5000,0,650,5000, pDummy, 1);      
    }
  }
  else
  {
    if (fChecked)
      SetPicture(64*3, 0, 64, 64, pDummy);
    else
      SetPicture(64*4, 0, 64, 64, pDummy);
  }
  if (!fSelectable)
    SetClrModulation(RGB(128,128,128),pDummy);
  return pDummy;
}

private func CreatePlusMinus(bool fPlus, bool fSelectable)
{
  var pDummy=CreateObject(MS4C,0,0,-1);
  if(fPlus)
    SetPicture(64*1, 0, 64, 64, pDummy);
  else
    SetPicture(64*2, 0, 64, 64, pDummy);
  if (!fSelectable)
    SetClrModulation(RGB(128,128,128),pDummy);
  return pDummy;
}
