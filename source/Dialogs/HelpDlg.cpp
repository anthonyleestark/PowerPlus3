/**
 * @file		HelpDlg.cpp
 * @brief		Source file for Help dialog
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "Dialogs/HelpDlg.h"
#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;


// Implement methods for CHelpDlg
IMPLEMENT_DYNAMIC(CHelpDlg, SDialog)


/**
 * @brief	Constructor
 */
CHelpDlg::CHelpDlg(CWnd* parentWnd /*=NULL*/)
	: SDialog(IDD_HELP_DLG, parentWnd)
{
	// Initialize member variables
	helpInfoString_ = Constant::String::Empty;
	viewMode_ = ViewMode::HelpFile;
}

/**
 * @brief	Destructor
 */
CHelpDlg::~CHelpDlg()
{
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CHelpDlg::registerDialogManagement(void)
{
	size_t returnValue = SDialog::registerDialogManagement();
	if (returnValue != 0) {
		TRACE_ERROR("Error: Register dialog management failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return returnValue;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		returnValue = pCtrlMan->addControl(IDC_HELPINFO_EDITBOX, Edit_Control);
		returnValue = pCtrlMan->addControl(IDC_HELP_CLOSE_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_HELP_SWITCHVIEWMODE_BTN, Button);
	}

	return returnValue;
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CHelpDlg::unregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->removeControl(IDC_HELPINFO_EDITBOX);
		pCtrlMan->removeControl(IDC_HELP_CLOSE_BTN);
		pCtrlMan->removeControl(IDC_HELP_SWITCHVIEWMODE_BTN);
	}

	return SDialog::unregisterDialogManagement();
}


// CHelpDlg dialog items ID map
BEGIN_RESOURCEID_MAP(CHelpDlg)
	ON_ID_DIALOG(IDD_HELP_DLG,					"HelpDlg")
	ON_ID_CONTROL(IDC_HELPINFO_EDITBOX,			"HelpInfoEditView")
	ON_ID_CONTROL(IDC_HELP_CLOSE_BTN,			"CloseButton")
	ON_ID_CONTROL(IDC_HELP_SWITCHVIEWMODE_BTN,	"SwitchViewModeButton")
END_RESOURCEID_MAP()


// CHelpDlg dialog message map
BEGIN_MESSAGE_MAP(CHelpDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_HELP_CLOSE_BTN,			&CHelpDlg::OnCloseButton)
	ON_BN_CLICKED(IDC_HELP_SWITCHVIEWMODE_BTN,	&CHelpDlg::OnSwitchViewMode)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// CHelpDlg message handlers

/**
 * @brief	Initialize Help dialog and setup dialog content
 * @param	None
 * @return	BOOL - Default
 */
BOOL CHelpDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Setup dialog interface
	setupLanguage();

	// Save dialog event log if enabled
	outputEventLog(LOG_EVENT_DLG_INIT, this->getCaption());

	return true;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void CHelpDlg::OnClose()
{
	// Close dialog
	SDialog::OnClose();
}

/**
 * @brief	Default method for dialog destroying
 * @param	None
 * @return	None
 */
void CHelpDlg::OnDestroy()
{
	// Save app event log if enabled
	outputEventLog(LOG_EVENT_DLG_DESTROYED, this->getCaption());

	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	Handle clicking event for [Close] button
 * @param	None
 * @return	None
 */
void CHelpDlg::OnCloseButton()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HELP_CLOSE_BTN);

	// Close dialog
	EndDialog(IDOK);
}

/**
 * @brief	Handle clicking event for [Switch View Mode] button
 * @param	None
 * @return	None
 */
void CHelpDlg::OnSwitchViewMode()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HELP_SWITCHVIEWMODE_BTN);

	// Switch view mode
	if (getViewMode() == ViewMode::HelpFile) {
		setViewMode(ViewMode::Changelog);
	}
	else if (getViewMode() == ViewMode::Changelog) {
		setViewMode(ViewMode::HelpFile);
	}

	// Reupdate file data
	loadResourceFileData(helpInfoString_);
	CEdit* editCtrlPtr = (CEdit*)GetDlgItem(IDC_HELPINFO_EDITBOX);
	if (editCtrlPtr != NULL)
		editCtrlPtr->SetWindowText(helpInfoString_);

	// Update switch view mode button
	updateSwitchViewModeButton();
}

/**
 * @brief	Setup language for dialog
 * @param	None
 * @return	None
 */
void CHelpDlg::setupLanguage()
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Setup dialog title
	this->setCaptionFromLanguage(getDialogId());

	// Set [Close] button title
	CWnd* windowPtr = GetDlgItem(IDC_HELP_CLOSE_BTN);
	if (windowPtr != NULL) {
		const wchar_t* wndText = getLanguageString(languageTablePtr, IDC_HELP_CLOSE_BTN);
		windowPtr->SetWindowText(wndText);
	}

	// Set [Switch view mode] button title
	updateSwitchViewModeButton();

	// Load help contents by default
	loadResourceFileData(helpInfoString_);
	windowPtr = GetDlgItem(IDC_HELPINFO_EDITBOX);
	if (windowPtr != NULL) {
		windowPtr->SetWindowText(helpInfoString_);
	}

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Setup help info box
 * @param	editCtrlPtr - Pointer of edit control
 * @return	None
 */
void CHelpDlg::setupEditbox(CEdit& editCtrlPtr)
{
	// Setup editbox property: UPPERCASE
	SetWindowLong(editCtrlPtr.m_hWnd, IDC_HELPINFO_EDITBOX, ES_UPPERCASE);
	editCtrlPtr.UpdateWindow();
}

/**
 * @brief	Load help content from Resource-embeded text data
 * @param	resourceFileDataString - File data result (ref-value)
 * @return	bool - Result of file loading process
 */
bool CHelpDlg::loadResourceFileData(String& resourceFileDataString)
{
	// Get currently displaying language
	unsigned currentLanguage = ((CPowerPlusApp*)AfxGetApp())->getAppLanguageOption(true);

	// Remove existing data
	resourceFileDataString.empty();

	// View help file mode
	if (getViewMode() == ViewMode::HelpFile) {
		switch (currentLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			resourceFileDataString = StringUtils::loadResourceTextData(IDR_FILE_HELP_ENG);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			resourceFileDataString = StringUtils::loadResourceTextData(IDR_FILE_HELP_VIE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			resourceFileDataString = StringUtils::loadResourceTextData(IDR_FILE_HELP_CHS);
			break;
		}
	}
	//View changelog mode
	else if (getViewMode() == ViewMode::Changelog) {
		switch (currentLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			resourceFileDataString = StringUtils::loadResourceTextData(IDR_FILE_CHANGELOG_ENG);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			resourceFileDataString = StringUtils::loadResourceTextData(IDR_FILE_CHANGELOG_VIE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			resourceFileDataString = StringUtils::loadResourceTextData(IDR_FILE_CHANGELOG_CHS);
			break;
		}
	}

	// Load RCData failed
	if (resourceFileDataString.isEmpty()) {

		// Load app language package
		LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
		if (getViewMode() == ViewMode::HelpFile) {
			// Can not load help file
			resourceFileDataString = getLanguageString(languageTablePtr, ERROR_HELPDLG_NOHELPFILE);

			// Trace error
			TRACE_ERROR("Error: [RCData] Help file not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		}
		else if (getViewMode() == ViewMode::Changelog) {
			// Can not load change log file
			resourceFileDataString = getLanguageString(languageTablePtr, ERROR_HELPDLG_NOCHANGELOGFILE);

			// Trace error
			TRACE_ERROR("Error: [RCData] Changelog file not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		}
	}

	return (!resourceFileDataString.isEmpty());
}

/**
 * @brief	Update title for [Switch View Mode] button
 * @param	None
 * @return	None
 */
void CHelpDlg::updateSwitchViewModeButton(void)
{
	// Language string ID
	unsigned stringId = INT_NULL;
	switch (viewMode_)
	{
	case ViewMode::HelpFile:
		stringId = BTN_HELPDLG_VIEWMODE_CHANGELOG;
		break;
	case ViewMode::Changelog:
		stringId = BTN_HELPDLG_VIEWMODE_HELPFILE;
		break;

	default:
		return;
	}

	// Get app current language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
	const wchar_t* titleString = getLanguageString(languageTablePtr, stringId);

	// Reupdate button title
	CButton* buttonPtr = (CButton*)GetDlgItem(IDC_HELP_SWITCHVIEWMODE_BTN);
	if (buttonPtr != NULL)
		buttonPtr->SetWindowText(titleString);
}

