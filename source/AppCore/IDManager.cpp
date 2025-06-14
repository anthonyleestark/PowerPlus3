/**
 * @file		IDManager.cpp
 * @brief		Implement methods for components and items ID management
 * @author		AnthonyLeeStark
 * @date		2024.02.25
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "AppCore/IDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//	Implement methods for SResourceIDMap
IMPLEMENT_DYNAMIC(SResourceIDMap, CObject)


//	Initialize static members for SResourceIDMap
SResourceIDMap* SResourceIDMap::m_thisInstance = NULL;
std::mutex		SResourceIDMap::m_mutexLockGuard;


/**
 * @brief	Constructor
 */
SResourceIDMap::SResourceIDMap() : CObject()
{
	// Initialization
	m_pIDMapData = NULL;
	m_nSize = 0;
}

/**
 * @brief	Destructor
 */
SResourceIDMap::~SResourceIDMap()
{
	// Clean-up data
	RemoveAll();
}

/**
 * @brief	Return the resource ID map entry at specific given index
 * @param	nIndex - Index of resource ID map entry to get
 * @return	const RESOURCE_ID_MAP_ENTRY&
 */
const RESOURCE_ID_MAP_ENTRY& SResourceIDMap::operator[](size_t nIndex)
{
	return this->GetAt(nIndex);
}

/**
 * @brief	Get the single instance of resource ID map
 * @param	None
 * @return	SResourceIDMap*
 */
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

/**
 * @brief	Clean-up resource ID map data
					and destroy the single instance of it
 * @param	None
 * @return	None
 */
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

/**
 * @brief	Copy another resource ID map data to current map data
 * @param	pSrc  - Source data pointer
 * @param	nSize - Source data size
 * @return	None
 */
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

/**
 * @brief	Append another resource ID map data into current map data
 * @param	pSrc - Source data pointer
 * @param	nSize - Source data size
 * @return	None
 */
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

/**
 * @brief	Add a resource ID map entry to resource ID map data
 * @param	byTypeID   - Resource type ID
 * @param	dwResID	   - Resource ID
 * @param	lpszNameID - Resource name string ID
 * @return	None
 */
void SResourceIDMap::Add(byte byTypeID, DWORD dwResID, const char* lpszNameID)
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

/**
 * @brief	Modify name ID of specific resource ID item in resource ID map
 * @param	dwResID		  - Resource ID
 * @param	lpszNewNameID - New resource name string ID
 * @return	None
 */
void SResourceIDMap::Modify(DWORD dwResID, const char* lpszNewNameID)
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

/**
 * @brief	Remove a resource ID map entry with specified ID out of
					the current resource ID map
 * @param	dwResID	- Resource ID
 * @return	None
 */
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

/**
 * @brief	Remove all resource ID map data
 * @param	None
 * @return	None
 */
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

/**
 * @brief	Get resource ID from given name string ID
 * @param	lpszNameID - Resource name string ID
 * @return	None
 */
unsigned SResourceIDMap::GetResourceID(const char* lpszNameID) const
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

/**
 * @brief	Get name string ID from given resource ID
 * @param	dwResID	- Resource ID
 * @return	None
 */
const char* SResourceIDMap::GetNameID(DWORD dwResID) const
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

/**
 * @brief	Return index of the first item with specific resource ID
					and return -1 if resource ID is not found
 * @param	dwResID - Resource ID
 * @return	long long
 */
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

/**
 * @brief	Return index of the first item with specific name ID
					and return -1 if name ID is not found
					lpszNameID - Name string ID
 * @return	long long
 */
long long SResourceIDMap::FindNameID(const char* lpszNameID) const
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

/**
 * @brief	Return the resource ID map entry at specific given index
 * @param	nIndex - Index of resource ID map entry to get
 * @return	const RESOURCE_ID_MAP_ENTRY&
 */
const RESOURCE_ID_MAP_ENTRY& SResourceIDMap::GetAt(size_t nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < m_nSize));
	if ((nIndex >= 0) && (nIndex < m_nSize)) {
		return m_pIDMapData[nIndex];
	}

	// Invalid argument
	AfxThrowInvalidArgException();
}

/**
 * @brief	Return the number of elements of resource ID map
 * @param	None
 * @return	size_t - Number of resource ID map entries
 */
size_t SResourceIDMap::GetMapCount(void) const
{
	return m_nSize;
}
