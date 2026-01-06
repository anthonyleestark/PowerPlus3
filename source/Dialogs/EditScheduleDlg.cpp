/**
 * @file		EditScheduleDlg.cpp
 * @brief		Source file for Edit Schedule details dialog
 * @author		AnthonyLeeStark
 * @date		2017.03.08
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

 #include "MainApp/PowerPlus.h"
 #include "Dialogs/EditScheduleDlg.h"
 
 #ifdef _DEBUG
 #define new DEBUG_NEW
 #endif
 
 using namespace MapTable;
 using namespace Language;
 using namespace AppCore;
 
 
 // Active days list table constants
 constexpr const int kCheckboxColID = 0;
 constexpr const int kDayTitleColID = 1;
 constexpr const int kCheckboxColSize = 30;


 //	Implement methods for CEditScheduleDlg
IMPLEMENT_DYNAMIC(CEditScheduleDlg, SDialog)


/**
 * @brief	Constructor
 */
CEditScheduleDlg::CEditScheduleDlg() : SDialog(IDD_EDITSCHEDULE_DLG)
{
	// Initialize member variables

	// Dialog control item
	actionListPtr_ = NULL;
	timeEditPtr_ = NULL;
	timeSpinCtrlPtr_ = NULL;
	activeDayTablePtr_ = NULL;

	// Data variables
	isEnabled_ = false;
	isRepeated_ = false;
	actionId_ = APP_ACTION_NOTHING;

	// Data container variables
	ZeroMemory(&scheduleItem_, sizeof(ScheduleItem));
	ZeroMemory(&tempScheduleItem_, sizeof(ScheduleItem));

	// Other variables
	displayMode_ = Mode::Init;
	activeTableSizePtr_ = NULL;
}

/**
 * @brief	Destructor
 */
CEditScheduleDlg::~CEditScheduleDlg()
{
	// Grid table list controls
	if (activeDayTablePtr_) {
		delete activeDayTablePtr_;
		activeDayTablePtr_ = NULL;
	}

	// Other variables
	if (activeTableSizePtr_ != NULL) {
		delete activeTableSizePtr_;
		activeTableSizePtr_ = NULL;
	}
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CEditScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_EDITSCHEDULE_ENABLE_CHK,		 isEnabled_);
	DDX_Check(pDX, IDC_EDITSCHEDULE_REPEATDAILY_CHK, isRepeated_);
}

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CEditScheduleDlg::registerDialogManagement(void)
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
		returnValue = pCtrlMan->addControl(IDC_EDITSCHEDULE_ENABLE_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_EDITSCHEDULE_ACTION_LABEL, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_EDITSCHEDULE_ACTION_LIST, Combo_Box);
		returnValue = pCtrlMan->addControl(IDC_EDITSCHEDULE_TIME_LABEL, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_EDITSCHEDULE_TIME_EDITBOX, Edit_Control);
		returnValue = pCtrlMan->addControl(IDC_EDITSCHEDULE_TIME_SPIN, Spin_Control);
		returnValue = pCtrlMan->addControl(IDC_EDITSCHEDULE_REPEATDAILY_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX, List_Box);
		returnValue = pCtrlMan->addControl(IDC_EDITSCHEDULE_APPLY_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_EDITSCHEDULE_CANCEL_BTN, Button);
	}

	return returnValue;
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CEditScheduleDlg::unregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->removeControl(IDC_EDITSCHEDULE_ENABLE_CHK);
		pCtrlMan->removeControl(IDC_EDITSCHEDULE_ACTION_LABEL);
		pCtrlMan->removeControl(IDC_EDITSCHEDULE_ACTION_LIST);
		pCtrlMan->removeControl(IDC_EDITSCHEDULE_TIME_LABEL);
		pCtrlMan->removeControl(IDC_EDITSCHEDULE_TIME_EDITBOX);
		pCtrlMan->removeControl(IDC_EDITSCHEDULE_TIME_SPIN);
		pCtrlMan->removeControl(IDC_EDITSCHEDULE_REPEATDAILY_CHK);
		pCtrlMan->removeControl(IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX);
		pCtrlMan->removeControl(IDC_EDITSCHEDULE_APPLY_BTN);
		pCtrlMan->removeControl(IDC_EDITSCHEDULE_CANCEL_BTN);
	}

	return SDialog::unregisterDialogManagement();
}


// CEditScheduleDlg dialog items ID map
BEGIN_RESOURCEID_MAP(CEditScheduleDlg)
	ON_ID_DIALOG(IDD_EDITSCHEDULE_DLG,					"EditScheduleDlg")
	ON_ID_CONTROL(IDC_EDITSCHEDULE_ENABLE_CHK,			"EnableScheduleCheck")
	ON_ID_CONTROL(IDC_EDITSCHEDULE_ACTION_LABEL,		"ActionLabel")
	ON_ID_CONTROL(IDC_EDITSCHEDULE_ACTION_LIST,			"ActionList")
	ON_ID_CONTROL(IDC_EDITSCHEDULE_TIME_LABEL,			"TimeLabel")
	ON_ID_CONTROL(IDC_EDITSCHEDULE_TIME_EDITBOX,		"TimeEditbox")
	ON_ID_CONTROL(IDC_EDITSCHEDULE_TIME_SPIN,			"TimeSpinButton")
	ON_ID_CONTROL(IDC_EDITSCHEDULE_REPEATDAILY_CHK,		"RepeatDailyCheck")
	ON_ID_CONTROL(IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX,  "ActiveDayList")
	ON_ID_CONTROL(IDC_EDITSCHEDULE_APPLY_BTN,			"SaveButton")
	ON_ID_CONTROL(IDC_EDITSCHEDULE_CANCEL_BTN,			"CancelButton")
END_RESOURCEID_MAP()


// CEditScheduleDlg dialog message map
BEGIN_MESSAGE_MAP(CEditScheduleDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_EDITSCHEDULE_APPLY_BTN,					&CEditScheduleDlg::OnApply)
	ON_BN_CLICKED(IDC_EDITSCHEDULE_CANCEL_BTN,					&CEditScheduleDlg::OnExit)
	ON_BN_CLICKED(IDC_EDITSCHEDULE_ENABLE_CHK,					&CEditScheduleDlg::OnEnableSchedule)
	ON_BN_CLICKED(IDC_EDITSCHEDULE_REPEATDAILY_CHK,				&CEditScheduleDlg::OnChangeRepeatDaily)
	ON_CBN_SELCHANGE(IDC_EDITSCHEDULE_ACTION_LIST,				&CEditScheduleDlg::OnChangeAction)
	ON_EN_SETFOCUS(IDC_EDITSCHEDULE_TIME_EDITBOX,				&CEditScheduleDlg::OnTimeEditSetFocus)
	ON_EN_KILLFOCUS(IDC_EDITSCHEDULE_TIME_EDITBOX,				&CEditScheduleDlg::OnTimeEditKillFocus)
	ON_NOTIFY(UDN_DELTAPOS, IDC_EDITSCHEDULE_TIME_SPIN,			&CEditScheduleDlg::OnTimeSpinChange)
	ON_NOTIFY(NM_CLICK, IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX,	&CEditScheduleDlg::OnClickActiveDayList)
	ON_NOTIFY(NM_RCLICK, IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX,	&CEditScheduleDlg::OnRightClickActiveDayList)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// CEditScheduleDlg message handlers


/**
 * @brief	Initialize Edit Schedule dialog and setup dialog content
 * @param	None
 * @return	BOOL - Default
 */
BOOL CEditScheduleDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Do not use Enter button
	setUseEnter(false);

	// Init dialog items
	setupLanguage();

	// Update data
	setupDialogItemState();

	// Save dialog event log if enabled
	outputEventLog(LOG_EVENT_DLG_INIT, this->getCaption());

	return true;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnClose()
{
	// If not forced closing by request
	if (!isForceClosingByRequest()) {

		// If data changed, ask for saving before closing dialog
		if (getFlagValue(AppFlagID::dialogDataChanged) == true) {
			// Setup messagebox language
			LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
			const wchar_t* messagePrompt = getLanguageString(languageTablePtr, MSGBOX_EDITSCHEDULE_CHANGED_CONTENT);
			const wchar_t* messageCaption = getLanguageString(languageTablePtr, MSGBOX_EDITSCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int confirm = MessageBox(messagePrompt, messageCaption, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {

				// Update data
				saveScheduleItem();

				// Return UPDATE flag
				setReturnFlag(ReturnFlag::Update);
			}
			else {
				// Return CLOSE flag
				setReturnFlag(ReturnFlag::Close);
			}
		}
	}
	else {
		// Return CLOSE flag
		setReturnFlag(ReturnFlag::Close);
	}

	// Close dialog
	SDialog::OnClose();
}

/**
 * @brief	Default method for dialog destroying
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnDestroy()
{
	// Save app event log if enabled
	outputEventLog(LOG_EVENT_DLG_DESTROYED, this->getCaption());

	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	Request current dialog to close
 * @param	None
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT CEditScheduleDlg::requestCloseDialog(void)
{
	// If data changed, ask for saving before closing dialog
	if (getFlagValue(AppFlagID::dialogDataChanged) == true) {
		// Setup messagebox language
		LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
		const wchar_t* messagePrompt = getLanguageString(languageTablePtr, MSGBOX_EDITSCHEDULE_CHANGED_CONTENT);
		const wchar_t* messageCaption = getLanguageString(languageTablePtr, MSGBOX_EDITSCHEDULE_CHANGED_CAPTION);

		int confirm = MessageBox(messagePrompt, messageCaption, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (confirm == IDYES) {

			// Update data
			saveScheduleItem();

			// Return UPDATE flag
			setReturnFlag(ReturnFlag::Update);
		}
		else if (confirm == IDCANCEL) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}
	else {
		// Return CLOSE flag
		setReturnFlag(ReturnFlag::Close);
	}

	// Request accepted
	return SDialog::requestCloseDialog();
}

/**
 * @brief	Setup language for dialog items
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::setupLanguage()
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Setup dialog title
	this->setCaptionFromLanguage(getDialogId());

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		unsigned id = childWndPtr->GetDlgCtrlID();

		switch (id)
		{
		case IDC_EDITSCHEDULE_SUBDETAIL_STATIC:
		case IDC_EDITSCHEDULE_TIME_EDITBOX:
		case IDC_EDITSCHEDULE_TIME_SPIN:
		case IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX:
			// Skip these items
			break;

		case IDC_EDITSCHEDULE_ACTION_LIST:
			setupComboBox(id, languageTablePtr);
			break;

		default:
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Setup Active day list
	setupActiveDayList(languageTablePtr);

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Setup data for combo-boxes
 * @param	comboId	- ID of combo box
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CEditScheduleDlg::setupComboBox(unsigned comboId, LANGTABLE_PTR languageTablePtr)
{
	// Action list
	if (comboId == IDC_EDITSCHEDULE_ACTION_LIST) {

		// Initialization
		if (actionListPtr_ == NULL) {
			actionListPtr_ = (CComboBox*)GetDlgItem(IDC_EDITSCHEDULE_ACTION_LIST);
		}

		// Setup data
		if (actionListPtr_ != NULL) {
			actionListPtr_->ResetContent();
			actionListPtr_->AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_DISPLAYOFF));	// Turn off display
			actionListPtr_->AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SLEEP));		// Sleep
			actionListPtr_->AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SHUTDOWN));		// Shutdown
			actionListPtr_->AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_RESTART));		// Restart
			actionListPtr_->AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SIGNOUT));		// Log out
			actionListPtr_->AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_HIBERNATE));	// Hibernate
		}
	}

	// Default
	SDialog::setupComboBox(comboId, languageTablePtr);
}

/**
 * @brief	Initialize and setup language for Active Day list
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CEditScheduleDlg::setupActiveDayList(LANGTABLE_PTR /*languageTablePtr*/)
{
	// Get parent list frame rect
	CWnd* listFrameWndPtr = GetDlgItem(IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX);
	if (listFrameWndPtr == NULL) return;

	RECT listFrameWndRect;
	listFrameWndPtr->GetWindowRect(&listFrameWndRect);
	ScreenToClient(&listFrameWndRect);

	// Get frame size
	if (activeTableSizePtr_ == NULL) {
		activeTableSizePtr_ = new Size();
		activeTableSizePtr_->_width = listFrameWndRect.right - listFrameWndRect.left;
		activeTableSizePtr_->_height = listFrameWndRect.bottom - listFrameWndRect.top;
	}

	// Initialization
	VERIFY_INITIALIZATION(activeDayTablePtr_, CGridCtrl)

	// Create table
	if (activeDayTablePtr_ == NULL) return;
	DWORD style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	activeDayTablePtr_->Create(listFrameWndRect, this, IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX, style);

	// Destroy frame
	listFrameWndPtr->DestroyWindow();

	// Cell format
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)activeDayTablePtr_->GetDefaultCell(false, false);
	if (cellPtr == NULL) return;
	cellPtr->SetFormat(cellPtr->GetFormat());
	cellPtr->SetMargin(0);
	cellPtr->SetBackClr(Color::White);
	cellPtr->SetTextClr(Color::Black);
	cellPtr->SetHeight(Constant::UI::GridCtrl::Height::Row_Ex);

	// Setup table
	activeDayTablePtr_->SetColumnCount(2);
	activeDayTablePtr_->SetRowCount(Constant::Max::DaysOfWeek);

	// Draw table
	drawActiveDayTable(isReadOnlyMode());

	// Display table
	activeDayTablePtr_->SetListMode(true);
	activeDayTablePtr_->SetEditable(false);
	activeDayTablePtr_->SetColumnResize(false);
	activeDayTablePtr_->SetRowResize(false);
	activeDayTablePtr_->EnableSelection(true);
	activeDayTablePtr_->SetSingleRowSelection(true);
	activeDayTablePtr_->SetSingleColSelection(false);
	activeDayTablePtr_->ShowWindow(SW_SHOW);
	activeDayTablePtr_->SetRedraw(true);
}

/**
 * @brief	Draw active day list table
 * @param	isReadOnly - Read-only mode
 * @return	None
 */
void CEditScheduleDlg::drawActiveDayTable(bool isReadOnly /* = false */)
{
	// Check table validity
	if (activeDayTablePtr_ == NULL) return;

	// Check table format data validity
	if (activeTableSizePtr_ == NULL) return;

	// Get app pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) return;

	// Re-update default cell properties
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)activeDayTablePtr_->GetDefaultCell(false, false);
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
	int frameHeight = activeTableSizePtr_->height();
	int frameWidth = activeTableSizePtr_->width();
	if (AppCore::getWindowsOSVersion() == WINDOWS_VERSION_10) {
		// Windows 10 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl_Win10;
	}
	else {
		// Windows 11 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl;
	}
	if ((Constant::Max::DaysOfWeek * Constant::UI::GridCtrl::Height::Row_Ex) >= frameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int scrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		frameWidth -= (scrollBarWidth + Constant::UI::Offset::Width::VScrollBar);
	}

	// Setup columns
	activeDayTablePtr_->SetColumnWidth(kCheckboxColID, kCheckboxColSize);
	activeDayTablePtr_->SetColumnWidth(kDayTitleColID, frameWidth - kCheckboxColSize);

	// Setup rows
	unsigned itemState = kNullInteger;
	for (int row = 0; row < Constant::Max::DaysOfWeek; row++) {

		/*------------------------------------- Checkbox column -------------------------------------*/

		// Set cell type: Checkbox
		if (!activeDayTablePtr_->SetCellType(row, kCheckboxColID, RUNTIME_CLASS(CGridCellCheck)))
			continue;

		// Set cell checkbox placement: Centering
		CGridCellCheck* cellCheckPtr = (CGridCellCheck*)activeDayTablePtr_->GetCell(row, kCheckboxColID);
		if (cellCheckPtr == NULL) continue;
		cellCheckPtr->SetCheckPlacement(SCP_CENTERING);

		/*------------------------------------ Day title column -------------------------------------*/

		// Update cell state
		itemState = activeDayTablePtr_->GetItemState(row, kDayTitleColID);
		if (!activeDayTablePtr_->SetItemState(row, kDayTitleColID, itemState | GVIS_READONLY))
			continue;

		// Set cell alignment: Center
		CGridCellBase* cellPtr = (CGridCellBase*)activeDayTablePtr_->GetCell(row, kDayTitleColID);
		if (cellPtr == NULL) continue;
		cellPtr->SetFormat(cellPtr->GetFormat() | DT_CENTER);

		/*-------------------------------------------------------------------------------------------*/
	}
}

/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::setupDialogItemState()
{
	// Setup checkboxes
	isEnabled_ = tempScheduleItem_.isEnabled();
	isRepeated_ = tempScheduleItem_.isRepeatEnabled();

	// If is currently in read-only or view mode
	if ((isReadOnlyMode() == true) || (getDispMode() == Mode::View)) {
		// Disable top checkbox
		enableItem(IDC_EDITSCHEDULE_ENABLE_CHK, false);
	}

	// Enable/disable sub-items
	enableSubItems(isEnabled_);

	// Setup action list combo value
	actionId_ = tempScheduleItem_.getAction();
	if (actionListPtr_ != NULL) {
		actionListPtr_->SetCurSel(AppCore::opt2Sel(APP_ACTION, actionId_));
	}

	UpdateData(false);

	// Setup time spin button properties
	int timeSpinPos = 0;
	ClockTimeUtils::time2SpinPos(tempScheduleItem_.getTime(), timeSpinPos);

	// Time spin initialization
	if (timeSpinCtrlPtr_ == NULL) {
		timeSpinCtrlPtr_ = (CSpinButtonCtrl*)GetDlgItem(IDC_EDITSCHEDULE_TIME_SPIN);
		if (timeSpinCtrlPtr_ == NULL) {
			TRACE_ERROR("Error: Time spin control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Setup properties
	if (timeSpinCtrlPtr_ != NULL) {
		timeSpinCtrlPtr_->SetBuddy(GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX));
		timeSpinCtrlPtr_->SetRange(Constant::Min::TimeSpin, Constant::Max::TimeSpin);
		timeSpinCtrlPtr_->SetPos(timeSpinPos);
	}

	// Setup time editbox
	ClockTime clockTimeTemp = tempScheduleItem_.getTime();
	updateTimeSetting(clockTimeTemp, false);

	// Enable/disable active day table (also update its display)
	disableActiveDayTable(!(tempScheduleItem_.isEnabled() && tempScheduleItem_.isRepeatEnabled()));

	// Disable save button at first
	enableSaveButton(false);

	// Read-only mode (if enabled)
	if (isReadOnlyMode() == true) {
		CWnd* childWndPtr = GetTopWindow();
		while (childWndPtr != NULL) {
			childWndPtr->EnableWindow(false);
			childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT);
		}
	}

	// Default
	SDialog::setupDialogItemState();
}

/**
 * @brief	Update Active days list table
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::updateActiveDayList()
{
	// Check table validity
	if (activeDayTablePtr_ == NULL) return;

	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Print items
	int dayOfWeekId = kInvalidInteger;
	CGridCellCheck* cellCheckPtr = NULL;
	for (int rowIndex = 0; rowIndex < Constant::Max::DaysOfWeek; rowIndex++) {

		// Day of week
		dayOfWeekId = rowIndex;

		// Active state
		bool isActive = (tempScheduleItem_.isDayActive((DayOfWeek)dayOfWeekId)) ? true : false;
		cellCheckPtr = (CGridCellCheck*)activeDayTablePtr_->GetCell(rowIndex, kCheckboxColID);
		if (cellCheckPtr != NULL) {
			cellCheckPtr->SetCheck(isActive);
		}

		// Day title
		const wchar_t* tempString = getLanguageString(languageTablePtr, getPairedID(IDTable::DayOfWeek, dayOfWeekId));
		activeDayTablePtr_->SetItemText(rowIndex, kDayTitleColID, tempString);
	}
}

/**
 * @brief	Disable mouse click events for Active Days table
 * @param	isDisabled - Disable/enable
 * @return	None
 */
void CEditScheduleDlg::disableActiveDayTable(bool isDisabled)
{
	// If is currently in read-only or view mode
	if ((isReadOnlyMode() == true) || (getDispMode() == Mode::View)) {
		// Force disable
		isDisabled = true;
	}

	// Redraw read-only style
	RedrawActiveDayTable(isDisabled);

	// Check table validity
	if (activeDayTablePtr_ == NULL) return;

	// Disable/enable mouse events
	activeDayTablePtr_->DisableMouseClick(isDisabled);
	activeDayTablePtr_->DisableMouseMove(isDisabled);
}

/**
 * @brief	Update and redraw Active day table
 * @param	bool isReadOnly - Read-only mode
 * @return	None
 */
void CEditScheduleDlg::RedrawActiveDayTable(bool isReadOnly /* = false */)
{
	// Check table validity
	if (activeDayTablePtr_ == NULL) return;

	// Draw table
	drawActiveDayTable(isReadOnly);

	// Update table data
	updateActiveDayList();

	// Trigger redrawing table
	activeDayTablePtr_->RedrawWindow();
}

/**
 * @brief	Get schedule item data
 * @param	scheduleItemPtr - Return item
 * @return	None
 */
void CEditScheduleDlg::getScheduleItem(PScheduleItem scheduleItemPtr)
{
	// Copy data
	if (scheduleItemPtr == NULL) return;
	scheduleItemPtr->copy(scheduleItem_);
}

/**
 * @brief	Set schedule item data
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::setScheduleItem(const ScheduleItem& scheduleItemPtr)
{
	// Copy data
	scheduleItem_.copy(scheduleItemPtr);
	tempScheduleItem_.copy(scheduleItem_);
}

/**
 * @brief	Update and refresh schedule settings
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::updateScheduleItem()
{
	// Update data
	UpdateData(true);

	// Update checkbox values
	tempScheduleItem_.enableItem(isEnabled_);
	tempScheduleItem_.enableRepeat(isRepeated_);

	// Update action list combo value
	int currenSelection = actionListPtr_->GetCurSel();
	actionId_ = AppCore::sel2Opt(APP_ACTION, currenSelection);
	tempScheduleItem_.setAction(actionId_);

	// Update time value
	ClockTime tempTimeValue;
	updateTimeSetting(tempTimeValue, true);

	tempScheduleItem_.setTime(tempTimeValue);

	// Update active day table changes
	BYTE repeatDays = 0;
	CGridCellCheck* cellCheckActivePtr = NULL;
	if (activeDayTablePtr_ == NULL) return;
	for (int rowIndex = 0; rowIndex < Constant::Max::DaysOfWeek; rowIndex++) {
		// Get checkbox cell
		cellCheckActivePtr = (CGridCellCheck*)activeDayTablePtr_->GetCell(rowIndex, kCheckboxColID);
		if (cellCheckActivePtr == NULL) continue;

		// Get checked states
		bool isActive = cellCheckActivePtr->GetCheck();

		// Update active days of week data
		repeatDays |= isActive << rowIndex;
	}

	// Update active day data
	tempScheduleItem_.setActiveDays(repeatDays);
}

/**
 * @brief	Update and save schedule settings
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::saveScheduleItem()
{
	// Update settings
	updateScheduleItem();
	scheduleItem_.copy(tempScheduleItem_);
}

/**
 * @brief	Compare control values with current config to check for changes
 * @param	None
 * @return	bool - Flag to mark data changed or not
 */
bool CEditScheduleDlg::checkDataChangeState()
{
	// Update item
	updateScheduleItem();

	// Data comparison
	bool changeFlag = (tempScheduleItem_.compare(scheduleItem_) != true);

	return changeFlag;
}

/**
 * @brief	Enable/disable save button
 * @param	isEnabled - Enable or disable button
 * @return	None
 */
void CEditScheduleDlg::enableSaveButton(bool isEnabled)
{
	// If is currently in read-only or view mode, do not enable
	if ((isReadOnlyMode() == true) || (getDispMode() == Mode::View)) {
		// Force disable
		isEnabled = false;
	}

	// If new state is the same as current state, do nothing
	CButton* saveButtonPtr = (CButton*)GetDlgItem(IDC_EDITSCHEDULE_APPLY_BTN);
	if (saveButtonPtr != NULL) {
		if (saveButtonPtr->IsWindowEnabled() == isEnabled)
			return;
	}

	// Update state
	saveButtonPtr->EnableWindow(isEnabled);
}

/**
 * @brief	Enable/disable sub items of function
 * @param	isEnabled - Enable or disable sub items
 * @return	None
 */
void CEditScheduleDlg::enableSubItems(bool isEnabled)
{
	// If is currently in read-only or view mode, do not enable
	if ((isReadOnlyMode() == true) || (getDispMode() == Mode::View)) {
		// Force disable
		isEnabled = false;
	}

	// Enable/disable items
	GetDlgItem(IDC_EDITSCHEDULE_ACTION_LABEL)->EnableWindow(isEnabled);
	GetDlgItem(IDC_EDITSCHEDULE_ACTION_LIST)->EnableWindow(isEnabled);
	GetDlgItem(IDC_EDITSCHEDULE_TIME_LABEL)->EnableWindow(isEnabled);
	GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX)->EnableWindow(isEnabled);
	GetDlgItem(IDC_EDITSCHEDULE_TIME_SPIN)->EnableWindow(isEnabled);
	GetDlgItem(IDC_EDITSCHEDULE_REPEATDAILY_CHK)->EnableWindow(isEnabled);
}

/**
 * @brief	Update time value from/to time edit control
 * @param	clockTime  - Clock-time data
 * @param	updateFlag	   - Update or not (YES/true by default)
 * @return	None
 */
void CEditScheduleDlg::updateTimeSetting(ClockTime& clockTime, bool updateFlag /* = true */)
{
	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Get time editbox pointer
	if (timeEditPtr_ == NULL) {
		timeEditPtr_ = (CEdit*)GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX);
		if (timeEditPtr_ == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	if (updateFlag == true) {
		// Get value from time editbox
		const int buffLength = timeEditPtr_->GetWindowTextLength();
		std::vector<wchar_t> tempBuff(buffLength + 1);
		timeEditPtr_->GetWindowText(tempBuff.data(), buffLength + 1);
		String timeFormatString = tempBuff.data();

		// Get hour value
		int hour = _wtoi(timeFormatString.left(2));
		const String timePeriod = timeFormatString.right(2);
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
		clockTime.setMinute(_wtoi(timeFormatString.mid(3, 2)));
	}
	else {
		// Set value for time editbox
		String timeFormatString;
		timeFormatString = ClockTimeUtils::format(languageTablePtr, IDS_FORMAT_SHORTTIME, clockTime);
		timeEditPtr_->SetWindowText(timeFormatString);
	}
}

/**
 * @brief	Get dialog display mode
 * @param	None
 * @return	int - Display mode
 */
int CEditScheduleDlg::getDispMode(void) const
{
	return displayMode_;
}

/**
 * @brief	Set dialog display mode
 * @param	mode - Display mode
 * @return	None
 */
void CEditScheduleDlg::setDispMode(int mode)
{
	displayMode_ = mode;
}

/**
 * @brief	Handle clicking event for [Save] button
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnApply()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EDITSCHEDULE_APPLY_BTN);

	// Save data if changed
	if (getFlagValue(AppFlagID::dialogDataChanged) == true) {

		// Update data
		saveScheduleItem();

		// Return UPDATE flag
		setReturnFlag(ReturnFlag::Update);
	}
	else {
		// Return OK flag
		setReturnFlag(ReturnFlag::OK);
	}

	// Close dialog
	SDialog::OnOK();
}

/**
 * @brief	Handle clicking event for [Exit] button
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnExit()
{
	// If not forced closing by request
	if (!isForceClosingByRequest()) {

		// Save app event log if enabled
		outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EDITSCHEDULE_CANCEL_BTN);

		// If data changed, ask for saving before closing dialog
		if (getFlagValue(AppFlagID::dialogDataChanged) == true) {
			// Setup messagebox language
			LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
			const wchar_t* messagePrompt = getLanguageString(languageTablePtr, MSGBOX_EDITSCHEDULE_CHANGED_CONTENT);
			const wchar_t* messageCaption = getLanguageString(languageTablePtr, MSGBOX_EDITSCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int confirm = MessageBox(messagePrompt, messageCaption, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {

				// Update data
				saveScheduleItem();

				// Return UPDATE flag
				setReturnFlag(ReturnFlag::Update);
			}
			else {
				// Return CANCEL flag
				setReturnFlag(ReturnFlag::Cancel);
			}
		}
	}
	else {
		// Return CANCEL flag
		setReturnFlag(ReturnFlag::Cancel);
	}

	// Close dialog
	SDialog::OnCancel();
}

/**
 * @brief	Handle clicking event for [EnableSchedule] checkbox
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnEnableSchedule()
{
	// Update dialog item state
	UpdateData(true);
	enableSubItems(isEnabled_);

	// Check for data change
	setFlagValue(AppFlagID::dialogDataChanged, checkDataChangeState());

	// Enable/disable active day table
	disableActiveDayTable(!(tempScheduleItem_.isEnabled() && tempScheduleItem_.isRepeatEnabled()));

	// Enable/disable save button
	enableSaveButton(getFlagValue(AppFlagID::dialogDataChanged));
}

/**
 * @brief	Update when action list selection changes
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnChangeAction()
{
	// Save app event log if enabled
	actionListPtr_->GetCurSel();
	outputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_EDITSCHEDULE_ACTION_LIST);

	// Check for value change and enable/disable save button
	setFlagValue(AppFlagID::dialogDataChanged, checkDataChangeState());
	enableSaveButton(getFlagValue(AppFlagID::dialogDataChanged));
}

/**
 * @brief	Update when repeat checkbox changes
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnChangeRepeatDaily()
{
	// Check for data change
	setFlagValue(AppFlagID::dialogDataChanged, checkDataChangeState());

	// Enable/disable active day table
	disableActiveDayTable(!tempScheduleItem_.isRepeatEnabled());

	// Enable/disable save button
	enableSaveButton(getFlagValue(AppFlagID::dialogDataChanged));
}

/**
 * @brief	Select all text when editbox is set focus
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnTimeEditSetFocus()
{
	/*********************************************************************/
	/*																	 */
	/*			TODO: Time edit set focus --> Select all text			 */
	/*																	 */
	/*********************************************************************/

	// Check control validity
	if (timeEditPtr_ == NULL) {
		timeEditPtr_ = (CEdit*)GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX);
		if (timeEditPtr_ == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Select all text
	timeEditPtr_->PostMessage(EM_SETSEL, 0, -1);
}

/**
 * @brief	Update when time editbox is killed focus
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnTimeEditKillFocus()
{
	/*********************************************************************/
	/*																	 */
	/*			TODO: Time edit kill forcus --> Update data				 */
	/*																	 */
	/*********************************************************************/

	// Check control validity
	if (timeEditPtr_ == NULL) {
		timeEditPtr_ = (CEdit*)GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX);
		if (timeEditPtr_ == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Update data
	const int buffLength = timeEditPtr_->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(buffLength + 1);
	timeEditPtr_->GetWindowText(tempBuff.data(), buffLength + 1);
	String timeTextValue = tempBuff.data();

	ClockTime clockTime;
	if (ClockTimeUtils::inputText2Time(clockTime, timeTextValue)) {

		// Update new time value
		updateTimeSetting(clockTime, false);
		
		// Update timespin new position
		int timeSpinPos = 0;
		ClockTimeUtils::time2SpinPos(clockTime, timeSpinPos);
		if (timeSpinCtrlPtr_ != NULL) {
			timeSpinCtrlPtr_->SetPos(timeSpinPos);
		}
	}
	else {
		// Restore old time value
		clockTime = tempScheduleItem_.getTime();
		updateTimeSetting(clockTime, false);
		return;
	}

	// Check for value change and enable/disable save button
	setFlagValue(AppFlagID::dialogDataChanged, checkDataChangeState());
	enableSaveButton(getFlagValue(AppFlagID::dialogDataChanged));
}

/**
 * @brief	Update when time spin value changes
 * @param	pNMDHR  - Default of spin event handler
 * @param	pResult - Default of spin event handler
 * @return	None
 */
void CEditScheduleDlg::OnTimeSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN upDownPtr = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// Get timespin position and convert to time value
	int position = upDownPtr->iPos;
	ClockTime clockTime;
	ClockTimeUtils::spinPos2Time(clockTime, position);
	updateTimeSetting(clockTime, false);

	*pResult = NULL;

	// Check for value change and enable/disable save button
	setFlagValue(AppFlagID::dialogDataChanged, checkDataChangeState());
	enableSaveButton(getFlagValue(AppFlagID::dialogDataChanged));
}

/**
 * @brief	Handle click event on Active days of week table
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CEditScheduleDlg::OnClickActiveDayList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Success (return 0)
	*pResult = NULL;

	// Update data (also check change state)
	setFlagValue(AppFlagID::dialogDataChanged, checkDataChangeState());

	// Enable/disable save button
	enableSaveButton(getFlagValue(AppFlagID::dialogDataChanged));
}

/**
 * @brief	Handle right click event on Active days of week table
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CEditScheduleDlg::OnRightClickActiveDayList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Success (return 0)
	*pResult = NULL;

	// Update data (also check change state)
	setFlagValue(AppFlagID::dialogDataChanged, checkDataChangeState());

	// Enable/disable save button
	enableSaveButton(getFlagValue(AppFlagID::dialogDataChanged));
}

/**
 * @brief	Pre-handle dialog messages
 * @param	message - Message ID
 * @param	wParam	- First param (HIWORD)
 * @param	lParam	- Second param (LOWORD)
 * @return	None
 */
LRESULT CEditScheduleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONUP:
	{
		// Get clicked point
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (timeEditPtr_ == NULL)
			return 0;

		// Get the editbox rect
		RECT editBoxRect;
		timeEditPtr_->GetWindowRect(&editBoxRect);
		ScreenToClient(&editBoxRect);

		// If clicked point is inside the editbox area
		if (((pt.x > editBoxRect.left) && (pt.x < editBoxRect.right)) &&
			((pt.y > editBoxRect.top) && (pt.y < editBoxRect.bottom))) {
			if (timeEditPtr_->IsWindowEnabled()) {
				// Select all text
				timeEditPtr_->SetSel(0, -1, true);
			}
			return 0;
		}
		else {
			// If the edit box is focused, kill its focus
			CWnd* curCtrlWndPtr = GetFocus();
			if (curCtrlWndPtr == NULL) return 0;
			if (curCtrlWndPtr->GetDlgCtrlID() == IDC_EDITSCHEDULE_TIME_EDITBOX) {
				curCtrlWndPtr->PostMessage(WM_KILLFOCUS);
				this->SetFocus();	// Return focus to dialog
				return 0;
			}
		}
	} break;
	}

	// Default
	return SDialog::WindowProc(message, wParam, lParam);
}

