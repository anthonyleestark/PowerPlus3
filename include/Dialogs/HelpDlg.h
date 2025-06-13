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
	String  m_strFileData;
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
	virtual int RegisterDialogManagement(void);
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
	afx_msg void OnCloseButton();
	afx_msg void OnSwitchViewMode();

protected:
	// Member functions
	void SetupLanguage(void);
	void SetupEditbox(CEdit& pEdit);
	BOOL LoadRCFileData(String& strRCFileData);
	void UpdateSwitchViewModeButton(void);

	// Get/set properties
	UINT GetViewMode(void) const;
	void SetViewMode(UINT nViewMode);
};


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _HELPDLG_ENABLE_INLINES
	#include "Dialogs.inl"
	#ifdef _HELPDLG_INLINE_INCLUDED
		#pragma message("-- Dialogs inline library included (HelpDlg.h)")
	#else
		#pragma error("-- Linking error in HelpDlg.h: Unable to link to inline header!")
	#endif
	#undef _HELPDLG_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in HelpDlg.h: Inline is not enabled!")
#endif
