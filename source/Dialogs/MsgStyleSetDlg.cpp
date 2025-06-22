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
constexpr const unsigned maxFontSizeDigits = 3;
constexpr const unsigned maxIconSizeDigits = 3;
constexpr const unsigned maxTimeoutDigits = 4;
constexpr const unsigned maxMarginValDigits = 3;

// Caption format
constexpr const wchar_t* captionDetailFormat = _T(" - [%s]");

// Local string variables
String balloonTitleInvalidValue;
String balloonFormatInvalidValue;


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
	m_flagDataSet = DataSetFlag::commonStyle;
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
 * @brief	Handle click event for [Apply] button
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::OnApply()
{
	// Save data
	UpdateDialogData(true);
	m_rmsMsgStyleData.Copy(m_rmsMsgStyleTemp);

	// Close the dialog
	SetReturnFlag(ReturnFlag::OK);
	EndDialog(IDC_MSGSTYLESET_APPLY_BTN);
}

/**
 * @brief	Handle click event for [Cancel] button
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::OnCancel()
{
	// Close the dialog
	SetReturnFlag(ReturnFlag::Cancel);
	EndDialog(IDC_MSGSTYLESET_CANCEL_BTN);
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

		case IDC_MSGSTYLESET_FONTSIZE_COMBO:
		{
			if (HIWORD(wParam) == CBN_EDITCHANGE && m_pFontSizePickCombo != NULL)
			{
				// Validation
				HWND hFontSizeEdit = ::GetWindow(m_pFontSizePickCombo->GetSafeHwnd(), GW_CHILD);
				int nFontSize = GetEditValue(hFontSizeEdit, maxFontSizeDigits);
				ValidateEditValue(hFontSizeEdit, nFontSize, RmdMsgStyleSet::minFontSize, RmdMsgStyleSet::maxFontSize, true);

				return true;
			}
			else if (HIWORD(wParam) == CBN_KILLFOCUS && m_pFontSizePickCombo != NULL)
			{
				// Auto-correction
				HWND hFontSizeEdit = ::GetWindow(m_pFontSizePickCombo->GetSafeHwnd(), GW_CHILD);
				int nFontSize = GetEditValue(hFontSizeEdit, maxFontSizeDigits);
				if (!ValidateAndCorrect(nFontSize, RmdMsgStyleSet::minFontSize, RmdMsgStyleSet::maxFontSize))
					m_pFontSizePickCombo->SelectString(-1, String::FromNumber(nFontSize));

				return true;
			}

			break;
		}

		case IDC_MSGSTYLESET_ICONID_COMBO:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE && m_pIconPreviewStatic != NULL)
			{
				// Trigger redrawing icon preview
				::InvalidateRect(m_pIconPreviewStatic->GetSafeHwnd(), NULL, true);
				return true;
			}

			break;
		}

		case IDC_MSGSTYLESET_ICONSIZE_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE && m_pIconSizeEdit != NULL)
			{
				// Validation
				int nIconSize = GetEditValue(m_pIconSizeEdit->GetSafeHwnd(), maxIconSizeDigits);
				bool isValid = ValidateEditValue(m_pIconSizeEdit->GetSafeHwnd(), nIconSize, RmdMsgStyleSet::minIconSize, RmdMsgStyleSet::maxIconSize, true);

				// Trigger redrawing icon preview
				if (m_pIconPreviewStatic != NULL && isValid)
					::InvalidateRect(m_pIconPreviewStatic->GetSafeHwnd(), NULL, true);

				return true;
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS && m_pIconSizeEdit != NULL)
			{
				// Auto-correction
				int nIconSize = GetEditValue(m_pIconSizeEdit->GetSafeHwnd(), maxIconSizeDigits);
				if (!ValidateAndCorrect(nIconSize, RmdMsgStyleSet::minIconSize, RmdMsgStyleSet::maxIconSize))
					m_pIconSizeEdit->SetWindowText(String::FromNumber(nIconSize));

				// Trigger redrawing icon preview
				if (m_pIconPreviewStatic != NULL)
					::InvalidateRect(m_pIconPreviewStatic->GetSafeHwnd(), NULL, true);

				return true;
			}

			break;
		}

		case IDC_MSGSTYLESET_TIMEOUT_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE && m_pTimeoutEdit != NULL)
			{
				// Validation
				int nTimeout = GetEditValue(m_pTimeoutEdit->GetSafeHwnd(), maxTimeoutDigits);
				ValidateEditValue(m_pTimeoutEdit->GetSafeHwnd(), nTimeout, RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut, true);

				return true;
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS && m_pTimeoutEdit != NULL)
			{
				// Auto-correction
				int nTimeout = GetEditValue(m_pTimeoutEdit->GetSafeHwnd(), maxTimeoutDigits);
				if (!ValidateAndCorrect(nTimeout, RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut))
					m_pTimeoutEdit->SetWindowText(String::FromNumber(nTimeout));

				// Update timeout spin position
				if (m_pTimeoutSpin != NULL)
					m_pTimeoutSpin->SetPos(nTimeout);

				return true;
			}

			break;
		}

		case IDC_MSGSTYLESET_HMARGIN_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE && m_pHorizontalMarginEdit != NULL)
			{
				// Validation
				int nHMargin = GetEditValue(m_pHorizontalMarginEdit->GetSafeHwnd(), maxMarginValDigits);
				ValidateEditValue(m_pHorizontalMarginEdit->GetSafeHwnd(), nHMargin, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal, true);

				return true;
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS && m_pHorizontalMarginEdit != NULL)
			{
				// Auto-correction
				int nHMargin = GetEditValue(m_pHorizontalMarginEdit->GetSafeHwnd(), maxMarginValDigits);
				if (!ValidateAndCorrect(nHMargin, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal))
					m_pHorizontalMarginEdit->SetWindowText(String::FromNumber(nHMargin));

				return true;
			}

			break;
		}

		case IDC_MSGSTYLESET_VMARGIN_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE && m_pVerticalMarginEdit != NULL)
			{
				// Validation
				int nVMargin = GetEditValue(m_pVerticalMarginEdit->GetSafeHwnd(), maxMarginValDigits);
				ValidateEditValue(m_pVerticalMarginEdit->GetSafeHwnd(), nVMargin, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal, true);

				return true;
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS && m_pVerticalMarginEdit != NULL)
			{
				// Auto-correction
				int nVMargin = GetEditValue(m_pVerticalMarginEdit->GetSafeHwnd(), maxMarginValDigits);
				if (!ValidateAndCorrect(nVMargin, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal))
					m_pVerticalMarginEdit->SetWindowText(String::FromNumber(nVMargin));

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
		case WM_CLOSE:
			SetReturnFlag(ReturnFlag::Close);
			EndDialog(IDC_MSGSTYLESET_CANCEL_BTN);
			break;

		case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT pDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
			HBRUSH hFillBrush;

			switch (pDrawItemStruct->CtlID)
			{
			case IDC_MSGSTYLESET_BKGRDCLR_PICKER:
				hFillBrush = CreateSolidBrush(m_clrMsgBackground);
				break;
			case IDC_MSGSTYLESET_TEXTCLR_PICKER:
				hFillBrush = CreateSolidBrush(m_clrMsgText);
				break;
			case IDC_MSGSTYLESET_MSGICON_VIEW:
			{
				RedrawIconPreview(pDrawItemStruct);
				return true;
			}

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
				m_pIconPreviewStatic->Invalidate();

			break;
		}

		case WM_NOTIFY:
		{
			LPNMHDR lpNMHDR = reinterpret_cast<LPNMHDR>(lParam);
			if (lpNMHDR->code == UDN_DELTAPOS && lpNMHDR->idFrom == IDC_MSGSTYLESET_TIMEOUT_SPIN)
			{
				LPNMUPDOWN lpNMUpDown = reinterpret_cast<LPNMUPDOWN>(lpNMHDR);
				int nNewPos = lpNMUpDown->iPos + lpNMUpDown->iDelta;
				if (nNewPos >= RmdMsgStyleSet::minTimeOut && nNewPos <= RmdMsgStyleSet::maxTimeOut) {
					if (m_pTimeoutEdit != NULL)
						m_pTimeoutEdit->SetWindowText(String::FromNumber(nNewPos));
					if (m_pTimeoutSpin != NULL)
						m_pTimeoutSpin->SetPos(nNewPos);
				}

				return true;
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

	// Set dialog caption
	String dialogCaption = GetLanguageString(pAppLang, GetDialogID());
	if (m_flagDataSet == DataSetFlag::commonStyle)
		dialogCaption += StringUtils::StringFormat(captionDetailFormat, GetLanguageString(pAppLang, PWRRMD_STYLE_COMMONSTYLE));
	else if (m_flagDataSet == DataSetFlag::customStyle)
		dialogCaption += StringUtils::StringFormat(captionDetailFormat, GetLanguageString(pAppLang, PWRRMD_STYLE_CUSTOMSTYLE));
	this->SetCaption(dialogCaption);

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

	// Initialize local string variables
	balloonTitleInvalidValue = GetLanguageString(pAppLang, BALLOON_TIP_INVALID_VALUE_TITLE);
	balloonFormatInvalidValue = GetLanguageString(pAppLang, BALLOON_TIP_INVALID_VALUE_FORMAT);

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
			for (int fontSize = RmdMsgStyleSet::minFontSize; fontSize <= RmdMsgStyleSet::maxFontSize; fontSize++)
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

		// Set maximum length
		if (m_pTimeoutEdit == NULL) return;
		m_pTimeoutEdit->SetLimitText(maxTimeoutDigits);

		// Set buddy: Timeout edit control
		m_pTimeoutSpin->SetBuddy(m_pTimeoutEdit);

		// Set spin edit value
		m_pTimeoutSpin->SetRange(RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut);
		m_pTimeoutSpin->SetPos(RmdMsgStyleSet::defaultTimeout);
	}

	// Setup font size picker
	if (m_pFontSizePickCombo != NULL) {
		HWND hFontSizeEdit = ::GetWindow(m_pFontSizePickCombo->GetSafeHwnd(), GW_CHILD);
		SetWindowLongPtr(hFontSizeEdit, GWL_STYLE, GetWindowLongPtr(hFontSizeEdit, GWL_STYLE) | ES_CENTER);
		m_pFontSizePickCombo->SendMessage(CB_LIMITTEXT, (WPARAM)maxFontSizeDigits, 0);
	}

	// Set maximum length for edit-boxes
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
		::InvalidateRect(m_pIconPreviewStatic->GetSafeHwnd(), NULL, true);

	// Default
	SDialog::RefreshDialogItemState(bRecheckState);
}

/**
 * @brief	Refresh and redraw the icon preview static control
 * @param	lpDrawItemStruct - Item drawing data
 * @return	None
 */
void CRmdMsgStyleSetDlg::RedrawIconPreview(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// Validation
	if (lpDrawItemStruct == NULL) return;

	// Device context handle
	HDC hDC = lpDrawItemStruct->hDC;

	// Icon ID
	int nIconID = RmdMsgStyleSet::defaultIconID;
	if (m_pIconIDPickCombo != NULL) {
		int nCurSel = m_pIconIDPickCombo->GetCurSel();
		nIconID = IDTable::SystemIcon[nCurSel].first;
	}

	// Icon size
	int nIconSize = RmdMsgStyleSet::defaultIconSize;
	if (m_pIconSizeEdit != NULL) {
		nIconSize = GetEditValue(m_pIconSizeEdit->GetSafeHwnd(), maxIconSizeDigits);
		ValidateAndCorrect(nIconSize, RmdMsgStyleSet::minIconSize, RmdMsgStyleSet::maxIconSize);
	}
	int cx = nIconSize;
	int cy = nIconSize;

	// Erase the icon preview control before redrawing
	RECT rcPreviewCtrl = lpDrawItemStruct->rcItem;
	HBRUSH hFillBrush = GetSysColorBrush(COLOR_3DFACE);
	FillRect(hDC, &rcPreviewCtrl, hFillBrush);

	// Draw area rect
	Rect drawAreaRect;
	drawAreaRect._top = rcPreviewCtrl.top;
	drawAreaRect._left = rcPreviewCtrl.left;
	drawAreaRect._bottom = rcPreviewCtrl.bottom;
	drawAreaRect._right = rcPreviewCtrl.right;

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
		DrawIconEx(hDC, iconDrawPosition._x, iconDrawPosition._y, hIcon, cx, cy, NULL, NULL, DI_NORMAL);
	}
}

/**
 * @brief	Return the input value of the specified edit control
 * @param	hEditCtrl	- Edit control handle
 * @param	maxDigits	- Maximum number of digits
 * @return	int
 */
int CRmdMsgStyleSetDlg::GetEditValue(HWND hEditCtrl, unsigned maxDigits)
{
	// Argument validation
	if (hEditCtrl == NULL || maxDigits <= 0)
		return false;

	// Get the value
	const size_t maxBuffSize = maxDigits + 1;
	std::vector<wchar_t> tempBuff(maxBuffSize);
	::GetWindowText(hEditCtrl, tempBuff.data(), static_cast<int>(maxBuffSize));
	return _wtoi(tempBuff.data());
}

/**
 * @brief	Validate, auto-correct and display value out of bounds warning for edit controls
 * @param	hEditCtrl	- Edit control handle
 * @param	maxDigits	- Maximum number of digits
 * @param	minVal		- Minimium value
 * @param	maxVal		- Maximum value
 * @param	showWarning - Whether to show warning
 * @return	true/false
 */
bool CRmdMsgStyleSetDlg::ValidateEditValue(HWND hEditCtrl, int& inputVal, int minVal, int maxVal, bool showWarning /* = true */)
{
	// Validation & auto-correction
	if (ValidateAndCorrect(inputVal, minVal, maxVal)) {
		if (hEditCtrl != NULL)
			::SendMessage(hEditCtrl, EM_HIDEBALLOONTIP, 0, 0);
		return true;
	}
	else {
		// Show warning
		if (hEditCtrl != NULL) {
			if (showWarning)
			{
				EDITBALLOONTIP editBalloonTip = { sizeof(EDITBALLOONTIP) };
				editBalloonTip.pszTitle = balloonTitleInvalidValue;
				editBalloonTip.pszText = StringUtils::StringFormat(balloonFormatInvalidValue, minVal, maxVal);
				editBalloonTip.ttiIcon = TTI_WARNING;

				::SendMessage(hEditCtrl, EM_SHOWBALLOONTIP, 0, reinterpret_cast<LPARAM>(&editBalloonTip));
			}
			else {
				::SendMessage(hEditCtrl, EM_HIDEBALLOONTIP, 0, 0);
			}
		}

		return false;
	}
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
			nCurSel = m_pFontNamePickCombo->GetCurSel();
			m_pFontNamePickCombo->GetLBText(nCurSel, tempBuff);
			fontName = tempBuff;
		}
		m_rmsMsgStyleTemp.SetFontName(fontName);

		// Font size
		int nFontSize = RmdMsgStyleSet::defaultFontSize;
		if (m_pFontSizePickCombo != NULL) {
			HWND hFontSizeEdit = ::GetWindow(m_pFontSizePickCombo->GetSafeHwnd(), GW_CHILD);
			nFontSize = GetEditValue(hFontSizeEdit, maxFontSizeDigits);
			ValidateAndCorrect(nFontSize, RmdMsgStyleSet::minFontSize, RmdMsgStyleSet::maxFontSize);
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
			nIconSize = GetEditValue(m_pIconSizeEdit->GetSafeHwnd(), maxIconSizeDigits);
			ValidateAndCorrect(nIconSize, RmdMsgStyleSet::minIconSize, RmdMsgStyleSet::maxIconSize);
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
			ValidateAndCorrect(nTimeout, RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut);
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
			nHMargin = GetEditValue(m_pHorizontalMarginEdit->GetSafeHwnd(), maxMarginValDigits);
			ValidateAndCorrect(nIconSize, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal);
		}
		m_rmsMsgStyleTemp.SetHorizontalMargin(nHMargin);

		// Vertical margin
		int nVMargin = RmdMsgStyleSet::defaultVerticalMargin;
		if (m_pVerticalMarginEdit != NULL) {
			nVMargin = GetEditValue(m_pVerticalMarginEdit->GetSafeHwnd(), maxMarginValDigits);
			ValidateAndCorrect(nVMargin, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal);
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
		ValidateAndCorrect(nFontSize, RmdMsgStyleSet::minFontSize, RmdMsgStyleSet::maxFontSize);
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
		ValidateAndCorrect(nIconSize, RmdMsgStyleSet::minIconSize, RmdMsgStyleSet::maxIconSize);
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
		ValidateAndCorrect(nTimeout, RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut);
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
		ValidateAndCorrect(nHMargin, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal);
		if (m_pHorizontalMarginEdit != NULL) {
			String marginValStr = String::FromNumber(nHMargin);
			m_pHorizontalMarginEdit->SetWindowText(marginValStr);
		}

		// Vertical margin
		int nVMargin = m_rmsMsgStyleTemp.GetVerticalMargin();
		ValidateAndCorrect(nVMargin, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal);
		if (m_pVerticalMarginEdit != NULL) {
			String marginValStr = String::FromNumber(nVMargin);
			m_pVerticalMarginEdit->SetWindowText(marginValStr);
		}

		// Update data
		UpdateData(false);
		RefreshDialogItemState();
	}
}
