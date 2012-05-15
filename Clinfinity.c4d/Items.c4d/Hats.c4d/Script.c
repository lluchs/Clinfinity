/*  Script: Hats
    Library: Provides functionality for hats. */

#strict 2

/*  Function: IsHat
	Yes, it is!

	Returns:
	_true_ */
public func IsHat() { return true; }

/*  Function: AttachTo
	Attaches the hat to a clonk.

	Parameters:
	clonk - the clonk */
public func AttachTo(object clonk) {
	SetAction("Attach", clonk);
	SetActionData();
}

/*  Function: AddHat
	Adds a hat to the calling clonk.

	Parameters:
	hat - the hat's id
	
	Returns:
	The hat. */
global func AddHat(id hat) {
	var hat = CreateObject(hat, 0, 0, GetOwner());
	hat->AttachTo(this);
	return hat;
}

