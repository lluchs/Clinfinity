/*-- Steamlauncher --*/

#strict 2

#include L_SS


/* Objekt ist ein Gewehr */
public func IsRifle () {
    return 1;
}

/* Objekt wurde ausgew�hlt */
public func Selection () {
    Sound("RevolverDraw");
}
