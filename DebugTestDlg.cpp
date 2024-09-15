
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		DebugTestDlg.cpp
//		Description:	Source file for DebugTest dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2024.07.13:		Create new
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
// Default min/max size

#define DEFAULT_MIN_WIDTH	720
#define DEFAULT_MIN_HEIGHT	480
#define DEFAULT_MAX_WIDTH	1600
#define DEFAULT_MAX_HEIGHT	900


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
	m_strBuffer = DEF_STRING_EMPTY;
	m_strBufferBak = DEF_STRING_EMPTY;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CDebugTestDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CDebugTestDlg::~CDebugTestDlg()
{
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


BEGIN_MESSAGE_MAP(CDebugTestDlg, SDialog)
	ON_EN_CHANGE(IDC_DEBUGTEST_EDITVIEW, &CDebugTestDlg::OnDebugViewChange)
	ON_MESSAGE(SM_APP_DEBUGOUTPUT,		 &CDebugTestDlg::OnDebugOutput)
	ON_MESSAGE(SM_WND_DEBUGVIEWCLRSCR,	 &CDebugTestDlg::OnDebugViewClear)
	ON_WM_DESTROY()
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
	CString strDlgTitle;
	strDlgTitle.LoadString(IDS_APP_DEBUGTESTDLG_TITLE);
	this->SetWindowText(strDlgTitle);

	// Get DebugTest edit view
	m_pDebugEditView = (CEdit*)GetDlgItem(IDC_DEBUGTEST_EDITVIEW);
	if (m_pDebugEditView == NULL)
		return FALSE;

	RECT rcClient;
	this->GetClientRect(&rcClient);

	// Set edit view position
	int nXPos = rcClient.left, nYPos = rcClient.top;
	int nWidth = rcClient.right - rcClient.left;
	int nHeight = rcClient.bottom - rcClient.top;
	m_pDebugEditView->SetWindowPos(NULL, nXPos, nYPos, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOZORDER);

	// Clear buffer
	ClearViewBuffer();

	return TRUE;
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
	EndDialog(IDCANCEL);
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
	if (m_pDebugEditView == NULL) {
		m_pDebugEditView = (CEdit*)GetDlgItem(IDC_DEBUGTEST_EDITVIEW);
		if (m_pDebugEditView == NULL)
			return;
	}

	RECT rcClient;
	this->GetClientRect(&rcClient);

	// Set edit view position/size
	int nXPos = rcClient.left, nYPos = rcClient.top;
	nWidth = rcClient.right - rcClient.left;
	nHeight = rcClient.bottom - rcClient.top;
	m_pDebugEditView->SetWindowPos(NULL, nXPos, nYPos, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDebugViewChange
//	Description:	Handle event when debug contents changed
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::OnDebugViewChange(void)
{
	// Check debug view pointer validity
	if (m_pDebugEditView == NULL)
		return;

	// Update buffer content
	m_pDebugEditView->GetWindowText(m_strBuffer);

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
		return (LRESULT)0;

	// Format log
	CString strDebugLog;
	strDebugLog.Format(_T(">> %s"), (LPCTSTR)lParam);

	AddLine(strDebugLog);

	// Display log and move cursor to end
	UpdateDisplay(TRUE);

	// Backup buffer
	BackupDebugViewBuffer();

	return (LRESULT)1;
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
	return (LRESULT)1;
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
	// Handle key pressed event for DebugTest view
	if (pMsg->message == WM_KEYDOWN) {
		// Get current window that has focus
		CWnd* pFocusWnd = GetFocus();

		// Check if focus belongs to edit control
		if (pFocusWnd == GetDlgItem(IDC_DEBUGTEST_EDITVIEW)) {
			// Get pressed key
			DWORD dwKey = pMsg->wParam;

			// If [Enter] key is pressed
			if (dwKey == VK_RETURN) {
				//Send debug command
				SendDebugCommand((AfxGetMainWnd()->m_hWnd));
			}
			// If [Backspace] or [Delete] keys are pressed
			else if ((dwKey == VK_BACK) || (dwKey == VK_DELETE)) {
				// Only allow erasing inputted content
				if (m_strBuffer.Compare(m_strBufferBak) == 0)
					return TRUE;
			}
			// If arrow keys are pressed
			else if ((dwKey == VK_UP) || (dwKey == VK_DOWN) ||
					(dwKey == VK_LEFT) || (dwKey == VK_RIGHT)) {
				// Block --> Do nothing
				return TRUE;
			}
		}
	}

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

void CDebugTestDlg::SendDebugCommand(HWND hRcvWnd)
{
	// Get buffer length
	int nBufferLength = m_strBuffer.GetLength();
	int nBufferBakLength = m_strBufferBak.GetLength();

	// If buffer length did not increase, do not send
	if (nBufferLength <= nBufferBakLength)
		return;

	// Backup buffer
	BackupDebugViewBuffer();

	// Prepare debug command content
	CString strDebugCommand = m_strBuffer;
	strDebugCommand.Delete(0, nBufferBakLength);
	FormatDebugCommand(strDebugCommand);

	// Send debug command
	WPARAM wParam = (WPARAM)strDebugCommand.GetLength();
	LPARAM lParam = (LPARAM)strDebugCommand.GetBuffer();
	::PostMessage(hRcvWnd, SM_APP_DEBUGCOMMAND, wParam, lParam);
	strDebugCommand.ReleaseBuffer();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	BackupDebugViewBuffer
//	Description:	Backup debug view screen buffer content
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::BackupDebugViewBuffer(void)
{
	// Backup buffer
	m_strBufferBak = m_strBuffer;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	BackupDebugViewBuffer
//	Description:	Backup debug view screen buffer content
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CDebugTestDlg::FormatDebugCommand(CString& strDebugCommand)
{
	// If debug command is empty, do nothing
	if (strDebugCommand.IsEmpty())
		return;

	// Format debug command
	strDebugCommand.Trim();
	strDebugCommand.Remove(DEF_CHAR_RETURN);
	strDebugCommand.Remove(DEF_CHAR_ENDLINE);
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
	if (m_pDebugEditView == NULL)
		return;

	// Clear buffer
	m_strBuffer = DEF_STRING_EMPTY;
	m_pDebugEditView->SetWindowText(m_strBuffer);

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
	// If buffer not empty, add an endline first
	if (!m_strBuffer.IsEmpty()) {
		m_strBuffer += DEF_STRING_NEWLINEWRET;
	}

	// Add string line
	m_strBuffer += lpszString;
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
	// Update display text
	m_pDebugEditView->SetWindowText(m_strBuffer);

	// Move to end
	if (bSeekToEnd == TRUE) {
		m_pDebugEditView->SetSel(-1);
	}
}

