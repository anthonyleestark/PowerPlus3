/**
 * @file		IDManager.h
 * @brief		Define variables and methods for components and items ID management
 * @author		AnthonyLeeStark
 * @date		2024.02.25
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore.h"


// Get application-managed resource ID map data pointer
#define GET_RESOURCEID_MAP()	\
		SResourceIDMap::getResourceIdMap()

// Destroy and clean-up application-managed resource ID map data
#define DESTROY_RESOURCEID_MAP()	\
		SResourceIDMap::destroyResourceIdMap();

// Declare descendant-class-level resource ID map
#define DECLARE_RESOURCEID_MAP()	\
	protected: \
		static const size_t PASCAL UpdateThisResourceIDMap(); \
		const size_t UpdateResourceIDMap() override;

// Begin the sequece of updating class resource ID map data
#define BEGIN_RESOURCEID_MAP(theClass) \
	PTM_WARNING_DISABLE \
	const size_t theClass::UpdateResourceIDMap() \
	{ \
		return UpdateThisResourceIDMap(); \
	} \
	const size_t theClass::UpdateThisResourceIDMap() \
	{ \
		__pragma(warning(push))	\
		__pragma(warning(disable: 4640)) \
		static const RESOURCE_ID_MAP_ENTRY _mapEntries[] = \
		{

// Making entry of dialog ID
#define ON_ID_DIALOG(resourceID, nameID) \
		{ \
			Dialog, resourceID, nameID \
		},

// Making entry of control ID
#define ON_ID_CONTROL(resourceID, nameID) \
		{ \
			Control, resourceID, nameID \
		},

// Making entry of menu item ID
#define ON_ID_MENU(resourceID, nameID) \
		{ \
			Menu, resourceID, nameID \
		},

// Add resource ID to map
#define ADD_RESOURCE_ID(typeID, resourceID, nameID) \
		if (GET_RESOURCEID_MAP()->findResourceId(resourceID) == -1) \
			GET_RESOURCEID_MAP()->add(typeID, resourceID, nameID);

// Modify resource name ID
#define MODIFY_RESOURCE_ID(resourceID, newNameID) \
		GET_RESOURCEID_MAP()->modify(resourceID, newNameID);

// Remove resource ID from map
#define REMOVE_RESOURCE_ID(resourceID) \
		GET_RESOURCEID_MAP()->remove(resourceID);

// Get control name ID from map
#define GET_RESOURCE_ID(nameID) \
		GET_RESOURCEID_MAP()->getResourceId(nameID)

// Get resource name ID from map
#define GET_NAME_ID(resourceID) \
		GET_RESOURCEID_MAP()->getNameId(resourceID)

// Clean-up resource ID map data
#define CLEAR_RESOURCE_ID_MAP() \
		GET_RESOURCEID_MAP()->removeAll();

// End the sequece of updating class resource ID map data
#define END_RESOURCEID_MAP() \
		{ Resource_Null, 0, "#NULL" } \
		}; \
		__pragma(warning(pop)) \
		size_t _resourceIDMapCount = 0;	\
		SResourceIDMap* _resourceIDMap = GET_RESOURCEID_MAP(); \
		ASSERT(_resourceIDMap != NULL); \
		if (_resourceIDMap != NULL) \
		{ \
			size_t _srcMapSize = sizeof(_mapEntries) / sizeof(_mapEntries[0]); \
			_resourceIDMap->append(&_mapEntries[0], --(_srcMapSize)); \
			_resourceIDMapCount = _resourceIDMap->getMapCount(); \
		} \
		return _resourceIDMapCount; \
	} \
	PTM_WARNING_RESTORE


// Resource type IDs
enum ResourceType {
	Resource_Null = 0,					// Invalid resource type
	Dialog,								// Dialog
	Control,							// Control
	Menu,								// Menu item
};


// For resource ID mapping function
struct RESOURCE_ID_MAP_ENTRY
{
	byte	 typeID;					// Resource type ID
	DWORD	 resourceID;				// Resource ID (integer type)
	CStringA nameID;					// Resource mapped name string ID
};

// Define new typename for Resource ID Map data
using RESOURCE_ID_MAP = RESOURCE_ID_MAP_ENTRY*;


// For resource ID mapping function
class SResourceIDMap : public CObject
{
	DECLARE_DYNAMIC(SResourceIDMap)

private:
	// Data container
	RESOURCE_ID_MAP idMapData_;
	size_t			mapSize_;

	// Single instance and thread safety guard
	static SResourceIDMap*	resourceIdMapInstance_;
	static std::mutex		mutexLockGuard_;

private:
	// Singleton
	SResourceIDMap();												// constructor
	SResourceIDMap(const SResourceIDMap&) = delete;					// no copy constructor
	~SResourceIDMap();												// destructor

public:
	// Operators
	SResourceIDMap& operator=(const SResourceIDMap&) = delete;		// no copy assignment operator
	const RESOURCE_ID_MAP_ENTRY& operator[](size_t index);

public:
	// Get the single map instance:
	// Because the resource ID map will be applied for the entire program,
	// there must be one and only instance of it
	static SResourceIDMap* getResourceIdMap(void);
	static void destroyResourceIdMap(void);

	// Initialization
	void copy(const RESOURCE_ID_MAP_ENTRY* src, size_t size);
	void append(const RESOURCE_ID_MAP_ENTRY* src, size_t size);

	// Data processing functions
	void add(byte typeID, DWORD resID, const char* nameID);
	void modify(DWORD resID, const char* newNameID);
	void remove(DWORD resID);
	void removeAll(void);
	
	// Data acquirement functions
	unsigned	getResourceId(const char* nameID) const;
	const char*	getNameId(DWORD resID) const;
	int64		findResourceId(DWORD resID) const;
	int64		findNameID(const char* nameID) const;

	// Attributes get/set functions
	const RESOURCE_ID_MAP_ENTRY& getAt(size_t index) const;
	size_t getMapCount(void) const;
};
