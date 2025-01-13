
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		IDMapping.h
//		Description:	Define variables and methods for components and items ID mapping
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.25:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IDMAPPING_H_INCLUDED
#define _IDMAPPING_H_INCLUDED

#include "stdafx.h"
#include "Core.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define macros for ID Mapping, these macros will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DECLARE_APP_IDMAP()	\
	public: \
		static SIDMapping* _pAppIDMap;	\
		static SIDMapping* GetAppIDMap();
#define INIT_APP_IDMAP(theAppClass)	\
	PTM_WARNING_DISABLE \
	SIDMapping* theAppClass::_pAppIDMap = new SIDMapping; \
	__pragma(warning(push)) \
	__pragma(warning(disable: 4640)) \
	SIDMapping* theAppClass::GetAppIDMap() { \
		VERIFY (_pAppIDMap != NULL); \
		return theAppClass::_pAppIDMap; \
	} \
	PTM_WARNING_RESTORE
#define DESTROY_APP_IDMAP()	\
		if (_pAppIDMap != NULL) { \
			delete _pAppIDMap; \
			_pAppIDMap = NULL; \
		}
#define DECLARE_CLASS_IDMAP()	\
	protected: \
		static void UpdateClassIDMap();
#define INIT_CLASS_IDMAP() \
	UpdateClassIDMap();
#define BEGIN_ID_MAPPING(theClass) \
	PTM_WARNING_DISABLE \
	void theClass::UpdateClassIDMap() {	\
		SIDMapping *_classIDMap = ((SWinApp*)AfxGetApp())->GetAppIDMap(); \
		if (_classIDMap == NULL) return;
#define IDMAP_ADD(nID, lpszStringID) \
		if (_classIDMap->FindID(nID) == -1) _classIDMap->Add(nID, lpszStringID);
#define IDMAP_MODIFY(nID, lpszStringID) \
		((SWinApp*)AfxGetApp())->GetAppIDMap()->Modify(nID, lpszStringID);
#define IDMAP_REMOVE(nID) \
		((SWinApp*)AfxGetApp())->GetAppIDMap()->Remove(nID);
#define IDMAP_GET \
		((SWinApp*)AfxGetApp())->GetAppIDMap()->GetID
#define IDMAP_CLEAR() \
		((SWinApp*)AfxGetApp())->GetAppIDMap()->Clear();
#define END_ID_MAPPING() \
	} \
	PTM_WARNING_RESTORE


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define data types for ID mapping
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagIDMAPPAIR
{
	DWORD	dwResourceID;	// Resource ID (integer type)
	LPCSTR  lpszStringID;	// String ID
} IDMAPPAIR, *PIDMAPPAIR;

typedef CArray<IDMAPPAIR, IDMAPPAIR>	IDMAPDATA;


////////////////////////////////////////////////////////
//
//	Class name:	 SIDMapping
//  Description: Using for app ID mapping function
//
////////////////////////////////////////////////////////

class SIDMapping
{
private:
	// Data container
	IDMAPDATA IDMapData;

public:
	// Member functions
	void Add(UINT nID, LPCSTR lpszStringID);
	void Modify(UINT nID, LPCSTR lpszStringID);
	void Remove(UINT nID);
	void Clear();
	
	UINT	GetID(LPCSTR lpszStringID);
	LPCSTR	GetID(UINT nID);
	int		FindID(UINT nID);
	int		FindID(LPCSTR lpszStringID);
	int		GetMapCount();
};

#endif	ifndef _IDMAPPING_H_INCLUDED