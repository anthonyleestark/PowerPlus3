
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Global.inl
//		Description:	Implement definition of global inline functions which will be used everywhere in program
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.05.29:		Create new for version 3.2
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


///////////////////////////////////////////////////////
//// Implementations

#ifdef _GLOBAL_ENABLE_INLINES

#ifndef _GLOBAL_INLINE_INCLUDED
#define _GLOBAL_INLINE_INCLUDED
#endif

///////////////////////////////////////////////////////

// Access application launch-time data
inline const DateTime& GetAppLaunchTime(void) {
	return Global::g_stAppLaunchTime;
}
inline void SetAppLaunchTime(const DateTime& stTime) {
	Global::g_stAppLaunchTime = stTime;
}

// Access application launch-time counter data
inline const UINT GetAppLaunchTimeCounter(void) {
	return Global::g_uiAppLaunchTimeCounter;
}
inline const BOOL IsAppFirstLaunch(void) {
	return (Global::g_uiAppLaunchTimeCounter <= 1);
}
inline void SetAppLaunchTimeCounter(UINT uiValue) {
	Global::g_uiAppLaunchTimeCounter = uiValue;
}
inline void UpdateAppLaunchTimeCounter(void) {
	++Global::g_uiAppLaunchTimeCounter;
}

///////////////////////////////////////////////////////

// Application global/shared flag manager
inline FlagManager& GetGlobalFlagManager(void) {
	return g_sharedFlagManager;
}

///////////////////////////////////////////////////////

// Dummy test mode flag
inline const BOOL GetDummyTestMode(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::dummyTestMode);
}
inline void SetDummyTestMode(BOOL bValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::dummyTestMode, bValue);
}

// Debug mode flag
inline const BOOL GetDebugMode(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::debugMode);
}
inline void SetDebugMode(BOOL bValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::debugMode, bValue);
}

// Debug log output target flag
inline const INT GetDebugOutputTarget(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::debugOutputTarget);
}
inline void SetDebugOutputTarget(int nValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::debugOutputTarget, nValue);
}

// Test feature enable flag
inline const BOOL GetTestFeatureEnable(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::testFeatureEnabled);
}
inline void SetTestFeatureEnable(BOOL bValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::testFeatureEnabled, bValue);
}

///////////////////////////////////////////////////////

// Trace error log file pointer
inline CFile* GetTraceErrorLogFile(void) {
	return Global::g_pFileLogTraceError;
}

//Trace debug info log file pointer
inline CFile* GetTraceDebugLogFile(void) {
	return Global::g_pFileLogTraceDebug;
}

// Debug info output log file pointer
inline CFile* GetDebugInfoLogFile(void) {
	return Global::g_pFileLogDebugInfo;
}

// Trace error log file exception pointer
inline CFileException* GetTraceErrorException(void) {
	return Global::g_pExcLogTraceError;
}

// Trace debug info log file exception pointer
inline CFileException* GetTraceDebugException(void) {
	return Global::g_pExcLogTraceDebug;
}

// Debug info output log file exception pointer
inline CFileException* GetDebugInfoException(void) {
	return Global::g_pExcLogDebugInfo;
}

///////////////////////////////////////////////////////

// Power action trace flag
inline const BYTE GetPwrActionFlag(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::pwrActionFlag);
}
inline void SetPwrActionFlag(BYTE byValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::pwrActionFlag, byValue);
}

// System suspended trace flag
inline const BYTE GetSystemSuspendFlag(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::systemSuspendFlag);
}
inline void SetSystemSuspendFlag(BYTE byValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::systemSuspendFlag, byValue);
}

// Session ended trace flag
inline const BYTE GetSessionEndFlag(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::sessionEndFlag);
}
inline void SetSessionEndFlag(BYTE byValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::sessionEndFlag, byValue);
}

// Previously safe termination trace flag
inline const BYTE GetSafeTerminationFlag(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::safeTerminationFlag);
}
inline void SetSafeTerminationFlag(BYTE byValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::safeTerminationFlag, byValue);
}

///////////////////////////////////////////////////////

// Session lock trace flag
inline const BYTE GetSessionLockFlag(void) {
	return GetGlobalFlagManager().GetFlagValue(AppFlagID::sessionLockFlag);
}
inline void SetSessionLockFlag(BYTE byValue) {
	GetGlobalFlagManager().SetFlagValue(AppFlagID::sessionLockFlag, byValue);
}

#endif		// ifdef _GLOBAL_ENABLE_INLINES
