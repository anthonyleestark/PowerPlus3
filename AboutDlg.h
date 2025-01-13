
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

#include "Core.h"
#include "Config.h"
#include "Logging.h"
#include "IDMapping.h"
#include "SDialog.h"


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
	virtual INT_PTR RegisterDialogManagement(void);
	virtual BOOL UnregisterDialogManagement(void);

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_CLASS_IDMAP()

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnDevProfileLinkClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseButton();

protected:
	// Member functions
	void SetupLanguage(void);
	void SetAppNameLabel(void);
	void SetAppInfoLabel(void);
};


////////////////////////////////////////////////////////
//
//	CHelpDlg dialog used for App Help
//
////////////////////////////////////////////////////////

class CHelpDlg : public SDialog
{
	DECLARE_DYNAMIC(CHelpDlg)

private:
	CString m_strFileData;
	UINT	m_nViewMode;

public:
	CHelpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHelpDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HELP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Dialog control management
	virtual INT_PTR RegisterDialogManagement(void);
	virtual BOOL UnregisterDialogManagement(void);

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_CLASS_IDMAP()

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnCloseButton();
	afx_msg void OnSwitchViewMode();

protected:
	// Member functions
	void SetupLanguage(void);
	void SetupEditbox(CEdit& pEdit);
	BOOL LoadFileData(CString& strFileData);
	void UpdateSwitchViewModeButton(void);
	UINT GetViewMode(void);
	void SetViewMode(UINT nViewMode);
};

#endif	// ifndef _ABOUTDLG_H_INCLUDED