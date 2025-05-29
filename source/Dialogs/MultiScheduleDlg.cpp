
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		MultiScheduleDlg.cpp
//		Description:	Source file for Schedule dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2017.03.08:		Create new
//						<1> 2024.01.27:		Update to version 3.0
//						<2> 2024.07.06:		Update to version 3.1
//						<3> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PowerPlus.h"
#include "PowerPlusDlg.h"
#include "Dialogs\MultiScheduleDlg.h"
#include "Dialogs\EditScheduleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MapTable;
using namespace Language;
using namespace AppCore;
using namespace AppRegistry;


////////////////////////////////////////////////////////
//
//	Define macros for data list table
//
////////////////////////////////////////////////////////

#define COL_FIXED_NUM			1
#define ROW_FIXED_NUM			1
#define ROW_INDEX_DEFAULT		1
#define ROW_INDEX_EXTRASTART	2


////////////////////////////////////////////////////////
//
//	Implement methods for CMultiScheduleDlg
//
////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMultiScheduleDlg, SDialog)

////////////////////////////////////////////////////////
// 
//	Function name:	CMultiScheduleDlg
//	Description:	Constructor
//
////////////////////////////////////////////////////////

CMultiScheduleDlg::CMultiScheduleDlg() : SDialog(IDD_MULTISCHEDULE_DLG)
{
	// Initialize member variables

	// Dialog control item
	m_pDataItemListTable = NULL;

	// Child dialog
	m_pEditScheduleDlg = NULL;

	// Data container variables
	m_schSchedule.Init();
	m_schScheduleTemp.Init();

	// Table format and properties
	m_nColNum = 0;
	m_apGrdColFormat = NULL;
	m_pszDataTableFrameSize = NULL;

	// Other variables
	m_nCurMode = 0;
	m_nCheckCount = 0;
	m_nCurSelIndex = -1;
	m_nCurDispIndex = -2;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CMultiScheduleDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CMultiScheduleDlg::~CMultiScheduleDlg()
{
	// Delete child dialog
	if (m_pEditScheduleDlg != NULL) {
		// Destroy dialog
		m_pEditScheduleDlg->DestroyWindow();
		delete m_pEditScheduleDlg;
		m_pEditScheduleDlg = NULL;
	}

	// Grid table list control
	if (m_pDataItemListTable) {
		delete m_pDataItemListTable;
		m_pDataItemListTable = NULL;
	}

	// Remove schedule data
	m_schSchedule.DeleteAll();
	m_schScheduleTemp.DeleteAll();

	// Other variables
	if (m_apGrdColFormat != NULL) {
		delete[] m_apGrdColFormat;
		m_apGrdColFormat = NULL;
	}
	if (m_pszDataTableFrameSize != NULL) {
		delete m_pszDataTableFrameSize;
		m_pszDataTableFrameSize = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegisterDialogManagement
//	Description:	Register dialog control management
//  Arguments:		None
//  Return value:	INT_PTR - Number of controls added to management
//
//////////////////////////////////////////////////////////////////////////

INT_PTR CMultiScheduleDlg::RegisterDialogManagement(void)
{
	INT_PTR nRet = SDialog::RegisterDialogManagement();
	if (nRet != 0) {
		TRACE_ERROR("Error: Register dialog management failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return nRet;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_ITEM_LISTBOX, List_Control);
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_ADD_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_EDIT_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_REMOVE_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_REMOVEALL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_CHECKALL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_UNCHECKALL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_VIEWDETAILS_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_SETDEFAULT_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_APPLY_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_MULTISCHEDULE_CANCEL_BTN, Button);
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

BOOL CMultiScheduleDlg::UnregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_ITEM_LISTBOX);
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_ADD_BTN);
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_EDIT_BTN);
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_REMOVE_BTN);
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_REMOVEALL_BTN);
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_CHECKALL_BTN);
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_UNCHECKALL_BTN);
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_VIEWDETAILS_BTN);
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_SETDEFAULT_BTN);
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_APPLY_BTN);
		pCtrlMan->RemoveControl(IDC_MULTISCHEDULE_CANCEL_BTN);
	}

	return SDialog::UnregisterDialogManagement();
}

//////////////////////////////////////////////////////////////////////////
//
//	CMultiScheduleDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCEID_MAP(CMultiScheduleDlg)
	ON_ID_DIALOG(IDD_MULTISCHEDULE_DLG,				 "ScheduleDlg")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_ITEM_LISTBOX,	 "ScheduleItemList")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_ADD_BTN,		 "AddButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_EDIT_BTN,		 "EditButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_REMOVE_BTN,		 "RemoveButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_REMOVEALL_BTN,	 "RemoveAllButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_CHECKALL_BTN,	 "CheckAllButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_UNCHECKALL_BTN,	 "UncheckAllButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_VIEWDETAILS_BTN, "ViewDetailsButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_SETDEFAULT_BTN,	 "SetDefaultButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_APPLY_BTN,		 "SaveButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_CANCEL_BTN,		 "CancelButton")
END_RESOURCEID_MAP()

//////////////////////////////////////////////////////////////////////////
//
//	CMultiScheduleDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMultiScheduleDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MULTISCHEDULE_APPLY_BTN,					&CMultiScheduleDlg::OnApply)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_CANCEL_BTN,					&CMultiScheduleDlg::OnExit)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_ADD_BTN,					&CMultiScheduleDlg::OnAdd)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_EDIT_BTN,					&CMultiScheduleDlg::OnEdit)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_REMOVE_BTN,					&CMultiScheduleDlg::OnRemove)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_REMOVEALL_BTN,				&CMultiScheduleDlg::OnRemoveAll)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_CHECKALL_BTN,				&CMultiScheduleDlg::OnCheckAll)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_UNCHECKALL_BTN,				&CMultiScheduleDlg::OnUncheckAll)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_VIEWDETAILS_BTN,			&CMultiScheduleDlg::OnViewDetails)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_SETDEFAULT_BTN,				&CMultiScheduleDlg::OnSetDefault)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MULTISCHEDULE_ITEM_LISTBOX,	&CMultiScheduleDlg::OnSelectScheduleItem)
	ON_NOTIFY(NM_CLICK, IDC_MULTISCHEDULE_ITEM_LISTBOX,			&CMultiScheduleDlg::OnClickDataItemList)
	ON_NOTIFY(NM_RCLICK, IDC_MULTISCHEDULE_ITEM_LISTBOX,		&CMultiScheduleDlg::OnRightClickDataItemList)
	ON_MESSAGE(SCM_NOTIFY_DIALOG_DESTROY,						&CMultiScheduleDlg::OnChildDialogDestroy)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementation

// CMultiScheduleDlg message handlers

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize Schedule dialog and setup dialog content
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CMultiScheduleDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Do not use Enter button
	SetUseEnter(FALSE);

	// Register message box caption
	RegisterMessageBoxCaption(MSGBOX_MULTISCHEDULE_CAPTION);

	// Load data
	LoadScheduleSettings();

	// Init dialog items
	LoadLayoutInfo();
	SetupLanguage();
	SetupDialogItemState();

	// Update data
	UpdateDataItemList();
	RefreshDialogItemState(TRUE);

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

void CMultiScheduleDlg::OnClose()
{
	// If not forced closing by request
	if (!IsForceClosingByRequest()) {

		// If data changed, ask for saving before closing dialog
		if (GetFlagValue(AppFlagID::dialogDataChanged) == TRUE) {
			// Setup messagebox language
			LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
			CString strMessage = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
			CString strMsgCaption = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int nConfirm = MessageBox(strMessage, strMsgCaption, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Save data
				SaveScheduleSettings();
			}
		}
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

void CMultiScheduleDlg::OnDestroy()
{
	// Save app event log if enabled
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, this->GetCaption());

	// Save layout info data
	UpdateLayoutInfo();
	SaveLayoutInfo();

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

LRESULT CMultiScheduleDlg::RequestCloseDialog(void)
{
	// If detail edit dialog is opening
	if (m_pEditScheduleDlg != NULL) {
		// Request close dialog
		LRESULT resCloseReq = m_pEditScheduleDlg->RequestCloseDialog();
		if (resCloseReq != RESULT_SUCCESS)
			return resCloseReq;
	}

	// If data changed, ask for saving before closing dialog
	if (GetFlagValue(AppFlagID::dialogDataChanged) == TRUE) {
		// Setup messagebox language
		LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		CString strMessage = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
		CString strMsgCaption = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

		int nConfirm = MessageBox(strMessage, strMsgCaption, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Save data
			SaveScheduleSettings();
		}
		else if (nConfirm == IDCANCEL) {
			// Request denied
			return LRESULT(RESULT_FAILED);
		}
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

void CMultiScheduleDlg::SetupLanguage()
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
		case IDC_MULTISCHEDULE_ITEM_LISTBOX:
			// Skip these items
			break;
		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}

	// Setup Schedule data item list
	SetupDataItemList(pAppLang);

	// Default
	SDialog::SetupLanguage();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupDataItemList
//	Description:	Initialize and setup language for Schedule item list
//  Arguments:		ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::SetupDataItemList(LANGTABLE_PTR ptrLanguage)
{
	// Get parent list frame rect
	CWnd* pListFrameWnd = GetDlgItem(IDC_MULTISCHEDULE_ITEM_LISTBOX);
	if (pListFrameWnd == NULL) return;
	RECT rcListFrameWnd;
	pListFrameWnd->GetWindowRect(&rcListFrameWnd);
	ScreenToClient(&rcListFrameWnd);

	// Get frame size
	if (m_pszDataTableFrameSize == NULL) {
		m_pszDataTableFrameSize = new CSize();
		m_pszDataTableFrameSize->cx = rcListFrameWnd.right - rcListFrameWnd.left;
		m_pszDataTableFrameSize->cy = rcListFrameWnd.bottom - rcListFrameWnd.top;
	}

	// Initialization
	if (m_pDataItemListTable == NULL) {
		m_pDataItemListTable = new CGridCtrl();
	}

	// Create table
	if (m_pDataItemListTable == NULL) return;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	m_pDataItemListTable->Create(rcListFrameWnd, this, IDC_MULTISCHEDULE_ITEM_LISTBOX, dwStyle);

	// Destroy frame
	pListFrameWnd->DestroyWindow();

	// Cell format
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pDataItemListTable->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;
	pCell->SetFormat(pCell->GetFormat());
	pCell->SetMargin(0);
	pCell->SetBackClr(COLOR_WHITE);
	pCell->SetTextClr(COLOR_BLACK);
	pCell->SetHeight(GRIDCTRL_HEIGHT_ROW_EX);

	// Table format and properties
	int nRowNum = (GetTotalItemNum() + ROW_FIXED_NUM);
	int nColNum = m_nColNum;

	// Setup table
	m_pDataItemListTable->SetColumnCount(nColNum);
	m_pDataItemListTable->SetFixedColumnCount(COL_FIXED_NUM);
	m_pDataItemListTable->SetRowCount(nRowNum);
	m_pDataItemListTable->SetFixedRowCount(ROW_FIXED_NUM);
	m_pDataItemListTable->SetRowHeight(GRIDCTRL_INDEX_HEADER_ROW, GRIDCTRL_HEIGHT_HEADER);

	// Draw table
	DrawDataTable(GetReadOnlyMode());

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DrawDataTable
//	Description:	Draw Schedule data list table
//  Arguments:		bReadOnly - Read-only mode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::DrawDataTable(BOOL bReadOnly /* = FALSE */)
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Check table format data validity
	if (m_pszDataTableFrameSize == NULL) return;

	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;

	// Get app language package
	LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();

	// Re-update default cell properties
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pDataItemListTable->GetDefaultCell(FALSE, FALSE);
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
	
	// Table properties
	int nColNum = m_nColNum;
	int nRowNum = (GetTotalItemNum() + ROW_FIXED_NUM);

	// Setup display size
	int nFrameHeight = m_pszDataTableFrameSize->cy;
	int nFrameWidth = m_pszDataTableFrameSize->cx;
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
	if ((GRIDCTRL_HEIGHT_HEADER + ((nRowNum - 1) * GRIDCTRL_HEIGHT_ROW_EX)) >= nFrameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nFrameWidth -= (nScrollBarWidth + OFFSET_WIDTH_VSCRLBR);
	}

	// Setup columns
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Set header row style
		SetFixedCellStyle(m_pDataItemListTable, GRIDCTRL_INDEX_HEADER_ROW, nCol);

		// Column header title
		CString strHdrTitle = STRING_EMPTY;
		UINT nHeaderTitleID = m_apGrdColFormat[nCol].nHeaderTitleID;
		if (nHeaderTitleID != INT_NULL) {
			strHdrTitle = GetLanguageString(ptrLanguage, nHeaderTitleID);
		}
		m_pDataItemListTable->SetItemText(GRIDCTRL_INDEX_HEADER_ROW, nCol, strHdrTitle);

		// Column width
		int nColWidth = m_apGrdColFormat[nCol].nWidth;
		if (nColWidth != -1) {
			// Set column width as defined
			if (m_pDataItemListTable->SetColumnWidth(nCol, nColWidth)) {
				// Calculate remaining width
				nFrameWidth -= nColWidth;
			}
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

			// Highlight default schedule item
			if ((nRow == ROW_INDEX_DEFAULT) && (nColStyle != COLSTYLE_FIXED) && (bReadOnly != TRUE)) {
				CGridCellBase* pCellBase = m_pDataItemListTable->GetCell(nRow, nCol);
				if (pCellBase != NULL) {
					pCellBase->SetBackClr(COLOR_YELLOW);
					pCellBase->SetTextClr(COLOR_RED);
				}
			}

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateLayoutInfo
//	Description:	Update layout info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::UpdateLayoutInfo(void)
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadLayoutInfo
//	Description:	Load layout info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::LoadLayoutInfo(void)
{
	// Define default table columns format
	const GRIDCTRLCOLFORMAT arrGrdColFormat[] = {
	//-----------ID------------------------Header title ID---------------Width(px)---Column style--------Align Center---
		{	ColumnID::Index,		GRIDCOLUMN_MULTISCHEDULE_INDEX,			26,		COLSTYLE_FIXED,			TRUE,	},
		{	ColumnID::EnableState,	GRIDCOLUMN_MULTISCHEDULE_STATE,			52,		COLSTYLE_CHECKBOX,		TRUE,	},
		{	ColumnID::ActionID,		GRIDCOLUMN_MULTISCHEDULE_ACTIONID,		140,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	ColumnID::TimeValue,	GRIDCOLUMN_MULTISCHEDULE_TIMEVALUE,		135,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	ColumnID::Repeat,		GRIDCOLUMN_MULTISCHEDULE_REPEAT,		-1,		COLSTYLE_CHECKBOX,		TRUE,	},
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
	CString strKeyName;
	for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
		strKeyName.Format(IDS_REGKEY_LAYOUT_GRIDCOLUMNSIZE, nIndex);
		if (GetLayoutInfo(IDS_REGSECTION_LAYOUT_MULTISCHEDULE, strKeyName, nRet)) {
			if (m_apGrdColFormat != NULL) {
				m_apGrdColFormat[nIndex].nWidth = nRet;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveLayoutInfo
//	Description:	Save layout info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::SaveLayoutInfo(void)
{
	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Save layout info data to registry
	int nRef = 0;
	CString strKeyName;
	for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
		nRef = m_apGrdColFormat[nIndex].nWidth;
		strKeyName.Format(IDS_REGKEY_LAYOUT_GRIDCOLUMNSIZE, nIndex);
		WriteLayoutInfo(IDS_REGSECTION_LAYOUT_MULTISCHEDULE, strKeyName, nRef);
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

void CMultiScheduleDlg::SetupDialogItemState()
{
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
//	Function name:	UpdateDataItemList
//	Description:	Update Schedule data item list table
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::UpdateDataItemList()
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// If there's no item, do nothing
	int nItemNum = GetTotalItemNum();
	if (nItemNum <= 0)
		return;

	// Load app language package
	LANGTABLE_PTR ptrLanguage = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Print items
	CString strTemp;
	int nTemp = -1;
	int nExtraItemIndex = 0;
	CGridCellCheck* pCellCheck = NULL;
	for (int nRowIndex = ROW_INDEX_DEFAULT; nRowIndex <= nItemNum; nRowIndex++) {
		
		// Get schedule item
		Item schItem;
		if (nRowIndex == ROW_INDEX_DEFAULT) {
			// Get schedule default item
			schItem = m_schScheduleTemp.GetDefaultItem();
		}
		else {
			// Get schedule extra item
			nExtraItemIndex = nRowIndex - ROW_INDEX_EXTRASTART;
			schItem = m_schScheduleTemp.GetItemAt(nExtraItemIndex);
		}

		// If schedule item is empty
		if (schItem.IsEmpty()) continue;

		// Item index
		strTemp.Format(_T("%d"), nRowIndex);
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::Index, strTemp);

		// Enable state
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::EnableState);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(schItem.IsEnabled());
		}

		// Action name
		nTemp = GetPairedID(IDTable::ActionName, schItem.GetAction());
		strTemp = GetLanguageString(ptrLanguage, nTemp);
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::ActionID, strTemp);

		// Time setting
		CString strFormat = GetLanguageString(ptrLanguage, GRIDCOLUMN_MULTISCHEDULE_TIMEFORMAT);
		strTemp = FormatDispTime(ptrLanguage, strFormat, schItem.GetTime());
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::TimeValue, strTemp);

		// Repeat
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::Repeat);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(schItem.IsRepeatEnabled());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisableDataTable
//	Description:	Disable mouse click events for Schedule data table
//  Arguments:		bDisable - Disable/enable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::DisableDataTable(BOOL bDisable)
{
	// Redraw read-only style
	RedrawDataTable(bDisable);

	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Disable/enable mouse events
	m_pDataItemListTable->DisableMouseClick(bDisable);
	m_pDataItemListTable->DisableMouseMove(bDisable);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RedrawDataTable
//	Description:	Update and redraw Schedule data table
//  Arguments:		BOOL bReadOnly - Read-only mode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::RedrawDataTable(BOOL bReadOnly /* = FALSE */)
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Update new row number
	int nCurRowNum = (GetTotalItemNum() + ROW_FIXED_NUM);
	m_pDataItemListTable->SetRowCount(nCurRowNum);

	// Draw table
	DrawDataTable(bReadOnly);

	// Update table data
	UpdateDataItemList();

	// Trigger redrawing table
	m_pDataItemListTable->RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RefreshDialogItemState
//	Description:	Refresh and update state for dialog items
//  Arguments:		bRecheckState - Recheck all item's state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::RefreshDialogItemState(BOOL bRecheckState /* = FALSE */)
{
	CWnd* pBtn = NULL;

	// If dialog items are being locked, do nothing
	if (GetLockState() == TRUE)
		return;

	// Check if any item is selected or not
	BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	// Check if selected item is an extra item or not
	BOOL bIsExtraSelected = ((bIsSelected == TRUE) && ((m_nCurSelIndex + ROW_FIXED_NUM) >= ROW_INDEX_EXTRASTART));

	// Check if number of extra item has reached the limit
	BOOL bIsMaxNum = (GetExtraItemNum() >= DEF_SCHEDULE_MAX_ITEMNUM);

	// Check if data is all empty or not
	BOOL bIsAllEmpty = m_schScheduleTemp.IsAllEmpty();

	// Get app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Disable [Add] button if item number has reached the limit
	pBtn = GetDlgItem(IDC_MULTISCHEDULE_ADD_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(!bIsMaxNum);
		pBtn->SetWindowText(GetLanguageString(pAppLang, IDC_MULTISCHEDULE_ADD_BTN));
	}

	// Disable [Remove/Remove All] buttons if data is all empty
	pBtn = GetDlgItem(IDC_MULTISCHEDULE_REMOVE_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(!bIsAllEmpty);

		// Enable [Remove] button if any item is selected
		pBtn->EnableWindow(bIsSelected);
	}
	pBtn = GetDlgItem(IDC_MULTISCHEDULE_REMOVEALL_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(!bIsAllEmpty);
	}

	// Enable [Edit] and [View Details] button if any item is selected
	pBtn = GetDlgItem(IDC_MULTISCHEDULE_EDIT_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(bIsSelected);
		pBtn->SetWindowText(GetLanguageString(pAppLang, IDC_MULTISCHEDULE_EDIT_BTN));
	}
	pBtn = GetDlgItem(IDC_MULTISCHEDULE_VIEWDETAILS_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(bIsSelected);
	}

	// Enable [Set Default] button if any extra item is selected
	pBtn = GetDlgItem(IDC_MULTISCHEDULE_SETDEFAULT_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(bIsExtraSelected);
	}

	// Check if data is changed or not
	BOOL bIsChanged = CheckDataChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);

	// Enable [Apply] button if data is changed
	pBtn = GetDlgItem(IDC_MULTISCHEDULE_APPLY_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(bIsChanged);
	}

	// Update [Check/Uncheck All] button state
	UpdateCheckAllBtnState(bRecheckState);

	// Default
	SDialog::RefreshDialogItemState(bRecheckState);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateCheckAllBtnState
//	Description:	Refresh and update state for [Check/Uncheck All] button
//  Arguments:		bRecheck - Recheck all items enable state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::UpdateCheckAllBtnState(BOOL bRecheck /* = FALSE */)
{
	// If dialog items are being locked, do nothing
	if (GetLockState() == TRUE)
		return;

	// Get buttons
	CWnd* pCheckAllBtn = GetDlgItem(IDC_MULTISCHEDULE_CHECKALL_BTN);
	CWnd* pUncheckAllBtn = GetDlgItem(IDC_MULTISCHEDULE_UNCHECKALL_BTN);
	if ((pCheckAllBtn == NULL) || (pUncheckAllBtn == NULL))
		return;

	// Get number of items
	int nItemNum = GetTotalItemNum();
	if (nItemNum == 0) {
		// Disable both [Check/Uncheck All] buttons
		pCheckAllBtn->EnableWindow(FALSE);
		pUncheckAllBtn->EnableWindow(FALSE);
		return;
	}

	// Recheck all items state
	if (bRecheck == TRUE) {

		// Reset counter
		m_nCheckCount = 0;
		// Check default item
		if (m_schScheduleTemp.GetDefaultItem().IsEnabled() == TRUE) {
			// Increase counter
			m_nCheckCount++;
		}
		// Check extra items
		for (int nExtraIndex = 0; nExtraIndex < GetExtraItemNum(); nExtraIndex++) {
			const Item& schTemp = m_schScheduleTemp.GetItemAt(nExtraIndex);
			if (schTemp.IsEnabled() == TRUE) {
				// Increase counter
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadScheduleSettings
//	Description:	Load schedule settings from configurations
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CMultiScheduleDlg::LoadScheduleSettings()
{
	// Get app Schedule data pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp == NULL) return FALSE;
	Data* pschData = pApp->GetAppScheduleData();
	if (pschData == NULL)
		return FALSE;

	// Copy data
	m_schSchedule.Copy(*pschData);
	m_schScheduleTemp.Copy(m_schSchedule);

	// Reset change flag
	SetFlagValue(AppFlagID::dialogDataChanged, FALSE);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveScheduleSettings
//	Description:	Save schedule settings to configurations
//  Arguments:		None
//  Return value:	BOOL - Result of saving process
//
//////////////////////////////////////////////////////////////////////////

BOOL CMultiScheduleDlg::SaveScheduleSettings()
{
	// Update settings
	m_schSchedule.Copy(m_schScheduleTemp);

	// Save app schedule data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp == NULL) return FALSE;
	pApp->SetAppScheduleData(&m_schSchedule);
	pApp->SaveRegistryAppData(APPDATA_SCHEDULE);

	// Notify main dialog to re-update schedule data
	CPowerPlusDlg* pMainDlg = (CPowerPlusDlg*)(pApp->GetMainWnd());
	VERIFY(pMainDlg != NULL);
	if (pMainDlg == NULL) return FALSE;
	pMainDlg->PostMessage(SM_APP_UPDATE_SCHEDULEDATA, NULL, NULL);

	// Reset change flag
	SetFlagValue(AppFlagID::dialogDataChanged, FALSE);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateScheduleSettings
//	Description:	Update and refresh schedule settings
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::UpdateScheduleSettings()
{
	// Update enable and repeat states of each item
	int nExtraItemIndex = 0;
	CGridCellCheck* pCellCheckEnable = NULL;
	CGridCellCheck* pCellCheckRepeat = NULL;
	int nItemRowNum = (m_pDataItemListTable->GetRowCount() - ROW_FIXED_NUM);
	for (int nRowIndex = ROW_INDEX_DEFAULT; nRowIndex <= nItemRowNum; nRowIndex++) {
		
		// In case of extra item rows
		if (nRowIndex > ROW_INDEX_DEFAULT) {
			// Get extra item index
			nExtraItemIndex = nRowIndex - ROW_INDEX_EXTRASTART;
		}

		// Get checkbox cells
		pCellCheckEnable = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::EnableState);
		pCellCheckRepeat = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::Repeat);
		if ((pCellCheckEnable == NULL) || (pCellCheckRepeat == NULL)) continue;

		// Get checked states
		BOOL bEnabled = pCellCheckEnable->GetCheck();
		BOOL bRepeat = pCellCheckRepeat->GetCheck();

		// Update item enable and repeat states
		if (nRowIndex == ROW_INDEX_DEFAULT) {
			// Update default item data
			Item& schTempDefault = m_schScheduleTemp.GetDefaultItem();
			schTempDefault.EnableItem(bEnabled);
			schTempDefault.EnableRepeat(bRepeat);
		}
		else {
			// Update extra item data
			Item& schTempItem = m_schScheduleTemp.GetItemAt(nExtraItemIndex);
			schTempItem.EnableItem(bEnabled);
			schTempItem.EnableRepeat(bRepeat);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CheckDataChangeState
//	Description:	Compare control values with current config to check for changes
//  Arguments:		None
//  Return value:	BOOL - Flag to mark data changed or not
//
//////////////////////////////////////////////////////////////////////////

BOOL CMultiScheduleDlg::CheckDataChangeState()
{
	BOOL bChangeFlag = FALSE;

	// Update settings
	UpdateScheduleSettings();

	// Check if number of extra items changed
	int nExtraItemNum = m_schScheduleTemp.GetExtraItemNum();
	bChangeFlag |= (nExtraItemNum != m_schSchedule.GetExtraItemNum());
	if (bChangeFlag == TRUE)
		return bChangeFlag;

	// Check if default item's data changed
	bChangeFlag |= (m_schScheduleTemp.GetDefaultItem().Compare(m_schSchedule.GetDefaultItem()) != TRUE);
	if (bChangeFlag == TRUE)
		return bChangeFlag;

	// Check if each extra item's data changed
	for (int nExtraIndex = 0; nExtraIndex < nExtraItemNum; nExtraIndex++) {
		// Get current item and temp item
		const Item& schCurItem = m_schSchedule.GetItemAt(nExtraIndex);
		const Item& schTempItem = m_schScheduleTemp.GetItemAt(nExtraIndex);

		// Data comparison
		bChangeFlag |= (schTempItem.Compare(schCurItem) != TRUE);

		// Stop on the first different item encountered
		if (bChangeFlag == TRUE) break;
	}

	return bChangeFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add item into schedule data
//  Arguments:		schItem - Schedule item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::Add(Item& schItem)
{
	// If item is empty, do nothing
	if (schItem.IsEmpty())
		return;

	// Check data validity
	BOOL bValid = Validate(schItem, TRUE);
	if (bValid == FALSE)
		return;

	// Update item to Action Schedule data item list
	DWORD dwRet = m_schScheduleTemp.Update(schItem);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Update
//	Description:	Update schedule item
//  Arguments:		schItem - Schedule item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::Update(Item& schItem)
{
	// If item is empty, do nothing
	if (schItem.IsEmpty())
		return;

	// Check data validity
	BOOL bValid = Validate(schItem, TRUE);
	if (bValid == FALSE)
		return;

	// Update item to Action Schedule data item list
	DWORD dwRet = m_schScheduleTemp.Update(schItem);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Remove
//	Description:	Remove a schedule item by index
//  Arguments:		nIndex - Index of item to remove
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::Remove(int nIndex)
{
	// Remove item at index
	m_schScheduleTemp.Delete(nIndex);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all schedule items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::RemoveAll()
{
	// Remove all items
	m_schScheduleTemp.DeleteExtra();

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAllItemState
//	Description:	Check/uncheck all Schedule data items
//  Arguments:		bState - Item state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::SetAllItemState(BOOL bState)
{
	// Check/uncheck all --> Update all items enable state
	Item& schDefTemp = m_schScheduleTemp.GetDefaultItem();
	if (schDefTemp.IsEnabled() != bState) {
		// Change checked state
		schDefTemp.EnableItem(bState);
	}
	for (int nExtraIndex = 0; nExtraIndex < GetExtraItemNum(); nExtraIndex++) {
		Item& schTemp = m_schScheduleTemp.GetItemAt(nExtraIndex);
		if (schTemp.IsEnabled() != bState) {
			// Change checked state
			schTemp.EnableItem(bState);
		}
	}

	// Update number of checked items
	m_nCheckCount = (bState == FALSE) ? 0 : GetTotalItemNum();

	// Update data item list
	UpdateDataItemList();

	// Refresh button state
	RefreshDialogItemState(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Validate
//	Description:	Check Schedule item validity
//  Arguments:		schItem		 - Item to validate
//					bShowMsg	 - Show validation message box or not
//					bAutoCorrect - Invalid value auto correction (ON/OFF)
//  Return value:	BOOL - Result of validation process
//
//////////////////////////////////////////////////////////////////////////

BOOL CMultiScheduleDlg::Validate(Item& schItem, BOOL bShowMsg /* = FALSE */, BOOL bAutoCorrect /* = FALSE */)
{
	BOOL bResult = TRUE;

	int nMsgStringID;
	CStringArray arrMsgString;
	arrMsgString.RemoveAll();

	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Check item ID
	if ((schItem.GetItemID() != DEF_SCHEDULE_DEFAULT_ITEMID) &&
		((schItem.GetItemID() < DEF_SCHEDULE_MIN_ITEMID) || (schItem.GetItemID() > DEF_SCHEDULE_MAX_ITEMID))) {
		nMsgStringID = MSGBOX_MULTISCHEDULE_INVALIDITEM_ITEMID;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Get next item ID
			schItem.SetItemID(m_schScheduleTemp.GetNextID());
		}
	}

	// Check action ID
	if ((schItem.GetAction() < APP_ACTION_NOTHING) || (schItem.GetAction() > APP_ACTION_HIBERNATE)) {
		nMsgStringID = MSGBOX_MULTISCHEDULE_INVALIDITEM_ACTIONID;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default action
			schItem.SetAction(DEF_SCHEDULE_INIT_ACTION);
		}
	}

	// Check repeat data
	if ((schItem.IsRepeatEnabled() == TRUE) && (schItem.GetActiveDays() == NULL)) {
		nMsgStringID = MSGBOX_MULTISCHEDULE_INVALIDITEM_ACTIVEDAYS;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default data
			schItem.SetActiveDays(DEF_REPEATSET_DEFAULT_ACTIVEDAYS);
		}
	}

	// Show error message if enabled
	if ((bShowMsg == TRUE) && (!arrMsgString.IsEmpty())) {
		for (int nIndex = 0; nIndex < arrMsgString.GetSize(); nIndex++) {
			// If auto correction is ON
			if (bAutoCorrect == TRUE) {
				// Add "Data will be automatically reset to default"
				CString strErrMessage = arrMsgString.GetAt(nIndex);
				strErrMessage += GetLanguageString(pLang, MSGBOX_MULTISCHEDULE_INVALIDITEM_AUTOCORRECT);
				DisplayMessageBox(strErrMessage, NULL, MB_OK | MB_ICONERROR);
			}
			else {
				// Display error message
				DisplayMessageBox(arrMsgString.GetAt(nIndex), NULL, MB_OK | MB_ICONERROR);
			}
		}
	}

	// Remove all message after displaying
	arrMsgString.RemoveAll();
	arrMsgString.FreeExtra();

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnApply
//	Description:	Handle clicking event for [Save] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnApply()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_APPLY_BTN);

	// Save data if changed
	BOOL bIsChanged = CheckDataChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
	if (bIsChanged == TRUE) {
		SaveScheduleSettings();
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

void CMultiScheduleDlg::OnExit()
{
	// If not forced closing by request
	if (!IsForceClosingByRequest()) {

		// Save app event log if enabled
		OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_CANCEL_BTN);

		// If data changed, ask for saving before closing dialog
		BOOL bIsChanged = CheckDataChangeState();
		SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
		if (bIsChanged == TRUE) {
			// Setup messagebox language
			LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
			CString strMessage = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
			CString strMsgCaption = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int nConfirm = MessageBox(strMessage, strMsgCaption, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Save data
				SaveScheduleSettings();
			}
		}
	}

	// Close dialog
	SDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnAdd
//	Description:	Handle click event for [Add] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnAdd()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_ADD_BTN);

	// Initialize new item template
	Item schTemp;
	schTemp.SetItemID(m_schScheduleTemp.GetNextID());
	schTemp.SetAction(DEF_SCHEDULE_INIT_ACTION);

	// Open edit schedule dialog
	if (m_pEditScheduleDlg == NULL) {
		// Initialize
		m_pEditScheduleDlg = new CEditScheduleDlg;
		m_pEditScheduleDlg->SetParentWnd(this);
		m_pEditScheduleDlg->SetScheduleItem(schTemp);
		m_pEditScheduleDlg->SetDispMode(MODE_ADD);
		m_pEditScheduleDlg->DoModal();
	}
	else {
		// Update dialog
		m_pEditScheduleDlg->SetParentWnd(this);
		m_pEditScheduleDlg->SetScheduleItem(schTemp);
		m_pEditScheduleDlg->SetDispMode(MODE_ADD);
		m_pEditScheduleDlg->ShowWindow(SW_SHOW);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnEdit
//	Description:	Handle click event for [Edit] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnEdit()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_EDIT_BTN);

	// Check if any item is selected or not
	BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	if (bIsSelected == TRUE) {

		// Get selected row index
		int nSelRowIndex = m_nCurSelIndex + ROW_FIXED_NUM;

		// Get selected item
		Item schItem;
		if (nSelRowIndex == ROW_INDEX_DEFAULT) {
			// Get default item
			schItem = m_schScheduleTemp.GetDefaultItem();
		}
		else {
			// Get extra item
			int nExtraItemIndex = nSelRowIndex - ROW_INDEX_EXTRASTART;
			schItem = m_schScheduleTemp.GetItemAt(nExtraItemIndex);
		}

		// If item is empty, do nothing
		if (schItem.IsEmpty())
			return;

		// Open edit schedule dialog
		if (m_pEditScheduleDlg == NULL) {
			// Initialize
			m_pEditScheduleDlg = new CEditScheduleDlg;
			m_pEditScheduleDlg->SetParentWnd(this);
			m_pEditScheduleDlg->SetScheduleItem(schItem);
			m_pEditScheduleDlg->SetDispMode(MODE_UPDATE);
			m_pEditScheduleDlg->DoModal();
		}
		else {
			// Update dialog
			m_pEditScheduleDlg->SetParentWnd(this);
			m_pEditScheduleDlg->SetScheduleItem(schItem);
			m_pEditScheduleDlg->SetDispMode(MODE_UPDATE);
			m_pEditScheduleDlg->ShowWindow(SW_SHOW);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRemove
//	Description:	Handle click event for [Remove] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnRemove()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_REMOVE_BTN);

	// If there's no item, do nothing
	int nItemNum = GetTotalItemNum();
	if (nItemNum <= 0)
		return;

	// Get current select item index
	int nSelItemIndex = m_nCurSelIndex;
	int nSelRowIndex = nSelItemIndex + ROW_FIXED_NUM;

	// If item at selected index is default item, can not remove
	if (nSelRowIndex == ROW_INDEX_DEFAULT) {
		DisplayMessageBox(MSGBOX_MULTISCHEDULE_NOTREMOVE_DEFAULT, NULL, MB_OK | MB_ICONINFORMATION);
		return;
	}

	// In case of extra item rows
	else if (nSelRowIndex > ROW_INDEX_DEFAULT) {
		// Get extra item index
		nSelItemIndex = nSelRowIndex - ROW_INDEX_EXTRASTART;
	}

	// If item at selected index is empy, do nothing
	if (m_schScheduleTemp.IsEmpty(nSelItemIndex) == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_MULTISCHEDULE_REMOVE_ITEM, NULL, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {
		// Remove item
		Remove(nSelItemIndex);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRemoveAll
//	Description:	Handle click event for [Remove All] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnRemoveAll()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_REMOVEALL_BTN);

	// If all item are empty, do nothing
	if (m_schScheduleTemp.IsAllEmpty() == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_MULTISCHEDULE_REMOVEALL_ITEMS, NULL, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {
		// Remove all items
		RemoveAll();
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCheckAll
//	Description:	Handle click event for [Check All] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnCheckAll()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_CHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_schScheduleTemp.IsAllEmpty() == TRUE)
		return;

	// Check all items
	SetAllItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnUncheckAll
//	Description:	Handle click event for [Uncheck All] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnUncheckAll()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_UNCHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_schScheduleTemp.IsAllEmpty() == TRUE)
		return;

	// Uncheck all items
	SetAllItemState(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnViewDetails
//	Description:	Handle click event for [View Details] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnViewDetails()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_VIEWDETAILS_BTN);

	// Check if any item is selected or not
	BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	if (bIsSelected == TRUE) {

		// Get selected row index
		int nSelRowIndex = m_nCurSelIndex + ROW_FIXED_NUM;

		// Get selected item
		Item schItem;
		if (nSelRowIndex == ROW_INDEX_DEFAULT) {
			// Get default item
			schItem = m_schScheduleTemp.GetDefaultItem();
		}
		else {
			// Get extra item
			int nExtraItemIndex = nSelRowIndex - ROW_INDEX_EXTRASTART;
			schItem = m_schScheduleTemp.GetItemAt(nExtraItemIndex);
		}

		// If item is empty, do nothing
		if (schItem.IsEmpty())
			return;

		// Open edit schedule dialog
		if (m_pEditScheduleDlg == NULL) {
			// Initialize
			m_pEditScheduleDlg = new CEditScheduleDlg;
			m_pEditScheduleDlg->SetParentWnd(this);
			m_pEditScheduleDlg->SetScheduleItem(schItem);
			m_pEditScheduleDlg->SetDispMode(MODE_VIEW);
			m_pEditScheduleDlg->DoModal();
		}
		else {
			// Update dialog
			m_pEditScheduleDlg->SetParentWnd(this);
			m_pEditScheduleDlg->SetScheduleItem(schItem);
			m_pEditScheduleDlg->SetDispMode(MODE_VIEW);
			m_pEditScheduleDlg->ShowWindow(SW_SHOW);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSetDefault
//	Description:	Handle click event for [Set Default] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnSetDefault()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_SETDEFAULT_BTN);

	// Check if any item is selected or not
	BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));
	if (bIsSelected != TRUE)
		return;

	// Check if selected item is an extra item or not
	int nSelRowIndex = m_nCurSelIndex + ROW_FIXED_NUM;
	BOOL bIsExtraSelected = ((bIsSelected == TRUE) && (nSelRowIndex >= ROW_INDEX_EXTRASTART));

	if (bIsExtraSelected == TRUE) {

		// Display confirmation message
		int nConfirm = DisplayMessageBox(MSGBOX_MULTISCHEDULE_CONFIRM_SETDEFAULT, NULL, MB_YESNO | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Check if currently selected item is empty
			int nExtraItemIndex = nSelRowIndex - ROW_INDEX_EXTRASTART;
			const Item& schCurSelItem = m_schScheduleTemp.GetItemAt(nExtraItemIndex);
			if (schCurSelItem.IsEmpty())
				return;
			
			// Overwrite default item data with currently selected extra item data
			Item& schDefaultItem = m_schScheduleTemp.GetDefaultItem();
			schDefaultItem.Copy(schCurSelItem);
			schDefaultItem.SetItemID(DEF_SCHEDULE_DEFAULT_ITEMID);

			// Remove the selected extra item after changing
			m_schScheduleTemp.Delete(nExtraItemIndex);

			// Update table
			RedrawDataTable();

			// Refresh button state
			RefreshDialogItemState(TRUE);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSelectScheduleItem
//	Description:	Show details when selecting a schedule item
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnSelectScheduleItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	//Get current selection index
	m_nCurSelIndex = nRow - ROW_FIXED_NUM;
	int nItemCount = GetTotalItemNum();

	// Success (return 0)
	*pResult = NULL;

	// Invalid selection
	if ((m_nCurSelIndex < 0) || (m_nCurSelIndex >= nItemCount))
		return;

	// Refresh display
	RefreshDialogItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClickDataItemList
//	Description:	Handle click event on Power Reminder data 
//					item list control
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nClickedCol = pItem->iColumn;
	int nClickedRow = pItem->iRow;

	// Check value validity
	int nItemNum = GetTotalItemNum();
	if ((nClickedRow <= GRIDCTRL_INDEX_HEADER_ROW) || (nClickedRow > nItemNum)) {
		return;
	}

	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	RefreshDialogItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRightClickDataItemList
//	Description:	Handle right click event on Power Reminder data 
//					item list control
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::OnRightClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nClickedCol = pItem->iColumn;
	int nClickedRow = pItem->iRow;

	// Check value validity
	int nItemNum = GetTotalItemNum();
	if ((nClickedRow <= GRIDCTRL_INDEX_HEADER_ROW) || (nClickedRow > nItemNum)) {
		return;
	}

	// Delete this (not used)
	*pResult = NULL;

	// Refresh button states
	RefreshDialogItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnChildDialogDestroy
//	Description:	Handle event when a child dialog is destroyed
//  Arguments:		wParam - Child dialog ID
//					lParam - Not used
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CMultiScheduleDlg::OnChildDialogDestroy(WPARAM wParam, LPARAM lParam)
{
	// Get dialog ID
	UINT nDialogID = (UINT)wParam;

	// Edit schedule dialog
	if (nDialogID == IDD_EDITSCHEDULE_DLG) {

		// Initialize info data
		Item schItemTemp;
		int nMode = MODE_INIT;
		int nRetFlag = RETFLAG_INVALID;

		// Update info data
		if (m_pEditScheduleDlg != NULL) {
			m_pEditScheduleDlg->GetScheduleItem(&schItemTemp);
			nMode = m_pEditScheduleDlg->GetDispMode();
			nRetFlag = m_pEditScheduleDlg->GetReturnFlag();
		}

		// Validate return info
		if ((!schItemTemp.IsEmpty()) && (nRetFlag == RETFLAG_UPDATE)) {
			// Update data by mode
			if (nMode == MODE_ADD) {
				// Add item
				Add(schItemTemp);
			}
			else if (nMode == MODE_UPDATE) {
				// Edit item --> Update changes
				Update(schItemTemp);
			}
		}

		// Dialog clean-up
		if (m_pEditScheduleDlg != NULL) {
			// Delete dialog
			delete m_pEditScheduleDlg;
			m_pEditScheduleDlg = NULL;
		}
	}

	return LRESULT(RESULT_SUCCESS);	// ERROR_SUCCESS
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

LRESULT CMultiScheduleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// Default
	return SDialog::WindowProc(message, wParam, lParam);
}

