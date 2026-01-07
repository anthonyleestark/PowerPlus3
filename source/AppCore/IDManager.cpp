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
SResourceIDMap* SResourceIDMap::resourceIdMapInstance_ = NULL;
std::mutex		SResourceIDMap::mutexLockGuard_;


/**
 * @brief	Constructor
 */
SResourceIDMap::SResourceIDMap() : CObject()
{
	// Initialization
	idMapData_ = NULL;
	mapSize_ = 0;
}

/**
 * @brief	Destructor
 */
SResourceIDMap::~SResourceIDMap()
{
	// Clean-up data
	removeAll();
}

/**
 * @brief	Return the resource ID map entry at specific given index
 * @param	index - Index of resource ID map entry to get
 * @return	const RESOURCE_ID_MAP_ENTRY&
 */
const RESOURCE_ID_MAP_ENTRY& SResourceIDMap::operator[](size_t index)
{
	return this->getAt(index);
}

/**
 * @brief	Get the single instance of resource ID map
 * @param	None
 * @return	SResourceIDMap*
 */
SResourceIDMap* SResourceIDMap::getResourceIdMap(void)
{
	// Ensure thread safety
	std::lock_guard<std::mutex> lock(mutexLockGuard_);

	// Get this single instance
	if (resourceIdMapInstance_ == NULL) {
		resourceIdMapInstance_ = new SResourceIDMap();
		ASSERT(resourceIdMapInstance_ != NULL);
	}
	return resourceIdMapInstance_;
}

/**
 * @brief	Clean-up resource ID map data and destroy the single instance of it
 * @param	None
 * @return	None
 */
void SResourceIDMap::destroyResourceIdMap(void)
{
	// Ensure instance validity
	ASSERT(resourceIdMapInstance_ != NULL);
	if (resourceIdMapInstance_ != NULL) {

		// Remove all map data
		resourceIdMapInstance_->removeAll();

		// Destroy instance
		delete resourceIdMapInstance_;
		resourceIdMapInstance_ = NULL;
	}
}

/**
 * @brief	Copy another resource ID map data to current map data
 * @param	src  - Source data pointer
 * @param	size - Source data size
 * @return	None
 */
void SResourceIDMap::copy(const RESOURCE_ID_MAP_ENTRY* src, size_t size)
{
	// Check source data validity
	ASSERT((src != NULL) && (size > 0));
	if ((src == NULL) || (size <= 0))
		return;

	// If the destination data is not empty
	if (idMapData_ != NULL) {

		// Do not copy itself
		if (idMapData_ == src)
			return;

		// Clean it up to prepare for copying
		delete[] idMapData_;
		idMapData_ = NULL;
	}

	// Allocated and initialize destination data
	idMapData_ = new RESOURCE_ID_MAP_ENTRY[size];
	ASSERT(idMapData_ != NULL);

	// Copy data
	CopyElements(idMapData_, src, size);
	mapSize_ = size;
}

/**
 * @brief	Append another resource ID map data into current map data
 * @param	src - Source data pointer
 * @param	size - Source data size
 * @return	None
 */
void SResourceIDMap::append(const RESOURCE_ID_MAP_ENTRY* src, size_t size)
{
	// Check source data validity
	ASSERT((src != NULL) && (size > 0));
	if ((src == NULL) || (size <= 0))
		return;

	// If the destination data is not allocated, copy data
	if (idMapData_ == NULL) {
		copy(src, size);
		return;
	}

	// Filter items in source map which doesn't exist in destination map
	size_t filterCount = 0;
	RESOURCE_ID_MAP filterSrc = new RESOURCE_ID_MAP_ENTRY[size];
	ASSERT(filterSrc != NULL);
	for (size_t index = 0; index < size; index++) {
		if (findResourceId(src[index].resourceID) == Constant::kInvalidInteger) {
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
	size_t newSize = mapSize_ + filterCount;
	RESOURCE_ID_MAP newMapData = new RESOURCE_ID_MAP_ENTRY[newSize];
	ASSERT(newMapData != NULL);

	// Copy current map data to new map data
	CopyElements(newMapData, idMapData_, mapSize_);

	// Append filtered source map data to new map data
	CopyElements(newMapData + mapSize_, filterSrc, filterCount);
	delete[] filterSrc;

	// Copy data back
	delete[] idMapData_;
	idMapData_ = newMapData;
	mapSize_ = newSize;
}

/**
 * @brief	Add a resource ID map entry to resource ID map data
 * @param	typeID   - Resource type ID
 * @param	resID	   - Resource ID
 * @param	nameID - Resource name string ID
 * @return	None
 */
void SResourceIDMap::add(byte typeID, DWORD resID, const char* nameID)
{
	// Check data validity
	ASSERT(idMapData_ != NULL);
	if (idMapData_ == NULL)
		return;

	// Create a new clone map data
	int newSize = mapSize_ + 1;
	RESOURCE_ID_MAP newMapData = new RESOURCE_ID_MAP_ENTRY[newSize];
	ASSERT(newMapData != NULL);
	CopyElements(newMapData, idMapData_, mapSize_);

	// Add new control ID map entry
	newMapData[mapSize_].typeID = typeID;
	newMapData[mapSize_].resourceID = resID;
	newMapData[mapSize_].nameID = nameID;

	// Copy data back
	delete[] idMapData_;
	idMapData_ = newMapData;
	mapSize_ = newSize;
}

/**
 * @brief	Modify name ID of specific resource ID item in resource ID map
 * @param	resID		  - Resource ID
 * @param	newNameID - New resource name string ID
 * @return	None
 */
void SResourceIDMap::modify(DWORD resID, const char* newNameID)
{
	// Check data validity
	ASSERT(idMapData_ != NULL);
	if (idMapData_ == NULL)
		return;

	// Find item index
	int index = findResourceId(resID);
	if (index == Constant::kInvalidInteger)
		return;
	
	// Modify item at index
	idMapData_[index].nameID = newNameID;
}

/**
 * @brief	Remove a resource ID map entry with specified ID out of
					the current resource ID map
 * @param	dwResID	- Resource ID
 * @return	None
 */
void SResourceIDMap::remove(DWORD resID)
{
	// Check data validity
	ASSERT(idMapData_ != NULL);
	if (idMapData_ == NULL)
		return;

	// Find item index
	int itemIndex = findResourceId(resID);
	if (itemIndex == Constant::kInvalidInteger)
		return;

	// Create a new clone map data
	size_t newSize = (mapSize_ - 1);
	RESOURCE_ID_MAP newMapData = new RESOURCE_ID_MAP_ENTRY[mapSize_];
	ASSERT(newMapData != NULL);

	// Copy old data to new data
	// except for the item at the index we need to remove
	size_t destIdx = 0;
	for (size_t srcIdx = 0; srcIdx < mapSize_; srcIdx++) {
		if (srcIdx == itemIndex) continue;
		newMapData[destIdx] = idMapData_[srcIdx];
		destIdx++;
	}

	// Copy data back
	delete[] idMapData_;
	idMapData_ = newMapData;
	mapSize_ = newSize;
}

/**
 * @brief	Remove all resource ID map data
 * @param	None
 * @return	None
 */
void SResourceIDMap::removeAll(void)
{
	// Check data validity
	if (idMapData_ == NULL)
		return;

	// Clean-up data
	delete[] idMapData_;
	idMapData_ = NULL;
}

/**
 * @brief	Get resource ID from given name string ID
 * @param	nameID - Resource name string ID
 * @return	None
 */
unsigned SResourceIDMap::getResourceId(const char* nameID) const
{
	// Check data validity
	ASSERT(idMapData_ != NULL);
	if (idMapData_ == NULL)
		return Constant::kNullInteger;

	// Find index
	int index = findNameID(nameID);
	if (index == Constant::kInvalidInteger)
		return Constant::kNullInteger;

	// Return control resource ID
	return idMapData_[index].resourceID;
}

/**
 * @brief	Get name string ID from given resource ID
 * @param	resID	- Resource ID
 * @return	None
 */
const char* SResourceIDMap::getNameId(DWORD resID) const
{
	// Check data validity
	ASSERT(idMapData_ != NULL);
	if (idMapData_ == NULL)
		return "#NULL";

	// Find index
	int index = findResourceId(resID);
	if (index == Constant::kInvalidInteger)
		return "#NULL";

	// Return string ID
	return idMapData_[index].nameID;
}

/**
 * @brief	Return index of the first item with specific resource ID
					and return -1 if resource ID is not found
 * @param	dwResID - Resource ID
 * @return	int64
 */
int64 SResourceIDMap::findResourceId(DWORD resID) const
{
	// Check data validity
	ASSERT(idMapData_ != NULL);
	if (idMapData_ == NULL)
		return Constant::kInvalidInteger;

	// Find ID
	int64 resIndex = Constant::kInvalidInteger;
	for (size_t index = 0; index < mapSize_; index++) {
		if (idMapData_[index].resourceID == resID) {
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
 * @return	int64
 */
int64 SResourceIDMap::findNameID(const char* nameID) const
{
	// Check data validity
	ASSERT(idMapData_ != NULL);
	if (idMapData_ == NULL)
		return Constant::kInvalidInteger;

	// Find ID
	int64 resIndex = Constant::kInvalidInteger;
	for (size_t index = 0; index < mapSize_; index++) {
		if (strcmp(idMapData_[index].nameID, nameID) == 0) {
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
const RESOURCE_ID_MAP_ENTRY& SResourceIDMap::getAt(size_t index) const
{
	ASSERT((index >= 0) && (index < mapSize_));
	if ((index >= 0) && (index < mapSize_)) {
		return idMapData_[index];
	}

	// Invalid argument
	AfxThrowInvalidArgException();
}

/**
 * @brief	Return the number of elements of resource ID map
 * @param	None
 * @return	size_t - Number of resource ID map entries
 */
size_t SResourceIDMap::getMapCount(void) const
{
	return mapSize_;
}
