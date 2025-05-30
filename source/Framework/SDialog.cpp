
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

#include "Framework\SElements.h"
#include "Framework\SWinApp.h"
#include "Framework\SDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;


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
	m_nDescendantCount = INT_NULL;
	SetFlagValue(AppFlagID::dialogUseEnterKey, TRUE);
	SetFlagValue(AppFlagID::dialogUseEscapeKey, TRUE);
	SetReturnFlag(ReturnFlag::Invalid);

	// Properties set flags
	m_bBkgrdColorSet = FALSE;
	m_bTextColorSet = FALSE;
	m_bTopMostSet = FALSE;
	m_bInitSoundSet = FALSE;

	// Lock state exception IDs
	m_paLockExceptionIDList = NULL;

	// Dialog anchor point
	m_ptAnchorPoint = POINT_NULL;

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
	m_nDescendantCount = INT_NULL;
	SetFlagValue(AppFlagID::dialogUseEnterKey, TRUE);
	SetFlagValue(AppFlagID::dialogUseEscapeKey, TRUE);
	SetReturnFlag(ReturnFlag::Invalid);

	// Properties set flags
	m_bBkgrdColorSet = FALSE;
	m_bTextColorSet = FALSE;
	m_bTopMostSet = FALSE;
	m_bInitSoundSet = FALSE;

	// Lock state exception IDs
	m_paLockExceptionIDList = NULL;

	// Dialog anchor point
	m_ptAnchorPoint = POINT_NULL;

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
	m_nDescendantCount = INT_NULL;
	SetFlagValue(AppFlagID::dialogUseEnterKey, TRUE);
	SetFlagValue(AppFlagID::dialogUseEscapeKey, TRUE);
	SetReturnFlag(ReturnFlag::Invalid);

	// Properties set flags
	m_bBkgrdColorSet = FALSE;
	m_bTextColorSet = FALSE;
	m_bTopMostSet = FALSE;
	m_bInitSoundSet = FALSE;

	// Lock state exception IDs
	m_paLockExceptionIDList = NULL;

	// Dialog anchor point
	m_ptAnchorPoint = POINT_NULL;

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
	ON_MESSAGE(SCM_NOTIFY_DIALOG_INIT, &SDialog::OnChildDialogInit)
	ON_MESSAGE(SCM_NOTIFY_DIALOG_DESTROY, &SDialog::OnChildDialogDestroy)
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

	// Initialize dialog resource ID map
	UpdateResourceIDMap();

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
	this->NotifyParent(SCM_NOTIFY_DIALOG_INIT, wParam, NULL);

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
	this->NotifyParent(SCM_NOTIFY_DIALOG_CLOSE, wParam, NULL);

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
	this->NotifyParent(SCM_NOTIFY_DIALOG_DESTROY, wParam, NULL);

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
		this->NotifyParent(SCM_NOTIFY_DIALOG_ACTIVE, wParam, NULL);
	}
	else if (nState == WA_INACTIVE) {
		this->NotifyParent(SCM_NOTIFY_DIALOG_INACTIVE, wParam, NULL);
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
	// Default
	CDialogEx::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnChildDialogInit
//	Description:	Handle event when a child dialog is initialized
//  Arguments:		wParam - Child dialog ID
//					lParam - Modal or modeless state
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT SDialog::OnChildDialogInit(WPARAM wParam, LPARAM lParam)
{
	// Update descendant dialog counter
	++m_nDescendantCount;

	// Default: Success
	return LRESULT(RESULT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnChildDialogDestroy
//	Description:	Handle event when a child dialog is closed/destroyed
//  Arguments:		wParam - Child dialog ID
//					lParam - Modal or modeless state
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT SDialog::OnChildDialogDestroy(WPARAM wParam, LPARAM lParam)
{
	// Update descendant dialog counter
	--m_nDescendantCount;

	// Default: Success
	return LRESULT(RESULT_SUCCESS);
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
	if (GetFlagValue(AppFlagID::dialogSetMinSize) == TRUE) {
		pMinMaxInfo->ptMinTrackSize.x = m_szMinSize.cx;
		pMinMaxInfo->ptMinTrackSize.y = m_szMinSize.cy;
	}
	if (GetFlagValue(AppFlagID::dialogSetMaxSize) == TRUE) {
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
			if (GetFlagValue(AppFlagID::dialogUseEscapeKey) != TRUE) {
				// Do not use Escape button
				return TRUE;
			}
			break;
		case VK_RETURN:
			if (GetFlagValue(AppFlagID::dialogUseEnterKey) != TRUE) {
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
//	Function name:	DoModal
//	Description:	Run a modal dialog and return the result
//  Arguments:		None
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

INT_PTR SDialog::DoModal()
{
	return CDialogEx::DoModal();
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
	// Get control manager
	if (m_pCtrlManager == NULL) return;

	// Update control data
	m_pCtrlManager->UpdateData(NULL);
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
	LONG dx = 0, dy = 0;

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
		LONG nHCenter = rcCurPos.right - rcCurPos.left;
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
		LONG nVCenter = rcCurPos.bottom - rcCurPos.top;
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

void SDialog::MoveDialog(LONG dx, LONG dy, LPRECT lpNewRect /* = NULL */)
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
		LONG x = rcCurPos.left;
		LONG y = rcCurPos.top;
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
	if ((m_szRegisterSize.cx > INT_INVALID) && (m_szRegisterSize.cy > INT_INVALID)) {
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
	LONG lCurWidth = (rcCurRect.right - rcCurRect.left);
	LONG lCurHeight = (rcCurRect.bottom - rcCurRect.top);
	if ((lCurWidth == m_szDefaultSize.cx) && (lCurHeight == m_szDefaultSize.cy))
		return;

	// Reset to default
	m_szRegisterSize.cx = m_szDefaultSize.cx;
	m_szRegisterSize.cy = m_szDefaultSize.cy;

	// Resize dialog
	ResizeDialog(TRUE);
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
	logItemDialogEvent.SetCategory(usEvent);
	logItemDialogEvent.SetTime(GetCurSysTime());
	logItemDialogEvent.SetProcessID();
	if (lpszDescription != NULL) {
		// Include event description
		logItemDialogEvent.SetLogString(lpszDescription);
	}
	if (pDetailInfo != NULL) {
		// Include event detail info data
		for (int nIndex = 0; nIndex < pDetailInfo->size(); nIndex++) {
			logItemDialogEvent.AddDetail(pDetailInfo->at(nIndex));
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
	{
		// Button ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, nButtonID);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(nButtonID)));
	}

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
	{
		// Checkbox ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, nCheckboxID);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(nCheckboxID)));

		// Checkbox checked state
		logDetailInfo.AddDetail(EventDetail::CheckState, pChkBtn->GetCheck());
	}

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
	{
		// Radio button ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, nRadButtonID);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(nRadButtonID)));

		// Radio button checked state
		logDetailInfo.AddDetail(EventDetail::CheckState, pRadBtn->GetCheck());
	}

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
	CString strComboCaption;
	LOGDETAILINFO logDetailInfo;
	{
		// Combo-box ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, nComboID);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(nComboID)));

		// Combo-box control info
		SControlManager* pCtrlMan = GetControlManager();
		if (pCtrlMan != NULL) {
			SCtrlInfoWrap* pComboWrap = pCtrlMan->GetControl(nComboID);
			if (pComboWrap != NULL) {
				// Combo-box caption
				pComboWrap->GetCaption(strComboCaption);

				// Combo-box current selection string
				INT_PTR nCurSel = pComboWrap->GetInteger();
				CStringArray arrDataList;
				pComboWrap->GetStringArray(arrDataList);
				if ((!arrDataList.IsEmpty()) && (arrDataList.GetCount() > nCurSel)) {
					logDetailInfo.AddDetail(EventDetail::Selection, arrDataList.GetAt(nCurSel));
				}
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
	CString strEditBoxCaption;
	LOGDETAILINFO logDetailInfo;
	{
		// Edit box ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, nEditID);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(nEditID)));

		// Edit box control info
		SControlManager* pCtrlMan = GetControlManager();
		if (pCtrlMan != NULL) {
			SCtrlInfoWrap* pEditBoxWrap = pCtrlMan->GetControl(nEditID);
			if (pEditBoxWrap != NULL) {
				// Edit box caption
				pEditBoxWrap->GetCaption(strEditBoxCaption);

				// Edit box content
				CString strContent;
				pEditBoxWrap->GetString(strContent);
				logDetailInfo.AddDetail(EventDetail::DataValue, strContent);
			}
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
	CString strListBoxCaption;
	LOGDETAILINFO logDetailInfo;
	{
		// List box ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, nListBoxID);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(nListBoxID)));

		// List box control info
		SControlManager* pCtrlMan = GetControlManager();
		if (pCtrlMan != NULL) {
			SCtrlInfoWrap* pListBoxWrap = pCtrlMan->GetControl(nListBoxID);
			if (pListBoxWrap != NULL) {
				// List box caption
				pListBoxWrap->GetCaption(strListBoxCaption);

				// List box current selection string
				INT_PTR nCurSel = pListBoxWrap->GetInteger();
				CStringArray arrDataList;
				pListBoxWrap->GetStringArray(arrDataList);
				if ((!arrDataList.IsEmpty()) && (arrDataList.GetCount() > nCurSel)) {
					logDetailInfo.AddDetail(EventDetail::Selection, arrDataList.GetAt(nCurSel));
				}
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
	{
		// Spin control ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, nSpinCtrlID);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(nSpinCtrlID)));
	}

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
	{
		// Menu item ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, nMenuItemID);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(nMenuItemID)));
	}

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

	// Update dialog control attributes
	UpdateDialogManagement();
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
	// Update dialog control attributes
	UpdateDialogManagement();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetButtonIcon
//	Description:	Set icon for dialog button
//  Arguments:		nButtonID	   - ID of button
//					nIconID		   - ID of icon resource
//					bReUpdateTitle - Whether to reupdate button title
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetButtonIcon(UINT nButtonID, UINT nIconID, BOOL bReUpdateTitle /* = FALSE */)
{
	// Backup title
	CString strButtonTitle;
	if (bReUpdateTitle == TRUE) {
		GetDlgItemText(nButtonID, strButtonTitle);
	}

	// Icon size
	int cx = BUTTON_ICON_WIDTH;
	int cy = BUTTON_ICON_HEIGHT;

	// Load icon from resource
	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	HICON hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(nIconID), IMAGE_ICON, cx, cy, LR_DEFAULTCOLOR);
	if (hIcon == NULL)
		return;

	// Draw button icon
	SendDlgItemMessage(nButtonID, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

	// Restore title
	if (bReUpdateTitle == TRUE) {
		SetDlgItemText(nButtonID, strButtonTitle);
	}
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
	LONG lOrgX = INT_INVALID, lOrgY = INT_INVALID;

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
		if ((lOrgX == INT_INVALID) || (rcCtrlWnd.left <= lOrgX)) {
			lOrgX = rcCtrlWnd.left;
		}

		// Find smallest Y
		if ((lOrgY == INT_INVALID) || (rcCtrlWnd.top <= lOrgY)) {
			lOrgY = rcCtrlWnd.top;
		}
	}

	// Calculate moving distance
	int nDeltaX = ptNewPosition.x - lOrgX;
	int nDeltaY = ptNewPosition.y - lOrgY;

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
	LONG lNewX = 0, lNewY = 0;

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
			lNewX = rcCtrlWnd.left + nDistance;
			lNewY = rcCtrlWnd.top;
			pCtrlWnd->SetWindowPos(NULL, lNewX, lNewY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		// Move vertical direction
		else if (nDirection == MOVDIR_VERTICAL) {
			lNewX = rcCtrlWnd.left;
			lNewY = rcCtrlWnd.top + nDistance;
			pCtrlWnd->SetWindowPos(NULL, lNewX, lNewY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ShowItem
//	Description:	Show/hide dialog item with corresponding ID
//  Arguments:		pDlgItemWnd - Dialog item window pointer
//					bVisible	- Visible state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::ShowItem(CWnd* pDlgItemWnd, BOOL bVisible)
{
	// Check dialog item validity
	ASSERT(pDlgItemWnd != NULL);
	if (pDlgItemWnd == NULL) return;

	// If new control state is the same as current state, do nothing
	if (pDlgItemWnd->IsWindowVisible() == bVisible)
		return;

	// Show/hide control
	pDlgItemWnd->ShowWindow(bVisible);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableItem
//	Description:	Enable/disable dialog item with corresponding ID
//  Arguments:		nDlgItemID - Dialog item ID
//					bEnabled   - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::EnableItem(CWnd* pDlgItemWnd, BOOL bEnabled)
{
	// Check dialog item validity
	ASSERT(pDlgItemWnd != NULL);
	if (pDlgItemWnd == NULL) return;

	// If new control state is the same as current state, do nothing
	if (pDlgItemWnd->IsWindowEnabled() == bEnabled)
		return;

	// Enable/disable control
	pDlgItemWnd->EnableWindow(bEnabled);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupDialogItemState
//	Description:	Setup properties and values for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::SetupDialogItemState(void)
{
	// Update dialog control attributes
	UpdateDialogManagement();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RefreshDialogItemState
//	Description:	Refresh and update state for dialog items
//  Arguments:		bRecheckState - Recheck all item's state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SDialog::RefreshDialogItemState(BOOL bRecheckState /* = FALSE */)
{
	// Update dialog control attributes
	UpdateDialogManagement();
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
	// Update data for dialog control management
	UpdateDialogManagement();
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

int SDialog::GetAppOption(AppOptionID eAppOptionID, BOOL bTemp /* = FALSE */) const
{
	int nResult = INT_INVALID;
	int nTempResult = INT_INVALID;

	// Acquire option value from application main window
	SDialog* pMainDlg = (SDialog*)AfxGetMainWnd();
	if (pMainDlg != NULL) {
		nResult = pMainDlg->GetAppOption(eAppOptionID, FALSE);
		nTempResult = pMainDlg->GetAppOption(eAppOptionID, TRUE);
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

int SDialog::GetFlagValue(AppFlagID eFlagID) const
{
	int nValue = FLAG_OFF;

	switch (eFlagID)
	{
	// Special dialog-base flags (not managed by FlagManager)
	case AppFlagID::dialogSetMinSize:
		nValue = ((m_szMinSize.cx >= 0) &&
			(m_szMinSize.cy >= 0));
		break;
	case AppFlagID::dialogSetMaxSize:
		nValue = ((m_szMaxSize.cx > m_szMinSize.cx) &&
			(m_szMaxSize.cy > m_szMinSize.cy));
		break;

	// Dialog-base properties/flags
	case AppFlagID::dialogDataChanged:
	case AppFlagID::dialogReturnFlag:
	case AppFlagID::dialogReadOnlyMode:
	case AppFlagID::dialogExpanded:
	case AppFlagID::dialogLockState:
	case AppFlagID::dialogForceClosing:
	case AppFlagID::dialogUseEscapeKey:
	case AppFlagID::dialogUseEnterKey:
	case AppFlagID::dialogSetBackgroundColor:
	case AppFlagID::dialogSetTextColor:
	case AppFlagID::dialogSetTopMost:
	case AppFlagID::dialogSetInitSound:
		nValue = m_flagManager.GetFlagValue(eFlagID);
		break;

	default:
		// Request the flag value from application
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

void SDialog::SetFlagValue(AppFlagID eFlagID, int nValue)
{
	// Check value validity
	if (nValue == INT_INVALID)
		return;

	switch (eFlagID)
	{
	// Special flags (not managed by FlagManager)
	case AppFlagID::dialogSetMinSize:
	case AppFlagID::dialogSetMaxSize:
		break;

	// Dialog-base properties/flags
	case AppFlagID::dialogDataChanged:
	case AppFlagID::dialogReturnFlag:
	case AppFlagID::dialogReadOnlyMode:
	case AppFlagID::dialogExpanded:
	case AppFlagID::dialogLockState:
	case AppFlagID::dialogForceClosing:
	case AppFlagID::dialogUseEscapeKey:
	case AppFlagID::dialogUseEnterKey:
	case AppFlagID::dialogSetBackgroundColor:
	case AppFlagID::dialogSetTextColor:
	case AppFlagID::dialogSetTopMost:
	case AppFlagID::dialogSetInitSound:
		m_flagManager.SetFlagValue(eFlagID, nValue);
		break;

	default:
		// Let the application manage the flags
		((SWinApp*)AfxGetApp())->SetFlagValue(eFlagID, nValue);
		break;
	}
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
	SetFlagValue(AppFlagID::dialogForceClosing, TRUE);

	// Default: Close the dialog
	this->PostMessage(WM_CLOSE);

	// Request accepted
	return LRESULT(RESULT_SUCCESS);	// ERROR_SUCCESS
}
