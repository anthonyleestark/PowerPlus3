
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		MultiScheduleDlg.cpp
//		Description:	Source file for Schedule dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2017.03.08:		Create new
//						<1> 2024.01.27:		Update to version 3.0
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PowerPlus.h"
#include "PowerPlusDlg.h"
#include "MultiScheduleDlg.h"
#include "EditScheduleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace PairFuncs;
using namespace CoreFuncs;


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
	ZeroMemory(&m_schSchedule, sizeof(SCHEDULEDATA));
	ZeroMemory(&m_schScheduleTemp, sizeof(SCHEDULEDATA));

	// Other variables
	m_nColNum = 0;
	m_apGrdColFormat = NULL;
	m_pszDataTableFrameSize = NULL;

	// Other variables
	m_nCurMode = 0;
	m_nCheckCount = 0;
	m_nCurSelIndex = -1;
	m_nCurDispIndex = -2;

	INIT_CLASS_IDMAP()
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

	// Grid table list controls
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
//	CMultiScheduleDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_ID_MAPPING(CMultiScheduleDlg)
	IDMAP_ADD(IDD_MULTISCHEDULE_DLG,				"ScheduleDlg")
	IDMAP_ADD(IDC_MULTISCHEDULE_ITEM_LISTBOX,		"ScheduleItemList")
	IDMAP_ADD(IDC_MULTISCHEDULE_ADD_BTN,			"AddButton")
	IDMAP_ADD(IDC_MULTISCHEDULE_EDIT_BTN,			"EditButton")
	IDMAP_ADD(IDC_MULTISCHEDULE_REMOVE_BTN,			"RemoveButton")
	IDMAP_ADD(IDC_MULTISCHEDULE_REMOVEALL_BTN,		"RemoveAllButton")
	IDMAP_ADD(IDC_MULTISCHEDULE_CHECKALL_BTN,		"CheckAllButton")
	IDMAP_ADD(IDC_MULTISCHEDULE_UNCHECKALL_BTN,		"UncheckAllButton")
	IDMAP_ADD(IDC_MULTISCHEDULE_VIEWDETAILS_BTN,	"ViewDetailsButton")
	IDMAP_ADD(IDC_MULTISCHEDULE_SETDEFAULT_BTN,		"SetDefaultButton")
	IDMAP_ADD(IDC_MULTISCHEDULE_APPLY_BTN,			"SaveButton")
	IDMAP_ADD(IDC_MULTISCHEDULE_CANCEL_BTN,			"CancelButton")
END_ID_MAPPING()

//////////////////////////////////////////////////////////////////////////
//
//	CMultiScheduleDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMultiScheduleDlg, SDialog)
	ON_WM_CLOSE()
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
	ON_MESSAGE(SCM_DLGDESTROY_NOTIFY,							&CMultiScheduleDlg::OnChildDialogDestroy)
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
	SDialog::OnInitDialog();

	// Do not use Enter button
	SetUseEnter(FALSE);

	// Save app event log if enabled
	OutputDialogLog(GetDialogID(), LOG_EVENT_DLG_STARTUP);

	// Load data
	LoadScheduleSettings();

	// Init dialog items
	SetupLanguage();
	SetupDlgItemState();

	// Update data
	UpdateDataItemList();
	RefreshDlgItemState(TRUE);

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
		if (m_bChangeFlag == TRUE) {
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

	// Save app event log if enabled
	OutputDialogLog(GetDialogID(), LOG_EVENT_DLG_DESTROYED);

	SDialog::OnClose();
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
	// If data changed, ask for saving before closing dialog
	if (m_bChangeFlag == TRUE) {
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
			return LRESULT(DEF_RESULT_FAILED);
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
	CString strWndText = GetLanguageString(pAppLang, GetDialogID());
	this->SetWindowText(strWndText);

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
	pCell->SetBackClr(DEF_COLOR_WHITE);
	pCell->SetTextClr(DEF_COLOR_BLACK);
	pCell->SetHeight(DEF_GRIDCTRL_ROWHEIGHTEX);

	// Define table columns format
	GRIDCTRLCOLFORMAT arrGrdColFormat[] = {
	//-----------ID------------------------Header title ID---------------Width(px)---Width unit----------Column style--------Align Center---
		{	SCHCOL_ID_INDEX,		GRIDCOLUMN_MULTISCHEDULE_INDEX,			26,		COLSIZE_PIXEL,		COLSTYLE_FIXED,			TRUE,	},
		{	SCHCOL_ID_STATE,		GRIDCOLUMN_MULTISCHEDULE_STATE,			52,		COLSIZE_PIXEL,		COLSTYLE_CHECKBOX,		TRUE,	},
		{	SCHCOL_ID_ACTIONID,		GRIDCOLUMN_MULTISCHEDULE_ACTIONID,		45,		COLSIZE_PERCENT,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	SCHCOL_ID_TIMEVALUE,	GRIDCOLUMN_MULTISCHEDULE_TIMEVALUE,		40,		COLSIZE_PERCENT,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	PWRCOL_ID_REPEAT,		GRIDCOLUMN_MULTISCHEDULE_REPEAT,		53,		COLSIZE_PIXEL,		COLSTYLE_CHECKBOX,		TRUE,	},
	//--------------------------------------------------------------------------------------------------------------------------------------
	};

	// Table format and properties
	int nRowNum = (GetTotalItemNum() + ROW_FIXED_NUM);
	int nColNum = (sizeof(arrGrdColFormat) / sizeof(GRIDCTRLCOLFORMAT));

	// Backup format data
	m_nColNum = nColNum;
	if (m_apGrdColFormat == NULL) {
		m_apGrdColFormat = new GRIDCTRLCOLFORMAT[m_nColNum];
		for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
			// Copy and backup table format data
			m_apGrdColFormat[nIndex] = arrGrdColFormat[nIndex];
		}
	}

	// Setup table
	m_pDataItemListTable->SetColumnCount(nColNum);
	m_pDataItemListTable->SetFixedColumnCount(COL_FIXED_NUM);
	m_pDataItemListTable->SetRowCount(nRowNum);
	m_pDataItemListTable->SetFixedRowCount(ROW_FIXED_NUM);
	m_pDataItemListTable->SetRowHeight(DEF_GRIDCTRL_ROWHEADER, DEF_GRIDCTRL_HEADERHEIGHT);

	// Draw table
	DrawDataTable(GetReadOnlyMode());

	// Display table
	m_pDataItemListTable->SetListMode(TRUE);
	m_pDataItemListTable->SetEditable(FALSE);
	m_pDataItemListTable->SetColumnResize(FALSE);
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
		pCell->SetBackClr(DEF_COLOR_BRIGHT_GRAY);
		pCell->SetTextClr(DEF_COLOR_DARK_GRAY);
	}
	else {
		pCell->SetBackClr(DEF_COLOR_WHITE);
		pCell->SetTextClr(DEF_COLOR_BLACK);
	}
	
	// Table properties
	int nColNum = m_nColNum;
	int nRowNum = (GetTotalItemNum() + ROW_FIXED_NUM);

	// Setup display size
	int nFrameHeight = m_pszDataTableFrameSize->cy;
	int nFrameWidth = m_pszDataTableFrameSize->cx;
	nFrameWidth -= DEF_OFFSET_LISTCTRLWIDTH;
	if (pApp->GetWindowsOSVersion() == DEF_WINVER_WIN10) {
		// Windows 10 list control offset
		nFrameWidth -= DEF_OFFSET_LISTCTRL_WIN10;
		nFrameHeight -= DEF_OFFSET_LISTCTRL_WIN10;
	}
	if ((DEF_GRIDCTRL_HEADERHEIGHT + ((nRowNum - 1) * DEF_GRIDCTRL_ROWHEIGHT)) >= nFrameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nFrameWidth -= (nScrollBarWidth + DEF_OFFSET_VSCRLBRWIDTH);
	}

	// Setup columns
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Set header row style
		SetFixedCellStyle(m_pDataItemListTable, DEF_GRIDCTRL_ROWHEADER, nCol);

		// Column header title
		CString strHdrTitle = DEF_STRING_EMPTY;
		UINT nHeaderTitleID = m_apGrdColFormat[nCol].nHeaderTitleID;
		if (nHeaderTitleID != DEF_INTEGER_NULL) {
			strHdrTitle = GetLanguageString(ptrLanguage, nHeaderTitleID);
		}
		m_pDataItemListTable->SetItemText(DEF_GRIDCTRL_ROWHEADER, nCol, strHdrTitle);

		// Column width
		int nColWidth = m_apGrdColFormat[nCol].nWidth;
		int nColWidthUnit = m_apGrdColFormat[nCol].nWidthUnit;
		if (nColWidthUnit == COLSIZE_PIXEL) {					// Width unit: Pixel
			nFrameWidth -= nColWidth;
			m_pDataItemListTable->SetColumnWidth(nCol, nColWidth);
		}
		else if (nColWidthUnit == COLSIZE_PERCENT) {			// Width unit: Percent
			int nColWidthPx = floor(float((nFrameWidth * nColWidth) / 100));
			m_pDataItemListTable->SetColumnWidth(nCol, nColWidthPx);
		}
	}

	// Setup rows
	int nColStyle = -1;
	UINT nItemState = DEF_INTEGER_NULL;
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
					pCellBase->SetBackClr(DEF_COLOR_YELLOW);
					pCellBase->SetTextClr(DEF_COLOR_RED);
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
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SwitchMode
//	Description:	Switch between modes and re-update dialog item states
//  Arguments:		bRedraw - Redraw dialog items or not
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::SwitchMode(BOOL bRedraw /* = FALSE */)
{
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupDlgItemState
//	Description:	Setup properties and values for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::SetupDlgItemState()
{
	// Read-only mode (if enabled)
	if (GetReadOnlyMode() == TRUE) {
		CWnd* pWndChild = GetTopWindow();
		while (pWndChild != NULL) {
			pWndChild->EnableWindow(FALSE);
			pWndChild = pWndChild->GetWindow(GW_HWNDNEXT);
		}
	}
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
		SCHEDULEITEM schItem;
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
		m_pDataItemListTable->SetItemText(nRowIndex, SCHCOL_ID_INDEX, strTemp);

		// Enable state
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, SCHCOL_ID_STATE);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(schItem.bEnable);
		}

		// Action name
		nTemp = GetPairedID(idplActionName, schItem.nAction);
		strTemp = GetLanguageString(ptrLanguage, nTemp);
		m_pDataItemListTable->SetItemText(nRowIndex, SCHCOL_ID_ACTIONID, strTemp);

		// Time setting
		CString strFormat = GetLanguageString(ptrLanguage, GRIDCOLUMN_MULTISCHEDULE_TIMEFORMAT);
		strTemp = FormatDispTime(ptrLanguage, strFormat, schItem.stTime);
		m_pDataItemListTable->SetItemText(nRowIndex, SCHCOL_ID_TIMEVALUE, strTemp);

		// Repeat
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, SCHCOL_ID_REPEAT);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(schItem.bRepeat);
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
//	Function name:	RefreshDlgItemState
//	Description:	Refresh and update state for dialog items
//  Arguments:		bRecheckState - Recheck all items state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::RefreshDlgItemState(BOOL bRecheckState)
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
	m_bChangeFlag = CheckDataChangeState();

	// Enable [Apply] button if data is changed
	pBtn = GetDlgItem(IDC_MULTISCHEDULE_APPLY_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(m_bChangeFlag);
	}

	// Update [Check/Uncheck All] button state
	UpdateCheckAllBtnState(bRecheckState);
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
		if (m_schScheduleTemp.GetDefaultItem().bEnable == TRUE) {
			// Increase counter
			m_nCheckCount++;
		}
		// Check extra items
		for (int nExtraIndex = 0; nExtraIndex < GetExtraItemNum(); nExtraIndex++) {
			SCHEDULEITEM& schTemp = m_schScheduleTemp.GetItemAt(nExtraIndex);
			if (schTemp.bEnable == TRUE) {
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
	PSCHEDULEDATA pschData = pApp->GetAppScheduleData();
	if (pschData == NULL)
		return FALSE;

	// Copy data
	m_schSchedule.Copy(*pschData);
	m_schScheduleTemp.Copy(m_schSchedule);

	// Reset change flag
	m_bChangeFlag = FALSE;

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
	m_bChangeFlag = FALSE;

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
		pCellCheckEnable = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, SCHCOL_ID_STATE);
		pCellCheckRepeat = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, SCHCOL_ID_REPEAT);
		if ((pCellCheckEnable == NULL) || (pCellCheckRepeat == NULL)) continue;

		// Get checked states
		BOOL bEnable = pCellCheckEnable->GetCheck();
		BOOL bRepeat = pCellCheckRepeat->GetCheck();

		// Update item enable and repeat states
		if (nRowIndex == ROW_INDEX_DEFAULT) {
			// Update default item data
			SCHEDULEITEM& schTempDefault = m_schScheduleTemp.GetDefaultItem();
			schTempDefault.bEnable = bEnable;
			schTempDefault.bRepeat = bRepeat;
		}
		else {
			// Update extra item data
			SCHEDULEITEM& schTempItem = m_schScheduleTemp.GetItemAt(nExtraItemIndex);
			schTempItem.bEnable = bEnable;
			schTempItem.bRepeat = bRepeat;
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
	bChangeFlag |= (m_schScheduleTemp.GetDefaultItem().bEnable != m_schSchedule.GetDefaultItem().bEnable);
	bChangeFlag |= (m_schScheduleTemp.GetDefaultItem().bRepeat != m_schSchedule.GetDefaultItem().bRepeat);
	bChangeFlag |= (m_schScheduleTemp.GetDefaultItem().nAction != m_schSchedule.GetDefaultItem().nAction);
	bChangeFlag |= (m_schScheduleTemp.GetDefaultItem().stTime.wHour != m_schSchedule.GetDefaultItem().stTime.wHour);
	bChangeFlag |= (m_schScheduleTemp.GetDefaultItem().stTime.wMinute != m_schSchedule.GetDefaultItem().stTime.wMinute);
	bChangeFlag |= (m_schScheduleTemp.GetDefaultItem().byRepeatDays != m_schSchedule.GetDefaultItem().byRepeatDays);
	if (bChangeFlag == TRUE)
		return bChangeFlag;

	// Check if each extra item's data changed
	for (int nExtraIndex = 0; nExtraIndex < nExtraItemNum; nExtraIndex++) {
		// Get current item and temp item
		SCHEDULEITEM schCurItem = m_schSchedule.GetItemAt(nExtraIndex);
		SCHEDULEITEM schTempItem = m_schScheduleTemp.GetItemAt(nExtraIndex);

		// Data comparison
		bChangeFlag |= (schTempItem.bEnable != schCurItem.bEnable);
		bChangeFlag |= (schTempItem.bRepeat != schCurItem.bRepeat);
		bChangeFlag |= (schTempItem.nAction != schCurItem.nAction);
		bChangeFlag |= (schTempItem.stTime.wHour != schCurItem.stTime.wHour);
		bChangeFlag |= (schTempItem.stTime.wMinute != schCurItem.stTime.wMinute);
		bChangeFlag |= (schTempItem.byRepeatDays != schCurItem.byRepeatDays);

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

void CMultiScheduleDlg::Add(SCHEDULEITEM& schItem)
{
	// If item is empty, do nothing
	if (schItem.IsEmpty())
		return;

	// Check data validity
	BOOL bValid = Validate(schItem, TRUE);
	if (bValid == FALSE)
		return;

	// Update item to Action Schedule data item list
	m_schScheduleTemp.Update(schItem);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDlgItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Update
//	Description:	Update schedule item
//  Arguments:		schItem - Schedule item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CMultiScheduleDlg::Update(SCHEDULEITEM& schItem)
{
	// If item is empty, do nothing
	if (schItem.IsEmpty())
		return;

	// Check data validity
	BOOL bValid = Validate(schItem, TRUE);
	if (bValid == FALSE)
		return;

	// Update item to Action Schedule data item list
	m_schScheduleTemp.Update(schItem);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDlgItemState(TRUE);
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
	RefreshDlgItemState(TRUE);
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
	RefreshDlgItemState(TRUE);
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
	SCHEDULEITEM& schDefTemp = m_schScheduleTemp.GetDefaultItem();
	if (schDefTemp.bEnable != bState) {
		// Change checked state
		schDefTemp.bEnable = bState;
	}
	for (int nExtraIndex = 0; nExtraIndex < GetExtraItemNum(); nExtraIndex++) {
		SCHEDULEITEM& schTemp = m_schScheduleTemp.GetItemAt(nExtraIndex);
		if (schTemp.bEnable != bState) {
			// Change checked state
			schTemp.bEnable = bState;
		}
	}

	// Update number of checked items
	m_nCheckCount = (bState == FALSE) ? 0 : GetTotalItemNum();

	// Update data item list
	UpdateDataItemList();

	// Refresh button state
	RefreshDlgItemState(FALSE);
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

BOOL CMultiScheduleDlg::Validate(SCHEDULEITEM& schItem, BOOL bShowMsg /* = FALSE */, BOOL bAutoCorrect /* = FALSE */)
{
	BOOL bResult = TRUE;

	int nMsgStringID;
	CStringArray arrMsgString;
	arrMsgString.RemoveAll();

	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Check item ID
	if ((schItem.nItemID != DEF_SCHEDULE_DEFAULT_ITEMID) &&
		((schItem.nItemID < DEF_SCHEDULE_MIN_ITEMID) || (schItem.nItemID > DEF_SCHEDULE_MAX_ITEMID))) {
		nMsgStringID = MSGBOX_MULTISCHEDULE_INVALIDITEM_ITEMID;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Get next item ID
			schItem.nItemID = m_schScheduleTemp.GetNextID();
		}
	}

	// Check action ID
	if ((schItem.nAction < DEF_APP_ACTION_NOTHING) || (schItem.nAction > DEF_APP_ACTION_HIBERNATE)) {
		nMsgStringID = MSGBOX_MULTISCHEDULE_INVALIDITEM_ACTIONID;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default action
			schItem.nAction = DEF_SCHEDULE_INIT_ACTION;
		}
	}

	// Check repeat data
	if ((schItem.bRepeat == TRUE) && (schItem.byRepeatDays == NULL)) {
		nMsgStringID = MSGBOX_MULTISCHEDULE_INVALIDITEM_ACTIVEDAYS;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default data
			schItem.byRepeatDays = DEF_SCHEDULE_DEFAULT_REPEAT;
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
				DisplayMessageBox(strErrMessage, MSGBOX_MULTISCHEDULE_CAPTION, MB_OK | MB_ICONERROR);
			}
			else {
				// Display error message
				DisplayMessageBox(arrMsgString.GetAt(nIndex), MSGBOX_MULTISCHEDULE_CAPTION, MB_OK | MB_ICONERROR);
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
	OutputButtonLog(GetDialogID(), IDC_MULTISCHEDULE_APPLY_BTN);

	// Save data if changed
	m_bChangeFlag = CheckDataChangeState();
	if (m_bChangeFlag == TRUE) {
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
		OutputButtonLog(GetDialogID(), IDC_MULTISCHEDULE_CANCEL_BTN);

		// If data changed, ask for saving before closing dialog
		m_bChangeFlag = CheckDataChangeState();
		if (m_bChangeFlag == TRUE) {
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
	OutputButtonLog(GetDialogID(), IDC_MULTISCHEDULE_ADD_BTN);

	// Initialize new item template
	SCHEDULEITEM schTemp;
	schTemp.nItemID = m_schScheduleTemp.GetNextID();
	schTemp.nAction = DEF_SCHEDULE_INIT_ACTION;

	// Open edit schedule dialog
	if (m_pEditScheduleDlg == NULL) {
		// Initialize
		m_pEditScheduleDlg = new CEditScheduleDlg;
		m_pEditScheduleDlg->SetParentWnd(this);
		m_pEditScheduleDlg->SetScheduleItem(schTemp);
		m_pEditScheduleDlg->SetDispMode(DEF_MODE_ADD);
		m_pEditScheduleDlg->DoModal();
	}
	else {
		// Update dialog
		m_pEditScheduleDlg->SetParentWnd(this);
		m_pEditScheduleDlg->SetScheduleItem(schTemp);
		m_pEditScheduleDlg->SetDispMode(DEF_MODE_ADD);
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
	OutputButtonLog(GetDialogID(), IDC_MULTISCHEDULE_EDIT_BTN);

	// Check if any item is selected or not
	BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	if (bIsSelected == TRUE) {

		// Get selected row index
		int nSelRowIndex = m_nCurSelIndex + ROW_FIXED_NUM;

		// Get selected item
		SCHEDULEITEM schItem;
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
			m_pEditScheduleDlg->SetDispMode(DEF_MODE_UPDATE);
			m_pEditScheduleDlg->DoModal();
		}
		else {
			// Update dialog
			m_pEditScheduleDlg->SetParentWnd(this);
			m_pEditScheduleDlg->SetScheduleItem(schItem);
			m_pEditScheduleDlg->SetDispMode(DEF_MODE_UPDATE);
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
	OutputButtonLog(GetDialogID(), IDC_MULTISCHEDULE_REMOVE_BTN);

	// If there's no item, do nothing
	int nItemNum = GetTotalItemNum();
	if (nItemNum <= 0)
		return;

	// Get current select item index
	int nSelItemIndex = m_nCurSelIndex;
	int nSelRowIndex = nSelItemIndex + ROW_FIXED_NUM;

	// If item at selected index is default item, can not remove
	if (nSelRowIndex == ROW_INDEX_DEFAULT) {
		DisplayMessageBox(MSGBOX_MULTISCHEDULE_NOTREMOVE_DEFAULT, MSGBOX_MULTISCHEDULE_CAPTION, MB_OK | MB_ICONINFORMATION);
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
	int nConfirm = DisplayMessageBox(MSGBOX_MULTISCHEDULE_REMOVE_ITEM, MSGBOX_MULTISCHEDULE_CAPTION, MB_YESNO | MB_ICONQUESTION);
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
	OutputButtonLog(GetDialogID(), IDC_MULTISCHEDULE_REMOVEALL_BTN);

	// If all item are empty, do nothing
	if (m_schScheduleTemp.IsAllEmpty() == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_MULTISCHEDULE_REMOVEALL_ITEMS, MSGBOX_MULTISCHEDULE_CAPTION, MB_YESNO | MB_ICONQUESTION);
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
	OutputButtonLog(GetDialogID(), IDC_MULTISCHEDULE_CHECKALL_BTN);

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
	OutputButtonLog(GetDialogID(), IDC_MULTISCHEDULE_UNCHECKALL_BTN);

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
	OutputButtonLog(GetDialogID(), IDC_MULTISCHEDULE_VIEWDETAILS_BTN);

	// Check if any item is selected or not
	BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	if (bIsSelected == TRUE) {

		// Get selected row index
		int nSelRowIndex = m_nCurSelIndex + ROW_FIXED_NUM;

		// Get selected item
		SCHEDULEITEM schItem;
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
			m_pEditScheduleDlg->SetDispMode(DEF_MODE_VIEW);
			m_pEditScheduleDlg->DoModal();
		}
		else {
			// Update dialog
			m_pEditScheduleDlg->SetParentWnd(this);
			m_pEditScheduleDlg->SetScheduleItem(schItem);
			m_pEditScheduleDlg->SetDispMode(DEF_MODE_VIEW);
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
	OutputButtonLog(GetDialogID(), IDC_MULTISCHEDULE_SETDEFAULT_BTN);

	// Check if any item is selected or not
	BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));
	if (bIsSelected != TRUE)
		return;

	// Check if selected item is an extra item or not
	int nSelRowIndex = m_nCurSelIndex + ROW_FIXED_NUM;
	BOOL bIsExtraSelected = ((bIsSelected == TRUE) && (nSelRowIndex >= ROW_INDEX_EXTRASTART));

	if (bIsExtraSelected == TRUE) {

		// Display confirmation message
		int nConfirm = DisplayMessageBox(MSGBOX_MULTISCHEDULE_CONFIRM_SETDEFAULT, MSGBOX_MULTISCHEDULE_CAPTION, MB_YESNO | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Check if currently selected item is empty
			int nExtraItemIndex = nSelRowIndex - ROW_INDEX_EXTRASTART;
			SCHEDULEITEM& schCurSelItem = m_schScheduleTemp.GetItemAt(nExtraItemIndex);
			if (schCurSelItem.IsEmpty())
				return;
			
			// Overwrite default item data with currently selected extra item data
			SCHEDULEITEM& schDefaultItem = m_schScheduleTemp.GetDefaultItem();
			schDefaultItem.Copy(schCurSelItem);
			schDefaultItem.nItemID = DEF_SCHEDULE_DEFAULT_ITEMID;

			// Remove the selected extra item after changing
			m_schScheduleTemp.Delete(nExtraItemIndex);

			// Update table
			RedrawDataTable();

			// Refresh button state
			RefreshDlgItemState(TRUE);
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

	*pResult = NULL;

	// Invalid selection
	if ((m_nCurSelIndex < 0) || (m_nCurSelIndex >= nItemCount))
		return;

	// Refresh display
	RefreshDlgItemState(TRUE);
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
	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	// Check value validity
	int nItemNum = GetTotalItemNum();
	if ((nRow <= DEF_GRIDCTRL_ROWHEADER) || (nRow > nItemNum)) {
		return;
	}

#if 0
	// Handle click event on Checkbox columns
	if (((nCol == SCHCOL_ID_STATE) || (nCol == SCHCOL_ID_REPEAT)) && (nRow != DEF_GRIDCTRL_ROWHEADER)) {
		if (m_pDataItemListTable == NULL) return;
		CGridCellCheck* clickedCell = (CGridCellCheck*)(m_pDataItemListTable->GetCell(nRow, nCol));
		if (clickedCell == NULL) return;

		// Change cell selected state
		BOOL bCheck = clickedCell->GetCheck();
		clickedCell->SetCheck(!bCheck);

		// Update cell
		m_pDataItemListTable->RedrawCell(nRow, nCol);
	}
#endif

	*pResult = NULL;

	// Refresh button states
	RefreshDlgItemState(TRUE);
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
	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	// Check value validity
	int nItemNum = GetTotalItemNum();
	if ((nRow <= DEF_GRIDCTRL_ROWHEADER) || (nRow > nItemNum)) {
		return;
	}

#if 0
	// Handle click event on Checkbox columns
	if (((nCol == SCHCOL_ID_STATE) || (nCol == SCHCOL_ID_REPEAT)) && (nRow != DEF_GRIDCTRL_ROWHEADER)) {
		if (m_pDataItemListTable == NULL) return;
		CGridCellCheck* clickedCell = (CGridCellCheck*)(m_pDataItemListTable->GetCell(nRow, nCol));
		if (clickedCell == NULL) return;

		// Change cell selected state
		BOOL bCheck = clickedCell->GetCheck();
		clickedCell->SetCheck(!bCheck);

		// Update cell
		m_pDataItemListTable->RedrawCell(nRow, nCol);
	}
#endif

	*pResult = NULL;

	// Refresh button states
	RefreshDlgItemState(TRUE);
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
		SCHEDULEITEM schItemTemp;
		int nMode = DEF_MODE_INIT;
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
			if (nMode == DEF_MODE_ADD) {
				// Add item
				Add(schItemTemp);
			}
			else if (nMode == DEF_MODE_UPDATE) {
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

	return LRESULT(DEF_RESULT_SUCCESS);	// ERROR_SUCCESS
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
	return SDialog::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetTotalItemNum
//	Description:	Get data table total item number (including default item)
//  Arguments:		None
//  Return value:	int - Number of total items
//
//////////////////////////////////////////////////////////////////////////

int CMultiScheduleDlg::GetTotalItemNum()
{
	return GetExtraItemNum() + DEF_SCHEDULE_DEFAULT_ITEMNUM;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetExtraItemNum
//	Description:	Get data table extra item number only
//  Arguments:		None
//  Return value:	int - Number of extra items only
//
//////////////////////////////////////////////////////////////////////////

int CMultiScheduleDlg::GetExtraItemNum()
{
	return m_schScheduleTemp.GetExtraItemNum();
}

