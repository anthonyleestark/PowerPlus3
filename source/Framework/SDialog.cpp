/**
 * @file		SDialog.h
 * @brief		Custom base class for dialogs (source file)
 * @author		AnthonyLeeStark
 * @date		2024.07.06
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "Framework/SElements.h"
#include "Framework/SWinApp.h"
#include "Framework/SDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;


// Implement methods for SDialog
IMPLEMENT_DYNAMIC(SDialog, CDialogEx)


/**
 * @brief	Constructor
 */
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
	SetFlagValue(AppFlagID::dialogUseEnterKey, true);
	SetFlagValue(AppFlagID::dialogUseEscapeKey, true);
	SetReturnFlag(ReturnFlag::Invalid);

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
	m_clBkgrdColor = Color::White;
	m_clTextColor = Color::Black;
}

SDialog::SDialog(unsigned nIDTemplate, CWnd* pParentWnd /* = NULL */) : CDialogEx(nIDTemplate, pParentWnd)
{
	// Parent window
	m_pParentWnd = pParentWnd;

	// Tooltip control
	m_pToolTip = NULL;

	// Dialog control management
	m_pCtrlManager = NULL;

	// Dialog special flags
	m_nDescendantCount = INT_NULL;
	SetFlagValue(AppFlagID::dialogUseEnterKey, true);
	SetFlagValue(AppFlagID::dialogUseEscapeKey, true);
	SetReturnFlag(ReturnFlag::Invalid);

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
	m_clBkgrdColor = Color::White;
	m_clTextColor = Color::Black;
}

SDialog::SDialog(const wchar_t* templateName, CWnd* pParentWnd /* = NULL */) : CDialogEx(templateName, pParentWnd)
{
	// Parent window
	m_pParentWnd = pParentWnd;

	// Tooltip control
	m_pToolTip = NULL;

	// Dialog control management
	m_pCtrlManager = NULL;

	// Dialog special flags
	m_nDescendantCount = INT_NULL;
	SetFlagValue(AppFlagID::dialogUseEnterKey, true);
	SetFlagValue(AppFlagID::dialogUseEscapeKey, true);
	SetReturnFlag(ReturnFlag::Invalid);

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
	m_clBkgrdColor = Color::White;
	m_clTextColor = Color::Black;
}

/**
 * @brief	Destructor
 */
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
		m_pBkgrdBrush->DeleteObject();
		delete m_pBkgrdBrush;
		m_pBkgrdBrush = NULL;
	}
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
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


/**
 * @brief	Default method for [OK] button
 * @param	None
 * @return	None
 */
void SDialog::OnOK()
{
	// Default
	CDialogEx::OnOK();
}

/**
 * @brief	Default method for [Cancel] button
 * @param	None
 * @return	None
 */
void SDialog::OnCancel()
{
	// Default
	CDialogEx::OnCancel();
}

/**
 * @brief	Default method for dialog initialization
 * @param	None
 * @return	Default
 */
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
		GetToolTipCtrl()->Activate(true);
	}

	// If dialog custom caption is set
	if (!m_strCaption.IsEmpty()) {

		// Update dialog caption
		this->SetWindowText(m_strCaption);
	}
	else {
		// Use defined caption in resource
		const int captionLength = this->GetWindowTextLength();
		std::vector<wchar_t> tempBuff(captionLength + 1);
		this->GetWindowText(tempBuff.data(), captionLength + 1);
		String resourceCaption = tempBuff.data();
		this->SetCaption(resourceCaption);
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
	this->CreateBrush();

	// If dialog is set as top-most 
	if (GetFlagValue(AppFlagID::dialogSetTopMost) == true) {
		// Set window position
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	// If init sound is set
	if (GetFlagValue(AppFlagID::dialogSetInitSound) == true) {
		MessageBeep(0xFFFFFFFF);
	}
	
	// Register dialog control management
	RegisterDialogManagement();

	// Get dialog ID
	WPARAM wParam = static_cast<WPARAM>(GetDialogID());

	// Notify parent window about dialog initialization
	this->NotifyParent(SCM_NOTIFY_DIALOG_INIT, wParam, NULL);

	return true;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void SDialog::OnClose()
{
	// Get dialog ID
	WPARAM wParam = static_cast<WPARAM>(GetDialogID());

	// Notify parent window about dialog closing
	this->NotifyParent(SCM_NOTIFY_DIALOG_CLOSE, wParam, NULL);

	// Close dialog
	CDialogEx::OnClose();
}

/**
 * @brief	Default method for dialog destroying
 * @param	None
 * @return	None
 */
void SDialog::OnDestroy()
{
	// Pre-destroy dialog
	SDialog::PreDestroyDialog();

	// Get dialog ID
	WPARAM wParam = static_cast<WPARAM>(GetDialogID());

	// Notify parent window about dialog destroying
	this->NotifyParent(SCM_NOTIFY_DIALOG_DESTROY, wParam, NULL);

	// Destroy dialog
	CDialogEx::OnDestroy();
}

/**
 * @brief	Default method for dialog activate message handling
 * @param	Default
 * @return	None
 */
void SDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	// Default
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// Get dialog ID
	WPARAM wParam = static_cast<WPARAM>(GetDialogID());

	// Notify parent window about dialog active/inactive state
	if (nState == WA_ACTIVE) {
		this->NotifyParent(SCM_NOTIFY_DIALOG_ACTIVE, wParam, NULL);
	}
	else if (nState == WA_INACTIVE) {
		this->NotifyParent(SCM_NOTIFY_DIALOG_INACTIVE, wParam, NULL);
	}
}

/**
 * @brief	Default method for handling mouse moving event within dialog
 * @param	Default
 * @return	None
 */
void SDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	// Default
	CDialogEx::OnMouseMove(nFlags, point);
}

/**
 * @brief	Handle event when a child dialog is initialized
 * @param	wParam - Child dialog ID
 * @param	lParam - Modal or modeless state
 * @return	LRESULT
 */
LRESULT SDialog::OnChildDialogInit(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Update descendant dialog counter
	++m_nDescendantCount;

	// Default: Success
	return LRESULT(Result::Success);
}

/**
 * @brief	Handle event when a child dialog is closed/destroyed
 * @param	wParam - Child dialog ID
 * @param	lParam - Modal or modeless state
 * @return	LRESULT
 */
LRESULT SDialog::OnChildDialogDestroy(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Update descendant dialog counter
	--m_nDescendantCount;

	// Default: Success
	return LRESULT(Result::Success);
}

/**
 * @brief	Dialog get min/max info handler
 * @param	Default
 * @return	None
 */
void SDialog::OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo)
{
	// Fix min/max size
	if (GetFlagValue(AppFlagID::dialogSetMinSize) == true) {
		pMinMaxInfo->ptMinTrackSize.x = m_szMinSize.cx;
		pMinMaxInfo->ptMinTrackSize.y = m_szMinSize.cy;
	}
	if (GetFlagValue(AppFlagID::dialogSetMaxSize) == true) {
		pMinMaxInfo->ptMaxTrackSize.x = m_szMaxSize.cx;
		pMinMaxInfo->ptMaxTrackSize.y = m_szMaxSize.cy;
	}

	// Default
	CDialogEx::OnGetMinMaxInfo(pMinMaxInfo);
}

/**
 * @brief	Default method for pre-translate message
 * @param	pMsg - Default
 * @return	None
 */
BOOL SDialog::PreTranslateMessage(MSG* pMsg)
{
	// Handle system key pressed message
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) 
		{
		case VK_ESCAPE:
			if (GetFlagValue(AppFlagID::dialogUseEscapeKey) != true) {
				// Do not use Escape button
				return true;
			}
			break;

		case VK_RETURN:
			if (GetFlagValue(AppFlagID::dialogUseEnterKey) != true) {
				// Do not use Enter button
				return true;
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

/**
 * @brief	Execute some actions before destroying dialog
 * @param	None
 * @return	int
 */
int SDialog::PreDestroyDialog()
{
	// Unregister dialog control management
	UnregisterDialogManagement();

	return Result::Success;
}

/**
 * @brief	Handle app window messages
 * @param	message - Message ID
 * @param	wParam - First param (HIWORD)
 * @param	lParam - Second param (LOWORD)
 * @return	LRESULT
 */
LRESULT SDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// Default
	return CDialogEx::WindowProc(message, wParam, lParam);
}

/**
 * @brief	Run a modal dialog and return the result
 * @param	None
 * @return	INT_PTR
 */
INT_PTR SDialog::DoModal()
{
	return CDialogEx::DoModal();
}

/**
 * @brief	Post a message to parent window for notifying some special events
 * @param	nMsg   - Message
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	true/false
 */
bool SDialog::NotifyParent(unsigned nMsg, WPARAM wParam, LPARAM lParam)
{
	bool bRetNotify = false;

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

/**
 * @brief	Similar to NotifyParent but this will send the message
					and get the result immediately instead of posting it
 * @param	nMsg   - Message
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	LRESULT
 */
LRESULT SDialog::SendMessageToParent(unsigned nMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRetNotify = Result::Failure;

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

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int SDialog::RegisterDialogManagement(void)
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

/**
 * @brief	Update dialog control management
 * @param	None
 * @return	None
 */
void SDialog::UpdateDialogManagement(void)
{
	// Get control manager
	if (m_pCtrlManager == NULL) return;

	// Update control data
	m_pCtrlManager->UpdateData(NULL);
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool SDialog::UnregisterDialogManagement(void)
{
	// Clean-up dialog control management
	if (m_pCtrlManager == NULL)
		return false;

	if (m_pCtrlManager != NULL) {
		if (!m_pCtrlManager->DeleteAll())
			return false;

		delete m_pCtrlManager;
		m_pCtrlManager = NULL;
	}

	return true;
}

/**
 * @brief	Add dialog extended styles
 * @param	dwAddStyle - Styles to add
 * @return	true/false
 */
bool SDialog::AddStyle(DWORD dwAddStyle)
{
	DWORD dwStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	dwStyle |= dwAddStyle;
	LONG lRet = SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, dwStyle);
	return (lRet != 0);
}

/**
 * @brief	Remove dialog extended styles
 * @param	dwRemoveStyle - Styles to remove
 * @return	true/false
 */
bool SDialog::RemoveStyle(DWORD dwRemoveStyle)
{
	DWORD dwStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	dwStyle &= ~dwRemoveStyle;
	LONG lRet = SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, dwStyle);
	return (lRet != 0);
}

/**
 * @brief	Add a control ID to lock state exception list
 * @param	nID - Control ID
 * @return	None
 */
void SDialog::AddLockStateException(unsigned nID)
{
	// Initialize the list if not yet allocated
	if (m_paLockExceptionIDList == NULL) {
		m_paLockExceptionIDList = new UIntArray;
		m_paLockExceptionIDList->clear();
	}

	// Loop through all list and find if item existed in list
	int nItemNum = m_paLockExceptionIDList->size();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		if (m_paLockExceptionIDList->at(nIndex) == nID) {
			// No need to add
			return;
		}
	}

	// Add to list
	m_paLockExceptionIDList->push_back(nID);
}

/**
 * @brief	Remove a control ID out of lock state exception list
 * @param	nID - Control ID
 * @return	None
 */
void SDialog::RemoveLockStateException(unsigned nID)
{
	// If the list is not yet allocated, no need to remove
	if (m_paLockExceptionIDList == NULL)
		return;

	// Loop through all list and remove item
	int nItemNum = m_paLockExceptionIDList->size();
	for (int nIndex = (nItemNum - 1); nIndex >= 0; nIndex--) {
		if (m_paLockExceptionIDList->at(nIndex) == nID) {
			m_paLockExceptionIDList->erase(m_paLockExceptionIDList->begin() + nIndex);
		}
	}
}

/**
 * @brief	Remove all lock state exception list
 * @param	nID - Control ID
 * @return	None
 */
void SDialog::ResetLockStateExceptionList(void)
{
	if (m_paLockExceptionIDList != NULL) {
		m_paLockExceptionIDList->clear();
	}
}

/**
 * @brief	Move dialog position and return new rectangle
 * @param	ptPosition - Dialog position (anchor point)
 * @param	lpNewRect  - New dialog rectangle (OUT)
 * @return	None
 */
void SDialog::MoveDialog(POINT ptPosition, LPRECT lpNewRect /* = NULL */)
{
	// Get current dialog rectangle
	CRect rcCurPos;
	this->GetWindowRect(&rcCurPos);

	// Get dialog alignment
	unsigned nAlign = GetAlignment();

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

/**
 * @brief	Move dialog position as given delta and return new rect
 * @param	dx		  - Horizontal delta
 * @param	dy		  - Vertical delta
 * @param	lpNewRect - New dialog rectangle (OUT)
 * @return	None
 */
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
		bool bRet = this->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		// If moving successfully, update new rectangle 
		if ((bRet == true) && (lpNewRect != NULL)) {
			lpNewRect->left = rcCurPos.left;
			lpNewRect->right = rcCurPos.right;
			lpNewRect->top = rcCurPos.top;
			lpNewRect->bottom = rcCurPos.bottom;
		}
	}
}

/**
 * @brief	Resize dialog (after change size)
 * @param	bCenterDialog - Replace dialog to center
 * @return	None
 */
void SDialog::ResizeDialog(bool bCenterDialog)
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
	if (bCenterDialog == true) {
		this->CenterWindow(GetParentWnd());
	}
}

/**
 * @brief	Reset dialog size to default
 * @param	None
 * @return	None
 */
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
	ResizeDialog(true);
}

/**
 * @brief	Set new display area, update margin and resize dialog
 * @param	rcNewDispArea - New display area rectangle
 * @param	bResizeDialog - Resize dialog
 * @param	bCenter		  - Center rectangle area
 * @return	None
 */
void SDialog::SetDisplayArea(RECT rcNewDispArea, bool bResizeDialog, bool bCenter)
{
	// Get current margin
	CRect rcCurMargin;
	this->GetMargin(&rcCurMargin);

	// Horizontal center margin
	bool bHorzCenter = false;
	if (rcCurMargin.left == rcCurMargin.right) {
		bHorzCenter = true;
	}

	// Vertical center margin
	bool bVertCenter = false;
	if (rcCurMargin.top == rcCurMargin.bottom) {
		bVertCenter = true;
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
	if ((bResizeDialog != true) && (bCenter == true)) {
		// Horizontal center margin
		if (bHorzCenter == true) {
			rcNewMargin.left = rcCurMargin.left + (szCurDispAreaSize.cx - szNewDispAreaSize.cx);
			rcNewMargin.right = rcNewMargin.left;
		}
		// Vertical center margin
		if (bVertCenter == true) {
			rcNewMargin.top = rcCurMargin.top + (szCurDispAreaSize.cy - szNewDispAreaSize.cy);
			rcNewMargin.bottom = rcNewMargin.top;
		}
	}

	// Set new dialog rectangle top-left
	rcNewDlgRect.left = (rcNewDispArea.left - rcNewMargin.left) - rcDlgClientOffset.left;
	rcNewDlgRect.top = (rcNewDispArea.top - rcNewMargin.top) - rcDlgClientOffset.top;

	// If resize is specified
	if (bResizeDialog == true) {
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
		if (bCenter == true) {
			this->CenterWindow(GetParentWnd());
		}
	}

	// Save margin update
	this->SetLeftMargin(rcNewMargin.left);
	this->SetTopMargin(rcNewMargin.top);
	this->SetRightMargin(rcNewMargin.right);
	this->SetBottomMargin(rcNewMargin.bottom);
}

/**
 * @brief	Set dialog caption by resource string ID
 * @param	nResourceStringID - Resource string ID
 * @return	None
 */
void SDialog::SetCaptionFromResource(unsigned nResourceStringID)
{
	String captionString = StringUtils::LoadResourceString(nResourceStringID);
	ASSERT(!captionString.IsEmpty());
	if (!captionString.IsEmpty()) {
		this->SetCaption(captionString);
	}
}

/**
 * @brief	Set dialog caption by language string ID
 * @param	nLangStringID - Language string ID
 * @return	None
 */
void SDialog::SetCaptionFromLanguage(unsigned nLangStringID)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->GetAppLanguage();

	String captionString = GetLanguageString(pAppLang, nLangStringID);
	this->SetCaption(captionString);
}

/**
 * @brief	Create (or re-create) dialog owned brush
 * @param	None
 * @return	true/false
 */
bool SDialog::CreateBrush(void)
{
	if (GetFlagValue(AppFlagID::dialogSetBackgroundColor)) {

		// Re-create if brush existed
		if (m_pBkgrdBrush != NULL) {
			m_pBkgrdBrush->DeleteObject();
			delete m_pBkgrdBrush;
			m_pBkgrdBrush = NULL;
		}

		// Initialization
		m_pBkgrdBrush = new CBrush();
		if (m_pBkgrdBrush != NULL) {

			// Create background color brush
			return m_pBkgrdBrush->CreateSolidBrush(m_clBkgrdColor);
		}
	}

	return false;
}

/**
 * @brief	Register default caption for message box-es
 * @param	nCaptionID  - ID of message caption string
 * @param	lpszCaption	- Message caption string
 * @return	None
 */
void SDialog::RegisterMessageBoxCaption(unsigned nCaptionID)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->GetAppLanguage();
	String captionString = Constant::String::Empty;
	if (nCaptionID != NULL) {

		// Get language string caption
		String langCaption = GetLanguageString(pAppLang, nCaptionID);
		if (IS_NOT_NULL_STRING(langCaption)) {
			// Set caption string
			captionString = langCaption;
		}
	}

	// If caption is empty
	if (captionString.IsEmpty()) {
		// Use default app window caption
		captionString = ((SWinApp*)AfxGetApp())->GetAppWindowCaption();
	}

	// Register message box caption
	RegisterMessageBoxCaption(captionString);
}

/**
 * @brief	Display message box using language string ID
 * @param	nPromptID  - ID of prompt message string
 * @param	nCaptionID - ID of message caption string
 * @param	nStyle	   - Message box style
 * @return	int	- Result of message box
 */
int SDialog::DisplayMessageBox(unsigned nPromptID, unsigned nCaptionID /* = NULL */, unsigned nStyle /* = NULL */)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->GetAppLanguage();

	String messagePrompt = GetLanguageString(pAppLang, nPromptID);
	String messageCaption = ((SWinApp*)AfxGetApp())->GetAppWindowCaption();
	if (nCaptionID != NULL) {

		// Get language string caption
		String langCaption = GetLanguageString(pAppLang, nCaptionID);
		if (IS_NOT_NULL_STRING(langCaption))
			messageCaption = langCaption;
	}
	else {
		// Using registered message box caption
		if (!m_strMsgCaption.IsEmpty()) {
			messageCaption = m_strMsgCaption;
		}
	}

	// Display message box
	int nResult = DisplayMessageBox(messagePrompt, messageCaption, nStyle);

	return nResult;
}

/**
 * @brief	Display message box using language string
 * @param	prompt  - Message string
 * @param	caption - Message caption string
 * @param	nStyle	- Message box style
 * @return	int	- Result of message box
 */
int SDialog::DisplayMessageBox(const wchar_t* prompt, const wchar_t* caption /* = NULL */, unsigned nStyle /* = NULL */)
{
	// Set default style
	if (nStyle == NULL) {
		// Information message with [OK] button
		nStyle = MB_OK | MB_ICONINFORMATION;
	}

	// If caption is not set
	String messageCaption(caption);
	if (messageCaption.IsEmpty()) {

		// If message box caption is registered
		if (!m_strMsgCaption.IsEmpty()) {
			// Use registered caption
			messageCaption = m_strMsgCaption;
		}

		// Otherwise,
		else {
			// Use app window caption
			messageCaption = ((SWinApp*)AfxGetApp())->GetAppWindowCaption();
		}
	}
	
	// Display message box
	nStyle |= MB_SYSTEMMODAL;
	return MessageBox(prompt, messageCaption, nStyle);
}

/**
 * @brief	Output application event log
 * @param	usEvent		- Event ID
 * @param	description - Additional description
 * @param	pDetailInfo	- Log detail info (array pointer)
 * @return	None
 */
void SDialog::OutputEventLog(USHORT usEvent, const wchar_t* description /* = NULL */, LOGDETAILINFO* pDetailInfo /* = NULL */)
{
	// Prepare event log info
	LOGITEM logItemDialogEvent;
	logItemDialogEvent.SetCategory(usEvent);
	logItemDialogEvent.SetTime(DateTimeUtils::GetCurrentDateTime());
	logItemDialogEvent.SetProcessID();
	if (description) {
		// Include event description
		logItemDialogEvent.SetLogString(description);
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

/**
 * @brief	Output button event log
 * @param	usEvent	  - Event ID
 * @param	nButtonID - Button ID
 * @return	None
 */
void SDialog::OutputButtonLog(USHORT usEvent, unsigned nButtonID)
{
	// Prepare button event log info
	CButton* pButton = (CButton*)GetDlgItem(nButtonID);
	if (pButton == NULL) return;

	// Get button caption
	const int captionLength = pButton->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(captionLength + 1);
	pButton->GetWindowText(tempBuff.data(), captionLength + 1);
	String buttonCaption = tempBuff.data();

	// Detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Button ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, nButtonID);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(nButtonID)));
	}

	// Output button event log
	OutputEventLog(usEvent, buttonCaption, &logDetailInfo);
}

/**
 * @brief	Output checkbox event log
 * @param	usEvent		- Event ID
 * @param	nCheckboxID - Checkbox ID
 * @return	None
 */
void SDialog::OutputCheckBoxLog(USHORT usEvent, unsigned nCheckboxID)
{
	// Prepare checkbox event log info
	CButton* pChkBtn = (CButton*)GetDlgItem(nCheckboxID);
	if (pChkBtn == NULL) return;

	// Get checkbox caption
	const int captionLength = pChkBtn->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(captionLength + 1);
	pChkBtn->GetWindowText(tempBuff.data(), captionLength + 1);
	String checkBoxCaption = tempBuff.data();

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
	OutputEventLog(usEvent, checkBoxCaption, &logDetailInfo);
}

/**
 * @brief	Output radio button event log
 * @param	usEvent		- Event ID
 * @param	nCheckboxID - Checkbox ID
 * @return	None
 */
void SDialog::OutputRadButtonLog(USHORT usEvent, unsigned nRadButtonID)
{
	// Prepare radio button event log info
	CButton* pRadBtn = (CButton*)GetDlgItem(nRadButtonID);
	if (pRadBtn == NULL) return;

	// Get radio button caption
	const int captionLength = pRadBtn->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(captionLength + 1);
	pRadBtn->GetWindowText(tempBuff.data(), captionLength + 1);
	String radButtonCaption = tempBuff.data();

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
	OutputEventLog(usEvent, radButtonCaption, &logDetailInfo);
}

/**
 * @brief	Output combo-box event log
 * @param	usEvent	 - Event ID
 * @param	nComboID - Combo-box ID
 * @return	None
 */
void SDialog::OutputComboBoxLog(USHORT usEvent, unsigned nComboID)
{
	// Prepare combo-box event log info
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nComboID);
	if (pCombo == NULL) return;

	// Detail info
	String comboBoxCaption;
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
				pComboWrap->GetCaption(comboBoxCaption);

				// Combo-box current selection string
				size_t nCurSel = pComboWrap->GetInteger();
				StringArray arrDataList;
				pComboWrap->GetStringArray(arrDataList);
				if ((!arrDataList.empty()) && (arrDataList.size() > nCurSel)) {
					logDetailInfo.AddDetail(EventDetail::Selection, arrDataList.at(nCurSel));
				}
			}
		}
	}

	// Output combo-box event log
	OutputEventLog(usEvent, comboBoxCaption, &logDetailInfo);
}

/**
 * @brief	Output edit box event log
 * @param	usEvent	- Event ID
 * @param	nEditID - Edit box ID
 * @return	None
 */
void SDialog::OutputEditBoxLog(USHORT usEvent, unsigned nEditID)
{
	// Prepare edit box event log info
	CEdit* pEdit = (CEdit*)GetDlgItem(nEditID);
	if (pEdit == NULL) return;

	// Detail info
	String editBoxCaption;
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
				pEditBoxWrap->GetCaption(editBoxCaption);

				// Edit box content
				String editBoxContent;
				pEditBoxWrap->GetString(editBoxContent);
				logDetailInfo.AddDetail(EventDetail::DataValue, editBoxContent);
			}
		}
	}

	// Output edit box event log
	OutputEventLog(usEvent, editBoxCaption, &logDetailInfo);
}

/**
 * @brief	Output list box event log
 * @param	usEvent	   - Event ID
 * @param	nListBoxID - List box ID
 * @return	None
 */
void SDialog::OutputListBoxLog(USHORT usEvent, unsigned nListBoxID)
{
	// Prepare list box event log info
	CListBox* pListBox = (CListBox*)GetDlgItem(nListBoxID);
	if (pListBox == NULL) return;

	// Detail info
	String listBoxCaption;
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
				pListBoxWrap->GetCaption(listBoxCaption);

				// List box current selection string
				size_t nCurSel = pListBoxWrap->GetInteger();
				StringArray arrDataList;
				pListBoxWrap->GetStringArray(arrDataList);
				if ((!arrDataList.empty()) && (arrDataList.size() > nCurSel)) {
					logDetailInfo.AddDetail(EventDetail::Selection, arrDataList.at(nCurSel));
				}
			}
		}
	}

	// Output list box event log
	OutputEventLog(usEvent, listBoxCaption, &logDetailInfo);
}

/**
 * @brief	Output spin control event log
 * @param	usEvent		- Event ID
 * @param	nSpinCtrlID - Spin control ID
 * @return	None
 */
void SDialog::OutputSpinCtrlLog(USHORT usEvent, unsigned nSpinCtrlID)
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

/**
 * @brief	Output menu event log
 * @param	usEvent		- Event ID
 * @param	nMenuItemID - Menu item ID
 * @return	None
 */
void SDialog::OutputMenuLog(USHORT usEvent, unsigned nMenuItemID)
{
	// Prepare menu event log info
	// Get menu item title from language table
	String menuItemCaption;
	LANGTABLE_PTR pLanguage = ((SWinApp*)AfxGetApp())->GetAppLanguage();
	if (pLanguage != NULL) {
		menuItemCaption = GetLanguageString(pLanguage, nMenuItemID);
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
	OutputEventLog(usEvent, menuItemCaption, &logDetailInfo);
}

/**
 * @brief	Setup language for dialog controls
 * @param	None
 * @return	None
 */
void SDialog::SetupLanguage(void)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->GetAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		unsigned nID = pWndChild->GetDlgCtrlID();

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

/**
 * @brief	Setup data for combo-boxes
 * @param	nComboID	- ID of combo box
 * @param	ptrLanguage - Language package pointer
 * @return	None
 */
void SDialog::SetupComboBox(unsigned /*nComboID*/, LANGTABLE_PTR /*ptrLanguage*/)
{
	// Update dialog control attributes
	UpdateDialogManagement();
}

/**
 * @brief	Set icon for dialog button
 * @param	nButtonID	   - ID of button
 * @param	nIconID		   - ID of icon resource
 * @param	bReUpdateTitle - Whether to reupdate button title
 * @return	None
 */
void SDialog::SetButtonIcon(unsigned nButtonID, unsigned nIconID, bool bReUpdateTitle /* = false */)
{
	// Get button
	CWnd* pButton = GetDlgItem(nButtonID);
	if (!pButton) return;

	// Backup title
	String buttonTitle;
	if (bReUpdateTitle == true) {
		const int captionLength = pButton->GetWindowTextLength();
		std::vector<wchar_t> tempBuff(captionLength + 1);
		pButton->GetWindowText(tempBuff.data(), captionLength + 1);
		buttonTitle = tempBuff.data();
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
	if (bReUpdateTitle == true) {
		SetDlgItemText(nButtonID, buttonTitle);
	}
}

/**
 * @brief	Update control text label
 * @param	nCtrlID	   - ID of control item
 * @param	newCaption - New caption string
 * @return	None
 */
void SDialog::UpdateItemText(unsigned nCtrlID, const wchar_t* newCaption)
{
	// Get item by ID
	CWnd* pCtrlWnd = this->GetDlgItem(nCtrlID);
	if (pCtrlWnd == NULL)
		return;

	// Update item text
	pCtrlWnd->SetWindowText(newCaption);
}

/**
 * @brief	Update control text label
 * @param	nCtrlID		  - ID of control item
 * @param	nNewCaptionID - New caption language string ID
 * @param	ptrLanguage   - Language package pointer
 * @return	None
 */
void SDialog::UpdateItemText(unsigned nCtrlID , unsigned nNewCaptionID /* = NULL */, LANGTABLE_PTR ptrLanguage /* = NULL */)
{
	// Check language package validity
	if (ptrLanguage == NULL) {
		ptrLanguage = ((SWinApp*)AfxGetApp())->GetAppLanguage();
		if (ptrLanguage == NULL)
			return;
	}

	// Get caption language string
	String wndItemText;
	if (nNewCaptionID != NULL) {
		// Get new caption
		wndItemText = GetLanguageString(ptrLanguage, nNewCaptionID);
	}
	else {
		// Get its own caption string ID
		wndItemText = GetLanguageString(ptrLanguage, nCtrlID);
	}

	// Update item text
	UpdateItemText(nCtrlID, wndItemText);
}

/**
 * @brief	Set control text label
 * @param	pCtrlWnd	- Pointer of control window
 * @param	nCtrlID		- ID of control item
 * @param	ptrLanguage - Language package pointer
 * @return	None
 */
void SDialog::SetControlText(CWnd* pCtrlWnd, unsigned nCtrlID, LANGTABLE_PTR ptrLanguage /* = NULL */)
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
	String wndItemText = GetLanguageString(ptrLanguage, nCtrlID);
	
	// Set control text
	pCtrlWnd->SetWindowText(wndItemText);
}

/**
 * @brief	Move a group of dialog items to specific position
 * @param	arrCtrlIDGroup  - Array of control IDs
 * @param	ptNewPosition	- New position
 * @return	None
 */
void SDialog::MoveItemGroup(const UIntArray& arrCtrlIDGroup, POINT ptNewPosition)
{
	// Check data validity
	if (arrCtrlIDGroup.empty())
		return;

	RECT rcCtrlWnd;
	CWnd* pCtrlWnd = NULL;
	LONG lOrgX = INT_INVALID, lOrgY = INT_INVALID;

	// Find the original point
	for (int nIndex = 0; nIndex < arrCtrlIDGroup.size(); nIndex++)
	{
		// Get item
		pCtrlWnd = GetDlgItem(arrCtrlIDGroup.at(nIndex));
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
	for (int nIndex = 0; nIndex < arrCtrlIDGroup.size(); nIndex++)
	{
		// Get item
		pCtrlWnd = GetDlgItem(arrCtrlIDGroup.at(nIndex));
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

/**
 * @brief	Move a group of dialog items to specific position
 * @param	arrCtrlIDGroup  - Array of control IDs
 * @param	nDirection		- Moving direction
 * @param	nDistance		- Moving distance
 * @return	None
 */
void SDialog::MoveItemGroup(const UIntArray& arrCtrlIDGroup, int nDirection, int nDistance)
{
	// Check data validity
	if (arrCtrlIDGroup.empty())
		return;

	RECT rcCtrlWnd;
	CWnd* pCtrlWnd = NULL;
	LONG lNewX = 0, lNewY = 0;

	// Loop through each item and move
	for (int nIndex = 0; nIndex < arrCtrlIDGroup.size(); nIndex++)
	{
		// Get item
		pCtrlWnd = GetDlgItem(arrCtrlIDGroup.at(nIndex));
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

/**
 * @brief	Show/hide dialog item with corresponding ID
 * @param	pDlgItemWnd - Dialog item window pointer
 * @param	bVisible	- Visible state
 * @return	None
 */
void SDialog::ShowItem(CWnd* pDlgItemWnd, bool bVisible)
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

/**
 * @brief	Enable/disable dialog item with corresponding ID
 * @param	nDlgItemID - Dialog item ID
 * @param	bEnabled   - Enable or disable
 * @return	None
 */
void SDialog::EnableItem(CWnd* pDlgItemWnd, bool bEnabled)
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

/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void SDialog::SetupDialogItemState(void)
{
	// Update dialog control attributes
	UpdateDialogManagement();
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	bRecheckState - Recheck all item's state
 * @return	None
 */
void SDialog::RefreshDialogItemState(bool /* bRecheckState = false */)
{
	// Update dialog control attributes
	UpdateDialogManagement();
}

/**
 * @brief	Update data values for dialog items
 * @param	bSaveAndValidate - Same as default MFC UpdateData function
 * @return	None
 */
void SDialog::UpdateDialogData(bool /* bSaveAndValidate = true */)
{
	// Update data for dialog control management
	UpdateDialogManagement();
}

/**
 * @brief	Return option value by ID
 * @param	eAppOptionID - ID of specific option
 * @param	bTemp		 - Temp value or saved value (saved value by default)
 * @return	int - Option value
 */
int SDialog::GetAppOption(AppOptionID eAppOptionID, bool bTemp /* = false */) const
{
	int nResult = INT_INVALID;
	int nTempResult = INT_INVALID;

	// Acquire option value from application main window
	SDialog* pMainDlg = (SDialog*)AfxGetMainWnd();
	if (pMainDlg != NULL) {
		nResult = pMainDlg->GetAppOption(eAppOptionID, false);
		nTempResult = pMainDlg->GetAppOption(eAppOptionID, true);
	}

	// Return temp data if required and the result is valid
	if ((bTemp == true) && (nTempResult != INT_INVALID))
		return nTempResult;

	return nResult;
}

/**
 * @brief	Return flag value by ID
 * @param	eFlagID - ID of specific flag
 * @return	int - Flag value
 */
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

/**
 * @brief	Update flag value by ID
 * @param	eFlagID - ID of specific flag
 * @param	nValue  - Value to set
 * @return	None
 */
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

/**
 * @brief	Request current dialog to close
 * @param	None
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT SDialog::RequestCloseDialog(void)
{
	// Set force closing flag
	SetFlagValue(AppFlagID::dialogForceClosing, true);

	// Default: Close the dialog
	this->PostMessage(WM_CLOSE);

	// Request accepted
	return LRESULT(Result::Success);	// ERROR_SUCCESS
}
