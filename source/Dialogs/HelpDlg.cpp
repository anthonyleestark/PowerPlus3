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
CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: SDialog(IDD_HELP_DLG, pParent)
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
int CHelpDlg::RegisterDialogManagement(void)
{
	size_t nRet = SDialog::RegisterDialogManagement();
	if (nRet != 0) {
		TRACE_ERROR("Error: Register dialog management failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return nRet;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		nRet = pCtrlMan->AddControl(IDC_HELPINFO_EDITBOX, Edit_Control);
		nRet = pCtrlMan->AddControl(IDC_HELP_CLOSE_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HELP_SWITCHVIEWMODE_BTN, Button);
	}

	return nRet;
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CHelpDlg::UnregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->RemoveControl(IDC_HELPINFO_EDITBOX);
		pCtrlMan->RemoveControl(IDC_HELP_CLOSE_BTN);
		pCtrlMan->RemoveControl(IDC_HELP_SWITCHVIEWMODE_BTN);
	}

	return SDialog::UnregisterDialogManagement();
}

//////////////////////////////////////////////////////////////////////////
//
//	CHelpDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCEID_MAP(CHelpDlg)
	ON_ID_DIALOG(IDD_HELP_DLG,					"HelpDlg")
	ON_ID_CONTROL(IDC_HELPINFO_EDITBOX,			"HelpInfoEditView")
	ON_ID_CONTROL(IDC_HELP_CLOSE_BTN,			"CloseButton")
	ON_ID_CONTROL(IDC_HELP_SWITCHVIEWMODE_BTN,	"SwitchViewModeButton")
END_RESOURCEID_MAP()


//////////////////////////////////////////////////////////////////////////
//
//	CHelpDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

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
	SetupLanguage();

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, this->GetCaption());

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
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, this->GetCaption());

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HELP_CLOSE_BTN);

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HELP_SWITCHVIEWMODE_BTN);

	// Switch view mode
	if (GetViewMode() == ViewMode::HelpFile) {
		SetViewMode(ViewMode::Changelog);
	}
	else if (GetViewMode() == ViewMode::Changelog) {
		SetViewMode(ViewMode::HelpFile);
	}

	// Reupdate file data
	LoadRCFileData(m_strFileData);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_HELPINFO_EDITBOX);
	if (pEdit != NULL)
		pEdit->SetWindowText(m_strFileData);

	// Update switch view mode button
	UpdateSwitchViewModeButton();
}

/**
 * @brief	Setup language for dialog
 * @param	None
 * @return	None
 */
void CHelpDlg::SetupLanguage()
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Setup dialog title
	this->SetCaptionFromLanguage(GetDialogID());

	// Set [Close] button title
	CWnd* pWnd = GetDlgItem(IDC_HELP_CLOSE_BTN);
	if (pWnd != NULL) {
		const wchar_t* wndText = GetLanguageString(pAppLang, IDC_HELP_CLOSE_BTN);
		pWnd->SetWindowText(wndText);
	}

	// Set [Switch view mode] button title
	UpdateSwitchViewModeButton();

	// Load help contents by default
	LoadRCFileData(m_strFileData);
	pWnd = GetDlgItem(IDC_HELPINFO_EDITBOX);
	if (pWnd != NULL) {
		pWnd->SetWindowText(m_strFileData);
	}

	// Default
	SDialog::SetupLanguage();
}

/**
 * @brief	Setup help info box
 * @param	pEdit - Pointer of edit control
 * @return	None
 */
void CHelpDlg::SetupEditbox(CEdit& pEdit)
{
	// Setup editbox property: UPPERCASE
	SetWindowLong(pEdit.m_hWnd, IDC_HELPINFO_EDITBOX, ES_UPPERCASE);
	pEdit.UpdateWindow();
}

/**
 * @brief	Load help content from Resource-embeded text data
 * @param	strRCFileData - File data result (ref-value)
 * @return	bool - Result of file loading process
 */
bool CHelpDlg::LoadRCFileData(String& strRCFileData)
{
	// Get currently displaying language
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	unsigned nCurLanguage = pApp->GetAppLanguageOption(true);

	// Remove existing data
	if (!strRCFileData.IsEmpty())
		strRCFileData.Empty();

	// View help file mode
	if (GetViewMode() == ViewMode::HelpFile) {
		switch (nCurLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			strRCFileData = StringUtils::LoadResourceTextData(IDR_FILE_HELP_ENG);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			strRCFileData = StringUtils::LoadResourceTextData(IDR_FILE_HELP_VIE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			strRCFileData = StringUtils::LoadResourceTextData(IDR_FILE_HELP_CHS);
			break;
		}
	}
	//View changelog mode
	else if (GetViewMode() == ViewMode::Changelog) {
		switch (nCurLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			strRCFileData = StringUtils::LoadResourceTextData(IDR_FILE_CHANGELOG_ENG);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			strRCFileData = StringUtils::LoadResourceTextData(IDR_FILE_CHANGELOG_VIE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			strRCFileData = StringUtils::LoadResourceTextData(IDR_FILE_CHANGELOG_CHS);
			break;
		}
	}

	// Load RCData failed
	if (strRCFileData.IsEmpty()) {

		// Load app language package
		LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		if (GetViewMode() == ViewMode::HelpFile) {
			// Can not load help file
			strRCFileData = GetLanguageString(pAppLang, ERROR_HELPDLG_NOHELPFILE);

			// Trace error
			TRACE_ERROR("Error: [RCData] Help file not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		}
		else if (GetViewMode() == ViewMode::Changelog) {
			// Can not load change log file
			strRCFileData = GetLanguageString(pAppLang, ERROR_HELPDLG_NOCHANGELOGFILE);

			// Trace error
			TRACE_ERROR("Error: [RCData] Changelog file not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		}
	}

	return (!strRCFileData.IsEmpty());
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
	LANGTABLE_PTR ptrLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
	const wchar_t* titleString = GetLanguageString(ptrLang, nStringID);

	// Reupdate button title
	CButton* pBtn = (CButton*)GetDlgItem(IDC_HELP_SWITCHVIEWMODE_BTN);
	if (pBtn != NULL)
		pBtn->SetWindowText(titleString);
}

