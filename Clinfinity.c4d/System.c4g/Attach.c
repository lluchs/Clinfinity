/*  Script: Attach.c
    Helper functions related to attaching stuff! */

#strict 2

/*  Function: AttachTo
	Attaches the calling object to the object _to_.
	*Note:* This also changes the object order, so _to_ is internally sorted before the calling object.

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
	EffectVar(1, this, effect) = start;
	EffectVar(2, this, effect) = start + num;
}

/*  Function: RemoveCopiedVertices
    Removes vertices which were previously copied using _CopyVertices_.

	Parameters:
	from - Object from which the vertices were copied. */
global func RemoveCopiedVertices(object from) {
	var i = GetEffectCount("CopiedVertices", this);
	while(i--) {
		var effect = GetEffect("CopiedVertices", this, i);
		if(EffectVar(0, this, effect) == from) {
			var start = EffectVar(1, this, effect), end = EffectVar(2, this, effect);
			// indexes will shift, so counting up won't work
			for(var i = end - 1; i >= start; i--)
				RemoveVertex(i);
			RemoveEffect(0, this, effect);
			return true;
		}
	}
	return false;
}
