/**
 * @file		MapTable.cpp
 * @brief		Implement methods for map table processing
 * @author		AnthonyLeeStark
 * @date		2025.04.03
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "AppCore/MapTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/**
 * @brief	Find and return ID paired with specified macro ID
 * @param	pIDTableRef - Reference ID mapping table
 * @param	nID			- First ID
 * @param	bReverse	- Reverse search
 * @return	unsigned - Second paired ID
 */
unsigned MapTable::GetPairedID(IDMAPTABLE_REF pIDTableRef, unsigned nID, bool bReverse /* = false */)
{
	// Return INVALID if ID mapping table is invalid
	ASSERT(pIDTableRef != NULL);
	if (pIDTableRef == NULL) {
		return (unsigned)INT_INVALID;
	}

	// Find and return corresponding ID paired with specified macro ID
	int nIndex = 0;
	do {
		// Get ID pair entry
		IDPAIR idPair = pIDTableRef[nIndex++];

		// End of table
		if (idPair.first == INFINITE)
			break;

		// Reverse search
		if (bReverse == true) {
			if (idPair.second == nID)
				return idPair.first;
		}
		else {
			if (idPair.first == nID)
				return idPair.second;
		}
	} while (nIndex < MAX_TABLESIZE);

	// Return INVALID if not found
	return (unsigned)INT_INVALID;
}

/**
 * @brief	Find and return ID paired with given string
 * @param	pStringTableRef - Reference string table
 * @param	input			- Given string
 * @return	unsigned - String ID
 */
unsigned MapTable::GetStringID(STRINGTABLE_REF pStringTableRef, const wchar_t* input)
{
	// Return NULL string if language table is invalid
	ASSERT(pStringTableRef != NULL);
	if (pStringTableRef == NULL) {
		return (unsigned)INT_INVALID;
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
		if (stringPair.id == INFINITE)
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
	return (unsigned)INT_INVALID;
}

/**
 * @brief	Find and return string paired with specified ID
 * @param	pStringTableRef  - Reference string table
 * @param	nID				 - String ID
 * @return	const wchar_t* - Paired string
 */
const wchar_t* MapTable::GetString(STRINGTABLE_REF pStringTableRef, unsigned nID)
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
		if (stringPair.id == INFINITE)
			break;

		// Compare string
		if (stringPair.id == nID)
			return stringPair.langString;

	} while (nIndex < MAX_TABLESIZE);

	return Constant::String::Null;
}
