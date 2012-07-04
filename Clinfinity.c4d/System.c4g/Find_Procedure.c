/*  Script: Find_Procedure.c
	Provides procedure finding functions for FindObject2/FindObjects. */
#strict 2

/*  Function: Find_Procedure
	FindObject2/FindObjects criteria: Find objects with the given procedure.

	Parameters:
	procedure - The procedure to look for. */
global func Find_Procedure(string procedure) {
	return Find_Func("CompareProdecure", procedure);
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
