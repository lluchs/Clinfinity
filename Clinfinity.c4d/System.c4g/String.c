/*  Script: String.c
	Some helper functions for using strings. */
#strict 2

/*  Function: Substring
	Returns a subset of a string between one index and another, or through the end of the string.

	<Substring> extracts characters from indexA up to but not including indexB.

	When indexB is omitted or 0 the function will extract characters to the end of the string.

	Parameters:
	str    - The string.
	indexA - An integer between 0 and one less than the length of the string.
	indexB - (optional) An integer between 0 and the length of the string.

	Returns:
	The specified substring. */
global func Substring(string str, int indexA, int indexB) {
	var len = GetLength(str), result = "";
	if(indexA < 0)
		indexA = 0;
	if(indexB <= 0)
		indexB = len;
	for(var i = indexA; i < indexB && i < len; i++)
		result = Format("%s%s", result, GetStrChar(str, i));
	return result;
}

/*  Function: IndexOf

	Parameters:
	str         - The string in which will be searched.
	searchValue - A string representing the value to search for.
	fromIndex   - The location within the string to start the search from.
	
	Returns:
	The index within the calling String object of the first occurrence of the specified value, starting the search at fromIndex,
	or -1 if the value is not found. */
global func IndexOf(string str, string searchValue, int fromIndex) {
	var len = GetLength(str), slen = GetLength(searchValue);
	for(var i = fromIndex; i < len - slen; i++) {
		var matching = true;
		for(var j = 0; j < slen; j++) {
			if(GetChar(str, i + j) != GetChar(searchValue, j)) {
				matching = false;
				break;
			}
		}
		if(matching)
			return i;
	}
	return -1;
}

/*  Function: GetStrChar
	Combination of <GetStr> and <GetChar>.

	Parameters:
	str - The string whose character will be extracted.
	pos - The position of the character to extract.

	Returns:
	A string containing the character at the given position. */
global func GetStrChar(string str, int pos) {
	return GetStr(GetChar(str, pos));
}

/*  Function: GetStr
	Maps characters according to CP-1252.
	
	Parameters:
	char - ASCII code

	Returns:
	A string that contains the corresponding character. */
global func GetStr(int char) {
	return [" ", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?", "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_", "`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~", "DEL", "€", "", "‚", "ƒ", "„", "…", "†", "‡", "ˆ", "‰", "Š", "‹", "Œ", "", "Ž", "", "", "‘", "’", "“", "”", "•", "–", "—", "˜", "™", "š", "›", "œ", "", "ž", "Ÿ", "NBSP", "¡", "¢", "£", "¤", "¥", "¦", "§", "¨", "©", "ª", "«", "¬", "SHY", "®", "¯", "°", "±", "²", "³", "´", "µ", "¶", "·", "¸", "¹", "º", "»", "¼", "½", "¾", "¿", "À", "Á", "Â", "Ã", "Ä", "Å", "Æ", "Ç", "È", "É", "Ê", "Ë", "Ì", "Í", "Î", "Ï", "Ð", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö", "×", "Ø", "Ù", "Ú", "Û", "Ü", "Ý", "Þ", "ß", "à", "á", "â", "ã", "ä", "å", "æ", "ç", "è", "é", "ê", "ë", "ì", "í", "î", "ï", "ð", "ñ", "ò", "ó", "ô", "õ", "ö", "÷", "ø", "ù", "ú", "û", "ü", "ý", "þ", "ÿ"][char - 32];
}
