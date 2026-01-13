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
	String   helpInfoString_;
	unsigned viewMode_;

public:
	CHelpDlg(CWnd* parentWnd = NULL);   // standard constructor
	virtual ~CHelpDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HELP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Dialog control management
	virtual int registerDialogManagement(void);
	virtual bool unregisterDialogManagement(void);

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
	void setupLanguage(void);
	void setupEditbox(CEdit& editCtrlPtr);
	bool loadResourceFileData(String& resourceFileDataString);
	void updateSwitchViewModeButton(void);

	// Get/set properties
	unsigned getViewMode() const {
		return viewMode_;
	}
	void setViewMode(unsigned viewMode) {
		viewMode_ = viewMode;
	}
};

