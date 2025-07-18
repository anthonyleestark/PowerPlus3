﻿/**
 * @file		AppCore.cpp
 * @brief		Implement core methods which will be used elsewhere in program
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "AppCore/AppCore.h"
#include "AppCore/Global.h"
#include "AppCore/MapTable.h"
#include "AppCore/Logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/**
 * @brief	Constructor
 */
ConfigData::ConfigData()
{
	// Main settings
	this->nLMBAction = APP_ACTION_DISPLAYOFF;							// Left mouse button action
	this->nMMBAction = APP_ACTION_SLEEP;								// Middle mouse button action
	this->nRMBAction = APP_ACTION_SHOWMENU;								// Right mouse button action
	this->bRMBShowMenu = true;											// Right mouse button: Only show menu

	// Display setting
	this->nLanguageID = APP_LANGUAGE_ENGLISH;							// Language setting

	// System settings
	this->bShowDlgAtStartup = true;										// Show dialog at startup
	this->bStartupEnabled = true;										// Startup with Windows
	this->bConfirmAction = true;										// Show confirm message before doing action
	this->bSaveHistoryLog = false;										// Save app history log
	this->bSaveAppEventLog = true;										// Save app event log
	this->bRunAsAdmin = false;											// Run with admin privileges
	this->bShowErrorMsg = true;											// Show action error message
	this->bNotifySchedule = true;										// Show notify tip for schedule action
	this->bAllowCancelSchedule = false;									// Allow canceling schedule when notify
	this->bEnableBackgroundHotkey = false;								// Enable background action hotkeys
	this->bLockStateHotkey = true;										// Allow background hotkeys on lockscreen
	this->bEnablePowerReminder = true;									// Enable Power Peminder feature
}


/**
 * @brief	Copy data from another config data
 * @param	other - Pointer of input data
 * @return	None
 */
void ConfigData::Copy(const ConfigData& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Main settings
	this->nLMBAction = other.nLMBAction;								// Left mouse button action
	this->nMMBAction = other.nMMBAction;								// Middle mouse button action
	this->nRMBAction = other.nRMBAction;								// Right mouse button action
	this->bRMBShowMenu = other.bRMBShowMenu;							// Right mouse button: Only show menu

	// Display setting
	this->nLanguageID = other.nLanguageID;								// Language setting

	// System advanced settings
	this->bShowDlgAtStartup = other.bShowDlgAtStartup;					// Show dialog at startup
	this->bStartupEnabled = other.bStartupEnabled;						// Startup with Windows
	this->bConfirmAction = other.bConfirmAction;						// Show confirm message before doing action
	this->bSaveHistoryLog = other.bSaveHistoryLog;						// Save app history log
	this->bSaveAppEventLog = other.bSaveAppEventLog;					// Save app event log
	this->bRunAsAdmin = other.bRunAsAdmin;								// Run with admin privileges
	this->bShowErrorMsg = other.bShowErrorMsg;							// Show action error message
	this->bNotifySchedule = other.bNotifySchedule;						// Show notify tip for schedule action
	this->bAllowCancelSchedule = other.bAllowCancelSchedule;			// Allow canceling schedule when notify
	this->bEnableBackgroundHotkey = other.bEnableBackgroundHotkey;		// Enable background action hotkeys
	this->bLockStateHotkey = other.bLockStateHotkey;					// Allow background hotkeys on lockscreen
	this->bEnablePowerReminder = other.bEnablePowerReminder;			// Enable Power Peminder feature
}


/**
 * @brief	Compare with another given data
 * @param	other - Pointer of given data
 * @return	true/false
 */
constexpr bool ConfigData::Compare(const ConfigData& other) const noexcept
{
	bool bRet = true;

	// Compare Main settings
	bRet &= (this->nLMBAction == other.nLMBAction);								// Left mouse button action
	bRet &= (this->nMMBAction == other.nMMBAction);								// Middle mouse button action
	bRet &= (this->nRMBAction == other.nRMBAction);								// Right mouse button action
	bRet &= (this->bRMBShowMenu == other.bRMBShowMenu);							// Right mouse button: Only show menu

	// Compare Display setting
	bRet &= (this->nLanguageID == other.nLanguageID);							// Language setting

	// Compare System advanced settings
	bRet &= (this->bShowDlgAtStartup == other.bShowDlgAtStartup);				// Show dialog at startup
	bRet &= (this->bStartupEnabled == other.bStartupEnabled);					// Startup with Windows
	bRet &= (this->bConfirmAction == other.bConfirmAction);						// Show confirm message before doing action
	bRet &= (this->bSaveHistoryLog == other.bSaveHistoryLog);					// Save app history log
	bRet &= (this->bSaveAppEventLog == other.bSaveAppEventLog);					// Save app event log
	bRet &= (this->bRunAsAdmin == other.bRunAsAdmin);							// Run with admin privileges
	bRet &= (this->bShowErrorMsg == other.bShowErrorMsg);						// Show action error message
	bRet &= (this->bNotifySchedule == other.bNotifySchedule);					// Show notify tip for schedule action
	bRet &= (this->bAllowCancelSchedule == other.bAllowCancelSchedule);			// Allow canceling schedule when notify
	bRet &= (this->bEnableBackgroundHotkey == other.bEnableBackgroundHotkey);	// Enable background action hotkeys
	bRet &= (this->bLockStateHotkey == other.bLockStateHotkey);					// Allow background hotkeys on lockscreen
	bRet &= (this->bEnablePowerReminder == other.bEnablePowerReminder);			// Enable Power Peminder feature

	return bRet;
}


/**
 * @brief	Get a clone copy of config data
 * @param	pData - Output config data (out)
 * @return	None
 */
 void ConfigData::GetData(CONFIGDATAINFO& pData) const noexcept
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


/**
 * @brief	Get application option data by ID
 * @param	eAppOptionID - Option ID
 * @return	int - Option value
 */
 int ConfigData::GetAppOption(AppOptionID eAppOptionID) const noexcept
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


/**
 * @brief	Constructor
 */
 PwrRepeatSet::PwrRepeatSet()
{
	// Init data
	m_bRepeat = false;										// Repeat daily
	m_bAllowSnooze = true;									// Allow snoozing mode
	m_nSnoozeInterval = defaultSnoozeInterval;				// Snooze interval
	m_byRepeatDays = defaultActiveDays;						// Default repeat: All days of week
}


/**
 * @brief	Copy data from another Repeat set data
 * @param	other - Pointer of input item
 * @return	None
 */
void PwrRepeatSet::Copy(const PwrRepeatSet& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	m_bRepeat = other.m_bRepeat;							// Repeat daily
	m_bAllowSnooze = other.m_bAllowSnooze;					// Allow snoozing mode
	m_nSnoozeInterval = other.m_nSnoozeInterval;			// Snooze interval
	m_byRepeatDays = other.m_byRepeatDays;					// Days of week (for repeating)
}


/**
 * @brief	Compare data with another Repeat set data
 * @param	other - Pointer of input item
 * @return	true/false
 */
constexpr bool PwrRepeatSet::Compare(const PwrRepeatSet& other) const noexcept
{
	bool bRetCompare = true;

	// Compare data
	bRetCompare &= (this->m_bRepeat == other.m_bRepeat);					// Repeat daily
	bRetCompare &= (this->m_bAllowSnooze == other.m_bAllowSnooze);			// Allow snoozing mode
	bRetCompare &= (this->m_nSnoozeInterval == other.m_nSnoozeInterval);	// Snooze interval
	bRetCompare &= (this->m_byRepeatDays == other.m_byRepeatDays);			// Days of week (for repeating)

	return bRetCompare;
}


/**
 * @brief	Constructor
 */
ScheduleItem::ScheduleItem()
{
	// Initialize
	m_nItemID = ScheduleData::minItemID;				// Item ID
	m_bEnabled = false;									// Enable/disable status
	m_nActionID = APP_ACTION_NOTHING;					// Schedule action ID
	m_stTime = ClockTime();								// Schedule time
	m_rpsRepeatSet = PwrRepeatSet();					// Repeat set data
}

ScheduleItem::ScheduleItem(unsigned nItemID)
{
	// Initialize
	m_nItemID = nItemID;								// Item ID
	m_bEnabled = false;									// Enable/disable status
	m_nActionID = APP_ACTION_NOTHING;					// Schedule action
	m_stTime = ClockTime();								// Schedule time
	m_rpsRepeatSet = PwrRepeatSet();					// Repeat set data
}


/**
 * @brief	Copy data from another schedule item
 * @param	other - Pointer of input item
 * @return	None
 */
void ScheduleItem::Copy(const ScheduleItem& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	m_nItemID = other.m_nItemID;						// Item ID
	m_bEnabled = other.m_bEnabled;						// Enable/disable status
	m_nActionID = other.m_nActionID;					// Schedule action ID
	m_stTime = other.m_stTime;							// Schedule time
	m_rpsRepeatSet.Copy(other.m_rpsRepeatSet);			// Repeat set data
}


/**
 * @brief	Compare with another given item
 * @param	other - Pointer of given item
 * @return	true/false
 */
constexpr bool ScheduleItem::Compare(const ScheduleItem& other) const noexcept
{
	bool bRet = true;

	// Compare item (do not compare item ID)
	bRet &= (this->m_bEnabled == other.m_bEnabled);
	bRet &= (this->m_nActionID == other.m_nActionID);
	bRet &= (this->m_stTime.Hour() == other.m_stTime.Hour());
	bRet &= (this->m_stTime.Minute() == other.m_stTime.Minute());
	bRet &= (this->m_rpsRepeatSet.Compare(other.m_rpsRepeatSet));

	return bRet;
}


/**
 * @brief	Print schedule item
 * @param	outputString - Output printed string
 * @return	None
 */
void ScheduleItem::Print(String& outputString) const
{
	// Use table, language and core functions
	using namespace MapTable;
	using namespace Language;
	using namespace AppCore;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format schedule data
	const wchar_t* enableState = (m_bEnabled == true) ? Constant::Value::True : Constant::Value::False;							// Enable/disable state
	unsigned nActionStringID = GetPairedID(IDTable::ActionName, m_nActionID);
	const wchar_t* actionName = GetLanguageString(ptrLanguage, nActionStringID);												// Schedule action
	const wchar_t* timeFormat = ClockTimeUtils::Format(ptrLanguage, IDS_FORMAT_SHORTTIME, m_stTime).GetString();				// Schedule time
	const wchar_t* repeatState = (m_rpsRepeatSet.IsRepeatEnabled() == true) ? Constant::Value::True : Constant::Value::False;	// Repeat daily

	// Print item
	outputString.Format(_T("Active=(%s), ItemID=%d, Action=(%s), Time=(%s), Repeat=(%s)"),
					enableState, m_nItemID, actionName, timeFormat, repeatState);
}


/**
 * @brief	Constructor
 */
ScheduleData::ScheduleData()
{
	// Initialize
	m_schDefaultItem = ScheduleItem(ScheduleData::defaultItemID);
	m_arrSchedExtraItemList.clear();
}


/**
 * @brief	Copy data from another Action Schedule data
 * @param	other - Pointer of input data
 * @return	None
 */
void ScheduleData::Copy(const ScheduleData& other)
{
	// Do not copy itself
	if (this == &other) return;

	// Remove existing data
	this->DeleteAll();

	// Copy default item
	this->m_schDefaultItem.Copy(other.m_schDefaultItem);

	// Copy extra data
	for (int nIndex = 0; nIndex < other.GetExtraItemNum(); nIndex++) {
		ScheduleItem schItem = other.m_arrSchedExtraItemList.at(nIndex);
		this->m_arrSchedExtraItemList.push_back(schItem);
	}
}


/**
 * @brief	Add an Action Schedule item
 * @param	pItem - Pointer of input item
 * @return	DWORD - Error code
 */
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
		if (pItemTemp.Compare(pItem) == true) {
			// All data is duplicated
			return Error::ItemDuplicated;
		}
		else if (ClockTimeUtils::IsMatching(pItemTemp.GetTime(), pItem.GetTime())) {
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


/**
 * @brief	Update a Action Schedule item
 * @param	pItem - Pointer of input item
 * @return	DWORD - Error code
 */
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


/**
 * @brief	Remove a schedule item by index
 * @param	nAtIndex - Index of item to remove
 * @return	None
 */
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


/**
 * @brief	Adjust Action Schedule data validity
 * @param	None
 * @return	None
 */
void ScheduleData::Adjust(void)
{
	// If default item is empty but extra data is not
	if (IsDefaultEmpty() && IsExtraEmpty() == false) {
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


/**
 * @brief	Get next item ID (to add new item)
 * @param	None
 * @return	unsigned
 */
unsigned ScheduleData::GetNextID(void) const
{
	// Get currently max ID
	unsigned nRetNextID = ScheduleData::minItemID;
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


/**
 * @brief	Check if extra data is empty
 * @param	None
 * @return	bool - Result of all item empty
 */
constexpr bool ScheduleData::IsExtraEmpty(void) const noexcept
{
	// If there's no item, return true
	if (m_arrSchedExtraItemList.empty())
		return true;

	// Check each item
	bool bExtraEmpty = true;
	for (int nIndex = 0; nIndex < GetExtraItemNum(); nIndex++) {
		if (IsEmpty(nIndex) == false) {
			bExtraEmpty = false;
			break;
		}
	}

	return bExtraEmpty;
}


/**
 * @brief	Delete a reminder item by index
 * @param	nAtIndex - Index of item to delete
 * @return	None
 */
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


/**
 * @brief	Constructor
 */
HotkeySetItem::HotkeySetItem()
{
	// Initialize
	m_bEnabled = false;								// Hotkey enabled/disabled
	m_nHKActionID = 0;								// Hotkey action ID
	m_dwModifiers = 0;								// Modifier keys
	m_dwVirtualKey = 0;								// Virtual key code
}

HotkeySetItem::HotkeySetItem(unsigned nHKActionID)
{
	// Initialize
	m_bEnabled = false;								// Hotkey enabled/disabled
	m_nHKActionID = nHKActionID;					// Hotkey action ID
	m_dwModifiers = 0;								// Modifier keys
	m_dwVirtualKey = 0;								// Virtual key code
}


/**
 * @brief	Copy data from another Hotkeyset item
 * @param	other - Pointer of input item
 * @return	None
 */
void HotkeySetItem::Copy(const HotkeySetItem& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	m_bEnabled = other.m_bEnabled;					// Hotkey enabled/disabled
	m_nHKActionID = other.m_nHKActionID;			// Hotkey action ID
	m_dwModifiers = other.m_dwModifiers;			// Modifier keys
	m_dwVirtualKey = other.m_dwVirtualKey;			// Virtual key code
}


/**
 * @brief	Print HotkeySet item
 * @param	outputString - Output printed string
 * @return	None
 */
void HotkeySetItem::Print(String& outputString) const
{
	// Use table and language functions
	using namespace MapTable;
	using namespace Language;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	const wchar_t* lpszEnable = (m_bEnabled == true) ? _T("Enabled") : _T("Disabled");
	unsigned nActionNameID = GetPairedID(IDTable::ActionName, GetPairedID(IDTable::HKActionID, m_nHKActionID));
	const wchar_t* lpszAction = GetLanguageString(ptrLanguage, nActionNameID);
	String keyStrokesStr = Constant::String::Empty;
	PrintKeyStrokes(keyStrokesStr);

	// Print item
	outputString.Format(_T("State=(%s), Action=(%s), Keystrokes=(%s)"),  lpszEnable, lpszAction, keyStrokesStr.GetString());
}


/**
 * @brief	Print HotkeySet item keystrokes data
 * @param	outputString - Output printed keystrokes string
 * @return	None
 */
void HotkeySetItem::PrintKeyStrokes(String& outputString) const
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
	outputString.Empty();
	outputString = strKeyStrokes;
}


/**
 * @brief	Copy data from another HotkeySet data
 * @param	other - Pointer of input data
 * @return	None
 */
void HotkeySetData::Copy(const HotkeySetData& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Remove existing data
	this->DeleteAll();

	// Copy data
	for (int nIndex = 0; nIndex < other.GetItemNum(); nIndex++) {
		const Item& hksItem = other.m_arrHotkeySetList.at(nIndex);
		this->m_arrHotkeySetList.push_back(hksItem);
	}
}


/**
 * @brief	Set default for HotkeySet data
 * @param	pcfgData - Pointer of HotkeySet data
 * @return	None
 */
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


/**
 * @brief	Add a hotkeyset item
 * @param	pItem - Pointer of input item
 * @return	None
 */
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
		if (pItemTemp.Compare(pItem) == true)
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


/**
 * @brief	Update a hotkeyset item
 * @param	pItem - Pointer of input item
 * @return	None
 */
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
		if (hksTemp.CompareKeycode(pItem) == true) {
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


/**
 * @brief	Remove a hotkeyset item by index
 * @param	nAtIndex - Index of item to remove
 * @return	None
 */
void HotkeySetData::Remove(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= GetItemNum()))
		return;

	// Get item data
	Item& hksItem = GetItemAt(nAtIndex);

	// Reset item value
	hksItem.EnableItem(false);
	hksItem.SetKeyCode(NULL, NULL);
}


/**
 * @brief	Adjust HotkeySet data validity
 * @param	None
 * @return	None
 */
void HotkeySetData::Adjust(void)
{
	DWORD dwCtrlKey, dwFuncKey;
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {

		// Get hotkeyset item keycode
		Item& hksItem = GetItemAt(nIndex);
		hksItem.GetKeyCode(dwCtrlKey, dwFuncKey);

		// Not enable hotkeyset item if no keystroke data
		if ((dwCtrlKey == 0) || (dwFuncKey == 0)) {
			hksItem.EnableItem(false);
			hksItem.SetKeyCode(NULL, NULL);
		}
	}
}


/**
 * @brief	Check if all item are empty
 * @param	None
 * @return	bool - Result of all item empty
 */
bool HotkeySetData::IsAllEmpty(void) const noexcept
{
	// If there's no item, return true
	if (m_arrHotkeySetList.empty())
		return true;

	// Check each item
	bool bAllEmpty = true;
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		if (IsEmpty(nIndex) == false) {
			bAllEmpty = false;
			break;
		}
	}

	return bAllEmpty;
}


/**
 * @brief	Delete a hotkeyset item by index
 * @param	nAtIndex - Index of item to delete
 * @return	None
 */
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


/**
 * @brief	Print HotkeySet item keystrokes data by ID
 * @param	nHKID		 - Item hotkey ID
 * @param	outputString - Output printed keystrokes string
 * @return	None
 */
void HotkeySetData::PrintKeyStrokes(unsigned nHKID, String& outputString) const
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


/**
 * @brief	Constructor
 */
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


/**
 * @brief	Copy data from another Reminder message style set data
 * @param	other - Pointer of input item
 * @return	None
 */
void RmdMsgStyleSet::Copy(const RmdMsgStyleSet& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	m_colorBkgrd = other.m_colorBkgrd;							// Background color
	m_colorText = other.m_colorText;							// Text color
	m_strFontName = other.m_strFontName;						// Font name
	m_uiFontSize = other.m_uiFontSize;							// Font size
	m_uiTimeout = other.m_uiTimeout;							// Timeout (auto-close) interval
	m_uiIconID = other.m_uiIconID;								// Message icon ID
	m_nIconSize = other.m_nIconSize;							// Message icon size
	m_byIconPos = other.m_byIconPos;							// Message icon position
	m_byDisplayPos = other.m_byDisplayPos;						// Message display position
	m_uiHMargin = other.m_uiHMargin;							// Display area horizontal margin
	m_uiVMargin = other.m_uiVMargin;							// Display area vertical margin
}


/**
 * @brief	Compare data with another Repeat set data
 * @param	other - Pointer of input item
 * @return	true/false
 */
bool RmdMsgStyleSet::Compare(const RmdMsgStyleSet& other) const noexcept
{
	bool bRetCompare = true;

	// Compare data
	bRetCompare &= (this->m_colorBkgrd == other.m_colorBkgrd);		// Background color
	bRetCompare &= (this->m_colorText == other.m_colorText);		// Text color
	bRetCompare &= (this->m_strFontName == other.m_strFontName);	// Font name
	bRetCompare &= (this->m_uiFontSize == other.m_uiFontSize);		// Font size
	bRetCompare &= (this->m_uiTimeout == other.m_uiTimeout);		// Timeout (auto-close) interval
	bRetCompare &= (this->m_uiIconID == other.m_uiIconID);			// Message icon ID
	bRetCompare &= (this->m_nIconSize == other.m_nIconSize);		// Message icon size
	bRetCompare &= (this->m_byIconPos == other.m_byIconPos);		// Message icon position
	bRetCompare &= (this->m_byDisplayPos == other.m_byDisplayPos);	// Message display position
	bRetCompare &= (this->m_uiHMargin == other.m_uiHMargin);		// Display area horizontal margin
	bRetCompare &= (this->m_uiVMargin == other.m_uiVMargin);		// Display area vertical margin

	return bRetCompare;
}


/**
 * @brief	Constructor
 */
PwrReminderItem::PwrReminderItem()
{
	// Init data
	m_bEnabled = false;										// Enable state
	m_nItemID = PwrReminderData::minItemID;					// Item ID
	m_strMessage = Constant::String::Empty;					// Message content
	m_nEventID = Event::atSetTime;							// Event ID
	m_stTime = ClockTime();									// Event time
	m_dwMsgStyle = Style::messageBox;						// Reminder style
	m_rpsRepeatSet = PwrRepeatSet();						// Repeat set
	m_bUseCustomStyle = false;								// Use message custom style
	m_rmsMsgStyleSet = RmdMsgStyleSet();					// Reminder message style set
}


/**
 * @brief	Copy data from another Power Reminder item
 * @param	other - Pointer of input item
 * @return	None
 */
void PwrReminderItem::Copy(const PwrReminderItem& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	m_bEnabled = other.m_bEnabled;							// Enable state
	m_nItemID = other.m_nItemID;							// Item ID
	m_strMessage = other.m_strMessage;						// Message content
	m_nEventID = other.m_nEventID;							// Event ID
	m_stTime = other.m_stTime;								// Event time
	m_dwMsgStyle = other.m_dwMsgStyle;						// Reminder style
	m_rpsRepeatSet.Copy(other.m_rpsRepeatSet);				// Repeat set
	m_bUseCustomStyle = other.m_bUseCustomStyle;			// Use message custom style
	m_rmsMsgStyleSet.Copy(other.m_rmsMsgStyleSet);			// Reminder message style set
}


/**
 * @brief	Compare with another given item
 * @param	other - Pointer of given item
 * @return	true/false
 */
bool PwrReminderItem::Compare(const PwrReminderItem& other) const noexcept
{
	bool bRet = true;

	// Compare item (do not compare item ID)
	bRet &= (this->m_strMessage == other.m_strMessage);
	bRet &= (this->m_nEventID == other.m_nEventID);
	bRet &= (this->m_stTime.Hour() == other.m_stTime.Hour());
	bRet &= (this->m_stTime.Minute() == other.m_stTime.Minute());
	bRet &= (this->m_dwMsgStyle == other.m_dwMsgStyle);
	bRet &= (this->m_rpsRepeatSet.Compare(other.m_rpsRepeatSet));
	bRet &= (this->m_bUseCustomStyle == other.m_bUseCustomStyle);
	bRet &= (this->m_rmsMsgStyleSet.Compare(other.m_rmsMsgStyleSet));

	return bRet;
}


/**
 * @brief	Check if item snooze mode is available
 * @param	None
 * @return	true/false
 */
bool PwrReminderItem::IsAllowSnoozing(void) const noexcept
{
	// If current eventID is not at settime
	if (m_nEventID != Event::atSetTime) {
		// Not allow snooze mode
		return false;
	}
	// If repeat option is currently OFF
	if (IsRepeatEnabled() != true) {
		// Not allow snooze mode
		return false;
	}
	// If allow snooze option is OFF
	if (m_rpsRepeatSet.IsAllowSnoozing() != true) {
		// Not allow snooze mode
		return false;
	}

	return true;
}


/**
 * @brief	Print reminder item
 * @param	outputString - Output printed string
 * @return	None
 */
void PwrReminderItem::Print(String& outputString) const
{
	// Use table, language and core functions
	using namespace MapTable;
	using namespace Language;
	using namespace AppCore;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	const wchar_t* enableStr = (m_bEnabled == true) ? _T("Enabled") : _T("Disabled");
	String messageStr = m_strMessage;
	if (messageStr.GetLength() > (Constant::Max::DisplayLogStringLength + 3)) {
		messageStr = m_strMessage.Left(Constant::Max::DisplayLogStringLength) + _T("...");
	}
	int nTemp = GetPairedID(IDTable::PwrReminderEvent, m_nEventID);
	String eventStr = GetLanguageString(ptrLanguage, nTemp);
	if (m_nEventID == Event::atSetTime) {
		// Format time string
		String formatString = eventStr;
		eventStr = ClockTimeUtils::Format(ptrLanguage, formatString, m_stTime);
	}
	nTemp = GetPairedID(IDTable::PwrReminderStyle, m_dwMsgStyle);
	const wchar_t* styleStr = GetLanguageString(ptrLanguage, nTemp);

	// Print item
	outputString.Format(_T("State=(%s), ItemID=%d, Msg=(%s), Event=(%s), Style=(%s), Repeat=%d"),
		enableStr, m_nItemID, messageStr.GetString(), eventStr.GetString(), styleStr, m_rpsRepeatSet.IsRepeatEnabled());
}


/**
 * @brief	Constructor
 */
PwrReminderData::PwrReminderData()
{
	// Initialize
	m_arrRmdItemList.clear();
	m_rmdCommonStyle = RmdMsgStyleSet();
}


/**
 * @brief	Init Power Reminder data (NULL)
 * @param	None
 * @return	None
 */
void PwrReminderData::Init() noexcept
{
	// Initialize
	m_arrRmdItemList.clear();
	m_rmdCommonStyle = RmdMsgStyleSet();
}


/**
 * @brief	Copy data from another Power Reminder data
 * @param	other - Pointer of input data
 * @return	None
 */
 void PwrReminderData::Copy(const PwrReminderData& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Remove existing data
	this->DeleteAll();

	// Copy reminder data
	for (int nIndex = 0; nIndex < other.GetItemNum(); nIndex++) {
		PwrReminderItem pwrItem = other.m_arrRmdItemList.at(nIndex);
		this->m_arrRmdItemList.push_back(pwrItem);
	}

	// Copy common message style data
	this->m_rmdCommonStyle.Copy(other.m_rmdCommonStyle);
}


/**
 * @brief	Add a Power Reminder item
 * @param	pItem - Pointer of input item
 * @return	None
 */
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
		if (pItemTemp.Compare(pItem) == true)
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


/**
 * @brief	Update a Power Reminder item
 * @param	pItem - Pointer of input item
 * @return	None
 */
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


/**
 * @brief	Remove a reminder item by index
 * @param	nAtIndex - Index of item to remove
 * @return	None
 */
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


/**
 * @brief	Adjust Power Reminder data validity
 * @param	None
 * @return	None
 */
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


/**
 * @brief	Get next item ID (to add new item)
 * @param	None
 * @return	unsigned
 */
 unsigned PwrReminderData::GetNextID(void) const noexcept
{
	// Get max ID
	unsigned nRetNextID = PwrReminderData::minItemID;
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


/**
 * @brief	Check if all item are empty
 * @param	None
 * @return	bool - Result of all item empty
 */
 bool PwrReminderData::IsAllEmpty() const noexcept
{
	// If there's no item, return true
	if (m_arrRmdItemList.empty())
		return true;

	// Check each item
	bool bAllEmpty = true;
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		if (IsEmpty(nIndex) == false) {
			bAllEmpty = false;
			break;
		}
	}

	return bAllEmpty;
}


/**
 * @brief	Delete a reminder item by index
 * @param	nAtIndex - Index of item to delete
 * @return	None
 */
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


/**
 * @brief	Constructor
 */
 PwrRuntimeItem::PwrRuntimeItem()
{
	// Init data
	m_nCategory = INT_INVALID;								// Item category
	m_nItemID = INT_NULL;									// Power Reminder item ID
	m_nDisplayFlag = FLAG_OFF;								// Item displaying flag
	m_nSkipFlag = FLAG_OFF;									// Item skip flag
	m_nSnoozeFlag = FLAG_OFF;								// Item snooze trigger flag
	m_stNextSnoozeTime = ClockTime();						// Next snooze trigger time
}


/**
 * @brief	Copy data from another Power++ runtime info item
 * @param	other - Pointer of input item
 * @return	None
 */
 void PwrRuntimeItem::Copy(const PwrRuntimeItem& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	m_nCategory = other.m_nCategory;						// Item category
	m_nItemID = other.m_nItemID;							// Power Reminder item ID
	m_nDisplayFlag = other.m_nDisplayFlag;					// Item displaying flag
	m_nSkipFlag = other.m_nSkipFlag;						// Item skip flag
	m_nSnoozeFlag = other.m_nSnoozeFlag;					// Item snooze trigger flag
	m_stNextSnoozeTime = other.m_stNextSnoozeTime;			// Next snooze trigger time
}


/**
 * @brief	Calculate next snooze trigger time
 * @param	nInterval - Snooze interval
 * @return	None
 */
void PwrRuntimeItem::CalcNextSnoozeTime(int nInterval) noexcept
{
	ClockTimeUtils::CalculateOffset(m_stNextSnoozeTime, nInterval);
}


/**
 * @brief	Constructor
 */
 HistoryInfoData::HistoryInfoData()
{
	// Init data
	m_bInitState = false;									// Init state
	m_nCategoryID = INT_NULL;								// Category ID
	m_stTimestamp = DateTime();								// Timestamp of history
	m_nItemID = INT_NULL;									// Item ID
	m_nActionID = INT_NULL;									// History action ID
	m_bActionResult = true;									// Action result
	m_dwErrorCode = APP_ERROR_SUCCESS;						// Returned error code
	m_strDescription = Constant::String::Empty;				// History description (attached info)
}


/**
 * @brief	Copy data from another action history data
 * @param	other - Pointer of input data
 * @return	None
 */
void HistoryInfoData::Copy(const HistoryInfoData& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	m_bInitState = other.m_bInitState;						// Init state
	m_nCategoryID = other.m_nCategoryID;					// Category ID
	m_stTimestamp = other.m_stTimestamp;					// Timestamp of history
	m_nItemID = other.m_nItemID;							// Item ID
	m_nActionID = other.m_nActionID;						// History action ID
	m_bActionResult = other.m_bActionResult;				// Action result
	m_dwErrorCode = other.m_dwErrorCode;					// Returned error code
	m_strDescription = other.m_strDescription;				// History description (attached info)
}


/**
 * @brief	Initialization
 * @param	nCategoryID - Category ID
 * @return	None
 */
void HistoryInfoData::Init(unsigned nCategoryID) noexcept
{
	// Reset data
	RemoveAll();
	m_bInitState = true;									// Init state
	m_nCategoryID = nCategoryID;							// Category ID
	m_stTimestamp = DateTimeUtils::GetCurrentDateTime();	// Timestamp of history
	m_bActionResult = true;									// Action result
	m_dwErrorCode = APP_ERROR_SUCCESS;						// Returned error code
	m_strDescription = Constant::String::Empty;				// History description (attached info)
}


/**
 * @brief	Constructor
 */
SystemEvent::SystemEvent(EventID eventID)
{
	// Initialize
	m_sysEventID = eventID;									// System event ID
	m_timeStamp = DateTime();								// Event timestamp
}

SystemEvent::SystemEvent(const SystemEvent& other)
{
	// Copy data
	m_sysEventID = other.m_sysEventID;						// System event ID
	m_timeStamp = other.m_timeStamp;						// Event timestamp
}


/**
 * @brief	Copy assignment operator
 * @param	other - Pointer of input item
 * @return	None
 */
SystemEvent& SystemEvent::operator=(const SystemEvent& other)
{
	// Copy data
	m_sysEventID = other.m_sysEventID;						// System event ID
	m_timeStamp = other.m_timeStamp;						// Event timestamp

	return *this;
}


/**
 * @brief	Constructor
 */
SystemEventTracker::SystemEventTracker()
{
	// Initialize
	m_arrTrackingData.clear();								// System event tracking data
}

SystemEventTracker::SystemEventTracker(const SystemEventTracker& other)
{
	// Copy data
	m_arrTrackingData = other.m_arrTrackingData;			// System event tracking data
}


/**
 * @brief	Copy assignment operator
 * @param	other - Pointer of input data
 * @return	None
 */
SystemEventTracker& SystemEventTracker::operator=(const SystemEventTracker& other)
{
	// Copy data
	m_arrTrackingData = other.m_arrTrackingData;			// System event tracking data

	return *this;
}


/**
 * @brief	Format string (same as default MFC Format function)
 * @param	formatTemplateID  - ID of resource format template string
 * @param	...				  - Same as default MFC Format function
 * @return	String - Returned formatted string
 */
String StringUtils::StringFormat(unsigned formatTemplateID, ...)
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
	va_start(argList, formatTemplateID);
	resultString.FormatV(templateString, argList);
	va_end(argList);

	return resultString;
}


/**
 * @brief	Format string (same as default MFC Format function)
 * @param	formatTemplate - Format template string
 * @param	...			   - Same as default MFC Format function
 * @return	String	- Returned formatted string
 */
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


/**
 * @brief	Load resource ID and return the string
 * @param	nResStringID - ID of resource string
 * @return	String	- Returned resource string
 */
String StringUtils::LoadResourceString(unsigned resourceStringID)
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


/**
 * @brief	Load resource ID and return the string
 * @param	strResult	 - Returned resource string
 * @param	nResStringID - ID of resource string
 * @return	true/false
 */
bool StringUtils::LoadResourceString(String& resultStr, unsigned resourceStringID)
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


/**
 * @brief	Load resource text file by ID and return the text data
 * @param	resourceFileID - ID of the file in resource
 * @return	true/false
 */
String StringUtils::LoadResourceTextData(unsigned resourceFileID)
{
	// Get resource handle
	HINSTANCE hResInstance = AfxGetResourceHandle();
	if (hResInstance == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get resource handle failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return Constant::String::Empty;
	}

	// Find resource file by ID
	HRSRC hRes = FindResource(hResInstance, MAKEINTRESOURCE(resourceFileID), RT_RCDATA);
	if (hRes != NULL) {

		// Load resource data
		HGLOBAL hData = LoadResource(hResInstance, hRes);
		if (hData != NULL) {

			// Convert resource data to text data
			const size_t dataSize = SizeofResource(hResInstance, hRes);
			const wchar_t* dataBuffer = static_cast<const wchar_t*>(LockResource(hData));
			if (dataBuffer) {
				const size_t wcharCount = dataSize / sizeof(wchar_t);
				std::vector<wchar_t> textBuffer(dataBuffer, dataBuffer + wcharCount);
				textBuffer.push_back(Constant::Char::EndString);
				return String(textBuffer.data());
			}
		}
	}

	return Constant::String::Empty;
}


/**
 * @brief	Get application executable file path
 * @param	includeExeName - Including executable file name
 * @return	String - Return application path
 */
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


/**
 * @brief	Get full sub-folder path
 * @param	lpszSubFolderName - Subfolder name
 * @return	String
 */
String StringUtils::GetSubFolderPath(const wchar_t* subFolderName)
{
	// Get application executable file path
	String appPath = GetApplicationPath(false);

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


/**
 * @brief	Make file path by given part names
 * @param	directory - Directory path
 * @param	fileName  - File name
 * @param	extension - File extension
 * @return	String
 */
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


/**
 * @brief	Get app executable file product version info
 * @param	isFullVersion - Full product version number (x.x.x.x)
 * @param	or short version number (x.x)
 * @return	String - Product version string
 */
String StringUtils::GetProductVersion(bool isFullVersion)
{
	// Get product file name
	String productFileName = StringUtils::MakeFilePath(NULL, Constant::File::Name::App_Executable, Constant::File::Extension::Exe);
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
	unsigned uLen;
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


/**
 * @brief	Get app executable file product version info
 * @param	fullVersion  - Full product version number (x.x.x.x)
 * @param	shortVersion - Short product version number (x.x)
 * @return	true/false
 */
bool StringUtils::GetProductVersion(String& fullVersion, String& shortVersion)
{
	// Get product file name
	String productFileName = StringUtils::MakeFilePath(NULL, Constant::File::Name::App_Executable, Constant::File::Extension::Exe);
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
	unsigned uLen;
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


/**
 * @brief	Get the computer device name
 * @param	deviceName - Device name (out)
 * @return	true/false
 */
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


/**
 * @brief	Get the current Windows user name
 * @param	userName - User name (out)
 * @return	true/false
 */
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


/**
 * @brief	Print list of characters of given string
 * @param	srcStr	  - Given string
 * @param	outputStr - Result string
 * @return	int - Number of characters
 */
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


/**
 * @brief	Get current local clock-time (including milliseconds)
 * @param	None
 * @return	ClockTime - Return clock-time data
 */
ClockTime ClockTimeUtils::GetCurrentClockTime(void)
{
	SYSTEMTIME _tempSysTime{};
	::GetLocalTime(&_tempSysTime);

	return FromSystemTime(_tempSysTime);
}


/**
 * @brief	Convert Windows-based SYSTEMTIME to ClockTime data
 * @param	sysTime - Windows-based SYSTEMTIME data
 * @return	ClockTime - Return clock-time data
 */
ClockTime ClockTimeUtils::FromSystemTime(SYSTEMTIME sysTime)
{
	int hour = static_cast<int>(sysTime.wHour);
	int minute = static_cast<int>(sysTime.wMinute);
	int second = static_cast<int>(sysTime.wSecond);
	int millisecs = static_cast<int>(sysTime.wMilliseconds);

	return ClockTime(hour, minute, second, millisecs);
}


/**
 * @brief	Convert ClockTime data to Windows-based SYSTEMTIME data
 * @param	clockTime - Clock-time data
 * @return	SYSTEMTIME - Return Windows-based SYSTEMTIME data
 */
SYSTEMTIME ClockTimeUtils::ToSystemTime(const ClockTime& clockTime)
{
	SYSTEMTIME _sysTime{};
	_sysTime.wHour = static_cast<unsigned short>(clockTime.Hour());
	_sysTime.wMinute = static_cast<unsigned short>(clockTime.Minute());
	_sysTime.wSecond = static_cast<unsigned short>(clockTime.Second());
	_sysTime.wMilliseconds = static_cast<unsigned short>(clockTime.Millisecond());

	return _sysTime;
}


/**
 * @brief	Convert editbox input text into valid time value
 * @param	stTime		- Return time data (ref-value)
 * @param	inputText	- Input text
 * @return	bool - Result of converting process
 */
bool ClockTimeUtils::InputText2Time(ClockTime& clockTime, const wchar_t* inputText)
{
	// Check input text validity
	int nLength = wcslen(inputText);
	if ((nLength == 0) || (nLength > 4))
		return false;

	String timeString(inputText);

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
		return false;

	// Only return if both the hour and minute values are valid
	if ((nHour > INT_INVALID) && (nMinute > INT_INVALID)) {
		clockTime.SetHour(nHour);
		clockTime.SetMinute(nMinute);
	}

	return true;
}


/**
 * @brief	Convert editbox input text into valid time value
 * @param	stTime		- Return time data (ref-value)
 * @param	inputText	- Input text
 * @return	bool - Result of converting process
 * @note	Old/base function (no longer used)
 */
bool ClockTimeUtils::InputText2TimeBase(ClockTime& clockTime, const wchar_t* inputText)
{
	// Check input text validity
	int nLength = wcslen(inputText);
	if ((nLength == 0) || (nLength > 4))
		return false;

	// Get input text length
	unsigned nTime = _tstoi(inputText);
	unsigned nTimeTemp = nTime;
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
		return false;

	if ((nHour > -1) && (nMinute > -1)) {
		clockTime.SetHour(nHour);
		clockTime.SetMinute(nMinute);
	}

	return true;
}


/**
 * @brief	Convert timespin position to time value
 * @param	stTime  - Return time data (ref-value)
 * @param	nPos	- Input spin position
 * @return	None
 */
void ClockTimeUtils::SpinPos2Time(ClockTime& clockTime, int nPos)
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
		clockTime.SetHour(nHour);
		clockTime.SetMinute(nMinute);
	}
}


/**
 * @brief	Convert time value to timespin position
 * @param	stTime  - Return time data
 * @param	nPos	- Input spin position (ref-value)
 * @return	None
 */
void ClockTimeUtils::Time2SpinPos(const ClockTime& clockTime, int& nPos)
{
	// Convert
	nPos = (clockTime.Hour() * 60) + clockTime.Minute();

	// Invalid result
	if (nPos < Constant::Min::TimeSpin)
		nPos = Constant::Min::TimeSpin;
	else if (nPos > Constant::Max::TimeSpin)
		nPos = Constant::Max::TimeSpin;
}


/**
 * @brief	Calculate time with given offset in seconds
 * @param	clockTime - Clock-time data (in/out)
 * @param	offInSecs - Offset value (in seconds)
 * @return	None
 */
void ClockTimeUtils::CalculateOffset(ClockTime& clockTime, int offInSecs)
{
	if (offInSecs < 0)
		clockTime.DecreaseSeconds(abs(offInSecs));
	else
		clockTime.IncreaseSeconds(offInSecs);
}


/**
 * @brief	Clock-time comparison with allowable offset in seconds
 * @param	thisTime  - This clock-time data
 * @param	otherTime - The other clock-time data to compare
 * @param	offInSecs - Offset value (in seconds, 0 by default)
 * @return	true/false - Clock-time values are matching or
 * @return	different within the allowable offset
 */
bool ClockTimeUtils::IsMatching(ClockTime thisTime, ClockTime otherTime, int offInSecs /* = 0 */)
{
	TimeSpan _diff = thisTime - otherTime;
	int _diffInSecs = static_cast<int>(_diff.TotalSeconds());
	if (offInSecs == 0)
		return (_diffInSecs == 0);
	else if (offInSecs > 0)
		return (_diffInSecs >= 0 && _diffInSecs <= offInSecs);
	else /* if (offInSecs < 0) */		return (_diffInSecs >= offInSecs && _diffInSecs <= 0);
}


/**
 * @brief	Format clock-time value for displaying or printing
 * @param	pLang	  - Language table pointer
 * @param	nFormatID - Format string ID
 * @param	clockTime - Given clock-time data
 * @return	String - Format clock-time string
 */
String ClockTimeUtils::Format(LANGTABLE_PTR pLang, unsigned nFormatID, const ClockTime& clockTime)
{
	// Load format string
	String formatString = StringUtils::LoadResourceString(nFormatID);
	return Format(pLang, formatString, clockTime);
}


/**
 * @brief	Format clock-time value for displaying or printing
 * @param	pLang		 - Language table pointer
 * @param	formatString - Format string
 * @param	clockTime	 - Given clock-time data
 * @return	String - Format clock-time string
 */
String ClockTimeUtils::Format(LANGTABLE_PTR pLang, const wchar_t* formatString, const ClockTime& clockTime)
{
	// Format time string
	unsigned nTimePeriod = (clockTime.Hour() < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	const wchar_t* timePeriodFormat = Language::GetLanguageString(pLang, nTimePeriod);
	int hourVal = (clockTime.Hour() > 12) ? (clockTime.Hour() - 12) : clockTime.Hour();
	int minuteVal = clockTime.Minute();

	return StringUtils::StringFormat(formatString, hourVal, minuteVal, timePeriodFormat);
}


/**
 * @brief	Get current local date/time (including milliseconds)
 * @param	None
 * @return	DateTime - Return date/time data
 */
DateTime DateTimeUtils::GetCurrentDateTime(void)
{
	SYSTEMTIME _tempSysTime{};
	::GetLocalTime(&_tempSysTime);

	return FromSystemTime(_tempSysTime);
}


/**
 * @brief	Convert Windows-based SYSTEMTIME to DateTime data
 * @param	sysTime - Windows-based SYSTEMTIME data
 * @return	DateTime - Return date/time data
 */
DateTime DateTimeUtils::FromSystemTime(SYSTEMTIME sysTime)
{
	int year = static_cast<int>(sysTime.wYear);
	unsigned int month = static_cast<unsigned int>(sysTime.wMonth);
	unsigned int day = static_cast<unsigned int>(sysTime.wDay);
	int hour = static_cast<int>(sysTime.wHour);
	int minute = static_cast<int>(sysTime.wMinute);
	int second = static_cast<int>(sysTime.wSecond);
	int millisecs = static_cast<int>(sysTime.wMilliseconds);

	return DateTime(year, month, day, hour, minute, second, millisecs);
}


/**
 * @brief	Convert DateTime data to Windows-based SYSTEMTIME data
 * @param	dateTime - Date/time data
 * @return	SYSTEMTIME - Return Windows-based SYSTEMTIME data
 */
SYSTEMTIME DateTimeUtils::ToSystemTime(const DateTime& dateTime)
{
	SYSTEMTIME _sysTime{};
	_sysTime.wYear = static_cast<unsigned short>(dateTime.Year());
	_sysTime.wMonth = static_cast<unsigned short>(dateTime.Month());
	_sysTime.wDay = static_cast<unsigned short>(dateTime.Day());
	_sysTime.wDayOfWeek = static_cast<unsigned short>(dateTime.DayOfWeek());
	_sysTime.wHour = static_cast<unsigned short>(dateTime.Hour());
	_sysTime.wMinute = static_cast<unsigned short>(dateTime.Minute());
	_sysTime.wSecond = static_cast<unsigned short>(dateTime.Second());
	_sysTime.wMilliseconds = static_cast<unsigned short>(dateTime.Millisecond());

	return _sysTime;
}


/**
 * @brief	Format date/time value for displaying or printing
 * @param	pLang	  - Language table pointer
 * @param	nFormatID - Format string ID
 * @param	dateTime  - Given date/time data
 * @return	String - Format date/time string
 */
String DateTimeUtils::Format(LANGTABLE_PTR pLang, unsigned nFormatID, const DateTime& dateTime)
{
	// Load format string
	String formatString = StringUtils::LoadResourceString(nFormatID);
	return Format(pLang, formatString, dateTime);
}


/**
 * @brief	Format date/time value for displaying or printing
 * @param	pLang		 - Language table pointer
 * @param	formatString - Format string
 * @param	dateTime	 - Given date/time data
 * @return	String - Format time string
 */
String DateTimeUtils::Format(LANGTABLE_PTR pLang, const wchar_t* formatString, const DateTime& dateTime)
{
	// Format time string
	unsigned nTimePeriod = (dateTime.Hour() < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	const wchar_t* timePeriodFormat = Language::GetLanguageString(pLang, nTimePeriod);
	int hourVal = (dateTime.Hour() > 12) ? (dateTime.Hour() - 12) : dateTime.Hour();
	int minuteVal = dateTime.Minute();

	return StringUtils::StringFormat(formatString, hourVal, minuteVal, timePeriodFormat);
}


/**
 * @brief	Constructor
 */
PerformanceCounter::PerformanceCounter()
{
	// Initialization
	this->m_bIsRunning = false;
	QueryPerformanceFrequency(&m_liFrequency);
}


/**
 * @brief	Destructor
 */
PerformanceCounter::~PerformanceCounter()
{
	// Stop counting
	this->Stop();
}


/**
 * @brief	Start performance counter
 * @param	None
 * @return	None
 */
void PerformanceCounter::Start(void)
{
	// Start performance counter
	if (!m_bIsRunning) {
		QueryPerformanceCounter(&m_liStartTime);
		this->m_bIsRunning = true;
	}
}


/**
 * @brief	Stop performance counter
 * @param	None
 * @return	None
 */
void PerformanceCounter::Stop(void)
{
	// Stop performance counter
	if (m_bIsRunning) {
		QueryPerformanceCounter(&m_liEndTime);
		this->m_bIsRunning = false;
	}
}


/**
 * @brief	Get function execution elapsed time
 * @param	None
 * @return	None
 */
double PerformanceCounter::GetElapsedTime(bool bToMillisecs) const noexcept
{
	// Get elapsed time
	double dRetCounter = static_cast<double>(m_liEndTime.QuadPart - m_liStartTime.QuadPart) / m_liFrequency.QuadPart;
	if (bToMillisecs == true) {
		dRetCounter *= 1000;
	}
	return dRetCounter;
}


//////////////////////////////////////////////////////////////////////////
//
//	Define core methods for program
//
//////////////////////////////////////////////////////////////////////////


/**
 * @brief	Get the title name of current language
 * @param	nCurLanguage	- Current language ID
 * @param	bGetDescription - Get language package description
 * @return	const wchar_t* - Language name
 */
const wchar_t* Language::GetLanguageName(unsigned nCurLanguage, bool bGetDescription /* = false */)
{
	// Load language table package
	LANGTABLE_PTR ptrLangTable = LoadLanguageTable(nCurLanguage);
	if (ptrLangTable == NULL)
		return Constant::Value::Unknown;

	// Get language package info
	unsigned nInfoTargetID = (bGetDescription) ? LANGPACKINFO_DESCRIPTIONFULL : LANGPACKINFO_LANGNAMEID;
	const wchar_t* retInfoString = GetLanguageString(ptrLangTable, nInfoTargetID);
	if (IS_NULL_STRING(retInfoString))
		retInfoString = Constant::Value::Unknown;

	return retInfoString;
}


/**
 * @brief	Load language table by specified language option
 * @param	nCurLanguage   - Current language ID
 * @return	LANGTABLE_PTR - Language package pointer
 */
LANGTABLE_PTR Language::LoadLanguageTable(unsigned nCurLanguage)
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


/**
 * @brief	Find and return language string by ID
 * @param	ptLanguage - Language package pointer
 * @param	nID		   - Language string ID
 * @param	pszResult  - Result string (reference-type)
 * @return	const wchar_t*	- Language string
 */
const wchar_t* Language::GetLanguageString(LANGTABLE_PTR ptLanguage, unsigned nID)
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


/**
 * @brief	Main power action function
 * @param	nActionType - Type of action
 * @param	nMessage	- Action message
 * @param	dwErrorCode - Return error code (ref-value)
 * @return	bool - Result of action execution
 */
bool AppCore::ExecutePowerAction(unsigned nActionType, unsigned nMessage, DWORD& dwErrCode)
{
	bool bRet = true;

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
					unsigned uExitWinExFlags = nMessage;
					uExitWinExFlags |= EWX_FORCE;

					HANDLE hToken;
					TOKEN_PRIVILEGES tkPrivileges{};

					// Get process token
					if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
						// Get error code
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, OpenProcessToken failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}

					// Lookup privilege value
					if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkPrivileges.Privileges[0].Luid)) {
						// Get error code
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, LookupPrivilegeValue failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}

					// Adjust token privileges
					tkPrivileges.PrivilegeCount = 1;
					tkPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
					if (!AdjustTokenPrivileges(hToken, false, &tkPrivileges, 0, (PTOKEN_PRIVILEGES)NULL, 0)) {
						// Adjust token privileges failed
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, AdjustTokenPrivileges failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}

					// Exit Windows
					if (!ExitWindowsEx(uExitWinExFlags, 0)) {
						// Get exit Windows error
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, ExitWindowsEx failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}

				} break;

				case APP_MESSAGE_SLEEP:
					// Sleep mode
					if (!SetSuspendState(false, false, false)) {		// Stand by (sleep)
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, SetSuspendState failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}
					break;

				case APP_MESSAGE_HIBERNATE:
					// Hibernate mode
					if (!SetSuspendState(true, false, false)) {			// Hibernate
						dwErrCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, SetSuspendState failed!!! (Code: 0x%08X)", dwErrCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}
					break;
			}
		} break;

	default:
		// Wrong argument
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
		TRACE_FORMAT("Error: Cannot execute action, wrong argument!!! (Code: 0x%08X)", dwErrCode);
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		bRet = false;
		break;
	}

	return bRet;
}


/**
 * @brief	Dummy power action function (use for testing)
 * @param	nActionType - Type of action
 * @param	nMessage	- Action message
 * @param	dwErrorCode - Return error code (ref-value)
 * @return	bool - Result of action execution
 */
bool AppCore::ExecutePowerActionDummy(unsigned nActionType, unsigned nMessage, DWORD& dwErrCode)
{
	// Get action execution time
	DateTime currentDateTime = DateTimeUtils::GetCurrentDateTime();

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
	const wchar_t* timePeriod = (currentDateTime.Hour() < 12) ? Constant::Symbol::AnteMeridiem : Constant::Symbol::PostMeridiem;
	String templateFormatStr = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	timeFormatStr.Format(templateFormatStr, currentDateTime.Year(), currentDateTime.Month(), currentDateTime.Day(),
						currentDateTime.Hour(), currentDateTime.Minute(), currentDateTime.Second(), currentDateTime.Millisecond(), timePeriod);

	// Message format
	String messageFormatStr;
	messageFormatStr.Format(_T("[ExecutePowerAction]\nAction: %s\nTime: %s"), actionInfoString.GetString(), timeFormatStr.GetString());

	// Show dummy test message
	HWND hWnd = GET_HANDLE_MAINWND();
	MessageBox(hWnd, messageFormatStr, _T("DummyTest"), MB_OK | MB_ICONINFORMATION);
	return true;
}


/**
 * @brief	Create a loop and wait for specified message
 * @param	nMsg	 - Message to wait for
 * @param	nTimeout - Timeout (tick-count)
 * @return	LRESULT
 * @note	Be careful when using this function, it may cause the program to be not responding
 */
LRESULT	AppCore::WaitMessage(unsigned nMsg, int nTimeout /* = DEF_WAITMESSAGE_TIMEOUT */)
{
	LRESULT lResult = Result::Success;

	// Get begin timestamp (for timeout counter)
	ULONGLONG ullBeginTimestamp = GetTickCount64();

	// Wait for message
	while (1) {
		MSG msg = {0};
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


/**
 * @brief	Show error message by error code
 * @param	hMsgOwnerWnd - Handle of Message Box's owner window
 * @param	nLanguageID	 - Language option ID
 * @param	dwErrorCode	 - Error code
 * @param	lParam		 - Additional attached param (description string)
 * @return	None
 */
void AppCore::ShowErrorMessage(HWND hMsgOwnerWnd, unsigned nLanguageID, DWORD dwErrorCode, LPARAM lParam /* = NULL */)
{
	// Use table and language functions
	using namespace MapTable;
	using namespace Language;

	// Get application-defined error code from system-defined error code
	DWORD dwAppErrCode = GetPairedID(IDTable::ErrorCode, dwErrorCode, true);
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


/**
 * @brief	Find and return DebugTest dialog handle
 * @param	None
 * @return	HWND
 */
HWND AppCore::FindDebugTestDlg()
{
	String debugDlgTitle = StringUtils::LoadResourceString(IDS_APP_DEBUGTESTDLG_TITLE);
	if (debugDlgTitle.IsEmpty()) return NULL;
	return ::FindWindow(NULL, debugDlgTitle);
}


/**
 * @brief	Set fixed cell style (header row, base column)
 * @param	pGridCtrl	- Grid control table pointer
 * @param	nRow & nCol - Cell position (row & column)
 * @return	None
 */
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


/**
 * @brief	Set dark mode for dialog with specified handle
 * @param	pWnd			- Pointer of window
 * @param	bEnableDarkMode - Enable/disable dark mode
 * @return	bool - Result of dark mode setting process
 */
bool AppCore::SetDarkMode(CWnd* pWnd, bool bEnableDarkMode)
{
	// Load theme library
	HMODULE hUxTheme = LoadLibraryEx(_T("uxtheme.dll"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hUxTheme == NULL) {
		// Trace error
		TRACE_FORMAT("Error: Load UXTheme library failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get function pointer
	using fnAllowDarkMode = bool (WINAPI*)(HWND hWND, bool bAllow);
	static const fnAllowDarkMode AllowDarkModeForWindow = (fnAllowDarkMode)GetProcAddress(hUxTheme, MAKEINTRESOURCEA(133));
	if (AllowDarkModeForWindow == NULL) {
		// Trace error
		TRACE_FORMAT("Error: Get AllowDarkModeForWindow function address failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
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

	return true;
}


/**
 * @brief	Create button with icon
 * @param	pBtn		 - Pointer of button item
 * @param	nIconID		 - ID of button icon
 * @param	lpszBtnTitle - Title of button
 * @return	None
 */
void AppCore::DrawButton(CButton*& pBtn, unsigned nIconID, const wchar_t* buttonTitle /* = Constant::String::Empty */)
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
		wchar_t _tempBuffer[Constant::Max::StringLength] = { 0 };
		pBtn->GetWindowText(_tempBuffer, Constant::Max::StringLength);
		buttonTitleString.SetString(_tempBuffer);
	}

	// Update button
	pBtn->SetButtonStyle(BS_ICON);
	pBtn->SetIcon(hBtnIcon);
	pBtn->UpdateWindow();
}


/////////////////////////////////////////////////////////////////////////////
// Additional functions


/**
 * @brief	Get Windows OS build version
 * @param	None
 * @return	unsigned - Windows version macro
 */
unsigned AppCore::GetWindowsOSVersion(void)
{
	// Init info data
	OSVERSIONINFOEX oviOSVersion{};
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
	unsigned nRetWinVer = WINDOWS_VERSION_NONE;
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


/**
 * @brief	Play "BEEP" sound when sound is enabled
 * @param	bSoundEnable - Enable sound
 * @param	nTypeOfSound - Type of sound
 * @return	None
 */
void AppCore::PlaySound(bool bSoundEnable, unsigned nTypeOfSound)
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


/**
 * @brief	Open a file to view using external standard fileviewer
 * @param	eFileType - File type
 * @param	filePath  - Path of file
 * @return	bool - Result of file opening process
 */
bool AppCore::FileViewStd(FILETYPE eFileType, const wchar_t* filePath)
{
	String appPath = Constant::String::Empty;

	switch (eFileType) 
	{
	case FILETYPE_TEXT:
		appPath = Constant::Path::Notepad;
		break;
	case FILETYPE_IMAGE:
		break;
	default:
		return false;
	}

	// Run a file viewer instance
	HWND hWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	HINSTANCE hInstance = ShellExecute(hWnd, Constant::Command::Open, appPath, filePath, NULL, SW_SHOW);
	return (hInstance != NULL);
}


/**
 * @brief	Open web URL using default web browser
 * @param	webUrl - String of web URL
 * @return	bool - Result of web URL opening process
 */
bool AppCore::OpenWebURL(const wchar_t* webUrl)
{
	// Run a web browser instance
	HINSTANCE hInstance = ShellExecute(0, 0, webUrl, NULL, NULL, SW_NORMAL);
	return (hInstance != NULL);
}


/**
 * @brief	Run an application by specified path
 * @param	appPath		- Path of excutive file
 * @param	bRunAsAdmin	- Run as admin flag
 * @param	bShowFlag	- Show window flag
 * @return	LRESULT - Result of app launching process
 */
LRESULT AppCore::RunApp(const wchar_t* appPath, bool bRunAsAdmin /* = false */, bool bShowFlag /* = true */)
{
	// Param set
	String runAsFlag = (bRunAsAdmin) ? Constant::Command::RunAs : Constant::Command::Open;
	int nShowFlag = (bShowFlag) ? SW_SHOW : SW_HIDE;

	// Run an executable instance
	HINSTANCE hInstance = ShellExecute(NULL, runAsFlag, appPath, 0, 0, nShowFlag);
	return (LRESULT)(hInstance != NULL);
}


/**
 * @brief	Execute CMD command
 * @param	commandString - Command string
 * @param	bRunAsAdmin	  - Run as admin flag
 * @param	bShowFlag	  - Show window flag
 * @return	LRESULT - Result of command execution process
 */
LRESULT AppCore::ExecuteCommand(const wchar_t* commandString, bool bRunAsAdmin /* = true */, bool bShowFlag /* = true */)
{
	// Format input command
	String commandFormat = StringUtils::StringFormat(_T("/C %s"), commandString);

	// Flag param set
	String strRunAsFlag = (bRunAsAdmin) ? Constant::Command::RunAs : Constant::Command::Open;
	int nShowFlag = (bShowFlag) ? SW_SHOW : SW_HIDE;

	// Excute command
	HINSTANCE hInstance = ShellExecute(NULL, strRunAsFlag, Constant::Path::SystemCMD, commandFormat, 0, nShowFlag);
	return (LRESULT)(hInstance != NULL);
}


/**
 * @brief	Create app process
 * @param	appPath		 - App executable file path
 * @param	commandLine	 - Command line
 * @param	nStyle		 - App process style
 * @param	dwErrorCode	 - Returned error code
 * @return	bool
 */
bool AppCore::CreateAppProcess(const wchar_t* appPath, wchar_t* commandLine, unsigned nStyle, DWORD& dwErrorCode)
{
	// Startup info
	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO);

	// Process info
	PROCESS_INFORMATION ProcessInfo;
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));

	// Create process
	bool bResult = CreateProcess(appPath, commandLine, (LPSECURITY_ATTRIBUTES)NULL,
		(LPSECURITY_ATTRIBUTES)NULL, false, (DWORD)nStyle, NULL, NULL, &StartupInfo, &ProcessInfo);

	if (bResult == false) {
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


/**
 * @brief	Callback function used with the EnumFontFamiliesEx
 * @param	Default (see MSDN "EnumFontFamProc callback function")
 * @return	true/false
 */
static bool CALLBACK EnumFontFamiliesExProc(ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* /*lpntme*/, DWORD /*FontType*/, LPARAM lParam) 
{
	using wstring_vector = typename std::vector<std::wstring>;
	wstring_vector* fontNames = reinterpret_cast<wstring_vector*>(lParam);
	fontNames->push_back(lpelfe->elfLogFont.lfFaceName);
	return true;
}


/**
 * @brief	Enumerate all currently available fonts
 * @param	fontNames - Array to contain enumerated fonts
 * @return	true/false
 */
bool AppCore::EnumFontNames(std::vector<std::wstring>& fontNames)
{
	// Define temp font
	LOGFONT logfont = {0};
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


/**
 * @brief	Check if an input string is a valid font name
 * @param	fontName - Input font name
 * @return	true/false
 */
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

