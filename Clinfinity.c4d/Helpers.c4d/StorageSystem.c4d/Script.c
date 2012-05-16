/*  Script: Storage System
    Library: Provides functionality for saving arbitrary integer values in a hash.
    It allows the definition of maximum fill levels, a sound to play when changing the fill level and overlaying numbers
    indicating the fill level. These options can be configured by overloading functions, see below. */

#strict 2

protected func Construction() {
	if(GetID() == L_SS) {
		DebugLog("ERROR: Dieses Objekt darf nicht erstellt werden");
		RemoveObject();
	}
	hFillLevel = CreateHash();
	UpdatePicture();
	return _inherited(...);
}

// Füllstand
local hFillLevel; // h wie Hash :x

private func TypeCheck() {
	if(GetType(hFillLevel) != C4V_Array)
		hFillLevel = CreateHash();
}
/* Füllung */

/*  Function: GetFill
    
	Parameters:
	Key - The hash key.

	Returns:
	A reference to the fill level. */
public func & GetFill(Key) { TypeCheck(); return HashGet(hFillLevel, Key); }

/*  Function: GetMaxFill
    Checks for the maximum fill by first calling MaxFill_[Key] and then, if that function does not exist or returns
	zero, the function MaxFill. These functions must be declared public.

	Parameters:
	Key - The hash key.

    Returns:
	The maximum fill for the given key or -1 if there is no maximum. */
public func GetMaxFill(Key) {
	var szFunc = "MaxFill", szFunc2, iFill;
	if(Key) {
		if(GetType(Key) == C4V_String)
			szFunc2 = Format("%s_%s", szFunc, Key);
		else
			szFunc2 = Format("%s_%v", szFunc, Key);
	}
	if(!(szFunc2 && (iFill = eval(Format("this->~%s()", szFunc2)))))
		iFill = Call(szFunc);
	
	return iFill;
}

/*  Function: DoFill
    Changes the fill level, optionally playing the fill change sound.

	This function will check for the fill bounds: 0 is the fixed lower bound and GetMaxFill(Key) is the upper bound.

	Parameters:
	iChange  - The fill level will be changed by adding this number.
	Key      - The hash key.
	fNoSound - No sound will be played if this variable is true.
	
	Returns:
	The actual change of the fill level. */
public func DoFill(int iChange, Key, bool fNoSound) {
	// fixed minimum: 0
	var iNewFill = Max(GetFill(Key) + iChange, 0);
	var maxFill = GetMaxFill(Key);
	// negative maxFill -> no maximum
	if(maxFill > 0)
		iNewFill = Min(iNewFill, maxFill);
	if (iNewFill == GetFill(Key)) return;
	iChange = iNewFill - GetFill(Key);
	OnFillChange(Key, iChange);
	if(!fNoSound)
		FillSound(Key, iChange);
	HashPut(hFillLevel, Key, iNewFill);
	UpdatePicture();
	// Tatsächliche Änderung des Füllstandes zurückgeben
	return iChange;
}

/*  Function: IsFull

	Parameters:
	Key - The hash key.

	Returns:
	Whether the fill level for the given key is full. */
public func IsFull(Key) { 
	return GetFill(Key) == GetMaxFill(Key);
}

/*  Function: UpdatePicture
    Updates the fill picture.

	This function is automatically called by DoFill, but you will need to call it when updating the fill level via
	GetFill's reference.

	Returns:
	true if the picture was successfully updated, false otherwise. */
public func UpdatePicture() {
	var Key = FillPicture();
	if(Key == -1)
		return false;
	if(GetFill(Key)>99) {
		SetGraphics(0,0,GetNumberID(GetFill(Key) / 100),1,GFXOV_MODE_Picture);    
		SetObjDrawTransform(400,0,-14000,0,400,+10000, this, 1);
	}
	else SetGraphics(0,0,0,1,0);

	if(GetFill(Key)>9) {
		SetGraphics(0,0,GetNumberID(GetFill(Key) / 10 - GetFill(Key) / 100 * 10),2,GFXOV_MODE_Picture);    
		SetObjDrawTransform(400,0,-7000,0,400,+10000, this, 2);
	}
	else SetGraphics(0,0,0,2,0);  

	SetGraphics(0,0,GetNumberID(GetFill(Key) % 10),3,GFXOV_MODE_Picture);   
	SetObjDrawTransform(400,0,0,0,400,+10000, this, 3);
	return true;
}

/* ID des passenden Zahlobjektes ausgeben */
private func GetNumberID(i) {
	return(C4Id(Format("SNB%d", i)));
}

/* Bei Bedarf überladen */

/*  Function: FillPicture
    This function needs to be overwritten to use the fill level indicator.

	Returns:
	The hash key for the fill level to show or -1 (default) if there should not be an indicator. */
private func FillPicture() { return -1; }

/*  Function: FillSound
    This function will be called to play a fill sound. It doesn't do anything as default action.

	When overriding, the function shouldn't do anything but calling Sound().

	Parameters:
	Key     - The hash key.
	iChange - The amount by which the fill level was changed. */
private func FillSound(Key, int iChange) { return; }

/*  Function: OnFillChange
    This function will be called whenever any fill level changes. It doesn't do anything as default action.

	Parameters:
	Key     - The hash key.
	iChange - The amount by which the fill level was changed. */
private func OnFillChange(Key, int iChange) { return; }
