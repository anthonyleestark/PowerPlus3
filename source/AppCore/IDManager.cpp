
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		IDManager.cpp
//		Description:	Implement methods for components and items ID management
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.25:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//						<3> 2025.06.03:		Rename from IDMapping to IDManager
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "AppCore/IDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////
/// Implementations


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for SResourceIDMap
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(SResourceIDMap, CObject)


//////////////////////////////////////////////////////////////////////////
//
//	Initialize static members for SResourceIDMap
//
//////////////////////////////////////////////////////////////////////////

SResourceIDMap* SResourceIDMap::m_thisInstance = NULL;
std::mutex		SResourceIDMap::m_mutexLockGuard;


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SResourceIDMap
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

SResourceIDMap::SResourceIDMap() : CObject()
{
	// Initialization
	m_pIDMapData = NULL;
	m_nSize = 0;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~SResourceIDMap
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

SResourceIDMap::~SResourceIDMap()
{
	// Clean-up data
	RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator[]
//	Description:	Return the resource ID map entry at specific given index
//  Arguments:		nIndex - Index of resource ID map entry to get
//  Return value:	const RESOURCE_ID_MAP_ENTRY&
//
//////////////////////////////////////////////////////////////////////////

const RESOURCE_ID_MAP_ENTRY& SResourceIDMap::operator[](size_t nIndex)
{
	return this->GetAt(nIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetResourceIDMap
//	Description:	Get the single instance of resource ID map
//  Arguments:		None
//  Return value:	SResourceIDMap*
//
//////////////////////////////////////////////////////////////////////////

SResourceIDMap* SResourceIDMap::GetResourceIDMap(void)
{
	// Ensure thread safety
	std::lock_guard<std::mutex> lock(m_mutexLockGuard);

	// Get this single instance
	if (m_thisInstance == NULL) {
		m_thisInstance = new SResourceIDMap();
		ASSERT(m_thisInstance != NULL);
	}
	return m_thisInstance;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DestroyResourceIDMap
//	Description:	Clean-up resource ID map data
//					and destroy the single instance of it
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SResourceIDMap::DestroyResourceIDMap(void)
{
	// Ensure instance validity
	ASSERT(m_thisInstance != NULL);
	if (m_thisInstance != NULL) {

		// Remove all map data
		m_thisInstance->RemoveAll();

		// Destroy instance
		delete m_thisInstance;
		m_thisInstance = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy another resource ID map data to current map data
//  Arguments:		pSrc  - Source data pointer
//					nSize - Source data size
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SResourceIDMap::Copy(const RESOURCE_ID_MAP_ENTRY* pSrc, size_t nSize)
{
	// Check source data validity
	ASSERT((pSrc != NULL) && (nSize > 0));
	if ((pSrc == NULL) || (nSize <= 0))
		return;

	// If the destination data is not empty
	if (m_pIDMapData != NULL) {

		// Do not copy itself
		if (m_pIDMapData == pSrc)
			return;

		// Clean it up to prepare for copying
		delete[] m_pIDMapData;
		m_pIDMapData = NULL;
	}

	// Allocated and initialize destination data
	m_pIDMapData = new RESOURCE_ID_MAP_ENTRY[nSize];
	ASSERT(m_pIDMapData != NULL);

	// Copy data
	CopyElements(m_pIDMapData, pSrc, nSize);
	m_nSize = nSize;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Append
//	Description:	Append another resource ID map data into current map data
//  Arguments:		pSrc - Source data pointer
//					nSize - Source data size
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SResourceIDMap::Append(const RESOURCE_ID_MAP_ENTRY* pSrc, size_t nSize)
{
	// Check source data validity
	ASSERT((pSrc != NULL) && (nSize > 0));
	if ((pSrc == NULL) || (nSize <= 0))
		return;

	// If the destination data is not allocated, copy data
	if (m_pIDMapData == NULL) {
		Copy(pSrc, nSize);
		return;
	}

	// Filter items in source map which doesn't exist in destination map
	size_t nFilterCount = 0;
	RESOURCE_ID_MAP pFilterSrc = new RESOURCE_ID_MAP_ENTRY[nSize];
	ASSERT(pFilterSrc != NULL);
	for (size_t nIndex = 0; nIndex < nSize; nIndex++) {
		if (FindResourceID(pSrc[nIndex].dwResourceID) == INT_INVALID) {
			// Copy item into filter map
			pFilterSrc[nFilterCount] = pSrc[nIndex];
			// Increase filter map count
			nFilterCount++;
		}
	}

	// If filter map is empty (which means all items already existed), do not append
	if ((pFilterSrc == NULL) || (nFilterCount <= 0))
		return;

	// Create new map data
	size_t nNewSize = m_nSize + nFilterCount;
	RESOURCE_ID_MAP pNewMapData = new RESOURCE_ID_MAP_ENTRY[nNewSize];
	ASSERT(pNewMapData != NULL);

	// Copy current map data to new map data
	CopyElements(pNewMapData, m_pIDMapData, m_nSize);

	// Append filtered source map data to new map data
	CopyElements(pNewMapData + m_nSize, pFilterSrc, nFilterCount);
	delete[] pFilterSrc;

	// Copy data back
	delete[] m_pIDMapData;
	m_pIDMapData = pNewMapData;
	m_nSize = nNewSize;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add a resource ID map entry to resource ID map data
//  Arguments:		byTypeID   - Resource type ID
//					dwResID	   - Resource ID
//					lpszNameID - Resource name string ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SResourceIDMap::Add(BYTE byTypeID, DWORD dwResID, LPCSTR lpszNameID)
{
	// Check data validity
	ASSERT(m_pIDMapData != NULL);
	if (m_pIDMapData == NULL)
		return;

	// Create a new clone map data
	int nNewSize = m_nSize + 1;
	RESOURCE_ID_MAP pNewMapData = new RESOURCE_ID_MAP_ENTRY[nNewSize];
	ASSERT(pNewMapData != NULL);
	CopyElements(pNewMapData, m_pIDMapData, m_nSize);

	// Add new control ID map entry
	pNewMapData[m_nSize].byTypeID = byTypeID;
	pNewMapData[m_nSize].dwResourceID = dwResID;
	pNewMapData[m_nSize].strNameID = lpszNameID;

	// Copy data back
	delete[] m_pIDMapData;
	m_pIDMapData = pNewMapData;
	m_nSize = nNewSize;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Modify
//	Description:	Modify name ID of specific resource ID item in resource ID map
//  Arguments:		dwResID		  - Resource ID
//					lpszNewNameID - New resource name string ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SResourceIDMap::Modify(DWORD dwResID, LPCSTR lpszNewNameID)
{
	// Check data validity
	ASSERT(m_pIDMapData != NULL);
	if (m_pIDMapData == NULL)
		return;

	// Find item index
	int nIndex = FindResourceID(dwResID);
	if (nIndex == INT_INVALID)
		return;
	
	// Modify item at index
	m_pIDMapData[nIndex].strNameID = lpszNewNameID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Remove
//	Description:	Remove a resource ID map entry with specified ID out of
//					the current resource ID map
//  Arguments:		dwResID	- Resource ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SResourceIDMap::Remove(DWORD dwResID)
{
	// Check data validity
	ASSERT(m_pIDMapData != NULL);
	if (m_pIDMapData == NULL)
		return;

	// Find item index
	int nItemIndex = FindResourceID(dwResID);
	if (nItemIndex == INT_INVALID)
		return;

	// Create a new clone map data
	size_t nNewSize = (m_nSize - 1);
	RESOURCE_ID_MAP pNewMapData = new RESOURCE_ID_MAP_ENTRY[m_nSize];
	ASSERT(pNewMapData != NULL);

	// Copy old data to new data
	// except for the item at the index we need to remove
	size_t nDestIdx = 0;
	for (size_t nSrcIdx = 0; nSrcIdx < m_nSize; nSrcIdx++) {
		if (nSrcIdx == nItemIndex) continue;
		pNewMapData[nDestIdx] = m_pIDMapData[nSrcIdx];
		nDestIdx++;
	}

	// Copy data back
	delete[] m_pIDMapData;
	m_pIDMapData = pNewMapData;
	m_nSize = nNewSize;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all resource ID map data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SResourceIDMap::RemoveAll(void)
{
	// Check data validity
	ASSERT(m_pIDMapData != NULL);
	if (m_pIDMapData == NULL)
		return;

	// Clean-up data
	delete[] m_pIDMapData;
	m_pIDMapData = NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetResourceID
//	Description:	Get resource ID from given name string ID
//  Arguments:		lpszNameID - Resource name string ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

UINT SResourceIDMap::GetResourceID(LPCSTR lpszNameID) const
{
	// Check data validity
	ASSERT(m_pIDMapData != NULL);
	if (m_pIDMapData == NULL)
		return INT_NULL;

	// Find index
	int nIndex = FindNameID(lpszNameID);
	if (nIndex == INT_INVALID)
		return INT_NULL;

	// Return control resource ID
	return m_pIDMapData[nIndex].dwResourceID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetNameID
//	Description:	Get name string ID from given resource ID
//  Arguments:		dwResID	- Resource ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

LPCSTR SResourceIDMap::GetNameID(DWORD dwResID) const
{
	// Check data validity
	ASSERT(m_pIDMapData != NULL);
	if (m_pIDMapData == NULL)
		return "#NULL";

	// Find index
	int nIndex = FindResourceID(dwResID);
	if (nIndex == INT_INVALID)
		return "#NULL";

	// Return string ID
	return m_pIDMapData[nIndex].strNameID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FindResourceID
//	Description:	Return index of the first item with specific resource ID
//					and return -1 if resource ID is not found
//  Arguments:		dwResID - Resource ID
//  Return value:	long long
//
//////////////////////////////////////////////////////////////////////////

long long SResourceIDMap::FindResourceID(DWORD dwResID) const
{
	// Check data validity
	ASSERT(m_pIDMapData != NULL);
	if (m_pIDMapData == NULL)
		return INT_INVALID;

	// Find ID
	long long resIndex = INT_INVALID;
	for (size_t index = 0; index < m_nSize; index++) {
		if (m_pIDMapData[index].dwResourceID == dwResID) {
			resIndex = index;	// Index found
			break;
		}
	}
	return resIndex;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FindNameID
//	Description:	Return index of the first item with specific name ID
//					and return -1 if name ID is not found
//					lpszNameID - Name string ID
//  Return value:	long long
//
//////////////////////////////////////////////////////////////////////////

long long SResourceIDMap::FindNameID(LPCSTR lpszNameID) const
{
	// Check data validity
	ASSERT(m_pIDMapData != NULL);
	if (m_pIDMapData == NULL)
		return INT_INVALID;

	// Find ID
	long long resIndex = INT_INVALID;
	for (size_t index = 0; index < m_nSize; index++) {
		if (strcmp(m_pIDMapData[index].strNameID, lpszNameID) == 0) {
			resIndex = index;	// Index found
			break;
		}
	}
	return resIndex;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAt
//	Description:	Return the resource ID map entry at specific given index
//  Arguments:		nIndex - Index of resource ID map entry to get
//  Return value:	const RESOURCE_ID_MAP_ENTRY&
//
//////////////////////////////////////////////////////////////////////////

const RESOURCE_ID_MAP_ENTRY& SResourceIDMap::GetAt(size_t nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < m_nSize));
	if ((nIndex >= 0) && (nIndex < m_nSize)) {
		return m_pIDMapData[nIndex];
	}

	// Invalid argument
	AfxThrowInvalidArgException();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMapCount
//	Description:	Return the number of elements of resource ID map
//  Arguments:		None
//  Return value:	size_t - Number of resource ID map entries
//
//////////////////////////////////////////////////////////////////////////

size_t SResourceIDMap::GetMapCount(void) const
{
	return m_nSize;
}
