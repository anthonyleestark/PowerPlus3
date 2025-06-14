/**
 * @file		HelpDlg.h
 * @brief		Header file for Help dialog
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


// Help dialog class
class CHelpDlg : public SDialog
{
	DECLARE_DYNAMIC(CHelpDlg)

private:
	enum ViewMode {
		HelpFile = 0,
		Changelog
	};

private:
	String   m_strFileData;
	unsigned m_nViewMode;

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
	afx_msg void OnCloseButton();
	afx_msg void OnSwitchViewMode();

protected:
	// Member functions
	void SetupLanguage(void);
	void SetupEditbox(CEdit& pEdit);
	bool LoadRCFileData(String& strRCFileData);
	void UpdateSwitchViewModeButton(void);

	// Get/set properties
	unsigned GetViewMode() const {
		return m_nViewMode;
	};
	void SetViewMode(unsigned nViewMode) {
		m_nViewMode = nViewMode;
	};
};

