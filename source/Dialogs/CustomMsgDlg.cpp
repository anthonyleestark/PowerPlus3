/**
 * @file		CustomMsgDlg.cpp
 * @brief		Source file for Custom Message dialog
 * @author		AnthonyLeeStark
 * @date		2024.08.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "Dialogs/CustomMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace AppCore;


// Dialog default size
constexpr const int defaultWidth = 320;
constexpr const int defaultHeight = 240;


// Implement methods for CCustomMsgDlg
IMPLEMENT_DYNAMIC(CCustomMsgDlg, SDialog)


/**
 * @brief	Constructor
 */
CCustomMsgDlg::CCustomMsgDlg() : SDialog(IDD_CUSTOM_MESSAGE_DLG)
{
	// Message string buffer
	m_strBuffer = Constant::String::Empty;
}

/**
 * @brief	Destructor
 */
CCustomMsgDlg::~CCustomMsgDlg()
{
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CCustomMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCustomMsgDlg, SDialog)
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// CCustomMsgDlg message handlers

/**
 * @brief	Initialize DebugTest dialog
 * @param	None
 * @return	BOOL - Default
 */
BOOL CCustomMsgDlg::OnInitDialog()
{
	RECT rcClient;
	this->GetClientRect(&rcClient);

	return TRUE;
}

/**
 * @brief	Destroy dialog
 * @param	None
 * @return	None
 */
void CCustomMsgDlg::OnDestroy()
{
	EndDialog(IDCANCEL);
}

/**
 * @brief	Dialog get min/max info handler
 * @param	Default
 * @return	None
 */
void CCustomMsgDlg::OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo)
{
	// Fix min size
	pMinMaxInfo->ptMinTrackSize.x = defaultWidth;
	pMinMaxInfo->ptMinTrackSize.y = defaultHeight;

	// Fix max size
	pMinMaxInfo->ptMaxTrackSize.x = defaultWidth * 3;
	pMinMaxInfo->ptMinTrackSize.y = defaultHeight * 3;

	SDialog::OnGetMinMaxInfo(pMinMaxInfo);
}

/**
 * @brief	Resizelog
 * @param	Default
 * @return	None
 */
void CCustomMsgDlg::OnSize(UINT nType, int nWidth, int nHeight)
{
	SDialog::OnSize(nType, nWidth, nHeight);
}
