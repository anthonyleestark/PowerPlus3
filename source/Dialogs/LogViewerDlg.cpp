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
constexpr const int kFixedColumnNum = 0;
constexpr const int kFixedRowNum = 1;
constexpr const int kStartRowIndex = 1;


// Implement methods for CLogViewerDlg
IMPLEMENT_DYNAMIC(CLogViewerDlg, SDialog)


/**
 * @brief	Constructor
 */
CLogViewerDlg::CLogViewerDlg(CWnd* parentWnd /*=nullptr*/)
	: SDialog(IDD_LOGVIEWER_DLG, parentWnd)
{
	// Init member variables
	logViewerListPtr_ = NULL;
	appEventLoggerPtr_ = NULL;
	logCount_ = 0;

	// Table format and properties
	columnCount_ = 0;
	gridCtrlFormatInfoPtr_ = NULL;
	logViewerTableSizePtr_ = NULL;

	// Other variables
	currentMode_ = 0;
	checkCount_ = 0;
	curSelIndex_ = -1;
}

/**
 * @brief	Destructor
 */
CLogViewerDlg::~CLogViewerDlg()
{
	// Grid table list control
	if (logViewerListPtr_) {
		delete logViewerListPtr_;
		logViewerListPtr_ = NULL;
	}

	// Other variables
	if (gridCtrlFormatInfoPtr_ != NULL) {
		delete[] gridCtrlFormatInfoPtr_;
		gridCtrlFormatInfoPtr_ = NULL;
	}
	if (logViewerTableSizePtr_ != NULL) {
		delete logViewerTableSizePtr_;
		logViewerTableSizePtr_ = NULL;
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
	setUseEnter(FALSE);

	// Register message box caption
	registerMessageBoxCaption(MSGBOX_LOGVIEWER_CAPTION);

	// Load app event log data pointer
	loadAppEventLogData();

	// Setup dialog
	loadLayoutInfo();
	setupLanguage();
	updateLogViewer();

	// Disable [Details] button
	CWnd* detailButtonPtr = (CWnd*)GetDlgItem(IDC_LOGVIEWER_DETAILS_BTN);
	if (detailButtonPtr != NULL) {
		detailButtonPtr->EnableWindow(FALSE);
	}

	// Save dialog event log if enabled
	outputEventLog(LOG_EVENT_DLG_INIT, this->getCaption());

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
	outputEventLog(LOG_EVENT_DLG_DESTROYED, this->getCaption());

	// Save layout info data
	updateLayoutInfo();
	saveLayoutInfo();

	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	Request current dialog to close
 * @param	None
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT CLogViewerDlg::requestCloseDialog(void)
{
	// Request accepted
	return SDialog::requestCloseDialog();
}

/**
 * @brief	Handle click event for [Remove All Records] button
 * @param	None
 * @return	None
 */
void CLogViewerDlg::OnRemoveAllBtn()
{
	// Get app event logging pointer
	if (appEventLoggerPtr_ == NULL) return;

	// Confirm before removing
	int returnValue = displayMessageBox(MSGBOX_LOGVIEWER_CONFIRM_REMOVEALLRECORDS, NULL, MB_YESNO | MB_ICONQUESTION);
	if (returnValue == IDNO) return;

	// Remove all app event log records
	appEventLoggerPtr_->deleteAll();

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
	if (appEventLoggerPtr_ == NULL) return;

	// Check selection index validity
	if (curSelIndex_ < 0 || curSelIndex_ > logCount_)
		return;

	// Display log details
	displayLogDetails(curSelIndex_);
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
void CLogViewerDlg::setupLanguage(void)
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Setup dialog title
	this->setCaptionFromLanguage(getDialogId());

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		unsigned id = childWndPtr->GetDlgCtrlID();

		switch (id)
		{
		case IDC_LOGVIEWER_LOGDATA_LISTBOX:
			// Skip these items
			break;
		case IDC_LOGVIEWER_DETAILS_BTN:
			showItem(id, false);
			break;
		default:
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Setup LogViewer list
	setupLogViewerList(languageTablePtr);

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Initialize and setup language for LogViewer list
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CLogViewerDlg::setupLogViewerList(LANGTABLE_PTR /*languageTablePtr*/)
{
	// Get parent list frame rect
	CWnd* listFrameWndPtr = GetDlgItem(IDC_LOGVIEWER_LOGDATA_LISTBOX);
	if (listFrameWndPtr == NULL) return;
	RECT listFrameWndRect;
	listFrameWndPtr->GetWindowRect(&listFrameWndRect);
	ScreenToClient(&listFrameWndRect);

	// Get frame size
	if (logViewerTableSizePtr_ == NULL) {
		logViewerTableSizePtr_ = new Size();
		logViewerTableSizePtr_->_width = listFrameWndRect.right - listFrameWndRect.left;
		logViewerTableSizePtr_->_height = listFrameWndRect.bottom - listFrameWndRect.top;
	}

	// Initialization
	VERIFY_INITIALIZATION(logViewerListPtr_, CGridCtrl)

	// Create table
	if (logViewerListPtr_ == NULL) return;
	DWORD style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	logViewerListPtr_->Create(listFrameWndRect, this, IDC_LOGVIEWER_LOGDATA_LISTBOX, style);

	// Destroy frame
	listFrameWndPtr->DestroyWindow();

	// Cell format
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)logViewerListPtr_->GetDefaultCell(FALSE, FALSE);
	if (cellPtr == NULL) return;
	cellPtr->SetFormat(cellPtr->GetFormat());
	cellPtr->SetMargin(0);
	cellPtr->SetBackClr(Color::White);
	cellPtr->SetTextClr(Color::Black);
	cellPtr->SetHeight(Constant::UI::GridCtrl::Height::Row_Ex);

	// Table format and properties
	int rowCount = (logCount_ + kFixedRowNum);
	int colCount = columnCount_;

	// Setup table
	logViewerListPtr_->SetColumnCount(colCount);
	logViewerListPtr_->SetRowCount(rowCount);
	logViewerListPtr_->SetFixedRowCount(kFixedRowNum);
	logViewerListPtr_->SetRowHeight(Constant::UI::GridCtrl::Index::Header_Row, Constant::UI::GridCtrl::Height::Row);

	// Draw table
	drawLogViewerTable();

	// Update layout info
	updateLayoutInfo();

	// Display table
	logViewerListPtr_->SetListMode(TRUE);
	logViewerListPtr_->SetEditable(FALSE);
	logViewerListPtr_->SetColumnResize(TRUE);
	logViewerListPtr_->SetRowResize(FALSE);
	logViewerListPtr_->EnableSelection(TRUE);
	logViewerListPtr_->SetSingleRowSelection(TRUE);
	logViewerListPtr_->SetSingleColSelection(FALSE);
	logViewerListPtr_->SetFixedRowSelection(FALSE);
	logViewerListPtr_->SetFixedColumnSelection(FALSE);
	logViewerListPtr_->ShowWindow(SW_SHOW);
	logViewerListPtr_->SetRedraw(TRUE);
}

/**
 * @brief	Draw LogViewer list table
 * @param	None
 * @return	None
 */
void CLogViewerDlg::drawLogViewerTable(void)
{
	// Check table validity
	if (logViewerListPtr_ == NULL) return;

	// Check table format data validity
	if (logViewerTableSizePtr_ == NULL) return;

	// Get app pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) return;

	// Get app language package
	LANGTABLE_PTR languageTablePtr = theAppPtr->getAppLanguage();

	// Re-update default cell properties
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)logViewerListPtr_->GetDefaultCell(FALSE, FALSE);
	if (cellPtr == NULL) return;

	// Table properties
	int colCount = columnCount_;
	int rowCount = (logCount_ + kFixedRowNum);

	// Setup display size
	int frameHeight = logViewerTableSizePtr_->height();
	int frameWidth = logViewerTableSizePtr_->width();
	int colWidthOffset = 0;
	if (AppCore::getWindowsOSVersion() == WindowsOS::Version::Win10) {
		// Windows 10 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl_Win10;
		frameHeight -= Constant::UI::Offset::Height::ListCtrl_Win10;
		colWidthOffset = Constant::UI::Offset::Width::ListColumn;
	}
	else {
		// Windows 11 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl;
		frameHeight -= Constant::UI::Offset::Height::ListCtrl;
	}
	if ((Constant::UI::Offset::Width::ListCtrl + ((rowCount - 1) * Constant::UI::GridCtrl::Height::Row_Ex)) >= frameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int scrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		frameWidth -= (scrollBarWidth + Constant::UI::Offset::Width::VScrollBar);
	}

	// Setup columns
	for (int col = 0; col < colCount; col++) {
		// Set header row style
		AppCore::setFixedCellStyle(logViewerListPtr_, Constant::UI::GridCtrl::Index::Header_Row, col);

		// Column header title
		String headerTitle = Constant::String::Empty;
		unsigned headerTitleId = gridCtrlFormatInfoPtr_[col].headerTitleId;
		if (headerTitleId != Constant::NullInteger) {
			headerTitle = getLanguageString(languageTablePtr, headerTitleId);
		}
		logViewerListPtr_->SetItemText(Constant::UI::GridCtrl::Index::Header_Row, col, headerTitle);

		// Column width
		int colWidth = gridCtrlFormatInfoPtr_[col].width;
		if (colWidth != -1) {
			// Set column width as defined (with offset)
			int nColWidthPx = colWidth - colWidthOffset;
			if (logViewerListPtr_->SetColumnWidth(col, nColWidthPx)) {
				// Calculate remaining width (not using offset)
				frameWidth -= colWidth;
			}
		}
		else {
			// Set remaining width for current column
			logViewerListPtr_->SetColumnWidth(col, frameWidth);
		}
	}

	// Setup rows
	int colStyle = -1;
	unsigned itemState = Constant::NullInteger;
	for (int row = 1; row < rowCount; row++) {
		for (int col = 0; col < columnCount_; col++) {

			// Get column style & item state
			colStyle = gridCtrlFormatInfoPtr_[col].columnStyle;
			itemState = logViewerListPtr_->GetItemState(row, col);
			itemState |= GVIS_READONLY;

			// Base column - header-like style
			if (colStyle == COLSTYLE_FIXED) {
				// Set fixed cell style
				AppCore::setFixedCellStyle(logViewerListPtr_, row, col);
			}

			// Checkbox column
			else if (colStyle == COLSTYLE_CHECKBOX) {
				// Set cell type: Checkbox
				if (!logViewerListPtr_->SetCellType(row, col, RUNTIME_CLASS(CGridCellCheck)))
					continue;

				// Get cell
				CGridCellCheck* cellPtr = (CGridCellCheck*)logViewerListPtr_->GetCell(row, col);

				// Set center alignment if defined
				if (gridCtrlFormatInfoPtr_[col].isCentered == TRUE) {
					if (cellPtr == NULL) continue;
					cellPtr->SetCheckPlacement(SCP_CENTERING);
				}
			}

			// Normal column
			else if (colStyle == COLSTYLE_NORMAL) {
				// Set item state
				if (!logViewerListPtr_->SetItemState(row, col, itemState))
					continue;

				// Get cell
				CGridCellBase* cellPtr = (CGridCellBase*)logViewerListPtr_->GetCell(row, col);

				// Set center alignment if defined
				if (gridCtrlFormatInfoPtr_[col].isCentered == TRUE) {
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
 * @brief	Load app event log data
 * @param	None
 * @return	TRUE/FALSE
 */
BOOL CLogViewerDlg::loadAppEventLogData(void)
{
	// Get app event logging pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) return FALSE;
	appEventLoggerPtr_ = theAppPtr->getAppEventLog();
	if (appEventLoggerPtr_ == NULL) return FALSE;

	// Get log data item count
	logCount_ = appEventLoggerPtr_->getLogCount();

	return TRUE;
}

/**
 * @brief	Update logviewer list
 * @param	None
 * @return	None
 */
void CLogViewerDlg::updateLogViewer(void)
{
	// Check list table validity
	if (logViewerListPtr_ == NULL) {
		TRACE_ERROR("Error: LogViewer list control not found!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Get app event logging pointer
	if (!loadAppEventLogData()) {
		TRACE_ERROR("Error: Load app event log data failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Load app language package
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) return;
	LANGTABLE_PTR languageTablePtr = theAppPtr->getAppLanguage();

	// Print items
	int itemIndex = 0;
	for (int rowIndex = kStartRowIndex; rowIndex <= logCount_; rowIndex++) {
		
		// Get log item
		itemIndex = rowIndex - kStartRowIndex;
		Item logItem = appEventLoggerPtr_->getLogItem(itemIndex);

		// If log item is empty
		if (logItem.isEmpty()) continue;

		// Date/time
		String tempString = logItem.formatDateTime();
		logViewerListPtr_->SetItemText(rowIndex, ColumnID::DateTime, tempString);

		// Category
		tempString = getLanguageString(languageTablePtr, logItem.getCategory());
		logViewerListPtr_->SetItemText(rowIndex, ColumnID::CategoryID, tempString);

		// Additional description
		tempString = logItem.getLogString();
		logViewerListPtr_->SetItemText(rowIndex, ColumnID::Description, tempString);
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
	if (logViewerListPtr_ == NULL)
		return;

	// Get clicked item info
	NM_GRIDVIEW* reminderItem = (NM_GRIDVIEW*)pNMHDR;
	if (reminderItem == NULL) return;
	int row = reminderItem->iRow;

	//Get current selection index
	curSelIndex_ = row - kFixedRowNum;

	// Get app event logging pointer
	if (appEventLoggerPtr_ == NULL) return;

	// Check selection index validity
	CWnd* detailButtonPtr = (CWnd*)GetDlgItem(IDC_LOGVIEWER_DETAILS_BTN);
	if (!detailButtonPtr || !detailButtonPtr->IsWindowVisible()) return;
	if (curSelIndex_ < 0 || curSelIndex_ > logCount_) {
		// Disable [Details] button
		detailButtonPtr->EnableWindow(FALSE);
	}
	else {
		// Enable [Details] button
		detailButtonPtr->EnableWindow(TRUE);
	}
}

/**
 * @brief	Display details of log item at specified index
 * @param	index - Index of item to display
 * @return	None
 */
void CLogViewerDlg::displayLogDetails(int /*index*/)
{
	// Get app event logging pointer
	if (appEventLoggerPtr_ == NULL) return;
}

/**
 * @brief	Update layout info data
 * @param	None
 * @return	None
 */
void CLogViewerDlg::updateLayoutInfo(void)
{
	// Check table validity
	if (logViewerListPtr_ == NULL) return;

	// Check table column format data validity
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Get table column count
	int colCount = logViewerListPtr_->GetColumnCount();

	// Update size of table columns
	for (int index = 0; index < colCount; index++) {
		int colSize = logViewerListPtr_->GetColumnWidth(index);
		gridCtrlFormatInfoPtr_[index].width = colSize;
	}
}

/**
 * @brief	Load layout info data
 * @param	None
 * @return	None
 */
void CLogViewerDlg::loadLayoutInfo(void)
{
	// Define default table columns format
	const GRIDCTRLCOLFORMAT gridColFormatInfo[] = {
	//-----------ID------------------------Header title ID---------------Width(px)---Column style--------Align Center---
		{	ColumnID::DateTime,		GRIDCOLUMN_LOGVIEWER_DATETIME,			220,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	ColumnID::CategoryID,	GRIDCOLUMN_LOGVIEWER_CATEGORY,			220,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	ColumnID::Description,	GRIDCOLUMN_LOGVIEWER_DESCRIPTION,		-1,		COLSTYLE_NORMAL,		FALSE,	},
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
		if (getLayoutInfo(Section::LayoutInfo::LogViewerTable, keyName, returnValue)) {
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
void CLogViewerDlg::saveLayoutInfo(void)
{
	// Check table column format data validity
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Save layout info data to registry
	int referValue = 0;
	String keyName;
	for (int index = 0; index < columnCount_; index++) {
		referValue = gridCtrlFormatInfoPtr_[index].width;
		keyName = Key::LayoutInfo::GridColSize(index);
		writeLayoutInfo(Section::LayoutInfo::LogViewerTable, keyName, referValue);
	}
}
