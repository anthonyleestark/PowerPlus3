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
	debugViewPtr_ = NULL;
	debugCommandInputPtr_ = NULL;

	// Font & brush
	debugScreenFontPtr_ = NULL;
	debugScreenBrushPtr_ = NULL;

	// Buffer content
	bufferString_ = Constant::String::Empty;
	backupBufferString_ = Constant::String::Empty;
	commandBuffer_ = Constant::String::Empty;

	// Specific flags
	isCommandPrefixEnabled_ = true;

	// Debug command history
	isCurrentlyDisplayHistory_ = false;
	currentHistoryIndex_ = 0;
	commandHistoryList_.clear();
}

/**
 * @brief	Destructor
 */
CDebugTestV2Dlg::~CDebugTestV2Dlg()
{
	// Clean-up debug command history
	ClearDebugCommandHistory();

	// Delete font
	if (debugScreenFontPtr_ != NULL) {
		debugScreenFontPtr_->DeleteObject();
		delete debugScreenFontPtr_;
		debugScreenFontPtr_ = NULL;
	}

	// Delete brush
	if (debugScreenBrushPtr_ != NULL) {
		debugScreenBrushPtr_->DeleteObject();
		delete debugScreenBrushPtr_;
		debugScreenBrushPtr_ = NULL;
	}
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CDebugTestV2Dlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


// CDebugTestV2Dlg dialog message map
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
	this->setCaptionFromResource(IDS_APP_DEBUGTESTDLG_TITLE);

	// Get DebugScreen (DebugView & Debug command input)
	bool returnFlag = InitDebugScreen();
	if (returnFlag == false) {
		TRACE_ERROR("Error: DebugScreen initialization failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return returnFlag;
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
void CDebugTestV2Dlg::OnGetMinMaxInfo(MINMAXINFO* minMaxInfoPtr)
{
	// Fix min size
	minMaxInfoPtr->ptMinTrackSize.x = defaultMinWidth;
	minMaxInfoPtr->ptMinTrackSize.y = defaultMinHeight;

	// Fix max size
	minMaxInfoPtr->ptMaxTrackSize.x = defaultMaxWidth;
	minMaxInfoPtr->ptMinTrackSize.y = defaultMaxHeight;

	// Default
	SDialog::OnGetMinMaxInfo(minMaxInfoPtr);
}

/**
 * @brief	Set DebugScreen background and text color
 * @param	Default
 * @return	None
 */
HBRUSH CDebugTestV2Dlg::OnCtlColor(CDC* pDC, CWnd* windowPtr, UINT nCtlColor)
{
	HBRUSH hBrush = SDialog::OnCtlColor(pDC, windowPtr, nCtlColor);

	if (nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_STATIC)
	{
		// Text color: Yellow
		pDC->SetTextColor(Color::Yellow);

		// Background color: Black
		pDC->SetBkColor(Color::Black);

		// Use our custom background brush
		if (debugScreenBrushPtr_ != NULL)
			return *debugScreenBrushPtr_;
	}

	return hBrush;
}

/**
 * @brief	Resizelog
 * @param	Default
 * @return	None
 */
void CDebugTestV2Dlg::OnSize(UINT nType, int width, int height)
{
	// Implement base class method
	SDialog::OnSize(nType, width, height);

	// Get DebugScreen
	if (!IsDebugScreenValid()) {
		bool returnFlag = InitDebugScreen();
		if (returnFlag == false)
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
	bool showFlag = true;
	if (wParam != NULL) {
		showFlag = static_cast<bool>(wParam);
	}

	// Show/hide dialog
	if (showFlag == true) {

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
 * @param	wParam - First param (HIWORD)
 * @param	lParam - Second param (LOWORD)
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
 * @param	messagePtr - Default
 * @return	BOOL
 */
BOOL CDebugTestV2Dlg::PreTranslateMessage(MSG* messagePtr)
{
	// Handle key pressed event for DebugTest edit view
	if (messagePtr->message == WM_KEYDOWN) {

		// Get pressed key
		DWORD keyCode = messagePtr->wParam;

		// Process "Ctrl + Key" shortcuts
		if (IS_PRESSED(VK_CONTROL)) {

			// Select All (Ctrl + A)
			if (keyCode == 0x41) {
				PostMessage(WM_COMMAND, IDM_DEBUGTEST_SELECT_ALL, NULL);
				return true;
			}

			// Copy (Ctrl + C)
			else if (keyCode == 0x43) {
				PostMessage(WM_COMMAND, IDM_DEBUGTEST_COPY, NULL);
				return true;
			}

			// Paste (Ctrl + V)
			else if (keyCode == 0x56) {
				PostMessage(WM_COMMAND, IDM_DEBUGTEST_PASTE, NULL);
				return true;
			}

			return true;
		}

		// If the Debug command input has the focus
		else if (IsDebugCommandInputFocused()) {

			// If [Enter] key is pressed
			if (keyCode == VK_RETURN) {

				// Send debug command
				SendDebugCommand();
				return true;
			}

			// If the [Up/Down] arrow keys are pressed
			else if ((keyCode == VK_UP) || (keyCode == VK_DOWN)) {

				// If debug command history is empty
				if (IsDebugCommandHistoryEmpty())
					return true;

				// Get command history display index
				int historyDispIndex = 0;

				// [Up] arrow key --> Display previous command
				if (keyCode == VK_UP) {

					// Get index
					historyDispIndex = GetHistoryCurrentDispIndex() - 1;

					// If current index is 0
					if (historyDispIndex < 0)
						return true;
				}
				// [Down] arrow key --> Display next command
				else if (keyCode == VK_DOWN) {

					// Get index
					historyDispIndex = GetHistoryCurrentDispIndex() + 1;

					// If current index exceeded limit
					if (historyDispIndex >= GetDebugCommandHistoryCount())
						return true;
				}

				// Display debug command
				DispDebugCommandHistory(historyDispIndex);
				return true;
			}
		}
	}

	// Handle right mouse click for DebugTest edit view
	else if (messagePtr->message == WM_RBUTTONDOWN ||
		messagePtr->message == WM_RBUTTONUP ||
		messagePtr->message == WM_RBUTTONDBLCLK) {

		// Get clicked point
		POINT pt;
		pt.x = GET_X_LPARAM(messagePtr->lParam);
		pt.y = GET_Y_LPARAM(messagePtr->lParam);

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
	return SDialog::PreTranslateMessage(messagePtr);
}

/**
 * @brief	Send debug command to main parts of program
 * @param	receivedWndHandle - Receive window handle
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
	int commandLength = FormatDebugCommand(debugCommand);

	// If debug command is empty, do not send
	if (commandLength <= 0)
		return false;

	// Prepare params
	WPARAM wParam = MAKE_WPARAM_STRING(debugCommand);
	LPARAM lParam = MAKE_LPARAM_STRING(debugCommand.getString());

	// Send debug command message to parent window
	this->notifyParent(SM_APP_DEBUG_COMMAND, wParam, lParam);

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
	debugViewPtr_ = (CEdit*)GetDlgItem(IDC_DEBUGTESTV2_VIEW);
	debugCommandInputPtr_ = (CEdit*)GetDlgItem(IDC_DEBUGTESTV2_INPUT);

	// Set DebugScreen font & background color
	if (IsDebugScreenValid()) {

		if (!CreateDebugScreenFont() || !CreateDebugScreenBrush())
			return false;

		// Set font
		debugViewPtr_->SetFont(debugScreenFontPtr_);
		debugCommandInputPtr_->SetFont(debugScreenFontPtr_);

		// Force redraw
		debugViewPtr_->Invalidate();
		debugCommandInputPtr_->Invalidate();
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
	if (debugScreenFontPtr_ == NULL) {
		debugScreenFontPtr_ = new CFont();
		if (debugScreenFontPtr_ == NULL) {
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
	bool returnFlag = false;
	if (debugScreenFontPtr_ != NULL) {
		returnFlag = debugScreenFontPtr_->CreateFontIndirect(&lf);
	}

	return returnFlag;
}

/**
 * @brief	Initialize brush for adjust DebugScreen color
 * @param	None
 * @return	true/false
 */
bool CDebugTestV2Dlg::CreateDebugScreenBrush(void)
{
	// Initialization
	if (debugScreenBrushPtr_ == NULL) {
		debugScreenBrushPtr_ = new CBrush();
		if (debugScreenBrushPtr_ == NULL) {
			// Trace error
			TRACE_ERROR("Error: Failed to create DebugScreen brush!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
	}

	// Create brush
	bool returnFlag = false;
	if (debugScreenBrushPtr_ != NULL) {
		returnFlag = debugScreenBrushPtr_->CreateSolidBrush(Color::Black);
	}

	return returnFlag;
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

		RECT clientRect;
		this->GetClientRect(&clientRect);

		// DebugScreen position
		int xPos = clientRect.left;
		int nDBViewYPos = clientRect.top;
		int nDBCmdInputYPos = clientRect.bottom - debugCmdInputHeight;

		// DebugScreen size
		int width = clientRect.right - clientRect.left;
		int nDBViewHeight = nDBCmdInputYPos - nDBViewYPos;
		int nDBCmdInputHeight = debugCmdInputHeight;

		// Set DebugScreen position
		GetDebugView()->SetWindowPos(NULL, xPos, nDBViewYPos, width, nDBViewHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
		GetDebugCommandInput()->SetWindowPos(NULL, xPos, nDBCmdInputYPos, width, nDBCmdInputHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
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
	CMenu menuDebugTest, * contextMenuPtr;
	menuDebugTest.LoadMenu(IDR_MENU_DEBUGTEST_CONTEXT);
	contextMenuPtr = menuDebugTest.GetSubMenu(0);
	if (contextMenuPtr == NULL)
		return false;

	// Check DebugTest edit view validity
	if (!IsDebugScreenValid())
		return false;

	// Modify menu items
	for (int menuItem = 0; menuItem < contextMenuPtr->GetMenuItemCount(); menuItem++) {
		// Get menu item ID
		unsigned itemId = contextMenuPtr->GetMenuItemID(menuItem);
		// Menu "Copy" item
		if (itemId == IDM_DEBUGTEST_COPY) {
			// If currently not selecting any text
			int selStart, selEnd;
			GetDebugView()->GetSel(selStart, selEnd);
			// Start and end selection index are equal
			// means not selecting anything 
			if (selStart == selEnd) {
				// Disable menu item
				contextMenuPtr->EnableMenuItem(menuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
		// Menu "Paste" item
		else if (itemId == IDM_DEBUGTEST_PASTE) {
			// Check if clipboard content available in text format
			bool isClipboardTextAvailable = IsClipboardFormatAvailable(CF_TEXT);
			// If not available
			if (isClipboardTextAvailable != true) {
				// Disable menu item
				contextMenuPtr->EnableMenuItem(menuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
		// Menu "Previous command" item
		else if (itemId == IDM_DEBUGTEST_DISP_PREVCOMMAND) {
			// If debug command history is empty
			// or it is currently displaying first command
			if ((IsDebugCommandHistoryEmpty()) || (GetHistoryCurrentDispIndex() == 0)) {
				// Disable menu item
				contextMenuPtr->EnableMenuItem(menuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
		// Menu "Next command" item
		else if (itemId == IDM_DEBUGTEST_DISP_NEXTCOMMAND) {
			// If debug command history is empty
			// or it is currently displaying last command
			if ((IsDebugCommandHistoryEmpty()) ||
				(GetHistoryCurrentDispIndex() >= (GetDebugCommandHistoryCount() - 1))) {
				// Disable menu item
				contextMenuPtr->EnableMenuItem(menuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
		// Menu "Clear buffer" item
		else if (itemId == IDM_DEBUGTEST_CLEAR_BUFFER) {
			// If DebugTest buffer screen is empty
			if (bufferString_.isEmpty()) {
				// Disable menu item
				contextMenuPtr->EnableMenuItem(menuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
	}

	// Show menu
	POINT cursorPoint;
	GetCursorPos(&cursorPoint);
	unsigned flags = TPM_LEFTALIGN | TPM_TOPALIGN;
	bool result = contextMenuPtr->TrackPopupMenu(flags, cursorPoint.x, cursorPoint.y, (CWnd*)this, NULL);

	return result;
}

/**
 * @brief	Re-format debug command and return its length
 * @param	debugCommand - Debug command (IN & OUT)
 * @return	int - Length of debug command
 */
int CDebugTestV2Dlg::FormatDebugCommand(String& debugCommand) const
{
	// If debug command is empty, do nothing
	if (debugCommand.isEmpty())
		return 0;

	// Remove prefix
	int nSrcLength = debugCommand.getLength();
	if (isCommandPrefixEnabled_ == true) {
		nSrcLength -= wcslen(debugCommandPrefix);
		String tempString = debugCommand.right(nSrcLength);
		debugCommand = tempString;
	}

	// Remove leading/trailing spaces
	debugCommand.trim();

	// Initialize a temporary string buffer
	nSrcLength = debugCommand.getLength();
	std::vector<wchar_t> tempBuff(nSrcLength + 1);

	// Remove invalid characters
	for (int index = 0; index < nSrcLength; index++) {

		wchar_t ch = debugCommand.getAt(index);
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
	debugCommand.empty();
	debugCommand.setString(tempBuff.data());

	// Remove leading/trailing spaces again
	debugCommand.trim();

	// Return the debug command's new length
	return debugCommand.getLength();
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
	if (isCommandPrefixEnabled_ == true) {
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
	bufferString_ = Constant::String::Empty;
	GetDebugView()->SetWindowText(bufferString_);

	// Backup buffer
	BackupDebugViewBuffer();
}

/**
 * @brief	Add a string line to debug screen
 * @param	lineString - String line
 * @param	newLine   - Whether to add a new empty line
 * @return	None
 */
void CDebugTestV2Dlg::AddLine(const wchar_t* lineString, bool newLine /* = true */)
{
	// If buffer not empty
	if (!bufferString_.isEmpty()) {
		// Get end of buffer character
		int bufferLength = bufferString_.getLength();
		TCHAR lastCharacter = bufferString_.getAt(bufferLength - 1);

		// If end of buffer is not an endline
		if (lastCharacter != Constant::Char::Return && lastCharacter != Constant::Char::EndLine) {
			// Add an endline first
			bufferString_.append(Constant::String::NewLine);
		}
	}

	// Add string line
	if (IS_NOT_EMPTY_STRING(lineString)) {
		bufferString_.append(lineString);
	}

	// Re-check the end of buffer character
	if (newLine == true) {
		int bufferLength = bufferString_.getLength();
		TCHAR lastCharacter = bufferString_.getAt(bufferLength - 1);

		// If end of buffer is not an endline
		if (lastCharacter != Constant::Char::Return && lastCharacter != Constant::Char::EndLine) {
			// Add a new empty line
			bufferString_.append(Constant::String::NewLine);
		}
	}
}

/**
 * @brief	Update debug screen display
 * @param	isSeekToEnd	  - Move cursor to end of view
 * @param	notifyParent - Notify to parent window about display update
 * @return	None
 */
void CDebugTestV2Dlg::UpdateDisplay(bool isSeekToEnd /* = false */, bool notifyParent /* = true */)
{
	// Get debug edit view
	CEdit* pDebugView = GetDebugView();
	if (!IsDebugViewValid())
		return;

	// Update display text
	pDebugView->SetWindowText(bufferString_);
	pDebugView->Invalidate();

	// Move to end
	if (isSeekToEnd == true) {
		pDebugView->SetSel(static_cast<DWORD>(-1));
	}

	// Notify to parent window about display update
	if (notifyParent == true) {
		this->notifyParent(SM_WND_DEBUGOUTPUT_DISP, NULL, NULL);
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
		commandHistoryList_.push_back(commandString);

		// Not currently displaying history
		if (!IsCurrentlyDispHistory()) {
			int count = GetDebugCommandHistoryCount();
			SetHistoryCurrentDispIndex(count);
		}
	}

	// Return new history item count
	return GetDebugCommandHistoryCount();
}

/**
 * @brief	Display history command by index
 * @param	historyIndex - History index
 * @return	None
 */
void CDebugTestV2Dlg::DispDebugCommandHistory(int historyIndex)
{
	// If debug command history is empty, do nothing
	if (IsDebugCommandHistoryEmpty())
		return;

	// Check index validity
	if ((historyIndex < 0) && (historyIndex >= GetDebugCommandHistoryCount()))
		return;

	// Get command at index
	String commandString = commandHistoryList_.at(historyIndex);
	if (commandString.isEmpty())
		return;

	// Check if DebugTest edit view is available and focused
	if (IsDebugCommandInputFocused()) {

		// Display command
		ClearDebugCommandInput(commandString);

		// Update current displaying history index
		SetHistoryCurrentDispIndex(historyIndex);

		// Set currently displaying history flag
		SetCurrentlyDispHistoryState(true);
	}
}
