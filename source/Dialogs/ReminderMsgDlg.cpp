/**
 * @file		ReminderMsgDlg.cpp
 * @brief		Source file for Reminder Message dialog
 * @author		AnthonyLeeStark
 * @date		2024.08.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "Dialogs/ReminderMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace AppCore;


// Dialog constant
constexpr const int kDefaultTextIconSpacing = 10;
constexpr const int kScreenCornerHMargin = 20;
constexpr const int kScreenCornerVMargin = 20;


// Implement methods for CReminderMsgDlg
IMPLEMENT_DYNAMIC(CReminderMsgDlg, SDialog)


/**
 * @brief	Constructor
 */
CReminderMsgDlg::CReminderMsgDlg(CWnd* parentWnd /*= NULL*/)
	: SDialog(IDD_REMINDERMSG_DLG, parentWnd)
{
	// Message string buffer
	bufferString_ = Constant::String::Empty;

	// Message font & icon
	messageFontPtr_ = NULL;
	messageIconHandle_ = NULL;
	iconSize_ = SIZE_NULL;

	// Flags
	isTimerSet_ = false;
	isDisplayIcon_ = false;
	isLockDialogSize_ = false;
	isLockFontSize_ = false;
	isSnoozingAllowed_ = false;
	snoozeFlag_ = FLAG_OFF;

	// Properties
	backgroundColor_ = Color::White;
	textColor_ = Color::Black;
	autoCloseInterval_ = 0;
}

/**
 * @brief	Destructor
 */
CReminderMsgDlg::~CReminderMsgDlg()
{
	// Delete font info
	if (messageFontPtr_ != NULL) {
		delete messageFontPtr_;
		messageFontPtr_ = NULL;
	}

	// Destroy message icon
	::DestroyIcon(messageIconHandle_);
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CReminderMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


// CReminderMsgDlg dialog message map
BEGIN_MESSAGE_MAP(CReminderMsgDlg, SDialog)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// CReminderMsgDlg message handlers

/**
 * @brief	Initialize DebugTest dialog
 * @param	None
 * @return	BOOL - Default
 */
BOOL CReminderMsgDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// If message style initialization failed
	if (!initMessageStyle())
		return false;

	// Set margin
	int marginHorizontal = messageStyleData_.getHorizontalMargin();
	int marginVertical = messageStyleData_.getVerticalMargin();
	setCenterMargin(marginHorizontal, marginVertical);

	// Shift margin if icon is displaying
	if ((isDisplayIcon_ == true) && (messageIconHandle_ != NULL)) {

		// Get current margin
		Rect dialogMargin;
		this->getMargin(dialogMargin);

		byte iconPosVal = messageStyleData_.getIconPosition();
		if (iconPosVal == MsgIconPosition::IconOnTheTop) {

			// Shift top margin
			dialogMargin._top += iconSize_.height() + kDefaultTextIconSpacing;
			this->setTopMargin(dialogMargin.top());
		}
		else if (iconPosVal == MsgIconPosition::IconOnTheLeft) {

			// Shift left margin
			dialogMargin._left += iconSize_.width() + kDefaultTextIconSpacing;
			this->setLeftMargin(dialogMargin.left());
		}
	}

	// Get display area
	Rect displayArea;
	getDisplayArea(displayArea);

	// Get dialog size
	Size dialogSize;
	this->getSize(dialogSize);

	// If set lock font size
	if (isLockFontSize_ == true) {

		// Calculate text rectangle
		textToClient(displayArea);

		// Set display area and resize dialog
		setDisplayArea(displayArea, true, true);
	}

	// Display message content
	CWnd* messageCtrlPtr = this->GetDlgItem(IDC_REMINDERMSG_MSGTEXT_STATIC);
	if (messageCtrlPtr != NULL) {

		// Fix text display area size and position
		RECT newAreaRect;
		newAreaRect.left = displayArea.left();
		newAreaRect.top = displayArea.top();
		newAreaRect.right = displayArea.right();
		newAreaRect.bottom = displayArea.bottom();
		messageCtrlPtr->MoveWindow(&newAreaRect);

		// Set text font
		if (messageFontPtr_ != NULL) {
			messageCtrlPtr->SetFont(messageFontPtr_);
		}

		// Display text
		messageCtrlPtr->SetWindowText(bufferString_);
	}

	// Start auto-close timer if set
	if (getAutoCloseInterval() != 0) {
		unsigned returnValue = SetTimer(enumToValue(TimerId::ReminderMsgAutoClose), 1000, NULL);
		isTimerSet_ = (returnValue != 0);
	}

	// Move to specific display position
	moveToDisplayPosition((MsgDispPosition)messageStyleData_.getDisplayPosition());

	// Bring to top (by default)
	this->PostMessage(SM_WND_SHOWDIALOG, true);

	return true;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void CReminderMsgDlg::OnClose()
{
	// Kill timer if set
	if (isTimerSet_ == true && KillTimer(enumToValue(TimerId::ReminderMsgAutoClose)))
		isTimerSet_ = false;	// Reset flag

	// Close dialog
	SDialog::OnClose();
}

/**
 * @brief	Destroy dialog
 * @param	Noíe
 * @return	None
 */
void CReminderMsgDlg::OnDestroy()
{
	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	PostNcDestroy function
 * @param	None
 * @return	None
 */
void CReminderMsgDlg::PostNcDestroy()
{
	// Default
	SDialog::PostNcDestroy();
}

/**
 * @brief	OnPaint function
 * @param	None
 * @return	None
 */
void CReminderMsgDlg::OnPaint()
{
	CPaintDC dc(this);

	// Draw message icon
	if ((isDisplayIcon_ == true) && (messageIconHandle_ != NULL)) {
		// Calculate icon postion
		Point iconPosition;
		calcMsgIconPosition(iconPosition);
		// Get icon size
		int cx = iconSize_.width();
		int cy = iconSize_.height();
		// Draw icon (with scale up/down);
		DrawIconEx(dc, iconPosition._x, iconPosition._y, messageIconHandle_, cx, cy, NULL, NULL, DI_NORMAL);
		return;
	}

	// Default
	SDialog::OnPaint();
}

/**
 * @brief	OnTimer function
 * @param	eventId - Time event ID
 * @return	None
 */
void CReminderMsgDlg::OnTimer(UINT_PTR eventId)
{
	// Handle auto-close timer if set
	if (eventId == enumToValue(TimerId::ReminderMsgAutoClose)) {
		if (isTimerSet_ == true) {
			unsigned count = getAutoCloseInterval();
			if (count > 0) {
				// Countdown
				setAutoCloseInterval(--count);
			}
			if (count == 0) {
				// Close message
				EndDialog(IDCANCEL);
			}
		}
	}

	// Default
	SDialog::OnTimer(eventId);
}

/**
 * @brief	OnCreate function
 * @param	Default
 * @return	int - Default
 */
int CReminderMsgDlg::OnCreate(LPCREATESTRUCT createStructPtr)
{
	// Default creation
	if (SDialog::OnCreate(createStructPtr) == Constant::InvalidInteger)
		return Constant::InvalidInteger;

	// Set allow snoozing mode
	if (getAllowSnoozeMode() == true) {
		// Display minimize button
		LONG currentStyle = GetWindowLong(this->m_hWnd, GWL_STYLE);
		SetWindowLong(this->m_hWnd, GWL_STYLE, currentStyle | WS_MINIMIZEBOX);
	}

	return 0;
}

/**
 * @brief	OnSysCommand function
 * @param	Default
 * @return	None
 */
void CReminderMsgDlg::OnSysCommand(UINT id, LPARAM lParam)
{
	if ((id & 0xFFF0) == SC_MINIMIZE) {
		// Handle minimize button event
		if (getAllowSnoozeMode() == true) {
			// Trigger snooze mode
			setSnoozeTriggerFLag(FLAG_ON);
		}
		EndDialog(IDOK);
	}
	else {
		// Execute default syscommand
		SDialog::OnSysCommand(id, lParam);
	}
}

/**
 * @brief	WM_CTLCOLOR handling function
 * @param	Default
 * @return	HBRUSH
 */
HBRUSH CReminderMsgDlg::OnCtlColor(CDC* pDC, CWnd* windowPtr, UINT nCtlColor)
{
	// Default initialization
	HBRUSH hBrush = SDialog::OnCtlColor(pDC, windowPtr, nCtlColor);

	if (windowPtr == this) {
		if ((getFlagValue(AppFlagID::dialogSetBackgroundColor) == true) && (backgroudBrush_ != NULL)) {
			// Get brush
			hBrush = (HBRUSH)(*backgroudBrush_);
		}
	}

	if (nCtlColor == CTLCOLOR_STATIC) {
		// Set background color
		if ((getFlagValue(AppFlagID::dialogSetBackgroundColor) == true) && (backgroudBrush_ != NULL)) {
			hBrush = (HBRUSH)(*backgroudBrush_);
			pDC->SetBkColor(backgroundColor_);
		}
		// Set text color
		if (getFlagValue(AppFlagID::dialogSetTextColor) == true) {
			pDC->SetTextColor(textColor_);
		}
	}

	return hBrush;
}

/**
 * @brief	Handle window messages
 * @param	message - Message ID
 * @param	wParam - First param (HIWORD)
 * @param	lParam - Second param (LOWORD)
 * @return	LRESULT
 */
LRESULT CReminderMsgDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case SM_WND_SHOWDIALOG:
		{
			// Get flag value
			bool showFlag = static_cast<bool>(wParam);

			// Show/hide dialog
			if (showFlag == true) {

				// Show dialog
				this->ShowWindow(SW_SHOW);

				// Bring window to top
				this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				this->SetForegroundWindow();
				this->BringWindowToTop();

				// If the application is not foreground
				if (GetForegroundWindow() != this)
				{
					// Flash the taskbar
					FLASHWINFO fwInfo = { sizeof(FLASHWINFO) };
					fwInfo.hwnd = this->m_hWnd;
					fwInfo.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
					fwInfo.uCount = 5;
					fwInfo.dwTimeout = 0;
					::FlashWindowEx(&fwInfo);

					// Show tray icon balloon tip
					if (HWND mainWndHandle = GET_HANDLE_MAINWND()) {
						WPARAM wParam = MAKE_WPARAM_STRING(this->getDispMessage());
						LPARAM lParam = MAKE_LPARAM_STRING(this->getDispMessage());
						::PostMessage(mainWndHandle, SM_APP_SHOW_REMINDER_BALLOON_TIP, wParam, lParam);
					}
				}
			}
			else {
				// Hide dialog
				this->ShowWindow(SW_HIDE);
			}

			return true;
		}
	}

	// Default
	return SDialog::WindowProc(message, wParam, lParam);
}

/**
 * @brief	Initialize message style info data
 * @param	None
 * @return	true/false
 */
bool CReminderMsgDlg::initMessageStyle(void)
{
	// Background & text color
	setBackgroundColor(messageStyleData_.getBkgrdColor());
	setTextColor(messageStyleData_.getTextColor());
	if (!createBrush()) {

		// Error: Apply color failed
		TRACE_ERROR("Error: Apply color failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Trigger force redrawing
	this->Invalidate();

	// Reset font
	if (messageFontPtr_ != NULL)
		delete messageFontPtr_;

	// Reset flag
	isLockFontSize_ = false;

	// Initialize message font
	messageFontPtr_ = new CFont;
	ASSERT(messageFontPtr_ != NULL);
	if (messageFontPtr_ != NULL) {

		// Create font
		int fontPointSize = int(messageStyleData_.getFontSize() * 10);
		String fontName = messageStyleData_.getFontName();
		if (!messageFontPtr_->CreatePointFont(fontPointSize, fontName, NULL)) {

			// Error: Create font failed
			TRACE_ERROR("Error: Create font failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
	}

	// Set lock font size flag
	isLockFontSize_ = true;

	// Icon size
	iconSize_._width = messageStyleData_.getIconSize();
	iconSize_._height = messageStyleData_.getIconSize();

	int cx = iconSize_.width();
	int cy = iconSize_.height();

	// Invalid input size, reset to system default
	if ((cx < 30) || (cx > 100)) {
		cx = GetSystemMetrics(SM_CXICON);
		cy = GetSystemMetrics(SM_CYICON);
	}

	// If icon ID is not set
	int iconId = messageStyleData_.getIconId();
	if (iconId == NULL) {
		isDisplayIcon_ = false;
		return true;
	}

	// Load system icon by ID and scale size
	HRESULT result = ::LoadIconWithScaleDown(NULL, MAKEINTRESOURCE(iconId), cx, cy, &messageIconHandle_);

	// Load icon failed
	if ((result != S_OK) || (messageIconHandle_ == NULL)) {
		TRACE_ERROR("Error: Load icon failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get icon info
	ICONINFO iiIconInfo;
	bool returnFlag = ::GetIconInfo(messageIconHandle_, &iiIconInfo);
	if (returnFlag == false) {
		TRACE_ERROR("Error: Get icon info failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Update icon size
	BITMAP bmp;
	ZeroMemory(&bmp, sizeof(BITMAP));
	if (iiIconInfo.hbmColor) {
		if (::GetObject(iiIconInfo.hbmColor, sizeof(bmp), &bmp) > 0)
			iconSize_ = Size(bmp.bmWidth, bmp.bmHeight);
	}
	else if (iiIconInfo.hbmMask) {
		if (::GetObject(iiIconInfo.hbmMask, sizeof(bmp), &bmp) > 0)
			iconSize_ = Size(bmp.bmWidth, bmp.bmHeight);
	}

	// Delete icon info temp objects
	if (iiIconInfo.hbmColor)	DeleteObject(iiIconInfo.hbmColor);
	if (iiIconInfo.hbmMask)		DeleteObject(iiIconInfo.hbmMask);

	// Update flag
	isDisplayIcon_ = true;

	return true;
}

/**
 * @brief	Calculate message icon position (top-left point)
 * @param	iconPosition - Icon position (point)
 * @return	true/false
 */
bool CReminderMsgDlg::calcMsgIconPosition(Point& iconPosition) const
{
	// Get display margin
	Rect currentMargin;
	this->getMargin(currentMargin);

	// Get client rectangle
	RECT clientRect;
	this->GetClientRect(&clientRect);

	// Calculate icon top-left point
	int textIconSpacing = kDefaultTextIconSpacing;
	byte iconPosVal = messageStyleData_.getIconPosition();
	if (iconPosVal == MsgIconPosition::IconOnTheTop) {
		iconPosition._y = currentMargin.top() - (iconSize_.height() + textIconSpacing);
		iconPosition._x = ((clientRect.right - clientRect.left) - iconSize_.width()) / 2;
	}
	else if (iconPosVal == MsgIconPosition::IconOnTheLeft) {
		iconPosition._x = currentMargin.left() - (iconSize_.width() + textIconSpacing);
		iconPosition._y = ((clientRect.bottom - clientRect.top) - iconSize_.height()) / 2;
	}

	return true;
}

/**
 * @brief	Move the dialog to specific display position
 * @param	displayPosition - Display position
 * @return	void
 */
void CReminderMsgDlg::moveToDisplayPosition(MsgDispPosition displayPosition)
{
	// Get desktop screen size
	Size screenSize;
	screenSize._width = GetSystemMetrics(SM_CXSCREEN);
	screenSize._height = GetSystemMetrics(SM_CYSCREEN);

	// Get this dialog rect
	RECT dialogRect;
	this->GetWindowRect(&dialogRect);

	// Dialog size
	Size dialogSize;
	dialogSize._width = dialogRect.right - dialogRect.left;
	dialogSize._height = dialogRect.bottom - dialogRect.top;

	// Top-left position
	Point topLeft = Point(dialogRect.left, dialogRect.top);

	// Calculate new top-left position based on display position
	switch (displayPosition)
	{
	case MsgDispPosition::AtCenter:
		topLeft._x = (screenSize.width() - dialogSize.width()) / 2;
		topLeft._y = (screenSize.height() - dialogSize.height()) / 2;
		break;

	case MsgDispPosition::OnTopLeft:
		topLeft._x = kScreenCornerHMargin;
		topLeft._y = kScreenCornerVMargin;
		break;

	case MsgDispPosition::OnTopRight:
		topLeft._y = kScreenCornerVMargin;
		topLeft._x = screenSize.width() - dialogSize.width() - kScreenCornerHMargin;
		break;

	case MsgDispPosition::OnBottomLeft:
		topLeft._x = kScreenCornerHMargin;
		topLeft._y = screenSize.height() - dialogSize.height() - kScreenCornerVMargin;
		break;

	case MsgDispPosition::OnBottomRight:
		topLeft._x = screenSize.width() - dialogSize.width() - kScreenCornerHMargin;
		topLeft._y = screenSize.height() - dialogSize.height() - kScreenCornerVMargin;
		break;

	default:
		return;
	}

	// Move to new position
	this->SetWindowPos(NULL, topLeft.getX(), topLeft.getY(), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

/**
 * @brief	Calculate text rect from dialog client size
 * @param	rect - Returned rect
 * @return	void
 */
void CReminderMsgDlg::clientToText(Rect& /*rect*/) const
{
}

/**
 * @brief	Calculate dialog client rect from text size
 * @param	lpRect - Returned rect
 * @return	void
 */
void CReminderMsgDlg::textToClient(Rect& rect) const
{
	// If message text is not set, do nothing
	if (bufferString_.isEmpty())
		return;

	// Get font
	CFont* messageFontPtr = this->GetFont();
	if (messageFontPtr_ != NULL) {
		messageFontPtr = messageFontPtr_;
	}

	// Invalid font
	if (messageFontPtr == NULL) {
		TRACE_ERROR("Error: Invalid font!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Set draw area
	CWnd* windowPtr = this->GetDlgItem(IDC_REMINDERMSG_MSGTEXT_STATIC);
	if (windowPtr == NULL) {
		TRACE_ERROR("Error: Message text draw area not found!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Set font
	CDC* pDC = windowPtr->GetDC();
	CFont* defaultFontPtr = NULL;
	defaultFontPtr = pDC->SelectObject(messageFontPtr);

	// Backup rectangle
	RECT tempRect;
	tempRect.left = rect.left();
	tempRect.top = rect.top();
	tempRect.right = rect.right();
	tempRect.bottom = rect.bottom();

	// Calculate new client rectangle
	DWORD format = DT_CENTER | DT_WORDBREAK | DT_CALCRECT;
	pDC->DrawText(bufferString_, bufferString_.getLength(), &tempRect, format);
	rect = Rect(tempRect.left, tempRect.top, tempRect.right, tempRect.bottom);

	// Reset device context
	pDC->SelectObject(defaultFontPtr);
	windowPtr->ReleaseDC(pDC);
}
