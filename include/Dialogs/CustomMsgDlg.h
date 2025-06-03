
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		CustomMsgDlg.h
//		Description:	Header file for Custom Message dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2024.08.12:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/Global.h"
#include "Framework/SDialog.h"


////////////////////////////////////////////////////////
//
//	CCustomMsgDlg dialog used for Custom Message
//
////////////////////////////////////////////////////////

class CCustomMsgDlg : public SDialog
{
	DECLARE_DYNAMIC(CCustomMsgDlg)

public:
	CCustomMsgDlg();	// constructor
	~CCustomMsgDlg();	// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOM_MESSAGE_DLG };
#endif

private:
	// Message string buffer
	CString m_strBuffer;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

public:
	// Generated message handle functions
	virtual BOOL OnInitDialog();
	virtual void OnDestroy();
	virtual void OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo);
	virtual void OnSize(UINT nType, int nWidth, int nHeight);
};
