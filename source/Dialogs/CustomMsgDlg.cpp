﻿
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		CustomMsgDlg.cpp
//		Description:	Source file for Custom Message dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2024.08.12:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dialogs/CustomMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_WIDTH	320
#define DEFAULT_HEIGHT	240

using namespace AppCore;


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CCustomMsgDlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CCustomMsgDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CCustomMsgDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CCustomMsgDlg::CCustomMsgDlg() : SDialog(IDD_CUSTOM_MESSAGE_DLG)
{
	// Message string buffer
	m_strBuffer = STRING_EMPTY;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CCustomMsgDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CCustomMsgDlg::~CCustomMsgDlg()
{
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize DebugTest dialog
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CCustomMsgDlg::OnInitDialog()
{
	RECT rcClient;
	this->GetClientRect(&rcClient);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDestroy
//	Description:	Destroy dialog
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CCustomMsgDlg::OnDestroy()
{
	EndDialog(IDCANCEL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnGetMinMaxInfo
//	Description:	Dialog get min/max info handler
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CCustomMsgDlg::OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo)
{
	// Fix min/max size
	pMinMaxInfo->ptMinTrackSize = CPoint(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	pMinMaxInfo->ptMaxTrackSize = CPoint((DEFAULT_WIDTH * 3), (DEFAULT_HEIGHT * 3));

	SDialog::OnGetMinMaxInfo(pMinMaxInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSize
//	Description:	Resizelog
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CCustomMsgDlg::OnSize(UINT nType, int nWidth, int nHeight)
{
	SDialog::OnSize(nType, nWidth, nHeight);
}
