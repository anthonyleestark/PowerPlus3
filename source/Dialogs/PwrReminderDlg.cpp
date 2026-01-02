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
#include "Dialogs/MsgStyleSetDlg.h"

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


// Data flag for Customize dialog
using MsgStyleSetFlag = CRmdMsgStyleSetDlg::DataSetFlag;


// Implement methods for CPwrReminderDlg
IMPLEMENT_DYNAMIC(CPwrReminderDlg, SDialog)


/**
 * @brief	Constructor
 */
CPwrReminderDlg::CPwrReminderDlg(CWnd* parentWnd /*=nullptr*/)
	: SDialog(IDD_PWRREMINDER_DLG, parentWnd)
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
	m_pStyleUseCommonRad = NULL;
	m_pStyleUseCustomRad = NULL;
	m_pStyleCustomizeBtn = NULL;

	// Properties child dialogs
	m_pRmdPreviewMsgDlg = NULL;
	m_pRepeatSetDlg = NULL;
	m_pMsgStyleSetDlg = NULL;

	// Checkbox/radio button variables
	m_bEvtSetTimeRad = false;
	m_bEvtAppStartupRad = false;
	m_bEvtSysWakeupRad = false;
	m_bEvtBfrPwrActionRad = false;
	m_bEvtPwrActionWakeRad = false;
	m_bEvtAppExitRad = false;
	m_bStyleMsgBoxRad = false;
	m_bStyleDialogRad = false;
	m_bStyleUseCommonRad = false;
	m_bStyleUseCustomRad = false;

	// Table format and properties
	columnCount_ = 0;
	m_pszFrameWndSize = NULL;
	gridCtrlFormatInfoPtr_ = NULL;

	// Other variables
	m_nCurMode = 0;
	checkCount_ = 0;
	curSelIndex_ = -1;
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
	if (m_pMsgStyleSetDlg != NULL) {
		// Destroy dialog
		m_pMsgStyleSetDlg->DestroyWindow();
		delete m_pMsgStyleSetDlg;
		m_pMsgStyleSetDlg = NULL;
	}

	// Data item list control
	if (m_pDataItemListTable) {
		delete m_pDataItemListTable;
		m_pDataItemListTable = NULL;
	}
	
	// Remove Reminder data
	m_pwrReminderData.deleteAll();
	m_pwrReminderDataTemp.deleteAll();

	// Table format and properties
	if (m_pszFrameWndSize != NULL) {
		delete m_pszFrameWndSize;
		m_pszFrameWndSize = NULL;
	}

	if (gridCtrlFormatInfoPtr_ != NULL) {
		delete[] gridCtrlFormatInfoPtr_;
		gridCtrlFormatInfoPtr_ = NULL;
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
	DDX_Check(pDX, IDC_PWRREMINDER_MSGSTYLE_USECOMMON_RADBTN,	m_bStyleUseCommonRad);
	DDX_Check(pDX, IDC_PWRREMINDER_MSGSTYLE_USECUSTOM_RADBTN,	m_bStyleUseCustomRad);
}

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CPwrReminderDlg::registerDialogManagement(void)
{
	size_t returnValue = SDialog::registerDialogManagement();
	if (returnValue != 0) {
		TRACE_ERROR("Error: Register dialog management failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return returnValue;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_ITEM_LISTBOX, List_Control);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_ADD_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EDIT_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_REMOVE_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_REMOVEALL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_CHECKALL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_UNCHECKALL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_PREVIEW_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_APPLY_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_CANCEL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_DETAIL_STATIC, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTRING_TITLE, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTRING_EDITBOX, Edit_Control);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTRING_COUNTER, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_TITLE, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN, Radio_Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX, Edit_Control);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_SETTIME_SPIN, Spin_Control);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_REPEATSET_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN, Radio_Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN, Radio_Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN, Radio_Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN, Radio_Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN, Radio_Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTYLE_TITLE, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN, Radio_Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN, Radio_Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZATION_TITLE, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTYLE_USECOMMON_RADBTN, Radio_Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTYLE_USECUSTOM_RADBTN, Radio_Button);
		returnValue = pCtrlMan->AddControl(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZE_BTN, Button);
	}

	return returnValue;
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CPwrReminderDlg::unregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

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
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZATION_TITLE);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_MSGSTYLE_USECOMMON_RADBTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_MSGSTYLE_USECUSTOM_RADBTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZE_BTN);
	}

	return SDialog::unregisterDialogManagement();
}


// CPwrReminderDlg dialog items ID map
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
	ON_ID_CONTROL(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZATION_TITLE,	"StyleConfigurationGroup")
	ON_ID_CONTROL(IDC_PWRREMINDER_MSGSTYLE_USECOMMON_RADBTN,	"StyleUseCommonRadio")
	ON_ID_CONTROL(IDC_PWRREMINDER_MSGSTYLE_USECUSTOM_RADBTN,	"StyleUseCustomRad")
	ON_ID_CONTROL(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZE_BTN,		"StyleCustomizeButton")
END_RESOURCEID_MAP()


// CPwrReminderDlg dialog message map
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
	ON_BN_CLICKED(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZE_BTN,		&CPwrReminderDlg::OnCustomizeStyle)
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
	setUseEnter(false);

	// Register message box caption
	registerMessageBoxCaption(MSGBOX_PWRREMINDER_CAPTION);

	// Load data
	LoadPwrReminderData();

	// Init dialog items
	loadLayoutInfo();
	setupLanguage();
	setupDialogItemState();

	// Update data
	UpdateDataItemList();
	DisplayItemDetails(INT_INVALID);
	refreshDialogItemState(true);

	// Save dialog event log if enabled
	outputEventLog(LOG_EVENT_DLG_INIT, this->getCaption());

	// Read-only mode (if enabled)
	if (isReadOnlyMode() == true) {
		CWnd* childWndPtr = GetTopWindow();
		while (childWndPtr != NULL) {
			childWndPtr->EnableWindow(false);
			childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT);
		}
	}

	return true;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnClose()
{
	// If not forced closing by request
	if (!isForceClosingByRequest()) {

		// Exit current mode
		int confirm = -1;
		int nCurMode = GetCurMode();
		if ((nCurMode == Mode::Add) || (nCurMode == Mode::Update)) {
			// Show switch mode confirmation message
			confirm = displayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, NULL, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {
				// Switch mode
				SetCurMode(Mode::View);
			}
			return;
		}

		// Ask for saving before exiting if data changed
		bool isChanged = checkDataChangeState();
		setFlagValue(AppFlagID::dialogDataChanged, isChanged);
		if (isChanged == true) {
			// Show save confirmation message
			confirm = displayMessageBox(MSGBOX_PWRREMINDER_CHANGED_CONTENT, NULL, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {
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
	outputEventLog(LOG_EVENT_DLG_DESTROYED, this->getCaption());

	// Save layout info data
	updateLayoutInfo();
	saveLayoutInfo();

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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_APPLY_BTN);

	// Save data if changed
	bool isChanged = checkDataChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);
	if (isChanged == true) {
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
	if (!isForceClosingByRequest()) {

		// Save app event log if enabled
		outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_CANCEL_BTN);

		// Exit current mode
		int confirm = -1;
		int nCurMode = GetCurMode();
		if ((nCurMode & Mode::Add) || (nCurMode & Mode::Update)) {
			// Show switch mode confirmation message
			confirm = displayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, NULL, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {
				// Switch mode
				SetCurMode(Mode::View);
			}
			return;
		}

		// Ask for saving before exiting if data changed
		bool isChanged = checkDataChangeState();
		setFlagValue(AppFlagID::dialogDataChanged, isChanged);
		if (isChanged == true) {
			// Show save confirmation message
			confirm = displayMessageBox(MSGBOX_PWRREMINDER_CHANGED_CONTENT, NULL, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {
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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_ADD_BTN);

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
		// Mark an empty item as in editting
		m_pwrItemInEdit = PwrReminderItem();

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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_EDIT_BTN);

	// Get current mode
	int nCurMode = GetCurMode();

	// Mode: Edit
	if (nCurMode & Mode::Update) {
		// Check if any item is selected or not
		bool isSelected = ((curSelIndex_ >= 0) && (curSelIndex_ < GetItemNum()));

		if (isSelected == true) {
			// Edit current selected item
			Edit(curSelIndex_);
		}

		// Reset mode
		SetCurMode(Mode::View);
	}
	// Mode: Add
	else if (nCurMode & Mode::Add) {
		// Do nothing
	}
	else {
		// Mark current selected item as in editting
		bool isSelected = ((curSelIndex_ >= 0) && (curSelIndex_ < GetItemNum()));
		if (isSelected == true)
			m_pwrItemInEdit.copy(m_pwrReminderDataTemp.getItemAt(curSelIndex_));

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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_REMOVE_BTN);

	// If there's no item, do nothing
	int itemNum = GetItemNum();
	if (itemNum <= 0)
		return;

	// Get current select item index
	int index = curSelIndex_;

	// If item at selected index is empy, do nothing
	if (m_pwrReminderDataTemp.isEmpty(index) == true)
		return;

	// Ask before remove
	int confirm = displayMessageBox(MSGBOX_PWRREMINDER_REMOVE_ITEM, NULL, MB_YESNO | MB_ICONQUESTION);
	if (confirm == IDYES) {
		// Remove item
		Remove(index);
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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_REMOVEALL_BTN);
	
	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.isAllEmpty() == true)
		return;

	// Ask before remove
	int confirm = displayMessageBox(MSGBOX_PWRREMINDER_REMOVEALL_ITEMS, NULL, MB_YESNO | MB_ICONQUESTION);
	if (confirm == IDYES) {
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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_CHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.isAllEmpty() == true)
		return;

	// Check all items
	SetAllItemState(true);
}

/**
 * @brief	Handle click event for [Uncheck All] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnUncheckAll()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_UNCHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.isAllEmpty() == true)
		return;

	// Uncheck all items
	SetAllItemState(false);
}

/**
 * @brief	Handle click event for [Preview] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnPreviewItem()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_PREVIEW_BTN);

	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.isAllEmpty() == true)
		return;

	// Get current selection index
	int currenSelection = curSelIndex_;
	if (m_pwrReminderDataTemp.isEmpty(currenSelection) == true)
		return;

	// Preview item
	int index = currenSelection;
	PreviewItem(index);
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
	NM_GRIDVIEW* reminderItem = (NM_GRIDVIEW*)pNMHDR;
	if (reminderItem == NULL) return;
	int row = reminderItem->iRow;

	//Get current selection index
	curSelIndex_ = row - fixedRowNum;
	int nItemCount = GetItemNum();

	*pResult = NULL;

	// Invalid selection
	if ((curSelIndex_ < 0) || (curSelIndex_ >= nItemCount))
		return;

	// Check read-only mode
	int nCurMode = GetCurMode();
	if ((nCurMode != Mode::Init) && (nCurMode != Mode::View))
		return;

	// Display item details
	DisplayItemDetails(curSelIndex_);
	refreshDialogItemState(true);
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
	NM_GRIDVIEW* reminderItem = (NM_GRIDVIEW*)pNMHDR;
	if (reminderItem == NULL) return;
	int nClickedRow = reminderItem->iRow;

	// Check value validity
	int itemNum = GetItemNum();
	if ((nClickedRow <= Constant::UI::GridCtrl::Index::Header_Row) || (nClickedRow > itemNum)) {
		return;
	}

	// Check read-only mode
	int nCurMode = GetCurMode();
	if ((nCurMode != Mode::Init) && (nCurMode != Mode::View))
		return;

	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	refreshDialogItemState(true);
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
	NM_GRIDVIEW* reminderItem = (NM_GRIDVIEW*)pNMHDR;
	if (reminderItem == NULL) return;
	int nClickedRow = reminderItem->iRow;

	// Check value validity
	int itemNum = GetItemNum();
	if ((nClickedRow <= Constant::UI::GridCtrl::Index::Header_Row) || (nClickedRow > itemNum)) {
		return;
	}

	// Check read-only mode
	int nCurMode = GetCurMode();
	if ((nCurMode != Mode::Init) && (nCurMode != Mode::View))
		return;

	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	refreshDialogItemState(true);
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
	int count = textValue.getLength();
	UpdateMsgCounter(count);
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
	UpdateTimeSetting(m_stDispTimeBak, true);

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
	if (ClockTimeUtils::inputText2Time(clockTime, timeTextValue)) {

		// Update new time value
		UpdateTimeSetting(clockTime, false);

		// Update timespin new position
		int timeSpinPos = 0;
		ClockTimeUtils::time2SpinPos(clockTime, timeSpinPos);
		if (m_pEvtSetTimeSpin != NULL) {
			m_pEvtSetTimeSpin->SetPos(timeSpinPos);
		}
	}
	else {
		// Restore backed-up time value
		UpdateTimeSetting(m_stDispTimeBak, false);
		return;
	}

	// Check for value change and enable/disable save button
	setFlagValue(AppFlagID::dialogDataChanged, checkDataChangeState());
}

/**
 * @brief	Update when time spin value changes
 * @param	pNMDHR  - Default of spin event handler
 * @param	pResult - Default of spin event handler
 * @return	None
 */
void CPwrReminderDlg::OnTimeSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN upDownPtr = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// Get timespin position
	int position = upDownPtr->iPos;

	// Convert to time value
	ClockTime clockTime;
	ClockTimeUtils::spinPos2Time(clockTime, position);

	// Update time edit value
	UpdateTimeSetting(clockTime, false);

	*pResult = NULL;

	// Check for value change and enable/disable save button
	setFlagValue(AppFlagID::dialogDataChanged, checkDataChangeState());
}

/**
 * @brief	Handle clicked for event radio buttons
 * @param	id - ID of button
 * @return	None
 */
void CPwrReminderDlg::OnPwrEventRadBtnClicked(UINT id)
{
	// Ignore if clicked on items with in-range IDs but not radio buttons
	if ((id == IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX) ||
		(id == IDC_PWRREMINDER_EVENT_SETTIME_SPIN))
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
	if (m_pEvtSetTimeRad->IsWindowEnabled() == false)
		return;

	// Update checked state
	int state = m_pEvtSetTimeRad->GetCheck();

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

	// Enable/disable time spinedit and [RepeatSet] button
	if (state == 1) {
		m_pEvtSetTimeEdit->EnableWindow(true);
		m_pEvtSetTimeSpin->EnableWindow(true);
		m_pEvtRepeatSetBtn->EnableWindow(true);
	}
	else {
		m_pEvtSetTimeEdit->EnableWindow(false);
		m_pEvtSetTimeSpin->EnableWindow(false);
		m_pEvtRepeatSetBtn->EnableWindow(false);
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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_EVENT_REPEATSET_BTN);

	// Initialize RepeatSet dialog if not available
	if (m_pRepeatSetDlg == NULL) {
		m_pRepeatSetDlg = new CRmdRepeatSetDlg;
		m_pRepeatSetDlg->Create(IDD_RMDREPEATSET_DLG);
		m_pRepeatSetDlg->removeStyle(DS_MODALFRAME | WS_CAPTION);
		m_pRepeatSetDlg->setParent(this);
	}

	// Get button position
	RECT rcButton{};
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
			unsigned alignment = SDA_LEFTALIGN | SDA_TOPALIGN;
			m_pRepeatSetDlg->setAlignment(alignment);

			// Get button top-right point
			Point btnTopRight(rcButton.right, rcButton.top);

			// Set dialog anchor point
			m_pRepeatSetDlg->setAnchorPoint(btnTopRight);

			// Show dialog
			m_pRepeatSetDlg->ShowWindow(SW_SHOW);
		}
	}
}

/**
 * @brief	Handle click event for [Customize] button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::OnCustomizeStyle()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZE_BTN);

	// Initialize Customize dialog if not available
	if (!m_pMsgStyleSetDlg || !IsWindow(m_pMsgStyleSetDlg->GetSafeHwnd())) {
		delete m_pMsgStyleSetDlg;
		m_pMsgStyleSetDlg = new CRmdMsgStyleSetDlg;
		m_pMsgStyleSetDlg->setParent(this);
	}

	// Preparation
	RmdMsgStyleSet styleSetData;
	MsgStyleSetFlag styleSetDataFlag;

	// Which style configuration data???
	if (m_pStyleUseCommonRad && m_pStyleUseCommonRad->GetCheck() == true) {
		styleSetData = m_pwrReminderDataTemp.getCommonStyle();
		styleSetDataFlag = MsgStyleSetFlag::commonStyle;
	}
	else if (m_pStyleUseCustomRad && m_pStyleUseCustomRad->GetCheck() == true) {
		styleSetData = m_pwrItemInEdit.getMessageStyleData();
		styleSetDataFlag = MsgStyleSetFlag::customStyle;
	}
	else return;

	// Set dialog data
	m_pMsgStyleSetDlg->SetData(styleSetData);
	m_pMsgStyleSetDlg->SetDataFlag(styleSetDataFlag);

	// Show the dialog in modal state
	m_pMsgStyleSetDlg->DoModal();

	// Update common style data
	if (m_pMsgStyleSetDlg->getReturnFlag() == ReturnFlag::OK && styleSetDataFlag == MsgStyleSetFlag::commonStyle) {
		m_pMsgStyleSetDlg->GetData(styleSetData);
		m_pwrReminderDataTemp.getCommonStyle().copy(styleSetData);
	}
}

/**
 * @brief	Request current dialog to close
 * @param	None
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT CPwrReminderDlg::requestCloseDialog(void)
{
	// Close preview message dialog if opening
	if (m_pRmdPreviewMsgDlg != NULL) {
		// Request close message dialog
		LRESULT resClosePreview = m_pRmdPreviewMsgDlg->requestCloseDialog();
		if (resClosePreview != Result::Success) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}

	// If RepeatSet edit dialog is opening
	if (m_pRepeatSetDlg != NULL) {
		// Request close dialog
		LRESULT resCloseRepeatSet = m_pRepeatSetDlg->requestCloseDialog();
		if (resCloseRepeatSet != Result::Success)
			return resCloseRepeatSet;
	}

	// Exit current mode
	int confirm = -1;
	int nCurMode = GetCurMode();
	if ((nCurMode == Mode::Add) || (nCurMode == Mode::Update)) {
		confirm = displayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (confirm == IDYES) {
			// Switch mode
			SetCurMode(Mode::View);
		}
		else if (confirm == IDCANCEL) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}

	// Ask for saving before exiting if data changed
	bool isChanged = checkDataChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);
	if (isChanged == true) {
		confirm = displayMessageBox(MSGBOX_PWRREMINDER_CHANGED_CONTENT, NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (confirm == IDYES) {
			// Save data
			SavePwrReminderData();
		}
		else if (confirm == IDCANCEL) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}

	// Request accepted
	return SDialog::requestCloseDialog();
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
		RECT editBoxRect;
		m_pEvtSetTimeEdit->GetWindowRect(&editBoxRect);
		ScreenToClient(&editBoxRect);

		// If clicked point is inside the editbox area
		if (((pt.x > editBoxRect.left) && (pt.x < editBoxRect.right)) &&
			((pt.y > editBoxRect.top) && (pt.y < editBoxRect.bottom))) {
			if (m_pEvtSetTimeEdit->IsWindowEnabled()) {
				// Select all text
				m_pEvtSetTimeEdit->SetSel(0, -1, true);
			}
			return 0;
		}
		else {
			// If the edit box is focused, kill its focus
			CWnd* curCtrlWndPtr = GetFocus();
			if (curCtrlWndPtr == NULL) return 0;
			if (curCtrlWndPtr->GetDlgCtrlID() == IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX) {
				curCtrlWndPtr->PostMessage(WM_KILLFOCUS);
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
void CPwrReminderDlg::setupLanguage()
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Setup dialog title
	this->setCaptionFromLanguage(getDialogId());

	// Loop through all dialog items and setup language for each one of them
	for (CWnd* childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		unsigned id = childWndPtr->GetDlgCtrlID();

		switch (id)
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
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Setup data item list
	SetupDataItemList(languageTablePtr);

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Initialize and setup language for Power Reminder data item list
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CPwrReminderDlg::SetupDataItemList(LANGTABLE_PTR languageTablePtr)
{
	// Get parent list frame rect
	CWnd* listFrameWndPtr = GetDlgItem(IDC_PWRREMINDER_ITEM_LISTBOX);
	if (listFrameWndPtr == NULL) return;
	RECT listFrameWndRect;
	listFrameWndPtr->GetWindowRect(&listFrameWndRect);
	ScreenToClient(&listFrameWndRect);

	// Get frame size
	if (m_pszFrameWndSize == NULL) {
		m_pszFrameWndSize = new Size();
		m_pszFrameWndSize->_width = listFrameWndRect.right - listFrameWndRect.left;
		m_pszFrameWndSize->_height = listFrameWndRect.bottom - listFrameWndRect.top;
	}

	// Initialization
	VERIFY_INITIALIZATION(m_pDataItemListTable, CGridCtrl)

	// Create table
	if (m_pDataItemListTable == NULL) return;
	DWORD style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	m_pDataItemListTable->Create(listFrameWndRect, this, IDC_PWRREMINDER_ITEM_LISTBOX, style);

	// Destroy frame
	listFrameWndPtr->DestroyWindow();

	// Cell format
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)m_pDataItemListTable->GetDefaultCell(false, false);
	if (cellPtr == NULL) return;
	cellPtr->SetFormat(cellPtr->GetFormat());
	cellPtr->SetMargin(0);
	cellPtr->SetBackClr(Color::White);
	cellPtr->SetTextClr(Color::Black);
	cellPtr->SetHeight(Constant::UI::GridCtrl::Height::Row);

	// Table format and properties
	int nRowNum = (GetItemNum() + fixedRowNum);
	int nColNum = columnCount_;

	// Setup table
	m_pDataItemListTable->SetColumnCount(nColNum);
	m_pDataItemListTable->SetFixedColumnCount(fixedColumnNum);
	m_pDataItemListTable->SetRowCount(nRowNum);
	m_pDataItemListTable->SetFixedRowCount(fixedColumnNum);
	m_pDataItemListTable->SetRowHeight(Constant::UI::GridCtrl::Index::Header_Row, Constant::UI::GridCtrl::Height::Header);

	// Draw table
	DrawDataTable(m_pszFrameWndSize, nColNum, nRowNum, false, languageTablePtr);

	// Update layout info
	updateLayoutInfo();

	// Display table
	m_pDataItemListTable->SetListMode(true);
	m_pDataItemListTable->SetEditable(false);
	m_pDataItemListTable->SetRowResize(false);
	m_pDataItemListTable->EnableSelection(true);
	m_pDataItemListTable->SetSingleRowSelection(true);
	m_pDataItemListTable->SetSingleColSelection(false);
	m_pDataItemListTable->SetFixedRowSelection(false);
	m_pDataItemListTable->SetFixedColumnSelection(false);
	m_pDataItemListTable->ShowWindow(SW_SHOW);
	m_pDataItemListTable->SetRedraw(true);
}

/**
 * @brief	Draw data list table
 * @param	szFrameWndSize	- Frame size
 * @param	nColNum			- Number of table columns
 * @param	nRowNum			- Number of table rows
 * @param	isReadOnly		- Read-only mode
 * @param	languageTablePtr		- Pointer to app language
 * @return	None
 */
void CPwrReminderDlg::DrawDataTable(Size* pszFrameWndSize, int nColNum, int nRowNum, bool isReadOnly /* = false */, LANGTABLE_PTR languageTablePtr /* = NULL */)
{
	// Check table validity
	if (m_pDataItemListTable == NULL)
		return;

	// Check table format data validity
	if (pszFrameWndSize == NULL) return;
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Check row and column number validity
	if ((nColNum <= 0) || (nRowNum < fixedRowNum))
		return;

	// Get app pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) return;

	// Load app language package
	if (languageTablePtr == NULL) {
		languageTablePtr = theAppPtr->getAppLanguage();
	}

	// Re-update default cell properties
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)m_pDataItemListTable->GetDefaultCell(false, false);
	if (cellPtr == NULL) return;

	// Read-only mode --> Change cell color
	if (isReadOnly == true) {
		cellPtr->SetBackClr(Color::Bright_Gray);
		cellPtr->SetTextClr(Color::Dark_Gray);
	}
	else {
		cellPtr->SetBackClr(Color::White);
		cellPtr->SetTextClr(Color::Black);
	}

	// Setup display size
	int frameHeight = pszFrameWndSize->height();
	int frameWidth = pszFrameWndSize->width();
	if (AppCore::getWindowsOSVersion() == WINDOWS_VERSION_10) {
		// Windows 10 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl_Win10;
		//frameHeight -= OFFSET_HEIGHT_LISTCTRL_WIN10;
	}
	else {
		// Windows 11 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl;
		//frameHeight -= OFFSET_HEIGHT_LISTCTRL;
	}
	if ((Constant::UI::GridCtrl::Height::Header + ((nRowNum - 1) * Constant::UI::GridCtrl::Height::Row)) >= frameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int scrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		frameWidth -= (scrollBarWidth + Constant::UI::Offset::Width::VScrollBar);
	}

	// Setup columns
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Set header row style
		AppCore::setFixedCellStyle(m_pDataItemListTable, Constant::UI::GridCtrl::Index::Header_Row, nCol);

		// Column header title
		String headerTitle = Constant::String::Empty;
		unsigned nHeaderTitleID = gridCtrlFormatInfoPtr_[nCol].headerTitleId;
		if (nHeaderTitleID != INT_NULL) {
			headerTitle = getLanguageString(languageTablePtr, nHeaderTitleID);
		}
		m_pDataItemListTable->SetItemText(Constant::UI::GridCtrl::Index::Header_Row, nCol, headerTitle);

		// Column width
		int nColWidth = gridCtrlFormatInfoPtr_[nCol].width;
		if (nColWidth != -1) {
			// Set column width as defined
			m_pDataItemListTable->SetColumnWidth(nCol, nColWidth);
			// Calculate remaining width
			frameWidth -= nColWidth;
		}
		else {
			// Set remaining width for current column
			m_pDataItemListTable->SetColumnWidth(nCol, frameWidth);
		}
	}

	// Setup rows
	int nColStyle = -1;
	unsigned itemState = INT_NULL;
	for (int row = 1; row < nRowNum; row++) {
		for (int nCol = 0; nCol < columnCount_; nCol++) {
			// Get column style & item state
			nColStyle = gridCtrlFormatInfoPtr_[nCol].columnStyle;
			itemState = m_pDataItemListTable->GetItemState(row, nCol);
			itemState |= GVIS_READONLY;

			// Base column - header-like style
			if (nColStyle == COLSTYLE_FIXED) {
				// Set fixed cell style
				AppCore::setFixedCellStyle(m_pDataItemListTable, row, nCol);
			}

			// Checkbox column
			else if (nColStyle == COLSTYLE_CHECKBOX) {
				// Set cell type: Checkbox
				if (!m_pDataItemListTable->SetCellType(row, nCol, RUNTIME_CLASS(CGridCellCheck)))
					continue;

				// Get cell
				CGridCellCheck* cellPtr = (CGridCellCheck*)m_pDataItemListTable->GetCell(row, nCol);

				// Set center alignment if defined
				if (gridCtrlFormatInfoPtr_[nCol].isCentered == true) {
					if (cellPtr == NULL) continue;
					cellPtr->SetCheckPlacement(SCP_CENTERING);
				}
			}

			// Normal column
			else if (nColStyle == COLSTYLE_NORMAL) {
				// Set item state
				if (!m_pDataItemListTable->SetItemState(row, nCol, itemState))
					continue;

				// Get cell
				CGridCellBase* cellPtr = (CGridCellBase*)m_pDataItemListTable->GetCell(row, nCol);

				// Set center alignment if defined
				if (gridCtrlFormatInfoPtr_[nCol].isCentered == true) {
					if (cellPtr == NULL) continue;
					cellPtr->SetFormat(cellPtr->GetFormat() | DT_CENTER);
				}
				else {
					// Set margin (left alignment)
					if (cellPtr == NULL) continue;
					cellPtr->SetMargin(Constant::UI::GridCtrl::Margin::Left);
				}
			}
		}
	}
}

/**
 * @brief	Setup data for combo-boxes
 * @param	comboId	- ID of combo box
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CPwrReminderDlg::setupComboBox(unsigned comboId, LANGTABLE_PTR languageTablePtr)
{
	// Check combo validity
	if (m_pMsgStyleCombo == NULL) return;

	switch (comboId)
	{
	case IDC_PWRREMINDER_MSGSTYLE_COMBO:
		m_pMsgStyleCombo->ResetContent();
		m_pMsgStyleCombo->AddString(getLanguageString(languageTablePtr, COMBOBOX_MSGSTYLE_MESSAGEBOX));		// Message Box
		m_pMsgStyleCombo->AddString(getLanguageString(languageTablePtr, COMBOBOX_MSGSTYLE_DIALOGBOX));		// Dialog Box
		break;

	default:
		break;
	}

	// Default
	SDialog::setupComboBox(comboId, languageTablePtr);
}

/**
 * @brief	Switch between modes and re-update dialog item states
 * @param	bRedraw - Redraw dialog items or not
 * @return	None
 */
void CPwrReminderDlg::SwitchMode(bool /* bRedraw = false */)
{
	int nCurMode = GetCurMode();
	if (nCurMode == Mode::Init) {
		// Unlock dialog items
		setLockState(false);

		// Restore [Add/Edit] buttons caption
		updateItemText(IDC_PWRREMINDER_ADD_BTN);
		updateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Enable table
		DisableTable(false);

		// Refresh detail view
		RefreshDetailView(Mode::Init);

		// Refresh dialog item states
		refreshDialogItemState(true);
	}
	else if (nCurMode == Mode::View) {
		// Lock dialog items
		setLockState(false);

		// Restore [Add/Edit] buttons caption
		updateItemText(IDC_PWRREMINDER_ADD_BTN);
		updateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Enable table
		DisableTable(false);

		// Refresh detail view
		RefreshDetailView(Mode::View);
		DisplayItemDetails(curSelIndex_);

		// Refresh dialog item states
		refreshDialogItemState(true);
	}
	else if (nCurMode == Mode::Add) {
		// Lock dialog items
		setLockState(true);

		// Enable/disable controls
		enableItem(IDC_PWRREMINDER_ADD_BTN,			true);
		enableItem(IDC_PWRREMINDER_EDIT_BTN,		false);
		enableItem(IDC_PWRREMINDER_REMOVE_BTN,		false);
		enableItem(IDC_PWRREMINDER_REMOVEALL_BTN,	false);
		enableItem(IDC_PWRREMINDER_CHECKALL_BTN,	false);
		enableItem(IDC_PWRREMINDER_UNCHECKALL_BTN,	false);
		enableItem(IDC_PWRREMINDER_PREVIEW_BTN,		false);
		enableItem(IDC_PWRREMINDER_APPLY_BTN,		false);

		// Change [Add] button title to [Save]
		updateItemText(IDC_PWRREMINDER_ADD_BTN, BTN_PWRRMDDLG_SAVECHANGES);

		// Disable table
		DisableTable(true);

		// Refresh detail view
		RefreshDetailView(Mode::Add);
		DisplayItemDetails(INT_INVALID);
	}
	else if (nCurMode == Mode::Update) {
		// Lock dialog items
		setLockState(true);

		// Enable/disable controls
		enableItem(IDC_PWRREMINDER_ADD_BTN,			false);
		enableItem(IDC_PWRREMINDER_EDIT_BTN,		true);
		enableItem(IDC_PWRREMINDER_REMOVE_BTN,		false);
		enableItem(IDC_PWRREMINDER_REMOVEALL_BTN,	false);
		enableItem(IDC_PWRREMINDER_CHECKALL_BTN,	false);
		enableItem(IDC_PWRREMINDER_UNCHECKALL_BTN,	false);
		enableItem(IDC_PWRREMINDER_PREVIEW_BTN,		false);
		enableItem(IDC_PWRREMINDER_APPLY_BTN,		false);

		// Change [Edit] button title to [Save]
		updateItemText(IDC_PWRREMINDER_EDIT_BTN, BTN_PWRRMDDLG_SAVECHANGES);

		// Disable table
		DisableTable(true);

		// Refresh detail view
		RefreshDetailView(Mode::Update);
	}
	else if (nCurMode == Mode::Disable) {
		// Lock dialog items
		setLockState(true);

		// Restore [Add/Edit] buttons caption
		updateItemText(IDC_PWRREMINDER_ADD_BTN);
		updateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Disable table
		DisableTable(true);

		// Refresh dialog item states
		refreshDialogItemState(true);
	}
}

/**
 * @brief	Update layout info data
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::updateLayoutInfo(void)
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Check table column format data validity
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Get table column count
	int nColNum = m_pDataItemListTable->GetColumnCount();

	// Update size of table columns
	for (int index = 0; index < nColNum; index++) {
		int nColSize = m_pDataItemListTable->GetColumnWidth(index);
		gridCtrlFormatInfoPtr_[index].width = nColSize;
	}
}

/**
 * @brief	Load layout info data
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::loadLayoutInfo(void)
{
	// Define default table columns format
	const GRIDCTRLCOLFORMAT arrGrdColFormat[] = {
	//-----------ID--------------------------Header title ID-------------Width(px)---Column style--------Align Center---
		{	ColumnID::Index,			GRIDCOLUMN_PWRREMINDER_INDEX,		26,		COLSTYLE_FIXED,			true,	},
		{	ColumnID::EnableState,		GRIDCOLUMN_PWRREMINDER_STATE,		55,		COLSTYLE_CHECKBOX,		true,	},
		{	ColumnID::ItemID,			GRIDCOLUMN_PWRREMINDER_ITEMID,		75,		COLSTYLE_NORMAL,		true,	},
		{ 	ColumnID::MessageContent,	GRIDCOLUMN_PWRREMINDER_MESSAGE,		237,	COLSTYLE_NORMAL,		false,	},
		{ 	ColumnID::EventID,			GRIDCOLUMN_PWRREMINDER_EVENTID,		140,	COLSTYLE_NORMAL,		true,	},
		{ 	ColumnID::MsgStyle,			GRIDCOLUMN_PWRREMINDER_STYLE,		107,	COLSTYLE_NORMAL,		true,	},
		{ 	ColumnID::Repeat,			GRIDCOLUMN_PWRREMINDER_REPEAT,		56,		COLSTYLE_CHECKBOX,		true,	},
	//------------------------------------------------------------------------------------------------------------------
	};

	// Backup format data
	columnCount_ = (sizeof(arrGrdColFormat) / sizeof(GRIDCTRLCOLFORMAT));

	// Initialize table format info data
	if (gridCtrlFormatInfoPtr_ == NULL) {
		gridCtrlFormatInfoPtr_ = new GRIDCTRLCOLFORMAT[columnCount_];
		for (int index = 0; index < columnCount_; index++) {
			// Copy default table column format data
			gridCtrlFormatInfoPtr_[index] = arrGrdColFormat[index];
		}
	}

	// Load layout info data from registry
	int returnValue = 0;
	String keyName;
	for (int index = 0; index < columnCount_; index++) {
		keyName = Key::LayoutInfo::GridColSize(index);
		if (GetLayoutInfo(Section::LayoutInfo::PwrReminderTable, keyName, returnValue)) {
			if (gridCtrlFormatInfoPtr_ != NULL) {
				gridCtrlFormatInfoPtr_[index].width = returnValue;
			}
		}
	}
}

/**
 * @brief	Save layout info data
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::saveLayoutInfo(void)
{
	// Check table column format data validity
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Save layout info data to registry
	int nRef = 0;
	String keyName;
	for (int index = 0; index < columnCount_; index++) {
		nRef = gridCtrlFormatInfoPtr_[index].width;
		keyName = Key::LayoutInfo::GridColSize(index);
		WriteLayoutInfo(Section::LayoutInfo::PwrReminderTable, keyName, nRef);
	}
}

/**
 * @brief	Setup state and properties for dialog items
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::setupDialogItemState()
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
			TRACE_ERROR("Error: [RepeatSet] button not found!!!");
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
	if (m_pStyleUseCommonRad == NULL) {
		m_pStyleUseCommonRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_USECOMMON_RADBTN);
		if (m_pStyleUseCommonRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (StyleUseCommonRadio)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pStyleUseCustomRad == NULL) {
		m_pStyleUseCustomRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_USECUSTOM_RADBTN);
		if (m_pStyleUseCustomRad == NULL) {
			TRACE_ERROR("Error: Radio button not found (StyleUseCustomRadio)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pStyleCustomizeBtn == NULL) {
		m_pStyleCustomizeBtn = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZE_BTN);
		if (m_pStyleCustomizeBtn == NULL) {
			TRACE_ERROR("Error: [Customize] button not found!!!");
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
	ClockTimeUtils::spinPos2Time(clockTime, 0);
	UpdateTimeSetting(clockTime, false);

	// Initialize counter display
	UpdateMsgCounter(0);

	// Default
	SDialog::setupDialogItemState();
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
	int itemNum = GetItemNum();
	if (itemNum <= 0)
		return;

	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
	
	// Print items
	int nTemp = -1;
	int rowIndex = 0;
	CGridCellCheck* cellCheckPtr = NULL;
	for (int index = 0; index < itemNum; index++) {

		// Get row index
		rowIndex = index + fixedRowNum;

		// Get item
		const Item& reminderItem = m_pwrReminderDataTemp.getItemAt(index);

		// Item index
		String tempString = StringUtils::stringFormat(_T("%d"), rowIndex);
		m_pDataItemListTable->SetItemText(rowIndex, ColumnID::Index, tempString);

		// Enable state
		cellCheckPtr = (CGridCellCheck*)m_pDataItemListTable->GetCell(rowIndex, ColumnID::EnableState);
		if (cellCheckPtr != NULL) {
			cellCheckPtr->SetCheck(reminderItem.isEnabled());
		}

		// ItemID
		tempString.format(_T("%d"), reminderItem.getItemId());
		m_pDataItemListTable->SetItemText(rowIndex, ColumnID::ItemID, tempString);

		// Message content
		tempString = reminderItem.getMessage();
		m_pDataItemListTable->SetItemText(rowIndex, ColumnID::MessageContent, tempString);

		// EventID
		nTemp = GetPairedID(IDTable::PwrReminderEvent, reminderItem.getEventId());
		tempString = getLanguageString(languageTablePtr, nTemp);
		if (reminderItem.getEventId() == Event::atSetTime) {
			// Format time string
			String formatTime = tempString;
			tempString = ClockTimeUtils::format(languageTablePtr, formatTime, reminderItem.getTime());
		}
		m_pDataItemListTable->SetItemText(rowIndex, ColumnID::EventID, tempString);

		// Message style
		nTemp = GetPairedID(IDTable::PwrReminderStyle, reminderItem.getMessageStyle());
		tempString = getLanguageString(languageTablePtr, nTemp);
		m_pDataItemListTable->SetItemText(rowIndex, ColumnID::MsgStyle, tempString);

		// Repeat
		cellCheckPtr = (CGridCellCheck*)m_pDataItemListTable->GetCell(rowIndex, ColumnID::Repeat);
		if (cellCheckPtr != NULL) {
			cellCheckPtr->SetCheck(reminderItem.isRepeatEnabled());
		}
	}
}

/**
 * @brief	Disable mouse click events for table
 * @param	isDisabled - Disable/enable
 * @return	None
 */
void CPwrReminderDlg::DisableTable(bool isDisabled)
{
	// Redraw read-only style
	RedrawDataTable(isDisabled);

	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Disable/enable mouse events
	m_pDataItemListTable->DisableMouseClick(isDisabled);
	m_pDataItemListTable->DisableMouseMove(isDisabled);
}

/**
 * @brief	Update and redraw data table
 * @param	bool isReadOnly - Read-only mode
 * @return	None
 */
void CPwrReminderDlg::RedrawDataTable(bool isReadOnly /* = false */)
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Update new row number
	int nCurRowNum = (GetItemNum() + fixedRowNum);
	m_pDataItemListTable->SetRowCount(nCurRowNum);

	// Draw table
	DrawDataTable(m_pszFrameWndSize, columnCount_, nCurRowNum, isReadOnly);
	
	// Update table data
	UpdateDataItemList();

	// Trigger redrawing table
	m_pDataItemListTable->RedrawWindow();
}

/**
 * @brief	Display details of an item at specified index
 * @param	index - Index of item to display
 * @return	None
 */
void CPwrReminderDlg::DisplayItemDetails(int index)
{
	// Check index validity
	if ((index < INT_INVALID) || (index > GetItemNum()))
		return;

	// Get item at index
	Item reminderItem;
	if (index != INT_INVALID) {
		reminderItem = m_pwrReminderDataTemp.getItemAt(index);
	}

	// Init default data for mode add
	if (GetCurMode() == Mode::Add) {
		reminderItem.setMessage(Constant::String::Empty);
		reminderItem.setEventId(Event::atSetTime);
		reminderItem.setTime(ClockTimeUtils::getCurrentClockTime());
		reminderItem.setMessageStyle(Style::messageBox);
		reminderItem.enableCustomStyle(false);
		reminderItem.resetRepeatInfo();
		reminderItem.resetMessageStyleInfo();
	}

	// If item is empty
	if (reminderItem.isEmpty()) {
		// TODO: Disable all detail item controls
		RefreshDetailView(Mode::Init);
	}
	else {
		// TODO: Update current displaying item index
		RefreshDetailView(GetCurMode());
		m_nCurDispIndex = index;
	}

	// Initialize RepeatSet dialog if not available
	if (!m_pRepeatSetDlg) {
		m_pRepeatSetDlg = new CRmdRepeatSetDlg;
		m_pRepeatSetDlg->Create(IDD_RMDREPEATSET_DLG);
		m_pRepeatSetDlg->removeStyle(DS_MODALFRAME | WS_CAPTION);
		m_pRepeatSetDlg->setParent(this);
	}

	// Initialize Customize dialog if not available
	if (!m_pMsgStyleSetDlg || !IsWindow(m_pMsgStyleSetDlg->GetSafeHwnd())) {
		delete m_pMsgStyleSetDlg;
		m_pMsgStyleSetDlg = new CRmdMsgStyleSetDlg;
		m_pMsgStyleSetDlg->setParent(this);
	}

	// Display item details
	UpdateItemData(reminderItem, false);
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	isRecheckState - Recheck all item's state
 * @return	None
 */
void CPwrReminderDlg::refreshDialogItemState(bool isRecheckState /* = false */)
{
	CWnd* buttonPtr = NULL;

	// If dialog items are being locked, do nothing
	if (getLockState() == true)
		return;

	// Check if any item is selected or not
	bool bIsSelected = ((curSelIndex_ >= 0) && (curSelIndex_ < GetItemNum()));

	// Check if number of item has reached the limit
	bool bIsMaxNum = (GetItemNum() >= PwrReminderData::maxItemNum);

	// Check if data is all empty or not
	bool bIsAllEmpty = m_pwrReminderDataTemp.isAllEmpty();

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Disable [Add] button if item number has reached the limit
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_ADD_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(!bIsMaxNum);
		buttonPtr->SetWindowText(getLanguageString(languageTablePtr, IDC_PWRREMINDER_ADD_BTN));
	}

	// Disable [Remove/Remove All] buttons if data is all empty
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_REMOVE_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(!bIsAllEmpty);

		// Enable [Remove] button if any item is selected
		buttonPtr->EnableWindow(bIsSelected);
	}
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_REMOVEALL_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(!bIsAllEmpty);
	}

	// Enable [Edit] and [Preview] button if any item is selected
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_EDIT_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(bIsSelected);
		buttonPtr->SetWindowText(getLanguageString(languageTablePtr, IDC_PWRREMINDER_EDIT_BTN));
	}
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_PREVIEW_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(bIsSelected);
	}

	// Check if data is changed or not
	bool isChanged = checkDataChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);

	// Enable [Apply] button if data is changed
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_APPLY_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(isChanged);
	}

	// Update [Check/Uncheck All] button state
	updateCheckAllBtnState(isRecheckState);

	// Default
	SDialog::refreshDialogItemState(isRecheckState);
}

/**
 * @brief	Refresh and update state for [Check/Uncheck All] button
 * @param	isRecheck - Recheck all items enable state
 * @return	None
 */
void CPwrReminderDlg::updateCheckAllBtnState(bool isRecheck /* = false */)
{
	// If dialog items are being locked, do nothing
	if (getLockState() == true)
		return;

	// Get buttons
	CWnd* pCheckAllBtn = GetDlgItem(IDC_PWRREMINDER_CHECKALL_BTN);
	CWnd* pUncheckAllBtn = GetDlgItem(IDC_PWRREMINDER_UNCHECKALL_BTN);
	if ((pCheckAllBtn == NULL) || (pUncheckAllBtn == NULL))
		return;

	// Get number of items
	int itemNum = GetItemNum();
	if (itemNum == 0) {
		// Disable both [Check/Uncheeck All] buttons
		pCheckAllBtn->EnableWindow(false);
		pUncheckAllBtn->EnableWindow(false);
		return;
	}

	// Recheck all items state
	if (isRecheck == true) {
		checkCount_ = 0; // Reset counter
		for (int index = 0; index < itemNum; index++) {
			const Item& pwrTemp = m_pwrReminderDataTemp.getItemAt(index);
			if (pwrTemp.isEnabled() == true) {
				checkCount_++;
			}
		}
	}

	// Update button state
	if (checkCount_ == 0) {
		// Enable [Check All] button
		pCheckAllBtn->EnableWindow(true);
		// Disable [Uncheck All] button
		pUncheckAllBtn->EnableWindow(false);
	}
	else if (checkCount_ == itemNum) {
		// Disable [Check All] button
		pCheckAllBtn->EnableWindow(false);
		// Enable [Uncheck All] button
		pUncheckAllBtn->EnableWindow(true);
	}
	else {
		// Enable both [Check/Uncheck All] buttons
		pCheckAllBtn->EnableWindow(true);
		pUncheckAllBtn->EnableWindow(true);
	}
}

/**
 * @brief	Refresh and update state for detail view
 * @param	mode - Detail view mode
 * @return	None
 */
void CPwrReminderDlg::RefreshDetailView(int mode)
{
	// Set state and init value by mode
	bool isEnabled = true;
	if ((mode == Mode::Init) || (mode == Mode::View)) {
		isEnabled = false;
	}
	else if ((mode == Mode::Add) || (mode == Mode::Update)) {
		isEnabled = true;
	}
	else if (mode == Mode::Disable) {
		isEnabled = false;
	}

	/***************************************************************/
	/*															   */
	/*				Update detail view item states				   */
	/*															   */
	/***************************************************************/

	CWnd* windowPtr = NULL;

	// Message content
	windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_TITLE);
	if (windowPtr != NULL) {
		windowPtr->EnableWindow(isEnabled);
	}
	if (m_pMsgStringEdit != NULL) {
		m_pMsgStringEdit->EnableWindow(isEnabled);
	}
	windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_COUNTER);
	if (windowPtr != NULL) {
		windowPtr->EnableWindow(isEnabled);
	}
	// Event
	windowPtr = GetDlgItem(IDC_PWRREMINDER_EVENT_TITLE);
	if (windowPtr != NULL) {
		windowPtr->EnableWindow(isEnabled);
	}
	if (m_pEvtSetTimeRad != NULL) {
		m_pEvtSetTimeRad->EnableWindow(isEnabled);
		bool bEnableSetTime = (isEnabled && m_pEvtSetTimeRad->GetCheck());
		if (m_pEvtSetTimeEdit != NULL) {
			m_pEvtSetTimeEdit->EnableWindow(bEnableSetTime);
			UpdateTimeSetting(m_stDispTimeBak, false);
		}
		if (m_pEvtSetTimeSpin != NULL) {
			m_pEvtSetTimeSpin->EnableWindow(bEnableSetTime);
		}
		if (m_pEvtRepeatSetBtn != NULL) {
			m_pEvtRepeatSetBtn->EnableWindow(bEnableSetTime);
		}
	}
	if (m_pEvtAppStartupRad != NULL) {
		m_pEvtAppStartupRad->EnableWindow(isEnabled);
	}
	if (m_pEvtSysWakeupRad != NULL) {
		m_pEvtSysWakeupRad->EnableWindow(isEnabled);
	}
	if (m_pEvtBfrPwrActionRad != NULL) {
		m_pEvtBfrPwrActionRad->EnableWindow(isEnabled);
	}
	if (m_pEvtPwrActionWakeRad != NULL) {
		m_pEvtPwrActionWakeRad->EnableWindow(isEnabled);
	}
	if (m_pEvtAtAppExitRad != NULL) {
		m_pEvtAtAppExitRad->EnableWindow(isEnabled);
	}
	// Message style
	windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_TITLE);
	if (windowPtr != NULL) {
		windowPtr->EnableWindow(isEnabled);
	}
	if (m_pStyleMsgBoxRad != NULL) {
		m_pStyleMsgBoxRad->EnableWindow(isEnabled);
	}
	if (m_pStyleDialogBoxRad != NULL) {
		m_pStyleDialogBoxRad->EnableWindow(isEnabled);
	}
	windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZATION_TITLE);
	if (windowPtr != NULL) {
		windowPtr->EnableWindow(isEnabled);
	}
	if (m_pStyleUseCommonRad != NULL) {
		m_pStyleUseCommonRad->EnableWindow(isEnabled);
	}
	if (m_pStyleUseCustomRad != NULL) {
		m_pStyleUseCustomRad->EnableWindow(isEnabled);
	}
	if (m_pStyleCustomizeBtn != NULL) {
		m_pStyleCustomizeBtn->EnableWindow(isEnabled);
	}
}

/**
 * @brief	Update message content length counter
 * @param	count - Character number counter
 * @return	None
 */
void CPwrReminderDlg::UpdateMsgCounter(int count)
{
	// Get dialog item
	CWnd* pCounter = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_COUNTER);
	if (pCounter == NULL) return;

	// Check counter value validity
	if ((count < 0) || (count > Constant::Max::StringLength)) return;

	// Display counter
	String countFormatString;
	countFormatString.format(_T("%d/%d"), count, Constant::Max::StringLength);
	pCounter->SetWindowText(countFormatString);
}

/**
 * @brief	Update time value from/to time edit control
 * @param	clockTime  - Clock-time data
 * @param	updateFlag	   - Update or not (YES/true by default)
 * @return	None
 */
void CPwrReminderDlg::UpdateTimeSetting(ClockTime& clockTime, bool updateFlag /* = true */)
{
	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Check time editbox validity
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	if (updateFlag == true) {

		// Get value from time editbox
		const int buffLength = m_pEvtSetTimeEdit->GetWindowTextLength();
		std::vector<wchar_t> tempBuff(buffLength + 1);
		m_pEvtSetTimeEdit->GetWindowText(tempBuff.data(), buffLength + 1);
		String timeTextValue = tempBuff.data();

		// Get hour value
		int hour = _wtoi(timeTextValue.left(2));
		const String timePeriod = timeTextValue.right(2);
		if (timePeriod == getLanguageString(languageTablePtr, FORMAT_TIMEPERIOD_ANTE_MERIDIEM)) {
			// Before midday
			clockTime.setHour(hour);
		}
		else if ((timePeriod == getLanguageString(languageTablePtr, FORMAT_TIMEPERIOD_POST_MERIDIEM)) && hour < 12) {
			// After midday
			clockTime.setHour(hour + 12);
		}
		else {
			// Keep value
			clockTime.setHour(hour);
		}

		// Get minute value
		clockTime.setMinute(_wtoi(timeTextValue.mid(3, 2)));
	}
	else {
		// Set value for time editbox
		String timeFormatString;
		timeFormatString = ClockTimeUtils::format(languageTablePtr, IDS_FORMAT_SHORTTIME, clockTime);
		m_pEvtSetTimeEdit->SetWindowText(timeFormatString);

		// Backup current displaying time value
		m_stDispTimeBak = clockTime;

		// Update time spin position
		if (m_pEvtSetTimeSpin != NULL) {
			int timeSpinPos = 0;
			ClockTimeUtils::time2SpinPos(clockTime, timeSpinPos);
			m_pEvtSetTimeSpin->SetPos(timeSpinPos);
		}
	}
}

/**
 * @brief	Load Power Reminder data
 * @param	None
 * @return	bool - Result of loading process
 */
bool CPwrReminderDlg::LoadPwrReminderData()
{
	// Get app Reminder data pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	VERIFY(theAppPtr != NULL);
	if (theAppPtr == NULL) return false;
	Data* reminderDataPtr = theAppPtr->getAppPwrReminderData();
	if (reminderDataPtr == NULL)
		return false;

	// Copy data
	m_pwrReminderData.copy(*reminderDataPtr);
	m_pwrReminderDataTemp.copy(m_pwrReminderData);

	// Reset change flag
	setFlagValue(AppFlagID::dialogDataChanged, false);

	// Validate data and auto-correction
	for (int index = 0; index < GetItemNum(); index++) {
		Item& reminderItem = m_pwrReminderDataTemp.getItemAt(index);
		if (!Validate(reminderItem, true, true)) {
			// Update temp data
			m_pwrReminderDataTemp.update(reminderItem);
			setFlagValue(AppFlagID::dialogDataChanged, true);	// Update change flag
		}
	}

	return true;
}

/**
 * @brief	Save Power Reminder data
 * @param	None
 * @return	bool - Result of saving process
 */
bool CPwrReminderDlg::SavePwrReminderData()
{
	// Copy data and adjust validity
	m_pwrReminderData.copy(m_pwrReminderDataTemp);
	m_pwrReminderData.adjust();

	// Save app Power Reminder data
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	VERIFY(theAppPtr != NULL);
	if (theAppPtr == NULL) return false;
	theAppPtr->setAppPwrReminderData(&m_pwrReminderData);
	theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);

	// Notify main dialog to re-update Power Reminder data
	CPowerPlusDlg* mainDialogPtr = (CPowerPlusDlg*)(theAppPtr->GetMainWnd());
	VERIFY(mainDialogPtr != NULL);
	if (mainDialogPtr == NULL) return false;
	mainDialogPtr->PostMessage(SM_APP_UPDATE_PWRREMINDERDATA, NULL, NULL);

	// Reset change flag
	setFlagValue(AppFlagID::dialogDataChanged, false);
	
	return true;
}

/**
 * @brief	Check if settings changed
 * @param	None
 * @return	None
 */
bool CPwrReminderDlg::checkDataChangeState()
{
	bool changeFlag = false;

	// Update enable and repeat states of each item
	int rowIndex = 0;
	CGridCellCheck* pCellCheckEnable = NULL;
	CGridCellCheck* pCellCheckRepeat = NULL;
	int nItemRowNum = (m_pDataItemListTable->GetRowCount() - fixedRowNum);
	for (int index = 0; index < nItemRowNum; index++) {
		// Get row index
		rowIndex = (index + fixedRowNum);

		// Get checkbox cells
		pCellCheckEnable = (CGridCellCheck*)m_pDataItemListTable->GetCell(rowIndex, ColumnID::EnableState);
		pCellCheckRepeat = (CGridCellCheck*)m_pDataItemListTable->GetCell(rowIndex, ColumnID::Repeat);
		if ((pCellCheckEnable == NULL) || (pCellCheckRepeat == NULL)) continue;
		
		// Get checked states
		bool isEnabled = pCellCheckEnable->GetCheck();
		bool bRepeat = pCellCheckRepeat->GetCheck();
		
		// Update item enable and repeat states
		Item& pwrTempItem = m_pwrReminderDataTemp.getItemAt(index);
		pwrTempItem.enableItem(isEnabled);
		pwrTempItem.enableRepeat(bRepeat);
	}

	// Check if number of items changed
	int itemNum = GetItemNum();
	changeFlag |= (itemNum != m_pwrReminderData.getItemNum());
	if (changeFlag == true)
		return changeFlag;

	// Check if common style data changed
	const RmdMsgStyleSet& rmdCurCommonStyle = m_pwrReminderData.getCommonStyle();
	const RmdMsgStyleSet& rmdTempCommonStyle = m_pwrReminderDataTemp.getCommonStyle();
	changeFlag |= (rmdCurCommonStyle.compare(rmdTempCommonStyle) != true);
	if (changeFlag == true)
		return changeFlag;

	// Check if each item's data changed
	for (int index = 0; index < itemNum; index++) {
		// Get current item and temp item
		const Item& pwrCurItem = m_pwrReminderData.getItemAt(index);
		const Item& pwrTempItem = m_pwrReminderDataTemp.getItemAt(index);
 
		// Data comparison
		changeFlag |= (pwrTempItem.isEnabled() != pwrCurItem.isEnabled());
		changeFlag |= (pwrTempItem.getItemId() != pwrCurItem.getItemId());
		changeFlag |= (pwrTempItem.compare(pwrCurItem) != true);

		// Stop on the first different item encountered
		if (changeFlag == true) break;
	}
	
	return changeFlag;
}

/**
 * @brief	Add current settings to reminder data list
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::Add()
{
	// Update data
	UpdateData(true);

	// Create temp Reminder item
	Item pwrTemp;
	pwrTemp.setItemId(m_pwrReminderDataTemp.getNextId());

	// Update data
	UpdateItemData(pwrTemp, true);

	// Check data validity
	bool bValid = Validate(pwrTemp, true);
	if (bValid == false)
		return;

	// Update item to Power Reminder data item list
	m_pwrReminderDataTemp.update(pwrTemp);

	// Update table
	RedrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Edit a reminder item by index
 * @param	index - Index of item to edit
 * @return	None
 */
void CPwrReminderDlg::Edit(int index)
{
	// Update data
	UpdateData(true);

	// Check index validity
	if ((index < 0) || (index > GetItemNum()))
		return;

	// Get item at given index
	Item pwrTemp = m_pwrReminderDataTemp.getItemAt(index);

	// Update data
	UpdateItemData(pwrTemp, true);

	// Check data validity
	bool bValid = Validate(pwrTemp, true);
	if (bValid == false)
		return;

	// Update item to Power Reminder data item list
	m_pwrReminderDataTemp.update(pwrTemp);

	// Update table
	RedrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Remove a reminder item by index
 * @param	index - Index of item to remove
 * @return	None
 */
void CPwrReminderDlg::Remove(int index)
{
	// Remove item at index
	m_pwrReminderDataTemp.deleteItem(index);

	// Update table
	RedrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Remove all reminder items
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::RemoveAll()
{
	// Remove all items
	m_pwrReminderDataTemp.deleteAll();

	// Update table
	RedrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Check/uncheck all Power Reminder items
 * @param	state - Item state
 * @return	None
 */
void CPwrReminderDlg::SetAllItemState(bool state)
{
	// Check/uncheck all --> Update all items enable state
	int itemNum = GetItemNum();
	for (int index = 0; index < itemNum; index++) {
		Item& pwrTemp = m_pwrReminderDataTemp.getItemAt(index);
		if (pwrTemp.isEnabled() != state) {
			pwrTemp.enableItem(state);
		}
	}

	// Update number of checked items
	checkCount_ = (state == false) ? 0 : itemNum;
	
	// Update data item list
	UpdateDataItemList();

	// Refresh button state
	refreshDialogItemState(false);
}

/**
 * @brief	Preview Power Reminder item
 * @param	index - Item index
 * @return	None
 */
void CPwrReminderDlg::PreviewItem(int index)
{
	// Check index validity
	if ((index < 0) || (index >= GetItemNum()))
		return;

	// Get item
	const Item& displayItem = m_pwrReminderDataTemp.getItemAt(index);

	// Check message content validity
	String messageContent = displayItem.getMessage();
	if ((messageContent.isEmpty()) ||
		(IS_NULL_STRING(messageContent))) {
		// Invalid message content
		return;
	}

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Style: MessageBox
	if (displayItem.getMessageStyle() == Style::messageBox) {
		// Display message box
		const wchar_t* messageCaption = getLanguageString(languageTablePtr, IDC_PWRREMINDER_PREVIEW_BTN);
		DWORD messageStyle = MB_OK | MB_ICONINFORMATION;
		displayMessageBox(messageContent, messageCaption, messageStyle);
	}
	// Style: Dialog
	else if (displayItem.getMessageStyle() == Style::dialogBox) {
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
			RmdMsgStyleSet rmdMessageStyle = m_pwrReminderDataTemp.getCommonStyle();
			if (displayItem.isCustomStyleEnabled())
				rmdMessageStyle = displayItem.getMessageStyleData();

			// Default timeout for previewing
			int nDefTimeout = PwrReminderData::previewTimeout;

			// Set properties
			m_pRmdPreviewMsgDlg->setCaptionFromLanguage(IDC_PWRREMINDER_PREVIEW_BTN);
			m_pRmdPreviewMsgDlg->SetDispMessage(messageContent);
			m_pRmdPreviewMsgDlg->SetMessageStyle(rmdMessageStyle);
			m_pRmdPreviewMsgDlg->SetAutoCloseInterval(nDefTimeout);

			// Set notify state flags
			m_pRmdPreviewMsgDlg->setTopMost(false);
			m_pRmdPreviewMsgDlg->setInitSound(true);

			// Display message
			m_pRmdPreviewMsgDlg->DoModal();
			
			delete m_pRmdPreviewMsgDlg;
			m_pRmdPreviewMsgDlg = NULL;
		}
	}
}

/**
 * @brief	Update reminder data from/to dialog controls
 * @param	reminderItem - Power Reminder item
 * @param	updateFlag - Update data flag
 * @return	None
 */
void CPwrReminderDlg::UpdateItemData(Item& reminderItem, bool updateFlag)
{
	if (updateFlag == true) {

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
			reminderItem.setMessage(tempString);
		}

		/*----------------------------Event----------------------------*/

		bool isTemp = false;

		// Event: At set time
		if (m_pEvtSetTimeRad != NULL) {
			isTemp = m_pEvtSetTimeRad->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::atSetTime);
				if (m_pEvtSetTimeEdit != NULL) {
					ClockTime clockTimeTemp;
					UpdateTimeSetting(clockTimeTemp, true);
					reminderItem.setTime(clockTimeTemp);
				}
			}
		}
		// Update data for RepeatSet dialog
		if (m_pRepeatSetDlg != NULL) {
			m_pRepeatSetDlg->updateDialogData(reminderItem, true);
		}
		// Event: At app startup
		if (m_pEvtAppStartupRad != NULL) {
			isTemp = m_pEvtAppStartupRad->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::atAppStartup);
			}
		}
		// Event: At system wake
		if (m_pEvtSysWakeupRad != NULL) {
			isTemp = m_pEvtSysWakeupRad->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::atSysWakeUp);
			}
		}
		// Event: Before power action
		if (m_pEvtBfrPwrActionRad != NULL) {
			isTemp = m_pEvtBfrPwrActionRad->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::beforePwrAction);
			}
		}
		// Event: Wake after action
		if (m_pEvtPwrActionWakeRad != NULL) {
			isTemp = m_pEvtPwrActionWakeRad->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::wakeAfterAction);
			}
		}
		// Event: Before app exit
		if (m_pEvtAtAppExitRad != NULL) {
			isTemp = m_pEvtAtAppExitRad->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::atAppExit);
			}
		}

		/*------------------------Message style------------------------*/

		// Style: MessageBox
		if (m_pStyleMsgBoxRad != NULL) {
			isTemp = m_pStyleMsgBoxRad->GetCheck();
			if (isTemp == true) {
				reminderItem.setMessageStyle(Style::messageBox);
			}
		}
		// Style: Dialog Box
		if (m_pStyleDialogBoxRad != NULL) {
			isTemp = m_pStyleDialogBoxRad->GetCheck();
			if (isTemp == true) {
				reminderItem.setMessageStyle(Style::dialogBox);
			}
		}
		// Style Configuration: Use common style
		if (m_pStyleUseCommonRad != NULL) {
			isTemp = m_pStyleUseCommonRad->GetCheck();
			if (isTemp == true) {
				reminderItem.enableCustomStyle(false);
			}
		}
		// Style Configuration: Use common style
		if (m_pStyleUseCustomRad != NULL) {
			isTemp = m_pStyleUseCustomRad->GetCheck();
			if (isTemp == true) {
				reminderItem.enableCustomStyle(true);
			}
		}
		// Update data for Customize dialog
		// Note: Common style data is updated in OnCustomizeStyle()
		if (m_pMsgStyleSetDlg != NULL &&
			m_pMsgStyleSetDlg->GetDataFlag() == MsgStyleSetFlag::customStyle) {
			m_pMsgStyleSetDlg->GetData(reminderItem.getMessageStyleData());
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

		bool isEnabled = true;
		int mode = GetCurMode();
		if ((mode == Mode::Init) || (mode == Mode::View)) {
			// Disable items
			isEnabled = false;
		}
		else if ((mode == Mode::Add) || (mode == Mode::Update)) {
			// Enable items
			isEnabled = true;
		}
		else if (mode == Mode::Disable) {
			// Disable items
			isEnabled = false;
		}

		/*----------------------Get item details-----------------------*/

		String messageContent = reminderItem.getMessage();
		ClockTime itemTime = reminderItem.getTime();
		unsigned eventId = reminderItem.getEventId();
		DWORD messageStyle = reminderItem.getMessageStyle();
		bool bUseCustomStyle = reminderItem.isCustomStyleEnabled();

		/*-----------------------Message content-----------------------*/

		CWnd* windowPtr = NULL;
		windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_TITLE);
		if (windowPtr != NULL) {
			windowPtr->EnableWindow(isEnabled);
		}
		if (m_pMsgStringEdit != NULL) {
			m_pMsgStringEdit->EnableWindow(isEnabled);
			m_pMsgStringEdit->SetWindowText(messageContent);
		}
		windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_COUNTER);
		if (windowPtr != NULL) {
			windowPtr->EnableWindow(isEnabled);
			UpdateMsgCounter(messageContent.getLength());
		}

		/*----------------------------Event----------------------------*/

		bool isTemp = false;
		windowPtr = GetDlgItem(IDC_PWRREMINDER_EVENT_TITLE);
		if (windowPtr != NULL) {
			windowPtr->EnableWindow(isEnabled);
		}

		// Event: At set time
		if (m_pEvtSetTimeRad != NULL) {
			m_pEvtSetTimeRad->EnableWindow(isEnabled);
			isTemp = (eventId == Event::atSetTime);
			m_pEvtSetTimeRad->SetCheck(isTemp);

			// Set time edit and spin value
			isTemp &= isEnabled;
			if (m_pEvtSetTimeEdit != NULL) {
				m_pEvtSetTimeEdit->EnableWindow(isTemp);
				UpdateTimeSetting(itemTime, false);
			}
			if (m_pEvtSetTimeSpin != NULL) {
				m_pEvtSetTimeSpin->EnableWindow(isTemp);
			}
		}
		// Update data for RepeatSet dialog
		if (m_pRepeatSetDlg != NULL) {
			m_pRepeatSetDlg->updateDialogData(reminderItem, false);
		}
		// Event: At app startup
		if (m_pEvtAppStartupRad != NULL) {
			m_pEvtAppStartupRad->EnableWindow(isEnabled);
			isTemp = (eventId == Event::atAppStartup);
			m_pEvtAppStartupRad->SetCheck(isTemp);
		}
		// Event: At system wake
		if (m_pEvtSysWakeupRad != NULL) {
			m_pEvtSysWakeupRad->EnableWindow(isEnabled);
			isTemp = (eventId == Event::atSysWakeUp);
			m_pEvtSysWakeupRad->SetCheck(isTemp);
		}
		// Event: Before power action
		if (m_pEvtBfrPwrActionRad != NULL) {
			m_pEvtBfrPwrActionRad->EnableWindow(isEnabled);
			isTemp = (eventId == Event::beforePwrAction);
			m_pEvtBfrPwrActionRad->SetCheck(isTemp);
		}
		// Event: Wake after action
		if (m_pEvtPwrActionWakeRad != NULL) {
			m_pEvtPwrActionWakeRad->EnableWindow(isEnabled);
			isTemp = (eventId == Event::wakeAfterAction);
			m_pEvtPwrActionWakeRad->SetCheck(isTemp);
		}
		// Event: Before app exit
		if (m_pEvtAtAppExitRad != NULL) {
			m_pEvtAtAppExitRad->EnableWindow(isEnabled);
			isTemp = (eventId == Event::atAppExit);
			m_pEvtAtAppExitRad->SetCheck(isTemp);
		}

		/*------------------------Message style------------------------*/

		windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_TITLE);
		if (windowPtr != NULL) {
			windowPtr->EnableWindow(isEnabled);
		}
		// Message Box style
		if (m_pStyleMsgBoxRad != NULL) {
			m_pStyleMsgBoxRad->EnableWindow(isEnabled);
			isTemp = (messageStyle == Style::messageBox);
			m_pStyleMsgBoxRad->SetCheck(isTemp);
		}
		// Dialog Box style
		if (m_pStyleDialogBoxRad != NULL) {
			m_pStyleDialogBoxRad->EnableWindow(isEnabled);
			isTemp = (messageStyle == Style::dialogBox);
			m_pStyleDialogBoxRad->SetCheck(isTemp);
		}

		// Style customization
		windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZATION_TITLE);
		if (windowPtr != NULL) {
			windowPtr->EnableWindow(isEnabled);
		}
		// Use common style configuration
		if (m_pStyleUseCommonRad != NULL) {
			m_pStyleUseCommonRad->EnableWindow(isEnabled);
			m_pStyleUseCommonRad->SetCheck(!bUseCustomStyle);
		}
		// Use custom style configuration
		if (m_pStyleUseCustomRad != NULL) {
			m_pStyleUseCustomRad->EnableWindow(isEnabled);
			m_pStyleUseCustomRad->SetCheck(bUseCustomStyle);
		}
		// [Customize] button
		if (m_pStyleCustomizeBtn != NULL) {
			m_pStyleCustomizeBtn->EnableWindow(isEnabled);
		}
		// Update data for Customize dialog
		if (m_pMsgStyleSetDlg && IsWindow(m_pMsgStyleSetDlg->GetSafeHwnd())) {
			if (!bUseCustomStyle) {
				m_pMsgStyleSetDlg->SetData(m_pwrReminderDataTemp.getCommonStyle());
				m_pMsgStyleSetDlg->SetDataFlag(MsgStyleSetFlag::commonStyle);
			}
			else {
				m_pMsgStyleSetDlg->SetData(reminderItem.getMessageStyleData());
				m_pMsgStyleSetDlg->SetDataFlag(MsgStyleSetFlag::customStyle);
			}
			m_pMsgStyleSetDlg->updateDialogData(false);
		}

		/*-------------------------------------------------------------*/
	}
}

/**
 * @brief	Check Power Reminder item validity
 * @param	reminderItem		 - Item to validate
 * @param	showMsg	 - Show validation message box or not
 * @param	bAutoCorrect - Invalid value auto correction (ON/OFF)
 * @return	bool - Result of validation process
 */
bool CPwrReminderDlg::Validate(Item& reminderItem, bool showMsg /* = false */, bool bAutoCorrect /* = false */)
{
	bool result = true;

	int messageStringId;
	StringArray arrMsgString;
	arrMsgString.clear();

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Check item ID
	if ((reminderItem.getItemId() < PwrReminderData::minItemID) || (reminderItem.getItemId() > PwrReminderData::maxItemID)) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_ITEMID;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Get next item ID
			reminderItem.setItemId(m_pwrReminderDataTemp.getNextId());
		}
	}

	// Check message content
	String messageContent = reminderItem.getMessage();
	if (messageContent.isEmpty()) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_EMPTY;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (bAutoCorrect == true) {
			reminderItem.setMessage(getLanguageString(languageTablePtr, PWRRMD_MSGCONTENT_SAMPLE));
			if (IS_NOT_NULL_STRING(reminderItem.getMessage())) {
				// Re-format sample message
				String messageFormat = reminderItem.getMessage();
				messageContent.format(messageFormat, reminderItem.getItemId());
				reminderItem.setMessage(messageContent);
			}
		}
	}
	else if (messageContent.getLength() > Constant::Max::StringLength) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_OUTOFLIMIT;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Only get character numbers in range
			String tempString = messageContent.left(Constant::Max::StringLength);
			reminderItem.setMessage(tempString);
		}
	}

	// Check event ID
	if ((reminderItem.getEventId() < Event::atSetTime) || (reminderItem.getEventId() > Event::atAppExit)) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_EVENTID;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Set default event ID
			reminderItem.setEventId(Event::atSetTime);
		}
	}

	// Check snooze interval data
	if ((reminderItem.getSnoozeInterval() < PwrRepeatSet::minSnoozeInterval) || (reminderItem.getSnoozeInterval() > PwrRepeatSet::maxSnoozeInterval)) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_SNOOZEINTERVAL;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Set default snooze interval
			reminderItem.setSnoozeInterval(PwrRepeatSet::defaultSnoozeInterval);
		}
	}

	// Check repeat set data
	if ((reminderItem.isRepeatEnabled() == true) && (reminderItem.getActiveDays() == NULL)) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_ACTIVEDAYS;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Set default data
			reminderItem.setActiveDays(PwrRepeatSet::defaultActiveDays);
		}
	}

	// Check style ID
	if ((reminderItem.getMessageStyle() < Style::messageBox) || (reminderItem.getMessageStyle() > Style::dialogBox)) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_STYLEID;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Set default style ID
			reminderItem.setMessageStyle(Style::messageBox);
		}
	}
	
	// Show error message if enabled
	if ((showMsg == true) && (!arrMsgString.empty())) {
		for (int index = 0; index < arrMsgString.size(); index++) {
			// If auto correction is ON
			if (bAutoCorrect == true) {
				// Add "Data will be automatically reset to default"
				String errorMessage = arrMsgString.at(index);
				errorMessage += getLanguageString(languageTablePtr, MSGBOX_PWRREMINDER_INVALIDITEM_AUTOCORRECT);
				displayMessageBox(errorMessage, NULL, MB_OK | MB_ICONERROR);
			}
			else {
				// Display error message
				displayMessageBox(arrMsgString.at(index), NULL, MB_OK | MB_ICONERROR);
			}
		}
	}

	// Remove all message after displaying
	arrMsgString.clear();

	return result;
}

/**
 * @brief	Set current mode
 * @param	mode - Mode to set
 * @return	None
 */
void CPwrReminderDlg::SetCurMode(int mode)
{
	// If new mode is the same, do nothing
	if (mode == GetCurMode()) return;

	// Set new mode
	m_nCurMode = mode;
	
	// Switch mode
	SwitchMode(true);
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
		int cx = Constant::UI::Button::IconWidth;
		int cy = Constant::UI::Button::IconHeight;
		HINSTANCE instanceHandle = AfxGetApp()->m_hInstance;
		HICON hRepeatIcon = (HICON)LoadImage(instanceHandle, MAKEINTRESOURCE(IDI_ICON_REPEAT), IMAGE_ICON, cx, cy, LR_DEFAULTCOLOR);
		if (hRepeatIcon == NULL)
			return;

		// Set button icon
		m_pEvtRepeatSetBtn->ModifyStyle(NULL, BS_ICON);
		m_pEvtRepeatSetBtn->SetIcon(hRepeatIcon);
	}
}
