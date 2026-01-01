/**
 * @file		AboutDlg.cpp
 * @brief		Source file for About dialog
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "Dialogs/AboutDlg.h"
#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;


//	Implementations for CAboutDlg
IMPLEMENT_DYNAMIC(CAboutDlg, SDialog)


/**
 * @brief	Constructor
 */
CAboutDlg::CAboutDlg() : SDialog(IDD_ABOUT_DLG)
{
}

/**
 * @brief	Destructor
 */
CAboutDlg::~CAboutDlg()
{
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CAboutDlg::registerDialogManagement(void)
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
		returnValue = pCtrlMan->AddControl(IDC_APPNAME_LABEL, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_COPYRIGHT_LABEL, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_VIEW_FACEBOOK_PROFILE_LINK, SysLink_Control);
		returnValue = pCtrlMan->AddControl(IDC_AUTH_LABEL, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_APPINFO_LABEL, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_ABOUT_CLOSE_BTN, Button);
	}

	return returnValue;
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CAboutDlg::unregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->RemoveControl(IDC_APPNAME_LABEL);
		pCtrlMan->RemoveControl(IDC_COPYRIGHT_LABEL);
		pCtrlMan->RemoveControl(IDC_VIEW_FACEBOOK_PROFILE_LINK);
		pCtrlMan->RemoveControl(IDC_AUTH_LABEL);
		pCtrlMan->RemoveControl(IDC_APPINFO_LABEL);
		pCtrlMan->RemoveControl(IDC_ABOUT_CLOSE_BTN);
	}

	return SDialog::unregisterDialogManagement();
}


// CAboutDlg dialog items ID map
BEGIN_RESOURCEID_MAP(CAboutDlg)
	ON_ID_DIALOG(IDD_ABOUT_DLG,						"AboutDlg")
	ON_ID_CONTROL(IDC_ABOUT_CLOSE_BTN,				"CloseButton")
	ON_ID_CONTROL(IDC_VIEW_FACEBOOK_PROFILE_LINK,	"DevProfileLink")
	ON_ID_CONTROL(IDC_APPNAME_LABEL,				"AppNameLabel")
	ON_ID_CONTROL(IDC_COPYRIGHT_LABEL,				"CopyrightLabel")
	ON_ID_CONTROL(IDC_AUTH_LABEL,					"AuthorLabel")
	ON_ID_CONTROL(IDC_APPINFO_LABEL,				"AuthorInfoLabel")
END_RESOURCEID_MAP()


// CAboutDlg dialog message map
BEGIN_MESSAGE_MAP(CAboutDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ABOUT_CLOSE_BTN,					&CAboutDlg::OnCloseButton)
	ON_NOTIFY(NM_CLICK, IDC_VIEW_FACEBOOK_PROFILE_LINK, &CAboutDlg::OnViewFacebookProfileLink)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

/**
 * @brief	Initialize About dialog and setup dialog content
 * @param	None
 * @return	true/false - Default
 */
BOOL CAboutDlg::OnInitDialog()
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
void CAboutDlg::OnClose()
{
	// Close dialog
	SDialog::OnClose();
}

/**
 * @brief	Default method for dialog destroying
 * @param	None
 * @return	None
 */
void CAboutDlg::OnDestroy()
{
	// Save app event log if enabled
	outputEventLog(LOG_EVENT_DLG_DESTROYED, this->getCaption());

	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	Setup language for dialog controls
 * @param	None
 * @return	None
 */
void CAboutDlg::setupLanguage(void)
{
	// Load app language packages
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Setup dialog title
	this->setCaptionFromLanguage(getDialogId());

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		unsigned id = childWndPtr->GetDlgCtrlID();
		switch (id)
		{
		case IDI_ICON_APP:
		case IDC_STATIC:
			// Skip these items
			continue;

		case IDC_APPNAME_LABEL:
			setAppNameLabel();
			break;

		case IDC_APPINFO_LABEL:
			setAppInfoLabel();
			break;

		default:
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Set application name label info
 * @param	None
 * @return	None
 */
void CAboutDlg::setAppNameLabel(void)
{
	// Get app name label static
	CWnd* appNameLabelPtr = GetDlgItem(IDC_APPNAME_LABEL);
	if (appNameLabelPtr == NULL) return;

	// Get product version info
	String fullProdVersion;
	String shortProdVersion;
	if (!StringUtils::getProductVersion(fullProdVersion, shortProdVersion))
		return;

	// Get app name label format from app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
	const wchar_t* formatString = getLanguageString(languageTablePtr, IDC_APPNAME_LABEL);
	if (IS_NULL_STRING(formatString)) return;

	// Set app name label
	String appNameLabelString = StringUtils::stringFormat(formatString, shortProdVersion.getString(), fullProdVersion.getString());
	appNameLabelPtr->SetWindowText(appNameLabelString);
}

/**
 * @brief	Set application info label
 * @param	None
 * @return	None
 */
void CAboutDlg::setAppInfoLabel(void)
{
	// Get app info label static
	CWnd* appInfoLabelPtr = GetDlgItem(IDC_APPINFO_LABEL);
	if (appInfoLabelPtr == NULL) return;

	// Get product version info (short number)
	String productVersion = StringUtils::getProductVersion(false);
	if (productVersion.isEmpty()) return;

	// Get app info label format from app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
	String formatString = getLanguageString(languageTablePtr, IDC_APPINFO_LABEL);
	if (IS_NULL_STRING(formatString)) return;

	// Set app info label
	String appInfoLabelString = StringUtils::stringFormat(formatString, productVersion.getString());
	appInfoLabelPtr->SetWindowText(appInfoLabelString);
}

/**
 * @brief	Handle clicking event for [OK/Close] button
 * @param	None
 * @return	None
 */
void CAboutDlg::OnCloseButton()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_ABOUT_CLOSE_BTN);

	// Close dialog
	EndDialog(IDOK);
}

/**
 * @brief	Handle click event for [View Facebook Profile] link
 * @param	pNMHDR  - Default of link clicked event handler
 * @param	pResult - Default of link clicked event handler
 * @return	None
 */
void CAboutDlg::OnViewFacebookProfileLink(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_VIEW_FACEBOOK_PROFILE_LINK);

	// Open profile link
	AppCore::openWebURL(_T("https://facebook.com/anthonyleestark"));

	// Return result
	*pResult = 0;
}

