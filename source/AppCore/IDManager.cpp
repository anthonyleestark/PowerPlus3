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
	m_idMapData = NULL;
	m_size = 0;
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
 * @param	index - Index of resource ID map entry to get
 * @return	const RESOURCE_ID_MAP_ENTRY&
 */
const RESOURCE_ID_MAP_ENTRY& SResourceIDMap::operator[](size_t index)
{
	return this->GetAt(index);
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
 * @brief	Clean-up resource ID map data and destroy the single instance of it
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
 * @param	src  - Source data pointer
 * @param	size - Source data size
 * @return	None
 */
void SResourceIDMap::Copy(const RESOURCE_ID_MAP_ENTRY* src, size_t size)
{
	// Check source data validity
	ASSERT((src != NULL) && (size > 0));
	if ((src == NULL) || (size <= 0))
		return;

	// If the destination data is not empty
	if (m_idMapData != NULL) {

		// Do not copy itself
		if (m_idMapData == src)
			return;

		// Clean it up to prepare for copying
		delete[] m_idMapData;
		m_idMapData = NULL;
	}

	// Allocated and initialize destination data
	m_idMapData = new RESOURCE_ID_MAP_ENTRY[size];
	ASSERT(m_idMapData != NULL);

	// Copy data
	CopyElements(m_idMapData, src, size);
	m_size = size;
}

/**
 * @brief	Append another resource ID map data into current map data
 * @param	src - Source data pointer
 * @param	size - Source data size
 * @return	None
 */
void SResourceIDMap::Append(const RESOURCE_ID_MAP_ENTRY* src, size_t size)
{
	// Check source data validity
	ASSERT((src != NULL) && (size > 0));
	if ((src == NULL) || (size <= 0))
		return;

	// If the destination data is not allocated, copy data
	if (m_idMapData == NULL) {
		Copy(src, size);
		return;
	}

	// Filter items in source map which doesn't exist in destination map
	size_t filterCount = 0;
	RESOURCE_ID_MAP filterSrc = new RESOURCE_ID_MAP_ENTRY[size];
	ASSERT(filterSrc != NULL);
	for (size_t index = 0; index < size; index++) {
		if (FindResourceID(src[index].resourceID) == INT_INVALID) {
			// Copy item into filter map
			filterSrc[filterCount] = src[index];
			// Increase filter map count
			filterCount++;
		}
	}

	// If filter map is empty (which means all items already existed), do not append
	if ((filterSrc == NULL) || (filterCount <= 0))
		return;

	// Create new map data
	size_t newSize = m_size + filterCount;
	RESOURCE_ID_MAP newMapData = new RESOURCE_ID_MAP_ENTRY[newSize];
	ASSERT(newMapData != NULL);

	// Copy current map data to new map data
	CopyElements(newMapData, m_idMapData, m_size);

	// Append filtered source map data to new map data
	CopyElements(newMapData + m_size, filterSrc, filterCount);
	delete[] filterSrc;

	// Copy data back
	delete[] m_idMapData;
	m_idMapData = newMapData;
	m_size = newSize;
}

/**
 * @brief	Add a resource ID map entry to resource ID map data
 * @param	typeID   - Resource type ID
 * @param	resID	   - Resource ID
 * @param	nameID - Resource name string ID
 * @return	None
 */
void SResourceIDMap::Add(byte typeID, DWORD resID, const char* nameID)
{
	// Check data validity
	ASSERT(m_idMapData != NULL);
	if (m_idMapData == NULL)
		return;

	// Create a new clone map data
	int newSize = m_size + 1;
	RESOURCE_ID_MAP newMapData = new RESOURCE_ID_MAP_ENTRY[newSize];
	ASSERT(newMapData != NULL);
	CopyElements(newMapData, m_idMapData, m_size);

	// Add new control ID map entry
	newMapData[m_size].typeID = typeID;
	newMapData[m_size].resourceID = resID;
	newMapData[m_size].nameID = nameID;

	// Copy data back
	delete[] m_idMapData;
	m_idMapData = newMapData;
	m_size = newSize;
}

/**
 * @brief	Modify name ID of specific resource ID item in resource ID map
 * @param	resID		  - Resource ID
 * @param	newNameID - New resource name string ID
 * @return	None
 */
void SResourceIDMap::Modify(DWORD resID, const char* newNameID)
{
	// Check data validity
	ASSERT(m_idMapData != NULL);
	if (m_idMapData == NULL)
		return;

	// Find item index
	int index = FindResourceID(resID);
	if (index == INT_INVALID)
		return;
	
	// Modify item at index
	m_idMapData[index].nameID = newNameID;
}

/**
 * @brief	Remove a resource ID map entry with specified ID out of
					the current resource ID map
 * @param	dwResID	- Resource ID
 * @return	None
 */
void SResourceIDMap::Remove(DWORD resID)
{
	// Check data validity
	ASSERT(m_idMapData != NULL);
	if (m_idMapData == NULL)
		return;

	// Find item index
	int itemIndex = FindResourceID(resID);
	if (itemIndex == INT_INVALID)
		return;

	// Create a new clone map data
	size_t newSize = (m_size - 1);
	RESOURCE_ID_MAP newMapData = new RESOURCE_ID_MAP_ENTRY[m_size];
	ASSERT(newMapData != NULL);

	// Copy old data to new data
	// except for the item at the index we need to remove
	size_t destIdx = 0;
	for (size_t srcIdx = 0; srcIdx < m_size; srcIdx++) {
		if (srcIdx == itemIndex) continue;
		newMapData[destIdx] = m_idMapData[srcIdx];
		destIdx++;
	}

	// Copy data back
	delete[] m_idMapData;
	m_idMapData = newMapData;
	m_size = newSize;
}

/**
 * @brief	Remove all resource ID map data
 * @param	None
 * @return	None
 */
void SResourceIDMap::RemoveAll(void)
{
	// Check data validity
	if (m_idMapData == NULL)
		return;

	// Clean-up data
	delete[] m_idMapData;
	m_idMapData = NULL;
}

/**
 * @brief	Get resource ID from given name string ID
 * @param	nameID - Resource name string ID
 * @return	None
 */
unsigned SResourceIDMap::GetResourceID(const char* nameID) const
{
	// Check data validity
	ASSERT(m_idMapData != NULL);
	if (m_idMapData == NULL)
		return INT_NULL;

	// Find index
	int index = FindNameID(nameID);
	if (index == INT_INVALID)
		return INT_NULL;

	// Return control resource ID
	return m_idMapData[index].resourceID;
}

/**
 * @brief	Get name string ID from given resource ID
 * @param	resID	- Resource ID
 * @return	None
 */
const char* SResourceIDMap::GetNameID(DWORD resID) const
{
	// Check data validity
	ASSERT(m_idMapData != NULL);
	if (m_idMapData == NULL)
		return "#NULL";

	// Find index
	int index = FindResourceID(resID);
	if (index == INT_INVALID)
		return "#NULL";

	// Return string ID
	return m_idMapData[index].nameID;
}

/**
 * @brief	Return index of the first item with specific resource ID
					and return -1 if resource ID is not found
 * @param	dwResID - Resource ID
 * @return	long long
 */
long long SResourceIDMap::FindResourceID(DWORD resID) const
{
	// Check data validity
	ASSERT(m_idMapData != NULL);
	if (m_idMapData == NULL)
		return INT_INVALID;

	// Find ID
	long long resIndex = INT_INVALID;
	for (size_t index = 0; index < m_size; index++) {
		if (m_idMapData[index].resourceID == resID) {
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
long long SResourceIDMap::FindNameID(const char* nameID) const
{
	// Check data validity
	ASSERT(m_idMapData != NULL);
	if (m_idMapData == NULL)
		return INT_INVALID;

	// Find ID
	long long resIndex = INT_INVALID;
	for (size_t index = 0; index < m_size; index++) {
		if (strcmp(m_idMapData[index].nameID, nameID) == 0) {
			resIndex = index;	// Index found
			break;
		}
	}
	return resIndex;
}

/**
 * @brief	Return the resource ID map entry at specific given index
 * @param	index - Index of resource ID map entry to get
 * @return	const RESOURCE_ID_MAP_ENTRY&
 */
const RESOURCE_ID_MAP_ENTRY& SResourceIDMap::GetAt(size_t index) const
{
	ASSERT((index >= 0) && (index < m_size));
	if ((index >= 0) && (index < m_size)) {
		return m_idMapData[index];
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
	return m_size;
}
