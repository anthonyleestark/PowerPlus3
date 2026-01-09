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
constexpr const int kFixedColumnNum = 1;
constexpr const int kFixedRowNum = 1;


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
	dataListTablePtr_ = NULL;
	msgStringEditPtr_ = NULL;
	evtSetTimeRadPtr_ = NULL;
	evtSetTimeEditPtr_ = NULL;
	evtSetTimeSpinPtr_ = NULL;
	evtRepeatSetBtnPtr_ = NULL;
	evtAppStartupRadPtr_ = NULL;
	evtSysWakeupRadPtr_ = NULL;
	evtBfrPwrActionRadPtr_ = NULL;
	evtPwrActionWakeRadPtr_ = NULL;
	evtAtAppExitRadPtr_ = NULL;
	styleMsgBoxRadPtr_ = NULL;
	styleDialogBoxRadPtr_ = NULL;
	msgStyleComboPtr_ = NULL;
	styleUseCommonRadPtr_ = NULL;
	styleUseCustomRadPtr_ = NULL;
	styleCustomizeBtnPtr_ = NULL;

	// Properties child dialogs
	previewMsgDlgPtr_ = NULL;
	repeatSetDlgPtr_ = NULL;
	msgStyleSetDlgPtr_ = NULL;

	// Checkbox/radio button variables
	isEventtSetTimeEnabled_ = false;
	isEventAppStartupEnabled_ = false;
	isEventSysWakeupEnabled_ = false;
	isEventBfrPwrActionEnabled_ = false;
	isEventPwrActionWakeEnabled_ = false;
	isEventAppExitEnabled_ = false;
	isStyleMsgBoxEnabled_ = false;
	isStyleDialogEnabled_ = false;
	isStyleUseCommonEnabled_ = false;
	isStyleUseCustomEnabled_ = false;

	// Table format and properties
	columnCount_ = 0;
	tableFrameSizePtr_ = NULL;
	gridCtrlFormatInfoPtr_ = NULL;

	// Other variables
	currentMode_ = 0;
	checkCount_ = 0;
	curSelIndex_ = -1;
	curDispIndex_ = -2;
	displayedTimeBackup_ = ClockTime();
}

/**
 * @brief	Destructor
 */
CPwrReminderDlg::~CPwrReminderDlg()
{
	// Delete child dialogs
	if (previewMsgDlgPtr_ != NULL) {
		// Destroy dialog
		previewMsgDlgPtr_->DestroyWindow();
		delete previewMsgDlgPtr_;
		previewMsgDlgPtr_ = NULL;
	}
	if (repeatSetDlgPtr_ != NULL) {
		// Destroy dialog
		repeatSetDlgPtr_->DestroyWindow();
		delete repeatSetDlgPtr_;
		repeatSetDlgPtr_ = NULL;
	}
	if (msgStyleSetDlgPtr_ != NULL) {
		// Destroy dialog
		msgStyleSetDlgPtr_->DestroyWindow();
		delete msgStyleSetDlgPtr_;
		msgStyleSetDlgPtr_ = NULL;
	}

	// Data item list control
	if (dataListTablePtr_) {
		delete dataListTablePtr_;
		dataListTablePtr_ = NULL;
	}
	
	// Remove Reminder data
	reminderData_.deleteAll();
	tempReminderData_.deleteAll();

	// Table format and properties
	if (tableFrameSizePtr_ != NULL) {
		delete tableFrameSizePtr_;
		tableFrameSizePtr_ = NULL;
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
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_SETTIME_RADBTN,		isEventtSetTimeEnabled_);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN,		isEventAppStartupEnabled_);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN,		isEventSysWakeupEnabled_);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN,	isEventBfrPwrActionEnabled_);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN,	isEventPwrActionWakeEnabled_);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN,		isEventAppExitEnabled_);
	DDX_Check(pDX, IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN,		isStyleMsgBoxEnabled_);
	DDX_Check(pDX, IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN,		isStyleDialogEnabled_);
	DDX_Check(pDX, IDC_PWRREMINDER_MSGSTYLE_USECOMMON_RADBTN,	isStyleUseCommonEnabled_);
	DDX_Check(pDX, IDC_PWRREMINDER_MSGSTYLE_USECUSTOM_RADBTN,	isStyleUseCustomEnabled_);
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
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_ITEM_LISTBOX, List_Control);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_ADD_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EDIT_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_REMOVE_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_REMOVEALL_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_CHECKALL_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_UNCHECKALL_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_PREVIEW_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_APPLY_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_CANCEL_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_DETAIL_STATIC, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_MSGSTRING_TITLE, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_MSGSTRING_EDITBOX, Edit_Control);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_MSGSTRING_COUNTER, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EVENT_TITLE, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN, Radio_Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX, Edit_Control);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EVENT_SETTIME_SPIN, Spin_Control);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EVENT_REPEATSET_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN, Radio_Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN, Radio_Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN, Radio_Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN, Radio_Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN, Radio_Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_MSGSTYLE_TITLE, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN, Radio_Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN, Radio_Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZATION_TITLE, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_MSGSTYLE_USECOMMON_RADBTN, Radio_Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_MSGSTYLE_USECUSTOM_RADBTN, Radio_Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZE_BTN, Button);
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
		pCtrlMan->removeControl(IDC_PWRREMINDER_ITEM_LISTBOX);
		pCtrlMan->removeControl(IDC_PWRREMINDER_ADD_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EDIT_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_REMOVE_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_REMOVEALL_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_CHECKALL_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_UNCHECKALL_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_PREVIEW_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_APPLY_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_CANCEL_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_DETAIL_STATIC);
		pCtrlMan->removeControl(IDC_PWRREMINDER_MSGSTRING_TITLE);
		pCtrlMan->removeControl(IDC_PWRREMINDER_MSGSTRING_EDITBOX);
		pCtrlMan->removeControl(IDC_PWRREMINDER_MSGSTRING_COUNTER);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EVENT_TITLE);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EVENT_SETTIME_SPIN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EVENT_REPEATSET_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_MSGSTYLE_TITLE);
		pCtrlMan->removeControl(IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZATION_TITLE);
		pCtrlMan->removeControl(IDC_PWRREMINDER_MSGSTYLE_USECOMMON_RADBTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_MSGSTYLE_USECUSTOM_RADBTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZE_BTN);
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
	loadPwrReminderData();

	// Init dialog items
	loadLayoutInfo();
	setupLanguage();
	setupDialogItemState();

	// Update data
	updateDataItemList();
	displayItemDetails(Constant::InvalidInteger);
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
		int currentMode = getCurMode();
		if ((currentMode == Mode::Add) || (currentMode == Mode::Update)) {
			// Show switch mode confirmation message
			confirm = displayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, NULL, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {
				// Switch mode
				setCurMode(Mode::View);
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
				savePwrReminderData();
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
		savePwrReminderData();
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
		int currentMode = getCurMode();
		if ((currentMode & Mode::Add) || (currentMode & Mode::Update)) {
			// Show switch mode confirmation message
			confirm = displayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, NULL, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {
				// Switch mode
				setCurMode(Mode::View);
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
				savePwrReminderData();
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
	int currentMode = getCurMode();

	// Current mode: Add
	if (currentMode & Mode::Add) {
		// Add item
		add();

		// Reset mode
		setCurMode(Mode::View);
	}
	// Current mode: Edit
	else if (currentMode & Mode::Update) {
		// Do nothing
	}
	else {
		// Mark an empty item as in editting
		itemInEdit_ = PwrReminderItem();

		// Switch mode
		setCurMode(Mode::Add);
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
	int currentMode = getCurMode();

	// Mode: Edit
	if (currentMode & Mode::Update) {
		// Check if any item is selected or not
		bool isSelected = ((curSelIndex_ >= 0) && (curSelIndex_ < getItemNum()));

		if (isSelected == true) {
			// Edit current selected item
			edit(curSelIndex_);
		}

		// Reset mode
		setCurMode(Mode::View);
	}
	// Mode: Add
	else if (currentMode & Mode::Add) {
		// Do nothing
	}
	else {
		// Mark current selected item as in editting
		bool isSelected = ((curSelIndex_ >= 0) && (curSelIndex_ < getItemNum()));
		if (isSelected == true)
			itemInEdit_.copy(tempReminderData_.getItemAt(curSelIndex_));

		// Switch mode
		setCurMode(Mode::Update);
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
	int itemNum = getItemNum();
	if (itemNum <= 0)
		return;

	// Get current select item index
	int index = curSelIndex_;

	// If item at selected index is empy, do nothing
	if (tempReminderData_.isEmpty(index) == true)
		return;

	// Ask before remove
	int confirm = displayMessageBox(MSGBOX_PWRREMINDER_REMOVE_ITEM, NULL, MB_YESNO | MB_ICONQUESTION);
	if (confirm == IDYES) {
		// Remove item
		remove(index);
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
	if (tempReminderData_.isAllEmpty() == true)
		return;

	// Ask before remove
	int confirm = displayMessageBox(MSGBOX_PWRREMINDER_REMOVEALL_ITEMS, NULL, MB_YESNO | MB_ICONQUESTION);
	if (confirm == IDYES) {
		// Remove all items
		removeAll();
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
	if (tempReminderData_.isAllEmpty() == true)
		return;

	// Check all items
	setAllItemState(true);
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
	if (tempReminderData_.isAllEmpty() == true)
		return;

	// Uncheck all items
	setAllItemState(false);
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
	if (tempReminderData_.isAllEmpty() == true)
		return;

	// Get current selection index
	int currenSelection = curSelIndex_;
	if (tempReminderData_.isEmpty(currenSelection) == true)
		return;

	// Preview item
	int index = currenSelection;
	previewItem(index);
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
	curSelIndex_ = row - kFixedRowNum;
	int itemCount = getItemNum();

	*pResult = NULL;

	// Invalid selection
	if ((curSelIndex_ < 0) || (curSelIndex_ >= itemCount))
		return;

	// Check read-only mode
	int currentMode = getCurMode();
	if ((currentMode != Mode::Init) && (currentMode != Mode::View))
		return;

	// Display item details
	displayItemDetails(curSelIndex_);
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
	int clickedRow = reminderItem->iRow;

	// Check value validity
	int itemNum = getItemNum();
	if ((clickedRow <= Constant::UI::GridCtrl::Index::Header_Row) || (clickedRow > itemNum)) {
		return;
	}

	// Check read-only mode
	int currentMode = getCurMode();
	if ((currentMode != Mode::Init) && (currentMode != Mode::View))
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
	int clickedRow = reminderItem->iRow;

	// Check value validity
	int itemNum = getItemNum();
	if ((clickedRow <= Constant::UI::GridCtrl::Index::Header_Row) || (clickedRow > itemNum)) {
		return;
	}

	// Check read-only mode
	int currentMode = getCurMode();
	if ((currentMode != Mode::Init) && (currentMode != Mode::View))
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
	if (msgStringEditPtr_ == NULL) {
		TRACE_ERROR("Error: Message content edit control not found!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Update data
	const int buffLength = msgStringEditPtr_->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(buffLength + 1);
	msgStringEditPtr_->GetWindowText(tempBuff.data(), buffLength + 1);
	String textValue = tempBuff.data();

	// Update message counter
	int count = textValue.getLength();
	updateMsgCounter(count);
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
	if (evtSetTimeEditPtr_ == NULL) {
		evtSetTimeEditPtr_ = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (evtSetTimeEditPtr_ == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Backup current displaying time value
	updateTimeSetting(displayedTimeBackup_, true);

	// Select all text
	evtSetTimeEditPtr_->PostMessage(EM_SETSEL, 0, -1);
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
	if (evtSetTimeEditPtr_ == NULL) {
		evtSetTimeEditPtr_ = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (evtSetTimeEditPtr_ == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Update data
	const int buffLength = evtSetTimeEditPtr_->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(buffLength + 1);
	evtSetTimeEditPtr_->GetWindowText(tempBuff.data(), buffLength + 1);
	String timeTextValue = tempBuff.data();

	ClockTime clockTime;
	if (ClockTimeUtils::inputText2Time(clockTime, timeTextValue)) {

		// Update new time value
		updateTimeSetting(clockTime, false);

		// Update timespin new position
		int timeSpinPos = 0;
		ClockTimeUtils::time2SpinPos(clockTime, timeSpinPos);
		if (evtSetTimeSpinPtr_ != NULL) {
			evtSetTimeSpinPtr_->SetPos(timeSpinPos);
		}
	}
	else {
		// Restore backed-up time value
		updateTimeSetting(displayedTimeBackup_, false);
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
	updateTimeSetting(clockTime, false);

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
	if (evtSetTimeRadPtr_ == NULL) {
		evtSetTimeRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN);
		if (evtSetTimeRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventSetTimeRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// If button is disabled, do nothing
	if (evtSetTimeRadPtr_->IsWindowEnabled() == false)
		return;

	// Update checked state
	int state = evtSetTimeRadPtr_->GetCheck();

	// Check control validity
	if (evtSetTimeEditPtr_ == NULL) {
		evtSetTimeEditPtr_ = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (evtSetTimeEditPtr_ == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (evtSetTimeSpinPtr_ == NULL) {
		evtSetTimeSpinPtr_ = (CSpinButtonCtrl*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_SPIN);
		if (evtSetTimeSpinPtr_ == NULL) {
			TRACE_ERROR("Error: Time spin control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Enable/disable time spinedit and [RepeatSet] button
	if (state == 1) {
		evtSetTimeEditPtr_->EnableWindow(true);
		evtSetTimeSpinPtr_->EnableWindow(true);
		evtRepeatSetBtnPtr_->EnableWindow(true);
	}
	else {
		evtSetTimeEditPtr_->EnableWindow(false);
		evtSetTimeSpinPtr_->EnableWindow(false);
		evtRepeatSetBtnPtr_->EnableWindow(false);
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
	if (repeatSetDlgPtr_ == NULL) {
		repeatSetDlgPtr_ = new CRmdRepeatSetDlg;
		repeatSetDlgPtr_->Create(IDD_RMDREPEATSET_DLG);
		repeatSetDlgPtr_->removeStyle(DS_MODALFRAME | WS_CAPTION);
		repeatSetDlgPtr_->setParent(this);
	}

	// Get button position
	RECT buttonRect{};
	if (evtRepeatSetBtnPtr_ != NULL) {
		// Get button rectangle
		evtRepeatSetBtnPtr_->GetWindowRect(&buttonRect);
	}

	// If the dialog has already been initialized
	if (repeatSetDlgPtr_ != NULL) {
		// If the dialog is currently displaying
		if (repeatSetDlgPtr_->IsWindowVisible()) {
			// Hide the dialog
			repeatSetDlgPtr_->ShowWindow(SW_HIDE);
		}
		else {
			// Set dialog alignment
			unsigned alignment = SDA_LEFTALIGN | SDA_TOPALIGN;
			repeatSetDlgPtr_->setAlignment(alignment);

			// Get button top-right point
			Point topRightPoint(buttonRect.right, buttonRect.top);

			// Set dialog anchor point
			repeatSetDlgPtr_->setAnchorPoint(topRightPoint);

			// Show dialog
			repeatSetDlgPtr_->ShowWindow(SW_SHOW);
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
	if (!msgStyleSetDlgPtr_ || !IsWindow(msgStyleSetDlgPtr_->GetSafeHwnd())) {
		delete msgStyleSetDlgPtr_;
		msgStyleSetDlgPtr_ = new CRmdMsgStyleSetDlg;
		msgStyleSetDlgPtr_->setParent(this);
	}

	// Preparation
	RmdMsgStyleSet styleSetData;
	MsgStyleSetFlag styleSetDataFlag;

	// Which style configuration data???
	if (styleUseCommonRadPtr_ && styleUseCommonRadPtr_->GetCheck() == true) {
		styleSetData = tempReminderData_.getCommonStyle();
		styleSetDataFlag = MsgStyleSetFlag::commonStyle;
	}
	else if (styleUseCustomRadPtr_ && styleUseCustomRadPtr_->GetCheck() == true) {
		styleSetData = itemInEdit_.getMessageStyleData();
		styleSetDataFlag = MsgStyleSetFlag::customStyle;
	}
	else return;

	// Set dialog data
	msgStyleSetDlgPtr_->setData(styleSetData);
	msgStyleSetDlgPtr_->setDataFlag(styleSetDataFlag);

	// Show the dialog in modal state
	msgStyleSetDlgPtr_->DoModal();

	// Update common style data
	if (msgStyleSetDlgPtr_->getReturnFlag() == ReturnFlag::OK && styleSetDataFlag == MsgStyleSetFlag::commonStyle) {
		msgStyleSetDlgPtr_->getData(styleSetData);
		tempReminderData_.getCommonStyle().copy(styleSetData);
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
	if (previewMsgDlgPtr_ != NULL) {
		// Request close message dialog
		LRESULT resClosePreview = previewMsgDlgPtr_->requestCloseDialog();
		if (resClosePreview != Result::Success) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}

	// If RepeatSet edit dialog is opening
	if (repeatSetDlgPtr_ != NULL) {
		// Request close dialog
		LRESULT resCloseRepeatSet = repeatSetDlgPtr_->requestCloseDialog();
		if (resCloseRepeatSet != Result::Success)
			return resCloseRepeatSet;
	}

	// Exit current mode
	int confirm = -1;
	int currentMode = getCurMode();
	if ((currentMode == Mode::Add) || (currentMode == Mode::Update)) {
		confirm = displayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (confirm == IDYES) {
			// Switch mode
			setCurMode(Mode::View);
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
			savePwrReminderData();
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

		if (evtSetTimeEditPtr_ == NULL)
			return 0;

		// Get the editbox rect
		RECT editBoxRect;
		evtSetTimeEditPtr_->GetWindowRect(&editBoxRect);
		ScreenToClient(&editBoxRect);

		// If clicked point is inside the editbox area
		if (((pt.x > editBoxRect.left) && (pt.x < editBoxRect.right)) &&
			((pt.y > editBoxRect.top) && (pt.y < editBoxRect.bottom))) {
			if (evtSetTimeEditPtr_->IsWindowEnabled()) {
				// Select all text
				evtSetTimeEditPtr_->SetSel(0, -1, true);
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
			drawRepeatSetButton();
			break;

		default:
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Setup data item list
	setupDataItemList(languageTablePtr);

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Initialize and setup language for Power Reminder data item list
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CPwrReminderDlg::setupDataItemList(LANGTABLE_PTR languageTablePtr)
{
	// Get parent list frame rect
	CWnd* listFrameWndPtr = GetDlgItem(IDC_PWRREMINDER_ITEM_LISTBOX);
	if (listFrameWndPtr == NULL) return;
	RECT listFrameWndRect;
	listFrameWndPtr->GetWindowRect(&listFrameWndRect);
	ScreenToClient(&listFrameWndRect);

	// Get frame size
	if (tableFrameSizePtr_ == NULL) {
		tableFrameSizePtr_ = new Size();
		tableFrameSizePtr_->_width = listFrameWndRect.right - listFrameWndRect.left;
		tableFrameSizePtr_->_height = listFrameWndRect.bottom - listFrameWndRect.top;
	}

	// Initialization
	VERIFY_INITIALIZATION(dataListTablePtr_, CGridCtrl)

	// Create table
	if (dataListTablePtr_ == NULL) return;
	DWORD style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	dataListTablePtr_->Create(listFrameWndRect, this, IDC_PWRREMINDER_ITEM_LISTBOX, style);

	// Destroy frame
	listFrameWndPtr->DestroyWindow();

	// Cell format
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)dataListTablePtr_->GetDefaultCell(false, false);
	if (cellPtr == NULL) return;
	cellPtr->SetFormat(cellPtr->GetFormat());
	cellPtr->SetMargin(0);
	cellPtr->SetBackClr(Color::White);
	cellPtr->SetTextClr(Color::Black);
	cellPtr->SetHeight(Constant::UI::GridCtrl::Height::Row);

	// Table format and properties
	int rowCount = (getItemNum() + kFixedRowNum);
	int colCount = columnCount_;

	// Setup table
	dataListTablePtr_->SetColumnCount(colCount);
	dataListTablePtr_->SetFixedColumnCount(kFixedColumnNum);
	dataListTablePtr_->SetRowCount(rowCount);
	dataListTablePtr_->SetFixedRowCount(kFixedColumnNum);
	dataListTablePtr_->SetRowHeight(Constant::UI::GridCtrl::Index::Header_Row, Constant::UI::GridCtrl::Height::Header);

	// Draw table
	drawDataTable(tableFrameSizePtr_, colCount, rowCount, false, languageTablePtr);

	// Update layout info
	updateLayoutInfo();

	// Display table
	dataListTablePtr_->SetListMode(true);
	dataListTablePtr_->SetEditable(false);
	dataListTablePtr_->SetRowResize(false);
	dataListTablePtr_->EnableSelection(true);
	dataListTablePtr_->SetSingleRowSelection(true);
	dataListTablePtr_->SetSingleColSelection(false);
	dataListTablePtr_->SetFixedRowSelection(false);
	dataListTablePtr_->SetFixedColumnSelection(false);
	dataListTablePtr_->ShowWindow(SW_SHOW);
	dataListTablePtr_->SetRedraw(true);
}

/**
 * @brief	Draw data list table
 * @param	szFrameWndSize	- Frame size
 * @param	colCount			- Number of table columns
 * @param	rowCount			- Number of table rows
 * @param	isReadOnly		- Read-only mode
 * @param	languageTablePtr		- Pointer to app language
 * @return	None
 */
void CPwrReminderDlg::drawDataTable(Size* tableFrameSizePtr, int colCount, int rowCount, bool isReadOnly /* = false */, LANGTABLE_PTR languageTablePtr /* = NULL */)
{
	// Check table validity
	if (dataListTablePtr_ == NULL)
		return;

	// Check table format data validity
	if (tableFrameSizePtr == NULL) return;
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Check row and column number validity
	if ((colCount <= 0) || (rowCount < kFixedRowNum))
		return;

	// Get app pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) return;

	// Load app language package
	if (languageTablePtr == NULL) {
		languageTablePtr = theAppPtr->getAppLanguage();
	}

	// Re-update default cell properties
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)dataListTablePtr_->GetDefaultCell(false, false);
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
	int frameHeight = tableFrameSizePtr->height();
	int frameWidth = tableFrameSizePtr->width();
	if (AppCore::getWindowsOSVersion() == WindowsOS::Version::Win10) {
		// Windows 10 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl_Win10;
		//frameHeight -= OFFSET_HEIGHT_LISTCTRL_WIN10;
	}
	else {
		// Windows 11 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl;
		//frameHeight -= OFFSET_HEIGHT_LISTCTRL;
	}
	if ((Constant::UI::GridCtrl::Height::Header + ((rowCount - 1) * Constant::UI::GridCtrl::Height::Row)) >= frameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int scrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		frameWidth -= (scrollBarWidth + Constant::UI::Offset::Width::VScrollBar);
	}

	// Setup columns
	for (int col = 0; col < colCount; col++) {
		// Set header row style
		AppCore::setFixedCellStyle(dataListTablePtr_, Constant::UI::GridCtrl::Index::Header_Row, col);

		// Column header title
		String headerTitle = Constant::String::Empty;
		unsigned headerTitleId = gridCtrlFormatInfoPtr_[col].headerTitleId;
		if (headerTitleId != Constant::NullInteger) {
			headerTitle = getLanguageString(languageTablePtr, headerTitleId);
		}
		dataListTablePtr_->SetItemText(Constant::UI::GridCtrl::Index::Header_Row, col, headerTitle);

		// Column width
		int colWidth = gridCtrlFormatInfoPtr_[col].width;
		if (colWidth != -1) {
			// Set column width as defined
			dataListTablePtr_->SetColumnWidth(col, colWidth);
			// Calculate remaining width
			frameWidth -= colWidth;
		}
		else {
			// Set remaining width for current column
			dataListTablePtr_->SetColumnWidth(col, frameWidth);
		}
	}

	// Setup rows
	int colStyle = -1;
	unsigned itemState = Constant::NullInteger;
	for (int row = 1; row < rowCount; row++) {
		for (int col = 0; col < columnCount_; col++) {
			// Get column style & item state
			colStyle = gridCtrlFormatInfoPtr_[col].columnStyle;
			itemState = dataListTablePtr_->GetItemState(row, col);
			itemState |= GVIS_READONLY;

			// Base column - header-like style
			if (colStyle == COLSTYLE_FIXED) {
				// Set fixed cell style
				AppCore::setFixedCellStyle(dataListTablePtr_, row, col);
			}

			// Checkbox column
			else if (colStyle == COLSTYLE_CHECKBOX) {
				// Set cell type: Checkbox
				if (!dataListTablePtr_->SetCellType(row, col, RUNTIME_CLASS(CGridCellCheck)))
					continue;

				// Get cell
				CGridCellCheck* cellPtr = (CGridCellCheck*)dataListTablePtr_->GetCell(row, col);

				// Set center alignment if defined
				if (gridCtrlFormatInfoPtr_[col].isCentered == true) {
					if (cellPtr == NULL) continue;
					cellPtr->SetCheckPlacement(SCP_CENTERING);
				}
			}

			// Normal column
			else if (colStyle == COLSTYLE_NORMAL) {
				// Set item state
				if (!dataListTablePtr_->SetItemState(row, col, itemState))
					continue;

				// Get cell
				CGridCellBase* cellPtr = (CGridCellBase*)dataListTablePtr_->GetCell(row, col);

				// Set center alignment if defined
				if (gridCtrlFormatInfoPtr_[col].isCentered == true) {
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
	if (msgStyleComboPtr_ == NULL) return;

	switch (comboId)
	{
	case IDC_PWRREMINDER_MSGSTYLE_COMBO:
		msgStyleComboPtr_->ResetContent();
		msgStyleComboPtr_->AddString(getLanguageString(languageTablePtr, COMBOBOX_MSGSTYLE_MESSAGEBOX));		// Message Box
		msgStyleComboPtr_->AddString(getLanguageString(languageTablePtr, COMBOBOX_MSGSTYLE_DIALOGBOX));		// Dialog Box
		break;

	default:
		break;
	}

	// Default
	SDialog::setupComboBox(comboId, languageTablePtr);
}

/**
 * @brief	Switch between modes and re-update dialog item states
 * @param	redrawFlag - Redraw dialog items or not
 * @return	None
 */
void CPwrReminderDlg::switchMode(bool /* redrawFlag = false */)
{
	int currentMode = getCurMode();
	if (currentMode == Mode::Init) {
		// Unlock dialog items
		setLockState(false);

		// Restore [Add/Edit] buttons caption
		updateItemText(IDC_PWRREMINDER_ADD_BTN);
		updateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Enable table
		disableTable(false);

		// Refresh detail view
		refreshDetailView(Mode::Init);

		// Refresh dialog item states
		refreshDialogItemState(true);
	}
	else if (currentMode == Mode::View) {
		// Lock dialog items
		setLockState(false);

		// Restore [Add/Edit] buttons caption
		updateItemText(IDC_PWRREMINDER_ADD_BTN);
		updateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Enable table
		disableTable(false);

		// Refresh detail view
		refreshDetailView(Mode::View);
		displayItemDetails(curSelIndex_);

		// Refresh dialog item states
		refreshDialogItemState(true);
	}
	else if (currentMode == Mode::Add) {
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
		disableTable(true);

		// Refresh detail view
		refreshDetailView(Mode::Add);
		displayItemDetails(Constant::InvalidInteger);
	}
	else if (currentMode == Mode::Update) {
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
		disableTable(true);

		// Refresh detail view
		refreshDetailView(Mode::Update);
	}
	else if (currentMode == Mode::Disable) {
		// Lock dialog items
		setLockState(true);

		// Restore [Add/Edit] buttons caption
		updateItemText(IDC_PWRREMINDER_ADD_BTN);
		updateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Disable table
		disableTable(true);

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
	if (dataListTablePtr_ == NULL) return;

	// Check table column format data validity
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Get table column count
	int colCount = dataListTablePtr_->GetColumnCount();

	// Update size of table columns
	for (int index = 0; index < colCount; index++) {
		int colSize = dataListTablePtr_->GetColumnWidth(index);
		gridCtrlFormatInfoPtr_[index].width = colSize;
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
	const GRIDCTRLCOLFORMAT gridColFormatInfo[] = {
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
	columnCount_ = (sizeof(gridColFormatInfo) / sizeof(GRIDCTRLCOLFORMAT));

	// Initialize table format info data
	if (gridCtrlFormatInfoPtr_ == NULL) {
		gridCtrlFormatInfoPtr_ = new GRIDCTRLCOLFORMAT[columnCount_];
		for (int index = 0; index < columnCount_; index++) {
			// Copy default table column format data
			gridCtrlFormatInfoPtr_[index] = gridColFormatInfo[index];
		}
	}

	// Load layout info data from registry
	int returnValue = 0;
	String keyName;
	for (int index = 0; index < columnCount_; index++) {
		keyName = Key::LayoutInfo::GridColSize(index);
		if (getLayoutInfo(Section::LayoutInfo::PwrReminderTable, keyName, returnValue)) {
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
	int referValue = 0;
	String keyName;
	for (int index = 0; index < columnCount_; index++) {
		referValue = gridCtrlFormatInfoPtr_[index].width;
		keyName = Key::LayoutInfo::GridColSize(index);
		writeLayoutInfo(Section::LayoutInfo::PwrReminderTable, keyName, referValue);
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
	if (msgStringEditPtr_ == NULL) {
		msgStringEditPtr_ = (CEdit*)GetDlgItem(IDC_PWRREMINDER_MSGSTRING_EDITBOX);
		if (msgStringEditPtr_ == NULL) {
			TRACE_ERROR("Error: Message content edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (evtSetTimeRadPtr_ == NULL) {
		evtSetTimeRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN);
		if (evtSetTimeRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventSetTimeRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (evtSetTimeEditPtr_ == NULL) {
		evtSetTimeEditPtr_ = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (evtSetTimeEditPtr_ == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (evtSetTimeSpinPtr_ == NULL) {
		evtSetTimeSpinPtr_ = (CSpinButtonCtrl*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_SPIN);
		if (evtSetTimeSpinPtr_ == NULL) {
			TRACE_ERROR("Error: Time spin control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (evtRepeatSetBtnPtr_ == NULL) {
		evtRepeatSetBtnPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_REPEATSET_BTN);
		if (evtRepeatSetBtnPtr_ == NULL) {
			TRACE_ERROR("Error: [RepeatSet] button not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (evtAppStartupRadPtr_ == NULL) {
		evtAppStartupRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN);
		if (evtAppStartupRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventAppStartupRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (evtSysWakeupRadPtr_ == NULL) {
		evtSysWakeupRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN);
		if (evtSysWakeupRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventSysWakeupRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (evtBfrPwrActionRadPtr_ == NULL) {
		evtBfrPwrActionRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN);
		if (evtBfrPwrActionRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventBfrPwrActionRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (evtPwrActionWakeRadPtr_ == NULL) {
		evtPwrActionWakeRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN);
		if (evtPwrActionWakeRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventPwrActionWakeRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (evtAtAppExitRadPtr_ == NULL) {
		evtAtAppExitRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN);
		if (evtAtAppExitRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (EventAtAppExitRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (styleMsgBoxRadPtr_ == NULL) {
		styleMsgBoxRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN);
		if (styleMsgBoxRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (StyleMsgBoxRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (styleDialogBoxRadPtr_ == NULL) {
		styleDialogBoxRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN);
		if (styleDialogBoxRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (StyleDlgBoxRad)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (styleUseCommonRadPtr_ == NULL) {
		styleUseCommonRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_USECOMMON_RADBTN);
		if (styleUseCommonRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (StyleUseCommonRadio)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (styleUseCustomRadPtr_ == NULL) {
		styleUseCustomRadPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_USECUSTOM_RADBTN);
		if (styleUseCustomRadPtr_ == NULL) {
			TRACE_ERROR("Error: Radio button not found (StyleUseCustomRadio)!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (styleCustomizeBtnPtr_ == NULL) {
		styleCustomizeBtnPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZE_BTN);
		if (styleCustomizeBtnPtr_ == NULL) {
			TRACE_ERROR("Error: [Customize] button not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Setup properties
	if (evtSetTimeSpinPtr_ != NULL) {
		if (evtSetTimeEditPtr_ == NULL) return;

		// Set buddy: Time edit control
		evtSetTimeSpinPtr_->SetBuddy(evtSetTimeEditPtr_);
		evtSetTimeSpinPtr_->SetRange(Constant::Min::TimeSpin, Constant::Max::TimeSpin);
		evtSetTimeSpinPtr_->SetPos(0);
	}

	// Setup time editbox
	ClockTime clockTime;
	ClockTimeUtils::spinPos2Time(clockTime, 0);
	updateTimeSetting(clockTime, false);

	// Initialize counter display
	updateMsgCounter(0);

	// Default
	SDialog::setupDialogItemState();
}

/**
 * @brief	Update Power Reminder data item list
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::updateDataItemList()
{
	// Check table validity
	if (dataListTablePtr_ == NULL) return;

	// If there's no item, do nothing
	int itemNum = getItemNum();
	if (itemNum <= 0)
		return;

	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
	
	// Print items
	int tempValue = -1;
	int rowIndex = 0;
	CGridCellCheck* cellCheckPtr = NULL;
	for (int index = 0; index < itemNum; index++) {

		// Get row index
		rowIndex = index + kFixedRowNum;

		// Get item
		const Item& reminderItem = tempReminderData_.getItemAt(index);

		// Item index
		String tempString = StringUtils::stringFormat(_T("%d"), rowIndex);
		dataListTablePtr_->SetItemText(rowIndex, ColumnID::Index, tempString);

		// Enable state
		cellCheckPtr = (CGridCellCheck*)dataListTablePtr_->GetCell(rowIndex, ColumnID::EnableState);
		if (cellCheckPtr != NULL) {
			cellCheckPtr->SetCheck(reminderItem.isEnabled());
		}

		// ItemID
		tempString.format(_T("%d"), reminderItem.getItemId());
		dataListTablePtr_->SetItemText(rowIndex, ColumnID::ItemID, tempString);

		// Message content
		tempString = reminderItem.getMessage();
		dataListTablePtr_->SetItemText(rowIndex, ColumnID::MessageContent, tempString);

		// EventID
		tempValue = getPairedID(IDTable::PwrReminderEvent, reminderItem.getEventId());
		tempString = getLanguageString(languageTablePtr, tempValue);
		if (reminderItem.getEventId() == Event::atSetTime) {
			// Format time string
			String formatTime = tempString;
			tempString = ClockTimeUtils::format(languageTablePtr, formatTime, reminderItem.getTime());
		}
		dataListTablePtr_->SetItemText(rowIndex, ColumnID::EventID, tempString);

		// Message style
		tempValue = getPairedID(IDTable::PwrReminderStyle, reminderItem.getMessageStyle());
		tempString = getLanguageString(languageTablePtr, tempValue);
		dataListTablePtr_->SetItemText(rowIndex, ColumnID::MsgStyle, tempString);

		// Repeat
		cellCheckPtr = (CGridCellCheck*)dataListTablePtr_->GetCell(rowIndex, ColumnID::Repeat);
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
void CPwrReminderDlg::disableTable(bool isDisabled)
{
	// Redraw read-only style
	redrawDataTable(isDisabled);

	// Check table validity
	if (dataListTablePtr_ == NULL) return;

	// Disable/enable mouse events
	dataListTablePtr_->DisableMouseClick(isDisabled);
	dataListTablePtr_->DisableMouseMove(isDisabled);
}

/**
 * @brief	Update and redraw data table
 * @param	bool isReadOnly - Read-only mode
 * @return	None
 */
void CPwrReminderDlg::redrawDataTable(bool isReadOnly /* = false */)
{
	// Check table validity
	if (dataListTablePtr_ == NULL) return;

	// Update new row number
	int curRowCount = (getItemNum() + kFixedRowNum);
	dataListTablePtr_->SetRowCount(curRowCount);

	// Draw table
	drawDataTable(tableFrameSizePtr_, columnCount_, curRowCount, isReadOnly);
	
	// Update table data
	updateDataItemList();

	// Trigger redrawing table
	dataListTablePtr_->RedrawWindow();
}

/**
 * @brief	Display details of an item at specified index
 * @param	index - Index of item to display
 * @return	None
 */
void CPwrReminderDlg::displayItemDetails(int index)
{
	// Check index validity
	if ((index < Constant::InvalidInteger) || (index > getItemNum()))
		return;

	// Get item at index
	Item reminderItem;
	if (index != Constant::InvalidInteger) {
		reminderItem = tempReminderData_.getItemAt(index);
	}

	// Init default data for mode add
	if (getCurMode() == Mode::Add) {
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
		refreshDetailView(Mode::Init);
	}
	else {
		// TODO: Update current displaying item index
		refreshDetailView(getCurMode());
		curDispIndex_ = index;
	}

	// Initialize RepeatSet dialog if not available
	if (!repeatSetDlgPtr_) {
		repeatSetDlgPtr_ = new CRmdRepeatSetDlg;
		repeatSetDlgPtr_->Create(IDD_RMDREPEATSET_DLG);
		repeatSetDlgPtr_->removeStyle(DS_MODALFRAME | WS_CAPTION);
		repeatSetDlgPtr_->setParent(this);
	}

	// Initialize Customize dialog if not available
	if (!msgStyleSetDlgPtr_ || !IsWindow(msgStyleSetDlgPtr_->GetSafeHwnd())) {
		delete msgStyleSetDlgPtr_;
		msgStyleSetDlgPtr_ = new CRmdMsgStyleSetDlg;
		msgStyleSetDlgPtr_->setParent(this);
	}

	// Display item details
	updateItemData(reminderItem, false);
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
	bool isSelected = ((curSelIndex_ >= 0) && (curSelIndex_ < getItemNum()));

	// Check if number of item has reached the limit
	bool isMaxNum = (getItemNum() >= PwrReminderData::kMaxItemNum);

	// Check if data is all empty or not
	bool isAllEmpty = tempReminderData_.isAllEmpty();

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Disable [Add] button if item number has reached the limit
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_ADD_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(!isMaxNum);
		buttonPtr->SetWindowText(getLanguageString(languageTablePtr, IDC_PWRREMINDER_ADD_BTN));
	}

	// Disable [Remove/Remove All] buttons if data is all empty
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_REMOVE_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(!isAllEmpty);

		// Enable [Remove] button if any item is selected
		buttonPtr->EnableWindow(isSelected);
	}
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_REMOVEALL_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(!isAllEmpty);
	}

	// Enable [Edit] and [Preview] button if any item is selected
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_EDIT_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(isSelected);
		buttonPtr->SetWindowText(getLanguageString(languageTablePtr, IDC_PWRREMINDER_EDIT_BTN));
	}
	buttonPtr = GetDlgItem(IDC_PWRREMINDER_PREVIEW_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(isSelected);
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
	CWnd* checkAllButtonPtr = GetDlgItem(IDC_PWRREMINDER_CHECKALL_BTN);
	CWnd* uncheckAllButtonPtr = GetDlgItem(IDC_PWRREMINDER_UNCHECKALL_BTN);
	if ((checkAllButtonPtr == NULL) || (uncheckAllButtonPtr == NULL))
		return;

	// Get number of items
	int itemNum = getItemNum();
	if (itemNum == 0) {
		// Disable both [Check/Uncheeck All] buttons
		checkAllButtonPtr->EnableWindow(false);
		uncheckAllButtonPtr->EnableWindow(false);
		return;
	}

	// Recheck all items state
	if (isRecheck == true) {
		checkCount_ = 0; // Reset counter
		for (int index = 0; index < itemNum; index++) {
			const Item& tempItem = tempReminderData_.getItemAt(index);
			if (tempItem.isEnabled() == true) {
				checkCount_++;
			}
		}
	}

	// Update button state
	if (checkCount_ == 0) {
		// Enable [Check All] button
		checkAllButtonPtr->EnableWindow(true);
		// Disable [Uncheck All] button
		uncheckAllButtonPtr->EnableWindow(false);
	}
	else if (checkCount_ == itemNum) {
		// Disable [Check All] button
		checkAllButtonPtr->EnableWindow(false);
		// Enable [Uncheck All] button
		uncheckAllButtonPtr->EnableWindow(true);
	}
	else {
		// Enable both [Check/Uncheck All] buttons
		checkAllButtonPtr->EnableWindow(true);
		uncheckAllButtonPtr->EnableWindow(true);
	}
}

/**
 * @brief	Refresh and update state for detail view
 * @param	mode - Detail view mode
 * @return	None
 */
void CPwrReminderDlg::refreshDetailView(int mode)
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
	if (msgStringEditPtr_ != NULL) {
		msgStringEditPtr_->EnableWindow(isEnabled);
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
	if (evtSetTimeRadPtr_ != NULL) {
		evtSetTimeRadPtr_->EnableWindow(isEnabled);
		bool bEnableSetTime = (isEnabled && evtSetTimeRadPtr_->GetCheck());
		if (evtSetTimeEditPtr_ != NULL) {
			evtSetTimeEditPtr_->EnableWindow(bEnableSetTime);
			updateTimeSetting(displayedTimeBackup_, false);
		}
		if (evtSetTimeSpinPtr_ != NULL) {
			evtSetTimeSpinPtr_->EnableWindow(bEnableSetTime);
		}
		if (evtRepeatSetBtnPtr_ != NULL) {
			evtRepeatSetBtnPtr_->EnableWindow(bEnableSetTime);
		}
	}
	if (evtAppStartupRadPtr_ != NULL) {
		evtAppStartupRadPtr_->EnableWindow(isEnabled);
	}
	if (evtSysWakeupRadPtr_ != NULL) {
		evtSysWakeupRadPtr_->EnableWindow(isEnabled);
	}
	if (evtBfrPwrActionRadPtr_ != NULL) {
		evtBfrPwrActionRadPtr_->EnableWindow(isEnabled);
	}
	if (evtPwrActionWakeRadPtr_ != NULL) {
		evtPwrActionWakeRadPtr_->EnableWindow(isEnabled);
	}
	if (evtAtAppExitRadPtr_ != NULL) {
		evtAtAppExitRadPtr_->EnableWindow(isEnabled);
	}
	// Message style
	windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_TITLE);
	if (windowPtr != NULL) {
		windowPtr->EnableWindow(isEnabled);
	}
	if (styleMsgBoxRadPtr_ != NULL) {
		styleMsgBoxRadPtr_->EnableWindow(isEnabled);
	}
	if (styleDialogBoxRadPtr_ != NULL) {
		styleDialogBoxRadPtr_->EnableWindow(isEnabled);
	}
	windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZATION_TITLE);
	if (windowPtr != NULL) {
		windowPtr->EnableWindow(isEnabled);
	}
	if (styleUseCommonRadPtr_ != NULL) {
		styleUseCommonRadPtr_->EnableWindow(isEnabled);
	}
	if (styleUseCustomRadPtr_ != NULL) {
		styleUseCustomRadPtr_->EnableWindow(isEnabled);
	}
	if (styleCustomizeBtnPtr_ != NULL) {
		styleCustomizeBtnPtr_->EnableWindow(isEnabled);
	}
}

/**
 * @brief	Update message content length counter
 * @param	count - Character number counter
 * @return	None
 */
void CPwrReminderDlg::updateMsgCounter(int count)
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
void CPwrReminderDlg::updateTimeSetting(ClockTime& clockTime, bool updateFlag /* = true */)
{
	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Check time editbox validity
	if (evtSetTimeEditPtr_ == NULL) {
		evtSetTimeEditPtr_ = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (evtSetTimeEditPtr_ == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	if (updateFlag == true) {

		// Get value from time editbox
		const int buffLength = evtSetTimeEditPtr_->GetWindowTextLength();
		std::vector<wchar_t> tempBuff(buffLength + 1);
		evtSetTimeEditPtr_->GetWindowText(tempBuff.data(), buffLength + 1);
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
		evtSetTimeEditPtr_->SetWindowText(timeFormatString);

		// Backup current displaying time value
		displayedTimeBackup_ = clockTime;

		// Update time spin position
		if (evtSetTimeSpinPtr_ != NULL) {
			int timeSpinPos = 0;
			ClockTimeUtils::time2SpinPos(clockTime, timeSpinPos);
			evtSetTimeSpinPtr_->SetPos(timeSpinPos);
		}
	}
}

/**
 * @brief	Load Power Reminder data
 * @param	None
 * @return	bool - Result of loading process
 */
bool CPwrReminderDlg::loadPwrReminderData()
{
	// Get app Reminder data pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	VERIFY(theAppPtr != NULL);
	if (theAppPtr == NULL) return false;
	Data* reminderDataPtr = theAppPtr->getAppPwrReminderData();
	if (reminderDataPtr == NULL)
		return false;

	// Copy data
	reminderData_.copy(*reminderDataPtr);
	tempReminderData_.copy(reminderData_);

	// Reset change flag
	setFlagValue(AppFlagID::dialogDataChanged, false);

	// Validate data and auto-correction
	for (int index = 0; index < getItemNum(); index++) {
		Item& reminderItem = tempReminderData_.getItemAt(index);
		if (!validate(reminderItem, true, true)) {
			// Update temp data
			tempReminderData_.update(reminderItem);
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
bool CPwrReminderDlg::savePwrReminderData()
{
	// Copy data and adjust validity
	reminderData_.copy(tempReminderData_);
	reminderData_.adjust();

	// Save app Power Reminder data
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	VERIFY(theAppPtr != NULL);
	if (theAppPtr == NULL) return false;
	theAppPtr->setAppPwrReminderData(&reminderData_);
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
	CGridCellCheck* cellCheckEnablePtr = NULL;
	CGridCellCheck* pCellCheckRepeat = NULL;
	int itemRowCount = (dataListTablePtr_->GetRowCount() - kFixedRowNum);
	for (int index = 0; index < itemRowCount; index++) {
		// Get row index
		rowIndex = (index + kFixedRowNum);

		// Get checkbox cells
		cellCheckEnablePtr = (CGridCellCheck*)dataListTablePtr_->GetCell(rowIndex, ColumnID::EnableState);
		pCellCheckRepeat = (CGridCellCheck*)dataListTablePtr_->GetCell(rowIndex, ColumnID::Repeat);
		if ((cellCheckEnablePtr == NULL) || (pCellCheckRepeat == NULL)) continue;
		
		// Get checked states
		bool isEnabled = cellCheckEnablePtr->GetCheck();
		bool isRepeated = pCellCheckRepeat->GetCheck();
		
		// Update item enable and repeat states
		Item& tempItem = tempReminderData_.getItemAt(index);
		tempItem.enableItem(isEnabled);
		tempItem.enableRepeat(isRepeated);
	}

	// Check if number of items changed
	int itemNum = getItemNum();
	changeFlag |= (itemNum != reminderData_.getItemNum());
	if (changeFlag == true)
		return changeFlag;

	// Check if common style data changed
	const RmdMsgStyleSet& currentCommonStyle = reminderData_.getCommonStyle();
	const RmdMsgStyleSet& tempCommonStyle = tempReminderData_.getCommonStyle();
	changeFlag |= (currentCommonStyle.compare(tempCommonStyle) != true);
	if (changeFlag == true)
		return changeFlag;

	// Check if each item's data changed
	for (int index = 0; index < itemNum; index++) {
		// Get current item and temp item
		const Item& currentItem = reminderData_.getItemAt(index);
		const Item& tempItem = tempReminderData_.getItemAt(index);
 
		// Data comparison
		changeFlag |= (tempItem.isEnabled() != currentItem.isEnabled());
		changeFlag |= (tempItem.getItemId() != currentItem.getItemId());
		changeFlag |= (tempItem.compare(currentItem) != true);

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
void CPwrReminderDlg::add()
{
	// Update data
	UpdateData(true);

	// Create temp Reminder item
	Item tempItem;
	tempItem.setItemId(tempReminderData_.getNextId());

	// Update data
	updateItemData(tempItem, true);

	// Check data validity
	bool isValid = validate(tempItem, true);
	if (isValid == false)
		return;

	// Update item to Power Reminder data item list
	tempReminderData_.update(tempItem);

	// Update table
	redrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Edit a reminder item by index
 * @param	index - Index of item to edit
 * @return	None
 */
void CPwrReminderDlg::edit(int index)
{
	// Update data
	UpdateData(true);

	// Check index validity
	if ((index < 0) || (index > getItemNum()))
		return;

	// Get item at given index
	Item tempItem = tempReminderData_.getItemAt(index);

	// Update data
	updateItemData(tempItem, true);

	// Check data validity
	bool isValid = validate(tempItem, true);
	if (isValid == false)
		return;

	// Update item to Power Reminder data item list
	tempReminderData_.update(tempItem);

	// Update table
	redrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Remove a reminder item by index
 * @param	index - Index of item to remove
 * @return	None
 */
void CPwrReminderDlg::remove(int index)
{
	// Remove item at index
	tempReminderData_.deleteItem(index);

	// Update table
	redrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Remove all reminder items
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::removeAll()
{
	// Remove all items
	tempReminderData_.deleteAll();

	// Update table
	redrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Check/uncheck all Power Reminder items
 * @param	state - Item state
 * @return	None
 */
void CPwrReminderDlg::setAllItemState(bool state)
{
	// Check/uncheck all --> Update all items enable state
	int itemNum = getItemNum();
	for (int index = 0; index < itemNum; index++) {
		Item& tempItem = tempReminderData_.getItemAt(index);
		if (tempItem.isEnabled() != state) {
			tempItem.enableItem(state);
		}
	}

	// Update number of checked items
	checkCount_ = (state == false) ? 0 : itemNum;
	
	// Update data item list
	updateDataItemList();

	// Refresh button state
	refreshDialogItemState(false);
}

/**
 * @brief	Preview Power Reminder item
 * @param	index - Item index
 * @return	None
 */
void CPwrReminderDlg::previewItem(int index)
{
	// Check index validity
	if ((index < 0) || (index >= getItemNum()))
		return;

	// Get item
	const Item& displayItem = tempReminderData_.getItemAt(index);

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
		if (previewMsgDlgPtr_ != NULL) {
			// Destroy dialog
			if (::IsWindow(previewMsgDlgPtr_->GetSafeHwnd())) {
				previewMsgDlgPtr_->DestroyWindow();
			}
			delete previewMsgDlgPtr_;
			previewMsgDlgPtr_ = NULL;
		}

		// Init reminder message dialog
		if (previewMsgDlgPtr_ == NULL) {
			previewMsgDlgPtr_ = new CReminderMsgDlg();
			if (previewMsgDlgPtr_ == NULL) return;

			// Message style
			RmdMsgStyleSet msgStyleSet = tempReminderData_.getCommonStyle();
			if (displayItem.isCustomStyleEnabled())
				msgStyleSet = displayItem.getMessageStyleData();

			// Default timeout for previewing
			int defaultTimeout = PwrReminderData::kPreviewTimeout;

			// Set properties
			previewMsgDlgPtr_->setCaptionFromLanguage(IDC_PWRREMINDER_PREVIEW_BTN);
			previewMsgDlgPtr_->setDispMessage(messageContent);
			previewMsgDlgPtr_->setMessageStyle(msgStyleSet);
			previewMsgDlgPtr_->setAutoCloseInterval(defaultTimeout);

			// Set notify state flags
			previewMsgDlgPtr_->setTopMost(false);
			previewMsgDlgPtr_->setInitSound(true);

			// Display message
			previewMsgDlgPtr_->DoModal();
			
			delete previewMsgDlgPtr_;
			previewMsgDlgPtr_ = NULL;
		}
	}
}

/**
 * @brief	Update reminder data from/to dialog controls
 * @param	reminderItem - Power Reminder item
 * @param	updateFlag - Update data flag
 * @return	None
 */
void CPwrReminderDlg::updateItemData(Item& reminderItem, bool updateFlag)
{
	if (updateFlag == true) {

		/***************************************************************/
		/*															   */
		/*				Update data from dialog controls			   */
		/*															   */
		/***************************************************************/

		/*-----------------------Message content-----------------------*/

		String tempString = Constant::String::Empty;
		if (msgStringEditPtr_ != NULL) {
			const int buffLength = msgStringEditPtr_->GetWindowTextLength();
			std::vector<wchar_t> tempBuff(buffLength + 1);
			msgStringEditPtr_->GetWindowText(tempBuff.data(), buffLength + 1);
			tempString = tempBuff.data();
			reminderItem.setMessage(tempString);
		}

		/*----------------------------Event----------------------------*/

		bool isTemp = false;

		// Event: At set time
		if (evtSetTimeRadPtr_ != NULL) {
			isTemp = evtSetTimeRadPtr_->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::atSetTime);
				if (evtSetTimeEditPtr_ != NULL) {
					ClockTime clockTimeTemp;
					updateTimeSetting(clockTimeTemp, true);
					reminderItem.setTime(clockTimeTemp);
				}
			}
		}
		// Update data for RepeatSet dialog
		if (repeatSetDlgPtr_ != NULL) {
			repeatSetDlgPtr_->updateDialogData(reminderItem, true);
		}
		// Event: At app startup
		if (evtAppStartupRadPtr_ != NULL) {
			isTemp = evtAppStartupRadPtr_->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::atAppStartup);
			}
		}
		// Event: At system wake
		if (evtSysWakeupRadPtr_ != NULL) {
			isTemp = evtSysWakeupRadPtr_->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::atSysWakeUp);
			}
		}
		// Event: Before power action
		if (evtBfrPwrActionRadPtr_ != NULL) {
			isTemp = evtBfrPwrActionRadPtr_->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::beforePwrAction);
			}
		}
		// Event: Wake after action
		if (evtPwrActionWakeRadPtr_ != NULL) {
			isTemp = evtPwrActionWakeRadPtr_->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::wakeAfterAction);
			}
		}
		// Event: Before app exit
		if (evtAtAppExitRadPtr_ != NULL) {
			isTemp = evtAtAppExitRadPtr_->GetCheck();
			if (isTemp == true) {
				reminderItem.setEventId(Event::atAppExit);
			}
		}

		/*------------------------Message style------------------------*/

		// Style: MessageBox
		if (styleMsgBoxRadPtr_ != NULL) {
			isTemp = styleMsgBoxRadPtr_->GetCheck();
			if (isTemp == true) {
				reminderItem.setMessageStyle(Style::messageBox);
			}
		}
		// Style: Dialog Box
		if (styleDialogBoxRadPtr_ != NULL) {
			isTemp = styleDialogBoxRadPtr_->GetCheck();
			if (isTemp == true) {
				reminderItem.setMessageStyle(Style::dialogBox);
			}
		}
		// Style Configuration: Use common style
		if (styleUseCommonRadPtr_ != NULL) {
			isTemp = styleUseCommonRadPtr_->GetCheck();
			if (isTemp == true) {
				reminderItem.enableCustomStyle(false);
			}
		}
		// Style Configuration: Use common style
		if (styleUseCustomRadPtr_ != NULL) {
			isTemp = styleUseCustomRadPtr_->GetCheck();
			if (isTemp == true) {
				reminderItem.enableCustomStyle(true);
			}
		}
		// Update data for Customize dialog
		// Note: Common style data is updated in OnCustomizeStyle()
		if (msgStyleSetDlgPtr_ != NULL &&
			msgStyleSetDlgPtr_->getDataFlag() == MsgStyleSetFlag::customStyle) {
			msgStyleSetDlgPtr_->getData(reminderItem.getMessageStyleData());
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
		int mode = getCurMode();
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
		bool isUseCustomStyle = reminderItem.isCustomStyleEnabled();

		/*-----------------------Message content-----------------------*/

		CWnd* windowPtr = NULL;
		windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_TITLE);
		if (windowPtr != NULL) {
			windowPtr->EnableWindow(isEnabled);
		}
		if (msgStringEditPtr_ != NULL) {
			msgStringEditPtr_->EnableWindow(isEnabled);
			msgStringEditPtr_->SetWindowText(messageContent);
		}
		windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_COUNTER);
		if (windowPtr != NULL) {
			windowPtr->EnableWindow(isEnabled);
			updateMsgCounter(messageContent.getLength());
		}

		/*----------------------------Event----------------------------*/

		bool isTemp = false;
		windowPtr = GetDlgItem(IDC_PWRREMINDER_EVENT_TITLE);
		if (windowPtr != NULL) {
			windowPtr->EnableWindow(isEnabled);
		}

		// Event: At set time
		if (evtSetTimeRadPtr_ != NULL) {
			evtSetTimeRadPtr_->EnableWindow(isEnabled);
			isTemp = (eventId == Event::atSetTime);
			evtSetTimeRadPtr_->SetCheck(isTemp);

			// Set time edit and spin value
			isTemp &= isEnabled;
			if (evtSetTimeEditPtr_ != NULL) {
				evtSetTimeEditPtr_->EnableWindow(isTemp);
				updateTimeSetting(itemTime, false);
			}
			if (evtSetTimeSpinPtr_ != NULL) {
				evtSetTimeSpinPtr_->EnableWindow(isTemp);
			}
		}
		// Update data for RepeatSet dialog
		if (repeatSetDlgPtr_ != NULL) {
			repeatSetDlgPtr_->updateDialogData(reminderItem, false);
		}
		// Event: At app startup
		if (evtAppStartupRadPtr_ != NULL) {
			evtAppStartupRadPtr_->EnableWindow(isEnabled);
			isTemp = (eventId == Event::atAppStartup);
			evtAppStartupRadPtr_->SetCheck(isTemp);
		}
		// Event: At system wake
		if (evtSysWakeupRadPtr_ != NULL) {
			evtSysWakeupRadPtr_->EnableWindow(isEnabled);
			isTemp = (eventId == Event::atSysWakeUp);
			evtSysWakeupRadPtr_->SetCheck(isTemp);
		}
		// Event: Before power action
		if (evtBfrPwrActionRadPtr_ != NULL) {
			evtBfrPwrActionRadPtr_->EnableWindow(isEnabled);
			isTemp = (eventId == Event::beforePwrAction);
			evtBfrPwrActionRadPtr_->SetCheck(isTemp);
		}
		// Event: Wake after action
		if (evtPwrActionWakeRadPtr_ != NULL) {
			evtPwrActionWakeRadPtr_->EnableWindow(isEnabled);
			isTemp = (eventId == Event::wakeAfterAction);
			evtPwrActionWakeRadPtr_->SetCheck(isTemp);
		}
		// Event: Before app exit
		if (evtAtAppExitRadPtr_ != NULL) {
			evtAtAppExitRadPtr_->EnableWindow(isEnabled);
			isTemp = (eventId == Event::atAppExit);
			evtAtAppExitRadPtr_->SetCheck(isTemp);
		}

		/*------------------------Message style------------------------*/

		windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_TITLE);
		if (windowPtr != NULL) {
			windowPtr->EnableWindow(isEnabled);
		}
		// Message Box style
		if (styleMsgBoxRadPtr_ != NULL) {
			styleMsgBoxRadPtr_->EnableWindow(isEnabled);
			isTemp = (messageStyle == Style::messageBox);
			styleMsgBoxRadPtr_->SetCheck(isTemp);
		}
		// Dialog Box style
		if (styleDialogBoxRadPtr_ != NULL) {
			styleDialogBoxRadPtr_->EnableWindow(isEnabled);
			isTemp = (messageStyle == Style::dialogBox);
			styleDialogBoxRadPtr_->SetCheck(isTemp);
		}

		// Style customization
		windowPtr = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_CUSTOMIZATION_TITLE);
		if (windowPtr != NULL) {
			windowPtr->EnableWindow(isEnabled);
		}
		// Use common style configuration
		if (styleUseCommonRadPtr_ != NULL) {
			styleUseCommonRadPtr_->EnableWindow(isEnabled);
			styleUseCommonRadPtr_->SetCheck(!isUseCustomStyle);
		}
		// Use custom style configuration
		if (styleUseCustomRadPtr_ != NULL) {
			styleUseCustomRadPtr_->EnableWindow(isEnabled);
			styleUseCustomRadPtr_->SetCheck(isUseCustomStyle);
		}
		// [Customize] button
		if (styleCustomizeBtnPtr_ != NULL) {
			styleCustomizeBtnPtr_->EnableWindow(isEnabled);
		}
		// Update data for Customize dialog
		if (msgStyleSetDlgPtr_ && IsWindow(msgStyleSetDlgPtr_->GetSafeHwnd())) {
			if (!isUseCustomStyle) {
				msgStyleSetDlgPtr_->setData(tempReminderData_.getCommonStyle());
				msgStyleSetDlgPtr_->setDataFlag(MsgStyleSetFlag::commonStyle);
			}
			else {
				msgStyleSetDlgPtr_->setData(reminderItem.getMessageStyleData());
				msgStyleSetDlgPtr_->setDataFlag(MsgStyleSetFlag::customStyle);
			}
			msgStyleSetDlgPtr_->updateDialogData(false);
		}

		/*-------------------------------------------------------------*/
	}
}

/**
 * @brief	Check Power Reminder item validity
 * @param	reminderItem		 - Item to validate
 * @param	showMsg	 - Show validation message box or not
 * @param	isAutoCorrect - Invalid value auto correction (ON/OFF)
 * @return	bool - Result of validation process
 */
bool CPwrReminderDlg::validate(Item& reminderItem, bool showMsg /* = false */, bool isAutoCorrect /* = false */)
{
	bool result = true;

	int messageStringId;
	StringArray messageStringList;
	messageStringList.clear();

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Check item ID
	if ((reminderItem.getItemId() < PwrReminderData::kMinItemID) || (reminderItem.getItemId() > PwrReminderData::kMaxItemID)) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_ITEMID;
		messageStringList.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (isAutoCorrect == true) {
			// Get next item ID
			reminderItem.setItemId(tempReminderData_.getNextId());
		}
	}

	// Check message content
	String messageContent = reminderItem.getMessage();
	if (messageContent.isEmpty()) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_EMPTY;
		messageStringList.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (isAutoCorrect == true) {
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
		messageStringList.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (isAutoCorrect == true) {
			// Only get character numbers in range
			String tempString = messageContent.left(Constant::Max::StringLength);
			reminderItem.setMessage(tempString);
		}
	}

	// Check event ID
	if ((reminderItem.getEventId() < Event::atSetTime) || (reminderItem.getEventId() > Event::atAppExit)) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_EVENTID;
		messageStringList.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (isAutoCorrect == true) {
			// Set default event ID
			reminderItem.setEventId(Event::atSetTime);
		}
	}

	// Check snooze interval data
	if ((reminderItem.getSnoozeInterval() < PwrRepeatSet::kMinSnoozeInterval) || (reminderItem.getSnoozeInterval() > PwrRepeatSet::kMaxSnoozeInterval)) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_SNOOZEINTERVAL;
		messageStringList.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (isAutoCorrect == true) {
			// Set default snooze interval
			reminderItem.setSnoozeInterval(PwrRepeatSet::kDefaultSnoozeInterval);
		}
	}

	// Check repeat set data
	if ((reminderItem.isRepeatEnabled() == true) && (reminderItem.getActiveDays() == NULL)) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_ACTIVEDAYS;
		messageStringList.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (isAutoCorrect == true) {
			// Set default data
			reminderItem.setActiveDays(PwrRepeatSet::kDefaultActiveDays);
		}
	}

	// Check style ID
	if ((reminderItem.getMessageStyle() < Style::messageBox) || (reminderItem.getMessageStyle() > Style::dialogBox)) {
		messageStringId = MSGBOX_PWRREMINDER_INVALIDITEM_STYLEID;
		messageStringList.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (isAutoCorrect == true) {
			// Set default style ID
			reminderItem.setMessageStyle(Style::messageBox);
		}
	}
	
	// Show error message if enabled
	if ((showMsg == true) && (!messageStringList.empty())) {
		for (int index = 0; index < messageStringList.size(); index++) {
			// If auto correction is ON
			if (isAutoCorrect == true) {
				// Add "Data will be automatically reset to default"
				String errorMessage = messageStringList.at(index);
				errorMessage += getLanguageString(languageTablePtr, MSGBOX_PWRREMINDER_INVALIDITEM_AUTOCORRECT);
				displayMessageBox(errorMessage, NULL, MB_OK | MB_ICONERROR);
			}
			else {
				// Display error message
				displayMessageBox(messageStringList.at(index), NULL, MB_OK | MB_ICONERROR);
			}
		}
	}

	// Remove all message after displaying
	messageStringList.clear();

	return result;
}

/**
 * @brief	Set current mode
 * @param	mode - Mode to set
 * @return	None
 */
void CPwrReminderDlg::setCurMode(int mode)
{
	// If new mode is the same, do nothing
	if (mode == getCurMode()) return;

	// Set new mode
	currentMode_ = mode;
	
	// Switch mode
	switchMode(true);
}

/**
 * @brief	Draw icon for repeat set button
 * @param	None
 * @return	None
 */
void CPwrReminderDlg::drawRepeatSetButton(void)
{
	// Check button validity
	if (evtRepeatSetBtnPtr_ == NULL) {
		evtRepeatSetBtnPtr_ = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_REPEATSET_BTN);
		if (evtRepeatSetBtnPtr_ == NULL) {
			TRACE_ERROR("Error: RepeatSet button not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Draw button icon
	if (evtRepeatSetBtnPtr_ != NULL) {

		// Load icon
		int cx = Constant::UI::Button::IconWidth;
		int cy = Constant::UI::Button::IconHeight;
		HINSTANCE instanceHandle = AfxGetApp()->m_hInstance;
		HICON repeatIconHandle = (HICON)LoadImage(instanceHandle, MAKEINTRESOURCE(IDI_ICON_REPEAT), IMAGE_ICON, cx, cy, LR_DEFAULTCOLOR);
		if (repeatIconHandle == NULL)
			return;

		// Set button icon
		evtRepeatSetBtnPtr_->ModifyStyle(NULL, BS_ICON);
		evtRepeatSetBtnPtr_->SetIcon(repeatIconHandle);
	}
}
