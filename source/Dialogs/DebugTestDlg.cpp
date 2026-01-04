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
constexpr const int kDefaultMinWidth = 840;
constexpr const int kDefaultMinHeight = 500;
constexpr const int kDefaultMaxWidth = 1600;
constexpr const int kDefaultMaxHeight = 900;

// Output format
constexpr const wchar_t* kDebugOutputFormat = L">> %s";


// Implement methods for CDebugTestDlg
IMPLEMENT_DYNAMIC(CDebugTestDlg, SDialog)


/**
 * @brief	Constructor
 */
CDebugTestDlg::CDebugTestDlg() : SDialog(IDD_DEBUGTEST_DLG)
{
	// Edit view
	debugEditViewPtr_ = NULL;
	debugViewFontPtr_ = NULL;
	debugViewBrushPtr_ = NULL;

	// Buffer content
	bufferString_ = Constant::String::Empty;
	backupBufferString_ = Constant::String::Empty;

	// Debug command history
	isCurrentlyDisplayHistory_ = false;
	currentHistoryIndex_ = 0;
	commandHistoryList_.clear();
}

/**
 * @brief	Destructor
 */
CDebugTestDlg::~CDebugTestDlg()
{
	// Clean-up debug command history
	clearDebugCommandHistory();

	// Delete font
	if (debugViewFontPtr_ != NULL) {
		debugViewFontPtr_->DeleteObject();
		delete debugViewFontPtr_;
		debugViewFontPtr_ = NULL;
	}

	// Delete brush
	if (debugViewBrushPtr_ != NULL) {
		debugViewBrushPtr_->DeleteObject();
		delete debugViewBrushPtr_;
		debugViewBrushPtr_ = NULL;
	}
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CDebugTestDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


// CDebugTestDlg dialog message map
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
	this->setCaptionFromResource(IDS_APP_DEBUGTESTDLG_TITLE);

	// Get DebugTest edit view
	bool returnFlag = initDebugEditView(IDC_DEBUGTEST_EDITVIEW);
	if (returnFlag == false) {
		TRACE_ERROR("Error: Debug edit view initialization failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return returnFlag;
	}

	RECT clientRect;
	this->GetClientRect(&clientRect);

	// Set edit view position
	int xPos = clientRect.left, yPos = clientRect.top;
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;
	getDebugEditView()->SetWindowPos(NULL, xPos, yPos, width, height, SWP_SHOWWINDOW | SWP_NOZORDER);

	// Bring window to top
	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	this->SetForegroundWindow();

	// Clear buffer
	clearViewBuffer();

	// Init debug command history
	clearDebugCommandHistory();

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
void CDebugTestDlg::OnGetMinMaxInfo(MINMAXINFO* minMaxInfoPtr)
{
	// Fix min size
	minMaxInfoPtr->ptMinTrackSize.x = kDefaultMinWidth;
	minMaxInfoPtr->ptMinTrackSize.y = kDefaultMinHeight;

	// Fix max size
	minMaxInfoPtr->ptMaxTrackSize.x = kDefaultMaxWidth;
	minMaxInfoPtr->ptMinTrackSize.y = kDefaultMaxHeight;

	// Default
	SDialog::OnGetMinMaxInfo(minMaxInfoPtr);
}

/**
 * @brief	Set background and text color
 * @param	Default
 * @return	None
 */
HBRUSH CDebugTestDlg::OnCtlColor(CDC* pDC, CWnd* windowPtr, UINT nCtlColor)
{
	HBRUSH hBrush = SDialog::OnCtlColor(pDC, windowPtr, nCtlColor);

	if (nCtlColor == CTLCOLOR_EDIT)
	{
		// Text color: Yellow
		pDC->SetTextColor(Color::Yellow);

		// Background color: Black
		pDC->SetBkColor(Color::Black);

		// Use our custom background brush
		if (debugViewBrushPtr_ != NULL)
			return *debugViewBrushPtr_;
	}

	return hBrush;
}

/**
 * @brief	Resize dialog
 * @param	Default
 * @return	None
 */
void CDebugTestDlg::OnSize(UINT nType, int width, int height)
{
	// Implement base class method
	SDialog::OnSize(nType, width, height);

	// Get DebugTest edit view
	if (!isDebugEditViewValid()) {
		bool returnFlag = initDebugEditView(IDC_DEBUGTEST_EDITVIEW);
		if (returnFlag == false)
			return;
	}

	RECT clientRect;
	this->GetClientRect(&clientRect);

	// Set edit view position/size
	int xPos = clientRect.left, yPos = clientRect.top;
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;
	getDebugEditView()->SetWindowPos(NULL, xPos, yPos, width, height, SWP_SHOWWINDOW | SWP_NOZORDER);
}

/**
 * @brief	Handle event when debug view edit contents changed
 * @param	None
 * @return	None
 */
void CDebugTestDlg::OnDebugViewEditChange(void)
{
	// Check debug view pointer validity
	if (!isDebugEditViewValid())
		return;

	// Update buffer content
	const int kBuffLength = getDebugEditView()->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(kBuffLength + 1);
	getDebugEditView()->GetWindowText(tempBuff.data(), kBuffLength + 1);
	bufferString_ = tempBuff.data();

	// If buffer length did not increase
	int bufferLength = bufferString_.getLength();
	int backupBufferLength = backupBufferString_.getLength();
	if (bufferLength <= backupBufferLength) {
		// Backup buffer
		backupDebugViewBuffer();
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
	debugOutputLogStr.format(kDebugOutputFormat, LPARAM_TO_STRING(lParam));

	// Add debug output string
	addLine(debugOutputLogStr);

	// Display log and move cursor to end
	updateDisplay(true);

	// Backup buffer
	backupDebugViewBuffer();

	// Reset currently displaying history flag
	setCurrentlyDispHistoryState(false);

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
	addLine(Constant::String::Empty, false);

	// Display log and move cursor to end
	updateDisplay(true, false);

	// Backup buffer
	backupDebugViewBuffer();

	// Reset currently displaying history flag
	setCurrentlyDispHistoryState(false);

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
	clearViewBuffer();

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
BOOL CDebugTestDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Process commands
	switch (LOWORD(wParam))
	{
	case IDM_DEBUGTEST_COPY:
		// Copy selection text to clipboard
		getDebugEditView()->Copy();
		break;

	case IDM_DEBUGTEST_PASTE:
		// Check if focus belongs to DebugTest edit view
		if (isDebugEditViewFocus() == true) {
			// Get caret position
			int caretPosition = getCaretPosition();
			// Get line index by caret position
			int caretLineIndex = getDebugEditView()->LineFromChar(caretPosition);
			// If the caret position is not in the last line
			if (caretLineIndex != (getDebugEditView()->GetLineCount() - 1)) {
				// Move caret to end of DebugTest edit view
				getDebugEditView()->SetSel(static_cast<DWORD>(-1));
			}
			// Paste clipboard text to DebugTest edit view
			getDebugEditView()->Paste();
		} break;

	case IDM_DEBUGTEST_DISP_PREVCOMMAND:
		// Display previous command
		dispDebugCommandHistory(getHistoryCurrentDispIndex() - 1);
		break;

	case IDM_DEBUGTEST_DISP_NEXTCOMMAND:
		// Display next command
		dispDebugCommandHistory(getHistoryCurrentDispIndex() + 1);
		break;

	case IDM_DEBUGTEST_CLEAR_BUFFER:
		// Clear view buffer
		clearViewBuffer();
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
BOOL CDebugTestDlg::PreTranslateMessage(MSG* messagePtr)
{
	// Handle key pressed event for DebugTest edit view
	if (messagePtr->message == WM_KEYDOWN) {

		// Check if focus belongs to DebugTest edit view
		if (isDebugEditViewFocus() == true) {

			// Get pressed key
			DWORD keyCode = messagePtr->wParam;

			// Get caret position
			int caretPosition = getCaretPosition();

			// Get line index by caret position
			int caretLineIndex = getDebugEditView()->LineFromChar(caretPosition);

			// If the caret position is not in the last line
			if (caretLineIndex != (getDebugEditView()->GetLineCount() - 1)) {

				// If "Ctrl+C" keys are pressed
				if ((keyCode == 0x43) && (IS_PRESSED(VK_CONTROL))) {
					// If currently selecting a text
					int selStart, selEnd;
					getDebugEditView()->GetSel(selStart, selEnd);

					if (selStart != selEnd) {
						// Copy the current selection
						getDebugEditView()->Copy();
						return true;
					}
				}
				else {
					// Block --> Do nothing
					return true;
				}
			}

			// If [Enter] key is pressed
			if (keyCode == VK_RETURN) {

				//Send debug command
				bool returnFlag = SendDebugCommand();

				// If debug command is sent, block the break-line
				if (returnFlag == true) return returnFlag;
			}
			// If [Backspace] or [Delete] keys are pressed
			else if ((keyCode == VK_BACK) || (keyCode == VK_DELETE)) {

				// Only allow erasing inputted content
				if (bufferString_.compare(backupBufferString_) == 0)
					return true;

				// [Backspace] key --> Can not delete empty line
				if (keyCode == VK_BACK) {

					// Get line begin character index
					size_t lineBeginIndex = getDebugEditView()->LineIndex(caretLineIndex);

					// Get last line index
					size_t lastLineIndex = getDebugEditView()->GetLineCount() - 1;

					// If the caret position is not in the last line
					// or is in the beginning of last line
					if ((caretLineIndex != lastLineIndex) ||
						((caretPosition == lineBeginIndex) && (caretLineIndex == lastLineIndex))) {
						// Block --> Do nothing
						return true;
					}
				}
			}
			// If "Ctrl+A" keys are pressed
			else if ((keyCode == 0x41) && (IS_PRESSED(VK_CONTROL))) {
				// Block --> Do nothing
				return true;
			}
			// If the [Up/Down] arrow keys are pressed
			else if ((keyCode == VK_UP) || (keyCode == VK_DOWN)) {

				// If debug command history is empty
				if (isDebugCommandHistoryEmpty())
					return true;

				// Get command history display index
				int historyDispIndex = 0;

				// [Up] arrow key --> Display previous command
				if (keyCode == VK_UP) {

					// Get index
					historyDispIndex = getHistoryCurrentDispIndex() - 1;

					// If current index is 0
					if (historyDispIndex < 0)
						return true;
				}
				// [Down] arrow key --> Display next command
				else if (keyCode == VK_DOWN) {

					// Get index
					historyDispIndex = getHistoryCurrentDispIndex() + 1;

					// If current index exceeded limit
					if (historyDispIndex >= getDebugCommandHistoryCount())
						return true;
				}

				// Display debug command
				dispDebugCommandHistory(historyDispIndex);
				return true;
			}
			// If the [Left/Right] arrow key is pressed
			else if (keyCode == VK_LEFT) {

				// Get line begin character index
				size_t lineBeginIndex = getDebugEditView()->LineIndex(caretLineIndex);

				// Get last line index
				size_t lastLineIndex = getDebugEditView()->GetLineCount() - 1;

				// If the caret position is not in the last line
				// or is in the beginning of last line
				if ((caretLineIndex != lastLineIndex) ||
					((caretPosition == lineBeginIndex) && (caretLineIndex == lastLineIndex))) {
					// Block --> Do nothing
					return true;
				}
			}
		}
	}
	// Handle right mouse click for DebugTest edit view
	else if (messagePtr->message == WM_RBUTTONDOWN ||
			 messagePtr->message == WM_RBUTTONUP ||
			 messagePtr->message == WM_RBUTTONDBLCLK) {

		// Get clicked point
		Point pt(GET_X_LPARAM(messagePtr->lParam), GET_Y_LPARAM(messagePtr->lParam));

		// Get DebugTest edit view
		if (!isDebugEditViewValid())
			return 0;

		// Get the editbox rect
		RECT debugEditViewRect;
		getDebugEditView()->GetWindowRect(&debugEditViewRect);
		ScreenToClient(&debugEditViewRect);

		// If clicked point is inside the editbox area
		if (((pt._x > debugEditViewRect.left) && (pt._x < debugEditViewRect.right)) &&
			((pt._y > debugEditViewRect.top) && (pt._y < debugEditViewRect.bottom))) {
			// Show DebugTest edit view menu
			showDebugTestEditViewMenu();
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
bool CDebugTestDlg::SendDebugCommand(void)
{
	// Check DebugTest edit view validity
	if (!isDebugEditViewValid())
		return false;

	// Backup buffer
	backupDebugViewBuffer();

	// Get the debug command line length
	int currentLine = (getDebugEditView()->GetLineCount() - 1);
	int lineIndex = getDebugEditView()->LineIndex(currentLine);
	int lineLength = getDebugEditView()->LineLength(lineIndex);

	// Get the debug command line
	std::wstring tempBuff{};
	tempBuff.resize(lineLength + 1);
	int bufferLength = getDebugEditView()->GetLine(currentLine, &tempBuff[0], lineLength + 1);
	tempBuff.resize(bufferLength);
	String debugCommand = tempBuff;

	// Re-format the debug command
	int commandLength = formatDebugCommand(debugCommand);

	// If debug command is empty, do not send
	if (commandLength <= 0)
		return false;

	// Prepare params
	WPARAM wParam = MAKE_WPARAM_STRING(debugCommand);
	LPARAM lParam = MAKE_LPARAM_STRING(debugCommand.getString());
	
	// Send debug command message to parent window
	this->notifyParent(SM_APP_DEBUG_COMMAND, wParam, lParam);

	// Update debug command history
	addDebugCommandHistory(debugCommand);

	return true;
}


//////////////////////////////////////////////////////////////////////////
// Protected methods

/**
 * @brief	Initialize the DebugTest edit view pointer
 * @param	controlId - Dialog control ID
 * @return	true/false
 */
bool CDebugTestDlg::initDebugEditView(unsigned controlId)
{
	// If it has already been initialized, do nothing
	if (isDebugEditViewValid())
		return true;

	// Initialize
	debugEditViewPtr_ = (CEdit*)GetDlgItem(controlId);

	// Set DebugView font & background color
	if (isDebugEditViewValid()) {
		if (createDebugViewFont()) {
			debugEditViewPtr_->SetFont(debugViewFontPtr_);
		}
		if (!createDebugViewBrush())
			return false;
	}

	return isDebugEditViewValid();
}

/**
 * @brief	Initialize font for DebugTest edit view
 * @param	None
 * @return	true/false
 */
bool CDebugTestDlg::createDebugViewFont(void)
{
	// Initialization
	if (debugViewFontPtr_ == NULL) {
		debugViewFontPtr_ = new CFont();
		if (debugViewFontPtr_ == NULL) {
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
	bool returnFlag = false;
	if (debugViewFontPtr_ != NULL) {
		returnFlag = debugViewFontPtr_->CreateFontIndirect(&lf);
	}

	return returnFlag;
}

/**
 * @brief	Initialize brush for adjust DebugTest edit view color
 * @param	None
 * @return	true/false
 */
bool CDebugTestDlg::createDebugViewBrush(void)
{
	// Initialization
	if (debugViewBrushPtr_ == NULL) {
		debugViewBrushPtr_ = new CBrush();
		if (debugViewBrushPtr_ == NULL) {
			// Trace error
			TRACE_ERROR("Error: Failed to create brush!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
	}

	// Create brush
	bool returnFlag = false;
	if (debugViewBrushPtr_ != NULL) {
		returnFlag = debugViewBrushPtr_->CreateSolidBrush(Color::Black);
	}

	return returnFlag;
}

/**
 * @brief	Get the caret's current position
 * @param	None
 * @return	int - Caret position
 */
int CDebugTestDlg::getCaretPosition(void)
{
	// Check DebugTest edit view validity
	if (!isDebugEditViewValid())
		return INT_INVALID;

	// Get caret position
	int selStart, selEnd;
	getDebugEditView()->GetSel(selStart, selEnd);
	return selStart;
}

/**
 * @brief	Show DebugTest edit view context menu
 * @param	None
 * @param	Return value:	bool - Show menu successfully or failed
 */
bool CDebugTestDlg::showDebugTestEditViewMenu(void)
{
	// Prepare menu
	CMenu menuDebugTest, *contextMenuPtr;
	menuDebugTest.LoadMenu(IDR_MENU_DEBUGTEST_CONTEXT);
	contextMenuPtr = menuDebugTest.GetSubMenu(0);
	if (contextMenuPtr == NULL)
		return false;

	// Check DebugTest edit view validity
	if (!isDebugEditViewValid())
		return false;

	// Modify menu items
	for (int menuItem = 0; menuItem < contextMenuPtr->GetMenuItemCount(); menuItem++) {
		// Get menu item ID
		unsigned itemId = contextMenuPtr->GetMenuItemID(menuItem);
		// Menu "Copy" item
		if (itemId == IDM_DEBUGTEST_COPY) {
			// If currently not selecting any text
			int selStart, selEnd;
			getDebugEditView()->GetSel(selStart, selEnd);
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
			if ((isDebugCommandHistoryEmpty()) || (getHistoryCurrentDispIndex() == 0)) {
				// Disable menu item
				contextMenuPtr->EnableMenuItem(menuItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
		}
		// Menu "Next command" item
		else if (itemId == IDM_DEBUGTEST_DISP_NEXTCOMMAND) {
			// If debug command history is empty
			// or it is currently displaying last command
			if ((isDebugCommandHistoryEmpty()) || 
				(getHistoryCurrentDispIndex() >= (getDebugCommandHistoryCount() - 1))) {
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
int CDebugTestDlg::formatDebugCommand(String& debugCommand)
{
	// If debug command is empty, do nothing
	if (debugCommand.isEmpty())
		return 0;

	// Remove whitespace
	debugCommand.trim();

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
	debugCommand.empty();
	debugCommand.setString(tempNewBuffer);

	// Return the debug command's new length
	return debugCommand.getLength();
}

/**
 * @brief	Clear debug view screen buffer content
 * @param	None
 * @return	None
 */
void CDebugTestDlg::clearViewBuffer(void)
{
	if (!isDebugEditViewValid())
		return;

	// Clear buffer
	bufferString_ = Constant::String::Empty;
	getDebugEditView()->SetWindowText(bufferString_);

	// Backup buffer
	backupDebugViewBuffer();
}

/**
 * @brief	Add a string line to debug screen
 * @param	lineString - String line
 * @param	newLine   - Whether to add a new empty line
 * @return	None
 */
void CDebugTestDlg::addLine(const wchar_t* lineString, bool newLine /* = true */)
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
void CDebugTestDlg::updateDisplay(bool isSeekToEnd /* = false */, bool notifyParent /* = true */)
{
	// Get debug edit view
	CEdit* debugEditViewPtr = getDebugEditView();
	if (!isDebugEditViewValid())
		return;

	// Update display text
	debugEditViewPtr->SetWindowText(bufferString_);
	debugEditViewPtr->Invalidate();

	// Move to end
	if (isSeekToEnd == true) {
		debugEditViewPtr->SetSel(static_cast<DWORD>(-1));
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
size_t CDebugTestDlg::addDebugCommandHistory(const wchar_t* commandString)
{
	// Only add if input command is not empty
	if (IS_NOT_EMPTY_STRING(commandString)) {
		commandHistoryList_.push_back(commandString);

		// Not currently displaying history
		if (!isCurrentlyDispHistory()) {
			int count = getDebugCommandHistoryCount();
			setHistoryCurrentDispIndex(count);
		}
	}
	
	// Return new history item count
	return getDebugCommandHistoryCount();
}

/**
 * @brief	Display history command by index
 * @param	historyIndex - History index
 * @return	None
 */
void CDebugTestDlg::dispDebugCommandHistory(int historyIndex)
{
	// If debug command history is empty, do nothing
	if (isDebugCommandHistoryEmpty())
		return;

	// Check index validity
	if ((historyIndex < 0) && (historyIndex >= getDebugCommandHistoryCount()))
		return;

	// Get command at index
	String commandString = commandHistoryList_.at(historyIndex);
	if (commandString.isEmpty())
		return;

	// Check if DebugTest edit view is available and focused
	if (isDebugEditViewFocus()) {
		// Get last (current) line index
		int currentLine = getDebugEditView()->GetLineCount() - 1;

		// Get the character index of the start of the specified line
		int lineStart = getDebugEditView()->LineIndex(currentLine);
		if (lineStart != -1)
		{
			// Get the length of the line
			int lineLength = getDebugEditView()->LineLength(lineStart);

			// Select the line
			getDebugEditView()->SetSel(lineStart, lineStart + lineLength);

			// Replace the selected line with the command line
			getDebugEditView()->ReplaceSel(commandString);

			// Update current displaying history index
			setHistoryCurrentDispIndex(historyIndex);

			// Set currently displaying history flag
			setCurrentlyDispHistoryState(true);
		}
	}
}

