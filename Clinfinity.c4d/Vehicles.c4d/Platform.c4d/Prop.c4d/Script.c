/*	Script: Prop
	Attached to the underside of platforms.
	Generates enough force to make platforms fly! */

#strict 2

local leftDraft, rightDraft;

// width of one propeller
static const PROP_Width = 25;
// height of draft
static const PROP_DraftHeight = 50;

/*	Constructor: CreateProp
	Factory method for propellers.
	The coordinates are relative to the calling object in local calls, otherwise global.
	*Note:*	You should always create a prop using this method.

	Parameters:
	x			- Horizontal coordinate.
	y			- Vertical coordinate.
	forPlatform	- The platform this prop attaches to.

	Returns:
	The created prop. */
public func CreateProp(int x, int y, object forPlatform) {
	var prop = CreateObject(PROP, x, y, forPlatform->GetOwner());
	prop->AttachTo(forPlatform);
	return prop;
}

/*	Function: MovementEvent
	Event handler for movement events.

	Parameters:
	direction	- Direction of movement, specified by one of the COMD_* constants.
	source		- Source of the event.

	See Also:
	<Control mediator> */
public func MovementEvent(int direction, object source) {
	if(direction == COMD_Up) {
		SetAction("AttachFast");
		var height = PROP_DraftHeight * 5 / 4;
		leftDraft->SetSize(PROP_Width, height);
		rightDraft->SetSize(PROP_Width, height);
	} else if(direction == COMD_Down) {
		SetAction("AttachSlow");
		var height = PROP_DraftHeight * 3 / 4;
		leftDraft->SetSize(PROP_Width, height);
		rightDraft->SetSize(PROP_Width, height);
	} else {
		SetAction("Attach");
		leftDraft->SetSize(PROP_Width, PROP_DraftHeight);
		rightDraft->SetSize(PROP_Width, PROP_DraftHeight);
	}
}

protected func Initialize() {
	leftDraft = CreateDraft();
	rightDraft = CreateDraft();
	rightDraft->SetActionData(1);
}

private func CreateDraft() {
	var draft = CreateObject(DRFT, 0, 0, NO_OWNER);
	draft->SetR(180);
	draft->SetSize(25, 50);
	draft->SetAction("Attach", this);
	return draft;
}

protected func Destruction() {
	leftDraft->RemoveObject();
	rightDraft->RemoveObject();
}
