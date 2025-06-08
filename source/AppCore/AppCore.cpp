
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

#include "AppCore/AppCore.h"
#include "AppCore/Global.h"
#include "AppCore/MapTable.h"

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
	// Copy data
	this->Copy(pData);
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
	// Do not copy itself
	if (this == &pData) return;

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
//	Function name:	GetAppOption
//	Description:	Get application option data by ID
//  Arguments:		eAppOptionID - Option ID
//  Return value:	int - Option value
//
//////////////////////////////////////////////////////////////////////////

int ConfigData::GetAppOption(AppOptionID eAppOptionID) const
{
	int nResult = INT_INVALID;

	switch (eAppOptionID)
	{
	case AppOptionID::invalid:
		nResult = INT_INVALID;
		break;
	case AppOptionID::leftMouseAction:
		nResult = this->nLMBAction;
		break;
	case AppOptionID::middleMouseAction:
		nResult = this->nMMBAction;
		break;
	case AppOptionID::rightMouseAction:
		nResult = this->nRMBAction;
		break;
	case AppOptionID::rightMouseShowMenu:
		nResult = this->bRMBShowMenu;
		break;
	case AppOptionID::languageID:
		nResult = this->nLanguageID;
		break;
	case AppOptionID::showDlgAtStartup:
		nResult = this->bShowDlgAtStartup;
		break;
	case AppOptionID::startupEnabled:
		nResult = this->bStartupEnabled;
		break;
	case AppOptionID::confirmBeforeExecuting:
		nResult = this->bConfirmAction;
		break;
	case AppOptionID::saveAppEventLog:
		nResult = this->bSaveAppEventLog;
		break;
	case AppOptionID::saveAppHistoryLog:
		nResult = this->bSaveHistoryLog;
		break;
	case AppOptionID::runAsAdmin:
		nResult = this->bRunAsAdmin;
		break;
	case AppOptionID::showErrorMessage:
		nResult = this->bShowErrorMsg;
		break;
	case AppOptionID::notifySchedule:
		nResult = this->bNotifySchedule;
		break;
	case AppOptionID::allowCancelingSchedule:
		nResult = this->bAllowCancelSchedule;
		break;
	case AppOptionID::backgroundHotkeyEnabled:
		nResult = this->bEnableBackgroundHotkey;
		break;
	case AppOptionID::lockStateHotkeyEnabled:
		nResult = this->bLockStateHotkey;
		break;
	case AppOptionID::pwrReminderEnabled:
		nResult = this->bEnablePowerReminder;
		break;
	}

	return nResult;
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
	m_nSnoozeInterval = defaultSnoozeInterval;				// Snooze interval
	m_byRepeatDays = defaultActiveDays;						// Default repeat: All days of week
}

PwrRepeatSet::PwrRepeatSet(const PwrRepeatSet& pItem)
{
	// Copy data
	this->Copy(pItem);
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
	this->Copy(pItem);
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
	// Do not copy itself
	if (this == &pItem) return;

	// Copy data
	m_bRepeat = pItem.m_bRepeat;							// Repeat daily
	m_bAllowSnooze = pItem.m_bAllowSnooze;					// Allow snoozing mode
	m_nSnoozeInterval = pItem.m_nSnoozeInterval;			// Snooze interval
	m_byRepeatDays = pItem.m_byRepeatDays;					// Days of week (for repeating)
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
//	Function name:	ScheduleItem
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

ScheduleItem::ScheduleItem()
{
	// Initialize
	m_nItemID = ScheduleData::minItemID;				// Item ID
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
	this->Copy(pItem);
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
	this->Copy(pItem);
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
	// Do not copy itself
	if (this == &pItem) return;

	// Copy data
	m_nItemID = pItem.m_nItemID;						// Item ID
	m_bEnabled = pItem.m_bEnabled;						// Enable/disable status
	m_nActionID = pItem.m_nActionID;					// Schedule action ID
	m_stTime = pItem.m_stTime;							// Schedule time
	m_rpsRepeatSet.Copy(pItem.m_rpsRepeatSet);			// Repeat set data
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
	static const ScheduleItem schDummyItem;

	// Compare with this item and return result
	return this->Compare(schDummyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Print
//	Description:	Print schedule item
//  Arguments:		outputString - Output printed string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void ScheduleItem::Print(String& outputString) const
{
	// Use table, language and core functions
	using namespace MapTable;
	using namespace Language;
	using namespace AppCore;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format schedule data
	const wchar_t* enableState = (m_bEnabled == TRUE) ? Constant::Value::True : Constant::Value::False;							// Enable/disable state
	UINT nActionStringID = GetPairedID(IDTable::ActionName, m_nActionID);
	const wchar_t* actionName = GetLanguageString(ptrLanguage, nActionStringID);												// Schedule action
	const wchar_t* timeFormat = FormatDispTime(ptrLanguage, IDS_FORMAT_SHORTTIME, m_stTime).GetString();						// Schedule time
	const wchar_t* repeatState = (m_rpsRepeatSet.IsRepeatEnabled() == TRUE) ? Constant::Value::True : Constant::Value::False;	// Repeat daily

	// Print item
	outputString.Format(_T("Active=(%s), ItemID=%d, Action=(%s), Time=(%s), Repeat=(%s)"),
					enableState, m_nItemID, actionName, timeFormat, repeatState);
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
	m_schDefaultItem = ScheduleItem(ScheduleData::defaultItemID);
	m_arrSchedExtraItemList.clear();
}

ScheduleData::ScheduleData(const ScheduleData& pData)
{
	// Copy data
	this->Copy(pData);
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
	// Copy data
	this->Copy(pData);
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
	m_schDefaultItem = ScheduleItem(ScheduleData::defaultItemID);
	m_arrSchedExtraItemList.clear();
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
	// Do not copy itself
	if (this == &pData) return;

	// Remove existing data
	this->DeleteAll();

	// Copy default item
	this->m_schDefaultItem.Copy(pData.m_schDefaultItem);

	// Copy extra data
	for (int nIndex = 0; nIndex < pData.GetExtraItemNum(); nIndex++) {
		ScheduleItem schItem = pData.m_arrSchedExtraItemList.at(nIndex);
		this->m_arrSchedExtraItemList.push_back(schItem);
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

DWORD ScheduleData::Add(const ScheduleItem& pItem)
{
	// If item is empty, can not update
	if (pItem.IsEmpty())
		return Error::ItemIsEmpty;

	// If default item is currently empty
	if (m_schDefaultItem.IsEmpty()) {
		// Make item as default
		ScheduleItem schDefaultTemp(pItem);
		schDefaultTemp.SetItemID(ScheduleData::defaultItemID);
		m_schDefaultItem.Copy(schDefaultTemp);
		return Error::Success;
	}

	// If extra schedule data is currently empty
	if (m_arrSchedExtraItemList.empty()) {
		// Just add the item
		m_arrSchedExtraItemList.push_back(pItem);
		return Error::Success;
	}

	// If number of items exceeded limit
	if (GetExtraItemNum() >= ScheduleData::maxItemNum)
		return Error::MaxItemReached;

	// Check if item is duplicated, if yes, do not add
	for (int nIndex = 0; nIndex < GetExtraItemNum(); nIndex++) {
		ScheduleItem pItemTemp = GetItemAt(nIndex);
		if (pItemTemp.Compare(pItem) == TRUE) {
			// All data is duplicated
			return Error::ItemDuplicated;
		}
		else if (AppCore::CheckTimeMatch(pItemTemp.GetTime(), pItem.GetTime())) {
			// Time value is duplicated
			// Can not execute multiple action at the same time
			return Error::TimeDuplicated;
		}
	}

	// Create new temporary data
	ScheduleData* pNew = new ScheduleData;
	pNew->m_arrSchedExtraItemList.clear();

	// Copy old data to new one
	pNew->m_schDefaultItem.Copy(this->m_schDefaultItem);
	for (int nIndex = 0; nIndex < this->GetExtraItemNum(); nIndex++) {
		ScheduleItem schItem = this->GetItemAt(nIndex);
		pNew->m_arrSchedExtraItemList.push_back(schItem);
	}

	// Add new item and copy back to old data
	pNew->m_arrSchedExtraItemList.push_back(pItem);
	this->Copy(*pNew);

	// Delete data
	pNew->DeleteAll();
	if (pNew != NULL) {
		delete pNew;
		pNew = NULL;
	}

	return Error::Success;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Update
//	Description:	Update a Action Schedule item
//  Arguments:		pItem - Pointer of input item
//  Return value:	DWORD - Error code
//
//////////////////////////////////////////////////////////////////////////

DWORD ScheduleData::Update(const ScheduleItem& pItem)
{
	// If default item or extra schedule data is currently empty
	if (GetDefaultItem().IsEmpty() || IsAllEmpty()) {
		// Just add item
		return Add(pItem);
	}

	// If item is empty, can not update
	if (pItem.IsEmpty())
		return Error::ItemIsEmpty;

	// If item ID is matching with default item
	if (pItem.GetItemID() == ScheduleData::defaultItemID) {
		// Update default item
		GetDefaultItem().Copy(pItem);
		return Error::Success;
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
		ScheduleItem& schTemp = GetItemAt(nRetItemIndex);
		schTemp.Copy(pItem);
		return Error::Success;
	}
	// Otherwise,
	else {
		// Just add new
		return Add(pItem);
	}
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
	ScheduleItem& schItem = GetItemAt(nAtIndex);

	// Reset item value
	schItem.Copy(ScheduleItem());
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
		m_schDefaultItem.SetItemID(ScheduleData::defaultItemID);

		// Remove that extra item
		Delete(0);
	}

	// Check and remove empty extra items
	for (int nIndex = (GetExtraItemNum() - 1); nIndex >= 0; nIndex--) {
		ScheduleItem schTemp = GetItemAt(nIndex);
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
	UINT nRetNextID = ScheduleData::minItemID;
	for (int nIndex = 0; nIndex < GetExtraItemNum(); nIndex++) {
		ScheduleItem schItem = GetItemAt(nIndex);
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
//	Function name:	IsExtraEmpty
//	Description:	Check if extra data is empty
//  Arguments:		None
//  Return value:	BOOL - Result of all item empty
//
//////////////////////////////////////////////////////////////////////////

BOOL ScheduleData::IsExtraEmpty(void) const
{
	// If there's no item, return TRUE
	if (m_arrSchedExtraItemList.empty())
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
	pNew->m_arrSchedExtraItemList.clear();

	// Copy old data to new one (except the AtIndex item)
	pNew->m_schDefaultItem.Copy(this->m_schDefaultItem);
	for (int nIndex = 0; nIndex < this->GetExtraItemNum(); nIndex++) {
		if (nIndex == nAtIndex) continue;
		pNew->m_arrSchedExtraItemList.push_back(this->m_arrSchedExtraItemList.at(nIndex));
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
	m_arrSchedExtraItemList.clear();
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
	m_schDefaultItem = ScheduleItem(ScheduleData::defaultItemID);
	m_arrSchedExtraItemList.clear();
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
	m_dwModifiers = 0;								// Modifier keys
	m_dwVirtualKey = 0;								// Virtual key code
}

HotkeySetItem::HotkeySetItem(UINT nHKActionID)
{
	// Initialize
	m_bEnabled = FALSE;								// Hotkey enabled/disabled
	m_nHKActionID = nHKActionID;					// Hotkey action ID
	m_dwModifiers = 0;								// Modifier keys
	m_dwVirtualKey = 0;								// Virtual key code
}

HotkeySetItem::HotkeySetItem(const HotkeySetItem& pItem)
{
	// Copy data
	this->Copy(pItem);
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
	this->Copy(pItem);
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
	// Do not copy itself
	if (this == &pItem) return;

	// Copy data
	m_bEnabled = pItem.m_bEnabled;					// Hotkey enabled/disabled
	m_nHKActionID = pItem.m_nHKActionID;			// Hotkey action ID
	m_dwModifiers = pItem.m_dwModifiers;			// Modifier keys
	m_dwVirtualKey = pItem.m_dwVirtualKey;			// Virtual key code
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Print
//	Description:	Print HotkeySet item
//  Arguments:		outputString - Output printed string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetItem::Print(String& outputString) const
{
	// Use table and language functions
	using namespace MapTable;
	using namespace Language;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	const wchar_t* lpszEnable = (m_bEnabled == TRUE) ? _T("Enabled") : _T("Disabled");
	UINT nActionNameID = GetPairedID(IDTable::ActionName, GetPairedID(IDTable::HKActionID, m_nHKActionID));
	const wchar_t* lpszAction = GetLanguageString(ptrLanguage, nActionNameID);
	String keyStrokesStr = Constant::String::Empty;
	PrintKeyStrokes(keyStrokesStr);

	// Print item
	outputString.Format(_T("State=(%s), Action=(%s), Keystrokes=(%s)"),  lpszEnable, lpszAction, keyStrokesStr.GetString());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PrintKeyStrokes
//	Description:	Print HotkeySet item keystrokes data
//  Arguments:		strOutput - Output printed keystrokes string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetItem::PrintKeyStrokes(String& strOutput) const
{
	// Use table and language functions
	using namespace MapTable;
	using namespace Language;

	// Format keystrokes
	String strKeyStrokes = Constant::String::Empty;
	if (m_dwModifiers & MOD_CONTROL)	strKeyStrokes += _T("Ctrl + ");
	if (m_dwModifiers & MOD_ALT)		strKeyStrokes += _T("Alt + ");
	if (m_dwModifiers & MOD_WIN)		strKeyStrokes += _T("Win + ");
	strKeyStrokes += GetString(StringTable::FunctionKeys, m_dwVirtualKey);

	// Output string
	strOutput.Empty();
	strOutput = strKeyStrokes;
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
	m_arrHotkeySetList.clear();
}

HotkeySetData::HotkeySetData(const HotkeySetData& pData)
{
	// Copy data
	this->Copy(pData);
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
	// Copy data
	this->Copy(pData);
	return *this;
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
	// Do not copy itself
	if (this == &pData) return;

	// Remove existing data
	this->DeleteAll();

	// Copy data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		const Item& hksItem = pData.m_arrHotkeySetList.at(nIndex);
		this->m_arrHotkeySetList.push_back(hksItem);
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
	m_arrHotkeySetList.reserve(6);
	m_arrHotkeySetList.push_back(Item(HKID::displayOff));
	m_arrHotkeySetList.push_back(Item(HKID::sleep));
	m_arrHotkeySetList.push_back(Item(HKID::shutdown));
	m_arrHotkeySetList.push_back(Item(HKID::restart));
	m_arrHotkeySetList.push_back(Item(HKID::signOut));
	m_arrHotkeySetList.push_back(Item(HKID::hibernate));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add a hotkeyset item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::Add(const Item& pItem)
{
	// If data list is current empty
	if (m_arrHotkeySetList.empty()) {
		// Just add item
		m_arrHotkeySetList.push_back(pItem);
		return;
	}

	// Check if item exists, if yes, do not add
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		const Item& pItemTemp = GetItemAt(nIndex);
		if (pItemTemp.Compare(pItem) == TRUE)
			return;
	}

	// Create new temporary data
	HotkeySetData* pNew = new HotkeySetData;
	pNew->m_arrHotkeySetList.clear();
	
	// Copy old data to new one
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		const Item& hksItem = this->GetItemAt(nIndex);
		pNew->m_arrHotkeySetList.push_back(hksItem);
	}

	// Add new item and copy back to old data
	pNew->m_arrHotkeySetList.push_back(pItem);
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

void HotkeySetData::Update(const Item& pItem)
{
	// If data list is current empty
	if (m_arrHotkeySetList.empty()) {
		// Just add item
		Add(pItem);
		return;
	}

	// Check if item with same action ID or keystrokes exists
	int nDupActionIndex = INT_INVALID;
	int nDupKeyIndex = INT_INVALID;

	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		const Item& hksTemp = GetItemAt(nIndex);
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
		Item& hksTemp = GetItemAt(nDupActionIndex);
		hksTemp.EnableItem(pItem.IsEnabled());
		return;
	}

	// Delete existed duplicate keystrokes
	if (nDupKeyIndex != INT_INVALID) {
		Remove(nDupKeyIndex);
	}

	// If item with same action ID existed, update its data
	if (nDupActionIndex != INT_INVALID) {
		Item& hksTemp = GetItemAt(nDupActionIndex);
		hksTemp.Copy(pItem);
	}
	// Otherwise, add new
	else {
		Add(pItem);
	}
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
	Item& hksItem = GetItemAt(nAtIndex);

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
		Item& hksItem = GetItemAt(nIndex);
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
//	Function name:	IsAllEmpty
//	Description:	Check if all item are empty
//  Arguments:		None
//  Return value:	BOOL - Result of all item empty
//
//////////////////////////////////////////////////////////////////////////

BOOL HotkeySetData::IsAllEmpty() const
{
	// If there's no item, return TRUE
	if (m_arrHotkeySetList.empty())
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
	pNew->m_arrHotkeySetList.clear();

	// Copy old data to new one (except the AtIndex item)
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		if (nIndex == nAtIndex) continue;
		pNew->m_arrHotkeySetList.push_back(this->GetItemAt(nIndex));
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
//	Function name:	PrintKeyStrokes
//	Description:	Print HotkeySet item keystrokes data by ID
//  Arguments:		nHKID		 - Item hotkey ID
//					outputString - Output printed keystrokes string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void HotkeySetData::PrintKeyStrokes(UINT nHKID, String& outputString) const
{
	// Search for hotkey ID and get keystrokes string
	String keyStrokesStr = Constant::String::Empty;
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		Item hksItem = this->GetItemAt(nIndex);
		if (hksItem.GetActionID() == nHKID) {
			hksItem.PrintKeyStrokes(keyStrokesStr);
			break;
		}
	}

	// Output string
	outputString.Empty();
	outputString = keyStrokesStr;
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
	m_colorBkgrd = defaultBkgrdColor;							// Background color
	m_colorText = defaultTextColor;								// Text color
	m_strFontName = defaultFontName;							// Font name
	m_uiFontSize = defaultFontSize;								// Font size
	m_uiTimeout = defaultTimeout;								// Timeout (auto-close) interval
	m_uiIconID = defaultIconID;									// Message icon ID
	m_nIconSize = defaultIconSize;								// Message icon size
	m_byIconPos = defaultIconPosition;							// Message icon position
	m_byDisplayPos = defaultDisplayPosition;					// Message display position
	m_uiHMargin = defaultHorizontalMargin;						// Display area horizontal margin
	m_uiVMargin = defaultVerticalMargin;						// Display area vertical margin
}

RmdMsgStyleSet::RmdMsgStyleSet(const RmdMsgStyleSet& pItem)
{
	// Copy data
	this->Copy(pItem);
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
	this->Copy(pItem);
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
	// Do not copy itself
	if (this == &pItem) return;

	// Copy data
	m_colorBkgrd = pItem.m_colorBkgrd;							// Background color
	m_colorText = pItem.m_colorText;							// Text color
	m_strFontName = pItem.m_strFontName;						// Font name
	m_uiFontSize = pItem.m_uiFontSize;							// Font size
	m_uiTimeout = pItem.m_uiTimeout;							// Timeout (auto-close) interval
	m_uiIconID = pItem.m_uiIconID;								// Message icon ID
	m_nIconSize = pItem.m_nIconSize;							// Message icon size
	m_byIconPos = pItem.m_byIconPos;							// Message icon position
	m_byDisplayPos = pItem.m_byDisplayPos;						// Message display position
	m_uiHMargin = pItem.m_uiHMargin;							// Display area horizontal margin
	m_uiVMargin = pItem.m_uiVMargin;							// Display area vertical margin
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
//
//////////////////////////////////////////////////////////////////////////

PwrReminderItem::PwrReminderItem()
{
	// Init data
	m_bEnabled = FALSE;										// Enable state
	m_nItemID = PwrReminderData::minItemID;					// Item ID
	m_strMessage = Constant::String::Empty;					// Message content
	m_nEventID = Event::atSetTime;							// Event ID
	m_stTime = SYSTEMTIME_ZERO;								// Event time
	m_dwMsgStyle = Style::messageBox;						// Reminder style
	m_rpsRepeatSet = PwrRepeatSet();						// Repeat set
	m_bUseCustomStyle = FALSE;								// Use message custom style
	m_rmsMsgStyleSet = RmdMsgStyleSet();					// Reminder message style set
}

PwrReminderItem::PwrReminderItem(const PwrReminderItem& pItem)
{
	// Copy data
	this->Copy(pItem);
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
	this->Copy(pItem);
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
	// Do not copy itself
	if (this == &pItem) return;

	// Copy data
	m_bEnabled = pItem.m_bEnabled;							// Enable state
	m_nItemID = pItem.m_nItemID;							// Item ID
	m_strMessage = pItem.m_strMessage;						// Message content
	m_nEventID = pItem.m_nEventID;							// Event ID
	m_stTime = pItem.m_stTime;								// Event time
	m_dwMsgStyle = pItem.m_dwMsgStyle;						// Reminder style
	m_rpsRepeatSet.Copy(pItem.m_rpsRepeatSet);				// Repeat set
	m_bUseCustomStyle = pItem.m_bUseCustomStyle;			// Use message custom style
	m_rmsMsgStyleSet.Copy(pItem.m_rmsMsgStyleSet);			// Reminder message style set
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
	static const PwrReminderItem pwrDummyItem;

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
//	Function name:	IsAllowSnoozing
//	Description:	Check if item snooze mode is available
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderItem::IsAllowSnoozing(void) const
{
	// If current eventID is not at settime
	if (m_nEventID != Event::atSetTime) {
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
//	Function name:	Print
//	Description:	Print reminder item
//  Arguments:		outputString - Output printed string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void PwrReminderItem::Print(String& outputString) const
{
	// Use table, language and core functions
	using namespace MapTable;
	using namespace Language;
	using namespace AppCore;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	const wchar_t* enableStr = (m_bEnabled == TRUE) ? _T("Enabled") : _T("Disabled");
	String messageStr = m_strMessage;
	if (messageStr.GetLength() > (Constant::Max::DisplayLogStringLength + 3)) {
		messageStr = m_strMessage.Left(Constant::Max::DisplayLogStringLength) + _T("...");
	}
	int nTemp = GetPairedID(IDTable::PwrReminderEvent, m_nEventID);
	String eventStr = GetLanguageString(ptrLanguage, nTemp);
	if (m_nEventID == Event::atSetTime) {
		// Format time string
		String formatString = eventStr;
		eventStr = FormatDispTime(ptrLanguage, formatString, m_stTime);
	}
	nTemp = GetPairedID(IDTable::PwrReminderStyle, m_dwMsgStyle);
	const wchar_t* styleStr = GetLanguageString(ptrLanguage, nTemp);

	// Print item
	outputString.Format(_T("State=(%s), ItemID=%d, Msg=(%s), Event=(%s), Style=(%s), Repeat=%d"),
		enableStr, m_nItemID, messageStr.GetString(), eventStr.GetString(), styleStr, m_rpsRepeatSet.IsRepeatEnabled());
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
	m_arrRmdItemList.clear();
	m_rmdCommonStyle = RmdMsgStyleSet();
}

PwrReminderData::PwrReminderData(const PwrReminderData& pData)
{
	// Copy data
	this->Copy(pData);
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
	// Copy data
	this->Copy(pData);
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
	m_arrRmdItemList.clear();
	m_rmdCommonStyle = RmdMsgStyleSet();
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
	// Do not copy itself
	if (this == &pData) return;

	// Remove existing data
	this->DeleteAll();

	// Copy reminder data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		PwrReminderItem pwrItem = pData.m_arrRmdItemList.at(nIndex);
		this->m_arrRmdItemList.push_back(pwrItem);
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

void PwrReminderData::Add(const PwrReminderItem& pItem)
{
	// If data list is current empty
	if (m_arrRmdItemList.empty()) {
		// Just add item
		m_arrRmdItemList.push_back(pItem);
		return;
	}

	// Check if item exists, if yes, do not add
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		PwrReminderItem pItemTemp = GetItemAt(nIndex);
		if (pItemTemp.Compare(pItem) == TRUE)
			return;
	}

	// Create new temporary data
	PwrReminderData* pNew = new PwrReminderData;
	pNew->m_arrRmdItemList.clear();

	// Copy common message style data
	pNew->m_rmdCommonStyle.Copy(this->m_rmdCommonStyle);

	// Copy old data to new one
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		PwrReminderItem pwrItem = this->GetItemAt(nIndex);
		pNew->m_arrRmdItemList.push_back(pwrItem);
	}

	// Add new item and copy back to old data
	pNew->m_arrRmdItemList.push_back(pItem);
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

void PwrReminderData::Update(const PwrReminderItem& pItem)
{
	// If data list is current empty
	if (m_arrRmdItemList.empty()) {
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
		PwrReminderItem& pwrTemp = GetItemAt(nRetItemIndex);
		pwrTemp.Copy(pItem);
	}
	// Otherwise, add new
	else {
		Add(pItem);
	}
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
	PwrReminderItem& pwrItem = GetItemAt(nAtIndex);

	// Reset item value
	pwrItem.Copy(PwrReminderItem());
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
		PwrReminderItem pwrTemp = GetItemAt(nIndex);
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
	UINT nRetNextID = PwrReminderData::minItemID;
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		PwrReminderItem pwrItem = GetItemAt(nIndex);
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
//	Function name:	IsAllEmpty
//	Description:	Check if all item are empty
//  Arguments:		None
//  Return value:	BOOL - Result of all item empty
//
//////////////////////////////////////////////////////////////////////////

BOOL PwrReminderData::IsAllEmpty() const
{
	// If there's no item, return TRUE
	if (m_arrRmdItemList.empty())
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
	pNew->m_arrRmdItemList.clear();

	// Copy common message style data
	pNew->m_rmdCommonStyle.Copy(this->m_rmdCommonStyle);

	// Copy old data to new one (except the AtIndex item)
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		if (nIndex == nAtIndex) continue;
		pNew->m_arrRmdItemList.push_back(this->m_arrRmdItemList.at(nIndex));
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
	this->Copy(pItem);
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
	this->Copy(pItem);
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
	// Do not copy itself
	if (this == &pItem) return;

	// Copy data
	m_nCategory = pItem.m_nCategory;						// Item category
	m_nItemID = pItem.m_nItemID;							// Power Reminder item ID
	m_nDisplayFlag = pItem.m_nDisplayFlag;					// Item displaying flag
	m_nSkipFlag = pItem.m_nSkipFlag;						// Item skip flag
	m_nSnoozeFlag = pItem.m_nSnoozeFlag;					// Item snooze trigger flag
	m_stNextSnoozeTime = pItem.m_stNextSnoozeTime;			// Next snooze trigger time
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	HistoryInfoData
//	Description:	Constructor
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
	m_strDescription = Constant::String::Empty;				// History description (attached info)
}

HistoryInfoData::HistoryInfoData(const HistoryInfoData& pData)
{
	// Copy data
	this->Copy(pData);
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
	this->Copy(pData);
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
	// Do not copy itself
	if (this == &pData) return;

	// Copy data
	m_bInitState = pData.m_bInitState;						// Init state
	m_nCategoryID = pData.m_nCategoryID;					// Category ID
	m_stTimestamp = pData.m_stTimestamp;					// Timestamp of history
	m_nItemID = pData.m_nItemID;							// Item ID
	m_nActionID = pData.m_nActionID;						// History action ID
	m_bActionResult = pData.m_bActionResult;				// Action result
	m_dwErrorCode = pData.m_dwErrorCode;					// Returned error code
	m_strDescription = pData.m_strDescription;				// History description (attached info)
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
	m_strDescription = Constant::String::Empty;				// History description (attached info)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SystemEvent
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

SystemEvent::SystemEvent(EventID eventID)
{
	// Initialize
	m_sysEventID = eventID;									// System event ID
	m_timeStamp = SYSTEMTIME_ZERO;							// Event timestamp
}

SystemEvent::SystemEvent(const SystemEvent& pItem)
{
	// Copy data
	m_sysEventID = pItem.m_sysEventID;						// System event ID
	m_timeStamp = pItem.m_timeStamp;						// Event timestamp
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

SystemEvent& SystemEvent::operator=(const SystemEvent& pItem)
{
	// Copy data
	m_sysEventID = pItem.m_sysEventID;						// System event ID
	m_timeStamp = pItem.m_timeStamp;						// Event timestamp
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SystemEventTracker
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

SystemEventTracker::SystemEventTracker()
{
	// Initialize
	m_arrTrackingData.clear();								// System event tracking data
}

SystemEventTracker::SystemEventTracker(const SystemEventTracker& pData)
{
	// Copy data
	m_arrTrackingData = pData.m_arrTrackingData;			// System event tracking data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

SystemEventTracker& SystemEventTracker::operator=(const SystemEventTracker& pData)
{
	// Copy data
	m_arrTrackingData = pData.m_arrTrackingData;			// System event tracking data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegistryValue
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

RegistryValue::RegistryValue()
{
	// Initialize
	m_pstrValue = NULL;										// String value
	m_pdwValue = NULL;										// DWORD (32-bit) value
	m_pqwValue = NULL;										// QWORD (64-bit) value
	m_pastrValue = NULL;									// Multi-string value
}

RegistryValue::RegistryValue(const RegistryValue& pItem)
{
	// Copy data
	this->Copy(pItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~RegistryValue
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

RegistryValue::~RegistryValue()
{
	// Remove all data
	this->Reset();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

RegistryValue& RegistryValue::operator=(const RegistryValue& pItem)
{
	// Copy data
	this->Copy(pItem);
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Initialize with specific data type
//  Arguments:		nType - Value type
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL RegistryValue::Init(UINT nRegValueType)
{
	// Reset data
	this->Reset();

	// Initialize with specific data type
	switch (nRegValueType)
	{
	case Type::STRING:
		m_pstrValue = new String(Constant::String::Empty);
		return (m_pstrValue != NULL);

	case Type::DWORD_32:
		m_pdwValue = new DWORD(INT_NULL);
		return (m_pdwValue != NULL);

	case Type::QWORD_64:
		m_pqwValue = new QWORD(INT_NULL);
		return (m_pqwValue != NULL);

	case Type::MULTI_STRING:
		m_pastrValue = new StringArray();
		return (m_pastrValue != NULL);

	default:
		return FALSE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another registry value item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryValue::Copy(const RegistryValue& pItem)
{
	// Do not copy itself
	if (this == &pItem) return;

	// Copy data (one value only)
	if (pItem.m_pstrValue != NULL) {
		if (m_pstrValue != NULL) {
			delete m_pstrValue;
		}
		// Copy string value
		m_pstrValue = new String(*pItem.m_pstrValue);
	}
	else if (pItem.m_pdwValue != NULL) {
		if (m_pdwValue != NULL) {
			delete m_pdwValue;
		}
		// Copy DWORD (32-bit) value
		m_pdwValue = new DWORD(*pItem.m_pdwValue);
	}
	else if (pItem.m_pqwValue != NULL) {
		if (m_pqwValue != NULL) {
			delete m_pqwValue;
		}
		// Copy QWORD (64-bit) value
		m_pqwValue = new QWORD(*pItem.m_pqwValue);
	}
	else if (pItem.m_pastrValue != NULL) {
		if (m_pastrValue != NULL) {
			m_pastrValue->clear();
			delete m_pastrValue;
		}
		// Copy Multi-string value
		m_pastrValue = new StringArray;
		m_pastrValue->assign(pItem.m_pastrValue->begin(), pItem.m_pastrValue->end());
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Reset
//	Description:	Remove all registry value data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryValue::Reset(void)
{
	// Cleanup string data
	if (m_pstrValue != NULL) {
		delete m_pstrValue;
		m_pstrValue = NULL;
	}

	// Cleanup numeric data
	if (m_pdwValue != NULL) {
		delete m_pdwValue;
		m_pdwValue = NULL;
	}
	if (m_pqwValue != NULL) {
		delete m_pqwValue;
		m_pqwValue = NULL;
	}

	// Cleanup array data
	if (m_pastrValue != NULL) {
		m_pastrValue->clear();
		delete m_pastrValue;
		m_pastrValue = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Refactor
//	Description:	Refactor data and optimize by keeping current value
//					and reset the other data pointers
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryValue::Refactor(void)
{
	UINT nType = GetType();
	if (nType == Type::STRING) {
		String tempStr = *m_pstrValue;
		if (Init(Type::STRING)) {
			*m_pstrValue = tempStr;
		}
	}
	else if (nType == Type::DWORD_32) {
		DWORD dwTemp = *m_pdwValue;
		if (Init(Type::DWORD_32)) {
			*m_pdwValue = dwTemp;
		}
	}
	else if (nType == Type::QWORD_64) {
		QWORD qwTemp = *m_pqwValue;
		if (Init(Type::QWORD_64)) {
			*m_pqwValue = qwTemp;
		}
	}
	else if (nType == Type::MULTI_STRING) {
		StringArray astrTemp;
		astrTemp.assign(m_pastrValue->begin(), m_pastrValue->end());
		if (Init(Type::MULTI_STRING)) {
			m_pastrValue->assign(astrTemp.begin(), astrTemp.end());
		}
	}
	else {
		// Reset all data
		this->Reset();
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetType
//	Description:	Get current data value type
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

UINT RegistryValue::GetType(void) const
{
	if (m_pstrValue != NULL)
		return Type::STRING;
	else if (m_pdwValue != NULL)
		return Type::DWORD_32;
	else if (m_pqwValue != NULL)
		return Type::QWORD_64;
	else if (m_pastrValue)
		return Type::MULTI_STRING;
	else
		return Type::NONE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegistryKey
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

RegistryKey::RegistryKey()
{
	// Initialize
	m_strKeyName = Constant::String::Empty;					// Key name (string)
	m_regValue = REGISTRYVALUE();							// Register value data
}

RegistryKey::RegistryKey(const RegistryKey& pItem)
{
	// Copy data
	this->Copy(pItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

RegistryKey& RegistryKey::operator=(const RegistryKey& pItem)
{
	// Copy data
	this->Copy(pItem);
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

void RegistryKey::Copy(const RegistryKey& pItem)
{
	// Copy data
	m_strKeyName = pItem.m_strKeyName;						// Key name (string)
	m_regValue = pItem.m_regValue;							// Register value data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Clear
//	Description:	Remove all registry key info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryKey::Clear(void)
{
	// Reset data
	m_strKeyName = Constant::String::Empty;					// Key name (string)
	m_regValue.Reset();										// Data values
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
	m_strRootKey = Constant::String::Empty;					// Root key (string)
	m_astrSubkeyPath.clear();								// Subkey path (string array)
	m_strCompanyName = Constant::String::Empty;				// Company name (string)
	m_strProductName = Constant::String::Empty;				// Product name (string)
	m_astrSectionArray.clear();								// Section array (string)
	m_regKeyInfo = RegistryKey();							// Registry key info
}

RegistryInfo::RegistryInfo(const RegistryInfo& pItem)
{
	// Copy data
	this->Copy(pItem);
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
	this->Copy(pItem);
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
	// Do not copy itself
	if (this == &pItem) return;

	// Copy data
	m_hRootKey = pItem.m_hRootKey;							// Root key (HKEY)
	m_strRootKey = pItem.m_strRootKey;						// Root key (string)
	m_astrSubkeyPath = pItem.m_astrSubkeyPath;				// Subkey path (string array)
	m_strCompanyName = pItem.m_strCompanyName;				// Company name (string)
	m_strProductName = pItem.m_strProductName;				// Product name (string)
	m_astrSectionArray = pItem.m_astrSectionArray;			// Section array (string)
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
	m_strRootKey = Constant::String::Empty;					// Root key (string)
	m_astrSubkeyPath.clear();								// Subkey path (string array)
	m_strCompanyName = Constant::String::Empty;				// Company name (string)
	m_strProductName = Constant::String::Empty;				// Product name (string)
	m_astrSectionArray.clear();								// Section array (string)
	m_regKeyInfo.Clear();									// Registry key info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSubkeyPath
//	Description:	Get Subkey path array
//  Arguments:		arrOutput - Output string array
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::GetSubkeyPath(StringArray& arrOutput) const
{
	// Empty destination output array
	arrOutput.clear();

	// Copy data
	arrOutput = m_astrSubkeyPath;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSectionName
//	Description:	Get Section name array
//  Arguments:		arrOutput - Output string array
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void RegistryInfo::GetSectionName(StringArray& arrOutput) const
{
	// Empty destination output array
	arrOutput.clear();

	// Copy data
	arrOutput = m_astrSectionArray;
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
	m_strLeft = Constant::String::Empty;			// Left part
	m_strMid = Constant::String::Empty;				// Middle part
	m_strRight = Constant::String::Empty;			// Right part
}

Substring::Substring(const Substring& pData)
{
	// Copy data
	this->Copy(pData);
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
	this->Copy(pData);
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
	// Do not copy itself
	if (this == &pData) return;

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
//	Function name:	StringFormat
//	Description:	Format string (same as default MFC Format function)
//  Arguments:		formatTemplateID  - ID of resource format template string
//					...				  - Same as default MFC Format function
//  Return value:	String - Returned formatted string
//
//////////////////////////////////////////////////////////////////////////

String StringUtils::StringFormat(UINT formatTemplateID, ...)
{
	// Load resource format template string
	String templateString = StringUtils::LoadResourceString(formatTemplateID);
	if (templateString.IsEmpty())
		return Constant::String::Empty;

	// Template string validation
	ATLASSERT(AtlIsValidString(templateString));

	// Result string
	String resultString;

	// Format string
	va_list argList;
	va_start(argList, templateString.GetString());
	resultString.FormatV(templateString, argList);
	va_end(argList);

	return resultString;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	StringFormat
//	Description:	Format string (same as default MFC Format function)
//  Arguments:		formatTemplate - Format template string
//					...			   - Same as default MFC Format function
//  Return value:	String	- Returned formatted string
//
//////////////////////////////////////////////////////////////////////////

String StringUtils::StringFormat(const wchar_t* formatTemplate, ...)
{
	// Template string validation
	ATLASSERT(AtlIsValidString(formatTemplate));

	// Result string
	String resultString;

	// Format string
	va_list argList;
	va_start(argList, formatTemplate);
	resultString.FormatV(formatTemplate, argList);
	va_end(argList);

	return resultString;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadResourceString
//	Description:	Load resource ID and return the string
//  Arguments:		nResStringID - ID of resource string
//  Return value:	String	- Returned resource string
//
//////////////////////////////////////////////////////////////////////////

String StringUtils::LoadResourceString(UINT resourceStringID)
{
	// Output result
	String resultString;

	// Get resource handle
	HINSTANCE hResInstance = AfxGetResourceHandle();
	if (hResInstance == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get resource handle failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return resultString;
	}

	// Load resource string
	wchar_t _tempBuffer[Constant::Max::StringLength] = {0};
	int _length = ::LoadStringW(hResInstance, resourceStringID, _tempBuffer, static_cast<int>(std::size(_tempBuffer)));
	if (_length <= 0)
		resultString = Constant::String::Null;
	else
		resultString.SetString(_tempBuffer);

	return resultString;
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

bool StringUtils::LoadResourceString(String& resultStr, UINT resourceStringID)
{
	// Output result
	resultStr.Empty();

	// Get resource handle
	HINSTANCE hResInstance = AfxGetResourceHandle();
	if (hResInstance == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get resource handle failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Load resource string
	wchar_t _tempBuffer[Constant::Max::StringLength] = {0};
	int _length = ::LoadStringW(hResInstance, resourceStringID, _tempBuffer, static_cast<int>(std::size(_tempBuffer)));
	if (_length <= 0) {
		resultStr = Constant::String::Null;
		return false;
	}
	else
		resultStr.SetString(_tempBuffer);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadResourceTextFile
//	Description:	Load resource text file by ID and return the text data
//  Arguments:		resourceFileID - ID of the file in resource
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

String StringUtils::LoadResourceTextData(UINT resourceFileID)
{
	// Empty result text data
	String resultTextData;

	// Get resource handle
	HINSTANCE hResInstance = AfxGetResourceHandle();
	if (hResInstance == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get resource handle failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return resultTextData;
	}

	// Find resource file by ID
	HRSRC hRes = FindResource(hResInstance, MAKEINTRESOURCE(resourceFileID), RT_RCDATA);
	if (hRes != NULL) {

		// Load resource data
		HGLOBAL hData = LoadResource(hResInstance, hRes);
		if (hData != NULL) {

			// Convert data to text data
			const wchar_t* dataBuffer = static_cast<const wchar_t*>(LockResource(hData));
			resultTextData = dataBuffer;
		}
	}

	return resultTextData;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetApplicationPath
//	Description:	Get application executable file path
//  Arguments:		includeExeName - Including executable file name
//  Return value:	String - Return application path
//
//////////////////////////////////////////////////////////////////////////

String StringUtils::GetApplicationPath(bool includeExeName)
{
	// Get the application's module handle
	HMODULE hModule = GetModuleHandle(NULL);

	// Get the full path of the executable file of the module
	wchar_t appPathBuffer[MAX_PATH];
	if (!GetModuleFileName(hModule, appPathBuffer, MAX_PATH)) {
		// Trace error
		TRACE_FORMAT("Error: Get module file name failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return Constant::String::Empty;
	}

	// Full path result
	String retAppPath(appPathBuffer);

	// If not including the executable file name
	if (!includeExeName) {
		// Remove the executable file name (and the last '\' as well)
		int nPos = retAppPath.ReverseFind(Constant::Char::Backslash);
		if (nPos != INT_INVALID) {
			String strTemp = retAppPath.Left(nPos);
			retAppPath = strTemp;
		}
	}

	return retAppPath;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSubFolderPath
//	Description:	Get full sub-folder path
//  Arguments:		lpszSubFolderName - Subfolder name
//  Return value:	String
//
//////////////////////////////////////////////////////////////////////////

String StringUtils::GetSubFolderPath(const wchar_t* subFolderName)
{
	// Get application executable file path
	String appPath = GetApplicationPath(FALSE);

	// Initialize result string
	String retSubFolderPath;

	// Make sub-folder path
	retSubFolderPath = appPath;
	if (subFolderName) {
		retSubFolderPath.Append(Constant::Symbol::Backslash);
		retSubFolderPath.Append(subFolderName);
	}

	return retSubFolderPath;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	MakeFilePath
//	Description:	Make file path by given part names
//  Arguments:		directory - Directory path
//					fileName  - File name
//					extension - File extension
//  Return value:	String
//
//////////////////////////////////////////////////////////////////////////

String StringUtils::MakeFilePath(const wchar_t* directory, const wchar_t* fileName, const wchar_t* extension)
{
	// Format file path
	String strFilePath;

	// Directory path, it may or may not be specified
	// If not specified, it means targeted file is in the same folder with executable file
	if (directory) {
		// Add directory path
		strFilePath.Append(directory);
		strFilePath.Append(Constant::Symbol::Backslash);
	}

	// File name must be specified
	if (fileName) {
		strFilePath.Append(fileName);
	}
	else {
		strFilePath.Empty();
		return strFilePath;
	}

	// File extension, it may or may not be specified
	// If not specified, it means targeted file has no extension
	if (extension) {
		// Add file extension
		strFilePath.Append(extension);
	}

	return strFilePath;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetProductVersion
//	Description:	Get app executable file product version info
//  Arguments:		isFullVersion - Full product version number (x.x.x.x) 
//								    or short version number (x.x)
//  Return value:	String - Product version string
//
//////////////////////////////////////////////////////////////////////////

String StringUtils::GetProductVersion(bool isFullVersion)
{
	// Get product file name
	String productFileName = StringUtils::MakeFilePath(NULL, FILENAME_APPEXEFILE, FILEEXT_EXEFILE);
	if (productFileName.IsEmpty()) {
		// Trace error
		TRACE_ERROR("Error: Make file path failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return Constant::String::Empty;
	}

	// Get file version info size
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(productFileName, &dwHandle);
	if (dwSize <= 0) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info size failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return Constant::String::Empty;
	}

	// Get file verision info structure
	BYTE* pVersionInfo = new BYTE[dwSize];
	if (!GetFileVersionInfo(productFileName, dwHandle, dwSize, pVersionInfo)) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return Constant::String::Empty;
	}

	// Querry version value
	UINT uLen;
	VS_FIXEDFILEINFO* lpFfi;
	if (!VerQueryValue(pVersionInfo, Constant::Symbol::Backslash, (LPVOID*)&lpFfi, &uLen)) {
		// Trace error
		TRACE_FORMAT("Error: Querry version value failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return Constant::String::Empty;
	}

	// Result product version
	String productVersion;

	// Get product version successfully
	DWORD dwProductVersionMS = lpFfi->dwProductVersionMS;
	DWORD dwProductVersionLS = lpFfi->dwProductVersionLS;
	if (isFullVersion) {
		// Get full product version number (x.x.x.x)
		productVersion.Format(_T("%d.%d.%d.%d"),
			HIWORD(dwProductVersionMS),
			LOWORD(dwProductVersionMS),
			HIWORD(dwProductVersionLS),
			LOWORD(dwProductVersionLS));
	}
	else {
		// Get short product version number (x.x)
		productVersion.Format(_T("%d.%d"),
			HIWORD(dwProductVersionMS),
			LOWORD(dwProductVersionMS));
	}

	delete[] pVersionInfo;

	return productVersion;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetProductVersion
//	Description:	Get app executable file product version info
//  Arguments:		fullVersion  - Full product version number (x.x.x.x) 
//					shortVersion - Short product version number (x.x)
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

bool StringUtils::GetProductVersion(String& fullVersion, String& shortVersion)
{
	// Get product file name
	String productFileName = StringUtils::MakeFilePath(NULL, FILENAME_APPEXEFILE, FILEEXT_EXEFILE);
	if (productFileName.IsEmpty()) {
		// Trace error
		TRACE_ERROR("Error: Make file path failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get product version info size
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(productFileName, &dwHandle);
	if (dwSize <= 0) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info size failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get product version info structure
	BYTE* pVersionInfo = new BYTE[dwSize];
	if (!GetFileVersionInfo(productFileName, dwHandle, dwSize, pVersionInfo)) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info structure failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return false;
	}

	// Querry version value
	UINT uLen;
	VS_FIXEDFILEINFO* lpFfi;
	if (!VerQueryValue(pVersionInfo, Constant::Symbol::Backslash, (LPVOID*)&lpFfi, &uLen)) {
		// Trace error
		TRACE_FORMAT("Error: Querry version value failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return false;
	}

	// Empty destination product version strings
	fullVersion.Empty();
	shortVersion.Empty();

	// Get product version successfully
	DWORD dwProductVersionMS = lpFfi->dwProductVersionMS;
	DWORD dwProductVersionLS = lpFfi->dwProductVersionLS;

	// Get full product version number (x.x.x.x)
	fullVersion.Format(_T("%d.%d.%d.%d"),
		HIWORD(dwProductVersionMS),
		LOWORD(dwProductVersionMS),
		HIWORD(dwProductVersionLS),
		LOWORD(dwProductVersionLS));

	// Get short product version number (x.x)
	shortVersion.Format(_T("%d.%d"),
		HIWORD(dwProductVersionMS),
		LOWORD(dwProductVersionMS));

	delete[] pVersionInfo;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDeviceName
//	Description:	Get the computer device name
//  Arguments:		deviceName - Device name (out)
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

bool StringUtils::GetDeviceName(String& deviceName)
{
	// Empty the output string
	deviceName.Empty();

	// Get the computer device name
	wchar_t deviceNameBuffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwNameLength = sizeof(deviceNameBuffer) / sizeof(wchar_t);
	if (!GetComputerName(deviceNameBuffer, &dwNameLength)) {
		// Trace error
		TRACE_FORMAT("Error: Get computer name failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Return the computer name
	deviceName.SetString(deviceNameBuffer);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetCurrentUserName
//	Description:	Get the current Windows user name
//  Arguments:		userName - User name (out)
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

bool StringUtils::GetCurrentUserName(String& userName)
{
	// Empty the output string
	userName.Empty();

	// Get the current user name
	wchar_t userNameBuffer[UNLEN + 1];
	DWORD dwNameLength = sizeof(userNameBuffer) / sizeof(wchar_t);
	if (!GetUserName(userNameBuffer, &dwNameLength)) {
		// Trace error
		TRACE_FORMAT("Error: Get user name failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Return the user name
	userName.SetString(userNameBuffer);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PrintCharList
//	Description:	Print list of characters of given string
//  Arguments:		srcStr	  - Given string
//					outputStr - Result string
//  Return value:	int - Number of characters
//
//////////////////////////////////////////////////////////////////////////

int StringUtils::PrintCharList(const wchar_t* srcStr, String& outputStr)
{
	// Invalid source string
	if (!srcStr)
		return INT_INVALID;

	// Get source string
	String _srcStr(srcStr);

	// Prepare output string
	outputStr.Empty();
	outputStr.Append(_T("{ "));

	// Print character list
	String replaceStr = Constant::String::Empty;
	int nSrcLength = wcslen(srcStr);
	for (int nIndex = 0; nIndex < nSrcLength; nIndex++) {
		wchar_t ch = _srcStr.At(nIndex);
		switch (ch)
		{
		case Constant::Char::Tab:
			replaceStr = _T("#TAB");
			outputStr.Append(replaceStr);
			break;
		case Constant::Char::Return:
			replaceStr = _T("#RET");
			outputStr.Append(replaceStr);
			break;
		case Constant::Char::EndLine:
			replaceStr = _T("#ENDL");
			outputStr.Append(replaceStr);
			break;
		default:
			outputStr.AppendChar(ch);
			break;
		}

		// Add separator
		if (nIndex < nSrcLength - 1) {
			outputStr.Append(_T(", "));
		}
	}

	// End result
	outputStr.Append(_T(" }"));

	return nSrcLength;
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
//  Return value:	const wchar_t* - Language name
//
//////////////////////////////////////////////////////////////////////////

const wchar_t* Language::GetLanguageName(UINT nCurLanguage, BOOL bGetDescription /* = FALSE */)
{
	// Load language table package
	LANGTABLE_PTR ptrLangTable = LoadLanguageTable(nCurLanguage);
	if (ptrLangTable == NULL)
		return Constant::Value::Unknown;

	// Get language package info
	UINT nInfoTargetID = (bGetDescription) ? LANGPACKINFO_DESCRIPTIONFULL : LANGPACKINFO_LANGNAMEID;
	const wchar_t* retInfoString = GetLanguageString(ptrLangTable, nInfoTargetID);
	if (IS_NULL_STRING(retInfoString))
		retInfoString = Constant::Value::Unknown;

	return retInfoString;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadLanguageTable
//	Description:	Load language table by specified language option
//  Arguments:		nCurLanguage   - Current language ID
//  Return value:	LANGTABLE_PTR - Language package pointer
//
//////////////////////////////////////////////////////////////////////////

LANGTABLE_PTR Language::LoadLanguageTable(UINT nCurLanguage)
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

	return ptrLangTable;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLanguageString
//	Description:	Find and return language string by ID
//  Arguments:		ptLanguage - Language package pointer
//					nID		   - Language string ID
//					pszResult  - Result string (reference-type)
//  Return value:	const wchar_t*	- Language string
//
//////////////////////////////////////////////////////////////////////////

const wchar_t* Language::GetLanguageString(LANGTABLE_PTR ptLanguage, UINT nID)
{
	// Return NULL string if language table is empty
	if ((ptLanguage == NULL) || (ptLanguage->empty()))
		return Constant::String::Null;

	// Find and return corresponding language string paired with specified ID
	for (int nIndex = 0; nIndex < ptLanguage->size(); nIndex++) {
		LANGTEXT langText = ptLanguage->at(nIndex);

		if (langText.id == nID)
			return langText.langString;
	}

	return Constant::String::Null;
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
	String actionInfoString;
	if (nActionType == APP_ACTIONTYPE_MONITOR) {
		actionInfoString.SetString(_T("Turn off display"));
		dwErrCode = ERROR_SUCCESS;
	}
	else if (nActionType == APP_ACTIONTYPE_POWER && 
			(nMessage != APP_MESSAGE_SLEEP && nMessage != APP_MESSAGE_HIBERNATE)) {
		switch (nMessage)
		{
		case APP_MESSAGE_SHUTDOWN:
			// Shutdown
			actionInfoString.SetString(_T("Shutdown"));
			dwErrCode = ERROR_SUCCESS;
			break;

		case APP_MESSAGE_REBOOT:
			// Restart
			actionInfoString.SetString(_T("Restart"));
			dwErrCode = ERROR_SUCCESS;
			break;

		case APP_MESSAGE_SIGNOUT:
			// Sign out
			actionInfoString.SetString(_T("Sign out"));
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
			actionInfoString.SetString(_T("Sleep"));
			dwErrCode = ERROR_SUCCESS;
			break;

		case APP_MESSAGE_HIBERNATE:
			// Hibernate
			actionInfoString.SetString(_T("Hibernate"));
			dwErrCode = ERROR_SUCCESS;
			break;
		}
	}
	else {
		// Wrong argument
		actionInfoString.SetString(_T("Invalid"));
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
	}

	// Time format
	String timeFormatStr;
	const wchar_t* timePeriod = (sysExecTime.wHour < 12) ? Constant::Symbol::AnteMeridiem : Constant::Symbol::PostMeridiem;
	String templateFormatStr = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	timeFormatStr.Format(templateFormatStr, sysExecTime.wYear,
											sysExecTime.wMonth,
											sysExecTime.wDay,
											sysExecTime.wHour,
											sysExecTime.wMinute,
											sysExecTime.wSecond,
											sysExecTime.wMilliseconds,
											timePeriod);

	// Message format
	String messageFormatStr;
	messageFormatStr.Format(_T("[ExecutePowerAction]\nAction: %s\nTime: %s"), actionInfoString.GetString(), timeFormatStr.GetString());

	// Show dummy test message
	HWND hWnd = GET_HANDLE_MAINWND();
	MessageBox(hWnd, messageFormatStr, _T("DummyTest"), MB_OK | MB_ICONINFORMATION);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceError
//	Description:	Output exception/error trace log string to log file
//  Arguments:		traceLogA - Output trace log string (ANSI)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::TraceError(const char* traceLogA)
{
	// Convert ANSI string to UNICODE
	const wchar_t* traceLogW = MAKEUNICODE(traceLogA);
	TraceError(traceLogW);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceError
//	Description:	Output exception/error trace log string to log file
//  Arguments:		traceLogW - Output trace log string (Unicode)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::TraceError(const wchar_t* traceLogW)
{
	// Write trace log file: TraceError.log
	WriteTraceErrorLogFile(traceLogW);
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

void AppCore::TraceErrorFormat(const char* traceLogFormatA, ...)
{
	ATLASSERT(AtlIsValidString(traceLogFormatA));

	// Format source string (ANSI)
	CStringA strLogFormatA;

	va_list argList;
	va_start(argList, traceLogFormatA);
	strLogFormatA.FormatV(traceLogFormatA, argList);
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

void AppCore::TraceErrorFormat(const wchar_t* traceLogFormatW, ...)
{
	ATLASSERT(AtlIsValidString(traceLogFormatW));

	// Format source string (Unicode)
	String logFormatStringW;

	va_list argList;
	va_start(argList, traceLogFormatW);
	logFormatStringW.FormatV(traceLogFormatW, argList);
	va_end(argList);

	// Output trace log
	TraceError(logFormatStringW);
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

void AppCore::TraceDebugInfo(const char* funcName, const char* fileName, int lineIndex)
{
	// Debug trace info
	const wchar_t* _funcName = MAKEUNICODE(funcName);
	const wchar_t* _fileName = MAKEUNICODE(fileName);

	// Format debug trace log
	String debugTraceFormat = StringUtils::StringFormat(_T("Function: %s, File: %s(%d)"), _funcName, _fileName, lineIndex);

	// Write debug trace log: TraceDebug.log
	WriteTraceDebugLogFile(debugTraceFormat.GetString());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDebugLog
//	Description:	Output debug log string
//  Arguments:		debugLog	- Debug log string (Unicode)
//					forceOutput - Force output target
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::OutputDebugLog(const wchar_t* debugLog, int forceOutput /* = INT_INVALID */)
{
	// Get debug mode enable state
	BOOL bDebugModeEnable = GetDebugMode();

	// Get debug log string
	String debugLogStr = debugLog;

	// Get DebugTest tool dialog handle
	HWND hDebugTestWnd = FindDebugTestDlg();

	// Debug log output target
	int nDebugOutputTarget = forceOutput;
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
		OutputDebugString(debugLogStr);
	}
	else if (nDebugOutputTarget == DebugInfoFile) {
		// Ouput debug log to file: DebugInfo.log
		WriteDebugInfoLogFile(debugLogStr);
	}
	else if (nDebugOutputTarget == DebugTestTool) {
		// Output debug log to DebugTest tool
		if (hDebugTestWnd == NULL) return;
		WPARAM wParam = MAKE_WPARAM_STRING(debugLogStr);
		LPARAM lParam = MAKE_LPARAM_STRING(debugLogStr);
		SendMessage(hDebugTestWnd, SM_APP_DEBUG_OUTPUT, wParam, lParam);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDebugLogFormat
//	Description:	Output debug log string format
//  Arguments:		debugLogFormat - Debug log format string (Unicode)
//					...			   - Same as default MFC Format function
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void AppCore::OutputDebugLogFormat(const wchar_t* debugLogFormat, ...)
{
	ATLASSERT(AtlIsValidString(debugLogFormat));

	// Format source string
	String logFormatString;

	va_list argList;
	va_start(argList, debugLogFormat);
	logFormatString.FormatV(debugLogFormat, argList);
	va_end(argList);

	// Output debug string
	OutputDebugLog(logFormatString);
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

void AppCore::OutputDebugStringFormat(const wchar_t* debugStringFormat, ...)
{
	ATLASSERT(AtlIsValidString(debugStringFormat));

	// Format source string
	String logDebugStringFormat;

	va_list argList;
	va_start(argList, debugStringFormat);
	logDebugStringFormat.FormatV(debugStringFormat, argList);
	va_end(argList);

	// Output debug string
	OutputDebugString(logDebugStringFormat);
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
	String strFolderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);

	// Log file path
	String strFilePath = StringUtils::MakeFilePath(strFolderPath, FILENAME_TRACE_ERROR_LOG, FILEEXT_LOGFILE);

	// If the log file is not being opened
	while (pTraceErrorLogFile->m_hFile == CFile::hFileNull) {

		// Open the log file
		if (!pTraceErrorLogFile->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
			// Show error message
			DWORD dwErrorCode = GetLastError();
			AppCore::ShowErrorMessage(NULL, NULL, dwErrorCode);
			return FALSE;
		}

		// Go to end of file
		ULONGLONG ullFileSize = pTraceErrorLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			pTraceErrorLogFile->Close();

			// Step2: Rename file extension to BAK
			if (!AppCore::BackupOldLogFile(strFilePath, FILENAME_TRACE_ERROR_LOG))
				return FALSE;

			// Step3: Create new file and reopen
			continue;
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
	String strFolderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);

	// Log file path
	String strFilePath = StringUtils::MakeFilePath(strFolderPath, FILENAME_TRACE_DEBUG_LOG, FILEEXT_LOGFILE);

	// If the log file is not being opened
	while (pTraceDebugLogFile->m_hFile == CFile::hFileNull) {

		// Open the log file
		if (!pTraceDebugLogFile->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
			// Show error message
			DWORD dwErrorCode = GetLastError();
			AppCore::ShowErrorMessage(NULL, NULL, dwErrorCode);
			return FALSE;
		}

		// Go to end of file
		ULONGLONG ullFileSize = pTraceDebugLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			pTraceDebugLogFile->Close();

			// Step2: Rename file extension to BAK
			if (!AppCore::BackupOldLogFile(strFilePath, FILENAME_TRACE_DEBUG_LOG))
				return FALSE;

			// Step3: Create new file and reopen
			continue;
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
	String strFolderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);

	// Log file path
	String strFilePath = StringUtils::MakeFilePath(strFolderPath, FILENAME_DEBUG_INFO_LOG, FILEEXT_LOGFILE);

	// If the log file is not being opened
	while (pDebugInfoLogFile->m_hFile == CFile::hFileNull) {

		// Open the log file
		if (!pDebugInfoLogFile->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
			// Show error message
			DWORD dwErrorCode = GetLastError();
			AppCore::ShowErrorMessage(NULL, NULL, dwErrorCode);
			return FALSE;
		}

		// Go to end of file
		ULONGLONG ullFileSize = pDebugInfoLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			pDebugInfoLogFile->Close();

			// Step2: Rename file extension to BAK
			if (!AppCore::BackupOldLogFile(strFilePath, FILENAME_DEBUG_INFO_LOG))
				return FALSE;

			// Step3: Create new file and reopen
			continue;
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
//  Arguments:		filePath	- File path (in/out)
//					logFileName	- Log file name
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::BackupOldLogFile(String& filePath, const wchar_t* logFileName)
{
	CFileFind Finder;

	// If file path is not specified, do nothing
	if (filePath.IsEmpty()) return FALSE;

	// Log folder path
	String folderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);

	// Search for backup file list
	for (int nNum = 0; nNum < Constant::Max::BackupFileNumber; nNum++) {
		
		// Make backup file path template
		String filePathTemp = StringUtils::MakeFilePath(folderPath, logFileName, FILEEXT_BAKLOGFILE);
		if (filePathTemp.IsEmpty())
			return FALSE;

		// Format backup file path
		String bakFilePath;
		bakFilePath.Format(filePathTemp, nNum);

		// Check if file has already existed
		if (Finder.FindFile(bakFilePath) == TRUE) {

			// If backup file number exceeded the limit, can not backup more
			if (nNum == (Constant::Max::BackupFileNumber - 1)) return FALSE;
			else continue;
		}

		// Rename file
		CFile::Rename(filePath, bakFilePath);
		break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteTraceErrorLogFile
//	Description:	Write trace error log string to file
//  Arguments:		logStringW	- Log string
//  Return value:	None
//	Notes:			TraceError.log
//					File to output trace error detail log strings
//
//////////////////////////////////////////////////////////////////////////

void AppCore::WriteTraceErrorLogFile(const wchar_t* logStringW)
{
	// Get current time up to milisecs
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);

	// Format log date/time
	const wchar_t* middayFlag = (stTime.wHour >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::StringFormat(templateFormat, stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, middayFlag);

	// Format output log string
	templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::StringFormat(templateFormat, timeFormatString.GetString(), logStringW, Constant::String::Empty);

	// If output log string is empty, do nothing
	if (logOutputFormatString.IsEmpty())
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
		pTraceErrorLogFile->Write(logOutputFormatString, logOutputFormatString.GetLength() * sizeof(wchar_t));
		pTraceErrorLogFile->Flush();
	}

	// Re-check file size after writing
	{
		// Go to end of file
		ULONGLONG ullFileSize = pTraceErrorLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			pTraceErrorLogFile->Close();

			// Step2: Rename file extension to BAK
			String strFolderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);
			String strOrgFilePath = StringUtils::MakeFilePath(strFolderPath.GetString(), FILENAME_TRACE_ERROR_LOG, FILEEXT_LOGFILE);
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
//  Arguments:		logStringW	- Log string
//  Return value:	None
//	Notes:			TraceDebug.log
//					File to output trace debug log strings (including 
//					the function name, code file and line where it failed)
//
//////////////////////////////////////////////////////////////////////////

void AppCore::WriteTraceDebugLogFile(const wchar_t* logStringW)
{
	// Get current time up to milisecs
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);

	// Format log date/time
	const wchar_t* middayFlag = (stTime.wHour >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::StringFormat(templateFormat, stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, middayFlag);

	// Format output log string
	templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::StringFormat(templateFormat, timeFormatString.GetString(), logStringW, Constant::String::Empty);

	// If output log string is empty, do nothing
	if (logOutputFormatString.IsEmpty()) return;

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
		pTraceDebugLogFile->Write(logOutputFormatString, logOutputFormatString.GetLength() * sizeof(wchar_t));
		pTraceDebugLogFile->Flush();
	}

	// Re-check file size after writing
	{
		// Go to end of file
		ULONGLONG ullFileSize = pTraceDebugLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			pTraceDebugLogFile->Close();

			// Step2: Rename file extension to BAK
			String strFolderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);
			String strOrgFilePath = StringUtils::MakeFilePath(strFolderPath, FILENAME_TRACE_DEBUG_LOG, FILEEXT_LOGFILE);
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

void AppCore::WriteDebugInfoLogFile(const wchar_t* logStringW)
{
	// Get current time up to milisecs
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);

	// Format log date/time
	const wchar_t* middayFlag = (stTime.wHour >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::StringFormat(templateFormat, stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, middayFlag);

	// Format output log string
	templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::StringFormat(templateFormat, timeFormatString.GetString(), logStringW, Constant::String::Empty);

	// If output log string is empty, do nothing
	if (logOutputFormatString.IsEmpty()) return;

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
		pDebugInfoLogFile->Write(logOutputFormatString, logOutputFormatString.GetLength() * sizeof(wchar_t));
		pDebugInfoLogFile->Flush();
	}

	// Recheck file size after writing
	{
		// Go to end of file
		ULONGLONG ullFileSize = pDebugInfoLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			pDebugInfoLogFile->Close();

			// Step2: Rename file extension to BAK
			String strFolderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);
			String strOrgFilePath = StringUtils::MakeFilePath(strFolderPath, FILENAME_DEBUG_INFO_LOG, FILEEXT_LOGFILE);
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

void AppCore::WriteTraceNDebugLogFileBase(const wchar_t* fileName, const wchar_t* logStringW)
{
	// Log file path
	String filePath = StringUtils::MakeFilePath(SUBFOLDER_LOG, fileName, FILEEXT_LOGFILE);

	CFile fTrcDbgLogFile;

	// Check if file is opening, if not, open it
	while (fTrcDbgLogFile.m_hFile == CFile::hFileNull) {

		BOOL bResult = fTrcDbgLogFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == FALSE) {
			// Show error message
			DWORD dwErrorCode = GetLastError();
			LPARAM lParam = (LPARAM)fileName;
			ShowErrorMessage(NULL, NULL, dwErrorCode, lParam);
			return;
		}

		// Go to end of file
		ULONGLONG ullFileSize = fTrcDbgLogFile.SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			fTrcDbgLogFile.Close();
			
			// Step2: Rename file extension to BAK
			CFileFind Finder;
			String backupFilePath;
			for (int nNum = 0; nNum < Constant::Max::BackupFileNumber; nNum++) {
				backupFilePath.Format((filePath + FILEEXT_BAKLOGFILE), nNum);
				if (Finder.FindFile(backupFilePath) == TRUE) {
					if (nNum == (Constant::Max::BackupFileNumber - 1)) return;
					else continue;
				}
				CFile::Rename(filePath, backupFilePath);
				break;
			}

			// Step3: Create new file and reopen
			continue;
		}
	}

	// Get current time up to milisecs
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);

	// Format log date/time
	const wchar_t* middayFlag = (stTime.wHour >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::StringFormat(templateFormat, stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, middayFlag);

	// Format output log string
	templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::StringFormat(templateFormat, timeFormatString.GetString(), logStringW, Constant::String::Empty);

	if (!logOutputFormatString.IsEmpty()) {
		// Write log string to file
		fTrcDbgLogFile.Write(logOutputFormatString, logOutputFormatString.GetLength() * sizeof(wchar_t));
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
	LRESULT lResult = Result::Success;

	// Get begin timestamp (for timeout counter)
	ULONGLONG ullBeginTimestamp = GetTickCount64();

	// Wait for message
	while (1) {
		MSG msg = STRUCT_ZERO;
		if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {

			// If received specified message, break the loop
			if (msg.message == nMsg) {

				// Success
				lResult = Result::Success;
				break;
			}
		}
		// Check for timeout
		if (GetTickCount64() - ullBeginTimestamp >= nTimeout) {

			// Timeout --> Failed
			lResult = Result::Failure;
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
	String errorMessage = GetLanguageString(pAppLang, nErrMsgID);
	const wchar_t* errorCaption = GetLanguageString(pAppLang, MSGBOX_ERROR_CAPTION);

	// In case of unknown error, attach the error code
	if (nErrMsgID == MSGBOX_ERROR_UNKNOWN) {
		String tempStr;
		tempStr.Format(errorMessage, dwErrorCode);
		errorMessage = tempStr;
	}

	// Get attached param
	String descriptionStr = Constant::String::Null;
	if (lParam != NULL) {
		// Convert to description string
		descriptionStr = LPARAM_TO_STRING(lParam);
	}

	// Attach additional description if available
	if (IS_NOT_NULL_STRING(descriptionStr)) {
		errorMessage.Append(Constant::String::NewLine);
		errorMessage.Append(descriptionStr);
	}

	// Show error message
	MessageBox(hMsgOwnerWnd, errorMessage, errorCaption, MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);

	// Notify application class about error message showing
	WPARAM wAppMsgParam = (WPARAM)dwErrorCode;
	LPARAM lAppMsgParam = MAKE_LPARAM_STRING(errorMessage);
	PostMessage(NULL, SM_APP_SHOW_ERROR_MSG, wAppMsgParam, lAppMsgParam);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Text2Time
//	Description:	Convert editbox input text into valid time value
//  Arguments:		stTime  - Return time data (ref-value)
//					text	- Input text
//  Return value:	BOOL - Result of converting process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::Text2Time(SYSTEMTIME& stTime, const wchar_t* text)
{
	// Check input text validity
	int nLength = wcslen(text);
	if ((nLength == 0) || (nLength > 4))
		return FALSE;

	String timeString(text);

	int nHour = INT_INVALID;
	int nMinute = INT_INVALID;

	// Break the time value into combinations of digits
	int nLeft1Digit = _tstoi(timeString.Left(1));
	int nLeft2Digits = _tstoi(timeString.Left(2));
	int	nRight1Digit = _tstoi(timeString.Right(1));
	int nRight2Digits = _tstoi(timeString.Right(2));

	// Convert
	switch (nLength)
	{
	case 1:
		// Ex: 3 -> 03:00, 9 -> 09:00, ...
		nHour = _tstoi(timeString);			// The given time value will be the hour value
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
			nHour = _tstoi(timeString);				// All digits will be the hour value
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
//					text	- Input text
//  Return value:	BOOL - Result of converting process
//	Note:			Old/base function (no longer used)
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::Text2TimeBase(SYSTEMTIME& stTime, const wchar_t* text)
{
	// Check input text validity
	int nLength = wcslen(text);
	if ((nLength == 0) || (nLength > 4))
		return FALSE;

	// Get input text length
	UINT nTime = _tstoi(text);
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
	if (nPos < Constant::Min::TimeSpin)
		nPos = Constant::Min::TimeSpin;
	else if (nPos > Constant::Max::TimeSpin)
		nPos = Constant::Max::TimeSpin;

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
	if (nPos < Constant::Min::TimeSpin)
		nPos = Constant::Min::TimeSpin;
	else if (nPos > Constant::Max::TimeSpin)
		nPos = Constant::Max::TimeSpin;
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
	String debugDlgTitle = StringUtils::LoadResourceString(IDS_APP_DEBUGTESTDLG_TITLE);
	if (debugDlgTitle.IsEmpty()) return NULL;
	return ::FindWindow(NULL, debugDlgTitle);
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
	// Check control validity
	if (pGridCtrl == NULL) return;

	// Set base style
	CGridCellBase* pHeaderCell = (CGridCellBase*)pGridCtrl->GetCell(nRow, nCol);
	if (pHeaderCell == NULL) return;
	pHeaderCell->SetFormat(pHeaderCell->GetFormat() | DT_CENTER);
	pHeaderCell->SetMargin(0);
	pHeaderCell->SetBackClr(Color::Gray);
	pHeaderCell->SetTextClr(Color::Black);
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

void AppCore::DrawGridTableRow(CGridCtrl* pGridCtrl, int nRow, int /*nRowNum*/, int nColNum, GRIDCTRLCOLFORMAT* apGrdColFormat)
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
//  Return value:	String	- Format time string
//
//////////////////////////////////////////////////////////////////////////

String	AppCore::FormatDispTime(LANGTABLE_PTR pLang, UINT nFormatID, SYSTEMTIME timeVal)
{
	// Load format string
	String formatString = StringUtils::LoadResourceString(nFormatID);
	return FormatDispTime(pLang, formatString, timeVal);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatDispTime
//	Description:	Format time value for displaying
//  Arguments:		pLang			 - Language table pointer
//					lpszFormatString - Format string
//					timeVal			 - Given time value
//  Return value:	String	- Format time string
//
//////////////////////////////////////////////////////////////////////////

String	AppCore::FormatDispTime(LANGTABLE_PTR pLang, const wchar_t* formatString, SYSTEMTIME timeVal)
{
	// Format time string
	UINT nTimePeriod = (timeVal.wHour < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	const wchar_t* timePeriodFormat = Language::GetLanguageString(pLang, nTimePeriod);
	WORD wHour = (timeVal.wHour > 12) ? (timeVal.wHour - 12) : timeVal.wHour;
	WORD wMinute = timeVal.wMinute;

	String resultString = StringUtils::StringFormat(formatString, wHour, wMinute, timePeriodFormat);
	return resultString;
}

/////////////////////////////////////////////////////////////////////////////
// Additional functions

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
//	Function name:	AddRegistryKey
//	Description:	Add new key into registry
//  Arguments:		regInfo - Registry key info data
//  Return value:	BOOL - Registry processing result
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::AddRegistryKey(const RegistryInfo& regInfo)
{
	CRegKey regKey;
	RegistryKey regKeyInfo = regInfo.GetRegistryKey();
	LONG lResult = regKey.Open(regInfo.GetRootKey(), regKeyInfo.GetName(), KEY_ALL_ACCESS);
	if (lResult != ERROR_SUCCESS) {
		// Create register key if not found
		if (lResult == ERROR_NOT_FOUND) {
			regKey.Create(regInfo.GetRootKey(), regKeyInfo.GetName(), REG_NONE,
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
//  Return value:	const wchar_t* - Registry path
//
//////////////////////////////////////////////////////////////////////////

const wchar_t* AppCore::MakeRegistryPath(const RegistryInfo& regInfo, UINT nRegPathType /* = RegistryPathType::fullPath */, BOOL bIncRootKey /* = TRUE */)
{
	// Check root key info validity
	if (bIncRootKey != FALSE) {
		String rootKeyName = regInfo.GetRootKeyName();
		if ((regInfo.GetRootKey() == NULL) && (rootKeyName.IsEmpty())) {
			// Trace error
			TRACE_ERROR("Error: Make registry path failed, rootkey name is invalid!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return Constant::String::Null;
		}
	}

	// Check sub-key path validity
	if ((nRegPathType == RegistryPathType::fullPath) || (nRegPathType >= RegistryPathType::includingSubKeyPath)) {
		StringArray astrSubkeyPath;
		regInfo.GetSubkeyPath(astrSubkeyPath);
		if (astrSubkeyPath.empty()) {
			// Trace error
			TRACE_ERROR("Error: Make registry path failed, subkey info is invalid!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return Constant::String::Null;
		}
	}

	// Check profile key validity
	if ((nRegPathType == RegistryPathType::fullPath) || (nRegPathType >= RegistryPathType::includingCompanyName)) {
		String companyNameStr = regInfo.GetCompanyName();
		if (companyNameStr.IsEmpty()) {
			// Trace error
			TRACE_ERROR("Error: Make registry path failed, profile key name is invalid!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return Constant::String::Null;
		}
	}

	// Check app name validity
	if ((nRegPathType == RegistryPathType::fullPath) || (nRegPathType >= RegistryPathType::includingProductName)) {
		String productNameStr = regInfo.GetProductName();
		if (productNameStr.IsEmpty()) {
			// Trace error
			TRACE_ERROR("Error: Make registry path failed, application name is invalid!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return Constant::String::Null;
		}
	}

	// Check section name array validity
	if ((nRegPathType == RegistryPathType::fullPath) || (nRegPathType >= RegistryPathType::includingSectionName)) {
		StringArray astrSectionArray;
		regInfo.GetSubkeyPath(astrSectionArray);
		if (astrSectionArray.empty()) {
			// Trace error
			TRACE_ERROR("Error: Make registry path failed, section name is invalid!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return Constant::String::Null;
		}
	}

	// Root key (string)
	String rootKeyName;
	String rootKeyNameTemp = regInfo.GetRootKeyName();
	if (!rootKeyNameTemp.IsEmpty()) {
		rootKeyName = rootKeyNameTemp;
	}

	// Sub-key path
	String subKeyPath;
	StringArray astrSubkeyPath;
	regInfo.GetSubkeyPath(astrSubkeyPath);
	for (int nIndex = 0; nIndex < astrSubkeyPath.size(); nIndex++) {
		if (nIndex > 0) {
			subKeyPath.Append(Constant::Symbol::Backslash);
		}
		subKeyPath.Append(astrSubkeyPath.at(nIndex));
	}

	// Company name
	String companyName;
	String companyNameTemp = regInfo.GetCompanyName();
	if (!companyNameTemp.IsEmpty()) {
		companyName = companyNameTemp;
	}

	// Product name
	String productName;
	String productNameTemp = regInfo.GetProductName();
	if (!productNameTemp.IsEmpty()) {
		productName = productNameTemp;
	}

	// Section name
	String sectionName;
	StringArray astrSectionArray;
	regInfo.GetSectionName(astrSectionArray);
	for (int nIndex = 0; nIndex < astrSectionArray.size(); nIndex++) {
		if (nIndex > 0) {
			sectionName.Append(Constant::Symbol::Backslash);
		}
		sectionName.Append(astrSectionArray.at(nIndex));
	}

	// Key name
	String keyName;
	RegistryKey regKeyInfo = regInfo.GetRegistryKey();
	String keyNameTemp = regKeyInfo.GetName();
	if (!keyNameTemp.IsEmpty()) {
		keyName = keyNameTemp;
	}

	// Result string
	String resultRegistryPath;

	// Make registry path by type
	int nRetFailedFlag = FLAG_OFF;
	if (!rootKeyName.IsEmpty()) {

		// Initialize and include root key name
		resultRegistryPath.Empty();
		if (bIncRootKey != FALSE) {
			resultRegistryPath.Append(rootKeyName);
		}

		if ((nRegPathType == RegistryPathType::fullPath) || (nRegPathType >= RegistryPathType::includingSubKeyPath)) {
			if ((nRetFailedFlag != FLAG_ON) && (!subKeyPath.IsEmpty())) {
				// Include separator character if rootkey is included
				if (bIncRootKey != FALSE) {
					resultRegistryPath.Append(Constant::Symbol::Backslash);
				}
				// Include sub-key path
				resultRegistryPath.Append(subKeyPath);
			}
			else {
				// Turn failed flag ON
				nRetFailedFlag = FLAG_ON;
			}
		}

		if ((nRegPathType == RegistryPathType::fullPath) || (nRegPathType >= RegistryPathType::includingCompanyName)) {
			if ((nRetFailedFlag != FLAG_ON) && (!companyName.IsEmpty())) {
				// Include profile key name
				resultRegistryPath.Append(Constant::Symbol::Backslash);
				resultRegistryPath.Append(companyName);
			}
			else {
				// Turn failed flag ON
				nRetFailedFlag = FLAG_ON;
			}
		}

		if ((nRegPathType == RegistryPathType::fullPath) || (nRegPathType >= RegistryPathType::includingProductName)) {
			if ((nRetFailedFlag != FLAG_ON) && (!productName.IsEmpty())) {
				// Include application name
				resultRegistryPath.Append(Constant::Symbol::Backslash);
				resultRegistryPath.Append(productName);
			}
			else {
				// Turn failed flag ON
				nRetFailedFlag = FLAG_ON;
			}
		}

		if ((nRegPathType == RegistryPathType::fullPath) || (nRegPathType >= RegistryPathType::includingSectionName)) {
			if ((nRetFailedFlag != FLAG_ON) && (!sectionName.IsEmpty())) {
				// Include section name
				resultRegistryPath.Append(Constant::Symbol::Backslash);
				resultRegistryPath.Append(sectionName);
			}
			else {
				// Turn failed flag ON
				nRetFailedFlag = FLAG_ON;
			}
		}

		if ((nRegPathType == RegistryPathType::fullPath) || (nRegPathType >= RegistryPathType::includingKeyName)) {
			if ((nRetFailedFlag != FLAG_ON) && (!keyName.IsEmpty())) {
				// Include key name
				resultRegistryPath.Append(Constant::Symbol::Backslash);
				resultRegistryPath.Append(keyName);
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
		resultRegistryPath.Empty();
	}

	return resultRegistryPath;
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
//  Arguments:		eFileType - File type
//					filePath  - Path of file
//  Return value:	BOOL - Result of file opening process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::FileViewStd(FILETYPE eFileType, const wchar_t* filePath)
{
	String appPath = Constant::String::Empty;

	switch (eFileType) 
	{
	case FILETYPE_TEXT:
		appPath = PATH_APP_NOTEPAD;
		break;
	case FILETYPE_IMAGE:
		break;
	default:
		return FALSE;
	}

	// Run a file viewer instance
	HWND hWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	HINSTANCE hInstance = ShellExecute(hWnd, Constant::Command::Open, appPath, filePath, NULL, SW_SHOW);
	return (hInstance != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OpenWebURL
//	Description:	Open web URL using default web browser
//  Arguments:		webUrl - String of web URL
//  Return value:	BOOL - Result of web URL opening process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::OpenWebURL(const wchar_t* webUrl)
{
	// Run a web browser instance
	HINSTANCE hInstance = ShellExecute(0, 0, webUrl, NULL, NULL, SW_NORMAL);
	return (hInstance != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RunApp
//	Description:	Run an application by specified path
//  Arguments:		appPath		- Path of excutive file
//					bRunAsAdmin	- Run as admin flag
//					bShowFlag	- Show window flag
//  Return value:	LRESULT - Result of app launching process
//
//////////////////////////////////////////////////////////////////////////

LRESULT AppCore::RunApp(const wchar_t* appPath, BOOL bRunAsAdmin /* = FALSE */, BOOL bShowFlag /* = TRUE */)
{
	// Param set
	String runAsFlag = (bRunAsAdmin) ? Constant::Command::RunAs : Constant::Command::Open;
	int nShowFlag = (bShowFlag) ? SW_SHOW : SW_HIDE;

	// Run an executable instance
	HINSTANCE hInstance = ShellExecute(NULL, runAsFlag, appPath, 0, 0, nShowFlag);
	return (LRESULT)(hInstance != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ExecuteCommand
//	Description:	Execute CMD command
//  Arguments:		commandString - Command string
//					bRunAsAdmin	  - Run as admin flag
//					bShowFlag	  - Show window flag
//  Return value:	LRESULT - Result of command execution process
//
//////////////////////////////////////////////////////////////////////////

LRESULT AppCore::ExecuteCommand(const wchar_t* commandString, BOOL bRunAsAdmin /* = TRUE */, BOOL bShowFlag /* = TRUE */)
{
	// Format input command
	String commandFormat = StringUtils::StringFormat(_T("/C %s"), commandString);

	// Flag param set
	String strRunAsFlag = (bRunAsAdmin) ? Constant::Command::RunAs : Constant::Command::Open;
	int nShowFlag = (bShowFlag) ? SW_SHOW : SW_HIDE;

	// Excute command
	HINSTANCE hInstance = ShellExecute(NULL, strRunAsFlag, PATH_APP_SYSTEMCMD, commandFormat, 0, nShowFlag);
	return (LRESULT)(hInstance != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CreateAppProcess
//	Description:	Create app process
//  Arguments:		appPath		 - App executable file path
//					commandLine	 - Command line
//					nStyle		 - App process style
//					dwErrorCode	 - Returned error code
//  Return value:	BOOL
//
//////////////////////////////////////////////////////////////////////////

BOOL AppCore::CreateAppProcess(const wchar_t* appPath, wchar_t* commandLine, UINT nStyle, DWORD& dwErrorCode)
{
	// Startup info
	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO);

	// Process info
	PROCESS_INFORMATION ProcessInfo;
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));

	// Create process
	BOOL bResult = CreateProcess(appPath, commandLine, (LPSECURITY_ATTRIBUTES)NULL,
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

void AppCore::DrawButton(CButton*& pBtn, UINT nIconID, const wchar_t* buttonTitle /* = Constant::String::Empty */)
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
	String buttonTitleString;
	buttonTitleString.SetString(buttonTitle);
	if (!buttonTitleString.IsEmpty()) {
		wchar_t _tempBuffer[Constant::Max::StringLength] = {0};
		pBtn->GetWindowText(_tempBuffer, Constant::Max::StringLength);
		buttonTitleString.SetString(_tempBuffer);
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

BOOL CALLBACK EnumFontFamiliesExProc(ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* /*lpntme*/, DWORD /*FontType*/, LPARAM lParam) 
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

bool AppCore::EnumFontNames(std::vector<std::wstring>& fontNames)
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
//  Arguments:		fontName - Input font name
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

bool AppCore::ValidateFontName(const wchar_t* fontName)
{
	// Array to get returned font names
	std::vector<std::wstring> fontNames;

	// Enumerate all currently available fonts
	bool bRet = EnumFontNames(fontNames);
	if (!bRet) {
		// Trace error
		TRACE_ERROR("Error: Enumerate fonts failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Find for input font name within the acquired font families
	bRet = (std::find(fontNames.begin(), fontNames.end(), fontName) != fontNames.end());
	if (bRet)
		return bRet;

	// For easier comparison, convert all to lowercase
	std::wstring lowerInput(fontName);
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

