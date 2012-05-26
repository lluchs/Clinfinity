/*  Script: Attach.c
    Helper functions related to attaching stuff! */

#strict 2

/*  Function: AttachTo
	Attaches the calling object to the object _to_.
	*Note:* This also changes the object order, so _to_ is internally sorted before the calling object.
	Furthermore, the target object may receive an attach event by providing a method called AttachEvent().

	If the calling object isn't already in an action with procedure ATTACH the function will set the action "Attach".

	When callerVertex or targetVertex are omitted, this function will add new vertices so that the object won't move.
	*Important:* Vertex indices are starting at 1 instead of 0.

	Parameters:
	to - Object to which the caller should be attached.
	callerVertex - The caller object's vertex which will be attached.
	targetVertex - The vertex to attach to. */
global func AttachTo(object to, int callerVertex, int targetVertex) {
	if(GetProcedure() != "ATTACH")
		SetAction("Attach");
	SetActionTargets(to);

	// correct z-order
	SetObjectOrder(to);

	// optionally add vertices
	if(callerVertex == 0) {
		if(targetVertex == 0)
			// use first vertex
			targetVertex = 1;
		var tx = to->GetVertex(targetVertex - 1, false), ty = to->GetVertex(targetVertex - 1, true);
		AddVertex(AbsX(tx + to->GetX()), AbsY(ty + to->GetY()));
		callerVertex = GetVertexNum();
	}
	else if(targetVertex == 0) {
		var cx = GetVertex(callerVertex - 1, false), cy = GetVertex(callerVertex - 1, true);
		to->AddVertex(to->AbsX(cx + GetX()), to->AbsY(cy + GetY()));
		targetVertex = to->GetVertexNum();
	} else {
		var newX = to->GetVertex(targetVertex - 1, false) - GetVertex(callerVertex - 1, false) + to->GetX();
		var newY = to->GetVertex(targetVertex - 1, true) - GetVertex(callerVertex - 1, true) + to->GetY();
		SetPositionWithChildren(newX, newY);
	}

	callerVertex--;
	targetVertex--;

	SetActionData(256 * callerVertex + targetVertex);

	to->~AttachEvent(this, to, false, this);
}

/*  Function: CopyVertices
    Copies all *DefCore-defined* vertices from another object.

	The copied vertices are saved using an Effect, allowing easy removal using _RemoveCopiedVertices_.

	Parameters:
	from - Object from which the vertices are copied. */
global func CopyVertices(object from) {
	var start = GetVertexNum();
	for(var num = from->GetDefCoreVal("Vertices", "DefCore"), i = 0; i < num; i++) {
		var vx = from->GetDefCoreVal("VertexX", "DefCore", 0, i),
		    vy = from->GetDefCoreVal("VertexY", "DefCore", 0, i);
		AddVertex(AbsX(vx + from->GetX()), AbsY(vy + from->GetY()));
		SetVertex(start + i, 2, from->GetDefCoreVal("VertexCNAT", "DefCore", 0, i));
		SetVertex(start + i, 3, from->GetDefCoreVal("VertexFriction", "DefCore", 0, i));
	}
	// save vertices
	var effect = AddEffect("CopiedVertices", this, 1);
	EffectVar(0, this, effect) = from;
	//Log("%v add: %v", this, from);
}

static vertexRemoveOperation;

/*  Function: StartCopiedVerticesRemoval
	Improves performance of removing multiple vertices by only re-adding the remaining vertices in the end.

	*Important:* You need to call <EndCopiedVerticesRemoval> after all calls to <RemoveCopiedVertices> are done. */
global func StartCopiedVerticesRemoval() {
	vertexRemoveOperation = true;
}

/*  Function: EndCopiedVerticesRemoval
	Ends a batch removal using <RemoveCopiedVertices>. */
global func EndCopiedVerticesRemoval() {
	vertexRemoveOperation = false;
	RemoveCopiedVertices();
}

/*  Function: RemoveCopiedVertices
    Removes vertices which were previously copied using <CopyVertices>.

	*Important:* This function will remove all user-defined vertices that aren't copied from other objects.
	
	Note: Use <StartCopiedVerticesRemoval> when calling this function repeatedly with different objects.

	Parameters:
	from - Object from which the vertices were copied. */
global func RemoveCopiedVertices(object from) {
	// remove all additional vertices
	SetR(GetR());
	var i = GetEffectCount("CopiedVertices", this), success = false;
	var effects = CreateArray(i);
	while(i--) {
		var effect = GetEffect("CopiedVertices", this, i);
		var obj = EffectVar(0, this, effect);
		effects[i] = [effect, obj];
	}
	for(var e in effects) {
		var effect = e[0], obj = e[1];
		if(obj == from) {
			RemoveEffect(0, this, effect);
			success = true;
		} else if(!vertexRemoveOperation) {
			RemoveEffect(0, this, effect);
			// re-add vertices
			CopyVertices(obj);
		}
	}
	//Log("%v remove: %v", this, from);
	return success;
}

/*  Function: RemoveAllCopiedVertices
    Removes all vertices which were previously copied using <CopyVertices>. */
global func RemoveAllCopiedVertices() {
	// remove all additional vertices
	SetR(GetR());
	var i = GetEffectCount("CopiedVertices", this);
	while(i--) {
		RemoveEffect("CopiedVertices", this, i);
	}
	return true;
}

/*  Function: DebugCopiedVertices
	Prints all objects from which vertices were copied to the log. */
global func DebugCopiedVertices() {
	var i = GetEffectCount("CopiedVertices", this);
	while(i--) {
		var effect = GetEffect("CopiedVertices", this, i);
		var obj = EffectVar(0, this, effect);
		Log("%v", obj);
	}
}

/*	Function: CopyChildrenVertices
	Copies all *DefCore-defined* vertices from all attached objects.
	This is done recursively, thus the vertices of both directly and indirectly attached objects are copied.

	Parameters:
	child	- [optional] The attached object where the copying starts. */
global func CopyChildrenVertices(object child) {
	if(child == 0) {
		child = this;
	} else {
		CopyVertices(child);
	}
	var grandchildren = FindObjects(Find_ActionTarget(child), Find_Func("CompareProdecure", "ATTACH"));
	for(var grandchild in grandchildren) {
		CopyChildrenVertices(grandchild);
	}
}

/*	Function: RemoveCopiedChildrenVertices
	Removes all vertices which were previously copied from all attached objects.
	This is done recursively, all vertices of directly and indirectly attached objects are removed.

	Parameters:
	child	- [optional] The attached object where the removal starts.
	rec     - internal parameter, ignore. */
global func RemoveCopiedChildrenVertices(object child, bool rec) {
	if(!rec)
		StartCopiedVerticesRemoval();
	if(child == 0) {
		child = this;
	}
	var grandchildren = FindObjects(Find_ActionTarget(child), Find_Func("CompareProdecure", "ATTACH"));
	for(var grandchild in grandchildren) {
		RemoveCopiedChildrenVertices(grandchild, true);
	}
	if(child != this) {
		RemoveCopiedVertices(child);
	}
	if(!rec)
		EndCopiedVerticesRemoval();
}

/*	Function: CompareProdecure
	Compares the procedure of an object to the given procedure.

	Parameters:
	procedure	- Comparison value.

	Returns:
	*true* if the procedure matches, *false* otherwise. */
global func CompareProdecure(string procedure) {
	return GetProcedure() == procedure;
}

/*	Function: SetPositionWithChildren
	Sets the position of the calling object, including all objects attached to it.
	Coordinates are global.

	Parameters:
	x	- New horizontal coordinate.
	y	- New vertical coordinate. */
global func SetPositionWithChildren(int x, int y) {
	var children = FindObjects(Find_ActionTarget(this), Find_Func("CompareProdecure", "ATTACH"));
	for(var child in children) {
		var childX = child->GetX() - GetX() + x;
		var childY = child->GetY() - GetY() + y;
		child->SetPositionWithChildren(childX, childY);
	}
	SetPosition(x, y);
}
