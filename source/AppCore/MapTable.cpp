
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		MapTable.cpp
//		Description:	Implement methods for map table processing
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2025.04.03:		Create new
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "AppCore/MapTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////
//// Implementations

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetPairedID
//	Description:	Find and return ID paired with specified macro ID
//  Arguments:		pIDTableRef - Reference ID mapping table
//					nID			- First ID
//					bReverse	- Reverse search
//  Return value:	UINT - Second paired ID
//
//////////////////////////////////////////////////////////////////////////

UINT MapTable::GetPairedID(IDMAPTABLE_REF pIDTableRef, UINT nID, BOOL bReverse /* = FALSE */)
{
	// Return INVALID if ID mapping table is invalid
	ASSERT(pIDTableRef != NULL);
	if (pIDTableRef == NULL) {
		return (UINT)INT_INVALID;
	}

	// Find and return corresponding ID paired with specified macro ID
	int nIndex = 0;
	do {
		// Get ID pair entry
		IDPAIR idPair = pIDTableRef[nIndex++];

		// End of table
		if (idPair.first == INT_NULL)
			break;

		// Reverse search
		if (bReverse == TRUE) {
			if (idPair.second == nID)
				return idPair.first;
		}
		else {
			if (idPair.first == nID)
				return idPair.second;
		}
	} while (nIndex < MAX_TABLESIZE);

	// Return INVALID if not found
	return (UINT)INT_INVALID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetStringID
//	Description:	Find and return ID paired with given string
//  Arguments:		pStringTableRef - Reference string table
//					input			- Given string
//  Return value:	UINT - String ID
//
//////////////////////////////////////////////////////////////////////////

UINT MapTable::GetStringID(STRINGTABLE_REF pStringTableRef, const wchar_t* input)
{
	// Return NULL string if language table is invalid
	ASSERT(pStringTableRef != NULL);
	if (pStringTableRef == NULL) {
		return (UINT)INT_INVALID;
	}

	// Convert input string to lowercase
	String inputString(input);
	inputString.ToLower();

	// Find and return corresponding ID paired with specified string
	int nIndex = 0;
	String pairedString;
	do {
		// Get string pair entry
		LANGTEXT stringPair = pStringTableRef[nIndex++];

		// End of table
		if (stringPair.id == INT_NULL)
			break;

		// Also convert language string to lower for easier comparison
		pairedString = stringPair.langString;
		pairedString.ToLower();

		// Compare string ID
		if (!_tcscmp(pairedString, inputString)) {
			return stringPair.id;
		}
	} while (nIndex < MAX_TABLESIZE);

	// Return INVALID if not found
	return (UINT)INT_INVALID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetString
//	Description:	Find and return string paired with specified ID
//  Arguments:		pStringTableRef  - Reference string table
//					nID				 - String ID
//  Return value:	const wchar_t* - Paired string
//
//////////////////////////////////////////////////////////////////////////

const wchar_t* MapTable::GetString(STRINGTABLE_REF pStringTableRef, UINT nID)
{
	// Return NULL string if language table is invalid
	ASSERT(pStringTableRef != NULL);
	if (pStringTableRef == NULL)
		return Constant::String::Null;

	// Find and return corresponding string paired with specified ID
	int nIndex = 0;
	do {
		// Get string pair entry
		LANGTEXT stringPair = pStringTableRef[nIndex++];

		// End of table
		if (stringPair.id == INT_NULL)
			break;

		// Compare string
		if (stringPair.id == nID)
			return stringPair.langString;

	} while (nIndex < MAX_TABLESIZE);

	return Constant::String::Null;
}
