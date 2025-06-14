/**
 * @file		AboutDlg.h
 * @brief		Header file for About dialog
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/Logging.h"
#include "AppCore/IDManager.h"
#include "AppCore/Serialization.h"
#include "Framework/SDialog.h"


// About dialog class
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
	virtual int RegisterDialogManagement(void);
	virtual bool UnregisterDialogManagement(void);

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
