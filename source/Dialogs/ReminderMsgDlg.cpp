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


// Implement methods for CReminderMsgDlg
IMPLEMENT_DYNAMIC(CReminderMsgDlg, SDialog)


/**
 * @brief	Constructor
 */
CReminderMsgDlg::CReminderMsgDlg(CWnd* pParentWnd /*= NULL*/)
	: SDialog(IDD_REMINDERMSG_DLG, pParentWnd)
{
	// Message string buffer
	m_strBuffer = Constant::String::Empty;

	// Message font & icon
	m_pMsgFont = NULL;
	m_hMsgIcon = NULL;
	m_szIconSize = SIZE_NULL;

	// Flags
	m_bTimerSet = FALSE;
	m_bDispIcon = FALSE;
	m_bLockDlgSize = FALSE;
	m_bLockFontSize = FALSE;
	m_bAllowSnooze = FALSE;
	m_nSnoozeFlag = FLAG_OFF;

	// Properties
	m_clBkgrdColor = Color::White;
	m_clTextColor = Color::Black;
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


//////////////////////////////////////////////////////////////////////////
//
//	CReminderMsgDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

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
		return FALSE;

	// Set margin
	int nHMargin = m_rmdMsgStyleSet.GetHorizontalMargin();
	int nVMargin = m_rmdMsgStyleSet.GetVerticalMargin();
	SetCenterMargin(nHMargin, nVMargin);

	// Shift margin if icon is displaying
	if ((m_bDispIcon == TRUE) && (m_hMsgIcon != NULL)) {

		// Get current margin
		CRect rcDialogMargin;
		this->GetMargin(&rcDialogMargin);

		BYTE byIconPosition = m_rmdMsgStyleSet.GetIconPosition();
		if (byIconPosition == RmdMsgStyleSet::IconPosition::IconOnTheTop) {

			// Shift top margin
			rcDialogMargin.top += m_szIconSize.cy;
			rcDialogMargin.top -= defaultTextIconSpacing;
			this->SetTopMargin(rcDialogMargin.top);
		}
		else if (byIconPosition == RmdMsgStyleSet::IconPosition::IconOnTheLeft) {

			// Shift left margin
			rcDialogMargin.left += m_szIconSize.cx;
			rcDialogMargin.left -= defaultTextIconSpacing;
			this->SetLeftMargin(rcDialogMargin.left);
		}
	}

	// Get display area
	CRect rcDispArea;
	GetDisplayArea(&rcDispArea);

	// Get dialog size
	CSize szDialogSize;
	this->GetSize(&szDialogSize);

	// If set lock font size
	if (m_bLockFontSize == TRUE) {
		// Calculate text rectangle
		TextToClient(&rcDispArea);

		// Set display area and resize dialog
		SetDisplayArea(rcDispArea, TRUE, TRUE);
	}

	// Display message content
	CWnd* pWndMsg = this->GetDlgItem(IDC_REMINDERMSG_MSGTEXT_STATIC);
	if (pWndMsg != NULL) {
		// Fix text display area size and position
		pWndMsg->MoveWindow(&rcDispArea);
		// Set text font
		if (m_pMsgFont != NULL) {
			pWndMsg->SetFont(m_pMsgFont);
		}
		// Display text
		pWndMsg->SetWindowText(m_strBuffer);
	}

	// Start auto-close timer if set
	if (GetAutoCloseInterval() != 0) {
		UINT nRet = SetTimer(TIMERID_RMDMSG_AUTOCLOSE, 1000, NULL);
		m_bTimerSet = (nRet != 0);
	}

	return TRUE;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void CReminderMsgDlg::OnClose()
{
	// Close dialog
	SDialog::OnClose();
}

/**
 * @brief	Destroy dialog
 * @param	None
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
	// Kill timer if set
	if (m_bTimerSet == TRUE) {
		BOOL bRet = KillTimer(TIMERID_RMDMSG_AUTOCLOSE);
		if (bRet == TRUE) {
			// Reset flag
			m_bTimerSet = FALSE;
		}
	}

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
	if ((m_bDispIcon == TRUE) && (m_hMsgIcon != NULL)) {
		// Calculate icon postion
		CPoint ptIconPos;
		CalcMsgIconPosition(&ptIconPos);
		// Get icon size
		int cx = m_szIconSize.cx;
		int cy = m_szIconSize.cy;
		// Draw icon (with scale up/down);
		DrawIconEx(dc, ptIconPos.x, ptIconPos.y, m_hMsgIcon, cx, cy, NULL, NULL, DI_NORMAL);
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
		if (m_bTimerSet == TRUE) {
			UINT nCounter = GetAutoCloseInterval();
			if (nCounter > 0) {
				// Countdown
				SetAutoCloseInterval(--nCounter);
			}
			if (nCounter == 0) {
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
	if (GetAllowSnoozeMode() == TRUE) {
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
void CReminderMsgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == SC_MINIMIZE) {
		// Handle minimize button event
		if (GetAllowSnoozeMode() == TRUE) {
			// Trigger snooze mode
			SetSnoozeTriggerFLag(FLAG_ON);
		}
		EndDialog(IDOK);
	}
	else {
		// Execute default syscommand
		SDialog::OnSysCommand(nID, lParam);
	}
}

/**
 * @brief	WM_CTLCOLOR handling function
 * @param	Default
 * @return	HBRUSH
 */
HBRUSH CReminderMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Default initialization
	HBRUSH hBrush = SDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd == this) {
		if ((GetFlagValue(AppFlagID::dialogSetBackgroundColor) == TRUE) && (m_pBkgrdBrush != NULL)) {
			// Get brush
			hBrush = (HBRUSH)(*m_pBkgrdBrush);
		}
	}

	if (nCtlColor == CTLCOLOR_STATIC) {
		// Set background color
		if ((GetFlagValue(AppFlagID::dialogSetBackgroundColor) == TRUE) && (m_pBkgrdBrush != NULL)) {
			hBrush = (HBRUSH)(*m_pBkgrdBrush);
			pDC->SetBkColor(m_clBkgrdColor);
		}
		// Set text color
		if (GetFlagValue(AppFlagID::dialogSetTextColor) == TRUE) {
			pDC->SetTextColor(m_clTextColor);
		}
	}

	return hBrush;
}

/**
 * @brief	Initialize message style info data
 * @param	None
 * @return	TRUE/FALSE
 */
BOOL CReminderMsgDlg::InitMessageStyle(void)
{
	// Background & text color
	SetBkgrdColor(m_rmdMsgStyleSet.GetBkgrdColor());
	SetTextColor(m_rmdMsgStyleSet.GetTextColor());
	if (!CreateBrush()) {

		// Error: Apply color failed
		TRACE_ERROR("Error: Apply color failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Trigger force redrawing
	this->Invalidate();

	// Reset font
	if (m_pMsgFont != NULL)
		delete m_pMsgFont;

	// Reset flag
	m_bLockFontSize = FALSE;

	// Initialize message font
	m_pMsgFont = new CFont;
	ASSERT(m_pMsgFont != NULL);
	if (m_pMsgFont != NULL) {

		// Create font
		int nFontPointSize = int(m_rmdMsgStyleSet.GetFontSize() * 10);
		String fontName = m_rmdMsgStyleSet.GetFontName();
		if (!m_pMsgFont->CreatePointFont(nFontPointSize, fontName, NULL)) {

			// Error: Create font failed
			TRACE_ERROR("Error: Create font failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return FALSE;
		}
	}

	// Set lock font size flag
	m_bLockFontSize = TRUE;

	// Icon size
	m_szIconSize.cx = m_rmdMsgStyleSet.GetIconSize();
	m_szIconSize.cy = m_rmdMsgStyleSet.GetIconSize();

	int cx = m_szIconSize.cx;
	int cy = m_szIconSize.cy;

	// Invalid input size, reset to system default
	if ((cx < 30) || (cx > 100)) {
		cx = GetSystemMetrics(SM_CXICON);
		cy = GetSystemMetrics(SM_CYICON);
	}

	// If icon ID is not set
	int nIconID = m_rmdMsgStyleSet.GetIconID();
	if (nIconID == NULL) {
		m_bDispIcon = FALSE;
		return TRUE;
	}

	// Load system icon by ID and scale size
	HRESULT hResult = ::LoadIconWithScaleDown(NULL, MAKEINTRESOURCE(nIconID), cx, cy, &m_hMsgIcon);

	// Load icon failed
	if ((hResult != S_OK) || (m_hMsgIcon == NULL)) {
		TRACE_ERROR("Error: Load icon failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Get icon info
	ICONINFO iiIconInfo;
	BOOL bRet = ::GetIconInfo(m_hMsgIcon, &iiIconInfo);
	if (bRet == FALSE) {
		TRACE_ERROR("Error: Get icon info failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Update icon size
	BITMAP bmp;
	ZeroMemory(&bmp, sizeof(BITMAP));
	if (iiIconInfo.hbmColor) {
		if (::GetObject(iiIconInfo.hbmColor, sizeof(bmp), &bmp) > 0) {
			m_szIconSize.cx = bmp.bmWidth;
			m_szIconSize.cy = bmp.bmHeight;
		}
	}
	else if (iiIconInfo.hbmMask) {
		if (::GetObject(iiIconInfo.hbmMask, sizeof(bmp), &bmp) > 0) {
			m_szIconSize.cx = bmp.bmWidth;
			m_szIconSize.cy = bmp.bmHeight;
		}
	}

	// Delete icon info temp objects
	if (iiIconInfo.hbmColor)	DeleteObject(iiIconInfo.hbmColor);
	if (iiIconInfo.hbmMask)		DeleteObject(iiIconInfo.hbmMask);

	// Update flag
	m_bDispIcon = TRUE;

	return TRUE;
}

/**
 * @brief	Calculate message icon position (top-left point)
 * @param	lpptIcon - Icon position (point)
 * @return	TRUE/FALSE
 */
BOOL CReminderMsgDlg::CalcMsgIconPosition(LPPOINT lpptIcon) const
{
	// Check pointer validity
	if (lpptIcon == NULL) return FALSE;

	// Get display margin
	CRect rcCurMargin;
	this->GetMargin(&rcCurMargin);

	// Get client rectangle
	CRect rcClient;
	this->GetClientRect(&rcClient);

	// Calculate icon top-left point
	int nText2IconDist = defaultTextIconSpacing;
	BYTE byIconPosition = m_rmdMsgStyleSet.GetIconPosition();
	if (byIconPosition == RmdMsgStyleSet::IconPosition::IconOnTheTop) {
		lpptIcon->y = rcCurMargin.top - (m_szIconSize.cy + nText2IconDist);
		lpptIcon->x = (rcClient.Width() - m_szIconSize.cx) / 2;
	}
	else if (byIconPosition == RmdMsgStyleSet::IconPosition::IconOnTheLeft) {
		lpptIcon->x = rcCurMargin.left - (m_szIconSize.cx + nText2IconDist);
		lpptIcon->y = (rcClient.Height() - m_szIconSize.cy) / 2;
	}

	return TRUE;
}

/**
 * @brief	Calculate text rect from dialog client size
 * @param	lpRect - Returned rect
 * @return	void
 */
void CReminderMsgDlg::ClientToText(LPRECT /*lpRect*/) const
{
}

/**
 * @brief	Calculate dialog client rect from text size
 * @param	lpRect - Returned rect
 * @return	void
 */
void CReminderMsgDlg::TextToClient(LPRECT lpRect) const
{
	// If message text is not set, do nothing
	if (m_strBuffer.IsEmpty())
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
	CWnd* pWnd = this->GetDlgItem(IDC_REMINDERMSG_MSGTEXT_STATIC);
	if (pWnd == NULL) {
		TRACE_ERROR("Error: Message text draw area not found!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Set font
	CDC* pDC = pWnd->GetDC();
	CFont* pDefFont = NULL;
	pDefFont = pDC->SelectObject(pMsgFont);

	// Calculate rectangle
	DWORD dwFormat = DT_CENTER | DT_WORDBREAK | DT_CALCRECT;
	pDC->DrawText(m_strBuffer, m_strBuffer.GetLength(), lpRect, dwFormat);

	// Reset device context
	pDC->SelectObject(pDefFont);
	pWnd->ReleaseDC(pDC);
}
