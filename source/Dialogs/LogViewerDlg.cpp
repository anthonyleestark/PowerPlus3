/**
 * @file		LogViewerDlg.cpp
 * @brief		Source file for LogViewer dialog
 * @author		AnthonyLeeStark
 * @date		2024.02.24
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "AppCore/Logging.h"
#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"
#include "Dialogs/LogViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;
using namespace AppRegistry;


// Data list table constants
constexpr const int fixedColumnNum = 0;
constexpr const int fixedRowNum = 1;
constexpr const int startRowIndex = 1;


// Implement methods for CLogViewerDlg
IMPLEMENT_DYNAMIC(CLogViewerDlg, SDialog)


/**
 * @brief	Constructor
 */
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
}

/**
 * @brief	Destructor
 */
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

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CLogViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


// CLogViewerDlg dialog items ID map
BEGIN_RESOURCEID_MAP(CLogViewerDlg)
	ON_ID_DIALOG(IDD_LOGVIEWER_DLG,				 "LogViewerDlg")
	ON_ID_CONTROL(IDC_LOGVIEWER_LOGDATA_LISTBOX, "LogViewerList")
	ON_ID_CONTROL(IDC_LOGVIEWER_REMOVEALL_BTN,   "RemoveAllButton")
	ON_ID_CONTROL(IDC_LOGVIEWER_DETAILS_BTN,	 "DetailButton")
	ON_ID_CONTROL(IDC_LOGVIEWER_CLOSE_BTN,		 "CloseButton")
END_RESOURCEID_MAP()


// CLogViewerDlg dialog message map
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

/**
 * @brief	Initialize Logviewer dialog and setup dialog content
 * @param	None
 * @return	BOOL - Default
 */
BOOL CLogViewerDlg::OnInitDialog()
{
	// First, initialize base dialog class
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
	OutputEventLog(LOG_EVENT_DLG_INIT, this->GetCaption());

	return TRUE;
}

/**
 * @brief	Handle event when closing the dialog
 * @param	None
 * @return	None
 */
void CLogViewerDlg::OnClose()
{
	// Close dialog
	SDialog::OnClose();
}

/**
 * @brief	Default method for dialog destroying
 * @param	None
 * @return	None
 */
void CLogViewerDlg::OnDestroy()
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
 * @brief	Request current dialog to close
 * @param	None
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT CLogViewerDlg::RequestCloseDialog(void)
{
	// Request accepted
	return SDialog::RequestCloseDialog();
}

/**
 * @brief	Handle click event for [Remove All Records] button
 * @param	None
 * @return	None
 */
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

/**
 * @brief	Handle click event for [Details] button
 * @param	None
 * @return	None
 */
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

/**
 * @brief	Handle click event for [Close] button
 * @param	None
 * @return	None
 */
void CLogViewerDlg::OnCloseBtn()
{
	// Close dialog
	PostMessage(WM_CLOSE);
}

/**
 * @brief	Setup language for dialog items
 * @param	None
 * @return	None
 */
void CLogViewerDlg::SetupLanguage(void)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Setup dialog title
	this->SetCaptionFromLanguage(GetDialogID());

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		unsigned nID = pWndChild->GetDlgCtrlID();

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

	// Default
	SDialog::SetupLanguage();
}

/**
 * @brief	Initialize and setup language for LogViewer list
 * @param	ptrLanguage - Language package pointer
 * @return	None
 */
void CLogViewerDlg::SetupLogViewerList(LANGTABLE_PTR /*ptrLanguage*/)
{
	// Get parent list frame rect
	CWnd* pListFrameWnd = GetDlgItem(IDC_LOGVIEWER_LOGDATA_LISTBOX);
	if (pListFrameWnd == NULL) return;
	RECT rcListFrameWnd;
	pListFrameWnd->GetWindowRect(&rcListFrameWnd);
	ScreenToClient(&rcListFrameWnd);

	// Get frame size
	if (m_pszTableFrameSize == NULL) {
		m_pszTableFrameSize = new Size();
		m_pszTableFrameSize->_width = rcListFrameWnd.right - rcListFrameWnd.left;
		m_pszTableFrameSize->_height = rcListFrameWnd.bottom - rcListFrameWnd.top;
	}

	// Initialization
	VERIFY_INITIALIZATION(m_pLogViewerList, CGridCtrl)

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
	pCell->SetBackClr(Color::White);
	pCell->SetTextClr(Color::Black);
	pCell->SetHeight(Constant::UI::GridCtrl::Height::Row_Ex);

	// Table format and properties
	int nRowNum = (m_nLogCount + fixedRowNum);
	int nColNum = m_nColNum;

	// Setup table
	m_pLogViewerList->SetColumnCount(nColNum);
	m_pLogViewerList->SetRowCount(nRowNum);
	m_pLogViewerList->SetFixedRowCount(fixedRowNum);
	m_pLogViewerList->SetRowHeight(Constant::UI::GridCtrl::Index::Header_Row, Constant::UI::GridCtrl::Height::Row);

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

/**
 * @brief	Draw LogViewer list table
 * @param	None
 * @return	None
 */
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
	int nRowNum = (m_nLogCount + fixedRowNum);

	// Setup display size
	int nFrameHeight = m_pszTableFrameSize->Height();
	int nFrameWidth = m_pszTableFrameSize->Width();
	int nColWidthOffset = 0;
	if (GetWindowsOSVersion() == WINDOWS_VERSION_10) {
		// Windows 10 list control offset
		nFrameWidth -= Constant::UI::Offset::Width::ListCtrl_Win10;
		nFrameHeight -= Constant::UI::Offset::Height::ListCtrl_Win10;
		nColWidthOffset = Constant::UI::Offset::Width::ListColumn;
	}
	else {
		// Windows 11 list control offset
		nFrameWidth -= Constant::UI::Offset::Width::ListCtrl;
		nFrameHeight -= Constant::UI::Offset::Height::ListCtrl;
	}
	if ((Constant::UI::Offset::Width::ListCtrl + ((nRowNum - 1) * Constant::UI::GridCtrl::Height::Row_Ex)) >= nFrameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nFrameWidth -= (nScrollBarWidth + Constant::UI::Offset::Width::VScrollBar);
	}

	// Setup columns
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Set header row style
		SetFixedCellStyle(m_pLogViewerList, Constant::UI::GridCtrl::Index::Header_Row, nCol);

		// Column header title
		String headerTitle = Constant::String::Empty;
		unsigned nHeaderTitleID = m_apGrdColFormat[nCol].nHeaderTitleID;
		if (nHeaderTitleID != INT_NULL) {
			headerTitle = GetLanguageString(ptrLanguage, nHeaderTitleID);
		}
		m_pLogViewerList->SetItemText(Constant::UI::GridCtrl::Index::Header_Row, nCol, headerTitle);

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
	unsigned nItemState = INT_NULL;
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
					pCell->SetMargin(Constant::UI::GridCtrl::Margin::Left);
				}
			}
		}
	}
}

/**
 * @brief	Load app event log data
 * @param	None
 * @return	TRUE/FALSE
 */
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

/**
 * @brief	Update logviewer list
 * @param	None
 * @return	None
 */
void CLogViewerDlg::UpdateLogViewer(void)
{
	// Check list table validity
	if (m_pLogViewerList == NULL) {
		TRACE_ERROR("Error: LogViewer list control not found!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Get app event logging pointer
	if (!LoadAppEventLogData()) {
		TRACE_ERROR("Error: Load app event log data failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Load app language package
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;
	LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();

	// Print items
	int nItemIndex = 0;
	for (int nRowIndex = startRowIndex; nRowIndex <= m_nLogCount; nRowIndex++) {
		
		// Get log item
		nItemIndex = nRowIndex - startRowIndex;
		Item logItem = m_ptrAppEventLog->GetLogItem(nItemIndex);

		// If log item is empty
		if (logItem.IsEmpty()) continue;

		// Date/time
		String tempString = logItem.FormatDateTime();
		m_pLogViewerList->SetItemText(nRowIndex, ColumnID::DateTime, tempString);

		// Category
		tempString = GetLanguageString(ptrLanguage, logItem.GetCategory());
		m_pLogViewerList->SetItemText(nRowIndex, ColumnID::CategoryID, tempString);

		// Additional description
		tempString = logItem.GetLogString();
		m_pLogViewerList->SetItemText(nRowIndex, ColumnID::Description, tempString);
	}
}

/**
 * @brief	Show log details when selecting a log item
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CLogViewerDlg::OnSelectLogItem(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	// Check table validity
	if (m_pLogViewerList == NULL)
		return;

	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nRow = pItem->iRow;

	//Get current selection index
	m_nCurSelIndex = nRow - fixedRowNum;

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

/**
 * @brief	Display details of log item at specified index
 * @param	nIndex - Index of item to display
 * @return	None
 */
void CLogViewerDlg::DisplayLogDetails(int /*nIndex*/)
{
	// Get app event logging pointer
	if (m_ptrAppEventLog == NULL) return;
}

/**
 * @brief	Update layout info data
 * @param	None
 * @return	None
 */
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

/**
 * @brief	Load layout info data
 * @param	None
 * @return	None
 */
void CLogViewerDlg::LoadLayoutInfo(void)
{
	// Define default table columns format
	const GRIDCTRLCOLFORMAT arrGrdColFormat[] = {
	//-----------ID------------------------Header title ID---------------Width(px)---Column style--------Align Center---
		{	ColumnID::DateTime,		GRIDCOLUMN_LOGVIEWER_DATETIME,			220,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	ColumnID::CategoryID,	GRIDCOLUMN_LOGVIEWER_CATEGORY,			220,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	ColumnID::Description,	GRIDCOLUMN_LOGVIEWER_DESCRIPTION,		-1,		COLSTYLE_NORMAL,		FALSE,	},
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
		if (GetLayoutInfo(Section::LayoutInfo::LogViewerTable, keyName, nRet)) {
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
void CLogViewerDlg::SaveLayoutInfo(void)
{
	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Save layout info data to registry
	int nRef = 0;
	String keyName;
	for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
		nRef = m_apGrdColFormat[nIndex].nWidth;
		keyName = Key::LayoutInfo::GridColSize(nIndex);
		WriteLayoutInfo(Section::LayoutInfo::LogViewerTable, keyName, nRef);
	}
}
