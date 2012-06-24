#strict 2

protected func Initialize() {
	Local(0) = 4; // Line colour
	Local(1) = 4; // Vertex colour
	SetAction("Connect");
	SetVertex(0,0,GetX()); SetVertex(0,1,GetY());
	SetVertex(1,0,GetX()); SetVertex(1,1,GetY());
	SetPosition(0, 0, this());
}
