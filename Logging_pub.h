﻿
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Logging_pub.h
//		Description:	Define public resources and info using for logging function
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.12.08:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LOGGING_PUB_H_INCLUDED
#define _LOGGING_PUB_H_INCLUDED

#include "stdafx.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define macros for logging features
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Log category macros
#define LOG_MACRO_NONE						0x00
#define LOG_MACRO_EVENT_APP					(LOG_MACRO_NONE+0x01)
#define LOG_MACRO_EVENT_UI					(LOG_MACRO_NONE+0x02)
#define LOG_MACRO_EVENT_DATACHG				(LOG_MACRO_NONE+0x03)
#define LOG_MACRO_HISTORY					(LOG_MACRO_NONE+0x04)

// Application events
#define LOG_EVENT_INIT_INSTANCE				(LOG_MACRO_EVENT_APP<<8) + 0x001
#define LOG_EVENT_EXIT_INSTANCE				(LOG_MACRO_EVENT_APP<<8) + 0x002
#define LOG_EVENT_ERROR_MESSAGE				(LOG_MACRO_EVENT_APP<<8) + 0x003
#define LOG_EVENT_EXEC_DEBUGCMD				(LOG_MACRO_EVENT_APP<<8) + 0x004
#define LOG_EVENT_CHANGE_LANGUAGE			(LOG_MACRO_EVENT_APP<<8) + 0x005

// UI events
#define LOG_EVENT_BTN_CLICKED				(LOG_MACRO_EVENT_UI<<8) + 0x001
#define LOG_EVENT_CHK_CLICKED				(LOG_MACRO_EVENT_UI<<8) + 0x002
#define LOG_EVENT_RAD_CLICKED				(LOG_MACRO_EVENT_UI<<8) + 0x003
#define LOG_EVENT_EDIT_CHANGED				(LOG_MACRO_EVENT_UI<<8) + 0x004
#define LOG_EVENT_CMB_SELCHANGE				(LOG_MACRO_EVENT_UI<<8) + 0x005
#define LOG_EVENT_LIST_SELCHANGE			(LOG_MACRO_EVENT_UI<<8) + 0x006
#define LOG_EVENT_SPIN_CHANGED				(LOG_MACRO_EVENT_UI<<8) + 0x007
#define LOG_EVENT_MENU_SELECTED				(LOG_MACRO_EVENT_UI<<8) + 0x008

#define LOG_EVENT_DLG_INIT					(LOG_MACRO_EVENT_UI<<8) + 0x009
#define LOG_EVENT_DLG_DESTROYED				(LOG_MACRO_EVENT_UI<<8) + 0x00A
#define LOG_EVENT_DLG_SHOWED				(LOG_MACRO_EVENT_UI<<8) + 0x00B
#define LOG_EVENT_DLG_HIDDEN				(LOG_MACRO_EVENT_UI<<8) + 0x00C
#define LOG_EVENT_DLG_EXPANDED				(LOG_MACRO_EVENT_UI<<8) + 0x00D
#define LOG_EVENT_DLG_COLLAPSED				(LOG_MACRO_EVENT_UI<<8) + 0x00E

#define LOG_EVENT_TRAYICO_LMBCLICKED		(LOG_MACRO_EVENT_UI<<8) + 0x00F
#define LOG_EVENT_TRAYICO_MMBCLICKED		(LOG_MACRO_EVENT_UI<<8) + 0x010
#define LOG_EVENT_TRAYICO_RMBCLICKED		(LOG_MACRO_EVENT_UI<<8) + 0x011

#define LOG_EVENT_EXEC_PWRACTION			(LOG_MACRO_EVENT_UI<<8) + 0x012
#define LOG_EVENT_CANCEL_PWRACTION			(LOG_MACRO_EVENT_UI<<8) + 0x013
#define LOG_EVENT_EXEC_SCHEDULE				(LOG_MACRO_EVENT_UI<<8) + 0x014
#define LOG_EVENT_CANCEL_SCHEDULE			(LOG_MACRO_EVENT_UI<<8) + 0x015
#define LOG_EVENT_EXEC_HOTKEY				(LOG_MACRO_EVENT_UI<<8) + 0x016
#define LOG_EVENT_DISP_PWRREMINDER			(LOG_MACRO_EVENT_UI<<8) + 0x017

// Data change events
#define LOG_EVENT_DATACHG_CONFIG			(LOG_MACRO_EVENT_DATACHG<<8) + 0x001
#define LOG_EVENT_DATACHG_SCHEDULE			(LOG_MACRO_EVENT_DATACHG<<8) + 0x002
#define LOG_EVENT_DATACHG_HOTKEYSET			(LOG_MACRO_EVENT_DATACHG<<8) + 0x003
#define LOG_EVENT_DATACHG_PWRREMINDER		(LOG_MACRO_EVENT_DATACHG<<8) + 0x004

// History log
#define LOG_HISTORY_EXEC_PWRACTION			(LOG_MACRO_HISTORY<<8) + 0x001
#define LOG_HISTORY_EXEC_SCHEDULE			(LOG_MACRO_HISTORY<<8) + 0x002
#define LOG_HISTORY_EXEC_HOTKEY				(LOG_MACRO_HISTORY<<8) + 0x003
#define LOG_HISTORY_DISP_PWRREMINDER		(LOG_MACRO_HISTORY<<8) + 0x004


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define enum types for logging
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum eLOGWRITEMODE {
	LOG_WRITEMODE_NONE = 0,									// Read-only log data (do not write)
	LOG_WRITEMODE_ONCALL,									// Write log on-call mode
	LOG_WRITEMODE_INSTANT,									// Write log instantly mode
} LOGWRITEMODE;

typedef enum eLOGDETAILCATE {
	LOG_DETAIL_RESOURCEID = 0,								// UI component resource ID
	LOG_DETAIL_MAPTEXTID,									// Mapped text ID
	LOG_DETAIL_DIALOGCAPTION,								// Dialog caption
	LOG_DETAIL_CTRLCAPTION,									// Control caption
	LOG_DETAIL_CHKSTATE,									// Checkbox/radio button, ... checked state
	LOG_DETAIL_SELECTION,									// Combo-box/listbox, ... selection
	LOG_DETAIL_DATAVALUE,									// Data value
	LOG_DETAIL_DATACHANGE,									// Data change state
	LOG_DETAIL_CONTENTID,									// Content ID
	LOG_DETAIL_MESSAGETEXT,									// Message box text
} LOGDETAILCATE;

typedef enum eLOGDATATYPE {
	DATA_TYPE_UNSPECIFIED = -1,								// Unspecified type
	DATA_TYPE_VOID = 0,										// No type (unusable)
	DATA_TYPE_NUM_U1,										// Unsigned integer (1-byte)
	DATA_TYPE_NUM_U2,										// Unsigned integer (2-byte)
	DATA_TYPE_NUM_U4,										// Unsigned integer (4-byte)
	DATA_TYPE_NUM_U8,										// Unsigned integer (8-byte)
	DATA_TYPE_NUM_I1,										// Signed integer (1-byte)
	DATA_TYPE_NUM_I2,										// Signed integer (2-byte)
	DATA_TYPE_NUM_I4,										// Signed integer (4-byte)
	DATA_TYPE_NUM_I8,										// Signed integer (8-byte)
	DATA_TYPE_NUM_F1,										// Float number (1-byte)
	DATA_TYPE_NUM_F2,										// Float number (2-byte)
	DATA_TYPE_NUM_F4,										// Float number (4-byte)
	DATA_TYPE_NUM_F8,										// Float number (8-byte)
	DATA_TYPE_NUM_BOOLEAN,									// Boolean number (TRUE/FALSE)
	DATA_TYPE_TEXT_ASCII,									// Text value (ASCII)
	DATA_TYPE_TEXT_UNICODE,									// Text value (Unicode)
	DATA_TYPE_SYSTEMTIME,									// SYSTEMTIME struct
} LOGDATATYPE;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif