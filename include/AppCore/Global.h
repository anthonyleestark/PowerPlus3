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
static inline const DateTime& getAppLaunchTime(void) {
	return Global::g_stAppLaunchTime;
};
static inline void setAppLaunchTime(const DateTime& stTime) {
	Global::g_stAppLaunchTime = stTime;
};

// Access application launch-time counter data
static inline const unsigned getAppLaunchTimeCounter(void) {
	return Global::g_uiAppLaunchTimeCounter;
};
static inline const bool isAppFirstLaunch(void) {
	return (Global::g_uiAppLaunchTimeCounter <= 1);
};
static inline void setAppLaunchTimeCounter(unsigned valueUnsigned) {
	Global::g_uiAppLaunchTimeCounter = valueUnsigned;
};
static inline void updateAppLaunchTimeCounter(void) {
	++Global::g_uiAppLaunchTimeCounter;
};

/*-----------------------------------------------------------------------------------------------------------*/


// Application global/shared flag manager
extern FlagManager g_sharedFlagManager;
extern inline FlagManager& getGlobalFlagManager(void) {
	return g_sharedFlagManager;
};


/*----------------------------------------------Debug/Test flags---------------------------------------------*/

// Define default value
#define DEFAULT_DUMMYTEST			FALSE
#define DEFAULT_DEBUGMODE			FALSE
#define DEFAULT_DEBUGOUTPUT			DefaultOutput
#define DEFAULT_TESTFEATURE			FALSE

// Dummy test mode flag
static inline const bool getDummyTestMode(void) {
	return getGlobalFlagManager().getFlagValue(AppFlagID::dummyTestMode);
};
static inline void setDummyTestMode(bool bValue) {
	getGlobalFlagManager().setFlagValue(AppFlagID::dummyTestMode, bValue);
};

// Debug mode flag
static inline const bool getDebugMode(void) {
	return getGlobalFlagManager().getFlagValue(AppFlagID::debugMode);
};
static inline void setDebugMode(bool bValue) {
	getGlobalFlagManager().setFlagValue(AppFlagID::debugMode, bValue);
};

// Debug log output target flag
enum DebugOutput {
	DefaultOutput = 0,
	DebugInfoFile,
	DebugTestTool,
};
static inline const int getDebugOutputTarget(void) {
	return getGlobalFlagManager().getFlagValue(AppFlagID::debugOutputTarget);
};
static inline void setDebugOutputTarget(int value) {
	getGlobalFlagManager().setFlagValue(AppFlagID::debugOutputTarget, value);
};

// Test feature enable flag
static inline const bool getTestFeatureEnable(void) {
	return getGlobalFlagManager().getFlagValue(AppFlagID::testFeatureEnabled);
};
static inline void setTestFeatureEnable(bool bValue) {
	getGlobalFlagManager().setFlagValue(AppFlagID::testFeatureEnabled, bValue);
};

/*-----------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------App tracing flags---------------------------------------------*/

// Power action trace flag
static inline const byte getPwrActionFlag(void) {
	return getGlobalFlagManager().getFlagValue(AppFlagID::pwrActionFlag);
};
static inline void setPwrActionFlag(byte byValue) {
	getGlobalFlagManager().setFlagValue(AppFlagID::pwrActionFlag, byValue);
};

// System suspended trace flag
static inline const byte getSystemSuspendFlag(void) {
	return getGlobalFlagManager().getFlagValue(AppFlagID::systemSuspendFlag);
};
static inline void setSystemSuspendFlag(byte byValue) {
	getGlobalFlagManager().setFlagValue(AppFlagID::systemSuspendFlag, byValue);
};

// Session ended trace flag
static inline const byte getSessionEndFlag(void) {
	return getGlobalFlagManager().getFlagValue(AppFlagID::sessionEndFlag);
};
static inline void setSessionEndFlag(byte byValue) {
	getGlobalFlagManager().setFlagValue(AppFlagID::sessionEndFlag, byValue);
};

// Previously safe termination trace flag
static inline const byte getSafeTerminationFlag(void) {
	return getGlobalFlagManager().getFlagValue(AppFlagID::safeTerminationFlag);
};
static inline void setSafeTerminationFlag(byte byValue) {
	getGlobalFlagManager().setFlagValue(AppFlagID::safeTerminationFlag, byValue);
};

/*-----------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------Memory-only flags---------------------------------------------*/

// Session lock trace flag
static inline const byte getSessionLockFlag(void) {
	return getGlobalFlagManager().getFlagValue(AppFlagID::sessionLockFlag);
};
static inline void setSessionLockFlag(byte byValue) {
	getGlobalFlagManager().setFlagValue(AppFlagID::sessionLockFlag, byValue);
};

/*-----------------------------------------------------------------------------------------------------------*/

