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
		controlManagerPtr_->deleteAll();
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
void SDialog::OnActivate(UINT state, CWnd* otherWndPtr, BOOL isMinimized)
{
	// Default
	CDialogEx::OnActivate(state, otherWndPtr, isMinimized);

	// Get dialog ID
	WPARAM wParam = static_cast<WPARAM>(getDialogId());

	// Notify parent window about dialog active/inactive state
	if (state == WA_ACTIVE) {
		this->notifyParent(SCM_NOTIFY_DIALOG_ACTIVE, wParam, NULL);
	}
	else if (state == WA_INACTIVE) {
		this->notifyParent(SCM_NOTIFY_DIALOG_INACTIVE, wParam, NULL);
	}
}

/**
 * @brief	Default method for handling mouse moving event within dialog
 * @param	Default
 * @return	None
 */
void SDialog::OnMouseMove(UINT flags, CPoint point)
{
	// Default
	CDialogEx::OnMouseMove(flags, point);
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
void SDialog::OnGetMinMaxInfo(MINMAXINFO* minMaxInfoPtr)
{
	// Fix min/max size
	if (getFlagValue(AppFlagID::dialogSetMinSize) == true) {
		minMaxInfoPtr->ptMinTrackSize.x = minSize_.width();
		minMaxInfoPtr->ptMinTrackSize.y = minSize_.height();
	}
	if (getFlagValue(AppFlagID::dialogSetMaxSize) == true) {
		minMaxInfoPtr->ptMaxTrackSize.x = maxSize_.width();
		minMaxInfoPtr->ptMaxTrackSize.y = maxSize_.height();
	}

	// Default
	CDialogEx::OnGetMinMaxInfo(minMaxInfoPtr);
}

/**
 * @brief	Default method for pre-translate message
 * @param	messagePtr - Default
 * @return	None
 */
BOOL SDialog::PreTranslateMessage(MSG* messagePtr)
{
	// Handle system key pressed message
	if (messagePtr->message == WM_KEYDOWN) {
		switch (messagePtr->wParam) 
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
		this->tooltipCtrlPtr_->RelayEvent(messagePtr);
	}

	// Default
	return CDialogEx::PreTranslateMessage(messagePtr);
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
	bool notifyResult = false;

	// If parent window is set and available
	if (isParentAvailable()) {
		// Notify to parent window
		notifyResult = m_pParentWnd->PostMessage(message, wParam, lParam);
	}
	// Check if main window is available
	else if (CWnd* mainWndPtr = AfxGetMainWnd()) {
		// Send to main window to process
		notifyResult = mainWndPtr->PostMessage(message, wParam, lParam);
	}
	else {
		// Notify to application's main thread instead
		// and let it handle the message on its own
		notifyResult = ::PostMessage(NULL, message, wParam, lParam);
	}

	return notifyResult;
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
	LRESULT notifyResult = Result::Failure;

	// If parent window is set and available
	if (isParentAvailable()) {
		// Send message to parent window
		notifyResult = m_pParentWnd->SendMessage(message, wParam, lParam);
	}
	// Check if main window is available
	else if (CWnd* mainWndPtr = AfxGetMainWnd()) {
		// Send to main window to process
		notifyResult = mainWndPtr->SendMessage(message, wParam, lParam);
	}
	else {
		// Send message to application's main thread instead
		// and let it handle the message on its own
		notifyResult = ::SendMessage(NULL, message, wParam, lParam);
	}

	return notifyResult;
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
		controlManagerPtr_->initialize();
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
	controlManagerPtr_->updateData(NULL);
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
		if (!controlManagerPtr_->deleteAll())
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
	DWORD currentStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	currentStyle |= style;
	LONG result = SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, currentStyle);
	return (result != 0);
}

/**
 * @brief	Remove dialog extended styles
 * @param	style - Styles to remove
 * @return	true/false
 */
bool SDialog::removeStyle(DWORD style)
{
	DWORD currentStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	currentStyle &= ~style;
	LONG result = SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, currentStyle);
	return (result != 0);
}

/**
 * @brief	Add a control ID to lock state exception list
 * @param	id - Control ID
 * @return	None
 */
void SDialog::addLockStateException(unsigned id)
{
	// Initialize the list if not yet allocated
	if (lockExceptionIdList_ == NULL) {
		lockExceptionIdList_ = new uint32_array;
		lockExceptionIdList_->clear();
	}

	// Loop through all list and find if item existed in list
	int itemNum = lockExceptionIdList_->size();
	for (int index = 0; index < itemNum; index++) {
		if (lockExceptionIdList_->at(index) == id) {
			// No need to add
			return;
		}
	}

	// Add to list
	lockExceptionIdList_->push_back(id);
}

/**
 * @brief	Remove a control ID out of lock state exception list
 * @param	id - Control ID
 * @return	None
 */
void SDialog::removeLockStateException(unsigned id)
{
	// If the list is not yet allocated, no need to remove
	if (lockExceptionIdList_ == NULL)
		return;

	// Loop through all list and remove item
	int itemNum = lockExceptionIdList_->size();
	for (int index = (itemNum - 1); index >= 0; index--) {
		if (lockExceptionIdList_->at(index) == id) {
			lockExceptionIdList_->erase(lockExceptionIdList_->begin() + index);
		}
	}
}

/**
 * @brief	Remove all lock state exception list
 * @param	id - Control ID
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
	RECT currentPositionRect;
	this->GetWindowRect(&currentPositionRect);

	// Get dialog alignment
	unsigned alignment = getAlignment();

	// Calculate moving delta by alignments
	LONG dx = 0, dy = 0;

	// --> Calculate horizontal delta
	if (alignment & SDA_LEFTALIGN) {
		// Move left rect
		dx = position._x - currentPositionRect.left;
	}
	else if (alignment & SDA_RIGHTALIGN) {
		// Move right rect
		dx = position._x - currentPositionRect.right;
	}
	else if (alignment & SDA_HCENTERALIGN) {
		// Move center rect
		LONG nHCenter = currentPositionRect.right - currentPositionRect.left;
		dx = position._x - nHCenter;
	}

	// --> Calculate vertical delta
	if (alignment & SDA_TOPALIGN) {
		// Move top rect
		dy = position._y - currentPositionRect.top;
	}
	else if (alignment & SDA_BOTTOMALIGN) {
		// Move bottom rect
		dy = position._y - currentPositionRect.bottom;
	}
	else if (alignment & SDA_VCENTERALIGN) {
		// Move center rect
		LONG verticalCenter = currentPositionRect.bottom - currentPositionRect.top;
		dy = position._y - verticalCenter;
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
	RECT dialogRect;
	this->GetWindowRect(&dialogRect);

	// Shift rectangle
	if (dx != 0) {
		// Move horizontal direction --> Shift X value
		dialogRect.left += dx;
		dialogRect.right += dx;
	}
	if (dy != 0) {
		// Move vertical direction --> Shift Y value
		dialogRect.top += dy;
		dialogRect.bottom += dy;
	}

	// Move dialog
	if ((dx != 0) || (dy != 0)) {
		// Top-left point
		LONG x = dialogRect.left;
		LONG y = dialogRect.top;
		// Set dialog position
		bool returnFlag = this->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		// If moving successfully, update new rectangle 
		if ((returnFlag == true) && (newRect != nullptr)) {
			newRect->_left = dialogRect.left;
			newRect->_right = dialogRect.right;
			newRect->_top = dialogRect.top;
			newRect->_bottom = dialogRect.bottom;
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
	RECT dialogRect;
	this->GetWindowRect(&dialogRect);

	// Set new rectangle
	if (!registeredSize_.isEmpty()) {
		dialogRect.right = (dialogRect.left + registeredSize_.width());
		dialogRect.bottom = (dialogRect.top + registeredSize_.height());
	}

	// Resize
	SetWindowPos(NULL, 0, 0, (dialogRect.right - dialogRect.left), (dialogRect.bottom - dialogRect.top), SWP_NOMOVE | SWP_NOZORDER);

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
	RECT currentRect;
	this->GetWindowRect(&currentRect);

	// If current size is default size, do nothing
	LONG currentWidth = (currentRect.right - currentRect.left);
	LONG currentHeight = (currentRect.bottom - currentRect.top);
	if ((currentWidth == defaultSize_.width()) && (currentHeight == defaultSize_.height()))
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
	bool isHorizontalCenter = (currentMargin.left() == currentMargin.right());
	bool isVerticalCenter = (currentMargin.top() == currentMargin.bottom());

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
		if (isHorizontalCenter == true) {
			newMargin._left = currentMargin.left() + (curDispAreaSize.width() - newDispAreaSize.width());
			newMargin._right = newMargin.left();
		}
		// Vertical center margin
		if (isVerticalCenter == true) {
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
	LANGTABLE_PTR languageTablePtr = ((SWinApp*)AfxGetApp())->getAppLanguage();

	String captionString = getLanguageString(languageTablePtr, langStringId);
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
 * @param	captionId  - ID of message caption string
 * @param	lpszCaption	- Message caption string
 * @return	None
 */
void SDialog::registerMessageBoxCaption(unsigned captionId)
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((SWinApp*)AfxGetApp())->getAppLanguage();
	String captionString = Constant::String::Empty;
	if (captionId != NULL) {

		// Get language string caption
		String langCaption = getLanguageString(languageTablePtr, captionId);
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
 * @param	captionId - ID of message caption string
 * @param	nStyle	   - Message box style
 * @return	int	- Result of message box
 */
int SDialog::displayMessageBox(unsigned promptId, unsigned captionId /* = NULL */, unsigned style /* = NULL */)
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((SWinApp*)AfxGetApp())->getAppLanguage();

	String messagePrompt = getLanguageString(languageTablePtr, promptId);
	String messageCaption = ((SWinApp*)AfxGetApp())->getAppWindowCaption();
	if (captionId != NULL) {

		// Get language string caption
		String langCaption = getLanguageString(languageTablePtr, captionId);
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
	int result = displayMessageBox(messagePrompt, messageCaption, style);

	return result;
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
void SDialog::outputEventLog(uint16 eventId, const wchar_t* description /* = NULL */, LOGDETAILINFO* detailInfoPtr /* = NULL */)
{
	// Prepare event log info
	LOGITEM logItemDialogEvent;
	logItemDialogEvent.setCategory(eventId);
	logItemDialogEvent.setTime(DateTimeUtils::getCurrentDateTime());
	logItemDialogEvent.setProcessId();
	if (description) {
		// Include event description
		logItemDialogEvent.setLogString(description);
	}
	if (detailInfoPtr != NULL) {
		// Include event detail info data
		for (int index = 0; index < detailInfoPtr->size(); index++) {
			logItemDialogEvent.addDetail(detailInfoPtr->at(index));
		}
	}

	// Output dialog event log
	SWinApp* theAppPtr = (SWinApp*)AfxGetApp();
	ASSERT(theAppPtr);
	if (theAppPtr == NULL) return;
	if (Logger* appEventLoggerPtr = theAppPtr->getAppEventLog()) {
		appEventLoggerPtr->outputItem(logItemDialogEvent);
	}
}

/**
 * @brief	Output button event log
 * @param	eventId	  - Event ID
 * @param	buttonId - Button ID
 * @return	None
 */
void SDialog::outputButtonLog(uint16 eventId, unsigned buttonId)
{
	// Prepare button event log info
	CButton* buttonPtr = (CButton*)GetDlgItem(buttonId);
	if (buttonPtr == NULL) return;

	// Get button caption
	const int captionLength = buttonPtr->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(captionLength + 1);
	buttonPtr->GetWindowText(tempBuff.data(), captionLength + 1);
	String buttonCaption = tempBuff.data();

	// Detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Button ID
		logDetailInfo.addDetail(EventDetail::ResourceID, buttonId);

		// Mapped ID
		logDetailInfo.addDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(buttonId)));
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
void SDialog::outputCheckBoxLog(uint16 eventId, unsigned checkboxId)
{
	// Prepare checkbox event log info
	CButton* checkboxPtr = (CButton*)GetDlgItem(checkboxId);
	if (checkboxPtr == NULL) return;

	// Get checkbox caption
	const int captionLength = checkboxPtr->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(captionLength + 1);
	checkboxPtr->GetWindowText(tempBuff.data(), captionLength + 1);
	String checkBoxCaption = tempBuff.data();

	// Detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Checkbox ID
		logDetailInfo.addDetail(EventDetail::ResourceID, checkboxId);

		// Mapped ID
		logDetailInfo.addDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(checkboxId)));

		// Checkbox checked state
		logDetailInfo.addDetail(EventDetail::CheckState, checkboxPtr->GetCheck());
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
void SDialog::outputRadButtonLog(uint16 eventId, unsigned radButtonId)
{
	// Prepare radio button event log info
	CButton* radioButtonPtr = (CButton*)GetDlgItem(radButtonId);
	if (radioButtonPtr == NULL) return;

	// Get radio button caption
	const int captionLength = radioButtonPtr->GetWindowTextLength();
	std::vector<wchar_t> tempBuff(captionLength + 1);
	radioButtonPtr->GetWindowText(tempBuff.data(), captionLength + 1);
	String radButtonCaption = tempBuff.data();

	// Detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Radio button ID
		logDetailInfo.addDetail(EventDetail::ResourceID, radButtonId);

		// Mapped ID
		logDetailInfo.addDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(radButtonId)));

		// Radio button checked state
		logDetailInfo.addDetail(EventDetail::CheckState, radioButtonPtr->GetCheck());
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
void SDialog::outputComboBoxLog(uint16 eventId, unsigned comboId)
{
	// Prepare combo-box event log info
	CComboBox* comboBoxPtr = (CComboBox*)GetDlgItem(comboId);
	if (comboBoxPtr == NULL) return;

	// Detail info
	String comboBoxCaption;
	LOGDETAILINFO logDetailInfo;
	{
		// Combo-box ID
		logDetailInfo.addDetail(EventDetail::ResourceID, comboId);

		// Mapped ID
		logDetailInfo.addDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(comboId)));

		// Combo-box control info
		SControlManager* pCtrlMan = getControlManager();
		if (pCtrlMan != NULL) {
			SCtrlInfoWrap* comboInfoWrapPtr = pCtrlMan->getControl(comboId);
			if (comboInfoWrapPtr != NULL) {
				// Combo-box caption
				comboInfoWrapPtr->getCaption(comboBoxCaption);

				// Combo-box current selection string
				size_t currenSelection = comboInfoWrapPtr->getInteger();
				StringArray dataList;
				comboInfoWrapPtr->getStringArray(dataList);
				if ((!dataList.empty()) && (dataList.size() > currenSelection)) {
					logDetailInfo.addDetail(EventDetail::Selection, dataList.at(currenSelection));
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
void SDialog::outputEditBoxLog(uint16 eventId, unsigned editId)
{
	// Prepare edit box event log info
	CEdit* editCtrlPtr = (CEdit*)GetDlgItem(editId);
	if (editCtrlPtr == NULL) return;

	// Detail info
	String editBoxCaption;
	LOGDETAILINFO logDetailInfo;
	{
		// Edit box ID
		logDetailInfo.addDetail(EventDetail::ResourceID, editId);

		// Mapped ID
		logDetailInfo.addDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(editId)));

		// Edit box control info
		SControlManager* pCtrlMan = getControlManager();
		if (pCtrlMan != NULL) {
			SCtrlInfoWrap* pEditBoxWrap = pCtrlMan->getControl(editId);
			if (pEditBoxWrap != NULL) {
				// Edit box caption
				pEditBoxWrap->getCaption(editBoxCaption);

				// Edit box content
				String editBoxContent;
				pEditBoxWrap->getString(editBoxContent);
				logDetailInfo.addDetail(EventDetail::DataValue, editBoxContent);
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
void SDialog::outputListBoxLog(uint16 eventId, unsigned listBoxId)
{
	// Prepare list box event log info
	CListBox* listBoxPtr = (CListBox*)GetDlgItem(listBoxId);
	if (listBoxPtr == NULL) return;

	// Detail info
	String listBoxCaption;
	LOGDETAILINFO logDetailInfo;
	{
		// List box ID
		logDetailInfo.addDetail(EventDetail::ResourceID, listBoxId);

		// Mapped ID
		logDetailInfo.addDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(listBoxId)));

		// List box control info
		SControlManager* pCtrlMan = getControlManager();
		if (pCtrlMan != NULL) {
			SCtrlInfoWrap* listBoxInfoWrapPtr = pCtrlMan->getControl(listBoxId);
			if (listBoxInfoWrapPtr != NULL) {
				// List box caption
				listBoxInfoWrapPtr->getCaption(listBoxCaption);

				// List box current selection string
				size_t currenSelection = listBoxInfoWrapPtr->getInteger();
				StringArray dataList;
				listBoxInfoWrapPtr->getStringArray(dataList);
				if ((!dataList.empty()) && (dataList.size() > currenSelection)) {
					logDetailInfo.addDetail(EventDetail::Selection, dataList.at(currenSelection));
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
void SDialog::outputSpinCtrlLog(uint16 eventId, unsigned spinCtrlId)
{
	// Prepare spin control event log info
	CSpinButtonCtrl* spinCtrlPtr = (CSpinButtonCtrl*)GetDlgItem(spinCtrlId);
	if (spinCtrlPtr == NULL) return;

	// Detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Spin control ID
		logDetailInfo.addDetail(EventDetail::ResourceID, spinCtrlId);

		// Mapped ID
		logDetailInfo.addDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(spinCtrlId)));
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
void SDialog::outputMenuLog(uint16 eventId, unsigned menuItemId)
{
	// Prepare menu event log info
	// Get menu item title from language table
	String menuItemCaption;
	LANGTABLE_PTR languageTablePtr = ((SWinApp*)AfxGetApp())->getAppLanguage();
	if (languageTablePtr != NULL) {
		menuItemCaption = getLanguageString(languageTablePtr, menuItemId);
	}

	// Detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Menu item ID
		logDetailInfo.addDetail(EventDetail::ResourceID, menuItemId);

		// Mapped ID
		logDetailInfo.addDetail(EventDetail::NameID, MAKEUNICODE(GET_NAME_ID(menuItemId)));
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
	LANGTABLE_PTR languageTablePtr = ((SWinApp*)AfxGetApp())->getAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		unsigned id = childWndPtr->GetDlgCtrlID();

		switch (id)
		{
		case IDOK:
		case IDCANCEL:
			setControlText(childWndPtr, id, languageTablePtr);
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
	CWnd* buttonPtr = GetDlgItem(buttonId);
	if (!buttonPtr) return;

	// Backup title
	String buttonTitle;
	if (reupdateTitle == true) {
		const int captionLength = buttonPtr->GetWindowTextLength();
		std::vector<wchar_t> tempBuff(captionLength + 1);
		buttonPtr->GetWindowText(tempBuff.data(), captionLength + 1);
		buttonTitle = tempBuff.data();
	}

	// Icon size
	int cx = Constant::UI::Button::IconWidth;
	int cy = Constant::UI::Button::IconHeight;

	// Load icon from resource
	HINSTANCE instanceHandle = AfxGetApp()->m_hInstance;
	HICON hIcon = (HICON)LoadImage(instanceHandle, MAKEINTRESOURCE(iconId), IMAGE_ICON, cx, cy, LR_DEFAULTCOLOR);
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
void SDialog::moveItemGroup(const uint32_array& controlIdGroup, const Point& newPosition)
{
	// Check data validity
	if (controlIdGroup.empty())
		return;

	RECT controlRect;
	CWnd* controlPtr = NULL;
	LONG originX = INT_INVALID, originY = INT_INVALID;

	// Find the original point
	for (int index = 0; index < controlIdGroup.size(); index++)
	{
		// Get item
		controlPtr = GetDlgItem(controlIdGroup.at(index));
		if (controlPtr == NULL)
			continue;

		// Get item rect
		controlPtr->GetWindowRect(&controlRect);

		// Find smallest X
		if ((originX == INT_INVALID) || (controlRect.left <= originX)) {
			originX = controlRect.left;
		}

		// Find smallest Y
		if ((originY == INT_INVALID) || (controlRect.top <= originY)) {
			originY = controlRect.top;
		}
	}

	// Calculate moving distance
	int deltaX = newPosition._x - originX;
	int deltaY = newPosition._y - originY;

	// Move all items to new position
	int nNewX = 0, nNewY = 0;
	for (int index = 0; index < controlIdGroup.size(); index++)
	{
		// Get item
		controlPtr = GetDlgItem(controlIdGroup.at(index));
		if (controlPtr == NULL)
			continue;

		// Get item rect
		controlPtr->GetWindowRect(&controlRect);

		// Move item
		nNewX = controlRect.left + deltaX;
		nNewY = controlRect.top + deltaY;
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
void SDialog::moveItemGroup(const uint32_array& controlIdGroup, int direction, int distance)
{
	// Check data validity
	if (controlIdGroup.empty())
		return;

	RECT controlRect;
	CWnd* controlPtr = NULL;
	LONG newX = 0, newY = 0;

	// Loop through each item and move
	for (int index = 0; index < controlIdGroup.size(); index++)
	{
		// Get item
		controlPtr = GetDlgItem(controlIdGroup.at(index));
		if (controlPtr == NULL)
			continue;

		// Get item rect
		controlPtr->GetWindowRect(&controlRect);

		// Move horizontal direction
		if (direction == MOVDIR_HORIZONTAL) {
			newX = controlRect.left + distance;
			newY = controlRect.top;
			controlPtr->SetWindowPos(NULL, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		// Move vertical direction
		else if (direction == MOVDIR_VERTICAL) {
			newX = controlRect.left;
			newY = controlRect.top + distance;
			controlPtr->SetWindowPos(NULL, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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
 * @param	optionId - ID of specific option
 * @param	isTemp		 - Temp value or saved value (saved value by default)
 * @return	int - Option value
 */
int SDialog::getAppOption(AppOptionID optionId, bool isTemp /* = false */) const
{
	int result = INT_INVALID;
	int tempResult = INT_INVALID;

	// Acquire option value from application main window
	SDialog* mainDialogPtr = (SDialog*)AfxGetMainWnd();
	if (mainDialogPtr != NULL) {
		result = mainDialogPtr->getAppOption(optionId, false);
		tempResult = mainDialogPtr->getAppOption(optionId, true);
	}

	// Return temp data if required and the result is valid
	if ((isTemp == true) && (tempResult != INT_INVALID))
		return tempResult;

	return result;
}

/**
 * @brief	Return flag value by ID
 * @param	flagId - ID of specific flag
 * @return	int - Flag value
 */
int SDialog::getFlagValue(AppFlagID flagId) const
{
	int value = FLAG_OFF;

	switch (flagId)
	{
	// Special dialog-base flags (not managed by FlagManager)
	case AppFlagID::dialogSetMinSize:
		value = (!minSize_.isEmpty() &&	!minSize_.isZero());
		break;
	case AppFlagID::dialogSetMaxSize:
		value = (maxSize_ > minSize_);
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
		value = flagManager_.getFlagValue(flagId);
		break;

	default:
		// Request the flag value from application
		value = ((SWinApp*)AfxGetApp())->getFlagValue(flagId);
		break;
	}

	return value;
}

/**
 * @brief	Update flag value by ID
 * @param	flagId - ID of specific flag
 * @param	value  - Value to set
 * @return	None
 */
void SDialog::setFlagValue(AppFlagID flagId, int value)
{
	// Check value validity
	if (value == INT_INVALID)
		return;

	switch (flagId)
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
		flagManager_.setFlagValue(flagId, value);
		break;

	default:
		// Let the application manage the flags
		((SWinApp*)AfxGetApp())->setFlagValue(flagId, value);
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
