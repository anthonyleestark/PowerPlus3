/**
 * @file		MsgStyleSetDlg.cpp
 * @brief		Source file for Reminder message style set dialog
 * @author		AnthonyLeeStark
 * @date		2025.06.17
 *
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "MainApp/PowerPlus.h"
#include "Dialogs/MsgStyleSetDlg.h"
#include "Dialogs/ReminderMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace MapTable;
using namespace AppCore;


// Dialog default size
constexpr const int defaultWidth = 320;
constexpr const int defaultHeight = 240;

// Maximum input text length
constexpr const unsigned maxFontSizeDigits = 2;
constexpr const unsigned maxIconSizeDigits = 3;
constexpr const unsigned maxMarginValDigits = 3;


// Implement methods for CRmdMsgStyleSetDlg
IMPLEMENT_DYNAMIC(CRmdMsgStyleSetDlg, SDialog)


/**
 * @brief	Constructor
 */
CRmdMsgStyleSetDlg::CRmdMsgStyleSetDlg() : SDialog(IDD_MSGSTYLESET_DLG)
{
	// Background/Text color settings
	m_pBkgrdColorPickBtn = NULL;
	m_pTextColorPickBtn = NULL;

	// Font settings
	m_pFontNamePickCombo = NULL;
	m_pFontSizePickCombo = NULL;

	// Icon settings
	m_pIconIDPickCombo = NULL;
	m_pIconSizeEdit = NULL;
	m_pIconPositionOnTopRad = NULL;
	m_pIconPositionOnLeftRad = NULL;
	m_pIconPreviewStatic = NULL;

	// Timeout settings
	m_pTimeoutEdit = NULL;
	m_pTimeoutSpin = NULL;

	// Display position setting
	m_pDisplayPosCombo = NULL;

	// Margin settings
	m_pHorizontalMarginEdit = NULL;
	m_pVerticalMarginEdit = NULL;

	// Data
	m_rmsMsgStyleData = RmdMsgStyleSet();
	m_rmsMsgStyleTemp = RmdMsgStyleSet();
	m_clrMsgBackground = RmdMsgStyleSet::defaultBkgrdColor;
	m_clrMsgText = RmdMsgStyleSet::defaultTextColor;
}

/**
 * @brief	Destructor
 */
CRmdMsgStyleSetDlg::~CRmdMsgStyleSetDlg()
{
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CRmdMsgStyleSetDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


// CRmdMsgStyleSetDlg dialog message map
BEGIN_MESSAGE_MAP(CRmdMsgStyleSetDlg, SDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MSGSTYLESET_APPLY_BTN,	&CRmdMsgStyleSetDlg::OnApply)
	ON_BN_CLICKED(IDC_MSGSTYLESET_CANCEL_BTN,	&CRmdMsgStyleSetDlg::OnCancel)
	ON_BN_CLICKED(IDC_MSGSTYLESET_PREVIEW_BTN,	&CRmdMsgStyleSetDlg::OnPreview)
	ON_BN_CLICKED(IDC_MSGSTYLESET_RESET_BTN,	&CRmdMsgStyleSetDlg::OnReset)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// CRmdMsgStyleSetDlg message handlers

/**
 * @brief	Initialize Reminder message style set dialog
 * @param	None
 * @return	BOOL - Default
 */
BOOL CRmdMsgStyleSetDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Setup display
	SetupLanguage();
	SetupDialogItemState();
	UpdateDialogData(false);

	return true;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::OnClose()
{
	// Hide the dialog itself
	this->ShowWindow(SW_HIDE);
}

/**
 * @brief	Handle click event for [Apply] button
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::OnApply()
{
	// Save data
	UpdateDialogData(true);
	m_rmsMsgStyleData.Copy(m_rmsMsgStyleTemp);

	// Hide the dialog itself
	this->ShowWindow(SW_HIDE);
}

/**
 * @brief	Handle click event for [Cancel] button
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::OnCancel()
{
	// Hide the dialog itself
	this->ShowWindow(SW_HIDE);
}

/**
 * @brief	Handle click event for [Preview] button
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::OnPreview()
{
	// Update data
	UpdateDialogData(true);

	// Default timeout for previewing
	int nDefTimeout = PwrReminderData::previewTimeout;

	// Initialize preview message
	CReminderMsgDlg m_previewMsgDlg;
	m_previewMsgDlg.SetCaptionFromLanguage(IDC_MSGSTYLESET_PREVIEW_BTN);
	m_previewMsgDlg.SetDispMessage(L"Preview message style");
	m_previewMsgDlg.SetMessageStyle(m_rmsMsgStyleTemp);
	m_previewMsgDlg.SetAutoCloseInterval(nDefTimeout);

	// Set notify state flags
	m_previewMsgDlg.SetTopMost(false);
	m_previewMsgDlg.SetInitSound(true);

	// Display message
	m_previewMsgDlg.DoModal();
}

/**
 * @brief	Handle click event for [Reset] button
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::OnReset()
{
	// Reset data
	m_rmsMsgStyleTemp = RmdMsgStyleSet();

	// Apply state
	UpdateDialogData(false);
}

/**
 * @brief	Handle app command messages (WM_COMMAND)
 * @param	wParam - First param (HIWORD)
 * @param	lParam - Second param (LOWORD)
 * @return	BOOL
 */
BOOL CRmdMsgStyleSetDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Process commands
	switch (LOWORD(wParam))
	{
		case IDC_MSGSTYLESET_BKGRDCLR_PICKER:
		case IDC_MSGSTYLESET_TEXTCLR_PICKER:
		{
			CHOOSECOLOR cc = { sizeof(cc) };
			static COLORREF acrCustClr[16] = { 0 };
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			cc.lpCustColors = acrCustClr;

			if (LOWORD(wParam) == IDC_MSGSTYLESET_BKGRDCLR_PICKER) {
				cc.rgbResult = m_clrMsgBackground;
			}
			else {
				cc.rgbResult = m_clrMsgText;
			}

			cc.hwndOwner = this->GetSafeHwnd();
			if (ChooseColor(&cc)) {
				if (LOWORD(wParam) == IDC_MSGSTYLESET_BKGRDCLR_PICKER) {
					m_clrMsgBackground = cc.rgbResult;
					if (m_pBkgrdColorPickBtn != NULL)
						m_pBkgrdColorPickBtn->RedrawWindow();
				}
				else {
					m_clrMsgText = cc.rgbResult;
					if (m_pTextColorPickBtn != NULL)
						m_pTextColorPickBtn->RedrawWindow();
				}
			}
			return true;
		}
		case IDC_MSGSTYLESET_ICONID_COMBO:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE && m_pIconPreviewStatic != NULL) {
				::RedrawWindow(m_pIconPreviewStatic->GetSafeHwnd(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				return true;
			}

			break;
		}
		case IDC_MSGSTYLESET_ICONSIZE_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE && m_pIconSizeEdit != NULL)
			{
				wchar_t tempBuff[maxIconSizeDigits + 1];
				m_pIconSizeEdit->GetWindowText(tempBuff, sizeof(tempBuff));
				int nIconSize = _wtoi(tempBuff);
				if (m_pIconPreviewStatic != NULL &&
					(nIconSize >= RmdMsgStyleSet::minIconSize && nIconSize <= RmdMsgStyleSet::maxIconSize))
					::RedrawWindow(m_pIconPreviewStatic->GetSafeHwnd(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				return true;
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS && m_pIconSizeEdit != NULL)
			{
				wchar_t tempBuff[maxIconSizeDigits + 1];
				m_pIconSizeEdit->GetWindowText(tempBuff, sizeof(tempBuff));
				int nIconSize = _wtoi(tempBuff);
				if (nIconSize < RmdMsgStyleSet::minIconSize) {
					nIconSize = RmdMsgStyleSet::minIconSize;
					m_pIconSizeEdit->SetWindowText(String::FromNumber(nIconSize));
				}
				else if (nIconSize > RmdMsgStyleSet::maxIconSize) {
					nIconSize = RmdMsgStyleSet::maxIconSize;
					m_pIconSizeEdit->SetWindowText(String::FromNumber(nIconSize));
				}

				if (m_pIconPreviewStatic != NULL)
					::RedrawWindow(m_pIconPreviewStatic->GetSafeHwnd(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				return true;
			}

			break;
		}
	}

	// Default
	return SDialog::OnCommand(wParam, lParam);
}

/**
 * @brief	Handle dialog window messages
 * @param	message - Message ID
 * @param	wParam - First param (HIWORD)
 * @param	lParam - Second param (LOWORD)
 * @return	LRESULT
 */
LRESULT CRmdMsgStyleSetDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT pDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
			HBRUSH hFillBrush;

			switch (pDrawItemStruct->CtlID) {
			case IDC_MSGSTYLESET_BKGRDCLR_PICKER:
				hFillBrush = CreateSolidBrush(m_clrMsgBackground);
				break;
			case IDC_MSGSTYLESET_TEXTCLR_PICKER:
				hFillBrush = CreateSolidBrush(m_clrMsgText);
				break;
			default:
				return false;
			}

			FillRect(pDrawItemStruct->hDC, &pDrawItemStruct->rcItem, hFillBrush);
			FrameRect(pDrawItemStruct->hDC, &pDrawItemStruct->rcItem, (HBRUSH)GetStockObject(BLACK_BRUSH));
			DeleteObject(hFillBrush);

			return true;
		}
		case WM_PAINT:
		{
			if (m_pIconPreviewStatic != NULL)
			{
				OutputDebugString(_T("[ALS] => Redrawing Icon Preview..."));

				// Handle painting the icon
				CPaintDC dc(m_pIconPreviewStatic);

				// Icon ID
				int nIconID = RmdMsgStyleSet::defaultIconID;
				if (m_pIconIDPickCombo != NULL) {
					int nCurSel = m_pIconIDPickCombo->GetCurSel();
					nIconID = IDTable::SystemIcon[nCurSel].first;
				}

				// Icon size
				int nIconSize = RmdMsgStyleSet::defaultIconSize;
				if (m_pIconSizeEdit != NULL) {
					wchar_t tempBuff[maxIconSizeDigits + 1];
					m_pIconSizeEdit->GetWindowText(tempBuff, sizeof(tempBuff));
					nIconSize = _wtoi(tempBuff);
					if (nIconSize < RmdMsgStyleSet::minIconSize)
						nIconSize = RmdMsgStyleSet::minIconSize;
					else if (nIconSize > RmdMsgStyleSet::maxIconSize)
						nIconSize = RmdMsgStyleSet::maxIconSize;
				}
				int cx = nIconSize;
				int cy = nIconSize;

				// Get rect
				RECT rcTemp;
				m_pIconPreviewStatic->GetClientRect(&rcTemp);

				// Draw area rect
				Rect drawAreaRect;
				drawAreaRect._top = rcTemp.top;
				drawAreaRect._left = rcTemp.left;
				drawAreaRect._bottom = rcTemp.bottom;
				drawAreaRect._right = rcTemp.right;

				// Icon position
				Point iconDrawPosition = drawAreaRect.TopLeft();
				Vector2D drawAreaCenter = drawAreaRect.Center();
				iconDrawPosition._x = drawAreaCenter.GetX() - cx / 2;
				iconDrawPosition._y = drawAreaCenter.GetY() - cy / 2;

				// Load system icon by ID and scale size
				HICON hIcon = NULL;
				HRESULT hResult = ::LoadIconWithScaleDown(NULL, MAKEINTRESOURCE(nIconID), cx, cy, &hIcon);

				// Load icon failed
				if ((hResult != S_OK) || (hIcon == NULL)) {
					TRACE_ERROR("Error: Load icon failed!!!");
					TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
				}
				else {
					// Draw icon (with scale up/down);
					DrawIconEx(dc, iconDrawPosition._x, iconDrawPosition._y, hIcon, cx, cy, NULL, NULL, DI_NORMAL);
				}
			}
			break;
		}
	}

	// Default
	return SDialog::WindowProc(message, wParam, lParam);
}

/**
 * @brief	Setup language for dialog controls
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::SetupLanguage()
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		unsigned nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDC_MSGSTYLESET_BKGRDCLR_PICKER:
		case IDC_MSGSTYLESET_TEXTCLR_PICKER:
		case IDC_MSGSTYLESET_ICONSIZE_EDIT:
		case IDC_MSGSTYLESET_MSGICON_VIEW:
		case IDC_MSGSTYLESET_TIMEOUT_EDIT:
		case IDC_MSGSTYLESET_TIMEOUT_SPIN:
		case IDC_MSGSTYLESET_HMARGIN_EDIT:
		case IDC_MSGSTYLESET_VMARGIN_EDIT:
			// Skip these items
			break;

		case IDC_MSGSTYLESET_FONTNAME_COMBO:
		case IDC_MSGSTYLESET_FONTSIZE_COMBO:
		case IDC_MSGSTYLESET_ICONID_COMBO:
		case IDC_MSGSTYLESET_DISPLAYPOS_COMBO:
			SetupComboBox(nID, pAppLang);
			break;

		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}

	// Default
	SDialog::SetupLanguage();
}

/**
 * @brief	Setup data for combo-boxes
 * @param	nComboID	- ID of combo box
 * @param	ptrLanguage - Language package pointer
 * @return	None
 */
void CRmdMsgStyleSetDlg::SetupComboBox(unsigned nComboID, LANGTABLE_PTR ptrLanguage)
{
	switch (nComboID)
	{
		case IDC_MSGSTYLESET_FONTNAME_COMBO:
		{
			// Initialization
			if (m_pFontNamePickCombo == NULL) {
				m_pFontNamePickCombo = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_FONTNAME_COMBO);
				NULL_POINTER_BREAK(m_pFontNamePickCombo, return);
			}

			// Enumerate all currently available font names
			std::vector<std::wstring> fontNames;
			if (!EnumFontNames(fontNames)) {
				// Enumerate font names failed
				TRACE_ERROR("Error: Enumerate font names failed");
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
				return;
			}

			// Parse to combo-box
			m_pFontNamePickCombo->ResetContent();
			for (String fontName : fontNames)
				m_pFontNamePickCombo->AddString(fontName);

			return;
		}

		case IDC_MSGSTYLESET_FONTSIZE_COMBO:
		{
			// Initialization
			if (m_pFontSizePickCombo == NULL) {
				m_pFontSizePickCombo = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_FONTSIZE_COMBO);
				NULL_POINTER_BREAK(m_pFontSizePickCombo, return);
			}

			// List-up available font size values
			m_pFontSizePickCombo->ResetContent();
			for (int fontSize = RmdMsgStyleSet::minFontSize; fontSize < RmdMsgStyleSet::maxFontSize; fontSize++)
				m_pFontSizePickCombo->AddString(String::FromNumber(fontSize));

			return;
		}

		case IDC_MSGSTYLESET_ICONID_COMBO:
		{
			// Initialization
			if (m_pIconIDPickCombo == NULL) {
				m_pIconIDPickCombo = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_ICONID_COMBO);
				NULL_POINTER_BREAK(m_pIconIDPickCombo, return);
			}

			// List-up available system icon IDs
			m_pIconIDPickCombo->ResetContent();
			for (int nIndex = 0; nIndex < TABLE_SIZE(IDTable::SystemIcon); nIndex++)
				m_pIconIDPickCombo->AddString(GetLanguageString(ptrLanguage, IDTable::SystemIcon[nIndex].second));

			return;
		}

		case IDC_MSGSTYLESET_DISPLAYPOS_COMBO:
		{
			// Initialization
			if (m_pDisplayPosCombo == NULL) {
				m_pDisplayPosCombo = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_DISPLAYPOS_COMBO);
				NULL_POINTER_BREAK(m_pDisplayPosCombo, return);
			}

			// List-up available message display positions
			m_pDisplayPosCombo->ResetContent();
			for (int nIndex = 0; nIndex < TABLE_SIZE(IDTable::DisplayPosition); nIndex++)
				m_pDisplayPosCombo->AddString(GetLanguageString(ptrLanguage, IDTable::DisplayPosition[nIndex].second));

			return;
		}
	}

	// Default
	SDialog::SetupComboBox(nComboID, ptrLanguage);
}
	
/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::SetupDialogItemState(void)
{
	// Initialize dialog items
	if (m_pBkgrdColorPickBtn == NULL) {
		m_pBkgrdColorPickBtn = (CButton*)GetDlgItem(IDC_MSGSTYLESET_BKGRDCLR_PICKER);
		if (m_pBkgrdColorPickBtn == NULL) {
			TRACE_ERROR("Error: Background color picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pTextColorPickBtn == NULL) {
		m_pTextColorPickBtn = (CButton*)GetDlgItem(IDC_MSGSTYLESET_TEXTCLR_PICKER);
		if (m_pTextColorPickBtn == NULL) {
			TRACE_ERROR("Error: Text color picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pFontNamePickCombo == NULL) {
		m_pFontNamePickCombo = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_FONTNAME_COMBO);
		if (m_pFontNamePickCombo == NULL) {
			TRACE_ERROR("Error: Font name picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pFontSizePickCombo == NULL) {
		m_pFontSizePickCombo = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_FONTSIZE_COMBO);
		if (m_pFontSizePickCombo == NULL) {
			TRACE_ERROR("Error: Font size picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pFontSizePickCombo == NULL) {
		m_pFontSizePickCombo = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_FONTSIZE_COMBO);
		if (m_pFontSizePickCombo == NULL) {
			TRACE_ERROR("Error: Font size picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pIconIDPickCombo == NULL) {
		m_pIconIDPickCombo = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_ICONID_COMBO);
		if (m_pIconIDPickCombo == NULL) {
			TRACE_ERROR("Error: IconID picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pIconSizeEdit == NULL) {
		m_pIconSizeEdit = (CEdit*)GetDlgItem(IDC_MSGSTYLESET_ICONSIZE_EDIT);
		if (m_pIconSizeEdit == NULL) {
			TRACE_ERROR("Error: Icon size edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pIconPositionOnTopRad == NULL) {
		m_pIconPositionOnTopRad = (CButton*)GetDlgItem(IDC_MSGSTYLESET_ICONPOS_TOP_RAD);
		if (m_pIconPositionOnTopRad == NULL) {
			TRACE_ERROR("Error: 'Icon on Top' radio button not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pIconPositionOnLeftRad == NULL) {
		m_pIconPositionOnLeftRad = (CButton*)GetDlgItem(IDC_MSGSTYLESET_ICONPOS_LEFT_RAD);
		if (m_pIconPositionOnLeftRad == NULL) {
			TRACE_ERROR("Error: 'Icon on Left' radio button not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pIconPreviewStatic == NULL) {
		m_pIconPreviewStatic = (CStatic*)GetDlgItem(IDC_MSGSTYLESET_MSGICON_VIEW);
		if (m_pIconPreviewStatic == NULL) {
			TRACE_ERROR("Error: Icon preview control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pTimeoutEdit == NULL) {
		m_pTimeoutEdit = (CEdit*)GetDlgItem(IDC_MSGSTYLESET_TIMEOUT_EDIT);
		if (m_pTimeoutEdit == NULL) {
			TRACE_ERROR("Error: Timeout edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pTimeoutSpin == NULL) {
		m_pTimeoutSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_MSGSTYLESET_TIMEOUT_SPIN);
		if (m_pTimeoutSpin == NULL) {
			TRACE_ERROR("Error: Timeout spin control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pDisplayPosCombo == NULL) {
		m_pDisplayPosCombo = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_DISPLAYPOS_COMBO);
		if (m_pDisplayPosCombo == NULL) {
			TRACE_ERROR("Error: Display position picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pHorizontalMarginEdit == NULL) {
		m_pHorizontalMarginEdit = (CEdit*)GetDlgItem(IDC_MSGSTYLESET_HMARGIN_EDIT);
		if (m_pHorizontalMarginEdit == NULL) {
			TRACE_ERROR("Error: H-Margin edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pVerticalMarginEdit == NULL) {
		m_pVerticalMarginEdit = (CEdit*)GetDlgItem(IDC_MSGSTYLESET_VMARGIN_EDIT);
		if (m_pVerticalMarginEdit == NULL) {
			TRACE_ERROR("Error: V-Margin edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Setup timeout spin-edit
	if (m_pTimeoutSpin != NULL) {
		if (m_pTimeoutEdit == NULL) return;

		// Set buddy: Timeout edit control
		m_pTimeoutSpin->SetBuddy(m_pTimeoutEdit);

		// Set spin edit value
		m_pTimeoutSpin->SetRange(RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut);
		m_pTimeoutSpin->SetPos(RmdMsgStyleSet::defaultTimeout);
	}

	// Set maximum length for edit-boxes
	if (m_pFontSizePickCombo != NULL)
		m_pFontSizePickCombo->SendMessage(CB_LIMITTEXT, (WPARAM)maxFontSizeDigits, 0);
	if (m_pIconSizeEdit != NULL)
		m_pIconSizeEdit->SetLimitText(maxIconSizeDigits);
	if (m_pHorizontalMarginEdit != NULL)
		m_pHorizontalMarginEdit->SetLimitText(maxMarginValDigits);
	if (m_pVerticalMarginEdit != NULL)
		m_pVerticalMarginEdit->SetLimitText(maxMarginValDigits);

	// Default
	SDialog::SetupDialogItemState();
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	bRecheckState - Recheck all item's state
 * @return	None
 */
void CRmdMsgStyleSetDlg::RefreshDialogItemState(bool bRecheckState /* = false */)
{
	// Redraw background/text color picker
	if (m_pBkgrdColorPickBtn != NULL)
		m_pBkgrdColorPickBtn->RedrawWindow();
	if (m_pTextColorPickBtn != NULL)
		m_pTextColorPickBtn->RedrawWindow();

	// Redraw icon preview
	if (m_pIconPreviewStatic != NULL)
		::RedrawWindow(m_pIconPreviewStatic->GetSafeHwnd(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	// Default
	SDialog::RefreshDialogItemState(bRecheckState);
}

/**
 * @brief	Update message style data from/to dialog controls
 * @param	bUpdate - Update data flag
 * @return	None
 */
void CRmdMsgStyleSetDlg::UpdateDialogData(bool bUpdate)
{
	if (bUpdate == true) {

		/***************************************************************/
		/*															   */
		/*				Update data from dialog controls			   */
		/*															   */
		/***************************************************************/

		int nCurSel = 0;

		// Background color
		m_rmsMsgStyleTemp.SetBkgrdColor(m_clrMsgBackground);

		// Text color
		m_rmsMsgStyleTemp.SetTextColor(m_clrMsgText);

		// Font name
		String fontName = RmdMsgStyleSet::defaultFontName;
		if (m_pFontNamePickCombo != NULL) {
			wchar_t tempBuff[Constant::Max::StringLength];
			nCurSel = m_pFontSizePickCombo->GetCurSel();
			m_pFontSizePickCombo->GetLBText(nCurSel, tempBuff);
			fontName = tempBuff;
		}
		m_rmsMsgStyleTemp.SetFontName(fontName);

		// Font size
		int nFontSize = RmdMsgStyleSet::defaultFontSize;
		if (m_pFontSizePickCombo != NULL) {
			wchar_t tempBuff[maxFontSizeDigits + 1];
			nCurSel = m_pFontSizePickCombo->GetCurSel();
			m_pFontSizePickCombo->GetLBText(nCurSel, tempBuff);
			nFontSize = _wtoi(tempBuff);
		}
		m_rmsMsgStyleTemp.SetFontSize(nFontSize);

		// Icon ID
		int nIconID = RmdMsgStyleSet::defaultIconID;
		if (m_pIconIDPickCombo != NULL) {
			nCurSel = m_pIconIDPickCombo->GetCurSel();
			nIconID = IDTable::SystemIcon[nCurSel].first;
		}
		m_rmsMsgStyleTemp.SetIconID(nIconID);

		// Icon size
		int nIconSize = RmdMsgStyleSet::defaultIconSize;
		if (m_pIconSizeEdit != NULL) {
			wchar_t tempBuff[maxIconSizeDigits + 1];
			m_pIconSizeEdit->GetWindowText(tempBuff, sizeof(tempBuff));
			nIconSize = _wtoi(tempBuff);
			if (nIconSize < RmdMsgStyleSet::minIconSize)
				nIconSize = RmdMsgStyleSet::minIconSize;
			else if (nIconSize > RmdMsgStyleSet::maxIconSize)
				nIconSize = RmdMsgStyleSet::maxIconSize;
		}
		m_rmsMsgStyleTemp.SetIconSize(nIconSize);

		// Icon position
		int nIconPos = RmdMsgStyleSet::defaultIconPosition;
		if (m_pIconPositionOnTopRad != NULL && m_pIconPositionOnTopRad->GetCheck())
			nIconPos = RmdMsgStyleSet::IconPosition::IconOnTheTop;
		else if (m_pIconPositionOnLeftRad != NULL && m_pIconPositionOnLeftRad->GetCheck())
			nIconPos = RmdMsgStyleSet::IconPosition::IconOnTheLeft;
		m_rmsMsgStyleTemp.SetIconPosition(nIconPos);

		// Timeout
		int nTimeout = RmdMsgStyleSet::defaultTimeout;
		if (m_pTimeoutSpin != NULL) {
			nTimeout = m_pTimeoutSpin->GetPos();
			if (nTimeout < RmdMsgStyleSet::minTimeOut)
				nTimeout = RmdMsgStyleSet::minTimeOut;
			else if (nTimeout > RmdMsgStyleSet::maxTimeOut)
				nTimeout = RmdMsgStyleSet::maxTimeOut;
		}
		m_rmsMsgStyleTemp.SetTimeout(nTimeout);

		// Display position
		int nDisplayPos = RmdMsgStyleSet::defaultDisplayPosition;
		if (m_pDisplayPosCombo != NULL) {
			nCurSel = m_pDisplayPosCombo->GetCurSel();
			nDisplayPos = IDTable::DisplayPosition[nCurSel].first;
		}
		m_rmsMsgStyleTemp.SetDisplayPosition(nDisplayPos);

		// Horizontal margin
		int nHMargin = RmdMsgStyleSet::defaultHorizontalMargin;
		if (m_pHorizontalMarginEdit != NULL) {
			wchar_t tempBuff[maxMarginValDigits + 1];
			m_pHorizontalMarginEdit->GetWindowText(tempBuff, sizeof(tempBuff));
			nHMargin = _wtoi(tempBuff);
			if (nHMargin < RmdMsgStyleSet::minMarginVal)
				nHMargin = RmdMsgStyleSet::minMarginVal;
			else if (nHMargin > RmdMsgStyleSet::maxMarginVal)
				nHMargin = RmdMsgStyleSet::maxMarginVal;
		}
		m_rmsMsgStyleTemp.SetHorizontalMargin(nHMargin);

		// Vertical margin
		int nVMargin = RmdMsgStyleSet::defaultVerticalMargin;
		if (m_pVerticalMarginEdit != NULL) {
			wchar_t tempBuff[maxMarginValDigits + 1];
			m_pVerticalMarginEdit->GetWindowText(tempBuff, sizeof(tempBuff));
			nVMargin = _wtoi(tempBuff);
			if (nVMargin < RmdMsgStyleSet::minMarginVal)
				nVMargin = RmdMsgStyleSet::minMarginVal;
			else if (nVMargin > RmdMsgStyleSet::maxMarginVal)
				nVMargin = RmdMsgStyleSet::maxMarginVal;
		}
		m_rmsMsgStyleTemp.SetVerticalMargin(nVMargin);
	}
	else {

		/***************************************************************/
		/*															   */
		/*				  Bind data to dialog controls				   */
		/*															   */
		/***************************************************************/

		// Load app language package
		LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

		// Background color
		m_clrMsgBackground = m_rmsMsgStyleTemp.GetBkgrdColor();

		// Text color
		m_clrMsgText = m_rmsMsgStyleTemp.GetTextColor();

		// Font name
		String fontName = m_rmsMsgStyleTemp.GetFontName();
		if (m_pFontNamePickCombo != NULL)
			m_pFontNamePickCombo->SelectString(-1, fontName);

		// Font size
		int nFontSize = m_rmsMsgStyleTemp.GetFontSize();
		if (nFontSize < RmdMsgStyleSet::minFontSize)
			nFontSize = RmdMsgStyleSet::minFontSize;
		else if (nFontSize > RmdMsgStyleSet::maxFontSize)
			nFontSize = RmdMsgStyleSet::maxFontSize;
		if (m_pFontSizePickCombo != NULL) {
			String fontSizeStr = String::FromNumber(nFontSize);
			m_pFontSizePickCombo->SelectString(-1, fontSizeStr);
		}

		// Icon ID
		int nIconID = m_rmsMsgStyleTemp.GetIconID();
		if (m_pIconIDPickCombo != NULL) {
			String iconName = GetLanguageString(pAppLang, GetPairedID(IDTable::SystemIcon, nIconID));
			if (IS_NOT_NULL_STRING(iconName))
				m_pIconIDPickCombo->SelectString(-1, iconName);
		}

		// Icon size
		int nIconSize = m_rmsMsgStyleTemp.GetIconSize();
		if (nIconSize < RmdMsgStyleSet::minIconSize)
			nIconSize = RmdMsgStyleSet::minIconSize;
		else if (nIconSize > RmdMsgStyleSet::maxIconSize)
			nIconSize = RmdMsgStyleSet::maxIconSize;
		if (m_pIconSizeEdit != NULL) {
			String iconSizeStr = String::FromNumber(nIconSize);
			m_pIconSizeEdit->SetWindowText(iconSizeStr);
		}

		// Icon position
		int nIconPos = m_rmsMsgStyleTemp.GetIconPosition();
		if (m_pIconPositionOnTopRad != NULL)
			m_pIconPositionOnTopRad->SetCheck(nIconPos == RmdMsgStyleSet::IconPosition::IconOnTheTop);
		if (m_pIconPositionOnLeftRad != NULL)
			m_pIconPositionOnLeftRad->SetCheck(nIconPos == RmdMsgStyleSet::IconPosition::IconOnTheLeft);

		// Timeout
		int nTimeout = m_rmsMsgStyleTemp.GetTimeout();
		if (nTimeout < RmdMsgStyleSet::minTimeOut)
			nTimeout = RmdMsgStyleSet::minTimeOut;
		else if (nTimeout > RmdMsgStyleSet::maxTimeOut)
			nTimeout = RmdMsgStyleSet::maxTimeOut;
		if (m_pTimeoutEdit != NULL) {
			String timeOutValStr = String::FromNumber(nTimeout);
			m_pTimeoutEdit->SetWindowText(timeOutValStr);
		}
		if (m_pTimeoutSpin != NULL)
			m_pTimeoutSpin->SetPos(nTimeout);

		// Display position
		int nDisplayPos = m_rmsMsgStyleTemp.GetDisplayPosition();
		if (m_pDisplayPosCombo != NULL) {
			String displayPosStr = GetLanguageString(pAppLang, GetPairedID(IDTable::DisplayPosition, nDisplayPos));
			if (IS_NOT_NULL_STRING(displayPosStr))
				m_pDisplayPosCombo->SelectString(-1, displayPosStr);
		}

		// Horizontal margin
		int nHMargin = m_rmsMsgStyleTemp.GetHorizontalMargin();
		if (nHMargin < RmdMsgStyleSet::minMarginVal)
			nHMargin = RmdMsgStyleSet::minMarginVal;
		else if (nHMargin > RmdMsgStyleSet::maxMarginVal)
			nHMargin = RmdMsgStyleSet::maxMarginVal;
		if (m_pHorizontalMarginEdit != NULL) {
			String marginValStr = String::FromNumber(nHMargin);
			m_pHorizontalMarginEdit->SetWindowText(marginValStr);
		}

		// Vertical margin
		int nVMargin = m_rmsMsgStyleTemp.GetVerticalMargin();
		if (nVMargin < RmdMsgStyleSet::minMarginVal)
			nVMargin = RmdMsgStyleSet::minMarginVal;
		else if (nVMargin > RmdMsgStyleSet::maxMarginVal)
			nVMargin = RmdMsgStyleSet::maxMarginVal;
		if (m_pVerticalMarginEdit != NULL) {
			String marginValStr = String::FromNumber(nVMargin);
			m_pVerticalMarginEdit->SetWindowText(marginValStr);
		}

		// Update data
		UpdateData(false);
		RefreshDialogItemState();
	}
}
