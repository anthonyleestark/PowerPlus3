
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

#include "Core.h"


///////////////////////////////////////////////////////
//// Implementations


//////////////////////////////////////////////////////////////////////////
//
//	Define member functions of data structures
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another config data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagCONFIGDATA::Copy(const tagCONFIGDATA& pData)
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

BOOL tagCONFIGDATA::Compare(const tagCONFIGDATA& pData) const
{
	BOOL bRet = FALSE;

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
//	Function name:	tagPWRREPEATSET
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagPWRREPEATSET::tagPWRREPEATSET()
{
	// Init data
	this->bRepeat = FALSE;									// Repeat daily
	this->bAllowSnooze = TRUE;								// Allow snoozing mode
	this->nSnoozeInterval = DEF_REPEATSET_DEFAULT_SNOOZE;	// Snooze interval
	this->byRepeatDays = DEF_REPEATSET_DEFAULT_ACTIVEDAYS;	// Default repeat: All days of week
}

tagPWRREPEATSET::tagPWRREPEATSET(const tagPWRREPEATSET& pItem)
{
	// Copy data
	this->bRepeat = pItem.bRepeat;							// Repeat daily
	this->bAllowSnooze = pItem.bAllowSnooze;				// Allow snoozing mode
	this->nSnoozeInterval = pItem.nSnoozeInterval;			// Snooze interval
	this->byRepeatDays = pItem.byRepeatDays;				// Default repeat: All days of week
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagPWRREPEATSET& tagPWRREPEATSET::operator=(const tagPWRREPEATSET& pItem)
{
	// Copy data
	this->bRepeat = pItem.bRepeat;							// Repeat daily
	this->bAllowSnooze = pItem.bAllowSnooze;				// Allow snoozing mode
	this->nSnoozeInterval = pItem.nSnoozeInterval;			// Snooze interval
	this->byRepeatDays = pItem.byRepeatDays;				// Default repeat: All days of week

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

void tagPWRREPEATSET::Copy(const tagPWRREPEATSET& pItem)
{
	// Copy data
	this->bRepeat = pItem.bRepeat;							// Repeat daily
	this->bAllowSnooze = pItem.bAllowSnooze;				// Allow snoozing mode
	this->nSnoozeInterval = pItem.nSnoozeInterval;			// Snooze interval
	this->byRepeatDays = pItem.byRepeatDays;				// Days of week (for repeating)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare data with another Repeat set data
//  Arguments:		pItem - Pointer of input item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagPWRREPEATSET::Compare(const tagPWRREPEATSET& pItem) const
{
	BOOL bRetCompare = TRUE;

	// Compare data
	bRetCompare &= (this->bRepeat == pItem.bRepeat);					// Repeat daily
	bRetCompare &= (this->bAllowSnooze == pItem.bAllowSnooze);			// Allow snoozing mode
	bRetCompare &= (this->nSnoozeInterval == pItem.nSnoozeInterval);	// Snooze interval
	bRetCompare &= (this->byRepeatDays == pItem.byRepeatDays);			// Days of week (for repeating)

	return bRetCompare;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsDayActive
//	Description:	Check if day of week is active
//  Arguments:		dayOfWeek - Day of week
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagPWRREPEATSET::IsDayActive(DAYOFWEEK dayOfWeek) const
{
	// Invalid day of week
	if ((dayOfWeek < SUNDAY) || (dayOfWeek > SATURDAY))
		return FALSE;

	return ((this->byRepeatDays & (1 << dayOfWeek)) >> dayOfWeek);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagSCHEDULEITEM
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagSCHEDULEITEM::tagSCHEDULEITEM()
{
	// Initialize
	this->nItemID = DEF_SCHEDULE_MIN_ITEMID;			// Item ID
	this->bEnable = FALSE;								// Enable/disable status
	this->nAction = APP_ACTION_NOTHING;					// Schedule action
	this->stTime = SYSTEMTIME_ZERO;						// Schedule time
	this->rpsRepeatSet = PWRREPEATSET();				// Repeat set data
}

tagSCHEDULEITEM::tagSCHEDULEITEM(UINT nItemID)
{
	// Initialize
	this->nItemID = nItemID;							// Item ID
	this->bEnable = FALSE;								// Enable/disable status
	this->nAction = APP_ACTION_NOTHING;					// Schedule action
	this->stTime = SYSTEMTIME_ZERO;						// Schedule time
	this->rpsRepeatSet = PWRREPEATSET();				// Repeat set data
}

tagSCHEDULEITEM::tagSCHEDULEITEM(const tagSCHEDULEITEM& pItem)
{
	// Copy data
	this->nItemID = pItem.nItemID;						// Item ID
	this->bEnable = pItem.bEnable;						// Enable/disable status
	this->nAction = pItem.nAction;						// Schedule action
	this->stTime = pItem.stTime;						// Schedule time
	this->rpsRepeatSet.Copy(pItem.rpsRepeatSet);		// Repeat set data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagSCHEDULEITEM& tagSCHEDULEITEM::operator=(const tagSCHEDULEITEM& pItem)
{
	// Copy data
	this->nItemID = pItem.nItemID;						// Item ID
	this->bEnable = pItem.bEnable;						// Enable/disable status
	this->nAction = pItem.nAction;						// Schedule action
	this->stTime = pItem.stTime;						// Schedule time
	this->rpsRepeatSet.Copy(pItem.rpsRepeatSet);		// Repeat set data

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

void tagSCHEDULEITEM::Copy(const tagSCHEDULEITEM& pItem)
{
	// Copy data
	this->nItemID = pItem.nItemID;						// Item ID
	this->bEnable = pItem.bEnable;						// Enable/disable status
	this->nAction = pItem.nAction;						// Schedule action
	this->stTime = pItem.stTime;						// Schedule time
	this->rpsRepeatSet.Copy(pItem.rpsRepeatSet);		// Repeat set data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagSCHEDULEITEM::Compare(const tagSCHEDULEITEM& pItem) const
{
	BOOL bRet = FALSE;

	// Compare item (do not compare item ID)
	bRet &= (this->bEnable == pItem.bEnable);
	bRet &= (this->nAction == pItem.nAction);
	bRet &= (this->stTime.wHour == pItem.stTime.wHour);
	bRet &= (this->stTime.wMinute == pItem.stTime.wMinute);
	bRet &= (this->rpsRepeatSet.Compare(pItem.rpsRepeatSet));

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetActiveState
//	Description:	Set/change item active state
//  Arguments:		bActive - New active state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagSCHEDULEITEM::SetActiveState(BOOL bActive)
{
	this->bEnable = bActive;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if schedule item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagSCHEDULEITEM::IsEmpty(void) const
{
	// Initialize an empty item
	static const SCHEDULEITEM schDummyItem;

	// Compare with this item and return result
	return this->Compare(schDummyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsRepeatEnable
//	Description:	Check if repeat is enabled
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagSCHEDULEITEM::IsRepeatEnable(void) const
{
	return (this->rpsRepeatSet.bRepeat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsAllowSnoozing
//	Description:	Check if item snooze mode is available
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagSCHEDULEITEM::IsAllowSnoozing(void) const
{
	// If repeat option is currently OFF
	if (this->IsRepeatEnable() != TRUE) {
		// Not allow snooze mode
		return FALSE;
	}
	// If allow snooze option is OFF
	if (this->rpsRepeatSet.bAllowSnooze != TRUE) {
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

BOOL tagSCHEDULEITEM::IsDayActive(DAYOFWEEK dayOfWeek) const
{
	return (this->rpsRepeatSet.IsDayActive(dayOfWeek));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetActiveDays
//	Description:	Get repeat active days
//  Arguments:		None
//  Return value:	BYTE
//
//////////////////////////////////////////////////////////////////////////

BYTE tagSCHEDULEITEM::GetActiveDays(void) const
{
	return (this->rpsRepeatSet.byRepeatDays);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Print
//	Description:	Print schedule item
//  Arguments:		strOutput - Output printed string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagSCHEDULEITEM::Print(CString& strOutput)
{
	using namespace TableFuncs;
	using namespace CoreFuncs;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format schedule data
	CString strActive = (this->bEnable == TRUE) ? VALUE_TRUE : VALUE_FALSE;						// Enable/disable state
	UINT nActionStringID = GetPairedID(idTableActionName, this->nAction);
	CString strAction = GetLanguageString(ptrLanguage, nActionStringID);						// Schedule action
	CString strTimeFormat = FormatDispTime(ptrLanguage, IDS_FORMAT_SHORTTIME, this->stTime);	// Schedule time
	CString strRepeat = (this->rpsRepeatSet.bRepeat == TRUE) ? VALUE_TRUE : VALUE_FALSE;		// Repeat daily

	// Print item
	strOutput.Format(_T("Active=(%s), ItemID=%d, Action=(%s), Time=(%s), Repeat=(%s)"),
						strActive, this->nItemID, strAction, strTimeFormat, strRepeat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagSCHEDULEDATA
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagSCHEDULEDATA::tagSCHEDULEDATA()
{
	// Initialize
	this->schDefaultItem = SCHEDULEITEM(DEF_SCHEDULE_DEFAULT_ITEMID);
	this->arrSchedExtraItemList.RemoveAll();
}

tagSCHEDULEDATA::tagSCHEDULEDATA(const tagSCHEDULEDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy default item
	this->schDefaultItem.Copy(pData.schDefaultItem);

	// Copy extra data
	for (int nIndex = 0; nIndex < pData.GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM schItem = pData.arrSchedExtraItemList.GetAt(nIndex);
		this->arrSchedExtraItemList.Add(schItem);
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

tagSCHEDULEDATA& tagSCHEDULEDATA::operator=(const tagSCHEDULEDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy default item
	this->schDefaultItem.Copy(pData.schDefaultItem);

	// Copy extra data
	for (int nIndex = 0; nIndex < pData.GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM schItem = pData.arrSchedExtraItemList.GetAt(nIndex);
		this->arrSchedExtraItemList.Add(schItem);
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

void tagSCHEDULEDATA::Init()
{
	// Initialize
	this->schDefaultItem = SCHEDULEITEM(DEF_SCHEDULE_DEFAULT_ITEMID);
	this->arrSchedExtraItemList.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another Action Schedule data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagSCHEDULEDATA::Copy(const tagSCHEDULEDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy default item
	this->schDefaultItem.Copy(pData.schDefaultItem);

	// Copy extra data
	for (int nIndex = 0; nIndex < pData.GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM schItem = pData.arrSchedExtraItemList.GetAt(nIndex);
		this->arrSchedExtraItemList.Add(schItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add an Action Schedule item
//  Arguments:		pItem - Pointer of input item
//  Return value:	DWORD - Error code
//
//////////////////////////////////////////////////////////////////////////

DWORD tagSCHEDULEDATA::Add(const SCHEDULEITEM& pItem)
{
	// If item is empty, can not update
	if (pItem.IsEmpty())
		return DEF_SCHEDULE_ERROR_EMPTY;

	// If default item is currently empty
	if (this->schDefaultItem.IsEmpty()) {
		// Make item as default
		SCHEDULEITEM schDefault(pItem);
		schDefault.nItemID = DEF_SCHEDULE_DEFAULT_ITEMID;
		this->schDefaultItem.Copy(schDefault);
		return DEF_SCHEDULE_ERROR_SUCCESS;
	}

	// If extra schedule data is currently empty
	if (this->arrSchedExtraItemList.IsEmpty()) {
		// Just add the item
		this->arrSchedExtraItemList.Add(pItem);
		return DEF_SCHEDULE_ERROR_SUCCESS;
	}

	// If number of items exceeded limit
	if (this->GetExtraItemNum() >= DEF_SCHEDULE_ERROR_MAXITEM)
		return DEF_SCHEDULE_ERROR_MAXITEM;

	// Check if item is duplicated, if yes, do not add
	for (int nIndex = 0; nIndex < (this->GetExtraItemNum()); nIndex++) {
		SCHEDULEITEM pItemTemp = this->GetItemAt(nIndex);
		if (pItemTemp.Compare(pItem) == TRUE) {
			// All data is duplicated
			return DEF_SCHEDULE_ERROR_DUPLICATE;
		}
		else if (CoreFuncs::CheckTimeMatch(pItemTemp.stTime, pItem.stTime)) {
			// Time value is duplicated
			// Can not execute multiple action at the same time
			return DEF_SCHEDULE_ERROR_DUPLICATETIME;
		}
	}

	// Create new temporary data
	PSCHEDULEDATA pNew = new SCHEDULEDATA;
	pNew->arrSchedExtraItemList.RemoveAll();

	// Copy old data to new one
	pNew->schDefaultItem.Copy(this->schDefaultItem);
	for (int nIndex = 0; nIndex < this->GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM schItem = this->GetItemAt(nIndex);
		pNew->arrSchedExtraItemList.Add(schItem);
	}

	// Add new item and copy back to old data
	pNew->arrSchedExtraItemList.Add(pItem);
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

DWORD tagSCHEDULEDATA::Update(const SCHEDULEITEM& pItem)
{
	// If default item or extra schedule data is currently empty
	if ((this->GetDefaultItem().IsEmpty()) || (this->IsAllEmpty())) {
		// Just add item
		return this->Add(pItem);
	}

	// If item is empty, can not update
	if (pItem.IsEmpty())
		return DEF_SCHEDULE_ERROR_EMPTY;

	// If item ID is matching with default item
	if (pItem.nItemID == DEF_SCHEDULE_DEFAULT_ITEMID) {
		// Update default item
		this->GetDefaultItem().Copy(pItem);
		return DEF_SCHEDULE_ERROR_SUCCESS;
	}

	// Find extra item with matching ID
	int nRetItemIndex = INT_INVALID;
	for (int nIndex = 0; nIndex < this->GetExtraItemNum(); nIndex++) {
		if (this->GetItemAt(nIndex).nItemID == pItem.nItemID) {
			nRetItemIndex = nIndex;
			break;
		}
	}

	// Update item if found
	if (nRetItemIndex != INT_INVALID) {
		SCHEDULEITEM& schTemp = this->GetItemAt(nRetItemIndex);
		schTemp.Copy(pItem);
		return DEF_SCHEDULE_ERROR_SUCCESS;
	}
	// Otherwise,
	else {
		// Just add new
		return this->Add(pItem);
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

const SCHEDULEITEM& tagSCHEDULEDATA::GetDefaultItem(void) const
{
	return this->schDefaultItem;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDefaultItem
//	Description:	Get the default Action Schedule item
//  Arguments:		None
//  Return value:	SCHEDULEITEM
//
//////////////////////////////////////////////////////////////////////////

SCHEDULEITEM& tagSCHEDULEDATA::GetDefaultItem(void)
{
	return this->schDefaultItem;
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

const SCHEDULEITEM& tagSCHEDULEDATA::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < this->GetExtraItemNum()));
	if ((nIndex >= 0) && (nIndex < this->GetExtraItemNum()))
		return this->arrSchedExtraItemList.GetAt(nIndex);

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

SCHEDULEITEM& tagSCHEDULEDATA::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < this->GetExtraItemNum()));
	if ((nIndex >= 0) && (nIndex < this->GetExtraItemNum()))
		return this->arrSchedExtraItemList.GetAt(nIndex);

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

void tagSCHEDULEDATA::Remove(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= this->GetExtraItemNum()))
		return;

	// Get item data
	SCHEDULEITEM& schItem = this->GetItemAt(nAtIndex);

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

void tagSCHEDULEDATA::RemoveAll(void)
{
	// Remove each item
	for (int nIndex = 0; nIndex < this->GetExtraItemNum(); nIndex++) {
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

void tagSCHEDULEDATA::Adjust(void)
{
	// If default item is empty but extra data is not
	if ((this->IsDefaultEmpty()) && (this->IsExtraEmpty() == FALSE)) {
		// Make first extra item default
		this->schDefaultItem.Copy(this->GetItemAt(0));
		this->schDefaultItem.nItemID = DEF_SCHEDULE_DEFAULT_ITEMID;

		// Remove that extra item
		this->Delete(0);
	}

	// Check and remove empty extra items
	for (int nIndex = (this->GetExtraItemNum() - 1); nIndex >= 0; nIndex--) {
		SCHEDULEITEM schTemp = this->GetItemAt(nIndex);
		if (!schTemp.IsEmpty()) continue;

		// Remove item
		this->Delete(nIndex);
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

UINT tagSCHEDULEDATA::GetNextID(void)
{
	// Get currently max ID
	UINT nRetNextID = DEF_SCHEDULE_MIN_ITEMID;
	for (int nIndex = 0; nIndex < this->GetExtraItemNum(); nIndex++) {
		SCHEDULEITEM schItem = this->GetItemAt(nIndex);
		if (schItem.nItemID > nRetNextID) {
			nRetNextID = schItem.nItemID;
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

AFX_INLINE INT_PTR tagSCHEDULEDATA::GetExtraItemNum(void) const
{
	return this->arrSchedExtraItemList.GetSize();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsDefaultEmpty
//	Description:	Check if default item is empty
//  Arguments:		None
//  Return value:	BOOL - Result of empty or not
//
//////////////////////////////////////////////////////////////////////////

BOOL tagSCHEDULEDATA::IsDefaultEmpty(void) const
{
	return this->schDefaultItem.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if item at index is empty
//  Arguments:		nIndex - Item index to check
//  Return value:	BOOL - Result of empty or not
//
//////////////////////////////////////////////////////////////////////////

BOOL tagSCHEDULEDATA::IsEmpty(int nIndex) const
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= this->GetExtraItemNum()))
		return TRUE;

	// Check if item is empty
	SCHEDULEITEM schItem = this->GetItemAt(nIndex);
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

BOOL tagSCHEDULEDATA::IsExtraEmpty(void) const
{
	// If there's no item, return TRUE
	if (this->arrSchedExtraItemList.IsEmpty())
		return TRUE;

	// Check each item
	BOOL bExtraEmpty = TRUE;
	for (int nIndex = 0; nIndex < this->GetExtraItemNum(); nIndex++) {
		if (this->IsEmpty(nIndex) == FALSE) {
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

BOOL tagSCHEDULEDATA::IsAllEmpty(void) const
{
	return ((this->IsDefaultEmpty()) && (this->IsExtraEmpty()));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Delete
//	Description:	Delete a reminder item by index
//  Arguments:		nAtIndex - Index of item to delete
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagSCHEDULEDATA::Delete(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= this->GetExtraItemNum()))
		return;

	// Create new temporary data
	PSCHEDULEDATA pNew = new SCHEDULEDATA;
	pNew->arrSchedExtraItemList.RemoveAll();

	// Copy old data to new one (except the AtIndex item)
	pNew->schDefaultItem.Copy(this->schDefaultItem);
	for (int nIndex = 0; nIndex < this->GetExtraItemNum(); nIndex++) {
		if (nIndex == nAtIndex) continue;
		pNew->arrSchedExtraItemList.Add(this->arrSchedExtraItemList.GetAt(nIndex));
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

void tagSCHEDULEDATA::DeleteExtra(void)
{
	// Reset data
	this->arrSchedExtraItemList.RemoveAll();
	this->arrSchedExtraItemList.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DeleteAll
//	Description:	Delete all Action Schedule data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagSCHEDULEDATA::DeleteAll(void)
{
	// Reset data
	this->schDefaultItem = SCHEDULEITEM(DEF_SCHEDULE_DEFAULT_ITEMID);
	this->arrSchedExtraItemList.RemoveAll();
	this->arrSchedExtraItemList.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagHOTKEYSETITEM
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagHOTKEYSETITEM::tagHOTKEYSETITEM()
{
	// Initialize
	this->bEnable = FALSE;							// Hotkey enabled/disabled
	this->nHKActionID = 0;							// Hotkey action ID
	this->dwCtrlKeyCode = 0;						// Control Keycode #1
	this->dwFuncKeyCode = 0;						// Function Keycode #2
}

tagHOTKEYSETITEM::tagHOTKEYSETITEM(UINT nHKActionID)
{
	// Initialize
	this->bEnable = FALSE;							// Hotkey enabled/disabled
	this->nHKActionID = nHKActionID;				// Hotkey action ID
	this->dwCtrlKeyCode = 0;						// Control Keycode #1
	this->dwFuncKeyCode = 0;						// Function Keycode #2
}

tagHOTKEYSETITEM::tagHOTKEYSETITEM(const tagHOTKEYSETITEM& pItem)
{
	// Copy data
	this->bEnable = pItem.bEnable;					// Hotkey enabled/disabled
	this->nHKActionID = pItem.nHKActionID;			// Hotkey action ID
	this->dwCtrlKeyCode = pItem.dwCtrlKeyCode;		// Control Keycode #1
	this->dwFuncKeyCode = pItem.dwFuncKeyCode;		// Function Keycode #2
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagHOTKEYSETITEM& tagHOTKEYSETITEM::operator=(const tagHOTKEYSETITEM& pItem)
{
	// Copy data
	this->bEnable = pItem.bEnable;					// Hotkey enabled/disabled
	this->nHKActionID = pItem.nHKActionID;			// Hotkey action ID
	this->dwCtrlKeyCode = pItem.dwCtrlKeyCode;		// Control Keycode #1
	this->dwFuncKeyCode = pItem.dwFuncKeyCode;		// Function Keycode #2

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

void tagHOTKEYSETITEM::Copy(const tagHOTKEYSETITEM& pItem)
{
	// Copy data
	this->bEnable = pItem.bEnable;					// Hotkey enabled/disabled
	this->nHKActionID = pItem.nHKActionID;			// Hotkey action ID
	this->dwCtrlKeyCode = pItem.dwCtrlKeyCode;		// Control Keycode #1
	this->dwFuncKeyCode = pItem.dwFuncKeyCode;		// Function Keycode #2
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if HotkeySet item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagHOTKEYSETITEM::IsEmpty(void) const
{
	BOOL bIsEmpty = FALSE;

	// Check if keystroke value is empty
	bIsEmpty &= (this->dwCtrlKeyCode == 0);
	bIsEmpty &= (this->dwFuncKeyCode == 0);

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

BOOL tagHOTKEYSETITEM::Compare(const tagHOTKEYSETITEM& pItem) const
{
	BOOL bRet = FALSE;

	// Compare item
	bRet &= (this->nHKActionID == pItem.nHKActionID);
	bRet &= (this->dwCtrlKeyCode == pItem.dwCtrlKeyCode);
	bRet &= (this->dwFuncKeyCode == pItem.dwFuncKeyCode);

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

void tagHOTKEYSETITEM::Print(CString& strOutput)
{
	using namespace TableFuncs;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	CString strEnable = (this->bEnable == TRUE) ? _T("Enabled") : _T("Disabled");
	UINT nActionNameID = GetPairedID(idTableActionName, GetPairedID(idTableHKActionID, this->nHKActionID));
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

void tagHOTKEYSETITEM::PrintKeyStrokes(CString& strOutput)
{
	using namespace TableFuncs;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format keystrokes
	CString strKeyStrokes = STRING_EMPTY;
	if (this->dwCtrlKeyCode & MOD_CONTROL)	strKeyStrokes += _T("Ctrl + ");
	if (this->dwCtrlKeyCode & MOD_ALT)		strKeyStrokes += _T("Alt + ");
	if (this->dwCtrlKeyCode & MOD_WIN)		strKeyStrokes += _T("Win + ");
	strKeyStrokes += GetString(strTableFuncKeyList, this->dwFuncKeyCode);

	// Output string
	strOutput.Empty();
	strOutput = strKeyStrokes;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagHOTKEYSETDATA
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagHOTKEYSETDATA::tagHOTKEYSETDATA()
{
	// Initialize
	this->arrHotkeySetList.RemoveAll();
}

tagHOTKEYSETDATA::tagHOTKEYSETDATA(const tagHOTKEYSETDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksItem = pData.arrHotkeySetList.GetAt(nIndex);
		this->arrHotkeySetList.Add(hksItem);
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

tagHOTKEYSETDATA& tagHOTKEYSETDATA::operator=(const tagHOTKEYSETDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksItem = pData.arrHotkeySetList.GetAt(nIndex);
		this->arrHotkeySetList.Add(hksItem);
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

void tagHOTKEYSETDATA::Init()
{
	// Initialize
	this->arrHotkeySetList.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another HotkeySet data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagHOTKEYSETDATA::Copy(const tagHOTKEYSETDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksItem = pData.arrHotkeySetList.GetAt(nIndex);
		this->arrHotkeySetList.Add(hksItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add a hotkeyset item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagHOTKEYSETDATA::Add(const HOTKEYSETITEM& pItem)
{
	// If data list is current empty
	if (this->arrHotkeySetList.IsEmpty()) {
		// Just add item
		this->arrHotkeySetList.Add(pItem);
		return;
	}

	// Check if item exists, if yes, do not add
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		HOTKEYSETITEM pItemTemp = this->GetItemAt(nIndex);
		if (pItemTemp.Compare(pItem) == TRUE)
			return;
	}

	// Create new temporary data
	PHOTKEYSETDATA pNew = new HOTKEYSETDATA;
	pNew->arrHotkeySetList.RemoveAll();
	
	// Copy old data to new one
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksItem = this->GetItemAt(nIndex);
		pNew->arrHotkeySetList.Add(hksItem);
	}

	// Add new item and copy back to old data
	pNew->arrHotkeySetList.Add(pItem);
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

void tagHOTKEYSETDATA::Update(const HOTKEYSETITEM& pItem)
{
	// If data list is current empty
	if (this->arrHotkeySetList.IsEmpty()) {
		// Just add item
		this->Add(pItem);
		return;
	}

	// Check if item with same action ID or keystrokes exists
	int nDupActionIndex = INT_INVALID;
	int nDupKeyIndex = INT_INVALID;

	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksTemp = this->GetItemAt(nIndex);
		if (hksTemp.nHKActionID == pItem.nHKActionID) {
			// Duplicate action ID found
			nDupActionIndex = nIndex;
		}
		if ((hksTemp.dwCtrlKeyCode == pItem.dwCtrlKeyCode) &&
			(hksTemp.dwFuncKeyCode == pItem.dwFuncKeyCode)) {
			// Duplicate keystrokes found
			nDupKeyIndex = nIndex;
		}
	}

	// If same item existed (same action ID and same keystrokes), update its state
	if (nDupActionIndex == nDupKeyIndex) {
		HOTKEYSETITEM& hksTemp = this->GetItemAt(nDupActionIndex);
		hksTemp.bEnable = pItem.bEnable;
		return;
	}

	// Delete existed duplicate keystrokes
	if (nDupKeyIndex != INT_INVALID) {
		this->Remove(nDupKeyIndex);
	}

	// If item with same action ID existed, update its data
	if (nDupActionIndex != INT_INVALID) {
		HOTKEYSETITEM& hksTemp = this->GetItemAt(nDupActionIndex);
		hksTemp.Copy(pItem);
	}
	// Otherwise, add new
	else {
		this->Add(pItem);
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

const HOTKEYSETITEM& tagHOTKEYSETDATA::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < this->GetItemNum()));
	if ((nIndex >= 0) && (nIndex < this->GetItemNum()))
		return this->arrHotkeySetList.GetAt(nIndex);

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

HOTKEYSETITEM& tagHOTKEYSETDATA::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < this->GetItemNum()));
	if ((nIndex >= 0) && (nIndex < this->GetItemNum()))
		return this->arrHotkeySetList.GetAt(nIndex);

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

void tagHOTKEYSETDATA::Remove(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= this->GetItemNum()))
		return;

	// Get item data
	HOTKEYSETITEM& hksItem = this->GetItemAt(nAtIndex);

	// Reset item value
	hksItem.bEnable = FALSE;
	hksItem.dwCtrlKeyCode = 0;
	hksItem.dwFuncKeyCode = 0;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all HotkeySet data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagHOTKEYSETDATA::RemoveAll(void)
{
	// Remove each item
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
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

void tagHOTKEYSETDATA::Adjust(void)
{
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {

		// Get hotkeyset item
		HOTKEYSETITEM& hksItem = this->GetItemAt(nIndex);

		// Not enable hotkeyset item if no keystroke data
		if ((hksItem.dwCtrlKeyCode == 0) || (hksItem.dwFuncKeyCode == 0)) {
			hksItem.bEnable = FALSE;
			hksItem.dwCtrlKeyCode = 0;
			hksItem.dwFuncKeyCode = 0;
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

AFX_INLINE INT_PTR tagHOTKEYSETDATA::GetItemNum(void) const
{
	return this->arrHotkeySetList.GetSize();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if item at index is empty
//  Arguments:		nIndex - Item index to check
//  Return value:	BOOL - Result of empty or not
//
//////////////////////////////////////////////////////////////////////////

BOOL tagHOTKEYSETDATA::IsEmpty(int nIndex) const
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= this->GetItemNum()))
		return TRUE;

	// Get item data
	HOTKEYSETITEM hksItem = this->GetItemAt(nIndex);

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

BOOL tagHOTKEYSETDATA::IsAllEmpty() const
{
	// If there's no item, return TRUE
	if (this->arrHotkeySetList.IsEmpty())
		return TRUE;

	// Check each item
	BOOL bAllEmpty = TRUE;
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		if (this->IsEmpty(nIndex) == FALSE) {
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

void tagHOTKEYSETDATA::Delete(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= this->GetItemNum()))
		return;

	// Create new temporary data
	PHOTKEYSETDATA pNew = new HOTKEYSETDATA;
	pNew->arrHotkeySetList.RemoveAll();

	// Copy old data to new one (except the AtIndex item)
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		if (nIndex == nAtIndex) continue;
		pNew->arrHotkeySetList.Add(this->GetItemAt(nIndex));
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

void tagHOTKEYSETDATA::DeleteAll(void)
{
	// Reset data
	this->arrHotkeySetList.RemoveAll();
	this->arrHotkeySetList.FreeExtra();
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

void tagHOTKEYSETDATA::PrintKeyStrokes(UINT nHKID, CString& strOutput)
{
	// Search for hotkey ID and get keystrokes string
	CString strKeyStrokes = STRING_EMPTY;
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		HOTKEYSETITEM hksItem = this->GetItemAt(nIndex);
		if (hksItem.nHKActionID == nHKID) {
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
//	Function name:	tagRMDMSGSTYLESET
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagRMDMSGSTYLESET::tagRMDMSGSTYLESET()
{
	// Init data
	this->colorBkgrd = DEFAULT_MSGBKGRDCLR;						// Background color
	this->colorText = DEFAULT_MSGTEXTCLR;						// Text color
	this->strFontName = DEFAULT_MSGFONTNAME;					// Font name
	this->uiFontSize = DEFAULT_MSGFONTSIZE;						// Font size
	this->uiTimeout = DEFAULT_MSGTIMEOUT;						// Timeout (auto-close) interval
	this->uiIconID = DEFAULT_MSGICONID;							// Message icon ID
	this->nIconSize = DEFAULT_MSGICONSIZE;						// Message icon size
	this->byIconPos = DEFAULT_MSGICONPOS;						// Message icon position
	this->byDisplayPos = DEFAULT_MSGDISPPOS;					// Message display position
	this->uiHMargin = DEFAULT_MSGHMARGIN;						// Display area horizontal margin
	this->uiVMargin = DEFAULT_MSGVMARGIN;						// Display area vertical margin
}

tagRMDMSGSTYLESET::tagRMDMSGSTYLESET(const tagRMDMSGSTYLESET& pItem)
{
	// Copy data
	this->colorBkgrd = pItem.colorBkgrd;						// Background color
	this->colorText = pItem.colorText;							// Text color
	this->strFontName = pItem.strFontName;						// Font name
	this->uiFontSize = pItem.uiFontSize;						// Font size
	this->uiTimeout = pItem.uiTimeout;							// Timeout (auto-close) interval
	this->uiIconID = pItem.uiIconID;							// Message icon ID
	this->nIconSize = pItem.nIconSize;							// Message icon size
	this->byIconPos = pItem.byIconPos;							// Message icon position
	this->byDisplayPos = pItem.byDisplayPos;					// Message display position
	this->uiHMargin = pItem.uiHMargin;							// Display area horizontal margin
	this->uiVMargin = pItem.uiVMargin;							// Display area vertical margin
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagRMDMSGSTYLESET& tagRMDMSGSTYLESET::operator=(const tagRMDMSGSTYLESET& pItem)
{
	// Copy data
	this->colorBkgrd = pItem.colorBkgrd;						// Background color
	this->colorText = pItem.colorText;							// Text color
	this->strFontName = pItem.strFontName;						// Font name
	this->uiFontSize = pItem.uiFontSize;						// Font size
	this->uiTimeout = pItem.uiTimeout;							// Timeout (auto-close) interval
	this->uiIconID = pItem.uiIconID;							// Message icon ID
	this->nIconSize = pItem.nIconSize;							// Message icon size
	this->byIconPos = pItem.byIconPos;							// Message icon position
	this->byDisplayPos = pItem.byDisplayPos;					// Message display position
	this->uiHMargin = pItem.uiHMargin;							// Display area horizontal margin
	this->uiVMargin = pItem.uiVMargin;							// Display area vertical margin

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

void tagRMDMSGSTYLESET::Copy(const tagRMDMSGSTYLESET& pItem)
{
	// Copy data
	this->colorBkgrd = pItem.colorBkgrd;						// Background color
	this->colorText = pItem.colorText;							// Text color
	this->strFontName = pItem.strFontName;						// Font name
	this->uiFontSize = pItem.uiFontSize;						// Font size
	this->uiTimeout = pItem.uiTimeout;							// Timeout (auto-close) interval
	this->uiIconID = pItem.uiIconID;							// Message icon ID
	this->nIconSize = pItem.nIconSize;							// Message icon size
	this->byIconPos = pItem.byIconPos;							// Message icon position
	this->byDisplayPos = pItem.byDisplayPos;					// Message display position
	this->uiHMargin = pItem.uiHMargin;							// Display area horizontal margin
	this->uiVMargin = pItem.uiVMargin;							// Display area vertical margin
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare data with another Repeat set data
//  Arguments:		pItem - Pointer of input item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagRMDMSGSTYLESET::Compare(const tagRMDMSGSTYLESET& pItem) const
{
	BOOL bRetCompare = TRUE;

	// Compare data
	bRetCompare &= (this->colorBkgrd == pItem.colorBkgrd);		// Background color
	bRetCompare &= (this->colorText == pItem.colorText);		// Text color
	bRetCompare &= (this->strFontName == pItem.strFontName);	// Font name
	bRetCompare &= (this->uiFontSize == pItem.uiFontSize);		// Font size
	bRetCompare &= (this->uiTimeout == pItem.uiTimeout);		// Timeout (auto-close) interval
	bRetCompare &= (this->uiIconID == pItem.uiIconID);			// Message icon ID
	bRetCompare &= (this->nIconSize == pItem.nIconSize);		// Message icon size
	bRetCompare &= (this->byIconPos == pItem.byIconPos);		// Message icon position
	bRetCompare &= (this->byDisplayPos == pItem.byDisplayPos);	// Message display position
	bRetCompare &= (this->uiHMargin == pItem.uiHMargin);		// Display area horizontal margin
	bRetCompare &= (this->uiVMargin == pItem.uiVMargin);		// Display area vertical margin

	return bRetCompare;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagPWRREMINDERITEM
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagPWRREMINDERITEM::tagPWRREMINDERITEM()
{
	// Init data
	this->bEnable = FALSE;								// Enable state
	this->nItemID = DEF_PWRREMINDER_MIN_ITEMID;			// Item ID
	this->strMessage = STRING_EMPTY;					// Message content
	this->nEventID = PREVT_AT_SETTIME;					// Event ID
	this->stTime = SYSTEMTIME_ZERO;						// Event time
	this->dwMsgStyle = PRSTYLE_MSGBOX;					// Reminder style
	this->rpsRepeatSet = PWRREPEATSET();				// Repeat set
	this->bUseCustomStyle = FALSE;						// Use message custom style
	this->rmsMsgStyleSet = RMDMSGSTYLESET();			// Reminder message style set
}

tagPWRREMINDERITEM::tagPWRREMINDERITEM(const tagPWRREMINDERITEM& pItem)
{
	// Copy data
	this->bEnable = pItem.bEnable;						// Enable state
	this->nItemID = pItem.nItemID;						// Item ID
	this->strMessage = pItem.strMessage;				// Message content
	this->nEventID = pItem.nEventID;					// Event ID
	this->stTime = pItem.stTime;						// Event time
	this->dwMsgStyle = pItem.dwMsgStyle;				// Reminder style
	this->rpsRepeatSet.Copy(pItem.rpsRepeatSet);		// Repeat set
	this->bUseCustomStyle = pItem.bUseCustomStyle;		// Use message custom style
	this->rmsMsgStyleSet.Copy(pItem.rmsMsgStyleSet);	// Reminder message style set
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagPWRREMINDERITEM& tagPWRREMINDERITEM::operator=(const tagPWRREMINDERITEM& pItem)
{
	// Copy data
	this->bEnable = pItem.bEnable;						// Enable state
	this->nItemID = pItem.nItemID;						// Item ID
	this->strMessage = pItem.strMessage;				// Message content
	this->nEventID = pItem.nEventID;					// Event ID
	this->stTime = pItem.stTime;						// Event time
	this->dwMsgStyle = pItem.dwMsgStyle;				// Reminder style
	this->rpsRepeatSet.Copy(pItem.rpsRepeatSet);		// Repeat set
	this->bUseCustomStyle = pItem.bUseCustomStyle;		// Use message custom style
	this->rmsMsgStyleSet.Copy(pItem.rmsMsgStyleSet);	// Reminder message style set

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

void tagPWRREMINDERITEM::Copy(const tagPWRREMINDERITEM& pItem)
{
	// Copy data
	this->bEnable = pItem.bEnable;						// Enable state
	this->nItemID = pItem.nItemID;						// Item ID
	this->strMessage = pItem.strMessage;				// Message content
	this->nEventID = pItem.nEventID;					// Event ID
	this->stTime = pItem.stTime;						// Event time
	this->dwMsgStyle = pItem.dwMsgStyle;				// Reminder style
	this->rpsRepeatSet.Copy(pItem.rpsRepeatSet);		// Repeat set
	this->bUseCustomStyle = pItem.bUseCustomStyle;		// Use message custom style
	this->rmsMsgStyleSet.Copy(pItem.rmsMsgStyleSet);	// Reminder message style set
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if Power Reminder item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagPWRREMINDERITEM::IsEmpty() const
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

BOOL tagPWRREMINDERITEM::Compare(const tagPWRREMINDERITEM& pItem) const
{
	BOOL bRet = TRUE;

	// Compare item (do not compare item ID)
	bRet &= (this->strMessage == pItem.strMessage);
	bRet &= (this->nEventID == pItem.nEventID);
	bRet &= (this->stTime.wHour == pItem.stTime.wHour);
	bRet &= (this->stTime.wMinute == pItem.stTime.wMinute);
	bRet &= (this->dwMsgStyle == pItem.dwMsgStyle);
	bRet &= (this->rpsRepeatSet.Compare(pItem.rpsRepeatSet));
	bRet &= (this->rmsMsgStyleSet.Compare(pItem.rmsMsgStyleSet));

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetEnableState
//	Description:	Set/change item enable state
//  Arguments:		bEnable - New enable state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagPWRREMINDERITEM::SetEnableState(BOOL bEnable)
{
	this->bEnable = bEnable;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsRepeatEnable
//	Description:	Check if item repeat mode is enabled
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL tagPWRREMINDERITEM::IsRepeatEnable(void) const
{
	return (this->rpsRepeatSet.bRepeat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsDayActive
//	Description:	Check if day of week is active
//  Arguments:		dayOfWeek - Day of week
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagPWRREMINDERITEM::IsDayActive(DAYOFWEEK dayOfWeek) const
{
	return (this->rpsRepeatSet.IsDayActive(dayOfWeek));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsAllowSnoozing
//	Description:	Check if item snooze mode is available
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagPWRREMINDERITEM::IsAllowSnoozing(void) const
{
	// If current eventID is not at settime
	if (this->nEventID != PREVT_AT_SETTIME) {
		// Not allow snooze mode
		return FALSE;
	}
	// If repeat option is currently OFF
	if (this->IsRepeatEnable() != TRUE) {
		// Not allow snooze mode
		return FALSE;
	}
	// If allow snooze option is OFF
	if (this->rpsRepeatSet.bAllowSnooze != TRUE) {
		// Not allow snooze mode
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetActiveDays
//	Description:	Get repeat active days
//  Arguments:		None
//  Return value:	BYTE
//
//////////////////////////////////////////////////////////////////////////

BYTE tagPWRREMINDERITEM::GetActiveDays(void) const
{
	return (this->rpsRepeatSet.byRepeatDays);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Print
//	Description:	Print reminder item
//  Arguments:		strOutput - Output printed string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagPWRREMINDERITEM::Print(CString& strOutput)
{
	using namespace TableFuncs;
	using namespace CoreFuncs;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	CString strEnable = (this->bEnable == TRUE) ? _T("Enabled") : _T("Disabled");
	CString strMsg = this->strMessage;
	if (strMsg.GetLength() > (MAX_DISP_LOGSTRING_LENGTH + 3)) {
		strMsg = this->strMessage.Left(MAX_DISP_LOGSTRING_LENGTH) + _T("...");
	}
	int nTemp = GetPairedID(idTablePwrReminderEvt, this->nEventID);
	CString strEvent = GetLanguageString(ptrLanguage, nTemp);
	if (this->nEventID == PREVT_AT_SETTIME) {
		// Format time string
		CString strFormat = strEvent;
		strEvent = FormatDispTime(ptrLanguage, strFormat, this->stTime);
	}
	nTemp = GetPairedID(idTablePwrReminderStyle, this->dwMsgStyle);
	CString strStyle = GetLanguageString(ptrLanguage, nTemp);

	// Print item
	strOutput.Format(_T("State=(%s), ItemID=%d, Msg=(%s), Event=(%s), Style=(%s), Repeat=%d"),
				strEnable, this->nItemID, strMsg, strEvent, strStyle, this->rpsRepeatSet.bRepeat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagPWRREMINDERDATA
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagPWRREMINDERDATA::tagPWRREMINDERDATA()
{
	// Initialize
	this->arrRmdItemList.RemoveAll();
	this->rmdCommonStyle = RMDMSGSTYLESET();
}

tagPWRREMINDERDATA::tagPWRREMINDERDATA(const tagPWRREMINDERDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy reminder data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		PWRREMINDERITEM pwrItem = pData.arrRmdItemList.GetAt(nIndex);
		this->arrRmdItemList.Add(pwrItem);
	}

	// Copy common message style data
	this->rmdCommonStyle.Copy(pData.rmdCommonStyle);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagPWRREMINDERDATA& tagPWRREMINDERDATA::operator=(const tagPWRREMINDERDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy reminder data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		PWRREMINDERITEM pwrItem = pData.arrRmdItemList.GetAt(nIndex);
		this->arrRmdItemList.Add(pwrItem);
	}

	// Copy common message style data
	this->rmdCommonStyle.Copy(pData.rmdCommonStyle);

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

void tagPWRREMINDERDATA::Init()
{
	// Initialize
	this->arrRmdItemList.RemoveAll();
	this->rmdCommonStyle = RMDMSGSTYLESET();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another Power Reminder data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagPWRREMINDERDATA::Copy(const tagPWRREMINDERDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy reminder data
	for (int nIndex = 0; nIndex < pData.GetItemNum(); nIndex++) {
		PWRREMINDERITEM pwrItem = pData.arrRmdItemList.GetAt(nIndex);
		this->arrRmdItemList.Add(pwrItem);
	}

	// Copy common message style data
	this->rmdCommonStyle.Copy(pData.rmdCommonStyle);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add a Power Reminder item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagPWRREMINDERDATA::Add(const PWRREMINDERITEM& pItem)
{
	// If data list is current empty
	if (this->arrRmdItemList.IsEmpty()) {
		// Just add item
		this->arrRmdItemList.Add(pItem);
		return;
	}

	// Check if item exists, if yes, do not add
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		PWRREMINDERITEM pItemTemp = this->GetItemAt(nIndex);
		if (pItemTemp.Compare(pItem) == TRUE)
			return;
	}

	// Create new temporary data
	PPWRREMINDERDATA pNew = new PWRREMINDERDATA;
	pNew->arrRmdItemList.RemoveAll();

	// Copy common message style data
	pNew->rmdCommonStyle.Copy(this->rmdCommonStyle);

	// Copy old data to new one
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		PWRREMINDERITEM pwrItem = this->GetItemAt(nIndex);
		pNew->arrRmdItemList.Add(pwrItem);
	}

	// Add new item and copy back to old data
	pNew->arrRmdItemList.Add(pItem);
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

void tagPWRREMINDERDATA::Update(const PWRREMINDERITEM& pItem)
{
	// If data list is current empty
	if (this->arrRmdItemList.IsEmpty()) {
		// Just add item
		this->Add(pItem);
		return;
	}

	// Find item index
	int nRetItemIndex = INT_INVALID;
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		if (this->GetItemAt(nIndex).nItemID == pItem.nItemID) {
			nRetItemIndex = nIndex;
			break;
		}
	}

	// Update item if found
	if (nRetItemIndex != INT_INVALID) {
		PWRREMINDERITEM& pwrTemp = this->GetItemAt(nRetItemIndex);
		pwrTemp.Copy(pItem);
	}
	// Otherwise, add new
	else {
		this->Add(pItem);
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

const PWRREMINDERITEM& tagPWRREMINDERDATA::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < this->GetItemNum()));
	if ((nIndex >= 0) && (nIndex < this->GetItemNum()))
		return this->arrRmdItemList.GetAt(nIndex);

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

PWRREMINDERITEM& tagPWRREMINDERDATA::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < this->GetItemNum()));
	if ((nIndex >= 0) && (nIndex < this->GetItemNum()))
		return this->arrRmdItemList.GetAt(nIndex);

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

void tagPWRREMINDERDATA::Remove(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= this->GetItemNum()))
		return;

	// Get item data
	PWRREMINDERITEM& pwrItem = this->GetItemAt(nAtIndex);

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

void tagPWRREMINDERDATA::RemoveAll(void)
{
	// Remove each item
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
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

void tagPWRREMINDERDATA::Adjust(void)
{
	// Remove garbage items
	for (int nIndex = (this->GetItemNum() - 1); nIndex >= 0; nIndex--) {
		// Get item
		PWRREMINDERITEM pwrTemp = this->GetItemAt(nIndex);
		if (!pwrTemp.IsEmpty()) continue;

		// Remove empty if item
		this->Delete(nIndex);
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

UINT tagPWRREMINDERDATA::GetNextID(void)
{
	// Get max ID
	UINT nRetNextID = DEF_PWRREMINDER_MIN_ITEMID;
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		PWRREMINDERITEM pwrItem = this->GetItemAt(nIndex);
		if (pwrItem.nItemID > nRetNextID) {
			nRetNextID = pwrItem.nItemID;
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

AFX_INLINE INT_PTR tagPWRREMINDERDATA::GetItemNum(void) const
{
	return this->arrRmdItemList.GetSize();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if item at index is empty
//  Arguments:		nIndex - Item index to check
//  Return value:	BOOL - Result of empty or not
//
//////////////////////////////////////////////////////////////////////////

BOOL tagPWRREMINDERDATA::IsEmpty(int nIndex) const
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= this->GetItemNum()))
		return TRUE;

	// Check if item is empty
	PWRREMINDERITEM pwrItem = this->GetItemAt(nIndex);
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

BOOL tagPWRREMINDERDATA::IsAllEmpty() const
{
	// If there's no item, return TRUE
	if (this->arrRmdItemList.IsEmpty())
		return TRUE;

	// Check each item
	BOOL bAllEmpty = TRUE;
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		if (this->IsEmpty(nIndex) == FALSE) {
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

void tagPWRREMINDERDATA::Delete(int nAtIndex)
{
	// Check index validity
	if ((nAtIndex < 0) || (nAtIndex >= this->GetItemNum()))
		return;

	// Create new temporary data
	PPWRREMINDERDATA pNew = new PWRREMINDERDATA;
	pNew->arrRmdItemList.RemoveAll();

	// Copy common message style data
	pNew->rmdCommonStyle.Copy(this->rmdCommonStyle);

	// Copy old data to new one (except the AtIndex item)
	for (int nIndex = 0; nIndex < this->GetItemNum(); nIndex++) {
		if (nIndex == nAtIndex) continue;
		pNew->arrRmdItemList.Add(this->arrRmdItemList.GetAt(nIndex));
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

void tagPWRREMINDERDATA::DeleteAll(void)
{
	// Reset data
	this->arrRmdItemList.RemoveAll();
	this->arrRmdItemList.FreeExtra();
	this->rmdCommonStyle = RMDMSGSTYLESET();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagPWRRUNTIMEITEM
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagPWRRUNTIMEITEM::tagPWRRUNTIMEITEM()
{
	// Init data
	this->nCategory = INT_INVALID;							// Item category
	this->nItemID = INT_NULL;								// Power Reminder item ID
	this->nDisplayFlag = FLAG_OFF;							// Item displaying flag
	this->nSkipFlag = FLAG_OFF;								// Item skip flag
	this->nSnoozeFlag = FLAG_OFF;							// Item snooze trigger flag
	this->stNextSnoozeTime = SYSTEMTIME_ZERO;				// Next snooze trigger time
}

tagPWRRUNTIMEITEM::tagPWRRUNTIMEITEM(const tagPWRRUNTIMEITEM& pItem)
{
	// Copy data
	this->nCategory = pItem.nCategory;						// Item category
	this->nItemID = pItem.nItemID;							// Power Reminder item ID
	this->nDisplayFlag = pItem.nDisplayFlag;				// Item displaying flag
	this->nSkipFlag = pItem.nSkipFlag;						// Item skip flag
	this->nSnoozeFlag = pItem.nSnoozeFlag;					// Item snooze trigger flag
	this->stNextSnoozeTime = pItem.stNextSnoozeTime;		// Next snooze trigger time
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagPWRRUNTIMEITEM& tagPWRRUNTIMEITEM::operator=(const tagPWRRUNTIMEITEM& pItem)
{
	// Copy data
	this->nCategory = pItem.nCategory;						// Item category
	this->nItemID = pItem.nItemID;							// Power Reminder item ID
	this->nDisplayFlag = pItem.nDisplayFlag;				// Item displaying flag
	this->nSkipFlag = pItem.nSkipFlag;						// Item skip flag
	this->nSnoozeFlag = pItem.nSnoozeFlag;					// Item snooze trigger flag
	this->stNextSnoozeTime = pItem.stNextSnoozeTime;		// Next snooze trigger time

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

void tagPWRRUNTIMEITEM::Copy(const tagPWRRUNTIMEITEM& pItem)
{
	// Copy data
	this->nCategory = pItem.nCategory;						// Item category
	this->nItemID = pItem.nItemID;							// Item ID
	this->nDisplayFlag = pItem.nDisplayFlag;				// Item displaying flag
	this->nSkipFlag = pItem.nSkipFlag;						// Item skip flag
	this->nSnoozeFlag = pItem.nSnoozeFlag;					// Item snooze trigger flag
	this->stNextSnoozeTime = pItem.stNextSnoozeTime;		// Next snooze trigger time
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CalcNextSnoozeTime
//	Description:	Calculate Power Reminder item next snooze time
//  Arguments:		nInterval - Snooze interval
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagPWRRUNTIMEITEM::CalcNextSnoozeTime(int nInterval)
{
	// Calculate time with offset
	CoreFuncs::CalcTimeOffset(this->stNextSnoozeTime, nInterval);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagHISTORYINFODATA
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagHISTORYINFODATA::tagHISTORYINFODATA()
{
	// Init data
	this->bInitState = FALSE;								// Init state
	this->nCategoryID = INT_NULL;							// Category ID
	this->stTimestamp = SYSTEMTIME_ZERO;					// Timestamp of history
	this->nItemID = INT_NULL;								// Item ID
	this->nActionNameID = INT_NULL;							// Name of action (string ID)
	this->bActionResult = FALSE;							// Action result
	this->dwErrorCode = INT_NULL;							// Returned error code
	this->strDescription = STRING_EMPTY;					// History description (attached info)
}

tagHISTORYINFODATA::tagHISTORYINFODATA(const tagHISTORYINFODATA& pData)
{
	// Copy data
	this->bInitState = pData.bInitState;					// Init state
	this->nCategoryID = pData.nCategoryID;					// Category ID
	this->stTimestamp = pData.stTimestamp;					// Timestamp of history
	this->nItemID = pData.nItemID;							// Item ID
	this->nActionNameID = pData.nActionNameID;				// Name of action (string ID)
	this->bActionResult = pData.bActionResult;				// Action result
	this->dwErrorCode = pData.dwErrorCode;					// Returned error code
	this->strDescription = pData.strDescription;			// History description (attached info)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagHISTORYINFODATA& tagHISTORYINFODATA::operator=(const tagHISTORYINFODATA& pData)
{
	// Copy data
	this->bInitState = pData.bInitState;					// Init state
	this->nCategoryID = pData.nCategoryID;					// Category ID
	this->stTimestamp = pData.stTimestamp;					// Timestamp of history
	this->nItemID = pData.nItemID;							// Item ID
	this->nActionNameID = pData.nActionNameID;				// Name of action (string ID)
	this->bActionResult = pData.bActionResult;				// Action result
	this->dwErrorCode = pData.dwErrorCode;					// Returned error code
	this->strDescription = pData.strDescription;			// History description (attached info)

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

void tagHISTORYINFODATA::Copy(const tagHISTORYINFODATA& pData)
{
	// Copy data
	this->bInitState = pData.bInitState;					// Init state
	this->nCategoryID = pData.nCategoryID;					// Category ID
	this->stTimestamp = pData.stTimestamp;					// Timestamp of history
	this->nItemID = pData.nItemID;							// Item ID
	this->nActionNameID = pData.nActionNameID;				// Name of action (string ID)
	this->bActionResult = pData.bActionResult;				// Action result
	this->dwErrorCode = pData.dwErrorCode;					// Returned error code
	this->strDescription = pData.strDescription;			// History description (attached info)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Initialization
//  Arguments:		nCategoryID - Category ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagHISTORYINFODATA::Init(UINT nCategoryID)
{
	// Reset data
	this->RemoveAll();
	this->bInitState = TRUE;								// Init state
	this->nCategoryID = nCategoryID;						// Category ID
	this->stTimestamp = CoreFuncs::GetCurSysTime();			// Timestamp of history
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all action data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagHISTORYINFODATA::RemoveAll(void)
{
	// Reset data
	this->bInitState = FALSE;								// Init state
	this->nCategoryID = INT_NULL;							// Category ID
	this->stTimestamp = SYSTEMTIME_ZERO;					// Timestamp of history
	this->nItemID = INT_NULL;								// Item ID
	this->nActionNameID = INT_NULL;							// Name of action (string ID)
	this->bActionResult = FALSE;							// Action result
	this->dwErrorCode = INT_NULL;							// Returned error code
	this->strDescription.Empty();							// History description (attached info)
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
//	Function name:	tagREGISTRYINFO
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagREGISTRYINFO::tagREGISTRYINFO()
{
	// Initialize
	this->hRootKey = NULL;									// Root key (HKEY)
	this->strRootKey = STRING_EMPTY;						// Root key (string)
	this->astrSubkeyPath.RemoveAll();						// Subkey path (string array)
	this->strProfileName = STRING_EMPTY;					// Profile key name (string)
	this->strAppName = STRING_EMPTY;						// App name (string)
	this->astrSectionArray.RemoveAll();						// Section array (string)
	this->regKeyInfo = REGISTRYKEY();						// Registry key info
}

tagREGISTRYINFO::tagREGISTRYINFO(const tagREGISTRYINFO& pItem)
{
	// Copy data
	this->hRootKey = pItem.hRootKey;						// Root key (HKEY)
	this->strRootKey = pItem.strRootKey;					// Root key (string)
	this->astrSubkeyPath.Copy(pItem.astrSubkeyPath);		// Subkey path (string array)
	this->strProfileName = pItem.strProfileName;			// Profile key name (string)
	this->strAppName = pItem.strAppName;					// App name (string)
	this->astrSectionArray.Copy(pItem.astrSectionArray);	// Section array (string)
	this->regKeyInfo.Copy(pItem.regKeyInfo);				// Registry key info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagREGISTRYINFO& tagREGISTRYINFO::operator=(const tagREGISTRYINFO& pItem)
{
	// Copy data
	this->hRootKey = pItem.hRootKey;						// Root key (HKEY)
	this->strRootKey = pItem.strRootKey;					// Root key (string)
	this->astrSubkeyPath.Copy(pItem.astrSubkeyPath);		// Subkey path (string array)
	this->strProfileName = pItem.strProfileName;			// Profile key name (string)
	this->strAppName = pItem.strAppName;					// App name (string)
	this->astrSectionArray.Copy(pItem.astrSectionArray);	// Section array (string)
	this->regKeyInfo.Copy(pItem.regKeyInfo);				// Registry key info

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

void tagREGISTRYINFO::Copy(const tagREGISTRYINFO& pItem)
{
	// Copy data
	this->hRootKey = pItem.hRootKey;						// Root key (HKEY)
	this->strRootKey = pItem.strRootKey;					// Root key (string)
	this->astrSubkeyPath.Copy(pItem.astrSubkeyPath);		// Subkey path (string array)
	this->strProfileName = pItem.strProfileName;			// Profile key name (string)
	this->strAppName = pItem.strAppName;					// App name (string)
	this->astrSectionArray.Copy(pItem.astrSectionArray);	// Section array (string)
	this->regKeyInfo.Copy(pItem.regKeyInfo);				// Registry key info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all registry key info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagREGISTRYINFO::RemoveAll(void)
{
	// Reset data
	this->hRootKey = NULL;									// Root key (HKEY)
	this->strRootKey = STRING_EMPTY;						// Root key (string)
	this->astrSubkeyPath.RemoveAll();						// Subkey path (string array)
	this->strProfileName = STRING_EMPTY;					// Profile key name (string)
	this->strAppName = STRING_EMPTY;						// App name (string)
	this->astrSectionArray.RemoveAll();						// Section array (string)
	this->regKeyInfo.RemoveAll();							// Registry key info

	// Cleanup array data
	this->astrSubkeyPath.FreeExtra();						// Subkey path (string array)
	this->astrSectionArray.FreeExtra();						// Section array (string)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetRootKeyName
//	Description:	Set registry info root key name
//  Arguments:		nResourceID - Root key name resource string ID (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagREGISTRYINFO::SetRootKeyName(UINT nResourceID)
{
	// Set root key name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	this->strRootKey = strKeyName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetSubkeyPath
//	Description:	Set registry info sub-key path
//  Arguments:		nResourceID	   - Sub-key path resource string ID (in)
//					astrSubkeyPath - Sub-key path array
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagREGISTRYINFO::SetSubkeyPath(UINT nResourceID)
{
	// Set sub-key path (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	this->astrSubkeyPath.Add(strKeyName);
}

void tagREGISTRYINFO::SetSubkeyPath(CStringArray& astrSubkeyPath)
{
	// Set sub-key path
	this->astrSubkeyPath.Copy(astrSubkeyPath);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetProfileName
//	Description:	Set registry info profile key name
//  Arguments:		nResourceID - Profile key name resource string ID (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagREGISTRYINFO::SetProfileName(UINT nResourceID)
{
	// Set profile key name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	this->strProfileName = strKeyName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppName
//	Description:	Set registry info app name
//  Arguments:		nResourceID - App name resource string ID (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagREGISTRYINFO::SetAppName(UINT nResourceID)
{
	// Set app name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	this->strAppName = strKeyName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetSectionName
//	Description:	Set registry info section name
//  Arguments:		nResourceID		 - Section name resource string ID (in)
//					astrSectionArray - Section name array
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagREGISTRYINFO::SetSectionName(UINT nResourceID)
{
	// Set section name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	this->astrSectionArray.Add(strKeyName);
}

void tagREGISTRYINFO::SetSectionName(CStringArray& astrSectionArray)
{
	// Set section name array
	this->astrSectionArray.Copy(astrSectionArray);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagSUBSTRING
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

tagSUBSTRING::tagSUBSTRING()
{
	// Initialization
	strLeft = STRING_EMPTY;					// Left part
	strMid = STRING_EMPTY;					// Middle part
	strRight = STRING_EMPTY;				// Right part
}

tagSUBSTRING::tagSUBSTRING(const tagSUBSTRING& pData)
{
	// Copy data
	strLeft = pData.strLeft;				// Left part
	strMid = pData.strMid;					// Middle part
	strRight = pData.strRight;				// Right part
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagSUBSTRING& tagSUBSTRING::operator=(const tagSUBSTRING& pData)
{
	// Copy data
	strLeft = pData.strLeft;				// Left part
	strMid = pData.strMid;					// Middle part
	strRight = pData.strRight;				// Right part

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

void tagSUBSTRING::Copy(const tagSUBSTRING& pData)
{
	// Copy data
	strLeft = pData.strLeft;				// Left part
	strMid = pData.strMid;					// Middle part
	strRight = pData.strRight;				// Right part
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all registry key info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagSUBSTRING::RemoveAll(void)
{
	// Reset data
	strLeft.Empty();						// Left part
	strMid.Empty();							// Middle part
	strRight.Empty();						// Right part
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if data is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL tagSUBSTRING::IsEmpty(void) const
{
	return (strLeft.IsEmpty() && strMid.IsEmpty() && strRight.IsEmpty());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Left
//	Description:	Get left part
//  Arguments:		None
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE LPCTSTR tagSUBSTRING::Left(void) const
{
	return (this->strLeft.GetString());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Mid
//	Description:	Get middle part
//  Arguments:		None
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE LPCTSTR tagSUBSTRING::Mid(void) const
{
	return (this->strMid.GetString());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Right
//	Description:	Get right part
//  Arguments:		None
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE LPCTSTR tagSUBSTRING::Right(void) const
{
	return (this->strRight.GetString());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagPERFORMANCECOUNTER
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

tagPERFORMANCECOUNTER::tagPERFORMANCECOUNTER()
{
	// Initialization
	QueryPerformanceFrequency(&this->frequency);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Start
//	Description:	Start performance counter
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagPERFORMANCECOUNTER::Start(void)
{
	// Start performance counter
	QueryPerformanceCounter(&this->startTime);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Stop
//	Description:	Stop performance counter
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagPERFORMANCECOUNTER::Stop(void)
{
	// Stop performance counter
	QueryPerformanceCounter(&this->endTime);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetElapsedTime
//	Description:	Get function execution elapsed time
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

double tagPERFORMANCECOUNTER::GetElapsedTime(BOOL bToMillisecs) const
{
	// Get elapsed time
	double dRetCounter = static_cast<double>(this->endTime.QuadPart - this->startTime.QuadPart) / this->frequency.QuadPart;
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
//	Function name:	GetPairedID
//	Description:	Find and return ID paired with specified macro ID
//  Arguments:		idTableRef  - Reference ID mapping table
//					nID			- First ID
//					bReverse	- Reverse search
//  Return value:	UINT - Second paired ID
//
//////////////////////////////////////////////////////////////////////////

UINT TableFuncs::GetPairedID(IDMAPTABLE& idTableRef, UINT nID, BOOL bReverse /* = FALSE */)
{
	// Return INVALID if ID mapping table is empty
	int nSize = idTableRef.size();
	if (nSize == 0) {
		return (UINT)INT_INVALID;
	}

	// Find and return corresponding ID paired with specified macro ID
	for (int nIndex = 0; nIndex < nSize; nIndex++) {
		IDPAIR idPair = idTableRef[nIndex];

		// Reverse search
		if (bReverse == TRUE) {
			if (idPair.nSecondID == nID)
				return idPair.nFirstID;
		}
		else {
			if (idPair.nFirstID == nID)
				return idPair.nSecondID;
		}
	}

	// Return INVALID if not found
	return (UINT)INT_INVALID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetStringID
//	Description:	Find and return ID paired with given string
//  Arguments:		strTableRef  - Reference string table
//					lpszInput	 - Given string
//  Return value:	UINT - String ID
//
//////////////////////////////////////////////////////////////////////////

UINT TableFuncs::GetStringID(STRINGTABLE& strTableRef, LPCTSTR lpszInput)
{
	// Return NULL string if language table is empty
	int nSize = strTableRef.size();
	if (nSize == 0) {
		return (UINT)INT_INVALID;
	}

	// Convert input string to lowercase
	CString strInput = lpszInput;
	strInput.MakeLower();

	// Find and return corresponding ID paired with specified string
	CString strPairedString = STRING_EMPTY;
	for (int nIndex = 0; nIndex < nSize; nIndex++) {
		LANGTEXT strPair = strTableRef[nIndex];

		// Also convert language string to lower for easier comparison
		strPairedString = strPair.lpszLangString;
		strPairedString.MakeLower();

		if (!_tcscmp(strPairedString, strInput)) {
			return strPair.dwLangStringID;
		}
	}

	// Return INVALID if not found
	return (UINT)INT_INVALID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetString
//	Description:	Find and return string paired with specified ID
//  Arguments:		strplRef  - Reference string table
//					nID		  - First ID
//					pszResult - Result string (reference type)
//  Return value:	LPCTSTR - Paired string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR	TableFuncs::GetString(STRINGTABLE& strTableRef, UINT nID, LPTSTR pszResult /* = NULL */)
{
	// Return NULL string if language table is empty
	int nSize = strTableRef.size();
	if (nSize == 0) {
		if (pszResult != NULL) pszResult = STRING_NULL;
		return STRING_NULL;
	}

	// Find and return corresponding string paired with specified ID
	for (int nIndex = 0; nIndex < nSize; nIndex++) {
		LANGTEXT strPair = strTableRef[nIndex];

		if (strPair.dwLangStringID == nID) {
			if (pszResult != NULL) {
				_tcscpy(pszResult, strPair.lpszLangString);
			}
			return strPair.lpszLangString;
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
//	Function name:	GetLanguageName
//	Description:	Get the title name of current language
//  Arguments:		nCurLanguage	- Current language ID
//					bGetDescription - Get language package description
//					pszResult		- Result string pointer (in/out)
//  Return value:	LPCTSTR - Language name
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR TableFuncs::GetLanguageName(UINT nCurLanguage, BOOL bGetDescription /* = FALSE */, LPTSTR pszResult /* = NULL */)
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

LANGTABLE_PTR TableFuncs::LoadLanguageTable(UINT nCurLanguage, LPTSTR pszRetLangName /* = NULL */, int* pnSize /* = NULL */)
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

LPCTSTR TableFuncs::GetLanguageString(LANGTABLE_PTR ptLanguage, UINT nID, LPTSTR pszResult /* = NULL */)
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

BOOL CoreFuncs::ExecutePowerAction(UINT nActionType, UINT nMessage, DWORD& dwErrCode)
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
						TRCFMT("Error: Execute action failed (Code: 0x%08X)", dwErrCode);
						TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}

					// Lookup privilege value
					if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkPrivileges.Privileges[0].Luid)) {
						// Get error code
						dwErrCode = GetLastError();
						TRCFMT("Error: Execute action failed (Code: 0x%08X)", dwErrCode);
						TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}

					// Adjust token privileges
					tkPrivileges.PrivilegeCount = 1;
					tkPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
					if (!AdjustTokenPrivileges(hToken, FALSE, &tkPrivileges, 0, (PTOKEN_PRIVILEGES)NULL, 0)) {
						// Adjust token privileges failed
						dwErrCode = GetLastError();
						TRCFMT("Error: Execute action failed (Code: 0x%08X)", dwErrCode);
						TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}

					// Exit Windows
					if (!ExitWindowsEx(uExitWinExFlags, 0)) {
						// Get exit Windows error
						dwErrCode = GetLastError();
						TRCFMT("Error: Execute action failed (Code: 0x%08X)", dwErrCode);
						TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}

				} break;

				case APP_MESSAGE_SLEEP:
					// Sleep mode
					if (!SetSuspendState(FALSE, FALSE, FALSE)) {		// Stand by (sleep)
						dwErrCode = GetLastError();
						TRCFMT("Error: Execute action failed (Code: 0x%08X)", dwErrCode);
						TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}
					break;

				case APP_MESSAGE_HIBERNATE:
					// Hibernate mode
					if (!SetSuspendState(TRUE, FALSE, FALSE)) {			// Hibernate
						dwErrCode = GetLastError();
						TRCFMT("Error: Execute action failed (Code: 0x%08X)", dwErrCode);
						TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
						return FALSE;
					}
					break;
			}
		} break;

	default:
		// Wrong argument
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
		TRCFMT("Error: Execute action failed (Code: 0x%08X)", dwErrCode);
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
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

BOOL CoreFuncs::ExecutePowerActionDummy(UINT nActionType, UINT nMessage, DWORD& dwErrCode)
{
	CString strAction;
	strAction.Format(_T("ExecutePowerAction: "));
	if (nActionType == APP_ACTIONTYPE_MONITOR) {
		strAction += _T("Turn off display");
		dwErrCode = ERROR_SUCCESS;
	}
	else if (nActionType == APP_ACTIONTYPE_POWER && 
			(nMessage != APP_MESSAGE_SLEEP && nMessage != APP_MESSAGE_HIBERNATE)) {
		switch (nMessage)
		{
		case APP_MESSAGE_SHUTDOWN:
			// Shutdown
			strAction += _T("Shutdown");
			dwErrCode = ERROR_SUCCESS;
			break;
		case APP_MESSAGE_REBOOT:
			// Restart
			strAction += _T("Restart");
			dwErrCode = ERROR_SUCCESS;
			break;
		case APP_MESSAGE_SIGNOUT:
			// Sign out
			strAction += _T("Sign out");
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
			strAction += _T("Sleep");
			dwErrCode = ERROR_SUCCESS;
			break;
		case APP_MESSAGE_HIBERNATE:
			// Hibernate
			strAction += _T("Hibernate");
			dwErrCode = ERROR_SUCCESS;
			break;
		}
	}
	else {
		// Wrong argument
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
	}

	// Show dummy test message
	HWND hWnd = GET_HANDLE_MAINWND();
	MessageBox(hWnd, strAction, _T("DummyTest"), MB_OK | MB_ICONINFORMATION);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDefaultData
//	Description:	Set default for config data
//  Arguments:		pcfgData - Pointer of config data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CoreFuncs::SetDefaultData(PCONFIGDATA pcfgData)
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

void CoreFuncs::SetDefaultData(PSCHEDULEDATA pschData)
{
	// Check data validity
	if (pschData == NULL)
		return;

	// Initialize data
	pschData->Init();

#ifdef DEBUG
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

void CoreFuncs::SetDefaultData(PHOTKEYSETDATA phksData)
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

#ifdef DEBUG
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

void CoreFuncs::SetDefaultData(PPWRREMINDERDATA ppwrData)
{
	// Check data validity
	if (ppwrData == NULL)
		return;

	// Initialize data
	ppwrData->Init();

#ifdef DEBUG
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceLog
//	Description:	Output exception trace log string to file
//  Arguments:		lpszTraceLogA - Output trace log string (ANSI)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CoreFuncs::TraceLog(LPCSTR lpszTraceLogA)
{
	// Convert ANSI string to UNICODE
	LPCTSTR lpszTraceLogW = MAKEUNICODE(lpszTraceLogA);
	TraceLog(lpszTraceLogW);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceLog
//	Description:	Output exception trace log string to file
//  Arguments:		lpszTraceLogW - Output trace log string (Unicode)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CoreFuncs::TraceLog(LPCTSTR lpszTraceLogW)
{
	// Write trace log file: TraceError.log
	WriteTraceErrorLogFile(lpszTraceLogW);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceLogFormat
//	Description:	Output exception trace log string to file
//  Arguments:		lpszTraceLogFormat - Trace log format string (ANSI)
//					...				   - Same as default MFC Format function
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CoreFuncs::TraceLogFormat(LPCSTR lpszTraceLogFormat, ...)
{
	ATLASSERT(AtlIsValidString(lpszTraceLogFormat));

	// Format source string
	CStringA strLogFormat;

	va_list argList;
	va_start(argList, lpszTraceLogFormat);
	strLogFormat.FormatV(lpszTraceLogFormat, argList);
	va_end(argList);

	// Output trace log
	TraceLog(strLogFormat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceLogFormat
//	Description:	Output exception trace log string to file
//  Arguments:		lpszTraceLogFormat - Trace log format string (Unicode)
//					...				   - Same as default MFC Format function
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CoreFuncs::TraceLogFormat(LPCTSTR lpszTraceLogFormat, ...)
{
	ATLASSERT(AtlIsValidString(lpszTraceLogFormat));

	// Format source string
	CString strLogFormat;

	va_list argList;
	va_start(argList, lpszTraceLogFormat);
	strLogFormat.FormatV(lpszTraceLogFormat, argList);
	va_end(argList);

	// Output trace log
	TraceLog(strLogFormat);
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

void CoreFuncs::TraceDebugInfo(LPCSTR lpszFuncName, LPCSTR lpszFileName, int nLineIndex)
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

void CoreFuncs::OutputDebugLog(LPCTSTR lpszDebugLog, int nForceOutput /* = INT_INVALID */)
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
		nDebugOutputTarget = DBOUT_DEBUGTESTTOOL;
	}

	// If debug mode not enabled, do nothing
	if (bDebugModeEnable == FALSE)
		return;

	// Output debug string
	if (nDebugOutputTarget == DBOUT_DEFAULT) {
		// Default output target: OutputDebugString
		// Debug strings can be watched by using VS Output screen or DebugView tool
		OutputDebugString(strDebugLog);
	}
	else if (nDebugOutputTarget == DBOUT_DEBUGINFOFILE) {
		// Ouput debug log to file: DebugInfo.log
		WriteDebugInfoLogFile(strDebugLog);
	}
	else if (nDebugOutputTarget == DBOUT_DEBUGTESTTOOL) {
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

void CoreFuncs::OutputDebugLogFormat(LPCTSTR lpszDebugLogFormat, ...)
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

void CoreFuncs::OutputDebugStringFormat(LPCTSTR lpszDebugStringFormat, ...)
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
	// If global trace error log file is not initialized
	if (g_pFileLogTraceError == NULL) {
		// Initialize global trace error log file
		g_pFileLogTraceError = new CFile();
		if (g_pFileLogTraceError == NULL)
			return FALSE;
	}

	// Get trace error log file pointer
	CFile* pTraceErrorLogFile = GetTraceErrorLogFile();

	// Log file path
	CString strFilePath;
	CoreFuncs::MakeFilePath(strFilePath, SUBFOLDER_LOG, FILENAME_TRACE_ERROR_LOG, FILEEXT_LOGFILE);

	// If the log file is not being opened
	if (pTraceErrorLogFile->m_hFile == CFile::hFileNull) {
		OPENFILE: {
			// Open the log file
			if (!pTraceErrorLogFile->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
				// Show error message
				DWORD dwErrorCode = GetLastError();
				CoreFuncs::ShowErrorMessage(NULL, NULL, dwErrorCode);
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
			if (!CoreFuncs::BackupOldLogFile(strFilePath, FILENAME_TRACE_ERROR_LOG))
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
	// If global trace debug log file is not initialized
	if (g_pFileLogTraceDebug == NULL) {
		// Initialize global trace debug log file
		g_pFileLogTraceDebug = new CFile();
		if (g_pFileLogTraceDebug == NULL)
			return FALSE;
	}

	// Get trace debug log file pointer
	CFile* pTraceDebugLogFile = GetTraceDebugLogFile();

	// Log file path
	CString strFilePath;
	CoreFuncs::MakeFilePath(strFilePath, SUBFOLDER_LOG, FILENAME_TRACE_DEBUG_LOG, FILEEXT_LOGFILE);

	// If the log file is not being opened
	if (pTraceDebugLogFile->m_hFile == CFile::hFileNull) {
		OPENFILE: {
			// Open the log file
			if (!pTraceDebugLogFile->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
				// Show error message
				DWORD dwErrorCode = GetLastError();
				CoreFuncs::ShowErrorMessage(NULL, NULL, dwErrorCode);
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
			if (!CoreFuncs::BackupOldLogFile(strFilePath, FILENAME_TRACE_DEBUG_LOG))
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
	// If global debug info log file is not initialized
	if (g_pFileLogDebugInfo == NULL) {
		// Initialize global debug info log file
		g_pFileLogDebugInfo = new CFile();
		if (g_pFileLogDebugInfo == NULL)
			return FALSE;
	}

	// Get debug info log file pointer
	CFile* pDebugInfoLogFile = GetDebugInfoLogFile();

	// Log file path
	CString strFilePath;
	CoreFuncs::MakeFilePath(strFilePath, SUBFOLDER_LOG, FILENAME_DEBUG_INFO_LOG, FILEEXT_LOGFILE);

	// If the log file is not being opened
	if (pDebugInfoLogFile->m_hFile == CFile::hFileNull) {
		OPENFILE: {
			// Open the log file
			if (!pDebugInfoLogFile->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
				// Show error message
				DWORD dwErrorCode = GetLastError();
				CoreFuncs::ShowErrorMessage(NULL, NULL, dwErrorCode);
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
			if (!CoreFuncs::BackupOldLogFile(strFilePath, FILENAME_DEBUG_INFO_LOG))
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

BOOL CoreFuncs::BackupOldLogFile(CString& strFilePath, LPCTSTR lpszLogFileName)
{
	CFileFind Finder;
	CString strBakFilePath;
	CString strFilePathTemp;

	// If file path is not specified, do nothing
	if (strFilePath.IsEmpty()) return FALSE;

	// Search for backup file list
	for (int nNum = 0; nNum < MAX_BAKFILE_COUNT; nNum++) {
		
		// Make backup file path template
		if (!MakeFilePath(strFilePathTemp, SUBFOLDER_LOG, lpszLogFileName, FILEEXT_BAKLOGFILE))
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

void CoreFuncs::WriteTraceErrorLogFile(LPCTSTR lpszLogStringW)
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
			MakeFilePath(strOrgFilePath, SUBFOLDER_LOG, FILENAME_TRACE_ERROR_LOG, FILEEXT_LOGFILE);
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

void CoreFuncs::WriteTraceDebugLogFile(LPCTSTR lpszLogStringW)
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
			MakeFilePath(strOrgFilePath, SUBFOLDER_LOG, FILENAME_TRACE_DEBUG_LOG, FILEEXT_LOGFILE);
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

void CoreFuncs::WriteDebugInfoLogFile(LPCTSTR lpszLogStringW)
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
			MakeFilePath(strOrgFilePath, SUBFOLDER_LOG, FILENAME_DEBUG_INFO_LOG, FILEEXT_LOGFILE);
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

void CoreFuncs::WriteTraceNDebugLogFileBase(LPCTSTR lpszFileName, LPCTSTR lpszLogStringW)
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

LRESULT	CoreFuncs::WaitMessage(UINT nMsg, int nTimeout /* = DEF_WAITMESSAGE_TIMEOUT */)
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

void CoreFuncs::ShowErrorMessage(HWND hMsgOwnerWnd, UINT nLanguageID, DWORD dwErrorCode, LPARAM lParam /* = NULL */)
{
	// Use table functions
	using namespace TableFuncs;

	// Get application-defined error code from system-defined error code
	DWORD dwAppErrCode = GetPairedID(idTableErrorCode, dwErrorCode, TRUE);
	if (dwAppErrCode != INT_INVALID) {
		// Replace with application-defined error code
		dwErrorCode = dwAppErrCode;
	}

	// Get error message string ID
	int nErrMsgID = GetPairedID(idTableErrorMessage, dwErrorCode);

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

UINT CoreFuncs::Sel2Opt(UINT nOptionMacro, UINT nSelection)
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

UINT CoreFuncs::Opt2Sel(UINT nOptionMacro, UINT nCurOption)
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

BOOL CoreFuncs::Text2Time(SYSTEMTIME& stTime, CString strText)
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

BOOL CoreFuncs::Text2TimeBase(SYSTEMTIME& stTime, CString strText)
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

void CoreFuncs::SpinPos2Time(SYSTEMTIME& stTime, int nPos)
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

void CoreFuncs::Time2SpinPos(SYSTEMTIME stTime, int& nPos)
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

int CoreFuncs::GetListCurSel(CListCtrl& pListCtrl)
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

HWND CoreFuncs::FindDebugTestDlg()
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

void CoreFuncs::SetFixedCellStyle(CGridCtrl* pGridCtrl, int nRow, int nCol)
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

void CoreFuncs::DrawGridTableRow(CGridCtrl* pGridCtrl, int nRow, int nRowNum, int nColNum, GRIDCTRLCOLFORMAT* apGrdColFormat)
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
//  Arguments:		None
//  Return value:	SYSTEMTIME - Return time data
//
//////////////////////////////////////////////////////////////////////////

SYSTEMTIME CoreFuncs::GetCurSysTime(void)
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

void CoreFuncs::CalcTimeOffset(SYSTEMTIME& stTime, int nOffset)
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

BOOL CoreFuncs::CheckTimeMatch(SYSTEMTIME timeDest, SYSTEMTIME timePar, int nOffset /* = 0 */)
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

CString	CoreFuncs::FormatDispTime(LANGTABLE_PTR pLang, UINT nFormatID, SYSTEMTIME timeVal)
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

CString	CoreFuncs::FormatDispTime(LANGTABLE_PTR pLang, LPCTSTR lpszFormatString, SYSTEMTIME timeVal)
{
	// Format time string
	UINT nMiddayFlag = (timeVal.wHour < 12) ? FORMAT_TIME_BEFOREMIDDAY : FORMAT_TIME_AFTERMIDDAY;
	CString strMiddayFormat = TableFuncs::GetLanguageString(pLang, nMiddayFlag);
	WORD wHour = (timeVal.wHour > 12) ? (timeVal.wHour - 12) : timeVal.wHour;
	WORD wMinute = timeVal.wMinute;

	CString strResult;
	strResult.Format(lpszFormatString, wHour, wMinute, strMiddayFormat);

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

LPCTSTR CoreFuncs::LoadResourceString(UINT nResStringID)
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

BOOL CoreFuncs::LoadResourceString(CString& strResult, UINT nResStringID)
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

BOOL CoreFuncs::LoadResourceTextFile(CString& strTextData, UINT nResourceFileID)
{
	// Empty result text data
	strTextData.Empty();

	// Get resource handle
	HINSTANCE hResInstance = AfxGetResourceHandle();
	if (hResInstance == NULL)
		return FALSE;

	// Find resource file by ID
	HRSRC hRes = FindResource(hResInstance, MAKEINTRESOURCE(nResourceFileID), RT_RCDATA);
	if (hRes != NULL) {

		// Load resource data
		HGLOBAL hData = LoadResource(hResInstance, hRes);
		if (hData != NULL) {

			// Get text data size
			DWORD dwSize = SizeofResource(hResInstance, hRes);

			// Convert data to text data
			LPCSTR lpData = static_cast<LPCSTR>(LockResource(hData));
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

int	CoreFuncs::GetTokenList(LPTSTR lpszBuff, PBUFFER retBuff, LPCTSTR lpszKeyChars)
{
	// Get length
	int nBuffLength = _tcslen(lpszBuff);
	int nKeyLength = _tcslen(lpszKeyChars);

	// Re-format given string buffer
	int nBuffIdx = 0;
	for (int nIndex = 0; nIndex < nBuffLength; nIndex++) {
		// Invalid characters
		if ((lpszBuff[nIndex] == CHAR_ENDLINE) ||
			(lpszBuff[nIndex] == CHAR_RETURN))
			continue;
		// Keep valid characters only
		lpszBuff[nBuffIdx] = lpszBuff[nIndex];
		nBuffIdx++;
		// End string
		if (lpszBuff[nBuffIdx] == CHAR_ENDSTRING)
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
		if ((tcCurChar == CHAR_ENDLINE) || (tcCurChar == CHAR_RETURN))
			continue;
		// In case of quotation mark
		if (tcCurChar == CHAR_QUOTAMARK) {
			// Change flag
			nQuoteFlag = (nQuoteFlag == 0) ? FLAG_ON : FLAG_OFF;
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
				// Token number count-up
				nTokenCount++;
			}
		}
		// Current character is the quotation mark itself
		else if (tcCurChar == CHAR_QUOTAMARK) {
			// If token number exceeds max count, stop
			if (nTokenCount > MAX_TOKEN_COUNT)
				break;
			else {
				// Index count-up
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

		// Index count-up
		nCurCharIndex++;

		// If end of string or token number exceeds max count, stop
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

void CoreFuncs::UpperEachWord(CString& strInput, BOOL bTrim)
{
	// Get string
	LPTSTR lpszString = strInput.GetBuffer();

	// Get string length
	int nLength = _tcslen(lpszString);
	if (nLength <= 0)
		return;

	// Lambda functions
	auto IsNotSpace = [](TCHAR tcChar) { return (!std::isspace(tcChar)); };
	auto BothSpaces = [](TCHAR tcFirst, TCHAR tcSecond) {
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
		if (lpszString[nIndex] != _T(' ')) {
			if ((nIndex == 0)	/* First character */ ||
				/* Not the first character and standing after a space */
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
//	Function name:	MakeFilePath
//	Description:	Make file path by given part names
//  Arguments:		strOutput	  - Output file path
//					lpszDirectory - Directory path
//					lpszFileName  - File name
//					lpszExtension - File extension
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL CoreFuncs::MakeFilePath(CString& strOutput, LPCTSTR lpszDirectory, LPCTSTR lpszFileName, LPCTSTR lpszExtension)
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

BOOL CoreFuncs::StringValidate(LPCTSTR lpszSrc, DWORD& dwError)
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

LPCTSTR CoreFuncs::StringFormat(UINT nFormatTemplateID, ...)
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

LPCTSTR CoreFuncs::StringFormat(LPCTSTR lpszFormatTemplate, ...)
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

BOOL CoreFuncs::SubString(LPCTSTR lpszSrc, SUBSTRING& subDest, TCHAR tcFirstChar, TCHAR tcLastChar, BOOL bIncSepChar /* = FALSE */)
{
	CString strSrc(lpszSrc);

	// Empty destination data
	subDest.RemoveAll();

	// If source string is empty
	if (strSrc.IsEmpty()) {
		TRCLOG("Function: CoreFuncs::GetSubString(), Error: Source string is empty");
		return FALSE;
	}

	// Find starting and ending character index
	INT_PTR nFirstIndex = (tcFirstChar != NULL) ? (strSrc.Find(tcFirstChar)) : INT_INVALID;
	INT_PTR nLastIndex = (tcLastChar != NULL) ? (strSrc.ReverseFind(tcLastChar)) : INT_INVALID;

	// Debug log
	OutputDebugStringFormat(_T("[ALSTest] ==> GetSubString: nFirstIndex=%d, nLastIndex=%d"), nFirstIndex, nLastIndex);

	// Temporary output data
	SUBSTRING subResult;

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
		subResult.strLeft = strSrc.Left(nFirstIndex);
		subResult.strMid = strSrc.Mid((nFirstIndex + 1), (nLastIndex - nFirstIndex));
		subResult.strRight = strSrc.Right(strSrc.GetLength() - (nLastIndex + 1));
	}
	else if ((nFirstIndex == INT_INVALID) && (nLastIndex != INT_INVALID)) {

		// Case #2:
		subResult.strLeft = strSrc.Left(nLastIndex);
		subResult.strMid = STRING_EMPTY;
		subResult.strRight = strSrc.Right(strSrc.GetLength() - (nLastIndex + 1));
	}
	else if ((nFirstIndex != INT_INVALID) && (nLastIndex == INT_INVALID)) {

		// Case #3:
		subResult.strLeft = strSrc.Left(nFirstIndex);
		subResult.strMid = STRING_EMPTY;
		subResult.strRight = strSrc.Right(strSrc.GetLength() - (nFirstIndex + 1));
	}

	// Get result
	BOOL bRet = (!subResult.IsEmpty());
	if (bRet != FALSE) {

		// Trim spaces
		subResult.strLeft.Trim();
		subResult.strMid.Trim();
		subResult.strRight.Trim();

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

LPCTSTR CoreFuncs::GetApplicationPath(BOOL bIncludeExeName)
{
	// Get the application's module handle
	HMODULE hModule = GetModuleHandle(NULL);

	// Get the full path of the executable file of the module
	TCHAR tcAppPath[MAX_PATH];
	if (!GetModuleFileName(hModule, tcAppPath, MAX_PATH))
		return STRING_EMPTY;

	// Full path result
	static CString strRetAppPath;
	strRetAppPath.Empty();
	strRetAppPath = tcAppPath;

	// If not including the executable file name
	if (bIncludeExeName != TRUE) {
		// Remove the executable file name from the path
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

CString CoreFuncs::GetProductVersion(BOOL bFullVersion)
{
	// Get product file name
	CString strProductFileName;
	if (!MakeFilePath(strProductFileName, NULL, FILENAME_APPEXEFILE, FILEEXT_EXEFILE))
		return STRING_EMPTY;

	// Get product version
	CString strProductVersion;
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(strProductFileName, &dwHandle);
	if (dwSize > 0) {
		BYTE* pVersionInfo = new BYTE[dwSize];
		if (GetFileVersionInfo(strProductFileName, dwHandle, dwSize, pVersionInfo)) {
			UINT uLen;
			VS_FIXEDFILEINFO* lpFfi;
			if (VerQueryValue(pVersionInfo, SYMBOL_BACKSLASH, (LPVOID*)&lpFfi, &uLen)) {
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
			}
		}
		delete[] pVersionInfo;
	}
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

BOOL CoreFuncs::GetProductVersion(CString& strFullVersion, CString& strShortVersion)
{
	// Get product file name
	CString strProductFileName;
	if (!MakeFilePath(strProductFileName, NULL, FILENAME_APPEXEFILE, FILEEXT_EXEFILE))
		return FALSE;

	// Get product version info size
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(strProductFileName, &dwHandle);
	if (dwSize <= 0)
		return FALSE;

	// Get product version info
	BYTE* pVersionInfo = new BYTE[dwSize];
	if (!GetFileVersionInfo(strProductFileName, dwHandle, dwSize, pVersionInfo)) {
		delete[] pVersionInfo;
		return FALSE;
	}

	UINT uLen;
	VS_FIXEDFILEINFO* lpFfi;
	if (!VerQueryValue(pVersionInfo, SYMBOL_BACKSLASH, (LPVOID*)&lpFfi, &uLen)) {
		delete[] pVersionInfo;
		return FALSE;
	}

	// Get product version number
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

UINT CoreFuncs::GetWindowsOSVersion(void)
{
	// Init info data
	OSVERSIONINFOEX oviOSVersion;
	oviOSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

	// Get Window OS version
	if (RtlGetVersion != NULL)
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

BOOL CoreFuncs::GetDeviceName(CString& strDeviceName)
{
	// Empty the output string
	strDeviceName.Empty();

	// Get the computer device name
	TCHAR tcDeviceName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwNameLength = sizeof(tcDeviceName) / sizeof(TCHAR);
	if (!GetComputerName(tcDeviceName, &dwNameLength))
		return FALSE;

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

BOOL CoreFuncs::GetCurrentUserName(CString& strUserName)
{
	// Empty the output string
	strUserName.Empty();

	// Get the current user name
	TCHAR tcUserName[UNLEN + 1];
	DWORD dwNameLength = sizeof(tcUserName) / sizeof(TCHAR);
	if (!GetUserName(tcUserName, &dwNameLength))
		return FALSE;

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

BOOL CoreFuncs::AddRegistryKey(const REGISTRYINFO& regInfo)
{
	CRegKey regKey;
	LONG lResult = regKey.Open(regInfo.hRootKey, regInfo.regKeyInfo.strKeyName, KEY_ALL_ACCESS);
	if (lResult != ERROR_SUCCESS) {
		// Create register key if not found
		if (lResult == ERROR_NOT_FOUND) {
			regKey.Create(regInfo.hRootKey, regInfo.regKeyInfo.strKeyName, REG_NONE,
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

LPCTSTR CoreFuncs::MakeRegistryPath(const REGISTRYINFO& regInfo, UINT nRegPathType /* = REGPATH_FULL */, BOOL bIncRootKey /* = TRUE */)
{
	// Check root key info validity
	if (bIncRootKey != FALSE) {
		if ((regInfo.hRootKey == NULL) && (regInfo.strRootKey.IsEmpty()))
			return STRING_NULL;
	}

	// Check sub-key path validity
	if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_SUBPATH)) {
		if (regInfo.astrSubkeyPath.IsEmpty())
			return STRING_NULL;
	}

	// Check profile key validity
	if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_PROFILEKEY)) {
		if (regInfo.strProfileName.IsEmpty())
			return STRING_NULL;
	}

	// Check app name validity
	if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_APPNAME)) {
		if (regInfo.strAppName.IsEmpty())
			return STRING_NULL;
	}

	// Check section name array validity
	if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_SECTIONNAME)) {
		if (regInfo.astrSectionArray.IsEmpty())
			return STRING_NULL;
	}

	// Root key (string)
	CString strRootKey;
	if (regInfo.strRootKey != STRING_EMPTY) {
		strRootKey.Format(regInfo.strRootKey);
	}

	// Sub-key path
	CString strSubPath;
	for (int nIndex = 0; nIndex < regInfo.astrSubkeyPath.GetSize(); nIndex++) {
		if (nIndex > 0) {
			strSubPath.Append(SYMBOL_BACKSLASH);
		}
		strSubPath.Append(regInfo.astrSubkeyPath.GetAt(nIndex));
	}

	// Profile key name
	CString strProfileKeyName;
	if (regInfo.strProfileName != STRING_EMPTY) {
		strProfileKeyName.Format(regInfo.strProfileName);
	}

	// App name
	CString strAppName;
	if (regInfo.strAppName != STRING_EMPTY) {
		strAppName.Format(regInfo.strAppName);
	}

	// Section name
	CString strSectionName;
	for (int nIndex = 0; nIndex < regInfo.astrSectionArray.GetSize(); nIndex++) {
		if (nIndex > 0) {
			strSectionName.Append(SYMBOL_BACKSLASH);
		}
		strSectionName.Append(regInfo.astrSectionArray.GetAt(nIndex));
	}


	// Key name
	CString strKeyName;
	if (regInfo.regKeyInfo.strKeyName != STRING_EMPTY) {
		strKeyName.Format(regInfo.regKeyInfo.strKeyName);
	}

	// Result string
	static CString strRegFullPath;
	strRegFullPath.Empty();

	// Make registry path by type
	int nRetFailedFlag = FLAG_OFF;
	if (!strRootKey.IsEmpty()) {

		// Initialize and include root key name
		strRegFullPath.Empty();
		if ((bIncRootKey != FALSE) && (!strRootKey.IsEmpty())) {
			strRegFullPath.Append(strRootKey);
		}

		if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_SUBPATH)) {
			if ((nRetFailedFlag != FLAG_ON) && (!strSubPath.IsEmpty())) {
				// Include separator character if rootkey is included
				if ((bIncRootKey != FALSE) && (!strRootKey.IsEmpty())) {
					strRegFullPath.Append(SYMBOL_BACKSLASH);
				}
				// Include sub-key path
				strRegFullPath.Append(strSubPath);
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
			if ((nRetFailedFlag != FLAG_ON) && (!strSectionName.IsEmpty())) {
				// Include section name
				strRegFullPath.Append(SYMBOL_BACKSLASH);
				strRegFullPath.Append(strSectionName);
			}
			else {
				// Turn failed flag ON
				nRetFailedFlag = FLAG_ON;
			}
		}

		if ((nRegPathType == REGPATH_FULL) || (nRegPathType >= REGPATH_KEYNAME)) {
			if ((nRetFailedFlag != FLAG_ON) && (!strKeyName.IsEmpty())) {
				// Include key name
				strRegFullPath.Append(SYMBOL_BACKSLASH);
				strRegFullPath.Append(strKeyName);
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

void CoreFuncs::PlaySound(BOOL bSoundEnable, UINT nTypeOfSound)
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

BOOL CoreFuncs::FileViewStd(FILETYPE eFileType, LPCTSTR lpszFilePath)
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

BOOL CoreFuncs::OpenWebURL(LPCTSTR lpszWebUrl)
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

LRESULT CoreFuncs::RunApp(LPCTSTR lpszAppPath, BOOL bRunAsAdmin /* = FALSE */, BOOL bShowFlag /* = TRUE */)
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

LRESULT CoreFuncs::ExecuteCommand(LPCTSTR lpszCommand, BOOL bRunAsAdmin /* = TRUE */, BOOL bShowFlag /* = TRUE */)
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

BOOL CoreFuncs::CreateAppProcess(LPCWSTR lpszAppPath, LPWSTR lpszCmdLine, UINT nStyle, DWORD& dwErrorCode)
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
		TRCFMT("Error: Create app process failed (Code: 0x%08X)", dwErrorCode);
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
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

BOOL CoreFuncs::SetDarkMode(CWnd* pWnd, BOOL bEnableDarkMode)
{
	// Load theme library
	HMODULE hUxTheme = LoadLibraryEx(_T("uxtheme.dll"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hUxTheme == NULL)
		return FALSE;

	// Get function pointer
	using fnAllowDarkMode = BOOL (WINAPI*)(HWND hWND, BOOL bAllow);
	static const fnAllowDarkMode AllowDarkModeForWindow = (fnAllowDarkMode)GetProcAddress(hUxTheme, MAKEINTRESOURCEA(133));
	if (AllowDarkModeForWindow == NULL)
		return FALSE;

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

void CoreFuncs::DrawButton(CButton*& pBtn, UINT nIconID, LPCTSTR lpszBtnTitle /* = STRING_EMPTY */)
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
	std::vector<std::wstring>* fontNames = reinterpret_cast<std::vector<std::wstring>*>(lParam);
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

BOOL CoreFuncs::EnumFontNames(std::vector<std::wstring>& fontNames)
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

BOOL CoreFuncs::ValidateFontName(LPCTSTR lpszFontName)
{
	// Array to get returned font names
	std::vector<std::wstring> fontNames;

	// Enumerate all currently available fonts
	BOOL bRet = EnumFontNames(fontNames);
	if (bRet == FALSE) {
		TRCLOG("Error: Enumerate fonts failed!");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
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

