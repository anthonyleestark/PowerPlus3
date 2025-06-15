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
int CMultiScheduleDlg::RegisterDialogManagement(void)
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

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CMultiScheduleDlg::UnregisterDialogManagement(void)
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
	SetUseEnter(false);

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
	RefreshDialogItemState(true);

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, this->GetCaption());

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
	if (!IsForceClosingByRequest()) {

		// If data changed, ask for saving before closing dialog
		if (GetFlagValue(AppFlagID::dialogDataChanged) == true) {
			// Setup messagebox language
			LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
			const wchar_t* messagePrompt = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
			const wchar_t* messageCaption = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

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
LRESULT CMultiScheduleDlg::RequestCloseDialog(void)
{
	// If detail edit dialog is opening
	if (m_pEditScheduleDlg != NULL) {
		// Request close dialog
		LRESULT resCloseReq = m_pEditScheduleDlg->RequestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}

	// If data changed, ask for saving before closing dialog
	if (GetFlagValue(AppFlagID::dialogDataChanged) == true) {
		// Setup messagebox language
		LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		const wchar_t* messagePrompt = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
		const wchar_t* messageCaption = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

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
	return SDialog::RequestCloseDialog();
}

/**
 * @brief	Setup language for dialog items
 * @param	None
 * @return	None
 */
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
		unsigned nID = pWndChild->GetDlgCtrlID();

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

/**
 * @brief	Initialize and setup language for Schedule item list
 * @param	ptrLanguage - Language package pointer
 * @return	None
 */
void CMultiScheduleDlg::SetupDataItemList(LANGTABLE_PTR /*ptrLanguage*/)
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
	VERIFY_INITIALIZATION(m_pDataItemListTable, CGridCtrl)

	// Create table
	if (m_pDataItemListTable == NULL) return;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	m_pDataItemListTable->Create(rcListFrameWnd, this, IDC_MULTISCHEDULE_ITEM_LISTBOX, dwStyle);

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
	DrawDataTable(GetReadOnlyMode());

	// Update layout info
	UpdateLayoutInfo();

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
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;

	// Get app language package
	LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();

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
	int nFrameHeight = m_pszDataTableFrameSize->cy;
	int nFrameWidth = m_pszDataTableFrameSize->cx;
	if (GetWindowsOSVersion() == WINDOWS_VERSION_10) {
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
		SetFixedCellStyle(m_pDataItemListTable, Constant::UI::GridCtrl::Index::Header_Row, nCol);

		// Column header title
		String headerTitle = Constant::String::Empty;
		unsigned nHeaderTitleID = m_apGrdColFormat[nCol].nHeaderTitleID;
		if (nHeaderTitleID != INT_NULL) {
			headerTitle = GetLanguageString(ptrLanguage, nHeaderTitleID);
		}
		m_pDataItemListTable->SetItemText(Constant::UI::GridCtrl::Index::Header_Row, nCol, headerTitle);

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
	unsigned nItemState = INT_NULL;
	for (int nRow = 1; nRow < nRowNum; nRow++) {
		for (int nCol = 0; nCol < m_nColNum; nCol++) {

			// Get column style & item state
			nColStyle = m_apGrdColFormat[nCol].nColStyle;
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
				if (m_apGrdColFormat[nCol].bCenter == true) {
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
				if (m_apGrdColFormat[nCol].bCenter == true) {
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

/**
 * @brief	Load layout info data
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::LoadLayoutInfo(void)
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
		if (GetLayoutInfo(Section::LayoutInfo::MultiScheduleTable, keyName, nRet)) {
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
void CMultiScheduleDlg::SaveLayoutInfo(void)
{
	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Save layout info data to registry
	int nRef = 0;
	String keyName;
	for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
		nRef = m_apGrdColFormat[nIndex].nWidth;
		keyName = Key::LayoutInfo::GridColSize(nIndex);
		WriteLayoutInfo(Section::LayoutInfo::MultiScheduleTable, keyName, nRef);
	}
}

/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::SetupDialogItemState()
{
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
 * @brief	Update Schedule data item list table
 * @param	None
 * @return	None
 */
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
	int nTemp = -1;
	int nExtraItemIndex = 0;
	CGridCellCheck* pCellCheck = NULL;
	for (int nRowIndex = defaultRowIndex; nRowIndex <= nItemNum; nRowIndex++) {
		
		// Get schedule item
		Item schItem;
		if (nRowIndex == defaultRowIndex) {
			// Get schedule default item
			schItem = m_schScheduleTemp.GetDefaultItem();
		}
		else {
			// Get schedule extra item
			nExtraItemIndex = nRowIndex - extraStartRowIndex;
			schItem = m_schScheduleTemp.GetItemAt(nExtraItemIndex);
		}

		// If schedule item is empty
		if (schItem.IsEmpty()) continue;

		// Item index
		String tempString = StringUtils::StringFormat(_T("%d"), nRowIndex);
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::Index, tempString);

		// Enable state
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::EnableState);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(schItem.IsEnabled());
		}

		// Action name
		nTemp = GetPairedID(IDTable::ActionName, schItem.GetAction());
		tempString = GetLanguageString(ptrLanguage, nTemp);
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::ActionID, tempString);

		// Time setting
		const wchar_t* formatString = GetLanguageString(ptrLanguage, GRIDCOLUMN_MULTISCHEDULE_TIMEFORMAT);
		tempString = ClockTimeUtils::Format(ptrLanguage, formatString, schItem.GetTime());
		m_pDataItemListTable->SetItemText(nRowIndex, ColumnID::TimeValue, tempString);

		// Repeat
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, ColumnID::Repeat);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(schItem.IsRepeatEnabled());
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
void CMultiScheduleDlg::RefreshDialogItemState(bool bRecheckState /* = false */)
{
	CWnd* pBtn = NULL;

	// If dialog items are being locked, do nothing
	if (GetLockState() == true)
		return;

	// Check if any item is selected or not
	bool bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	// Check if selected item is an extra item or not
	bool bIsExtraSelected = ((bIsSelected == true) && ((m_nCurSelIndex + fixedRowNum) >= extraStartRowIndex));

	// Check if number of extra item has reached the limit
	bool bIsMaxNum = (GetExtraItemNum() >= ScheduleData::maxItemNum);

	// Check if data is all empty or not
	bool bIsAllEmpty = m_schScheduleTemp.IsAllEmpty();

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
	bool bIsChanged = CheckDataChangeState();
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

/**
 * @brief	Refresh and update state for [Check/Uncheck All] button
 * @param	bRecheck - Recheck all items enable state
 * @return	None
 */
void CMultiScheduleDlg::UpdateCheckAllBtnState(bool bRecheck /* = false */)
{
	// If dialog items are being locked, do nothing
	if (GetLockState() == true)
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
		pCheckAllBtn->EnableWindow(false);
		pUncheckAllBtn->EnableWindow(false);
		return;
	}

	// Recheck all items state
	if (bRecheck == true) {

		// Reset counter
		m_nCheckCount = 0;
		// Check default item
		if (m_schScheduleTemp.GetDefaultItem().IsEnabled() == true) {
			// Increase counter
			m_nCheckCount++;
		}
		// Check extra items
		for (int nExtraIndex = 0; nExtraIndex < GetExtraItemNum(); nExtraIndex++) {
			const Item& schTemp = m_schScheduleTemp.GetItemAt(nExtraIndex);
			if (schTemp.IsEnabled() == true) {
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
	else if (m_nCheckCount == nItemNum) {
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
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp == NULL) return false;
	Data* pschData = pApp->GetAppScheduleData();
	if (pschData == NULL)
		return false;

	// Copy data
	m_schSchedule.Copy(*pschData);
	m_schScheduleTemp.Copy(m_schSchedule);

	// Reset change flag
	SetFlagValue(AppFlagID::dialogDataChanged, false);

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
	m_schSchedule.Copy(m_schScheduleTemp);

	// Save app schedule data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp == NULL) return false;
	pApp->SetAppScheduleData(&m_schSchedule);
	pApp->SaveRegistryAppData(APPDATA_SCHEDULE);

	// Notify main dialog to re-update schedule data
	CPowerPlusDlg* pMainDlg = (CPowerPlusDlg*)(pApp->GetMainWnd());
	VERIFY(pMainDlg != NULL);
	if (pMainDlg == NULL) return false;
	pMainDlg->PostMessage(SM_APP_UPDATE_SCHEDULEDATA, NULL, NULL);

	// Reset change flag
	SetFlagValue(AppFlagID::dialogDataChanged, false);

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
		bool bEnabled = pCellCheckEnable->GetCheck();
		bool bRepeat = pCellCheckRepeat->GetCheck();

		// Update item enable and repeat states
		if (nRowIndex == defaultRowIndex) {
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

/**
 * @brief	Compare control values with current config to check for changes
 * @param	None
 * @return	bool - Flag to mark data changed or not
 */
bool CMultiScheduleDlg::CheckDataChangeState()
{
	bool bChangeFlag = false;

	// Update settings
	UpdateScheduleSettings();

	// Check if number of extra items changed
	int nExtraItemNum = m_schScheduleTemp.GetExtraItemNum();
	bChangeFlag |= (nExtraItemNum != m_schSchedule.GetExtraItemNum());
	if (bChangeFlag == true)
		return bChangeFlag;

	// Check if default item's data changed
	bChangeFlag |= (m_schScheduleTemp.GetDefaultItem().Compare(m_schSchedule.GetDefaultItem()) != true);
	if (bChangeFlag == true)
		return bChangeFlag;

	// Check if each extra item's data changed
	for (int nExtraIndex = 0; nExtraIndex < nExtraItemNum; nExtraIndex++) {
		// Get current item and temp item
		const Item& schCurItem = m_schSchedule.GetItemAt(nExtraIndex);
		const Item& schTempItem = m_schScheduleTemp.GetItemAt(nExtraIndex);

		// Data comparison
		bChangeFlag |= (schTempItem.Compare(schCurItem) != true);

		// Stop on the first different item encountered
		if (bChangeFlag == true) break;
	}

	return bChangeFlag;
}

/**
 * @brief	Add item into schedule data
 * @param	schItem - Schedule item
 * @return	None
 */
void CMultiScheduleDlg::Add(Item& schItem)
{
	// If item is empty, do nothing
	if (schItem.IsEmpty())
		return;

	// Check data validity
	bool bValid = Validate(schItem, true);
	if (bValid == false)
		return;

	// Update item to Action Schedule data item list
	m_schScheduleTemp.Update(schItem);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(true);
}

/**
 * @brief	Update schedule item
 * @param	schItem - Schedule item
 * @return	None
 */
void CMultiScheduleDlg::Update(Item& schItem)
{
	// If item is empty, do nothing
	if (schItem.IsEmpty())
		return;

	// Check data validity
	bool bValid = Validate(schItem, true);
	if (bValid == false)
		return;

	// Update item to Action Schedule data item list
	m_schScheduleTemp.Update(schItem);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(true);
}

/**
 * @brief	Remove a schedule item by index
 * @param	nIndex - Index of item to remove
 * @return	None
 */
void CMultiScheduleDlg::Remove(int nIndex)
{
	// Remove item at index
	m_schScheduleTemp.Delete(nIndex);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(true);
}

/**
 * @brief	Remove all schedule items
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::RemoveAll()
{
	// Remove all items
	m_schScheduleTemp.DeleteExtra();

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDialogItemState(true);
}

/**
 * @brief	Check/uncheck all Schedule data items
 * @param	bState - Item state
 * @return	None
 */
void CMultiScheduleDlg::SetAllItemState(bool bState)
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
	m_nCheckCount = (bState == false) ? 0 : GetTotalItemNum();

	// Update data item list
	UpdateDataItemList();

	// Refresh button state
	RefreshDialogItemState(false);
}

/**
 * @brief	Check Schedule item validity
 * @param	schItem		 - Item to validate
 * @param	bShowMsg	 - Show validation message box or not
 * @param	bAutoCorrect - Invalid value auto correction (ON/OFF)
 * @return	bool - Result of validation process
 */
bool CMultiScheduleDlg::Validate(Item& schItem, bool bShowMsg /* = false */, bool bAutoCorrect /* = false */)
{
	bool bResult = true;

	int nMsgStringID;
	StringArray arrMsgString;
	arrMsgString.clear();

	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Check item ID
	if ((schItem.GetItemID() != ScheduleData::defaultItemID) &&
		((schItem.GetItemID() < ScheduleData::minItemID) || (schItem.GetItemID() > ScheduleData::maxItemID))) {
		nMsgStringID = MSGBOX_MULTISCHEDULE_INVALIDITEM_ITEMID;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Get next item ID
			schItem.SetItemID(m_schScheduleTemp.GetNextID());
		}
	}

	// Check action ID
	if ((schItem.GetAction() < APP_ACTION_NOTHING) || (schItem.GetAction() > APP_ACTION_HIBERNATE)) {
		nMsgStringID = MSGBOX_MULTISCHEDULE_INVALIDITEM_ACTIONID;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Set default action
			schItem.SetAction(ScheduleData::defaultActionID);
		}
	}

	// Check repeat data
	if ((schItem.IsRepeatEnabled() == true) && (schItem.GetActiveDays() == NULL)) {
		nMsgStringID = MSGBOX_MULTISCHEDULE_INVALIDITEM_ACTIVEDAYS;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = false;

		// Auto correction
		if (bAutoCorrect == true) {
			// Set default data
			schItem.SetActiveDays(PwrRepeatSet::defaultActiveDays);
		}
	}

	// Show error message if enabled
	if ((bShowMsg == true) && (!arrMsgString.empty())) {
		for (int nIndex = 0; nIndex < arrMsgString.size(); nIndex++) {
			// If auto correction is ON
			if (bAutoCorrect == true) {
				// Add "Data will be automatically reset to default"
				String errorMessage = arrMsgString.at(nIndex);
				errorMessage += GetLanguageString(pLang, MSGBOX_MULTISCHEDULE_INVALIDITEM_AUTOCORRECT);
				DisplayMessageBox(errorMessage, NULL, MB_OK | MB_ICONERROR);
			}
			else {
				// Display error message
				DisplayMessageBox(arrMsgString.at(nIndex), NULL, MB_OK | MB_ICONERROR);
			}
		}
	}

	// Remove all message after displaying
	arrMsgString.clear();

	return bResult;
}

/**
 * @brief	Handle clicking event for [Save] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnApply()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_APPLY_BTN);

	// Save data if changed
	bool bIsChanged = CheckDataChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
	if (bIsChanged == true) {
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
	if (!IsForceClosingByRequest()) {

		// Save app event log if enabled
		OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_CANCEL_BTN);

		// If data changed, ask for saving before closing dialog
		bool bIsChanged = CheckDataChangeState();
		SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
		if (bIsChanged == true) {
			// Setup messagebox language
			LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
			const wchar_t* messagePrompt = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
			const wchar_t* messageCaption = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_ADD_BTN);

	// Initialize new item template
	Item schTemp;
	schTemp.SetItemID(m_schScheduleTemp.GetNextID());
	schTemp.SetAction(ScheduleData::defaultActionID);

	// Open edit schedule dialog
	if (m_pEditScheduleDlg == NULL) {
		// Initialize
		m_pEditScheduleDlg = new CEditScheduleDlg;
		m_pEditScheduleDlg->SetParentWnd(this);
		m_pEditScheduleDlg->SetScheduleItem(schTemp);
		m_pEditScheduleDlg->SetDispMode(Mode::Add);
		m_pEditScheduleDlg->DoModal();
	}
	else {
		// Update dialog
		m_pEditScheduleDlg->SetParentWnd(this);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_EDIT_BTN);

	// Check if any item is selected or not
	bool bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	if (bIsSelected == true) {

		// Get selected row index
		int nSelRowIndex = m_nCurSelIndex + fixedRowNum;

		// Get selected item
		Item schItem;
		if (nSelRowIndex == defaultRowIndex) {
			// Get default item
			schItem = m_schScheduleTemp.GetDefaultItem();
		}
		else {
			// Get extra item
			int nExtraItemIndex = nSelRowIndex - extraStartRowIndex;
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
			m_pEditScheduleDlg->SetDispMode(Mode::Update);
			m_pEditScheduleDlg->DoModal();
		}
		else {
			// Update dialog
			m_pEditScheduleDlg->SetParentWnd(this);
			m_pEditScheduleDlg->SetScheduleItem(schItem);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_REMOVE_BTN);

	// If there's no item, do nothing
	int nItemNum = GetTotalItemNum();
	if (nItemNum <= 0)
		return;

	// Get current select item index
	int nSelItemIndex = m_nCurSelIndex;
	int nSelRowIndex = nSelItemIndex + fixedRowNum;

	// If item at selected index is default item, can not remove
	if (nSelRowIndex == defaultRowIndex) {
		DisplayMessageBox(MSGBOX_MULTISCHEDULE_NOTREMOVE_DEFAULT, NULL, MB_OK | MB_ICONINFORMATION);
		return;
	}

	// In case of extra item rows
	else if (nSelRowIndex > defaultRowIndex) {
		// Get extra item index
		nSelItemIndex = nSelRowIndex - extraStartRowIndex;
	}

	// If item at selected index is empy, do nothing
	if (m_schScheduleTemp.IsEmpty(nSelItemIndex) == true)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_MULTISCHEDULE_REMOVE_ITEM, NULL, MB_YESNO | MB_ICONQUESTION);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_REMOVEALL_BTN);

	// If all item are empty, do nothing
	if (m_schScheduleTemp.IsAllEmpty() == true)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_MULTISCHEDULE_REMOVEALL_ITEMS, NULL, MB_YESNO | MB_ICONQUESTION);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_CHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_schScheduleTemp.IsAllEmpty() == true)
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_UNCHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_schScheduleTemp.IsAllEmpty() == true)
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_VIEWDETAILS_BTN);

	// Check if any item is selected or not
	bool bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));

	if (bIsSelected == true) {

		// Get selected row index
		int nSelRowIndex = m_nCurSelIndex + fixedRowNum;

		// Get selected item
		Item schItem;
		if (nSelRowIndex == defaultRowIndex) {
			// Get default item
			schItem = m_schScheduleTemp.GetDefaultItem();
		}
		else {
			// Get extra item
			int nExtraItemIndex = nSelRowIndex - extraStartRowIndex;
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
			m_pEditScheduleDlg->SetDispMode(Mode::View);
			m_pEditScheduleDlg->DoModal();
		}
		else {
			// Update dialog
			m_pEditScheduleDlg->SetParentWnd(this);
			m_pEditScheduleDlg->SetScheduleItem(schItem);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_SETDEFAULT_BTN);

	// Check if any item is selected or not
	bool bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetTotalItemNum()));
	if (bIsSelected != true)
		return;

	// Check if selected item is an extra item or not
	int nSelRowIndex = m_nCurSelIndex + fixedRowNum;
	bool bIsExtraSelected = ((bIsSelected == true) && (nSelRowIndex >= extraStartRowIndex));

	if (bIsExtraSelected == true) {

		// Display confirmation message
		int nConfirm = DisplayMessageBox(MSGBOX_MULTISCHEDULE_CONFIRM_SETDEFAULT, NULL, MB_YESNO | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Check if currently selected item is empty
			int nExtraItemIndex = nSelRowIndex - extraStartRowIndex;
			const Item& schCurSelItem = m_schScheduleTemp.GetItemAt(nExtraItemIndex);
			if (schCurSelItem.IsEmpty())
				return;
			
			// Overwrite default item data with currently selected extra item data
			Item& schDefaultItem = m_schScheduleTemp.GetDefaultItem();
			schDefaultItem.Copy(schCurSelItem);
			schDefaultItem.SetItemID(ScheduleData::defaultItemID);

			// Remove the selected extra item after changing
			m_schScheduleTemp.Delete(nExtraItemIndex);

			// Update table
			RedrawDataTable();

			// Refresh button state
			RefreshDialogItemState(true);
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
	RefreshDialogItemState(true);
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
	int nItemNum = GetTotalItemNum();
	if ((nClickedRow <= Constant::UI::GridCtrl::Index::Header_Row) || (nClickedRow > nItemNum)) {
		return;
	}

	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	RefreshDialogItemState(true);
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
	int nItemNum = GetTotalItemNum();
	if ((nClickedRow <= Constant::UI::GridCtrl::Index::Header_Row) || (nClickedRow > nItemNum)) {
		return;
	}

	// Delete this (not used)
	*pResult = NULL;

	// Refresh button states
	RefreshDialogItemState(true);
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
	unsigned nDialogID = static_cast<unsigned>(wParam);

	// Edit schedule dialog
	if (nDialogID == IDD_EDITSCHEDULE_DLG) {

		// Initialize info data
		Item schItemTemp;
		int nMode = Mode::Init;
		int nRetFlag = ReturnFlag::Invalid;

		// Update info data
		if (m_pEditScheduleDlg != NULL) {
			m_pEditScheduleDlg->GetScheduleItem(&schItemTemp);
			nMode = m_pEditScheduleDlg->GetDispMode();
			nRetFlag = m_pEditScheduleDlg->GetReturnFlag();
		}

		// Validate return info
		if ((!schItemTemp.IsEmpty()) && (nRetFlag == ReturnFlag::Update)) {
			// Update data by mode
			if (nMode == Mode::Add) {
				// Add item
				Add(schItemTemp);
			}
			else if (nMode == Mode::Update) {
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

