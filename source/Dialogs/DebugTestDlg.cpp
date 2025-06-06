
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		DebugTestDlg.cpp
//		Description:	Source file for DebugTest dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2024.07.13:		Create new
//						<1> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Dialogs/DebugTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace AppCore;


//////////////////////////////////////////////////////////////////////////
//
// Define macros for DebugTest functions
//
//////////////////////////////////////////////////////////////////////////

// Default min/max size
#define DEFAULT_MIN_WIDTH		840
#define DEFAULT_MIN_HEIGHT		500
#define DEFAULT_MAX_WIDTH		1600
#define DEFAULT_MAX_HEIGHT		900

#define DEBUG_OUTPUT_FORMAT		_T(">> %s")


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CDebugTestDlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDebugTestDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CDebugTestDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CDebugTestDlg::CDebugTestDlg() : SDialog(IDD_DEBUGTEST_DLG)
{
	// Edit view
	m_pDebugEditView = NULL;
	m_pDebugViewFont = NULL;
	m_pDebugViewBrush = NULL;

	// Buffer content
	m_strBuffer = Constant::String::Empty;
	m_strBufferBak = Constant::String::Empty;

	// Debug command history
	m_bCurDispHistory = FALSE;
	m_nHistoryCurIndex = 0;
	m_astrCommandHistory.clear();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CDebugTestDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CDebugTestDlg::~CDebugTestDlg()
{
	// Clean-up debug command history
	ClearDebugCommandHistory();

	// Delete font
	if (m_pDebugViewFont != NULL) {
		m_pDebugViewFont->DeleteObject();
		delete m_pDebugViewFont;
		m_pDebugViewFont = NULL;
	}

	// Delete brush
	if (m_pDebugViewBrush != NULL) {
		m_pDebugViewBrush->DeleteObject();
		delete m_pDebugViewBrush;
		m_pDebugViewBrush = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


//////////////////////////////////////////////////////////////////////////
//
//	CDebugTestDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDebugTestDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_DEBUGTEST_EDITVIEW, &CDebugTestDlg::OnDebugViewEditChange)
	ON_MESSAGE(SM_APP_DEBUG_OUTPUT,		 &CDebugTestDlg::OnDebugOutput)
	ON_MESSAGE(SM_APP_DEBUGCMD_NOREPLY,	 &CDebugTestDlg::OnDebugCmdNoReply)
	ON_MESSAGE(SM_WND_DEBUGVIEW_CLRSCR,	 &CDebugTestDlg::OnDebugViewClear)
	ON_MESSAGE(SM_WND_SHOWDIALOG,		 &CDebugTestDlg::OnShowDialog)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// CDebugTestDlg message handlers

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize DebugTest dialog
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Prevent flickering on startup
	this->ShowWindow(SW_HIDE);

	// Set dialog title
	this->SetCaptionFromResource(IDS_APP_DEBUGTESTDLG_TITLE);

	// Get DebugTest edit view
	BOOL bRet = InitDebugEditView(IDC_DEBUGTEST_EDITVIEW);
	if (bRet == FALSE) {
		TRACE_ERROR("Error: Debug edit view initialization failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return bRet;
	}

	RECT rcClient;
	this->GetClientRect(&rcClient);

	// Set edit view position
	int nXPos = rcClient.left, nYPos = rcClient.top;
	int nWidth = rcClient.right - rcClient.left;
	int nHeight = rcClient.bottom - rcClient.top;
	GetDebugEditView()->SetWindowPos(NULL, nXPos, nYPos, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOZORDER);

	// Bring window to top
	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	this->SetForegroundWindow();

	// Clear buffer
	ClearViewBuffer();

	// Init debug command history
	ClearDebugCommandHistory();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClose
//	Description:	Close dialog
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::OnClose()
{
	// Only hide the dialog
	ShowWindow(SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDestroy
//	Description:	Destroy dialog
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::OnDestroy()
{
	// Destroy dialog
	SDialog::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnGetMinMaxInfo
//	Description:	Dialog get min/max info handler
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo)
{
	// Fix min/max size
	pMinMaxInfo->ptMinTrackSize = CPoint(DEFAULT_MIN_WIDTH, DEFAULT_MIN_HEIGHT);
	pMinMaxInfo->ptMaxTrackSize = CPoint(DEFAULT_MAX_WIDTH, DEFAULT_MAX_HEIGHT);

	// Default
	SDialog::OnGetMinMaxInfo(pMinMaxInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCtlColor
//	Description:	Set background and text color
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

HBRUSH CDebugTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hBrush = SDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_EDIT)
	{
		// Text color: Yellow
		pDC->SetTextColor(Color::Yellow);

		// Background color: Black
		pDC->SetBkColor(Color::Black);

		// Use our custom background brush
		if (m_pDebugViewBrush != NULL)
			return *m_pDebugViewBrush;
	}

	return hBrush;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSize
//	Description:	Resize dialog
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::OnSize(UINT nType, int nWidth, int nHeight)
{
	// Implement base class method
	SDialog::OnSize(nType, nWidth, nHeight);

	// Get DebugTest edit view
	if (!IsDebugEditViewValid()) {
		BOOL bRet = InitDebugEditView(IDC_DEBUGTEST_EDITVIEW);
		if (bRet == FALSE)
			return;
	}

	RECT rcClient;
	this->GetClientRect(&rcClient);

	// Set edit view position/size
	int nXPos = rcClient.left, nYPos = rcClient.top;
	nWidth = rcClient.right - rcClient.left;
	nHeight = rcClient.bottom - rcClient.top;
	GetDebugEditView()->SetWindowPos(NULL, nXPos, nYPos, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDebugViewEditChange
//	Description:	Handle event when debug view edit contents changed
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::OnDebugViewEditChange(void)
{
	// Check debug view pointer validity
	if (!IsDebugEditViewValid())
		return;

	// Update buffer content
	GetDebugEditView()->GetWindowText(m_strBuffer);

	// If buffer length did not increase
	int nBufferLength = m_strBuffer.GetLength();
	int nBufferBakLength = m_strBufferBak.GetLength();
	if (nBufferLength <= nBufferBakLength) {
		// Backup buffer
		BackupDebugViewBuffer();
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDebugOutput
//	Description:	Handle event when output debug contents
//  Arguments:		wParam - First param
//					lParam - Second param
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CDebugTestDlg::OnDebugOutput(WPARAM wParam, LPARAM lParam)
{
	// Check argument validity
	if ((wParam == 0) || (lParam == NULL))
		return LRESULT(Result::Failure);

	// Format debug output log string
	CString strDebugOutputLog;
	strDebugOutputLog.Format(DEBUG_OUTPUT_FORMAT, LPARAM_TO_STRING(lParam));

	// Add debug output string
	AddLine(strDebugOutputLog);

	// Display log and move cursor to end
	UpdateDisplay(TRUE);

	// Backup buffer
	BackupDebugViewBuffer();

	// Reset currently displaying history flag
	SetCurrentlyDispHistoryState(FALSE);

	return LRESULT(Result::Success);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDebugCmdNoReply
//	Description:	Handle event when a debug command has been processed 
//					but there's no reply
//  Arguments:		wParam - First param
//					lParam - Second param
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CDebugTestDlg::OnDebugCmdNoReply(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Add an empty new line
	AddLine(Constant::String::Empty, FALSE);

	// Display log and move cursor to end
	UpdateDisplay(TRUE, FALSE);

	// Backup buffer
	BackupDebugViewBuffer();

	// Reset currently displaying history flag
	SetCurrentlyDispHistoryState(FALSE);

	return LRESULT(Result::Success);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDebugViewClear
//	Description:	Handle debug view clear screen event
//  Arguments:		wParam - First param
//					lParam - Second param
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CDebugTestDlg::OnDebugViewClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Clear buffer
	ClearViewBuffer();

	return LRESULT(Result::Success);
}


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnShowDialog
//	Description:	Show/hide dialog when receiving message
//  Arguments:		wParam - Show/hide flag
//					lParam - Not used
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CDebugTestDlg::OnShowDialog(WPARAM wParam, LPARAM /*lParam*/)
{
	// Get flag value
	BOOL bShowFlag = TRUE;
	if (wParam != NULL) {
		bShowFlag = (BOOL)wParam;
	}

	// Show/hide dialog
	if (bShowFlag == TRUE) {

		// Show dialog
		this->ShowWindow(SW_SHOW);

		// Bring window to top
		this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		this->SetForegroundWindow();
		this->BringWindowToTop();
	}
	else {
		// Hide dialog
		this->ShowWindow(SW_HIDE);
	}

	// Default: Always success
	return LRESULT(Result::Success);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCommand
//	Description:	Handle app command messages (WM_COMMAND)
//	Arguments:		wParam - First param (HIWORD)
//					lParam - Second param (LOWORD)
//  Return value:	BOOL
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Process commands
	switch (LOWORD(wParam))
	{
	case IDM_DEBUGTEST_COPY:
		// Copy selection text to clipboard
		GetDebugEditView()->Copy();
		break;

	case IDM_DEBUGTEST_PASTE:
		// Check if focus belongs to DebugTest edit view
		if (IsDebugEditViewFocus() == TRUE) {
			// Get caret position
			int nCaretPos = GetCaretPosition();
			// Get line index by caret position
			int nCaretLineIdx = GetDebugEditView()->LineFromChar(nCaretPos);
			// If the caret position is not in the last line
			if (nCaretLineIdx != (GetDebugEditView()->GetLineCount() - 1)) {
				// Move caret to end of DebugTest edit view
				GetDebugEditView()->SetSel(static_cast<DWORD>(-1));
			}
			// Paste clipboard text to DebugTest edit view
			GetDebugEditView()->Paste();
		} break;

	case IDM_DEBUGTEST_DISP_PREVCOMMAND:
		// Display previous command
		DispDebugCommandHistory(GetHistoryCurrentDispIndex() - 1);
		break;

	case IDM_DEBUGTEST_DISP_NEXTCOMMAND:
		// Display next command
		DispDebugCommandHistory(GetHistoryCurrentDispIndex() + 1);
		break;

	case IDM_DEBUGTEST_CLEAR_BUFFER:
		// Clear view buffer
		ClearViewBuffer();
		break;

	case IDM_DEBUGTEST_CLOSE:
		// Close DebugTest
		EndDialog(IDCANCEL);
		break;

	default:
		break;
	}

	// Default
	return SDialog::OnCommand(wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PreTranslateMessage
//	Description:	Pre-translate message
//  Arguments:		pMsg - Default
//  Return value:	BOOL
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// Handle key pressed event for DebugTest edit view
	if (pMsg->message == WM_KEYDOWN) {

		// Check if focus belongs to DebugTest edit view
		if (IsDebugEditViewFocus() == TRUE) {

			// Get pressed key
			DWORD dwKey = pMsg->wParam;

			// Get caret position
			int nCaretPos = GetCaretPosition();

			// Get line index by caret position
			int nCaretLineIdx = GetDebugEditView()->LineFromChar(nCaretPos);

			// If the caret position is not in the last line
			if (nCaretLineIdx != (GetDebugEditView()->GetLineCount() - 1)) {

				// If "Ctrl+C" keys are pressed
				if ((dwKey == 0x43) && (IS_PRESSED(VK_CONTROL))) {
					// If currently selecting a text
					int nStartSel, nEndSel;
					GetDebugEditView()->GetSel(nStartSel, nEndSel);

					if (nStartSel != nEndSel) {
						// Copy the current selection
						GetDebugEditView()->Copy();
						return TRUE;
					}
				}
				else {
					// Block --> Do nothing
					return TRUE;
				}
			}

			// If [Enter] key is pressed
			if (dwKey == VK_RETURN) {

				//Send debug command
				BOOL bRet = SendDebugCommand();

				// If debug command is sent, block the break-line
				if (bRet == TRUE) return bRet;
			}
			// If [Backspace] or [Delete] keys are pressed
			else if ((dwKey == VK_BACK) || (dwKey == VK_DELETE)) {

				// Only allow erasing inputted content
				if (m_strBuffer.Compare(m_strBufferBak) == 0)
					return TRUE;

				// [Backspace] key --> Can not delete empty line
				if (dwKey == VK_BACK) {

					// Get line begin character index
					size_t nLineBeginIndex = GetDebugEditView()->LineIndex(nCaretLineIdx);

					// Get last line index
					size_t nLastLineIndex = GetDebugEditView()->GetLineCount() - 1;

					// If the caret position is not in the last line
					// or is in the beginning of last line
					if ((nCaretLineIdx != nLastLineIndex) ||
						((nCaretPos == nLineBeginIndex) && (nCaretLineIdx == nLastLineIndex))) {
						// Block --> Do nothing
						return TRUE;
					}
				}
			}
			// If "Ctrl+A" keys are pressed
			else if ((dwKey == 0x41) && (IS_PRESSED(VK_CONTROL))) {
				// Block --> Do nothing
				return TRUE;
			}
			// If the [Up/Down] arrow keys are pressed
			else if ((dwKey == VK_UP) || (dwKey == VK_DOWN)) {

				// If debug command history is empty
				if (IsDebugCommandHistoryEmpty())
					return TRUE;

				// Get command history display index
				int nHistoryDispIndex = 0;

				// [Up] arrow key --> Display previous command
				if (dwKey == VK_UP) {

					// Get index
					nHistoryDispIndex = GetHistoryCurrentDispIndex() - 1;

					// If current index is 0
					if (nHistoryDispIndex < 0)
						return TRUE;
				}
				// [Down] arrow key --> Display next command
				else if (dwKey == VK_DOWN) {

					// Get index
					nHistoryDispIndex = GetHistoryCurrentDispIndex() + 1;

					// If current index exceeded limit
					if (nHistoryDispIndex >= GetDebugCommandHistoryCount())
						return TRUE;
				}

				// Display debug command
				DispDebugCommandHistory(nHistoryDispIndex);
				return TRUE;
			}
			// If the [Left/Right] arrow key is pressed
			else if (dwKey == VK_LEFT) {

				// Get line begin character index
				size_t nLineBeginIndex = GetDebugEditView()->LineIndex(nCaretLineIdx);

				// Get last line index
				size_t nLastLineIndex = GetDebugEditView()->GetLineCount() - 1;

				// If the caret position is not in the last line
				// or is in the beginning of last line
				if ((nCaretLineIdx != nLastLineIndex) ||
					((nCaretPos == nLineBeginIndex) && (nCaretLineIdx == nLastLineIndex))) {
					// Block --> Do nothing
					return TRUE;
				}
			}
		}
	}
	// Handle right mouse click for DebugTest edit view
	else if (pMsg->message == WM_RBUTTONDOWN ||
			 pMsg->message == WM_RBUTTONUP ||
			 pMsg->message == WM_RBUTTONDBLCLK) {

		// Get clicked point
		POINT pt;
		pt.x = GET_X_LPARAM(pMsg->lParam);
		pt.y = GET_Y_LPARAM(pMsg->lParam);

		// Get DebugTest edit view
		if (!IsDebugEditViewValid())
			return 0;

		// Get the editbox rect
		RECT rcDebugEditView;
		GetDebugEditView()->GetWindowRect(&rcDebugEditView);
		ScreenToClient(&rcDebugEditView);

		// If clicked point is inside the editbox area
		if (((pt.x > rcDebugEditView.left) && (pt.x < rcDebugEditView.right)) &&
			((pt.y > rcDebugEditView.top) && (pt.y < rcDebugEditView.bottom))) {
			// Show DebugTest edit view menu
			ShowDebugTestEditViewMenu();
			return TRUE;
		}
	}

	// Default
	return SDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SendDebugCommand
//	Description:	Send debug command to main parts of program
//  Arguments:		hRcvWnd - Receive window handle
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestDlg::SendDebugCommand(void)
{
	// Check DebugTest edit view validity
	if (!IsDebugEditViewValid())
		return FALSE;

	// Backup buffer
	BackupDebugViewBuffer();

	// Get the debug command line
	CString strDebugCommand;
	int nCurLine = (GetDebugEditView()->GetLineCount() - 1);
	int nLineIndex = GetDebugEditView()->LineIndex(nCurLine);
	int nLineLength = GetDebugEditView()->LineLength(nLineIndex);
	GetDebugEditView()->GetLine(nCurLine, strDebugCommand.GetBuffer(nLineLength), nLineLength);
	strDebugCommand.ReleaseBuffer(nLineLength);

	// Re-format the debug command
	int nCommandLength = FormatDebugCommand(strDebugCommand);

	// If debug command is empty, do not send
	if (nCommandLength <= 0)
		return FALSE;

	// Prepare params
	WPARAM wParam = MAKE_WPARAM_STRING(strDebugCommand);
	LPARAM lParam = MAKE_LPARAM_STRING(strDebugCommand.GetString());
	
	// Send debug command message to parent window
	this->NotifyParent(SM_APP_DEBUG_COMMAND, wParam, lParam);

	// Update debug command history
	AddDebugCommandHistory(strDebugCommand);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// Protected methods

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitDebugEditView
//	Description:	Initialize the DebugTest edit view pointer
//  Arguments:		nCtrlID - Dialog control ID
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestDlg::InitDebugEditView(UINT nCtrlID)
{
	// If it has already been initialized, do nothing
	if (IsDebugEditViewValid())
		return TRUE;

	// Initialize
	m_pDebugEditView = (CEdit*)GetDlgItem(nCtrlID);

	// Set DebugView font & background color
	if (IsDebugEditViewValid()) {
		if (CreateDebugViewFont()) {
			m_pDebugEditView->SetFont(m_pDebugViewFont);
		}
		if (!CreateDebugViewBrush())
			return FALSE;
	}

	return IsDebugEditViewValid();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CreateDebugViewFont
//	Description:	Initialize font for DebugTest edit view
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestDlg::CreateDebugViewFont(void)
{
	// Initialization
	if (m_pDebugViewFont == NULL) {
		m_pDebugViewFont = new CFont();
		if (m_pDebugViewFont == NULL) {
			// Trace error
			TRACE_ERROR("Error: Failed to create font!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return FALSE;
		}
	}

	// Setup font properties
	LOGFONT lf = STRUCT_ZERO;
	_tcscpy_s(lf.lfFaceName, _T("Cascadia Mono"));
	lf.lfHeight = -16;
	lf.lfWeight = FW_SEMIBOLD;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;

	// Create font
	BOOL bRet = FALSE;
	if (m_pDebugViewFont != NULL) {
		bRet = m_pDebugViewFont->CreateFontIndirect(&lf);
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CreateDebugViewBrush
//	Description:	Initialize brush for adjust DebugTest edit view color
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestDlg::CreateDebugViewBrush(void)
{
	// Initialization
	if (m_pDebugViewBrush == NULL) {
		m_pDebugViewBrush = new CBrush();
		if (m_pDebugViewBrush == NULL) {
			// Trace error
			TRACE_ERROR("Error: Failed to create brush!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return FALSE;
		}
	}

	// Create brush
	BOOL bRet = FALSE;
	if (m_pDebugViewBrush != NULL) {
		bRet = m_pDebugViewBrush->CreateSolidBrush(Color::Black);
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetCaretPosition
//	Description:	Get the caret's current position
//  Arguments:		None
//  Return value:	int - Caret position
//
//////////////////////////////////////////////////////////////////////////

int CDebugTestDlg::GetCaretPosition(void)
{
	// Check DebugTest edit view validity
	if (!IsDebugEditViewValid())
		return INT_INVALID;

	// Get caret position
	int nStartSel, nEndSel;
	GetDebugEditView()->GetSel(nStartSel, nEndSel);
	return nStartSel;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ShowDebugTestEditViewMenu
//	Description:	Show DebugTest edit view context menu
//  Arguments:		None
//	Return value:	BOOL - Show menu successfully or failed
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestDlg::ShowDebugTestEditViewMenu(void)
{
	// Prepare menu
	CMenu menuDebugTest, *pContextMenu;
	menuDebugTest.LoadMenu(IDR_MENU_DEBUGTEST_CONTEXT);
	pContextMenu = menuDebugTest.GetSubMenu(0);
	if (pContextMenu == NULL)
		return FALSE;

	// Check DebugTest edit view validity
	if (!IsDebugEditViewValid())
		return FALSE;

	// Modify menu items
	for (int nMenuItem = 0; nMenuItem < pContextMenu->GetMenuItemCount(); nMenuItem++) {
		// Get menu item ID
		UINT nItemID = pContextMenu->GetMenuItemID(nMenuItem);
		// Menu "Copy" item
		if (nItemID == IDM_DEBUGTEST_COPY) {
			// If currently not selecting any text
			int nStartSel, nEndSel;
			GetDebugEditView()->GetSel(nStartSel, nEndSel);
			// Start and end selection index are equal
			// means not selecting anything 
			if (nStartSel == nEndSel) {
				// Disable menu item
				pContextMenu->EnableMenuItem(nMenuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
		// Menu "Paste" item
		else if (nItemID == IDM_DEBUGTEST_PASTE) {
			// Check if clipboard content available in text format
			BOOL bClipboardTextAvailable = IsClipboardFormatAvailable(CF_TEXT);
			// If not available
			if (bClipboardTextAvailable != TRUE) {
				// Disable menu item
				pContextMenu->EnableMenuItem(nMenuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
		// Menu "Previous command" item
		else if (nItemID == IDM_DEBUGTEST_DISP_PREVCOMMAND) {
			// If debug command history is empty
			// or it is currently displaying first command
			if ((IsDebugCommandHistoryEmpty()) || (GetHistoryCurrentDispIndex() == 0)) {
				// Disable menu item
				pContextMenu->EnableMenuItem(nMenuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
		// Menu "Next command" item
		else if (nItemID == IDM_DEBUGTEST_DISP_NEXTCOMMAND) {
			// If debug command history is empty
			// or it is currently displaying last command
			if ((IsDebugCommandHistoryEmpty()) || 
				(GetHistoryCurrentDispIndex() >= (GetDebugCommandHistoryCount() - 1))) {
				// Disable menu item
				pContextMenu->EnableMenuItem(nMenuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
		// Menu "Clear buffer" item
		else if (nItemID == IDM_DEBUGTEST_CLEAR_BUFFER) {
			// If DebugTest buffer screen is empty
			if (m_strBuffer.IsEmpty()) {
				// Disable menu item
				pContextMenu->EnableMenuItem(nMenuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
	}

	// Show menu
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	UINT nFlags = TPM_LEFTALIGN | TPM_TOPALIGN;
	BOOL bResult = pContextMenu->TrackPopupMenu(nFlags, ptCursor.x, ptCursor.y, (CWnd*)this, NULL);

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatDebugCommand
//	Description:	Re-format debug command and return its length
//  Arguments:		strDebugCommand - Debug command (IN & OUT)
//  Return value:	int - Length of debug command
//
//////////////////////////////////////////////////////////////////////////

int CDebugTestDlg::FormatDebugCommand(CString& strDebugCommand)
{
	// If debug command is empty, do nothing
	if (strDebugCommand.IsEmpty())
		return 0;

	// Remove whitespace
	strDebugCommand.Trim();

	// Initialize a temporary string buffer
	int nSrcLength = strDebugCommand.GetLength();
	LPTSTR lpszBuffTemp = new TCHAR[nSrcLength + 1];

	// Remove invalid characters
	int nBuffCount = 0;
	for (int nIndex = 0; nIndex < nSrcLength; nIndex++) {

		TCHAR tch = strDebugCommand.GetAt(nIndex);
		switch (tch)
		{
		case Constant::Char::Return:
		case Constant::Char::EndLine:
			break;
		case Constant::Char::Tab:
			// Replace with space
			lpszBuffTemp[nBuffCount++] = Constant::Char::Space;
			break;
		default:
			// Add to buffer
			lpszBuffTemp[nBuffCount++] = tch;
			break;
		}
	}

	// Copy back formatted string
	strDebugCommand.Empty();
	strDebugCommand.FreeExtra();
	strDebugCommand.SetString(lpszBuffTemp, nBuffCount);

	// Clean-up temporary string buffer
	delete[] lpszBuffTemp;

	// Return the debug command's new length
	return strDebugCommand.GetLength();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ClearViewBuffer
//	Description:	Clear debug view screen buffer content
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::ClearViewBuffer(void)
{
	if (!IsDebugEditViewValid())
		return;

	// Clear buffer
	m_strBuffer = Constant::String::Empty;
	GetDebugEditView()->SetWindowText(m_strBuffer);

	// Backup buffer
	BackupDebugViewBuffer();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddLine
//	Description:	Add a string line to debug screen
//  Arguments:		lpszString - String line
//					bNewLine   - Whether to add a new empty line
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::AddLine(LPCTSTR lpszString, BOOL bNewLine /* = TRUE */)
{
	// If buffer not empty
	if (!m_strBuffer.IsEmpty()) {
		// Get end of buffer character
		int nBuffLength = m_strBuffer.GetLength();
		TCHAR tcEndChar = m_strBuffer.GetAt(nBuffLength - 1);

		// If end of buffer is not an endline
		if (tcEndChar != Constant::Char::Return && tcEndChar != Constant::Char::EndLine) {
			// Add an endline first
			m_strBuffer.Append(Constant::String::NewLine);
		}
	}

	// Add string line
	if (IS_NOT_EMPTY_STRING(lpszString)) {
		m_strBuffer.Append(lpszString);
	}

	// Re-check the end of buffer character
	if (bNewLine == TRUE) {
		int nBuffLength = m_strBuffer.GetLength();
		TCHAR tcEndChar = m_strBuffer.GetAt(nBuffLength - 1);

		// If end of buffer is not an endline
		if (tcEndChar != Constant::Char::Return && tcEndChar != Constant::Char::EndLine) {
			// Add a new empty line
			m_strBuffer.Append(Constant::String::NewLine);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateDisplay
//	Description:	Update debug screen display
//  Arguments:		bSeekToEnd	  - Move cursor to end of view
//					bNotifyParent - Notify to parent window about display update
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::UpdateDisplay(BOOL bSeekToEnd /* = FALSE */, BOOL bNotifyParent /* = TRUE */)
{
	// Get debug edit view
	CEdit* pDebugEditView = GetDebugEditView();
	if (!IsDebugEditViewValid())
		return;

	// Update display text
	pDebugEditView->SetWindowText(m_strBuffer);
	pDebugEditView->Invalidate();

	// Move to end
	if (bSeekToEnd == TRUE) {
		pDebugEditView->SetSel(static_cast<DWORD>(-1));
	}

	// Notify to parent window about display update
	if (bNotifyParent == TRUE) {
		this->NotifyParent(SM_WND_DEBUGOUTPUT_DISP, NULL, NULL);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddDebugCommandHistory
//	Description:	Add debug command to history
//  Arguments:		lpszCommand - Input command
//  Return value:	size_t - New item count
//
//////////////////////////////////////////////////////////////////////////

size_t CDebugTestDlg::AddDebugCommandHistory(LPCTSTR lpszCommand)
{
	// Only add if input command is not empty
	if (IS_NOT_EMPTY_STRING(lpszCommand)) {
		m_astrCommandHistory.push_back(lpszCommand);

		// Not currently displaying history
		if (!IsCurrentlyDispHistory()) {
			int nCount = GetDebugCommandHistoryCount();
			SetHistoryCurrentDispIndex(nCount);
		}
	}
	
	// Return new history item count
	return GetDebugCommandHistoryCount();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DispDebugCommandHistory
//	Description:	Display history command by index
//  Arguments:		nHistoryIndex - History index
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::DispDebugCommandHistory(int nHistoryIndex)
{
	// If debug command history is empty, do nothing
	if (IsDebugCommandHistoryEmpty())
		return;

	// Check index validity
	if ((nHistoryIndex < 0) && (nHistoryIndex >= GetDebugCommandHistoryCount()))
		return;

	// Get command at index
	CString strCommand = m_astrCommandHistory.at(nHistoryIndex);
	if (strCommand.IsEmpty())
		return;

	// Check if DebugTest edit view is available and focused
	if (IsDebugEditViewFocus()) {
		// Get last (current) line index
		int nCurLine = GetDebugEditView()->GetLineCount() - 1;

		// Get the character index of the start of the specified line
		int nLineStart = GetDebugEditView()->LineIndex(nCurLine);
		if (nLineStart != -1)
		{
			// Get the length of the line
			int nLineLength = GetDebugEditView()->LineLength(nLineStart);

			// Select the line
			GetDebugEditView()->SetSel(nLineStart, nLineStart + nLineLength);

			// Replace the selected line with the command line
			GetDebugEditView()->ReplaceSel(strCommand);

			// Update current displaying history index
			SetHistoryCurrentDispIndex(nHistoryIndex);

			// Set currently displaying history flag
			SetCurrentlyDispHistoryState(TRUE);
		}
	}
}

