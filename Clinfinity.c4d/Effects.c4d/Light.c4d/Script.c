/*	Script: Light
	Provides functions concerning lights.
	Taken from the Hazard pack. */

#strict

global func IsDark() { return true; }

global func CalcLight(&alphamod, &sizemod) {
	sizemod = 100 + GetDarkness() * 3 / 2; // Stretch up ta 250%
	alphamod = (100 - GetDarkness()) / 5; // Add 0-20 to alpha

	// No darkness or darkness value = 0: Almost invisible.
	if(!IsDark() || !GetDarkness()) alphamod = 30;
}

/*	Function: GetDarkness
	Returns darkness in percent (0-100).
	The default value is 0 (no darkness).
	Scenario scripts, goals and rules may specify other values for darkness by implementing _GetDarkness()_.

	Returns:
	Darkness value in percent. */
global func GetDarkness() {
	return BoundBy(GameCallEx("GetDarkness"), 0, 100);
}

/* Below: Original Hazard script */

local iColor, bAmbience, iF;

//Licht initialisieren, Parameter setzen, etc.
protected func Init(int iSize, int iColor, object pTarget, string cGraphics) {
  if(bAmbience && !IsDark())
    Schedule("RemoveObject()",1,0,this());
//Werte
  if(!iSize)
    iSize = GetCon();  
  if(!pTarget)
    pTarget = GetActionTarget();

  SetOwner(GetController(pTarget));

//Setzen und so
  SetAction("Attach",pTarget);
  SetPosition(GetX(pTarget),GetY(pTarget));
  ChangeColor(iColor);
  ChangeSize(iSize);
  ChangeGraphics(cGraphics);
}

//Wenn das Licht kein Ziel mehr hat -> weg damit.
protected func AttachTargetLost() {
  RemoveObject();
}

//Licht ausschalten
public func TurnOff() { SetVisibility(VIS_None); }

//Licht einschalten
public func TurnOn() { SetVisibility(VIS_All); }

// größe des Lichts ändern
public func ChangeSize(int iNewSize) { 
  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);

  iF = iNewSize;
  SetCon(iF*sizemod/100);
}

// Licht updaten... verändert keine Werte sondern
// passt die Anzeige nur an
public func Update() {
  ChangeSize(iF);
  ChangeColor(iColor);
}

//Farbe des Lichts ändern
public func ChangeColor(int iNewColor) {
  
  iColor = iNewColor;
  
  //Wenn keine dunkelheit ist
  if(!IsDark()) {
    //Hintergrundlichter ausblenden
    if(bAmbience)
      iNewColor = RGBa(255,255,255,255);
  }  
  
  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);
  
  var r,g,b,a;
  SplitRGBaValue(iColor,r,g,b,a);
  iNewColor = RGBa(r,g,b,Min(a+60+alphamod,255));
  
  SetClrModulation(iNewColor);
  
  return(iNewColor);
}

//Grafik ändern
public func ChangeGraphics(string cNewGraphics) {
  SetGraphics(cNewGraphics, 0, GetID());
}

//festlegen, dass es ein Licht ist.
public func IsLight() { return(1); }
public func NoWarp() { return(true); }

/*--  Globale Funktionen zur Lichterzeugung  --*/

//Hilfsfunktion
global func CreateLight(id ID, int iSize, int iColor, object pTarget, string sGraphics) {
  var light = CreateObject(ID, 0, 0, GetController(pTarget));
  light->Init(iSize, iColor, pTarget, sGraphics);
  
  return(light);
}

//erzeugt ein Licht mit Größe und Farbe und hängt es an pTarget
global func AddLight(int iSize, int iColor, object pTarget) {
  if(!pTarget)
    if(!(pTarget = this()))
      return();
  
  return(CreateLight(LIGH, iSize, iColor, pTarget));
}


global func AddLightHalf(int iSize, int iColor, object pTarget) {
  if(!pTarget)
    if(!(pTarget = this()))
      return();
  
  return(CreateLight(LIGH, iSize, iColor, pTarget, "Half"));
}

global func AddLightCone(int iSize, int iColor, object pTarget) {
  if(!pTarget)
    if(!(pTarget = this()))
      return();
  
  return(CreateLight(LGHC, iSize, iColor, pTarget));
}

//Ambience-Light, Umgebungslicht, nicht sichtbar wenn keine Dunkelheit herrscht.
global func AddLightAmbience(int iSize, object pTarget, string cGraphics) {
  if(!pTarget)
    if(!(pTarget = this()))
      return();
      
  var light = CreateObject(LIGH,0,0,GetController(pTarget));
  light->LocalN("bAmbience") = true;
  light->Init(iSize*5, RGBa(255,255,255,50), pTarget, cGraphics);
  
  return(light);
}

//Lichtblitz!
global func AddLightFlash(int iSize, int iX, int iY, int iColor) {
  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);
  
  var r,g,b,a;
  SplitRGBaValue(iColor,r,g,b,a);
  iColor = RGBa(r,g,b,Min(a+alphamod,255));

  CreateParticle("LightFlash",iX,iY,0,0,iSize*5*sizemod/100,iColor);
}
