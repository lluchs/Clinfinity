/*-- Blimp --*/

#strict 2
local t;

protected func Initialize(){
  SetAction("Spin");
}

// pendle around, no solidmask though (yet)
public func Pendle() {
  t++;
  var r = Cos(t, 200);
  var fsin=Sin(r, 1000, 100);
  var fcos=Cos(r, 1000, 100);
  SetObjDrawTransform (
    +fcos, +fsin, (1000-fcos) - fsin,
    -fsin, +fcos, (1000-fcos) + fsin
  );
}

