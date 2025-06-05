/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		DebugTestDlgV2.cpp
//		Description:	Source file for DebugTestV2 dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2025.06.02:		Create new for version 3.2
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Dialogs/DebugTestV2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace AppCore;


//////////////////////////////////////////////////////////////////////////
//
// Define macros and constants for DebugTest functions
//
//////////////////////////////////////////////////////////////////////////

// Default min/max size
#define DEFAULT_MIN_WIDTH				840
#define DEFAULT_MIN_HEIGHT				500
#define DEFAULT_MAX_WIDTH				1600
#define DEFAULT_MAX_HEIGHT				900

// Default Debug command input height
#define DEBUG_COMMAND_INPUT_HEIGHT		50

// Define constants
const wchar_t* debugCommandPrefix = _T(">>> ");


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CDebugTestV2Dlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDebugTestV2Dlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CDebugTestV2Dlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

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
	m_bDispCommandPrefix = TRUE;

	// Debug command history
	m_bCurDispHistory = FALSE;
	m_nHistoryCurIndex = 0;
	m_astrCommandHistory.clear();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CDebugTestV2Dlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize DebugTest dialog
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestV2Dlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Prevent flickering on startup
	this->ShowWindow(SW_HIDE);

	// Set dialog title
	this->SetCaptionFromResource(IDS_APP_DEBUGTESTDLG_TITLE);

	// Get DebugScreen (DebugView & Debug command input)
	BOOL bRet = InitDebugScreen();
	if (bRet == FALSE) {
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

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClose
//	Description:	Close dialog
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestV2Dlg::OnClose()
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

void CDebugTestV2Dlg::OnDestroy()
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

void CDebugTestV2Dlg::OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo)
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
//	Description:	Set DebugScreen background and text color
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

HBRUSH CDebugTestV2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hBrush = SDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_STATIC)
	{
		// Text color: Yellow
		pDC->SetTextColor(COLOR_YELLOW);

		// Background color: Black
		pDC->SetBkColor(COLOR_BLACK);

		// Use our custom background brush
		if (m_pDebugScreenBrush != NULL)
			return *m_pDebugScreenBrush;
	}

	return hBrush;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSize
//	Description:	Resizelog
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestV2Dlg::OnSize(UINT nType, int nWidth, int nHeight)
{
	// Implement base class method
	SDialog::OnSize(nType, nWidth, nHeight);

	// Get DebugScreen
	if (!IsDebugScreenValid()) {
		BOOL bRet = InitDebugScreen();
		if (bRet == FALSE)
			return;
	}

	// Refresh DebugScreen
	RefreshDebugScreen(Refresh::ScreenSize);
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

LRESULT CDebugTestV2Dlg::OnDebugOutput(WPARAM wParam, LPARAM lParam)
{
	// Check argument validity
	if ((wParam == 0) || (lParam == NULL))
		return LRESULT(RESULT_FAILED);

	// Format debug output log string
	CString strDebugOutputLog = LPARAM_TO_STRING(lParam);

	// Add debug output string
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
//	Function name:	OnDebugCmdNoReply
//	Description:	Handle event when a debug command has been processed 
//					but there's no reply
//  Arguments:		wParam - First param
//					lParam - Second param
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CDebugTestV2Dlg::OnDebugCmdNoReply(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Add an empty new line
	AddLine(Constant::String::Empty, FALSE);

	// Display log and move cursor to end
	UpdateDisplay(TRUE, FALSE);

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

LRESULT CDebugTestV2Dlg::OnDebugViewClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Clear buffer
	ClearDebugViewBuffer();

	return LRESULT(RESULT_SUCCESS);
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

LRESULT CDebugTestV2Dlg::OnShowDialog(WPARAM wParam, LPARAM /*lParam*/)
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
		if (IsDebugCommandInputFocused() == TRUE) {
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PreTranslateMessage
//	Description:	Pre-translate message
//  Arguments:		pMsg - Default
//  Return value:	BOOL
//
//////////////////////////////////////////////////////////////////////////

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
				return TRUE;
			}

			// Copy (Ctrl + C)
			else if (dwKey == 0x43) {
				PostMessage(WM_COMMAND, IDM_DEBUGTEST_COPY, NULL);
				return TRUE;
			}

			// Paste (Ctrl + V)
			else if (dwKey == 0x56) {
				PostMessage(WM_COMMAND, IDM_DEBUGTEST_PASTE, NULL);
				return TRUE;
			}

			return TRUE;
		}

		// If the Debug command input has the focus
		else if (IsDebugCommandInputFocused()) {

			// If [Enter] key is pressed
			if (dwKey == VK_RETURN) {

				// Send debug command
				SendDebugCommand();
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
			return FALSE;

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

BOOL CDebugTestV2Dlg::SendDebugCommand(void)
{
	// Check Debug command input validity
	if (!IsDebugCommandInputValid())
		return FALSE;

	// Backup buffer
	BackupDebugViewBuffer();

	// Get the debug command line
	CString strDebugCommand;
	GetDebugCommandInput()->GetWindowText(strDebugCommand);

	// Clear Debug command input
	ClearDebugCommandInput();

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
//	Function name:	InitDebugView
//	Description:	Initialize the DebugTest view pointer
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestV2Dlg::InitDebugScreen(void)
{
	// If it has already been initialized, do nothing
	if (IsDebugScreenValid())
		return TRUE;

	// Initialize
	m_pDebugView = (CEdit*)GetDlgItem(IDC_DEBUGTESTV2_VIEW);
	m_pDebugCommandInput = (CEdit*)GetDlgItem(IDC_DEBUGTESTV2_INPUT);

	// Set DebugScreen font & background color
	if (IsDebugScreenValid()) {

		if (!CreateDebugScreenFont() || !CreateDebugScreenBrush())
			return FALSE;

		// Set font
		m_pDebugView->SetFont(m_pDebugScreenFont);
		m_pDebugCommandInput->SetFont(m_pDebugScreenFont);

		// Force redraw
		m_pDebugView->Invalidate();
		m_pDebugCommandInput->Invalidate();
	}

	return IsDebugScreenValid();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CreateDebugScreenFont
//	Description:	Initialize font for DebugScreen
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestV2Dlg::CreateDebugScreenFont(void)
{
	// Initialization
	if (m_pDebugScreenFont == NULL) {
		m_pDebugScreenFont = new CFont();
		if (m_pDebugScreenFont == NULL) {
			// Trace error
			TRACE_ERROR("Error: Failed to create DebugScreen font!!!");
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
	if (m_pDebugScreenFont != NULL) {
		bRet = m_pDebugScreenFont->CreateFontIndirect(&lf);
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CreateDebugScreenBrush
//	Description:	Initialize brush for adjust DebugScreen color
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestV2Dlg::CreateDebugScreenBrush(void)
{
	// Initialization
	if (m_pDebugScreenBrush == NULL) {
		m_pDebugScreenBrush = new CBrush();
		if (m_pDebugScreenBrush == NULL) {
			// Trace error
			TRACE_ERROR("Error: Failed to create DebugScreen brush!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return FALSE;
		}
	}

	// Create brush
	BOOL bRet = FALSE;
	if (m_pDebugScreenBrush != NULL) {
		bRet = m_pDebugScreenBrush->CreateSolidBrush(COLOR_BLACK);
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RefreshDebugScreen
//	Description:	Refresh and update DebugScreen
//  Arguments:		nFlag - Refresh flag
//	Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestV2Dlg::RefreshDebugScreen(int nFlag)
{
	// Check DebugScreen validity
	if (!IsDebugScreenValid())
		return FALSE;

	if (nFlag & Refresh::ScreenSize) {

		RECT rcClient;
		this->GetClientRect(&rcClient);

		// DebugScreen position
		int nXPos = rcClient.left;
		int nDBViewYPos = rcClient.top;
		int nDBCmdInputYPos = rcClient.bottom - DEBUG_COMMAND_INPUT_HEIGHT;

		// DebugScreen size
		int nWidth = rcClient.right - rcClient.left;
		int nDBViewHeight = nDBCmdInputYPos - nDBViewYPos;
		int nDBCmdInputHeight = DEBUG_COMMAND_INPUT_HEIGHT;

		// Set DebugScreen position
		GetDebugView()->SetWindowPos(NULL, nXPos, nDBViewYPos, nWidth, nDBViewHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
		GetDebugCommandInput()->SetWindowPos(NULL, nXPos, nDBCmdInputYPos, nWidth, nDBCmdInputHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ShowDebugScreenContextMenu
//	Description:	Show DebugScreen context menu
//  Arguments:		None
//	Return value:	BOOL - Show menu successfully or failed
//
//////////////////////////////////////////////////////////////////////////

BOOL CDebugTestV2Dlg::ShowDebugScreenContextMenu(void)
{
	// Prepare menu
	CMenu menuDebugTest, * pContextMenu;
	menuDebugTest.LoadMenu(IDR_MENU_DEBUGTEST_CONTEXT);
	pContextMenu = menuDebugTest.GetSubMenu(0);
	if (pContextMenu == NULL)
		return FALSE;

	// Check DebugTest edit view validity
	if (!IsDebugScreenValid())
		return FALSE;

	// Modify menu items
	for (int nMenuItem = 0; nMenuItem < pContextMenu->GetMenuItemCount(); nMenuItem++) {
		// Get menu item ID
		UINT nItemID = pContextMenu->GetMenuItemID(nMenuItem);
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

int CDebugTestV2Dlg::FormatDebugCommand(CString& strDebugCommand)
{
	// If debug command is empty, do nothing
	if (strDebugCommand.IsEmpty())
		return 0;

	// Remove prefix
	int nSrcLength = strDebugCommand.GetLength();
	if (m_bDispCommandPrefix == TRUE) {
		nSrcLength -= _tcslen(debugCommandPrefix);
		CString strTemp = strDebugCommand.Right(nSrcLength);
		strDebugCommand = strTemp;
	}

	// Remove leading/trailing spaces
	strDebugCommand.Trim();

	// Initialize a temporary string buffer
	nSrcLength = strDebugCommand.GetLength();
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

	// Remove leading/trailing spaces again
	strDebugCommand.Trim();

	// Clean-up temporary string buffer
	delete[] lpszBuffTemp;

	// Return the debug command's new length
	return strDebugCommand.GetLength();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ClearDebugCommandInput
//	Description:	Clear and re-initialize Debug command input buffer
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestV2Dlg::ClearDebugCommandInput(LPCTSTR lpszCmdBuff /* = Constant::String::Empty */)
{
	if (!IsDebugCommandInputValid())
		return;

	// Re-initialize Debug command buffer
	CString strCmdBuff = Constant::String::Empty;
	if (m_bDispCommandPrefix == TRUE) {
		strCmdBuff = debugCommandPrefix;
	}
	if (IS_NOT_EMPTY_STRING(lpszCmdBuff)) {
		strCmdBuff += lpszCmdBuff;
	}

	// Update display
	GetDebugCommandInput()->SetWindowText(strCmdBuff);

	// Move to end
	GetDebugCommandInput()->SetSel(static_cast<DWORD>(-1));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ClearDebugViewBuffer
//	Description:	Clear debug view screen buffer content
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddLine
//	Description:	Add a string line to debug screen
//  Arguments:		lpszString - String line
//					bNewLine   - Whether to add a new empty line
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestV2Dlg::AddLine(LPCTSTR lpszString, BOOL bNewLine /* = TRUE */)
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

void CDebugTestV2Dlg::UpdateDisplay(BOOL bSeekToEnd /* = FALSE */, BOOL bNotifyParent /* = TRUE */)
{
	// Get debug edit view
	CEdit* pDebugView = GetDebugView();
	if (!IsDebugViewValid())
		return;

	// Update display text
	pDebugView->SetWindowText(m_strBuffer);
	pDebugView->Invalidate();

	// Move to end
	if (bSeekToEnd == TRUE) {
		pDebugView->SetSel(static_cast<DWORD>(-1));
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

size_t CDebugTestV2Dlg::AddDebugCommandHistory(LPCTSTR lpszCommand)
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

void CDebugTestV2Dlg::DispDebugCommandHistory(int nHistoryIndex)
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
	if (IsDebugCommandInputFocused()) {

		// Display command
		ClearDebugCommandInput(strCommand);

		// Update current displaying history index
		SetHistoryCurrentDispIndex(nHistoryIndex);

		// Set currently displaying history flag
		SetCurrentlyDispHistoryState(TRUE);
	}
}
