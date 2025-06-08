
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		HelpDlg.cpp
//		Description:	Source file for Help dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2024.12.18:		Update to version 3.2
//						<2> 2025.03.06:		Split from AboutDlg.cpp
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Dialogs/HelpDlg.h"
#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CHelpDlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CHelpDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CHelpDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: SDialog(IDD_HELP_DLG, pParent)
{
	// Initialize member variables
	m_strFileData = Constant::String::Empty;
	m_nViewMode = ViewMode::HelpFile;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CHelpDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CHelpDlg::~CHelpDlg()
{
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegisterDialogManagement
//	Description:	Register dialog control management
//  Arguments:		None
//  Return value:	int
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UnregisterDialogManagement
//	Description:	Unregister dialog control management
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CHelpDlg::UnregisterDialogManagement(void)
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize Help dialog and setup dialog content
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CHelpDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Setup dialog interface
	SetupLanguage();

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, this->GetCaption());

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

void CHelpDlg::OnClose()
{
	// Close dialog
	SDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDestroy
//	Description:	Default method for dialog destroying
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHelpDlg::OnDestroy()
{
	// Save app event log if enabled
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, this->GetCaption());

	// Destroy dialog
	SDialog::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCloseButton
//	Description:	Handle clicking event for [Close] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHelpDlg::OnCloseButton()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HELP_CLOSE_BTN);

	// Close dialog
	EndDialog(IDOK);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSwitchViewMode
//	Description:	Handle clicking event for [Switch View Mode] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupEditbox
//	Description:	Setup help info box
//  Arguments:		pEdit - Pointer of edit control
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHelpDlg::SetupEditbox(CEdit& pEdit)
{
	// Setup editbox property: UPPERCASE
	SetWindowLong(pEdit.m_hWnd, IDC_HELPINFO_EDITBOX, ES_UPPERCASE);
	pEdit.UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadRCFileData
//	Description:	Load help content from Resource-embeded text data
//  Arguments:		strRCFileData - File data result (ref-value)
//  Return value:	BOOL - Result of file loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CHelpDlg::LoadRCFileData(String& strRCFileData)
{
	// Get currently displaying language
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	UINT nCurLanguage = pApp->GetAppLanguageOption(TRUE);

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateSwitchViewModeButton
//	Description:	Update title for [Switch View Mode] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHelpDlg::UpdateSwitchViewModeButton(void)
{
	// Language string ID
	UINT nStringID = INT_NULL;
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

