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
	backgroundColorPickBtnPtr_ = NULL;
	textColorPickBtnPtr_ = NULL;

	// Font settings
	fontNamePickComboPtr_ = NULL;
	fontSizePickComboPtr_ = NULL;

	// Icon settings
	iconIdPickComboPtr_ = NULL;
	iconSizeEditPtr_ = NULL;
	iconPositionOnTopRadPtr_ = NULL;
	iconPositionOnLeftRadPtr_ = NULL;
	iconPreviewStaticPtr_ = NULL;

	// Timeout settings
	timeoutEditPtr_ = NULL;
	timeoutSpinPtr_ = NULL;

	// Display position setting
	displayPosComboPtr_ = NULL;

	// Margin settings
	horizontalMarginEditPtr_ = NULL;
	verticalMarginEditPtr_ = NULL;

	// Data
	messageStyleData_ = RmdMsgStyleSet();
	tempMessageStyleData_ = RmdMsgStyleSet();
	messageBackgroundColor_ = RmdMsgStyleSet::defaultBkgrdColor;
	messageTextColor_ = RmdMsgStyleSet::defaultTextColor;
	flagDataSet_ = DataSetFlag::commonStyle;
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
	setupLanguage();
	setupDialogItemState();
	updateDialogData(false);

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
	updateDialogData(true);
	messageStyleData_.copy(tempMessageStyleData_);

	// Close the dialog
	setReturnFlag(ReturnFlag::OK);
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
	setReturnFlag(ReturnFlag::Cancel);
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
	updateDialogData(true);

	// Default timeout for previewing
	int defaultTimeout = PwrReminderData::previewTimeout;

	// Initialize preview message
	CReminderMsgDlg previewMsgDlg;
	previewMsgDlg.setCaptionFromLanguage(IDC_MSGSTYLESET_PREVIEW_BTN);
	previewMsgDlg.setDispMessage(L"Preview message style");
	previewMsgDlg.setMessageStyle(tempMessageStyleData_);
	previewMsgDlg.setAutoCloseInterval(defaultTimeout);

	// Set notify state flags
	previewMsgDlg.setTopMost(false);
	previewMsgDlg.setInitSound(true);

	// Display message
	previewMsgDlg.DoModal();
}

/**
 * @brief	Handle click event for [Reset] button
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::OnReset()
{
	// Reset data
	tempMessageStyleData_ = RmdMsgStyleSet();

	// Apply state
	updateDialogData(false);
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
				cc.rgbResult = messageBackgroundColor_;
			}
			else {
				cc.rgbResult = messageTextColor_;
			}

			cc.hwndOwner = this->GetSafeHwnd();
			if (ChooseColor(&cc)) {
				if (LOWORD(wParam) == IDC_MSGSTYLESET_BKGRDCLR_PICKER) {
					messageBackgroundColor_ = cc.rgbResult;
					if (backgroundColorPickBtnPtr_ != NULL)
						backgroundColorPickBtnPtr_->RedrawWindow();
				}
				else {
					messageTextColor_ = cc.rgbResult;
					if (textColorPickBtnPtr_ != NULL)
						textColorPickBtnPtr_->RedrawWindow();
				}
			}

			return true;
		}

		case IDC_MSGSTYLESET_FONTSIZE_COMBO:
		{
			if (HIWORD(wParam) == CBN_EDITCHANGE && fontSizePickComboPtr_ != NULL)
			{
				// Validation
				HWND fontSizeEditHandle = ::GetWindow(fontSizePickComboPtr_->GetSafeHwnd(), GW_CHILD);
				int fontSize = getEditValue(fontSizeEditHandle, maxFontSizeDigits);
				validateEditValue(fontSizeEditHandle, fontSize, RmdMsgStyleSet::minFontSize, RmdMsgStyleSet::maxFontSize, true);

				return true;
			}
			else if (HIWORD(wParam) == CBN_KILLFOCUS && fontSizePickComboPtr_ != NULL)
			{
				// Auto-correction
				HWND fontSizeEditHandle = ::GetWindow(fontSizePickComboPtr_->GetSafeHwnd(), GW_CHILD);
				int fontSize = getEditValue(fontSizeEditHandle, maxFontSizeDigits);
				if (!validateAndCorrect(fontSize, RmdMsgStyleSet::minFontSize, RmdMsgStyleSet::maxFontSize))
					fontSizePickComboPtr_->SelectString(-1, String::fromNumber(fontSize));

				return true;
			}

			break;
		}

		case IDC_MSGSTYLESET_ICONID_COMBO:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE && iconPreviewStaticPtr_ != NULL)
			{
				// Trigger redrawing icon preview
				::InvalidateRect(iconPreviewStaticPtr_->GetSafeHwnd(), NULL, true);
				return true;
			}

			break;
		}

		case IDC_MSGSTYLESET_ICONSIZE_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE && iconSizeEditPtr_ != NULL)
			{
				// Validation
				int iconSize = getEditValue(iconSizeEditPtr_->GetSafeHwnd(), maxIconSizeDigits);
				bool isValid = validateEditValue(iconSizeEditPtr_->GetSafeHwnd(), iconSize, RmdMsgStyleSet::minIconSize, RmdMsgStyleSet::maxIconSize, true);

				// Trigger redrawing icon preview
				if (iconPreviewStaticPtr_ != NULL && isValid)
					::InvalidateRect(iconPreviewStaticPtr_->GetSafeHwnd(), NULL, true);

				return true;
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS && iconSizeEditPtr_ != NULL)
			{
				// Auto-correction
				int iconSize = getEditValue(iconSizeEditPtr_->GetSafeHwnd(), maxIconSizeDigits);
				if (!validateAndCorrect(iconSize, RmdMsgStyleSet::minIconSize, RmdMsgStyleSet::maxIconSize))
					iconSizeEditPtr_->SetWindowText(String::fromNumber(iconSize));

				// Trigger redrawing icon preview
				if (iconPreviewStaticPtr_ != NULL)
					::InvalidateRect(iconPreviewStaticPtr_->GetSafeHwnd(), NULL, true);

				return true;
			}

			break;
		}

		case IDC_MSGSTYLESET_TIMEOUT_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE && timeoutEditPtr_ != NULL)
			{
				// Validation
				int timeoutValue = getEditValue(timeoutEditPtr_->GetSafeHwnd(), maxTimeoutDigits);
				validateEditValue(timeoutEditPtr_->GetSafeHwnd(), timeoutValue, RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut, true);

				return true;
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS && timeoutEditPtr_ != NULL)
			{
				// Auto-correction
				int timeoutValue = getEditValue(timeoutEditPtr_->GetSafeHwnd(), maxTimeoutDigits);
				if (!validateAndCorrect(timeoutValue, RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut))
					timeoutEditPtr_->SetWindowText(String::fromNumber(timeoutValue));

				// Update timeout spin position
				if (timeoutSpinPtr_ != NULL)
					timeoutSpinPtr_->SetPos(timeoutValue);

				return true;
			}

			break;
		}

		case IDC_MSGSTYLESET_HMARGIN_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE && horizontalMarginEditPtr_ != NULL)
			{
				// Validation
				int marginHorizontal = getEditValue(horizontalMarginEditPtr_->GetSafeHwnd(), maxMarginValDigits);
				validateEditValue(horizontalMarginEditPtr_->GetSafeHwnd(), marginHorizontal, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal, true);

				return true;
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS && horizontalMarginEditPtr_ != NULL)
			{
				// Auto-correction
				int marginHorizontal = getEditValue(horizontalMarginEditPtr_->GetSafeHwnd(), maxMarginValDigits);
				if (!validateAndCorrect(marginHorizontal, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal))
					horizontalMarginEditPtr_->SetWindowText(String::fromNumber(marginHorizontal));

				return true;
			}

			break;
		}

		case IDC_MSGSTYLESET_VMARGIN_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE && verticalMarginEditPtr_ != NULL)
			{
				// Validation
				int marginVertical = getEditValue(verticalMarginEditPtr_->GetSafeHwnd(), maxMarginValDigits);
				validateEditValue(verticalMarginEditPtr_->GetSafeHwnd(), marginVertical, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal, true);

				return true;
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS && verticalMarginEditPtr_ != NULL)
			{
				// Auto-correction
				int marginVertical = getEditValue(verticalMarginEditPtr_->GetSafeHwnd(), maxMarginValDigits);
				if (!validateAndCorrect(marginVertical, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal))
					verticalMarginEditPtr_->SetWindowText(String::fromNumber(marginVertical));

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
			setReturnFlag(ReturnFlag::Close);
			EndDialog(IDC_MSGSTYLESET_CANCEL_BTN);
			break;

		case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT drawItemStructPtr = (LPDRAWITEMSTRUCT)lParam;
			HBRUSH fillBrushHandle;

			switch (drawItemStructPtr->CtlID)
			{
			case IDC_MSGSTYLESET_BKGRDCLR_PICKER:
				fillBrushHandle = CreateSolidBrush(messageBackgroundColor_);
				break;
			case IDC_MSGSTYLESET_TEXTCLR_PICKER:
				fillBrushHandle = CreateSolidBrush(messageTextColor_);
				break;
			case IDC_MSGSTYLESET_MSGICON_VIEW:
			{
				redrawIconPreview(drawItemStructPtr);
				return true;
			}

			default:
				return false;
			}

			FillRect(drawItemStructPtr->hDC, &drawItemStructPtr->rcItem, fillBrushHandle);
			FrameRect(drawItemStructPtr->hDC, &drawItemStructPtr->rcItem, (HBRUSH)GetStockObject(BLACK_BRUSH));
			DeleteObject(fillBrushHandle);

			return true;
		}

		case WM_PAINT:
		{
			if (iconPreviewStaticPtr_ != NULL)
				iconPreviewStaticPtr_->Invalidate();

			break;
		}

		case WM_NOTIFY:
		{
			LPNMHDR notifyStructPtr = reinterpret_cast<LPNMHDR>(lParam);
			if (notifyStructPtr->code == UDN_DELTAPOS && notifyStructPtr->idFrom == IDC_MSGSTYLESET_TIMEOUT_SPIN)
			{
				LPNMUPDOWN upDownCtrlPtr = reinterpret_cast<LPNMUPDOWN>(notifyStructPtr);
				int newPos = upDownCtrlPtr->iPos + upDownCtrlPtr->iDelta;
				if (newPos >= RmdMsgStyleSet::minTimeOut && newPos <= RmdMsgStyleSet::maxTimeOut) {
					if (timeoutEditPtr_ != NULL)
						timeoutEditPtr_->SetWindowText(String::fromNumber(newPos));
					if (timeoutSpinPtr_ != NULL)
						timeoutSpinPtr_->SetPos(newPos);
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
void CRmdMsgStyleSetDlg::setupLanguage()
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Set dialog caption
	String dialogCaption = getLanguageString(languageTablePtr, getDialogId());
	if (flagDataSet_ == DataSetFlag::commonStyle)
		dialogCaption += StringUtils::stringFormat(captionDetailFormat, getLanguageString(languageTablePtr, PWRRMD_STYLE_COMMONSTYLE));
	else if (flagDataSet_ == DataSetFlag::customStyle)
		dialogCaption += StringUtils::stringFormat(captionDetailFormat, getLanguageString(languageTablePtr, PWRRMD_STYLE_CUSTOMSTYLE));
	this->setCaption(dialogCaption);

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		unsigned id = childWndPtr->GetDlgCtrlID();

		switch (id)
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
			setupComboBox(id, languageTablePtr);
			break;

		default:
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Initialize local string variables
	balloonTitleInvalidValue = getLanguageString(languageTablePtr, BALLOON_TIP_INVALID_VALUE_TITLE);
	balloonFormatInvalidValue = getLanguageString(languageTablePtr, BALLOON_TIP_INVALID_VALUE_FORMAT);

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Setup data for combo-boxes
 * @param	comboId	- ID of combo box
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CRmdMsgStyleSetDlg::setupComboBox(unsigned comboId, LANGTABLE_PTR languageTablePtr)
{
	switch (comboId)
	{
		case IDC_MSGSTYLESET_FONTNAME_COMBO:
		{
			// Initialization
			if (fontNamePickComboPtr_ == NULL) {
				fontNamePickComboPtr_ = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_FONTNAME_COMBO);
				NULL_POINTER_BREAK(fontNamePickComboPtr_, return);
			}

			// Enumerate all currently available font names
			std::vector<std::wstring> fontNames;
			if (!AppCore::enumFontNames(fontNames)) {
				// Enumerate font names failed
				TRACE_ERROR("Error: Enumerate font names failed");
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
				return;
			}

			// Parse to combo-box
			fontNamePickComboPtr_->ResetContent();
			for (String fontName : fontNames)
				fontNamePickComboPtr_->AddString(fontName);

			return;
		}

		case IDC_MSGSTYLESET_FONTSIZE_COMBO:
		{
			// Initialization
			if (fontSizePickComboPtr_ == NULL) {
				fontSizePickComboPtr_ = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_FONTSIZE_COMBO);
				NULL_POINTER_BREAK(fontSizePickComboPtr_, return);
			}

			// List-up available font size values
			fontSizePickComboPtr_->ResetContent();
			for (int fontSize = RmdMsgStyleSet::minFontSize; fontSize <= RmdMsgStyleSet::maxFontSize; fontSize++)
				fontSizePickComboPtr_->AddString(String::fromNumber(fontSize));

			return;
		}

		case IDC_MSGSTYLESET_ICONID_COMBO:
		{
			// Initialization
			if (iconIdPickComboPtr_ == NULL) {
				iconIdPickComboPtr_ = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_ICONID_COMBO);
				NULL_POINTER_BREAK(iconIdPickComboPtr_, return);
			}

			// List-up available system icon IDs
			iconIdPickComboPtr_->ResetContent();
			for (int index = 0; index < TABLE_SIZE(IDTable::SystemIcon); index++)
				iconIdPickComboPtr_->AddString(getLanguageString(languageTablePtr, IDTable::SystemIcon[index].second));

			return;
		}

		case IDC_MSGSTYLESET_DISPLAYPOS_COMBO:
		{
			// Initialization
			if (displayPosComboPtr_ == NULL) {
				displayPosComboPtr_ = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_DISPLAYPOS_COMBO);
				NULL_POINTER_BREAK(displayPosComboPtr_, return);
			}

			// List-up available message display positions
			displayPosComboPtr_->ResetContent();
			for (int index = 0; index < TABLE_SIZE(IDTable::DisplayPosition); index++)
				displayPosComboPtr_->AddString(getLanguageString(languageTablePtr, IDTable::DisplayPosition[index].second));

			return;
		}
	}

	// Default
	SDialog::setupComboBox(comboId, languageTablePtr);
}
	
/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void CRmdMsgStyleSetDlg::setupDialogItemState(void)
{
	// Initialize dialog items
	if (backgroundColorPickBtnPtr_ == NULL) {
		backgroundColorPickBtnPtr_ = (CButton*)GetDlgItem(IDC_MSGSTYLESET_BKGRDCLR_PICKER);
		if (backgroundColorPickBtnPtr_ == NULL) {
			TRACE_ERROR("Error: Background color picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (textColorPickBtnPtr_ == NULL) {
		textColorPickBtnPtr_ = (CButton*)GetDlgItem(IDC_MSGSTYLESET_TEXTCLR_PICKER);
		if (textColorPickBtnPtr_ == NULL) {
			TRACE_ERROR("Error: Text color picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (fontNamePickComboPtr_ == NULL) {
		fontNamePickComboPtr_ = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_FONTNAME_COMBO);
		if (fontNamePickComboPtr_ == NULL) {
			TRACE_ERROR("Error: Font name picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (fontSizePickComboPtr_ == NULL) {
		fontSizePickComboPtr_ = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_FONTSIZE_COMBO);
		if (fontSizePickComboPtr_ == NULL) {
			TRACE_ERROR("Error: Font size picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (fontSizePickComboPtr_ == NULL) {
		fontSizePickComboPtr_ = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_FONTSIZE_COMBO);
		if (fontSizePickComboPtr_ == NULL) {
			TRACE_ERROR("Error: Font size picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (iconIdPickComboPtr_ == NULL) {
		iconIdPickComboPtr_ = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_ICONID_COMBO);
		if (iconIdPickComboPtr_ == NULL) {
			TRACE_ERROR("Error: IconID picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (iconSizeEditPtr_ == NULL) {
		iconSizeEditPtr_ = (CEdit*)GetDlgItem(IDC_MSGSTYLESET_ICONSIZE_EDIT);
		if (iconSizeEditPtr_ == NULL) {
			TRACE_ERROR("Error: Icon size edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (iconPositionOnTopRadPtr_ == NULL) {
		iconPositionOnTopRadPtr_ = (CButton*)GetDlgItem(IDC_MSGSTYLESET_ICONPOS_TOP_RAD);
		if (iconPositionOnTopRadPtr_ == NULL) {
			TRACE_ERROR("Error: 'Icon on Top' radio button not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (iconPositionOnLeftRadPtr_ == NULL) {
		iconPositionOnLeftRadPtr_ = (CButton*)GetDlgItem(IDC_MSGSTYLESET_ICONPOS_LEFT_RAD);
		if (iconPositionOnLeftRadPtr_ == NULL) {
			TRACE_ERROR("Error: 'Icon on Left' radio button not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (iconPreviewStaticPtr_ == NULL) {
		iconPreviewStaticPtr_ = (CStatic*)GetDlgItem(IDC_MSGSTYLESET_MSGICON_VIEW);
		if (iconPreviewStaticPtr_ == NULL) {
			TRACE_ERROR("Error: Icon preview control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (timeoutEditPtr_ == NULL) {
		timeoutEditPtr_ = (CEdit*)GetDlgItem(IDC_MSGSTYLESET_TIMEOUT_EDIT);
		if (timeoutEditPtr_ == NULL) {
			TRACE_ERROR("Error: Timeout edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (timeoutSpinPtr_ == NULL) {
		timeoutSpinPtr_ = (CSpinButtonCtrl*)GetDlgItem(IDC_MSGSTYLESET_TIMEOUT_SPIN);
		if (timeoutSpinPtr_ == NULL) {
			TRACE_ERROR("Error: Timeout spin control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (displayPosComboPtr_ == NULL) {
		displayPosComboPtr_ = (CComboBox*)GetDlgItem(IDC_MSGSTYLESET_DISPLAYPOS_COMBO);
		if (displayPosComboPtr_ == NULL) {
			TRACE_ERROR("Error: Display position picker not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (horizontalMarginEditPtr_ == NULL) {
		horizontalMarginEditPtr_ = (CEdit*)GetDlgItem(IDC_MSGSTYLESET_HMARGIN_EDIT);
		if (horizontalMarginEditPtr_ == NULL) {
			TRACE_ERROR("Error: H-Margin edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (verticalMarginEditPtr_ == NULL) {
		verticalMarginEditPtr_ = (CEdit*)GetDlgItem(IDC_MSGSTYLESET_VMARGIN_EDIT);
		if (verticalMarginEditPtr_ == NULL) {
			TRACE_ERROR("Error: V-Margin edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Setup timeout spin-edit
	if (timeoutSpinPtr_ != NULL) {

		// Set maximum length
		if (timeoutEditPtr_ == NULL) return;
		timeoutEditPtr_->SetLimitText(maxTimeoutDigits);

		// Set buddy: Timeout edit control
		timeoutSpinPtr_->SetBuddy(timeoutEditPtr_);

		// Set spin edit value
		timeoutSpinPtr_->SetRange(RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut);
		timeoutSpinPtr_->SetPos(RmdMsgStyleSet::defaultTimeout);
	}

	// Setup font size picker
	if (fontSizePickComboPtr_ != NULL) {
		HWND fontSizeEditHandle = ::GetWindow(fontSizePickComboPtr_->GetSafeHwnd(), GW_CHILD);
		SetWindowLongPtr(fontSizeEditHandle, GWL_STYLE, GetWindowLongPtr(fontSizeEditHandle, GWL_STYLE) | ES_CENTER);
		fontSizePickComboPtr_->SendMessage(CB_LIMITTEXT, (WPARAM)maxFontSizeDigits, 0);
	}

	// Set maximum length for edit-boxes
	if (iconSizeEditPtr_ != NULL)
		iconSizeEditPtr_->SetLimitText(maxIconSizeDigits);
	if (horizontalMarginEditPtr_ != NULL)
		horizontalMarginEditPtr_->SetLimitText(maxMarginValDigits);
	if (verticalMarginEditPtr_ != NULL)
		verticalMarginEditPtr_->SetLimitText(maxMarginValDigits);

	// Default
	SDialog::setupDialogItemState();
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	isRecheckState - Recheck all item's state
 * @return	None
 */
void CRmdMsgStyleSetDlg::refreshDialogItemState(bool isRecheckState /* = false */)
{
	// Redraw background/text color picker
	if (backgroundColorPickBtnPtr_ != NULL)
		backgroundColorPickBtnPtr_->RedrawWindow();
	if (textColorPickBtnPtr_ != NULL)
		textColorPickBtnPtr_->RedrawWindow();

	// Redraw icon preview
	if (iconPreviewStaticPtr_ != NULL)
		::InvalidateRect(iconPreviewStaticPtr_->GetSafeHwnd(), NULL, true);

	// Default
	SDialog::refreshDialogItemState(isRecheckState);
}

/**
 * @brief	Refresh and redraw the icon preview static control
 * @param	drawItemStructPtr - Item drawing data
 * @return	None
 */
void CRmdMsgStyleSetDlg::redrawIconPreview(LPDRAWITEMSTRUCT drawItemStructPtr)
{
	// Validation
	if (drawItemStructPtr == NULL) return;

	// Device context handle
	HDC hDC = drawItemStructPtr->hDC;

	// Icon ID
	int iconId = RmdMsgStyleSet::defaultIconID;
	if (iconIdPickComboPtr_ != NULL) {
		int currenSelection = iconIdPickComboPtr_->GetCurSel();
		iconId = IDTable::SystemIcon[currenSelection].first;
	}

	// Icon size
	int iconSize = RmdMsgStyleSet::defaultIconSize;
	if (iconSizeEditPtr_ != NULL) {
		iconSize = getEditValue(iconSizeEditPtr_->GetSafeHwnd(), maxIconSizeDigits);
		validateAndCorrect(iconSize, RmdMsgStyleSet::minIconSize, RmdMsgStyleSet::maxIconSize);
	}
	int cx = iconSize;
	int cy = iconSize;

	// Erase the icon preview control before redrawing
	RECT previewCtrlRect = drawItemStructPtr->rcItem;
	HBRUSH fillBrushHandle = GetSysColorBrush(COLOR_3DFACE);
	FillRect(hDC, &previewCtrlRect, fillBrushHandle);

	// Draw area rect
	Rect drawAreaRect;
	drawAreaRect._top = previewCtrlRect.top;
	drawAreaRect._left = previewCtrlRect.left;
	drawAreaRect._bottom = previewCtrlRect.bottom;
	drawAreaRect._right = previewCtrlRect.right;

	// Icon position
	Point iconDrawPosition = drawAreaRect.topLeft();
	Vector2D drawAreaCenter = drawAreaRect.center();
	iconDrawPosition._x = drawAreaCenter.getX() - cx / 2;
	iconDrawPosition._y = drawAreaCenter.getY() - cy / 2;

	// Load system icon by ID and scale size
	HICON hIcon = NULL;
	HRESULT result = ::LoadIconWithScaleDown(NULL, MAKEINTRESOURCE(iconId), cx, cy, &hIcon);

	// Load icon failed
	if ((result != S_OK) || (hIcon == NULL)) {
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
 * @param	editCtrlHandle	- Edit control handle
 * @param	maxDigits	- Maximum number of digits
 * @return	int
 */
int CRmdMsgStyleSetDlg::getEditValue(HWND editCtrlHandle, unsigned maxDigits)
{
	// Argument validation
	if (editCtrlHandle == NULL || maxDigits <= 0)
		return false;

	// Get the value
	const size_t maxBuffSize = maxDigits + 1;
	std::vector<wchar_t> tempBuff(maxBuffSize);
	::GetWindowText(editCtrlHandle, tempBuff.data(), static_cast<int>(maxBuffSize));
	return _wtoi(tempBuff.data());
}

/**
 * @brief	Validate, auto-correct and display value out of bounds warning for edit controls
 * @param	editCtrlHandle	- Edit control handle
 * @param	maxDigits	- Maximum number of digits
 * @param	minVal		- Minimium value
 * @param	maxVal		- Maximum value
 * @param	showWarning - Whether to show warning
 * @return	true/false
 */
bool CRmdMsgStyleSetDlg::validateEditValue(HWND editCtrlHandle, int& inputVal, int minVal, int maxVal, bool showWarning /* = true */)
{
	// Validation & auto-correction
	if (validateAndCorrect(inputVal, minVal, maxVal)) {
		if (editCtrlHandle != NULL)
			::SendMessage(editCtrlHandle, EM_HIDEBALLOONTIP, 0, 0);
		return true;
	}
	else {
		// Show warning
		if (editCtrlHandle != NULL) {
			if (showWarning)
			{
				EDITBALLOONTIP editBalloonTip = { sizeof(EDITBALLOONTIP) };
				editBalloonTip.pszTitle = balloonTitleInvalidValue;
				editBalloonTip.pszText = StringUtils::stringFormat(balloonFormatInvalidValue, minVal, maxVal);
				editBalloonTip.ttiIcon = TTI_WARNING;

				::SendMessage(editCtrlHandle, EM_SHOWBALLOONTIP, 0, reinterpret_cast<LPARAM>(&editBalloonTip));
			}
			else {
				::SendMessage(editCtrlHandle, EM_HIDEBALLOONTIP, 0, 0);
			}
		}

		return false;
	}
}

/**
 * @brief	Update message style data from/to dialog controls
 * @param	updateFlag - Update data flag
 * @return	None
 */
void CRmdMsgStyleSetDlg::updateDialogData(bool updateFlag)
{
	if (updateFlag == true) {

		/***************************************************************/
		/*															   */
		/*				Update data from dialog controls			   */
		/*															   */
		/***************************************************************/

		int currenSelection = 0;

		// Background color
		tempMessageStyleData_.setBkgrdColor(messageBackgroundColor_);

		// Text color
		tempMessageStyleData_.setTextColor(messageTextColor_);

		// Font name
		String fontName = RmdMsgStyleSet::defaultFontName;
		if (fontNamePickComboPtr_ != NULL) {
			wchar_t tempBuff[Constant::Max::StringLength];
			currenSelection = fontNamePickComboPtr_->GetCurSel();
			fontNamePickComboPtr_->GetLBText(currenSelection, tempBuff);
			fontName = tempBuff;
		}
		tempMessageStyleData_.setFontName(fontName);

		// Font size
		int fontSize = RmdMsgStyleSet::defaultFontSize;
		if (fontSizePickComboPtr_ != NULL) {
			HWND fontSizeEditHandle = ::GetWindow(fontSizePickComboPtr_->GetSafeHwnd(), GW_CHILD);
			fontSize = getEditValue(fontSizeEditHandle, maxFontSizeDigits);
			validateAndCorrect(fontSize, RmdMsgStyleSet::minFontSize, RmdMsgStyleSet::maxFontSize);
		}
		tempMessageStyleData_.setFontSize(fontSize);

		// Icon ID
		int iconId = RmdMsgStyleSet::defaultIconID;
		if (iconIdPickComboPtr_ != NULL) {
			currenSelection = iconIdPickComboPtr_->GetCurSel();
			iconId = IDTable::SystemIcon[currenSelection].first;
		}
		tempMessageStyleData_.setIconId(iconId);

		// Icon size
		int iconSize = RmdMsgStyleSet::defaultIconSize;
		if (iconSizeEditPtr_ != NULL) {
			iconSize = getEditValue(iconSizeEditPtr_->GetSafeHwnd(), maxIconSizeDigits);
			validateAndCorrect(iconSize, RmdMsgStyleSet::minIconSize, RmdMsgStyleSet::maxIconSize);
		}
		tempMessageStyleData_.setIconSize(iconSize);

		// Icon position
		int iconPosition = RmdMsgStyleSet::defaultIconPosition;
		if (iconPositionOnTopRadPtr_ != NULL && iconPositionOnTopRadPtr_->GetCheck())
			iconPosition = RmdMsgStyleSet::IconPosition::IconOnTheTop;
		else if (iconPositionOnLeftRadPtr_ != NULL && iconPositionOnLeftRadPtr_->GetCheck())
			iconPosition = RmdMsgStyleSet::IconPosition::IconOnTheLeft;
		tempMessageStyleData_.setIconPosition(iconPosition);

		// Timeout
		int timeoutValue = RmdMsgStyleSet::defaultTimeout;
		if (timeoutSpinPtr_ != NULL) {
			timeoutValue = timeoutSpinPtr_->GetPos();
			validateAndCorrect(timeoutValue, RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut);
		}
		tempMessageStyleData_.setTimeout(timeoutValue);

		// Display position
		int displayPosition = RmdMsgStyleSet::defaultDisplayPosition;
		if (displayPosComboPtr_ != NULL) {
			currenSelection = displayPosComboPtr_->GetCurSel();
			displayPosition = IDTable::DisplayPosition[currenSelection].first;
		}
		tempMessageStyleData_.setDisplayPosition(displayPosition);

		// Horizontal margin
		int marginHorizontal = RmdMsgStyleSet::defaultHorizontalMargin;
		if (horizontalMarginEditPtr_ != NULL) {
			marginHorizontal = getEditValue(horizontalMarginEditPtr_->GetSafeHwnd(), maxMarginValDigits);
			validateAndCorrect(iconSize, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal);
		}
		tempMessageStyleData_.setHorizontalMargin(marginHorizontal);

		// Vertical margin
		int marginVertical = RmdMsgStyleSet::defaultVerticalMargin;
		if (verticalMarginEditPtr_ != NULL) {
			marginVertical = getEditValue(verticalMarginEditPtr_->GetSafeHwnd(), maxMarginValDigits);
			validateAndCorrect(marginVertical, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal);
		}
		tempMessageStyleData_.setVerticalMargin(marginVertical);
	}
	else {

		/***************************************************************/
		/*															   */
		/*				  Bind data to dialog controls				   */
		/*															   */
		/***************************************************************/

		// Load app language package
		LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

		// Background color
		messageBackgroundColor_ = tempMessageStyleData_.getBkgrdColor();

		// Text color
		messageTextColor_ = tempMessageStyleData_.getTextColor();

		// Font name
		String fontName = tempMessageStyleData_.getFontName();
		if (fontNamePickComboPtr_ != NULL)
			fontNamePickComboPtr_->SelectString(-1, fontName);

		// Font size
		int fontSize = tempMessageStyleData_.getFontSize();
		validateAndCorrect(fontSize, RmdMsgStyleSet::minFontSize, RmdMsgStyleSet::maxFontSize);
		if (fontSizePickComboPtr_ != NULL) {
			String fontSizeStr = String::fromNumber(fontSize);
			fontSizePickComboPtr_->SelectString(-1, fontSizeStr);
		}

		// Icon ID
		int iconId = tempMessageStyleData_.getIconId();
		if (iconIdPickComboPtr_ != NULL) {
			String iconName = getLanguageString(languageTablePtr, getPairedID(IDTable::SystemIcon, iconId));
			if (IS_NOT_NULL_STRING(iconName))
				iconIdPickComboPtr_->SelectString(-1, iconName);
		}

		// Icon size
		int iconSize = tempMessageStyleData_.getIconSize();
		validateAndCorrect(iconSize, RmdMsgStyleSet::minIconSize, RmdMsgStyleSet::maxIconSize);
		if (iconSizeEditPtr_ != NULL) {
			String iconSizeStr = String::fromNumber(iconSize);
			iconSizeEditPtr_->SetWindowText(iconSizeStr);
		}

		// Icon position
		int iconPosition = tempMessageStyleData_.getIconPosition();
		if (iconPositionOnTopRadPtr_ != NULL)
			iconPositionOnTopRadPtr_->SetCheck(iconPosition == RmdMsgStyleSet::IconPosition::IconOnTheTop);
		if (iconPositionOnLeftRadPtr_ != NULL)
			iconPositionOnLeftRadPtr_->SetCheck(iconPosition == RmdMsgStyleSet::IconPosition::IconOnTheLeft);

		// Timeout
		int timeoutValue = tempMessageStyleData_.getTimeout();
		validateAndCorrect(timeoutValue, RmdMsgStyleSet::minTimeOut, RmdMsgStyleSet::maxTimeOut);
		if (timeoutEditPtr_ != NULL) {
			String timeOutValStr = String::fromNumber(timeoutValue);
			timeoutEditPtr_->SetWindowText(timeOutValStr);
		}
		if (timeoutSpinPtr_ != NULL)
			timeoutSpinPtr_->SetPos(timeoutValue);

		// Display position
		int displayPosition = tempMessageStyleData_.getDisplayPosition();
		if (displayPosComboPtr_ != NULL) {
			String displayPosStr = getLanguageString(languageTablePtr, getPairedID(IDTable::DisplayPosition, displayPosition));
			if (IS_NOT_NULL_STRING(displayPosStr))
				displayPosComboPtr_->SelectString(-1, displayPosStr);
		}

		// Horizontal margin
		int marginHorizontal = tempMessageStyleData_.getHorizontalMargin();
		validateAndCorrect(marginHorizontal, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal);
		if (horizontalMarginEditPtr_ != NULL) {
			String marginValStr = String::fromNumber(marginHorizontal);
			horizontalMarginEditPtr_->SetWindowText(marginValStr);
		}

		// Vertical margin
		int marginVertical = tempMessageStyleData_.getVerticalMargin();
		validateAndCorrect(marginVertical, RmdMsgStyleSet::minMarginVal, RmdMsgStyleSet::maxMarginVal);
		if (verticalMarginEditPtr_ != NULL) {
			String marginValStr = String::fromNumber(marginVertical);
			verticalMarginEditPtr_->SetWindowText(marginValStr);
		}

		// Update data
		UpdateData(false);
		refreshDialogItemState();
	}
}
