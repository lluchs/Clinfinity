/*	Script: Arrays
	Various helper functions for working with arrays. */

#strict 2

/*	Function: InArray
	Tests if a value is contained in an array.

	Parameters:
	Test	- The value to test.
	aArray	- The array that might contain the value.
	
	Returns:
	*true* if Test is in aArray, *false* otherwise. */
global func InArray(Test, array aArray)
{
  return GetIndexOf(Test, aArray) != -1;
}

// Sucht einen Wert im Array und löscht diesen
global func RemoveArrayValue(Test, array &aArray)
{
  // Aus der Liste löschen
  var i = GetLength(aArray), iLength = GetLength(aArray);
  while(i--)
  	if(aArray[i] == Test)
   	{
      		aArray[i] = 0;
      		while(++i < iLength) aArray[i-1] = aArray[i];
      		SetLength(aArray, iLength-1);
      		return 1;
    	}
  return 0;
}

global func ShuffleArray(array& aArray)
{
  var rnd;
  var iCount = GetLength(aArray);
  var aShuffled = CreateArray(iCount);
  for(var i = 0; i < iCount; i++)
  {
    rnd = Random(iCount-i);
    aShuffled[i] = aArray[rnd];
    DeleteArrayItem2(rnd, aArray);
  }
  aArray = aShuffled;
  return aShuffled;
}

//Löscht ein Item aus einem Array
global func DeleteArrayItem(iNumber, &aArray)
{
 var temp=[];
 for(var cnt;cnt<GetLength(aArray);cnt++)
 {
  if(cnt==iNumber)continue;
  var dif=0;
  if(cnt>iNumber)dif=-1;
  temp[cnt+dif]=aArray[cnt];
 } 
 
 aArray=temp;
 return aArray;
}

//Löscht ein Item aus einem Array, kann möglicherweise umsortieren
global func DeleteArrayItem2(iNumber,&aArray)
{
 //Ein ganz leeres Array?
 if(GetLength(aArray)==1)return (aArray=CreateArray());
 //Wenn das letzte Element ist diese Funktion auch nciht toller.
 if(GetLength(aArray)-1==iNumber)
 return DeleteArrayItem(iNumber, aArray);
 
 //Los!
 var last=aArray[GetLength(aArray)-1];
 aArray[GetLength(aArray)-1]=0;
 SetLength(aArray,GetLength(aArray)-1);
 aArray[iNumber]=last;
 return aArray;
}

//Sucht ein Item im array
global func GetArrayItemPosition(&value,&aArray) { //2do: remove that 
	return GetIndexOf(value, aArray);
}

/*	Function: PushBack
	Appends an element to an array.

	Parameters:
	value	- The element to append.
	aArray	- The array that gets appended to. */
global func PushBack(value, &aArray) {
	aArray[GetLength(aArray)] = value;
}

//Fügt ein Item am Anfang ein
global func PushFront(value,&aArray)
{
 var aNew=[];
 aNew[GetLength(aNew)]=value;
 for(var cnt=0;cnt<GetLength(aArray);cnt++)
 aNew[GetLength(aNew)]=aArray[cnt];
 
 aArray=aNew;
 return 1;
}

//Führt mit jedem Item im Array einen beliebigen Vergleich vor, der als String vorliegen sollte und gibt ein Array mit Ergebnissen zurück
// zB ForEach("<0",myArray);
//    ForEach("->~IsClonk()",myArray);
/*global func ForEach(sFunction,aArray)
{
 var aResults=[];
 
 Log("%d",GetLength(aArray));
 
 for(var cnt=0;cnt<GetLength(aArray);cnt++)
 {
  //var result=
 // eval(Format("Var(0)=(aArray[%d]%s)",cnt,sFunction));
 Var(0)=eval(Format("(%d%s)",aArray[cnt],sFunction));
  PushBack(Var(0),aResults);
 }
 
 return aResults;
}

global func Test()
{
 var arr=[];
 arr[GetLength(arr)]=1;
 arr[GetLength(arr)]=20;
 arr[GetLength(arr)]=34;
 arr[GetLength(arr)]=21;
 arr[GetLength(arr)]=564;
 arr[GetLength(arr)]=13;
 arr[GetLength(arr)]=4;
 arr[GetLength(arr)]=76;
 arr[GetLength(arr)]=124;
 var res=ForEach("<150",arr);
 
 for(var integer in res)
 Log("%d",integer);
}*/

//blame JCaesar if this function sucks:
global func PopElements(array &aArray, int iStart, int iCount) {
	if(iCount < 1) iCount = 1;
	for(iStart+iCount; iStart<GetLength(aArray); iStart++) {
		aArray[iStart-iCount] = aArray[iStart];
	}
	SetLength(aArray, GetLength(aArray)-iCount);
}

global func ConcatArrays(array a, array b) {
	var lenA = GetLength(a), lenB = GetLength(b);
	SetLength(a, lenA + lenB);
	for(var i = 0; i < lenB; i++)
		a[lenA + i] = b[i];
	return a;
}

/*	Function: RemoveElement
	Removes the indicated element from the array.
	Might not preserve the order of elements.
	
	Parameters:
	element		- The element to remove.
	targetArray - The array to remove the element from.
	
	Returns:
	*true* if the element was removed, *false* otherwise. */
global func RemoveElement(element, array &targetArray) {
	var length = GetLength(targetArray);
	for(var i = 0; i < length; i++) {
		if(targetArray[i] == element) {
			if(i <= length - 1) {
				// Move last element here
				targetArray[i] = targetArray[length - 1];
			}
			// Shorten by one
			SetLength(targetArray, length - 1);
			return true;
		}
	}
	return false;
}