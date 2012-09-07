/*-- Lichtkegel --*/

#strict
#include LIGH

// Variablen

local iSizeX, iSizeY, iOffX, iOffY, iR, bDir;

/*
  iSizeX - X-Größe
  iSizeY - Y-Größe
  iOffX  - Verschiebung der Grafik in X-Richtung
  iOffY  - Verschiebung der Grafik in Y-Richtung
  //Alle Werte in 1000/Pixel
  iR     - Rotation
  bDir   - Wenn true wird ist das X-Offset mit der Dir gespiegelt
*/

// Licht updaten... verändert keine Werte sondern
// passt die Anzeige nur an
public func Update() {
  ChangeSizeXY(iSizeX, iSizeY);
  ChangeColor(iColor);
}

public func ChangeSize(int iNewSize) { 
  ChangeSizeXY(iNewSize,iNewSize);
}

public func ChangeSizeXY(int iNewSizeX, int iNewSizeY) {

  iSizeX = iNewSizeX;
  iSizeY = iNewSizeY;

  Draw();
}

public func ChangeOffset(int iNewOffX, int iNewOffY, bool bNewDir) {
  iOffX = iNewOffX;
  iOffY = iNewOffY;
  bDir = bNewDir;
  Draw();
}

public func ChangeR(int iNewR) {
  iR = -iNewR;
  Draw();
}

public func Draw() {
  var dir;
  if(!bDir) dir = 1;
  else      dir = 1-GetDir(GetActionTarget())*2;
  
  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);
  
  var fsin, fcos, xoff, yoff;
  fsin = Sin(iR, 1000);
  fcos = Cos(iR, 1000);
  xoff = 0;
  yoff = +64;
  
  var width = iSizeX*sizemod/100, height = iSizeY*sizemod/100;
  
  SetObjDrawTransform(
    +fcos*width/1000, +fsin*height/1000, (((1000-fcos)*xoff - fsin*yoff)*height)/1000+iOffX*1000*dir,
    -fsin*width/1000, +fcos*height/1000, (((1000-fcos)*yoff + fsin*xoff - 64000)*height)/1000+iOffY*1000
  );
}
