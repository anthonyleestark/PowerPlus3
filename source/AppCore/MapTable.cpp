
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
		if (idPair.nFirstID == INT_NULL)
			break;

		// Reverse search
		if (bReverse == TRUE) {
			if (idPair.nSecondID == nID)
				return idPair.nFirstID;
		}
		else {
			if (idPair.nFirstID == nID)
				return idPair.nSecondID;
		}
	} while (nIndex < MAX_TABLESIZE);

	// Return INVALID if not found
	return (UINT)INT_INVALID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetStringID
//	Description:	Find and return ID paired with given string
//  Arguments:		pStringTableRef  - Reference string table
//					lpszInput		 - Given string
//  Return value:	UINT - String ID
//
//////////////////////////////////////////////////////////////////////////

UINT MapTable::GetStringID(STRINGTABLE_REF pStringTableRef, LPCTSTR lpszInput)
{
	// Return NULL string if language table is invalid
	ASSERT(pStringTableRef != NULL);
	if (pStringTableRef == NULL) {
		return (UINT)INT_INVALID;
	}

	// Convert input string to lowercase
	CString strInput = lpszInput;
	strInput.MakeLower();

	// Find and return corresponding ID paired with specified string
	int nIndex = 0;
	CString strPairedString;
	do {
		// Get string pair entry
		LANGTEXT stringPair = pStringTableRef[nIndex++];

		// End of table
		if (stringPair.dwLangStringID == INT_NULL)
			break;

		// Also convert language string to lower for easier comparison
		strPairedString = stringPair.lpszLangString;
		strPairedString.MakeLower();

		// Compare string ID
		if (!_tcscmp(strPairedString, strInput)) {
			return stringPair.dwLangStringID;
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
//					pszResult		 - Result string (reference type)
//  Return value:	LPCTSTR - Paired string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR	MapTable::GetString(STRINGTABLE_REF pStringTableRef, UINT nID, LPTSTR pszResult /* = NULL */)
{
	// Return NULL string if language table is invalid
	ASSERT(pStringTableRef != NULL);
	if (pStringTableRef == NULL) {
		if (pszResult != NULL) pszResult = STRING_NULL;
		return STRING_NULL;
	}

	// Find and return corresponding string paired with specified ID
	int nIndex = 0;
	do {
		// Get string pair entry
		LANGTEXT stringPair = pStringTableRef[nIndex++];

		// End of table
		if (stringPair.dwLangStringID == INT_NULL)
			break;

		// Compare string
		if (stringPair.dwLangStringID == nID) {
			if (pszResult != NULL) {
				_tcscpy(pszResult, stringPair.lpszLangString);
			}
			return stringPair.lpszLangString;
		}
	} while (nIndex < MAX_TABLESIZE);

	// Return NULL string if not found
	if (pszResult != NULL) {
		pszResult = STRING_NULL;
	}

	return STRING_NULL;
}
