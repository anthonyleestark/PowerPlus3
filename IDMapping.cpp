
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		IDMapping.cpp
//		Description:	Implement methods for components and items ID mapping
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.25:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IDMapping.h"


///////////////////////////
/// Implementations

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add an ID pair to ID map
//  Arguments:		dwID		 - Integer typed ID
//					lpszStringID - String typed ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SIDMapping::Add(DWORD dwID, LPCSTR lpszStringID)
{
	IDMAPPAIR idMapPair = { dwID, lpszStringID };
	IDMapData.Add(idMapPair);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Modify
//	Description:	Modify an ID pair in ID map
//  Arguments:		dwID		 - Integer typed ID
//					lpszStringID - String typed ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SIDMapping::Modify(DWORD dwID, LPCSTR lpszStringID)
{
	// Find item index
	int nIndex = FindID(dwID);
	if (nIndex == INT_INVALID)
		return;
	
	// Modify item at index
	IDMapData.GetAt(nIndex).lpszStringID = lpszStringID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Remove
//	Description:	Remove an ID pair with specified ID out of ID map
//  Arguments:		dwID - Integer typed ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SIDMapping::Remove(DWORD dwID)
{
	// Find item index
	int nIndex = FindID(dwID);
	if (nIndex == INT_INVALID)
		return;

	// Remove item at index
	IDMapData.RemoveAt(nIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Clear
//	Description:	Clear all ID map
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SIDMapping::Clear()
{
	IDMapData.RemoveAll();
	IDMapData.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetID
//	Description:	Get an ID from ID map
//  Arguments:		dwID		 - Integer typed ID
//					lpszStringID - String typed ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

UINT SIDMapping::GetID(LPCSTR lpszStringID) const
{
	int nIndex = FindID(lpszStringID);
	if (nIndex == INT_INVALID)
		return INT_NULL;
	
	return IDMapData.GetAt(nIndex).dwResourceID;
}

LPCSTR SIDMapping::GetID(DWORD dwID) const
{
	int nIndex = FindID(dwID);
	if (nIndex == INT_INVALID)
		return "";
	
	return IDMapData.GetAt(nIndex).lpszStringID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FindID
//	Description:	Return position of the first item with specific ID
//					return -1 if ID not found
//  Arguments:		nID			 - Integer typed ID
//					lpszStringID - String typed ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

int SIDMapping::FindID(DWORD dwID) const
{
	int nRes = INT_INVALID;
	for (int nIndex = 0; nIndex < GetMapCount(); nIndex++) {
		if (IDMapData.GetAt(nIndex).dwResourceID == dwID) {
			nRes = nIndex;	// Index found
			break;
		}
	}

	return nRes;
}

int SIDMapping::FindID(LPCSTR lpszStringID) const
{
	int nRes = INT_INVALID;
	for (int nIndex = 0; nIndex < GetMapCount(); nIndex++) {
		if (strcmp(IDMapData.GetAt(nIndex).lpszStringID, lpszStringID) == 0) {
			nRes = nIndex;	// Index found
			break;
		}
	}

	return nRes;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMapCount
//	Description:	Return the number of elements of IDMap
//  Arguments:		None
//  Return value:	int - Number of ID map items
//
//////////////////////////////////////////////////////////////////////////

inline int SIDMapping::GetMapCount(void) const
{
	return (int)IDMapData.GetSize();
}
