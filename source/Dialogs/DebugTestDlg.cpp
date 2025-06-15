/**
 * @file		DebugTestDlg.cpp
 * @brief		Source file for DebugTest dialog
 * @author		AnthonyLeeStark
 * @date		2024.07.13
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "Dialogs/DebugTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace AppCore;


// Default min/max size
constexpr const int defaultMinWidth = 840;
constexpr const int defaultMinHeight = 500;
constexpr const int defaultMaxWidth = 1600;
constexpr const int defaultMaxHeight = 900;

// Output format
constexpr const wchar_t* debugOutputFormat = L">> %s";


// Implement methods for CDebugTestDlg
IMPLEMENT_DYNAMIC(CDebugTestDlg, SDialog)


/**
 * @brief	Constructor
 */
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
	m_bCurDispHistory = false;
	m_nHistoryCurIndex = 0;
	m_astrCommandHistory.clear();
}

/**
 * @brief	Destructor
 */
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

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
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

/**
 * @brief	Initialize DebugTest dialog
 * @param	None
 * @return	BOOL - Default
 */
BOOL CDebugTestDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Prevent flickering on startup
	this->ShowWindow(SW_HIDE);

	// Set dialog title
	this->SetCaptionFromResource(IDS_APP_DEBUGTESTDLG_TITLE);

	// Get DebugTest edit view
	bool bRet = InitDebugEditView(IDC_DEBUGTEST_EDITVIEW);
	if (bRet == false) {
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

	return true;
}

/**
 * @brief	Close dialog
 * @param	None
 * @return	None
 */
void CDebugTestDlg::OnClose()
{
	// Only hide the dialog
	ShowWindow(SW_HIDE);
}

/**
 * @brief	Destroy dialog
 * @param	None
 * @return	None
 */
void CDebugTestDlg::OnDestroy()
{
	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	Dialog get min/max info handler
 * @param	Default
 * @return	None
 */
void CDebugTestDlg::OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo)
{
	// Fix min/max size
	pMinMaxInfo->ptMinTrackSize = CPoint(defaultMinWidth, defaultMinHeight);
	pMinMaxInfo->ptMaxTrackSize = CPoint(defaultMaxWidth, defaultMaxHeight);

	// Default
	SDialog::OnGetMinMaxInfo(pMinMaxInfo);
}

/**
 * @brief	Set background and text color
 * @param	Default
 * @return	None
 */
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

/**
 * @brief	Resize dialog
 * @param	Default
 * @return	None
 */
void CDebugTestDlg::OnSize(UINT nType, int nWidth, int nHeight)
{
	// Implement base class method
	SDialog::OnSize(nType, nWidth, nHeight);

	// Get DebugTest edit view
	if (!IsDebugEditViewValid()) {
		bool bRet = InitDebugEditView(IDC_DEBUGTEST_EDITVIEW);
		if (bRet == false)
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

/**
 * @brief	Handle event when debug view edit contents changed
 * @param	None
 * @return	None
 */
void CDebugTestDlg::OnDebugViewEditChange(void)
{
	// Check debug view pointer validity
	if (!IsDebugEditViewValid())
		return;

	// Update buffer content
	const int buffLength = GetDebugEditView()->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(buffLength + 1);
	GetDebugEditView()->GetWindowText(tempBuff.data(), buffLength + 1);
	m_strBuffer = tempBuff.data();

	// If buffer length did not increase
	int nBufferLength = m_strBuffer.GetLength();
	int nBufferBakLength = m_strBufferBak.GetLength();
	if (nBufferLength <= nBufferBakLength) {
		// Backup buffer
		BackupDebugViewBuffer();
	}
}

/**
 * @brief	Handle event when output debug contents
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	LRESULT
 */
LRESULT CDebugTestDlg::OnDebugOutput(WPARAM wParam, LPARAM lParam)
{
	// Check argument validity
	if ((wParam == 0) || (lParam == NULL))
		return LRESULT(Result::Failure);

	// Format debug output log string
	String debugOutputLogStr;
	debugOutputLogStr.Format(debugOutputFormat, LPARAM_TO_STRING(lParam));

	// Add debug output string
	AddLine(debugOutputLogStr);

	// Display log and move cursor to end
	UpdateDisplay(true);

	// Backup buffer
	BackupDebugViewBuffer();

	// Reset currently displaying history flag
	SetCurrentlyDispHistoryState(false);

	return LRESULT(Result::Success);
}

/**
 * @brief	Handle event when a debug command has been processed
					but there's no reply
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	LRESULT
 */
LRESULT CDebugTestDlg::OnDebugCmdNoReply(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Add an empty new line
	AddLine(Constant::String::Empty, false);

	// Display log and move cursor to end
	UpdateDisplay(true, false);

	// Backup buffer
	BackupDebugViewBuffer();

	// Reset currently displaying history flag
	SetCurrentlyDispHistoryState(false);

	return LRESULT(Result::Success);
}

/**
 * @brief	Handle debug view clear screen event
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	LRESULT
 */
LRESULT CDebugTestDlg::OnDebugViewClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Clear buffer
	ClearViewBuffer();

	return LRESULT(Result::Success);
}


/**
 * @brief	Show/hide dialog when receiving message
 * @param	wParam - Show/hide flag
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CDebugTestDlg::OnShowDialog(WPARAM wParam, LPARAM /*lParam*/)
{
	// Get flag value
	bool bShowFlag = true;
	if (wParam != NULL) {
		bShowFlag = static_cast<bool>(wParam);
	}

	// Show/hide dialog
	if (bShowFlag == true) {

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

/**
 * @brief	Handle app command messages (WM_COMMAND)
	Arguments:		wParam - First param (HIWORD)
					lParam - Second param (LOWORD)
 * @return	BOOL
 */
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
		if (IsDebugEditViewFocus() == true) {
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

/**
 * @brief	Pre-translate message
 * @param	pMsg - Default
 * @return	BOOL
 */
BOOL CDebugTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// Handle key pressed event for DebugTest edit view
	if (pMsg->message == WM_KEYDOWN) {

		// Check if focus belongs to DebugTest edit view
		if (IsDebugEditViewFocus() == true) {

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
						return true;
					}
				}
				else {
					// Block --> Do nothing
					return true;
				}
			}

			// If [Enter] key is pressed
			if (dwKey == VK_RETURN) {

				//Send debug command
				bool bRet = SendDebugCommand();

				// If debug command is sent, block the break-line
				if (bRet == true) return bRet;
			}
			// If [Backspace] or [Delete] keys are pressed
			else if ((dwKey == VK_BACK) || (dwKey == VK_DELETE)) {

				// Only allow erasing inputted content
				if (m_strBuffer.Compare(m_strBufferBak) == 0)
					return true;

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
						return true;
					}
				}
			}
			// If "Ctrl+A" keys are pressed
			else if ((dwKey == 0x41) && (IS_PRESSED(VK_CONTROL))) {
				// Block --> Do nothing
				return true;
			}
			// If the [Up/Down] arrow keys are pressed
			else if ((dwKey == VK_UP) || (dwKey == VK_DOWN)) {

				// If debug command history is empty
				if (IsDebugCommandHistoryEmpty())
					return true;

				// Get command history display index
				int nHistoryDispIndex = 0;

				// [Up] arrow key --> Display previous command
				if (dwKey == VK_UP) {

					// Get index
					nHistoryDispIndex = GetHistoryCurrentDispIndex() - 1;

					// If current index is 0
					if (nHistoryDispIndex < 0)
						return true;
				}
				// [Down] arrow key --> Display next command
				else if (dwKey == VK_DOWN) {

					// Get index
					nHistoryDispIndex = GetHistoryCurrentDispIndex() + 1;

					// If current index exceeded limit
					if (nHistoryDispIndex >= GetDebugCommandHistoryCount())
						return true;
				}

				// Display debug command
				DispDebugCommandHistory(nHistoryDispIndex);
				return true;
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
					return true;
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
			return true;
		}
	}

	// Default
	return SDialog::PreTranslateMessage(pMsg);
}

/**
 * @brief	Send debug command to main parts of program
 * @param	hRcvWnd - Receive window handle
 * @return	None
 */
bool CDebugTestDlg::SendDebugCommand(void)
{
	// Check DebugTest edit view validity
	if (!IsDebugEditViewValid())
		return false;

	// Backup buffer
	BackupDebugViewBuffer();

	// Get the debug command line length
	int nCurLine = (GetDebugEditView()->GetLineCount() - 1);
	int nLineIndex = GetDebugEditView()->LineIndex(nCurLine);
	int nLineLength = GetDebugEditView()->LineLength(nLineIndex);

	// Get the debug command line
	std::wstring tempBuff{};
	tempBuff.resize(nLineLength + 1);
	int nBuffLength = GetDebugEditView()->GetLine(nCurLine, &tempBuff[0], nLineLength + 1);
	tempBuff.resize(nBuffLength);
	String debugCommand = tempBuff;

	// Re-format the debug command
	int nCommandLength = FormatDebugCommand(debugCommand);

	// If debug command is empty, do not send
	if (nCommandLength <= 0)
		return false;

	// Prepare params
	WPARAM wParam = MAKE_WPARAM_STRING(debugCommand);
	LPARAM lParam = MAKE_LPARAM_STRING(debugCommand.GetString());
	
	// Send debug command message to parent window
	this->NotifyParent(SM_APP_DEBUG_COMMAND, wParam, lParam);

	// Update debug command history
	AddDebugCommandHistory(debugCommand);

	return true;
}


//////////////////////////////////////////////////////////////////////////
// Protected methods

/**
 * @brief	Initialize the DebugTest edit view pointer
 * @param	nCtrlID - Dialog control ID
 * @return	true/false
 */
bool CDebugTestDlg::InitDebugEditView(unsigned nCtrlID)
{
	// If it has already been initialized, do nothing
	if (IsDebugEditViewValid())
		return true;

	// Initialize
	m_pDebugEditView = (CEdit*)GetDlgItem(nCtrlID);

	// Set DebugView font & background color
	if (IsDebugEditViewValid()) {
		if (CreateDebugViewFont()) {
			m_pDebugEditView->SetFont(m_pDebugViewFont);
		}
		if (!CreateDebugViewBrush())
			return false;
	}

	return IsDebugEditViewValid();
}

/**
 * @brief	Initialize font for DebugTest edit view
 * @param	None
 * @return	true/false
 */
bool CDebugTestDlg::CreateDebugViewFont(void)
{
	// Initialization
	if (m_pDebugViewFont == NULL) {
		m_pDebugViewFont = new CFont();
		if (m_pDebugViewFont == NULL) {
			// Trace error
			TRACE_ERROR("Error: Failed to create font!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
	}

	// Setup font properties
	LOGFONT lf = {0};
	_tcscpy_s(lf.lfFaceName, _T("Cascadia Mono"));
	lf.lfHeight = -16;
	lf.lfWeight = FW_SEMIBOLD;
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;

	// Create font
	bool bRet = false;
	if (m_pDebugViewFont != NULL) {
		bRet = m_pDebugViewFont->CreateFontIndirect(&lf);
	}

	return bRet;
}

/**
 * @brief	Initialize brush for adjust DebugTest edit view color
 * @param	None
 * @return	true/false
 */
bool CDebugTestDlg::CreateDebugViewBrush(void)
{
	// Initialization
	if (m_pDebugViewBrush == NULL) {
		m_pDebugViewBrush = new CBrush();
		if (m_pDebugViewBrush == NULL) {
			// Trace error
			TRACE_ERROR("Error: Failed to create brush!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
	}

	// Create brush
	bool bRet = false;
	if (m_pDebugViewBrush != NULL) {
		bRet = m_pDebugViewBrush->CreateSolidBrush(Color::Black);
	}

	return bRet;
}

/**
 * @brief	Get the caret's current position
 * @param	None
 * @return	int - Caret position
 */
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

/**
 * @brief	Show DebugTest edit view context menu
 * @param	None
 * @param	Return value:	bool - Show menu successfully or failed
 */
bool CDebugTestDlg::ShowDebugTestEditViewMenu(void)
{
	// Prepare menu
	CMenu menuDebugTest, *pContextMenu;
	menuDebugTest.LoadMenu(IDR_MENU_DEBUGTEST_CONTEXT);
	pContextMenu = menuDebugTest.GetSubMenu(0);
	if (pContextMenu == NULL)
		return false;

	// Check DebugTest edit view validity
	if (!IsDebugEditViewValid())
		return false;

	// Modify menu items
	for (int nMenuItem = 0; nMenuItem < pContextMenu->GetMenuItemCount(); nMenuItem++) {
		// Get menu item ID
		unsigned nItemID = pContextMenu->GetMenuItemID(nMenuItem);
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
			bool bClipboardTextAvailable = IsClipboardFormatAvailable(CF_TEXT);
			// If not available
			if (bClipboardTextAvailable != true) {
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
	unsigned nFlags = TPM_LEFTALIGN | TPM_TOPALIGN;
	bool bResult = pContextMenu->TrackPopupMenu(nFlags, ptCursor.x, ptCursor.y, (CWnd*)this, NULL);

	return bResult;
}

/**
 * @brief	Re-format debug command and return its length
 * @param	debugCommand - Debug command (IN & OUT)
 * @return	int - Length of debug command
 */
int CDebugTestDlg::FormatDebugCommand(String& debugCommand)
{
	// If debug command is empty, do nothing
	if (debugCommand.IsEmpty())
		return 0;

	// Remove whitespace
	debugCommand.Trim();

	// Initialize a temporary string buffer
	std::wstring copyBuffer = debugCommand;
	std::wstring tempNewBuffer = Constant::String::Empty;

	// Remove invalid characters
	for (wchar_t ch : copyBuffer)
	{
		switch (ch)
		{
		case Constant::Char::Return:
		case Constant::Char::EndLine:
			break;
		case Constant::Char::Tab:
			// Replace with space
			tempNewBuffer.push_back(Constant::Char::Space);
			break;
		default:
			// Add to buffer
			tempNewBuffer.push_back(ch);
			break;
		}
	}

	// Copy back formatted string
	debugCommand.Empty();
	debugCommand.SetString(tempNewBuffer);

	// Return the debug command's new length
	return debugCommand.GetLength();
}

/**
 * @brief	Clear debug view screen buffer content
 * @param	None
 * @return	None
 */
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

/**
 * @brief	Add a string line to debug screen
 * @param	lineString - String line
 * @param	bNewLine   - Whether to add a new empty line
 * @return	None
 */
void CDebugTestDlg::AddLine(const wchar_t* lineString, bool bNewLine /* = true */)
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
	if (IS_NOT_EMPTY_STRING(lineString)) {
		m_strBuffer.Append(lineString);
	}

	// Re-check the end of buffer character
	if (bNewLine == true) {
		int nBuffLength = m_strBuffer.GetLength();
		TCHAR tcEndChar = m_strBuffer.GetAt(nBuffLength - 1);

		// If end of buffer is not an endline
		if (tcEndChar != Constant::Char::Return && tcEndChar != Constant::Char::EndLine) {
			// Add a new empty line
			m_strBuffer.Append(Constant::String::NewLine);
		}
	}
}

/**
 * @brief	Update debug screen display
 * @param	bSeekToEnd	  - Move cursor to end of view
 * @param	bNotifyParent - Notify to parent window about display update
 * @return	None
 */
void CDebugTestDlg::UpdateDisplay(bool bSeekToEnd /* = false */, bool bNotifyParent /* = true */)
{
	// Get debug edit view
	CEdit* pDebugEditView = GetDebugEditView();
	if (!IsDebugEditViewValid())
		return;

	// Update display text
	pDebugEditView->SetWindowText(m_strBuffer);
	pDebugEditView->Invalidate();

	// Move to end
	if (bSeekToEnd == true) {
		pDebugEditView->SetSel(static_cast<DWORD>(-1));
	}

	// Notify to parent window about display update
	if (bNotifyParent == true) {
		this->NotifyParent(SM_WND_DEBUGOUTPUT_DISP, NULL, NULL);
	}
}

/**
 * @brief	Add debug command to history
 * @param	commandString - Input command
 * @return	size_t - New item count
 */
size_t CDebugTestDlg::AddDebugCommandHistory(const wchar_t* commandString)
{
	// Only add if input command is not empty
	if (IS_NOT_EMPTY_STRING(commandString)) {
		m_astrCommandHistory.push_back(commandString);

		// Not currently displaying history
		if (!IsCurrentlyDispHistory()) {
			int nCount = GetDebugCommandHistoryCount();
			SetHistoryCurrentDispIndex(nCount);
		}
	}
	
	// Return new history item count
	return GetDebugCommandHistoryCount();
}

/**
 * @brief	Display history command by index
 * @param	nHistoryIndex - History index
 * @return	None
 */
void CDebugTestDlg::DispDebugCommandHistory(int nHistoryIndex)
{
	// If debug command history is empty, do nothing
	if (IsDebugCommandHistoryEmpty())
		return;

	// Check index validity
	if ((nHistoryIndex < 0) && (nHistoryIndex >= GetDebugCommandHistoryCount()))
		return;

	// Get command at index
	String commandString = m_astrCommandHistory.at(nHistoryIndex);
	if (commandString.IsEmpty())
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
			GetDebugEditView()->ReplaceSel(commandString);

			// Update current displaying history index
			SetHistoryCurrentDispIndex(nHistoryIndex);

			// Set currently displaying history flag
			SetCurrentlyDispHistoryState(true);
		}
	}
}

