
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		LogViewerDlg.cpp
//		Description:	Source file for LogViewer dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.24:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Logging.h"
#include "Logging_pub.h"

#include "PowerPlus.h"
#include "PowerPlusDlg.h"
#include "LogViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace PairFuncs;
using namespace CoreFuncs;
using namespace RegFuncs;


////////////////////////////////////////////////////////
//
//	Define macros for data list table
//
////////////////////////////////////////////////////////

#define COL_FIXED_NUM			0
#define ROW_FIXED_NUM			1
#define ROW_INDEX_START			1

////////////////////////////////////////////////////////
//
//	Implement methods for CLogViewerDlg
//
////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CLogViewerDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CLogViewerDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CLogViewerDlg::CLogViewerDlg(CWnd* pParent /*=nullptr*/)
	: SDialog(IDD_LOGVIEWER_DLG, pParent)
{
	// Init member variables
	m_pLogViewerList = NULL;
	m_ptrAppEventLog = NULL;
	m_nLogCount = 0;

	// Table format and properties
	m_nColNum = 0;
	m_apGrdColFormat = NULL;
	m_pszTableFrameSize = NULL;

	// Other variables
	m_nCurMode = 0;
	m_nCheckCount = 0;
	m_nCurSelIndex = -1;

	INIT_CLASS_IDMAP()
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CLogViewerDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CLogViewerDlg::~CLogViewerDlg()
{
	// Grid table list control
	if (m_pLogViewerList) {
		delete m_pLogViewerList;
		m_pLogViewerList = NULL;
	}

	// Other variables
	if (m_apGrdColFormat != NULL) {
		delete[] m_apGrdColFormat;
		m_apGrdColFormat = NULL;
	}
	if (m_pszTableFrameSize != NULL) {
		delete m_pszTableFrameSize;
		m_pszTableFrameSize = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


//////////////////////////////////////////////////////////////////////////
//
//	CLogViewerDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_ID_MAPPING(CLogViewerDlg)
	IDMAP_ADD(IDD_LOGVIEWER_DLG,			 "LogViewerDlg")
	IDMAP_ADD(IDC_LOGVIEWER_LOGDATA_LISTBOX, "LogViewerList")
	IDMAP_ADD(IDC_LOGVIEWER_REMOVEALL_BTN,   "RemoveAllButton")
	IDMAP_ADD(IDC_LOGVIEWER_DETAILS_BTN,	 "DetailButton")
	IDMAP_ADD(IDC_LOGVIEWER_CLOSE_BTN,		 "CloseButton")
END_ID_MAPPING()


//////////////////////////////////////////////////////////////////////////
//
//	CLogViewerDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CLogViewerDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_LOGVIEWER_REMOVEALL_BTN,	&CLogViewerDlg::OnRemoveAllBtn)
	ON_BN_CLICKED(IDC_LOGVIEWER_DETAILS_BTN,	&CLogViewerDlg::OnDetailBtn)
	ON_BN_CLICKED(IDC_LOGVIEWER_CLOSE_BTN,		&CLogViewerDlg::OnCloseBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LOGVIEWER_LOGDATA_LISTBOX, &CLogViewerDlg::OnSelectLogItem)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// LogViewerDlg message handlers

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize Logviewer dialog and setup dialog content
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CLogViewerDlg::OnInitDialog()
{
	SDialog::OnInitDialog();

	// Do not use Enter button
	SetUseEnter(FALSE);

	// Register message box caption
	RegisterMessageBoxCaption(MSGBOX_LOGVIEWER_CAPTION);

	// Load app event log data pointer
	LoadAppEventLogData();

	// Setup dialog
	LoadLayoutInfo();
	SetupLanguage();
	UpdateLogViewer();

	// Disable [Details] button
	CWnd* pDetailBtn = (CWnd*)GetDlgItem(IDC_LOGVIEWER_DETAILS_BTN);
	if (pDetailBtn != NULL) {
		pDetailBtn->EnableWindow(FALSE);
	}

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, GetDialogCaption());

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClose
//	Description:	Handle event when closing the dialog
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::OnClose()
{
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

void CLogViewerDlg::OnDestroy()
{
	// Save app event log if enabled
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, GetDialogCaption());

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

LRESULT CLogViewerDlg::RequestCloseDialog(void)
{
	// Request accepted
	return SDialog::RequestCloseDialog();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRemoveAllBtn
//	Description:	Handle click event for [Remove All Records] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::OnRemoveAllBtn()
{
	// Get app event logging pointer
	if (m_ptrAppEventLog == NULL) return;

	// Confirm before removing
	int nRet = DisplayMessageBox(MSGBOX_LOGVIEWER_CONFIRM_REMOVEALLRECORDS, NULL, MB_YESNO | MB_ICONQUESTION);
	if (nRet == IDNO) return;

	// Remove all app event log records
	m_ptrAppEventLog->DeleteAll();

	// Close dialog
	PostMessage(WM_CLOSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDetailBtn
//	Description:	Handle click event for [Details] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::OnDetailBtn()
{
	// Get app event logging pointer
	if (m_ptrAppEventLog == NULL) return;

	// Check selection index validity
	if (m_nCurSelIndex < 0 || m_nCurSelIndex > m_nLogCount)
		return;

	// Display log details
	DisplayLogDetails(m_nCurSelIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCloseBtn
//	Description:	Handle click event for [Close] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::OnCloseBtn()
{
	// Close dialog
	PostMessage(WM_CLOSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::SetupLanguage(void)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Setup dialog title
	this->SetLangDialogCaption(GetDialogID());

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		UINT nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDC_LOGVIEWER_LOGDATA_LISTBOX:
			// Skip these items
			break;
		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}

	// Setup LogViewer list
	SetupLogViewerList(pAppLang);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLogViewerList
//	Description:	Initialize and setup language for LogViewer list
//  Arguments:		ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::SetupLogViewerList(LANGTABLE_PTR ptrLanguage)
{
	// Get parent list frame rect
	CWnd* pListFrameWnd = GetDlgItem(IDC_LOGVIEWER_LOGDATA_LISTBOX);
	if (pListFrameWnd == NULL) return;
	RECT rcListFrameWnd;
	pListFrameWnd->GetWindowRect(&rcListFrameWnd);
	ScreenToClient(&rcListFrameWnd);

	// Get frame size
	if (m_pszTableFrameSize == NULL) {
		m_pszTableFrameSize = new CSize();
		m_pszTableFrameSize->cx = rcListFrameWnd.right - rcListFrameWnd.left;
		m_pszTableFrameSize->cy = rcListFrameWnd.bottom - rcListFrameWnd.top;
	}

	// Initialization
	if (m_pLogViewerList == NULL) {
		m_pLogViewerList = new CGridCtrl();
	}

	// Create table
	if (m_pLogViewerList == NULL) return;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	m_pLogViewerList->Create(rcListFrameWnd, this, IDC_LOGVIEWER_LOGDATA_LISTBOX, dwStyle);

	// Destroy frame
	pListFrameWnd->DestroyWindow();

	// Cell format
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pLogViewerList->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;
	pCell->SetFormat(pCell->GetFormat());
	pCell->SetMargin(0);
	pCell->SetBackClr(DEF_COLOR_WHITE);
	pCell->SetTextClr(DEF_COLOR_BLACK);
	pCell->SetHeight(DEF_GRIDCTRL_ROWHEIGHTEX);

	// Table format and properties
	int nRowNum = (m_nLogCount + ROW_FIXED_NUM);
	int nColNum = m_nColNum;

	// Setup table
	m_pLogViewerList->SetColumnCount(nColNum);
	m_pLogViewerList->SetRowCount(nRowNum);
	m_pLogViewerList->SetFixedRowCount(ROW_FIXED_NUM);
	m_pLogViewerList->SetRowHeight(DEF_GRIDCTRL_ROWHEADER, DEF_GRIDCTRL_HEADERHEIGHT);

	// Draw table
	DrawLogViewerTable();

	// Update layout info
	UpdateLayoutInfo();

	// Display table
	m_pLogViewerList->SetListMode(TRUE);
	m_pLogViewerList->SetEditable(FALSE);
	m_pLogViewerList->SetColumnResize(TRUE);
	m_pLogViewerList->SetRowResize(FALSE);
	m_pLogViewerList->EnableSelection(TRUE);
	m_pLogViewerList->SetSingleRowSelection(TRUE);
	m_pLogViewerList->SetSingleColSelection(FALSE);
	m_pLogViewerList->SetFixedRowSelection(FALSE);
	m_pLogViewerList->SetFixedColumnSelection(FALSE);
	m_pLogViewerList->ShowWindow(SW_SHOW);
	m_pLogViewerList->SetRedraw(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DrawLogViewerTable
//	Description:	Draw LogViewer list table
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::DrawLogViewerTable(void)
{
	// Check table validity
	if (m_pLogViewerList == NULL) return;

	// Check table format data validity
	if (m_pszTableFrameSize == NULL) return;

	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;

	// Get app language package
	LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();

	// Re-update default cell properties
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pLogViewerList->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;

	// Table properties
	int nColNum = m_nColNum;
	int nRowNum = (m_nLogCount + ROW_FIXED_NUM);

	// Setup display size
	int nFrameHeight = m_pszTableFrameSize->cy;
	int nFrameWidth = m_pszTableFrameSize->cx;
	int nColWidthOffset = 0;
	if (pApp->GetWindowsOSVersion() == DEF_WINVER_WIN10) {
		// Windows 10 list control offset
		nFrameWidth -= DEF_OFFSET_LISTCTRL_WIN10;
		nFrameHeight -= DEF_OFFSET_LISTCTRL_WIN10;
		nColWidthOffset = DEF_OFFSET_LISTCOLWIDTH_WIN10;
	}
	else {
		// Windows 11 list control offset
		nFrameWidth -= DEF_OFFSET_LISTCTRL;
		nFrameHeight -= DEF_OFFSET_LISTCTRL;
	}
	if ((DEF_GRIDCTRL_HEADERHEIGHT + ((nRowNum - 1) * DEF_GRIDCTRL_ROWHEIGHTEX)) >= nFrameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nFrameWidth -= nScrollBarWidth;
	}

	// Setup columns
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Set header row style
		SetFixedCellStyle(m_pLogViewerList, DEF_GRIDCTRL_ROWHEADER, nCol);

		// Column header title
		CString strHdrTitle = DEF_STRING_EMPTY;
		UINT nHeaderTitleID = m_apGrdColFormat[nCol].nHeaderTitleID;
		if (nHeaderTitleID != DEF_INTEGER_NULL) {
			strHdrTitle = GetLanguageString(ptrLanguage, nHeaderTitleID);
		}
		m_pLogViewerList->SetItemText(DEF_GRIDCTRL_ROWHEADER, nCol, strHdrTitle);

		// Column width
		int nColWidth = m_apGrdColFormat[nCol].nWidth;
		if (nColWidth != -1) {
			// Set column width as defined (with offset)
			int nColWidthPx = nColWidth - nColWidthOffset;
			if (m_pLogViewerList->SetColumnWidth(nCol, nColWidthPx)) {
				// Calculate remaining width (not using offset)
				nFrameWidth -= nColWidth;
			}
		}
		else {
			// Set remaining width for current column
			m_pLogViewerList->SetColumnWidth(nCol, nFrameWidth);
		}
	}

	// Setup rows
	int nColStyle = -1;
	UINT nItemState = DEF_INTEGER_NULL;
	for (int nRow = 1; nRow < nRowNum; nRow++) {
		for (int nCol = 0; nCol < m_nColNum; nCol++) {

			// Get column style & item state
			nColStyle = m_apGrdColFormat[nCol].nColStyle;
			nItemState = m_pLogViewerList->GetItemState(nRow, nCol);
			nItemState |= GVIS_READONLY;

			// Base column - header-like style
			if (nColStyle == COLSTYLE_FIXED) {
				// Set fixed cell style
				SetFixedCellStyle(m_pLogViewerList, nRow, nCol);
			}

			// Checkbox column
			else if (nColStyle == COLSTYLE_CHECKBOX) {
				// Set cell type: Checkbox
				if (!m_pLogViewerList->SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck)))
					continue;

				// Get cell
				CGridCellCheck* pCell = (CGridCellCheck*)m_pLogViewerList->GetCell(nRow, nCol);

				// Set center alignment if defined
				if (m_apGrdColFormat[nCol].bCenter == TRUE) {
					if (pCell == NULL) continue;
					pCell->SetCheckPlacement(SCP_CENTERING);
				}
			}

			// Normal column
			else if (nColStyle == COLSTYLE_NORMAL) {
				// Set item state
				if (!m_pLogViewerList->SetItemState(nRow, nCol, nItemState))
					continue;

				// Get cell
				CGridCellBase* pCell = (CGridCellBase*)m_pLogViewerList->GetCell(nRow, nCol);

				// Set center alignment if defined
				if (m_apGrdColFormat[nCol].bCenter == TRUE) {
					if (pCell == NULL) continue;
					pCell->SetFormat(pCell->GetFormat() | DT_CENTER);
				}
				else {
					// Set margin (left alignment)
					if (pCell == NULL) continue;
					pCell->SetMargin(DEF_GRIDCELL_LEFTMARGIN);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadAppEventLogData
//	Description:	Load app event log data
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CLogViewerDlg::LoadAppEventLogData(void)
{
	// Get app event logging pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return FALSE;
	m_ptrAppEventLog = pApp->GetAppEventLog();
	if (m_ptrAppEventLog == NULL) return FALSE;

	// Get log data item count
	m_nLogCount = m_ptrAppEventLog->GetLogCount();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateLogViewer
//	Description:	Update logviewer list
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::UpdateLogViewer(void)
{
	// Check list table validity
	if (m_pLogViewerList == NULL) {
		TRCLOG("Error: LogViewer list control not found");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Get app event logging pointer
	if (!LoadAppEventLogData()) {
		TRCLOG("Error: Load app event log data failed");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Load app language package
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;
	LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();

	// Print items
	CString strTemp;
	int nTemp = -1;
	int nItemIndex = 0;
	for (int nRowIndex = ROW_INDEX_START; nRowIndex <= m_nLogCount; nRowIndex++) {
		
		// Get log item
		nItemIndex = nRowIndex - ROW_INDEX_START;
		LOGITEM logItem = m_ptrAppEventLog->GetLogItem(nItemIndex);

		// If log item is empty
		if (logItem.IsEmpty()) continue;

		// Date/time
		strTemp = logItem.FormatDateTime();
		m_pLogViewerList->SetItemText(nRowIndex, SCHCOL_ID_DATETIME, strTemp);

		// Category
		strTemp = GetLanguageString(ptrLanguage, logItem.usCategory);
		m_pLogViewerList->SetItemText(nRowIndex, SCHCOL_ID_CATEGORY, strTemp);

		// Additional description
		strTemp = logItem.strLogString;
		m_pLogViewerList->SetItemText(nRowIndex, SCHCOL_ID_DESCRIPTION, strTemp);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSelectLogItem
//	Description:	Show log details when selecting a log item
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::OnSelectLogItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Check table validity
	if (m_pLogViewerList == NULL)
		return;

	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	//Get current selection index
	m_nCurSelIndex = nRow - ROW_FIXED_NUM;

	// Get app event logging pointer
	if (m_ptrAppEventLog == NULL) return;

	// Check selection index validity
	CWnd* pDetailBtn = (CWnd*)GetDlgItem(IDC_LOGVIEWER_DETAILS_BTN);
	if (pDetailBtn == NULL) return;
	if (m_nCurSelIndex < 0 || m_nCurSelIndex > m_nLogCount) {
		// Disable [Details] button
		pDetailBtn->EnableWindow(FALSE);
	}
	else {
		// Enable [Details] button
		pDetailBtn->EnableWindow(TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayLogDetails
//	Description:	Display details of log item at specified index
//  Arguments:		nIndex - Index of item to display
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::DisplayLogDetails(int nIndex)
{
	// Get app event logging pointer
	if (m_ptrAppEventLog == NULL) return;

	// Get log item by index
	LOGITEM logItem = m_ptrAppEventLog->GetLogItem(nIndex);

	// Display details
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateLayoutInfo
//	Description:	Update layout info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::UpdateLayoutInfo(void)
{
	// Check table validity
	if (m_pLogViewerList == NULL) return;

	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Get table column count
	int nColNum = m_pLogViewerList->GetColumnCount();

	// Update size of table columns
	for (int nIndex = 0; nIndex < nColNum; nIndex++) {
		int nColSize = m_pLogViewerList->GetColumnWidth(nIndex);
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

void CLogViewerDlg::LoadLayoutInfo(void)
{
	// Define default table columns format
	const GRIDCTRLCOLFORMAT arrGrdColFormat[] = {
	//-----------ID------------------------Header title ID---------------Width(px)---Column style--------Align Center---
		{	SCHCOL_ID_DATETIME,		GRIDCOLUMN_LOGVIEWER_DATETIME,			220,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	SCHCOL_ID_CATEGORY,		GRIDCOLUMN_LOGVIEWER_CATEGORY,			220,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	SCHCOL_ID_DESCRIPTION,	GRIDCOLUMN_LOGVIEWER_DESCRIPTION,		-1,		COLSTYLE_NORMAL,		FALSE,	},
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
		if (GetLayoutInfo(IDS_REGSECTION_LAYOUT_LGVWRTABLE, strKeyName, nRet)) {
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

void CLogViewerDlg::SaveLayoutInfo(void)
{
	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Save layout info data to registry
	int nRef = 0;
	CString strKeyName;
	for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
		nRef = m_apGrdColFormat[nIndex].nWidth;
		strKeyName.Format(IDS_REGKEY_LAYOUT_GRIDCOLUMNSIZE, nIndex);
		WriteLayoutInfo(IDS_REGSECTION_LAYOUT_LGVWRTABLE, strKeyName, nRef);
	}
}
