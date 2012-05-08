/*  Script: Informationicon
    MatSys HUD display: Shows the contents of a specific material. */

#strict 2

func Initialize() {
	SetVisibility(VIS_Owner);
	return 1;
}

/*  Function: Set
    Sets the shown object.

	Parameters:
	idObj - The object's id. */
public func Set(id idObj) {
	// draw the overlay twice
	SetGraphics(0, this, idObj, GFX_Overlay, GFXOV_MODE_IngamePicture);
	AdjustGraphic(idObj, GFX_Overlay);
	SetGraphics(0, this, idObj, 2, GFXOV_MODE_IngamePicture);
	AdjustGraphic(idObj, 2);
}

private func AdjustGraphic(id idObj, int overlay) {
	var iWidth = 566, iHeight = 566, iXAdjust = 28000, iYAdjust = 13000;
	if(idObj != LHBK && idObj != LHTL) {
		iXAdjust *= 2;
		iXAdjust += 3000;
		iYAdjust *= 2;
		iWidth += 150;
		iHeight += 34;
	}
	SetObjDrawTransform(iWidth, 0, iXAdjust, 0, iHeight, iYAdjust, 0, overlay);
}

/*  Function: SetStatusMessage
    Creates the text below the symbol.

	Paramters:
	msg   - The message to show.
	lines - The number of lines the message has. */
public func SetStatusMessage(string msg, int lines) {
	CustomMessage(msg, this, GetOwner(), 30, 60 + lines * 95 / 10); // jede Zeile hat 9,5px
}

/*  Function: BuildMessage
    Creates a message showing how much of this material is needed.

	Parameters:
	iNeededMaterial - How much material is needed.
	iMaterial       - How much material is actually there. */
public func BuildMessage(int iNeededMaterial, int iMaterial) {
	var szMessage = Format("@%d|<c ", iMaterial);
	if(iNeededMaterial > iMaterial)
		Add(szMessage, "ff0000");
	else
		Add(szMessage, "00ff00");
	Add(szMessage, Format(">%d</c>", iNeededMaterial));
	SetStatusMessage(szMessage, 2);
	return 1;
}

private func Add(string &szString1, string szString2) {
	return szString1 = Format("%s%s", szString1, szString2);
}

/*  Function: Flash
    Flashes the material graphic red or green to indicate a change in storage amount.

	Paramters:
	change - How much the storage has changed. Negative -> red flash; positive -> green flash. */
public func Flash(int change) {
	// remove previous effect if there is one
	RemoveEffect("Flash", this);
	AddEffect("Flash", this, 1, 2, this, 0, change);
}

static const MatSys_FlashDuration = 20;

protected func FxFlashStart(object target, int effectNum, int temp, int change) {
	if(temp) return;
	EffectVar(0, target, effectNum) = change;
}

protected func FxFlashTimer(object target, int effectNum, int effectTime) {
	if(effectTime > MatSys_FlashDuration) {
		return -1;
	}
	var color, alpha = 255 * effectTime / MatSys_FlashDuration;
	if(EffectVar(0, target, effectNum) < 0)
		color = RGBa(255, 0, 0, alpha);
	else
		color = RGBa(0, 255, 0, alpha);
	SetClrModulation(color, this, 2);
}

