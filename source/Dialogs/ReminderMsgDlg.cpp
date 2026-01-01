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
constexpr const int defaultTextIconSpacing = 10;
constexpr const int screenCornerHMargin = 20;
constexpr const int screenCornerVMargin = 20;


// Implement methods for CReminderMsgDlg
IMPLEMENT_DYNAMIC(CReminderMsgDlg, SDialog)


/**
 * @brief	Constructor
 */
CReminderMsgDlg::CReminderMsgDlg(CWnd* parentWnd /*= NULL*/)
	: SDialog(IDD_REMINDERMSG_DLG, parentWnd)
{
	// Message string buffer
	m_strBuffer = Constant::String::Empty;

	// Message font & icon
	m_pMsgFont = NULL;
	m_hMsgIcon = NULL;
	m_szIconSize = SIZE_NULL;

	// Flags
	m_bTimerSet = false;
	m_bDispIcon = false;
	m_bLockDlgSize = false;
	m_bLockFontSize = false;
	m_bAllowSnooze = false;
	m_nSnoozeFlag = FLAG_OFF;

	// Properties
	backgroundColor_ = Color::White;
	textColor_ = Color::Black;
	m_nAutoCloseInterval = 0;
}

/**
 * @brief	Destructor
 */
CReminderMsgDlg::~CReminderMsgDlg()
{
	// Delete font info
	if (m_pMsgFont != NULL) {
		delete m_pMsgFont;
		m_pMsgFont = NULL;
	}

	// Destroy message icon
	::DestroyIcon(m_hMsgIcon);
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
	if (!InitMessageStyle())
		return false;

	// Set margin
	int nHMargin = m_rmdMsgStyleSet.getHorizontalMargin();
	int nVMargin = m_rmdMsgStyleSet.getVerticalMargin();
	setCenterMargin(nHMargin, nVMargin);

	// Shift margin if icon is displaying
	if ((m_bDispIcon == true) && (m_hMsgIcon != NULL)) {

		// Get current margin
		Rect dialogMargin;
		this->getMargin(dialogMargin);

		byte iconPosVal = m_rmdMsgStyleSet.getIconPosition();
		if (iconPosVal == MsgIconPosition::IconOnTheTop) {

			// Shift top margin
			dialogMargin._top += m_szIconSize.height() + defaultTextIconSpacing;
			this->setTopMargin(dialogMargin.top());
		}
		else if (iconPosVal == MsgIconPosition::IconOnTheLeft) {

			// Shift left margin
			dialogMargin._left += m_szIconSize.width() + defaultTextIconSpacing;
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
	if (m_bLockFontSize == true) {

		// Calculate text rectangle
		TextToClient(displayArea);

		// Set display area and resize dialog
		setDisplayArea(displayArea, true, true);
	}

	// Display message content
	CWnd* pWndMsg = this->GetDlgItem(IDC_REMINDERMSG_MSGTEXT_STATIC);
	if (pWndMsg != NULL) {

		// Fix text display area size and position
		RECT rcNewArea;
		rcNewArea.left = displayArea.left();
		rcNewArea.top = displayArea.top();
		rcNewArea.right = displayArea.right();
		rcNewArea.bottom = displayArea.bottom();
		pWndMsg->MoveWindow(&rcNewArea);

		// Set text font
		if (m_pMsgFont != NULL) {
			pWndMsg->SetFont(m_pMsgFont);
		}

		// Display text
		pWndMsg->SetWindowText(m_strBuffer);
	}

	// Start auto-close timer if set
	if (GetAutoCloseInterval() != 0) {
		unsigned returnValue = SetTimer(TIMERID_RMDMSG_AUTOCLOSE, 1000, NULL);
		m_bTimerSet = (returnValue != 0);
	}

	// Move to specific display position
	MoveToDisplayPosition((MsgDispPosition)m_rmdMsgStyleSet.getDisplayPosition());

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
	if (m_bTimerSet == true && KillTimer(TIMERID_RMDMSG_AUTOCLOSE))
		m_bTimerSet = false;	// Reset flag

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
	if ((m_bDispIcon == true) && (m_hMsgIcon != NULL)) {
		// Calculate icon postion
		Point iconPosition;
		CalcMsgIconPosition(iconPosition);
		// Get icon size
		int cx = m_szIconSize.width();
		int cy = m_szIconSize.height();
		// Draw icon (with scale up/down);
		DrawIconEx(dc, iconPosition._x, iconPosition._y, m_hMsgIcon, cx, cy, NULL, NULL, DI_NORMAL);
		return;
	}

	// Default
	SDialog::OnPaint();
}

/**
 * @brief	OnTimer function
 * @param	nIDEvent - Time event ID
 * @return	None
 */
void CReminderMsgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// Handle auto-close timer if set
	if (nIDEvent == TIMERID_RMDMSG_AUTOCLOSE) {
		if (m_bTimerSet == true) {
			unsigned count = GetAutoCloseInterval();
			if (count > 0) {
				// Countdown
				SetAutoCloseInterval(--count);
			}
			if (count == 0) {
				// Close message
				EndDialog(IDCANCEL);
			}
		}
	}

	// Default
	SDialog::OnTimer(nIDEvent);
}

/**
 * @brief	OnCreate function
 * @param	Default
 * @return	int - Default
 */
int CReminderMsgDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Default creation
	if (SDialog::OnCreate(lpCreateStruct) == INT_INVALID)
		return INT_INVALID;

	// Set allow snoozing mode
	if (GetAllowSnoozeMode() == true) {
		// Display minimize button
		LONG lCurStyle = GetWindowLong(this->m_hWnd, GWL_STYLE);
		SetWindowLong(this->m_hWnd, GWL_STYLE, lCurStyle | WS_MINIMIZEBOX);
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
		if (GetAllowSnoozeMode() == true) {
			// Trigger snooze mode
			SetSnoozeTriggerFLag(FLAG_ON);
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
						WPARAM wParam = MAKE_WPARAM_STRING(this->GetDispMessage());
						LPARAM lParam = MAKE_LPARAM_STRING(this->GetDispMessage());
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
bool CReminderMsgDlg::InitMessageStyle(void)
{
	// Background & text color
	setBackgroundColor(m_rmdMsgStyleSet.getBkgrdColor());
	setTextColor(m_rmdMsgStyleSet.getTextColor());
	if (!createBrush()) {

		// Error: Apply color failed
		TRACE_ERROR("Error: Apply color failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Trigger force redrawing
	this->Invalidate();

	// Reset font
	if (m_pMsgFont != NULL)
		delete m_pMsgFont;

	// Reset flag
	m_bLockFontSize = false;

	// Initialize message font
	m_pMsgFont = new CFont;
	ASSERT(m_pMsgFont != NULL);
	if (m_pMsgFont != NULL) {

		// Create font
		int nFontPointSize = int(m_rmdMsgStyleSet.getFontSize() * 10);
		String fontName = m_rmdMsgStyleSet.getFontName();
		if (!m_pMsgFont->CreatePointFont(nFontPointSize, fontName, NULL)) {

			// Error: Create font failed
			TRACE_ERROR("Error: Create font failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
	}

	// Set lock font size flag
	m_bLockFontSize = true;

	// Icon size
	m_szIconSize._width = m_rmdMsgStyleSet.getIconSize();
	m_szIconSize._height = m_rmdMsgStyleSet.getIconSize();

	int cx = m_szIconSize.width();
	int cy = m_szIconSize.height();

	// Invalid input size, reset to system default
	if ((cx < 30) || (cx > 100)) {
		cx = GetSystemMetrics(SM_CXICON);
		cy = GetSystemMetrics(SM_CYICON);
	}

	// If icon ID is not set
	int nIconID = m_rmdMsgStyleSet.getIconId();
	if (nIconID == NULL) {
		m_bDispIcon = false;
		return true;
	}

	// Load system icon by ID and scale size
	HRESULT hResult = ::LoadIconWithScaleDown(NULL, MAKEINTRESOURCE(nIconID), cx, cy, &m_hMsgIcon);

	// Load icon failed
	if ((hResult != S_OK) || (m_hMsgIcon == NULL)) {
		TRACE_ERROR("Error: Load icon failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get icon info
	ICONINFO iiIconInfo;
	bool returnFlag = ::GetIconInfo(m_hMsgIcon, &iiIconInfo);
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
			m_szIconSize = Size(bmp.bmWidth, bmp.bmHeight);
	}
	else if (iiIconInfo.hbmMask) {
		if (::GetObject(iiIconInfo.hbmMask, sizeof(bmp), &bmp) > 0)
			m_szIconSize = Size(bmp.bmWidth, bmp.bmHeight);
	}

	// Delete icon info temp objects
	if (iiIconInfo.hbmColor)	DeleteObject(iiIconInfo.hbmColor);
	if (iiIconInfo.hbmMask)		DeleteObject(iiIconInfo.hbmMask);

	// Update flag
	m_bDispIcon = true;

	return true;
}

/**
 * @brief	Calculate message icon position (top-left point)
 * @param	iconPosition - Icon position (point)
 * @return	true/false
 */
bool CReminderMsgDlg::CalcMsgIconPosition(Point& iconPosition) const
{
	// Get display margin
	Rect currentMargin;
	this->getMargin(currentMargin);

	// Get client rectangle
	RECT rcClient;
	this->GetClientRect(&rcClient);

	// Calculate icon top-left point
	int textIconSpacing = defaultTextIconSpacing;
	byte iconPosVal = m_rmdMsgStyleSet.getIconPosition();
	if (iconPosVal == MsgIconPosition::IconOnTheTop) {
		iconPosition._y = currentMargin.top() - (m_szIconSize.height() + textIconSpacing);
		iconPosition._x = ((rcClient.right - rcClient.left) - m_szIconSize.width()) / 2;
	}
	else if (iconPosVal == MsgIconPosition::IconOnTheLeft) {
		iconPosition._x = currentMargin.left() - (m_szIconSize.width() + textIconSpacing);
		iconPosition._y = ((rcClient.bottom - rcClient.top) - m_szIconSize.height()) / 2;
	}

	return true;
}

/**
 * @brief	Move the dialog to specific display position
 * @param	displayPosition - Display position
 * @return	void
 */
void CReminderMsgDlg::MoveToDisplayPosition(MsgDispPosition displayPosition)
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
		topLeft._x = screenCornerHMargin;
		topLeft._y = screenCornerVMargin;
		break;

	case MsgDispPosition::OnTopRight:
		topLeft._y = screenCornerVMargin;
		topLeft._x = screenSize.width() - dialogSize.width() - screenCornerHMargin;
		break;

	case MsgDispPosition::OnBottomLeft:
		topLeft._x = screenCornerHMargin;
		topLeft._y = screenSize.height() - dialogSize.height() - screenCornerVMargin;
		break;

	case MsgDispPosition::OnBottomRight:
		topLeft._x = screenSize.width() - dialogSize.width() - screenCornerHMargin;
		topLeft._y = screenSize.height() - dialogSize.height() - screenCornerVMargin;
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
void CReminderMsgDlg::ClientToText(Rect& /*rect*/) const
{
}

/**
 * @brief	Calculate dialog client rect from text size
 * @param	lpRect - Returned rect
 * @return	void
 */
void CReminderMsgDlg::TextToClient(Rect& rect) const
{
	// If message text is not set, do nothing
	if (m_strBuffer.isEmpty())
		return;

	// Get font
	CFont* pMsgFont = this->GetFont();
	if (m_pMsgFont != NULL) {
		pMsgFont = m_pMsgFont;
	}

	// Invalid font
	if (pMsgFont == NULL) {
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
	CFont* pDefFont = NULL;
	pDefFont = pDC->SelectObject(pMsgFont);

	// Backup rectangle
	RECT rcTemp;
	rcTemp.left = rect.left();
	rcTemp.top = rect.top();
	rcTemp.right = rect.right();
	rcTemp.bottom = rect.bottom();

	// Calculate new client rectangle
	DWORD dwFormat = DT_CENTER | DT_WORDBREAK | DT_CALCRECT;
	pDC->DrawText(m_strBuffer, m_strBuffer.getLength(), &rcTemp, dwFormat);
	rect = Rect(rcTemp.left, rcTemp.top, rcTemp.right, rcTemp.bottom);

	// Reset device context
	pDC->SelectObject(pDefFont);
	windowPtr->ReleaseDC(pDC);
}
