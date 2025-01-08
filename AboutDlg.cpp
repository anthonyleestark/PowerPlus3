
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		AboutDlg.cpp
//		Description:	Source file for About and Help dialogs
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AboutDlg.h"
#include "PowerPlus.h"
#include "PowerPlusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace PairFuncs;
using namespace CoreFuncs;


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CAboutDlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CAboutDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CAboutDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CAboutDlg::CAboutDlg() : SDialog(IDD_ABOUT_DLG)
{
	INIT_CLASS_IDMAP()
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CAboutDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CAboutDlg::~CAboutDlg()
{
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegisterDialogManagement
//	Description:	Register dialog control management
//  Arguments:		None
//  Return value:	INT_PTR - Number of controls added to management
//
//////////////////////////////////////////////////////////////////////////

INT_PTR CAboutDlg::RegisterDialogManagement(void)
{
	INT_PTR nRet = SDialog::RegisterDialogManagement();
	if (nRet != 0) {
		TRCLOG("Error: Register dialog management failed!!!");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		return nRet;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		nRet = pCtrlMan->AddControl(IDC_APPNAME_LABEL, CTRL_TYPE_STATIC);
		nRet = pCtrlMan->AddControl(IDC_COPYRIGHT_LABEL, CTRL_TYPE_STATIC);
		nRet = pCtrlMan->AddControl(IDC_DEVPROFILE_LINK, CTRL_TYPE_SYSLINKCTRL);
		nRet = pCtrlMan->AddControl(IDC_AUTH_LABEL, CTRL_TYPE_STATIC);
		nRet = pCtrlMan->AddControl(IDC_APPINFO_LABEL, CTRL_TYPE_STATIC);
		nRet = pCtrlMan->AddControl(IDC_ABOUT_CLOSE_BTN, CTRL_TYPE_BUTTON);
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

BOOL CAboutDlg::UnregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->RemoveControl(IDC_APPNAME_LABEL);
		pCtrlMan->RemoveControl(IDC_COPYRIGHT_LABEL);
		pCtrlMan->RemoveControl(IDC_DEVPROFILE_LINK);
		pCtrlMan->RemoveControl(IDC_AUTH_LABEL);
		pCtrlMan->RemoveControl(IDC_APPINFO_LABEL);
		pCtrlMan->RemoveControl(IDC_ABOUT_CLOSE_BTN);
	}

	return SDialog::UnregisterDialogManagement();
}

//////////////////////////////////////////////////////////////////////////
//
//	CAboutDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_ID_MAPPING(CAboutDlg)
	IDMAP_ADD(IDD_ABOUT_DLG,		"AboutDlg")
	IDMAP_ADD(IDC_ABOUT_CLOSE_BTN,	"CloseButton")
	IDMAP_ADD(IDC_DEVPROFILE_LINK,	"DevProfileLink")
	IDMAP_ADD(IDC_APPNAME_LABEL,	"AppNameLabel")
	IDMAP_ADD(IDC_COPYRIGHT_LABEL,	"CopyrightLabel")
	IDMAP_ADD(IDC_AUTH_LABEL,		"AuthorLabel")
	IDMAP_ADD(IDC_APPINFO_LABEL,	"AuthorInfoLabel")
END_ID_MAPPING()

//////////////////////////////////////////////////////////////////////////
//
//	CAboutDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAboutDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ABOUT_CLOSE_BTN,		 &CAboutDlg::OnCloseButton)
	ON_NOTIFY(NM_CLICK, IDC_DEVPROFILE_LINK, &CAboutDlg::OnDevProfileLinkClick)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// CAboutDlg message handlers

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize About dialog and setup dialog content
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CAboutDlg::OnInitDialog()
{
	SDialog::OnInitDialog();

	SetupLanguage();

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, GetDialogCaption());

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

void CAboutDlg::OnClose()
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

void CAboutDlg::OnDestroy()
{
	// Save app event log if enabled
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, GetDialogCaption());

	// Destroy dialog
	SDialog::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog controls
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CAboutDlg::SetupLanguage(void)
{
	// Load app language packages
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Setup dialog title
	this->SetLangDialogCaption(GetDialogID());

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		UINT nID = pWndChild->GetDlgCtrlID();
		switch (nID)
		{
		case IDI_ICON_DEFAULT:
		case IDC_STATIC:
			// Skip these items
			continue;
			break;
		case IDC_APPNAME_LABEL:
			SetAppNameLabel();
			break;
		case IDC_APPINFO_LABEL:
			SetAppInfoLabel();
			break;
		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppNameLabel
//	Description:	Set application name label info
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CAboutDlg::SetAppNameLabel(void)
{
	// Get app name label static
	CWnd* pAppNameLabel = GetDlgItem(IDC_APPNAME_LABEL);
	if (pAppNameLabel == NULL) return;

	// Get product version info
	CString strFullProductVersion;
	CString strShortProductVersion;
	if (!GetProductVersion(strFullProductVersion, strShortProductVersion))
		return;

	// Get app name label format from app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
	CString strFormat = GetLanguageString(pAppLang, IDC_APPNAME_LABEL);
	if (strFormat == DEF_STRING_NULL) return;

	// Set app name label
	CString strAppNameLabel;
	strAppNameLabel.Format(strFormat, strShortProductVersion, strFullProductVersion);
	pAppNameLabel->SetWindowText(strAppNameLabel);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppInfoLabel
//	Description:	Set application info label
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CAboutDlg::SetAppInfoLabel(void)
{
	// Get app info label static
	CWnd* pAppInfoLabel = GetDlgItem(IDC_APPINFO_LABEL);
	if (pAppInfoLabel == NULL) return;

	// Get product version info (short number)
	CString strProductVersion = GetProductVersion(FALSE);
	if (strProductVersion.IsEmpty()) return;

	// Get app info label format from app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
	CString strFormat = GetLanguageString(pAppLang, IDC_APPINFO_LABEL);
	if (strFormat == DEF_STRING_NULL) return;

	// Set app info label
	CString strAppInfoLabel;
	strAppInfoLabel.Format(strFormat, strProductVersion);
	pAppInfoLabel->SetWindowText(strAppInfoLabel);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCloseButton
//	Description:	Handle clicking event for [OK/Close] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnCloseButton()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_ABOUT_CLOSE_BTN);

	// Close dialog
	EndDialog(IDOK);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDevProfileLinkClick
//	Description:	Handle click event for DevProfile link
//  Arguments:		pNMHDR  - Default of link clicked event handler
//					pResult - Default of link clicked event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnDevProfileLinkClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_DEVPROFILE_LINK);

	// Open profile link
	OpenWebURL(_T("https://facebook.com/anthonyleestark"));

	// Return result
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CHelpDlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CHelpDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CHelpDlg
//	Description:	Constructor of CHelpDlg
//
//////////////////////////////////////////////////////////////////////////

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: SDialog(IDD_HELP_DLG, pParent)
{
	// Initialize member variables
	m_strFileData = DEF_STRING_EMPTY; 
	m_nViewMode = DEF_MODE_HELPVIEW_HELPFILE;
	
	INIT_CLASS_IDMAP()
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CHelpDlg
//	Description:	Destructor of CHelpDlg
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
//  Return value:	INT_PTR - Number of controls added to management
//
//////////////////////////////////////////////////////////////////////////

INT_PTR CHelpDlg::RegisterDialogManagement(void)
{
	INT_PTR nRet = SDialog::RegisterDialogManagement();
	if (nRet != 0) {
		TRCLOG("Error: Register dialog management failed!!!");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		return nRet;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		nRet = pCtrlMan->AddControl(IDC_HELPINFO_EDITBOX, CTRL_TYPE_EDITBOX);
		nRet = pCtrlMan->AddControl(IDC_HELP_CLOSE_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_HELP_SWITCHVIEWMODE_BTN, CTRL_TYPE_BUTTON);
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

BEGIN_ID_MAPPING(CHelpDlg)
	IDMAP_ADD(IDD_HELP_DLG,					"HelpDlg")
	IDMAP_ADD(IDC_HELPINFO_EDITBOX,			"HelpInfoEditView")
	IDMAP_ADD(IDC_HELP_CLOSE_BTN,			"CloseButton")
	IDMAP_ADD(IDC_HELP_SWITCHVIEWMODE_BTN,	"SwitchViewModeButton")
END_ID_MAPPING()


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
	SDialog::OnInitDialog();

	SetupLanguage();

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, GetDialogCaption());

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
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, GetDialogCaption());

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
	if (GetViewMode() == DEF_MODE_HELPVIEW_HELPFILE) {
		SetViewMode(DEF_MODE_HELPVIEW_CHANGELOG);
	}
	else if (GetViewMode() == DEF_MODE_HELPVIEW_CHANGELOG) {
		SetViewMode(DEF_MODE_HELPVIEW_HELPFILE);
	}

	// Reupdate file data
	LoadFileData(m_strFileData);
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
	this->SetLangDialogCaption(GetDialogID());

	// Set [Close] button title
	CWnd* pWnd = GetDlgItem(IDC_HELP_CLOSE_BTN);
	if (pWnd != NULL) {
		CString strWndText = GetLanguageString(pAppLang, IDC_HELP_CLOSE_BTN);
		pWnd->SetWindowText(strWndText);
	}

	// Set [Switch view mode] button title
	UpdateSwitchViewModeButton();

	// Load help contents by default
	LoadFileData(m_strFileData);
	pWnd = GetDlgItem(IDC_HELPINFO_EDITBOX);
	if (pWnd != NULL) {
		pWnd->SetWindowText(m_strFileData);
	}
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
//	Function name:	LoadHelpInfo
//	Description:	Load help contents
//  Arguments:		strFileData - File data result (ref-value)
//  Return value:	BOOL - Result of file loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CHelpDlg::LoadFileData(CString& strFileData)
{
	// Get currently displaying language
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	UINT nCurLanguage = pApp->GetAppLanguageOption(TRUE);

	// Get help file name
	CString strHelpFileName;

	// View help file mode
	if (GetViewMode() == DEF_MODE_HELPVIEW_HELPFILE) {
		switch (nCurLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			MakeFilePath(strHelpFileName, SUBFOLDER_HELP, FILENAME_HELP_ENG, FILEEXT_HELPFILE);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			MakeFilePath(strHelpFileName, SUBFOLDER_HELP, FILENAME_HELP_VIE, FILEEXT_HELPFILE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			MakeFilePath(strHelpFileName, SUBFOLDER_HELP, FILENAME_HELP_CHS, FILEEXT_HELPFILE);
			break;
		}
	}
	//View changelog mode
	else if (GetViewMode() == DEF_MODE_HELPVIEW_CHANGELOG) {
		switch (nCurLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			MakeFilePath(strHelpFileName, SUBFOLDER_HELP, FILENAME_CHANGELOG_ENG, FILEEXT_HELPFILE);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			MakeFilePath(strHelpFileName, SUBFOLDER_HELP, FILENAME_CHANGELOG_VIE, FILEEXT_HELPFILE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			MakeFilePath(strHelpFileName, SUBFOLDER_HELP, FILENAME_CHANGELOG_CHS, FILEEXT_HELPFILE);
			break;
		}
	}

	BOOL bRet = FALSE;
	CFile fHelpFile;
	CFileStatus fsStatus;

	// Remove existing data
	if (!strFileData.IsEmpty())
		strFileData.Empty();

	// Check if file exists
	bRet = CFile::GetStatus(strHelpFileName, fsStatus);
	if (bRet == TRUE)
	{
		// Open file
		bRet = fHelpFile.Open(strHelpFileName, CFile::modeRead | CFile::typeText | CFile::typeUnicode);
		if (bRet == TRUE)
		{
			// Read file buffer
			fHelpFile.SeekToBegin();
			UINT nFileLength = (UINT)fHelpFile.GetLength();
			int nCharNum = nFileLength / sizeof(TCHAR);
			nFileLength = fHelpFile.Read(strFileData.GetBuffer(nCharNum), nFileLength);
			strFileData.ReleaseBuffer(nCharNum);
		}
	}

	if (bRet == FALSE)
	{
		// Load app language package
		LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		if (GetViewMode() == DEF_MODE_HELPVIEW_HELPFILE) {
			strFileData = GetLanguageString(pAppLang, ERROR_HELPDLG_NOHELPFILE);
			TRCLOG("Error: Help file not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		}
		else if(GetViewMode() == DEF_MODE_HELPVIEW_CHANGELOG) {
			strFileData = GetLanguageString(pAppLang, ERROR_HELPDLG_NOCHANGELOGFILE);
			TRCLOG("Error: Changelog file not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		}
	}

	if (fHelpFile.m_hFile != CFile::hFileNull)
		fHelpFile.Close();

	return bRet;
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
	UINT nStringID = DEF_INTEGER_INVALID;
	switch (m_nViewMode)
	{
	case DEF_MODE_HELPVIEW_HELPFILE:
		nStringID = BTN_HELPDLG_VIEWMODE_CHANGELOG;
		break;
	case DEF_MODE_HELPVIEW_CHANGELOG:
		nStringID = BTN_HELPDLG_VIEWMODE_HELPFILE;
		break;
	default:
		return;
		break;
	}

	// Get app current language package
	LANGTABLE_PTR ptrLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
	CString strTitle = GetLanguageString(ptrLang, nStringID);

	// Reupdate button title
	CButton* pBtn = (CButton*)GetDlgItem(IDC_HELP_SWITCHVIEWMODE_BTN);
	if (pBtn != NULL)
		pBtn->SetWindowText(strTitle);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetViewMode
//	Description:	Get/set view mode
//  Arguments:		nViewMode - View mode
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

UINT CHelpDlg::GetViewMode()
{
	return m_nViewMode;
}

void CHelpDlg::SetViewMode(UINT nViewMode)
{
	m_nViewMode = nViewMode;
}

