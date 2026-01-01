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
	m_strFileData = Constant::String::Empty;
	m_nViewMode = ViewMode::HelpFile;
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
		returnValue = pCtrlMan->AddControl(IDC_HELPINFO_EDITBOX, Edit_Control);
		returnValue = pCtrlMan->AddControl(IDC_HELP_CLOSE_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_HELP_SWITCHVIEWMODE_BTN, Button);
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
		pCtrlMan->RemoveControl(IDC_HELPINFO_EDITBOX);
		pCtrlMan->RemoveControl(IDC_HELP_CLOSE_BTN);
		pCtrlMan->RemoveControl(IDC_HELP_SWITCHVIEWMODE_BTN);
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
	if (GetViewMode() == ViewMode::HelpFile) {
		SetViewMode(ViewMode::Changelog);
	}
	else if (GetViewMode() == ViewMode::Changelog) {
		SetViewMode(ViewMode::HelpFile);
	}

	// Reupdate file data
	LoadRCFileData(m_strFileData);
	CEdit* editCtrlPtr = (CEdit*)GetDlgItem(IDC_HELPINFO_EDITBOX);
	if (editCtrlPtr != NULL)
		editCtrlPtr->SetWindowText(m_strFileData);

	// Update switch view mode button
	UpdateSwitchViewModeButton();
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
	UpdateSwitchViewModeButton();

	// Load help contents by default
	LoadRCFileData(m_strFileData);
	windowPtr = GetDlgItem(IDC_HELPINFO_EDITBOX);
	if (windowPtr != NULL) {
		windowPtr->SetWindowText(m_strFileData);
	}

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Setup help info box
 * @param	editCtrlPtr - Pointer of edit control
 * @return	None
 */
void CHelpDlg::SetupEditbox(CEdit& editCtrlPtr)
{
	// Setup editbox property: UPPERCASE
	SetWindowLong(editCtrlPtr.m_hWnd, IDC_HELPINFO_EDITBOX, ES_UPPERCASE);
	editCtrlPtr.UpdateWindow();
}

/**
 * @brief	Load help content from Resource-embeded text data
 * @param	strRCFileData - File data result (ref-value)
 * @return	bool - Result of file loading process
 */
bool CHelpDlg::LoadRCFileData(String& strRCFileData)
{
	// Get currently displaying language
	unsigned currentLanguage = ((CPowerPlusApp*)AfxGetApp())->getAppLanguageOption(true);

	// Remove existing data
	strRCFileData.empty();

	// View help file mode
	if (GetViewMode() == ViewMode::HelpFile) {
		switch (currentLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			strRCFileData = StringUtils::loadResourceTextData(IDR_FILE_HELP_ENG);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			strRCFileData = StringUtils::loadResourceTextData(IDR_FILE_HELP_VIE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			strRCFileData = StringUtils::loadResourceTextData(IDR_FILE_HELP_CHS);
			break;
		}
	}
	//View changelog mode
	else if (GetViewMode() == ViewMode::Changelog) {
		switch (currentLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			strRCFileData = StringUtils::loadResourceTextData(IDR_FILE_CHANGELOG_ENG);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			strRCFileData = StringUtils::loadResourceTextData(IDR_FILE_CHANGELOG_VIE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			strRCFileData = StringUtils::loadResourceTextData(IDR_FILE_CHANGELOG_CHS);
			break;
		}
	}

	// Load RCData failed
	if (strRCFileData.isEmpty()) {

		// Load app language package
		LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
		if (GetViewMode() == ViewMode::HelpFile) {
			// Can not load help file
			strRCFileData = getLanguageString(languageTablePtr, ERROR_HELPDLG_NOHELPFILE);

			// Trace error
			TRACE_ERROR("Error: [RCData] Help file not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		}
		else if (GetViewMode() == ViewMode::Changelog) {
			// Can not load change log file
			strRCFileData = getLanguageString(languageTablePtr, ERROR_HELPDLG_NOCHANGELOGFILE);

			// Trace error
			TRACE_ERROR("Error: [RCData] Changelog file not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		}
	}

	return (!strRCFileData.isEmpty());
}

/**
 * @brief	Update title for [Switch View Mode] button
 * @param	None
 * @return	None
 */
void CHelpDlg::UpdateSwitchViewModeButton(void)
{
	// Language string ID
	unsigned nStringID = INT_NULL;
	switch (m_nViewMode)
	{
	case ViewMode::HelpFile:
		nStringID = BTN_HELPDLG_VIEWMODE_CHANGELOG;
		break;
	case ViewMode::Changelog:
		nStringID = BTN_HELPDLG_VIEWMODE_HELPFILE;
		break;

	default:
		return;
	}

	// Get app current language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
	const wchar_t* titleString = getLanguageString(languageTablePtr, nStringID);

	// Reupdate button title
	CButton* pBtn = (CButton*)GetDlgItem(IDC_HELP_SWITCHVIEWMODE_BTN);
	if (pBtn != NULL)
		pBtn->SetWindowText(titleString);
}

