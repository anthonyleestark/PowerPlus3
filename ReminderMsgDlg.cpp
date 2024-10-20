
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		ReminderMsgDlg.cpp
//		Description:	Source file for Reminder Message dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2024.08.12:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReminderMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_TEXT2ICON_DISTANCE	10

using namespace CoreFuncs;


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CReminderMsgDlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CReminderMsgDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CReminderMsgDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CReminderMsgDlg::CReminderMsgDlg(CWnd* pParentWnd /*= NULL*/)
	: SDialog(IDD_REMINDERMSG_DLG, pParentWnd)
{
	// Message string buffer
	m_strBuffer = DEF_STRING_EMPTY;

	// Message font
	m_pMsgFont = NULL;
	m_strMsgFontName = DEF_STRING_EMPTY;
	m_fMsgFontPoint = (float)0.0F;

	// Message icon
	m_hMsgIcon = NULL;
	m_bDispIcon = FALSE;
	m_byIconPosition = DEFAULT_MSGICONPOS;
	m_szIconSize.cx = DEFAULT_MSGICONSIZE;
	m_szIconSize.cy = DEFAULT_MSGICONSIZE;

	// Flags
	m_bTimerSet = FALSE;
	m_bLockDlgSize = FALSE;
	m_bLockFontSize = FALSE;
	m_bAllowSnooze = FALSE;
	m_nSnoozeFlag = FLAG_OFF;

	// Properties
	m_clBkgrdColor = DEF_COLOR_WHITE;
	m_clTextColor = DEF_COLOR_BLACK;
	m_nAutoCloseInterval = 0;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CReminderMsgDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize DebugTest dialog
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CReminderMsgDlg::OnInitDialog()
{
	SDialog::OnInitDialog();

	// Set margin
	int nHMargin = GetReminderMsgHMargin();
	int nVMargin = GetReminderMsgVMargin();
	SetCenterMargin(nHMargin, nVMargin);

	// Shift margin if icon is displaying
	if ((m_bDispIcon == TRUE) && (m_hMsgIcon != NULL)) {
		// Get current margin
		POINT ptTopLeft, ptBottomRight;
		this->GetMargin(&ptTopLeft, &ptBottomRight);
		if (m_byIconPosition == MSGICONPOS_ONTOP) {
			// Shift top margin
			ptTopLeft.y += m_szIconSize.cy;
			ptTopLeft.y -= DEFAULT_TEXT2ICON_DISTANCE;
			this->SetTopMargin(ptTopLeft.y);
		}
		else if (m_byIconPosition == MSGICONPOS_ONLEFT) {
			// Shift left margin
			ptTopLeft.x += m_szIconSize.cx;
			ptTopLeft.x -= DEFAULT_TEXT2ICON_DISTANCE;
			this->SetLeftMargin(ptTopLeft.x);
		}
	}

	// Get display area
	CRect rcDispArea;
	GetDisplayArea(&rcDispArea);

	// Get dialog size
	CSize szDlgSize;
	GetDlgSize(&szDlgSize);

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClose
//	Description:	Default method for dialog closing
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::OnClose()
{
	SDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDestroy
//	Description:	Destroy dialog
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::OnDestroy()
{
	SDialog::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PostNcDestroy
//	Description:	PostNcDestroy function
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnPaint
//	Description:	OnPaint function
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnTimer
//	Description:	OnTimer function
//  Arguments:		nIDEvent - Time event ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

	SDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCreate
//	Description:	OnCreate function
//  Arguments:		Default
//  Return value:	int - Default
//
//////////////////////////////////////////////////////////////////////////

int CReminderMsgDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Default creation
	if (SDialog::OnCreate(lpCreateStruct) == DEF_INTEGER_INVALID)
		return DEF_INTEGER_INVALID;

	// Set allow snoozing mode
	if (GetAllowSnoozeMode() == TRUE) {
		// Display minimize button
		LONG lCurStyle = GetWindowLong(this->m_hWnd, GWL_STYLE);
		SetWindowLong(this->m_hWnd, GWL_STYLE, lCurStyle | WS_MINIMIZEBOX);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSysCommand
//	Description:	OnSysCommand function
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCtlColor
//	Description:	WM_CTLCOLOR handling function
//  Arguments:		Default
//  Return value:	HBRUSH
//
//////////////////////////////////////////////////////////////////////////

HBRUSH CReminderMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Default initialization
	HBRUSH hBrush = SDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd == this) {
		if ((m_bSetBkgrdColor == TRUE) && (m_pBkgrdBrush != NULL)) {
			// Get brush
			hBrush = (HBRUSH)(*m_pBkgrdBrush);
		}
	}

	if (nCtlColor == CTLCOLOR_STATIC) {
		// Set background color
		if ((m_bSetBkgrdColor == TRUE) && (m_pBkgrdBrush != NULL)) {
			hBrush = (HBRUSH)(*m_pBkgrdBrush);
			pDC->SetBkColor(m_clBkgrdColor);
		}
		// Set text color
		if (m_bSetTextColor == TRUE) {
			pDC->SetTextColor(m_clTextColor);
		}
	}

	return hBrush;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetDispMessage
//	Description:	Get/set display message
//  Arguments:		lpszDispMsg - Display message
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR CReminderMsgDlg::GetDispMessage()
{
	return m_strBuffer.GetString();
}

void CReminderMsgDlg::SetDispMessage(LPCTSTR lpszDispMsg)
{
	m_strBuffer = lpszDispMsg;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetAutoCloseInterval
//	Description:	Get/set auto closing message interval
//  Arguments:		nSeconds - Number of seconds
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT CReminderMsgDlg::GetAutoCloseInterval()
{
	return m_nAutoCloseInterval;
}

void CReminderMsgDlg::SetAutoCloseInterval(UINT nSeconds)
{
	m_nAutoCloseInterval = nSeconds;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDlgSize
//	Description:	Set dialog size
//  Arguments:		szDlgSize - Dialog size
//					nWidth	  - Dialog width
//					nHeight   - Dialog height
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::SetDlgSize(CSize szDlgSize)
{
	// Set dialog size
	SDialog::SetDlgSize(szDlgSize);

	// Set lock size flag
	m_bLockDlgSize = TRUE;
}

void CReminderMsgDlg::SetDlgSize(int nWidth, int nHeight)
{
	// Set dialog size
	SDialog::SetDlgSize(nWidth, nHeight);

	// Set lock size flag
	m_bLockDlgSize = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMsgFontName
//	Description:	Get message font name
//  Arguments:		strFontName - Font name
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::GetMsgFontName(CString& strFontName)
{
	// Do not return if font is invalid
	if (m_pMsgFont == NULL)
		return;

	// Return font name
	strFontName = m_strMsgFontName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMsgFontPoint
//	Description:	Get message font point
//  Arguments:		fFontPoint - Font point
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::GetMsgFontPoint(float& fFontPoint)
{
	// Do not return if font is invalid
	if (m_pMsgFont == NULL)
		return;

	// Return font point
	fFontPoint = m_fMsgFontPoint;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMsgFont
//	Description:	Set message font
//  Arguments:		lpszFontName - Font name
//					fFontPoint	 - Dialog font
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::SetMsgFont(LPCTSTR lpszFontName, float fFontPoint)
{
	// Reset font
	if (m_pMsgFont != NULL) {
		delete m_pMsgFont;
	}

	// Reset flag
	m_bLockFontSize = FALSE;

	// Initialize
	m_pMsgFont = new CFont;
	ASSERT(m_pMsgFont != NULL);
	if (m_pMsgFont != NULL) {
		// Create font
		int nPointSize = int(fFontPoint * 10);
		BOOL bRet = m_pMsgFont->CreatePointFont(nPointSize, lpszFontName, NULL);

		// If point created successfully, set name and point
		if (bRet == TRUE) {
			m_strMsgFontName = lpszFontName;
			m_fMsgFontPoint = fFontPoint;
		}
	}

	// Set lock font size flag
	m_bLockFontSize = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMsgIcon
//	Description:	Set message icon
//  Arguments:		nIconID		 - Icon ID
//					nIconSqrSize - Icon size (square)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::SetMsgIcon(UINT nIconID, int nIconSqrSize)
{
	// Icon size
	int cx = nIconSqrSize;
	int cy = nIconSqrSize;

	m_szIconSize.cx = cx;
	m_szIconSize.cy = cy;

	// Invalid input size, reset to system default
	if ((cx < 30) || (cx > 100)) {
		cx = GetSystemMetrics(SM_CXICON);
		cy = GetSystemMetrics(SM_CYICON);
	}

	// Init result
	HRESULT hResult = E_FAIL;

	// Get icon info
	if (nIconID != 0) {
		// Load system icon by ID and scale size
		hResult = ::LoadIconWithScaleDown(NULL, MAKEINTRESOURCE(nIconID), cx, cy, &m_hMsgIcon);
	}

	// Load icon failed
	if ((hResult != S_OK) || (m_hMsgIcon == NULL)) {
		TRCLOG("Error: Load icon failed!");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return;
	}

	// Get icon info
	ICONINFO iiIconInfo;
	BOOL bRet = ::GetIconInfo(m_hMsgIcon, &iiIconInfo);
	if (bRet == FALSE) {
		TRCLOG("Error: Get icon info failed!");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return;
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
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMsgIconPosition
//	Description:	Set message icon position
//  Arguments:		byPosition - Icon position (option)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::SetMsgIconPosition(BYTE byPosition)
{
	m_byIconPosition = byPosition;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAllowSnoozeMode
//	Description:	Get allow snoozing mode enable status
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CReminderMsgDlg::GetAllowSnoozeMode()
{
	return m_bAllowSnooze;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAllowSnoozeMode
//	Description:	Set allow snoozing mode enable status
//  Arguments:		bValue - Status value (TRUE/FALSE -> ON/OFF)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::SetAllowSnoozeMode(BOOL bValue)
{
	m_bAllowSnooze = bValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSnoozeTriggerFlag
//	Description:	Get snooze mode trigger flag value
//  Arguments:		nValue - Return value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::GetSnoozeTriggerFlag(int& nValue)
{
	nValue = m_nSnoozeFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSnoozeTriggerFlag
//	Description:	Set snooze mode trigger flag value
//  Arguments:		nValue - Flag value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::SetSnoozeTriggerFLag(int nValue)
{
	m_nSnoozeFlag = nValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CalcMsgIconPosition
//	Description:	Calculate message icon position (top-left point)
//  Arguments:		lpptIcon - Icon position (point)
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CReminderMsgDlg::CalcMsgIconPosition(LPPOINT lpptIcon)
{
	// Check pointer validity
	if (lpptIcon == NULL) return FALSE;

	// Get display margin
	POINT ptTopLeft, ptBottomRight;
	this->GetMargin(&ptTopLeft, &ptBottomRight);

	// Get client rectangle
	CRect rcClient;
	this->GetClientRect(&rcClient);

	// Calculate icon top-left point
	int nText2IconDist = DEFAULT_TEXT2ICON_DISTANCE;
	if (m_byIconPosition == MSGICONPOS_ONTOP) {
		lpptIcon->y = ptTopLeft.y - (m_szIconSize.cy + nText2IconDist);
		lpptIcon->x = (rcClient.Width() - m_szIconSize.cx) / 2;
	}
	else if (m_byIconPosition == MSGICONPOS_ONLEFT) {
		lpptIcon->x = ptTopLeft.x - (m_szIconSize.cx + nText2IconDist);
		lpptIcon->y = (rcClient.Height() - m_szIconSize.cy) / 2;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ClientToText
//	Description:	Calculate text rect from dialog client size
//  Arguments:		lpRect - Returned rect
//  Return value:	void
//
//////////////////////////////////////////////////////////////////////////

void CReminderMsgDlg::ClientToText(LPRECT lpRect) const
{
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TextToClient
//	Description:	Calculate dialog client rect from text size
//  Arguments:		lpRect - Returned rect
//  Return value:	void
//
//////////////////////////////////////////////////////////////////////////

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
		TRCLOG("Error: Invalid font");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return;
	}

	// Set draw area
	CWnd* pWnd = this->GetDlgItem(IDC_REMINDERMSG_MSGTEXT_STATIC);
	if (pWnd == NULL) {
		TRCLOG("Error: Message text draw area not found!");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return;
	}

	// Set font
	CDC* pDC = pWnd->GetDC();
	CFont* pDefFont = NULL;
	pDefFont = pDC->SelectObject(pMsgFont);

	// Calculate rectangle
	DWORD dwFormat = DT_CENTER | DT_WORDBREAK | DT_CALCRECT;
	pDC->DrawText(m_strBuffer, lpRect, dwFormat);

	// Reset device context
	pDC->SelectObject(pDefFont);
	pWnd->ReleaseDC(pDC);
}
