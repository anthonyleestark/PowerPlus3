
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

#include "stdafx.h"
#include "DebugTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace CoreFuncs;


//////////////////////////////////////////////////////////////////////////
//
// Define macros for DebugTest functions
//
//////////////////////////////////////////////////////////////////////////

// Default min/max size
#define DEFAULT_MIN_WIDTH		720
#define DEFAULT_MIN_HEIGHT		480
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

	// Buffer content
	m_strBuffer = STRING_EMPTY;
	m_strBufferBak = STRING_EMPTY;

	// Debug command history
	m_bCurDispHistory = FALSE;
	m_nHistoryCurIndex = 0;
	m_astrCommandHistory.RemoveAll();
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
	ON_MESSAGE(SM_APP_DEBUGOUTPUT,		 &CDebugTestDlg::OnDebugOutput)
	ON_MESSAGE(SM_WND_DEBUGVIEWCLRSCR,	 &CDebugTestDlg::OnDebugViewClear)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
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
	// Set dialog title
	this->SetRcDialogCaption(IDS_APP_DEBUGTESTDLG_TITLE);

	// Get DebugTest edit view
	BOOL bRet = InitDebugEditView(IDC_DEBUGTEST_EDITVIEW);
	if (bRet == FALSE) {
		TRCLOG("Error: Debug edit view initialization failed!!!");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
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
	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);

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

	SDialog::OnGetMinMaxInfo(pMinMaxInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSize
//	Description:	Resizelog
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::OnSize(UINT nType, int nWidth, int nHeight)
{
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
		return LRESULT(RESULT_FAILED);

	// Format debug output log string
	CString strDebugOutputLog;
	strDebugOutputLog.Format(DEBUG_OUTPUT_FORMAT, LPARAM_TO_STRING(lParam));

	AddLine(strDebugOutputLog);

	// Display log and move cursor to end
	UpdateDisplay(TRUE);

	// Backup buffer
	BackupDebugViewBuffer();

	// Reset currently displaying history flag
	SetCurrentlyDispHistoryState(FALSE);

	return LRESULT(RESULT_SUCCESS);
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

LRESULT CDebugTestDlg::OnDebugViewClear(WPARAM wParam, LPARAM lParam)
{
	// Clear buffer
	ClearViewBuffer();

	return LRESULT(RESULT_SUCCESS);
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
				GetDebugEditView()->SetSel(-1);
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
					int nLineBeginIndex = GetDebugEditView()->LineIndex(nCaretLineIdx);

					// If the caret position is in the beginning of line
					// or is not in the last line
					if ((nCaretPos == nLineBeginIndex) ||
						(nCaretLineIdx != (GetDebugEditView()->GetLineCount() - 1))) {
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
				int nLineBeginIndex = GetDebugEditView()->LineIndex(nCaretLineIdx);

				// If the caret position is in the beginning of line
				// or is not in the last line
				if ((nCaretPos == nLineBeginIndex) ||
					(nCaretLineIdx != (GetDebugEditView()->GetLineCount() - 1))) {
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
	LPARAM lParam = MAKE_LPARAM_STRING(strDebugCommand);
	
	// Check if parent window is available
	if (IsParentWndAvailable()) {
		// Send to parent window
		GetParentWnd()->PostMessage(SM_APP_DEBUGCOMMAND, wParam, lParam);
	}
	// Check if main window is available
	else if (CWnd* pMainWnd = AfxGetMainWnd()) {
		// Send to main window
		pMainWnd->PostMessage(SM_APP_DEBUGCOMMAND, wParam, lParam);
	}
	// There's no window handle to send to
	else {
		// Just send to a NULL window and hope that app class will handle
		::PostMessage(NULL, SM_APP_DEBUGCOMMAND, wParam, lParam);
	}

	// Update debug command history
	AddDebugCommandHistory(strDebugCommand);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// Protected methods

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDebugEditView
//	Description:	Get access to the DebugTest edit view pointer
//  Arguments:		None
//  Return value:	CEdit* pointer
//
//////////////////////////////////////////////////////////////////////////

CEdit* CDebugTestDlg::GetDebugEditView(void)
{
	return m_pDebugEditView;
}

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
	return IsDebugEditViewValid();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsDebugEditViewValid
//	Description:	Check if the DebugTest edit view (pointer) is valid
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

inline BOOL CDebugTestDlg::IsDebugEditViewValid(void)
{
	return (GetDebugEditView() != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsDebugViewFocus
//	Description:	Check if the DebugTest edit view is focused
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

inline BOOL CDebugTestDlg::IsDebugEditViewFocus(void)
{
	// Check DebugTest edit view validity
	if (!IsDebugEditViewValid())
		return FALSE;

	// Check if it is focused
	HWND hCurFocusWnd = GetFocus()->GetSafeHwnd();
	return (hCurFocusWnd == GetDebugEditView()->GetSafeHwnd());
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
	menuDebugTest.LoadMenu(IDR_MENU_DEBUGTEST_EDITVIEW);
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
//	Function name:	BackupDebugViewBuffer
//	Description:	Backup debug view screen buffer content
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

inline void CDebugTestDlg::BackupDebugViewBuffer(void)
{
	// Backup buffer
	m_strBufferBak = m_strBuffer;
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
	LPTSTR lpszBuffTemp = new TCHAR[MAX_BUFFER_LENGTH];
	_tcscpy(lpszBuffTemp, strDebugCommand.operator LPCWSTR());

	// Remove invalid characters
	int nBuffIndex = 0;
	int nSrcLength = _tcslen(lpszBuffTemp);
	for (int nIndex = 0; nIndex < nSrcLength; nIndex++) {
		// If not an invalid character
		switch (lpszBuffTemp[nIndex])
		{
		case CHAR_RETURN:
		case CHAR_ENDLINE:
			break;
		default:
			// Add to buffer
			lpszBuffTemp[nBuffIndex] = lpszBuffTemp[nIndex];
			nBuffIndex++;
			break;
		}
	}

	// Copy back formatted string
	strDebugCommand.Empty();
	strDebugCommand.FreeExtra();
	strDebugCommand.SetString(lpszBuffTemp);

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
	m_strBuffer = STRING_EMPTY;
	GetDebugEditView()->SetWindowText(m_strBuffer);

	// Backup buffer
	BackupDebugViewBuffer();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddLine
//	Description:	Add a string line to debug screen
//  Arguments:		lpszString - String line
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::AddLine(LPCTSTR lpszString)
{
	// If buffer not empty
	if (!m_strBuffer.IsEmpty()) {
		// Get end of buffer character
		int nBuffLength = m_strBuffer.GetLength();
		TCHAR tcEndChar = m_strBuffer.GetAt(nBuffLength - 1);

		// If end of buffer is not an endline
		if (tcEndChar != CHAR_RETURN && tcEndChar != CHAR_ENDLINE) {
			// Add an endline first
			m_strBuffer.Append(STRING_NEWLINE);
		}
	}

	// Add string line
	m_strBuffer.Append(lpszString);
	m_strBuffer.Append(STRING_NEWLINE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateDisplay
//	Description:	Update debug screen display
//  Arguments:		bSeekToEnd - Move cursor to end of view
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::UpdateDisplay(BOOL bSeekToEnd /* = FALSE */)
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
		pDebugEditView->SetSel(-1);
	}

	// Check if parent window is available
	if (IsParentWndAvailable()) {
		// Send to parent window
		GetParentWnd()->PostMessage(SM_WND_DEBUGOUTPUTDISP);
	}
	// Check if main window is available
	else if (CWnd* pMainWnd = AfxGetMainWnd()) {
		// Send to main window to process
		pMainWnd->PostMessage(SM_WND_DEBUGOUTPUTDISP);
	}
	// There's no window handle to send to
	else {
		// Just send to a NULL window and hope that app class will handle it
		::PostMessage(NULL, SM_WND_DEBUGOUTPUTDISP, NULL, NULL);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddDebugCommandHistory
//	Description:	Add debug command to history
//  Arguments:		lpszCommand - Input command
//  Return value:	INT_PTR - New item count
//
//////////////////////////////////////////////////////////////////////////

INT_PTR CDebugTestDlg::AddDebugCommandHistory(LPCTSTR lpszCommand)
{
	// Only add if input command is not empty
	if (_tcscmp(lpszCommand, STRING_EMPTY)) {
		m_astrCommandHistory.Add(lpszCommand);

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
	CString strCommand = m_astrCommandHistory.GetAt(nHistoryIndex);
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ClearDebugCommandHistory
//	Description:	Clear current debug command history
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::ClearDebugCommandHistory(void)
{
	m_astrCommandHistory.RemoveAll();
	m_astrCommandHistory.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDebugCommandHistoryCount
//	Description:	Get debug command history item count
//  Arguments:		None
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

INT_PTR CDebugTestDlg::GetDebugCommandHistoryCount(void) const
{
	return m_astrCommandHistory.GetSize();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsDebugCommandHistoryEmpty
//	Description:	Check if current command history is empty or not
//  Arguments:		bSeekToEnd - Move cursor to end of view
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestDlg::IsDebugCommandHistoryEmpty(void) const
{
	return m_astrCommandHistory.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsCurrentlyDispHistory
//	Description:	Check if currently displaying command history or not
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

inline BOOL CDebugTestDlg::IsCurrentlyDispHistory(void) const
{
	return m_bCurDispHistory;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetCurrentlyDispHistoryState
//	Description:	Set currently displaying command history state
//  Arguments:		bState - State flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

inline void CDebugTestDlg::SetCurrentlyDispHistoryState(BOOL bState)
{
	m_bCurDispHistory = bState;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetHistoryCurrentDispIndex
//	Description:	Get command history current displaying index
//  Arguments:		None
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

inline INT_PTR CDebugTestDlg::GetHistoryCurrentDispIndex(void) const
{
	return m_nHistoryCurIndex;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetHistoryCurrentDispIndex
//	Description:	Set command history current displaying index
//  Arguments:		INT_PTR
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

inline void CDebugTestDlg::SetHistoryCurrentDispIndex(INT_PTR nCurIndex)
{
	m_nHistoryCurIndex = nCurIndex;
}

