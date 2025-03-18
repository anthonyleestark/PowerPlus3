
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		SDialog.h
//		Project name:	Stupid Framework
//		Description:	Custom base class for dialogs (source file)
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.07.06:		Create new
//						<1> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SWinApp.h"
#include "SDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace TableFuncs;
using namespace CoreFuncs;


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

	// Tooltip control
	m_pToolTip = NULL;

	// Dialog control management
	m_pCtrlManager = NULL;

	// Dialog special flags
	m_bReadOnlyMode = FALSE;
	m_bChangeFlag = FALSE;
	m_bLockState = FALSE;
	m_bForceClose = FALSE;
	m_bUseEnter = TRUE;
	m_bUseEscape = TRUE;

	// Properties set flags
	m_bBkgrdColorSet = FALSE;
	m_bTextColorSet = FALSE;
	m_bTopMostSet = FALSE;
	m_bInitSoundSet = FALSE;

	// Lock state exception IDs
	m_paLockExceptionIDList = NULL;

	// Dialog position
	m_ptPosition = POINT_NULL;

	// Dialog alignment
	m_nAlignment = SDA_LEFTALIGN | SDA_TOPALIGN;

	// Dialog size
	m_szRegisterSize = SIZE_NULL;
	m_szDefaultSize = SIZE_NULL;

	// Dialog min/max info
	m_szMinSize = SIZE_UNDEFINED;
	m_szMaxSize = SIZE_UNDEFINED;

	// Dialog client display margin
	m_rcClientMargin = MARGIN_DEFAULT;

	// Other properties
	m_strCaption.Empty();
	m_hDefaultIcon = NULL;
	m_strMsgCaption.Empty();

	m_pBkgrdBrush = NULL;
	m_clBkgrdColor = COLOR_WHITE;
	m_clTextColor = COLOR_BLACK;
}

SDialog::SDialog(UINT nIDTemplate, CWnd* pParentWnd /* = NULL */) : CDialogEx(nIDTemplate, pParentWnd)
{
	// Parent window
	m_pParentWnd = pParentWnd;

	// Tooltip control
	m_pToolTip = NULL;

	// Dialog control management
	m_pCtrlManager = NULL;

	// Dialog special flags
	m_bReadOnlyMode = FALSE;
	m_bChangeFlag = FALSE;
	m_bLockState = FALSE;
	m_bForceClose = FALSE;
	m_bUseEnter = TRUE;
	m_bUseEscape = TRUE;

	// Properties set flags
	m_bBkgrdColorSet = FALSE;
	m_bTextColorSet = FALSE;
	m_bTopMostSet = FALSE;
	m_bInitSoundSet = FALSE;

	// Lock state exception IDs
	m_paLockExceptionIDList = NULL;

	// Dialog position
	m_ptPosition = POINT_NULL;

	// Dialog alignment
	m_nAlignment = SDA_LEFTALIGN | SDA_TOPALIGN;

	// Dialog size
	m_szRegisterSize = SIZE_NULL;
	m_szDefaultSize = SIZE_NULL;

	// Dialog min/max info
	m_szMinSize = SIZE_UNDEFINED;
	m_szMaxSize = SIZE_UNDEFINED;

	// Dialog client display margin
	m_rcClientMargin = MARGIN_DEFAULT;

	// Other properties
	m_strCaption.Empty();
	m_hDefaultIcon = NULL;
	m_strMsgCaption.Empty();

	m_pBkgrdBrush = NULL;
	m_clBkgrdColor = COLOR_WHITE;
	m_clTextColor = COLOR_BLACK;
}

SDialog::SDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd /* = NULL */) : CDialogEx(lpszTemplateName, pParentWnd)
{
	// Parent window
	m_pParentWnd = pParentWnd;

	// Tooltip control
	m_pToolTip = NULL;

	// Dialog control management
	m_pCtrlManager = NULL;

	// Dialog special flags
	m_bReadOnlyMode = FALSE;
	m_bChangeFlag = FALSE;
	m_bLockState = FALSE;
	m_bForceClose = FALSE;
	m_bUseEnter = TRUE;
	m_bUseEscape = TRUE;

	// Properties set flags
	m_bBkgrdColorSet = FALSE;
	m_bTextColorSet = FALSE;
	m_bTopMostSet = FALSE;
	m_bInitSoundSet = FALSE;

	// Lock state exception IDs
	m_paLockExceptionIDList = NULL;

	// Dialog position
	m_ptPosition = POINT_NULL;

	// Dialog alignment
	m_nAlignment = SDA_LEFTALIGN | SDA_TOPALIGN;

	// Dialog size
	m_szRegisterSize = SIZE_NULL;
	m_szDefaultSize = SIZE_NULL;

	// Dialog min/max info
	m_szMinSize = SIZE_UNDEFINED;
	m_szMaxSize = SIZE_UNDEFINED;

	// Dialog client display margin
	m_rcClientMargin = MARGIN_DEFAULT;

	// Other properties
	m_strCaption.Empty();
	m_hDefaultIcon = NULL;
	m_strMsgCaption.Empty();

	m_pBkgrdBrush = NULL;
	m_clBkgrdColor = COLOR_WHITE;
	m_clTextColor = COLOR_BLACK;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~SDialog
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

SDialog::~SDialog()
{
	// Destroy tooltip control window
	if ((m_pToolTip != NULL) && (IsWindow(m_pToolTip->GetSafeHwnd()))) {
		m_pToolTip->DestroyToolTipCtrl();
	}

	// Clear dialog control manager
	if (m_pCtrlManager != NULL) {
		m_pCtrlManager->DeleteAll();
		delete m_pCtrlManager;
		m_pCtrlManager = NULL;
	}

	// Clear lock state exception IDs list
	ResetLockStateExceptionList();
	if (m_paLockExceptionIDList != NULL) {
		delete m_paLockExceptionIDList;
		m_paLockExceptionIDList = NULL;
	}

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
	// Default
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
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDOK, &SDialog::OnOK)
	ON_BN_CLICKED(IDCANCEL, &SDialog::OnCancel)
	ON_WM_ACTIVATE()
	ON_WM_GETMINMAXINFO()
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
	// Default
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
	// Default
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
	// Base class initialization
	CDialogEx::OnInitDialog();

	// If parent window is not set
	if (GetParentWnd() == NULL) {

		// Set top-most active window as its parent
		if (CWnd* pActiveWnd = CWnd::GetActiveWindow()) {
			this->SetParentWnd(pActiveWnd);
		}
		else {
			// There is currently no active window
			// Set desktop window as its parent instead
			this->SetParentWnd(CWnd::GetDesktopWindow());
		}
	}

	// Create and initialize tooltip control
	if (GetToolTipCtrl() == NULL) {
		this->m_pToolTip = new CToolTipCtrl();
		this->m_pToolTip->Create(this);
	}

	// Activate tooltip control
	if (IsToolTipCtrlAvailable()) {
		GetToolTipCtrl()->Activate(TRUE);
	}

	// If dialog custom caption is set
	if (!m_strCaption.IsEmpty()) {

		// Update dialog caption
		this->SetWindowText(m_strCaption);
	}
	else {
		// Use defined caption in resource
		CString strResourceCaption;
		this->GetWindowText(strResourceCaption);
		this->SetCaption(strResourceCaption);
	}

	// Get dialog rectangle
	CRect rectDlg;
	this->GetWindowRect(&rectDlg);

	// Backup default size
	m_szDefaultSize.cx = (rectDlg.right - rectDlg.left);
	m_szDefaultSize.cy = (rectDlg.bottom - rectDlg.top);

	// If dialog size is not registered, use default
	if ((m_szRegisterSize.cx <= 0) && (m_szRegisterSize.cy <= 0)) {
		m_szRegisterSize.cx = m_szDefaultSize.cx;
		m_szRegisterSize.cy = m_szDefaultSize.cy;
	}

	// Resize dialog
	if ((m_szRegisterSize.cx != m_szDefaultSize.cx) || (m_szRegisterSize.cy != m_szDefaultSize.cy)) {
		// Set width
		if (m_szRegisterSize.cx > -1) {
			rectDlg.right = (rectDlg.left + m_szRegisterSize.cx);
		}
		// Set height
		if (m_szRegisterSize.cy > -1) {
			rectDlg.bottom = (rectDlg.top + m_szRegisterSize.cy);
		}
	}
	
	// Center dialog
	this->MoveWindow(&rectDlg);
	this->CenterWindow(GetParentWnd());

	// Background color
	if (m_bBkgrdColorSet == TRUE) {
		// Initialize
		if (m_pBkgrdBrush == NULL) {
			m_pBkgrdBrush = new CBrush;
		}
		if (m_pBkgrdBrush != NULL) {
			// Create background color brush
			m_pBkgrdBrush->CreateSolidBrush(m_clBkgrdColor);
		}
	}

	// If dialog is set as top-most 
	if (m_bTopMostSet == TRUE) {
		// Set window position
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	// If init sound is set
	if (m_bInitSoundSet == TRUE) {
		MessageBeep(0xFFFFFFFF);
	}
	
	// Register dialog control management
	RegisterDialogManagement();

	// Get dialog ID
	WPARAM wParam = (WPARAM)GetDialogID();

	// Notify parent window about dialog initialization
	this->NotifyParent(SCM_NOTIFY_DIALOGINIT, wParam, NULL);

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

	// Notify parent window about dialog closing
	this->NotifyParent(SCM_NOTIFY_DIALOGCLOSE, wParam, NULL);

	// Close dialog
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
	// Pre-destroy dialog
	SDialog::PreDestroyDialog();

	// Get dialog ID
	WPARAM wParam = (WPARAM)GetDialogID();

	// Notify parent window about dialog destroying
	this->NotifyParent(SCM_NOTIFY_DIALOGDESTROY, wParam, NULL);

	// Destroy dialog
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
	// Default
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// Get dialog ID
	WPARAM wParam = (WPARAM)GetDialogID();

	// Notify parent window about dialog active/inactive state
	if (nState == WA_ACTIVE) {
		this->NotifyParent(SCM_NOTIFY_DIALOGACTIVE, wParam, NULL);
	}
	else if (nState == WA_INACTIVE) {
		this->NotifyParent(SCM_NOTIFY_DIALOGINACTIVE, wParam, NULL);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnMouseMove
//	Description:	Default method for handling mouse moving event within dialog
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OnMouseMove(UINT nFlags, CPoint point)
{
//	CWnd* pHitTestWnd = ChildWindowFromPoint(point);

	// Default
	CDialogEx::OnMouseMove(nFlags, point);
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
	if (GetFlagValue(FLAGID_MINSIZESET) == TRUE) {
		pMinMaxInfo->ptMinTrackSize.x = m_szMinSize.cx;
		pMinMaxInfo->ptMinTrackSize.y = m_szMinSize.cy;
	}
	if (GetFlagValue(FLAGID_MAXSIZESET) == TRUE) {
		pMinMaxInfo->ptMaxTrackSize.x = m_szMaxSize.cx;
		pMinMaxInfo->ptMaxTrackSize.y = m_szMaxSize.cy;
	}

	// Default
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
	// Handle system key pressed message
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

	// Allow the tooltip to receive mouse messages
	if (IsToolTipCtrlAvailable()) {
		this->m_pToolTip->RelayEvent(pMsg);
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
	// Unregister dialog control management
	UnregisterDialogManagement();

	return RESULT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WindowProc
//	Description:	Handle app window messages
//  Arguments:		message - Message ID
//					wParam - First param (HIWORD)
//					lParam - Second param (LOWORD)
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT SDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// Default
	return CDialogEx::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetParentWnd
//	Description:	Get current dialog's parent window
//  Arguments:		None
//  Return value:	CWnd*
//
//////////////////////////////////////////////////////////////////////////

CWnd* SDialog::GetParentWnd(void)
{
	return m_pParentWnd;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetParentWnd
//	Description:	Set parent window for current dialog
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

AFX_INLINE BOOL SDialog::IsParentWndAvailable(void) const
{
	return ((m_pParentWnd != NULL) && (m_pParentWnd->GetSafeHwnd() != NULL));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	NotifyParent
//	Description:	Post a message to parent window for notifying some
//					special events
//  Arguments:		nMsg   - Message
//					wParam - First param
//					lParam - Second param
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::NotifyParent(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bRetNotify = FALSE;

	// If parent window is set and available
	if (IsParentWndAvailable()) {
		// Notify to parent window
		bRetNotify = m_pParentWnd->PostMessage(nMsg, wParam, lParam);
	}
	// Check if main window is available
	else if (CWnd* pMainWnd = AfxGetMainWnd()) {
		// Send to main window to process
		bRetNotify = pMainWnd->PostMessage(nMsg, wParam, lParam);
	}
	else {
		// Notify to application's main thread instead
		// and let it handle the message on its own
		bRetNotify = ::PostMessage(NULL, nMsg, wParam, lParam);
	}

	return bRetNotify;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SendMessageToParent
//	Description:	Similar to NotifyParent but this will send the message
//					and get the result immediately instead of posting it
//  Arguments:		nMsg   - Message
//					wParam - First param
//					lParam - Second param
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT SDialog::SendMessageToParent(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRetNotify = RESULT_FAILED;

	// If parent window is set and available
	if (IsParentWndAvailable()) {
		// Send message to parent window
		lRetNotify = m_pParentWnd->SendMessage(nMsg, wParam, lParam);
	}
	// Check if main window is available
	else if (CWnd* pMainWnd = AfxGetMainWnd()) {
		// Send to main window to process
		lRetNotify = pMainWnd->SendMessage(nMsg, wParam, lParam);
	}
	else {
		// Send message to application's main thread instead
		// and let it handle the message on its own
		lRetNotify = ::SendMessage(NULL, nMsg, wParam, lParam);
	}

	return lRetNotify;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetToolTipCtrl
//	Description:	Get current dialog's tooltip control window
//  Arguments:		None
//  Return value:	CToolTipCtrl*
//
//////////////////////////////////////////////////////////////////////////

CToolTipCtrl* SDialog::GetToolTipCtrl(void)
{
	return m_pToolTip;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsToolTipCtrlAvailable
//	Description:	Check if tooltip control window (pointer) is available
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL SDialog::IsToolTipCtrlAvailable(void) const
{
	return ((m_pToolTip != NULL) && (m_pToolTip->GetSafeHwnd() != NULL));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetControlManager
//	Description:	Get dialog control manager pointer
//  Arguments:		None
//  Return value:	SControlManager*
//
//////////////////////////////////////////////////////////////////////////

SControlManager* SDialog::GetControlManager(void)
{
	return m_pCtrlManager;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegisterDialogManagement
//	Description:	Register dialog control management
//  Arguments:		None
//  Return value:	INT_PTR - Number of controls added to management
//
//////////////////////////////////////////////////////////////////////////

INT_PTR SDialog::RegisterDialogManagement(void)
{
	// Initialize dialog control management
	if (m_pCtrlManager == NULL) {
		m_pCtrlManager = new SControlManager(this);
		if (m_pCtrlManager == NULL)
			return INT_INVALID;
	}

	if (m_pCtrlManager != NULL) {
		m_pCtrlManager->Initialize();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateDialogManagement
//	Description:	Update dialog control management
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::UpdateDialogManagement(void)
{
	// TODO: Deriver this function for custom actions
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UnregisterDialogManagement
//	Description:	Unregister dialog control management
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::UnregisterDialogManagement(void)
{
	// Clean-up dialog control management
	if (m_pCtrlManager == NULL)
		return FALSE;

	if (m_pCtrlManager != NULL) {
		if (!m_pCtrlManager->DeleteAll())
			return FALSE;

		delete m_pCtrlManager;
		m_pCtrlManager = NULL;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddStyle
//	Description:	Add dialog extended styles
//  Arguments:		dwAddStyle - Styles to add
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::AddStyle(DWORD dwAddStyle)
{
	DWORD dwStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	dwStyle |= dwAddStyle;
	LONG lRet = SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, dwStyle);
	return (lRet != 0);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveStyle
//	Description:	Remove dialog extended styles
//  Arguments:		dwRemoveStyle - Styles to remove
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SDialog::RemoveStyle(DWORD dwRemoveStyle)
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

AFX_INLINE UINT SDialog::GetDialogID(void) const
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

AFX_INLINE BOOL SDialog::GetReadOnlyMode(void) const
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

AFX_INLINE BOOL SDialog::GetLockState(void) const
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
	// Initialize the list if not yet allocated
	if (m_paLockExceptionIDList == NULL) {
		m_paLockExceptionIDList = new CUIntArray;
		m_paLockExceptionIDList->RemoveAll();
	}

	// Loop through all list and find if item existed in list
	int nItemNum = m_paLockExceptionIDList->GetSize();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		if (m_paLockExceptionIDList->GetAt(nIndex) == nID) {
			// No need to add
			return;
		}
	}

	// Add to list
	m_paLockExceptionIDList->Add(nID);
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
	// If the list is not yet allocated, no need to remove
	if (m_paLockExceptionIDList == NULL)
		return;

	// Loop through all list and remove item
	int nItemNum = m_paLockExceptionIDList->GetSize();
	for (int nIndex = (nItemNum - 1); nIndex >= 0; nIndex--) {
		if (m_paLockExceptionIDList->GetAt(nIndex) == nID) {
			m_paLockExceptionIDList->RemoveAt(nIndex);
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

void SDialog::ResetLockStateExceptionList(void)
{
	if (m_paLockExceptionIDList != NULL) {
		m_paLockExceptionIDList->RemoveAll();
		m_paLockExceptionIDList->FreeExtra();
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetAlignment
//	Description:	Get/set dialog alignment
//  Arguments:		nAlignment - Dialog alignment
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE UINT SDialog::GetAlignment(void) const
{
	return m_nAlignment;
}

void SDialog::SetAlignment(UINT nAlignment)
{
	m_nAlignment = nAlignment;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetPosition
//	Description:	Get/set dialog position
//  Arguments:		lpPosition - Dialog position (anchor point) (OUT)
//					ptPosition - Dialog position (anchor point) (IN)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetPosition(LPPOINT lpPosition) const
{
	// Get position
	lpPosition->x = m_ptPosition.x;
	lpPosition->y = m_ptPosition.y;
}

void SDialog::SetPosition(POINT ptPosition)
{
	// Set position
	m_ptPosition.x = ptPosition.x;
	m_ptPosition.y = ptPosition.y;

	// Move dialog
	MoveDialog(m_ptPosition);
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

	// Get dialog alignment
	UINT nAlign = GetAlignment();

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
	if ((m_szRegisterSize.cx > -1) && (m_szRegisterSize.cy > -1)) {
		rectDlg.right = (rectDlg.left + m_szRegisterSize.cx);
		rectDlg.bottom = (rectDlg.top + m_szRegisterSize.cy);
	}

	// Resize
	SetWindowPos(NULL, 0, 0, (rectDlg.right - rectDlg.left), (rectDlg.bottom - rectDlg.top), SWP_NOMOVE | SWP_NOZORDER);

	// Make sure that the entire dialog box is visible on the screen
	SendMessage(DM_REPOSITION, 0, 0);

	// Center dialog
	if (bCenterDialog == TRUE) {
		this->CenterWindow(GetParentWnd());
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

void SDialog::ResetDialogSize(void)
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
	m_szRegisterSize.cx = m_szDefaultSize.cx;
	m_szRegisterSize.cy = m_szDefaultSize.cy;

	// Resize dialog
	ResizeDialog(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetSize
//	Description:	Get/set dialog size
//  Arguments:		lpRegSize - Dialog size
//					szRegSize - Dialog size
//					lWidth	  - Dialog width
//					lHeight	  - Dialog height
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetSize(LPSIZE lpRegSize) const
{
	// If size is not set, return default
	if ((m_szRegisterSize.cx <= 0) && (m_szRegisterSize.cy <= 0)) {
		lpRegSize->cx = m_szDefaultSize.cx;
		lpRegSize->cy = m_szDefaultSize.cy;
	}

	// Return dialog size
	lpRegSize->cx = m_szRegisterSize.cx;
	lpRegSize->cy = m_szRegisterSize.cy;
}

void SDialog::SetSize(SIZE szRegSize)
{
	// Copy data
	m_szRegisterSize.cx = szRegSize.cx;
	m_szRegisterSize.cy = szRegSize.cy;
}

void SDialog::SetSize(LONG lWidth, LONG lHeight)
{
	// Create dialog size data
	CSize szDialogSize(lWidth, lHeight);

	// Set dialog size
	this->SetSize(szDialogSize);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMin/MaxSize
//	Description:	Set dialog min/max size
//  Arguments:		lMinWidth	- Dialog min width
//					lMinHeight  - Dialog min height
//					lMaxWidth	- Dialog max width
//					lMaxHeight	- Dialog max height
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetMinSize(LONG lMinWidth, LONG lMinHeight)
{
	// Set min size
	m_szMinSize = CSize(lMinWidth, lMinHeight);
}

void SDialog::SetMaxSize(LONG lMaxWidth, LONG lMaxHeight)
{
	// Set min size
	m_szMaxSize = CSize(lMaxWidth, lMaxHeight);
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
	m_bTopMostSet = bTopMost;
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
	m_bInitSoundSet = bInitSound;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetTop/Left/Bottom/RightMargin
//	Description:	Set content display margin: top/left/bottom/right
//  Arguments:		lMargin	- Corresponding margin value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetLeftMargin(LONG lMargin)
{
	m_rcClientMargin.left = lMargin;
}

void SDialog::SetTopMargin(LONG lMargin)
{
	m_rcClientMargin.top = lMargin;
}

void SDialog::SetRightMargin(LONG lMargin)
{
	m_rcClientMargin.right = lMargin;
}

void SDialog::SetBottomMargin(LONG lMargin)
{
	m_rcClientMargin.bottom = lMargin;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetCenterMargin
//	Description:	Set content display center margin
//  Arguments:		ptMargin - Display margin
//					lHMargin - Horizontal margin
//					lVMargin - Vertical margin
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetCenterMargin(POINT ptMargin)
{
	// Top left
	m_rcClientMargin.left = ptMargin.x;
	m_rcClientMargin.top = ptMargin.y;
	
	// Bottom right
	m_rcClientMargin.right = ptMargin.x;
	m_rcClientMargin.bottom = ptMargin.y;
}

void SDialog::SetCenterMargin(LONG lHMargin, LONG lVMargin)
{
	// Top left
	m_rcClientMargin.left = lHMargin;
	m_rcClientMargin.top = lVMargin;

	// Bottom right
	m_rcClientMargin.right = lHMargin;
	m_rcClientMargin.bottom = lVMargin;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDisplayArea
//	Description:	Get content display area
//  Arguments:		lpRect - Display area rectangle
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetDisplayArea(LPRECT lpDispAreaRect) const
{
	// Get client rect
	this->GetClientRect(lpDispAreaRect);

	// Calculate display area with margin
	lpDispAreaRect->left += m_rcClientMargin.left;
	lpDispAreaRect->top += m_rcClientMargin.top;
	lpDispAreaRect->right -= m_rcClientMargin.right;
	lpDispAreaRect->bottom -= m_rcClientMargin.bottom;
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
	CRect rcCurMargin;
	this->GetMargin(&rcCurMargin);

	// Horizontal center margin
	BOOL bHorzCenter = FALSE;
	if (rcCurMargin.left == rcCurMargin.right) {
		bHorzCenter = TRUE;
	}

	// Vertical center margin
	BOOL bVertCenter = FALSE;
	if (rcCurMargin.top == rcCurMargin.bottom) {
		bVertCenter = TRUE;
	}

	// New dialog margin
	CRect rcNewMargin = CRect(rcCurMargin);

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
	CSize szDialogSize;
	this->GetSize(&szDialogSize);

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
			rcNewMargin.left = rcCurMargin.left + (szCurDispAreaSize.cx - szNewDispAreaSize.cx);
			rcNewMargin.right = rcNewMargin.left;
		}
		// Vertical center margin
		if (bVertCenter == TRUE) {
			rcNewMargin.top = rcCurMargin.top + (szCurDispAreaSize.cy - szNewDispAreaSize.cy);
			rcNewMargin.bottom = rcNewMargin.top;
		}
	}

	// Set new dialog rectangle top-left
	rcNewDlgRect.left = (rcNewDispArea.left - rcNewMargin.left) - rcDlgClientOffset.left;
	rcNewDlgRect.top = (rcNewDispArea.top - rcNewMargin.top) - rcDlgClientOffset.top;

	// If resize is specified
	if (bResizeDialog == TRUE) {
		// Set new dialog rectangle bottom-right
		rcNewDlgRect.right = (rcNewDispArea.right + rcNewMargin.right) + rcDlgClientOffset.right;
		rcNewDlgRect.bottom = (rcNewDispArea.bottom + rcNewMargin.bottom) + rcDlgClientOffset.bottom;

		// Resize dialog
		this->SetSize((rcNewDlgRect.right - rcNewDlgRect.left), (rcNewDlgRect.bottom - rcNewDlgRect.top));
		this->ResizeDialog(bCenter);
	}
	else {
		// Reposition following new margin offset
		rcNewDlgRect.left += (rcNewMargin.left - rcCurMargin.left);
		rcNewDlgRect.top += (rcNewMargin.top - rcCurMargin.top);

		// Set new dialog rectangle bottom-right
		rcNewDlgRect.right = (rcNewDlgRect.left + szDialogSize.cx);
		rcNewDlgRect.bottom = (rcNewDlgRect.top + szDialogSize.cy);

		// Recalculate bottom-right margin
		rcNewMargin.right = (rcNewDlgRect.right - rcDlgClientOffset.right) - rcNewDispArea.right;
		rcNewMargin.bottom = (rcNewDlgRect.bottom - rcDlgClientOffset.bottom) - rcNewDispArea.bottom;

		// Reposition dialog
		SetWindowPos(NULL, rcNewDlgRect.left, rcNewDlgRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		// Make sure that the entire dialog box is visible on the screen
		SendMessage(DM_REPOSITION, 0, 0);

		// Center dialog
		if (bCenter == TRUE) {
			this->CenterWindow(GetParentWnd());
		}
	}

	// Save margin update
	this->SetLeftMargin(rcNewMargin.left);
	this->SetTopMargin(rcNewMargin.top);
	this->SetRightMargin(rcNewMargin.right);
	this->SetBottomMargin(rcNewMargin.bottom);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMargin
//	Description:	Get content display margin
//  Arguments:		lpDialogMargin - Dialog display margin
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetMargin(LPRECT lpDialogMargin) const
{
	// Top left
	lpDialogMargin->left = m_rcClientMargin.left;
	lpDialogMargin->top = m_rcClientMargin.top;

	// Bottom right
	lpDialogMargin->right = m_rcClientMargin.right;
	lpDialogMargin->bottom = m_rcClientMargin.bottom;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetCaption
//	Description:	Get dialog caption
//  Arguments:		None
//  Return value:	LPCTSTR - Dialog caption string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR SDialog::GetCaption(void) const
{
	return m_strCaption;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetCaption
//	Description:	Get dialog caption
//  Arguments:		strCaption - Dialog caption
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetCaption(CString& strCaption) const
{
	strCaption = m_strCaption;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetCaption
//	Description:	Set dialog caption
//  Arguments:		lpszDlgTitle - Dialog caption
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetCaption(LPCTSTR lpszCaption)
{
	// Set caption
	m_strCaption = lpszCaption;

	// If dialog is already initialized, trigger updating title
	if (IsWindow(this->m_hWnd)) {
		this->SetWindowText(m_strCaption);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetCaptionFromResource
//	Description:	Set dialog caption by resource string ID
//  Arguments:		nResourceStringID - Resource string ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetCaptionFromResource(UINT nResourceStringID)
{
	CString strTemp;
	BOOL bRet = strTemp.LoadString(nResourceStringID);
	ASSERT(!strTemp.IsEmpty());
	if ((bRet == TRUE) && (!strTemp.IsEmpty())) {
		this->SetCaption(strTemp);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetCaptionFromLanguage
//	Description:	Set dialog caption by language string ID
//  Arguments:		nLangStringID - Language string ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetCaptionFromLanguage(UINT nLangStringID)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->GetAppLanguage();

	CString strTemp = GetLanguageString(pAppLang, nLangStringID);
	this->SetCaption(strTemp);
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

void SDialog::GetBkgrdColor(COLORREF& clBkgrdColor) const
{
	clBkgrdColor = m_clBkgrdColor;
}

void SDialog::SetBkgrdColor(COLORREF clBkgrdColor)
{
	m_clBkgrdColor = clBkgrdColor;
	m_bBkgrdColorSet = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetTextColor
//	Description:	Get/set text color
//  Arguments:		clTextColor - Text color
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetTextColor(COLORREF& clTextColor) const
{
	clTextColor = m_clTextColor;
}

void SDialog::SetTextColor(COLORREF clTextColor)
{
	m_clTextColor = clTextColor;
	m_bTextColorSet = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegisterMessageBoxCaption
//	Description:	Register default caption for message box-es
//  Arguments:		nCaptionID  - ID of message caption string
//					lpszCaption	- Message caption string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::RegisterMessageBoxCaption(UINT nCaptionID)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->GetAppLanguage();
	CString strCaption = STRING_EMPTY;
	if (nCaptionID != NULL) {

		// Get language string caption
		CString strTemp = GetLanguageString(pAppLang, nCaptionID);
		if (IS_NOT_NULL_STRING(strTemp)) {
			// Set caption string
			strCaption = strTemp;
		}
	}

	// If caption is empty
	if (strCaption.IsEmpty()) {
		// Use default app window caption
		strCaption = ((SWinApp*)AfxGetApp())->GetAppWindowCaption();
	}

	// Register message box caption
	RegisterMessageBoxCaption(strCaption);
}

void SDialog::RegisterMessageBoxCaption(LPCTSTR lpszCaption)
{
	// Register message box caption
	m_strMsgCaption = lpszCaption;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMessageBoxCaption
//	Description:	Get registered message box caption string
//  Arguments:		strRegMsgBoxCap - Registered message box caption
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::GetMessageBoxCaption(CString& strRegMsgBoxCap) const
{
	strRegMsgBoxCap = m_strMsgCaption;
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
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->GetAppLanguage();

	CString strMsg = GetLanguageString(pAppLang, nPromptID);
	CString strCaption = ((SWinApp*)AfxGetApp())->GetAppWindowCaption();
	if (nCaptionID != NULL) {

		// Get language string caption
		CString strTemp = GetLanguageString(pAppLang, nCaptionID);
		if (IS_NOT_NULL_STRING(strTemp))
			strCaption = strTemp;
	}
	else {
		// Using registered message box caption
		if (!m_strMsgCaption.IsEmpty()) {
			strCaption = m_strMsgCaption;
		}
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

int SDialog::DisplayMessageBox(LPCTSTR lpszPrompt, LPCTSTR lpszCaption /* = NULL */, UINT nStyle /* = NULL */)
{
	// Set default style
	if (nStyle == NULL) {
		// Information message with [OK] button
		nStyle = MB_OK | MB_ICONINFORMATION;
	}

	// If caption is not set
	CString strCaption(lpszCaption);
	if (strCaption.IsEmpty()) {

		// If message box caption is registered
		if (!m_strMsgCaption.IsEmpty()) {
			// Use registered caption
			strCaption = m_strMsgCaption;
		}

		// Otherwise,
		else {
			// Use app window caption
			strCaption = ((SWinApp*)AfxGetApp())->GetAppWindowCaption();
		}
	}
	
	// Display message box
	nStyle |= MB_SYSTEMMODAL;
	return MessageBox(lpszPrompt, strCaption, nStyle);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputEventLog
//	Description:	Output application event log
//  Arguments:		usEvent			- Event ID
//					lpszDescription - Additional description
//					pDetailInfo		- Log detail info (array pointer)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OutputEventLog(USHORT usEvent, LPCTSTR lpszDescription /* = NULL */, LOGDETAILINFO* pDetailInfo /* = NULL */)
{
	// Prepare event log info
	LOGITEM logItemDialogEvent;
	logItemDialogEvent.usCategory = usEvent;
	logItemDialogEvent.stTime = GetCurSysTime();
	logItemDialogEvent.dwProcessID = GetCurrentProcessId();
	if (lpszDescription != NULL) {
		// Include event description
		logItemDialogEvent.strLogString = lpszDescription;
	}
	if (pDetailInfo != NULL) {
		// Include event detail info data
		for (int nIndex = 0; nIndex < pDetailInfo->GetSize(); nIndex++) {
			logItemDialogEvent.AddDetail(pDetailInfo->GetAt(nIndex));
		}
	}

	// Output dialog event log
	SWinApp* pApp = (SWinApp*)AfxGetApp();
	ASSERT(pApp);
	if (pApp == NULL) return;
	if (SLogging* ptrAppEventLog = pApp->GetAppEventLog()) {
		ptrAppEventLog->OutputItem(logItemDialogEvent);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputButtonLog
//	Description:	Output button event log
//  Arguments:		usEvent	  - Event ID
//					nButtonID - Button ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OutputButtonLog(USHORT usEvent, UINT nButtonID)
{
	// Prepare button event log info
	CButton* pButton = (CButton*)GetDlgItem(nButtonID);
	if (pButton == NULL) return;
	CString strButtonCaption;
	pButton->GetWindowText(strButtonCaption);

	// Detail info
	LOGDETAILINFO logDetailInfo;

	// Button ID
	LOGDETAIL logDetail;
	logDetail.usCategory = EVENTLOG_DETAIL_RESOURCEID;
	logDetail.uiDetailInfo = nButtonID;
	logDetailInfo.Add(logDetail);

	// Mapped ID
	logDetail.Init();
	logDetail.usCategory = EVENTLOG_DETAIL_MAPTEXTID;
	logDetail.strDetailInfo = IDMAP_GET(nButtonID);
	logDetailInfo.Add(logDetail);

	// Output button event log
	OutputEventLog(usEvent, strButtonCaption, &logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputCheckBoxLog
//	Description:	Output checkbox event log
//  Arguments:		usEvent		- Event ID
//					nCheckboxID - Checkbox ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OutputCheckBoxLog(USHORT usEvent, UINT nCheckboxID)
{
	// Prepare checkbox event log info
	CButton* pChkBtn = (CButton*)GetDlgItem(nCheckboxID);
	if (pChkBtn == NULL) return;
	CString strChkCaption;
	pChkBtn->GetWindowText(strChkCaption);

	// Detail info
	LOGDETAILINFO logDetailInfo;

	// Checkbox ID
	LOGDETAIL logDetail;
	logDetail.usCategory = EVENTLOG_DETAIL_RESOURCEID;
	logDetail.uiDetailInfo = nCheckboxID;
	logDetailInfo.Add(logDetail);

	// Mapped ID
	logDetail.Init();
	logDetail.usCategory = EVENTLOG_DETAIL_MAPTEXTID;
	logDetail.strDetailInfo = IDMAP_GET(nCheckboxID);
	logDetailInfo.Add(logDetail);

	// Checkbox checked state
	logDetail.Init();
	logDetail.usCategory = EVENTLOG_DETAIL_CHKSTATE;
	logDetail.uiDetailInfo = pChkBtn->GetCheck();
	logDetailInfo.Add(logDetail);

	// Output checkbox event log
	OutputEventLog(usEvent, strChkCaption, &logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputRadButtonLog
//	Description:	Output radio button event log
//  Arguments:		usEvent		- Event ID
//					nCheckboxID - Checkbox ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OutputRadButtonLog(USHORT usEvent, UINT nRadButtonID)
{
	// Prepare radio button event log info
	CButton* pRadBtn = (CButton*)GetDlgItem(nRadButtonID);
	if (pRadBtn == NULL) return;
	CString strRadCaption;
	pRadBtn->GetWindowText(strRadCaption);

	// Detail info
	LOGDETAILINFO logDetailInfo;

	// Radio button ID
	LOGDETAIL logDetail;
	logDetail.usCategory = EVENTLOG_DETAIL_RESOURCEID;
	logDetail.uiDetailInfo = nRadButtonID;
	logDetailInfo.Add(logDetail);

	// Mapped ID
	logDetail.Init();
	logDetail.usCategory = EVENTLOG_DETAIL_MAPTEXTID;
	logDetail.strDetailInfo = IDMAP_GET(nRadButtonID);
	logDetailInfo.Add(logDetail);

	// Radio button checked state
	logDetail.Init();
	logDetail.usCategory = EVENTLOG_DETAIL_CHKSTATE;
	logDetail.uiDetailInfo = pRadBtn->GetCheck();
	logDetailInfo.Add(logDetail);

	// Output radio button event log
	OutputEventLog(usEvent, strRadCaption, &logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputComboBoxLog
//	Description:	Output combo-box event log
//  Arguments:		usEvent	 - Event ID
//					nComboID - Combo-box ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OutputComboBoxLog(USHORT usEvent, UINT nComboID)
{
	// Prepare combo-box event log info
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nComboID);
	if (pCombo == NULL) return;

	// Detail info
	LOGDETAILINFO logDetailInfo;

	// Combo-box ID
	LOGDETAIL logDetail;
	logDetail.usCategory = EVENTLOG_DETAIL_RESOURCEID;
	logDetail.uiDetailInfo = nComboID;
	logDetailInfo.Add(logDetail);

	// Mapped ID
	logDetail.Init();
	logDetail.usCategory = EVENTLOG_DETAIL_MAPTEXTID;
	logDetail.strDetailInfo = IDMAP_GET(nComboID);
	logDetailInfo.Add(logDetail);

	// Combo-box control info
	CString strComboCaption;
	SControlManager* pCtrlMan = GetControlManager();
	if (pCtrlMan != NULL) {
		SCtrlInfoWrap* pComboWrap = pCtrlMan->GetControl(nComboID);
		if (pComboWrap != NULL) {
			// Combo-box caption
			pComboWrap->GetCaption(strComboCaption);

			// Combo-box current selection string
			INT_PTR nCurSel = pComboWrap->GetValueInt();
			CStringArray arrDataList;
			pComboWrap->GetValueStringArray(arrDataList);
			if ((!arrDataList.IsEmpty()) && (arrDataList.GetCount() > nCurSel)) {
				logDetail.Init();
				logDetail.usCategory = EVENTLOG_DETAIL_SELECTION;
				logDetail.strDetailInfo = arrDataList.GetAt(nCurSel);
				logDetailInfo.Add(logDetail);
			}
		}
	}

	// Output combo-box event log
	OutputEventLog(usEvent, strComboCaption, &logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputEditBoxLog
//	Description:	Output edit box event log
//  Arguments:		usEvent	- Event ID
//					nEditID - Edit box ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OutputEditBoxLog(USHORT usEvent, UINT nEditID)
{
	// Prepare edit box event log info
	CEdit* pEdit = (CEdit*)GetDlgItem(nEditID);
	if (pEdit == NULL) return;

	// Detail info
	LOGDETAILINFO logDetailInfo;

	// Edit box ID
	LOGDETAIL logDetail;
	logDetail.usCategory = EVENTLOG_DETAIL_RESOURCEID;
	logDetail.uiDetailInfo = nEditID;
	logDetailInfo.Add(logDetail);

	// Mapped ID
	logDetail.Init();
	logDetail.usCategory = EVENTLOG_DETAIL_MAPTEXTID;
	logDetail.strDetailInfo = IDMAP_GET(nEditID);
	logDetailInfo.Add(logDetail);

	// Edit box control info
	CString strEditBoxCaption;
	SControlManager* pCtrlMan = GetControlManager();
	if (pCtrlMan != NULL) {
		SCtrlInfoWrap* pEditBoxWrap = pCtrlMan->GetControl(nEditID);
		if (pEditBoxWrap != NULL) {
			// Edit box caption
			pEditBoxWrap->GetCaption(strEditBoxCaption);
			
			// Edit box content
			logDetail.Init();
			logDetail.usCategory = EVENTLOG_DETAIL_DATAVALUE;
			pEditBoxWrap->GetValueString(logDetail.strDetailInfo);
			logDetailInfo.Add(logDetail);
		}
	}

	// Output edit box event log
	OutputEventLog(usEvent, strEditBoxCaption, &logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputListBoxLog
//	Description:	Output list box event log
//  Arguments:		usEvent	   - Event ID
//					nListBoxID - List box ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OutputListBoxLog(USHORT usEvent, UINT nListBoxID)
{
	// Prepare list box event log info
	CListBox* pListBox = (CListBox*)GetDlgItem(nListBoxID);
	if (pListBox == NULL) return;

	// Detail info
	LOGDETAILINFO logDetailInfo;

	// List box ID
	LOGDETAIL logDetail;
	logDetail.usCategory = EVENTLOG_DETAIL_RESOURCEID;
	logDetail.uiDetailInfo = nListBoxID;
	logDetailInfo.Add(logDetail);

	// Mapped ID
	logDetail.Init();
	logDetail.usCategory = EVENTLOG_DETAIL_MAPTEXTID;
	logDetail.strDetailInfo = IDMAP_GET(nListBoxID);
	logDetailInfo.Add(logDetail);

	// List box control info
	CString strListBoxCaption;
	SControlManager* pCtrlMan = GetControlManager();
	if (pCtrlMan != NULL) {
		SCtrlInfoWrap* pListBoxWrap = pCtrlMan->GetControl(nListBoxID);
		if (pListBoxWrap != NULL) {
			// List box caption
			pListBoxWrap->GetCaption(strListBoxCaption);

			// List box current selection string
			INT_PTR nCurSel = pListBoxWrap->GetValueInt();
			CStringArray arrDataList;
			pListBoxWrap->GetValueStringArray(arrDataList);
			if ((!arrDataList.IsEmpty()) && (arrDataList.GetCount() > nCurSel)) {
				logDetail.Init();
				logDetail.usCategory = EVENTLOG_DETAIL_SELECTION;
				logDetail.strDetailInfo = arrDataList.GetAt(nCurSel);
				logDetailInfo.Add(logDetail);
			}
		}
	}

	// Output list box event log
	OutputEventLog(usEvent, strListBoxCaption, &logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputSpinCtrlLog
//	Description:	Output spin control event log
//  Arguments:		usEvent		- Event ID
//					nSpinCtrlID - Spin control ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OutputSpinCtrlLog(USHORT usEvent, UINT nSpinCtrlID)
{
	// Prepare spin control event log info
	CSpinButtonCtrl* pSpinCtrl = (CSpinButtonCtrl*)GetDlgItem(nSpinCtrlID);
	if (pSpinCtrl == NULL) return;

	// Detail info
	LOGDETAILINFO logDetailInfo;

	// Spin control ID
	LOGDETAIL logDetail;
	logDetail.usCategory = EVENTLOG_DETAIL_RESOURCEID;
	logDetail.uiDetailInfo = nSpinCtrlID;
	logDetailInfo.Add(logDetail);

	// Mapped ID
	logDetail.Init();
	logDetail.usCategory = EVENTLOG_DETAIL_MAPTEXTID;
	logDetail.strDetailInfo = IDMAP_GET(nSpinCtrlID);
	logDetailInfo.Add(logDetail);

	// Output spin control event log
	OutputEventLog(usEvent, NULL, &logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputMenuLog
//	Description:	Output menu event log
//  Arguments:		usEvent		- Event ID
//					nMenuItemID - Menu item ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::OutputMenuLog(USHORT usEvent, UINT nMenuItemID)
{
	// Prepare menu event log info
	// Get menu item title from language table
	CString strMenuItemCaption;
	LANGTABLE_PTR pLanguage = ((SWinApp*)AfxGetApp())->GetAppLanguage();
	if (pLanguage != NULL) {
		strMenuItemCaption = GetLanguageString(pLanguage, nMenuItemID);
	}

	// Detail info
	LOGDETAILINFO logDetailInfo;

	// Menu item ID
	LOGDETAIL logDetail;
	logDetail.usCategory = EVENTLOG_DETAIL_RESOURCEID;
	logDetail.uiDetailInfo = nMenuItemID;
	logDetailInfo.Add(logDetail);

	// Mapped ID
	logDetail.Init();
	logDetail.usCategory = EVENTLOG_DETAIL_MAPTEXTID;
	logDetail.strDetailInfo = IDMAP_GET(nMenuItemID);
	logDetailInfo.Add(logDetail);

	// Output menu event log
	OutputEventLog(usEvent, strMenuItemCaption, &logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog controls
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetupLanguage(void)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->GetAppLanguage();

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
		ptrLanguage = ((SWinApp*)AfxGetApp())->GetAppLanguage();
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
		ptrLanguage = ((SWinApp*)AfxGetApp())->GetAppLanguage();
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
//	Function name:	MoveItemGroup
//	Description:	Move a group of dialog items to specific position
//  Arguments:		arrCtrlIDGroup  - Array of control IDs
//					ptNewPosition	- New position
//  Return value:	None
//  Notes:			This function will take the top-left point (smallest X&Y)
//					of the smallest rectangle which borders all the controls
//					as the original point. Then it will recalculate new positions
//					of all the controls and move them so that the original point
//					will be placed at the new position
//
//////////////////////////////////////////////////////////////////////////

void SDialog::MoveItemGroup(const CUIntArray& arrCtrlIDGroup, POINT ptNewPosition)
{
	// Check data validity
	if (arrCtrlIDGroup.IsEmpty())
		return;

	RECT rcCtrlWnd;
	CWnd* pCtrlWnd = NULL;
	int nOrgX = INT_INVALID, nOrgY = INT_INVALID;

	// Find the original point
	for (int nIndex = 0; nIndex < arrCtrlIDGroup.GetSize(); nIndex++)
	{
		// Get item
		pCtrlWnd = GetDlgItem(arrCtrlIDGroup.GetAt(nIndex));
		if (pCtrlWnd == NULL)
			continue;

		// Get item rect
		pCtrlWnd->GetWindowRect(&rcCtrlWnd);

		// Find smallest X
		if ((nOrgX == INT_INVALID) || (rcCtrlWnd.left <= nOrgX)) {
			nOrgX = rcCtrlWnd.left;
		}

		// Find smallest Y
		if ((nOrgY == INT_INVALID) || (rcCtrlWnd.top <= nOrgY)) {
			nOrgY = rcCtrlWnd.top;
		}
	}

	// Calculate moving distance
	int nDeltaX = ptNewPosition.x - nOrgX;
	int nDeltaY = ptNewPosition.y - nOrgY;

	// Move all items to new position
	int nNewX = 0, nNewY = 0;
	for (int nIndex = 0; nIndex < arrCtrlIDGroup.GetSize(); nIndex++)
	{
		// Get item
		pCtrlWnd = GetDlgItem(arrCtrlIDGroup.GetAt(nIndex));
		if (pCtrlWnd == NULL)
			continue;

		// Get item rect
		pCtrlWnd->GetWindowRect(&rcCtrlWnd);

		// Move item
		nNewX = rcCtrlWnd.left + nDeltaX;
		nNewY = rcCtrlWnd.top + nDeltaY;
		pCtrlWnd->SetWindowPos(NULL, nNewX, nNewY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	MoveItemGroup
//	Description:	Move a group of dialog items to specific position
//  Arguments:		arrCtrlIDGroup  - Array of control IDs
//					nDirection		- Moving direction
//					nDistance		- Moving distance
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::MoveItemGroup(const CUIntArray& arrCtrlIDGroup, int nDirection, int nDistance)
{
	// Check data validity
	if (arrCtrlIDGroup.IsEmpty())
		return;

	RECT rcCtrlWnd;
	CWnd* pCtrlWnd = NULL;
	int nNewX = 0, nNewY = 0;

	// Loop through each item and move
	for (int nIndex = 0; nIndex < arrCtrlIDGroup.GetSize(); nIndex++)
	{
		// Get item
		pCtrlWnd = GetDlgItem(arrCtrlIDGroup.GetAt(nIndex));
		if (pCtrlWnd == NULL)
			continue;

		// Get item rect
		pCtrlWnd->GetWindowRect(&rcCtrlWnd);

		// Move horizontal direction
		if (nDirection == MOVDIR_HORIZONTAL) {
			nNewX = rcCtrlWnd.left + nDistance;
			nNewY = rcCtrlWnd.top;
			pCtrlWnd->SetWindowPos(NULL, nNewX, nNewY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		// Move vertical direction
		else if (nDirection == MOVDIR_VERTICAL) {
			nNewX = rcCtrlWnd.left;
			nNewY = rcCtrlWnd.top + nDistance;
			pCtrlWnd->SetWindowPos(NULL, nNewX, nNewY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}
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

void SDialog::SetupDlgItemState(void)
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

void SDialog::RefreshDlgItemState(void)
{
	// TODO: Deriver this function for custom actions
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateLayoutInfo
//	Description:	Update layout info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::UpdateLayoutInfo(void)
{
	// TODO: Deriver this function for custom actions
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadLayoutInfo
//	Description:	Load layout info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::LoadLayoutInfo(void)
{
	// TODO: Deriver this function for custom actions
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveLayoutInfo
//	Description:	Save layout info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SaveLayoutInfo(void)
{
	// TODO: Deriver this function for custom actions
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateDialogData
//	Description:	Update data values for dialog items
//  Arguments:		bSaveAndValidate - Same as default MFC UpdateData function
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::UpdateDialogData(BOOL bSaveAndValidate /* = TRUE */)
{
	// TODO: Deriver this function for custom actions
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppOption
//	Description:	Return option value by ID
//  Arguments:		eAppOptionID - ID of specific option
//					bTemp		 - Temp value or saved value (saved value by default)
//  Return value:	int - Option value
//
//////////////////////////////////////////////////////////////////////////

int SDialog::GetAppOption(APPOPTIONID eAppOptionID, BOOL bTemp /* = FALSE */) const
{
	int nResult = INT_INVALID;
	int nTempResult = INT_INVALID;

	switch (eAppOptionID)
	{
	case OPTIONID_INVALID:		// *** Invalid option ***
		break;
	default:
		// Get application-base-class option value
		nResult = ((SWinApp*)AfxGetApp())->GetAppOption(eAppOptionID, FALSE);
		nTempResult = ((SWinApp*)AfxGetApp())->GetAppOption(eAppOptionID, TRUE);
		break;
	}

	// Return temp data if required and the result is valid
	if ((bTemp == TRUE) && (nTempResult != INT_INVALID))
		return nTempResult;

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetFlagValue
//	Description:	Return flag value by ID
//  Arguments:		eFlagID - ID of specific flag
//  Return value:	int - Flag value
//
//////////////////////////////////////////////////////////////////////////

int SDialog::GetFlagValue(APPFLAGID eFlagID) const
{
	int nValue = INT_INVALID;

	switch (eFlagID)
	{
	case FLAGID_INVALID:					// *** Invalid flag ID ***
		break;
	case FLAGID_CHANGEFLAG:					// Data/setting change flag
		nValue = m_bChangeFlag;
		break;
	case FLAGID_READONLYMODE:				// Read-only mode
		nValue = m_bReadOnlyMode;
		break;
	case FLAGID_LOCKSTATE:					// Lock state
		nValue = m_bLockState;
		break;
	case FLAGID_FORCECLOSING:				// Force closing by request
		nValue = m_bForceClose;
		break;
	case FLAGID_USEESCAPE:					// Use Escape key
		nValue = m_bUseEscape;
		break;
	case FLAGID_USEENTER:					// Use Enter key
		nValue = m_bUseEnter;
		break;
	case FLAGID_BKGRDCLRSET:				// Dialog background color is set
		nValue = m_bBkgrdColorSet;
		break;
	case FLAGID_TEXTCLRSET:					// Dialog text color is set
		nValue = m_bTextColorSet;
		break;
	case FLAGID_MINSIZESET:					// Dialog minimum size is set
		nValue = ((m_szMinSize.cx >= 0) &&
				  (m_szMinSize.cy >= 0));
		break;
	case FLAGID_MAXSIZESET:					// Dialog maximum size is set
		nValue = ((m_szMaxSize.cx > m_szMinSize.cx) &&
				  (m_szMaxSize.cy > m_szMinSize.cy));
		break;
	case FLAGID_TOPMOSTSET:					// Dialog top-most position is set
		nValue = m_bTopMostSet;
		break;
	case FLAGID_INITSOUNDSET:				// Dialog initialize sound is set
		nValue = m_bInitSoundSet;
		break;
	default:
		// Get application-base-class flag value
		nValue = ((SWinApp*)AfxGetApp())->GetFlagValue(eFlagID);
		break;
	}

	return nValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetFlagValue
//	Description:	Update flag value by ID
//  Arguments:		eFlagID - ID of specific flag
//					nValue  - Value to set
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetFlagValue(APPFLAGID eFlagID, int nValue)
{
	// Check value validity
	if (nValue == INT_INVALID)
		return;

	switch (eFlagID)
	{
	case FLAGID_INVALID:				// *** Invalid flag ID ***
		break;
	case FLAGID_CHANGEFLAG:				// Data/setting change flag
		m_bChangeFlag = nValue;
		break;
	case FLAGID_READONLYMODE:			// Read-only mode
		m_bReadOnlyMode = nValue;
		break;
	case FLAGID_LOCKSTATE:				// Lock state
		m_bLockState = nValue;
		break;
	case FLAGID_FORCECLOSING:			// Force closing by request
		m_bForceClose = nValue;
		break;
	case FLAGID_USEESCAPE:				// Use Escape key
		m_bUseEscape = nValue;
		break;
	case FLAGID_USEENTER:				// Use Enter key
		m_bUseEnter = nValue;
		break;
	case FLAGID_BKGRDCLRSET:			// Dialog background color is set
		m_bBkgrdColorSet = nValue;
		break;
	case FLAGID_TEXTCLRSET:				// Dialog text color is set
		m_bTextColorSet = nValue;
		break;
	case FLAGID_TOPMOSTSET:				// Dialog top-most position is set
		m_bTopMostSet = nValue;
		break;
	case FLAGID_INITSOUNDSET:			// Dialog initialize sound is set
		m_bInitSoundSet = nValue;
		break;
	default:
		// Set application-base-class flag value
		((SWinApp*)AfxGetApp())->SetFlagValue(eFlagID, nValue);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetChangeFlagValue
//	Description:	Get/set change flag value
//  Arguments:		bChangeFlag - Change flag
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL SDialog::GetChangeFlagValue(void) const
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

BOOL SDialog::CheckDataChangeState(void)
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

BOOL SDialog::CheckSettingChangeState(void)
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

AFX_INLINE BOOL SDialog::IsForceClosingByRequest(void) const
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
	return LRESULT(RESULT_SUCCESS);	// ERROR_SUCCESS
}
