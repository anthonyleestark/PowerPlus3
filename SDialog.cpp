
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		SDialog.h
//		Project name:	Stupid Framework
//		Description:	Custom base class for dialogs (source file)
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.07.06:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SDialog.h"
#include "PowerPlus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace PairFuncs;
using namespace CoreFuncs;


//////////////////////////////////////////////////////////////////////////
//
//	Define default properties
//
//////////////////////////////////////////////////////////////////////////

#define DEFAULT_MARGIN_HORZ		10
#define DEFAULT_MARGIN_VERT		10

//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for SDialog
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(SDialog, CDialogEx)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SDialog
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

SDialog::SDialog() : CDialogEx()
{
	// Parent window
	m_pParentWnd = NULL;

	// Dialog special flags
	m_bReadOnlyMode = FALSE;
	m_bChangeFlag = FALSE;
	m_bLockState = FALSE;
	m_bForceClose = FALSE;
	m_bUseEnter = TRUE;
	m_bUseEscape = TRUE;

	// Properties set flags
	m_bSetDlgTitle = FALSE;
	m_bSetBkgrdColor = FALSE;
	m_bSetTextColor = FALSE;

	m_bSetMinSize = FALSE;
	m_bSetMaxSize = FALSE;

	m_bSetTopMost = FALSE;
	m_bSetInitSound = FALSE;

	// Lock state exception IDs
	m_arrLockExceptionIDList.RemoveAll();

	// Dialog position
	m_ptDlgPosition = CPoint(0,0);

	// Dialog alignment
	m_nDlgAlign = SDA_LEFTALIGN | SDA_TOPALIGN;

	// Dialog size
	m_szDlgSize = CSize();
	m_szDefaultSize = CSize();

	m_ptMinSize = CPoint(0, 0);
	m_ptMaxSize = CPoint(0, 0);

	// Dialog margin
	m_ptMarginTopLeft = CPoint(DEFAULT_MARGIN_HORZ, DEFAULT_MARGIN_VERT);
	m_ptMarginBottomRight = CPoint(DEFAULT_MARGIN_HORZ, DEFAULT_MARGIN_VERT);

	// Other properties
	m_strDlgTitle = DEF_STRING_EMPTY;
	m_hDefaultIcon = NULL;

	m_pBkgrdBrush = NULL;
	m_clBkgrdColor = DEF_COLOR_WHITE;
	m_clTextColor = DEF_COLOR_BLACK;
}

SDialog::SDialog(UINT nIDTemplate, CWnd* pParent /* = NULL */) : CDialogEx(nIDTemplate, pParent)
{
	// Parent window
	m_pParentWnd = pParent;

	// Dialog special flags
	m_bReadOnlyMode = FALSE;
	m_bChangeFlag = FALSE;
	m_bLockState = FALSE;
	m_bForceClose = FALSE;
	m_bUseEnter = TRUE;
	m_bUseEscape = TRUE;

	// Properties set flags
	m_bSetDlgTitle = FALSE;
	m_bSetBkgrdColor = FALSE;
	m_bSetTextColor = FALSE;

	m_bSetMinSize = FALSE;
	m_bSetMaxSize = FALSE;

	m_bSetTopMost = FALSE;
	m_bSetInitSound = FALSE;

	// Lock state exception IDs
	m_arrLockExceptionIDList.RemoveAll();

	// Dialog position
	m_ptDlgPosition = CPoint(0, 0);

	// Dialog alignment
	m_nDlgAlign = SDA_LEFTALIGN | SDA_TOPALIGN;

	// Dialog size
	m_szDlgSize = CSize();
	m_szDefaultSize = CSize();

	m_ptMinSize = CPoint(0, 0);
	m_ptMaxSize = CPoint(0, 0);

	// Dialog margin
	m_ptMarginTopLeft = CPoint(DEFAULT_MARGIN_HORZ, DEFAULT_MARGIN_VERT);
	m_ptMarginBottomRight = CPoint(DEFAULT_MARGIN_HORZ, DEFAULT_MARGIN_VERT);

	// Other properties
	m_strDlgTitle = DEF_STRING_EMPTY;
	m_hDefaultIcon = NULL;

	m_pBkgrdBrush = NULL;
	m_clBkgrdColor = DEF_COLOR_WHITE;
	m_clTextColor = DEF_COLOR_BLACK;
}

SDialog::SDialog(LPCTSTR lpszTemplateName, CWnd* pParent /* = NULL */) : CDialogEx(lpszTemplateName, pParent)
{
	// Parent window
	m_pParentWnd = pParent;

	// Dialog special flags
	m_bReadOnlyMode = FALSE;
	m_bChangeFlag = FALSE;
	m_bLockState = FALSE;
	m_bForceClose = FALSE;
	m_bUseEnter = TRUE;
	m_bUseEscape = TRUE;

	// Properties set flags
	m_bSetDlgTitle = FALSE;
	m_bSetBkgrdColor = FALSE;
	m_bSetTextColor = FALSE;

	m_bSetMinSize = FALSE;
	m_bSetMaxSize = FALSE;

	m_bSetTopMost = FALSE;
	m_bSetInitSound = FALSE;

	// Lock state exception IDs
	m_arrLockExceptionIDList.RemoveAll();

	// Properties
	m_ptDlgPosition = CPoint(0, 0);

	// Dialog alignment
	m_nDlgAlign = SDA_LEFTALIGN | SDA_TOPALIGN;

	// Dialog size
	m_szDlgSize = CSize();
	m_szDefaultSize = CSize();

	m_ptMinSize = CPoint(0, 0);
	m_ptMaxSize = CPoint(0, 0);

	// Dialog margin
	m_ptMarginTopLeft = CPoint(DEFAULT_MARGIN_HORZ, DEFAULT_MARGIN_VERT);
	m_ptMarginBottomRight = CPoint(DEFAULT_MARGIN_HORZ, DEFAULT_MARGIN_VERT);

	// Other properties
	m_strDlgTitle = DEF_STRING_EMPTY;
	m_hDefaultIcon = NULL;

	m_pBkgrdBrush = NULL;
	m_clBkgrdColor = DEF_COLOR_WHITE;
	m_clTextColor = DEF_COLOR_BLACK;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~SDialog
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

SDialog::~SDialog()
{
	// Clear lock state exception IDs list
	ResetLockStateExceptionList();

	// Properties data cleanup
	::DeleteObject(m_hDefaultIcon);
	if (m_pBkgrdBrush != NULL) {
		delete m_pBkgrdBrush;
		m_pBkgrdBrush = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void SDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////
//
//	SDialog dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(SDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &SDialog::OnOK)
	ON_BN_CLICKED(IDCANCEL, &SDialog::OnCancel)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnOK
//	Description:	Default method for [OK] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OnOK()
{
	CDialogEx::OnOK();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCancel
//	Description:	Default method for [Cancel] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OnCancel()
{
	CDialogEx::OnCancel();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Default method for dialog initialization
//  Arguments:		None
//  Return value:	Default
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set dialog title
	if (m_bSetDlgTitle == TRUE) {
		this->SetWindowText(m_strDlgTitle);
	}

	// Get dialog rectangle
	CRect rectDlg;
	this->GetWindowRect(&rectDlg);

	// Backup default size
	m_szDefaultSize.cx = (rectDlg.right - rectDlg.left);
	m_szDefaultSize.cy = (rectDlg.bottom - rectDlg.top);

	// If size is not set, use default
	if ((m_szDlgSize.cx <= 0) && (m_szDlgSize.cy <= 0)) {
		m_szDlgSize.cx = m_szDefaultSize.cx;
		m_szDlgSize.cy = m_szDefaultSize.cy;
	}

	// Resize dialog
	if ((m_szDlgSize.cx != m_szDefaultSize.cx) || (m_szDlgSize.cy != m_szDefaultSize.cy)) {
		// Set width
		if (m_szDlgSize.cx > -1) {
			rectDlg.right = (rectDlg.left + m_szDlgSize.cx);
		}
		// Set height
		if (m_szDlgSize.cy > -1) {
			rectDlg.bottom = (rectDlg.top + m_szDlgSize.cy);
		}
	}
	
	// Center dialog
	this->MoveWindow(&rectDlg);
	CWnd* pParent = this->GetParent();
	if (pParent == NULL) {
		pParent = CWnd::GetDesktopWindow();
	}
	this->CenterWindow(pParent);

	// Background color
	if (m_bSetBkgrdColor == TRUE) {
		// Initialize
		if (m_pBkgrdBrush == NULL) {
			m_pBkgrdBrush = new CBrush;
		}
		if (m_pBkgrdBrush != NULL) {
			// Create background color brush
			m_pBkgrdBrush->CreateSolidBrush(m_clBkgrdColor);
		}
	}

	// Set top-most
	if (m_bSetTopMost == TRUE) {
		// Set window position
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	// Init sound
	if (m_bSetInitSound == TRUE) {
		MessageBeep(0xFFFFFFFF);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClose
//	Description:	Default method for dialog closing
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OnClose()
{
	// Get dialog ID
	WPARAM wParam = (WPARAM)GetDialogID();

	// If parent window is set
	if (m_pParentWnd != NULL) {
		// Notify parent window about dialog closing
		m_pParentWnd->PostMessage(SCM_DLGCLOSE_NOTIFY, wParam, NULL);
	}
	else {
		// Notify to app main thread
		::PostMessage(NULL, SCM_DLGCLOSE_NOTIFY, wParam, NULL);
	}

	CDialogEx::OnClose();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDestroy
//	Description:	Default method for dialog destroying
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OnDestroy()
{
	// Get dialog ID
	WPARAM wParam = (WPARAM)GetDialogID();

	// If parent window is set
	if (m_pParentWnd != NULL) {
		// Notify parent window about dialog destroying
		m_pParentWnd->PostMessage(SCM_DLGDESTROY_NOTIFY, wParam, NULL);
	}
	else {
		// Notify to app main thread
		::PostMessage(NULL, SCM_DLGDESTROY_NOTIFY, wParam, NULL);
	}

	CDialogEx::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnActivate
//	Description:	Default method for dialog activate message handling
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnGetMinMaxInfo
//	Description:	Dialog get min/max info handler
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo)
{
	// Fix min/max size
	if (m_bSetMinSize == TRUE) {
		pMinMaxInfo->ptMinTrackSize = m_ptMinSize;
	}
	if (m_bSetMaxSize == TRUE) {
		pMinMaxInfo->ptMaxTrackSize = m_ptMaxSize;
	}

	CDialogEx::OnGetMinMaxInfo(pMinMaxInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PreTranslateMessage
//	Description:	Default method for pre-translate message
//  Arguments:		pMsg - Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) 
		{
		case VK_ESCAPE:
			if (m_bUseEscape != TRUE) {
				// Do not use Escape button
				return TRUE;
			}
			break;
		case VK_RETURN:
			if (m_bUseEnter != TRUE) {
				// Do not use Enter button
				return TRUE;
			}
			break;
		default:
			break;
		}
	}

	// Default
	return CDialogEx::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PreDestroyDialog
//	Description:	Execute some actions before destroying dialog
//  Arguments:		None
//  Return value:	int
//
//////////////////////////////////////////////////////////////////////////

int SDialog::PreDestroyDialog()
{
	// TODO: Deriver this function for custom actions

	return DEF_RESULT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetParentWnd
//	Description:	Get current dialog's parent window
//  Arguments:		None
//  Return value:	CWnd*
//
//////////////////////////////////////////////////////////////////////////

CWnd* SDialog::GetParentWnd()
{
	return m_pParentWnd;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetParentWnd
//	Description:	Get parent window for current dialog
//  Arguments:		pParentWnd - Parent window
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetParentWnd(CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsParentWndAvailable
//	Description:	Check if parent window (pointer) is available
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::IsParentWndAvailable()
{
	return (m_pParentWnd != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddDialogStyle
//	Description:	Add dialog extended styles
//  Arguments:		dwAddStyle - Styles to add
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::AddDialogStyle(DWORD dwAddStyle)
{
	DWORD dwStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	dwStyle |= dwAddStyle;
	LONG lRet = SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, dwStyle);
	return (lRet != 0);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveDialogStyle
//	Description:	Remove dialog extended styles
//  Arguments:		dwRemoveStyle - Styles to remove
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::RemoveDialogStyle(DWORD dwRemoveStyle)
{
	DWORD dwStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	dwStyle &= ~dwRemoveStyle;
	LONG lRet = SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, dwStyle);
	return (lRet != 0);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDialogID
//	Description:	Get dialog ID
//  Arguments:		None
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT SDialog::GetDialogID()
{
	return m_nIDHelp;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetReadOnlyMode
//	Description:	Get/set read-only mode
//  Arguments:		bReadOnly - Read-only mode flag
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::GetReadOnlyMode()
{
	return m_bReadOnlyMode;
}

void SDialog::SetReadOnlyMode(BOOL bReadOnly)
{
	m_bReadOnlyMode = bReadOnly;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetLockState
//	Description:	Get/set current item lock state
//  Arguments:		bIsLock - Lock or not
//  Return value:	BOOL - Current state
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::GetLockState()
{
	return m_bLockState;
}

void SDialog::SetLockState(BOOL bIsLock)
{
	m_bLockState = bIsLock;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetUseEnter
//	Description:	Set use Enter button flag
//  Arguments:		bUseEnter - Flag value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetUseEnter(BOOL bUseEnter)
{
	m_bUseEnter = bUseEnter;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetUseEscape
//	Description:	Set use Escape button flag
//  Arguments:		bUseEscape - Flag value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetUseEscape(BOOL bUseEscape)
{
	m_bUseEscape = bUseEscape;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddLockStateException
//	Description:	Add a control ID to lock state exception list
//  Arguments:		nID - Control ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::AddLockStateException(UINT nID)
{
	// Loop through all list and find if item existed in list
	int nItemNum = m_arrLockExceptionIDList.GetSize();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		if (m_arrLockExceptionIDList.GetAt(nIndex) == nID) {
			// No need to add
			return;
		}
	}

	// Add to list
	m_arrLockExceptionIDList.Add(nID);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveLockStateException
//	Description:	Remove a control ID out of lock state exception list
//  Arguments:		nID - Control ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::RemoveLockStateException(UINT nID)
{
	// Loop through all list and remove item
	int nItemNum = m_arrLockExceptionIDList.GetSize();
	for (int nIndex = (nItemNum - 1); nIndex >= 0; nIndex--) {
		if (m_arrLockExceptionIDList.GetAt(nIndex) == nID) {
			m_arrLockExceptionIDList.RemoveAt(nIndex);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ResetLockStateExceptionList
//	Description:	Remove all lock state exception list
//  Arguments:		nID - Control ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::ResetLockStateExceptionList()
{
	m_arrLockExceptionIDList.RemoveAll();
	m_arrLockExceptionIDList.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetDialogAlign
//	Description:	Get/set dialog alignments
//  Arguments:		nAlign - Dialog alignments
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT SDialog::GetDialogAlign(void)
{
	return m_nDlgAlign;
}

void SDialog::SetDialogAlign(UINT nAlign)
{
	m_nDlgAlign = nAlign;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetDialogPosition
//	Description:	Get/set dialog position
//  Arguments:		lpPosition - Dialog position (anchor point) (OUT)
//					ptPosition - Dialog position (anchor point) (IN)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetDialogPosition(LPPOINT lpPosition)
{
	// Get position
	lpPosition->x = m_ptDlgPosition.x;
	lpPosition->y = m_ptDlgPosition.y;
}

void SDialog::SetDialogPosition(POINT ptPosition)
{
	// Set position
	m_ptDlgPosition.x = ptPosition.x;
	m_ptDlgPosition.y = ptPosition.y;

	// Move dialog
	MoveDialog(m_ptDlgPosition);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	MoveDialog
//	Description:	Move dialog position and return new rectangle
//  Arguments:		ptPosition - Dialog position (anchor point)
//					lpNewRect  - New dialog rectangle (OUT)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::MoveDialog(POINT ptPosition, LPRECT lpNewRect /* = NULL */)
{
	// Get current dialog rectangle
	CRect rcCurPos;
	this->GetWindowRect(&rcCurPos);

	// Get dialog alignments
	UINT nAlign = GetDialogAlign();

	// Calculate moving delta by alignments
	int dx = 0, dy = 0;

	// --> Calculate horizontal delta
	if (nAlign & SDA_LEFTALIGN) {
		// Move left rect
		dx = ptPosition.x - rcCurPos.left;
	}
	else if (nAlign & SDA_RIGHTALIGN) {
		// Move right rect
		dx = ptPosition.x - rcCurPos.right;
	}
	else if (nAlign & SDA_HCENTERALIGN) {
		// Move center rect
		int nHCenter = rcCurPos.right - rcCurPos.left;
		dx = ptPosition.x - nHCenter;
	}

	// --> Calculate vertical delta
	if (nAlign & SDA_TOPALIGN) {
		// Move top rect
		dy = ptPosition.y - rcCurPos.top;
	}
	else if (nAlign & SDA_BOTTOMALIGN) {
		// Move bottom rect
		dy = ptPosition.y - rcCurPos.bottom;
	}
	else if (nAlign & SDA_VCENTERALIGN) {
		// Move center rect
		int nVCenter = rcCurPos.bottom - rcCurPos.top;
		dy = ptPosition.y - nVCenter;
	}

	// Move dialog
	MoveDialog(dx, dy, lpNewRect);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	MoveDialog
//	Description:	Move dialog position as given delta and return new rect
//  Arguments:		dx		  - Horizontal delta
//					dy		  - Vertical delta
//					lpNewRect - New dialog rectangle (OUT)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::MoveDialog(int dx, int dy, LPRECT lpNewRect /* = NULL */)
{
	// Get current dialog rectangle
	CRect rcCurPos;
	this->GetWindowRect(&rcCurPos);

	// Shift rectangle
	if (dx != 0) {
		// Move horizontal direction --> Shift X value
		rcCurPos.left += dx;
		rcCurPos.right += dx;
	}
	if (dy != 0) {
		// Move vertical direction --> Shift Y value
		rcCurPos.top += dy;
		rcCurPos.bottom += dy;
	}

	// Move dialog
	if ((dx != 0) || (dy != 0)) {
		// Top-left point
		int x = rcCurPos.left;
		int y = rcCurPos.top;
		// Set dialog position
		BOOL bRet = this->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		// If moving successfully, update new rectangle 
		if ((bRet == TRUE) && (lpNewRect != NULL)) {
			lpNewRect->left = rcCurPos.left;
			lpNewRect->right = rcCurPos.right;
			lpNewRect->top = rcCurPos.top;
			lpNewRect->bottom = rcCurPos.bottom;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ResizeDialog
//	Description:	Resize dialog (after change size)
//  Arguments:		bCenterDialog - Replace dialog to center
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::ResizeDialog(BOOL bCenterDialog)
{
	// Get current dialog rectangle
	CRect rectDlg;
	this->GetWindowRect(&rectDlg);

	// Set new rectangle
	if ((m_szDlgSize.cx > -1) && (m_szDlgSize.cy > -1)) {
		rectDlg.right = (rectDlg.left + m_szDlgSize.cx);
		rectDlg.bottom = (rectDlg.top + m_szDlgSize.cy);
	}

	// Resize
	SetWindowPos(NULL, 0, 0, (rectDlg.right - rectDlg.left), (rectDlg.bottom - rectDlg.top), SWP_NOMOVE | SWP_NOZORDER);

	// Make sure that the entire dialog box is visible on the screen
	SendMessage(DM_REPOSITION, 0, 0);

	// Center dialog
	if (bCenterDialog == TRUE) {
		CWnd* pParent = this->GetParent();
		if (pParent == NULL) {
			pParent = CWnd::GetDesktopWindow();
		}
		this->CenterWindow(pParent);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ResetDialogSize
//	Description:	Reset dialog size to default
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::ResetDialogSize()
{
	// No default size
	if ((m_szDefaultSize.cx == 0) || (m_szDefaultSize.cy == 0))
		return;

	// Get current size
	CRect rcCurRect;
	this->GetWindowRect(&rcCurRect);

	// If current size is default size, do nothing
	int nCurWidth = (rcCurRect.right - rcCurRect.left);
	int nCurHeight = (rcCurRect.bottom - rcCurRect.top);
	if ((nCurWidth == m_szDefaultSize.cx) && (nCurHeight == m_szDefaultSize.cy))
		return;

	// Reset to default
	m_szDlgSize.cx = m_szDefaultSize.cx;
	m_szDlgSize.cy = m_szDefaultSize.cy;

	// Resize dialog
	ResizeDialog(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetDlgSize
//	Description:	Get/set dialog size
//  Arguments:		lpDlgSize	- Dialog size
//					szDlgSize	- Dialog size
//					nWidth		- Dialog width
//					nHeight		- Dialog height
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetDlgSize(LPSIZE lpDlgSize)
{
	// If size is not set, use default
	if ((m_szDlgSize.cx <= 0) && (m_szDlgSize.cy <= 0)) {
		lpDlgSize->cx = m_szDefaultSize.cx;
		lpDlgSize->cy = m_szDefaultSize.cy;
	}

	lpDlgSize->cx = m_szDlgSize.cx;
	lpDlgSize->cy = m_szDlgSize.cy;
}

void SDialog::SetDlgSize(SIZE szDlgSize)
{
	// Copy data
	m_szDlgSize.cx = szDlgSize.cx;
	m_szDlgSize.cy = szDlgSize.cy;
}

void SDialog::SetDlgSize(int nWidth, int nHeight)
{
	// Create temp size data
	CSize szDlgSizeTemp;
	szDlgSizeTemp.cx = nWidth;
	szDlgSizeTemp.cy = nHeight;

	// Set dialog size
	SetDlgSize(szDlgSizeTemp);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMin/MaxSize
//	Description:	Set dialog min/max size
//  Arguments:		nMinWidth	- Dialog min width
//					nMinHeight  - Dialog min height
//					nMaxWidth	- Dialog max width
//					nMaxHeight	- Dialog max height
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetMinSize(int nMinWidth, int nMinHeight)
{
	// Set min size
	m_ptMinSize = CPoint(nMinWidth, nMinHeight);

	// Set flag
	m_bSetMinSize = TRUE;
}

void SDialog::SetMaxSize(int nMaxWidth, int nMaxHeight)
{
	// Set min size
	m_ptMaxSize = CPoint(nMaxWidth, nMaxHeight);

	// Set flag
	m_bSetMaxSize = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetTopMost
//	Description:	Set dialog top-most flag
//  Arguments:		bTopMost - Flag value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetTopMost(BOOL bTopMost)
{
	m_bSetTopMost = bTopMost;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetInitSound
//	Description:	Set dialog init sound enable flag
//  Arguments:		bInitSound - Flag value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetInitSound(BOOL bInitSound)
{
	m_bSetInitSound = bInitSound;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetTop/Left/Bottom/RightMargin
//	Description:	Set content display margin: top/left/bottom/right
//  Arguments:		nTopMargin	  - Top margin
//					nLeftMargin	  - Left margin
//					nBottomMargin - Bottom margin
//					nRightMargin  - Right margin
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetTopMargin(int nTopMargin)
{
	m_ptMarginTopLeft.y = nTopMargin;
}

void SDialog::SetLeftMargin(int nLeftMargin)
{
	m_ptMarginTopLeft.x = nLeftMargin;
}

void SDialog::SetBottomMargin(int nBottomMargin)
{
	m_ptMarginBottomRight.y = nBottomMargin;
}

void SDialog::SetRightMargin(int nRightMargin)
{
	m_ptMarginBottomRight.x = nRightMargin;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetCenterMargin
//	Description:	Set content display center margin
//  Arguments:		ptMargin - Display margin
//					nHMargin - Horizontal margin
//					nVMargin - Vertical margin
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetCenterMargin(POINT ptMargin)
{
	// Top left
	m_ptMarginTopLeft.x = ptMargin.x;
	m_ptMarginTopLeft.y = ptMargin.y;
	
	// Bottom right
	m_ptMarginBottomRight.x = ptMargin.x;
	m_ptMarginBottomRight.y = ptMargin.y;
}

void SDialog::SetCenterMargin(int nHMargin, int nVMargin)
{
	// Top left
	m_ptMarginTopLeft.x = nHMargin;
	m_ptMarginTopLeft.y = nVMargin;

	// Bottom right
	m_ptMarginBottomRight.x = nHMargin;
	m_ptMarginBottomRight.y = nVMargin;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDisplayArea
//	Description:	Get content display area
//  Arguments:		lpRect - Display area rectangle
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetDisplayArea(LPRECT lpRect)
{
	// Get client rect
	this->GetClientRect(lpRect);

	// Calculate display area with margin
	lpRect->left += m_ptMarginTopLeft.x;
	lpRect->top += m_ptMarginTopLeft.y;
	lpRect->right -= m_ptMarginBottomRight.x;
	lpRect->bottom -= m_ptMarginBottomRight.y;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDisplayArea
//	Description:	Set new display area, update margin and resize dialog
//  Arguments:		rcNewDispArea - New display area rectangle
//					bResizeDialog - Resize dialog
//					bCenter		  - Center rectangle area
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetDisplayArea(RECT rcNewDispArea, BOOL bResizeDialog, BOOL bCenter)
{
	// Get current margin
	POINT ptTopLeft, ptBottomRight;
	this->GetMargin(&ptTopLeft, &ptBottomRight);

	// Horizontal center margin
	BOOL bHorzCenter = FALSE;
	if (ptTopLeft.x == ptBottomRight.x) {
		bHorzCenter = TRUE;
	}

	// Vertical center margin
	BOOL bVertCenter = FALSE;
	if (ptTopLeft.y == ptBottomRight.y) {
		bVertCenter = TRUE;
	}

	// New top-left margin
	POINT ptNewTopLeft;
	ptNewTopLeft.x = ptTopLeft.x;
	ptNewTopLeft.y = ptTopLeft.y;

	// New bottom-right margin
	POINT ptNewBottomRight;
	ptNewBottomRight.x = ptBottomRight.x;
	ptNewBottomRight.y = ptBottomRight.y;

	// Get current client size
	CRect rcCurClient;
	this->GetClientRect(&rcCurClient);
	this->ClientToScreen(&rcCurClient);

	// Get current display area
	CRect rcCurDispArea;
	this->GetDisplayArea(&rcCurDispArea);

	// Calculate current display area size
	CSize szCurDispAreaSize;
	szCurDispAreaSize.cx = (rcCurDispArea.right - rcCurDispArea.left);
	szCurDispAreaSize.cy = (rcCurDispArea.bottom - rcCurDispArea.top);

	// Get current dialog rectangle
	CRect rcCurDlgRect;
	this->GetWindowRect(&rcCurDlgRect);

	// Dialog and client rectangle offset
	CRect rcDlgClientOffset;
	rcDlgClientOffset.left = abs(rcCurDlgRect.left - rcCurClient.left);
	rcDlgClientOffset.top = abs(rcCurDlgRect.top - rcCurClient.top);
	rcDlgClientOffset.right = abs(rcCurDlgRect.right - rcCurClient.right);
	rcDlgClientOffset.bottom = abs(rcCurDlgRect.bottom - rcCurClient.bottom);

	// Get dialog size
	CSize szDlgSize;
	GetDlgSize(&szDlgSize);

	// New dialog rectangle
	CRect rcNewDlgRect;

	// Calculate new display area size
	CSize szNewDispAreaSize;
	szNewDispAreaSize.cx = (rcNewDispArea.right - rcNewDispArea.left);
	szNewDispAreaSize.cy = (rcNewDispArea.bottom - rcNewDispArea.top);

	// Update margin
	if ((bResizeDialog != TRUE) && (bCenter == TRUE)) {
		// Horizontal center margin
		if (bHorzCenter == TRUE) {
			ptNewTopLeft.x = ptTopLeft.x + (szCurDispAreaSize.cx - szNewDispAreaSize.cx);
			ptNewBottomRight.x = ptNewTopLeft.x;
		}
		// Vertical center margin
		if (bVertCenter == TRUE) {
			ptNewTopLeft.y = ptTopLeft.y + (szCurDispAreaSize.cy - szNewDispAreaSize.cy);
			ptNewBottomRight.y = ptNewTopLeft.y;
		}
	}

	// Set new dialog rectangle top-left
	rcNewDlgRect.left = (rcNewDispArea.left - ptNewTopLeft.x) - rcDlgClientOffset.left;
	rcNewDlgRect.top = (rcNewDispArea.top - ptNewTopLeft.y) - rcDlgClientOffset.top;

	// If resize is specified
	if (bResizeDialog == TRUE) {
		// Set new dialog rectangle bottom-right
		rcNewDlgRect.right = (rcNewDispArea.right + ptNewBottomRight.x) + rcDlgClientOffset.right;
		rcNewDlgRect.bottom = (rcNewDispArea.bottom + ptNewBottomRight.y) + rcDlgClientOffset.bottom;

		// Resize dialog
		SetDlgSize((rcNewDlgRect.right - rcNewDlgRect.left), (rcNewDlgRect.bottom - rcNewDlgRect.top));
		ResizeDialog(bCenter);
	}
	else {
		// Reposition following new margin offset
		rcNewDlgRect.left += (ptNewTopLeft.x - ptTopLeft.x);
		rcNewDlgRect.top += (ptNewTopLeft.y - ptTopLeft.y);

		// Set new dialog rectangle bottom-right
		rcNewDlgRect.right = (rcNewDlgRect.left + szDlgSize.cx);
		rcNewDlgRect.bottom = (rcNewDlgRect.top + szDlgSize.cy);

		// Recalculate bottom-right margin
		ptNewBottomRight.x = (rcNewDlgRect.right - rcDlgClientOffset.right) - rcNewDispArea.right;
		ptNewBottomRight.y = (rcNewDlgRect.bottom - rcDlgClientOffset.bottom) - rcNewDispArea.bottom;

		// Reposition dialog
		SetWindowPos(NULL, rcNewDlgRect.left, rcNewDlgRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		// Make sure that the entire dialog box is visible on the screen
		SendMessage(DM_REPOSITION, 0, 0);

		// Center dialog
		if (bCenter == TRUE) {
			CWnd* pParent = this->GetParent();
			if (pParent == NULL) {
				pParent = CWnd::GetDesktopWindow();
			}
			this->CenterWindow(pParent);
		}
	}

	// Save margin update
	SetLeftMargin(ptNewTopLeft.x);
	SetTopMargin(ptNewTopLeft.y);
	SetRightMargin(ptNewBottomRight.x);
	SetBottomMargin(ptNewBottomRight.y);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMargin
//	Description:	Get content display margin
//  Arguments:		ptTopLeft	  - Top left margin
//					ptBottomRight - Bottom right margin
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetMargin(LPPOINT ptTopLeft, LPPOINT ptBottomRight)
{
	// Top left
	ptTopLeft->x = m_ptMarginTopLeft.x;
	ptTopLeft->y = m_ptMarginTopLeft.y;

	// Bottom right
	ptBottomRight->x = m_ptMarginBottomRight.x;
	ptBottomRight->y = m_ptMarginBottomRight.y;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDlgTitle
//	Description:	Get dialog title
//  Arguments:		strDlgTitle - Dialog title
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetDlgTitle(CString& strDlgTitle)
{
	strDlgTitle = m_strDlgTitle;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDlgTitle
//	Description:	Set dialog title
//  Arguments:		lpszDlgTitle - Dialog title
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetDlgTitle(LPCTSTR lpszDlgTitle)
{
	m_strDlgTitle = lpszDlgTitle;
	m_bSetDlgTitle = TRUE;

	// If dialog is already initialized, trigger updating title
	if (::IsWindow(this->m_hWnd)) {
		this->SetWindowText(m_strDlgTitle);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetRcDlgTitle
//	Description:	Set dialog title by resource string ID
//  Arguments:		nResourceStringID - Resource string ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetRcDlgTitle(UINT nResourceStringID)
{
	CString strTemp;
	BOOL bRet = strTemp.LoadString(nResourceStringID);
	ASSERT(!strTemp.IsEmpty());
	if ((bRet == TRUE) && (!strTemp.IsEmpty())) {
		SetDlgTitle(strTemp);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetLangDlgTitle
//	Description:	Set dialog title by language string ID
//  Arguments:		nLangStringID - Language string ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetLangDlgTitle(UINT nLangStringID)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	CString strTemp = GetLanguageString(pAppLang, nLangStringID);
	SetDlgTitle(strTemp);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetIcon
//	Description:	Set icon by resource ID
//  Arguments:		nIconResourceID - Icon resource ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetIcon(UINT nIconResourceID)
{
	// Reset icon
	::DeleteObject(m_hDefaultIcon);
	m_hDefaultIcon = AfxGetApp()->LoadIcon(nIconResourceID);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetIcon
//	Description:	Set icon (same as CDialogEx base method)
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetIcon(HICON hIcon, BOOL bBigIcon)
{
	// Use default method
	CDialogEx::SetIcon(hIcon, bBigIcon);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetBkgrdColor
//	Description:	Get/set background color
//  Arguments:		clBkgrdColor - Background color
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetBkgrdColor(COLORREF& clBkgrdColor)
{
	clBkgrdColor = m_clBkgrdColor;
}

void SDialog::SetBkgrdColor(COLORREF clBkgrdColor)
{
	m_clBkgrdColor = clBkgrdColor;
	m_bSetBkgrdColor = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetTextColor
//	Description:	Get/set text color
//  Arguments:		clTextColor - Text color
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetTextColor(COLORREF& clTextColor)
{
	clTextColor = m_clTextColor;
}

void SDialog::SetTextColor(COLORREF clTextColor)
{
	m_clTextColor = clTextColor;
	m_bSetTextColor = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayMessageBox
//	Description:	Display message box using language string ID
//  Arguments:		nPromptID  - ID of prompt message string
//					nCaptionID - ID of message caption string
//					nStyle	   - Message box style
//  Return value:	int	- Result of message box
//
//////////////////////////////////////////////////////////////////////////

int SDialog::DisplayMessageBox(UINT nPromptID, UINT nCaptionID /* = NULL */, UINT nStyle /* = NULL */)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	CString strMsg = GetLanguageString(pAppLang, nPromptID);
	CString strCaption = ((CPowerPlusApp*)AfxGetApp())->GetAppWindowTitle();
	if (nCaptionID != NULL) {
		CString strTemp = GetLanguageString(pAppLang, nCaptionID);
		if (strTemp != DEF_STRING_NULL)
			strCaption = strTemp;
	}

	// Display message box
	int nResult = DisplayMessageBox(strMsg, strCaption, nStyle);

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayMessageBox
//	Description:	Display message box using language string
//  Arguments:		lpszPromptID - Message string
//					nCaptionID   - ID of message caption string
//					nStyle		 - Message box style
//  Return value:	int	- Result of message box
//
//////////////////////////////////////////////////////////////////////////

int SDialog::DisplayMessageBox(LPCTSTR lpszPrompt, UINT nCaptionID /* = NULL */, UINT nStyle /* = NULL */)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	CString strMsg(lpszPrompt);
	CString strCaption = ((CPowerPlusApp*)AfxGetApp())->GetAppWindowTitle();
	if (nCaptionID != NULL) {
		CString strTemp = GetLanguageString(pAppLang, nCaptionID);
		if (strTemp != DEF_STRING_NULL)
			strCaption = strTemp;
	}

	// Display message box
	int nResult = DisplayMessageBox(strMsg, strCaption, nStyle);

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayMessageBox
//	Description:	Display message box using language string
//  Arguments:		nPromptID     - ID of prompt message string
//					lpszCaptionID - Message caption string
//					nStyle		  - Message box style
//  Return value:	int	- Result of message box
//
//////////////////////////////////////////////////////////////////////////

int SDialog::DisplayMessageBox(UINT nPromptID, LPCTSTR lpszCaption, UINT nStyle /* = NULL */)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	CString strMsg = GetLanguageString(pAppLang, nPromptID);
	CString strCaption(lpszCaption);
	if (strCaption.IsEmpty()) {
		// Use default app window title as messagebox caption
		strCaption = ((CPowerPlusApp*)AfxGetApp())->GetAppWindowTitle();
	}

	// Display message box
	int nResult = DisplayMessageBox(strMsg, strCaption, nStyle);

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayMessageBox
//	Description:	Display message box using language string
//  Arguments:		lpszPromptID  - Message string
//					lpszCaptionID - Message caption string
//					nStyle		  - Message box style
//  Return value:	int	- Result of message box
//
//////////////////////////////////////////////////////////////////////////

int SDialog::DisplayMessageBox(LPCTSTR lpszPrompt, LPCTSTR lpszCaption, UINT nStyle /* = NULL */)
{
	// Display message box
	nStyle |= MB_SYSTEMMODAL;
	return MessageBox(lpszPrompt, lpszCaption, nStyle);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog controls
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetupLanguage()
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		UINT nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDOK:
		case IDCANCEL:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupComboBox
//	Description:	Setup data for combo-boxes
//  Arguments:		nComboID	- ID of combo box
//					ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage)
{
	// TODO: Deriver this function for custom actions
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateItemText
//	Description:	Update control text label
//  Arguments:		nCtrlID		   - ID of control item
//					lpszNewCaption - New caption string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::UpdateItemText(UINT nCtrlID, LPCTSTR lpszNewCaption)
{
	// Get item by ID
	CWnd* pCtrlWnd = this->GetDlgItem(nCtrlID);
	if (pCtrlWnd == NULL)
		return;

	// Update item text
	pCtrlWnd->SetWindowText(lpszNewCaption);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateItemText
//	Description:	Update control text label
//  Arguments:		nCtrlID		  - ID of control item
//					nNewCaptionID - New caption language string ID
//					ptrLanguage   - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::UpdateItemText(UINT nCtrlID , UINT nNewCaptionID /* = NULL */, LANGTABLE_PTR ptrLanguage /* = NULL */)
{
	// Check language package validity
	if (ptrLanguage == NULL) {
		ptrLanguage = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		if (ptrLanguage == NULL)
			return;
	}

	// Get caption language string
	CString strWndText;
	if (nNewCaptionID != NULL) {
		// Get new caption
		strWndText = GetLanguageString(ptrLanguage, nNewCaptionID);
	}
	else {
		// Get its own caption string ID
		strWndText = GetLanguageString(ptrLanguage, nCtrlID);
	}

	// Update item text
	UpdateItemText(nCtrlID, strWndText);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetControlText
//	Description:	Set control text label
//  Arguments:		pCtrlWnd	- Pointer of control window
//					nCtrlID		- ID of control item
//					ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetControlText(CWnd* pCtrlWnd, UINT nCtrlID, LANGTABLE_PTR ptrLanguage /* = NULL */)
{
	// Check control pointer validity
	if (pCtrlWnd == NULL) {
		pCtrlWnd = this->GetDlgItem(nCtrlID);
		if (pCtrlWnd == NULL) 
			return;
	}
	
	// Check language package validity
	if (ptrLanguage == NULL) {
		ptrLanguage = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		if (ptrLanguage == NULL)
			return;
	}

	// Get language string
	CString strWndText = GetLanguageString(ptrLanguage, nCtrlID);
	
	// Set control text
	pCtrlWnd->SetWindowText(strWndText);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableControl
//	Description:	Enable/disable control with corresponding ID
//  Arguments:		nCtrlID - Control ID
//					bEnable - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::EnableControl(UINT nCtrlID, BOOL bEnable)
{
	// If new control state is the same as current state, do nothing
	CWnd* pWndCtrl = GetDlgItem(nCtrlID);
	if (pWndCtrl != NULL) {
		if (pWndCtrl->IsWindowEnabled() == bEnable)
			return;
	}

	// Save app event log if enabled
	UINT nEventID;
	switch (nCtrlID)
	{
	// Buttons
	case IDOK:
	case IDCANCEL:
		nEventID = (bEnable == TRUE) ? LOG_EVENT_BUTTON_ENABLED : LOG_EVENT_BUTTON_DISABLED;
		OutputButtonLog(GetDialogID(), nCtrlID, nEventID);
		break;
	}

	// Enable/disable control
	pWndCtrl->EnableWindow(bEnable);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupDlgItemState
//	Description:	Setup properties and values for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetupDlgItemState()
{
	// TODO: Deriver this function for custom actions
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RefreshDlgItemState
//	Description:	Refresh and update state for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::RefreshDlgItemState()
{
	// TODO: Deriver this function for custom actions
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetChangeFlagValue
//	Description:	Get/set change flag value
//  Arguments:		bChangeFlag - Change flag
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::GetChangeFlagValue()
{
	return m_bChangeFlag;
}

void SDialog::SetChangeFlagValue(BOOL bChangeFlag)
{
	m_bChangeFlag = bChangeFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CheckDataChangeState
//	Description:	Check if data changed
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::CheckDataChangeState()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CheckSettingChangeState
//	Description:	Check if settings changed
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::CheckSettingChangeState()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsForceClosingByRequest
//	Description:	Check dialog force closing flag
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::IsForceClosingByRequest(void)
{
	return m_bForceClose;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RequestCloseDialog
//	Description:	Request current dialog to close
//  Arguments:		None
//  Return value:	LRESULT (0:Success, else:Failed)
//
//////////////////////////////////////////////////////////////////////////

LRESULT SDialog::RequestCloseDialog(void)
{
	// Set force closing flag
	m_bForceClose = TRUE;

	// Default: Close the dialog
	this->PostMessage(WM_CLOSE);

	// Request accepted
	return LRESULT(0);	// ERROR_SUCCESS
}
