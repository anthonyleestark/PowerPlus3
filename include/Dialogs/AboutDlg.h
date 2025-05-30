
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		AboutDlg.h
//		Description:	Header file for About and Help dialogs
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

#ifndef _ABOUTDLG_H_INCLUDED
#define _ABOUTDLG_H_INCLUDED

#include "AppCore/AppCore.h"
#include "AppCore/Config.h"
#include "AppCore/Logging.h"
#include "AppCore/IDMapping.h"
#include "Framework/SDialog.h"


////////////////////////////////////////////////////////
//
//	CAboutDlg dialog used for App About
//
////////////////////////////////////////////////////////

class CAboutDlg : public SDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg();	// constructor
	~CAboutDlg();	// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Dialog control management
	virtual size_t RegisterDialogManagement(void);
	virtual BOOL UnregisterDialogManagement(void);

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_RESOURCEID_MAP()

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnViewFacebookProfileLink(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseButton();

protected:
	// Member functions
	void SetupLanguage(void);
	void SetAppNameLabel(void);
	void SetAppInfoLabel(void);
};

#endif	// ifndef _ABOUTDLG_H_INCLUDED
