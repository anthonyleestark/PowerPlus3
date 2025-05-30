
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

#include "stdafx.h"

#include "Dialogs\HelpDlg.h"
#include "PowerPlus.h"
#include "PowerPlusDlg.h"

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
	m_strFileData = STRING_EMPTY;
	m_nViewMode = MODE_HELPVIEW_HELPFILE;
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
//  Return value:	INT_PTR - Number of controls added to management
//
//////////////////////////////////////////////////////////////////////////

INT_PTR CHelpDlg::RegisterDialogManagement(void)
{
	INT_PTR nRet = SDialog::RegisterDialogManagement();
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
	if (GetViewMode() == MODE_HELPVIEW_HELPFILE) {
		SetViewMode(MODE_HELPVIEW_CHANGELOG);
	}
	else if (GetViewMode() == MODE_HELPVIEW_CHANGELOG) {
		SetViewMode(MODE_HELPVIEW_HELPFILE);
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
		CString strWndText = GetLanguageString(pAppLang, IDC_HELP_CLOSE_BTN);
		pWnd->SetWindowText(strWndText);
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
//	Function name:	LoadFileData
//	Description:	Load help content from external file
//  Arguments:		strFileData - File data result (ref-value)
//  Return value:	BOOL - Result of file loading process
//  Note:			No longer used - Replaced with LoadRCFileData()
//
//////////////////////////////////////////////////////////////////////////

BOOL CHelpDlg::LoadFileData(CString& strFileData)
{
	// Get currently displaying language
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	UINT nCurLanguage = pApp->GetAppLanguageOption(TRUE);

	// Get help folder path
	CString strFolderPath = GetSubFolderPath(SUBFOLDER_HELP);

	// Get help file path
	CString strHelpFilePath;

	// View help file mode
	if (GetViewMode() == MODE_HELPVIEW_HELPFILE) {
		switch (nCurLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			MakeFilePath(strHelpFilePath, strFolderPath, FILENAME_HELP_ENG, FILEEXT_HELPFILE);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			MakeFilePath(strHelpFilePath, strFolderPath, FILENAME_HELP_VIE, FILEEXT_HELPFILE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			MakeFilePath(strHelpFilePath, strFolderPath, FILENAME_HELP_CHS, FILEEXT_HELPFILE);
			break;
		}
	}
	//View changelog mode
	else if (GetViewMode() == MODE_HELPVIEW_CHANGELOG) {
		switch (nCurLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			MakeFilePath(strHelpFilePath, strFolderPath, FILENAME_CHANGELOG_ENG, FILEEXT_HELPFILE);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			MakeFilePath(strHelpFilePath, strFolderPath, FILENAME_CHANGELOG_VIE, FILEEXT_HELPFILE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			MakeFilePath(strHelpFilePath, strFolderPath, FILENAME_CHANGELOG_CHS, FILEEXT_HELPFILE);
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
	bRet = CFile::GetStatus(strHelpFilePath, fsStatus);
	if (bRet == TRUE)
	{
		// Open file
		bRet = fHelpFile.Open(strHelpFilePath, CFile::modeRead | CFile::typeText | CFile::typeUnicode);
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
		if (GetViewMode() == MODE_HELPVIEW_HELPFILE) {
			// Can not load help file
			strFileData = GetLanguageString(pAppLang, ERROR_HELPDLG_NOHELPFILE);

			// Trace error
			TRACE_ERROR("Error: Help file not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		}
		else if (GetViewMode() == MODE_HELPVIEW_CHANGELOG) {
			// Can not load change log file
			strFileData = GetLanguageString(pAppLang, ERROR_HELPDLG_NOCHANGELOGFILE);

			// Trace error
			TRACE_ERROR("Error: Changelog file not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		}
	}

	if (fHelpFile.m_hFile != CFile::hFileNull)
		fHelpFile.Close();

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadRCFileData
//	Description:	Load help content from Resource-embeded text data
//  Arguments:		strRCFileData - File data result (ref-value)
//  Return value:	BOOL - Result of file loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CHelpDlg::LoadRCFileData(CString& strRCFileData)
{
	// Get currently displaying language
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	UINT nCurLanguage = pApp->GetAppLanguageOption(TRUE);

	BOOL bResLoadRCData = FALSE;

	// Remove existing data
	if (!strRCFileData.IsEmpty())
		strRCFileData.Empty();

	// View help file mode
	if (GetViewMode() == MODE_HELPVIEW_HELPFILE) {
		switch (nCurLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			bResLoadRCData = LoadResourceTextFile(strRCFileData, IDR_FILE_HELP_ENG);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			bResLoadRCData = LoadResourceTextFile(strRCFileData, IDR_FILE_HELP_VIE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			bResLoadRCData = LoadResourceTextFile(strRCFileData, IDR_FILE_HELP_CHS);
			break;
		}
	}
	//View changelog mode
	else if (GetViewMode() == MODE_HELPVIEW_CHANGELOG) {
		switch (nCurLanguage)
		{
		case APP_LANGUAGE_ENGLISH:
			bResLoadRCData = LoadResourceTextFile(strRCFileData, IDR_FILE_CHANGELOG_ENG);
			break;
		case APP_LANGUAGE_VIETNAMESE:
			bResLoadRCData = LoadResourceTextFile(strRCFileData, IDR_FILE_CHANGELOG_VIE);
			break;
		case APP_LANGUAGE_SIMPCHINESE:
			bResLoadRCData = LoadResourceTextFile(strRCFileData, IDR_FILE_CHANGELOG_CHS);
			break;
		}
	}

	// Load RCData failed
	if (bResLoadRCData != TRUE) {

		// Load app language package
		LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		if (GetViewMode() == MODE_HELPVIEW_HELPFILE) {
			// Can not load help file
			strRCFileData = GetLanguageString(pAppLang, ERROR_HELPDLG_NOHELPFILE);

			// Trace error
			TRACE_ERROR("Error: [RCData] Help file not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		}
		else if (GetViewMode() == MODE_HELPVIEW_CHANGELOG) {
			// Can not load change log file
			strRCFileData = GetLanguageString(pAppLang, ERROR_HELPDLG_NOCHANGELOGFILE);

			// Trace error
			TRACE_ERROR("Error: [RCData] Changelog file not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		}
	}

	return (bResLoadRCData && (!strRCFileData.IsEmpty()));
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
	UINT nStringID = INT_INVALID;
	switch (m_nViewMode)
	{
	case MODE_HELPVIEW_HELPFILE:
		nStringID = BTN_HELPDLG_VIEWMODE_CHANGELOG;
		break;
	case MODE_HELPVIEW_CHANGELOG:
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

