
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		SDialog.h
//		Project name:	Stupid Framework
//		Description:	Custom base class for dialogs (header file)
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.07.06:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _STUPIDFRAMEWORK_SDIALOG_H_INCLUDED
#define _STUPIDFRAMEWORK_SDIALOG_H_INCLUDED

#include "stdafx.h"
#include "afxdialogex.h"

#include "Core.h"
#include "IDMapping.h"
#include "Logging.h"
#include "GridCtrl.h"
#include "GridCellCheck.h"


////////////////////////////////////////////////////////
//
//	Class name:	 SDialog
//  Description: Custom base class for dialogs
//
////////////////////////////////////////////////////////

class SDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SDialog)

	// Construction
public:
	SDialog();														// constructor
	SDialog(UINT nIDTemplate, CWnd* pParent = NULL);				// custom constructor
	SDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);		// custom constructor
	virtual ~SDialog();												// destructor

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	DECLARE_MESSAGE_MAP()

protected:
	// Member variables
	BOOL	m_bChangeFlag;
	BOOL	m_bReadOnlyMode;
	BOOL	m_bLockState;
	BOOL	m_bSetDlgTitle;
	BOOL	m_bSetBkgrdColor;
	BOOL	m_bSetTextColor;
	BOOL	m_bUseEscape;
	BOOL	m_bUseEnter;
	BOOL	m_bSetMinSize;
	BOOL	m_bSetMaxSize;
	BOOL	m_bSetTopMost;
	BOOL	m_bSetInitSound;

	CUIntArray m_arrLockExceptionIDList;

	// Properties
	CSize	 m_szDlgSize;
	CSize	 m_szDefaultSize;
	CPoint	 m_ptMinSize;
	CPoint	 m_ptMaxSize;
	CPoint	 m_ptMarginTopLeft;
	CPoint	 m_ptMarginBottomRight;

	CString  m_strDlgTitle;
	HICON	 m_hDefaultIcon;
	CBrush*  m_pBkgrdBrush;
	COLORREF m_clBkgrdColor;
	COLORREF m_clTextColor;

public:
	// Message handlers
	afx_msg void OnOK();
	afx_msg void OnCancel();

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreDestroyDialog();

	// Member functions
	virtual UINT GetDialogID();
	virtual BOOL GetReadOnlyMode();
	virtual void SetReadOnlyMode(BOOL bReadOnly);
	virtual BOOL GetLockState();
	virtual void SetLockState(BOOL bIsLock);
	virtual void SetUseEnter(BOOL bUseEnter);
	virtual void SetUseEscape(BOOL bUseEscape);

	virtual void AddLockStateException(UINT nID);
	virtual void RemoveLockStateException(UINT nID);
	virtual void ResetLockStateExceptionList();

	// Setup dialog properties functions
	virtual void ResizeDialog(BOOL bCenterDialog);
	virtual void ResetDialogSize();
	virtual void GetDlgSize(LPSIZE lpszDlgSize);
	virtual void SetDlgSize(SIZE szDlgSize);
	virtual void SetDlgSize(int nWidth, int nHeight);
	virtual void SetMinSize(int nMinWidth, int nMinHeight);
	virtual void SetMaxSize(int nMaxWidth, int nMaxHeight);

	virtual void SetTopMost(BOOL bTopMost);
	virtual void SetInitSound(BOOL bInitSound);

	virtual void SetTopMargin(int nTopMargin);
	virtual void SetLeftMargin(int nLeftMargin);
	virtual void SetBottomMargin(int nBottomMargin);
	virtual void SetRightMargin(int nRightMargin);
	virtual void SetCenterMargin(POINT ptMargin);
	virtual void SetCenterMargin(int nHMargin, int nVMargin);
	virtual void GetDisplayArea(LPRECT lpRect);
	virtual void SetDisplayArea(RECT rcNewDispArea, BOOL bResizeDialog, BOOL bCenter);
	virtual void GetMargin(LPPOINT ptTopLeft, LPPOINT ptBottomRight);

	virtual void GetDlgTitle(CString& strDlgTitle);
	virtual void SetDlgTitle(LPCTSTR lpszDlgTitle);
	virtual void SetRcDlgTitle(UINT nResourceStringID);
	virtual void SetLangDlgTitle(UINT nLangStringID);

	virtual void SetIcon(UINT nIconResourceID);
	virtual void SetIcon(HICON hIcon, BOOL bBigIcon);

	virtual void GetBkgrdColor(COLORREF& clBkgrdColor);
	virtual void SetBkgrdColor(COLORREF clBkgrdColor);
	virtual void GetTextColor(COLORREF& clTextColor);
	virtual void SetTextColor(COLORREF clTextColor);

	// MessageBox and logging functions
	virtual int DisplayMessageBox(UINT nPromptID, UINT nCaptionID, UINT nStyle);
	virtual int DisplayMessageBox(LPCTSTR lpszPrompt, UINT nCaptionID, UINT nStyle);
	virtual int DisplayMessageBox(UINT nPromptID, LPCTSTR lpszCaption, UINT nStyle);
	virtual int DisplayMessageBox(LPCTSTR lpszPrompt, LPCTSTR lpszCaption, UINT nStyle);

	// Dialog and items setup functions
	virtual void SetupLanguage();
	virtual void SetupComboBox(UINT nComboID, LANGTABLE_PTR pLanguage);
	virtual void SetControlText(CWnd* pCtrlWnd, UINT nCtrlID, LANGTABLE_PTR ptrLanguage = NULL);
	virtual void EnableControl(UINT nCtrlID, BOOL bEnable);
	virtual void SetupDlgItemState();
	virtual void RefreshDlgItemState();

	// Data processing functions
	virtual BOOL GetChangeFlagValue();
	virtual void SetChangeFlagValue(BOOL bChangeFlag);
	virtual BOOL CheckDataChangeState();
	virtual BOOL CheckSettingChangeState();
};

#endif	// ifndef _STUPIDFRAMEWORK_SDIALOG_H_INCLUDED