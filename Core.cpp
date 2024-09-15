
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Core.h
//		Description:	Implement core methods which will be used elsewhere in program
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//						<3> 2024.07.06:		Update to version 3.1
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

	// System settings
	this->bShowDlgAtStartup = pData.bShowDlgAtStartup;					// Show dialog at startup
	this->bStartupEnabled = pData.bStartupEnabled;						// Startup with Windows
	this->bConfirmAction = pData.bConfirmAction;						// Show confirm message before doing action
	this->bSaveActionLog = pData.bSaveActionLog;						// Save action log
	this->bSaveAppEventLog = pData.bSaveAppEventLog;					// Save app event log
	this->bRunAsAdmin = pData.bRunAsAdmin;								// Run with admin privileges
	this->bShowErrorMsg = pData.bShowErrorMsg;							// Show action error message
	this->bNotifySchedule = pData.bNotifySchedule;						// Show notify tip for schedule action
	this->bAllowCancelSchedule = pData.bAllowCancelSchedule;			// Allow canceling schedule when notify
	this->bEnableBackgroundHotkey = pData.bEnableBackgroundHotkey;		// Enable background action hotkeys
	this->bEnablePowerReminder = pData.bEnablePowerReminder;			// Enable Power Peminder feature
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another schedule data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagSCHEDULEDATA::Copy(const tagSCHEDULEDATA& pData)
{
	this->bEnable = pData.bEnable;		// Enable/disable status
	this->bRepeat = pData.bRepeat;		// Repeat daily
	this->nAction = pData.nAction;		// Schedule action
	this->stTime = pData.stTime;		// Schedule time
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Activate
//	Description:	Activate schedule
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagSCHEDULEDATA::Activate()
{
	this->bEnable = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Deactivate
//	Description:	Deactivate schedule
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagSCHEDULEDATA::Deactivate()
{
	this->bEnable = FALSE;
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
	this->bEnable = pItem.bEnable;
	this->nHKActionID = pItem.nHKActionID;
	this->dwCtrlKeyCode = pItem.dwCtrlKeyCode;
	this->dwFuncKeyCode = pItem.dwFuncKeyCode;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if HotkeySet item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagHOTKEYSETITEM::IsEmpty()
{
	// Check if keystroke value is empty
	if ((this->dwCtrlKeyCode == 0) &&
		(this->dwFuncKeyCode == 0)) {
		return TRUE;
	}

	return FALSE;
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
	using namespace PairFuncs;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	UINT nActionNameID = GetPairedID(idplActionName, GetPairedID(idplHKActionID, this->nHKActionID));
	CString strAction = GetLanguageString(ptrLanguage, nActionNameID);
	CString strKeyStrokes = DEF_STRING_EMPTY;
	if (this->dwCtrlKeyCode & MOD_CONTROL)	strKeyStrokes += _T("Ctrl + ");
	if (this->dwCtrlKeyCode & MOD_ALT)		strKeyStrokes += _T("Alt + ");
	if (this->dwCtrlKeyCode & MOD_WIN)		strKeyStrokes += _T("Win + ");
	strKeyStrokes += GetPairedString(strplFuncKeyList, this->dwFuncKeyCode);
	CString strEnable = (this->bEnable == TRUE) ? _T("Enabled") : _T("Disabled");

	// Print item
	strOutput.Format(_T("State=(%s), Action=(%s), Keystrokes=(%s)"),  strEnable, strAction, strKeyStrokes);
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
	this->nItemNum = 0;
	this->arrHotkeySetList.RemoveAll();
}

tagHOTKEYSETDATA::tagHOTKEYSETDATA(const tagHOTKEYSETDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy data
	this->nItemNum = pData.nItemNum;
	if (this->nItemNum <= 0) {
		this->arrHotkeySetList.RemoveAll();
		this->arrHotkeySetList.FreeExtra();
	}
	else {
		// Copy each item
		for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
			HOTKEYSETITEM hksItem = pData.arrHotkeySetList.GetAt(nIndex);
			this->arrHotkeySetList.Add(hksItem);
		}
	}
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
	this->nItemNum = 0;
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
	this->nItemNum = pData.nItemNum;
	if (this->nItemNum <= 0) {
		this->arrHotkeySetList.RemoveAll();
		this->arrHotkeySetList.FreeExtra();
	}
	else {
		// Copy each item
		for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
			HOTKEYSETITEM hksItem = pData.arrHotkeySetList.GetAt(nIndex);
			this->arrHotkeySetList.Add(hksItem);
		}
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
	// If data list is current empty, just add
	if ((this->nItemNum == 0) && (this->arrHotkeySetList.IsEmpty())) {
		this->nItemNum = 1;
		this->arrHotkeySetList.Add(pItem);
		return;
	}

	// Check if item exists, if yes, do not add
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
		HOTKEYSETITEM pItemTemp = this->GetItemAt(nIndex);
		if ((pItemTemp.nHKActionID == pItem.nHKActionID) &&
			(pItemTemp.dwCtrlKeyCode == pItem.dwCtrlKeyCode) &&
			(pItemTemp.dwFuncKeyCode == pItem.dwFuncKeyCode))
			return;
	}

	// Create new temporary data
	PHOTKEYSETDATA pNew = new HOTKEYSETDATA;
	pNew->nItemNum = (this->nItemNum + 1);
	pNew->arrHotkeySetList.RemoveAll();
	
	// Copy old data to new one
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
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
	// If data list is current empty, just add
	if ((this->nItemNum == 0) || (this->arrHotkeySetList.IsEmpty())) {
		this->Add(pItem);
		return;
	}

	// Check if item with same action ID or keystrokes exists
	int nDupActionIndex = DEF_INTEGER_INVALID;
	int nDupKeyIndex = DEF_INTEGER_INVALID;

	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
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
	if (nDupKeyIndex != DEF_INTEGER_INVALID) {
		this->Remove(nDupKeyIndex);
	}

	// If item with same action ID existed, update its data
	if (nDupActionIndex != DEF_INTEGER_INVALID) {
		HOTKEYSETITEM& hksTemp = this->GetItemAt(nDupActionIndex);
		hksTemp.bEnable = pItem.bEnable;
		hksTemp.dwCtrlKeyCode = pItem.dwCtrlKeyCode;
		hksTemp.dwFuncKeyCode = pItem.dwFuncKeyCode;
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
//
//////////////////////////////////////////////////////////////////////////

HOTKEYSETITEM& tagHOTKEYSETDATA::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < (this->nItemNum)));
	if ((nIndex >= 0) && (nIndex < (this->nItemNum)))
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
	if ((nAtIndex < 0) || (nAtIndex >= (this->nItemNum)))
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
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
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
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {

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
//	Function name:	IsEmpty
//	Description:	Check if item at index is empty
//  Arguments:		nIndex - Item index to check
//  Return value:	BOOL - Result of empty or not
//
//////////////////////////////////////////////////////////////////////////

BOOL tagHOTKEYSETDATA::IsEmpty(int nIndex)
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= (this->nItemNum)))
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

BOOL tagHOTKEYSETDATA::IsAllEmpty()
{
	// If there's no item, return TRUE
	if ((this->nItemNum <= 0) && (this->arrHotkeySetList.IsEmpty()))
		return TRUE;

	// Check each item
	BOOL bAllEmpty = TRUE;
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
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
	if ((nAtIndex < 0) || (nAtIndex >= (this->nItemNum)))
		return;

	// Create new temporary data
	PHOTKEYSETDATA pNew = new HOTKEYSETDATA;
	pNew->nItemNum = (this->nItemNum - 1);
	pNew->arrHotkeySetList.RemoveAll();

	// Copy old data to new one (except the AtIndex item)
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
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
	this->nItemNum = 0;
	this->arrHotkeySetList.RemoveAll();
	this->arrHotkeySetList.FreeExtra();
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
	this->bEnable = FALSE;					// Enable state
	this->nItemID = 0;						// Item ID
	this->strMessage = DEF_STRING_EMPTY;	// Message content
	this->nEventID = 0;						// Event ID
	this->stTime = {0};						// Event time
	this->dwStyle = 0;						// Reminder style
	this->bRepeat = 0;						// Repeat daily
}

tagPWRREMINDERITEM::tagPWRREMINDERITEM(const tagPWRREMINDERITEM& pItem)
{
	// Copy data
	this->bEnable = pItem.bEnable;			// Enable state
	this->nItemID = pItem.nItemID;			// Item ID
	this->strMessage = pItem.strMessage;	// Message content
	this->nEventID = pItem.nEventID;		// Event ID
	this->stTime = pItem.stTime;			// Event time
	this->dwStyle = pItem.dwStyle;			// Reminder style
	this->bRepeat = pItem.bRepeat;			// Repeat daily
}

tagPWRREMINDERITEM::tagPWRREMINDERITEM(BOOL bSetEnable, UINT nSetItemID, LPCTSTR lpszSetMsg, 
									   UINT nSetEventID, SYSTEMTIME stSetTime, DWORD dwSetStyle, BOOL bSetRepeat)
{
	// Copy data
	this->bEnable = bSetEnable;				// Enable state
	this->nItemID = nSetItemID;				// Item ID
	this->strMessage = lpszSetMsg;			// Message content
	this->nEventID = nSetEventID;			// Event ID
	this->stTime = stSetTime;				// Event time
	this->dwStyle = dwSetStyle;				// Reminder style
	this->bRepeat = bSetRepeat;				// Repeat daily
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
	this->bEnable = pItem.bEnable;			// Enable state
	this->nItemID = pItem.nItemID;			// Item ID
	this->strMessage = pItem.strMessage;	// Message content
	this->nEventID = pItem.nEventID;		// Event ID
	this->stTime = pItem.stTime;			// Event time
	this->dwStyle = pItem.dwStyle;			// Reminder style
	this->bRepeat = pItem.bRepeat;			// Repeat daily
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if Power Reminder item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagPWRREMINDERITEM::IsEmpty()
{
	// Check if item data is empty
	if ((this->strMessage == DEF_STRING_EMPTY) &&
		(this->nEventID == 0) &&
		(this->stTime.wHour == 0) &&
		(this->stTime.wMinute == 0) &&
		(this->dwStyle == 0) &&
		(this->bRepeat == 0))
		return TRUE;

	return FALSE;
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
	using namespace PairFuncs;
	using namespace CoreFuncs;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	CString strEnable = (this->bEnable == TRUE) ? _T("Enabled") : _T("Disabled");
	CString strMsg = this->strMessage;
	if (strMsg.GetLength() > (DEF_LOGDISP_STRING_MAXLENGTH + 3)) {
		strMsg = this->strMessage.Left(DEF_LOGDISP_STRING_MAXLENGTH) + _T("...");
	}
	int nTemp = GetPairedID(idplPwrReminderEvt, this->nEventID);
	CString strEvent = GetLanguageString(ptrLanguage, nTemp);
	if (this->nEventID == PREVT_AT_SETTIME) {
		// Format time string
		CString strFormat = strEvent;
		strEvent = FormatDispTime(ptrLanguage, strFormat, this->stTime);
	}
	nTemp = GetPairedID(idplPwrReminderStyle, this->dwStyle);
	CString strStyle = GetLanguageString(ptrLanguage, nTemp);

	// Print item
	strOutput.Format(_T("State=(%s), ItemID=%d, Msg=(%s), Event=(%s), Style=(%s), Repeat=%d"),
				strEnable, this->nItemID, strMsg, strEvent, strStyle, this->bRepeat);
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
	this->nItemNum = 0;
	this->arrRmdItemList.RemoveAll();
}

tagPWRREMINDERDATA::tagPWRREMINDERDATA(const tagPWRREMINDERDATA& pData)
{
	// Remove existing data
	this->DeleteAll();

	// Copy data
	this->nItemNum = pData.nItemNum;
	if (this->nItemNum <= 0) {
		this->arrRmdItemList.RemoveAll();
		this->arrRmdItemList.FreeExtra();
	}
	else {
		// Copy each item
		for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
			PWRREMINDERITEM pwrItem = pData.arrRmdItemList.GetAt(nIndex);
			this->arrRmdItemList.Add(pwrItem);
		}
	}
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
	this->nItemNum = 0;
	this->arrRmdItemList.RemoveAll();
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

	// Copy data
	this->nItemNum = pData.nItemNum;
	if (this->nItemNum <= 0) {
		this->arrRmdItemList.RemoveAll();
		this->arrRmdItemList.FreeExtra();
	}
	else {
		// Copy each item
		for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
			PWRREMINDERITEM pwrItem = pData.arrRmdItemList.GetAt(nIndex);
			this->arrRmdItemList.Add(pwrItem);
		}
	}
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
	// If data list is current empty, just add
	if ((this->nItemNum == 0) || (this->arrRmdItemList.IsEmpty())) {
		this->nItemNum = 1;
		this->arrRmdItemList.Add(pItem);
		return;
	}

	// Check if item exists, if yes, do not add
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
		PWRREMINDERITEM pItemTemp = this->GetItemAt(nIndex);
		if ((pItemTemp.bEnable == pItem.bEnable) &&
			(pItemTemp.nItemID == pItem.nItemID) &&
			(pItemTemp.strMessage == pItem.strMessage) &&
			(pItemTemp.nEventID == pItem.nEventID) &&
			(pItemTemp.stTime.wHour == pItem.stTime.wHour) &&
			(pItemTemp.stTime.wMinute == pItem.stTime.wMinute) &&
			(pItemTemp.dwStyle == pItem.dwStyle))
			return;
	}

	// Create new temporary data
	PPWRREMINDERDATA pNew = new PWRREMINDERDATA;
	pNew->nItemNum = (this->nItemNum + 1);
	pNew->arrRmdItemList.RemoveAll();

	// Copy old data to new one
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
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
	// If data list is current empty, just add
	if ((this->nItemNum == 0) || (this->arrRmdItemList.IsEmpty())) {
		this->Add(pItem);
		return;
	}

	// If there's no item, do nothing
	if (this->nItemNum <= 0)
		return;

	// Find item index
	int nRetItemIndex = DEF_INTEGER_INVALID;
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
		if (this->GetItemAt(nIndex).nItemID == pItem.nItemID) {
			nRetItemIndex = nIndex;
			break;
		}
	}

	// Update item if found
	if (nRetItemIndex != DEF_INTEGER_INVALID) {
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
//
//////////////////////////////////////////////////////////////////////////

PWRREMINDERITEM& tagPWRREMINDERDATA::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < (this->nItemNum)));
	if ((nIndex >= 0) && (nIndex < (this->nItemNum)))
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
	if ((nAtIndex < 0) || (nAtIndex >= (this->nItemNum)))
		return;

	// Get item data
	PWRREMINDERITEM& pwrItem = this->GetItemAt(nAtIndex);

	// Reset item value
	pwrItem.bEnable = FALSE;
	pwrItem.nItemID = 0;
	pwrItem.strMessage = DEF_STRING_EMPTY;
	pwrItem.nEventID = 0;
	pwrItem.stTime.wHour = 0;
	pwrItem.stTime.wMinute = 0;
	pwrItem.dwStyle = 0;
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
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
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
	int nArrItemNum = arrRmdItemList.GetSize();
	if (nArrItemNum > nItemNum) {
		for (int nIndex = (nArrItemNum - 1); nIndex >= (this->nItemNum); nIndex--) {
			// Remove item out of data array
			arrRmdItemList.RemoveAt(nIndex);
		}

		// Free extra memory
		arrRmdItemList.FreeExtra();
	}

	for (int nIndex = (this->nItemNum - 1); nIndex >= 0 ; nIndex--) {
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
	UINT nMaxID = DEF_PWRREMINDER_MIN_ITEMID;
	PWRREMINDERITEM pwrItem;
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
		pwrItem = this->GetItemAt(nIndex);
		if (pwrItem.nItemID > nMaxID) {
			nMaxID = pwrItem.nItemID;
		}
	}

	// Increase value
	nMaxID++;

	// Get unexisting ID value
	UINT nAvailableID = DEF_PWRREMINDER_MIN_ITEMID;
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
		pwrItem = this->GetItemAt(nIndex);
		if (pwrItem.nItemID == nMaxID) {
			nMaxID = pwrItem.nItemID;
		}
	}

	return nMaxID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if item at index is empty
//  Arguments:		nIndex - Item index to check
//  Return value:	BOOL - Result of empty or not
//
//////////////////////////////////////////////////////////////////////////

BOOL tagPWRREMINDERDATA::IsEmpty(int nIndex)
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= (this->nItemNum)))
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

BOOL tagPWRREMINDERDATA::IsAllEmpty()
{
	// If there's no item, return TRUE
	if ((this->nItemNum <= 0) && (this->arrRmdItemList.IsEmpty()))
		return TRUE;

	// Check each item
	BOOL bAllEmpty = TRUE;
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
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
	if ((nAtIndex < 0) || (nAtIndex >= (this->nItemNum)))
		return;

	// Create new temporary data
	PPWRREMINDERDATA pNew = new PWRREMINDERDATA;
	pNew->nItemNum = (this->nItemNum - 1);
	pNew->arrRmdItemList.RemoveAll();

	// Copy old data to new one (except the AtIndex item)
	for (int nIndex = 0; nIndex < (this->nItemNum); nIndex++) {
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
	this->nItemNum = 0;
	this->arrRmdItemList.RemoveAll();
	this->arrRmdItemList.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another action data
//  Arguments:		pData - Pointer of input data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagACTIONDATA::Copy(const tagACTIONDATA& pData)
{
	this->nActionType = pData.nActionType;				// Action type
	this->stActionTime = pData.stActionTime;			// Time of action
	this->nActionNameID = pData.nActionNameID;			// Name of action (string ID)
	this->bActionSucceed = pData.bActionSucceed;		// Action success status
	this->nErrorCode = pData.nErrorCode;				// Action returned error code
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all action data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagACTIONDATA::RemoveAll(void)
{
	this->nActionType = 0;							// Action type
	this->stActionTime = {0};						// Time of action
	this->nActionNameID = 0;						// Name of action (string ID)
	this->bActionSucceed = FALSE;					// Action success status
	this->nErrorCode = 0;							// Action returned error code
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
//  Arguments:		idplRef  - Reference ID pair list
//					nID		 - First ID
//					bReverse - Reverse search
//  Return value:	UINT - Second paired ID
//
//////////////////////////////////////////////////////////////////////////

UINT PairFuncs::GetPairedID(IDPAIRLIST& idplRef, UINT nID, BOOL bReverse /* = FALSE */)
{
	// Return INVALID if ID pair list is empty
	int nSize = idplRef.size();
	if (nSize == 0) {
		return (UINT)DEF_INTEGER_INVALID;
	}

	// Find and return corresponding ID paired with specified macro ID
	for (int nIndex = 0; nIndex < nSize; nIndex++) {
		IDPAIR idPair = idplRef[nIndex];

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
	return (UINT)DEF_INTEGER_INVALID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetPairedString
//	Description:	Find and return string paired with specified ID
//  Arguments:		strplRef  - Reference string pair list
//					nID		  - First ID
//					pszResult - Result string (reference type)
//  Return value:	LPCTSTR - Paired string
//
//////////////////////////////////////////////////////////////////////////

UINT PairFuncs::GetStringID(STRINGPAIRLIST& strplRef, LPCTSTR lpszInput)
{
	// Return NULL string if language table is empty
	int nSize = strplRef.size();
	if (nSize == 0) {
		return (UINT)DEF_INTEGER_INVALID;
	}

	// Convert input string to lowercase
	CString strInput = lpszInput;
	strInput.MakeLower();

	// Find and return corresponding ID paired with specified string
	CString strPairedString = DEF_STRING_EMPTY;
	for (int nIndex = 0; nIndex < nSize; nIndex++) {
		LANGTEXT strPair = strplRef[nIndex];

		// Also convert language string to lower for easier comparison
		strPairedString = strPair.lpszLangString;
		strPairedString.MakeLower();

		if (!_tcscmp(strPairedString, strInput)) {
			return strPair.dwLangStringID;
		}
	}

	// Return INVALID if not found
	return (UINT)DEF_INTEGER_INVALID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetPairedString
//	Description:	Find and return string paired with specified ID
//  Arguments:		strplRef  - Reference string pair list
//					nID		  - First ID
//					pszResult - Result string (reference type)
//  Return value:	LPCTSTR - Paired string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR	PairFuncs::GetPairedString(STRINGPAIRLIST& strplRef, UINT nID, LPTSTR pszResult /* = NULL */)
{
	// Return NULL string if language table is empty
	int nSize = strplRef.size();
	if (nSize == 0) {
		if (pszResult != NULL) pszResult = DEF_STRING_NULL;
		return DEF_STRING_NULL;
	}

	// Find and return corresponding string paired with specified ID
	for (int nIndex = 0; nIndex < nSize; nIndex++) {
		LANGTEXT strPair = strplRef[nIndex];

		if (strPair.dwLangStringID == nID) {
			if (pszResult != NULL) {
				_tcscpy(pszResult, strPair.lpszLangString);
			}
			return strPair.lpszLangString;
		}
	}

	// Return NULL string if not found
	if (pszResult != NULL) {
		pszResult = DEF_STRING_NULL;
	}
	return DEF_STRING_NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLanguageName
//	Description:	Get the title name of current language
//  Arguments:		nCurLanguage - Current language ID
//  Return value:	LPCTSTR - Language name
//
//////////////////////////////////////////////////////////////////////////

LPCSTR PairFuncs::GetLanguageName(UINT nCurLanguage, LPSTR pszResult /* = NULL */)
{
	LPCSTR lpszLangName;
	switch (nCurLanguage)
	{
	case APP_LANGUAGE_ENGLISH:
		lpszLangName = "English";
		break;
	case APP_LANGUAGE_VIETNAMESE:
		lpszLangName = "Vietnamese";
		break;
	case APP_LANGUAGE_SIMPCHINESE:
		lpszLangName = "Simplified Chinese";
		break;
	default:
		lpszLangName = "Unknown";
		break;
	}
	// Get language table name
	if (pszResult != NULL) {
		strcpy(pszResult, lpszLangName);
	}

	return lpszLangName;
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

LANGTABLE_PTR PairFuncs::LoadLanguageTable(UINT nCurLanguage, LPSTR pszRetLangName /* = NULL */, int* pnSize /* = NULL */)
{
	LANGTABLE_PTR ptrLangTable = NULL;
	LPCSTR lpszLangName;

	switch (nCurLanguage)
	{
	case APP_LANGUAGE_ENGLISH:
		// Language: English (United States)
		ptrLangTable = &langtable_en_US;
		lpszLangName = "English";
		break;
	case APP_LANGUAGE_VIETNAMESE:
		// Language: Vietnamese (Vietnam)
		ptrLangTable = &langtable_vi_VN;
		lpszLangName = "Vietnamese";
		break;
	case APP_LANGUAGE_SIMPCHINESE:
		// Language: Simplified Chinese (China mainland)
		ptrLangTable = &langtable_zh_CH;
		lpszLangName = "Simplified Chinese";
		break;
	default:
		// Default language: English
		ptrLangTable = &langtable_en_US;
		lpszLangName = "English";
		break;
	}

	// Get language table name
	if (pszRetLangName != NULL) {
		strcpy(pszRetLangName, lpszLangName);
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

LPCTSTR PairFuncs::GetLanguageString(LANGTABLE_PTR ptLanguage, UINT nID, LPTSTR pszResult /* = NULL */)
{
	// Return NULL string if language table is empty
	if ((ptLanguage == NULL) || (ptLanguage->empty())) {
		if (pszResult != NULL) pszResult = DEF_STRING_NULL;
		return DEF_STRING_NULL;
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
		pszResult = DEF_STRING_NULL;
	}
	return DEF_STRING_NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoPowerAction
//	Description:	Main power action function
//  Arguments:		nActionType - Type of action
//					nMessage	- Action message
//					dwErrorCode - Return error code (ref-value)
//  Return value:	BOOL - Result of action execution
//
//////////////////////////////////////////////////////////////////////////

BOOL CoreFuncs::DoPowerAction(UINT nActionType, UINT nMessage, DWORD& dwErrCode)
{
	// Action here
	BOOL bRet = FALSE;
	switch (nActionType)
	{
		case DEF_APP_ACTIONTYPE_MONITOR:
			// Turn off display
			PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, (WPARAM)nMessage, (LPARAM)2);
			bRet = TRUE;
			dwErrCode = DEF_APP_ERROR_SUCCESS;
			break;
		case DEF_APP_ACTIONTYPE_POWER:
		{
			switch (nMessage)
			{
				case DEF_APP_MESSAGE_SHUTDOWN:
				case DEF_APP_MESSAGE_REBOOT:
				case DEF_APP_MESSAGE_SIGNOUT:
				{
					// Force action
					UINT nAction = nMessage;
					nAction |= EWX_FORCE;

					HANDLE hToken;
					TOKEN_PRIVILEGES tkPrivileges;

					// Get process token
					if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
						dwErrCode = GetLastError();
						return FALSE;
					}

					// Adjust token privileges
					LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkPrivileges.Privileges[0].Luid);
					tkPrivileges.PrivilegeCount = 1;
					tkPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
					AdjustTokenPrivileges(hToken, FALSE, &tkPrivileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);

					// Adjust token privileges failed
					dwErrCode = GetLastError();
					if (dwErrCode != DEF_APP_ERROR_SUCCESS)
						return FALSE;

					// Exit Windows
					bRet = ExitWindowsEx(nAction, 0);
					dwErrCode = GetLastError();

				} break;
				case DEF_APP_MESSAGE_SLEEP:
					// Sleep
					bRet = SetSuspendState(FALSE, FALSE, FALSE);	// Stand by (sleep)
					dwErrCode = GetLastError();
					break;
				case DEF_APP_MESSAGE_HIBERNATE:
					// Hibernate
					bRet = SetSuspendState(TRUE, FALSE, FALSE);		// Hibernate
					dwErrCode = GetLastError();
					break;
			}
		} break;
	default:
		// Wrong argument
		dwErrCode = DEF_APP_ERROR_WRONG_ARGUMENT;
		bRet = FALSE;
		break;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoPowerActionDummy
//	Description:	Dummy power action function (use for testing)
//  Arguments:		nActionType - Type of action
//					nMessage	- Action message
//					dwErrorCode - Return error code (ref-value)
//  Return value:	BOOL - Result of action execution
//
//////////////////////////////////////////////////////////////////////////

BOOL CoreFuncs::DoPowerActionDummy(UINT nActionType, UINT nMessage, DWORD& dwErrCode)
{
	CString strAction;
	strAction.Format(_T("DoPowerAction: "));
	if (nActionType == DEF_APP_ACTIONTYPE_MONITOR) {
		strAction += _T("Turn off display");
		dwErrCode = ERROR_SUCCESS;
	}
	else if (nActionType == DEF_APP_ACTIONTYPE_POWER && 
			(nMessage != DEF_APP_MESSAGE_SLEEP && nMessage != DEF_APP_MESSAGE_HIBERNATE)) {
		switch (nMessage)
		{
		case DEF_APP_MESSAGE_SHUTDOWN:
			// Shutdown
			strAction += _T("Shutdown");
			dwErrCode = ERROR_SUCCESS;
			break;
		case DEF_APP_MESSAGE_REBOOT:
			// Restart
			strAction += _T("Restart");
			dwErrCode = ERROR_SUCCESS;
			break;
		case DEF_APP_MESSAGE_SIGNOUT:
			// Sign out
			strAction += _T("Sign out");
			dwErrCode = ERROR_SUCCESS;
			break;
		}
	}
	else if (nActionType == DEF_APP_ACTIONTYPE_POWER && 
			(nMessage == DEF_APP_MESSAGE_SLEEP || nMessage == DEF_APP_MESSAGE_HIBERNATE)) {
		switch (nMessage)
		{
		case DEF_APP_MESSAGE_SLEEP:
			// Sleep
			strAction += _T("Sleep");
			dwErrCode = ERROR_SUCCESS;
			break;
		case DEF_APP_MESSAGE_HIBERNATE:
			// Hibernate
			strAction += _T("Hibernate");
			dwErrCode = ERROR_SUCCESS;
			break;
		}
	}
	else {
		// Wrong argument
		dwErrCode = DEF_APP_ERROR_WRONG_ARGUMENT;
	}

	// Show dummy test message
	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();
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

	// Set default data
	pcfgData->nLMBAction = DEF_APP_ACTION_DISPLAYOFF;
	pcfgData->nMMBAction = DEF_APP_ACTION_SLEEP;
	pcfgData->nRMBAction = DEF_APP_ACTION_SHOWMENU;
	pcfgData->bRMBShowMenu = TRUE;
	pcfgData->nLanguageID = APP_LANGUAGE_ENGLISH;
	pcfgData->bShowDlgAtStartup = TRUE;
	pcfgData->bStartupEnabled = TRUE;
	pcfgData->bConfirmAction = FALSE;
	pcfgData->bSaveActionLog = TRUE;
	pcfgData->bSaveAppEventLog = TRUE;
	pcfgData->bRunAsAdmin = FALSE;
	pcfgData->bShowErrorMsg = FALSE;
	pcfgData->bNotifySchedule = FALSE;
	pcfgData->bAllowCancelSchedule = FALSE;
	pcfgData->bEnableBackgroundHotkey = FALSE;
	pcfgData->bEnablePowerReminder = FALSE;
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

	// Set default data
	pschData->bEnable = FALSE;
	pschData->bRepeat = FALSE;
	pschData->nAction = DEF_APP_ACTION_NOTHING;
	pschData->stTime = GetCurSysTime();
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
	LPCTSTR lpszTraceLogW = CA2W(lpszTraceLogA).m_psz;
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
	// Write trace log file: Trace.log
	WriteTraceNDebugLogFile(FILE_TRACE_LOG, lpszTraceLogW);
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
//	Function name:	OutputDebugLog
//	Description:	Output debug log string
//  Arguments:		lpszDebugLog - Debug log string (Unicode)
//					nForceStyle	 - Force output log style
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CoreFuncs::OutputDebugLog(LPCTSTR lpszDebugLog, int nForceStyle /* = -1 */)
{
	// Get debug mode enable state
	BOOL bDebugMode = GetDebugMode();

	// Get log string
	CString strLog = lpszDebugLog;

	// Find DebugTest tool dialog
	HWND hDebugTestWnd = FindDebugTestDlg();

	// Debug log style
	int nDebugLogStyle = nForceStyle;
	if (nDebugLogStyle == -1) {
		nDebugLogStyle = GetDebugLogStyle();
	}
	if (hDebugTestWnd != NULL) {
		// Prefer output to DebugTest tool if showing
		nDebugLogStyle = DBLOG_OUTPUTTODBTOOL;
		bDebugMode = TRUE;
	}

	// If debug mode not enabled, do nothing
	if (bDebugMode == FALSE)
		return;

	// Output debug string
	if (nDebugLogStyle == DBLOG_OUTPUTDBSTRING) {
		// Default style: OutputDebugString
		// Debug string can be watched by using VS Ouput screen or DebugView tool
		OutputDebugString(strLog);
	}
	else if (nDebugLogStyle == DBLOG_OUTPUTTOFILE) {
		// Ouput debug log to file: Debug.log
		WriteTraceNDebugLogFile(FILE_DEBUG_LOG, strLog);
	}
	else if (nDebugLogStyle == DBLOG_OUTPUTTODBTOOL) {
		// Output debug log to DebugTest tool
		if (hDebugTestWnd == NULL) return;
		WPARAM wParam = (WPARAM)strLog.GetLength();
		LPARAM lParam = (LPARAM)strLog.GetBuffer();
		SendMessage(hDebugTestWnd, SM_APP_DEBUGOUTPUT, wParam, lParam);
		strLog.ReleaseBuffer();
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
//	Function name:	WriteTraceNDebugLogFile
//	Description:	Write trace and debug log string to file
//  Arguments:		lpszFileName	- Log file name
//					lpszLogStringW	- Log string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CoreFuncs::WriteTraceNDebugLogFile(LPCTSTR lpszFileName, LPCTSTR lpszLogStringW)
{
	// Log file path
	CString strFilePath;
	strFilePath.Format(_T("%s\\%s"), DIR_SUBDIR_LOG, lpszFileName);

	CString strALSLog;
	CFile fTrcDbgLogFile;

	// Check if file is opening, if not, open it
	if (fTrcDbgLogFile.m_hFile == CFile::hFileNull) {
		BOOL bResult = TRUE;
	OPENFILE: {
			bResult = fTrcDbgLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
			if (bResult == FALSE) {
				DWORD dwErrorCode = GetLastError();
				ShowErrorMessage(NULL, NULL, dwErrorCode);
				return;
			}
		}

		// Go to end of file
		ULONGLONG ullFileSize = fTrcDbgLogFile.SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= DEF_LOGFILE_MAXLENGTH) {

			// Step1: Close file
			fTrcDbgLogFile.Close();
			
			// Step2: Rename file extension to BAK
			CFileFind Finder;
			CString strBakFilePath;
			for (int nNum = 0; nNum < DEF_BAKFILE_MAXNUM; nNum++) {
				strBakFilePath.Format((strFilePath + DEF_FILEEXT_BAKFILEWNUM), nNum);
				if (Finder.FindFile(strBakFilePath) == TRUE) {
					if (nNum == (DEF_BAKFILE_MAXNUM - 1)) return;
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
	strTimeFormat.Format(IDS_STRFORMAT_FULLDATETIME, stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, strMiddayFlag);

	// Format output log string
	CString strLogFormat;
	strLogFormat.Format(IDS_STRFORMAT_LOGSTRING, strTimeFormat, lpszLogStringW, DEF_STRING_EMPTY);

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
//	Function name:	ShowErrorMessage
//	Description:	Show error message by error code
//  Arguments:		hWnd		- Handle of messagebox parent window
//					nLanguageID - Language option ID
//					dwError		- Error code
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CoreFuncs::ShowErrorMessage(HWND hWnd, UINT nLanguageID, DWORD dwError)
{
	int nErrMsgID = DEF_INTEGER_INVALID;

	// Check error code and show error message
	switch (dwError)
	{
	case DEF_APP_ERROR_SUCCESS:
		break;
	case DEF_APP_ERROR_FAILED:
		nErrMsgID = MSGBOX_ERROR_FAILED;
		break;
	case DEF_APP_ERROR_WRONG_ARGUMENT:
		nErrMsgID = MSGBOX_ERROR_WRONG_ARGUMENT;
		break;
	case DEF_APP_ERROR_INVALID_FUNCTION:
		nErrMsgID = MSGBOX_ERROR_INVALID_FUNCTION;
		break;
	case DEF_APP_ERROR_FILE_NOT_FOUND:
		nErrMsgID = MSGBOX_ERROR_FILE_NOT_FOUND;
		break;
	case DEF_APP_ERROR_PATH_NOT_FOUND:
		nErrMsgID = MSGBOX_ERROR_PATH_NOT_FOUND;
		break;
	case DEF_APP_ERROR_ACCESS_DENIED:
		nErrMsgID = MSGBOX_ERROR_ACCESS_DENIED;
		break;
	case DEF_APP_ERROR_INVALID_HANDLE:
		nErrMsgID = MSGBOX_ERROR_INVALID_HANDLE;
		break;
	case DEF_APP_ERROR_INVALID_DATA:
		nErrMsgID = MSGBOX_ERROR_INVALID_DATA;
		break;
	case DEF_APP_ERROR_NO_MORE_FILES:
		nErrMsgID = MSGBOX_ERROR_NO_MORE_FILES;
		break;
	case DEF_APP_ERROR_FILE_EXISTS:
		nErrMsgID = MSGBOX_ERROR_FILE_EXISTS;
		break;
	case DEF_APP_ERROR_CANNOT_MAKE:
		nErrMsgID = MSGBOX_ERROR_CANNOT_MAKE;
		break;
	case DEF_APP_ERROR_INVALID_PARAMETER:
		nErrMsgID = MSGBOX_ERROR_INVALID_PARAMETER;
		break;
	case DEF_APP_ERROR_OPEN_FAILED:
		nErrMsgID = MSGBOX_ERROR_OPEN_FAILED;
		break;
	case DEF_APP_ERROR_BUFFER_OVERFLOW:
		nErrMsgID = MSGBOX_ERROR_BUFFER_OVERFLOW;
		break;
	case DEF_APP_ERROR_INVALID_NAME:
		nErrMsgID = MSGBOX_ERROR_INVALID_NAME;
		break;
	case DEF_APP_ERROR_DIR_NOT_EMPTY:
		nErrMsgID = MSGBOX_ERROR_DIR_NOT_EMPTY;
		break;
	case DEF_APP_ERROR_FAIL_SHUTDOWN:
		nErrMsgID = MSGBOX_ERROR_FAIL_SHUTDOWN;
		break;
	case DEF_APP_ERROR_FAIL_RESTART:
		nErrMsgID = MSGBOX_ERROR_FAIL_RESTART;
		break;
	case DEF_APP_ERROR_INVALID_ADDRESS:
		nErrMsgID = MSGBOX_ERROR_INVALID_ADDRESS;
		break;
	case DEF_APP_ERROR_APP_INIT_FAILURE:
		nErrMsgID = MSGBOX_ERROR_APP_INIT_FAILURE;
		break;
	case DEF_APP_ERROR_CANNOT_LOAD_REGISTRY:
		nErrMsgID = MSGBOX_ERROR_CANNOT_LOAD_REGISTRY;
		break;
	case DEF_APP_ERROR_REGISTRY_QUOTA_LIMIT:
		nErrMsgID = MSGBOX_ERROR_REGISTRY_QUOTA_LIMIT;
		break;
	case DEF_APP_ERROR_SYSTEM_SHUTDOWN:
		nErrMsgID = MSGBOX_ERROR_SYSTEM_SHUTDOWN;
		break;
	case DEF_APP_ERROR_HIBERNATED:
		nErrMsgID = MSGBOX_ERROR_HIBERNATED;
		break;
	case DEF_APP_ERROR_RESUME_HIBERNATION:
		nErrMsgID = MSGBOX_ERROR_RESUME_HIBERNATION;
		break;
	case DEF_APP_ERROR_WAKE_SYSTEM:
		nErrMsgID = MSGBOX_ERROR_WAKE_SYSTEM;
		break;
	case DEF_APP_ERROR_BACKUP_REG_FAILED:
		nErrMsgID = MSGBOX_ERROR_BACKUP_REG_FAILED;
		break;
	case DEF_APP_ERROR_LOAD_CFG_INVALID:
	case DEF_APP_ERROR_LOAD_CFG_FAILED:
		nErrMsgID = MSGBOX_ERROR_LOAD_CFG_FAILED;
		break;
	case DEF_APP_ERROR_SAVE_CFG_INVALID:
	case DEF_APP_ERROR_SAVE_CFG_FAILED:
		nErrMsgID = MSGBOX_ERROR_SAVE_CFG_FAILED;
		break;
	case DEF_APP_ERROR_LOAD_SCHED_INVALID:
	case DEF_APP_ERROR_LOAD_SCHED_FAILED:
		nErrMsgID = MSGBOX_ERROR_LOAD_SCHED_FAILED;
		break;
	case DEF_APP_ERROR_SAVE_SCHED_INVALID:
	case DEF_APP_ERROR_SAVE_SCHED_FAILED:
		nErrMsgID = MSGBOX_ERROR_SAVE_SCHED_FAILED;
		break;
	case DEF_APP_ERROR_LOAD_HKEYSET_INVALID:
	case DEF_APP_ERROR_LOAD_HKEYSET_FAILED:
		nErrMsgID = MSGBOX_ERROR_LOAD_HKEYSET_FAILED;
		break;
	case DEF_APP_ERROR_SAVE_HKEYSET_INVALID:
	case DEF_APP_ERROR_SAVE_HKEYSET_FAILED:
		nErrMsgID = MSGBOX_ERROR_SAVE_HKEYSET_FAILED;
		break;
	case DEF_APP_ERROR_LOAD_PWRRMD_INVALID:
	case DEF_APP_ERROR_LOAD_PWRRMD_FAILED:
		nErrMsgID = MSGBOX_ERROR_LOAD_PWRRMD_FAILED;
		break;
	case DEF_APP_ERROR_SAVE_PWRRMD_INVALID:
	case DEF_APP_ERROR_SAVE_PWRRMD_FAILED:
		nErrMsgID = MSGBOX_ERROR_SAVE_PWRRMD_FAILED;
		break;
	case DEF_APP_ERROR_WRITE_LOG_FAILED:
		nErrMsgID = MSGBOX_ERROR_WRITE_LOG_FAILED;
		break;
	case DEF_APP_ERROR_OUTPUT_LOG_FAILED:
		nErrMsgID = MSGBOX_ERROR_OUTPUT_LOG_FAILED;
		break;
	case DEF_APP_ERROR_UNKNOWN:
		nErrMsgID = MSGBOX_ERROR_UNKNOWN;
		break;
	default:
		nErrMsgID = MSGBOX_ERROR_UNKNOWN;
		break;
	}

	// Invalid error message ID, do nothing 
	if (nErrMsgID == DEF_INTEGER_INVALID)
		return;

	// Load language package and language strings
	using namespace PairFuncs;
	LANGTABLE_PTR pAppLang = LoadLanguageTable(nLanguageID);
	CString strMsg = GetLanguageString(pAppLang, nErrMsgID);
	CString strCaption = GetLanguageString(pAppLang, MSGBOX_ERROR_CAPTION);

	// Show error message
	DisplayMessageBox(hWnd, strMsg, strCaption, MB_OK | MB_ICONERROR);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayMessageBox
//	Description:	Display message box using language string
//  Arguments:		hWnd		 - Handle of messagebox parent window
//					strPromptID	 - Message string
//					strCaptionID - Message caption string
//					nStyle		 - Message box style
//  Return value:	int	- Result of message box
//
//////////////////////////////////////////////////////////////////////////

int CoreFuncs::DisplayMessageBox(HWND hWnd, LPCTSTR strPrompt, LPCTSTR strCaption, UINT nStyle)
{
	nStyle |= MB_SYSTEMMODAL;
	return MessageBox(hWnd, strPrompt, strCaption, nStyle);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Sel2Opt
//	Description:	Convert combo-box selection into option ID
//  Arguments:		nOptionMacro - Option macro
//					nSelection	 - Selection
//  Return value:	UINT - Option ID
//
//////////////////////////////////////////////////////////////////////////

UINT CoreFuncs::Sel2Opt(UINT nOptionMacro, UINT nSelection)
{
	VERIFY(nOptionMacro > 0 && nOptionMacro < UINT_MAX);
	VERIFY(nSelection >= 0 && nSelection < UINT_MAX);
	return ((nOptionMacro << 8) + (nSelection + 1));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Opt2Sel
//	Description:	Convert option ID into combo-box selection
//  Arguments:		nOptionMacro - Option macro
//					nCurOption	 - Option ID
//  Return value:	UINT - Selection
//
//////////////////////////////////////////////////////////////////////////

UINT CoreFuncs::Opt2Sel(UINT nOptionMacro, UINT nCurOption)
{
	VERIFY(nOptionMacro > 0 && nOptionMacro < UINT_MAX);
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

	int nHour = DEF_INTEGER_INVALID;
	int nMinute = DEF_INTEGER_INVALID;
	int nLeft = 0, nLeft1 = 0;
	int	nRight = 0, nRight1 = 0;

	// Convert
	switch (nLength)
	{
	case 1:
		// Ex: 3 -> 03:00, 9 -> 09:00, ...
		nHour = _tstoi(strTime);
		nMinute = 0;
		break;
	case 2:
		nLeft = _tstoi(strTime.Left(1));
		nRight = _tstoi(strTime.Right(1));
		if ((nLeft == 0) ||										// Ex: 08 -> 00:08
			((nLeft > 2) || ((nLeft == 2) && (nRight > 3)))) {	// Ex: 35 -> 03:05, 24 -> 02:04, ...
			nHour = nLeft;
			nMinute = nRight;
		}
		else {
			// Ex: 13 -> 13:00, 18 -> 18:00, ...
			nHour = _tstoi(strTime);
			nMinute = 0;
		} break;
	case 3:
		nLeft = _tstoi(strTime.Left(1));
		nLeft1 = _tstoi(strTime.Left(2));
		if ((nLeft == 0) ||						// Ex: 034 -> 00:34, ...
			((nLeft > 2) || (nLeft1 >= 24))) {	// Ex: 320 -> 03:20, 250 -> 02:50, ...
			nHour = nLeft;
			nMinute = _tstoi(strTime.Right(2));
		}
		else {
			// Ex: 180 -> 18:00, 225 -> 22:05
			nHour = nLeft1;
			nMinute = _tstoi(strTime.Right(1));
		} break;
	case 4:
		// Ex: 1235 -> 12:35, 1840 -> 18:40, ...
		nHour = _tstoi(strTime.Left(2));
		nMinute = _tstoi(strTime.Right(2));
		break;
	}

	// If minute is larger than 60, hour increases by 1
	if (nMinute >= 60) {
		nHour++;
		nMinute -= 60;
	}

	// If hour exceeds 24, return invalid
	if (nHour >= 24)
		return FALSE;

	// Only return if both hour and minute values are valid
	if ((nHour > DEF_INTEGER_INVALID) && (nMinute > DEF_INTEGER_INVALID)) {
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
	if (nPos < DEF_SPINCTRL_TIMEMINPOS)
		nPos = DEF_SPINCTRL_TIMEMINPOS;
	else if (nPos > DEF_SPINCTRL_TIMEMAXPOS)
		nPos = DEF_SPINCTRL_TIMEMAXPOS;

	// Convert
	int nHour = nPos / 60;
	int nMinute = nPos - (nHour * 60);

	// Validate
	if ((nHour != DEF_INTEGER_INVALID) && (nMinute != DEF_INTEGER_INVALID)) {
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
	if (nPos < DEF_SPINCTRL_TIMEMINPOS)
		nPos = DEF_SPINCTRL_TIMEMINPOS;
	else if (nPos > DEF_SPINCTRL_TIMEMAXPOS)
		nPos = DEF_SPINCTRL_TIMEMAXPOS;
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
	int nResult = DEF_INTEGER_INVALID;

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
	pHeaderCell->SetBackClr(DEF_COLOR_GRAY);
	pHeaderCell->SetTextClr(DEF_COLOR_BLACK);

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
	UINT nItemState = DEF_INTEGER_NULL;
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
	SYSTEMTIME tsTemp;
	GetSystemTime(&tsTemp);
	WORD wMillisecs = tsTemp.wMilliseconds;
	(CTime::GetCurrentTime()).GetAsSystemTime(tsTemp);
	tsTemp.wMilliseconds = wMillisecs;
	return tsTemp;
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
	UINT nTotalSecs = (stTime.wHour * 3600) + (stTime.wMinute * 60) + (stTime.wSecond);
	nTotalSecs += nOffset;

	// Revert back to time data
	stTime.wHour = nTotalSecs / 3600;
	stTime.wMinute = (nTotalSecs % 3600) / 60;
	stTime.wSecond = (nTotalSecs % 3600) % 60;
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
	CString strMiddayFormat = PairFuncs::GetLanguageString(pLang, nMiddayFlag);
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

BOOL CoreFuncs::LoadResourceString(CString& strResult, UINT nResStringID)
{
	BOOL bRet = strResult.LoadString(nResStringID);
	if (bRet == FALSE) {
		strResult = DEF_STRING_NULL;
	}

	return bRet;
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

int	CoreFuncs::GetTokenList(LPTSTR lpszBuff, BUFFER* retBuff, LPCTSTR lpszKeyChars)
{
	// Get length
	int nBuffLength = _tcslen(lpszBuff);
	int nKeyLength = _tcslen(lpszKeyChars);

	// Re-format given string buffer
	for (int nIndex = 0; nIndex < 256; nIndex++) {
		if (lpszBuff[nIndex] == '\n') {
			lpszBuff[nIndex] = '\0';
		}
	}

	// Index and flag
	int nCurCharIndex = 0;
	int nTokenCount = 0, nTokenCharIndex = 0;
	int nQuoteFlag = 0;

	// Loop through given string buffer and separate tokens
	while ((nCurCharIndex <= nBuffLength) && (nCurCharIndex < 256)) {
		// Init flag OFF
		int nKeyFlag = FLAG_OFF;
		// Get current character
		TCHAR tcCurChar = lpszBuff[nCurCharIndex];
		// In case of quotation mark
		if (tcCurChar == '\"') {
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
		if ((nKeyFlag == FLAG_ON) || (tcCurChar == '\0')) {
			// Empty token means continuous key letters
			if (nTokenCharIndex > 0) {
				// End current token
				retBuff[nTokenCount].tcToken[nTokenCharIndex] = '\0';
				retBuff[nTokenCount].nLength = _tcsclen(retBuff[nTokenCount].tcToken);
				nTokenCharIndex = 0;
				// Token number count-up
				nTokenCount++;
			}
		}
		// Current character is the quotation mark itself
		else if (tcCurChar == '\"') {
			// If token number exceeds max count, stop
			if (nTokenCount > DEF_TOKEN_MAXCOUNT)
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
		if ((tcCurChar == '\0') || (nTokenCount > DEF_TOKEN_MAXCOUNT))
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
		return ((tcFirst == tcSecond) && (tcFirst == _T(' ')));
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
				((nIndex > 0) && (lpszString[nIndex - 1] == _T(' ')))) {
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
	CString strInvalidKey = DEF_STRING_EMPTY;
	
	// Mark as normal first
	dwError = STRVAL_ERR_NORMAL;

	// String validation
	CString strSrc = CString(lpszSrc);
	if (strSrc.IsEmpty()) {
		// String empty
		bResult = FALSE;
		dwError = STRVAL_ERR_EMPTY;
	}
	else if (strSrc.GetLength() > DEF_STRING_MAXLENGTH) {
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
//	Function name:	SubString
//	Description:	Get a substring from a source string
//  Arguments:		lpszSrc		   - Source string
//					strDest		   - Destination string
//					tcStart		   - Start character
//					tcEnd		   - End character
//					nSubStringType - Type of substring (left/mid/right)
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CoreFuncs::SubString(LPCTSTR lpszSrc, CString& strDest, TCHAR tcStart, TCHAR tcEnd, UINT nSubStringType)
{
	CString strSrc(lpszSrc);
	if (strSrc.IsEmpty()) {
		TraceLogFormat("Function: CoreFuncs::GetSubString(), Error: Source string is empty");
		strDest = DEF_STRING_EMPTY;
		return FALSE;
	}

	// Find starting and ending index
	int nStart = (tcStart != NULL) ? (strSrc.Find(tcStart)) : DEF_INTEGER_INVALID;
	int nEnd = (tcEnd != NULL) ? (strSrc.Find(tcEnd)) : DEF_INTEGER_INVALID;

	// Debug log
	OutputDebugLogFormat(_T("[ALSTest] ==> GetSubString: nStart=%d, nEnd=%d"), nStart, nEnd);

	CString strResult = DEF_STRING_EMPTY;

	switch (nSubStringType)
	{
	case SUBSTR_LEFT:
		if (nEnd != DEF_INTEGER_INVALID)
			strResult = strSrc.Left(nEnd);
		break;
	case SUBSTR_MID:
		if ((nStart != DEF_INTEGER_INVALID) && (nEnd != DEF_INTEGER_INVALID))
			strResult = strSrc.Mid((nStart + 1), (nEnd - (nStart + 1)));
		break;
	case SUBSTR_RIGHT:
		if (nStart != DEF_INTEGER_INVALID)
			strResult = strSrc.Right(strSrc.GetLength() - (nStart + 1));
		break;
	default:
		TRCFFMT(__FUNCTION__, "Invalid argument (2)");
		break;
	}

	// Debug log
	OutputDebugLogFormat(_T("[ALSTest] ==> GetSubString return: %s"), strResult);

	// Get result
	BOOL bRet = (!strResult.IsEmpty());
	strDest = DEF_STRING_EMPTY;
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SubString
//	Description:	Get a substring from a source string
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL CoreFuncs::Left(LPCTSTR lpszSrc, CString& strDest, TCHAR tcEnd)
{
	// Get substring
	return SubString(lpszSrc, strDest, NULL, tcEnd, SUBSTR_LEFT);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SubString
//	Description:	Get a substring from a source string
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL CoreFuncs::Mid(LPCTSTR lpszSrc, CString& strDest, TCHAR tcStart, TCHAR tcEnd)
{
	// Get substring
	return SubString(lpszSrc, strDest, tcStart, tcEnd, SUBSTR_MID);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SubString
//	Description:	Get a substring from a source string
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL CoreFuncs::Right(LPCTSTR lpszSrc, CString& strDest, TCHAR tcStart)
{
	// Get substring
	return SubString(lpszSrc, strDest, tcStart, NULL, SUBSTR_RIGHT);
}

/////////////////////////////////////////////////////////////////////////////
// Additional functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppPath
//	Description:	Get app executable file path
//  Arguments:		None
//  Return value:	LPCTSTR - Return app executive file path
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR CoreFuncs::GetAppPath(void)
{
	// Get app module handler
	HMODULE hModule = GetModuleHandle(NULL);

	// Get app module file path
	TCHAR tcAppPath[MAX_PATH];
	GetModuleFileName(hModule, tcAppPath, MAX_PATH);

	CString strAppPath(tcAppPath);
	return strAppPath.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddRegistryKey
//	Description:	Add new key into registry
//  Arguments:		regKeyInfo - Registry key info data
//  Return value:	BOOL - Registry processing result
//
//////////////////////////////////////////////////////////////////////////

BOOL CoreFuncs::AddRegistryKey(REGISTRYKEY regKeyInfo)
{
	CRegKey regKey;
	LONG lResult = regKey.Open(regKeyInfo.hkTagName, regKeyInfo.lpszKeyName, KEY_ALL_ACCESS);
	if (lResult != ERROR_SUCCESS) {
		// Create register key if not found
		if (lResult == ERROR_NOT_FOUND) {
			regKey.Create(regKeyInfo.hkTagName, regKeyInfo.lpszKeyName, REG_NONE,
					 REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, NULL);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	MakeRegFullPath
//	Description:	Make full registry path
//  Arguments:		regKeyInfo	 - Registry key info data
//					nRegPathTYpe - Registry path type
//  Return value:	LPCTSTR - Registry full path
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR CoreFuncs::MakeRegFullPath(REGISTRYKEY regKeyInfo, UINT nRegPathType /* = REGPATH_FULL */)
{
	// Check data validity
	if (((regKeyInfo.hkTagName == NULL) && (regKeyInfo.nTagNameID == NULL) && (regKeyInfo.lpszTagName == DEF_STRING_EMPTY)) &&
		((regKeyInfo.nSubPathID == NULL) && (regKeyInfo.lpszSubPath == DEF_STRING_EMPTY)) &&
		((regKeyInfo.dwMasterKeyID == NULL) && (regKeyInfo.nMasterKeyNameID == NULL) && (regKeyInfo.lpszMasterKeyName == DEF_STRING_EMPTY)) &&
		((regKeyInfo.nAppNameID == NULL) && (regKeyInfo.lpszAppName == DEF_STRING_EMPTY)) &&
		((regKeyInfo.nSectionNameID == NULL) && (regKeyInfo.lpszSectionName == DEF_STRING_EMPTY)) &&
		((regKeyInfo.nKeyNameID == NULL) && (regKeyInfo.lpszKeyName == DEF_STRING_EMPTY))) {
		return DEF_STRING_NULL;
	}

	// Tag name (root key)
	CString strTagName;
	if (regKeyInfo.nTagNameID != NULL) {
		strTagName.LoadString(regKeyInfo.nTagNameID);
	}
	else if (regKeyInfo.lpszTagName != DEF_STRING_EMPTY) {
		strTagName.Format(regKeyInfo.lpszTagName);
	}

	// Sub-path
	CString strSubPath;
	if (regKeyInfo.nSubPathID != NULL) {
		strSubPath.LoadString(regKeyInfo.nSubPathID);
	}
	else if (regKeyInfo.lpszSubPath != DEF_STRING_EMPTY) {
		strSubPath.Format(regKeyInfo.lpszSubPath);
	}

	// Master key name
	CString strMasterKeyName;
	if (regKeyInfo.nMasterKeyNameID != NULL) {
		strMasterKeyName.LoadString(regKeyInfo.nMasterKeyNameID);
	}
	else if (regKeyInfo.lpszMasterKeyName != DEF_STRING_EMPTY) {
		strMasterKeyName.Format(regKeyInfo.lpszMasterKeyName);
	}

	// App name
	CString strAppName;
	if (regKeyInfo.nAppNameID != NULL) {
		strAppName.LoadString(regKeyInfo.nAppNameID);
	}
	else if (regKeyInfo.lpszAppName != DEF_STRING_EMPTY) {
		strAppName.Format(regKeyInfo.lpszAppName);
	}

	// Section name
	CString strSectionName;
	if (regKeyInfo.nSectionNameID != NULL) {
		strSectionName.LoadString(regKeyInfo.nSectionNameID);
	}
	else if (regKeyInfo.lpszSectionName != DEF_STRING_EMPTY) {
		strSectionName.Format(regKeyInfo.lpszSectionName);
	}

	// Key name
	CString strKeyName;
	if (regKeyInfo.nKeyNameID != NULL) {
		strKeyName.LoadString(regKeyInfo.nKeyNameID);
	}
	else if (regKeyInfo.lpszKeyName != DEF_STRING_EMPTY) {
		strKeyName.Format(regKeyInfo.lpszKeyName);
	}

	// Registry path type
	CString strRegFullPath;
	switch (nRegPathType)
	{
	case REGPATH_FULL:
		strRegFullPath.Format(_T("%s\\%s\\%s\\%s\\%s\\%s"), strTagName, strSubPath,
						strMasterKeyName, strAppName, strSectionName, strKeyName);
		break;
	case REGPATH_NOMASTERKEY:
		strRegFullPath.Format(_T("%s\\%s"), strTagName, strSubPath);
		break;
	case REGPATH_NOAPPNAME:
		strRegFullPath.Format(_T("%s\\%s\\%s"), strTagName, strSubPath, strMasterKeyName);
		break;
	case REGPATH_NOSECTION:
		strRegFullPath.Format(_T("%s\\%s\\%s\\%s"), strTagName, strSubPath, strMasterKeyName, strAppName);
		break;
	case REGPATH_NOKEY:
		strRegFullPath.Format(_T("%s\\%s\\%s\\%s\\%s"), strTagName, strSubPath, strMasterKeyName, strAppName, strSectionName);
		break;
	default:
		strRegFullPath.Empty();
		break;
	}

	return strRegFullPath.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PlaySound
//	Description:	Play "BEEP" sound when sound enabled
//  Arguments:		bSoundEnable - Enable sound
//					nTypeOfSound - Type of sound
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CoreFuncs::PlaySound(BOOL bSoundEnable, UINT nTypeOfSound)
{
	// Sound not enabled, do nothing
	if (!bSoundEnable)
		return;

	// Play sound here
	switch (nTypeOfSound)
	{
	case DEF_APP_SOUND_ERROR:
		::PlaySound(_T("SystemExclamination"), NULL, SND_ASYNC);
		break;
	case DEF_APP_SOUND_SUCCESS:
		::PlaySound(_T("SystemExit"), NULL, SND_ASYNC);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FileViewStd
//	Description:	View a file using external fileviewer
//  Arguments:		eFileType	 - File type
//					lpszFilePath - Path of file
//  Return value:	BOOL - Result of file opening process
//
//////////////////////////////////////////////////////////////////////////

BOOL CoreFuncs::FileViewStd(FILETYPE eFileType, LPCTSTR lpszFilePath)
{
	CString strAppPath = DEF_STRING_EMPTY;

	switch (eFileType) 
	{
	case FILETYPE_TXT:
		strAppPath = APP_NOTEPAD_PATH;
		break;
	case FILETYPE_IMG:
		break;
	default:
		return FALSE;
	}

	// Run a file viewer instance
	HWND hWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	HINSTANCE hInstance = ShellExecute(hWnd, _T("open"), strAppPath, lpszFilePath, NULL, SW_SHOW);
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
	CString strRunAs = (bRunAsAdmin) ? _T("runas") : _T("open");
	int nShowFlag = (bShowFlag) ? SW_SHOW : SW_HIDE;

	// Run an executable instance
	HINSTANCE hInstance = ShellExecute(0, strRunAs, lpszAppPath, 0, 0, nShowFlag);
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

LRESULT CoreFuncs::ExecuteCommand(LPCTSTR lpszCmd, BOOL bRunAsAdmin /* = TRUE */, BOOL bShowFlag /* = TRUE */)
{
	// Format command
	CString strCmdFormat;
	strCmdFormat.Format(_T("/C %s"), lpszCmd);

	// Param set
	CString strRunAs = (bRunAsAdmin) ? _T("runas") : _T("open");
	int nShowFlag = (bShowFlag) ? SW_SHOW : SW_HIDE;

	// Excute command
	HINSTANCE hInstance = ShellExecute(0, strRunAs, _T("cmd.exe"), strCmdFormat, 0, nShowFlag);
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
	HMODULE hUxTheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
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

void CoreFuncs::DrawButton(CButton*& pBtn, UINT nIconID, LPCTSTR lpszBtnTitle)
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
	if (rcBtnRect == NULL)
		return;

	// Button title
	CString strBtnTitle;
	strBtnTitle.SetString(lpszBtnTitle);
	if (strBtnTitle.IsEmpty())
		pBtn->GetWindowText(strBtnTitle);

	// Update button
	pBtn->SetButtonStyle(BS_ICON);
	pBtn->SetIcon(hBtnIcon);
	pBtn->UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnumFontFamExProc
//	Description:	Callback function used with the EnumFontFamiliesEx
//  Arguments:		Default (see MSDN "EnumFontFamProc callback function")
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam) 
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
	LOGFONT logfont = { 0 };
	logfont.lfCharSet = DEFAULT_CHARSET;

	// Get font families
	HDC hdc = GetDC(NULL);
	EnumFontFamiliesEx(hdc, &logfont, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)&fontNames, 0);
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
	if (bRet == FALSE)
		return FALSE;

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

