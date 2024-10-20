
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		LogViewerDlg.cpp
//		Description:	Source file for LogViewer dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.24:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PowerPlus.h"
#include "PowerPlusDlg.h"
#include "LogViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace PairFuncs;
using namespace CoreFuncs;

////////////////////////////////////////////////////////
//
//	Implement methods for CLogViewerDlg
//
////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CLogViewerDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CLogViewerDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CLogViewerDlg::CLogViewerDlg(CWnd* pParent /*=nullptr*/)
	: SDialog(IDD_LOGVIEWER_DLG, pParent)
{
	// Init dialog item variables
	m_pLogViewerList = NULL;

	INIT_CLASS_IDMAP()
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CLogViewerDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CLogViewerDlg::~CLogViewerDlg()
{
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


BEGIN_ID_MAPPING(CLogViewerDlg)
	IDMAP_ADD(IDD_LOGVIEWER_DLG,			 "LogViewerDlg")
	IDMAP_ADD(IDC_LOGVIEWER_LOGDATA_LISTBOX, "LogViewerList")
	IDMAP_ADD(IDC_LOGVIEWER_DETAILS_LABEL,	 "DetailLabel")
	IDMAP_ADD(IDC_LOGVIEWER_DATETIME_LABEL,	 "DateTimeLabel")
	IDMAP_ADD(IDC_LOGVIEWER_DATETIME_VALUE,	 "DateTimeValueDisplay")
	IDMAP_ADD(IDC_LOGVIEWER_CONTENT_LABEL,	 "ContentLabel")
	IDMAP_ADD(IDC_LOGVIEWER_CONTENT_EDITBOX, "ContentDisplay")
END_ID_MAPPING()

BEGIN_MESSAGE_MAP(CLogViewerDlg, SDialog)
	ON_WM_CLOSE()
	ON_LBN_SELCHANGE(IDC_LOGVIEWER_LOGDATA_LISTBOX, OnSelectLogItem)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// LogViewerDlg message handlers

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize Logviewer dialog and setup dialog content
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CLogViewerDlg::OnInitDialog()
{
	SDialog::OnInitDialog();

	// Save app event log if enabled
	OutputDialogLog(GetDialogID(), LOG_EVENT_DLG_STARTUP);

	SetupLanguage();
	UpdateLogViewer();
	DisplayLogDetails(0);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClose
//	Description:	Handle event when closing the dialog
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::OnClose()
{
	// Save app event log if enabled
	OutputDialogLog(GetDialogID(), LOG_EVENT_DLG_DESTROYED);

	SDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::SetupLanguage()
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Setup dialog title
	CString strWndText = GetLanguageString(pAppLang, GetDialogID());
	this->SetWindowText(strWndText);

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		UINT nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDC_LOGVIEWER_LOGDATA_LISTBOX:
		case IDC_LOGVIEWER_DATETIME_VALUE:
		case IDC_LOGVIEWER_CONTENT_EDITBOX:
			break;
		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateLogViewer
//	Description:	Update logviewer list
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::UpdateLogViewer()
{
	// Initialization
	if (m_pLogViewerList == NULL) {
		m_pLogViewerList = (CListBox*)GetDlgItem(IDC_LOGVIEWER_LOGDATA_LISTBOX);
	}
	if (m_pLogViewerList == NULL) {
		TRCLOG("Error: LogViewer list control not found");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return;
	}

	// Get log data item count
	int nLogCount = GetAppLogCount();

	for (int i = 0; i < nLogCount; i++) {
		// Get log item
		LOGITEM logItem = GetAppLogItem(i);

		// Format output log string
		CString strLogFormat = logItem.FormatLogString();

		// Add log item to logviewer list
		if (m_pLogViewerList != NULL) {
			m_pLogViewerList->AddString(strLogFormat);
		}
	}

	// Set default current selection
	if (m_pLogViewerList != NULL) {
		m_pLogViewerList->SetCurSel(0);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSelectLogItem
//	Description:	Show log details when selecting a log item
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::OnSelectLogItem()
{
	// Get listbox pointer
	if (m_pLogViewerList == NULL) {
		m_pLogViewerList = (CListBox*)GetDlgItem(IDC_LOGVIEWER_LOGDATA_LISTBOX);
	}
	if (m_pLogViewerList == NULL)
		return;

	// Get current selection and log data item count
	int nCurSel = m_pLogViewerList->GetCurSel();
	int nLogCount = GetAppLogCount();

	// Check selection index validity
	if (nCurSel < 0 || nCurSel > nLogCount)
	{
		m_pLogViewerList->SetCurSel(0);
		nCurSel = 0;
	}
	
	// Display log details
	DisplayLogDetails(nCurSel);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayLogDetails
//	Description:	Display details of log item at specified index
//  Arguments:		nIndex - Index of item to display
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CLogViewerDlg::DisplayLogDetails(int nIndex)
{
	LOGITEM logItem = GetAppLogItem(nIndex);

	CWnd* pWnd = GetDlgItem(IDC_LOGVIEWER_DATETIME_VALUE);
	if (pWnd != NULL)
		pWnd->SetWindowText(logItem.FormatDateTime());

	pWnd = GetDlgItem(IDC_LOGVIEWER_CONTENT_EDITBOX);
	if (pWnd != NULL)
		pWnd->SetWindowText(logItem.strLogString);
}
