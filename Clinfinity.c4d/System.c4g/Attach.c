/*  Script: Attach.c
    Helper functions related to attaching stuff! */

#strict 2

/*  Function: AttachTo
	Attaches the calling object to the object _to_.

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
	}

	callerVertex--;
	targetVertex--;
	
	SetActionData(256 * callerVertex + targetVertex);
}