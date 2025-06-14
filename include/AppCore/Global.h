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
	extern unsigned	g_uiAppLaunchTimeCounter;
};

// Access application launch-time data
static inline const DateTime& GetAppLaunchTime(void) {
	return Global::g_stAppLaunchTime;
};
static inline void SetAppLaunchTime(const DateTime& stTime) {
	Global::g_stAppLaunchTime = stTime;
};

// Access application launch-time counter data
static inline const unsigned GetAppLaunchTimeCounter(void) {
	return Global::g_uiAppLaunchTimeCounter;
};
static inline const bool IsAppFirstLaunch(void) {
	return (Global::g_uiAppLaunchTimeCounter <= 1);
};
static inline void SetAppLaunchTimeCounter(unsigned uiValue) {
	Global::g_uiAppLaunchTimeCounter = uiValue;
};
static inline void UpdateAppLaunchTimeCounter(void) {
	++Global::g_uiAppLaunchTimeCounter;
};

/*-----------------------------------------------------------------------------------------------------------*/


// Application global/shared flag manager
extern FlagManager g_sharedFlagManager;
extern inline FlagManager& GetGlobalFlagManager(void) {
	return g_sharedFlagManager;
};


/*----------------------------------------------Debug/Test flags---------------------------------------------*/

// Define default value
#define DEFAULT_DUMMYTEST			FALSE
#define DEFAULT_DEBUGMODE			FALSE
#define DEFAULT_DEBUGOUTPUT			DefaultOutput
#define DEFAULT_TESTFEATURE			FALSE

// Dummy test mode flag
static inline const bool GetDummyTestMode(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::dummyTestMode);
};
static inline void SetDummyTestMode(bool bValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::dummyTestMode, bValue);
};

// Debug mode flag
static inline const bool GetDebugMode(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::debugMode);
};
static inline void SetDebugMode(bool bValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::debugMode, bValue);
};

// Debug log output target flag
enum DebugOutput {
	DefaultOutput = 0,
	DebugInfoFile,
	DebugTestTool,
};
static inline const int GetDebugOutputTarget(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::debugOutputTarget);
};
static inline void SetDebugOutputTarget(int nValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::debugOutputTarget, nValue);
};

// Test feature enable flag
static inline const bool GetTestFeatureEnable(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::testFeatureEnabled);
};
static inline void SetTestFeatureEnable(bool bValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::testFeatureEnabled, bValue);
};

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
static inline CFile* GetTraceErrorLogFile(void) {
	return Global::g_pFileLogTraceError;
};
static bool InitTraceErrorLogFile(void);
inline void ReleaseTraceErrorLogFile(void);

//Trace debug info log file pointer
static inline CFile* GetTraceDebugLogFile(void) {
	return Global::g_pFileLogTraceDebug;
};
static bool InitTraceDebugLogFile(void);
inline void ReleaseTraceDebugLogFile(void);

// Debug info output log file pointer
static inline CFile* GetDebugInfoLogFile(void) {
	return Global::g_pFileLogDebugInfo;
};
static bool InitDebugInfoLogFile(void);
inline void ReleaseDebugInfoLogFile(void);

// Trace error log file exception pointer
static inline CFileException* GetTraceErrorException(void) {
	return Global::g_pExcLogTraceError;
};

// Trace debug info log file exception pointer
static inline CFileException* GetTraceDebugException(void) {
	return Global::g_pExcLogTraceDebug;
};

// Debug info output log file exception pointer
static inline CFileException* GetDebugInfoException(void) {
	return Global::g_pExcLogDebugInfo;
};

/*-----------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------App tracing flags---------------------------------------------*/

// Power action trace flag
static inline const byte GetPwrActionFlag(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::pwrActionFlag);
};
static inline void SetPwrActionFlag(byte byValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::pwrActionFlag, byValue);
};

// System suspended trace flag
static inline const byte GetSystemSuspendFlag(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::systemSuspendFlag);
};
static inline void SetSystemSuspendFlag(byte byValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::systemSuspendFlag, byValue);
};

// Session ended trace flag
static inline const byte GetSessionEndFlag(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::sessionEndFlag);
};
static inline void SetSessionEndFlag(byte byValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::sessionEndFlag, byValue);
};

// Previously safe termination trace flag
static inline const byte GetSafeTerminationFlag(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::safeTerminationFlag);
};
static inline void SetSafeTerminationFlag(byte byValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::safeTerminationFlag, byValue);
};

/*-----------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------Memory-only flags---------------------------------------------*/

// Session lock trace flag
static inline const byte GetSessionLockFlag(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::sessionLockFlag);
};
static inline void SetSessionLockFlag(byte byValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::sessionLockFlag, byValue);
};

/*-----------------------------------------------------------------------------------------------------------*/

