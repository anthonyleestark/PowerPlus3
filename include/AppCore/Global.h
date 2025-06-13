/**
 * @file		Global.h
 * @brief		Define global data and objects which will be used everywhere in program
 * @author		AnthonyLeeStark
 * @date		2024.01.27
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore.h"


// Define categories of global data variables
#define DEF_GLBDATA_CATE_NONE			0x00			// No category (main profile section)
#define DEF_GLBDATA_CATE_DEBUGTEST		0x01			// Debugging/Testing variables
#define DEF_GLBDATA_CATE_APPFLAGS		0x02			// App special flags
#define DEF_GLBDATA_CATE_FEATURES		0x03			// Special feature variables
#define DEF_GLBDATA_CATE_TRACKING		0x04			// Tracking data variables
#define DEF_GLBDATA_CATE_OTHERS			0x05			// Other special variables


/*---------------------------------------------Launch-info data----------------------------------------------*/

namespace Global {
	extern DateTime g_stAppLaunchTime;
	extern UINT	g_uiAppLaunchTimeCounter;
};

// Access application launch-time data
static inline const DateTime& GetAppLaunchTime(void);
static inline void SetAppLaunchTime(const DateTime& stTime);

// Access application launch-time counter data
static inline const UINT GetAppLaunchTimeCounter(void);
static inline const BOOL IsAppFirstLaunch(void);
static inline void SetAppLaunchTimeCounter(UINT uiValue);
static inline void UpdateAppLaunchTimeCounter(void);

/*-----------------------------------------------------------------------------------------------------------*/


// Application global/shared flag manager
extern FlagManager g_sharedFlagManager;
extern inline FlagManager& GetGlobalFlagManager(void);


/*----------------------------------------------Debug/Test flags---------------------------------------------*/

// Define default value
#define DEFAULT_DUMMYTEST			FALSE
#define DEFAULT_DEBUGMODE			FALSE
#define DEFAULT_DEBUGOUTPUT			DefaultOutput
#define DEFAULT_TESTFEATURE			FALSE

// Dummy test mode flag
static inline const BOOL GetDummyTestMode(void);
static inline void SetDummyTestMode(BOOL bValue);

// Debug mode flag
static inline const BOOL GetDebugMode(void);
static inline void SetDebugMode(BOOL bValue);

// Debug log output target flag
enum DebugOutput {
	DefaultOutput = 0,
	DebugInfoFile,
	DebugTestTool,
};
static inline const INT GetDebugOutputTarget(void);
static inline void SetDebugOutputTarget(int nValue);

// Test feature enable flag
static inline const BOOL GetTestFeatureEnable(void);
static inline void SetTestFeatureEnable(BOOL bValue);

/*-----------------------------------------------------------------------------------------------------------*/

/*----------------------------------------Debug/Test special variables---------------------------------------*/

namespace Global {
	extern CFile* g_pFileLogTraceError;
	extern CFile* g_pFileLogTraceDebug;
	extern CFile* g_pFileLogDebugInfo;
	extern CFileException* g_pExcLogTraceError;
	extern CFileException* g_pExcLogTraceDebug;
	extern CFileException* g_pExcLogDebugInfo;
};

// Trace error log file pointer
static inline CFile* GetTraceErrorLogFile(void);
static BOOL InitTraceErrorLogFile(void);
inline void ReleaseTraceErrorLogFile(void);

//Trace debug info log file pointer
static inline CFile* GetTraceDebugLogFile(void);
static BOOL InitTraceDebugLogFile(void);
inline void ReleaseTraceDebugLogFile(void);

// Debug info output log file pointer
static inline CFile* GetDebugInfoLogFile(void);
static BOOL InitDebugInfoLogFile(void);
inline void ReleaseDebugInfoLogFile(void);

// Trace error log file exception pointer
static inline CFileException* GetTraceErrorException(void);

// Trace debug info log file exception pointer
static inline CFileException* GetTraceDebugException(void);

// Debug info output log file exception pointer
static inline CFileException* GetDebugInfoException(void);

/*-----------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------App tracing flags---------------------------------------------*/

// Power action trace flag
static inline const BYTE GetPwrActionFlag(void);
static inline void SetPwrActionFlag(BYTE byValue);

// System suspended trace flag
static inline const BYTE GetSystemSuspendFlag(void);
static inline void SetSystemSuspendFlag(BYTE byValue);

// Session ended trace flag
static inline const BYTE GetSessionEndFlag(void);
static inline void SetSessionEndFlag(BYTE byValue);

// Previously safe termination trace flag
static inline const BYTE GetSafeTerminationFlag(void);
static inline void SetSafeTerminationFlag(BYTE byValue);

/*-----------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------Memory-only flags---------------------------------------------*/

// Session lock trace flag
static inline const BYTE GetSessionLockFlag(void);
static inline void SetSessionLockFlag(BYTE byValue);

/*-----------------------------------------------------------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Include inline file for global inline functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _GLOBAL_ENABLE_INLINES
	#include "Global.inl"
	#ifdef _GLOBAL_INLINE_INCLUDED
		#pragma message("-- Global inline library included")
	#else
		#pragma error("-- Linking error in Global.h: Unable to link to inline header!")
	#endif
	#undef _GLOBAL_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in Global.h: Inline is not enabled!")
#endif
