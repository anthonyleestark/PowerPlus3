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
	tooltipCtrlPtr_ = NULL;

	// Dialog control management
	controlManagerPtr_ = NULL;

	// Dialog special flags
	descendantCount_ = INT_NULL;
	setFlagValue(AppFlagID::dialogUseEnterKey, true);
	setFlagValue(AppFlagID::dialogUseEscapeKey, true);
	setReturnFlag(ReturnFlag::Invalid);

	// Lock state exception IDs
	lockExceptionIdList_ = NULL;

	// Dialog anchor point
	anchorPoint_ = POINT_NULL;

	// Dialog alignment
	alignment_ = SDA_LEFTALIGN | SDA_TOPALIGN;

	// Dialog size
	registeredSize_ = SIZE_NULL;
	defaultSize_ = SIZE_NULL;

	// Dialog min/max info
	minSize_ = SIZE_UNDEFINED;
	maxSize_ = SIZE_UNDEFINED;

	// Dialog client display margin
	clientMargin_ = MARGIN_DEFAULT;

	// Other properties
	caption_.empty();
	defaultIcon_ = NULL;
	messageCaption_.empty();

	backgroudBrush_ = NULL;
	backgroundColor_ = Color::White;
	textColor_ = Color::Black;
}

SDialog::SDialog(unsigned templateId , CWnd* parentWnd /* = NULL */) : CDialogEx(templateId, parentWnd)
{
	// Parent window
	m_pParentWnd = parentWnd;

	// Tooltip control
	tooltipCtrlPtr_ = NULL;

	// Dialog control management
	controlManagerPtr_ = NULL;

	// Dialog special flags
	descendantCount_ = INT_NULL;
	setFlagValue(AppFlagID::dialogUseEnterKey, true);
	setFlagValue(AppFlagID::dialogUseEscapeKey, true);
	setReturnFlag(ReturnFlag::Invalid);

	// Lock state exception IDs
	lockExceptionIdList_ = NULL;

	// Dialog anchor point
	anchorPoint_ = POINT_NULL;

	// Dialog alignment
	alignment_ = SDA_LEFTALIGN | SDA_TOPALIGN;

	// Dialog size
	registeredSize_ = SIZE_NULL;
	defaultSize_ = SIZE_NULL;

	// Dialog min/max info
	minSize_ = SIZE_UNDEFINED;
	maxSize_ = SIZE_UNDEFINED;

	// Dialog client display margin
	clientMargin_ = MARGIN_DEFAULT;

	// Other properties
	caption_.empty();
	defaultIcon_ = NULL;
	messageCaption_.empty();

	backgroudBrush_ = NULL;
	backgroundColor_ = Color::White;
	textColor_ = Color::Black;
}

SDialog::SDialog(const wchar_t* templateName, CWnd* parentWnd /* = NULL */) : CDialogEx(templateName, parentWnd)
{
	// Parent window
	m_pParentWnd = parentWnd;

	// Tooltip control
	tooltipCtrlPtr_ = NULL;

	// Dialog control management
	controlManagerPtr_ = NULL;

	// Dialog special flags
	descendantCount_ = INT_NULL;
	setFlagValue(AppFlagID::dialogUseEnterKey, true);
	setFlagValue(AppFlagID::dialogUseEscapeKey, true);
	setReturnFlag(ReturnFlag::Invalid);

	// Lock state exception IDs
	lockExceptionIdList_ = NULL;

	// Dialog anchor point
	anchorPoint_ = POINT_NULL;

	// Dialog alignment
	alignment_ = SDA_LEFTALIGN | SDA_TOPALIGN;

	// Dialog size
	registeredSize_ = SIZE_NULL;
	defaultSize_ = SIZE_NULL;

	// Dialog min/max info
	minSize_ = SIZE_UNDEFINED;
	maxSize_ = SIZE_UNDEFINED;

	// Dialog client display margin
	clientMargin_ = MARGIN_DEFAULT;

	// Other properties
	caption_.empty();
	defaultIcon_ = NULL;
	messageCaption_.empty();

	backgroudBrush_ = NULL;
	backgroundColor_ = Color::White;
	textColor_ = Color::Black;
}

/**
 * @brief	Destructor
 */
SDialog::~SDialog()
{
	// Destroy tooltip control window
	if ((tooltipCtrlPtr_ != NULL) && (IsWindow(tooltipCtrlPtr_->GetSafeHwnd()))) {
		tooltipCtrlPtr_->DestroyToolTipCtrl();
	}

	// Clear dialog control manager
	if (controlManagerPtr_ != NULL) {
		controlManagerPtr_->DeleteAll();
		delete controlManagerPtr_;
		controlManagerPtr_ = NULL;
	}

	// Clear lock state exception IDs list
	resetLockStateExceptionList();
	if (lockExceptionIdList_ != NULL) {
		delete lockExceptionIdList_;
		lockExceptionIdList_ = NULL;
	}

	// Properties data cleanup
	::DeleteObject(defaultIcon_);
	if (backgroudBrush_ != NULL) {
		backgroudBrush_->DeleteObject();
		delete backgroudBrush_;
		backgroudBrush_ = NULL;
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


// SDialog dialog message map
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
	if (getParent() == NULL) {

		// Set top-most active window as its parent
		if (CWnd* pActiveWnd = CWnd::GetActiveWindow()) {
			this->setParent(pActiveWnd);
		}
		else {
			// There is currently no active window
			// Set desktop window as its parent instead
			this->setParent(CWnd::GetDesktopWindow());
		}
	}

	// Create and initialize tooltip control
	if (getToolTipCtrl() == NULL) {
		this->tooltipCtrlPtr_ = new CToolTipCtrl();
		this->tooltipCtrlPtr_->Create(this);
	}

	// Activate tooltip control
	if (isTooltipAvailable()) {
		getToolTipCtrl()->Activate(true);
	}

	// If dialog custom caption is set
	if (!caption_.isEmpty()) {

		// Update dialog caption
		this->SetWindowText(caption_);
	}
	else {
		// Use defined caption in resource
		const int captionLength = this->GetWindowTextLength();
		std::vector<wchar_t> tempBuff(captionLength + 1);
		this->GetWindowText(tempBuff.data(), captionLength + 1);
		String resourceCaption = tempBuff.data();
		this->setCaption(resourceCaption);
	}

	// Get dialog rectangle
	RECT dialogRect;
	this->GetWindowRect(&dialogRect);

	// Backup default size
	defaultSize_._width = (dialogRect.right - dialogRect.left);
	defaultSize_._height = (dialogRect.bottom - dialogRect.top);

	// If dialog size is not registered, use default
	if (registeredSize_.isEmpty()) {
		registeredSize_ = defaultSize_;
	}

	// Resize dialog
	if (registeredSize_ != defaultSize_) {
		// Set width
		if (registeredSize_.width() > -1) {
			dialogRect.right = (dialogRect.left + registeredSize_.width());
		}
		// Set height
		if (registeredSize_.height() > -1) {
			dialogRect.bottom = (dialogRect.top + registeredSize_.height());
		}
	}
	
	// Center dialog
	this->MoveWindow(&dialogRect);
	this->CenterWindow(getParent());

	// Background color
	this->createBrush();

	// If dialog is set as top-most 
	if (getFlagValue(AppFlagID::dialogSetTopMost) == true) {
		// Set window position
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	// If init sound is set
	if (getFlagValue(AppFlagID::dialogSetInitSound) == true) {
		MessageBeep(0xFFFFFFFF);
	}
	
	// Register dialog control management
	registerDialogManagement();

	// Get dialog ID
	WPARAM wParam = static_cast<WPARAM>(getDialogId());

	// Notify parent window about dialog initialization
	this->notifyParent(SCM_NOTIFY_DIALOG_INIT, wParam, NULL);

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
	WPARAM wParam = static_cast<WPARAM>(getDialogId());

	// Notify parent window about dialog closing
	this->notifyParent(SCM_NOTIFY_DIALOG_CLOSE, wParam, NULL);

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
	WPARAM wParam = static_cast<WPARAM>(getDialogId());

	// Notify parent window about dialog destroying
	this->notifyParent(SCM_NOTIFY_DIALOG_DESTROY, wParam, NULL);

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
	WPARAM wParam = static_cast<WPARAM>(getDialogId());

	// Notify parent window about dialog active/inactive state
	if (nState == WA_ACTIVE) {
		this->notifyParent(SCM_NOTIFY_DIALOG_ACTIVE, wParam, NULL);
	}
	else if (nState == WA_INACTIVE) {
		this->notifyParent(SCM_NOTIFY_DIALOG_INACTIVE, wParam, NULL);
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
	++descendantCount_;

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
	--descendantCount_;

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
	if (getFlagValue(AppFlagID::dialogSetMinSize) == true) {
		pMinMaxInfo->ptMinTrackSize.x = minSize_.width();
		pMinMaxInfo->ptMinTrackSize.y = minSize_.height();
	}
	if (getFlagValue(AppFlagID::dialogSetMaxSize) == true) {
		pMinMaxInfo->ptMaxTrackSize.x = maxSize_.width();
		pMinMaxInfo->ptMaxTrackSize.y = maxSize_.height();
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
			if (getFlagValue(AppFlagID::dialogUseEscapeKey) != true) {
				// Do not use Escape button
				return true;
			}
			break;

		case VK_RETURN:
			if (getFlagValue(AppFlagID::dialogUseEnterKey) != true) {
				// Do not use Enter button
				return true;
			}
			break;

		default:
			break;
		}
	}

	// Allow the tooltip to receive mouse messages
	if (isTooltipAvailable()) {
		this->tooltipCtrlPtr_->RelayEvent(pMsg);
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
	unregisterDialogManagement();

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
 * @param	message - Message
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	true/false
 */
bool SDialog::notifyParent(unsigned message, WPARAM wParam, LPARAM lParam)
{
	bool bRetNotify = false;

	// If parent window is set and available
	if (isParentAvailable()) {
		// Notify to parent window
		bRetNotify = m_pParentWnd->PostMessage(message, wParam, lParam);
	}
	// Check if main window is available
	else if (CWnd* pMainWnd = AfxGetMainWnd()) {
		// Send to main window to process
		bRetNotify = pMainWnd->PostMessage(message, wParam, lParam);
	}
	else {
		// Notify to application's main thread instead
		// and let it handle the message on its own
		bRetNotify = ::PostMessage(NULL, message, wParam, lParam);
	}

	return bRetNotify;
}

/**
 * @brief	Similar to NotifyParent but this will send the message
					and get the result immediately instead of posting it
 * @param	message - Message
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	LRESULT
 */
LRESULT SDialog::sendMessageToParent(unsigned message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRetNotify = Result::Failure;

	// If parent window is set and available
	if (isParentAvailable()) {
		// Send message to parent window
		lRetNotify = m_pParentWnd->SendMessage(message, wParam, lParam);
	}
	// Check if main window is available
	else if (CWnd* pMainWnd = AfxGetMainWnd()) {
		// Send to main window to process
		lRetNotify = pMainWnd->SendMessage(message, wParam, lParam);
	}
	else {
		// Send message to application's main thread instead
		// and let it handle the message on its own
		lRetNotify = ::SendMessage(NULL, message, wParam, lParam);
	}

	return lRetNotify;
}

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int SDialog::registerDialogManagement(void)
{
	// Initialize dialog control management
	if (controlManagerPtr_ == NULL) {
		controlManagerPtr_ = new SControlManager(this);
		if (controlManagerPtr_ == NULL)
			return INT_INVALID;
	}

	if (controlManagerPtr_ != NULL) {
		controlManagerPtr_->Initialize();
	}

	return 0;
}

/**
 * @brief	Update dialog control management
 * @param	None
 * @return	None
 */
void SDialog::updateDialogManagement(void)
{
	// Get control manager
	if (controlManagerPtr_ == NULL) return;

	// Update control data
	controlManagerPtr_->UpdateData(NULL);
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool SDialog::unregisterDialogManagement(void)
{
	// Clean-up dialog control management
	if (controlManagerPtr_ == NULL)
		return false;

	if (controlManagerPtr_ != NULL) {
		if (!controlManagerPtr_->DeleteAll())
			return false;

		delete controlManagerPtr_;
		controlManagerPtr_ = NULL;
	}

	return true;
}

/**
 * @brief	Add dialog extended styles
 * @param	style - Styles to add
 * @return	true/false
 */
bool SDialog::addStyle(DWORD style)
{
	DWORD dwStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	dwStyle |= style;
	LONG lRet = SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, dwStyle);
	return (lRet != 0);
}

/**
 * @brief	Remove dialog extended styles
 * @param	style - Styles to remove
 * @return	true/false
 */
bool SDialog::removeStyle(DWORD style)
{
	DWORD dwStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	dwStyle &= ~style;
	LONG lRet = SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, dwStyle);
	return (lRet != 0);
}

/**
 * @brief	Add a control ID to lock state exception list
 * @param	nID - Control ID
 * @return	None
 */
void SDialog::addLockStateException(unsigned id)
{
	// Initialize the list if not yet allocated
	if (lockExceptionIdList_ == NULL) {
		lockExceptionIdList_ = new UIntArray;
		lockExceptionIdList_->clear();
	}

	// Loop through all list and find if item existed in list
	int nItemNum = lockExceptionIdList_->size();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		if (lockExceptionIdList_->at(nIndex) == id) {
			// No need to add
			return;
		}
	}

	// Add to list
	lockExceptionIdList_->push_back(id);
}

/**
 * @brief	Remove a control ID out of lock state exception list
 * @param	nID - Control ID
 * @return	None
 */
void SDialog::removeLockStateException(unsigned id)
{
	// If the list is not yet allocated, no need to remove
	if (lockExceptionIdList_ == NULL)
		return;

	// Loop through all list and remove item
	int nItemNum = lockExceptionIdList_->size();
	for (int nIndex = (nItemNum - 1); nIndex >= 0; nIndex--) {
		if (lockExceptionIdList_->at(nIndex) == id) {
			lockExceptionIdList_->erase(lockExceptionIdList_->begin() + nIndex);
		}
	}
}

/**
 * @brief	Remove all lock state exception list
 * @param	nID - Control ID
 * @return	None
 */
void SDialog::resetLockStateExceptionList(void)
{
	if (lockExceptionIdList_ != NULL) {
		lockExceptionIdList_->clear();
	}
}

/**
 * @brief	Move dialog position and return new rectangle
 * @param	position - Dialog position (anchor point)
 * @param	lpNewRect  - New dialog rectangle (OUT)
 * @return	None
 */
void SDialog::move(const Point& position, Rect* newRect /* = nullptr */)
{
	// Get current dialog rectangle
	RECT rcCurPos;
	this->GetWindowRect(&rcCurPos);

	// Get dialog alignment
	unsigned nAlign = getAlignment();

	// Calculate moving delta by alignments
	LONG dx = 0, dy = 0;

	// --> Calculate horizontal delta
	if (nAlign & SDA_LEFTALIGN) {
		// Move left rect
		dx = position._x - rcCurPos.left;
	}
	else if (nAlign & SDA_RIGHTALIGN) {
		// Move right rect
		dx = position._x - rcCurPos.right;
	}
	else if (nAlign & SDA_HCENTERALIGN) {
		// Move center rect
		LONG nHCenter = rcCurPos.right - rcCurPos.left;
		dx = position._x - nHCenter;
	}

	// --> Calculate vertical delta
	if (nAlign & SDA_TOPALIGN) {
		// Move top rect
		dy = position._y - rcCurPos.top;
	}
	else if (nAlign & SDA_BOTTOMALIGN) {
		// Move bottom rect
		dy = position._y - rcCurPos.bottom;
	}
	else if (nAlign & SDA_VCENTERALIGN) {
		// Move center rect
		LONG nVCenter = rcCurPos.bottom - rcCurPos.top;
		dy = position._y - nVCenter;
	}

	// Move dialog
	move(dx, dy, newRect);
}

/**
 * @brief	Move dialog position as given delta and return new rect
 * @param	dx		  - Horizontal delta
 * @param	dy		  - Vertical delta
 * @param	lpNewRect - New dialog rectangle (OUT)
 * @return	None
 */
void SDialog::move(long dx, long dy, Rect* newRect /* = nullptr */)
{
	// Get current dialog rectangle
	RECT rcCurPos;
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
		if ((bRet == true) && (newRect != nullptr)) {
			newRect->_left = rcCurPos.left;
			newRect->_right = rcCurPos.right;
			newRect->_top = rcCurPos.top;
			newRect->_bottom = rcCurPos.bottom;
		}
	}
}

/**
 * @brief	Resize dialog (after change size)
 * @param	isCentered - Replace dialog to center
 * @return	None
 */
void SDialog::resize(bool isCentered)
{
	// Get current dialog rectangle
	RECT rectDlg;
	this->GetWindowRect(&rectDlg);

	// Set new rectangle
	if (!registeredSize_.isEmpty()) {
		rectDlg.right = (rectDlg.left + registeredSize_.width());
		rectDlg.bottom = (rectDlg.top + registeredSize_.height());
	}

	// Resize
	SetWindowPos(NULL, 0, 0, (rectDlg.right - rectDlg.left), (rectDlg.bottom - rectDlg.top), SWP_NOMOVE | SWP_NOZORDER);

	// Make sure that the entire dialog box is visible on the screen
	SendMessage(DM_REPOSITION, 0, 0);

	// Center dialog
	if (isCentered == true) {
		this->CenterWindow(getParent());
	}
}

/**
 * @brief	Reset dialog size to default
 * @param	None
 * @return	None
 */
void SDialog::resetSize(void)
{
	// No default size
	if (defaultSize_.isEmpty())
		return;

	// Get current size
	RECT rcCurRect;
	this->GetWindowRect(&rcCurRect);

	// If current size is default size, do nothing
	LONG lCurWidth = (rcCurRect.right - rcCurRect.left);
	LONG lCurHeight = (rcCurRect.bottom - rcCurRect.top);
	if ((lCurWidth == defaultSize_.width()) && (lCurHeight == defaultSize_.height()))
		return;

	// Reset to default
	registeredSize_ = defaultSize_;

	// Resize dialog
	resize(true);
}

/**
 * @brief	Set new display area, update margin and resize dialog
 * @param	newDispArea	  - New display area rectangle
 * @param	bResizeDialog - Resize dialog
 * @param	bCenter		  - Center rectangle area
 * @return	None
 */
void SDialog::setDisplayArea(const Rect& newDispArea, bool isResized, bool isCentered)
{
	// Get current margin
	Rect currentMargin;
	this->getMargin(currentMargin);

	// Is center margin???
	bool bHorzCenter = (currentMargin.left() == currentMargin.right());
	bool bVertCenter = (currentMargin.top() == currentMargin.bottom());

	// New dialog margin
	Rect newMargin = currentMargin;

	// Get current client and dialog rectangle
	RECT currentClientRect, currentDialogRect;
	this->GetClientRect(&currentClientRect);
	this->ClientToScreen(&currentClientRect);
	this->GetWindowRect(&currentDialogRect);

	// Get dialog size
	Size dialogSize;
	this->getSize(dialogSize);

	// Dialog and client rectangle offset
	Rect dialogClientOffset;
	dialogClientOffset._left = abs(currentDialogRect.left - currentClientRect.left);
	dialogClientOffset._top = abs(currentDialogRect.top - currentClientRect.top);
	dialogClientOffset._right = abs(currentDialogRect.right - currentClientRect.right);
	dialogClientOffset._bottom = abs(currentDialogRect.bottom - currentClientRect.bottom);

	// Get current display area
	Rect currentDispArea;
	this->getDisplayArea(currentDispArea);

	// Calculate display area size
	Size curDispAreaSize = currentDispArea.getSize();
	Size newDispAreaSize = newDispArea.getSize();

	// Update margin
	if ((isResized != true) && (isCentered == true)) {
		// Horizontal center margin
		if (bHorzCenter == true) {
			newMargin._left = currentMargin.left() + (curDispAreaSize.width() - newDispAreaSize.width());
			newMargin._right = newMargin.left();
		}
		// Vertical center margin
		if (bVertCenter == true) {
			newMargin._top = currentMargin.top() + (curDispAreaSize.height() - newDispAreaSize.height());
			newMargin._bottom = newMargin.top();
		}
	}

	// New dialog rectangle
	Rect newDialogRect;

	// Set new dialog rectangle top-left
	newDialogRect._left = (newDispArea.left() - newMargin.left()) - dialogClientOffset.left();
	newDialogRect._top = (newDispArea.top() - newMargin.top()) - dialogClientOffset.top();

	// If resize is specified
	if (isResized == true) {

		// Set new dialog rectangle bottom-right
		newDialogRect._right = (newDispArea.right() + newMargin.right()) + dialogClientOffset.right();
		newDialogRect._bottom = (newDispArea.bottom() + newMargin.bottom()) + dialogClientOffset.bottom();

		// Resize dialog
		this->setSize(newDialogRect.width(), newDialogRect.height());
		this->resize(isCentered);
	}
	else {
		// Reposition following new margin offset
		newDialogRect._left += (newMargin.left() - currentMargin.left());
		newDialogRect._top += (newMargin.top() - currentMargin.top());

		// Reposition dialog
		SetWindowPos(NULL, newDialogRect.left(), newDialogRect.top(), 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		// Make sure that the entire dialog box is visible on the screen
		SendMessage(DM_REPOSITION, 0, 0);

		// Center dialog
		if (isCentered == true) {
			this->CenterWindow(getParent());
		}

		// Get new rect after repositioning
		RECT newRect;
		this->GetWindowRect(&newRect);

		// Recalculate bottom-right margin
		newMargin._right = (newRect.right - dialogClientOffset.right()) - newDispArea.right();
		newMargin._bottom = (newRect.bottom - dialogClientOffset.bottom()) - newDispArea.bottom();
	}

	// Save margin update
	this->setLeftMargin(newMargin.left());
	this->setTopMargin(newMargin.top());
	this->setRightMargin(newMargin.right());
	this->setBottomMargin(newMargin.bottom());
}

/**
 * @brief	Set dialog caption by resource string ID
 * @param	resourceStringId - Resource string ID
 * @return	None
 */
void SDialog::setCaptionFromResource(unsigned resourceStringId)
{
	String captionString = StringUtils::loadResourceString(resourceStringId);
	ASSERT(!captionString.isEmpty());
	if (!captionString.isEmpty()) {
		this->setCaption(captionString);
	}
}

/**
 * @brief	Set dialog caption by language string ID
 * @param	langStringId - Language string ID
 * @return	None
 */
void SDialog::setCaptionFromLanguage(unsigned langStringId)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->getAppLanguage();

	String captionString = getLanguageString(pAppLang, langStringId);
	this->setCaption(captionString);
}

/**
 * @brief	Create (or re-create) dialog owned brush
 * @param	None
 * @return	true/false
 */
bool SDialog::createBrush(void)
{
	if (getFlagValue(AppFlagID::dialogSetBackgroundColor)) {

		// Re-create if brush existed
		if (backgroudBrush_ != NULL) {
			backgroudBrush_->DeleteObject();
			delete backgroudBrush_;
			backgroudBrush_ = NULL;
		}

		// Initialization
		backgroudBrush_ = new CBrush();
		if (backgroudBrush_ != NULL) {

			// Create background color brush
			return backgroudBrush_->CreateSolidBrush(backgroundColor_);
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
void SDialog::registerMessageBoxCaption(unsigned nCaptionID)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->getAppLanguage();
	String captionString = Constant::String::Empty;
	if (nCaptionID != NULL) {

		// Get language string caption
		String langCaption = getLanguageString(pAppLang, nCaptionID);
		if (IS_NOT_NULL_STRING(langCaption)) {
			// Set caption string
			captionString = langCaption;
		}
	}

	// If caption is empty
	if (captionString.isEmpty()) {
		// Use default app window caption
		captionString = ((SWinApp*)AfxGetApp())->getAppWindowCaption();
	}

	// Register message box caption
	registerMessageBoxCaption(captionString);
}

/**
 * @brief	Display message box using language string ID
 * @param	nPromptID  - ID of prompt message string
 * @param	nCaptionID - ID of message caption string
 * @param	nStyle	   - Message box style
 * @return	int	- Result of message box
 */
int SDialog::displayMessageBox(unsigned promptId, unsigned captionId /* = NULL */, unsigned style /* = NULL */)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->getAppLanguage();

	String messagePrompt = getLanguageString(pAppLang, promptId);
	String messageCaption = ((SWinApp*)AfxGetApp())->getAppWindowCaption();
	if (captionId != NULL) {

		// Get language string caption
		String langCaption = getLanguageString(pAppLang, captionId);
		if (IS_NOT_NULL_STRING(langCaption))
			messageCaption = langCaption;
	}
	else {
		// Using registered message box caption
		if (!messageCaption_.isEmpty()) {
			messageCaption = messageCaption_;
		}
	}

	// Display message box
	int nResult = displayMessageBox(messagePrompt, messageCaption, style);

	return nResult;
}

/**
 * @brief	Display message box using language string
 * @param	prompt  - Message string
 * @param	caption - Message caption string
 * @param	nStyle	- Message box style
 * @return	int	- Result of message box
 */
int SDialog::displayMessageBox(const wchar_t* prompt, const wchar_t* caption /* = NULL */, unsigned style /* = NULL */)
{
	// Set default style
	if (style == NULL) {
		// Information message with [OK] button
		style = MB_OK | MB_ICONINFORMATION;
	}

	// If caption is not set
	String messageCaption(caption);
	if (messageCaption.isEmpty()) {

		// If message box caption is registered
		if (!messageCaption_.isEmpty()) {
			// Use registered caption
			messageCaption = messageCaption_;
		}

		// Otherwise,
		else {
			// Use app window caption
			messageCaption = ((SWinApp*)AfxGetApp())->getAppWindowCaption();
		}
	}
	
	// Display message box
	style |= MB_SYSTEMMODAL;
	return MessageBox(prompt, messageCaption, style);
}

/**
 * @brief	Output application event log
 * @param	eventId		- Event ID
 * @param	description - Additional description
 * @param	detailInfoPtr	- Log detail info (array pointer)
 * @return	None
 */
void SDialog::outputEventLog(USHORT eventId, const wchar_t* description /* = NULL */, LOGDETAILINFO* detailInfoPtr /* = NULL */)
{
	// Prepare event log info
	LOGITEM logItemDialogEvent;
	logItemDialogEvent.SetCategory(eventId);
	logItemDialogEvent.SetTime(DateTimeUtils::getCurrentDateTime());
	logItemDialogEvent.SetProcessID();
	if (description) {
		// Include event description
		logItemDialogEvent.SetLogString(description);
	}
	if (detailInfoPtr != NULL) {
		// Include event detail info data
		for (int nIndex = 0; nIndex < detailInfoPtr->size(); nIndex++) {
			logItemDialogEvent.AddDetail(detailInfoPtr->at(nIndex));
		}
	}

	// Output dialog event log
	SWinApp* pApp = (SWinApp*)AfxGetApp();
	ASSERT(pApp);
	if (pApp == NULL) return;
	if (SLogging* ptrAppEventLog = pApp->getAppEventLog()) {
		ptrAppEventLog->OutputItem(logItemDialogEvent);
	}
}

/**
 * @brief	Output button event log
 * @param	eventId	  - Event ID
 * @param	buttonId - Button ID
 * @return	None
 */
void SDialog::outputButtonLog(USHORT eventId, unsigned buttonId)
{
	// Prepare button event log info
	CButton* pButton = (CButton*)GetDlgItem(buttonId);
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
		logDetailInfo.AddDetail(EventDetail::ResourceID, buttonId);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(buttonId)));
	}

	// Output button event log
	outputEventLog(eventId, buttonCaption, &logDetailInfo);
}

/**
 * @brief	Output checkbox event log
 * @param	eventId		- Event ID
 * @param	checkboxId - Checkbox ID
 * @return	None
 */
void SDialog::outputCheckBoxLog(USHORT eventId, unsigned checkboxId)
{
	// Prepare checkbox event log info
	CButton* pChkBtn = (CButton*)GetDlgItem(checkboxId);
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
		logDetailInfo.AddDetail(EventDetail::ResourceID, checkboxId);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(checkboxId)));

		// Checkbox checked state
		logDetailInfo.AddDetail(EventDetail::CheckState, pChkBtn->GetCheck());
	}

	// Output checkbox event log
	outputEventLog(eventId, checkBoxCaption, &logDetailInfo);
}

/**
 * @brief	Output radio button event log
 * @param	eventId		- Event ID
 * @param	checkboxId - Checkbox ID
 * @return	None
 */
void SDialog::outputRadButtonLog(USHORT eventId, unsigned radButtonId)
{
	// Prepare radio button event log info
	CButton* pRadBtn = (CButton*)GetDlgItem(radButtonId);
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
		logDetailInfo.AddDetail(EventDetail::ResourceID, radButtonId);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(radButtonId)));

		// Radio button checked state
		logDetailInfo.AddDetail(EventDetail::CheckState, pRadBtn->GetCheck());
	}

	// Output radio button event log
	outputEventLog(eventId, radButtonCaption, &logDetailInfo);
}

/**
 * @brief	Output combo-box event log
 * @param	eventId	 - Event ID
 * @param	comboId - Combo-box ID
 * @return	None
 */
void SDialog::outputComboBoxLog(USHORT eventId, unsigned comboId)
{
	// Prepare combo-box event log info
	CComboBox* pCombo = (CComboBox*)GetDlgItem(comboId);
	if (pCombo == NULL) return;

	// Detail info
	String comboBoxCaption;
	LOGDETAILINFO logDetailInfo;
	{
		// Combo-box ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, comboId);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(comboId)));

		// Combo-box control info
		SControlManager* pCtrlMan = getControlManager();
		if (pCtrlMan != NULL) {
			SCtrlInfoWrap* pComboWrap = pCtrlMan->GetControl(comboId);
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
	outputEventLog(eventId, comboBoxCaption, &logDetailInfo);
}

/**
 * @brief	Output edit box event log
 * @param	eventId	- Event ID
 * @param	editId - Edit box ID
 * @return	None
 */
void SDialog::outputEditBoxLog(USHORT eventId, unsigned editId)
{
	// Prepare edit box event log info
	CEdit* pEdit = (CEdit*)GetDlgItem(editId);
	if (pEdit == NULL) return;

	// Detail info
	String editBoxCaption;
	LOGDETAILINFO logDetailInfo;
	{
		// Edit box ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, editId);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(editId)));

		// Edit box control info
		SControlManager* pCtrlMan = getControlManager();
		if (pCtrlMan != NULL) {
			SCtrlInfoWrap* pEditBoxWrap = pCtrlMan->GetControl(editId);
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
	outputEventLog(eventId, editBoxCaption, &logDetailInfo);
}

/**
 * @brief	Output list box event log
 * @param	eventId	   - Event ID
 * @param	listBoxId - List box ID
 * @return	None
 */
void SDialog::outputListBoxLog(USHORT eventId, unsigned listBoxId)
{
	// Prepare list box event log info
	CListBox* pListBox = (CListBox*)GetDlgItem(listBoxId);
	if (pListBox == NULL) return;

	// Detail info
	String listBoxCaption;
	LOGDETAILINFO logDetailInfo;
	{
		// List box ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, listBoxId);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(listBoxId)));

		// List box control info
		SControlManager* pCtrlMan = getControlManager();
		if (pCtrlMan != NULL) {
			SCtrlInfoWrap* pListBoxWrap = pCtrlMan->GetControl(listBoxId);
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
	outputEventLog(eventId, listBoxCaption, &logDetailInfo);
}

/**
 * @brief	Output spin control event log
 * @param	eventId		- Event ID
 * @param	spinCtrlId - Spin control ID
 * @return	None
 */
void SDialog::outputSpinCtrlLog(USHORT eventId, unsigned spinCtrlId)
{
	// Prepare spin control event log info
	CSpinButtonCtrl* pSpinCtrl = (CSpinButtonCtrl*)GetDlgItem(spinCtrlId);
	if (pSpinCtrl == NULL) return;

	// Detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Spin control ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, spinCtrlId);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(spinCtrlId)));
	}

	// Output spin control event log
	outputEventLog(eventId, NULL, &logDetailInfo);
}

/**
 * @brief	Output menu event log
 * @param	eventId		- Event ID
 * @param	menuItemId - Menu item ID
 * @return	None
 */
void SDialog::outputMenuLog(USHORT eventId, unsigned menuItemId)
{
	// Prepare menu event log info
	// Get menu item title from language table
	String menuItemCaption;
	LANGTABLE_PTR pLanguage = ((SWinApp*)AfxGetApp())->getAppLanguage();
	if (pLanguage != NULL) {
		menuItemCaption = getLanguageString(pLanguage, menuItemId);
	}

	// Detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Menu item ID
		logDetailInfo.AddDetail(EventDetail::ResourceID, menuItemId);

		// Mapped ID
		logDetailInfo.AddDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(menuItemId)));
	}

	// Output menu event log
	outputEventLog(eventId, menuItemCaption, &logDetailInfo);
}

/**
 * @brief	Setup language for dialog controls
 * @param	None
 * @return	None
 */
void SDialog::setupLanguage(void)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((SWinApp*)AfxGetApp())->getAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		unsigned nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDOK:
		case IDCANCEL:
			setControlText(pWndChild, nID, pAppLang);
			break;
		}
	}

	// Update dialog control attributes
	updateDialogManagement();
}

/**
 * @brief	Setup data for combo-boxes
 * @param	comboId	- ID of combo box
 * @param	langTablePtr - Language package pointer
 * @return	None
 */
void SDialog::setupComboBox(unsigned /*comboId*/, LANGTABLE_PTR /*langTablePtr*/)
{
	// Update dialog control attributes
	updateDialogManagement();
}

/**
 * @brief	Set icon for dialog button
 * @param	buttonId	   - ID of button
 * @param	iconId		   - ID of icon resource
 * @param	bReUpdateTitle - Whether to reupdate button title
 * @return	None
 */
void SDialog::setButtonIcon(unsigned buttonId, unsigned iconId, bool reupdateTitle /* = false */)
{
	// Get button
	CWnd* pButton = GetDlgItem(buttonId);
	if (!pButton) return;

	// Backup title
	String buttonTitle;
	if (reupdateTitle == true) {
		const int captionLength = pButton->GetWindowTextLength();
		std::vector<wchar_t> tempBuff(captionLength + 1);
		pButton->GetWindowText(tempBuff.data(), captionLength + 1);
		buttonTitle = tempBuff.data();
	}

	// Icon size
	int cx = Constant::UI::Button::IconWidth;
	int cy = Constant::UI::Button::IconHeight;

	// Load icon from resource
	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	HICON hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(iconId), IMAGE_ICON, cx, cy, LR_DEFAULTCOLOR);
	if (hIcon == NULL)
		return;

	// Draw button icon
	SendDlgItemMessage(buttonId, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

	// Restore title
	if (reupdateTitle == true) {
		SetDlgItemText(buttonId, buttonTitle);
	}
}

/**
 * @brief	Update control text label
 * @param	controlId - ID of control item
 * @param	newCaption - New caption string
 * @return	None
 */
void SDialog::updateItemText(unsigned controlId, const wchar_t* newCaption)
{
	// Get item by ID
	CWnd* controlPtr = this->GetDlgItem(controlId);
	if (controlPtr == NULL)
		return;

	// Update item text
	controlPtr->SetWindowText(newCaption);
}

/**
 * @brief	Update control text label
 * @param	controlId - ID of control item
 * @param	newCaptionId - New caption language string ID
 * @param	langTablePtr   - Language package pointer
 * @return	None
 */
void SDialog::updateItemText(unsigned controlId , unsigned newCaptionId /* = NULL */, LANGTABLE_PTR langTablePtr /* = NULL */)
{
	// Check language package validity
	if (langTablePtr == NULL) {
		langTablePtr = ((SWinApp*)AfxGetApp())->getAppLanguage();
		if (langTablePtr == NULL)
			return;
	}

	// Get caption language string
	String wndItemText;
	if (newCaptionId != NULL) {
		// Get new caption
		wndItemText = getLanguageString(langTablePtr, newCaptionId);
	}
	else {
		// Get its own caption string ID
		wndItemText = getLanguageString(langTablePtr, controlId);
	}

	// Update item text
	updateItemText(controlId, wndItemText);
}

/**
 * @brief	Set control text label
 * @param	controlPtr	- Pointer of control window
 * @param	controlId - ID of control item
 * @param	langTablePtr - Language package pointer
 * @return	None
 */
void SDialog::setControlText(CWnd* controlPtr, unsigned controlId, LANGTABLE_PTR langTablePtr /* = NULL */)
{
	// Check control pointer validity
	if (controlPtr == NULL) {
		controlPtr = this->GetDlgItem(controlId);
		if (controlPtr == NULL) 
			return;
	}
	
	// Check language package validity
	if (langTablePtr == NULL) {
		langTablePtr = ((SWinApp*)AfxGetApp())->getAppLanguage();
		if (langTablePtr == NULL)
			return;
	}

	// Get language string
	String wndItemText = getLanguageString(langTablePtr, controlId);
	
	// Set control text
	controlPtr->SetWindowText(wndItemText);
}

/**
 * @brief	Move a group of dialog items to specific position
 * @param	controlIdGroup - Array of control IDs
 * @param	newPosition	- New position
 * @return	None
 */
void SDialog::moveItemGroup(const UIntArray& controlIdGroup, const Point& newPosition)
{
	// Check data validity
	if (controlIdGroup.empty())
		return;

	RECT rcCtrlWnd;
	CWnd* controlPtr = NULL;
	LONG lOrgX = INT_INVALID, lOrgY = INT_INVALID;

	// Find the original point
	for (int nIndex = 0; nIndex < controlIdGroup.size(); nIndex++)
	{
		// Get item
		controlPtr = GetDlgItem(controlIdGroup.at(nIndex));
		if (controlPtr == NULL)
			continue;

		// Get item rect
		controlPtr->GetWindowRect(&rcCtrlWnd);

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
	int nDeltaX = newPosition._x - lOrgX;
	int nDeltaY = newPosition._y - lOrgY;

	// Move all items to new position
	int nNewX = 0, nNewY = 0;
	for (int nIndex = 0; nIndex < controlIdGroup.size(); nIndex++)
	{
		// Get item
		controlPtr = GetDlgItem(controlIdGroup.at(nIndex));
		if (controlPtr == NULL)
			continue;

		// Get item rect
		controlPtr->GetWindowRect(&rcCtrlWnd);

		// Move item
		nNewX = rcCtrlWnd.left + nDeltaX;
		nNewY = rcCtrlWnd.top + nDeltaY;
		controlPtr->SetWindowPos(NULL, nNewX, nNewY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

/**
 * @brief	Move a group of dialog items to specific position
 * @param	controlIdGroup  - Array of control IDs
 * @param	direction		- Moving direction
 * @param	distance		- Moving distance
 * @return	None
 */
void SDialog::moveItemGroup(const UIntArray& controlIdGroup, int direction, int distance)
{
	// Check data validity
	if (controlIdGroup.empty())
		return;

	RECT rcCtrlWnd;
	CWnd* controlPtr = NULL;
	LONG lNewX = 0, lNewY = 0;

	// Loop through each item and move
	for (int nIndex = 0; nIndex < controlIdGroup.size(); nIndex++)
	{
		// Get item
		controlPtr = GetDlgItem(controlIdGroup.at(nIndex));
		if (controlPtr == NULL)
			continue;

		// Get item rect
		controlPtr->GetWindowRect(&rcCtrlWnd);

		// Move horizontal direction
		if (direction == MOVDIR_HORIZONTAL) {
			lNewX = rcCtrlWnd.left + distance;
			lNewY = rcCtrlWnd.top;
			controlPtr->SetWindowPos(NULL, lNewX, lNewY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		// Move vertical direction
		else if (direction == MOVDIR_VERTICAL) {
			lNewX = rcCtrlWnd.left;
			lNewY = rcCtrlWnd.top + distance;
			controlPtr->SetWindowPos(NULL, lNewX, lNewY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}
}

/**
 * @brief	Show/hide dialog item with corresponding ID
 * @param	itemPtr - Dialog item window pointer
 * @param	isVisible - Visible state
 * @return	None
 */
void SDialog::showItem(CWnd* itemPtr, bool isVisible)
{
	// Check dialog item validity
	ASSERT(itemPtr != NULL);
	if (itemPtr == NULL) return;

	// If new control state is the same as current state, do nothing
	if (itemPtr->IsWindowVisible() == isVisible)
		return;

	// Show/hide control
	itemPtr->ShowWindow(isVisible);
}

/**
 * @brief	Enable/disable dialog item with corresponding ID
 * @param	nDlgItemID - Dialog item ID
 * @param	isEnabled - Enable or disable
 * @return	None
 */
void SDialog::enableItem(CWnd* itemPtr, bool isEnabled)
{
	// Check dialog item validity
	ASSERT(itemPtr != NULL);
	if (itemPtr == NULL) return;

	// If new control state is the same as current state, do nothing
	if (itemPtr->IsWindowEnabled() == isEnabled)
		return;

	// Enable/disable control
	itemPtr->EnableWindow(isEnabled);
}

/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void SDialog::setupDialogItemState(void)
{
	// Update dialog control attributes
	updateDialogManagement();
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	recheckState - Recheck all item's state
 * @return	None
 */
void SDialog::refreshDialogItemState(bool /* recheckState = false */)
{
	// Update dialog control attributes
	updateDialogManagement();
}

/**
 * @brief	Update data values for dialog items
 * @param	saveAndValidate - Same as default MFC UpdateData function
 * @return	None
 */
void SDialog::updateDialogData(bool /* saveAndValidate = true */)
{
	// Update data for dialog control management
	updateDialogManagement();
}

/**
 * @brief	Return option value by ID
 * @param	eAppOptionID - ID of specific option
 * @param	isTemp		 - Temp value or saved value (saved value by default)
 * @return	int - Option value
 */
int SDialog::getAppOption(AppOptionID eAppOptionID, bool isTemp /* = false */) const
{
	int nResult = INT_INVALID;
	int nTempResult = INT_INVALID;

	// Acquire option value from application main window
	SDialog* pMainDlg = (SDialog*)AfxGetMainWnd();
	if (pMainDlg != NULL) {
		nResult = pMainDlg->getAppOption(eAppOptionID, false);
		nTempResult = pMainDlg->getAppOption(eAppOptionID, true);
	}

	// Return temp data if required and the result is valid
	if ((isTemp == true) && (nTempResult != INT_INVALID))
		return nTempResult;

	return nResult;
}

/**
 * @brief	Return flag value by ID
 * @param	eFlagID - ID of specific flag
 * @return	int - Flag value
 */
int SDialog::getFlagValue(AppFlagID eFlagID) const
{
	int nValue = FLAG_OFF;

	switch (eFlagID)
	{
	// Special dialog-base flags (not managed by FlagManager)
	case AppFlagID::dialogSetMinSize:
		nValue = (!minSize_.isEmpty() &&	!minSize_.isZero());
		break;
	case AppFlagID::dialogSetMaxSize:
		nValue = (maxSize_ > minSize_);
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
		nValue = flagManager_.getFlagValue(eFlagID);
		break;

	default:
		// Request the flag value from application
		nValue = ((SWinApp*)AfxGetApp())->getFlagValue(eFlagID);
		break;
	}

	return nValue;
}

/**
 * @brief	Update flag value by ID
 * @param	eFlagID - ID of specific flag
 * @param	value  - Value to set
 * @return	None
 */
void SDialog::setFlagValue(AppFlagID eFlagID, int value)
{
	// Check value validity
	if (value == INT_INVALID)
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
		flagManager_.setFlagValue(eFlagID, value);
		break;

	default:
		// Let the application manage the flags
		((SWinApp*)AfxGetApp())->setFlagValue(eFlagID, value);
		break;
	}
}

/**
 * @brief	Request current dialog to close
 * @param	None
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT SDialog::requestCloseDialog(void)
{
	// Set force closing flag
	setFlagValue(AppFlagID::dialogForceClosing, true);

	// Default: Close the dialog
	this->PostMessage(WM_CLOSE);

	// Request accepted
	return LRESULT(Result::Success);	// ERROR_SUCCESS
}
