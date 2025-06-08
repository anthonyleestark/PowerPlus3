
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		AboutDlg.cpp
//		Description:	Source file for About and Help dialogs
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//						<3> 2024.07.06:		Update to version 3.1
//						<4> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Dialogs/AboutDlg.h"
#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;


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
//  Return value:	int
//
//////////////////////////////////////////////////////////////////////////

int CAboutDlg::RegisterDialogManagement(void)
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
		nRet = pCtrlMan->AddControl(IDC_APPNAME_LABEL, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_COPYRIGHT_LABEL, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_VIEW_FACEBOOK_PROFILE_LINK, SysLink_Control);
		nRet = pCtrlMan->AddControl(IDC_AUTH_LABEL, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_APPINFO_LABEL, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_ABOUT_CLOSE_BTN, Button);
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
		pCtrlMan->RemoveControl(IDC_VIEW_FACEBOOK_PROFILE_LINK);
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

BEGIN_RESOURCEID_MAP(CAboutDlg)
	ON_ID_DIALOG(IDD_ABOUT_DLG,						"AboutDlg")
	ON_ID_CONTROL(IDC_ABOUT_CLOSE_BTN,				"CloseButton")
	ON_ID_CONTROL(IDC_VIEW_FACEBOOK_PROFILE_LINK,	"DevProfileLink")
	ON_ID_CONTROL(IDC_APPNAME_LABEL,				"AppNameLabel")
	ON_ID_CONTROL(IDC_COPYRIGHT_LABEL,				"CopyrightLabel")
	ON_ID_CONTROL(IDC_AUTH_LABEL,					"AuthorLabel")
	ON_ID_CONTROL(IDC_APPINFO_LABEL,				"AuthorInfoLabel")
END_RESOURCEID_MAP()

//////////////////////////////////////////////////////////////////////////
//
//	CAboutDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAboutDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ABOUT_CLOSE_BTN,					&CAboutDlg::OnCloseButton)
	ON_NOTIFY(NM_CLICK, IDC_VIEW_FACEBOOK_PROFILE_LINK, &CAboutDlg::OnViewFacebookProfileLink)
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
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, this->GetCaption());

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
	this->SetCaptionFromLanguage(GetDialogID());

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		UINT nID = pWndChild->GetDlgCtrlID();
		switch (nID)
		{
		case IDI_ICON_APP:
		case IDC_STATIC:
			// Skip these items
			continue;

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

	// Default
	SDialog::SetupLanguage();
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
	String fullProdVersion;
	String shortProdVersion;
	if (!StringUtils::GetProductVersion(fullProdVersion, shortProdVersion))
		return;

	// Get app name label format from app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
	const wchar_t* formatString = GetLanguageString(pAppLang, IDC_APPNAME_LABEL);
	if (IS_NULL_STRING(formatString)) return;

	// Set app name label
	String appNameLabel = StringUtils::StringFormat(formatString, shortProdVersion.GetString(), fullProdVersion.GetString());
	pAppNameLabel->SetWindowText(appNameLabel);
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
	String productVersion = StringUtils::GetProductVersion(false);
	if (productVersion.IsEmpty()) return;

	// Get app info label format from app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
	String strFormat = GetLanguageString(pAppLang, IDC_APPINFO_LABEL);
	if (IS_NULL_STRING(strFormat)) return;

	// Set app info label
	String appInfoLabel = StringUtils::StringFormat(strFormat, productVersion.GetString());
	pAppInfoLabel->SetWindowText(appInfoLabel);
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
//	Function name:	OnViewFacebookProfileLink
//	Description:	Handle click event for [View Facebook Profile] link
//  Arguments:		pNMHDR  - Default of link clicked event handler
//					pResult - Default of link clicked event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnViewFacebookProfileLink(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_VIEW_FACEBOOK_PROFILE_LINK);

	// Open profile link
	OpenWebURL(_T("https://facebook.com/anthonyleestark"));

	// Return result
	*pResult = 0;
}

