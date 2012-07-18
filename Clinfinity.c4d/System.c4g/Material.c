/*  Script: Material.c
	Provides helper functions related to material manipulation. */

#strict 2

/*  Function: DrawMaterialHLine
	Draws a horizontal material line.

	All coordinates are global.

	Parameters:
	matTex - Material-Texture combination for the material to be drawn.
	x1 - X coordinate of the start point.
	y1 - Y coordinate of the start point.
	wdt - Width of the line.
	sub - If true, the material will be drawn as 'underground'. */
global func DrawMaterialHLine(string matTex, int x, int y, int wdt, bool sub) {
	return DrawMaterialQuad(matTex, x, y, x, y+1, x+wdt, y, x+wdt, y+1, sub);
}

/*  Function: GetMaterialColorRGB
	Determines the color of a material.

	Parameters:
	mat - Index of the material of which to determine color.
	number - Material color index (0-2).

	Returns:
	The material color as RGB value. */
global func GetMaterialColorRGB(int mat, int number) {
	return RGB(GetMaterialColor(mat, number, 0), GetMaterialColor(mat, number, 1), GetMaterialColor(mat, number, 2));
}
