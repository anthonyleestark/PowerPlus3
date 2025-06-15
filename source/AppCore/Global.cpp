/**
 * @file		Global.cpp
 * @brief		Define global objects which will be used everywhere in program
 * @author		AnthonyLeeStark
 * @date		2024.01.27
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "AppCore/Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*---------------------------------------------Launch-info data----------------------------------------------*/
// Application launch-time
DateTime Global::g_stAppLaunchTime;

// Application launch-time counter
unsigned Global::g_uiAppLaunchTimeCounter = 0;

/*-----------------------------------------------------------------------------------------------------------*/

// Application global/shared flag manager
FlagManager g_sharedFlagManager;

/*-----------------------------------------------------------------------------------------------------------*/

// END_OF_FILE
