/**
 * @file		PwrReminderDlg.cpp
 * @brief		Source file for Power Reminder dialog
 * @author		AnthonyLeeStark
 * @date		2024.07.20
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"
#include "Dialogs/PwrReminderDlg.h"
#include "Dialogs/ReminderMsgDlg.h"
#include "Dialogs/RmdRepeatSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MapTable;
using namespace Language;
using namespace AppCore;
using namespace AppRegistry;


// Data list table constants
constexpr const int fixedColumnNum = 1;
constexpr const int fixedRowNum = 1;


// Implement methods for CPwrReminderDlg
IMPLEMENT_DYNAMIC(CPwrReminderDlg, SDialog)


/**
 * @brief	Constructor
 */
CPwrReminderDlg::CPwrReminderDlg(CWnd* pParent /*=nullptr*/)
	: SDialog(IDD_PWRREMINDER_DLG, pParent)
{
	// Initialize member variables
	m_pDataItemListTable = NULL;
	m_pMsgStringEdit = NULL;
	m_pEvtSetTimeRad = NULL;
	m_pEvtSetTimeEdit = NULL;
	m_pEvtSetTimeSpin = NULL;
	m_pEvtRepeatSetBtn = NULL;
	m_pEvtAppStartupRad = NULL;
	m_pEvtSysWakeupRad = NULL;
	m_pEvtBfrPwrActionRad = NULL;
	m_pEvtPwrActionWakeRad = NULL;
	m_pEvtAtAppExitRad = NULL;
	m_pStyleMsgBoxRad = NULL;
	m_pStyleDialogBoxRad = NULL;
	m_pMsgStyleCombo = NULL;

	// Properties child dialogs
	m_pRmdPreviewMsgDlg = NULL;
	m_pRepeatSetDlg = NULL;

	// Checkbox/radio button variables
	m_bEvtSetTimeRad = FALSE;
	m_bEvtAppStartupRad = FALSE;
	m_bEvtSysWakeupRad = FALSE;
	m_bEvtBfrPwrActionRad = FALSE;
	m_bEvtPwrActionWakeRad = FALSE;
	m_bEvtAppExitRad = FALSE;
	m_bStyleMsgBoxRad = FALSE;
	m_bStyleDialogRad = FALSE;

	// Table format and properties
	m_nColNum = 0;
	m_pszFrameWndSize = NULL;
	m_apGrdColFormat = NULL;

	// Other variables
	m_nCurMode = 0;
	m_nCheckCount = 0;
	m_nCurSelIndex = -1;
	m_nCurDispIndex = -2;
	m_stDispTimeBak = ClockTime();
}

/**
 * @brief	Destructor
 */
CPwrReminderDlg::~CPwrReminderDlg()
{
	// Delete child dialogs
	if (m_pRmdPreviewMsgDlg != NULL) {
		// Destroy dialog
		m_pRmdPreviewMsgDlg->DestroyWindow();
		delete m_pRmdPreviewMsgDlg;
		m_pRmdPreviewMsgDlg = NULL;
	}
	if (m_pRepeatSetDlg != NULL) {
		// Destroy dialog
		m_pRepeatSetDlg->DestroyWindow();
		delete m_pRepeatSetDlg;
		m_pRepeatSetDlg = NULL;
	}

	// Data item list control
	if (m_pDataItemListTable) {
		delete m_pDataItemListTable;
		m_pDataItemListTable = NULL;
	}
	
	// Remove Reminder data
	m_pwrReminderData.DeleteAll();
	m_pwrReminderDataTemp.DeleteAll();

	// Table format and properties
	if (m_pszFrameWndSize != NULL) {
		delete m_pszFrameWndSize;
		m_pszFrameWndSize = NULL;
	}

	if (m_apGrdColFormat != NULL) {
		delete[] m_apGrdColFormat;
		m_apGrdColFormat = NULL;
	}
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CPwrReminderDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_SETTIME_RADBTN,		m_bEvtSetTimeRad);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN,		m_bEvtAppStartupRad);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN,		m_bEvtSysWakeupRad);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN,	m_bEvtBfrPwrActionRad);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN,	m_bEvtPwrActionWakeRad);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN,		m_bEvtAppExitRad);
	DDX_Check(pDX, IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN,		m_bStyleMsgBoxRad);
	DDX_Check(pDX, IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN,		m_bStyleDialogRad);
}

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CPwrReminderDlg::RegisterDialogManagement(void)
{
	size_t nRet = SDialog::RegisterDialogManagement();
	if (nRet != 0) {
		TRACE_ERROR("Error: Register dialog management failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return nRet;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_ITEM_LISTBOX, List_Control);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_ADD_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EDIT_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_REMOVE_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_REMOVEALL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_CHECKALL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_UNCHECKALL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_PREVIEW_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_APPLY_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_CANCEL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_DETAIL_STATIC, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTRING_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTRING_EDITBOX, Edit_Control);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTRING_COUNTER, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN, Radio_Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX, Edit_Control);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_SETTIME_SPIN, Spin_Control);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_REPEATSET_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN, Radio_Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN, Radio_Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN, Radio_Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN, Radio_Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN, Radio_Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTYLE_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN, Radio_Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN, Radio_Button);
	}

	return nRet;
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	TRUE/FALSE
 */
BOOL CPwrReminderDlg::UnregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_ITEM_LISTBOX);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_ADD_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EDIT_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_REMOVE_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_REMOVEALL_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_CHECKALL_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_UNCHECKALL_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_PREVIEW_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_APPLY_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_CANCEL_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_DETAIL_STATIC);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_MSGSTRING_TITLE);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_MSGSTRING_EDITBOX);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_MSGSTRING_COUNTER);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EVENT_TITLE);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EVENT_SETTIME_SPIN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EVENT_REPEATSET_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_MSGSTYLE_TITLE);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN);
	}

	return SDialog::UnregisterDialogManagement();
}

//////////////////////////////////////////////////////////////////////////
//
//	CPwrReminderDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCEID_MAP(CPwrReminderDlg)
	ON_ID_DIALOG(IDD_PWRREMINDER_DLG,							"PwrReminderDlg")
	ON_ID_CONTROL(IDC_PWRREMINDER_ITEM_LISTBOX,					"PwrReminderItemList")
	ON_ID_CONTROL(IDC_PWRREMINDER_ADD_BTN,						"AddButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_EDIT_BTN,						"EditButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_REMOVE_BTN,					"RemoveButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_REMOVEALL_BTN,				"RemoveAllButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_CHECKALL_BTN,					"CheckAllButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_UNCHECKALL_BTN,				"UncheckAllButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_PREVIEW_BTN,					"PreviewButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_APPLY_BTN,					"ApplyButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_CANCEL_BTN,					"CancelButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_DETAIL_STATIC,				"DetailGroup")
	ON_ID_CONTROL(IDC_PWRREMINDER_MSGSTRING_TITLE,				"MsgContentTitle")
	ON_ID_CONTROL(IDC_PWRREMINDER_MSGSTRING_EDITBOX,			"MsgContentEdit")
	ON_ID_CONTROL(IDC_PWRREMINDER_MSGSTRING_COUNTER,			"MsgCounter")
	ON_ID_CONTROL(IDC_PWRREMINDER_EVENT_TITLE,					"EventGroup")
	ON_ID_CONTROL(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN,			"EventSetTimeRadio")
	ON_ID_CONTROL(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX,		"EventSetTimeEdit")
	ON_ID_CONTROL(IDC_PWRREMINDER_EVENT_SETTIME_SPIN,			"EventSetTimeSpin")
	ON_ID_CONTROL(IDC_PWRREMINDER_EVENT_REPEATSET_BTN,			"RepeatSetButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN,		"EventAppStartupRadio")
	ON_ID_CONTROL(IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN,		"EventSysWakeupRadio")
	ON_ID_CONTROL(IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN,	"EventBfrPwrActionRadio")
	ON_ID_CONTROL(IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN,	"EventPwrActionWakeRadio")
	ON_ID_CONTROL(IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN,		"EventAtAppExitRadio")
	ON_ID_CONTROL(IDC_PWRREMINDER_MSGSTYLE_TITLE,				"MsgStyleGroup")
	ON_ID_CONTROL(IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN,		"StyleMessageBoxRadio")
	ON_ID_CONTROL(IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN,		"StyleDialogBoxRadio")
END_RESOURCEID_MAP()


//////////////////////////////////////////////////////////////////////////
//
//	CPwrReminderDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPwrReminderDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PWRREMINDER_APPLY_BTN,					&CPwrReminderDlg::OnApply)
	ON_BN_CLICKED(IDC_PWRREMINDER_CANCEL_BTN,					&CPwrReminderDlg::OnCancel)
	ON_BN_CLICKED(IDC_PWRREMINDER_ADD_BTN,						&CPwrReminderDlg::OnAdd)
	ON_BN_CLICKED(IDC_PWRREMINDER_EDIT_BTN,						&CPwrReminderDlg::OnEdit)
	ON_BN_CLICKED(IDC_PWRREMINDER_REMOVE_BTN,					&CPwrReminderDlg::OnRemove)
	ON_BN_CLICKED(IDC_PWRREMINDER_REMOVEALL_BTN,		 		&CPwrReminderDlg::OnRemoveAll)
	ON_BN_CLICKED(IDC_PWRREMINDER_CHECKALL_BTN,			 		&CPwrReminderDlg::OnCheckAll)
	ON_BN_CLICKED(IDC_PWRREMINDER_UNCHECKALL_BTN,				&CPwrReminderDlg::OnUncheckAll)
	ON_BN_CLICKED(IDC_PWRREMINDER_PREVIEW_BTN,					&CPwrReminderDlg::OnPreviewItem)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PWRREMINDER_ITEM_LISTBOX,	&CPwrReminderDlg::OnSelectReminderItem)
	ON_NOTIFY(NM_CLICK, IDC_PWRREMINDER_ITEM_LISTBOX,			&CPwrReminderDlg::OnClickDataItemList)
	ON_NOTIFY(NM_RCLICK, IDC_PWRREMINDER_ITEM_LISTBOX,			&CPwrReminderDlg::OnRightClickDataItemList)
	ON_EN_CHANGE(IDC_PWRREMINDER_MSGSTRING_EDITBOX,				&CPwrReminderDlg::OnMsgContentEditChange)
	ON_EN_SETFOCUS(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX,		&CPwrReminderDlg::OnTimeEditSetFocus)
	ON_EN_KILLFOCUS(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX,		&CPwrReminderDlg::OnTimeEditKillFocus)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PWRREMINDER_EVENT_SETTIME_SPIN,	&CPwrReminderDlg::OnTimeSpinChange)
	ON_BN_CLICKED(IDC_PWRREMINDER_EVENT_REPEATSET_BTN,			&CPwrReminderDlg::OnRepeatSet)
	ON_COMMAND_RANGE(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN, IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN, &CPwrReminderDlg::OnPwrEventRadBtnClicked)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// PwrReminderDlg message handlers

/**
 * @brief	Initialize Power Reminder dialog and setup dialog content
 * @param	None
 * @return	BOOL - Default
 */
BOOL CPwrReminderDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Do not use Enter button
	SetUseEnter(FALSE);

	// Register message box caption
	RegisterMessageBoxCaption(MSGBOX_PWRREMINDER_CAPTION);

	// Load data
	LoadPwrReminderData();

	// Init dialog items
	LoadLayoutInfo();
	SetupLanguage();
	SetupDialogItemState();

	// Update data
	UpdateDataItemList();
	DisplayItemDetails(INT_INVALID);
	RefreshDialogItemState(TRUE);

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, this->GetCaption());

	// Read-only mode (if enabled)
	if (GetReadOnlyMode() == TRUE) {
		CWnd* pWndChild = GetTopWindow();
		while (pWndChild != NULL) {
			pWndChild->EnableWindow(FALSE);
			pWndChild = pWndChild->GetWindow(GW_HWNDNEXT);
		}
	}

	return TRUE;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnClose()
{
	// If not forced closing by request
	if (!IsForceClosingByRequest()) {

		// Exit current mode
		int nConfirm = -1;
		int nCurMode = GetCurMode();
		if ((nCurMode == Mode::Add) || (nCurMode == Mode::Update)) {
			// Show switch mode confirmation message
			nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, NULL, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Switch mode
				SetCurMode(Mode::View);
			}
			return;
		}

		// Ask for saving before exiting if data changed
		BOOL bIsChanged = CheckDataChangeState();
		SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
		if (bIsChanged == TRUE) {
			// Show save confirmation message
			nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CHANGED_CONTENT, NULL, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Save data
				SavePwrReminderData();
			}
		}
	}

	// Close dialog
	SDialog::OnClose();
}

/**
 * @brief	Default method for dialog destroying
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnDestroy()
{
	// Save app event log if enabled
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, this->GetCaption());

	// Save layout info data
	UpdateLayoutInfo();
	SaveLayoutInfo();

	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	Handle click event for [Apply] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnApply()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_APPLY_BTN);

	// Save data if changed
	BOOL bIsChanged = CheckDataChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
	if (bIsChanged == TRUE) {
		// Save data
		SavePwrReminderData();
	}

	// Close dialog
	SDialog::OnOK();
}

/**
 * @brief	Handle click event for [Cancel] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnCancel()
{
	// If not forced closing by request
	if (!IsForceClosingByRequest()) {

		// Save app event log if enabled
		OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_CANCEL_BTN);

		// Exit current mode
		int nConfirm = -1;
		int nCurMode = GetCurMode();
		if ((nCurMode & Mode::Add) || (nCurMode & Mode::Update)) {
			// Show switch mode confirmation message
			nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, NULL, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Switch mode
				SetCurMode(Mode::View);
			}
			return;
		}

		// Ask for saving before exiting if data changed
		BOOL bIsChanged = CheckDataChangeState();
		SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
		if (bIsChanged == TRUE) {
			// Show save confirmation message
			nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CHANGED_CONTENT, NULL, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Save data
				SavePwrReminderData();
			}
		}
	}

	// Close dialog
	SDialog::OnCancel();
}

/**
 * @brief	Handle click event for [Add] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnAdd()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_ADD_BTN);

	// Get current mode
	int nCurMode = GetCurMode();

	// Current mode: Add
	if (nCurMode & Mode::Add) {
		// Add item
		Add();

		// Reset mode
		SetCurMode(Mode::View);
	}
	// Current mode: Edit
	else if (nCurMode & Mode::Update) {
		// Do nothing
	}
	else {
		// Switch mode
		SetCurMode(Mode::Add);
	}
}

/**
 * @brief	Handle click event for [Edit] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnEdit()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_EDIT_BTN);

	// Get current mode
	int nCurMode = GetCurMode();

	// Mode: Edit
	if (nCurMode & Mode::Update) {
		// Check if any item is selected or not
		BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetItemNum()));

		if (bIsSelected == TRUE) {
			// Edit current selected item
			Edit(m_nCurSelIndex);
		}

		// Reset mode
		SetCurMode(Mode::View);
	}
	// Mode: Add
	else if (nCurMode & Mode::Add) {
		// Do nothing
	}
	else {
		// Switch mode
		SetCurMode(Mode::Update);
	}
}

/**
 * @brief	Handle click event for [Remove] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnRemove()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_REMOVE_BTN);

	// If there's no item, do nothing
	int nItemNum = GetItemNum();
	if (nItemNum <= 0)
		return;

	// Get current select item index
	int nIndex = m_nCurSelIndex;

	// If item at selected index is empy, do nothing
	if (m_pwrReminderDataTemp.IsEmpty(nIndex) == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_REMOVE_ITEM, NULL, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {
		// Remove item
		Remove(nIndex);
	}
}

/**
 * @brief	Handle click event for [Remove All] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnRemoveAll()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_REMOVEALL_BTN);
	
	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.IsAllEmpty() == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_REMOVEALL_ITEMS, NULL, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {
		// Remove all items
		RemoveAll();
	}
}

/**
 * @brief	Handle click event for [Check All] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnCheckAll()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_CHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.IsAllEmpty() == TRUE)
		return;

	// Check all items
	SetAllItemState(TRUE);
}

/**
 * @brief	Handle click event for [Uncheck All] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnUncheckAll()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_UNCHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.IsAllEmpty() == TRUE)
		return;

	// Uncheck all items
	SetAllItemState(FALSE);
}

/**
 * @brief	Handle click event for [Preview] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnPreviewItem()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_PREVIEW_BTN);

	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.IsAllEmpty() == TRUE)
		return;

	// Get current selection index
	int nCurSel = m_nCurSelIndex;
	if (m_pwrReminderDataTemp.IsEmpty(nCurSel) == TRUE)
		return;

	// Preview item
	int nIndex = nCurSel;
	PreviewItem(nIndex);
}

/**
 * @brief	Show details when selecting a reminder item
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CPwrReminderDlg::OnSelectReminderItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nRow = pItem->iRow;

	//Get current selection index
	m_nCurSelIndex = nRow - fixedRowNum;
	int nItemCount = GetItemNum();

	*pResult = NULL;

	// Invalid selection
	if ((m_nCurSelIndex < 0) || (m_nCurSelIndex >= nItemCount))
		return;

	// Check read-only mode
	int nCurMode = GetCurMode();
	if ((nCurMode != Mode::Init) && (nCurMode != Mode::View))
		return;

	// Display item details
	DisplayItemDetails(m_nCurSelIndex);
	RefreshDialogItemState(TRUE);
}

/**
 * @brief	Handle click event on Power Reminder data
					item list control
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CPwrReminderDlg::OnClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nClickedRow = pItem->iRow;

	// Check value validity
	int nItemNum = GetItemNum();
	if ((nClickedRow <= GRIDCTRL_INDEX_HEADER_ROW) || (nClickedRow > nItemNum)) {
		return;
	}

	// Check read-only mode
	int nCurMode = GetCurMode();
	if ((nCurMode != Mode::Init) && (nCurMode != Mode::View))
		return;

	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	RefreshDialogItemState(TRUE);
}

/**
 * @brief	Handle right click event on Power Reminder data
					item list control
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CPwrReminderDlg::OnRightClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nClickedRow = pItem->iRow;

	// Check value validity
	int nItemNum = GetItemNum();
	if ((nClickedRow <= GRIDCTRL_INDEX_HEADER_ROW) || (nClickedRow > nItemNum)) {
		return;
	}

	// Check read-only mode
	int nCurMode = GetCurMode();
	if ((nCurMode != Mode::Init) && (nCurMode != Mode::View))
		return;

	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	RefreshDialogItemState(TRUE);
}

/**
 * @brief	Update when message content editbox value changes
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnMsgContentEditChange()
{
	// Check control validity
	if (m_pMsgStringEdit == NULL) {
		TRACE_ERROR("Error: Message content edit control not found!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Update data
	const int buffLength = m_pMsgStringEdit->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(buffLength + 1);
	m_pMsgStringEdit->GetWindowText(tempBuff.data(), buffLength + 1);
	String textValue = tempBuff.data();

	// Update message counter
	int nCount = textValue.GetLength();
	UpdateMsgCounter(nCount);
}

/**
 * @brief	Select all text when editbox is set focus
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnTimeEditSetFocus()
{
	/*********************************************************************/
	/*																	 */
	/*			TODO: Time edit set focus --> Select all text			 */
	/*																	 */
	/*********************************************************************/
	// Check control validity
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Backup current displaying time value
	UpdateTimeSetting(m_stDispTimeBak, TRUE);

	// Select all text
	m_pEvtSetTimeEdit->PostMessage(EM_SETSEL, 0, -1);
}

/**
 * @brief	Update when time editbox is killed focus
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnTimeEditKillFocus()
{
	/*********************************************************************/
	/*																	 */
	/*			TODO: Time edit kill forcus --> Update data			     */
	/*																	 */
	/*********************************************************************/
	// Check control validity
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Update data
	const int buffLength = m_pEvtSetTimeEdit->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(buffLength + 1);
	m_pEvtSetTimeEdit->GetWindowText(tempBuff.data(), buffLength + 1);
	String timeTextValue = tempBuff.data();

	ClockTime clockTime;
	if (ClockTimeUtils::InputText2Time(clockTime, timeTextValue)) {

		// Update new time value
		UpdateTimeSetting(clockTime, FALSE);

		// Update timespin new position
		int nSpinPos = 0;
		ClockTimeUtils::Time2SpinPos(clockTime, nSpinPos);
		if (m_pEvtSetTimeSpin != NULL) {
			m_pEvtSetTimeSpin->SetPos(nSpinPos);
		}
	}
	else {
		// Restore backed-up time value
		UpdateTimeSetting(m_stDispTimeBak, FALSE);
		return;
	}

	// Check for value change and enable/disable save button
	SetFlagValue(AppFlagID::dialogDataChanged, CheckDataChangeState());
}

/**
 * @brief	Update when time spin value changes
 * @param	pNMDHR  - Default of spin event handler
 * @param	pResult - Default of spin event handler
 * @return	None
 */
void CPwrReminderDlg::OnTimeSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// Get timespin position
	int nPos = pNMUpDown->iPos;

	// Convert to time value
	ClockTime clockTime;
	ClockTimeUtils::SpinPos2Time(clockTime, nPos);

	// Update time edit value
	UpdateTimeSetting(clockTime, FALSE);

	*pResult = NULL;

	// Check for value change and enable/disable save button
	SetFlagValue(AppFlagID::dialogDataChanged, CheckDataChangeState());
}

/**
 * @brief	Handle clicked for event radio buttons
 * @param	nID - ID of button
 * @return	None
 */
void CPwrReminderDlg::OnPwrEventRadBtnClicked(UINT nID)
{
	// Ignore if clicked on items with in-range IDs but not radio buttons
	if ((nID == IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX) ||
		(nID == IDC_PWRREMINDER_EVENT_SETTIME_SPIN))
		return;

	// Get EventSetTime radio button
	if (m_pEvtSetTimeRad == NULL) {
		m_pEvtSetTimeRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN);
		if (m_pEvtSetTimeRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventSetTimeRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// If button is disabled, do nothing
	if (m_pEvtSetTimeRad->IsWindowEnabled() == FALSE)
		return;

	// Update checked state
	int nState = m_pEvtSetTimeRad->GetCheck();

	// Check control validity
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtSetTimeSpin == NULL) {
		m_pEvtSetTimeSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_SPIN);
		if (m_pEvtSetTimeSpin == NULL) {
			TRACE_ERROR("Error: Time spin control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Enable/disable time spinedit and RepeatSet button
	if (nState == 1) {
		m_pEvtSetTimeEdit->EnableWindow(TRUE);
		m_pEvtSetTimeSpin->EnableWindow(TRUE);
		m_pEvtRepeatSetBtn->EnableWindow(TRUE);
	}
	else {
		m_pEvtSetTimeEdit->EnableWindow(FALSE);
		m_pEvtSetTimeSpin->EnableWindow(FALSE);
		m_pEvtRepeatSetBtn->EnableWindow(FALSE);
	}
}

/**
 * @brief	Handle click event for [RepeatSet] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnRepeatSet()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_EVENT_REPEATSET_BTN);

	// Initialize RepeatSet dialog if not available
	if (m_pRepeatSetDlg == NULL) {
		m_pRepeatSetDlg = new CRmdRepeatSetDlg;
		m_pRepeatSetDlg->Create(IDD_RMDREPEATSET_DLG);
		m_pRepeatSetDlg->RemoveStyle(DS_MODALFRAME | WS_CAPTION);
		m_pRepeatSetDlg->SetParentWnd(this);
	}

	// Get button position
	CRect rcButton;
	if (m_pEvtRepeatSetBtn != NULL) {
		// Get button rectangle
		m_pEvtRepeatSetBtn->GetWindowRect(&rcButton);
	}

	// If the dialog has already been initialized
	if (m_pRepeatSetDlg != NULL) {
		// If the dialog is currently displaying
		if (m_pRepeatSetDlg->IsWindowVisible()) {
			// Hide the dialog
			m_pRepeatSetDlg->ShowWindow(SW_HIDE);
		}
		else {
			// Set dialog alignment
			UINT nAlign = SDA_LEFTALIGN | SDA_TOPALIGN;
			m_pRepeatSetDlg->SetAlignment(nAlign);

			// Get button top-right point
			POINT ptBtnTopRight;
			ptBtnTopRight.x = rcButton.right;
			ptBtnTopRight.y = rcButton.top;

			// Set dialog anchor point
			m_pRepeatSetDlg->SetAnchorPoint(ptBtnTopRight);

			// Show dialog
			m_pRepeatSetDlg->ShowWindow(SW_SHOW);
		}
	}
}

/**
 * @brief	Request current dialog to close
 * @param	None
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT CPwrReminderDlg::RequestCloseDialog(void)
{
	// Close preview message dialog if opening
	if (m_pRmdPreviewMsgDlg != NULL) {
		// Request close message dialog
		LRESULT resClosePreview = m_pRmdPreviewMsgDlg->RequestCloseDialog();
		if (resClosePreview != Result::Success) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}

	// If RepeatSet edit dialog is opening
	if (m_pRepeatSetDlg != NULL) {
		// Request close dialog
		LRESULT resCloseRepeatSet = m_pRepeatSetDlg->RequestCloseDialog();
		if (resCloseRepeatSet != Result::Success)
			return resCloseRepeatSet;
	}

	// Exit current mode
	int nConfirm = -1;
	int nCurMode = GetCurMode();
	if ((nCurMode == Mode::Add) || (nCurMode == Mode::Update)) {
		nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Switch mode
			SetCurMode(Mode::View);
		}
		else if (nConfirm == IDCANCEL) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}

	// Ask for saving before exiting if data changed
	BOOL bIsChanged = CheckDataChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
	if (bIsChanged == TRUE) {
		nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CHANGED_CONTENT, NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Save data
			SavePwrReminderData();
		}
		else if (nConfirm == IDCANCEL) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}

	// Request accepted
	return SDialog::RequestCloseDialog();
}

/**
 * @brief	Pre-handle dialog messages
 * @param	message - Message ID
 * @param	wParam	- First param (HIWORD)
 * @param	lParam	- Second param (LOWORD)
 * @return	None
 */
LRESULT CPwrReminderDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONUP:
	{
		// Get clicked point
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (m_pEvtSetTimeEdit == NULL)
			return 0;

		// Get the editbox rect
		RECT rcEditBox;
		m_pEvtSetTimeEdit->GetWindowRect(&rcEditBox);
		ScreenToClient(&rcEditBox);

		// If clicked point is inside the editbox area
		if (((pt.x > rcEditBox.left) && (pt.x < rcEditBox.right)) &&
			((pt.y > rcEditBox.top) && (pt.y < rcEditBox.bottom))) {
			if (m_pEvtSetTimeEdit->IsWindowEnabled()) {
				// Select all text
				m_pEvtSetTimeEdit->SetSel(0, -1, TRUE);
			}
			return 0;
		}
		else {
			// If the edit box is focused, kill its focus
			CWnd* pCurCtrl = GetFocus();
			if (pCurCtrl == NULL) return 0;
			if (pCurCtrl->GetDlgCtrlID() == IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX) {
				pCurCtrl->PostMessage(WM_KILLFOCUS);
				this->SetFocus();	// Return focus to dialog
				return 0;
			}
		}
	} break;
	}

	return SDialog::WindowProc(message, wParam, lParam);
}

/**
 * @brief	Setup language for dialog items
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::SetupLanguage()
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Setup dialog title
	this->SetCaptionFromLanguage(GetDialogID());

	// Loop through all dialog items and setup language for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		UINT nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDC_PWRREMINDER_DETAIL_STATIC:
		case IDC_PWRREMINDER_ITEM_LISTBOX:
		case IDC_PWRREMINDER_MSGSTRING_EDITBOX:
		case IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX:
		case IDC_PWRREMINDER_EVENT_SETTIME_SPIN:
			// Skip these items
			break;

		case IDC_PWRREMINDER_EVENT_REPEATSET_BTN:
			// Draw icon
			DrawRepeatSetButton();
			break;

		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}

	// Setup data item list
	SetupDataItemList(pAppLang);

	// Default
	SDialog::SetupLanguage();
}

/**
 * @brief	Initialize and setup language for Power Reminder data item list
 * @param	ptrLanguage - Language package pointer
 * @return	None
 */
void CPwrReminderDlg::SetupDataItemList(LANGTABLE_PTR ptrLanguage)
{
	// Get parent list frame rect
	CWnd* pListFrameWnd = GetDlgItem(IDC_PWRREMINDER_ITEM_LISTBOX);
	if (pListFrameWnd == NULL) return;
	RECT rcListFrameWnd;
	pListFrameWnd->GetWindowRect(&rcListFrameWnd);
	ScreenToClient(&rcListFrameWnd);

	// Get frame size
	if (m_pszFrameWndSize == NULL) {
		m_pszFrameWndSize = new CSize();
		m_pszFrameWndSize->cx = rcListFrameWnd.right - rcListFrameWnd.left;
		m_pszFrameWndSize->cy = rcListFrameWnd.bottom - rcListFrameWnd.top;
	}

	// Initialization
	VERIFY_INITIALIZATION(m_pDataItemListTable, CGridCtrl)

	// Create table
	if (m_pDataItemListTable == NULL) return;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	m_pDataItemListTable->Create(rcListFrameWnd, this, IDC_PWRREMINDER_ITEM_LISTBOX, dwStyle);

	// Destroy frame
	pListFrameWnd->DestroyWindow();

	// Cell format
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pDataItemListTable->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;
	pCell->SetFormat(pCell->GetFormat());
	pCell->SetMargin(0);
	pCell->SetBackClr(Color::White);
	pCell->SetTextClr(Color::Black);
	pCell->SetHeight(GRIDCTRL_HEIGHT_ROW);

	// Table format and properties
	int nRowNum = (GetItemNum() + fixedRowNum);
	int nColNum = m_nColNum;

	// Setup table
	m_pDataItemListTable->SetColumnCount(nColNum);
	m_pDataItemListTable->SetFixedColumnCount(fixedColumnNum);
	m_pDataItemListTable->SetRowCount(nRowNum);
	m_pDataItemListTable->SetFixedRowCount(fixedColumnNum);
	m_pDataItemListTable->SetRowHeight(GRIDCTRL_INDEX_HEADER_ROW, GRIDCTRL_HEIGHT_HEADER);

	// Draw table
	DrawDataTable(m_pszFrameWndSize, nColNum, nRowNum, FALSE, ptrLanguage);

	// Update layout info
	UpdateLayoutInfo();

	// Display table
	m_pDataItemListTable->SetListMode(TRUE);
	m_pDataItemListTable->SetEditable(FALSE);
	m_pDataItemListTable->SetRowResize(FALSE);
	m_pDataItemListTable->EnableSelection(TRUE);
	m_pDataItemListTable->SetSingleRowSelection(TRUE);
	m_pDataItemListTable->SetSingleColSelection(FALSE);
	m_pDataItemListTable->SetFixedRowSelection(FALSE);
	m_pDataItemListTable->SetFixedColumnSelection(FALSE);
	m_pDataItemListTable->ShowWindow(SW_SHOW);
	m_pDataItemListTable->SetRedraw(TRUE);
}

/**
 * @brief	Draw data list table
 * @param	szFrameWndSize	- Frame size
 * @param	nColNum			- Number of table columns
 * @param	nRowNum			- Number of table rows
 * @param	bReadOnly		- Read-only mode
 * @param	ptrLanguage		- Pointer to app language
 * @return	None
 */
void CPwrReminderDlg::DrawDataTable(CSize* pszFrameWndSize, int nColNum, int nRowNum, BOOL bReadOnly /* = FALSE */, LANGTABLE_PTR ptrLanguage /* = NULL */)
{
	// Check table validity
	if (m_pDataItemListTable == NULL)
		return;

	// Check table format data validity
	if (pszFrameWndSize == NULL) return;
	if (m_apGrdColFormat == NULL) return;

	// Check row and column number validity
	if ((nColNum <= 0) || (nRowNum < fixedRowNum))
		return;

	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;

	// Load app language package
	if (ptrLanguage == NULL) {
		ptrLanguage = pApp->GetAppLanguage();
	}

	// Re-update default cell properties
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pDataItemListTable->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;

	// Read-only mode --> Change cell color
	if (bReadOnly == TRUE) {
		pCell->SetBackClr(Color::Bright_Gray);
		pCell->SetTextClr(Color::Dark_Gray);
	}
	else {
		pCell->SetBackClr(Color::White);
		pCell->SetTextClr(Color::Black);
	}

	// Setup display size
	int nFrameHeight = pszFrameWndSize->cy;
	int nFrameWidth = pszFrameWndSize->cx;
	if (GetWindowsOSVersion() == WINDOWS_VERSION_10) {
		// Windows 10 list control offset
		nFrameWidth -= OFFSET_WIDTH_LISTCTRL_WIN10;
		//nFrameHeight -= OFFSET_HEIGHT_LISTCTRL_WIN10;
	}
	else {
		// Windows 11 list control offset
		nFrameWidth -= OFFSET_WIDTH_LISTCTRL;
		//nFrameHeight -= OFFSET_HEIGHT_LISTCTRL;
	}
	if ((GRIDCTRL_HEIGHT_HEADER + ((nRowNum - 1) * GRIDCTRL_HEIGHT_ROW)) >= nFrameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nFrameWidth -= (nScrollBarWidth + OFFSET_WIDTH_VSCRLBR);
	}

	// Setup columns
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Set header row style
		SetFixedCellStyle(m_pDataItemListTable, GRIDCTRL_INDEX_HEADER_ROW, nCol);

		// Column header title
		String headerTitle = Constant::String::Empty;
		UINT nHeaderTitleID = m_apGrdColFormat[nCol].nHeaderTitleID;
		if (nHeaderTitleID != INT_NULL) {
			headerTitle = GetLanguageString(ptrLanguage, nHeaderTitleID);
		}
		m_pDataItemListTable->SetItemText(GRIDCTRL_INDEX_HEADER_ROW, nCol, headerTitle);

		// Column width
		int nColWidth = m_apGrdColFormat[nCol].nWidth;
		if (nColWidth != -1) {
			// Set column width as defined
			m_pDataItemListTable->SetColumnWidth(nCol, nColWidth);
			// Calculate remaining width
			nFrameWidth -= nColWidth;
		}
		else {
			// Set remaining width for current column
			m_pDataItemListTable->SetColumnWidth(nCol, nFrameWidth);
		}
	}

	// Setup rows
	int nColStyle = -1;
	UINT nItemState = INT_NULL;
	for (int nRow = 1; nRow < nRowNum; nRow++) {
		for (int nCol = 0; nCol < m_nColNum; nCol++) {
			// Get column style & item state
			nColStyle = m_apGrdColFormat[nCol].nColStyle;
			nItemState = m_pDataItemListTable->GetItemState(nRow, nCol);
			nItemState |= GVIS_READONLY;

			// Base column - header-like style
			if (nColStyle == COLSTYLE_FIXED) {
				// Set fixed cell style
				SetFixedCellStyle(m_pDataItemListTable, nRow, nCol);
			}

			// Checkbox column
			else if (nColStyle == COLSTYLE_CHECKBOX) {
				// Set cell type: Checkbox
				if (!m_pDataItemListTable->SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck)))
					continue;

				// Get cell
				CGridCellCheck* pCell = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRow, nCol);

				// Set center alignment if defined
				if (m_apGrdColFormat[nCol].bCenter == TRUE) {
					if (pCell == NULL) continue;
					pCell->SetCheckPlacement(SCP_CENTERING);
				}
			}

			// Normal column
			else if (nColStyle == COLSTYLE_NORMAL) {
				// Set item state
				if (!m_pDataItemListTable->SetItemState(nRow, nCol, nItemState))
					continue;

				// Get cell
				CGridCellBase* pCell = (CGridCellBase*)m_pDataItemListTable->GetCell(nRow, nCol);

				// Set center alignment if defined
				if (m_apGrdColFormat[nCol].bCenter == TRUE) {
					if (pCell == NULL) continue;
					pCell->SetFormat(pCell->GetFormat() | DT_CENTER);
				}
				else {
					// Set margin (left alignment)
					if (pCell == NULL) continue;
					pCell->SetMargin(GRIDCELL_MARGIN_LEFT);
				}
			}
		}
	}
}

/**
 * @brief	Setup data for combo-boxes
 * @param	nComboID	- ID of combo box
 * @param	ptrLanguage - Language package pointer
 * @return	None
 */
void CPwrReminderDlg::SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage)
{
	// Check combo validity
	if (m_pMsgStyleCombo == NULL) return;

	switch (nComboID)
	{
	case IDC_PWRREMINDER_MSGSTYLE_COMBO:
		m_pMsgStyleCombo->ResetContent();
		m_pMsgStyleCombo->AddString(GetLanguageString(ptrLanguage, COMBOBOX_MSGSTYLE_MESSAGEBOX));		// Message Box
		m_pMsgStyleCombo->AddString(GetLanguageString(ptrLanguage, COMBOBOX_MSGSTYLE_DIALOGBOX));		// Dialog Box
		break;

	default:
		break;
	}

	// Default
	SDialog::SetupComboBox(nComboID, ptrLanguage);
}

/**
 * @brief	Switch between modes and re-update dialog item states
 * @param	bRedraw - Redraw dialog items or not
 * @return	None
 */
void CPwrReminderDlg::SwitchMode(BOOL /* bRedraw = FALSE */)
{
	int nCurMode = GetCurMode();
	if (nCurMode == Mode::Init) {
		// Unlock dialog items
		SetLockState(FALSE);

		// Restore [Add/Edit] buttons caption
		UpdateItemText(IDC_PWRREMINDER_ADD_BTN);
		UpdateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Enable table
		DisableTable(FALSE);

		// Refresh detail view
		RefreshDetailView(Mode::Init);

		// Refresh dialog item states
		RefreshDialogItemState(TRUE);
	}
	else if (nCurMode == Mode::View) {
		// Lock dialog items
		SetLockState(FALSE);

		// Restore [Add/Edit] buttons caption
		UpdateItemText(IDC_PWRREMINDER_ADD_BTN);
		UpdateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Enable table
		DisableTable(FALSE);

		// Refresh detail view
		RefreshDetailView(Mode::View);
		DisplayItemDetails(m_nCurSelIndex);

		// Refresh dialog item states
		RefreshDialogItemState(TRUE);
	}
	else if (nCurMode == Mode::Add) {
		// Lock dialog items
		SetLockState(TRUE);

		// Enable/disable controls
		EnableItem(IDC_PWRREMINDER_ADD_BTN,			TRUE);
		EnableItem(IDC_PWRREMINDER_EDIT_BTN,		FALSE);
		EnableItem(IDC_PWRREMINDER_REMOVE_BTN,		FALSE);
		EnableItem(IDC_PWRREMINDER_REMOVEALL_BTN,	FALSE);
		EnableItem(IDC_PWRREMINDER_CHECKALL_BTN,	FALSE);
		EnableItem(IDC_PWRREMINDER_UNCHECKALL_BTN,	FALSE);
		EnableItem(IDC_PWRREMINDER_PREVIEW_BTN,		FALSE);
		EnableItem(IDC_PWRREMINDER_APPLY_BTN,		FALSE);

		// Change [Add] button title to [Save]
		UpdateItemText(IDC_PWRREMINDER_ADD_BTN, BTN_PWRRMDDLG_SAVECHANGES);

		// Disable table
		DisableTable(TRUE);

		// Refresh detail view
		RefreshDetailView(Mode::Add);
		DisplayItemDetails(INT_INVALID);
	}
	else if (nCurMode == Mode::Update) {
		// Lock dialog items
		SetLockState(TRUE);

		// Enable/disable controls
		EnableItem(IDC_PWRREMINDER_ADD_BTN,			FALSE);
		EnableItem(IDC_PWRREMINDER_EDIT_BTN,		TRUE);
		EnableItem(IDC_PWRREMINDER_REMOVE_BTN,		FALSE);
		EnableItem(IDC_PWRREMINDER_REMOVEALL_BTN,	FALSE);
		EnableItem(IDC_PWRREMINDER_CHECKALL_BTN,	FALSE);
		EnableItem(IDC_PWRREMINDER_UNCHECKALL_BTN,	FALSE);
		EnableItem(IDC_PWRREMINDER_PREVIEW_BTN,		FALSE);
		EnableItem(IDC_PWRREMINDER_APPLY_BTN,		FALSE);

		// Change [Edit] button title to [Save]
		UpdateItemText(IDC_PWRREMINDER_EDIT_BTN, BTN_PWRRMDDLG_SAVECHANGES);

		// Disable table
		DisableTable(TRUE);

		// Refresh detail view
		RefreshDetailView(Mode::Update);
	}
	else if (nCurMode == Mode::Disable) {
		// Lock dialog items
		SetLockState(TRUE);

		// Restore [Add/Edit] buttons caption
		UpdateItemText(IDC_PWRREMINDER_ADD_BTN);
		UpdateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Disable table
		DisableTable(TRUE);

		// Refresh dialog item states
		RefreshDialogItemState(TRUE);
	}
}

/**
 * @brief	Update layout info data
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::UpdateLayoutInfo(void)
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Get table column count
	int nColNum = m_pDataItemListTable->GetColumnCount();

	// Update size of table columns
	for (int nIndex = 0; nIndex < nColNum; nIndex++) {
		int nColSize = m_pDataItemListTable->GetColumnWidth(nIndex);
		m_apGrdColFormat[nIndex].nWidth = nColSize;
	}
}

/**
 * @brief	Load layout info data
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::LoadLayoutInfo(void)
{
	// Define default table columns format
	const GRIDCTRLCOLFORMAT arrGrdColFormat[] = {
	//-----------ID--------------------------Header title ID-------------Width(px)---Column style--------Align Center---
		{	ColumnID::Index,			GRIDCOLUMN_PWRREMINDER_INDEX,		26,		COLSTYLE_FIXED,			TRUE,	},
		{	ColumnID::EnableState,		GRIDCOLUMN_PWRREMINDER_STATE,		55,		COLSTYLE_CHECKBOX,		TRUE,	},
		{	ColumnID::ItemID,			GRIDCOLUMN_PWRREMINDER_ITEMID,		75,		COLSTYLE_NORMAL,		TRUE,	},
		{ 	ColumnID::MessageContent,	GRIDCOLUMN_PWRREMINDER_MESSAGE,		237,	COLSTYLE_NORMAL,		FALSE,	},
		{ 	ColumnID::EventID,			GRIDCOLUMN_PWRREMINDER_EVENTID,		140,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	ColumnID::MsgStyle,			GRIDCOLUMN_PWRREMINDER_STYLE,		107,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	ColumnID::Repeat,			GRIDCOLUMN_PWRREMINDER_REPEAT,		56,		COLSTYLE_CHECKBOX,		TRUE,	},
	//------------------------------------------------------------------------------------------------------------------
	};

	// Backup format data
	m_nColNum = (sizeof(arrGrdColFormat) / sizeof(GRIDCTRLCOLFORMAT));

	// Initialize table format info data
	if (m_apGrdColFormat == NULL) {
		m_apGrdColFormat = new GRIDCTRLCOLFORMAT[m_nColNum];
		for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
			// Copy default table column format data
			m_apGrdColFormat[nIndex] = arrGrdColFormat[nIndex];
		}
	}

	// Load layout info data from registry
	int nRet = 0;
	String keyName;
	for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
		keyName = Key::LayoutInfo::GridColSize(nIndex);
		if (GetLayoutInfo(Section::LayoutInfo::PwrReminderTable, keyName, nRet)) {
			if (m_apGrdColFormat != NULL) {
				m_apGrdColFormat[nIndex].nWidth = nRet;
			}
		}
	}
}

/**
 * @brief	Save layout info data
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::SaveLayoutInfo(void)
{
	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Save layout info data to registry
	int nRef = 0;
	String keyName;
	for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
		nRef = m_apGrdColFormat[nIndex].nWidth;
		keyName = Key::LayoutInfo::GridColSize(nIndex);
		WriteLayoutInfo(Section::LayoutInfo::PwrReminderTable, keyName, nRef);
	}
}

/**
 * @brief	Setup state and properties for dialog items
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::SetupDialogItemState()
{
	// Initialize dialog items
	if (m_pMsgStringEdit == NULL) {
		m_pMsgStringEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_MSGSTRING_EDITBOX);
		if (m_pMsgStringEdit == NULL) {
			TRACE_ERROR("Error: Message content edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtSetTimeRad == NULL) {
		m_pEvtSetTimeRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN);
		if (m_pEvtSetTimeRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventSetTimeRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtSetTimeSpin == NULL) {
		m_pEvtSetTimeSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_SPIN);
		if (m_pEvtSetTimeSpin == NULL) {
			TRACE_ERROR("Error: Time spin control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtRepeatSetBtn == NULL) {
		m_pEvtRepeatSetBtn = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_REPEATSET_BTN);
		if (m_pEvtRepeatSetBtn == NULL) {
			TRACE_ERROR("Error: RepeatSet button not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtAppStartupRad == NULL) {
		m_pEvtAppStartupRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN);
		if (m_pEvtAppStartupRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventAppStartupRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtSysWakeupRad == NULL) {
		m_pEvtSysWakeupRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN);
		if (m_pEvtSysWakeupRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventSysWakeupRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtBfrPwrActionRad == NULL) {
		m_pEvtBfrPwrActionRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN);
		if (m_pEvtBfrPwrActionRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventBfrPwrActionRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtPwrActionWakeRad == NULL) {
		m_pEvtPwrActionWakeRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN);
		if (m_pEvtPwrActionWakeRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventPwrActionWakeRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtAtAppExitRad == NULL) {
		m_pEvtAtAppExitRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN);
		if (m_pEvtAtAppExitRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventAtAppExitRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pStyleMsgBoxRad == NULL) {
		m_pStyleMsgBoxRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN);
		if (m_pStyleMsgBoxRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (StyleMsgBoxRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pStyleDialogBoxRad == NULL) {
		m_pStyleDialogBoxRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN);
		if (m_pStyleDialogBoxRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (StyleDlgBoxRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Setup properties
	if (m_pEvtSetTimeSpin != NULL) {
		if (m_pEvtSetTimeEdit == NULL) return;

		// Set buddy: Time edit control
		m_pEvtSetTimeSpin->SetBuddy(m_pEvtSetTimeEdit);
		m_pEvtSetTimeSpin->SetRange(Constant::Min::TimeSpin, Constant::Max::TimeSpin);
		m_pEvtSetTimeSpin->SetPos(0);
	}

	// Setup time editbox
	ClockTime clockTime;
	ClockTimeUtils::SpinPos2Time(clockTime, 0);
	UpdateTimeSetting(clockTime, FALSE);

	// Initialize counter display
	UpdateMsgCounter(0);

	// Default
	SDialog::SetupDialogItemState();
}

/**
 * @brief	Update Power Reminder data item list
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::UpdateDataItemList()
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// If there's no item, do nothing
	int nItemNum = GetItemNum();
	if (nItemNum <= 0)
		return;

	// Load app language package
	LANGTABLE_PTR ptrLanguage = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
	
	// Print items
	int nTemp = -1;
	int nRowIndex = 0;
	CGridCellCheck* pCellCheck = NULL;
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

		// Get row index
		nRowIndex = nIndex + fixedRowNum;

		// Get item
		const Item& pwrItem = m_pwrReminderDataTemp.GetItemAt(nIndex);

		// Item index
		String tempString = StringUtils::StringFormat(_T("%d"), nRowIndex);
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::Index, tempString);

		// Enable state
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::EnableState);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(pwrItem.IsEnabled());
		}

		// ItemID
		tempString.Format(_T("%d"), pwrItem.GetItemID());
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::ItemID, tempString);

		// Message content
		tempString = pwrItem.GetMessage();
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::MessageContent, tempString);

		// EventID
		nTemp = GetPairedID(IDTable::PwrReminderEvent, pwrItem.GetEventID());
		tempString = GetLanguageString(ptrLanguage, nTemp);
		if (pwrItem.GetEventID() == Event::atSetTime) {
			// Format time string
			String formatTime = tempString;
			tempString = ClockTimeUtils::Format(ptrLanguage, formatTime, pwrItem.GetTime());
		}
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::EventID, tempString);

		// Message style
		nTemp = GetPairedID(IDTable::PwrReminderStyle, pwrItem.GetMessageStyle());
		tempString = GetLanguageString(ptrLanguage, nTemp);
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::MsgStyle, tempString);

		// Repeat
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::Repeat);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(pwrItem.IsRepeatEnabled());
		}
	}
}

/**
 * @brief	Disable mouse click events for table
 * @param	bDisable - Disable/enable
 * @return	None
 */
void CPwrReminderDlg::DisableTable(BOOL bDisable)
{
	// Redraw read-only style
	RedrawDataTable(bDisable);

	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Disable/enable mouse events
	m_pDataItemListTable->DisableMouseClick(bDisable);
	m_pDataItemListTable->DisableMouseMove(bDisable);
}

/**
 * @brief	Update and redraw data table
 * @param	BOOL bReadOnly - Read-only mode
 * @return	None
 */
void CPwrReminderDlg::RedrawDataTable(BOOL bReadOnly /* = FALSE */)
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Update new row number
	int nCurRowNum = (GetItemNum() + fixedRowNum);
	m_pDataItemListTable->SetRowCount(nCurRowNum);

	// Draw table
	DrawDataTable(m_pszFrameWndSize, m_nColNum, nCurRowNum, bReadOnly);
	
	// Update table data
	UpdateDataItemList();

	// Trigger redrawing table
	m_pDataItemListTable->RedrawWindow();
}

/**
 * @brief	Display details of an item at specified index
 * @param	nIndex - Index of item to display
 * @return	None
 */
void CPwrReminderDlg::DisplayItemDetails(int nIndex)
{
	// Check index validity
	if ((nIndex < INT_INVALID) || (nIndex > GetItemNum()))
		return;

	// Get item at index
	Item pwrItem;
	if (nIndex != INT_INVALID) {
		pwrItem = m_pwrReminderDataTemp.GetItemAt(nIndex);
	}

	// Init default data for mode add
	if (GetCurMode() == Mode::Add) {
		pwrItem.SetMessage(Constant::String::Empty);
		pwrItem.SetEventID(Event::atSetTime);
		pwrItem.SetTime(ClockTimeUtils::GetCurrentClockTime());
		pwrItem.SetMessageStyle(Style::messageBox);
		pwrItem.EnableCustomStyle(FALSE);
		pwrItem.ResetRepeatInfo();
		pwrItem.ResetMessageStyleInfo();
	}

	// If item is empty
	if (pwrItem.IsEmpty()) {
		// TODO: Disable all detail item controls
		RefreshDetailView(Mode::Init);
	}
	else {
		// TODO: Update current displaying item index
		RefreshDetailView(GetCurMode());
		m_nCurDispIndex = nIndex;
	}

	// Initialize RepeatSet dialog if not available
	if (m_pRepeatSetDlg == NULL) {
		m_pRepeatSetDlg = new CRmdRepeatSetDlg;
		m_pRepeatSetDlg->Create(IDD_RMDREPEATSET_DLG);
		m_pRepeatSetDlg->RemoveStyle(DS_MODALFRAME | WS_CAPTION);
		m_pRepeatSetDlg->SetParentWnd(this);
	}

	// Display item details
	UpdateItemData(pwrItem, FALSE);
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	bRecheckState - Recheck all item's state
 * @return	None
 */
void CPwrReminderDlg::RefreshDialogItemState(BOOL bRecheckState /* = FALSE */)
{
	CWnd* pBtn = NULL;

	// If dialog items are being locked, do nothing
	if (GetLockState() == TRUE)
		return;

	// Check if any item is selected or not
	BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetItemNum()));

	// Check if number of item has reached the limit
	BOOL bIsMaxNum = (GetItemNum() >= PwrReminderData::maxItemNum);

	// Check if data is all empty or not
	BOOL bIsAllEmpty = m_pwrReminderDataTemp.IsAllEmpty();

	// Get app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Disable [Add] button if item number has reached the limit
	pBtn = GetDlgItem(IDC_PWRREMINDER_ADD_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(!bIsMaxNum);
		pBtn->SetWindowText(GetLanguageString(pAppLang, IDC_PWRREMINDER_ADD_BTN));
	}

	// Disable [Remove/Remove All] buttons if data is all empty
	pBtn = GetDlgItem(IDC_PWRREMINDER_REMOVE_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(!bIsAllEmpty);

		// Enable [Remove] button if any item is selected
		pBtn->EnableWindow(bIsSelected);
	}
	pBtn = GetDlgItem(IDC_PWRREMINDER_REMOVEALL_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(!bIsAllEmpty);
	}

	// Enable [Edit] and [Preview] button if any item is selected
	pBtn = GetDlgItem(IDC_PWRREMINDER_EDIT_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(bIsSelected);
		pBtn->SetWindowText(GetLanguageString(pAppLang, IDC_PWRREMINDER_EDIT_BTN));
	}
	pBtn = GetDlgItem(IDC_PWRREMINDER_PREVIEW_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(bIsSelected);
	}

	// Check if data is changed or not
	BOOL bIsChanged = CheckDataChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);

	// Enable [Apply] button if data is changed
	pBtn = GetDlgItem(IDC_PWRREMINDER_APPLY_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(bIsChanged);
	}

	// Update [Check/Uncheck All] button state
	UpdateCheckAllBtnState(bRecheckState);

	// Default
	SDialog::RefreshDialogItemState(bRecheckState);
}

/**
 * @brief	Refresh and update state for [Check/Uncheck All] button
 * @param	bRecheck - Recheck all items enable state
 * @return	None
 */
void CPwrReminderDlg::UpdateCheckAllBtnState(BOOL bRecheck /* = FALSE */)
{
	// If dialog items are being locked, do nothing
	if (GetLockState() == TRUE)
		return;

	// Get buttons
	CWnd* pCheckAllBtn = GetDlgItem(IDC_PWRREMINDER_CHECKALL_BTN);
	CWnd* pUncheckAllBtn = GetDlgItem(IDC_PWRREMINDER_UNCHECKALL_BTN);
	if ((pCheckAllBtn == NULL) || (pUncheckAllBtn == NULL))
		return;

	// Get number of items
	int nItemNum = GetItemNum();
	if (nItemNum == 0) {
		// Disable both [Check/Uncheeck All] buttons
		pCheckAllBtn->EnableWindow(FALSE);
		pUncheckAllBtn->EnableWindow(FALSE);
		return;
	}

	// Recheck all items state
	if (bRecheck == TRUE) {
		m_nCheckCount = 0; // Reset counter
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
			const Item& pwrTemp = m_pwrReminderDataTemp.GetItemAt(nIndex);
			if (pwrTemp.IsEnabled() == TRUE) {
				m_nCheckCount++;
			}
		}
	}

	// Update button state
	if (m_nCheckCount == 0) {
		// Enable [Check All] button
		pCheckAllBtn->EnableWindow(TRUE);
		// Disable [Uncheck All] button
		pUncheckAllBtn->EnableWindow(FALSE);
	}
	else if (m_nCheckCount == nItemNum) {
		// Disable [Check All] button
		pCheckAllBtn->EnableWindow(FALSE);
		// Enable [Uncheck All] button
		pUncheckAllBtn->EnableWindow(TRUE);
	}
	else {
		// Enable both [Check/Uncheck All] buttons
		pCheckAllBtn->EnableWindow(TRUE);
		pUncheckAllBtn->EnableWindow(TRUE);
	}
}

/**
 * @brief	Refresh and update state for detail view
 * @param	nMode - Detail view mode
 * @return	None
 */
void CPwrReminderDlg::RefreshDetailView(int nMode)
{
	// Set state and init value by mode
	BOOL bEnable = TRUE;
	if ((nMode == Mode::Init) || (nMode == Mode::View)) {
		bEnable = FALSE;
	}
	else if ((nMode == Mode::Add) || (nMode == Mode::Update)) {
		bEnable = TRUE;
	}
	else if (nMode == Mode::Disable) {
		bEnable = FALSE;
	}

	/***************************************************************/
	/*															   */
	/*				Update detail view item states				   */
	/*															   */
	/***************************************************************/
	CWnd* pWnd = NULL;

	// Message content
	pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_TITLE);
	if (pWnd != NULL) {
		pWnd->EnableWindow(bEnable);
	}
	if (m_pMsgStringEdit != NULL) {
		m_pMsgStringEdit->EnableWindow(bEnable);
	}
	pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_COUNTER);
	if (pWnd != NULL) {
		pWnd->EnableWindow(bEnable);
	}
	// Event
	pWnd = GetDlgItem(IDC_PWRREMINDER_EVENT_TITLE);
	if (pWnd != NULL) {
		pWnd->EnableWindow(bEnable);
	}
	if (m_pEvtSetTimeRad != NULL) {
		m_pEvtSetTimeRad->EnableWindow(bEnable);
		BOOL bEnableSetTime = (bEnable & m_pEvtSetTimeRad->GetCheck());
		if (m_pEvtSetTimeEdit != NULL) {
			m_pEvtSetTimeEdit->EnableWindow(bEnableSetTime);
			UpdateTimeSetting(m_stDispTimeBak, FALSE);
		}
		if (m_pEvtSetTimeSpin != NULL) {
			m_pEvtSetTimeSpin->EnableWindow(bEnableSetTime);
		}
		if (m_pEvtRepeatSetBtn != NULL) {
			m_pEvtRepeatSetBtn->EnableWindow(bEnableSetTime);
		}
	}
	if (m_pEvtAppStartupRad != NULL) {
		m_pEvtAppStartupRad->EnableWindow(bEnable);
	}
	if (m_pEvtSysWakeupRad != NULL) {
		m_pEvtSysWakeupRad->EnableWindow(bEnable);
	}
	if (m_pEvtBfrPwrActionRad != NULL) {
		m_pEvtBfrPwrActionRad->EnableWindow(bEnable);
	}
	if (m_pEvtPwrActionWakeRad != NULL) {
		m_pEvtPwrActionWakeRad->EnableWindow(bEnable);
	}
	if (m_pEvtAtAppExitRad != NULL) {
		m_pEvtAtAppExitRad->EnableWindow(bEnable);
	}
	// Message style
	pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_TITLE);
	if (pWnd != NULL) {
		pWnd->EnableWindow(bEnable);
	}
	if (m_pStyleMsgBoxRad != NULL) {
		m_pStyleMsgBoxRad->EnableWindow(bEnable);
	}
	if (m_pStyleDialogBoxRad != NULL) {
		m_pStyleDialogBoxRad->EnableWindow(bEnable);
	}
}

/**
 * @brief	Update message content length counter
 * @param	nCount - Character number counter
 * @return	None
 */
void CPwrReminderDlg::UpdateMsgCounter(int nCount)
{
	// Get dialog item
	CWnd* pCounter = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_COUNTER);
	if (pCounter == NULL) return;

	// Check counter value validity
	if ((nCount < 0) || (nCount > Constant::Max::StringLength)) return;

	// Display counter
	String countFormatString;
	countFormatString.Format(_T("%d/%d"), nCount, Constant::Max::StringLength);
	pCounter->SetWindowText(countFormatString);
}

/**
 * @brief	Update time value from/to time edit control
 * @param	clockTime  - Clock-time data
 * @param	bUpdate	   - Update or not (YES/TRUE by default)
 * @return	None
 */
void CPwrReminderDlg::UpdateTimeSetting(ClockTime& clockTime, BOOL bUpdate /* = TRUE */)
{
	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Check time editbox validity
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	if (bUpdate == TRUE) {

		// Get value from time editbox
		const int buffLength = m_pEvtSetTimeEdit->GetWindowTextLength();
		std::vector<wchar_t> tempBuff(buffLength + 1);
		m_pEvtSetTimeEdit->GetWindowText(tempBuff.data(), buffLength + 1);
		String timeTextValue = tempBuff.data();

		// Get hour value
		int hour = _wtoi(timeTextValue.Left(2));
		const String timePeriod = timeTextValue.Right(2);
		if (timePeriod == GetLanguageString(pLang, FORMAT_TIMEPERIOD_ANTE_MERIDIEM)) {
			// Before midday
			clockTime.SetHour(hour);
		}
		else if ((timePeriod == GetLanguageString(pLang, FORMAT_TIMEPERIOD_POST_MERIDIEM)) && hour < 12) {
			// After midday
			clockTime.SetHour(hour + 12);
		}
		else {
			// Keep value
			clockTime.SetHour(hour);
		}

		// Get minute value
		clockTime.SetMinute(_wtoi(timeTextValue.Mid(3, 2)));
	}
	else {
		// Set value for time editbox
		String timeFormatString;
		timeFormatString = ClockTimeUtils::Format(pLang, IDS_FORMAT_SHORTTIME, clockTime);
		m_pEvtSetTimeEdit->SetWindowText(timeFormatString);

		// Backup current displaying time value
		m_stDispTimeBak = clockTime;

		// Update time spin position
		if (m_pEvtSetTimeSpin != NULL) {
			int nSpinPos = 0;
			ClockTimeUtils::Time2SpinPos(clockTime, nSpinPos);
			m_pEvtSetTimeSpin->SetPos(nSpinPos);
		}
	}
}

/**
 * @brief	Load Power Reminder data
 * @param	None
 * @return	BOOL - Result of loading process
 */
BOOL CPwrReminderDlg::LoadPwrReminderData()
{
	// Get app Reminder data pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp == NULL) return FALSE;
	Data* ppwrData = pApp->GetAppPwrReminderData();
	if (ppwrData == NULL)
		return FALSE;

	// Copy data
	m_pwrReminderData.Copy(*ppwrData);
	m_pwrReminderDataTemp.Copy(m_pwrReminderData);

	// Reset change flag
	SetFlagValue(AppFlagID::dialogDataChanged, FALSE);

	// Validate data and auto-correction
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		Item& pwrItem = m_pwrReminderDataTemp.GetItemAt(nIndex);
		if (!Validate(pwrItem, TRUE, TRUE)) {
			// Update temp data
			m_pwrReminderDataTemp.Update(pwrItem);
			SetFlagValue(AppFlagID::dialogDataChanged, TRUE);	// Update change flag
		}
	}

	return TRUE;
}

/**
 * @brief	Save Power Reminder data
 * @param	None
 * @return	BOOL - Result of saving process
 */
BOOL CPwrReminderDlg::SavePwrReminderData()
{
	// Copy data and adjust validity
	m_pwrReminderData.Copy(m_pwrReminderDataTemp);
	m_pwrReminderData.Adjust();

	// Save app Power Reminder data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp == NULL) return FALSE;
	pApp->SetAppPwrReminderData(&m_pwrReminderData);
	pApp->SaveRegistryAppData(APPDATA_PWRREMINDER);

	// Notify main dialog to re-update Power Reminder data
	CPowerPlusDlg* pMainDlg = (CPowerPlusDlg*)(pApp->GetMainWnd());
	VERIFY(pMainDlg != NULL);
	if (pMainDlg == NULL) return FALSE;
	pMainDlg->PostMessage(SM_APP_UPDATE_PWRREMINDERDATA, NULL, NULL);

	// Reset change flag
	SetFlagValue(AppFlagID::dialogDataChanged, FALSE);
	
	return TRUE;
}

/**
 * @brief	Check if settings changed
 * @param	None
 * @return	None
 */
BOOL CPwrReminderDlg::CheckDataChangeState()
{
	BOOL bChangeFlag = FALSE;

	// Update enable and repeat states of each item
	int nRowIndex = 0;
	CGridCellCheck* pCellCheckEnable = NULL;
	CGridCellCheck* pCellCheckRepeat = NULL;
	int nItemRowNum = (m_pDataItemListTable->GetRowCount() - fixedRowNum);
	for (int nIndex = 0; nIndex < nItemRowNum; nIndex++) {
		// Get row index
		nRowIndex = (nIndex + fixedRowNum);

		// Get checkbox cells
		pCellCheckEnable = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::EnableState);
		pCellCheckRepeat = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::Repeat);
		if ((pCellCheckEnable == NULL) || (pCellCheckRepeat == NULL)) continue;
		
		// Get checked states
		BOOL bEnabled = pCellCheckEnable->GetCheck();
		BOOL bRepeat = pCellCheckRepeat->GetCheck();
		
		// Update item enable and repeat states
		Item& pwrTempItem = m_pwrReminderDataTemp.GetItemAt(nIndex);
		pwrTempItem.EnableItem(bEnabled);
		pwrTempItem.EnableRepeat(bRepeat);
	}

	// Check if number of items changed
	int nItemNum = GetItemNum();
	bChangeFlag |= (nItemNum != m_pwrReminderData.GetItemNum());
	if (bChangeFlag == TRUE)
		return bChangeFlag;

	// Check if each item's data changed
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		// Get current item and temp item
		const Item& pwrCurItem = m_pwrReminderData.GetItemAt(nIndex);
		const Item& pwrTempItem = m_pwrReminderDataTemp.GetItemAt(nIndex);
 
		// Data comparison
		bChangeFlag |= (pwrTempItem.IsEnabled() != pwrCurItem.IsEnabled());
		bChangeFlag |= (pwrTempItem.GetItemID() != pwrCurItem.GetItemID());
		bChangeFlag |= (pwrTempItem.Compare(pwrCurItem) != TRUE);

		// Stop on the first different item encountered
		if (bChangeFlag == TRUE) break;
	}
	
	return bChangeFlag;
}

/**
 * @brief	Add current settings to reminder data list
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::Add()
{
	// Update data
	UpdateData(TRUE);

	// Create temp Reminder item
	Item pwrTemp;
	pwrTemp.SetItemID(m_pwrReminderDataTemp.GetNextID());

	// Update data
	UpdateItemData(pwrTemp, TRUE);

	// Check data validity
	BOOL bValid = Validate(pwrTemp, TRUE);
	if (bValid == FALSE)
		return;

	// Update item to Power Reminder data item list
	m_pwrReminderDataTemp.Update(pwrTemp);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(TRUE);
}

/**
 * @brief	Edit a reminder item by index
 * @param	nIndex - Index of item to edit
 * @return	None
 */
void CPwrReminderDlg::Edit(int nIndex)
{
	// Update data
	UpdateData(TRUE);

	// Check index validity
	if ((nIndex < 0) || (nIndex > GetItemNum()))
		return;

	// Get item at given index
	Item pwrTemp = m_pwrReminderDataTemp.GetItemAt(nIndex);

	// Update data
	UpdateItemData(pwrTemp, TRUE);

	// Check data validity
	BOOL bValid = Validate(pwrTemp, TRUE);
	if (bValid == FALSE)
		return;

	// Update item to Power Reminder data item list
	m_pwrReminderDataTemp.Update(pwrTemp);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(TRUE);
}

/**
 * @brief	Remove a reminder item by index
 * @param	nIndex - Index of item to remove
 * @return	None
 */
void CPwrReminderDlg::Remove(int nIndex)
{
	// Remove item at index
	m_pwrReminderDataTemp.Delete(nIndex);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(TRUE);
}

/**
 * @brief	Remove all reminder items
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::RemoveAll()
{
	// Remove all items
	m_pwrReminderDataTemp.DeleteAll();

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(TRUE);
}

/**
 * @brief	Check/uncheck all Power Reminder items
 * @param	bState - Item state
 * @return	None
 */
void CPwrReminderDlg::SetAllItemState(BOOL bState)
{
	// Check/uncheck all --> Update all items enable state
	int nItemNum = GetItemNum();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		Item& pwrTemp = m_pwrReminderDataTemp.GetItemAt(nIndex);
		if (pwrTemp.IsEnabled() != bState) {
			pwrTemp.EnableItem(bState);
		}
	}

	// Update number of checked items
	m_nCheckCount = (bState == FALSE) ? 0 : nItemNum;
	
	// Update data item list
	UpdateDataItemList();

	// Refresh button state
	RefreshDialogItemState(FALSE);
}

/**
 * @brief	Preview Power Reminder item
 * @param	nIndex - Item index
 * @return	None
 */
void CPwrReminderDlg::PreviewItem(int nIndex)
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= GetItemNum()))
		return;

	// Get item
	const Item& pwrDispItem = m_pwrReminderDataTemp.GetItemAt(nIndex);

	// Check message content validity
	String messageContent = pwrDispItem.GetMessage();
	if ((messageContent.IsEmpty()) ||
		(IS_NULL_STRING(messageContent))) {
		// Invalid message content
		return;
	}

	// Get app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Style: MessageBox
	if (pwrDispItem.GetMessageStyle() == Style::messageBox) {
		// Display message box
		const wchar_t* messageCaption = GetLanguageString(pAppLang, IDC_PWRREMINDER_PREVIEW_BTN);
		DWORD dwMsgStyle = MB_OK | MB_ICONINFORMATION;
		DisplayMessageBox(messageContent, messageCaption, dwMsgStyle);
	}
	// Style: Dialog
	else if (pwrDispItem.GetMessageStyle() == Style::dialogBox) {
		// Destroy preview reminder message dialog if is opening
		if (m_pRmdPreviewMsgDlg != NULL) {
			// Destroy dialog
			if (::IsWindow(m_pRmdPreviewMsgDlg->GetSafeHwnd())) {
				m_pRmdPreviewMsgDlg->DestroyWindow();
			}
			delete m_pRmdPreviewMsgDlg;
			m_pRmdPreviewMsgDlg = NULL;
		}

		// Init reminder message dialog
		if (m_pRmdPreviewMsgDlg == NULL) {
			m_pRmdPreviewMsgDlg = new CReminderMsgDlg();
			if (m_pRmdPreviewMsgDlg == NULL) return;

			// Message style
			RmdMsgStyleSet rmdMessageStyle = m_pwrReminderDataTemp.GetCommonStyle();
			if (pwrDispItem.IsCustomStyleEnabled())
				rmdMessageStyle = pwrDispItem.GetMessageStyleData();

			// Default timeout for previewing
			int nDefTimeout = PwrReminderData::previewTimeout;

			// Set properties
			m_pRmdPreviewMsgDlg->SetCaptionFromLanguage(IDC_PWRREMINDER_PREVIEW_BTN);
			m_pRmdPreviewMsgDlg->SetDispMessage(messageContent);
			m_pRmdPreviewMsgDlg->SetMessageStyle(rmdMessageStyle);
			m_pRmdPreviewMsgDlg->SetAutoCloseInterval(nDefTimeout);

			// Set notify state flags
			m_pRmdPreviewMsgDlg->SetTopMost(FALSE);
			m_pRmdPreviewMsgDlg->SetInitSound(TRUE);

			// Display message
			m_pRmdPreviewMsgDlg->DoModal();
			
			delete m_pRmdPreviewMsgDlg;
			m_pRmdPreviewMsgDlg = NULL;
		}
	}
}

/**
 * @brief	Update reminder data from/to dialog controls
 * @param	pwrItem - Power Reminder item
 * @param	bUpdate - Update data flag
 * @return	None
 */
void CPwrReminderDlg::UpdateItemData(Item& pwrItem, BOOL bUpdate)
{
	if (bUpdate == TRUE) {

		/***************************************************************/
		/*															   */
		/*				Update data from dialog controls			   */
		/*															   */
		/***************************************************************/
		/*-----------------------Message content-----------------------*/
		String tempString = Constant::String::Empty;
		if (m_pMsgStringEdit != NULL) {
			const int buffLength = m_pMsgStringEdit->GetWindowTextLength();
			std::vector<wchar_t> tempBuff(buffLength + 1);
			m_pMsgStringEdit->GetWindowText(tempBuff.data(), buffLength + 1);
			tempString = tempBuff.data();
			pwrItem.SetMessage(tempString);
		}

		/*----------------------------Event----------------------------*/
		BOOL bTemp = FALSE;

		// Event: At set time
		if (m_pEvtSetTimeRad != NULL) {
			bTemp = m_pEvtSetTimeRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.SetEventID(Event::atSetTime);
				if (m_pEvtSetTimeEdit != NULL) {
					ClockTime clockTimeTemp;
					UpdateTimeSetting(clockTimeTemp, TRUE);
					pwrItem.SetTime(clockTimeTemp);
				}
			}
		}
		// Update data for RepeatSet dialog
		if (m_pRepeatSetDlg != NULL) {
			m_pRepeatSetDlg->UpdateDialogData(pwrItem, TRUE);
		}
		// Event: At app startup
		if (m_pEvtAppStartupRad != NULL) {
			bTemp = m_pEvtAppStartupRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.SetEventID(Event::atAppStartup);
			}
		}
		// Event: At system wake
		if (m_pEvtSysWakeupRad != NULL) {
			bTemp = m_pEvtSysWakeupRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.SetEventID(Event::atSysWakeUp);
			}
		}
		// Event: Before power action
		if (m_pEvtBfrPwrActionRad != NULL) {
			bTemp = m_pEvtBfrPwrActionRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.SetEventID(Event::beforePwrAction);
			}
		}
		// Event: Wake after action
		if (m_pEvtPwrActionWakeRad != NULL) {
			bTemp = m_pEvtPwrActionWakeRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.SetEventID(Event::wakeAfterAction);
			}
		}
		// Event: Before app exit
		if (m_pEvtAtAppExitRad != NULL) {
			bTemp = m_pEvtAtAppExitRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.SetEventID(Event::atAppExit);
			}
		}

		/*------------------------Message style------------------------*/
		// Style: MessageBox
		if (m_pStyleMsgBoxRad != NULL) {
			bTemp = m_pStyleMsgBoxRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.SetMessageStyle(Style::messageBox);
			}
		}
		// Style: Dialog Box
		if (m_pStyleDialogBoxRad != NULL) {
			bTemp = m_pStyleDialogBoxRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.SetMessageStyle(Style::dialogBox);
			}
		}

		/*-------------------------------------------------------------*/
	}
	else {

		/***************************************************************/
		/*															   */
		/*				  Bind data to dialog controls				   */
		/*															   */
		/***************************************************************/
		/*-----------------Set state and init value by mode------------*/
		BOOL bEnable = TRUE;
		int nMode = GetCurMode();
		if ((nMode == Mode::Init) || (nMode == Mode::View)) {
			// Disable items
			bEnable = FALSE;
		}
		else if ((nMode == Mode::Add) || (nMode == Mode::Update)) {
			// Enable items
			bEnable = TRUE;
		}
		else if (nMode == Mode::Disable) {
			// Disable items
			bEnable = FALSE;
		}

		/*----------------------Get item details-----------------------*/
		String messageContent = pwrItem.GetMessage();
		ClockTime itemTime = pwrItem.GetTime();
		UINT nEventID = pwrItem.GetEventID();
		DWORD dwMsgStyle = pwrItem.GetMessageStyle();

		/*-----------------------Message content-----------------------*/
		CWnd* pWnd = NULL;
		pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_TITLE);
		if (pWnd != NULL) {
			pWnd->EnableWindow(bEnable);
		}
		if (m_pMsgStringEdit != NULL) {
			m_pMsgStringEdit->EnableWindow(bEnable);
			m_pMsgStringEdit->SetWindowText(messageContent);
		}
		pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_COUNTER);
		if (pWnd != NULL) {
			pWnd->EnableWindow(bEnable);
			UpdateMsgCounter(messageContent.GetLength());
		}

		/*----------------------------Event----------------------------*/
		BOOL bTemp = FALSE;
		pWnd = GetDlgItem(IDC_PWRREMINDER_EVENT_TITLE);
		if (pWnd != NULL) {
			pWnd->EnableWindow(bEnable);
		}

		// Event: At set time
		if (m_pEvtSetTimeRad != NULL) {
			m_pEvtSetTimeRad->EnableWindow(bEnable);
			bTemp = (nEventID == Event::atSetTime);
			m_pEvtSetTimeRad->SetCheck(bTemp);

			// Set time edit and spin value
			bTemp &= bEnable;
			if (m_pEvtSetTimeEdit != NULL) {
				m_pEvtSetTimeEdit->EnableWindow(bTemp);
				UpdateTimeSetting(itemTime, FALSE);
			}
			if (m_pEvtSetTimeSpin != NULL) {
				m_pEvtSetTimeSpin->EnableWindow(bTemp);
			}
		}
		// Update data for RepeatSet dialog
		if (m_pRepeatSetDlg != NULL) {
			m_pRepeatSetDlg->UpdateDialogData(pwrItem, FALSE);
		}
		// Event: At app startup
		if (m_pEvtAppStartupRad != NULL) {
			m_pEvtAppStartupRad->EnableWindow(bEnable);
			bTemp = (nEventID == Event::atAppStartup);
			m_pEvtAppStartupRad->SetCheck(bTemp);
		}
		// Event: At system wake
		if (m_pEvtSysWakeupRad != NULL) {
			m_pEvtSysWakeupRad->EnableWindow(bEnable);
			bTemp = (nEventID == Event::atSysWakeUp);
			m_pEvtSysWakeupRad->SetCheck(bTemp);
		}
		// Event: Before power action
		if (m_pEvtBfrPwrActionRad != NULL) {
			m_pEvtBfrPwrActionRad->EnableWindow(bEnable);
			bTemp = (nEventID == Event::beforePwrAction);
			m_pEvtBfrPwrActionRad->SetCheck(bTemp);
		}
		// Event: Wake after action
		if (m_pEvtPwrActionWakeRad != NULL) {
			m_pEvtPwrActionWakeRad->EnableWindow(bEnable);
			bTemp = (nEventID == Event::wakeAfterAction);
			m_pEvtPwrActionWakeRad->SetCheck(bTemp);
		}
		// Event: Before app exit
		if (m_pEvtAtAppExitRad != NULL) {
			m_pEvtAtAppExitRad->EnableWindow(bEnable);
			bTemp = (nEventID == Event::atAppExit);
			m_pEvtAtAppExitRad->SetCheck(bTemp);
		}

		/*------------------------Message style------------------------*/
		pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_TITLE);
		if (pWnd != NULL) {
			pWnd->EnableWindow(bEnable);
		}
		if (m_pStyleMsgBoxRad != NULL) {
			m_pStyleMsgBoxRad->EnableWindow(bEnable);
			bTemp = (dwMsgStyle == Style::messageBox);
			m_pStyleMsgBoxRad->SetCheck(bTemp);
		}
		if (m_pStyleDialogBoxRad != NULL) {
			m_pStyleDialogBoxRad->EnableWindow(bEnable);
			bTemp = (dwMsgStyle == Style::dialogBox);
			m_pStyleDialogBoxRad->SetCheck(bTemp);
		}

		/*-------------------------------------------------------------*/
	}
}

/**
 * @brief	Check Power Reminder item validity
 * @param	pwrItem		 - Item to validate
 * @param	bShowMsg	 - Show validation message box or not
 * @param	bAutoCorrect - Invalid value auto correction (ON/OFF)
 * @return	BOOL - Result of validation process
 */
BOOL CPwrReminderDlg::Validate(Item& pwrItem, BOOL bShowMsg /* = FALSE */, BOOL bAutoCorrect /* = FALSE */)
{
	BOOL bResult = TRUE;

	int nMsgStringID;
	StringArray arrMsgString;
	arrMsgString.clear();

	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Check item ID
	if ((pwrItem.GetItemID() < PwrReminderData::minItemID) || (pwrItem.GetItemID() > PwrReminderData::maxItemID)) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_ITEMID;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Get next item ID
			pwrItem.SetItemID(m_pwrReminderDataTemp.GetNextID());
		}
	}

	// Check message content
	String messageContent = pwrItem.GetMessage();
	if (messageContent.IsEmpty()) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_EMPTY;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			pwrItem.SetMessage(GetLanguageString(pLang, PWRRMD_MSGCONTENT_SAMPLE));
			if (IS_NOT_NULL_STRING(pwrItem.GetMessage())) {
				// Re-format sample message
				String messageFormat = pwrItem.GetMessage();
				messageContent.Format(messageFormat, pwrItem.GetItemID());
				pwrItem.SetMessage(messageContent);
			}
		}
	}
	else if (messageContent.GetLength() > Constant::Max::StringLength) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_OUTOFLIMIT;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Only get character numbers in range
			String tempString = messageContent.Left(Constant::Max::StringLength);
			pwrItem.SetMessage(tempString);
		}
	}

	// Check event ID
	if ((pwrItem.GetEventID() < Event::atSetTime) || (pwrItem.GetEventID() > Event::atAppExit)) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_EVENTID;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default event ID
			pwrItem.SetEventID(Event::atSetTime);
		}
	}

	// Check snooze interval data
	if ((pwrItem.GetSnoozeInterval() < PwrRepeatSet::minSnoozeInterval) || (pwrItem.GetSnoozeInterval() > PwrRepeatSet::maxSnoozeInterval)) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_SNOOZEINTERVAL;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default snooze interval
			pwrItem.SetSnoozeInterval(PwrRepeatSet::defaultSnoozeInterval);
		}
	}

	// Check repeat set data
	if ((pwrItem.IsRepeatEnabled() == TRUE) && (pwrItem.GetActiveDays() == NULL)) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_ACTIVEDAYS;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default data
			pwrItem.SetActiveDays(PwrRepeatSet::defaultActiveDays);
		}
	}

	// Check style ID
	if ((pwrItem.GetMessageStyle() < Style::messageBox) || (pwrItem.GetMessageStyle() > Style::dialogBox)) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_STYLEID;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default style ID
			pwrItem.SetMessageStyle(Style::messageBox);
		}
	}
	
	// Show error message if enabled
	if ((bShowMsg == TRUE) && (!arrMsgString.empty())) {
		for (int nIndex = 0; nIndex < arrMsgString.size(); nIndex++) {
			// If auto correction is ON
			if (bAutoCorrect == TRUE) {
				// Add "Data will be automatically reset to default"
				String errorMessage = arrMsgString.at(nIndex);
				errorMessage += GetLanguageString(pLang, MSGBOX_PWRREMINDER_INVALIDITEM_AUTOCORRECT);
				DisplayMessageBox(errorMessage, NULL, MB_OK | MB_ICONERROR);
			}
			else {
				// Display error message
				DisplayMessageBox(arrMsgString.at(nIndex), NULL, MB_OK | MB_ICONERROR);
			}
		}
	}

	// Remove all message after displaying
	arrMsgString.clear();

	return bResult;
}

/**
 * @brief	Set current mode
 * @param	nMode - Mode to set
 * @return	None
 */
void CPwrReminderDlg::SetCurMode(int nMode)
{
	// If new mode is the same, do nothing
	if (nMode == GetCurMode()) return;

	// Set new mode
	m_nCurMode = nMode;
	
	// Switch mode
	SwitchMode(TRUE);
}

/**
 * @brief	Draw icon for repeat set button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::DrawRepeatSetButton(void)
{
	// Check button validity
	if (m_pEvtRepeatSetBtn == NULL) {
		m_pEvtRepeatSetBtn = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_REPEATSET_BTN);
		if (m_pEvtRepeatSetBtn == NULL) {
			TRACE_ERROR("Error: RepeatSet button not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Draw button icon
	if (m_pEvtRepeatSetBtn != NULL) {

		// Load icon
		int cx = BUTTON_ICON_WIDTH;
		int cy = BUTTON_ICON_HEIGHT;
		HINSTANCE hInstance = AfxGetApp()->m_hInstance;
		HICON hRepeatIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON_REPEAT), IMAGE_ICON, cx, cy, LR_DEFAULTCOLOR);
		if (hRepeatIcon == NULL)
			return;

		// Set button icon
		m_pEvtRepeatSetBtn->ModifyStyle(NULL, BS_ICON);
		m_pEvtRepeatSetBtn->SetIcon(hRepeatIcon);
	}
}
