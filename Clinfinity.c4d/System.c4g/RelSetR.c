#strict 2

/** Rotiert ein Objekt um einen beliebigen Punkt herum. */
global func RelSetR(int rotation, int xPos, int yPos) {
  // Winkel zu den gedrehten Koordinaten
  var angle1 = Angle(0, 0, xPos, yPos);
  var angle2 = angle1 + GetR();
  var radius = Distance(0, 0, xPos, yPos);
  // angle2 + Radius => Umgerechneter Drehpunkt
  var rotX = (Sin(angle2, 100) * radius) / 100;
  var rotY = (-Cos(angle2, 100) * radius) / 100;
  // rotation + angle1 + Radius + umgerechneter Drehpunkt => Endpunkt
  angle1 += rotation;
  var targetX = (-Sin(angle1, 100) * radius) / 100 + rotX;
  var targetY = (Cos(angle1, 100) * radius) / 100 + rotY;
  SetPosition(GetX() + targetX, GetY() + targetY);
  SetR(rotation);
}