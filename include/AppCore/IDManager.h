﻿/**
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
		SResourceIDMap::GetResourceIDMap()

// Destroy and clean-up application-managed resource ID map data
#define DESTROY_RESOURCEID_MAP()	\
		SResourceIDMap::DestroyResourceIDMap();

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
		if (GET_RESOURCEID_MAP()->FindResourceID(resourceID) == -1) \
			GET_RESOURCEID_MAP()->Add(typeID, resourceID, nameID);

// Modify resource name ID
#define MODIFY_RESOURCE_ID(resourceID, newNameID) \
		GET_RESOURCEID_MAP()->Modify(resourceID, newNameID);

// Remove resource ID from map
#define REMOVE_RESOURCE_ID(resourceID) \
		GET_RESOURCEID_MAP()->Remove(resourceID);

// Get control name ID from map
#define GET_RESOURCE_ID(nameID) \
		GET_RESOURCEID_MAP()->GetResourceID(nameID)

// Get resource name ID from map
#define GET_NAME_ID(resourceID) \
		GET_RESOURCEID_MAP()->GetNameID(resourceID)

// Clean-up resource ID map data
#define CLEAR_RESOURCE_ID_MAP() \
		GET_RESOURCEID_MAP()->RemoveAll();

// End the sequece of updating class resource ID map data
#define END_RESOURCEID_MAP() \
		{ Resource_Null, 0, "#NULL" } \
		}; \
		__pragma(warning(pop)) \
		size_t _resourceIDMapCount = 0;	\
		SResourceIDMap* _pResourceIDMap = GET_RESOURCEID_MAP(); \
		ASSERT(_pResourceIDMap != NULL); \
		if (_pResourceIDMap != NULL) \
		{ \
			size_t _srcMapSize = sizeof(_mapEntries) / sizeof(_mapEntries[0]); \
			_pResourceIDMap->Append(&_mapEntries[0], --(_srcMapSize)); \
			_resourceIDMapCount = _pResourceIDMap->GetMapCount(); \
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
	byte	 byTypeID;					// Resource type ID
	DWORD	 dwResourceID;				// Resource ID (integer type)
	CStringA strNameID;					// Resource mapped name string ID
};

// Define new typename for Resource ID Map data
using RESOURCE_ID_MAP = RESOURCE_ID_MAP_ENTRY*;


// For resource ID mapping function
class SResourceIDMap : public CObject
{
	DECLARE_DYNAMIC(SResourceIDMap)

private:
	// Data container
	RESOURCE_ID_MAP m_pIDMapData;
	size_t			m_nSize;

	// Single instance and thread safety guard
	static SResourceIDMap*	m_thisInstance;
	static std::mutex		m_mutexLockGuard;

private:
	// Singleton
	SResourceIDMap();												// constructor
	SResourceIDMap(const SResourceIDMap&) = delete;					// no copy constructor
	~SResourceIDMap();												// destructor

public:
	// Operators
	SResourceIDMap& operator=(const SResourceIDMap&) = delete;		// no copy assignment operator
	const RESOURCE_ID_MAP_ENTRY& operator[](size_t nIndex);

public:
	// Get the single map instance:
	// Because the resource ID map will be applied for the entire program,
	// there must be one and only instance of it
	static SResourceIDMap* GetResourceIDMap(void);
	static void DestroyResourceIDMap(void);

	// Initialization
	void Copy(const RESOURCE_ID_MAP_ENTRY* pSrc, size_t nSize);
	void Append(const RESOURCE_ID_MAP_ENTRY* pSrc, size_t nSize);

	// Data processing functions
	void Add(byte byTypeID, DWORD dwResID, const char* lpszNameID);
	void Modify(DWORD dwResID, const char* lpszNewNameID);
	void Remove(DWORD dwResID);
	void RemoveAll(void);
	
	// Data acquirement functions
	unsigned	GetResourceID(const char* lpszNameID) const;
	const char*	GetNameID(DWORD dwResID) const;
	long long	FindResourceID(DWORD dwResID) const;
	long long	FindNameID(const char* lpszNameID) const;

	// Attributes get/set functions
	const RESOURCE_ID_MAP_ENTRY& GetAt(size_t nIndex) const;
	size_t GetMapCount(void) const;
};
