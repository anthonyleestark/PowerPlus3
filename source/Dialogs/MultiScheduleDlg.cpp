/**
 * @file		MultiScheduleDlg.cpp
 * @brief		Source file for Schedule dialog
 * @author		AnthonyLeeStark
 * @date		2017.03.08
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"
#include "Dialogs/MultiScheduleDlg.h"
#include "Dialogs/EditScheduleDlg.h"
 
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
constexpr const int defaultRowIndex = 1;
constexpr const int extraStartRowIndex = 2;

// Implement methods for CMultiScheduleDlg
IMPLEMENT_DYNAMIC(CMultiScheduleDlg, SDialog)


/**
 * @brief	Constructor
 */
CMultiScheduleDlg::CMultiScheduleDlg() : SDialog(IDD_MULTISCHEDULE_DLG)
{
	// Initialize member variables

	// Dialog control item
	m_pDataItemListTable = NULL;

	// Child dialog
	m_pEditScheduleDlg = NULL;

	// Data container variables
	m_schSchedule.init();
	m_schScheduleTemp.init();

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

/**
 * @brief	Destructor
 */
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
	m_schSchedule.deleteAll();
	m_schScheduleTemp.deleteAll();

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

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CMultiScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CMultiScheduleDlg::registerDialogManagement(void)
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
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_ITEM_LISTBOX, List_Control);
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_ADD_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_EDIT_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_REMOVE_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_REMOVEALL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_CHECKALL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_UNCHECKALL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_VIEWDETAILS_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_SETDEFAULT_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_APPLY_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_MULTISCHEDULE_CANCEL_BTN, Button);
	}

	return returnValue;
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CMultiScheduleDlg::unregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

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

	return SDialog::unregisterDialogManagement();
}


// CMultiScheduleDlg dialog items ID map
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


// CMultiScheduleDlg dialog message map
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

/**
 * @brief	Initialize Schedule dialog and setup dialog content
 * @param	None
 * @return	BOOL - Default
 */
BOOL CMultiScheduleDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Do not use Enter button
	setUseEnter(false);

	// Register message box caption
	registerMessageBoxCaption(MSGBOX_MULTISCHEDULE_CAPTION);

	// Load data
	LoadScheduleSettings();

	// Init dialog items
	loadLayoutInfo();
	setupLanguage();
	setupDialogItemState();

	// Update data
	UpdateDataItemList();
	refreshDialogItemState(true);

	// Save dialog event log if enabled
	outputEventLog(LOG_EVENT_DLG_INIT, this->getCaption());

	return true;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnClose()
{
	// If not forced closing by request
	if (!isForceClosingByRequest()) {

		// If data changed, ask for saving before closing dialog
		if (getFlagValue(AppFlagID::dialogDataChanged) == true) {
			// Setup messagebox language
			LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
			const wchar_t* messagePrompt = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
			const wchar_t* messageCaption = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int nConfirm = MessageBox(messagePrompt, messageCaption, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Save data
				SaveScheduleSettings();
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
void CMultiScheduleDlg::OnDestroy()
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
LRESULT CMultiScheduleDlg::requestCloseDialog(void)
{
	// If detail edit dialog is opening
	if (m_pEditScheduleDlg != NULL) {
		// Request close dialog
		LRESULT resCloseReq = m_pEditScheduleDlg->requestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}

	// If data changed, ask for saving before closing dialog
	if (getFlagValue(AppFlagID::dialogDataChanged) == true) {
		// Setup messagebox language
		LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
		const wchar_t* messagePrompt = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
		const wchar_t* messageCaption = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

		int nConfirm = MessageBox(messagePrompt, messageCaption, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Save data
			SaveScheduleSettings();
		}
		else if (nConfirm == IDCANCEL) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}

	// Request accepted
	return SDialog::requestCloseDialog();
}

/**
 * @brief	Setup language for dialog items
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::setupLanguage()
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
		case IDC_MULTISCHEDULE_ITEM_LISTBOX:
			// Skip these items
			break;
		default:
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Setup Schedule data item list
	SetupDataItemList(languageTablePtr);

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Initialize and setup language for Schedule item list
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CMultiScheduleDlg::SetupDataItemList(LANGTABLE_PTR /*languageTablePtr*/)
{
	// Get parent list frame rect
	CWnd* pListFrameWnd = GetDlgItem(IDC_MULTISCHEDULE_ITEM_LISTBOX);
	if (pListFrameWnd == NULL) return;
	RECT rcListFrameWnd;
	pListFrameWnd->GetWindowRect(&rcListFrameWnd);
	ScreenToClient(&rcListFrameWnd);

	// Get frame size
	if (m_pszDataTableFrameSize == NULL) {
		m_pszDataTableFrameSize = new Size();
		m_pszDataTableFrameSize->_width = rcListFrameWnd.right - rcListFrameWnd.left;
		m_pszDataTableFrameSize->_height = rcListFrameWnd.bottom - rcListFrameWnd.top;
	}

	// Initialization
	VERIFY_INITIALIZATION(m_pDataItemListTable, CGridCtrl)

	// Create table
	if (m_pDataItemListTable == NULL) return;
	DWORD style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	m_pDataItemListTable->Create(rcListFrameWnd, this, IDC_MULTISCHEDULE_ITEM_LISTBOX, style);

	// Destroy frame
	pListFrameWnd->DestroyWindow();

	// Cell format
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pDataItemListTable->GetDefaultCell(false, false);
	if (pCell == NULL) return;
	pCell->SetFormat(pCell->GetFormat());
	pCell->SetMargin(0);
	pCell->SetBackClr(Color::White);
	pCell->SetTextClr(Color::Black);
	pCell->SetHeight(Constant::UI::GridCtrl::Height::Row_Ex);

	// Table format and properties
	int nRowNum = (GetTotalItemNum() + fixedRowNum);
	int nColNum = m_nColNum;

	// Setup table
	m_pDataItemListTable->SetColumnCount(nColNum);
	m_pDataItemListTable->SetFixedColumnCount(fixedColumnNum);
	m_pDataItemListTable->SetRowCount(nRowNum);
	m_pDataItemListTable->SetFixedRowCount(fixedRowNum);
	m_pDataItemListTable->SetRowHeight(Constant::UI::GridCtrl::Index::Header_Row, Constant::UI::GridCtrl::Height::Header);

	// Draw table
	DrawDataTable(isReadOnlyMode());

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
 * @brief	Draw Schedule data list table
 * @param	bReadOnly - Read-only mode
 * @return	None
 */
void CMultiScheduleDlg::DrawDataTable(bool bReadOnly /* = false */)
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Check table format data validity
	if (m_pszDataTableFrameSize == NULL) return;

	// Get app pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) return;

	// Get app language package
	LANGTABLE_PTR languageTablePtr = theAppPtr->getAppLanguage();

	// Re-update default cell properties
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pDataItemListTable->GetDefaultCell(false, false);
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
	
	// Table properties
	int nColNum = m_nColNum;
	int nRowNum = (GetTotalItemNum() + fixedRowNum);

	// Setup display size
	int nFrameHeight = m_pszDataTableFrameSize->height();
	int nFrameWidth = m_pszDataTableFrameSize->width();
	if (AppCore::getWindowsOSVersion() == WINDOWS_VERSION_10) {
		// Windows 10 list control offset
		nFrameWidth -= Constant::UI::Offset::Width::ListCtrl_Win10;
		//nFrameHeight -= OFFSET_HEIGHT_LISTCTRL_WIN10;
	}
	else {
		// Windows 11 list control offset
		nFrameWidth -= Constant::UI::Offset::Width::ListCtrl;
		//nFrameHeight -= OFFSET_HEIGHT_LISTCTRL;
	}
	if ((Constant::UI::GridCtrl::Height::Header + ((nRowNum - 1) * Constant::UI::GridCtrl::Height::Row_Ex)) >= nFrameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nFrameWidth -= (nScrollBarWidth + Constant::UI::Offset::Width::VScrollBar);
	}

	// Setup columns
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Set header row style
		AppCore::setFixedCellStyle(m_pDataItemListTable, Constant::UI::GridCtrl::Index::Header_Row, nCol);

		// Column header title
		String headerTitle = Constant::String::Empty;
		unsigned nHeaderTitleID = m_apGrdColFormat[nCol].headerTitleId;
		if (nHeaderTitleID != INT_NULL) {
			headerTitle = getLanguageString(languageTablePtr, nHeaderTitleID);
		}
		m_pDataItemListTable->SetItemText(Constant::UI::GridCtrl::Index::Header_Row, nCol, headerTitle);

		// Column width
		int nColWidth = m_apGrdColFormat[nCol].width;
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
	unsigned nItemState = INT_NULL;
	for (int nRow = 1; nRow < nRowNum; nRow++) {
		for (int nCol = 0; nCol < m_nColNum; nCol++) {

			// Get column style & item state
			nColStyle = m_apGrdColFormat[nCol].columnStyle;
			nItemState = m_pDataItemListTable->GetItemState(nRow, nCol);
			nItemState |= GVIS_READONLY;

			// Highlight default schedule item
			if ((nRow == defaultRowIndex) && (nColStyle != COLSTYLE_FIXED) && (bReadOnly != true)) {
				CGridCellBase* pCellBase = m_pDataItemListTable->GetCell(nRow, nCol);
				if (pCellBase != NULL) {
					pCellBase->SetBackClr(Color::Yellow);
					pCellBase->SetTextClr(Color::Red);
				}
			}

			// Base column - header-like style
			if (nColStyle == COLSTYLE_FIXED) {
				// Set fixed cell style
				AppCore::setFixedCellStyle(m_pDataItemListTable, nRow, nCol);
			}

			// Checkbox column
			else if (nColStyle == COLSTYLE_CHECKBOX) {
				// Set cell type: Checkbox
				if (!m_pDataItemListTable->SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck)))
					continue;

				// Get cell
				CGridCellCheck* pCell = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRow, nCol);

				// Set center alignment if defined
				if (m_apGrdColFormat[nCol].isCentered == true) {
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
				if (m_apGrdColFormat[nCol].isCentered == true) {
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
 * @brief	Update layout info data
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::updateLayoutInfo(void)
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Get table column count
	int nColNum = m_pDataItemListTable->GetColumnCount();

	// Update size of table columns
	for (int index = 0; index < nColNum; index++) {
		int nColSize = m_pDataItemListTable->GetColumnWidth(index);
		m_apGrdColFormat[index].width = nColSize;
	}
}

/**
 * @brief	Load layout info data
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::loadLayoutInfo(void)
{
	// Define default table columns format
	const GRIDCTRLCOLFORMAT arrGrdColFormat[] = {
	//-----------ID------------------------Header title ID---------------Width(px)---Column style--------Align Center---
		{	ColumnID::Index,		GRIDCOLUMN_MULTISCHEDULE_INDEX,			26,		COLSTYLE_FIXED,			true,	},
		{	ColumnID::EnableState,	GRIDCOLUMN_MULTISCHEDULE_STATE,			52,		COLSTYLE_CHECKBOX,		true,	},
		{	ColumnID::ActionID,		GRIDCOLUMN_MULTISCHEDULE_ACTIONID,		140,	COLSTYLE_NORMAL,		true,	},
		{ 	ColumnID::TimeValue,	GRIDCOLUMN_MULTISCHEDULE_TIMEVALUE,		135,	COLSTYLE_NORMAL,		true,	},
		{ 	ColumnID::Repeat,		GRIDCOLUMN_MULTISCHEDULE_REPEAT,		-1,		COLSTYLE_CHECKBOX,		true,	},
	//------------------------------------------------------------------------------------------------------------------
	};

	// Backup format data
	m_nColNum = (sizeof(arrGrdColFormat) / sizeof(GRIDCTRLCOLFORMAT));

	// Initialize table format info data
	if (m_apGrdColFormat == NULL) {
		m_apGrdColFormat = new GRIDCTRLCOLFORMAT[m_nColNum];
		for (int index = 0; index < m_nColNum; index++) {
			// Copy default table column format data
			m_apGrdColFormat[index] = arrGrdColFormat[index];
		}
	}

	// Load layout info data from registry
	int returnValue = 0;
	String keyName;
	for (int index = 0; index < m_nColNum; index++) {
		keyName = Key::LayoutInfo::GridColSize(index);
		if (GetLayoutInfo(Section::LayoutInfo::MultiScheduleTable, keyName, returnValue)) {
			if (m_apGrdColFormat != NULL) {
				m_apGrdColFormat[index].width = returnValue;
			}
		}
	}
}

/**
 * @brief	Save layout info data
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::saveLayoutInfo(void)
{
	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Save layout info data to registry
	int nRef = 0;
	String keyName;
	for (int index = 0; index < m_nColNum; index++) {
		nRef = m_apGrdColFormat[index].width;
		keyName = Key::LayoutInfo::GridColSize(index);
		WriteLayoutInfo(Section::LayoutInfo::MultiScheduleTable, keyName, nRef);
	}
}

/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::setupDialogItemState()
{
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
 * @brief	Update Schedule data item list table
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::UpdateDataItemList()
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// If there's no item, do nothing
	int itemNum = GetTotalItemNum();
	if (itemNum <= 0)
		return;

	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Print items
	int nTemp = -1;
	int nExtraItemIndex = 0;
	CGridCellCheck* pCellCheck = NULL;
	for (int nRowIndex = defaultRowIndex; nRowIndex <= itemNum; nRowIndex++) {
		
		// Get schedule item
		Item scheduleItem;
		if (nRowIndex == defaultRowIndex) {
			// Get schedule default item
			scheduleItem = m_schScheduleTemp.getDefaultItem();
		}
		else {
			// Get schedule extra item
			nExtraItemIndex = nRowIndex - extraStartRowIndex;
			scheduleItem = m_schScheduleTemp.getItemAt(nExtraItemIndex);
		}

		// If schedule item is empty
		if (scheduleItem.isEmpty()) continue;

		// Item index
		String tempString = StringUtils::stringFormat(_T("%d"), nRowIndex);
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::Index, tempString);

		// Enable state
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::EnableState);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(scheduleItem.isEnabled());
		}

		// Action name
		nTemp = GetPairedID(IDTable::ActionName, scheduleItem.getAction());
		tempString = getLanguageString(languageTablePtr, nTemp);
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::ActionID, tempString);

		// Time setting
		const wchar_t* formatString = getLanguageString(languageTablePtr, GRIDCOLUMN_MULTISCHEDULE_TIMEFORMAT);
		tempString = ClockTimeUtils::format(languageTablePtr, formatString, scheduleItem.getTime());
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::TimeValue, tempString);

		// Repeat
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::Repeat);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(scheduleItem.isRepeatEnabled());
		}
	}
}

/**
 * @brief	Disable mouse click events for Schedule data table
 * @param	bDisable - Disable/enable
 * @return	None
 */
void CMultiScheduleDlg::DisableDataTable(bool bDisable)
{
	// Redraw read-only style
	RedrawDataTable(bDisable);

	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Disable/enable mouse events
	m_pDataItemListTable->DisableMouseClick(bDisable);
	m_pDataItemListTable->DisableMouseMove(bDisable);
}

/**
 * @brief	Update and redraw Schedule data table
 * @param	bool bReadOnly - Read-only mode
 * @return	None
 */
void CMultiScheduleDlg::RedrawDataTable(bool bReadOnly /* = false */)
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Update new row number
	int nCurRowNum = (GetTotalItemNum() + fixedRowNum);
	m_pDataItemListTable->SetRowCount(nCurRowNum);

	// Draw table
	DrawDataTable(bReadOnly);

	// Update table data
	UpdateDataItemList();

	// Trigger redrawing table
	m_pDataItemListTable->RedrawWindow();
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	bRecheckState - Recheck all item's state
 * @return	None
 */
void CMultiScheduleDlg::refreshDialogItemState(bool bRecheckState /* = false */)
{
	CWnd* pBtn = NULL;

	// If dialog items are being locked, do nothing
	if (getLockState() == true)
		return;

	// Check if any item is selected or not
	bool bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	// Check if selected item is an extra item or not
	bool bIsExtraSelected = ((bIsSelected == true) && ((m_nCurSelIndex + fixedRowNum) >= extraStartRowIndex));

	// Check if number of extra item has reached the limit
	bool bIsMaxNum = (GetExtraItemNum() >= ScheduleData::maxItemNum);

	// Check if data is all empty or not
	bool bIsAllEmpty = m_schScheduleTemp.isAllEmpty();

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Disable [Add] button if item number has reached the limit
	pBtn = GetDlgItem(IDC_MULTISCHEDULE_ADD_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(!bIsMaxNum);
		pBtn->SetWindowText(getLanguageString(languageTablePtr, IDC_MULTISCHEDULE_ADD_BTN));
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
		pBtn->SetWindowText(getLanguageString(languageTablePtr, IDC_MULTISCHEDULE_EDIT_BTN));
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
	bool isChanged = checkDataChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);

	// Enable [Apply] button if data is changed
	pBtn = GetDlgItem(IDC_MULTISCHEDULE_APPLY_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(isChanged);
	}

	// Update [Check/Uncheck All] button state
	UpdateCheckAllBtnState(bRecheckState);

	// Default
	SDialog::refreshDialogItemState(bRecheckState);
}

/**
 * @brief	Refresh and update state for [Check/Uncheck All] button
 * @param	bRecheck - Recheck all items enable state
 * @return	None
 */
void CMultiScheduleDlg::UpdateCheckAllBtnState(bool bRecheck /* = false */)
{
	// If dialog items are being locked, do nothing
	if (getLockState() == true)
		return;

	// Get buttons
	CWnd* pCheckAllBtn = GetDlgItem(IDC_MULTISCHEDULE_CHECKALL_BTN);
	CWnd* pUncheckAllBtn = GetDlgItem(IDC_MULTISCHEDULE_UNCHECKALL_BTN);
	if ((pCheckAllBtn == NULL) || (pUncheckAllBtn == NULL))
		return;

	// Get number of items
	int itemNum = GetTotalItemNum();
	if (itemNum == 0) {
		// Disable both [Check/Uncheck All] buttons
		pCheckAllBtn->EnableWindow(false);
		pUncheckAllBtn->EnableWindow(false);
		return;
	}

	// Recheck all items state
	if (bRecheck == true) {

		// Reset counter
		m_nCheckCount = 0;
		// Check default item
		if (m_schScheduleTemp.getDefaultItem().isEnabled() == true) {
			// Increase counter
			m_nCheckCount++;
		}
		// Check extra items
		for (int extraIndex = 0; extraIndex < GetExtraItemNum(); extraIndex++) {
			const Item& schTemp = m_schScheduleTemp.getItemAt(extraIndex);
			if (schTemp.isEnabled() == true) {
				// Increase counter
				m_nCheckCount++;
			}
		}
	}

	// Update button state
	if (m_nCheckCount == 0) {
		// Enable [Check All] button
		pCheckAllBtn->EnableWindow(true);
		// Disable [Uncheck All] button
		pUncheckAllBtn->EnableWindow(false);
	}
	else if (m_nCheckCount == itemNum) {
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
 * @brief	Load schedule settings from configurations
 * @param	None
 * @return	true/false
 */
bool CMultiScheduleDlg::LoadScheduleSettings()
{
	// Get app Schedule data pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	VERIFY(theAppPtr != NULL);
	if (theAppPtr == NULL) return false;
	Data* pschData = theAppPtr->getAppScheduleData();
	if (pschData == NULL)
		return false;

	// Copy data
	m_schSchedule.copy(*pschData);
	m_schScheduleTemp.copy(m_schSchedule);

	// Reset change flag
	setFlagValue(AppFlagID::dialogDataChanged, false);

	return true;
}

/**
 * @brief	Save schedule settings to configurations
 * @param	None
 * @return	bool - Result of saving process
 */
bool CMultiScheduleDlg::SaveScheduleSettings()
{
	// Update settings
	m_schSchedule.copy(m_schScheduleTemp);

	// Save app schedule data
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	VERIFY(theAppPtr != NULL);
	if (theAppPtr == NULL) return false;
	theAppPtr->setAppScheduleData(&m_schSchedule);
	theAppPtr->saveRegistryAppData(APPDATA_SCHEDULE);

	// Notify main dialog to re-update schedule data
	CPowerPlusDlg* mainDialogPtr = (CPowerPlusDlg*)(theAppPtr->GetMainWnd());
	VERIFY(mainDialogPtr != NULL);
	if (mainDialogPtr == NULL) return false;
	mainDialogPtr->PostMessage(SM_APP_UPDATE_SCHEDULEDATA, NULL, NULL);

	// Reset change flag
	setFlagValue(AppFlagID::dialogDataChanged, false);

	return true;
}

/**
 * @brief	Update and refresh schedule settings
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::UpdateScheduleSettings()
{
	// Update enable and repeat states of each item
	int nExtraItemIndex = 0;
	CGridCellCheck* pCellCheckEnable = NULL;
	CGridCellCheck* pCellCheckRepeat = NULL;
	int nItemRowNum = (m_pDataItemListTable->GetRowCount() - fixedRowNum);
	for (int nRowIndex = defaultRowIndex; nRowIndex <= nItemRowNum; nRowIndex++) {
		
		// In case of extra item rows
		if (nRowIndex > defaultRowIndex) {
			// Get extra item index
			nExtraItemIndex = nRowIndex - extraStartRowIndex;
		}

		// Get checkbox cells
		pCellCheckEnable = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::EnableState);
		pCellCheckRepeat = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::Repeat);
		if ((pCellCheckEnable == NULL) || (pCellCheckRepeat == NULL)) continue;

		// Get checked states
		bool isEnabled = pCellCheckEnable->GetCheck();
		bool bRepeat = pCellCheckRepeat->GetCheck();

		// Update item enable and repeat states
		if (nRowIndex == defaultRowIndex) {
			// Update default item data
			Item& tempDefaultItem = m_schScheduleTemp.getDefaultItem();
			tempDefaultItem.enableItem(isEnabled);
			tempDefaultItem.enableRepeat(bRepeat);
		}
		else {
			// Update extra item data
			Item& schTempItem = m_schScheduleTemp.getItemAt(nExtraItemIndex);
			schTempItem.enableItem(isEnabled);
			schTempItem.enableRepeat(bRepeat);
		}
	}
}

/**
 * @brief	Compare control values with current config to check for changes
 * @param	None
 * @return	bool - Flag to mark data changed or not
 */
bool CMultiScheduleDlg::checkDataChangeState()
{
	bool changeFlag = false;

	// Update settings
	UpdateScheduleSettings();

	// Check if number of extra items changed
	int extraItemNum = m_schScheduleTemp.getExtraItemNum();
	changeFlag |= (extraItemNum != m_schSchedule.getExtraItemNum());
	if (changeFlag == true)
		return changeFlag;

	// Check if default item's data changed
	changeFlag |= (m_schScheduleTemp.getDefaultItem().compare(m_schSchedule.getDefaultItem()) != true);
	if (changeFlag == true)
		return changeFlag;

	// Check if each extra item's data changed
	for (int extraIndex = 0; extraIndex < extraItemNum; extraIndex++) {
		// Get current item and temp item
		const Item& schCurItem = m_schSchedule.getItemAt(extraIndex);
		const Item& schTempItem = m_schScheduleTemp.getItemAt(extraIndex);

		// Data comparison
		changeFlag |= (schTempItem.compare(schCurItem) != true);

		// Stop on the first different item encountered
		if (changeFlag == true) break;
	}

	return changeFlag;
}

/**
 * @brief	Add item into schedule data
 * @param	scheduleItem - Schedule item
 * @return	None
 */
void CMultiScheduleDlg::Add(Item& scheduleItem)
{
	// If item is empty, do nothing
	if (scheduleItem.isEmpty())
		return;

	// Check data validity
	bool bValid = Validate(scheduleItem, true);
	if (bValid == false)
		return;

	// Update item to Action Schedule data item list
	m_schScheduleTemp.update(scheduleItem);

	// Update table
	RedrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Update schedule item
 * @param	scheduleItem - Schedule item
 * @return	None
 */
void CMultiScheduleDlg::Update(Item& scheduleItem)
{
	// If item is empty, do nothing
	if (scheduleItem.isEmpty())
		return;

	// Check data validity
	bool bValid = Validate(scheduleItem, true);
	if (bValid == false)
		return;

	// Update item to Action Schedule data item list
	m_schScheduleTemp.update(scheduleItem);

	// Update table
	RedrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Remove a schedule item by index
 * @param	index - Index of item to remove
 * @return	None
 */
void CMultiScheduleDlg::Remove(int index)
{
	// Remove item at index
	m_schScheduleTemp.deleteItem(index);

	// Update table
	RedrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Remove all schedule items
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::RemoveAll()
{
	// Remove all items
	m_schScheduleTemp.deleteExtra();

	// Update table
	RedrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Check/uncheck all Schedule data items
 * @param	bState - Item state
 * @return	None
 */
void CMultiScheduleDlg::SetAllItemState(bool bState)
{
	// Check/uncheck all --> Update all items enable state
	Item& schDefTemp = m_schScheduleTemp.getDefaultItem();
	if (schDefTemp.isEnabled() != bState) {
		// Change checked state
		schDefTemp.enableItem(bState);
	}
	for (int extraIndex = 0; extraIndex < GetExtraItemNum(); extraIndex++) {
		Item& schTemp = m_schScheduleTemp.getItemAt(extraIndex);
		if (schTemp.isEnabled() != bState) {
			// Change checked state
			schTemp.enableItem(bState);
		}
	}

	// Update number of checked items
	m_nCheckCount = (bState == false) ? 0 : GetTotalItemNum();

	// Update data item list
	UpdateDataItemList();

	// Refresh button state
	refreshDialogItemState(false);
}

/**
 * @brief	Check Schedule item validity
 * @param	scheduleItem		 - Item to validate
 * @param	bShowMsg	 - Show validation message box or not
 * @param	bAutoCorrect - Invalid value auto correction (ON/OFF)
 * @return	bool - Result of validation process
 */
bool CMultiScheduleDlg::Validate(Item& scheduleItem, bool bShowMsg /* = false */, bool bAutoCorrect /* = false */)
{
	bool result = true;

	int messageStringId;
	StringArray arrMsgString;
	arrMsgString.clear();

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Check item ID
	if ((scheduleItem.getItemId() != ScheduleData::defaultItemID) &&
		((scheduleItem.getItemId() < ScheduleData::minItemID) || (scheduleItem.getItemId() > ScheduleData::maxItemID))) {
		messageStringId = MSGBOX_MULTISCHEDULE_INVALIDITEM_ITEMID;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Get next item ID
			scheduleItem.setItemId(m_schScheduleTemp.getNextId());
		}
	}

	// Check action ID
	if ((scheduleItem.getAction() < APP_ACTION_NOTHING) || (scheduleItem.getAction() > APP_ACTION_HIBERNATE)) {
		messageStringId = MSGBOX_MULTISCHEDULE_INVALIDITEM_ACTIONID;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Set default action
			scheduleItem.setAction(ScheduleData::defaultActionID);
		}
	}

	// Check repeat data
	if ((scheduleItem.isRepeatEnabled() == true) && (scheduleItem.getActiveDays() == NULL)) {
		messageStringId = MSGBOX_MULTISCHEDULE_INVALIDITEM_ACTIVEDAYS;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Set default data
			scheduleItem.setActiveDays(PwrRepeatSet::defaultActiveDays);
		}
	}

	// Show error message if enabled
	if ((bShowMsg == true) && (!arrMsgString.empty())) {
		for (int index = 0; index < arrMsgString.size(); index++) {
			// If auto correction is ON
			if (bAutoCorrect == true) {
				// Add "Data will be automatically reset to default"
				String errorMessage = arrMsgString.at(index);
				errorMessage += getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_INVALIDITEM_AUTOCORRECT);
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
 * @brief	Handle clicking event for [Save] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnApply()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_APPLY_BTN);

	// Save data if changed
	bool isChanged = checkDataChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);
	if (isChanged == true) {
		SaveScheduleSettings();
	}

	// Close dialog
	SDialog::OnOK();
}

/**
 * @brief	Handle clicking event for [Exit] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnExit()
{
	// If not forced closing by request
	if (!isForceClosingByRequest()) {

		// Save app event log if enabled
		outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_CANCEL_BTN);

		// If data changed, ask for saving before closing dialog
		bool isChanged = checkDataChangeState();
		setFlagValue(AppFlagID::dialogDataChanged, isChanged);
		if (isChanged == true) {
			// Setup messagebox language
			LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
			const wchar_t* messagePrompt = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
			const wchar_t* messageCaption = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int nConfirm = MessageBox(messagePrompt, messageCaption, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Save data
				SaveScheduleSettings();
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
void CMultiScheduleDlg::OnAdd()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_ADD_BTN);

	// Initialize new item template
	Item schTemp;
	schTemp.setItemId(m_schScheduleTemp.getNextId());
	schTemp.setAction(ScheduleData::defaultActionID);

	// Open edit schedule dialog
	if (m_pEditScheduleDlg == NULL) {
		// Initialize
		m_pEditScheduleDlg = new CEditScheduleDlg;
		m_pEditScheduleDlg->setParent(this);
		m_pEditScheduleDlg->SetScheduleItem(schTemp);
		m_pEditScheduleDlg->SetDispMode(Mode::Add);
		m_pEditScheduleDlg->DoModal();
	}
	else {
		// Update dialog
		m_pEditScheduleDlg->setParent(this);
		m_pEditScheduleDlg->SetScheduleItem(schTemp);
		m_pEditScheduleDlg->SetDispMode(Mode::Add);
		m_pEditScheduleDlg->ShowWindow(SW_SHOW);
	}
}

/**
 * @brief	Handle click event for [Edit] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnEdit()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_EDIT_BTN);

	// Check if any item is selected or not
	bool bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	if (bIsSelected == true) {

		// Get selected row index
		int nSelRowIndex = m_nCurSelIndex + fixedRowNum;

		// Get selected item
		Item scheduleItem;
		if (nSelRowIndex == defaultRowIndex) {
			// Get default item
			scheduleItem = m_schScheduleTemp.getDefaultItem();
		}
		else {
			// Get extra item
			int nExtraItemIndex = nSelRowIndex - extraStartRowIndex;
			scheduleItem = m_schScheduleTemp.getItemAt(nExtraItemIndex);
		}

		// If item is empty, do nothing
		if (scheduleItem.isEmpty())
			return;

		// Open edit schedule dialog
		if (m_pEditScheduleDlg == NULL) {
			// Initialize
			m_pEditScheduleDlg = new CEditScheduleDlg;
			m_pEditScheduleDlg->setParent(this);
			m_pEditScheduleDlg->SetScheduleItem(scheduleItem);
			m_pEditScheduleDlg->SetDispMode(Mode::Update);
			m_pEditScheduleDlg->DoModal();
		}
		else {
			// Update dialog
			m_pEditScheduleDlg->setParent(this);
			m_pEditScheduleDlg->SetScheduleItem(scheduleItem);
			m_pEditScheduleDlg->SetDispMode(Mode::Update);
			m_pEditScheduleDlg->ShowWindow(SW_SHOW);
		}
	}
}

/**
 * @brief	Handle click event for [Remove] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnRemove()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_REMOVE_BTN);

	// If there's no item, do nothing
	int itemNum = GetTotalItemNum();
	if (itemNum <= 0)
		return;

	// Get current select item index
	int nSelItemIndex = m_nCurSelIndex;
	int nSelRowIndex = nSelItemIndex + fixedRowNum;

	// If item at selected index is default item, can not remove
	if (nSelRowIndex == defaultRowIndex) {
		displayMessageBox(MSGBOX_MULTISCHEDULE_NOTREMOVE_DEFAULT, NULL, MB_OK | MB_ICONINFORMATION);
		return;
	}

	// In case of extra item rows
	else if (nSelRowIndex > defaultRowIndex) {
		// Get extra item index
		nSelItemIndex = nSelRowIndex - extraStartRowIndex;
	}

	// If item at selected index is empy, do nothing
	if (m_schScheduleTemp.isEmpty(nSelItemIndex) == true)
		return;

	// Ask before remove
	int nConfirm = displayMessageBox(MSGBOX_MULTISCHEDULE_REMOVE_ITEM, NULL, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {
		// Remove item
		Remove(nSelItemIndex);
	}
}

/**
 * @brief	Handle click event for [Remove All] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnRemoveAll()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_REMOVEALL_BTN);

	// If all item are empty, do nothing
	if (m_schScheduleTemp.isAllEmpty() == true)
		return;

	// Ask before remove
	int nConfirm = displayMessageBox(MSGBOX_MULTISCHEDULE_REMOVEALL_ITEMS, NULL, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {
		// Remove all items
		RemoveAll();
	}
}

/**
 * @brief	Handle click event for [Check All] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnCheckAll()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_CHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_schScheduleTemp.isAllEmpty() == true)
		return;

	// Check all items
	SetAllItemState(true);
}

/**
 * @brief	Handle click event for [Uncheck All] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnUncheckAll()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_UNCHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_schScheduleTemp.isAllEmpty() == true)
		return;

	// Uncheck all items
	SetAllItemState(false);
}

/**
 * @brief	Handle click event for [View Details] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnViewDetails()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_VIEWDETAILS_BTN);

	// Check if any item is selected or not
	bool bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	if (bIsSelected == true) {

		// Get selected row index
		int nSelRowIndex = m_nCurSelIndex + fixedRowNum;

		// Get selected item
		Item scheduleItem;
		if (nSelRowIndex == defaultRowIndex) {
			// Get default item
			scheduleItem = m_schScheduleTemp.getDefaultItem();
		}
		else {
			// Get extra item
			int nExtraItemIndex = nSelRowIndex - extraStartRowIndex;
			scheduleItem = m_schScheduleTemp.getItemAt(nExtraItemIndex);
		}

		// If item is empty, do nothing
		if (scheduleItem.isEmpty())
			return;

		// Open edit schedule dialog
		if (m_pEditScheduleDlg == NULL) {
			// Initialize
			m_pEditScheduleDlg = new CEditScheduleDlg;
			m_pEditScheduleDlg->setParent(this);
			m_pEditScheduleDlg->SetScheduleItem(scheduleItem);
			m_pEditScheduleDlg->SetDispMode(Mode::View);
			m_pEditScheduleDlg->DoModal();
		}
		else {
			// Update dialog
			m_pEditScheduleDlg->setParent(this);
			m_pEditScheduleDlg->SetScheduleItem(scheduleItem);
			m_pEditScheduleDlg->SetDispMode(Mode::View);
			m_pEditScheduleDlg->ShowWindow(SW_SHOW);
		}
	}
}

/**
 * @brief	Handle click event for [Set Default] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnSetDefault()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_SETDEFAULT_BTN);

	// Check if any item is selected or not
	bool bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));
	if (bIsSelected != true)
		return;

	// Check if selected item is an extra item or not
	int nSelRowIndex = m_nCurSelIndex + fixedRowNum;
	bool bIsExtraSelected = ((bIsSelected == true) && (nSelRowIndex >= extraStartRowIndex));

	if (bIsExtraSelected == true) {

		// Display confirmation message
		int nConfirm = displayMessageBox(MSGBOX_MULTISCHEDULE_CONFIRM_SETDEFAULT, NULL, MB_YESNO | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Check if currently selected item is empty
			int nExtraItemIndex = nSelRowIndex - extraStartRowIndex;
			const Item& schCurSelItem = m_schScheduleTemp.getItemAt(nExtraItemIndex);
			if (schCurSelItem.isEmpty())
				return;
			
			// Overwrite default item data with currently selected extra item data
			Item& defaultScheduleItem = m_schScheduleTemp.getDefaultItem();
			defaultScheduleItem.copy(schCurSelItem);
			defaultScheduleItem.setItemId(ScheduleData::defaultItemID);

			// Remove the selected extra item after changing
			m_schScheduleTemp.deleteItem(nExtraItemIndex);

			// Update table
			RedrawDataTable();

			// Refresh button state
			refreshDialogItemState(true);
		}
	}
}

/**
 * @brief	Show details when selecting a schedule item
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CMultiScheduleDlg::OnSelectScheduleItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nRow = pItem->iRow;

	//Get current selection index
	m_nCurSelIndex = nRow - fixedRowNum;
	int nItemCount = GetTotalItemNum();

	// Success (return 0)
	*pResult = NULL;

	// Invalid selection
	if ((m_nCurSelIndex < 0) || (m_nCurSelIndex >= nItemCount))
		return;

	// Refresh display
	refreshDialogItemState(true);
}

/**
 * @brief	Handle click event on Power Reminder data
					item list control
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CMultiScheduleDlg::OnClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nClickedRow = pItem->iRow;

	// Check value validity
	int itemNum = GetTotalItemNum();
	if ((nClickedRow <= Constant::UI::GridCtrl::Index::Header_Row) || (nClickedRow > itemNum)) {
		return;
	}

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
void CMultiScheduleDlg::OnRightClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nClickedRow = pItem->iRow;

	// Check value validity
	int itemNum = GetTotalItemNum();
	if ((nClickedRow <= Constant::UI::GridCtrl::Index::Header_Row) || (nClickedRow > itemNum)) {
		return;
	}

	// Delete this (not used)
	*pResult = NULL;

	// Refresh button states
	refreshDialogItemState(true);
}

/**
 * @brief	Handle event when a child dialog is destroyed
 * @param	wParam - Child dialog ID
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CMultiScheduleDlg::OnChildDialogDestroy(WPARAM wParam, LPARAM /*lParam*/)
{
	// Get dialog ID
	unsigned dialogId = static_cast<unsigned>(wParam);

	// Edit schedule dialog
	if (dialogId == IDD_EDITSCHEDULE_DLG) {

		// Initialize info data
		Item schItemTemp;
		int mode = Mode::Init;
		int returnFlag = ReturnFlag::Invalid;

		// Update info data
		if (m_pEditScheduleDlg != NULL) {
			m_pEditScheduleDlg->GetScheduleItem(&schItemTemp);
			mode = m_pEditScheduleDlg->GetDispMode();
			returnFlag = m_pEditScheduleDlg->getReturnFlag();
		}

		// Validate return info
		if ((!schItemTemp.isEmpty()) && (returnFlag == ReturnFlag::Update)) {
			// Update data by mode
			if (mode == Mode::Add) {
				// Add item
				Add(schItemTemp);
			}
			else if (mode == Mode::Update) {
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

	return LRESULT(Result::Success);	// ERROR_SUCCESS
}

/**
 * @brief	Pre-handle dialog messages
 * @param	message - Message ID
 * @param	wParam	- First param (HIWORD)
 * @param	lParam	- Second param (LOWORD)
 * @return	None
 */
LRESULT CMultiScheduleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// Default
	return SDialog::WindowProc(message, wParam, lParam);
}

