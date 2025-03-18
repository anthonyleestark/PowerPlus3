
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		HelpDlg.h
//		Description:	Header file for Help dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2024.12.18:		Update to version 3.2
//						<2> 2025.03.06:		Split from AboutDlg.h
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HELPDLG_H_INCLUDED
#define _HELPDLG_H_INCLUDED

#include "Core.h"
#include "Config.h"
#include "Logging.h"
#include "IDMapping.h"
#include "SDialog.h"


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

	// Get/set properties
	UINT GetViewMode(void) const;
	void SetViewMode(UINT nViewMode);
};

#endif		// ifndef _HELPDLG_H_INCLUDED
