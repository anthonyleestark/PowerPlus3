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
 constexpr const int checkboxColID = 0;
 constexpr const int daytitleColID = 1;
 constexpr const int checkboxColSize = 30;


 //	Implement methods for CEditScheduleDlg
IMPLEMENT_DYNAMIC(CEditScheduleDlg, SDialog)


/**
 * @brief	Constructor
 */
CEditScheduleDlg::CEditScheduleDlg() : SDialog(IDD_EDITSCHEDULE_DLG)
{
	// Initialize member variables

	// Dialog control item
	m_pActionList = NULL;
	m_pTimeEdit = NULL;
	m_pTimeSpin = NULL;
	m_pActiveDayListTable = NULL;

	// Data variables
	m_bEnabled = false;
	m_bRepeat = false;
	m_nAction = APP_ACTION_NOTHING;

	// Data container variables
	ZeroMemory(&m_schScheduleItem, sizeof(ScheduleItem));
	ZeroMemory(&m_schScheduleItemTemp, sizeof(ScheduleItem));

	// Other variables
	m_nDispMode = Mode::Init;
	m_pszActiveTableFrameSize = NULL;
}

/**
 * @brief	Destructor
 */
CEditScheduleDlg::~CEditScheduleDlg()
{
	// Grid table list controls
	if (m_pActiveDayListTable) {
		delete m_pActiveDayListTable;
		m_pActiveDayListTable = NULL;
	}

	// Other variables
	if (m_pszActiveTableFrameSize != NULL) {
		delete m_pszActiveTableFrameSize;
		m_pszActiveTableFrameSize = NULL;
	}
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CEditScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_EDITSCHEDULE_ENABLE_CHK,		 m_bEnabled);
	DDX_Check(pDX, IDC_EDITSCHEDULE_REPEATDAILY_CHK, m_bRepeat);
}

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CEditScheduleDlg::RegisterDialogManagement(void)
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
		nRet = pCtrlMan->AddControl(IDC_EDITSCHEDULE_ENABLE_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_EDITSCHEDULE_ACTION_LABEL, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_EDITSCHEDULE_ACTION_LIST, Combo_Box);
		nRet = pCtrlMan->AddControl(IDC_EDITSCHEDULE_TIME_LABEL, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_EDITSCHEDULE_TIME_EDITBOX, Edit_Control);
		nRet = pCtrlMan->AddControl(IDC_EDITSCHEDULE_TIME_SPIN, Spin_Control);
		nRet = pCtrlMan->AddControl(IDC_EDITSCHEDULE_REPEATDAILY_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX, List_Box);
		nRet = pCtrlMan->AddControl(IDC_EDITSCHEDULE_APPLY_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_EDITSCHEDULE_CANCEL_BTN, Button);
	}

	return nRet;
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CEditScheduleDlg::UnregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->RemoveControl(IDC_EDITSCHEDULE_ENABLE_CHK);
		pCtrlMan->RemoveControl(IDC_EDITSCHEDULE_ACTION_LABEL);
		pCtrlMan->RemoveControl(IDC_EDITSCHEDULE_ACTION_LIST);
		pCtrlMan->RemoveControl(IDC_EDITSCHEDULE_TIME_LABEL);
		pCtrlMan->RemoveControl(IDC_EDITSCHEDULE_TIME_EDITBOX);
		pCtrlMan->RemoveControl(IDC_EDITSCHEDULE_TIME_SPIN);
		pCtrlMan->RemoveControl(IDC_EDITSCHEDULE_REPEATDAILY_CHK);
		pCtrlMan->RemoveControl(IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX);
		pCtrlMan->RemoveControl(IDC_EDITSCHEDULE_APPLY_BTN);
		pCtrlMan->RemoveControl(IDC_EDITSCHEDULE_CANCEL_BTN);
	}

	return SDialog::UnregisterDialogManagement();
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
	SetUseEnter(false);

	// Init dialog items
	SetupLanguage();

	// Update data
	SetupDialogItemState();

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, this->GetCaption());

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
	if (!IsForceClosingByRequest()) {

		// If data changed, ask for saving before closing dialog
		if (GetFlagValue(AppFlagID::dialogDataChanged) == true) {
			// Setup messagebox language
			LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
			const wchar_t* messagePrompt = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CONTENT);
			const wchar_t* messageCaption = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int nConfirm = MessageBox(messagePrompt, messageCaption, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {

				// Update data
				SaveScheduleItem();

				// Return UPDATE flag
				SetReturnFlag(ReturnFlag::Update);
			}
			else {
				// Return CLOSE flag
				SetReturnFlag(ReturnFlag::Close);
			}
		}
	}
	else {
		// Return CLOSE flag
		SetReturnFlag(ReturnFlag::Close);
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
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, this->GetCaption());

	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	Request current dialog to close
 * @param	None
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT CEditScheduleDlg::RequestCloseDialog(void)
{
	// If data changed, ask for saving before closing dialog
	if (GetFlagValue(AppFlagID::dialogDataChanged) == true) {
		// Setup messagebox language
		LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		const wchar_t* messagePrompt = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CONTENT);
		const wchar_t* messageCaption = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CAPTION);

		int nConfirm = MessageBox(messagePrompt, messageCaption, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nConfirm == IDYES) {

			// Update data
			SaveScheduleItem();

			// Return UPDATE flag
			SetReturnFlag(ReturnFlag::Update);
		}
		else if (nConfirm == IDCANCEL) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}
	else {
		// Return CLOSE flag
		SetReturnFlag(ReturnFlag::Close);
	}

	// Request accepted
	return SDialog::RequestCloseDialog();
}

/**
 * @brief	Setup language for dialog items
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::SetupLanguage()
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Setup dialog title
	this->SetCaptionFromLanguage(GetDialogID());

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		unsigned nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDC_EDITSCHEDULE_SUBDETAIL_STATIC:
		case IDC_EDITSCHEDULE_TIME_EDITBOX:
		case IDC_EDITSCHEDULE_TIME_SPIN:
		case IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX:
			// Skip these items
			break;

		case IDC_EDITSCHEDULE_ACTION_LIST:
			SetupComboBox(nID, pAppLang);
			break;

		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}

	// Setup Active day list
	SetupActiveDayList(pAppLang);

	// Default
	SDialog::SetupLanguage();
}

/**
 * @brief	Setup data for combo-boxes
 * @param	nComboID	- ID of combo box
 * @param	ptrLanguage - Language package pointer
 * @return	None
 */
void CEditScheduleDlg::SetupComboBox(unsigned nComboID, LANGTABLE_PTR ptrLanguage)
{
	// Action list
	if (nComboID == IDC_EDITSCHEDULE_ACTION_LIST) {

		// Initialization
		if (m_pActionList == NULL) {
			m_pActionList = (CComboBox*)GetDlgItem(IDC_EDITSCHEDULE_ACTION_LIST);
		}

		// Setup data
		if (m_pActionList != NULL) {
			m_pActionList->ResetContent();
			m_pActionList->AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_DISPLAYOFF));	// Turn off display
			m_pActionList->AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SLEEP));		// Sleep
			m_pActionList->AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SHUTDOWN));		// Shutdown
			m_pActionList->AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_RESTART));		// Restart
			m_pActionList->AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SIGNOUT));		// Log out
			m_pActionList->AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_HIBERNATE));	// Hibernate
		}
	}

	// Default
	SDialog::SetupComboBox(nComboID, ptrLanguage);
}

/**
 * @brief	Initialize and setup language for Active Day list
 * @param	ptrLanguage - Language package pointer
 * @return	None
 */
void CEditScheduleDlg::SetupActiveDayList(LANGTABLE_PTR /*ptrLanguage*/)
{
	// Get parent list frame rect
	CWnd* pListFrameWnd = GetDlgItem(IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX);
	if (pListFrameWnd == NULL) return;
	RECT rcListFrameWnd;
	pListFrameWnd->GetWindowRect(&rcListFrameWnd);
	ScreenToClient(&rcListFrameWnd);

	// Get frame size
	if (m_pszActiveTableFrameSize == NULL) {
		m_pszActiveTableFrameSize = new Size();
		m_pszActiveTableFrameSize->_width = rcListFrameWnd.right - rcListFrameWnd.left;
		m_pszActiveTableFrameSize->_height = rcListFrameWnd.bottom - rcListFrameWnd.top;
	}

	// Initialization
	VERIFY_INITIALIZATION(m_pActiveDayListTable, CGridCtrl)

	// Create table
	if (m_pActiveDayListTable == NULL) return;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	m_pActiveDayListTable->Create(rcListFrameWnd, this, IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX, dwStyle);

	// Destroy frame
	pListFrameWnd->DestroyWindow();

	// Cell format
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pActiveDayListTable->GetDefaultCell(false, false);
	if (pCell == NULL) return;
	pCell->SetFormat(pCell->GetFormat());
	pCell->SetMargin(0);
	pCell->SetBackClr(Color::White);
	pCell->SetTextClr(Color::Black);
	pCell->SetHeight(Constant::UI::GridCtrl::Height::Row_Ex);

	// Setup table
	m_pActiveDayListTable->SetColumnCount(2);
	m_pActiveDayListTable->SetRowCount(Constant::Max::DaysOfWeek);

	// Draw table
	DrawActiveDayTable(GetReadOnlyMode());

	// Display table
	m_pActiveDayListTable->SetListMode(true);
	m_pActiveDayListTable->SetEditable(false);
	m_pActiveDayListTable->SetColumnResize(false);
	m_pActiveDayListTable->SetRowResize(false);
	m_pActiveDayListTable->EnableSelection(true);
	m_pActiveDayListTable->SetSingleRowSelection(true);
	m_pActiveDayListTable->SetSingleColSelection(false);
	m_pActiveDayListTable->ShowWindow(SW_SHOW);
	m_pActiveDayListTable->SetRedraw(true);
}

/**
 * @brief	Draw active day list table
 * @param	bReadOnly - Read-only mode
 * @return	None
 */
void CEditScheduleDlg::DrawActiveDayTable(bool bReadOnly /* = false */)
{
	// Check table validity
	if (m_pActiveDayListTable == NULL) return;

	// Check table format data validity
	if (m_pszActiveTableFrameSize == NULL) return;

	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;

	// Re-update default cell properties
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pActiveDayListTable->GetDefaultCell(false, false);
	if (pCell == NULL) return;

	// Read-only mode --> Change cell color
	if (bReadOnly == true) {
		pCell->SetBackClr(Color::Bright_Gray);
		pCell->SetTextClr(Color::Dark_Gray);
	}
	else {
		pCell->SetBackClr(Color::White);
		pCell->SetTextClr(Color::Black);
	}

	// Setup display size
	int nFrameHeight = m_pszActiveTableFrameSize->Height();
	int nFrameWidth = m_pszActiveTableFrameSize->Width();
	if (GetWindowsOSVersion() == WINDOWS_VERSION_10) {
		// Windows 10 list control offset
		nFrameWidth -= Constant::UI::Offset::Width::ListCtrl_Win10;
	}
	else {
		// Windows 11 list control offset
		nFrameWidth -= Constant::UI::Offset::Width::ListCtrl;
	}
	if ((Constant::Max::DaysOfWeek * Constant::UI::GridCtrl::Height::Row_Ex) >= nFrameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nFrameWidth -= (nScrollBarWidth + Constant::UI::Offset::Width::VScrollBar);
	}

	// Setup columns
	m_pActiveDayListTable->SetColumnWidth(checkboxColID, checkboxColSize);
	m_pActiveDayListTable->SetColumnWidth(daytitleColID, nFrameWidth - checkboxColSize);

	// Setup rows
	unsigned nItemState = INT_NULL;
	for (int nRow = 0; nRow < Constant::Max::DaysOfWeek; nRow++) {

		/*------------------------------------- Checkbox column -------------------------------------*/
		// Set cell type: Checkbox
		if (!m_pActiveDayListTable->SetCellType(nRow, checkboxColID, RUNTIME_CLASS(CGridCellCheck)))
			continue;

		// Set cell checkbox placement: Centering
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_pActiveDayListTable->GetCell(nRow, checkboxColID);
		if (pCellCheck == NULL) continue;
		pCellCheck->SetCheckPlacement(SCP_CENTERING);

		/*------------------------------------ Day title column -------------------------------------*/
		// Update cell state
		nItemState = m_pActiveDayListTable->GetItemState(nRow, daytitleColID);
		if (!m_pActiveDayListTable->SetItemState(nRow, daytitleColID, nItemState | GVIS_READONLY))
			continue;

		// Set cell alignment: Center
		CGridCellBase* pCell = (CGridCellBase*)m_pActiveDayListTable->GetCell(nRow, daytitleColID);
		if (pCell == NULL) continue;
		pCell->SetFormat(pCell->GetFormat() | DT_CENTER);

		/*-------------------------------------------------------------------------------------------*/
	}
}

/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::SetupDialogItemState()
{
	// Setup checkboxes
	m_bEnabled = m_schScheduleItemTemp.IsEnabled();
	m_bRepeat = m_schScheduleItemTemp.IsRepeatEnabled();

	// If is currently in read-only or view mode
	if ((GetReadOnlyMode() == true) || (GetDispMode() == Mode::View)) {
		// Disable top checkbox
		EnableItem(IDC_EDITSCHEDULE_ENABLE_CHK, false);
	}

	// Enable/disable sub-items
	EnableSubItems(m_bEnabled);

	// Setup action list combo value
	m_nAction = m_schScheduleItemTemp.GetAction();
	if (m_pActionList != NULL) {
		m_pActionList->SetCurSel(Opt2Sel(APP_ACTION, m_nAction));
	}

	UpdateData(false);

	// Setup time spin button properties
	int nTimeSpinPos = 0;
	ClockTimeUtils::Time2SpinPos(m_schScheduleItemTemp.GetTime(), nTimeSpinPos);

	// Time spin initialization
	if (m_pTimeSpin == NULL) {
		m_pTimeSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_EDITSCHEDULE_TIME_SPIN);
		if (m_pTimeSpin == NULL) {
			TRACE_ERROR("Error: Time spin control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Setup properties
	if (m_pTimeSpin != NULL) {
		m_pTimeSpin->SetBuddy(GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX));
		m_pTimeSpin->SetRange(Constant::Min::TimeSpin, Constant::Max::TimeSpin);
		m_pTimeSpin->SetPos(nTimeSpinPos);
	}

	// Setup time editbox
	ClockTime clockTimeTemp = m_schScheduleItemTemp.GetTime();
	UpdateTimeSetting(clockTimeTemp, false);

	// Enable/disable active day table (also update its display)
	DisableActiveDayTable(!(m_schScheduleItemTemp.IsEnabled() && m_schScheduleItemTemp.IsRepeatEnabled()));

	// Disable save button at first
	EnableSaveButton(false);

	// Read-only mode (if enabled)
	if (GetReadOnlyMode() == true) {
		CWnd* pWndChild = GetTopWindow();
		while (pWndChild != NULL) {
			pWndChild->EnableWindow(false);
			pWndChild = pWndChild->GetWindow(GW_HWNDNEXT);
		}
	}

	// Default
	SDialog::SetupDialogItemState();
}

/**
 * @brief	Update Active days list table
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::UpdateActiveDayList()
{
	// Check table validity
	if (m_pActiveDayListTable == NULL) return;

	// Load app language package
	LANGTABLE_PTR ptrLanguage = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Print items
	int nDayOfWeekID = INT_INVALID;
	CGridCellCheck* pCellCheck = NULL;
	for (int nRowIndex = 0; nRowIndex < Constant::Max::DaysOfWeek; nRowIndex++) {

		// Day of week
		nDayOfWeekID = nRowIndex;

		// Active state
		bool bActive = (m_schScheduleItemTemp.IsDayActive((DayOfWeek)nDayOfWeekID)) ? true : false;
		pCellCheck = (CGridCellCheck*)m_pActiveDayListTable->GetCell(nRowIndex, checkboxColID);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(bActive);
		}

		// Day title
		const wchar_t* tempString = GetLanguageString(ptrLanguage, GetPairedID(IDTable::DayOfWeek, nDayOfWeekID));
		m_pActiveDayListTable->SetItemText(nRowIndex, daytitleColID, tempString);
	}
}

/**
 * @brief	Disable mouse click events for Active Days table
 * @param	bDisable - Disable/enable
 * @return	None
 */
void CEditScheduleDlg::DisableActiveDayTable(bool bDisable)
{
	// If is currently in read-only or view mode
	if ((GetReadOnlyMode() == true) || (GetDispMode() == Mode::View)) {
		// Force disable
		bDisable = true;
	}

	// Redraw read-only style
	RedrawActiveDayTable(bDisable);

	// Check table validity
	if (m_pActiveDayListTable == NULL) return;

	// Disable/enable mouse events
	m_pActiveDayListTable->DisableMouseClick(bDisable);
	m_pActiveDayListTable->DisableMouseMove(bDisable);
}

/**
 * @brief	Update and redraw Active day table
 * @param	bool bReadOnly - Read-only mode
 * @return	None
 */
void CEditScheduleDlg::RedrawActiveDayTable(bool bReadOnly /* = false */)
{
	// Check table validity
	if (m_pActiveDayListTable == NULL) return;

	// Draw table
	DrawActiveDayTable(bReadOnly);

	// Update table data
	UpdateActiveDayList();

	// Trigger redrawing table
	m_pActiveDayListTable->RedrawWindow();
}

/**
 * @brief	Get schedule item data
 * @param	pschItem - Return item
 * @return	None
 */
void CEditScheduleDlg::GetScheduleItem(PScheduleItem pschItem)
{
	// Copy data
	if (pschItem == NULL) return;
	pschItem->Copy(m_schScheduleItem);
}

/**
 * @brief	Set schedule item data
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::SetScheduleItem(const ScheduleItem& pschItem)
{
	// Copy data
	m_schScheduleItem.Copy(pschItem);
	m_schScheduleItemTemp.Copy(m_schScheduleItem);
}

/**
 * @brief	Update and refresh schedule settings
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::UpdateScheduleItem()
{
	// Update data
	UpdateData(true);

	// Update checkbox values
	m_schScheduleItemTemp.EnableItem(m_bEnabled);
	m_schScheduleItemTemp.EnableRepeat(m_bRepeat);

	// Update action list combo value
	int nCurSel = m_pActionList->GetCurSel();
	m_nAction = Sel2Opt(APP_ACTION, nCurSel);
	m_schScheduleItemTemp.SetAction(m_nAction);

	// Update time value
	ClockTime stTimeTemp;
	UpdateTimeSetting(stTimeTemp, true);

	m_schScheduleItemTemp.SetTime(stTimeTemp);

	// Update active day table changes
	BYTE byRepeatDays = 0;
	CGridCellCheck* pCellCheckActive = NULL;
	if (m_pActiveDayListTable == NULL) return;
	for (int nRowIndex = 0; nRowIndex < Constant::Max::DaysOfWeek; nRowIndex++) {
		// Get checkbox cell
		pCellCheckActive = (CGridCellCheck*)m_pActiveDayListTable->GetCell(nRowIndex, checkboxColID);
		if (pCellCheckActive == NULL) continue;

		// Get checked states
		bool bActive = pCellCheckActive->GetCheck();

		// Update active days of week data
		byRepeatDays |= bActive << nRowIndex;
	}

	// Update active day data
	m_schScheduleItemTemp.SetActiveDays(byRepeatDays);
}

/**
 * @brief	Update and save schedule settings
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::SaveScheduleItem()
{
	// Update settings
	UpdateScheduleItem();
	m_schScheduleItem.Copy(m_schScheduleItemTemp);
}

/**
 * @brief	Compare control values with current config to check for changes
 * @param	None
 * @return	bool - Flag to mark data changed or not
 */
bool CEditScheduleDlg::CheckDataChangeState()
{
	// Update item
	UpdateScheduleItem();

	// Data comparison
	bool bChangeFlag = (m_schScheduleItemTemp.Compare(m_schScheduleItem) != true);

	return bChangeFlag;
}

/**
 * @brief	Enable/disable save button
 * @param	bEnable - Enable or disable button
 * @return	None
 */
void CEditScheduleDlg::EnableSaveButton(bool bEnable)
{
	// If is currently in read-only or view mode, do not enable
	if ((GetReadOnlyMode() == true) || (GetDispMode() == Mode::View)) {
		// Force disable
		bEnable = false;
	}

	// If new state is the same as current state, do nothing
	CButton* pSaveBtn = (CButton*)GetDlgItem(IDC_EDITSCHEDULE_APPLY_BTN);
	if (pSaveBtn != NULL) {
		if (pSaveBtn->IsWindowEnabled() == bEnable)
			return;
	}

	// Update state
	pSaveBtn->EnableWindow(bEnable);
}

/**
 * @brief	Enable/disable sub items of function
 * @param	bEnable - Enable or disable sub items
 * @return	None
 */
void CEditScheduleDlg::EnableSubItems(bool bEnable)
{
	// If is currently in read-only or view mode, do not enable
	if ((GetReadOnlyMode() == true) || (GetDispMode() == Mode::View)) {
		// Force disable
		bEnable = false;
	}

	// Enable/disable items
	GetDlgItem(IDC_EDITSCHEDULE_ACTION_LABEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDITSCHEDULE_ACTION_LIST)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDITSCHEDULE_TIME_LABEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDITSCHEDULE_TIME_SPIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDITSCHEDULE_REPEATDAILY_CHK)->EnableWindow(bEnable);
}

/**
 * @brief	Update time value from/to time edit control
 * @param	clockTime  - Clock-time data
 * @param	bUpdate	   - Update or not (YES/true by default)
 * @return	None
 */
void CEditScheduleDlg::UpdateTimeSetting(ClockTime& clockTime, bool bUpdate /* = true */)
{
	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Get time editbox pointer
	if (m_pTimeEdit == NULL) {
		m_pTimeEdit = (CEdit*)GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX);
		if (m_pTimeEdit == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	if (bUpdate == true) {
		// Get value from time editbox
		const int buffLength = m_pTimeEdit->GetWindowTextLength();
		std::vector<wchar_t> tempBuff(buffLength + 1);
		m_pTimeEdit->GetWindowText(tempBuff.data(), buffLength + 1);
		String timeFormatString = tempBuff.data();

		// Get hour value
		int hour = _wtoi(timeFormatString.Left(2));
		const String timePeriod = timeFormatString.Right(2);
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
		clockTime.SetMinute(_wtoi(timeFormatString.Mid(3, 2)));
	}
	else {
		// Set value for time editbox
		String timeFormatString;
		timeFormatString = ClockTimeUtils::Format(pLang, IDS_FORMAT_SHORTTIME, clockTime);
		m_pTimeEdit->SetWindowText(timeFormatString);
	}
}

/**
 * @brief	Get dialog display mode
 * @param	None
 * @return	int - Display mode
 */
int CEditScheduleDlg::GetDispMode(void) const
{
	return m_nDispMode;
}

/**
 * @brief	Set dialog display mode
 * @param	nMode - Display mode
 * @return	None
 */
void CEditScheduleDlg::SetDispMode(int nMode)
{
	m_nDispMode = nMode;
}

/**
 * @brief	Handle clicking event for [Save] button
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnApply()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EDITSCHEDULE_APPLY_BTN);

	// Save data if changed
	if (GetFlagValue(AppFlagID::dialogDataChanged) == true) {

		// Update data
		SaveScheduleItem();

		// Return UPDATE flag
		SetReturnFlag(ReturnFlag::Update);
	}
	else {
		// Return OK flag
		SetReturnFlag(ReturnFlag::OK);
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
	if (!IsForceClosingByRequest()) {

		// Save app event log if enabled
		OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EDITSCHEDULE_CANCEL_BTN);

		// If data changed, ask for saving before closing dialog
		if (GetFlagValue(AppFlagID::dialogDataChanged) == true) {
			// Setup messagebox language
			LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
			const wchar_t* messagePrompt = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CONTENT);
			const wchar_t* messageCaption = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int nConfirm = MessageBox(messagePrompt, messageCaption, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {

				// Update data
				SaveScheduleItem();

				// Return UPDATE flag
				SetReturnFlag(ReturnFlag::Update);
			}
			else {
				// Return CANCEL flag
				SetReturnFlag(ReturnFlag::Cancel);
			}
		}
	}
	else {
		// Return CANCEL flag
		SetReturnFlag(ReturnFlag::Cancel);
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
	EnableSubItems(m_bEnabled);

	// Check for data change
	SetFlagValue(AppFlagID::dialogDataChanged, CheckDataChangeState());

	// Enable/disable active day table
	DisableActiveDayTable(!(m_schScheduleItemTemp.IsEnabled() && m_schScheduleItemTemp.IsRepeatEnabled()));

	// Enable/disable save button
	EnableSaveButton(GetFlagValue(AppFlagID::dialogDataChanged));
}

/**
 * @brief	Update when action list selection changes
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnChangeAction()
{
	// Save app event log if enabled
	m_pActionList->GetCurSel();
	OutputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_EDITSCHEDULE_ACTION_LIST);

	// Check for value change and enable/disable save button
	SetFlagValue(AppFlagID::dialogDataChanged, CheckDataChangeState());
	EnableSaveButton(GetFlagValue(AppFlagID::dialogDataChanged));
}

/**
 * @brief	Update when repeat checkbox changes
 * @param	None
 * @return	None
 */
void CEditScheduleDlg::OnChangeRepeatDaily()
{
	// Check for data change
	SetFlagValue(AppFlagID::dialogDataChanged, CheckDataChangeState());

	// Enable/disable active day table
	DisableActiveDayTable(!m_schScheduleItemTemp.IsRepeatEnabled());

	// Enable/disable save button
	EnableSaveButton(GetFlagValue(AppFlagID::dialogDataChanged));
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
	if (m_pTimeEdit == NULL) {
		m_pTimeEdit = (CEdit*)GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX);
		if (m_pTimeEdit == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Select all text
	m_pTimeEdit->PostMessage(EM_SETSEL, 0, -1);
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
	if (m_pTimeEdit == NULL) {
		m_pTimeEdit = (CEdit*)GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX);
		if (m_pTimeEdit == NULL) {
			TRACE_ERROR("Error: Time edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Update data
	const int buffLength = m_pTimeEdit->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(buffLength + 1);
	m_pTimeEdit->GetWindowText(tempBuff.data(), buffLength + 1);
	String timeTextValue = tempBuff.data();

	ClockTime clockTime;
	if (ClockTimeUtils::InputText2Time(clockTime, timeTextValue)) {

		// Update new time value
		UpdateTimeSetting(clockTime, false);
		
		// Update timespin new position
		int nSpinPos = 0;
		ClockTimeUtils::Time2SpinPos(clockTime, nSpinPos);
		if (m_pTimeSpin != NULL) {
			m_pTimeSpin->SetPos(nSpinPos);
		}
	}
	else {
		// Restore old time value
		clockTime = m_schScheduleItemTemp.GetTime();
		UpdateTimeSetting(clockTime, false);
		return;
	}

	// Check for value change and enable/disable save button
	SetFlagValue(AppFlagID::dialogDataChanged, CheckDataChangeState());
	EnableSaveButton(GetFlagValue(AppFlagID::dialogDataChanged));
}

/**
 * @brief	Update when time spin value changes
 * @param	pNMDHR  - Default of spin event handler
 * @param	pResult - Default of spin event handler
 * @return	None
 */
void CEditScheduleDlg::OnTimeSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// Get timespin position and convert to time value
	int nPos = pNMUpDown->iPos;
	ClockTime clockTime;
	ClockTimeUtils::SpinPos2Time(clockTime, nPos);
	UpdateTimeSetting(clockTime, false);

	*pResult = NULL;

	// Check for value change and enable/disable save button
	SetFlagValue(AppFlagID::dialogDataChanged, CheckDataChangeState());
	EnableSaveButton(GetFlagValue(AppFlagID::dialogDataChanged));
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
	SetFlagValue(AppFlagID::dialogDataChanged, CheckDataChangeState());

	// Enable/disable save button
	EnableSaveButton(GetFlagValue(AppFlagID::dialogDataChanged));
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
	SetFlagValue(AppFlagID::dialogDataChanged, CheckDataChangeState());

	// Enable/disable save button
	EnableSaveButton(GetFlagValue(AppFlagID::dialogDataChanged));
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

		if (m_pTimeEdit == NULL)
			return 0;

		// Get the editbox rect
		RECT rcEditBox;
		m_pTimeEdit->GetWindowRect(&rcEditBox);
		ScreenToClient(&rcEditBox);

		// If clicked point is inside the editbox area
		if (((pt.x > rcEditBox.left) && (pt.x < rcEditBox.right)) &&
			((pt.y > rcEditBox.top) && (pt.y < rcEditBox.bottom))) {
			if (m_pTimeEdit->IsWindowEnabled()) {
				// Select all text
				m_pTimeEdit->SetSel(0, -1, true);
			}
			return 0;
		}
		else {
			// If the edit box is focused, kill its focus
			CWnd* pCurCtrl = GetFocus();
			if (pCurCtrl == NULL) return 0;
			if (pCurCtrl->GetDlgCtrlID() == IDC_EDITSCHEDULE_TIME_EDITBOX) {
				pCurCtrl->PostMessage(WM_KILLFOCUS);
				this->SetFocus();	// Return focus to dialog
				return 0;
			}
		}
	} break;
	}

	// Default
	return SDialog::WindowProc(message, wParam, lParam);
}

