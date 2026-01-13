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
 * @param	id			- First ID
 * @param	bReverse	- Reverse search
 * @return	unsigned - Second paired ID
 */
unsigned MapTable::getPairedID(IDMAPTABLE_REF idTableRef, unsigned id, bool reverse /* = false */)
{
	// Return INVALID if ID mapping table is invalid
	ASSERT(idTableRef != NULL);
	if (idTableRef == NULL) {
		return (unsigned)Constant::InvalidInteger;
	}

	// Find and return corresponding ID paired with specified macro ID
	int index = 0;
	do {
		// Get ID pair entry
		IDPAIR idPair = idTableRef[index++];

		// End of table
		if (idPair.first == INFINITE)
			break;

		// Reverse search
		if (reverse == true) {
			if (idPair.second == id)
				return idPair.first;
		}
		else {
			if (idPair.first == id)
				return idPair.second;
		}
	} while (index < MAX_TABLESIZE);

	// Return INVALID if not found
	return (unsigned)Constant::InvalidInteger;
}

/**
 * @brief	Find and return ID paired with given string
 * @param	pStringTableRef - Reference string table
 * @param	input			- Given string
 * @return	unsigned - String ID
 */
unsigned MapTable::getStringID(STRINGTABLE_REF stringTableRef, const wchar_t* input)
{
	// Return NULL string if language table is invalid
	ASSERT(stringTableRef != NULL);
	if (stringTableRef == NULL) {
		return (unsigned)Constant::InvalidInteger;
	}

	// Convert input string to lowercase
	String inputString(input);
	inputString.toLower();

	// Find and return corresponding ID paired with specified string
	int index = 0;
	String pairedString;
	do {
		// Get string pair entry
		LANGTEXT stringPair = stringTableRef[index++];

		// End of table
		if (stringPair.id == INFINITE)
			break;

		// Also convert language string to lower for easier comparison
		pairedString = stringPair.langString;
		pairedString.toLower();

		// Compare string ID
		if (!_tcscmp(pairedString, inputString)) {
			return stringPair.id;
		}
	} while (index < MAX_TABLESIZE);

	// Return INVALID if not found
	return (unsigned)Constant::InvalidInteger;
}

/**
 * @brief	Find and return string paired with specified ID
 * @param	pStringTableRef  - Reference string table
 * @param	id				 - String ID
 * @return	const wchar_t* - Paired string
 */
const wchar_t* MapTable::getString(STRINGTABLE_REF stringTableRef, unsigned id)
{
	// Return NULL string if language table is invalid
	ASSERT(stringTableRef != NULL);
	if (stringTableRef == NULL)
		return Constant::String::Null;

	// Find and return corresponding string paired with specified ID
	int index = 0;
	do {
		// Get string pair entry
		LANGTEXT stringPair = stringTableRef[index++];

		// End of table
		if (stringPair.id == INFINITE)
			break;

		// Compare string
		if (stringPair.id == id)
			return stringPair.langString;

	} while (index < MAX_TABLESIZE);

	return Constant::String::Null;
}
