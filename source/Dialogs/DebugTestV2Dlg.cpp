/**
 * @file		DebugTestDlgV2.cpp
 * @brief		Source file for DebugTestV2 dialog
 * @author		AnthonyLeeStark
 * @date		2025.06.02
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "Dialogs/DebugTestV2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace AppCore;


// Default min/max size
constexpr const int defaultMinWidth = 840;
constexpr const int defaultMinHeight = 500;
constexpr const int defaultMaxWidth = 1600;
constexpr const int defaultMaxHeight = 900;

// Default Debug command input height
constexpr const int debugCmdInputHeight = 50;

// Define constants
const wchar_t* debugCommandPrefix = _T(">>> ");


// Implement methods for CDebugTestV2Dlg
IMPLEMENT_DYNAMIC(CDebugTestV2Dlg, SDialog)


/**
 * @brief	Constructor
 */
CDebugTestV2Dlg::CDebugTestV2Dlg() : SDialog(IDD_DEBUGTEST_DLG)
{
	// DebugScreen
	m_pDebugView = NULL;
	m_pDebugCommandInput = NULL;

	// Font & brush
	m_pDebugScreenFont = NULL;
	m_pDebugScreenBrush = NULL;

	// Buffer content
	m_strBuffer = Constant::String::Empty;
	m_strBackupBuffer = Constant::String::Empty;
	m_strCommandBuffer = Constant::String::Empty;

	// Specific flags
	m_bDispCommandPrefix = true;

	// Debug command history
	m_bCurDispHistory = false;
	m_nHistoryCurIndex = 0;
	m_astrCommandHistory.clear();
}

/**
 * @brief	Destructor
 */
CDebugTestV2Dlg::~CDebugTestV2Dlg()
{
	// Clean-up debug command history
	ClearDebugCommandHistory();

	// Delete font
	if (m_pDebugScreenFont != NULL) {
		m_pDebugScreenFont->DeleteObject();
		delete m_pDebugScreenFont;
		m_pDebugScreenFont = NULL;
	}

	// Delete brush
	if (m_pDebugScreenBrush != NULL) {
		m_pDebugScreenBrush->DeleteObject();
		delete m_pDebugScreenBrush;
		m_pDebugScreenBrush = NULL;
	}
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CDebugTestV2Dlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


//////////////////////////////////////////////////////////////////////////
//
//	CDebugTestV2Dlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDebugTestV2Dlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_MESSAGE(SM_APP_DEBUG_OUTPUT, &CDebugTestV2Dlg::OnDebugOutput)
	ON_MESSAGE(SM_APP_DEBUGCMD_NOREPLY, &CDebugTestV2Dlg::OnDebugCmdNoReply)
	ON_MESSAGE(SM_WND_DEBUGVIEW_CLRSCR, &CDebugTestV2Dlg::OnDebugViewClear)
	ON_MESSAGE(SM_WND_SHOWDIALOG, &CDebugTestV2Dlg::OnShowDialog)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// CDebugTestV2Dlg message handlers

/**
 * @brief	Initialize DebugTest dialog
 * @param	None
 * @return	BOOL - Default
 */
BOOL CDebugTestV2Dlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Prevent flickering on startup
	this->ShowWindow(SW_HIDE);

	// Set dialog title
	this->SetCaptionFromResource(IDS_APP_DEBUGTESTDLG_TITLE);

	// Get DebugScreen (DebugView & Debug command input)
	bool bRet = InitDebugScreen();
	if (bRet == false) {
		TRACE_ERROR("Error: DebugScreen initialization failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return bRet;
	}

	// Refresh DebugScreen
	RefreshDebugScreen(Refresh::ScreenSize);

	// Bring window to top
	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	this->SetForegroundWindow();

	// Clear buffer
	ClearDebugViewBuffer();
	ClearDebugCommandInput();

	// Init debug command history
	ClearDebugCommandHistory();

	// Set focus to Debug command input
	GetDebugCommandInput()->SetFocus();

	return false;
}

/**
 * @brief	Close dialog
 * @param	None
 * @return	None
 */
void CDebugTestV2Dlg::OnClose()
{
	// Only hide the dialog
	ShowWindow(SW_HIDE);
}

/**
 * @brief	Destroy dialog
 * @param	None
 * @return	None
 */
void CDebugTestV2Dlg::OnDestroy()
{
	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	Dialog get min/max info handler
 * @param	Default
 * @return	None
 */
void CDebugTestV2Dlg::OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo)
{
	// Fix min/max size
	pMinMaxInfo->ptMinTrackSize = CPoint(defaultMinWidth, defaultMinHeight);
	pMinMaxInfo->ptMaxTrackSize = CPoint(defaultMaxWidth, defaultMaxHeight);

	// Default
	SDialog::OnGetMinMaxInfo(pMinMaxInfo);
}

/**
 * @brief	Set DebugScreen background and text color
 * @param	Default
 * @return	None
 */
HBRUSH CDebugTestV2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hBrush = SDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_STATIC)
	{
		// Text color: Yellow
		pDC->SetTextColor(Color::Yellow);

		// Background color: Black
		pDC->SetBkColor(Color::Black);

		// Use our custom background brush
		if (m_pDebugScreenBrush != NULL)
			return *m_pDebugScreenBrush;
	}

	return hBrush;
}

/**
 * @brief	Resizelog
 * @param	Default
 * @return	None
 */
void CDebugTestV2Dlg::OnSize(UINT nType, int nWidth, int nHeight)
{
	// Implement base class method
	SDialog::OnSize(nType, nWidth, nHeight);

	// Get DebugScreen
	if (!IsDebugScreenValid()) {
		bool bRet = InitDebugScreen();
		if (bRet == false)
			return;
	}

	// Refresh DebugScreen
	RefreshDebugScreen(Refresh::ScreenSize);
}

/**
 * @brief	Handle event when output debug contents
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	LRESULT
 */
LRESULT CDebugTestV2Dlg::OnDebugOutput(WPARAM wParam, LPARAM lParam)
{
	// Check argument validity
	if ((wParam == 0) || (lParam == NULL))
		return LRESULT(Result::Failure);

	// Format debug output log string
	String debugOutputLogString = LPARAM_TO_STRING(lParam);

	// Add debug output string
	AddLine(debugOutputLogString);

	// Display log and move cursor to end
	UpdateDisplay(true);

	// Backup buffer
	BackupDebugViewBuffer();

	// Reset currently displaying history flag
	SetCurrentlyDispHistoryState(false);

	return LRESULT(Result::Success);
}

/**
 * @brief	Handle event when a debug command has been processed but there's no reply
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	LRESULT
 */
LRESULT CDebugTestV2Dlg::OnDebugCmdNoReply(WPARAM /*wParam*/, LPARAM /*lParam*/)
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
LRESULT CDebugTestV2Dlg::OnDebugViewClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Clear buffer
	ClearDebugViewBuffer();

	return LRESULT(Result::Success);
}


/**
 * @brief	Show/hide dialog when receiving message
 * @param	wParam - Show/hide flag
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CDebugTestV2Dlg::OnShowDialog(WPARAM wParam, LPARAM /*lParam*/)
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
BOOL CDebugTestV2Dlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Process commands
	switch (LOWORD(wParam))
	{
	case IDM_DEBUGTEST_COPY:
		// Copy selection text to clipboard
		GetDebugView()->Copy();
		break;

	case IDM_DEBUGTEST_PASTE:
		// Check if the Debug command input has the focus
		if (IsDebugCommandInputFocused() == true) {
			// Paste clipboard text to DebugTest edit view
			GetDebugCommandInput()->Paste();
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
		ClearDebugViewBuffer();
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
BOOL CDebugTestV2Dlg::PreTranslateMessage(MSG* pMsg)
{
	// Handle key pressed event for DebugTest edit view
	if (pMsg->message == WM_KEYDOWN) {

		// Get pressed key
		DWORD dwKey = pMsg->wParam;

		// Process "Ctrl + Key" shortcuts
		if (IS_PRESSED(VK_CONTROL)) {

			// Select All (Ctrl + A)
			if (dwKey == 0x41) {
				PostMessage(WM_COMMAND, IDM_DEBUGTEST_SELECT_ALL, NULL);
				return true;
			}

			// Copy (Ctrl + C)
			else if (dwKey == 0x43) {
				PostMessage(WM_COMMAND, IDM_DEBUGTEST_COPY, NULL);
				return true;
			}

			// Paste (Ctrl + V)
			else if (dwKey == 0x56) {
				PostMessage(WM_COMMAND, IDM_DEBUGTEST_PASTE, NULL);
				return true;
			}

			return true;
		}

		// If the Debug command input has the focus
		else if (IsDebugCommandInputFocused()) {

			// If [Enter] key is pressed
			if (dwKey == VK_RETURN) {

				// Send debug command
				SendDebugCommand();
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

		// Check DebugScreen validity
		if (!IsDebugScreenValid())
			return false;

		// Get the DebugView rect
		RECT rcDebugView;
		GetDebugView()->GetWindowRect(&rcDebugView);
		ScreenToClient(&rcDebugView);

		// Get the Debug command input rect
		RECT rcDebugCommandInput;
		GetDebugCommandInput()->GetWindowRect(&rcDebugCommandInput);
		ScreenToClient(&rcDebugCommandInput);

		// If clicked point is inside the DebugView area
		if (PtInRect(&rcDebugView, pt)) {
			// Show DebugView context menu
			ShowDebugScreenContextMenu();
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
bool CDebugTestV2Dlg::SendDebugCommand(void)
{
	// Check Debug command input validity
	if (!IsDebugCommandInputValid())
		return false;

	// Backup buffer
	BackupDebugViewBuffer();

	// Get the debug command line
	const int buffLength = GetDebugCommandInput()->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(buffLength + 1);
	GetDebugCommandInput()->GetWindowText(tempBuff.data(), buffLength + 1);
	String debugCommand = tempBuff.data();

	// Clear Debug command input
	ClearDebugCommandInput();

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
 * @brief	Initialize the DebugTest view pointer
 * @param	None
 * @return	true/false
 */
bool CDebugTestV2Dlg::InitDebugScreen(void)
{
	// If it has already been initialized, do nothing
	if (IsDebugScreenValid())
		return true;

	// Initialize
	m_pDebugView = (CEdit*)GetDlgItem(IDC_DEBUGTESTV2_VIEW);
	m_pDebugCommandInput = (CEdit*)GetDlgItem(IDC_DEBUGTESTV2_INPUT);

	// Set DebugScreen font & background color
	if (IsDebugScreenValid()) {

		if (!CreateDebugScreenFont() || !CreateDebugScreenBrush())
			return false;

		// Set font
		m_pDebugView->SetFont(m_pDebugScreenFont);
		m_pDebugCommandInput->SetFont(m_pDebugScreenFont);

		// Force redraw
		m_pDebugView->Invalidate();
		m_pDebugCommandInput->Invalidate();
	}

	return IsDebugScreenValid();
}

/**
 * @brief	Initialize font for DebugScreen
 * @param	None
 * @return	true/false
 */
bool CDebugTestV2Dlg::CreateDebugScreenFont(void)
{
	// Initialization
	if (m_pDebugScreenFont == NULL) {
		m_pDebugScreenFont = new CFont();
		if (m_pDebugScreenFont == NULL) {
			// Trace error
			TRACE_ERROR("Error: Failed to create DebugScreen font!!!");
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
	if (m_pDebugScreenFont != NULL) {
		bRet = m_pDebugScreenFont->CreateFontIndirect(&lf);
	}

	return bRet;
}

/**
 * @brief	Initialize brush for adjust DebugScreen color
 * @param	None
 * @return	true/false
 */
bool CDebugTestV2Dlg::CreateDebugScreenBrush(void)
{
	// Initialization
	if (m_pDebugScreenBrush == NULL) {
		m_pDebugScreenBrush = new CBrush();
		if (m_pDebugScreenBrush == NULL) {
			// Trace error
			TRACE_ERROR("Error: Failed to create DebugScreen brush!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
	}

	// Create brush
	bool bRet = false;
	if (m_pDebugScreenBrush != NULL) {
		bRet = m_pDebugScreenBrush->CreateSolidBrush(Color::Black);
	}

	return bRet;
}

/**
 * @brief	Refresh and update DebugScreen
 * @param	nFlag - Refresh flag
 * @param	Return value:	true/false
 */
bool CDebugTestV2Dlg::RefreshDebugScreen(int nFlag)
{
	// Check DebugScreen validity
	if (!IsDebugScreenValid())
		return false;

	if (nFlag & Refresh::ScreenSize) {

		RECT rcClient;
		this->GetClientRect(&rcClient);

		// DebugScreen position
		int nXPos = rcClient.left;
		int nDBViewYPos = rcClient.top;
		int nDBCmdInputYPos = rcClient.bottom - debugCmdInputHeight;

		// DebugScreen size
		int nWidth = rcClient.right - rcClient.left;
		int nDBViewHeight = nDBCmdInputYPos - nDBViewYPos;
		int nDBCmdInputHeight = debugCmdInputHeight;

		// Set DebugScreen position
		GetDebugView()->SetWindowPos(NULL, nXPos, nDBViewYPos, nWidth, nDBViewHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
		GetDebugCommandInput()->SetWindowPos(NULL, nXPos, nDBCmdInputYPos, nWidth, nDBCmdInputHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
	}

	return true;
}

/**
 * @brief	Show DebugScreen context menu
 * @param	None
 * @param	Return value:	bool - Show menu successfully or failed
 */
bool CDebugTestV2Dlg::ShowDebugScreenContextMenu(void)
{
	// Prepare menu
	CMenu menuDebugTest, * pContextMenu;
	menuDebugTest.LoadMenu(IDR_MENU_DEBUGTEST_CONTEXT);
	pContextMenu = menuDebugTest.GetSubMenu(0);
	if (pContextMenu == NULL)
		return false;

	// Check DebugTest edit view validity
	if (!IsDebugScreenValid())
		return false;

	// Modify menu items
	for (int nMenuItem = 0; nMenuItem < pContextMenu->GetMenuItemCount(); nMenuItem++) {
		// Get menu item ID
		unsigned nItemID = pContextMenu->GetMenuItemID(nMenuItem);
		// Menu "Copy" item
		if (nItemID == IDM_DEBUGTEST_COPY) {
			// If currently not selecting any text
			int nStartSel, nEndSel;
			GetDebugView()->GetSel(nStartSel, nEndSel);
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
int CDebugTestV2Dlg::FormatDebugCommand(String& debugCommand) const
{
	// If debug command is empty, do nothing
	if (debugCommand.IsEmpty())
		return 0;

	// Remove prefix
	int nSrcLength = debugCommand.GetLength();
	if (m_bDispCommandPrefix == true) {
		nSrcLength -= wcslen(debugCommandPrefix);
		String tempString = debugCommand.Right(nSrcLength);
		debugCommand = tempString;
	}

	// Remove leading/trailing spaces
	debugCommand.Trim();

	// Initialize a temporary string buffer
	nSrcLength = debugCommand.GetLength();
	std::vector<wchar_t> tempBuff(nSrcLength + 1);

	// Remove invalid characters
	for (int nIndex = 0; nIndex < nSrcLength; nIndex++) {

		wchar_t ch = debugCommand.GetAt(nIndex);
		switch (ch)
		{
		case Constant::Char::Return:
		case Constant::Char::EndLine:
			break;
		case Constant::Char::Tab:
			// Replace with space
			tempBuff.push_back(Constant::Char::Space);
			break;
		default:
			// Add to buffer
			tempBuff.push_back(ch);
			break;
		}
	}

	// Copy back formatted string
	debugCommand.Empty();
	debugCommand.SetString(tempBuff.data());

	// Remove leading/trailing spaces again
	debugCommand.Trim();

	// Return the debug command's new length
	return debugCommand.GetLength();
}

/**
 * @brief	Clear and re-initialize Debug command input buffer
 * @param	None
 * @return	None
 */
void CDebugTestV2Dlg::ClearDebugCommandInput(const wchar_t* commandBuff /* = Constant::String::Empty */)
{
	if (!IsDebugCommandInputValid())
		return;

	// Re-initialize Debug command buffer
	String commandTempStr = Constant::String::Empty;
	if (m_bDispCommandPrefix == true) {
		commandTempStr = debugCommandPrefix;
	}
	if (IS_NOT_EMPTY_STRING(commandBuff)) {
		commandTempStr += commandBuff;
	}

	// Update display
	GetDebugCommandInput()->SetWindowText(commandTempStr);

	// Move to end
	GetDebugCommandInput()->SetSel(static_cast<DWORD>(-1));
}

/**
 * @brief	Clear debug view screen buffer content
 * @param	None
 * @return	None
 */
void CDebugTestV2Dlg::ClearDebugViewBuffer(void)
{
	if (!IsDebugViewValid())
		return;

	// Clear buffer
	m_strBuffer = Constant::String::Empty;
	GetDebugView()->SetWindowText(m_strBuffer);

	// Backup buffer
	BackupDebugViewBuffer();
}

/**
 * @brief	Add a string line to debug screen
 * @param	lineString - String line
 * @param	bNewLine   - Whether to add a new empty line
 * @return	None
 */
void CDebugTestV2Dlg::AddLine(const wchar_t* lineString, bool bNewLine /* = true */)
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
void CDebugTestV2Dlg::UpdateDisplay(bool bSeekToEnd /* = false */, bool bNotifyParent /* = true */)
{
	// Get debug edit view
	CEdit* pDebugView = GetDebugView();
	if (!IsDebugViewValid())
		return;

	// Update display text
	pDebugView->SetWindowText(m_strBuffer);
	pDebugView->Invalidate();

	// Move to end
	if (bSeekToEnd == true) {
		pDebugView->SetSel(static_cast<DWORD>(-1));
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
size_t CDebugTestV2Dlg::AddDebugCommandHistory(const wchar_t* commandString)
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
void CDebugTestV2Dlg::DispDebugCommandHistory(int nHistoryIndex)
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
	if (IsDebugCommandInputFocused()) {

		// Display command
		ClearDebugCommandInput(commandString);

		// Update current displaying history index
		SetHistoryCurrentDispIndex(nHistoryIndex);

		// Set currently displaying history flag
		SetCurrentlyDispHistoryState(true);
	}
}
