
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Core.cpp
//		Description:	Implement core methods which will be used elsewhere in program
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//						<3> 2024.07.06:		Update to version 3.1
//						<4> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AppCore\AppCore.h"
#include "AppCore\MapTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////
//// Implementations


//////////////////////////////////////////////////////////////////////////
//
//	Define member functions of data structures/classes
//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ConfigData
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

ConfigData::ConfigData()
{
	// Main settings
	this->nLMBAction = APP_ACTION_DISPLAYOFF;							// Left mouse button action
	this->nMMBAction = APP_ACTION_SLEEP;								// Middle mouse button action
	this->nRMBAction = APP_ACTION_SHOWMENU;								// Right mouse button action
	this->bRMBShowMenu = TRUE;											// Right mouse button: Only show menu

	// Display setting
	this->nLanguageID = APP_LANGUAGE_ENGLISH;							// Language setting

	// System settings
	this->bShowDlgAtStartup = TRUE;										// Show dialog at startup
	this->bStartupEnabled = TRUE;										// Startup with Windows
	this->bConfirmAction = TRUE;										// Show confirm message before doing action
	this->bSaveHistoryLog = FALSE;										// Save app history log
	this->bSaveAppEventLog = TRUE;										// Save app event log
	this->bRunAsAdmin = FALSE;											// Run with admin privileges
	this->bShowErrorMsg = TRUE;											// Show action error message
	this->bNotifySchedule = TRUE;										// Show notify tip for schedule action
	this->bAllowCancelSchedule = FALSE;									// Allow canceling schedule when notify
	this->bEnableBackgroundHotkey = FALSE;								// Enable background action hotkeys
	this->bLockStateHotkey = TRUE;										// Allow background hotkeys on lockscreen
	this->bEnablePowerReminder = TRUE;									// Enable Power Peminder feature
}

ConfigData::ConfigData(const CONFIGDATA& pData)
{
	// Main settings
	this->nLMBAction = pData.nLMBAction;								// Left mouse button action
	this->nMMBAction = pData.nMMBAction;								// Middle mouse button action
	this->nRMBAction = pData.nRMBAction;								// Right mouse button action
	this->bRMBShowMenu = pData.bRMBShowMenu;							// Right mouse button: Only show menu

	// Display setting
	this->nLanguageID = pData.nLanguageID;								// Language setting

	// System advanced settings
	this->bShowDlgAtStartup = pData.bShowDlgAtStartup;					// Show dialog at startup
	this->bStartupEnabled = pData.bStartupEnabled;						// Startup with Windows
	this->bConfirmAction = pData.bConfirmAction;						// Show confirm message before doing action
	this->bSaveHistoryLog = pData.bSaveHistoryLog;						// Save app history log
	this->bSaveAppEventLog = pData.bSaveAppEventLog;					// Save app event log
	this->bRunAsAdmin = pData.bRunAsAdmin;								// Run with admin privileges
	this->bShowErrorMsg = pData.bShowErrorMsg;							// Show action error message
	this->bNotifySchedule = pData.bNotifySchedule;						// Show notify tip for schedule action
	this->bAllowCancelSchedule = pData.bAllowCancelSchedule;			// Allow canceling schedule when notify
	this->bEnableBackgroundHotkey = pData.bEnableBackgroundHotkey;		// Enable background action hotkeys
	this->bLockStateHotkey = pData.bLockStateHotkey;					// Allow background hotkeys on lockscreen
	this->bEnablePowerReminder = pData.bEnablePowerReminder;			// Enable Power Peminder feature
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another config data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ConfigData::Copy(const CONFIGDATA& pData)
{
	// Main settings
	this->nLMBAction = pData.nLMBAction;								// Left mouse button action
	this->nMMBAction = pData.nMMBAction;								// Middle mouse button action
	this->nRMBAction = pData.nRMBAction;								// Right mouse button action
	this->bRMBShowMenu = pData.bRMBShowMenu;							// Right mouse button: Only show menu

	// Display setting
	this->nLanguageID = pData.nLanguageID;								// Language setting

	// System advanced settings
	this->bShowDlgAtStartup = pData.bShowDlgAtStartup;					// Show dialog at startup
	this->bStartupEnabled = pData.bStartupEnabled;						// Startup with Windows
	this->bConfirmAction = pData.bConfirmAction;						// Show confirm message before doing action
	this->bSaveHistoryLog = pData.bSaveHistoryLog;						// Save app history log
	this->bSaveAppEventLog = pData.bSaveAppEventLog;					// Save app event log
	this->bRunAsAdmin = pData.bRunAsAdmin;								// Run with admin privileges
	this->bShowErrorMsg = pData.bShowErrorMsg;							// Show action error message
	this->bNotifySchedule = pData.bNotifySchedule;						// Show notify tip for schedule action
	this->bAllowCancelSchedule = pData.bAllowCancelSchedule;			// Allow canceling schedule when notify
	this->bEnableBackgroundHotkey = pData.bEnableBackgroundHotkey;		// Enable background action hotkeys
	this->bLockStateHotkey = pData.bLockStateHotkey;					// Allow background hotkeys on lockscreen
	this->bEnablePowerReminder = pData.bEnablePowerReminder;			// Enable Power Peminder feature
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given data
//  Arguments:		pData - Pointer of given data
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL ConfigData::Compare(const CONFIGDATA& pData) const
{
	BOOL bRet = TRUE;

	// Compare Main settings
	bRet &= (this->nLMBAction == pData.nLMBAction);								// Left mouse button action
	bRet &= (this->nMMBAction == pData.nMMBAction);								// Middle mouse button action
	bRet &= (this->nRMBAction == pData.nRMBAction);								// Right mouse button action
	bRet &= (this->bRMBShowMenu == pData.bRMBShowMenu);							// Right mouse button: Only show menu

	// Compare Display setting
	bRet &= (this->nLanguageID == pData.nLanguageID);							// Language setting

	// Compare System advanced settings
	bRet &= (this->bShowDlgAtStartup == pData.bShowDlgAtStartup);				// Show dialog at startup
	bRet &= (this->bStartupEnabled == pData.bStartupEnabled);					// Startup with Windows
	bRet &= (this->bConfirmAction == pData.bConfirmAction);						// Show confirm message before doing action
	bRet &= (this->bSaveHistoryLog == pData.bSaveHistoryLog);					// Save app history log
	bRet &= (this->bSaveAppEventLog == pData.bSaveAppEventLog);					// Save app event log
	bRet &= (this->bRunAsAdmin == pData.bRunAsAdmin);							// Run with admin privileges
	bRet &= (this->bShowErrorMsg == pData.bShowErrorMsg);						// Show action error message
	bRet &= (this->bNotifySchedule == pData.bNotifySchedule);					// Show notify tip for schedule action
	bRet &= (this->bAllowCancelSchedule == pData.bAllowCancelSchedule);			// Allow canceling schedule when notify
	bRet &= (this->bEnableBackgroundHotkey == pData.bEnableBackgroundHotkey);	// Enable background action hotkeys
	bRet &= (this->bLockStateHotkey == pData.bLockStateHotkey);					// Allow background hotkeys on lockscreen
	bRet &= (this->bEnablePowerReminder == pData.bEnablePowerReminder);			// Enable Power Peminder feature

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetData
//	Description:	Get a clone copy of config data
//  Arguments:		pData - Output config data (out)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ConfigData::GetData(CONFIGDATA& pData) const
{
	// Main settings
	pData.nLMBAction = this->nLMBAction;								// Left mouse button action
	pData.nMMBAction = this->nMMBAction;								// Middle mouse button action
	pData.nRMBAction = this->nRMBAction;								// Right mouse button action
	pData.bRMBShowMenu = this->bRMBShowMenu;							// Right mouse button: Only show menu

	// Display setting
	pData.nLanguageID = this->nLanguageID;								// Language setting

	// System advanced settings
	pData.bShowDlgAtStartup = this->bShowDlgAtStartup;					// Show dialog at startup
	pData.bStartupEnabled = this->bStartupEnabled;						// Startup with Windows
	pData.bConfirmAction = this->bConfirmAction;						// Show confirm message before doing action
	pData.bSaveHistoryLog = this->bSaveHistoryLog;						// Save app history log
	pData.bSaveAppEventLog = this->bSaveAppEventLog;					// Save app event log
	pData.bRunAsAdmin = this->bRunAsAdmin;								// Run with admin privileges
	pData.bShowErrorMsg = this->bShowErrorMsg;							// Show action error message
	pData.bNotifySchedule = this->bNotifySchedule;						// Show notify tip for schedule action
	pData.bAllowCancelSchedule = this->bAllowCancelSchedule;			// Allow canceling schedule when notify
	pData.bEnableBackgroundHotkey = this->bEnableBackgroundHotkey;		// Enable background action hotkeys
	pData.bLockStateHotkey = this->bLockStateHotkey;					// Allow background hotkeys on lockscreen
	pData.bEnablePowerReminder = this->bEnablePowerReminder;			// Enable Power Peminder feature
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDefaultData
//	Description:	Set default for config data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ConfigData::SetDefaultData(void)
{
	// Create a new default data
	static const CONFIGDATA defaultConfig;

	// Copy and overwrite current config data
	this->Copy(defaultConfig);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PwrRepeatSet
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

PwrRepeatSet::PwrRepeatSet()
{
	// Init data
	m_bRepeat = FALSE;										// Repeat daily
	m_bAllowSnooze = TRUE;									// Allow snoozing mode
	m_nSnoozeInterval = DEF_REPEATSET_DEFAULT_SNOOZE;		// Snooze interval
	m_byRepeatDays = DEF_REPEATSET_DEFAULT_ACTIVEDAYS;		// Default repeat: All days of week
}

PwrRepeatSet::PwrRepeatSet(const PwrRepeatSet& pItem)
{
	// Copy data
	this->m_bRepeat = pItem.m_bRepeat;						// Repeat daily
	this->m_bAllowSnooze = pItem.m_bAllowSnooze;			// Allow snoozing mode
	this->m_nSnoozeInterval = pItem.m_nSnoozeInterval;		// Snooze interval
	this->m_byRepeatDays = pItem.m_byRepeatDays;			// Days of week (for repeating)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

PwrRepeatSet& PwrRepeatSet::operator=(const PwrRepeatSet& pItem)
{
	// Copy data
	this->m_bRepeat = pItem.m_bRepeat;						// Repeat daily
	this->m_bAllowSnooze = pItem.m_bAllowSnooze;			// Allow snoozing mode
	this->m_nSnoozeInterval = pItem.m_nSnoozeInterval;		// Snooze interval
	this->m_byRepeatDays = pItem.m_byRepeatDays;			// Days of week (for repeating)

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another Repeat set data
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRepeatSet::Copy(const PwrRepeatSet& pItem)
{
	// Copy data
	this->m_bRepeat = pItem.m_bRepeat;						// Repeat daily
	this->m_bAllowSnooze = pItem.m_bAllowSnooze;			// Allow snoozing mode
	this->m_nSnoozeInterval = pItem.m_nSnoozeInterval;		// Snooze interval
	this->m_byRepeatDays = pItem.m_byRepeatDays;			// Days of week (for repeating)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare data with another Repeat set data
//  Arguments:		pItem - Pointer of input item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrRepeatSet::Compare(const PwrRepeatSet& pItem) const
{
	BOOL bRetCompare = TRUE;

	// Compare data
	bRetCompare &= (this->m_bRepeat == pItem.m_bRepeat);					// Repeat daily
	bRetCompare &= (this->m_bAllowSnooze == pItem.m_bAllowSnooze);			// Allow snoozing mode
	bRetCompare &= (this->m_nSnoozeInterval == pItem.m_nSnoozeInterval);	// Snooze interval
	bRetCompare &= (this->m_byRepeatDays == pItem.m_byRepeatDays);			// Days of week (for repeating)

	return bRetCompare;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsRepeatEnabled
//	Description:	Check if repeat option is enabled
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrRepeatSet::IsRepeatEnabled(void) const
{
	return m_bRepeat;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsAllowSnoozing
//	Description:	Check if snooze option is enabled
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrRepeatSet::IsAllowSnoozing(void) const
{
	return m_bAllowSnooze;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSnoozeInterval
//	Description:	Get snooze interval data
//  Arguments:		None
//  Return value:	INT
//
//////////////////////////////////////////////////////////////////////////

INT PwrRepeatSet::GetSnoozeInterval(void) const
{
	return m_nSnoozeInterval;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetActiveDays
//	Description:	Get repeat days data
//  Arguments:		None
//  Return value:	BYTE
//
//////////////////////////////////////////////////////////////////////////

BYTE PwrRepeatSet::GetActiveDays(void) const
{
	return m_byRepeatDays;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsDayActive
//	Description:	Check if day of week is active
//  Arguments:		dayOfWeek - Day of week
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrRepeatSet::IsDayActive(DAYOFWEEK dayOfWeek) const
{
	// Invalid day of week
	if ((dayOfWeek < SUNDAY) || (dayOfWeek > SATURDAY))
		return FALSE;

	return ((m_byRepeatDays & (1 << dayOfWeek)) >> dayOfWeek);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableRepeat
//	Description:	Set repeat enable state
//  Arguments:		bEnabled - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRepeatSet::EnableRepeat(BOOL bEnabled)
{
	m_bRepeat = bEnabled;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableSnoozing
//	Description:	Set allow snoozing state
//  Arguments:		bEnabled - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRepeatSet::EnableSnoozing(BOOL bEnabled)
{
	m_bAllowSnooze = bEnabled;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetSnoozeInterval
//	Description:	Set snooze interval data
//  Arguments:		nValue - Interval value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRepeatSet::SetSnoozeInterval(INT nValue)
{
	m_nSnoozeInterval = nValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetActiveDays
//	Description:	Set repeat days data
//  Arguments:		byActiveDays - Active days
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRepeatSet::SetActiveDays(BYTE byActiveDays)
{
	m_byRepeatDays = byActiveDays;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDayActive
//	Description:	Set active state for specific day of week
//  Arguments:		byActiveDays - Active days
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRepeatSet::SetDayActive(DAYOFWEEK dayOfWeek, BOOL bActive)
{
	// Invalid day of week
	if ((dayOfWeek < SUNDAY) || (dayOfWeek > SATURDAY))
		return;

	m_byRepeatDays |= bActive << dayOfWeek;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ScheduleItem
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

ScheduleItem::ScheduleItem()
{
	// Initialize
	m_nItemID = DEF_SCHEDULE_MIN_ITEMID;				// Item ID
	m_bEnabled = FALSE;									// Enable/disable status
	m_nActionID = APP_ACTION_NOTHING;					// Schedule action ID
	m_stTime = SYSTEMTIME_ZERO;							// Schedule time
	m_rpsRepeatSet = PwrRepeatSet();					// Repeat set data
}

ScheduleItem::ScheduleItem(UINT nItemID)
{
	// Initialize
	m_nItemID = nItemID;								// Item ID
	m_bEnabled = FALSE;									// Enable/disable status
	m_nActionID = APP_ACTION_NOTHING;					// Schedule action
	m_stTime = SYSTEMTIME_ZERO;							// Schedule time
	m_rpsRepeatSet = PwrRepeatSet();					// Repeat set data
}

ScheduleItem::ScheduleItem(const ScheduleItem& pItem)
{
	// Copy data
	this->m_nItemID = pItem.m_nItemID;					// Item ID
	this->m_bEnabled = pItem.m_bEnabled;				// Enable/disable status
	this->m_nActionID = pItem.m_nActionID;				// Schedule action ID
	this->m_stTime = pItem.m_stTime;					// Schedule time
	this->m_rpsRepeatSet.Copy(pItem.m_rpsRepeatSet);	// Repeat set data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

ScheduleItem& ScheduleItem::operator=(const ScheduleItem& pItem)
{
	// Copy data
	this->m_nItemID = pItem.m_nItemID;					// Item ID
	this->m_bEnabled = pItem.m_bEnabled;				// Enable/disable status
	this->m_nActionID = pItem.m_nActionID;				// Schedule action ID
	this->m_stTime = pItem.m_stTime;					// Schedule time
	this->m_rpsRepeatSet.Copy(pItem.m_rpsRepeatSet);	// Repeat set data

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another schedule item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::Copy(const ScheduleItem& pItem)
{
	// Copy data
	this->m_nItemID = pItem.m_nItemID;					// Item ID
	this->m_bEnabled = pItem.m_bEnabled;				// Enable/disable status
	this->m_nActionID = pItem.m_nActionID;				// Schedule action ID
	this->m_stTime = pItem.m_stTime;					// Schedule time
	this->m_rpsRepeatSet.Copy(pItem.m_rpsRepeatSet);	// Repeat set data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleItem::Compare(const ScheduleItem& pItem) const
{
	BOOL bRet = TRUE;

	// Compare item (do not compare item ID)
	bRet &= (this->m_bEnabled == pItem.m_bEnabled);
	bRet &= (this->m_nActionID == pItem.m_nActionID);
	bRet &= (this->m_stTime.wHour == pItem.m_stTime.wHour);
	bRet &= (this->m_stTime.wMinute == pItem.m_stTime.wMinute);
	bRet &= (this->m_rpsRepeatSet.Compare(pItem.m_rpsRepeatSet));

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if schedule item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleItem::IsEmpty(void) const
{
	// Initialize an empty item
	static const SCHEDULEITEM schDummyItem;

	// Compare with this item and return result
	return this->Compare(schDummyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemID
//	Description:	Get schedule item ID
//  Arguments:		None
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT ScheduleItem::GetItemID(void) const
{
	return m_nItemID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetItemID
//	Description:	Set schedule item ID
//  Arguments:		nItemID - Item ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::SetItemID(UINT nItemID)
{
	m_nItemID = nItemID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEnabled
//	Description:	Check if item is enabled
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleItem::IsEnabled(void) const
{
	return m_bEnabled;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableItem
//	Description:	Set/change item active state
//  Arguments:		bEnabled - New active state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::EnableItem(BOOL bEnabled)
{
	m_bEnabled = bEnabled;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAction
//	Description:	Get schedule item action ID
//  Arguments:		None
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT ScheduleItem::GetAction(void) const
{
	return m_nActionID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAction
//	Description:	Set schedule item action ID
//  Arguments:		nActionID - Action ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::SetAction(UINT nActionID)
{
	m_nActionID = nActionID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetTime
//	Description:	Get schedule item time data
//  Arguments:		None
//  Return value:	SYSTEMTIME
//
//////////////////////////////////////////////////////////////////////////

SYSTEMTIME ScheduleItem::GetTime(void) const
{
	return m_stTime;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetTime
//	Description:	Set schedule item time data
//  Arguments:		stTime - Time data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::SetTime(const SYSTEMTIME& stTime)
{
	m_stTime = stTime;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsRepeatEnabled
//	Description:	Check if repeat is enabled
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleItem::IsRepeatEnabled(void) const
{
	return (m_rpsRepeatSet.IsRepeatEnabled());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsAllowSnoozing
//	Description:	Check if item snooze mode is available
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleItem::IsAllowSnoozing(void) const
{
	// If repeat option is currently OFF
	if (IsRepeatEnabled() != TRUE) {
		// Not allow snooze mode
		return FALSE;
	}
	// If allow snooze option is OFF
	if (m_rpsRepeatSet.IsAllowSnoozing() != TRUE) {
		// Not allow snooze mode
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsDayActive
//	Description:	Check if day of week is active
//  Arguments:		dayOfWeek - Day of week
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleItem::IsDayActive(DAYOFWEEK dayOfWeek) const
{
	return (m_rpsRepeatSet.IsDayActive(dayOfWeek));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetActiveDays
//	Description:	Get repeat active days
//  Arguments:		None
//  Return value:	BYTE
//
//////////////////////////////////////////////////////////////////////////

BYTE ScheduleItem::GetActiveDays(void) const
{
	return (m_rpsRepeatSet.GetActiveDays());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableRepeat
//	Description:	Set repeat enable state
//  Arguments:		bEnabled - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::EnableRepeat(BOOL bEnabled)
{
	m_rpsRepeatSet.EnableRepeat(bEnabled);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableSnoozing
//	Description:	Set allow snoozing state
//  Arguments:		bEnabled - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::EnableSnoozing(BOOL bEnabled)
{
	m_rpsRepeatSet.EnableSnoozing(bEnabled);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetSnoozeInterval
//	Description:	Set snooze interval data
//  Arguments:		nValue - Interval value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::SetSnoozeInterval(INT nValue)
{
	m_rpsRepeatSet.SetSnoozeInterval(nValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetActiveDays
//	Description:	Set repeat days data
//  Arguments:		byActiveDays - Active days
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::SetActiveDays(BYTE byActiveDays)
{
	m_rpsRepeatSet.SetActiveDays(byActiveDays);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDayActive
//	Description:	Set active state for specific day of week
//  Arguments:		byActiveDays - Active days
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::SetDayActive(DAYOFWEEK dayOfWeek, BOOL bActive)
{
	m_rpsRepeatSet.SetDayActive(dayOfWeek, bActive);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Print
//	Description:	Print schedule item
//  Arguments:		strOutput - Output printed string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::Print(CString& strOutput) const
{
	// Use table, language and core functions
	using namespace MapTable;
	using namespace Language;
	using namespace AppCore;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format schedule data
	CString strActive = (m_bEnabled == TRUE) ? VALUE_TRUE : VALUE_FALSE;						// Enable/disable state
	UINT nActionStringID = GetPairedID(IDTable::ActionName, m_nActionID);
	CString strAction = GetLanguageString(ptrLanguage, nActionStringID);						// Schedule action
	CString strTimeFormat = FormatDispTime(ptrLanguage, IDS_FORMAT_SHORTTIME, m_stTime);	// Schedule time
	CString strRepeat = (m_rpsRepeatSet.IsRepeatEnabled() == TRUE) ? VALUE_TRUE : VALUE_FALSE;		// Repeat daily

	// Print item
	strOutput.Format(_T("Active=(%s), ItemID=%d, Action=(%s), Time=(%s), Repeat=(%s)"),
						strActive, m_nItemID, strAction, strTimeFormat, strRepeat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ScheduleData
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

ScheduleData::ScheduleData()
{
	// Initialize
	m_schDefaultItem = SCHEDULEITEM(DEF_SCHEDULE_DEFAULT_ITEMID);
	m_arrSchedExtraItemList.RemoveAll();
}

ScheduleData::ScheduleData(const ScheduleData& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy default item
	this->m_schDefaultItem.Copy(pData.m_schDefaultItem);

	// Copy extra data
	for (int nIndex = 0; nIndex < pData.GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM schItem = pData.m_arrSchedExtraItemList.GetAt(nIndex);
		this->m_arrSchedExtraItemList.Add(schItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

ScheduleData& ScheduleData::operator=(const ScheduleData& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy default item
	this->m_schDefaultItem.Copy(pData.m_schDefaultItem);

	// Copy extra data
	for (int nIndex = 0; nIndex < pData.GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM schItem = pData.m_arrSchedExtraItemList.GetAt(nIndex);
		this->m_arrSchedExtraItemList.Add(schItem);
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Init Action Schedule data (NULL)
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleData::Init()
{
	// Initialize
	m_schDefaultItem = SCHEDULEITEM(DEF_SCHEDULE_DEFAULT_ITEMID);
	m_arrSchedExtraItemList.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another Action Schedule data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleData::Copy(const ScheduleData& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy default item
	this->m_schDefaultItem.Copy(pData.m_schDefaultItem);

	// Copy extra data
	for (int nIndex = 0; nIndex < pData.GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM schItem = pData.m_arrSchedExtraItemList.GetAt(nIndex);
		this->m_arrSchedExtraItemList.Add(schItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDefaultData
//	Description:	Set default for schedule data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleData::SetDefaultData(void)
{
	// Reset data
	this->Init();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add an Action Schedule item
//  Arguments:		pItem - Pointer of input item
//  Return value:	DWORD - Error code
//
//////////////////////////////////////////////////////////////////////////

DWORD ScheduleData::Add(const SCHEDULEITEM& pItem)
{
	// If item is empty, can not update
	if (pItem.IsEmpty())
		return DEF_SCHEDULE_ERROR_EMPTY;

	// If default item is currently empty
	if (m_schDefaultItem.IsEmpty()) {
		// Make item as default
		SCHEDULEITEM schDefaultTemp(pItem);
		schDefaultTemp.SetItemID(DEF_SCHEDULE_DEFAULT_ITEMID);
		m_schDefaultItem.Copy(schDefaultTemp);
		return DEF_SCHEDULE_ERROR_SUCCESS;
	}

	// If extra schedule data is currently empty
	if (m_arrSchedExtraItemList.IsEmpty()) {
		// Just add the item
		m_arrSchedExtraItemList.Add(pItem);
		return DEF_SCHEDULE_ERROR_SUCCESS;
	}

	// If number of items exceeded limit
	if (GetExtraItemNum() >= DEF_SCHEDULE_ERROR_MAXITEM)
		return DEF_SCHEDULE_ERROR_MAXITEM;

	// Check if item is duplicated, if yes, do not add
	for (int nIndex = 0; nIndex < GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM pItemTemp = GetItemAt(nIndex);
		if (pItemTemp.Compare(pItem) == TRUE) {
			// All data is duplicated
			return DEF_SCHEDULE_ERROR_DUPLICATE;
		}
		else if (AppCore::CheckTimeMatch(pItemTemp.GetTime(), pItem.GetTime())) {
			// Time value is duplicated
			// Can not execute multiple action at the same time
			return DEF_SCHEDULE_ERROR_DUPLICATETIME;
		}
	}

	// Create new temporary data
	ScheduleData* pNew = new ScheduleData;
	pNew->m_arrSchedExtraItemList.RemoveAll();

	// Copy old data to new one
	pNew->m_schDefaultItem.Copy(this->m_schDefaultItem);
	for (int nIndex = 0; nIndex < this->GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM schItem = this->GetItemAt(nIndex);
		pNew->m_arrSchedExtraItemList.Add(schItem);
	}

	// Add new item and copy back to old data
	pNew->m_arrSchedExtraItemList.Add(pItem);
	this->Copy(*pNew);

	// Delete data
	pNew->DeleteAll();
	if (pNew != NULL) {
		delete pNew;
		pNew = NULL;
	}

	return DEF_SCHEDULE_ERROR_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Update
//	Description:	Update a Action Schedule item
//  Arguments:		pItem - Pointer of input item
//  Return value:	DWORD - Error code
//
//////////////////////////////////////////////////////////////////////////

DWORD ScheduleData::Update(const SCHEDULEITEM& pItem)
{
	// If default item or extra schedule data is currently empty
	if (GetDefaultItem().IsEmpty() || IsAllEmpty()) {
		// Just add item
		return Add(pItem);
	}

	// If item is empty, can not update
	if (pItem.IsEmpty())
		return DEF_SCHEDULE_ERROR_EMPTY;

	// If item ID is matching with default item
	if (pItem.GetItemID() == DEF_SCHEDULE_DEFAULT_ITEMID) {
		// Update default item
		GetDefaultItem().Copy(pItem);
		return DEF_SCHEDULE_ERROR_SUCCESS;
	}

	// Find extra item with matching ID
	int nRetItemIndex = INT_INVALID;
	for (int nIndex = 0; nIndex < GetExtraItemNum(); nIndex++) {
		if (GetItemAt(nIndex).GetItemID() == pItem.GetItemID()) {
			nRetItemIndex = nIndex;
			break;
		}
	}

	// Update item if found
	if (nRetItemIndex != INT_INVALID) {
		SCHEDULEITEM& schTemp = GetItemAt(nRetItemIndex);
		schTemp.Copy(pItem);
		return DEF_SCHEDULE_ERROR_SUCCESS;
	}
	// Otherwise,
	else {
		// Just add new
		return Add(pItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDefaultItem
//	Description:	Get the default Action Schedule item
//  Arguments:		None
//  Return value:	SCHEDULEITEM
//	Notes:			Constant function
//
//////////////////////////////////////////////////////////////////////////

const SCHEDULEITEM& ScheduleData::GetDefaultItem(void) const
{
	return m_schDefaultItem;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDefaultItem
//	Description:	Get the default Action Schedule item
//  Arguments:		None
//  Return value:	SCHEDULEITEM
//
//////////////////////////////////////////////////////////////////////////

SCHEDULEITEM& ScheduleData::GetDefaultItem(void)
{
	return m_schDefaultItem;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemAt
//	Description:	Get the Action Schedule item at index
//  Arguments:		nIndex - Item index
//  Return value:	SCHEDULEITEM
//	Notes:			Constant function
//
//////////////////////////////////////////////////////////////////////////

const SCHEDULEITEM& ScheduleData::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < GetExtraItemNum()));
	if ((nIndex >= 0) && (nIndex < GetExtraItemNum()))
		return m_arrSchedExtraItemList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemAt
//	Description:	Get the Action Schedule item at index
//  Arguments:		nIndex - Item index
//  Return value:	SCHEDULEITEM
//
//////////////////////////////////////////////////////////////////////////

SCHEDULEITEM& ScheduleData::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < GetExtraItemNum()));
	if ((nIndex >= 0) && (nIndex < GetExtraItemNum()))
		return m_arrSchedExtraItemList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Remove
//	Description:	Remove a schedule item by index
//  Arguments:		nAtIndex - Index of item to remove
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleData::Remove(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= GetExtraItemNum()))
		return;

	// Get item data
	SCHEDULEITEM& schItem = GetItemAt(nAtIndex);

	// Reset item value
	schItem.Copy(SCHEDULEITEM());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all Action Schedule data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleData::RemoveAll(void)
{
	// Remove each item
	for (int nIndex = 0; nIndex < GetExtraItemNum(); nIndex++) {
		Remove(nIndex);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Adjust
//	Description:	Adjust Action Schedule data validity
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleData::Adjust(void)
{
	// If default item is empty but extra data is not
	if (IsDefaultEmpty() && IsExtraEmpty() == FALSE) {
		// Make first extra item default
		m_schDefaultItem.Copy(GetItemAt(0));
		m_schDefaultItem.SetItemID(DEF_SCHEDULE_DEFAULT_ITEMID);

		// Remove that extra item
		Delete(0);
	}

	// Check and remove empty extra items
	for (int nIndex = (GetExtraItemNum() - 1); nIndex >= 0; nIndex--) {
		SCHEDULEITEM schTemp = GetItemAt(nIndex);
		if (!schTemp.IsEmpty()) continue;

		// Remove item
		Delete(nIndex);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetNextID
//	Description:	Get next item ID (to add new item)
//  Arguments:		None
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT ScheduleData::GetNextID(void)
{
	// Get currently max ID
	UINT nRetNextID = DEF_SCHEDULE_MIN_ITEMID;
	for (int nIndex = 0; nIndex < GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM schItem = GetItemAt(nIndex);
		if (schItem.GetItemID() > nRetNextID) {
			nRetNextID = schItem.GetItemID();
		}
	}

	// Increase value
	nRetNextID++;

	return nRetNextID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetExtraItemNum
//	Description:	Get number of extra items
//  Arguments:		None
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

INT_PTR ScheduleData::GetExtraItemNum(void) const
{
	return m_arrSchedExtraItemList.GetSize();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsDefaultEmpty
//	Description:	Check if default item is empty
//  Arguments:		None
//  Return value:	BOOL - Result of empty or not
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleData::IsDefaultEmpty(void) const
{
	return m_schDefaultItem.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if item at index is empty
//  Arguments:		nIndex - Item index to check
//  Return value:	BOOL - Result of empty or not
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleData::IsEmpty(int nIndex) const
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= GetExtraItemNum()))
		return TRUE;

	// Check if item is empty
	SCHEDULEITEM schItem = GetItemAt(nIndex);
	return schItem.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsExtraEmpty
//	Description:	Check if extra data is empty
//  Arguments:		None
//  Return value:	BOOL - Result of all item empty
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleData::IsExtraEmpty(void) const
{
	// If there's no item, return TRUE
	if (m_arrSchedExtraItemList.IsEmpty())
		return TRUE;

	// Check each item
	BOOL bExtraEmpty = TRUE;
	for (int nIndex = 0; nIndex < GetExtraItemNum(); nIndex++) {
		if (IsEmpty(nIndex) == FALSE) {
			bExtraEmpty = FALSE;
			break;
		}
	}

	return bExtraEmpty;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsAllEmpty
//	Description:	Check if all item are empty
//  Arguments:		None
//  Return value:	BOOL - Result of all item empty
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleData::IsAllEmpty(void) const
{
	return (IsDefaultEmpty() && IsExtraEmpty());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Delete
//	Description:	Delete a reminder item by index
//  Arguments:		nAtIndex - Index of item to delete
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleData::Delete(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= GetExtraItemNum()))
		return;

	// Create new temporary data
	ScheduleData* pNew = new ScheduleData;
	pNew->m_arrSchedExtraItemList.RemoveAll();

	// Copy old data to new one (except the AtIndex item)
	pNew->m_schDefaultItem.Copy(this->m_schDefaultItem);
	for (int nIndex = 0; nIndex < this->GetExtraItemNum(); nIndex++) {
		if (nIndex == nAtIndex) continue;
		pNew->m_arrSchedExtraItemList.Add(this->m_arrSchedExtraItemList.GetAt(nIndex));
	}

	// Copy back to old data
	this->Copy(*pNew);

	// Delete temporary data
	pNew->DeleteAll();
	if (pNew != NULL) {
		delete pNew;
		pNew = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DeleteExtra
//	Description:	Delete all Schedule extra items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleData::DeleteExtra(void)
{
	// Reset data
	m_arrSchedExtraItemList.RemoveAll();
	m_arrSchedExtraItemList.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DeleteAll
//	Description:	Delete all Action Schedule data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleData::DeleteAll(void)
{
	// Reset data
	m_schDefaultItem = SCHEDULEITEM(DEF_SCHEDULE_DEFAULT_ITEMID);
	m_arrSchedExtraItemList.RemoveAll();
	m_arrSchedExtraItemList.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	HotkeySetItem
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

HotkeySetItem::HotkeySetItem()
{
	// Initialize
	m_bEnabled = FALSE;								// Hotkey enabled/disabled
	m_nHKActionID = 0;								// Hotkey action ID
	m_dwCtrlKeyCode = 0;							// Control Keycode #1
	m_dwFuncKeyCode = 0;							// Function Keycode #2
}

HotkeySetItem::HotkeySetItem(UINT nHKActionID)
{
	// Initialize
	m_bEnabled = FALSE;								// Hotkey enabled/disabled
	m_nHKActionID = nHKActionID;					// Hotkey action ID
	m_dwCtrlKeyCode = 0;							// Control Keycode #1
	m_dwFuncKeyCode = 0;							// Function Keycode #2
}

HotkeySetItem::HotkeySetItem(const HotkeySetItem& pItem)
{
	// Copy data
	this->m_bEnabled = pItem.m_bEnabled;			// Hotkey enabled/disabled
	this->m_nHKActionID = pItem.m_nHKActionID;		// Hotkey action ID
	this->m_dwCtrlKeyCode = pItem.m_dwCtrlKeyCode;	// Control Keycode #1
	this->m_dwFuncKeyCode = pItem.m_dwFuncKeyCode;	// Function Keycode #2
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

HotkeySetItem& HotkeySetItem::operator=(const HotkeySetItem& pItem)
{
	// Copy data
	this->m_bEnabled = pItem.m_bEnabled;			// Hotkey enabled/disabled
	this->m_nHKActionID = pItem.m_nHKActionID;		// Hotkey action ID
	this->m_dwCtrlKeyCode = pItem.m_dwCtrlKeyCode;	// Control Keycode #1
	this->m_dwFuncKeyCode = pItem.m_dwFuncKeyCode;	// Function Keycode #2

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another Hotkeyset item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetItem::Copy(const HotkeySetItem& pItem)
{
	// Copy data
	this->m_bEnabled = pItem.m_bEnabled;			// Hotkey enabled/disabled
	this->m_nHKActionID = pItem.m_nHKActionID;		// Hotkey action ID
	this->m_dwCtrlKeyCode = pItem.m_dwCtrlKeyCode;	// Control Keycode #1
	this->m_dwFuncKeyCode = pItem.m_dwFuncKeyCode;	// Function Keycode #2
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if HotkeySet item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL HotkeySetItem::IsEmpty(void) const
{
	BOOL bIsEmpty = FALSE;

	// Check if keystroke value is empty
	bIsEmpty &= (m_dwCtrlKeyCode == 0);
	bIsEmpty &= (m_dwFuncKeyCode == 0);

	return bIsEmpty;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL HotkeySetItem::Compare(const HotkeySetItem& pItem) const
{
	BOOL bRet = TRUE;

	// Compare item
	bRet &= (this->m_nHKActionID == pItem.m_nHKActionID);
	bRet &= this->CompareKeycode(pItem);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CompareKeycode
//	Description:	Compare item keycode with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL HotkeySetItem::CompareKeycode(const HotkeySetItem& pItem) const
{
	BOOL bRet = TRUE;

	// Compare item keycode
	bRet &= (this->m_dwCtrlKeyCode == pItem.m_dwCtrlKeyCode);
	bRet &= (this->m_dwFuncKeyCode == pItem.m_dwFuncKeyCode);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Print
//	Description:	Print HotkeySet item
//  Arguments:		strOutput - Output printed string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetItem::Print(CString& strOutput) const
{
	// Use table and language functions
	using namespace MapTable;
	using namespace Language;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	CString strEnable = (m_bEnabled == TRUE) ? _T("Enabled") : _T("Disabled");
	UINT nActionNameID = GetPairedID(IDTable::ActionName, GetPairedID(IDTable::HKActionID, m_nHKActionID));
	CString strAction = GetLanguageString(ptrLanguage, nActionNameID);
	CString strKeyStrokes = STRING_EMPTY;
	PrintKeyStrokes(strKeyStrokes);

	// Print item
	strOutput.Format(_T("State=(%s), Action=(%s), Keystrokes=(%s)"),  strEnable, strAction, strKeyStrokes);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PrintKeyStrokes
//	Description:	Print HotkeySet item keystrokes data
//  Arguments:		strOutput - Output printed keystrokes string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetItem::PrintKeyStrokes(CString& strOutput) const
{
	// Use table and language functions
	using namespace MapTable;
	using namespace Language;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format keystrokes
	CString strKeyStrokes = STRING_EMPTY;
	if (m_dwCtrlKeyCode & MOD_CONTROL)	strKeyStrokes += _T("Ctrl + ");
	if (m_dwCtrlKeyCode & MOD_ALT)		strKeyStrokes += _T("Alt + ");
	if (m_dwCtrlKeyCode & MOD_WIN)		strKeyStrokes += _T("Win + ");
	strKeyStrokes += GetString(StringTable::FuncKeyList, m_dwFuncKeyCode);

	// Output string
	strOutput.Empty();
	strOutput = strKeyStrokes;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEnabled
//	Description:	Check if item is enabled
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL HotkeySetItem::IsEnabled(void) const
{
	return m_bEnabled;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableItem
//	Description:	Set item enable state
//  Arguments:		bEnabled - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetItem::EnableItem(BOOL bEnabled)
{
	m_bEnabled = bEnabled;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetActionID
//	Description:	Get Hotkey action ID
//  Arguments:		None
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT HotkeySetItem::GetActionID(void) const
{
	return m_nHKActionID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetActionID
//	Description:	Set Hotkey action ID
//  Arguments:		nHKActionID - Hotkey action ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetItem::SetActionID(UINT nHKActionID)
{
	m_nHKActionID = nHKActionID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetKeyCode
//	Description:	Get item keycode data
//  Arguments:		dwCtrlKey - Control keycode
//					dwFuncKey - Function keycode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetItem::GetKeyCode(DWORD& dwCtrlKey, DWORD& dwFuncKey) const
{
	dwCtrlKey = m_dwCtrlKeyCode;
	dwFuncKey = m_dwFuncKeyCode;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetKeyCode
//	Description:	Set item keycode data
//  Arguments:		dwCtrlKey - Control keycode
//					dwFuncKey - Function keycode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetItem::SetKeyCode(DWORD dwCtrlKey, DWORD dwFuncKey)
{
	m_dwCtrlKeyCode = dwCtrlKey;
	m_dwFuncKeyCode = dwFuncKey;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	HotkeySetData
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

HotkeySetData::HotkeySetData()
{
	// Initialize
	m_arrHotkeySetList.RemoveAll();
}

HotkeySetData::HotkeySetData(const HotkeySetData& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksItem = pData.m_arrHotkeySetList.GetAt(nIndex);
		this->m_arrHotkeySetList.Add(hksItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

HotkeySetData& HotkeySetData::operator=(const HotkeySetData& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksItem = pData.m_arrHotkeySetList.GetAt(nIndex);
		this->m_arrHotkeySetList.Add(hksItem);
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Init HotkeySet data (NULL)
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::Init()
{
	// Initialize
	m_arrHotkeySetList.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another HotkeySet data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::Copy(const HotkeySetData& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksItem = pData.m_arrHotkeySetList.GetAt(nIndex);
		this->m_arrHotkeySetList.Add(hksItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDefaultData
//	Description:	Set default for HotkeySet data
//  Arguments:		pcfgData - Pointer of HotkeySet data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::SetDefaultData(void)
{
	// Re-initialize data
	this->Init();

	// Create default data
	m_arrHotkeySetList.Add(HOTKEYSETITEM(HKID_DISPLAYOFF));
	m_arrHotkeySetList.Add(HOTKEYSETITEM(HKID_SLEEP));
	m_arrHotkeySetList.Add(HOTKEYSETITEM(HKID_SHUTDOWN));
	m_arrHotkeySetList.Add(HOTKEYSETITEM(HKID_RESTART));
	m_arrHotkeySetList.Add(HOTKEYSETITEM(HKID_SIGNOUT));
	m_arrHotkeySetList.Add(HOTKEYSETITEM(HKID_HIBERNATE));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add a hotkeyset item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::Add(const HOTKEYSETITEM& pItem)
{
	// If data list is current empty
	if (m_arrHotkeySetList.IsEmpty()) {
		// Just add item
		m_arrHotkeySetList.Add(pItem);
		return;
	}

	// Check if item exists, if yes, do not add
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		HOTKEYSETITEM pItemTemp = GetItemAt(nIndex);
		if (pItemTemp.Compare(pItem) == TRUE)
			return;
	}

	// Create new temporary data
	HotkeySetData* pNew = new HotkeySetData;
	pNew->m_arrHotkeySetList.RemoveAll();
	
	// Copy old data to new one
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksItem = this->GetItemAt(nIndex);
		pNew->m_arrHotkeySetList.Add(hksItem);
	}

	// Add new item and copy back to old data
	pNew->m_arrHotkeySetList.Add(pItem);
	this->Copy(*pNew);
	
	// Delete temporary data
	pNew->DeleteAll();
	if (pNew != NULL) {
		delete pNew;
		pNew = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Update
//	Description:	Update a hotkeyset item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::Update(const HOTKEYSETITEM& pItem)
{
	// If data list is current empty
	if (m_arrHotkeySetList.IsEmpty()) {
		// Just add item
		Add(pItem);
		return;
	}

	// Check if item with same action ID or keystrokes exists
	int nDupActionIndex = INT_INVALID;
	int nDupKeyIndex = INT_INVALID;

	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksTemp = GetItemAt(nIndex);
		if (hksTemp.GetActionID() == pItem.GetActionID()) {
			// Duplicate action ID found
			nDupActionIndex = nIndex;
		}
		if (hksTemp.CompareKeycode(pItem) == TRUE) {
			// Duplicate keystrokes found
			nDupKeyIndex = nIndex;
		}
	}

	// If same item existed (same action ID and same keystrokes), update its state
	if (nDupActionIndex == nDupKeyIndex) {
		HOTKEYSETITEM& hksTemp = GetItemAt(nDupActionIndex);
		hksTemp.EnableItem(pItem.IsEnabled());
		return;
	}

	// Delete existed duplicate keystrokes
	if (nDupKeyIndex != INT_INVALID) {
		Remove(nDupKeyIndex);
	}

	// If item with same action ID existed, update its data
	if (nDupActionIndex != INT_INVALID) {
		HOTKEYSETITEM& hksTemp = GetItemAt(nDupActionIndex);
		hksTemp.Copy(pItem);
	}
	// Otherwise, add new
	else {
		Add(pItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemAt
//	Description:	Get the hotkeyset item at index
//  Arguments:		nIndex - Item index
//  Return value:	HOTKEYSETITEM
//	Notes:			Constant function
//
//////////////////////////////////////////////////////////////////////////

const HOTKEYSETITEM& HotkeySetData::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrHotkeySetList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemAt
//	Description:	Get the hotkeyset item at index
//  Arguments:		nIndex - Item index
//  Return value:	HOTKEYSETITEM
//
//////////////////////////////////////////////////////////////////////////

HOTKEYSETITEM& HotkeySetData::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrHotkeySetList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Remove
//	Description:	Remove a hotkeyset item by index
//  Arguments:		nAtIndex - Index of item to remove
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::Remove(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= GetItemNum()))
		return;

	// Get item data
	HOTKEYSETITEM& hksItem = GetItemAt(nAtIndex);

	// Reset item value
	hksItem.EnableItem(FALSE);
	hksItem.SetKeyCode(NULL, NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all HotkeySet data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::RemoveAll(void)
{
	// Remove each item
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		Remove(nIndex);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Adjust
//	Description:	Adjust HotkeySet data validity
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::Adjust(void)
{
	DWORD dwCtrlKey, dwFuncKey;
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {

		// Get hotkeyset item keycode
		HOTKEYSETITEM& hksItem = GetItemAt(nIndex);
		hksItem.GetKeyCode(dwCtrlKey, dwFuncKey);

		// Not enable hotkeyset item if no keystroke data
		if ((dwCtrlKey == 0) || (dwFuncKey == 0)) {
			hksItem.EnableItem(FALSE);
			hksItem.SetKeyCode(NULL, NULL);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemNum
//	Description:	Get number of items
//  Arguments:		None
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

INT_PTR HotkeySetData::GetItemNum(void) const
{
	return m_arrHotkeySetList.GetSize();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if item at index is empty
//  Arguments:		nIndex - Item index to check
//  Return value:	BOOL - Result of empty or not
//
//////////////////////////////////////////////////////////////////////////

BOOL HotkeySetData::IsEmpty(int nIndex) const
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= GetItemNum()))
		return TRUE;

	// Get item data
	HOTKEYSETITEM hksItem = GetItemAt(nIndex);

	// Check if item keystroke value is empty
	return hksItem.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsAllEmpty
//	Description:	Check if all item are empty
//  Arguments:		None
//  Return value:	BOOL - Result of all item empty
//
//////////////////////////////////////////////////////////////////////////

BOOL HotkeySetData::IsAllEmpty() const
{
	// If there's no item, return TRUE
	if (m_arrHotkeySetList.IsEmpty())
		return TRUE;

	// Check each item
	BOOL bAllEmpty = TRUE;
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		if (IsEmpty(nIndex) == FALSE) {
			bAllEmpty = FALSE;
			break;
		}
	}

	return bAllEmpty;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Delete
//	Description:	Delete a hotkeyset item by index
//  Arguments:		nAtIndex - Index of item to delete
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::Delete(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= GetItemNum()))
		return;

	// Create new temporary data
	HotkeySetData* pNew = new HotkeySetData;
	pNew->m_arrHotkeySetList.RemoveAll();

	// Copy old data to new one (except the AtIndex item)
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		if (nIndex == nAtIndex) continue;
		pNew->m_arrHotkeySetList.Add(this->GetItemAt(nIndex));
	}

	// Copy back to old data
	this->Copy(*pNew);

	// Delete temporary data
	pNew->DeleteAll();
	if (pNew != NULL) {
		delete pNew;
		pNew = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DeleteAll
//	Description:	Delete all HotkeySet data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::DeleteAll(void)
{
	// Reset data
	m_arrHotkeySetList.RemoveAll();
	m_arrHotkeySetList.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PrintKeyStrokes
//	Description:	Print HotkeySet item keystrokes data by ID
//  Arguments:		nHKID	  - Item hotkey ID
//					strOutput - Output printed keystrokes string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::PrintKeyStrokes(UINT nHKID, CString& strOutput) const
{
	// Search for hotkey ID and get keystrokes string
	CString strKeyStrokes = STRING_EMPTY;
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksItem = this->GetItemAt(nIndex);
		if (hksItem.GetActionID() == nHKID) {
			hksItem.PrintKeyStrokes(strKeyStrokes);
			break;
		}
	}

	// Output string
	strOutput.Empty();
	strOutput = strKeyStrokes;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RmdMsgStyleSet
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

RmdMsgStyleSet::RmdMsgStyleSet()
{
	// Init data
	m_colorBkgrd = DEFAULT_MSG_BKGRDCLR;						// Background color
	m_colorText = DEFAULT_MSG_TEXTCLR;							// Text color
	m_strFontName = DEFAULT_MSG_FONTNAME;						// Font name
	m_uiFontSize = DEFAULT_MSG_FONTSIZE;						// Font size
	m_uiTimeout = DEFAULT_MSG_TIMEOUT;							// Timeout (auto-close) interval
	m_uiIconID = DEFAULT_MSG_ICONID;							// Message icon ID
	m_nIconSize = DEFAULT_MSG_ICONSIZE;							// Message icon size
	m_byIconPos = DEFAULT_MSG_ICONPLACEMENT;					// Message icon position
	m_byDisplayPos = DEFAULT_MSG_DISPLAYPOS;					// Message display position
	m_uiHMargin = DEFAULT_MSG_HMARGIN;							// Display area horizontal margin
	m_uiVMargin = DEFAULT_MSG_VMARGIN;							// Display area vertical margin
}

RmdMsgStyleSet::RmdMsgStyleSet(const RmdMsgStyleSet& pItem)
{
	// Copy data
	this->m_colorBkgrd = pItem.m_colorBkgrd;					// Background color
	this->m_colorText = pItem.m_colorText;						// Text color
	this->m_strFontName = pItem.m_strFontName;					// Font name
	this->m_uiFontSize = pItem.m_uiFontSize;					// Font size
	this->m_uiTimeout = pItem.m_uiTimeout;						// Timeout (auto-close) interval
	this->m_uiIconID = pItem.m_uiIconID;						// Message icon ID
	this->m_nIconSize = pItem.m_nIconSize;						// Message icon size
	this->m_byIconPos = pItem.m_byIconPos;						// Message icon position
	this->m_byDisplayPos = pItem.m_byDisplayPos;				// Message display position
	this->m_uiHMargin = pItem.m_uiHMargin;						// Display area horizontal margin
	this->m_uiVMargin = pItem.m_uiVMargin;						// Display area vertical margin
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

RmdMsgStyleSet& RmdMsgStyleSet::operator=(const RmdMsgStyleSet& pItem)
{
	// Copy data
	this->m_colorBkgrd = pItem.m_colorBkgrd;					// Background color
	this->m_colorText = pItem.m_colorText;						// Text color
	this->m_strFontName = pItem.m_strFontName;					// Font name
	this->m_uiFontSize = pItem.m_uiFontSize;					// Font size
	this->m_uiTimeout = pItem.m_uiTimeout;						// Timeout (auto-close) interval
	this->m_uiIconID = pItem.m_uiIconID;						// Message icon ID
	this->m_nIconSize = pItem.m_nIconSize;						// Message icon size
	this->m_byIconPos = pItem.m_byIconPos;						// Message icon position
	this->m_byDisplayPos = pItem.m_byDisplayPos;				// Message display position
	this->m_uiHMargin = pItem.m_uiHMargin;						// Display area horizontal margin
	this->m_uiVMargin = pItem.m_uiVMargin;						// Display area vertical margin

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another Reminder message style set data
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RmdMsgStyleSet::Copy(const RmdMsgStyleSet& pItem)
{
	// Copy data
	this->m_colorBkgrd = pItem.m_colorBkgrd;					// Background color
	this->m_colorText = pItem.m_colorText;						// Text color
	this->m_strFontName = pItem.m_strFontName;					// Font name
	this->m_uiFontSize = pItem.m_uiFontSize;					// Font size
	this->m_uiTimeout = pItem.m_uiTimeout;						// Timeout (auto-close) interval
	this->m_uiIconID = pItem.m_uiIconID;						// Message icon ID
	this->m_nIconSize = pItem.m_nIconSize;						// Message icon size
	this->m_byIconPos = pItem.m_byIconPos;						// Message icon position
	this->m_byDisplayPos = pItem.m_byDisplayPos;				// Message display position
	this->m_uiHMargin = pItem.m_uiHMargin;						// Display area horizontal margin
	this->m_uiVMargin = pItem.m_uiVMargin;						// Display area vertical margin
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare data with another Repeat set data
//  Arguments:		pItem - Pointer of input item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL RmdMsgStyleSet::Compare(const RmdMsgStyleSet& pItem) const
{
	BOOL bRetCompare = TRUE;

	// Compare data
	bRetCompare &= (this->m_colorBkgrd == pItem.m_colorBkgrd);		// Background color
	bRetCompare &= (this->m_colorText == pItem.m_colorText);		// Text color
	bRetCompare &= (this->m_strFontName == pItem.m_strFontName);	// Font name
	bRetCompare &= (this->m_uiFontSize == pItem.m_uiFontSize);		// Font size
	bRetCompare &= (this->m_uiTimeout == pItem.m_uiTimeout);		// Timeout (auto-close) interval
	bRetCompare &= (this->m_uiIconID == pItem.m_uiIconID);			// Message icon ID
	bRetCompare &= (this->m_nIconSize == pItem.m_nIconSize);		// Message icon size
	bRetCompare &= (this->m_byIconPos == pItem.m_byIconPos);		// Message icon position
	bRetCompare &= (this->m_byDisplayPos == pItem.m_byDisplayPos);	// Message display position
	bRetCompare &= (this->m_uiHMargin == pItem.m_uiHMargin);		// Display area horizontal margin
	bRetCompare &= (this->m_uiVMargin == pItem.m_uiVMargin);		// Display area vertical margin

	return bRetCompare;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PwrReminderItem
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

PwrReminderItem::PwrReminderItem()
{
	// Init data
	m_bEnabled = FALSE;										// Enable state
	m_nItemID = DEF_PWRREMINDER_MIN_ITEMID;					// Item ID
	m_strMessage = STRING_EMPTY;							// Message content
	m_nEventID = PREVT_AT_SETTIME;							// Event ID
	m_stTime = SYSTEMTIME_ZERO;								// Event time
	m_dwMsgStyle = PRSTYLE_MSGBOX;							// Reminder style
	m_rpsRepeatSet = PWRREPEATSET();						// Repeat set
	m_bUseCustomStyle = FALSE;								// Use message custom style
	m_rmsMsgStyleSet = RMDMSGSTYLESET();					// Reminder message style set
}

PwrReminderItem::PwrReminderItem(const PwrReminderItem& pItem)
{
	// Copy data
	this->m_bEnabled = pItem.m_bEnabled;					// Enable state
	this->m_nItemID = pItem.m_nItemID;						// Item ID
	this->m_strMessage = pItem.m_strMessage;				// Message content
	this->m_nEventID = pItem.m_nEventID;					// Event ID
	this->m_stTime = pItem.m_stTime;						// Event time
	this->m_dwMsgStyle = pItem.m_dwMsgStyle;				// Reminder style
	this->m_rpsRepeatSet.Copy(pItem.m_rpsRepeatSet);		// Repeat set
	this->m_bUseCustomStyle = pItem.m_bUseCustomStyle;		// Use message custom style
	this->m_rmsMsgStyleSet.Copy(pItem.m_rmsMsgStyleSet);	// Reminder message style set
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

PwrReminderItem& PwrReminderItem::operator=(const PwrReminderItem& pItem)
{
	// Copy data
	this->m_bEnabled = pItem.m_bEnabled;					// Enable state
	this->m_nItemID = pItem.m_nItemID;						// Item ID
	this->m_strMessage = pItem.m_strMessage;				// Message content
	this->m_nEventID = pItem.m_nEventID;					// Event ID
	this->m_stTime = pItem.m_stTime;						// Event time
	this->m_dwMsgStyle = pItem.m_dwMsgStyle;				// Reminder style
	this->m_rpsRepeatSet.Copy(pItem.m_rpsRepeatSet);		// Repeat set
	this->m_bUseCustomStyle = pItem.m_bUseCustomStyle;		// Use message custom style
	this->m_rmsMsgStyleSet.Copy(pItem.m_rmsMsgStyleSet);	// Reminder message style set

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another Power Reminder item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::Copy(const PwrReminderItem& pItem)
{
	// Copy data
	this->m_bEnabled = pItem.m_bEnabled;					// Enable state
	this->m_nItemID = pItem.m_nItemID;						// Item ID
	this->m_strMessage = pItem.m_strMessage;				// Message content
	this->m_nEventID = pItem.m_nEventID;					// Event ID
	this->m_stTime = pItem.m_stTime;						// Event time
	this->m_dwMsgStyle = pItem.m_dwMsgStyle;				// Reminder style
	this->m_rpsRepeatSet.Copy(pItem.m_rpsRepeatSet);		// Repeat set
	this->m_bUseCustomStyle = pItem.m_bUseCustomStyle;		// Use message custom style
	this->m_rmsMsgStyleSet.Copy(pItem.m_rmsMsgStyleSet);	// Reminder message style set
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if Power Reminder item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderItem::IsEmpty() const
{
	// Initialize an empty item
	static const PWRREMINDERITEM pwrDummyItem;

	// Compare with this item and return result
	return this->Compare(pwrDummyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderItem::Compare(const PwrReminderItem& pItem) const
{
	BOOL bRet = TRUE;

	// Compare item (do not compare item ID)
	bRet &= (this->m_strMessage == pItem.m_strMessage);
	bRet &= (this->m_nEventID == pItem.m_nEventID);
	bRet &= (this->m_stTime.wHour == pItem.m_stTime.wHour);
	bRet &= (this->m_stTime.wMinute == pItem.m_stTime.wMinute);
	bRet &= (this->m_dwMsgStyle == pItem.m_dwMsgStyle);
	bRet &= (this->m_rpsRepeatSet.Compare(pItem.m_rpsRepeatSet));
	bRet &= (this->m_bUseCustomStyle == pItem.m_bUseCustomStyle);
	bRet &= (this->m_rmsMsgStyleSet.Compare(pItem.m_rmsMsgStyleSet));

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetRepeatSetData
//	Description:	Get access to item RepeatSet info data
//  Arguments:		None
//  Return value:	PWRREPEATSET&
//
//////////////////////////////////////////////////////////////////////////

PWRREPEATSET& PwrReminderItem::GetRepeatSetData(void)
{
	return this->m_rpsRepeatSet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ResetRepeatInfo
//	Description:	Reset RepeatSet info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::ResetRepeatInfo(void)
{
	// Create a new default RepeatSet info data
	const PwrRepeatSet emptyData = PWRREPEATSET();

	// Copy and overwrite current data
	this->m_rpsRepeatSet.Copy(emptyData);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMessageStyleData
//	Description:	Get access to item Message Style info data
//  Arguments:		None
//  Return value:	RMDMSGSTYLESET&
//
//////////////////////////////////////////////////////////////////////////

RMDMSGSTYLESET& PwrReminderItem::GetMessageStyleData(void)
{
	return this->m_rmsMsgStyleSet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ResetMessageStyleInfo
//	Description:	Reset Message Style info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::ResetMessageStyleInfo(void)
{
	// Create a new default Message Style info data
	const RmdMsgStyleSet emptyData = RMDMSGSTYLESET();

	// Copy and overwrite current data
	this->m_rmsMsgStyleSet.Copy(emptyData);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEnabled
//	Description:	Check if item is enabled
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderItem::IsEnabled(void) const
{
	return m_bEnabled;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableItem
//	Description:	Set/change item enable state
//  Arguments:		bEnabled - New enable state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::EnableItem(BOOL bEnabled)
{
	m_bEnabled = bEnabled;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemID
//	Description:	Get Power Reminder item ID
//  Arguments:		None
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT PwrReminderItem::GetItemID(void) const
{
	return m_nItemID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetItemID
//	Description:	Set Power Reminder item ID
//  Arguments:		nItemID - Item ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::SetItemID(UINT nItemID)
{
	m_nItemID = nItemID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMessage
//	Description:	Get Power Reminder item message content
//  Arguments:		None
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR PwrReminderItem::GetMessage(void) const
{
	return m_strMessage.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMessage
//	Description:	Set Power Reminder item message content
//  Arguments:		lpszMessage - Message content
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::SetMessage(LPCTSTR lpszMessage)
{
	m_strMessage = lpszMessage;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetEventID
//	Description:	Get Power Reminder item event ID
//  Arguments:		None
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT PwrReminderItem::GetEventID(void) const
{
	return m_nEventID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetEventID
//	Description:	Set Power Reminder item event ID
//  Arguments:		nEventID - Item event ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::SetEventID(UINT nEventID)
{
	m_nEventID = nEventID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetTime
//	Description:	Get Power Reminder item time data
//  Arguments:		None
//  Return value:	SYSTEMTIME
//
//////////////////////////////////////////////////////////////////////////

SYSTEMTIME PwrReminderItem::GetTime(void) const
{
	return m_stTime;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetTime
//	Description:	Set Power Reminder item time data
//  Arguments:		stTime - Time data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::SetTime(const SYSTEMTIME& stTime)
{
	m_stTime = stTime;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMessageStyle
//	Description:	Get Power Reminder item message style ID
//  Arguments:		None
//  Return value:	DWORD
//
//////////////////////////////////////////////////////////////////////////

DWORD PwrReminderItem::GetMessageStyle(void) const
{
	return m_dwMsgStyle;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMessageStyle
//	Description:	Set Power Reminder item message style ID
//  Arguments:		nMsgStyleID - Item message style ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::SetMessageStyle(DWORD nMsgStyleID)
{
	m_dwMsgStyle = nMsgStyleID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsCustomStyleEnabled
//	Description:	Check if item message custom style is enabled
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderItem::IsCustomStyleEnabled(void) const
{
	return m_bUseCustomStyle;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableCustomStyle
//	Description:	Set/change item message custom style enable state
//  Arguments:		bEnabled - New enable state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::EnableCustomStyle(BOOL bEnabled)
{
	m_bUseCustomStyle = bEnabled;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsRepeatEnabled
//	Description:	Check if item repeat mode is enabled
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderItem::IsRepeatEnabled(void) const
{
	return m_rpsRepeatSet.IsRepeatEnabled();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsDayActive
//	Description:	Check if day of week is active
//  Arguments:		dayOfWeek - Day of week
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderItem::IsDayActive(DAYOFWEEK dayOfWeek) const
{
	return m_rpsRepeatSet.IsDayActive(dayOfWeek);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsAllowSnoozing
//	Description:	Check if item snooze mode is available
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderItem::IsAllowSnoozing(void) const
{
	// If current eventID is not at settime
	if (m_nEventID != PREVT_AT_SETTIME) {
		// Not allow snooze mode
		return FALSE;
	}
	// If repeat option is currently OFF
	if (IsRepeatEnabled() != TRUE) {
		// Not allow snooze mode
		return FALSE;
	}
	// If allow snooze option is OFF
	if (m_rpsRepeatSet.IsAllowSnoozing() != TRUE) {
		// Not allow snooze mode
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSnoozeInterval
//	Description:	Get item snooze interval value
//  Arguments:		None
//  Return value:	INT
//
//////////////////////////////////////////////////////////////////////////

INT PwrReminderItem::GetSnoozeInterval(void) const
{
	return m_rpsRepeatSet.GetSnoozeInterval();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetActiveDays
//	Description:	Get repeat active days
//  Arguments:		None
//  Return value:	BYTE
//
//////////////////////////////////////////////////////////////////////////

BYTE PwrReminderItem::GetActiveDays(void) const
{
	return m_rpsRepeatSet.GetActiveDays();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableRepeat
//	Description:	Set repeat enable state
//  Arguments:		bEnabled - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::EnableRepeat(BOOL bEnabled)
{
	m_rpsRepeatSet.EnableRepeat(bEnabled);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableSnoozing
//	Description:	Set allow snoozing state
//  Arguments:		bEnabled - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::EnableSnoozing(BOOL bEnabled)
{
	m_rpsRepeatSet.EnableSnoozing(bEnabled);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetSnoozeInterval
//	Description:	Set snooze interval data
//  Arguments:		nValue - Interval value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::SetSnoozeInterval(INT nValue)
{
	m_rpsRepeatSet.SetSnoozeInterval(nValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetActiveDays
//	Description:	Set repeat days data
//  Arguments:		byActiveDays - Active days
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::SetActiveDays(BYTE byActiveDays)
{
	m_rpsRepeatSet.SetActiveDays(byActiveDays);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDayActive
//	Description:	Set active state for specific day of week
//  Arguments:		byActiveDays - Active days
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::SetDayActive(DAYOFWEEK dayOfWeek, BOOL bActive)
{
	m_rpsRepeatSet.SetDayActive(dayOfWeek, bActive);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Print
//	Description:	Print reminder item
//  Arguments:		strOutput - Output printed string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::Print(CString& strOutput) const
{
	// Use table, language and core functions
	using namespace MapTable;
	using namespace Language;
	using namespace AppCore;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	CString strEnable = (m_bEnabled == TRUE) ? _T("Enabled") : _T("Disabled");
	CString strMsg = m_strMessage;
	if (strMsg.GetLength() > (MAX_DISP_LOGSTRING_LENGTH + 3)) {
		strMsg = m_strMessage.Left(MAX_DISP_LOGSTRING_LENGTH) + _T("...");
	}
	int nTemp = GetPairedID(IDTable::PwrReminderEvent, m_nEventID);
	CString strEvent = GetLanguageString(ptrLanguage, nTemp);
	if (m_nEventID == PREVT_AT_SETTIME) {
		// Format time string
		CString strFormat = strEvent;
		strEvent = FormatDispTime(ptrLanguage, strFormat, m_stTime);
	}
	nTemp = GetPairedID(IDTable::PwrReminderStyle, m_dwMsgStyle);
	CString strStyle = GetLanguageString(ptrLanguage, nTemp);

	// Print item
	strOutput.Format(_T("State=(%s), ItemID=%d, Msg=(%s), Event=(%s), Style=(%s), Repeat=%d"),
				strEnable, m_nItemID, strMsg, strEvent, strStyle, m_rpsRepeatSet.IsRepeatEnabled());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PwrReminderData
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

PwrReminderData::PwrReminderData()
{
	// Initialize
	m_arrRmdItemList.RemoveAll();
	m_rmdCommonStyle = RMDMSGSTYLESET();
}

PwrReminderData::PwrReminderData(const PwrReminderData& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy reminder data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		PWRREMINDERITEM pwrItem = pData.m_arrRmdItemList.GetAt(nIndex);
		this->m_arrRmdItemList.Add(pwrItem);
	}

	// Copy common message style data
	this->m_rmdCommonStyle.Copy(pData.m_rmdCommonStyle);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

PwrReminderData& PwrReminderData::operator=(const PwrReminderData& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy reminder data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		PWRREMINDERITEM pwrItem = pData.m_arrRmdItemList.GetAt(nIndex);
		this->m_arrRmdItemList.Add(pwrItem);
	}

	// Copy common message style data
	this->m_rmdCommonStyle.Copy(pData.m_rmdCommonStyle);

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Init Power Reminder data (NULL)
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderData::Init()
{
	// Initialize
	m_arrRmdItemList.RemoveAll();
	m_rmdCommonStyle = RMDMSGSTYLESET();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another Power Reminder data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderData::Copy(const PwrReminderData& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy reminder data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		PWRREMINDERITEM pwrItem = pData.m_arrRmdItemList.GetAt(nIndex);
		this->m_arrRmdItemList.Add(pwrItem);
	}

	// Copy common message style data
	this->m_rmdCommonStyle.Copy(pData.m_rmdCommonStyle);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDefaultData
//	Description:	Set default for Power Reminder data
//  Arguments:		ppwrData - Pointer of Power Reminder data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderData::SetDefaultData(void)
{
	// Re-initialize data
	this->Init();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add a Power Reminder item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderData::Add(const PWRREMINDERITEM& pItem)
{
	// If data list is current empty
	if (m_arrRmdItemList.IsEmpty()) {
		// Just add item
		m_arrRmdItemList.Add(pItem);
		return;
	}

	// Check if item exists, if yes, do not add
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		PWRREMINDERITEM pItemTemp = GetItemAt(nIndex);
		if (pItemTemp.Compare(pItem) == TRUE)
			return;
	}

	// Create new temporary data
	PwrReminderData* pNew = new PwrReminderData;
	pNew->m_arrRmdItemList.RemoveAll();

	// Copy common message style data
	pNew->m_rmdCommonStyle.Copy(this->m_rmdCommonStyle);

	// Copy old data to new one
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		PWRREMINDERITEM pwrItem = this->GetItemAt(nIndex);
		pNew->m_arrRmdItemList.Add(pwrItem);
	}

	// Add new item and copy back to old data
	pNew->m_arrRmdItemList.Add(pItem);
	this->Copy(*pNew);

	// Delete data
	pNew->DeleteAll();
	if (pNew != NULL) {
		delete pNew;
		pNew = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Update
//	Description:	Update a Power Reminder item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderData::Update(const PWRREMINDERITEM& pItem)
{
	// If data list is current empty
	if (m_arrRmdItemList.IsEmpty()) {
		// Just add item
		Add(pItem);
		return;
	}

	// Find item index
	int nRetItemIndex = INT_INVALID;
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		if (GetItemAt(nIndex).GetItemID() == pItem.GetItemID()) {
			nRetItemIndex = nIndex;
			break;
		}
	}

	// Update item if found
	if (nRetItemIndex != INT_INVALID) {
		PWRREMINDERITEM& pwrTemp = GetItemAt(nRetItemIndex);
		pwrTemp.Copy(pItem);
	}
	// Otherwise, add new
	else {
		Add(pItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemAt
//	Description:	Get the Power Reminder item at index
//  Arguments:		nIndex - Item index
//  Return value:	PWRREMINDERITEM
//	Notes:			Constant function
//
//////////////////////////////////////////////////////////////////////////

const PWRREMINDERITEM& PwrReminderData::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrRmdItemList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemAt
//	Description:	Get the Power Reminder item at index
//  Arguments:		nIndex - Item index
//  Return value:	PWRREMINDERITEM
//
//////////////////////////////////////////////////////////////////////////

PWRREMINDERITEM& PwrReminderData::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrRmdItemList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Remove
//	Description:	Remove a reminder item by index
//  Arguments:		nAtIndex - Index of item to remove
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderData::Remove(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= GetItemNum()))
		return;

	// Get item data
	PWRREMINDERITEM& pwrItem = GetItemAt(nAtIndex);

	// Reset item value
	pwrItem.Copy(PWRREMINDERITEM());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all Power Reminder data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderData::RemoveAll(void)
{
	// Remove each item
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		Remove(nIndex);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Adjust
//	Description:	Adjust Power Reminder data validity
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderData::Adjust(void)
{
	// Remove garbage items
	for (int nIndex = (GetItemNum() - 1); nIndex >= 0; nIndex--) {
		// Get item
		PWRREMINDERITEM pwrTemp = GetItemAt(nIndex);
		if (!pwrTemp.IsEmpty()) continue;

		// Remove empty if item
		Delete(nIndex);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetNextID
//	Description:	Get next item ID (to add new item)
//  Arguments:		None
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT PwrReminderData::GetNextID(void)
{
	// Get max ID
	UINT nRetNextID = DEF_PWRREMINDER_MIN_ITEMID;
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		PWRREMINDERITEM pwrItem = GetItemAt(nIndex);
		if (pwrItem.GetItemID() > nRetNextID) {
			nRetNextID = pwrItem.GetItemID();
		}
	}

	// Increase value
	nRetNextID++;

	return nRetNextID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemNum
//	Description:	Get number of items
//  Arguments:		None
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

INT_PTR PwrReminderData::GetItemNum(void) const
{
	return m_arrRmdItemList.GetSize();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if item at index is empty
//  Arguments:		nIndex - Item index to check
//  Return value:	BOOL - Result of empty or not
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderData::IsEmpty(int nIndex) const
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= GetItemNum()))
		return TRUE;

	// Check if item is empty
	PWRREMINDERITEM pwrItem = GetItemAt(nIndex);
	return pwrItem.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsAllEmpty
//	Description:	Check if all item are empty
//  Arguments:		None
//  Return value:	BOOL - Result of all item empty
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderData::IsAllEmpty() const
{
	// If there's no item, return TRUE
	if (m_arrRmdItemList.IsEmpty())
		return TRUE;

	// Check each item
	BOOL bAllEmpty = TRUE;
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		if (IsEmpty(nIndex) == FALSE) {
			bAllEmpty = FALSE;
			break;
		}
	}

	return bAllEmpty;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Delete
//	Description:	Delete a reminder item by index
//  Arguments:		nAtIndex - Index of item to delete
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderData::Delete(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= GetItemNum()))
		return;

	// Create new temporary data
	PwrReminderData* pNew = new PwrReminderData;
	pNew->m_arrRmdItemList.RemoveAll();

	// Copy common message style data
	pNew->m_rmdCommonStyle.Copy(this->m_rmdCommonStyle);

	// Copy old data to new one (except the AtIndex item)
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		if (nIndex == nAtIndex) continue;
		pNew->m_arrRmdItemList.Add(this->m_arrRmdItemList.GetAt(nIndex));
	}

	// Copy back to old data
	this->Copy(*pNew);

	// Delete temporary data
	pNew->DeleteAll();
	if (pNew != NULL) {
		delete pNew;
		pNew = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DeleteAll
//	Description:	Delete all Power Reminder data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderData::DeleteAll(void)
{
	// Reset data
	m_arrRmdItemList.RemoveAll();
	m_arrRmdItemList.FreeExtra();
	m_rmdCommonStyle = RMDMSGSTYLESET();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PwrRuntimeItem
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

PwrRuntimeItem::PwrRuntimeItem()
{
	// Init data
	m_nCategory = INT_INVALID;								// Item category
	m_nItemID = INT_NULL;									// Power Reminder item ID
	m_nDisplayFlag = FLAG_OFF;								// Item displaying flag
	m_nSkipFlag = FLAG_OFF;									// Item skip flag
	m_nSnoozeFlag = FLAG_OFF;								// Item snooze trigger flag
	m_stNextSnoozeTime = SYSTEMTIME_ZERO;					// Next snooze trigger time
}

PwrRuntimeItem::PwrRuntimeItem(const PwrRuntimeItem& pItem)
{
	// Copy data
	this->m_nCategory = pItem.m_nCategory;					// Item category
	this->m_nItemID = pItem.m_nItemID;						// Power Reminder item ID
	this->m_nDisplayFlag = pItem.m_nDisplayFlag;			// Item displaying flag
	this->m_nSkipFlag = pItem.m_nSkipFlag;					// Item skip flag
	this->m_nSnoozeFlag = pItem.m_nSnoozeFlag;				// Item snooze trigger flag
	this->m_stNextSnoozeTime = pItem.m_stNextSnoozeTime;	// Next snooze trigger time
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

PwrRuntimeItem& PwrRuntimeItem::operator=(const PwrRuntimeItem& pItem)
{
	// Copy data
	this->m_nCategory = pItem.m_nCategory;					// Item category
	this->m_nItemID = pItem.m_nItemID;						// Power Reminder item ID
	this->m_nDisplayFlag = pItem.m_nDisplayFlag;			// Item displaying flag
	this->m_nSkipFlag = pItem.m_nSkipFlag;					// Item skip flag
	this->m_nSnoozeFlag = pItem.m_nSnoozeFlag;				// Item snooze trigger flag
	this->m_stNextSnoozeTime = pItem.m_stNextSnoozeTime;	// Next snooze trigger time

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another Power++ runtime info item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRuntimeItem::Copy(const PwrRuntimeItem& pItem)
{
	// Copy data
	this->m_nCategory = pItem.m_nCategory;					// Item category
	this->m_nItemID = pItem.m_nItemID;						// Power Reminder item ID
	this->m_nDisplayFlag = pItem.m_nDisplayFlag;			// Item displaying flag
	this->m_nSkipFlag = pItem.m_nSkipFlag;					// Item skip flag
	this->m_nSnoozeFlag = pItem.m_nSnoozeFlag;				// Item snooze trigger flag
	this->m_stNextSnoozeTime = pItem.m_stNextSnoozeTime;	// Next snooze trigger time
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CalcNextSnoozeTime
//	Description:	Calculate Power Reminder item next snooze time
//  Arguments:		nInterval - Snooze interval
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRuntimeItem::CalcNextSnoozeTime(int nInterval)
{
	// Calculate time with offset
	AppCore::CalcTimeOffset(m_stNextSnoozeTime, nInterval);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetCategory
//	Description:	Get item category
//  Arguments:		None
//  Return value:	INT
//
//////////////////////////////////////////////////////////////////////////

INT PwrRuntimeItem::GetCategory(void) const
{
	return m_nCategory;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetCategory
//	Description:	Set item category
//  Arguments:		nValue - Value to set
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRuntimeItem::SetCategory(INT nValue)
{
	m_nCategory = nValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemID
//	Description:	Get item ID
//  Arguments:		None
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT PwrRuntimeItem::GetItemID(void) const
{
	return m_nItemID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetItemID
//	Description:	Set item ID
//  Arguments:		nValue - Value to set
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRuntimeItem::SetItemID(UINT nValue)
{
	m_nItemID = nValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDisplayFlag
//	Description:	Get item displaying flag
//  Arguments:		None
//  Return value:	INT
//
//////////////////////////////////////////////////////////////////////////

INT PwrRuntimeItem::GetDisplayFlag(void) const
{
	return m_nDisplayFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDisplayFlag
//	Description:	Set item displaying flag
//  Arguments:		nValue - Value to set
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRuntimeItem::SetDisplayFlag(INT nValue)
{
	m_nDisplayFlag = nValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSkipFlag
//	Description:	Get item skip flag
//  Arguments:		None
//  Return value:	INT
//
//////////////////////////////////////////////////////////////////////////

INT PwrRuntimeItem::GetSkipFlag(void) const
{
	return m_nSkipFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetSkipFlag
//	Description:	Set item skip flag
//  Arguments:		nValue - Value to set
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRuntimeItem::SetSkipFlag(INT nValue)
{
	m_nSkipFlag = nValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSnoozeFlag
//	Description:	Get item snooze flag
//  Arguments:		None
//  Return value:	INT
//
//////////////////////////////////////////////////////////////////////////

INT PwrRuntimeItem::GetSnoozeFlag(void) const
{
	return m_nSnoozeFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetSnoozeFlag
//	Description:	Set item snooze flag
//  Arguments:		nValue - Value to set
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRuntimeItem::SetSnoozeFlag(INT nValue)
{
	m_nSnoozeFlag = nValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetTime
//	Description:	Get next snooze trigger time
//  Arguments:		None
//  Return value:	SYSTEMTIME
//
//////////////////////////////////////////////////////////////////////////

SYSTEMTIME PwrRuntimeItem::GetTime(void) const
{
	return m_stNextSnoozeTime;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetTime
//	Description:	Set next snooze trigger time
//  Arguments:		stTime - Time data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrRuntimeItem::SetTime(const SYSTEMTIME& stTime)
{
	m_stNextSnoozeTime = stTime;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagHISTORYINFODATA
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

HistoryInfoData::HistoryInfoData()
{
	// Init data
	m_bInitState = FALSE;									// Init state
	m_nCategoryID = INT_NULL;								// Category ID
	m_stTimestamp = SYSTEMTIME_ZERO;						// Timestamp of history
	m_nItemID = INT_NULL;									// Item ID
	m_nActionID = INT_NULL;									// History action ID
	m_bActionResult = FALSE;								// Action result
	m_dwErrorCode = INT_NULL;								// Returned error code
	m_strDescription = STRING_EMPTY;						// History description (attached info)
}

HistoryInfoData::HistoryInfoData(const HistoryInfoData& pData)
{
	// Copy data
	this->m_bInitState = pData.m_bInitState;				// Init state
	this->m_nCategoryID = pData.m_nCategoryID;				// Category ID
	this->m_stTimestamp = pData.m_stTimestamp;				// Timestamp of history
	this->m_nItemID = pData.m_nItemID;						// Item ID
	this->m_nActionID = pData.m_nActionID;					// History action ID
	this->m_bActionResult = pData.m_bActionResult;			// Action result
	this->m_dwErrorCode = pData.m_dwErrorCode;				// Returned error code
	this->m_strDescription = pData.m_strDescription;		// History description (attached info)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

HistoryInfoData& HistoryInfoData::operator=(const HistoryInfoData& pData)
{
	// Copy data
	this->m_bInitState = pData.m_bInitState;				// Init state
	this->m_nCategoryID = pData.m_nCategoryID;				// Category ID
	this->m_stTimestamp = pData.m_stTimestamp;				// Timestamp of history
	this->m_nItemID = pData.m_nItemID;						// Item ID
	this->m_nActionID = pData.m_nActionID;					// History action ID
	this->m_bActionResult = pData.m_bActionResult;			// Action result
	this->m_dwErrorCode = pData.m_dwErrorCode;				// Returned error code
	this->m_strDescription = pData.m_strDescription;		// History description (attached info)

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another action history data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HistoryInfoData::Copy(const HistoryInfoData& pData)
{
	// Copy data
	this->m_bInitState = pData.m_bInitState;				// Init state
	this->m_nCategoryID = pData.m_nCategoryID;				// Category ID
	this->m_stTimestamp = pData.m_stTimestamp;				// Timestamp of history
	this->m_nItemID = pData.m_nItemID;						// Item ID
	this->m_nActionID = pData.m_nActionID;					// History action ID
	this->m_bActionResult = pData.m_bActionResult;			// Action result
	this->m_dwErrorCode = pData.m_dwErrorCode;				// Returned error code
	this->m_strDescription = pData.m_strDescription;		// History description (attached info)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Initialization
//  Arguments:		nCategoryID - Category ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HistoryInfoData::Init(UINT nCategoryID)
{
	// Reset data
	RemoveAll();
	m_bInitState = TRUE;									// Init state
	m_nCategoryID = nCategoryID;							// Category ID
	m_stTimestamp = AppCore::GetCurSysTime();				// Timestamp of history
	m_dwErrorCode = INT_NULL;								// Returned error code
	m_strDescription = STRING_EMPTY;						// History description (attached info)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all action data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HistoryInfoData::RemoveAll(void)
{
	// Create an empty item
	const HistoryInfoData emptyItem;

	// Copy and overwrite current data
	this->Copy(emptyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagREGISTRYKEY
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagREGISTRYKEY::tagREGISTRYKEY()
{
	// Initialize
	this->strKeyName = STRING_EMPTY;						// Key name (string)
	this->nValueType = REGTYPE_NONE;						// Value type
	this->regValue = REGISTRYVALUE();						// Data values
}

tagREGISTRYKEY::tagREGISTRYKEY(const tagREGISTRYKEY& pItem)
{
	// Copy data
	this->strKeyName = pItem.strKeyName;					// Key name (string)
	this->nValueType = pItem.nValueType;					// Value type
	this->regValue = pItem.regValue;						// Data values
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagREGISTRYVALUE
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagREGISTRYVALUE::tagREGISTRYVALUE()
{
	// Initialize
	this->strValue = STRING_EMPTY;							// String value
	this->dwValue = INT_NULL;								// DWORD (32-bit) value
	this->qwValue = INT_NULL;								// QWORD (64-bit) value
	this->astrStringValue.RemoveAll();						// Multi-string value
}

tagREGISTRYVALUE::tagREGISTRYVALUE(const tagREGISTRYVALUE& pItem)
{
	// Copy data
	this->strValue = pItem.strValue;						// String value
	this->dwValue = pItem.dwValue;							// DWORD (32-bit) value
	this->qwValue = pItem.qwValue;							// QWORD (64-bit) value
	this->astrStringValue.Copy(pItem.astrStringValue);		// Multi-string value
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~REGISTRYVALUE
//	Description:	Destructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagREGISTRYVALUE::~tagREGISTRYVALUE()
{
	// Cleanup array data
	this->astrStringValue.RemoveAll();
	this->astrStringValue.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagREGISTRYKEY& tagREGISTRYKEY::operator=(const tagREGISTRYKEY& pItem)
{
	// Copy data
	this->strKeyName = pItem.strKeyName;					// Key name (string)
	this->nValueType = pItem.nValueType;					// Value type
	this->regValue = pItem.regValue;						// Data values

	return *this;
}

tagREGISTRYVALUE& tagREGISTRYVALUE::operator=(const tagREGISTRYVALUE& pItem)
{
	// Copy data
	this->strValue = pItem.strValue;						// String value
	this->dwValue = pItem.dwValue;							// DWORD (32-bit) value
	this->qwValue = pItem.qwValue;							// QWORD (64-bit) value
	this->astrStringValue.Copy(pItem.astrStringValue);		// Multi-string value

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another registry key item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagREGISTRYKEY::Copy(const tagREGISTRYKEY& pItem)
{
	// Copy data
	this->strKeyName = pItem.strKeyName;					// Key name (string)
	this->nValueType = pItem.nValueType;					// Value type
	this->regValue = pItem.regValue;						// Data values
}

void tagREGISTRYVALUE::Copy(const tagREGISTRYVALUE& pItem)
{
	// Copy data
	this->strValue = pItem.strValue;						// String value
	this->dwValue = pItem.dwValue;							// DWORD (32-bit) value
	this->qwValue = pItem.qwValue;							// QWORD (64-bit) value
	this->astrStringValue.Copy(pItem.astrStringValue);		// Multi-string value
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all registry key info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagREGISTRYKEY::RemoveAll(void)
{
	// Reset data
	this->strKeyName = STRING_EMPTY;						// Key name (string)
	this->nValueType = REGTYPE_NONE;						// Value type
	this->regValue.RemoveAll();								// Data values
}

void tagREGISTRYVALUE::RemoveAll(void)
{
	// Cleanup array data
	this->astrStringValue.RemoveAll();
	this->astrStringValue.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetKeyName
//	Description:	Get/set registry key name
//  Arguments:		strKeyName  - Registry key name (out)
//					nResourceID - Registry key name resource string ID (in)
//					lpszKeyName - Registry key name (in)
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void tagREGISTRYKEY::GetKeyName(CString& strKeyName) const
{
	// Return key name
	strKeyName = this->strKeyName;
}

AFX_INLINE LPCTSTR tagREGISTRYKEY::GetKeyName(void) const
{
	// Return key name
	return this->strKeyName.GetString();
}

void tagREGISTRYKEY::SetKeyName(UINT nResourceID)
{
	// Set key name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	this->SetKeyName(strKeyName);
}

void tagREGISTRYKEY::SetKeyName(LPCTSTR lpszKeyName)
{
	// Set key name
	this->strKeyName = lpszKeyName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetValueType
//	Description:	Get/set registry key value type
//  Arguments:		dwValue - Value type (in)
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE UINT tagREGISTRYKEY::GetValueType(void) const
{
	// Return value type
	return this->nValueType;
}

void tagREGISTRYKEY::SetValueType(UINT nValueType)
{
	// Set value type
	this->nValueType = nValueType;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetStringValue
//	Description:	Get/set registry key string value
//  Arguments:		strValue  - String value (out)
//					lpszValue - String value (in)
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

void tagREGISTRYKEY::GetStringValue(CString& strValue) const
{
	// If registry key data type is not string
	if (this->nValueType != REGTYPE_STRING) {
		// Return empty string
		strValue.Empty();
	}
	else {
		// Return string value
		strValue = this->regValue.strValue;
	}
}

LPCTSTR tagREGISTRYKEY::GetStringValue(void) const
{
	// If registry key data type is not string
	if (this->nValueType != REGTYPE_STRING) {
		// Return empty string
		return STRING_EMPTY;
	}
	else {
		// Return string value
		return this->regValue.strValue.GetString();
	}
}

void tagREGISTRYKEY::SetStringValue(LPCTSTR lpszValue)
{
	// If registry key data type is not set
	if (this->nValueType == REGTYPE_NONE) {
		this->nValueType = REGTYPE_STRING;
	}

	// Set string value
	this->regValue.strValue = lpszValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetDWordValue
//	Description:	Get/set registry key DWORD (32-bit) value
//  Arguments:		dwValue - DWORD (32-bit) value (in)
//  Return value:	DWORD
//
//////////////////////////////////////////////////////////////////////////

DWORD tagREGISTRYKEY::GetDWordValue(void) const
{
	// If registry key data type is not DWORD (32-bit)
	if (this->nValueType != REGTYPE_DWORD32) {
		// Return zero (NULL)
		return INT_NULL;
	}
	else {
		// Return DWORD (32-bit) value
		return this->regValue.dwValue;
	}
}

void tagREGISTRYKEY::SetDWordValue(DWORD dwValue)
{
	// If registry key data type is not set
	if (this->nValueType == REGTYPE_NONE) {
		this->nValueType = REGTYPE_DWORD32;
	}

	// Set DWORD (32-bit) value
	this->regValue.dwValue = dwValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetQWordValue
//	Description:	Get/set registry key QWORD (64-bit) value
//  Arguments:		qwValue - QWORD (64-bit) value (in)
//  Return value:	QWORD
//
//////////////////////////////////////////////////////////////////////////

QWORD tagREGISTRYKEY::GetQWordValue(void) const
{
	// If registry key data type is not QWORD (64-bit)
	if (this->nValueType != REGTYPE_QWORD64) {
		// Return zero (NULL)
		return INT_NULL;
	}
	else {
		// Return QWORD (64-bit) value
		return this->regValue.qwValue;
	}
}

void tagREGISTRYKEY::SetQWordValue(QWORD qwValue)
{
	// If registry key data type is not set
	if (this->nValueType == REGTYPE_NONE) {
		this->nValueType = REGTYPE_QWORD64;
	}

	// Set QWORD (64-bit) value
	this->regValue.qwValue = qwValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetMultiStringValue
//	Description:	Get/set registry key Multi-string value
//  Arguments:		astrValue - Multi-string value (in/out)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagREGISTRYKEY::GetMultiStringValue(CStringArray& astrValue) const
{
	// If registry key data type is not Multi-string
	if (this->nValueType != REGTYPE_MULTISTRING) {
		// Return empty value
		astrValue.RemoveAll();
		astrValue.FreeExtra();
	}
	else {
		// Return Multi-string value
		astrValue.Copy(this->regValue.astrStringValue);
	}
}

void tagREGISTRYKEY::SetMultiStringValue(CStringArray& astrValue)
{
	// If registry key data type is not set
	if (this->nValueType == REGTYPE_NONE) {
		this->nValueType = REGTYPE_MULTISTRING;
	}

	// Set Multi-string value
	this->regValue.astrStringValue.Copy(astrValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegistryInfo
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

RegistryInfo::RegistryInfo()
{
	// Initialize
	m_hRootKey = NULL;										// Root key (HKEY)
	m_strRootKey = STRING_EMPTY;							// Root key (string)
	m_astrSubkeyPath.RemoveAll();							// Subkey path (string array)
	m_strProfileName = STRING_EMPTY;						// Profile key name (string)
	m_strAppName = STRING_EMPTY;							// App name (string)
	m_astrSectionArray.RemoveAll();							// Section array (string)
	m_regKeyInfo = REGISTRYKEY();							// Registry key info
}

RegistryInfo::RegistryInfo(const RegistryInfo& pItem)
{
	// Copy data
	m_hRootKey = pItem.m_hRootKey;							// Root key (HKEY)
	m_strRootKey = pItem.m_strRootKey;						// Root key (string)
	m_astrSubkeyPath.Copy(pItem.m_astrSubkeyPath);			// Subkey path (string array)
	m_strProfileName = pItem.m_strProfileName;				// Profile key name (string)
	m_strAppName = pItem.m_strAppName;						// App name (string)
	m_astrSectionArray.Copy(pItem.m_astrSectionArray);		// Section array (string)
	m_regKeyInfo.Copy(pItem.m_regKeyInfo);					// Registry key info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

RegistryInfo& RegistryInfo::operator=(const RegistryInfo& pItem)
{
	// Copy data
	m_hRootKey = pItem.m_hRootKey;							// Root key (HKEY)
	m_strRootKey = pItem.m_strRootKey;						// Root key (string)
	m_astrSubkeyPath.Copy(pItem.m_astrSubkeyPath);			// Subkey path (string array)
	m_strProfileName = pItem.m_strProfileName;				// Profile key name (string)
	m_strAppName = pItem.m_strAppName;						// App name (string)
	m_astrSectionArray.Copy(pItem.m_astrSectionArray);		// Section array (string)
	m_regKeyInfo.Copy(pItem.m_regKeyInfo);					// Registry key info

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another registry info item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::Copy(const RegistryInfo& pItem)
{
	// Copy data
	m_hRootKey = pItem.m_hRootKey;							// Root key (HKEY)
	m_strRootKey = pItem.m_strRootKey;						// Root key (string)
	m_astrSubkeyPath.Copy(pItem.m_astrSubkeyPath);			// Subkey path (string array)
	m_strProfileName = pItem.m_strProfileName;				// Profile key name (string)
	m_strAppName = pItem.m_strAppName;						// App name (string)
	m_astrSectionArray.Copy(pItem.m_astrSectionArray);		// Section array (string)
	m_regKeyInfo.Copy(pItem.m_regKeyInfo);					// Registry key info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all registry key info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::RemoveAll(void)
{
	// Reset data
	m_hRootKey = NULL;										// Root key (HKEY)
	m_strRootKey = STRING_EMPTY;							// Root key (string)
	m_astrSubkeyPath.RemoveAll();							// Subkey path (string array)
	m_strProfileName = STRING_EMPTY;						// Profile key name (string)
	m_strAppName = STRING_EMPTY;							// App name (string)
	m_astrSectionArray.RemoveAll();							// Section array (string)
	m_regKeyInfo.RemoveAll();								// Registry key info

	// Cleanup extra memory for array data
	m_astrSubkeyPath.FreeExtra();							// Subkey path (string array)
	m_astrSectionArray.FreeExtra();							// Section array (string)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetRootKey
//	Description:	Get root key
//  Arguments:		None
//  Return value:	HKEY
//
//////////////////////////////////////////////////////////////////////////

HKEY RegistryInfo::GetRootKey(void) const
{
	return m_hRootKey;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetRegistryKey
//	Description:	Get registry key info data
//  Arguments:		None
//  Return value:	const REGISTRYKEY&
//
//////////////////////////////////////////////////////////////////////////

const REGISTRYKEY& RegistryInfo::GetRegistryKey(void) const
{
	return m_regKeyInfo;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetRootKeyName
//	Description:	Get root key name
//  Arguments:		None
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR RegistryInfo::GetRootKeyName(void) const
{
	return m_strRootKey;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSubkeyPath
//	Description:	Get Subkey path array
//  Arguments:		arrOutput - Output string array
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::GetSubkeyPath(CStringArray& arrOutput) const
{
	// Empty destination output array
	arrOutput.RemoveAll();
	arrOutput.FreeExtra();

	// Copy data
	arrOutput.Copy(m_astrSubkeyPath);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetProfileName
//	Description:	Get Profile key name part
//  Arguments:		None
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR RegistryInfo::GetProfileName(void) const
{
	return m_strProfileName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppName
//	Description:	Get application name part
//  Arguments:		None
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR RegistryInfo::GetAppName(void) const
{
	return m_strAppName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSectionName
//	Description:	Get Section name array
//  Arguments:		arrOutput - Output string array
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::GetSectionName(CStringArray& arrOutput) const
{
	// Empty destination output array
	arrOutput.RemoveAll();
	arrOutput.FreeExtra();

	// Copy data
	arrOutput.Copy(m_astrSectionArray);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetRootKey
//	Description:	Set registry info root key
//  Arguments:		hRootKey - Root key handle
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::SetRootKey(HKEY hRootKey)
{
	m_hRootKey = hRootKey;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetRootKeyName
//	Description:	Set registry info root key name
//  Arguments:		nResourceID		- Root key name resource string ID (in)
//					lpszRootkeyName - Root key name (string) (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::SetRootKeyName(UINT nResourceID)
{
	// Set root key name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	SetRootKeyName(strKeyName);
}

void RegistryInfo::SetRootKeyName(LPCTSTR lpszRootkeyName)
{
	// Set root key name (string)
	m_strRootKey = lpszRootkeyName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetSubkeyPath
//	Description:	Set registry info sub-key path
//  Arguments:		nResourceID	   - Sub-key path resource string ID (in)
//					lpszSubKeyName - Sub-key name (string) (in)
//					astrSubkeyPath - Sub-key path array
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::SetSubkeyPath(UINT nResourceID)
{
	// Add sub-key path (by resource ID)
	CString strSubKeyName;
	VERIFY(strSubKeyName.LoadString(nResourceID));
	SetSubkeyPath(strSubKeyName);
}

void RegistryInfo::SetSubkeyPath(LPCTSTR lpszSubKeyName)
{
	// Add sub-key path (string)
	m_astrSubkeyPath.Add(lpszSubKeyName);
}

void RegistryInfo::SetSubkeyPath(CStringArray& astrSubkeyPath)
{
	// Set sub-key path
	m_astrSubkeyPath.Copy(astrSubkeyPath);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetProfileName
//	Description:	Set registry info profile key name
//  Arguments:		nResourceID - Profile key name resource string ID (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::SetProfileName(UINT nResourceID)
{
	// Set profile key name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	SetProfileName(strKeyName);
}

void RegistryInfo::SetProfileName(LPCTSTR lpszProfileName)
{
	// Set profile key name (string)
	m_strProfileName = lpszProfileName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppName
//	Description:	Set registry info app name
//  Arguments:		nResourceID - App name resource string ID (in)
//					lpszAppName - App name (string) (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::SetAppName(UINT nResourceID)
{
	// Set app name (by resource ID)
	CString strAppName;
	VERIFY(strAppName.LoadString(nResourceID));
	SetAppName(strAppName);
}

void RegistryInfo::SetAppName(LPCTSTR lpszAppName)
{
	// Set app name (string)
	m_strAppName = lpszAppName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetSectionName
//	Description:	Set registry info section name
//  Arguments:		nResourceID		 - Section name resource string ID (in)
//					lpszSectionName  - Section name (string) (in)
//					astrSectionArray - Section name array (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::SetSectionName(UINT nResourceID)
{
	// Add section name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	m_astrSectionArray.Add(strKeyName);
}

void RegistryInfo::SetSectionName(LPCTSTR lpszSectionName)
{
	// Add section name (string)
	m_astrSectionArray.Add(lpszSectionName);
}

void RegistryInfo::SetSectionName(CStringArray& astrSectionArray)
{
	// Set section name array
	m_astrSectionArray.Copy(astrSectionArray);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Substring
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

Substring::Substring()
{
	// Initialization
	m_strLeft = STRING_EMPTY;				// Left part
	m_strMid = STRING_EMPTY;				// Middle part
	m_strRight = STRING_EMPTY;				// Right part
}

Substring::Substring(const Substring& pData)
{
	// Copy data
	m_strLeft = pData.m_strLeft;			// Left part
	m_strMid = pData.m_strMid;				// Middle part
	m_strRight = pData.m_strRight;			// Right part
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

Substring& Substring::operator=(const Substring& pData)
{
	// Copy data
	m_strLeft = pData.m_strLeft;			// Left part
	m_strMid = pData.m_strMid;				// Middle part
	m_strRight = pData.m_strRight;			// Right part

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another substring data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void Substring::Copy(const Substring& pData)
{
	// Copy data
	m_strLeft = pData.m_strLeft;			// Left part
	m_strMid = pData.m_strMid;				// Middle part
	m_strRight = pData.m_strRight;			// Right part
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all registry key info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void Substring::RemoveAll(void)
{
	// Reset data
	m_strLeft.Empty();						// Left part
	m_strMid.Empty();						// Middle part
	m_strRight.Empty();						// Right part
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if data is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL Substring::IsEmpty(void) const
{
	return (m_strLeft.IsEmpty() && m_strMid.IsEmpty() && m_strRight.IsEmpty());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TrimLeft
//	Description:	Trim spaces for left part
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void Substring::TrimLeft(void)
{
	m_strLeft.Trim();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TrimMid
//	Description:	Trim spaces for middle part
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void Substring::TrimMid(void)
{
	m_strMid.Trim();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TrimAll
//	Description:	Trim spaces for all parts
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void Substring::TrimAll(void)
{
	m_strLeft.Trim();
	m_strMid.Trim();
	m_strRight.Trim();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Right
//	Description:	Trim spaces for right part
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void Substring::TrimRight(void)
{
	m_strRight.Trim();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Left
//	Description:	Get left part
//  Arguments:		None
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE LPCTSTR Substring::Left(void) const
{
	return (m_strLeft.GetString());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Mid
//	Description:	Get middle part
//  Arguments:		None
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE LPCTSTR Substring::Mid(void) const
{
	return (m_strMid.GetString());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Right
//	Description:	Get right part
//  Arguments:		None
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE LPCTSTR Substring::Right(void) const
{
	return (m_strRight.GetString());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetLeft
//	Description:	Set left part
//  Arguments:		lpszSrc - Source string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void Substring::SetLeft(LPCTSTR lpszSrc)
{
	m_strLeft = lpszSrc;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMid
//	Description:	Set middle part
//  Arguments:		lpszSrc - Source string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void Substring::SetMid(LPCTSTR lpszSrc)
{
	m_strMid = lpszSrc;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetRight
//	Description:	Set right part
//  Arguments:		lpszSrc - Source string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void Substring::SetRight(LPCTSTR lpszSrc)
{
	m_strRight = lpszSrc;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PerformanceCounter
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

PerformanceCounter::PerformanceCounter()
{
	// Initialization
	this->m_bIsRunning = FALSE;
	QueryPerformanceFrequency(&m_liFrequency);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~PerformanceCounter
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

PerformanceCounter::~PerformanceCounter()
{
	// Stop counting
	this->Stop();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Start
//	Description:	Start performance counter
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PerformanceCounter::Start(void)
{
	// Start performance counter
	if (!m_bIsRunning) {
		QueryPerformanceCounter(&m_liStartTime);
		this->m_bIsRunning = TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Stop
//	Description:	Stop performance counter
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PerformanceCounter::Stop(void)
{
	// Stop performance counter
	if (m_bIsRunning) {
		QueryPerformanceCounter(&m_liEndTime);
		this->m_bIsRunning = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetElapsedTime
//	Description:	Get function execution elapsed time
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

double PerformanceCounter::GetElapsedTime(BOOL bToMillisecs) const
{
	// Get elapsed time
	double dRetCounter = static_cast<double>(m_liEndTime.QuadPart - m_liStartTime.QuadPart) / m_liFrequency.QuadPart;
	if (bToMillisecs == TRUE) {
		dRetCounter *= 1000;
	}
	return dRetCounter;
}


//////////////////////////////////////////////////////////////////////////
//
//	Define core methods for program
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLanguageName
//	Description:	Get the title name of current language
//  Arguments:		nCurLanguage	- Current language ID
//					bGetDescription - Get language package description
//					pszResult		- Result string pointer (in/out)
//  Return value:	LPCTSTR - Language name
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR Language::GetLanguageName(UINT nCurLanguage, BOOL bGetDescription /* = FALSE */, LPTSTR pszResult /* = NULL */)
{
	// Load language table package
	LANGTABLE_PTR ptrLangTable = LoadLanguageTable(nCurLanguage);
	if (ptrLangTable == NULL) {
		// Unknown result
		if (pszResult != NULL)	pszResult = VALUE_UNKNOWN;
		return VALUE_UNKNOWN;
	}

	// Get language package info
	UINT nInfoTargetID = (bGetDescription) ? LANGPACKINFO_DESCRIPTIONFULL : LANGPACKINFO_LANGNAMEID;
	LPCTSTR lpszRetInfoString = GetLanguageString(ptrLangTable, nInfoTargetID);
	if (IS_NULL_STRING(lpszRetInfoString)) {
		// Unknown result
		lpszRetInfoString = VALUE_UNKNOWN;
	}

	// Return result
	if (pszResult != NULL) {
		_tcscpy(pszResult, lpszRetInfoString);
	}

	return lpszRetInfoString;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadLanguageTable
//	Description:	Load language table by specified language option
//  Arguments:		nCurLanguage   - Current language ID
//					pszRetLangName - Pointer to return language table name
//					pnSize		   - Pointer to return language table size
//  Return value:	LANGTABLE_PTR - Language package pointer
//
//////////////////////////////////////////////////////////////////////////

LANGTABLE_PTR Language::LoadLanguageTable(UINT nCurLanguage, LPTSTR pszRetLangName /* = NULL */, int* pnSize /* = NULL */)
{
	LANGTABLE_PTR ptrLangTable = NULL;

	switch (nCurLanguage)
	{
	case APP_LANGUAGE_ENGLISH:
		// Language: English (United States)
		ptrLangTable = &langtable_en_US;
		break;
	case APP_LANGUAGE_VIETNAMESE:
		// Language: Vietnamese (Vietnam)
		ptrLangTable = &langtable_vi_VN;
		break;
	case APP_LANGUAGE_SIMPCHINESE:
		// Language: Simplified Chinese (China mainland)
		ptrLangTable = &langtable_zh_CH;
		break;
	default:
		// Default language: English
		ptrLangTable = &langtable_en_US;
		break;
	}

	// Get language table name ID
	if ((pszRetLangName != NULL) && (ptrLangTable != NULL)) {
		LPCTSTR lpszLangName = GetLanguageString(ptrLangTable, LANGPACKINFO_LANGNAMEID);
		_tcscpy(pszRetLangName, lpszLangName);
	}

	// Get language table size
	if ((pnSize != NULL) && (ptrLangTable != NULL)) {
		*pnSize = ptrLangTable->size();
	}

	return ptrLangTable;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLanguageString
//	Description:	Find and return language string by ID
//  Arguments:		ptLanguage - Language package pointer
//					nID		   - Language string ID
//					pszResult  - Result string (reference-type)
//  Return value:	LPCTSTR	- Language string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR Language::GetLanguageString(LANGTABLE_PTR ptLanguage, UINT nID, LPTSTR pszResult /* = NULL */)
{
	// Return NULL string if language table is empty
	if ((ptLanguage == NULL) || (ptLanguage->empty())) {
		if (pszResult != NULL) pszResult = STRING_NULL;
		return STRING_NULL;
	}

	// Find and return corresponding language string paired with specified ID
	for (int nIndex = 0; nIndex < ptLanguage->size(); nIndex++) {
		LANGTEXT langString = ptLanguage->at(nIndex);

		if (langString.dwLangStringID == nID) {
			if (pszResult != NULL) {
				_tcscpy(pszResult, langString.lpszLangString);
			}
			return langString.lpszLangString;
		}
	}

	// Return NULL string if not found
	if (pszResult != NULL) {
		pszResult = STRING_NULL;
	}
	return STRING_NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ExecutePowerAction
//	Description:	Main power action function
//  Arguments:		nActionType - Type of action
//					nMessage	- Action message
//					dwErrorCode - Return error code (ref-value)
//  Return value:	BOOL - Result of action execution
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::ExecutePowerAction(UINT nActionType, UINT nMessage, DWORD& dwErrCode)
{
	BOOL bRet = TRUE;

	// Execute Power Actions in here
	switch (nActionType)
	{
		// Monitor typed action
		case APP_ACTIONTYPE_MONITOR:
			// Turn off display
			PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, (WPARAM)nMessage, (LPARAM)2);
			dwErrCode = APP_ERROR_SUCCESS;
			break;

		// Power typed actions
		case APP_ACTIONTYPE_POWER:
		{
			switch (nMessage)
			{
				case APP_MESSAGE_SHUTDOWN:
				case APP_MESSAGE_REBOOT:
				case APP_MESSAGE_SIGNOUT:
				{
					// Force action
					UINT uExitWinExFlags = nMessage;
					uExitWinExFlags |= EWX_FORCE;

					HANDLE hToken;
					TOKEN_PRIVILEGES tkPrivileges;

					// Get process token
					if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
						// Get error code
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, OpenProcessToken failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}

					// Lookup privilege value
					if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkPrivileges.Privileges[0].Luid)) {
						// Get error code
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, LookupPrivilegeValue failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}

					// Adjust token privileges
					tkPrivileges.PrivilegeCount = 1;
					tkPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
					if (!AdjustTokenPrivileges(hToken, FALSE, &tkPrivileges, 0, (PTOKEN_PRIVILEGES)NULL, 0)) {
						// Adjust token privileges failed
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, AdjustTokenPrivileges failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}

					// Exit Windows
					if (!ExitWindowsEx(uExitWinExFlags, 0)) {
						// Get exit Windows error
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, ExitWindowsEx failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}

				} break;

				case APP_MESSAGE_SLEEP:
					// Sleep mode
					if (!SetSuspendState(FALSE, FALSE, FALSE)) {		// Stand by (sleep)
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, SetSuspendState failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}
					break;

				case APP_MESSAGE_HIBERNATE:
					// Hibernate mode
					if (!SetSuspendState(TRUE, FALSE, FALSE)) {			// Hibernate
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, SetSuspendState failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}
					break;
			}
		} break;

	default:
		// Wrong argument
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
		TRACE_FORMAT("Error: Cannot execute action, wrong argument!!! (Code: 0x%08X)", dwErrCode);
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		bRet = FALSE;
		break;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ExecutePowerActionDummy
//	Description:	Dummy power action function (use for testing)
//  Arguments:		nActionType - Type of action
//					nMessage	- Action message
//					dwErrorCode - Return error code (ref-value)
//  Return value:	BOOL - Result of action execution
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::ExecutePowerActionDummy(UINT nActionType, UINT nMessage, DWORD& dwErrCode)
{
	// Get action execution time
	SYSTEMTIME sysExecTime = GetCurSysTime();

	// Get action name
	CString strAction;
	if (nActionType == APP_ACTIONTYPE_MONITOR) {
		strAction.SetString(_T("Turn off display"));
		dwErrCode = ERROR_SUCCESS;
	}
	else if (nActionType == APP_ACTIONTYPE_POWER && 
			(nMessage != APP_MESSAGE_SLEEP && nMessage != APP_MESSAGE_HIBERNATE)) {
		switch (nMessage)
		{
		case APP_MESSAGE_SHUTDOWN:
			// Shutdown
			strAction.SetString(_T("Shutdown"));
			dwErrCode = ERROR_SUCCESS;
			break;
		case APP_MESSAGE_REBOOT:
			// Restart
			strAction.SetString(_T("Restart"));
			dwErrCode = ERROR_SUCCESS;
			break;
		case APP_MESSAGE_SIGNOUT:
			// Sign out
			strAction.SetString(_T("Sign out"));
			dwErrCode = ERROR_SUCCESS;
			break;
		}
	}
	else if (nActionType == APP_ACTIONTYPE_POWER && 
			(nMessage == APP_MESSAGE_SLEEP || nMessage == APP_MESSAGE_HIBERNATE)) {
		switch (nMessage)
		{
		case APP_MESSAGE_SLEEP:
			// Sleep
			strAction.SetString(_T("Sleep"));
			dwErrCode = ERROR_SUCCESS;
			break;
		case APP_MESSAGE_HIBERNATE:
			// Hibernate
			strAction.SetString(_T("Hibernate"));
			dwErrCode = ERROR_SUCCESS;
			break;
		}
	}
	else {
		// Wrong argument
		strAction.SetString(_T("Invalid"));
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
	}

	// Time format
	CString strTimeFormat;
	CString strTimePeriod = (sysExecTime.wHour < 12) ? SYMBOL_ANTE_MERIDIEM : SYMBOL_POST_MERIDIEM;
	strTimeFormat.Format(IDS_FORMAT_FULLDATETIME, sysExecTime.wYear,
												  sysExecTime.wMonth,
												  sysExecTime.wDay,
												  sysExecTime.wHour,
												  sysExecTime.wMinute,
												  sysExecTime.wSecond,
												  sysExecTime.wMilliseconds,
												  strTimePeriod);

	// Message format
	CString strMsgFormat;
	strMsgFormat.Format(_T("[ExecutePowerAction]\nAction: %s\nTime: %s"), strAction, strTimeFormat);

	// Show dummy test message
	HWND hWnd = GET_HANDLE_MAINWND();
	MessageBox(hWnd, strMsgFormat, _T("DummyTest"), MB_OK | MB_ICONINFORMATION);
	return TRUE;
}

#ifdef _LEGACY_CODE_ACTIVE
//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDefaultData
//	Description:	Set default for config data
//  Arguments:		pcfgData - Pointer of config data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::SetDefaultData(PCONFIGDATA pcfgData)
{
	// Check data validity
	if (pcfgData == NULL)
		return;

	/*----------------- Set default data -----------------*/

	// Main settings
	pcfgData->nLMBAction = APP_ACTION_DISPLAYOFF;
	pcfgData->nMMBAction = APP_ACTION_SLEEP;
	pcfgData->nRMBAction = APP_ACTION_SHOWMENU;
	pcfgData->bRMBShowMenu = TRUE;

	// Display setting
	pcfgData->nLanguageID = APP_LANGUAGE_ENGLISH;

	// System settings
	pcfgData->bShowDlgAtStartup = TRUE;
	pcfgData->bStartupEnabled = TRUE;
	pcfgData->bConfirmAction = TRUE;
	pcfgData->bSaveHistoryLog = FALSE;
	pcfgData->bSaveAppEventLog = TRUE;
	pcfgData->bRunAsAdmin = FALSE;
	pcfgData->bShowErrorMsg = TRUE;
	pcfgData->bNotifySchedule = TRUE;
	pcfgData->bAllowCancelSchedule = FALSE;
	pcfgData->bEnableBackgroundHotkey = FALSE;
	pcfgData->bLockStateHotkey = TRUE;
	pcfgData->bEnablePowerReminder = TRUE;

	/*----------------------------------------------------*/
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDefaultData
//	Description:	Set default for schedule data
//  Arguments:		pcfgData - Pointer of schedule data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::SetDefaultData(PSCHEDULEDATA pschData)
{
	// Check data validity
	if (pschData == NULL)
		return;

	// Initialize data
	pschData->Init();

#ifdef _DEBUG_CODE_ACTIVE
	// Create default data
	const SCHEDULEITEM schDefItemList[] = {
	//-----Item ID----Enable state------Repeat----------Schedule action---------Time setting---------------Active days-----------------
		{	10000,		FALSE,			FALSE,		DEF_APP_ACTION_DISPLAYOFF,		{0},			DEF_SCHEDULE_DEFAULT_REPEAT		},
		{	10001,		FALSE,			FALSE,		DEF_APP_ACTION_SLEEP,			{0},			DEF_SCHEDULE_DEFAULT_REPEAT		},
		{	10002,		FALSE,			FALSE,		DEF_APP_ACTION_SHUTDOWN,		{0},			DEF_SCHEDULE_DEFAULT_REPEAT		},
		{	10003,		FALSE,			FALSE,		DEF_APP_ACTION_RESTART,			{0},			DEF_SCHEDULE_DEFAULT_REPEAT		},
		{	10004,		FALSE,			FALSE,		DEF_APP_ACTION_SIGNOUT,			{0},			DEF_SCHEDULE_DEFAULT_REPEAT		},
		{	10005,		FALSE,			FALSE,		DEF_APP_ACTION_HIBERNATE,		{0},			DEF_SCHEDULE_DEFAULT_REPEAT		},
	//---------------------------------------------------------------------------------------------------------------------------------
	};

	// Bind data
	pschData->nItemNum = (sizeof(schDefItemList) / sizeof(SCHEDULEITEM));
	pschData->arrSchedItemList.RemoveAll();
	for (int nIndex = 0; nIndex < pschData->nItemNum; nIndex++) {
		pschData->arrSchedItemList.Add(schDefItemList[nIndex]);
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDefaultData
//	Description:	Set default for HotkeySet data
//  Arguments:		pcfgData - Pointer of HotkeySet data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::SetDefaultData(PHOTKEYSETDATA phksData)
{
	// Check data validity
	if (phksData == NULL)
		return;

	// Initialize data
	phksData->Init();

	// Create default data
	phksData->arrHotkeySetList.Add(HOTKEYSETITEM(HKID_DISPLAYOFF));
	phksData->arrHotkeySetList.Add(HOTKEYSETITEM(HKID_SLEEP));
	phksData->arrHotkeySetList.Add(HOTKEYSETITEM(HKID_SHUTDOWN));
	phksData->arrHotkeySetList.Add(HOTKEYSETITEM(HKID_RESTART));
	phksData->arrHotkeySetList.Add(HOTKEYSETITEM(HKID_SIGNOUT));
	phksData->arrHotkeySetList.Add(HOTKEYSETITEM(HKID_HIBERNATE));

#ifdef _DEBUG_CODE_ACTIVE
	// Create default data
	const HOTKEYSETITEM hksDefItemList[] = {
	//----Enable state-----Hotkey action ID------Control key code----Function key code---
		{	FALSE,			HKID_DISPLAYOFF,			0,					0	},
		{	FALSE,			HKID_SLEEP,					0,					0	},
		{	FALSE,			HKID_SHUTDOWN,				0,					0	},
		{	FALSE,			HKID_RESTART,				0,					0	},
		{	FALSE,			HKID_SIGNOUT,				0,					0	},
		{	FALSE,			HKID_HIBERNATE,				0,					0	}
	//-----------------------------------------------------------------------------------
	};

	// Bind data
	phksData->nItemNum = (sizeof(hksDefItemList) / sizeof(HOTKEYSETITEM));
	phksData->arrHotkeySetList.RemoveAll();
	for (int nIndex = 0; nIndex < phksData->nItemNum; nIndex++) {
		phksData->arrHotkeySetList.Add(hksDefItemList[nIndex]);
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDefaultData
//	Description:	Set default for Power Reminder data
//  Arguments:		ppwrData - Pointer of Power Reminder data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::SetDefaultData(PPWRREMINDERDATA ppwrData)
{
	// Check data validity
	if (ppwrData == NULL)
		return;

	// Initialize data
	ppwrData->Init();

#ifdef _DEBUG_CODE_ACTIVE
	// Create default data
	const PWRREMINDERITEM pwrDefItemList[] = {
	//---Enable state---Item ID------------Message content----------------------Event ID-------------Event time-----Reminder style----Repeat daily---
		{	FALSE,		 10000,		_T("Sample #1: At set time"),			PREVT_AT_SETTIME,			{0},		PRSTYLE_MSGBOX,		FALSE	},
		{	FALSE,		 10001,		_T("Sample #2: At app startup"),		PREVT_AT_APPSTARTUP,		{0},		PRSTYLE_MSGBOX,		FALSE	},
		{	FALSE,		 10002,		_T("Sample #3: At system wake"),		PREVT_AT_SYSWAKEUP,			{0},		PRSTYLE_MSGBOX,		FALSE	},
		{	FALSE,		 10003,		_T("Sample #4: Before power action"),	PREVT_AT_BFRPWRACTION,		{0},		PRSTYLE_MSGBOX,		FALSE	},
		{	FALSE,		 10004,		_T("Sample #5: Wake after action"),		PREVT_AT_PWRACTIONWAKE,		{0},		PRSTYLE_MSGBOX,		FALSE	},
		{	FALSE,		 10005,		_T("Sample #6: Before app exit"),		PREVT_AT_APPEXIT,			{0},		PRSTYLE_MSGBOX,		FALSE	},
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	};

	// Bind data
	ppwrData->nItemNum = (sizeof(pwrDefItemList) / sizeof(PWRREMINDERITEM));
	ppwrData->arrRmdItemList.RemoveAll();
	for (int nIndex = 0; nIndex < ppwrData->nItemNum; nIndex++) {
		ppwrData->arrRmdItemList.Add(pwrDefItemList[nIndex]);
	}
#endif
}
#endif

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceError
//	Description:	Output exception/error trace log string to log file
//  Arguments:		lpszTraceLogA - Output trace log string (ANSI)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::TraceError(LPCSTR lpszTraceLogA)
{
	// Convert ANSI string to UNICODE
	LPCTSTR lpszTraceLogW = MAKEUNICODE(lpszTraceLogA);
	TraceError(lpszTraceLogW);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceError
//	Description:	Output exception/error trace log string to log file
//  Arguments:		lpszTraceLogW - Output trace log string (Unicode)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::TraceError(LPCTSTR lpszTraceLogW)
{
	// Write trace log file: TraceError.log
	WriteTraceErrorLogFile(lpszTraceLogW);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceErrorFormat
//	Description:	Format and output exception/error trace log string to log file
//  Arguments:		lpszTraceLogFormatA - Trace log format string (ANSI)
//					...				    - Same as default MFC Format function
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::TraceErrorFormat(LPCSTR lpszTraceLogFormatA, ...)
{
	ATLASSERT(AtlIsValidString(lpszTraceLogFormatA));

	// Format source string (ANSI)
	CStringA strLogFormatA;

	va_list argList;
	va_start(argList, lpszTraceLogFormatA);
	strLogFormatA.FormatV(lpszTraceLogFormatA, argList);
	va_end(argList);

	// Output trace log
	TraceError(strLogFormatA);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceErrorFormat
//	Description:	Format and output exception/error trace log string to log file
//  Arguments:		lpszTraceLogFormatW - Trace log format string (Unicode)
//					...				    - Same as default MFC Format function
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::TraceErrorFormat(LPCTSTR lpszTraceLogFormatW, ...)
{
	ATLASSERT(AtlIsValidString(lpszTraceLogFormatW));

	// Format source string (Unicode)
	CString strLogFormatW;

	va_list argList;
	va_start(argList, lpszTraceLogFormatW);
	strLogFormatW.FormatV(lpszTraceLogFormatW, argList);
	va_end(argList);

	// Output trace log
	TraceError(strLogFormatW);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceDebugInfo
//	Description:	Output debug trace information log
//  Arguments:		lpszFuncName - Code function name
//					lpszFileName - Code file name
//					nLineIndex	 - Code line number
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::TraceDebugInfo(LPCSTR lpszFuncName, LPCSTR lpszFileName, int nLineIndex)
{
	// Debug trace info
	CString strFuncName = MAKEUNICODE(lpszFuncName);
	CString strFileName = MAKEUNICODE(lpszFileName);

	// Format debug trace log
	CString strDebugTraceFormat;
	strDebugTraceFormat.Format(_T("Function: %s, File: %s(%d)"), strFuncName, strFileName, nLineIndex);

	// Write debug trace log: TraceDebug.log
	WriteTraceDebugLogFile(strDebugTraceFormat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDebugLog
//	Description:	Output debug log string
//  Arguments:		lpszDebugLog - Debug log string (Unicode)
//					nForceOutput - Force output target
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::OutputDebugLog(LPCTSTR lpszDebugLog, int nForceOutput /* = INT_INVALID */)
{
	// Get debug mode enable state
	BOOL bDebugModeEnable = GetDebugMode();

	// Get debug log string
	CString strDebugLog = lpszDebugLog;

	// Get DebugTest tool dialog handle
	HWND hDebugTestWnd = FindDebugTestDlg();

	// Debug log output target
	int nDebugOutputTarget = nForceOutput;
	if (nDebugOutputTarget == INT_INVALID) {
		nDebugOutputTarget = GetDebugOutputTarget();
	}
	if ((hDebugTestWnd != NULL) &&
		(IsWindowVisible(hDebugTestWnd))) {
		// Force enable debug mode and
		// prefer output target to DebugTest tool if it's displaying
		bDebugModeEnable = TRUE;
		nDebugOutputTarget = DebugTestTool;
	}

	// If debug mode not enabled, do nothing
	if (bDebugModeEnable == FALSE)
		return;

	// Output debug string
	if (nDebugOutputTarget == DefaultOutput) {
		// Default output target: OutputDebugString
		// Debug strings can be watched by using VS Output screen or DebugView tool
		OutputDebugString(strDebugLog);
	}
	else if (nDebugOutputTarget == DebugInfoFile) {
		// Ouput debug log to file: DebugInfo.log
		WriteDebugInfoLogFile(strDebugLog);
	}
	else if (nDebugOutputTarget == DebugTestTool) {
		// Output debug log to DebugTest tool
		if (hDebugTestWnd == NULL) return;
		WPARAM wParam = MAKE_WPARAM_STRING(strDebugLog);
		LPARAM lParam = MAKE_LPARAM_STRING(strDebugLog);
		SendMessage(hDebugTestWnd, SM_APP_DEBUG_OUTPUT, wParam, lParam);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDebugLogFormat
//	Description:	Output debug log string format
//  Arguments:		lpszDebugLogFormat - Debug log format string (Unicode)
//					...				   - Same as default MFC Format function
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::OutputDebugLogFormat(LPCTSTR lpszDebugLogFormat, ...)
{
	ATLASSERT(AtlIsValidString(lpszDebugLogFormat));

	// Format source string
	CString strLogFormat;

	va_list argList;
	va_start(argList, lpszDebugLogFormat);
	strLogFormat.FormatV(lpszDebugLogFormat, argList);
	va_end(argList);

	// Output debug string
	OutputDebugLog(strLogFormat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDebugStringFormat
//	Description:	Output debug string format (combined version of String.Format
//					and the default OutputDebugString function)
//  Arguments:		lpszDebugStringFormat - Debug log format string (Unicode)
//					...					  - Same as default MFC Format function
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::OutputDebugStringFormat(LPCTSTR lpszDebugStringFormat, ...)
{
	ATLASSERT(AtlIsValidString(lpszDebugStringFormat));

	// Format source string
	CString strStringFormat;

	va_list argList;
	va_start(argList, lpszDebugStringFormat);
	strStringFormat.FormatV(lpszDebugStringFormat, argList);
	va_end(argList);

	// Output debug string
	OutputDebugString(strStringFormat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitTraceErrorLogFile
//	Description:	Initialize trace error log file
//  Arguments:		None
//  Return value:	TRUE/FALSE
//	Notes:			TraceError.log
//					File to output trace error detail log strings
//
//////////////////////////////////////////////////////////////////////////

static BOOL InitTraceErrorLogFile(void)
{
	// Get access to Global data
	using namespace Global;

	// Verify global trace error log file pointer initialization
	VERIFY_INITIALIZATION(g_pFileLogTraceError, CFile);

	// Get trace error log file pointer
	CFile* pTraceErrorLogFile = GetTraceErrorLogFile();
	NULL_POINTER_BREAK(pTraceErrorLogFile, return FALSE);

	// Log folder path
	CString strFolderPath = AppCore::GetSubFolderPath(SUBFOLDER_LOG);

	// Log file path
	CString strFilePath;
	AppCore::MakeFilePath(strFilePath, strFolderPath, FILENAME_TRACE_ERROR_LOG, FILEEXT_LOGFILE);

	// If the log file is not being opened
	if (pTraceErrorLogFile->m_hFile == CFile::hFileNull) {
		OPENFILE: {
			// Open the log file
			if (!pTraceErrorLogFile->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
				// Show error message
				DWORD dwErrorCode = GetLastError();
				AppCore::ShowErrorMessage(NULL, NULL, dwErrorCode);
				return FALSE;
			}
		}

		// Go to end of file
		ULONGLONG ullFileSize = pTraceErrorLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= MAX_LOGFILE_SIZE) {

			// Step1: Close file
			pTraceErrorLogFile->Close();

			// Step2: Rename file extension to BAK
			if (!AppCore::BackupOldLogFile(strFilePath, FILENAME_TRACE_ERROR_LOG))
				return FALSE;

			// Step3: Create new file and reopen
			goto OPENFILE;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ReleaseTraceErrorLogFile
//	Description:	Release trace error log file
//  Arguments:		None
//  Return value:	None
//	Notes:			TraceError.log
//					File to output trace error detail log strings
//
//////////////////////////////////////////////////////////////////////////

inline void ReleaseTraceErrorLogFile(void)
{
	// Get access to Global data
	using namespace Global;

	// Clean up trace error log file pointer
	if (g_pFileLogTraceError != NULL) {
		// Close file if is opening
		if (g_pFileLogTraceError->m_hFile != CFile::hFileNull) {
			g_pFileLogTraceError->Flush();
			g_pFileLogTraceError->Close();
		}
		delete g_pFileLogTraceError;
		g_pFileLogTraceError = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitTraceDebugLogFile
//	Description:	Initialize trace debug log file
//  Arguments:		None
//  Return value:	TRUE/FALSE
//	Notes:			TraceDebug.log
//					File to output trace debug log strings (including 
//					the function name, code file and line where it failed)
//
//////////////////////////////////////////////////////////////////////////

static BOOL InitTraceDebugLogFile(void)
{
	// Get access to Global data
	using namespace Global;

	// Verify global trace debug log file pointer initialization
	VERIFY_INITIALIZATION(g_pFileLogTraceDebug, CFile);

	// Get trace debug log file pointer
	CFile* pTraceDebugLogFile = GetTraceDebugLogFile();
	NULL_POINTER_BREAK(pTraceDebugLogFile, return FALSE);

	// Log folder path
	CString strFolderPath = AppCore::GetSubFolderPath(SUBFOLDER_LOG);

	// Log file path
	CString strFilePath;
	AppCore::MakeFilePath(strFilePath, strFolderPath, FILENAME_TRACE_DEBUG_LOG, FILEEXT_LOGFILE);

	// If the log file is not being opened
	if (pTraceDebugLogFile->m_hFile == CFile::hFileNull) {
		OPENFILE: {
			// Open the log file
			if (!pTraceDebugLogFile->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
				// Show error message
				DWORD dwErrorCode = GetLastError();
				AppCore::ShowErrorMessage(NULL, NULL, dwErrorCode);
				return FALSE;
			}
		}

		// Go to end of file
		ULONGLONG ullFileSize = pTraceDebugLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= MAX_LOGFILE_SIZE) {

			// Step1: Close file
			pTraceDebugLogFile->Close();

			// Step2: Rename file extension to BAK
			if (!AppCore::BackupOldLogFile(strFilePath, FILENAME_TRACE_DEBUG_LOG))
				return FALSE;

			// Step3: Create new file and reopen
			goto OPENFILE;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ReleaseTraceDebugLogFile
//	Description:	Release trace debug log file
//  Arguments:		None
//  Return value:	None
//	Notes:			TraceDebug.log
//					File to output trace debug log strings (including 
//					the function name, code file and line where it failed)
//
//////////////////////////////////////////////////////////////////////////

inline void ReleaseTraceDebugLogFile(void)
{
	// Get access to Global data
	using namespace Global;

	// Clean up trace debug info log file pointer
	if (g_pFileLogTraceDebug != NULL) {
		// Close file if is opening
		if (g_pFileLogTraceDebug->m_hFile != CFile::hFileNull) {
			g_pFileLogTraceDebug->Flush();
			g_pFileLogTraceDebug->Close();
		}
		delete g_pFileLogTraceDebug;
		g_pFileLogTraceDebug = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitDebugInfoLogFile
//	Description:	Initialize debug info log file
//  Arguments:		None
//  Return value:	TRUE/FALSE
//	Notes:			DebugInfo.log
//					File to output debug info log strings (similar to
//					OutputDebugString, but output to file instead)
//
//////////////////////////////////////////////////////////////////////////

static BOOL InitDebugInfoLogFile(void)
{
	// Get access to Global data
	using namespace Global;

	// Verify global debug info log file pointer initialization
	VERIFY_INITIALIZATION(g_pFileLogDebugInfo, CFile);

	// Get debug info log file pointer
	CFile* pDebugInfoLogFile = GetDebugInfoLogFile();
	NULL_POINTER_BREAK(pDebugInfoLogFile, return FALSE);

	// Log folder path
	CString strFolderPath = AppCore::GetSubFolderPath(SUBFOLDER_LOG);

	// Log file path
	CString strFilePath;
	AppCore::MakeFilePath(strFilePath, strFolderPath, FILENAME_DEBUG_INFO_LOG, FILEEXT_LOGFILE);

	// If the log file is not being opened
	if (pDebugInfoLogFile->m_hFile == CFile::hFileNull) {
		OPENFILE: {
			// Open the log file
			if (!pDebugInfoLogFile->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
				// Show error message
				DWORD dwErrorCode = GetLastError();
				AppCore::ShowErrorMessage(NULL, NULL, dwErrorCode);
				return FALSE;
			}
		}

		// Go to end of file
		ULONGLONG ullFileSize = pDebugInfoLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= MAX_LOGFILE_SIZE) {

			// Step1: Close file
			pDebugInfoLogFile->Close();

			// Step2: Rename file extension to BAK
			if (!AppCore::BackupOldLogFile(strFilePath, FILENAME_DEBUG_INFO_LOG))
				return FALSE;

			// Step3: Create new file and reopen
			goto OPENFILE;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ReleaseDebugInfoLogFile
//	Description:	Release debug info log file
//  Arguments:		None
//  Return value:	None
//	Notes:			DebugInfo.log
//					File to output debug info log strings (similar to
//					OutputDebugString, but output to file instead)
//
//////////////////////////////////////////////////////////////////////////

inline void ReleaseDebugInfoLogFile(void)
{
	// Get access to Global data
	using namespace Global;

	// Clean up debug info log file pointer
	if (g_pFileLogDebugInfo != NULL) {
		// Close file if is opening
		if (g_pFileLogDebugInfo->m_hFile != CFile::hFileNull) {
			g_pFileLogDebugInfo->Flush();
			g_pFileLogDebugInfo->Close();
		}
		delete g_pFileLogDebugInfo;
		g_pFileLogDebugInfo = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	BackupOldLogFile
//	Description:	Backup old log file
//  Arguments:		strFilePath		- File path (in/out)
//					lpszLogFileName	- Log file name
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::BackupOldLogFile(CString& strFilePath, LPCTSTR lpszLogFileName)
{
	CFileFind Finder;
	CString strBakFilePath;
	CString strFilePathTemp;

	// If file path is not specified, do nothing
	if (strFilePath.IsEmpty()) return FALSE;

	// Log folder path
	CString strFolderPath = GetSubFolderPath(SUBFOLDER_LOG);

	// Search for backup file list
	for (int nNum = 0; nNum < MAX_BAKFILE_COUNT; nNum++) {
		
		// Make backup file path template
		if (!MakeFilePath(strFilePathTemp, strFolderPath, lpszLogFileName, FILEEXT_BAKLOGFILE))
			return FALSE;

		// If backup file path template is empty, do nothing
		if (strFilePathTemp.IsEmpty()) return FALSE;

		// Format backup file path
		strBakFilePath.Format(strFilePathTemp, nNum);

		// Check if file has already existed
		if (Finder.FindFile(strBakFilePath) == TRUE) {

			// If backup file number exceeded the limit, can not backup more
			if (nNum == (MAX_BAKFILE_COUNT - 1)) return FALSE;
			else continue;
		}

		// Rename file
		CFile::Rename(strFilePath, strBakFilePath);
		break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteTraceErrorLogFile
//	Description:	Write trace error log string to file
//  Arguments:		lpszLogStringW	- Log string
//  Return value:	None
//	Notes:			TraceError.log
//					File to output trace error detail log strings
//
//////////////////////////////////////////////////////////////////////////

void AppCore::WriteTraceErrorLogFile(LPCTSTR lpszLogStringW)
{
	// Get current time up to milisecs
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);

	// Format log date/time
	CString strTimeFormat;
	CString strMiddayFlag = (stTime.wHour >= 12) ? _T("PM") : _T("AM");
	strTimeFormat.Format(IDS_FORMAT_FULLDATETIME, stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, strMiddayFlag);

	// Format output log string
	CString strLogFormat;
	strLogFormat.Format(IDS_FORMAT_LOGSTRING, strTimeFormat, lpszLogStringW, STRING_EMPTY);

	// If output log string is empty, do nothing
	if (strLogFormat.IsEmpty())
		return;

	// If the file is not initialized or had been released
	if (GetTraceErrorLogFile() == NULL) {
		if (!InitTraceErrorLogFile())
			return;
	}

	// Re-acquire trace log file pointer
	CFile* pTraceErrorLogFile = GetTraceErrorLogFile();
	NULL_POINTER_BREAK(pTraceErrorLogFile, return NOTHING);
	{
		// Write log string to file
		pTraceErrorLogFile->Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		pTraceErrorLogFile->Flush();
	}

	// Re-check file size after writing
	{
		// Go to end of file
		ULONGLONG ullFileSize = pTraceErrorLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= MAX_LOGFILE_SIZE) {

			// Step1: Close file
			pTraceErrorLogFile->Close();

			// Step2: Rename file extension to BAK
			CString strOrgFilePath;
			CString strFolderPath = GetSubFolderPath(SUBFOLDER_LOG);
			MakeFilePath(strOrgFilePath, strFolderPath, FILENAME_TRACE_ERROR_LOG, FILEEXT_LOGFILE);
			if (!BackupOldLogFile(strOrgFilePath, FILENAME_TRACE_ERROR_LOG))
				return;

			// Step3: Release log file pointer --> Quit
			// New file will be re-initialized in the next function call
			ReleaseTraceErrorLogFile();
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteTraceDebugLogFile
//	Description:	Write trace debug log string to file
//  Arguments:		lpszLogStringW	- Log string
//  Return value:	None
//	Notes:			TraceDebug.log
//					File to output trace debug log strings (including 
//					the function name, code file and line where it failed)
//
//////////////////////////////////////////////////////////////////////////

void AppCore::WriteTraceDebugLogFile(LPCTSTR lpszLogStringW)
{
	// Get current time up to milisecs
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);

	// Format log date/time
	CString strTimeFormat;
	CString strMiddayFlag = (stTime.wHour >= 12) ? _T("PM") : _T("AM");
	strTimeFormat.Format(IDS_FORMAT_FULLDATETIME, stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, strMiddayFlag);

	// Format output log string
	CString strLogFormat;
	strLogFormat.Format(IDS_FORMAT_LOGSTRING, strTimeFormat, lpszLogStringW, STRING_EMPTY);

	// If output log string is empty, do nothing
	if (strLogFormat.IsEmpty()) return;

	// If the file is not initialized or had been released
	if (GetTraceDebugLogFile() == NULL) {
		if (!InitTraceDebugLogFile())
			return;
	}

	// Re-acquire trace debug log file pointer
	CFile* pTraceDebugLogFile = GetTraceDebugLogFile();
	NULL_POINTER_BREAK(pTraceDebugLogFile, return NOTHING);
	{
		// Write log string to file
		pTraceDebugLogFile->Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		pTraceDebugLogFile->Flush();
	}

	// Re-check file size after writing
	{
		// Go to end of file
		ULONGLONG ullFileSize = pTraceDebugLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= MAX_LOGFILE_SIZE) {

			// Step1: Close file
			pTraceDebugLogFile->Close();

			// Step2: Rename file extension to BAK
			CString strOrgFilePath;
			CString strFolderPath = GetSubFolderPath(SUBFOLDER_LOG);
			MakeFilePath(strOrgFilePath, strFolderPath, FILENAME_TRACE_DEBUG_LOG, FILEEXT_LOGFILE);
			if (!BackupOldLogFile(strOrgFilePath, FILENAME_TRACE_DEBUG_LOG))
				return;

			// Step3: Release log file pointer --> Quit
			// New file will be re-initialized in the next function call
			ReleaseTraceDebugLogFile();
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteDebugInfoLogFile
//	Description:	Write debug info output log string to file
//  Arguments:		lpszLogStringW	- Log string
//  Return value:	None
//	Notes:			DebugInfo.log
//					File to output debug info log strings (similar to
//					OutputDebugString, but output to file instead)
//
//////////////////////////////////////////////////////////////////////////

void AppCore::WriteDebugInfoLogFile(LPCTSTR lpszLogStringW)
{
	// Get current time up to milisecs
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);

	// Format log date/time
	CString strTimeFormat;
	CString strMiddayFlag = (stTime.wHour >= 12) ? _T("PM") : _T("AM");
	strTimeFormat.Format(IDS_FORMAT_FULLDATETIME, stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, strMiddayFlag);

	// Format output log string
	CString strLogFormat;
	strLogFormat.Format(IDS_FORMAT_LOGSTRING, strTimeFormat, lpszLogStringW, STRING_EMPTY);

	// If output log string is empty, do nothing
	if (strLogFormat.IsEmpty()) return;

	// If the file is not initialized or had been released
	if (GetDebugInfoLogFile() == NULL) {
		if (!InitDebugInfoLogFile())
			return;
	}

	// Re-acquire debug info log file pointer
	CFile* pDebugInfoLogFile = GetDebugInfoLogFile();
	NULL_POINTER_BREAK(pDebugInfoLogFile, return NOTHING);
	{
		// Write log string to file
		pDebugInfoLogFile->Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		pDebugInfoLogFile->Flush();
	}

	// Recheck file size after writing
	{
		// Go to end of file
		ULONGLONG ullFileSize = pDebugInfoLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= MAX_LOGFILE_SIZE) {

			// Step1: Close file
			pDebugInfoLogFile->Close();

			// Step2: Rename file extension to BAK
			CString strOrgFilePath;
			CString strFolderPath = GetSubFolderPath(SUBFOLDER_LOG);
			MakeFilePath(strOrgFilePath, strFolderPath, FILENAME_DEBUG_INFO_LOG, FILEEXT_LOGFILE);
			if (!BackupOldLogFile(strOrgFilePath, FILENAME_DEBUG_INFO_LOG))
				return;

			// Step3: Release log file pointer --> Quit
			// New file will be re-initialized in the next function call
			ReleaseDebugInfoLogFile();
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteTraceNDebugLogFileBase
//	Description:	Write trace and debug log string to file
//  Arguments:		lpszFileName	- Log file name
//					lpszLogStringW	- Log string
//  Return value:	None
//	Notes:			Base function - No longer used
//
//////////////////////////////////////////////////////////////////////////

void AppCore::WriteTraceNDebugLogFileBase(LPCTSTR lpszFileName, LPCTSTR lpszLogStringW)
{
	// Log file path
	CString strFilePath;
	MakeFilePath(strFilePath, SUBFOLDER_LOG, lpszFileName, FILEEXT_LOGFILE);

	CFile fTrcDbgLogFile;

	// Check if file is opening, if not, open it
	if (fTrcDbgLogFile.m_hFile == CFile::hFileNull) {
		BOOL bResult = TRUE;
	OPENFILE: {
			bResult = fTrcDbgLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
			if (bResult == FALSE) {
				// Show error message
				DWORD dwErrorCode = GetLastError();
				LPARAM lParam = (LPARAM)lpszFileName;
				ShowErrorMessage(NULL, NULL, dwErrorCode, lParam);
				return;
			}
		}

		// Go to end of file
		ULONGLONG ullFileSize = fTrcDbgLogFile.SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= MAX_LOGFILE_SIZE) {

			// Step1: Close file
			fTrcDbgLogFile.Close();
			
			// Step2: Rename file extension to BAK
			CFileFind Finder;
			CString strBakFilePath;
			for (int nNum = 0; nNum < MAX_BAKFILE_COUNT; nNum++) {
				strBakFilePath.Format((strFilePath + FILEEXT_BAKLOGFILE), nNum);
				if (Finder.FindFile(strBakFilePath) == TRUE) {
					if (nNum == (MAX_BAKFILE_COUNT - 1)) return;
					else continue;
				}
				CFile::Rename(strFilePath, strBakFilePath);
				break;
			}

			// Step3: Create new file and reopen
			goto OPENFILE;
		}
	}

	// Get current time up to milisecs
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);

	// Format log date/time
	CString strTimeFormat;
	CString strMiddayFlag = (stTime.wHour >= 12) ? _T("PM") : _T("AM");
	strTimeFormat.Format(IDS_FORMAT_FULLDATETIME, stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, strMiddayFlag);

	// Format output log string
	CString strLogFormat;
	strLogFormat.Format(IDS_FORMAT_LOGSTRING, strTimeFormat, lpszLogStringW, STRING_EMPTY);

	if (!strLogFormat.IsEmpty()) {
		// Write log string to file
		fTrcDbgLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		fTrcDbgLogFile.Flush();
	}

	// Close file after done writing
	if (fTrcDbgLogFile.m_hFile != CFile::hFileNull) {
		fTrcDbgLogFile.Close();
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WaitMessage
//	Description:	Create a loop and wait for specified message
//  Arguments:		nMsg	 - Message to wait for
//					nTimeout - Timeout (tick-count)
//  Return value:	LRESULT
//	Notes:			Be careful when using this function, it may cause the
//					program to be not responding
//
//////////////////////////////////////////////////////////////////////////

LRESULT	AppCore::WaitMessage(UINT nMsg, int nTimeout /* = DEF_WAITMESSAGE_TIMEOUT */)
{
	LRESULT lResult = RESULT_SUCCESS;

	// Get begin timestamp (for timeout counter)
	ULONGLONG ullBeginTimestamp = GetTickCount64();

	// Wait for message
	while (1) {
		MSG msg = STRUCT_ZERO;
		if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {

			// If received specified message, break the loop
			if (msg.message == nMsg) {

				// Success
				lResult = RESULT_SUCCESS;
				break;
			}
		}
		// Check for timeout
		if (GetTickCount64() - ullBeginTimestamp >= nTimeout) {

			// Timeout --> Failed
			lResult = RESULT_FAILED;
			break;
		}
	}

	return lResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ShowErrorMessage
//	Description:	Show error message by error code
//  Arguments:		hMsgOwnerWnd - Handle of Message Box's owner window
//					nLanguageID	 - Language option ID
//					dwErrorCode	 - Error code
//					lParam		 - Additional attached param (description string)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::ShowErrorMessage(HWND hMsgOwnerWnd, UINT nLanguageID, DWORD dwErrorCode, LPARAM lParam /* = NULL */)
{
	// Use table and language functions
	using namespace MapTable;
	using namespace Language;

	// Get application-defined error code from system-defined error code
	DWORD dwAppErrCode = GetPairedID(IDTable::ErrorCode, dwErrorCode, TRUE);
	if (dwAppErrCode != INT_INVALID) {
		// Replace with application-defined error code
		dwErrorCode = dwAppErrCode;
	}

	// Get error message string ID
	int nErrMsgID = GetPairedID(IDTable::ErrorMessage, dwErrorCode);

	// Invalid error message ID
	if (nErrMsgID == INT_INVALID) {
		// Show unknown error message
		nErrMsgID = MSGBOX_ERROR_UNKNOWN;
	}

	// If error message ID is NULL, do nothing
	if (nErrMsgID == INT_NULL)
		return;

	// Load language package
	LANGTABLE_PTR pAppLang = LoadLanguageTable(nLanguageID);
	if (pAppLang == NULL) 
		return;

	// Get language strings
	CString strErrMessage = GetLanguageString(pAppLang, nErrMsgID);
	CString strErrCaption = GetLanguageString(pAppLang, MSGBOX_ERROR_CAPTION);

	// In case of unknown error, attach the error code
	if (nErrMsgID == MSGBOX_ERROR_UNKNOWN) {
		CString strTemp;
		strTemp.Format(strErrMessage, dwErrorCode);
		strErrMessage = strTemp;
	}

	// Get attached param
	CString strDescription = STRING_NULL;
	if (lParam != NULL) {
		// Convert to description string
		strDescription = LPARAM_TO_STRING(lParam);
	}

	// Attach additional description if available
	if (IS_NOT_NULL_STRING(strDescription)) {
		strErrMessage.Append(STRING_NEWLINE);
		strErrMessage.Append(strDescription);
	}

	// Show error message
	MessageBox(hMsgOwnerWnd, strErrMessage, strErrCaption, MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);

	// Notify application class about error message showing
	WPARAM wAppMsgParam = (WPARAM)dwErrorCode;
	LPARAM lAppMsgParam = MAKE_LPARAM_STRING(strErrMessage);
	PostMessage(NULL, SM_APP_SHOW_ERROR_MSG, wAppMsgParam, lAppMsgParam);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Sel2Opt
//	Description:	Convert combo-box selection into option ID
//  Arguments:		nOptionMacro - Option macro
//					nSelection	 - Selection index
//  Return value:	UINT - Option ID
//
//////////////////////////////////////////////////////////////////////////

UINT AppCore::Sel2Opt(UINT nOptionMacro, UINT nSelection)
{
	VERIFY(nOptionMacro > 0x00 && nOptionMacro < UINT_MAX);
	VERIFY(nSelection >= 0 && nSelection < UINT_MAX);
	return ((nOptionMacro << 8) + (nSelection + 1));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Opt2Sel
//	Description:	Convert option ID into combo-box selection
//  Arguments:		nOptionMacro - Option macro
//					nCurOption	 - Option ID
//  Return value:	UINT - Selection index
//
//////////////////////////////////////////////////////////////////////////

UINT AppCore::Opt2Sel(UINT nOptionMacro, UINT nCurOption)
{
	VERIFY(nOptionMacro > 0x00 && nOptionMacro < UINT_MAX);
	VERIFY(nCurOption >= 0 && nCurOption < UINT_MAX);
	return (nCurOption - (nOptionMacro << 8) - 1);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Text2Time
//	Description:	Convert editbox input text into valid time value
//  Arguments:		stTime  - Return time data (ref-value)
//					strText - Input text
//  Return value:	BOOL - Result of converting process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::Text2Time(SYSTEMTIME& stTime, CString strText)
{
	// Check input text validity
	int nLength = strText.GetLength();
	if ((nLength == 0) || (nLength > 4))
		return FALSE;

	CString strTime = strText;

	int nHour = INT_INVALID;
	int nMinute = INT_INVALID;

	// Break the time value into combinations of digits
	int nLeft1Digit = _tstoi(strTime.Left(1));
	int nLeft2Digits = _tstoi(strTime.Left(2));
	int	nRight1Digit = _tstoi(strTime.Right(1));
	int nRight2Digits = _tstoi(strTime.Right(2));

	// Convert
	switch (nLength)
	{
	case 1:
		// Ex: 3 -> 03:00, 9 -> 09:00, ...
		nHour = _tstoi(strTime);			// The given time value will be the hour value
		nMinute = 0;						// The minute value will be zero (0)
		break;
	case 2:
		if ((nLeft1Digit == 0) ||													// Ex: 08 -> 00:08
			((nLeft1Digit > 2) || ((nLeft1Digit == 2) && (nRight1Digit > 3)))) {	// Ex: 35 -> 03:05, 24 -> 02:04, ...
			nHour = nLeft1Digit;													// The first half will be the hour value
			nMinute = nRight1Digit;													// The remaining will be the minute value
		}
		else {
			// Ex: 13 -> 13:00, 18 -> 18:00, ...
			nHour = _tstoi(strTime);				// All digits will be the hour value
			nMinute = 0;							// The minute value will be zero (0)
		} break;
	case 3:
		if ((nLeft1Digit == 0) ||								// Ex: 034 -> 00:34, ...
			((nLeft1Digit > 2) || (nLeft2Digits >= 24)) ||		// Ex: 320 -> 03:20, 250 -> 02:50, ...
			((nRight2Digits > 0) && (nRight2Digits < 60))) {	// Ex: 225 -> 02:25, 132 -> 01:32, ...
			nHour = nLeft1Digit;								// The first digit will be the hour value
			nMinute = nRight2Digits;							// The remaining will be the minute value
		}
		else {
			// Ex: 180 -> 18:00, 1530 -> 15:30, ...
			nHour = nLeft2Digits;					// The first 2 digits will be the hour value
			nMinute = nRight1Digit;					// The remaining will be the minute value
		} break;
	case 4:
		// Ex: 1235 -> 12:35, 1840 -> 18:40, ...
		nHour = nLeft2Digits;						// The first half will be the hour value
		nMinute = nRight2Digits;					// The remaining will be the minute value
		break;
	}

	// If the minute value is larger than 60
	if (nMinute >= 60) {
		nHour++;			// The hour value increases by 1
		nMinute -= 60;		// The minute value decreases by 60
	}

	// If the hour value exceeds 24, return invalid
	if (nHour >= 24)
		return FALSE;

	// Only return if both the hour and minute values are valid
	if ((nHour > INT_INVALID) && (nMinute > INT_INVALID)) {
		stTime.wHour = nHour;
		stTime.wMinute = nMinute;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Text2TimeBase
//	Description:	Convert editbox input text into valid time value 
//  Arguments:		stTime  - Return time data (ref-value)
//					strText - Input text
//  Return value:	BOOL - Result of converting process
//	Note:			Old/base function (no longer used)
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::Text2TimeBase(SYSTEMTIME& stTime, CString strText)
{
	// Check input text validity
	int nLength = strText.GetLength();
	if ((nLength == 0) || (nLength > 4))
		return FALSE;

	// Get input text length
	UINT nTime = _tstoi(strText);
	UINT nTimeTemp = nTime;
	nLength = 0;
	do {
		nLength++;
		nTimeTemp /= 10;
	} while (nTimeTemp != 0);

	// Convert
	int nHour = -1, nMinute = -1;
	switch (nLength)
	{
	case 1: 	// Ex: 3, 9, ...
		nHour = nTime;
		nMinute = 0;
		break;
	case 2:
		if ((nTime / 10 > 2) || 
			((nTime / 10 == 2) && (nTime % 10 > 3))) {	// Ex: 35, 24, ...
			nHour = nTime / 10;
			nMinute = nTime % 10;
		}
		else {	// Ex: 13, 18, ...
			nHour = nTime;
			nMinute = 0;
		} break;
	case 3:
		if ((nTime / 100 > 2) || (nTime / 10 >= 24)) {	// Ex: 320, 240
			nHour = nTime / 100;
			nMinute = nTime % 100;
		}
		else {	// Ex: 320, 245
			nHour = nTime / 10;
			nMinute = nTime % 10;
		} break;
	case 4:
		nHour = nTime / 100;
		nMinute = nTime % 100;
		break;
	}

	// Validate
	if ((nHour >= 24) || (nMinute >= 60))
		return FALSE;

	if ((nHour > -1) && (nMinute > -1)) {
		stTime.wHour = nHour;
		stTime.wMinute = nMinute;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SpinPos2Time
//	Description:	Convert timespin position to time value
//  Arguments:		stTime  - Return time data (ref-value)
//					nPos	- Input spin position
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::SpinPos2Time(SYSTEMTIME& stTime, int nPos)
{
	// Invalid input position
	if (nPos < TIMESPIN_MIN)
		nPos = TIMESPIN_MIN;
	else if (nPos > TIMESPIN_MAX)
		nPos = TIMESPIN_MAX;

	// Convert
	int nHour = nPos / 60;
	int nMinute = nPos - (nHour * 60);

	// Validate
	if ((nHour != INT_INVALID) && (nMinute != INT_INVALID)) {
		stTime.wHour = (WORD)nHour;
		stTime.wMinute = (WORD)nMinute;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Time2SpinPos
//	Description:	Convert time value to timespin position
//  Arguments:		stTime  - Return time data
//					nPos	- Input spin position (ref-value)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::Time2SpinPos(SYSTEMTIME stTime, int& nPos)
{
	// Convert
	nPos = (stTime.wHour * 60) + stTime.wMinute;

	// Invalid result
	if (nPos < TIMESPIN_MIN)
		nPos = TIMESPIN_MIN;
	else if (nPos > TIMESPIN_MAX)
		nPos = TIMESPIN_MAX;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetListCurSel
//	Description:	Return list control current selected item index
//  Arguments:		pListCtrl - Pointer of list control
//  Return value:	int - Selection
//
//////////////////////////////////////////////////////////////////////////

int AppCore::GetListCurSel(CListCtrl& pListCtrl)
{
	int nResult = INT_INVALID;

	// Get selected item
	int nItemCount = pListCtrl.GetItemCount();
	for (int nIndex = 0; nIndex < nItemCount; nIndex++) {
		if ((pListCtrl.GetItemState(nIndex, LVIS_SELECTED) & LVIS_SELECTED) == LVIS_SELECTED) {
			nResult = nIndex;
			break;
		}
	}

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FindDebugTestDlg
//	Description:	Find and return DebugTest dialog handle
//  Arguments:		None
//  Return value:	HWND
//
//////////////////////////////////////////////////////////////////////////

HWND AppCore::FindDebugTestDlg()
{
	CString strDebugDlgTitle;
	strDebugDlgTitle.LoadString(IDS_APP_DEBUGTESTDLG_TITLE);
	return ::FindWindow(NULL, strDebugDlgTitle);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetFixedCellStyle
//	Description:	Set fixed cell style (header row, base column)
//  Arguments:		pGridCtrl	- Grid control table pointer
//					nRow & nCol - Cell position (row & column)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::SetFixedCellStyle(CGridCtrl* pGridCtrl, int nRow, int nCol)
{
	/*
	CString strFontname;
	strFontname.LoadString(IDS_DF_COORDDLG_FONT_NAME);

	// Fonts
	CFont FontHeader;
	float fFontPointHeader;
	fFontPointHeader = CnvFontSize(IDS_CTRL_FONT_POINT);
	FontHeader.CreatePointFont((int)(fFontPointHeader * 10), strFontname, NULL);
	LOGFONT lfHeader;
	FontHeader.GetLogFont(&lfHeader);
	*/

	// Check control validity
	if (pGridCtrl == NULL) return;

	// Set base style
	CGridCellBase* pHeaderCell = (CGridCellBase*)pGridCtrl->GetCell(nRow, nCol);
	if (pHeaderCell == NULL) return;
	pHeaderCell->SetFormat(pHeaderCell->GetFormat() | DT_CENTER);
	pHeaderCell->SetMargin(0);
	//lfHeader.lfWeight = FW_BOLD;
	//pHeaderCell->SetFont(&lfHeader);
	pHeaderCell->SetBackClr(COLOR_GRAY);
	pHeaderCell->SetTextClr(COLOR_BLACK);

	//FontHeader.DeleteObject();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DrawGridTableRow
//	Description:	Setup style and draw grid control table row by row index
//  Arguments:		pGridCtrl		- Grid control table pointer
//					nRow			- Row position/index
//					nRowNum			- Number of table rows
//					nColNum			- Number of table columns
//					arrGrdColFormat - Grid control column format data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::DrawGridTableRow(CGridCtrl* pGridCtrl, int nRow, int nRowNum, int nColNum, GRIDCTRLCOLFORMAT* apGrdColFormat)
{
	// Check control validity
	if (pGridCtrl == NULL) return;

	// Check format data validity
	if (apGrdColFormat == NULL) return;

	// Check row index validity
	if ((nRow < 0) || (nRow > pGridCtrl->GetRowCount()))
		return;

	// Setup row
	int nColStyle = -1;
	UINT nItemState = INT_NULL;
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Get column style & item state
		nColStyle = apGrdColFormat[nCol].nColStyle;
		nItemState = pGridCtrl->GetItemState(nRow, nCol);
		nItemState |= GVIS_READONLY;

		// Base column - header-like style
		if (nColStyle == COLSTYLE_FIXED) {
			// Set fixed cell style
			SetFixedCellStyle(pGridCtrl, nRow, nCol);
		}

		// Checkbox column
		else if (nColStyle == COLSTYLE_CHECKBOX) {
			// Set cell type: Checkbox
			if (!pGridCtrl->SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck)))
				continue;

			// Set center alignment if defined
			if (apGrdColFormat[nCol].bCenter == TRUE) {
				CGridCellCheck* pCell = (CGridCellCheck*)pGridCtrl->GetCell(nRow, nCol);
				if (pCell == NULL) continue;
				pCell->SetCheckPlacement(SCP_CENTERING);
			}
		}

		// Normal column
		else if (nColStyle == COLSTYLE_NORMAL) {
			// Set item state
			if (!pGridCtrl->SetItemState(nRow, nCol, nItemState))
				continue;

			// Set center alignment if defined
			if (apGrdColFormat[nCol].bCenter == TRUE) {
				CGridCellBase* pCell = (CGridCellBase*)pGridCtrl->GetCell(nRow, nCol);
				if (pCell == NULL) continue;
				pCell->SetFormat(pCell->GetFormat() | DT_CENTER);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetCurSysTime
//	Description:	Get current time with milliseconds and return SYSTEMTIME
//  Arguments:		stTime - Current system time (out)
//  Return value:	SYSTEMTIME - Return time data
//
//////////////////////////////////////////////////////////////////////////

SYSTEMTIME AppCore::GetCurSysTime(void)
{
	// Get system time
	SYSTEMTIME tsSysTimeTemp;
	GetSystemTime(&tsSysTimeTemp);
	
	// Backup the millisecond value
	WORD wMillisecs = tsSysTimeTemp.wMilliseconds;
	
	// Get current time
	CTime timeTemp = CTime::GetCurrentTime();
	timeTemp.GetAsSystemTime(tsSysTimeTemp);

	// Restore the millisecond value
	tsSysTimeTemp.wMilliseconds = wMillisecs;

	return tsSysTimeTemp;
}

void AppCore::GetCurSysTime(SYSTEMTIME& stTime)
{
	// Get current system time
	stTime = GetCurSysTime();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CalcTimeOffset
//	Description:	Calculate time with specific offset by second
//  Arguments:		stTime  - Return time data (ref-value)
//					nOffset - Offset second number
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::CalcTimeOffset(SYSTEMTIME& stTime, int nOffset)
{
	// Convert to seconds and calculate offset
	UINT nTotalSecs = TIME_TO_SECONDS(stTime);
	nTotalSecs += nOffset;

	// Revert back to time data
	stTime.wHour = GET_HOUR(nTotalSecs);
	stTime.wMinute = GET_MINUTE(nTotalSecs);
	stTime.wSecond = GET_SECOND(nTotalSecs);

	// Re-correct time data
	if (stTime.wHour >= 24) {
		// Decrease by 24-hour (next day)
		stTime.wHour -= 24;
	}
	else if (stTime.wHour < 0) {
		// Increase by 24-hour (previous day)
		stTime.wHour += 24;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CheckTimeMatch
//	Description:	Compare two time-structs and check if matching
//  Arguments:		timeDest - Destination time data
//					timePar	 - Comparison time data
//					nOffset	 - Offset second number (0 by default)
//  Return value:	BOOL - Result of comparison process (match or not)
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::CheckTimeMatch(SYSTEMTIME timeDest, SYSTEMTIME timePar, int nOffset /* = 0 */)
{
	// Second is 0
	timePar.wSecond = 0;

	// Time offset is not 0
	if (nOffset != 0) {
		// Calculate time offset
		CalcTimeOffset(timePar, nOffset);
	}

	// Compare each part value
	BOOL bRet = (timeDest.wHour == timePar.wHour);
	bRet &= (timeDest.wMinute == timePar.wMinute);
	bRet &= (timeDest.wSecond == timePar.wSecond);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatDispTime
//	Description:	Format time value for displaying
//  Arguments:		pLang	  - Language table pointer
//					nFormatID - Format string
//					timeVal	  - Given time value
//  Return value:	CString	- Format time string
//
//////////////////////////////////////////////////////////////////////////

CString	AppCore::FormatDispTime(LANGTABLE_PTR pLang, UINT nFormatID, SYSTEMTIME timeVal)
{
	// Load format string
	CString strFormat;
	BOOL bRet = LoadResourceString(strFormat, nFormatID);

	return FormatDispTime(pLang, strFormat, timeVal);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatDispTime
//	Description:	Format time value for displaying
//  Arguments:		pLang			 - Language table pointer
//					lpszFormatString - Format string
//					timeVal			 - Given time value
//  Return value:	CString	- Format time string
//
//////////////////////////////////////////////////////////////////////////

CString	AppCore::FormatDispTime(LANGTABLE_PTR pLang, LPCTSTR lpszFormatString, SYSTEMTIME timeVal)
{
	// Format time string
	UINT nTimePeriod = (timeVal.wHour < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	CString strTimePeriodFormat = Language::GetLanguageString(pLang, nTimePeriod);
	WORD wHour = (timeVal.wHour > 12) ? (timeVal.wHour - 12) : timeVal.wHour;
	WORD wMinute = timeVal.wMinute;

	CString strResult;
	strResult.Format(lpszFormatString, wHour, wMinute, strTimePeriodFormat);

	return strResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadResourceString
//	Description:	Load resource ID and return the string
//  Arguments:		nResStringID - ID of resource string
//  Return value:	LPCTSTR	- Returned resource string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR AppCore::LoadResourceString(UINT nResStringID)
{
	// Output result
	static CString strResult;
	strResult.Empty();

	// Load resource string
	BOOL bRet = strResult.LoadString(nResStringID);
	if (bRet == FALSE) {
		// Null string
		strResult = STRING_NULL;
	}

	return strResult.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadResourceString
//	Description:	Load resource ID and return the string
//  Arguments:		strResult	 - Returned resource string
//					nResStringID - ID of resource string
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::LoadResourceString(CString& strResult, UINT nResStringID)
{
	BOOL bRet = strResult.LoadString(nResStringID);
	if (bRet == FALSE) {
		// Null string
		strResult = STRING_NULL;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadResourceTextFile
//	Description:	Load resource text file by ID and return the text data
//  Arguments:		strTextData		- Returned file's text data
//					nResourceFileID - ID of the file in resource
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::LoadResourceTextFile(CString& strTextData, UINT nResourceFileID)
{
	// Empty result text data
	strTextData.Empty();

	// Get resource handle
	HINSTANCE hResInstance = AfxGetResourceHandle();
	if (hResInstance == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get resource handle failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Find resource file by ID
	HRSRC hRes = FindResource(hResInstance, MAKEINTRESOURCE(nResourceFileID), RT_RCDATA);
	if (hRes != NULL) {

		// Load resource data
		HGLOBAL hData = LoadResource(hResInstance, hRes);
		if (hData != NULL) {

			// Get text data size
			DWORD dwSize = SizeofResource(hResInstance, hRes);

			// Convert data to text data
			LPCTSTR lpData = static_cast<LPCTSTR>(LockResource(hData));
			strTextData = CString(lpData, dwSize);

			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetTokenList
//	Description:	Get token list from given string buffer
//  Arguments:		lpszBuff	 - Given string
//					retBuff		 - Returned token list
//					lpszKeyChars - Key characters
//  Return value:	int
//
//////////////////////////////////////////////////////////////////////////

int	AppCore::GetTokenList(LPTSTR lpszBuff, PBUFFER retBuff, LPCTSTR lpszKeyChars)
{
	// Get length
	int nBuffLength = _tcslen(lpszBuff);
	int nKeyLength = _tcslen(lpszKeyChars);

	// Re-format given string buffer
	int nBuffIndex = 0;
	for (int nIndex = 0; nIndex < nBuffLength; nIndex++) {

		// Invalid characters
		if ((lpszBuff[nIndex] == CHAR_ENDLINE) ||
			(lpszBuff[nIndex] == CHAR_RETURN))
			continue;

		// Keep valid characters only
		lpszBuff[nBuffIndex] = lpszBuff[nIndex];
		nBuffIndex++;

		// End string
		if (lpszBuff[nBuffIndex] == CHAR_ENDSTRING)
			break;
	}

	// Index and flag
	int nCurCharIndex = 0;
	int nTokenCount = 0, nTokenCharIndex = 0;
	int nQuoteFlag = 0;

	// Loop through given string buffer and separate tokens
	while ((nCurCharIndex <= nBuffLength) && (nCurCharIndex < MAX_BUFFER_LENGTH)) {

		// Init flag OFF
		int nKeyFlag = FLAG_OFF;

		// Get current character
		TCHAR tcCurChar = lpszBuff[nCurCharIndex];

		// In case of newline character
		if ((tcCurChar == CHAR_ENDLINE) || (tcCurChar == CHAR_RETURN)) {

			// Go to next character
			nCurCharIndex++;
			continue;
		}

		// In case of quotation mark
		if (tcCurChar == CHAR_QUOTAMARK) {

			// Change flag
			nQuoteFlag = (nQuoteFlag == FLAG_OFF) ? FLAG_ON : FLAG_OFF;
		}

		// If not in a quotation
		if (nQuoteFlag == FLAG_OFF) {

			// Loop through key string and find keychar match
			for (int nKeyIndex = 0; nKeyIndex < nKeyLength; nKeyIndex++) {

				// If key letter is matched
				if (tcCurChar == lpszKeyChars[nKeyIndex]) {

					// Mark as ON
					nKeyFlag = FLAG_ON;
				}
			}
		}

		// If current character is a key letter or end of string
		if ((nKeyFlag == FLAG_ON) || (tcCurChar == CHAR_ENDSTRING)) {

			// Empty token means continuous key letters
			if (nTokenCharIndex > 0) {

				// End current token
				retBuff[nTokenCount].tcToken[nTokenCharIndex] = CHAR_ENDSTRING;
				retBuff[nTokenCount].nLength = _tcsclen(retBuff[nTokenCount].tcToken);
				nTokenCharIndex = 0;

				// Increase token number counter
				nTokenCount++;
			}
		}

		// Current character is the quotation mark itself
		else if (tcCurChar == CHAR_QUOTAMARK) {

			// If token number exceeds max count, stop processing
			if (nTokenCount > MAX_TOKEN_COUNT)
				break;

			else {
				// Go to next character
				nCurCharIndex++;
				continue;
			}
		}

		// Current character is not a key letter or is in a quotation
		else {
			// Add character to current token
			retBuff[nTokenCount].tcToken[nTokenCharIndex] = tcCurChar;
			nTokenCharIndex++;
		}

		// Go to next character
		nCurCharIndex++;

		// If end of string or token number exceeds maximum limitation, stop processing
		if ((tcCurChar == CHAR_ENDSTRING) || (nTokenCount > MAX_TOKEN_COUNT))
			break;
	}

	// Return number of tokens
	return (nTokenCount);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpperEachWord
//	Description:	Capitalize first character of each word of given string
//  Arguments:		strInput - Given string (also return as ref-value)
//					bTrim	 - Trim string (left, right and mid)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::UpperEachWord(CString& strInput, BOOL bTrim)
{
	// Get string
	LPTSTR lpszString = strInput.GetBuffer();

	// Get string length
	int nLength = _tcslen(lpszString);
	if (nLength <= 0)
		return;

	// Lambda functions
	static auto IsNotSpace = [](TCHAR tcChar) { return (!std::isspace(tcChar)); };
	static auto BothSpaces = [](TCHAR tcFirst, TCHAR tcSecond) {
		return ((tcFirst == tcSecond) && (tcFirst == CHAR_SPACE));
		};

	// Trim string
	if (bTrim == TRUE) {
		// Use 'std::wstring' methods 
		std::wstring wstrTemp = lpszString;
		std::wstring::iterator newend = std::unique(wstrTemp.begin(), wstrTemp.end(), BothSpaces);
		wstrTemp.erase(newend, wstrTemp.end());

		// Trim left and right
		wstrTemp.erase(wstrTemp.begin(), std::find_if(wstrTemp.begin(), wstrTemp.end(), IsNotSpace));
		wstrTemp.erase(std::find_if(wstrTemp.rbegin(), wstrTemp.rend(), IsNotSpace).base(), wstrTemp.end());

		// Copy returned string
		_tcscpy(lpszString, wstrTemp.c_str());
		
		// Update length
		nLength = _tcslen(lpszString);
	}

	// Capitalize first character of each word
	for (int nIndex = 0; nIndex < nLength; nIndex++) {
		if (lpszString[nIndex] != CHAR_SPACE) {
			if ((nIndex == 0)	/* First character */ ||
				/* Not the first character and standing right next to a space */
				((nIndex > 0) && (lpszString[nIndex - 1] == CHAR_SPACE))) {
				// Convert to uppercase
				lpszString[nIndex] = std::toupper(lpszString[nIndex]);
			}
		}
	}

	// Return result
	strInput.SetString(lpszString);
	strInput.ReleaseBuffer();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSubFolderPath
//	Description:	Get full sub-folder path
//  Arguments:		lpszSubFolderName - Subfolder name
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR AppCore::GetSubFolderPath(LPCTSTR lpszSubFolderName)
{
	// Get application executable file path
	static CString strAppPath;
	if (strAppPath.IsEmpty()) {
		strAppPath = GetApplicationPath(FALSE);
	}

	// Initialize result string
	static CString strRetSubFolderPath;
	strRetSubFolderPath.Empty();

	// Make sub-folder path
	strRetSubFolderPath.SetString(strAppPath);
	if (lpszSubFolderName != NULL) {
		strRetSubFolderPath.Append(SYMBOL_BACKSLASH);
		strRetSubFolderPath.Append(lpszSubFolderName);
	}

	return strRetSubFolderPath.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	MakeFilePath
//	Description:	Make file path by given part names
//  Arguments:		strOutput	  - Output file path
//					lpszDirectory - Directory path
//					lpszFileName  - File name
//					lpszExtension - File extension
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::MakeFilePath(CString& strOutput, LPCTSTR lpszDirectory, LPCTSTR lpszFileName, LPCTSTR lpszExtension)
{
	// Format file path
	CString strFilePath;

	// Directory path, it may or may not be specified
	// If not specified, it means targeted file is in the same folder with executable file
	if (lpszDirectory != NULL) {
		// Add directory path
		strFilePath.Append(lpszDirectory);
		strFilePath.Append(SYMBOL_BACKSLASH);
	}

	// File name must be specified, if not, do nothing
	if (lpszFileName == NULL) return FALSE;
	else {
		// Add file name
		strFilePath.Append(lpszFileName);
	}

	// File extension, it may or may not be specified
	// If not specified, it means targeted file has no extension
	if (lpszExtension != NULL) {
		// Add file extension
		strFilePath.Append(lpszExtension);
	}

	// Return output path
	strOutput = strFilePath;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	StringValidate
//	Description:	Validate given string value
//  Arguments:		lpszSrc	 - Given string
//					dwError	 - Returned error code
//  Return value:	BOOL - Validation result
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::StringValidate(LPCTSTR lpszSrc, DWORD& dwError)
{
	BOOL bResult = TRUE;
	CString strInvalidKey = STRING_EMPTY;
	
	// Mark as normal first
	dwError = STRVAL_ERR_NORMAL;

	// String validation
	CString strSrc = CString(lpszSrc);
	if (strSrc.IsEmpty()) {
		// String empty
		bResult = FALSE;
		dwError = STRVAL_ERR_EMPTY;
	}
	else if (strSrc.GetLength() > MAX_STRING_LENGTH) {
		// String oversize
		bResult = FALSE;
		dwError = STRVAL_ERR_OVERSIZE;
	}
	else {
		// Invalid characters
		if (!strInvalidKey.IsEmpty()) {
			int nSrcLength = strSrc.GetLength();
			int nKeyLength = strInvalidKey.GetLength();
			for (int nKChIdx = 0; nKChIdx < nKeyLength; nKChIdx++) {
				TCHAR tcKeyChar = strInvalidKey.GetAt(nKChIdx);
				for (int nSrcIdx = 0; nSrcIdx < nSrcLength; nSrcIdx++) {
					if (strSrc.GetAt(nSrcIdx) == tcKeyChar) {
						bResult = FALSE;
						dwError = STRVAL_ERR_INVALIDCHAR;
						break;
					}
				}

				// Break the loop
				if (bResult == FALSE) break;
			}
		}
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	StringFormat
//	Description:	Format string (same as default MFC Format function)
//  Arguments:		nFormatTemplateID  - ID of resource format template string
//					...				   - Same as default MFC Format function
//  Return value:	LPCTSTR	- Returned formatted string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR AppCore::StringFormat(UINT nFormatTemplateID, ...)
{
	// Load resource format template string
	CString strTemplate;
	VERIFY(strTemplate.LoadString(nFormatTemplateID));
	if (strTemplate.IsEmpty()) return STRING_EMPTY;

	// Template string validation
	ATLASSERT(AtlIsValidString(strTemplate));

	// Result string
	static CString strResult;
	strResult.Empty();

	// Format string
	va_list argList;
	va_start(argList, strTemplate);
	strResult.FormatV(strTemplate, argList);
	va_end(argList);

	return strResult.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	StringFormat
//	Description:	Format string (same as default MFC Format function)
//  Arguments:		lpszFormatTemplate - Format template string
//					...				   - Same as default MFC Format function
//  Return value:	LPCTSTR	- Returned formatted string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR AppCore::StringFormat(LPCTSTR lpszFormatTemplate, ...)
{
	// Template string validation
	ATLASSERT(AtlIsValidString(lpszFormatTemplate));

	// Result string
	static CString strResult;
	strResult.Empty();

	// Format string
	va_list argList;
	va_start(argList, lpszFormatTemplate);
	strResult.FormatV(lpszFormatTemplate, argList);
	va_end(argList);

	return strResult.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SubString
//	Description:	Get a substring from a source string
//  Arguments:		lpszSrc		 - Source string
//					subDest		 - Destination substring data
//					tcFirstChar	 - First separator character
//					tcLastChar	 - Last separator character
//					bIncSepChar	 - Include separator characters
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::SubString(LPCTSTR lpszSrc, Substring& subDest, TCHAR tcFirstChar, TCHAR tcLastChar, BOOL bIncSepChar /* = FALSE */)
{
	CString strSrc(lpszSrc);

	// Empty destination data
	subDest.RemoveAll();

	// If source string is empty
	if (strSrc.IsEmpty()) {
		TRACE_ERROR("Error: Source string is empty!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Find starting and ending character index
	INT_PTR nFirstIndex = (tcFirstChar != NULL) ? (strSrc.Find(tcFirstChar)) : INT_INVALID;
	INT_PTR nLastIndex = (tcLastChar != NULL) ? (strSrc.ReverseFind(tcLastChar)) : INT_INVALID;

	// Debug log
	OutputDebugStringFormat(_T("[ALSTest] ==> GetSubString: nFirstIndex=%d, nLastIndex=%d"), nFirstIndex, nLastIndex);

	// Temporary output data
	Substring subResult;

	/**********************************************************************************************************/
	/*                                                                                                        */
	/*	Visualize how to get substring parts                                                                  */
	/*	Example:                                                                                              */
	/*				Source string = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  --> Length = 26                             */
	/*				Case #1:                                                                                  */
	/*						First character = 'E' --> Index = 4                                               */
	/*						Last character = 'R'  --> Index = 17                                              */
	/*					Output:                                                                               */
	/*							Left	= "ABCD"			--> Range = [0 -> 3]	--> Count = 4             */
	/*							Middle	= "FGHIJKLMNOPQ"	--> Range = [5 -> 16]	--> Count = 12            */
	/*							Right	= "STUVWXYZ"		--> Range = [18 -> 25]	--> Count = 8             */
	/*				Case #2:                                                                                  */
	/*						First character = '?' --> Index = -1                                              */
	/*						Last character = 'M'  --> Index = 12                                              */
	/*					Output:                                                                               */
	/*							Left	= "ABCDEFGHIJKL"	--> Range = [0 -> 11]	--> Count = 12            */
	/*							Middle	= Empty                                                               */
	/*							Right	= "NOPQRSTUVWXYZ"	--> Range = [13 -> 25]	--> Count = 13            */
	/*				Case #3:                                                                                  */
	/*						First character = 'K' --> Index = 10                                              */
	/*						Last character = '?'  --> Index = -1                                              */
	/*					Output:                                                                               */
	/*							Left	= "ABCDEFGHIJ"		--> Range = [0 -> 9]	--> Count = 10            */
	/*							Middle	= Empty                                                               */
	/*							Right	= "LMNOPQRSTUVWXYZ"	--> Range = [11 -> 25]	--> Count = 15            */
	/*                                                                                                        */
	/**********************************************************************************************************/

	// Get substring parts
	if ((nFirstIndex != INT_INVALID) && (nLastIndex != INT_INVALID)) {

		// Case #1:
		subResult.SetLeft(strSrc.Left(nFirstIndex));
		subResult.SetMid(strSrc.Mid((nFirstIndex + 1), (nLastIndex - nFirstIndex)));
		subResult.SetRight(strSrc.Right(strSrc.GetLength() - (nLastIndex + 1)));
	}
	else if ((nFirstIndex == INT_INVALID) && (nLastIndex != INT_INVALID)) {

		// Case #2:
		subResult.SetLeft(strSrc.Left(nLastIndex));
		subResult.SetMid(STRING_EMPTY);
		subResult.SetRight(strSrc.Right(strSrc.GetLength() - (nLastIndex + 1)));
	}
	else if ((nFirstIndex != INT_INVALID) && (nLastIndex == INT_INVALID)) {

		// Case #3:
		subResult.SetLeft(strSrc.Left(nFirstIndex));
		subResult.SetMid(STRING_EMPTY);
		subResult.SetRight(strSrc.Right(strSrc.GetLength() - (nFirstIndex + 1)));
	}

	// Get result
	BOOL bRet = (!subResult.IsEmpty());
	if (bRet != FALSE) {

		// Trim spaces
		subResult.TrimAll();

		// Copy output result
		subDest.Copy(subResult);
	}

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Additional functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetApplicationPath
//	Description:	Get application executable file path
//  Arguments:		bIncludeExeName - Including executable file name
//  Return value:	LPCTSTR - Return application path
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR AppCore::GetApplicationPath(BOOL bIncludeExeName)
{
	// Get the application's module handle
	HMODULE hModule = GetModuleHandle(NULL);

	// Get the full path of the executable file of the module
	TCHAR tcAppPath[MAX_PATH];
	if (!GetModuleFileName(hModule, tcAppPath, MAX_PATH)) {
		// Trace error
		TRACE_FORMAT("Error: Get module file name failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return STRING_EMPTY;
	}

	// Full path result
	static CString strRetAppPath;
	strRetAppPath.Empty();
	strRetAppPath = tcAppPath;

	// If not including the executable file name
	if (bIncludeExeName != TRUE) {
		// Remove the executable file name (and the last '\' as well)
		int nPos = strRetAppPath.ReverseFind(CHAR_BACKSLASH);
		if (nPos != INT_INVALID) {
			CString strTemp = strRetAppPath.Left(nPos);
			strRetAppPath = strTemp;
		}
	}

	return strRetAppPath.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetProductVersion
//	Description:	Get app executable file product version info
//  Arguments:		bFullVersion - Full product version number (x.x.x.x) 
//								   or short version number (x.x)
//  Return value:	CString - Product version string
//
//////////////////////////////////////////////////////////////////////////

CString AppCore::GetProductVersion(BOOL bFullVersion)
{
	// Get product file name
	CString strProductFileName;
	if (!MakeFilePath(strProductFileName, NULL, FILENAME_APPEXEFILE, FILEEXT_EXEFILE)) {
		// Trace error
		TRACE_ERROR("Error: Make file path failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return STRING_EMPTY;
	}

	// Get file version info size
	CString strProductVersion;
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(strProductFileName, &dwHandle);
	if (dwSize <= 0) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info size failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return STRING_EMPTY;
	}

	// Get file verision info structure
	BYTE* pVersionInfo = new BYTE[dwSize];
	if (!GetFileVersionInfo(strProductFileName, dwHandle, dwSize, pVersionInfo)) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return STRING_EMPTY;
	}

	// Querry version value
	UINT uLen;
	VS_FIXEDFILEINFO* lpFfi;
	if (!VerQueryValue(pVersionInfo, SYMBOL_BACKSLASH, (LPVOID*)&lpFfi, &uLen)) {
		// Trace error
		TRACE_FORMAT("Error: Querry version value failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return FALSE;
	}

	// Get product version successfully
	DWORD dwProductVersionMS = lpFfi->dwProductVersionMS;
	DWORD dwProductVersionLS = lpFfi->dwProductVersionLS;
	if (bFullVersion == TRUE) {
		// Get full product version number (x.x.x.x)
		strProductVersion.Format(_T("%d.%d.%d.%d"),
			HIWORD(dwProductVersionMS),
			LOWORD(dwProductVersionMS),
			HIWORD(dwProductVersionLS),
			LOWORD(dwProductVersionLS));
	}
	else {
		// Get short product version number (x.x)
		strProductVersion.Format(_T("%d.%d"),
			HIWORD(dwProductVersionMS),
			LOWORD(dwProductVersionMS));
	}

	delete[] pVersionInfo;

	return strProductVersion;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetProductVersion
//	Description:	Get app executable file product version info
//  Arguments:		strFullVersion  - Full product version number (x.x.x.x) 
//					strShortVersion	- Short product version number (x.x)
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::GetProductVersion(CString& strFullVersion, CString& strShortVersion)
{
	// Get product file name
	CString strProductFileName;
	if (!MakeFilePath(strProductFileName, NULL, FILENAME_APPEXEFILE, FILEEXT_EXEFILE)) {
		// Trace error
		TRACE_ERROR("Error: Make file path failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Get product version info size
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(strProductFileName, &dwHandle);
	if (dwSize <= 0) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info size failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Get product version info structure
	BYTE* pVersionInfo = new BYTE[dwSize];
	if (!GetFileVersionInfo(strProductFileName, dwHandle, dwSize, pVersionInfo)) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info structure failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return FALSE;
	}

	// Querry version value
	UINT uLen;
	VS_FIXEDFILEINFO* lpFfi;
	if (!VerQueryValue(pVersionInfo, SYMBOL_BACKSLASH, (LPVOID*)&lpFfi, &uLen)) {
		// Trace error
		TRACE_FORMAT("Error: Querry version value failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return FALSE;
	}

	// Get product version successfully
	DWORD dwProductVersionMS = lpFfi->dwProductVersionMS;
	DWORD dwProductVersionLS = lpFfi->dwProductVersionLS;

	// Get full product version number (x.x.x.x)
	strFullVersion.Format(_T("%d.%d.%d.%d"),
		HIWORD(dwProductVersionMS),
		LOWORD(dwProductVersionMS),
		HIWORD(dwProductVersionLS),
		LOWORD(dwProductVersionLS));

	// Get short product version number (x.x)
	strShortVersion.Format(_T("%d.%d"),
		HIWORD(dwProductVersionMS),
		LOWORD(dwProductVersionMS));

	delete[] pVersionInfo;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetWindowsVersion
//	Description:	Get Windows OS build version
//  Arguments:		None
//  Return value:	UINT - Windows version macro
//
//////////////////////////////////////////////////////////////////////////

UINT AppCore::GetWindowsOSVersion(void)
{
	// Init info data
	OSVERSIONINFOEX oviOSVersion;
	oviOSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	// Get function address
	NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
	if (RtlGetVersion == NULL) {
		// Trace error
		TRACE_FORMAT("Error: Get RtlGetVersion function address failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return INT_NULL;
	}

	// Get Window OS version
	RtlGetVersion(&oviOSVersion);

	// Return Windows OS version macro
	UINT nRetWinVer = WINDOWS_VERSION_NONE;
	if (oviOSVersion.dwBuildNumber >= OS_BUILDNUMBER_W11_EARLIEST) {
		// Is Windows 11
		nRetWinVer = WINDOWS_VERSION_11;
	}
	else if (oviOSVersion.dwBuildNumber >= OS_BUILDNUMBER_W10_ORIGINAL) {
		// Is Windows 10
		nRetWinVer = WINDOWS_VERSION_10;
	}
	else if ((oviOSVersion.dwBuildNumber >= OS_BUILDNUMBER_W8_EARLIEST) &&
		(oviOSVersion.dwBuildNumber <= OS_BUILDNUMBER_W8_LATEST)) {
		// Is Windows 8/8.1
		nRetWinVer = WINDOWS_VERSION_8;
	}
	else if ((oviOSVersion.dwBuildNumber >= OS_BUILDNUMBER_W7_EARLIEST) &&
		(oviOSVersion.dwBuildNumber <= OS_BUILDNUMBER_W7_LATEST)) {
		// Is Windows 7
		nRetWinVer = WINDOWS_VERSION_7;
	}
	else {
		// Unknown version
		nRetWinVer = WINDOWS_VERSION_UNKNOWN;
	}

	return nRetWinVer;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDeviceName
//	Description:	Get the computer device name
//  Arguments:		strDeviceName - Device name (out)
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::GetDeviceName(CString& strDeviceName)
{
	// Empty the output string
	strDeviceName.Empty();

	// Get the computer device name
	TCHAR tcDeviceName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwNameLength = sizeof(tcDeviceName) / sizeof(TCHAR);
	if (!GetComputerName(tcDeviceName, &dwNameLength)) {
		// Trace error
		TRACE_FORMAT("Error: Get computer name failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Return the computer name
	strDeviceName = tcDeviceName;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetCurrentUserName
//	Description:	Get the current Windows user name
//  Arguments:		strUserName - User name (out)
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::GetCurrentUserName(CString& strUserName)
{
	// Empty the output string
	strUserName.Empty();

	// Get the current user name
	TCHAR tcUserName[UNLEN + 1];
	DWORD dwNameLength = sizeof(tcUserName) / sizeof(TCHAR);
	if (!GetUserName(tcUserName, &dwNameLength)) {
		// Trace error
		TRACE_FORMAT("Error: Get user name failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Return the user name
	strUserName = tcUserName;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddRegistryKey
//	Description:	Add new key into registry
//  Arguments:		regInfo - Registry key info data
//  Return value:	BOOL - Registry processing result
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::AddRegistryKey(const REGISTRYINFO& regInfo)
{
	CRegKey regKey;
	REGISTRYKEY regKeyInfo = regInfo.GetRegistryKey();
	LONG lResult = regKey.Open(regInfo.GetRootKey(), regKeyInfo.strKeyName, KEY_ALL_ACCESS);
	if (lResult != ERROR_SUCCESS) {
		// Create register key if not found
		if (lResult == ERROR_NOT_FOUND) {
			regKey.Create(regInfo.GetRootKey(), regKeyInfo.strKeyName, REG_NONE,
					 REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, NULL);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	MakeRegistryPath
//	Description:	Make full registry path
//  Arguments:		regInfo		 - Registry key info data
//					nRegPathTYpe - Registry path type
//					bIncRootKey  - Include root key name or not
//  Return value:	LPCTSTR - Registry path
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR AppCore::MakeRegistryPath(const REGISTRYINFO& regInfo, UINT nRegPathType /* = REGPATH_FULL */, BOOL bIncRootKey /* = TRUE */)
{
	// Check root key info validity
	if (bIncRootKey != FALSE) {
		CString strRootKey = regInfo.GetRootKeyName();
		if ((regInfo.GetRootKey() == NULL) && (strRootKey.IsEmpty())) {
			// Trace error
			TRACE_ERROR("Error: Make registry path failed, rootkey name is invalid!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return STRING_NULL;
		}
	}

	// Check sub-key path validity
	if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_SUBPATH)) {
		CStringArray astrSubkeyPath;
		regInfo.GetSubkeyPath(astrSubkeyPath);
		if (astrSubkeyPath.IsEmpty()) {
			// Trace error
			TRACE_ERROR("Error: Make registry path failed, subkey info is invalid!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return STRING_NULL;
		}
	}

	// Check profile key validity
	if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_PROFILEKEY)) {
		CString strProfileName = regInfo.GetProfileName();
		if (strProfileName.IsEmpty()) {
			// Trace error
			TRACE_ERROR("Error: Make registry path failed, profile key name is invalid!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return STRING_NULL;
		}
	}

	// Check app name validity
	if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_APPNAME)) {
		CString strAppName = regInfo.GetAppName();
		if (strAppName.IsEmpty()) {
			// Trace error
			TRACE_ERROR("Error: Make registry path failed, application name is invalid!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return STRING_NULL;
		}
	}

	// Check section name array validity
	if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_SECTIONNAME)) {
		CStringArray astrSectionArray;
		regInfo.GetSubkeyPath(astrSectionArray);
		if (astrSectionArray.IsEmpty()) {
			// Trace error
			TRACE_ERROR("Error: Make registry path failed, section name is invalid!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return STRING_NULL;
		}
	}

	// Root key (string)
	CString strRootKeyNameTemp;
	CString strRootKeyName = regInfo.GetRootKeyName();
	if (!strRootKeyName.IsEmpty()) {
		strRootKeyNameTemp.Format(strRootKeyName);
	}

	// Sub-key path
	CString strSubKeyPathTemp;
	CStringArray astrSubkeyPath;
	regInfo.GetSubkeyPath(astrSubkeyPath);
	for (int nIndex = 0; nIndex < astrSubkeyPath.GetSize(); nIndex++) {
		if (nIndex > 0) {
			strSubKeyPathTemp.Append(SYMBOL_BACKSLASH);
		}
		strSubKeyPathTemp.Append(astrSubkeyPath.GetAt(nIndex));
	}

	// Profile key name
	CString strProfileKeyNameTemp;
	CString strProfileKeyName = regInfo.GetProfileName();
	if (!strProfileKeyName.IsEmpty()) {
		strProfileKeyNameTemp.Format(strProfileKeyName);
	}

	// App name
	CString strAppNameTemp;
	CString strAppName = regInfo.GetAppName();
	if (!strAppName.IsEmpty()) {
		strAppNameTemp.Format(strAppName);
	}

	// Section name
	CString strSectionNameTemp;
	CStringArray astrSectionArray;
	regInfo.GetSubkeyPath(astrSectionArray);
	for (int nIndex = 0; nIndex < astrSectionArray.GetSize(); nIndex++) {
		if (nIndex > 0) {
			strSectionNameTemp.Append(SYMBOL_BACKSLASH);
		}
		strSectionNameTemp.Append(astrSectionArray.GetAt(nIndex));
	}


	// Key name
	CString strKeyNameTemp;
	REGISTRYKEY regKeyInfo = regInfo.GetRegistryKey();
	CString strKeyName = regKeyInfo.GetKeyName();
	if (!strKeyName.IsEmpty()) {
		strKeyNameTemp.Format(strKeyName);
	}

	// Result string
	static CString strRegFullPath;
	strRegFullPath.Empty();

	// Make registry path by type
	int nRetFailedFlag = FLAG_OFF;
	if (!strRootKeyNameTemp.IsEmpty()) {

		// Initialize and include root key name
		strRegFullPath.Empty();
		if (bIncRootKey != FALSE) {
			strRegFullPath.Append(strRootKeyNameTemp);
		}

		if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_SUBPATH)) {
			if ((nRetFailedFlag != FLAG_ON) && (!strSubKeyPathTemp.IsEmpty())) {
				// Include separator character if rootkey is included
				if (bIncRootKey != FALSE) {
					strRegFullPath.Append(SYMBOL_BACKSLASH);
				}
				// Include sub-key path
				strRegFullPath.Append(strSubKeyPathTemp);
			}
			else {
				// Turn failed flag ON
				nRetFailedFlag = FLAG_ON;
			}
		}

		if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_PROFILEKEY)) {
			if ((nRetFailedFlag != FLAG_ON) && (!strProfileKeyName.IsEmpty())) {
				// Include profile key name
				strRegFullPath.Append(SYMBOL_BACKSLASH);
				strRegFullPath.Append(strProfileKeyName);
			}
			else {
				// Turn failed flag ON
				nRetFailedFlag = FLAG_ON;
			}
		}

		if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_APPNAME)) {
			if ((nRetFailedFlag != FLAG_ON) && (!strAppName.IsEmpty())) {
				// Include application name
				strRegFullPath.Append(SYMBOL_BACKSLASH);
				strRegFullPath.Append(strAppName);
			}
			else {
				// Turn failed flag ON
				nRetFailedFlag = FLAG_ON;
			}
		}

		if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_SECTIONNAME)) {
			if ((nRetFailedFlag != FLAG_ON) && (!strSectionNameTemp.IsEmpty())) {
				// Include section name
				strRegFullPath.Append(SYMBOL_BACKSLASH);
				strRegFullPath.Append(strSectionNameTemp);
			}
			else {
				// Turn failed flag ON
				nRetFailedFlag = FLAG_ON;
			}
		}

		if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_KEYNAME)) {
			if ((nRetFailedFlag != FLAG_ON) && (!strKeyNameTemp.IsEmpty())) {
				// Include key name
				strRegFullPath.Append(SYMBOL_BACKSLASH);
				strRegFullPath.Append(strKeyNameTemp);
			}
			else {
				// Turn failed flag ON
				nRetFailedFlag = FLAG_ON;
			}
		}
	}
	else {
		// Turn flag ON and return empty result
		nRetFailedFlag = FLAG_ON;
		strRegFullPath.Empty();
	}

	return strRegFullPath.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PlaySound
//	Description:	Play "BEEP" sound when sound is enabled
//  Arguments:		bSoundEnable - Enable sound
//					nTypeOfSound - Type of sound
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::PlaySound(BOOL bSoundEnable, UINT nTypeOfSound)
{
	// If sound is not enabled, do nothing
	if (!bSoundEnable)
		return;

	// Play sound by type here
	switch (nTypeOfSound)
	{
	case APP_SOUND_ERROR:
		::PlaySound(_T("SystemExclamination"), NULL, SND_ASYNC);
		break;
	case APP_SOUND_SUCCESS:
		::PlaySound(_T("SystemExit"), NULL, SND_ASYNC);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FileViewStd
//	Description:	Open a file to view using external standard fileviewer
//  Arguments:		eFileType	 - File type
//					lpszFilePath - Path of file
//  Return value:	BOOL - Result of file opening process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::FileViewStd(FILETYPE eFileType, LPCTSTR lpszFilePath)
{
	CString strAppPath = STRING_EMPTY;

	switch (eFileType) 
	{
	case FILETYPE_TEXT:
		strAppPath = PATH_APP_NOTEPAD;
		break;
	case FILETYPE_IMAGE:
		break;
	default:
		return FALSE;
	}

	// Run a file viewer instance
	HWND hWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	HINSTANCE hInstance = ShellExecute(hWnd, COMMAND_FLAG_OPEN, strAppPath, lpszFilePath, NULL, SW_SHOW);
	return (hInstance != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OpenWebURL
//	Description:	Open web URL using default web browser
//  Arguments:		lpszWebUrl - String of web URL
//  Return value:	BOOL - Result of web URL opening process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::OpenWebURL(LPCTSTR lpszWebUrl)
{
	// Run a web browser instance
	HINSTANCE hInstance = ShellExecute(0, 0, lpszWebUrl, NULL, NULL, SW_NORMAL);
	return (hInstance != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RunApp
//	Description:	Run an application by specified path
//  Arguments:		lpszAppPath - Path of excutive file
//					bRunAsAdmin	- Run as admin flag
//					bShowFlag	- Show window flag
//  Return value:	LRESULT - Result of app launching process
//
//////////////////////////////////////////////////////////////////////////

LRESULT AppCore::RunApp(LPCTSTR lpszAppPath, BOOL bRunAsAdmin /* = FALSE */, BOOL bShowFlag /* = TRUE */)
{
	// Param set
	CString strRunAs = (bRunAsAdmin) ? COMMAND_FLAG_RUNAS : COMMAND_FLAG_OPEN;
	int nShowFlag = (bShowFlag) ? SW_SHOW : SW_HIDE;

	// Run an executable instance
	HINSTANCE hInstance = ShellExecute(NULL, strRunAs, lpszAppPath, 0, 0, nShowFlag);
	return (LRESULT)(hInstance != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ExecuteCommand
//	Description:	Execute CMD command
//  Arguments:		lpszCmd		- Command string
//					bRunAsAdmin	- Run as admin flag
//					bShowFlag	- Show window flag
//  Return value:	LRESULT - Result of command execution process
//
//////////////////////////////////////////////////////////////////////////

LRESULT AppCore::ExecuteCommand(LPCTSTR lpszCommand, BOOL bRunAsAdmin /* = TRUE */, BOOL bShowFlag /* = TRUE */)
{
	// Format input command
	CString strCommandFormat;
	strCommandFormat.Format(_T("/C %s"), lpszCommand);

	// Flag param set
	CString strRunAsFlag = (bRunAsAdmin) ? COMMAND_FLAG_RUNAS : COMMAND_FLAG_OPEN;
	int nShowFlag = (bShowFlag) ? SW_SHOW : SW_HIDE;

	// Excute command
	HINSTANCE hInstance = ShellExecute(NULL, strRunAsFlag, PATH_APP_SYSTEMCMD, strCommandFormat, 0, nShowFlag);
	return (LRESULT)(hInstance != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CreateAppProcess
//	Description:	Create app process
//  Arguments:		lpszAppPath	 - App executable file path
//					lpszCmdLine	 - Command line
//					nStyle		 - App process style
//					dwErrorCode	 - Returned error code
//  Return value:	BOOL
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::CreateAppProcess(LPCWSTR lpszAppPath, LPWSTR lpszCmdLine, UINT nStyle, DWORD& dwErrorCode)
{
	// Startup info
	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO);

	// Process info
	PROCESS_INFORMATION ProcessInfo;
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));

	// Create process
	BOOL bResult = CreateProcess(lpszAppPath, lpszCmdLine, (LPSECURITY_ATTRIBUTES)NULL,
		(LPSECURITY_ATTRIBUTES)NULL, FALSE, (DWORD)nStyle, NULL, NULL, &StartupInfo, &ProcessInfo);

	if (bResult == FALSE) {
		// Get error code
		dwErrorCode = GetLastError();
		TRACE_FORMAT("Error: Create app process failed!!! (Code: 0x%08X)", dwErrorCode);
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return bResult;
	}

	// Wait for process exitting
	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);

	// Check whether our command succeeded?
	GetExitCodeProcess(ProcessInfo.hProcess, &dwErrorCode);

	// Avoid memory leak by closing process handle
	CloseHandle(ProcessInfo.hProcess);
	CloseHandle(ProcessInfo.hThread);

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDarkMode
//	Description:	Set dark mode for dialog with specified handle
//  Arguments:		pWnd			- Pointer of window
//					bEnableDarkMode - Enable/disable dark mode
//  Return value:	BOOL - Result of dark mode setting process	
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::SetDarkMode(CWnd* pWnd, BOOL bEnableDarkMode)
{
	// Load theme library
	HMODULE hUxTheme = LoadLibraryEx(_T("uxtheme.dll"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hUxTheme == NULL) {
		// Trace error
		TRACE_FORMAT("Error: Load UXTheme library failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Get function pointer
	using fnAllowDarkMode = BOOL (WINAPI*)(HWND hWND, BOOL bAllow);
	static const fnAllowDarkMode AllowDarkModeForWindow = (fnAllowDarkMode)GetProcAddress(hUxTheme, MAKEINTRESOURCEA(133));
	if (AllowDarkModeForWindow == NULL) {
		// Trace error
		TRACE_FORMAT("Error: Get AllowDarkModeForWindow function address failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Set dark mode for each dialog item
	CWnd* pWndChild = NULL;
	for (pWndChild = pWnd->GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		HWND hWndChild = pWndChild->GetSafeHwnd();
		AllowDarkModeForWindow(hWndChild, bEnableDarkMode);
		SetWindowTheme(hWndChild, _T("DarkMode_Explorer"), NULL);
		SendMessage(hWndChild, WM_THEMECHANGED, 0, 0);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DrawButton
//	Description:	Create button with icon
//  Arguments:		pBtn		 - Pointer of button item
//					nIconID		 - ID of button icon
//					lpszBtnTitle - Title of button
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::DrawButton(CButton*& pBtn, UINT nIconID, LPCTSTR lpszBtnTitle /* = STRING_EMPTY */)
{
	// Check validity
	if (pBtn == NULL)
		return;

	// Load icon
	HICON hBtnIcon = AfxGetApp()->LoadIcon(nIconID);
	if (hBtnIcon == NULL)
		return;

	// Button rect
	CRect rcBtnRect;
	pBtn->GetWindowRect(&rcBtnRect);

	// Button title
	CString strBtnTitle;
	strBtnTitle.SetString(lpszBtnTitle);
	if (!strBtnTitle.IsEmpty()) {
		pBtn->GetWindowText(strBtnTitle);
	}

	// Update button
	pBtn->SetButtonStyle(BS_ICON);
	pBtn->SetIcon(hBtnIcon);
	pBtn->UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnumFontFamiliesExProc
//	Description:	Callback function used with the EnumFontFamiliesEx
//  Arguments:		Default (see MSDN "EnumFontFamProc callback function")
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK EnumFontFamiliesExProc(ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam) 
{
	using wstring_vector = typename std::vector<std::wstring>;
	wstring_vector* fontNames = reinterpret_cast<wstring_vector*>(lParam);
	fontNames->push_back(lpelfe->elfLogFont.lfFaceName);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnumFontNames
//	Description:	Enumerate all currently available fonts
//  Arguments:		fontNames - Array to contain enumerated fonts
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::EnumFontNames(std::vector<std::wstring>& fontNames)
{
	// Define temp font
	LOGFONT logfont = STRUCT_ZERO;
	logfont.lfCharSet = DEFAULT_CHARSET;

	// Get font families
	HDC hdc = GetDC(NULL);
	EnumFontFamiliesEx(hdc, &logfont, (FONTENUMPROC)EnumFontFamiliesExProc, (LPARAM)&fontNames, 0);
	ReleaseDC(NULL, hdc);

	// Remove duplicated font names
	std::sort(fontNames.begin(), fontNames.end());
	fontNames.erase(std::unique(fontNames.begin(), fontNames.end()), fontNames.end());

	return (!fontNames.empty());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ValidateFontName
//	Description:	Check if an input string is a valid font name
//  Arguments:		lpszFontName - Input font name
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::ValidateFontName(LPCTSTR lpszFontName)
{
	// Array to get returned font names
	std::vector<std::wstring> fontNames;

	// Enumerate all currently available fonts
	BOOL bRet = EnumFontNames(fontNames);
	if (bRet == FALSE) {
		// Trace error
		TRACE_ERROR("Error: Enumerate fonts failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Find for input font name within the acquired font families
	bRet = (std::find(fontNames.begin(), fontNames.end(), lpszFontName) != fontNames.end());
	if (bRet == TRUE)
		return bRet;

	// For easier comparison, convert all to lowercase
	std::wstring lowerInput = lpszFontName;
	std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::towlower);
	std::vector<std::wstring> fontLowNames;
	for (auto it = fontNames.begin(); it != fontNames.end(); it++) {
		std::transform(it->begin(), it->end(), it->begin(), ::towlower);
		fontLowNames.push_back((*it));
	}

	// Find for input font name within the lower font name array
	bRet = (std::find(fontLowNames.begin(), fontLowNames.end(), lowerInput) != fontNames.end());

	return bRet;
}

