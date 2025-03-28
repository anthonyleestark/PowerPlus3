
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		EditScheduleDlg.cpp
//		Description:	Source file for Edit Schedule details dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2017.03.08:		Create new
//						<1> 2024.01.27:		Update to version 3.0
//						<2> 2024.10.27:		Implement Multi schedule function
//						<3> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PowerPlus.h"
#include "EditScheduleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace TableFuncs;
using namespace CoreFuncs;


////////////////////////////////////////////////////////
//
//	Define macros for active day list table
//
////////////////////////////////////////////////////////

#define COL_ID_CHECKBOX		0
#define COL_ID_DAYTITLE		1
#define COL_SIZE_CHECKBOX	30


////////////////////////////////////////////////////////
//
//	Implement methods for CEditScheduleDlg
//
////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CEditScheduleDlg, SDialog)

////////////////////////////////////////////////////////
// 
//	Function name:	CEditScheduleDlg
//	Description:	Constructor
//
////////////////////////////////////////////////////////

CEditScheduleDlg::CEditScheduleDlg() : SDialog(IDD_EDITSCHEDULE_DLG)
{
	// Initialize member variables

	// Dialog control item
	m_pActionList = NULL;
	m_pTimeEdit = NULL;
	m_pTimeSpin = NULL;
	m_pActiveDayListTable = NULL;

	// Data variables
	m_bEnable = FALSE;
	m_bRepeat = FALSE;
	m_nAction = APP_ACTION_NOTHING;

	// Data container variables
	ZeroMemory(&m_schScheduleItem, sizeof(SCHEDULEITEM));
	ZeroMemory(&m_schScheduleItemTemp, sizeof(SCHEDULEITEM));

	// Other variables
	m_nRetFlag = RETFLAG_INVALID;
	m_nDispMode = MODE_INIT;
	m_pszActiveTableFrameSize = NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CEditScheduleDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_EDITSCHEDULE_ENABLE_CHK,		 m_bEnable);
	DDX_Check(pDX, IDC_EDITSCHEDULE_REPEATDAILY_CHK, m_bRepeat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegisterDialogManagement
//	Description:	Register dialog control management
//  Arguments:		None
//  Return value:	INT_PTR - Number of controls added to management
//
//////////////////////////////////////////////////////////////////////////

INT_PTR CEditScheduleDlg::RegisterDialogManagement(void)
{
	INT_PTR nRet = SDialog::RegisterDialogManagement();
	if (nRet != 0) {
		TRCLOG("Error: Register dialog management failed!!!");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UnregisterDialogManagement
//	Description:	Unregister dialog control management
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CEditScheduleDlg::UnregisterDialogManagement(void)
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

//////////////////////////////////////////////////////////////////////////
//
//	CEditScheduleDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
//
//	CEditScheduleDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

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
//// Implementation

// CEditScheduleDlg message handlers

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize Edit Schedule dialog and setup dialog content
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CEditScheduleDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Do not use Enter button
	SetUseEnter(FALSE);

	// Init dialog items
	SetupLanguage();

	// Update data
	SetupDialogItemState();

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, this->GetCaption());

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClose
//	Description:	Default method for dialog closing
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::OnClose()
{
	// If not forced closing by request
	if (!IsForceClosingByRequest()) {

		// If data changed, ask for saving before closing dialog
		if (GetFlagValue(FLAGID_CHANGE_FLAG) == TRUE) {
			// Setup messagebox language
			LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
			CString strMessage = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CONTENT);
			CString strMsgCaption = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int nConfirm = MessageBox(strMessage, strMsgCaption, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {

				// Update data
				SaveScheduleItem();

				// Return UPDATE flag
				SetReturnFlag(RETFLAG_UPDATE);
			}
			else {
				// Return CLOSE flag
				SetReturnFlag(RETFLAG_CLOSE);
			}
		}
	}
	else {
		// Return CLOSE flag
		SetReturnFlag(RETFLAG_CLOSE);
	}

	// Close dialog
	SDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDestroy
//	Description:	Default method for dialog destroying
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::OnDestroy()
{
	// Save app event log if enabled
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, this->GetCaption());

	// Destroy dialog
	SDialog::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RequestCloseDialog
//	Description:	Request current dialog to close
//  Arguments:		None
//  Return value:	LRESULT (0:Success, else:Failed)
//
//////////////////////////////////////////////////////////////////////////

LRESULT CEditScheduleDlg::RequestCloseDialog(void)
{
	// If data changed, ask for saving before closing dialog
	if (GetFlagValue(FLAGID_CHANGE_FLAG) == TRUE) {
		// Setup messagebox language
		LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		CString strMessage = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CONTENT);
		CString strMsgCaption = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CAPTION);

		int nConfirm = MessageBox(strMessage, strMsgCaption, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nConfirm == IDYES) {

			// Update data
			SaveScheduleItem();

			// Return UPDATE flag
			SetReturnFlag(RETFLAG_UPDATE);
		}
		else if (nConfirm == IDCANCEL) {
			// Request denied
			return LRESULT(RESULT_FAILED);
		}
	}
	else {
		// Return CLOSE flag
		SetReturnFlag(RETFLAG_CLOSE);
	}

	// Request accepted
	return SDialog::RequestCloseDialog();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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
		UINT nID = pWndChild->GetDlgCtrlID();

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupComboBox
//	Description:	Setup data for combo-boxes
//  Arguments:		nComboID	- ID of combo box
//					ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage)
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupActiveDayList
//	Description:	Initialize and setup language for Active Day list
//  Arguments:		ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::SetupActiveDayList(LANGTABLE_PTR ptrLanguage)
{
	// Get parent list frame rect
	CWnd* pListFrameWnd = GetDlgItem(IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX);
	if (pListFrameWnd == NULL) return;
	RECT rcListFrameWnd;
	pListFrameWnd->GetWindowRect(&rcListFrameWnd);
	ScreenToClient(&rcListFrameWnd);

	// Get frame size
	if (m_pszActiveTableFrameSize == NULL) {
		m_pszActiveTableFrameSize = new CSize();
		m_pszActiveTableFrameSize->cx = rcListFrameWnd.right - rcListFrameWnd.left;
		m_pszActiveTableFrameSize->cy = rcListFrameWnd.bottom - rcListFrameWnd.top;
	}

	// Initialization
	if (m_pActiveDayListTable == NULL) {
		m_pActiveDayListTable = new CGridCtrl();
	}

	// Create table
	if (m_pActiveDayListTable == NULL) return;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	m_pActiveDayListTable->Create(rcListFrameWnd, this, IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX, dwStyle);

	// Destroy frame
	pListFrameWnd->DestroyWindow();

	// Cell format
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pActiveDayListTable->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;
	pCell->SetFormat(pCell->GetFormat());
	pCell->SetMargin(0);
	pCell->SetBackClr(COLOR_WHITE);
	pCell->SetTextClr(COLOR_BLACK);
	pCell->SetHeight(GRIDCTRL_HEIGHT_ROW_EX);

	// Setup table
	m_pActiveDayListTable->SetColumnCount(2);
	m_pActiveDayListTable->SetRowCount(MAX_DAYS_OF_WEEK);

	// Draw table
	DrawActiveDayTable(GetReadOnlyMode());

	// Display table
	m_pActiveDayListTable->SetListMode(TRUE);
	m_pActiveDayListTable->SetEditable(FALSE);
	m_pActiveDayListTable->SetColumnResize(FALSE);
	m_pActiveDayListTable->SetRowResize(FALSE);
	m_pActiveDayListTable->EnableSelection(TRUE);
	m_pActiveDayListTable->SetSingleRowSelection(TRUE);
	m_pActiveDayListTable->SetSingleColSelection(FALSE);
	m_pActiveDayListTable->ShowWindow(SW_SHOW);
	m_pActiveDayListTable->SetRedraw(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DrawActiveDayTable
//	Description:	Draw active day list table
//  Arguments:		bReadOnly - Read-only mode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::DrawActiveDayTable(BOOL bReadOnly /* = FALSE */)
{
	// Check table validity
	if (m_pActiveDayListTable == NULL) return;

	// Check table format data validity
	if (m_pszActiveTableFrameSize == NULL) return;

	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;

	// Re-update default cell properties
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pActiveDayListTable->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;

	// Read-only mode --> Change cell color
	if (bReadOnly == TRUE) {
		pCell->SetBackClr(COLOR_BRIGHT_GRAY);
		pCell->SetTextClr(COLOR_DARK_GRAY);
	}
	else {
		pCell->SetBackClr(COLOR_WHITE);
		pCell->SetTextClr(COLOR_BLACK);
	}

	// Setup display size
	int nFrameHeight = m_pszActiveTableFrameSize->cy;
	int nFrameWidth = m_pszActiveTableFrameSize->cx;
	if (GetWindowsOSVersion() == WINDOWS_VERSION_10) {
		// Windows 10 list control offset
		nFrameWidth -= OFFSET_WIDTH_LISTCTRL_WIN10;
	}
	else {
		// Windows 11 list control offset
		nFrameWidth -= OFFSET_WIDTH_LISTCTRL;
	}
	if ((MAX_DAYS_OF_WEEK * GRIDCTRL_HEIGHT_ROW_EX) >= nFrameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nFrameWidth -= (nScrollBarWidth + OFFSET_WIDTH_VSCRLBR);
	}

	// Setup columns
	m_pActiveDayListTable->SetColumnWidth(COL_ID_CHECKBOX, COL_SIZE_CHECKBOX);
	m_pActiveDayListTable->SetColumnWidth(COL_ID_DAYTITLE, nFrameWidth - COL_SIZE_CHECKBOX);

	// Setup rows
	UINT nItemState = INT_NULL;
	for (int nRow = 0; nRow < MAX_DAYS_OF_WEEK; nRow++) {

		/*------------------------------------- Checkbox column -------------------------------------*/

		// Set cell type: Checkbox
		if (!m_pActiveDayListTable->SetCellType(nRow, COL_ID_CHECKBOX, RUNTIME_CLASS(CGridCellCheck)))
			continue;

		// Set cell checkbox placement: Centering
		CGridCellCheck* pCellCheck = (CGridCellCheck*)m_pActiveDayListTable->GetCell(nRow, COL_ID_CHECKBOX);
		if (pCellCheck == NULL) continue;
		pCellCheck->SetCheckPlacement(SCP_CENTERING);

		/*------------------------------------ Day title column -------------------------------------*/

		// Update cell state
		nItemState = m_pActiveDayListTable->GetItemState(nRow, COL_ID_DAYTITLE);
		if (!m_pActiveDayListTable->SetItemState(nRow, COL_ID_DAYTITLE, nItemState | GVIS_READONLY))
			continue;

		// Set cell alignment: Center
		CGridCellBase* pCell = (CGridCellBase*)m_pActiveDayListTable->GetCell(nRow, COL_ID_DAYTITLE);
		if (pCell == NULL) continue;
		pCell->SetFormat(pCell->GetFormat() | DT_CENTER);

		/*-------------------------------------------------------------------------------------------*/
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupDialogItemState
//	Description:	Setup properties and values for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::SetupDialogItemState()
{
	// Setup checkboxes
	m_bEnable = m_schScheduleItemTemp.bEnable;
	m_bRepeat = m_schScheduleItemTemp.IsRepeatEnable();

	// If is currently in read-only or view mode
	if ((GetReadOnlyMode() == TRUE) || (GetDispMode() == MODE_VIEW)) {
		// Disable top checkbox
		EnableItem(IDC_EDITSCHEDULE_ENABLE_CHK, FALSE);
	}

	// Enable/disable sub-items
	EnableSubItems(m_bEnable);

	// Setup action list combo value
	m_nAction = m_schScheduleItemTemp.nAction;
	if (m_pActionList != NULL) {
		m_pActionList->SetCurSel(Opt2Sel(APP_ACTION, m_nAction));
	}

	UpdateData(FALSE);

	// Setup time spin button properties
	int nTimeSpinPos;
	Time2SpinPos(m_schScheduleItemTemp.stTime, nTimeSpinPos);

	// Time spin initialization
	if (m_pTimeSpin == NULL) {
		m_pTimeSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_EDITSCHEDULE_TIME_SPIN);
		if (m_pTimeSpin == NULL) {
			TRCLOG("Error: Time spin control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Setup properties
	if (m_pTimeSpin != NULL) {
		m_pTimeSpin->SetBuddy(GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX));
		m_pTimeSpin->SetRange(TIMESPIN_MIN, TIMESPIN_MAX);
		m_pTimeSpin->SetPos(nTimeSpinPos);
	}

	// Setup time editbox
	UpdateTimeSetting(m_schScheduleItemTemp.stTime, FALSE);

	// Enable/disable active day table (also update its display)
	DisableActiveDayTable(!(m_schScheduleItemTemp.bEnable & m_schScheduleItemTemp.IsRepeatEnable()));

	// Disable save button at first
	EnableSaveButton(FALSE);

	// Read-only mode (if enabled)
	if (GetReadOnlyMode() == TRUE) {
		CWnd* pWndChild = GetTopWindow();
		while (pWndChild != NULL) {
			pWndChild->EnableWindow(FALSE);
			pWndChild = pWndChild->GetWindow(GW_HWNDNEXT);
		}
	}

	// Default
	SDialog::SetupDialogItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateActiveDayList
//	Description:	Update Active days list table
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::UpdateActiveDayList()
{
	// Check table validity
	if (m_pActiveDayListTable == NULL) return;

	// Load app language package
	LANGTABLE_PTR ptrLanguage = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Print items
	CString strTemp;
	int nDayOfWeekID = INT_INVALID;
	CGridCellCheck* pCellCheck = NULL;
	for (int nRowIndex = 0; nRowIndex < MAX_DAYS_OF_WEEK; nRowIndex++) {

		// Day of week
		nDayOfWeekID = nRowIndex;

		// Active state
		BOOL bActive = (m_schScheduleItemTemp.IsDayActive((DAYOFWEEK)nDayOfWeekID)) ? TRUE : FALSE;
		pCellCheck = (CGridCellCheck*)m_pActiveDayListTable->GetCell(nRowIndex, COL_ID_CHECKBOX);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(bActive);
		}

		// Day title
		strTemp = GetLanguageString(ptrLanguage, GetPairedID(idTableDayOfWeek, nDayOfWeekID));
		m_pActiveDayListTable->SetItemText(nRowIndex, COL_ID_DAYTITLE, strTemp);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisableActiveDayTable
//	Description:	Disable mouse click events for Active Days table
//  Arguments:		bDisable - Disable/enable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::DisableActiveDayTable(BOOL bDisable)
{
	// If is currently in read-only or view mode
	if ((GetReadOnlyMode() == TRUE) || (GetDispMode() == MODE_VIEW)) {
		// Force disable
		bDisable = TRUE;
	}

	// Redraw read-only style
	RedrawActiveDayTable(bDisable);

	// Check table validity
	if (m_pActiveDayListTable == NULL) return;

	// Disable/enable mouse events
	m_pActiveDayListTable->DisableMouseClick(bDisable);
	m_pActiveDayListTable->DisableMouseMove(bDisable);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RedrawActiveDayTable
//	Description:	Update and redraw Active day table
//  Arguments:		BOOL bReadOnly - Read-only mode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::RedrawActiveDayTable(BOOL bReadOnly /* = FALSE */)
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetScheduleItem
//	Description:	Get schedule item data
//  Arguments:		pschItem - Return item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::GetScheduleItem(PSCHEDULEITEM pschItem)
{
	// Copy data
	if (pschItem == NULL) return;
	pschItem->Copy(m_schScheduleItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetScheduleItem
//	Description:	Set schedule item data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::SetScheduleItem(const SCHEDULEITEM& pschItem)
{
	// Copy data
	m_schScheduleItem.Copy(pschItem);
	m_schScheduleItemTemp.Copy(m_schScheduleItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateScheduleItem
//	Description:	Update and refresh schedule settings
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::UpdateScheduleItem()
{
	// Update data
	UpdateData(TRUE);

	// Update checkbox values
	m_schScheduleItemTemp.bEnable = m_bEnable;
	m_schScheduleItemTemp.rpsRepeatSet.bRepeat = m_bRepeat;

	// Update action list combo value
	int nCurSel = m_pActionList->GetCurSel();
	m_nAction = Sel2Opt(APP_ACTION, nCurSel);
	m_schScheduleItemTemp.nAction = m_nAction;

	// Update time value
	SYSTEMTIME stTimeTemp;
	UpdateTimeSetting(stTimeTemp, TRUE);

	m_schScheduleItemTemp.stTime.wHour = stTimeTemp.wHour;
	m_schScheduleItemTemp.stTime.wMinute = stTimeTemp.wMinute;

	// Update active day table changes
	BYTE byRepeatDays = 0;
	CGridCellCheck* pCellCheckActive = NULL;
	if (m_pActiveDayListTable == NULL) return;
	for (int nRowIndex = 0; nRowIndex < MAX_DAYS_OF_WEEK; nRowIndex++) {
		// Get checkbox cell
		pCellCheckActive = (CGridCellCheck*)m_pActiveDayListTable->GetCell(nRowIndex, COL_ID_CHECKBOX);
		if (pCellCheckActive == NULL) continue;

		// Get checked states
		BOOL bActive = pCellCheckActive->GetCheck();

		// Update active days of week data
		byRepeatDays |= bActive << nRowIndex;
	}

	// Update active day data
	m_schScheduleItemTemp.rpsRepeatSet.byRepeatDays = byRepeatDays;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveScheduleItem
//	Description:	Update and save schedule settings
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::SaveScheduleItem()
{
	// Update settings
	UpdateScheduleItem();
	m_schScheduleItem.Copy(m_schScheduleItemTemp);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CheckDataChangeState
//	Description:	Compare control values with current config to check for changes
//  Arguments:		None
//  Return value:	BOOL - Flag to mark data changed or not
//
//////////////////////////////////////////////////////////////////////////

BOOL CEditScheduleDlg::CheckDataChangeState()
{
	// Update item
	UpdateScheduleItem();

	BOOL bChangeFlag = FALSE;

	bChangeFlag |= (m_schScheduleItemTemp.bEnable != m_schScheduleItem.bEnable);
	bChangeFlag |= (m_schScheduleItemTemp.nAction != m_schScheduleItem.nAction);

	bChangeFlag |= (m_schScheduleItemTemp.stTime.wHour != m_schScheduleItem.stTime.wHour);
	bChangeFlag |= (m_schScheduleItemTemp.stTime.wMinute != m_schScheduleItem.stTime.wMinute);

	bChangeFlag |= (m_schScheduleItemTemp.IsRepeatEnable() != m_schScheduleItem.IsRepeatEnable());
	bChangeFlag |= (m_schScheduleItemTemp.GetActiveDays() != m_schScheduleItem.GetActiveDays());

	return bChangeFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableSaveButton
//	Description:	Enable/disable save button
//  Arguments:		bEnable - Enable or disable button
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::EnableSaveButton(BOOL bEnable)
{
	// If is currently in read-only or view mode, do not enable
	if ((GetReadOnlyMode() == TRUE) || (GetDispMode() == MODE_VIEW)) {
		// Force disable
		bEnable = FALSE;
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableSubItems
//	Description:	Enable/disable sub items of function
//  Arguments:		bEnable - Enable or disable sub items
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::EnableSubItems(BOOL bEnable)
{
	// If is currently in read-only or view mode, do not enable
	if ((GetReadOnlyMode() == TRUE) || (GetDispMode() == MODE_VIEW)) {
		// Force disable
		bEnable = FALSE;
	}

	// Enable/disable items
	GetDlgItem(IDC_EDITSCHEDULE_ACTION_LABEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDITSCHEDULE_ACTION_LIST)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDITSCHEDULE_TIME_LABEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDITSCHEDULE_TIME_SPIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDITSCHEDULE_REPEATDAILY_CHK)->EnableWindow(bEnable);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateTimeSetting
//	Description:	Update time value from/to time edit control
//  Arguments:		stTime  - Time data
//					bUpdate - Update or not (YES/TRUE by default)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::UpdateTimeSetting(SYSTEMTIME& stTime, BOOL bUpdate /* = TRUE */)
{
	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Get time editbox pointer
	if (m_pTimeEdit == NULL) {
		m_pTimeEdit = (CEdit*)GetDlgItem(IDC_EDITSCHEDULE_TIME_EDITBOX);
		if (m_pTimeEdit == NULL) {
			TRCLOG("Error: Time edit control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	if (bUpdate == TRUE) {
		// Get value from time editbox
		CString strTimeFormat;
		m_pTimeEdit->GetWindowText(strTimeFormat);

		// Get hour value
		WORD wHour = (WORD)_tstoi(strTimeFormat.Left(2));
		CString strMiddayFlag = strTimeFormat.Right(2);
		if (strMiddayFlag == GetLanguageString(pLang, FORMAT_TIME_BEFOREMIDDAY)) {
			// Before midday
			stTime.wHour = wHour;
		}
		else if ((strMiddayFlag == GetLanguageString(pLang, FORMAT_TIME_AFTERMIDDAY)) && wHour < 12) {
			// After midday
			stTime.wHour = wHour + 12;
		}
		else {
			// Keep value
			stTime.wHour = wHour;
		}

		// Get minute value
		stTime.wMinute = (WORD)_tstoi(strTimeFormat.Mid(3, 2));
	}
	else {
		// Set value for time editbox
		CString strTimeFormat;
		strTimeFormat = FormatDispTime(pLang, IDS_FORMAT_SHORTTIME, stTime);
		m_pTimeEdit->SetWindowText(strTimeFormat);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetReturnFlag
//	Description:	Get dialog return flag
//  Arguments:		None
//  Return value:	int - Return flag
//
//////////////////////////////////////////////////////////////////////////

int CEditScheduleDlg::GetReturnFlag(void) const
{
	return m_nRetFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetReturnFlag
//	Description:	Set dialog return flag
//  Arguments:		nRetFlag - Return flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::SetReturnFlag(int nRetFlag)
{
	m_nRetFlag = nRetFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDispMode
//	Description:	Get dialog display mode
//  Arguments:		None
//  Return value:	int - Display mode
//
//////////////////////////////////////////////////////////////////////////

int CEditScheduleDlg::GetDispMode(void) const
{
	return m_nDispMode;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDispMode
//	Description:	Set dialog display mode
//  Arguments:		nMode - Display mode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::SetDispMode(int nMode)
{
	m_nDispMode = nMode;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnApply
//	Description:	Handle clicking event for [Save] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::OnApply()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EDITSCHEDULE_APPLY_BTN);

	// Save data if changed
	if (GetFlagValue(FLAGID_CHANGE_FLAG) == TRUE) {

		// Update data
		SaveScheduleItem();

		// Return UPDATE flag
		SetReturnFlag(RETFLAG_UPDATE);
	}
	else {
		// Return OK flag
		SetReturnFlag(RETFLAG_OK);
	}

	// Close dialog
	SDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnExit
//	Description:	Handle clicking event for [Exit] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::OnExit()
{
	// If not forced closing by request
	if (!IsForceClosingByRequest()) {

		// Save app event log if enabled
		OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EDITSCHEDULE_CANCEL_BTN);

		// If data changed, ask for saving before closing dialog
		if (GetFlagValue(FLAGID_CHANGE_FLAG) == TRUE) {
			// Setup messagebox language
			LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
			CString strMessage = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CONTENT);
			CString strMsgCaption = GetLanguageString(pAppLang, MSGBOX_EDITSCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int nConfirm = MessageBox(strMessage, strMsgCaption, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {

				// Update data
				SaveScheduleItem();

				// Return UPDATE flag
				SetReturnFlag(RETFLAG_UPDATE);
			}
			else {
				// Return CANCEL flag
				SetReturnFlag(RETFLAG_CANCEL);
			}
		}
	}
	else {
		// Return CANCEL flag
		SetReturnFlag(RETFLAG_CANCEL);
	}

	// Close dialog
	SDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnEnableSchedule
//	Description:	Handle clicking event for [EnableSchedule] checkbox
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::OnEnableSchedule()
{
	// Update dialog item state
	UpdateData(TRUE);
	EnableSubItems(m_bEnable);

	// Check for data change
	m_bChangeFlag = CheckDataChangeState();

	// Enable/disable active day table
	DisableActiveDayTable(!(m_schScheduleItemTemp.bEnable & m_schScheduleItemTemp.IsRepeatEnable()));

	// Enable/disable save button
	EnableSaveButton(m_bChangeFlag);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnChangeAction
//	Description:	Update when action list selection changes
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::OnChangeAction()
{
	// Save app event log if enabled
	int nSel = m_pActionList->GetCurSel();
	OutputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_EDITSCHEDULE_ACTION_LIST);

	// Check for value change and enable/disable save button
	m_bChangeFlag = CheckDataChangeState();
	EnableSaveButton(m_bChangeFlag);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnChangeRepeatDaily
//	Description:	Update when repeat checkbox changes
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::OnChangeRepeatDaily()
{
	// Check for data change
	m_bChangeFlag = CheckDataChangeState();

	// Enable/disable active day table
	DisableActiveDayTable(!m_schScheduleItemTemp.IsRepeatEnable());

	// Enable/disable save button
	EnableSaveButton(m_bChangeFlag);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnTimeEditSetFocus
//	Description:	Select all text when editbox is set focus
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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
			TRCLOG("Error: Time edit control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Select all text
	m_pTimeEdit->PostMessage(EM_SETSEL, 0, -1);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnTimeEditKillFocus
//	Description:	Update when time editbox is killed focus
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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
			TRCLOG("Error: Time edit control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Update data
	CString strText;
	m_pTimeEdit->GetWindowText(strText);

	SYSTEMTIME stTimeTemp;
	BOOL bRet = Text2Time(stTimeTemp, strText);
	if (bRet != FALSE) {
		// Update new time value
		UpdateTimeSetting(stTimeTemp, FALSE);
		
		// Update timespin new position
		int nSpinPos;
		Time2SpinPos(stTimeTemp, nSpinPos);
		if (m_pTimeSpin != NULL) {
			m_pTimeSpin->SetPos(nSpinPos);
		}
	}
	else {
		// Restore old time value
		UpdateTimeSetting(m_schScheduleItemTemp.stTime, FALSE);
		return;
	}

	// Check for value change and enable/disable save button
	m_bChangeFlag = CheckDataChangeState();
	EnableSaveButton(m_bChangeFlag);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnTimeSpinChange
//	Description:	Update when time spin value changes
//  Arguments:		pNMDHR  - Default of spin event handler
//					pResult - Default of spin event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::OnTimeSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// Get timespin position and convert to time value
	int nPos = pNMUpDown->iPos;
	SYSTEMTIME stTimeTemp;
	SpinPos2Time(stTimeTemp, nPos);
	UpdateTimeSetting(stTimeTemp, FALSE);

	*pResult = NULL;

	// Check for value change and enable/disable save button
	m_bChangeFlag = CheckDataChangeState();
	EnableSaveButton(m_bChangeFlag);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClickActiveDayList
//	Description:	Handle click event on Active days of week table
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::OnClickActiveDayList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Success (return 0)
	*pResult = NULL;

	// Update data (also check change state)
	m_bChangeFlag = CheckDataChangeState();

	// Enable/disable save button
	EnableSaveButton(m_bChangeFlag);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRightClickActiveDayList
//	Description:	Handle right click event on Active days of week table
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CEditScheduleDlg::OnRightClickActiveDayList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Success (return 0)
	*pResult = NULL;

	// Update data (also check change state)
	m_bChangeFlag = CheckDataChangeState();

	// Enable/disable save button
	EnableSaveButton(m_bChangeFlag);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WindowProc
//	Description:	Pre-handle dialog messages
//  Arguments:		message - Message ID
//					wParam	- First param (HIWORD)
//					lParam	- Second param (LOWORD)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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
				m_pTimeEdit->SetSel(0, -1, TRUE);
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

