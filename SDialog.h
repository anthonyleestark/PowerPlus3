
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


//////////////////////////////////////////////////////////////////////////
//
//	Define flags for dialog properties
//
//////////////////////////////////////////////////////////////////////////

// Flags for SetDialogAlign
#define SDA_LEFTALIGN		0x0001L
#define SDA_RIGHTALIGN		0x0002L
#define SDA_TOPALIGN		0x0004L
#define SDA_BOTTOMALIGN		0x0008L

#define SDA_HCENTERALIGN	0x0010L
#define SDA_VCENTERALIGN	0x0020L

#define SDA_LEFTBUTTON		0x0100L
#define SDA_RIGHTBUTTON		0x0200L


//////////////////////////////////////////////////////////////////////////
//
//	Define custom message for dialogs
//
//////////////////////////////////////////////////////////////////////////

#define SCM_DIALOG					(WM_USER+1000)

#define SCM_DLGINIT_NOTIFY			(SCM_DIALOG+1)
#define SCM_DLGCLOSE_NOTIFY			(SCM_DIALOG+2)
#define SCM_DLGHIDDEN_NOTIFY		(SCM_DIALOG+3)
#define SCM_DLGDESTROY_NOTIFY		(SCM_DIALOG+4)

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
	// Parent window
	CWnd* m_pParentWnd;

	// Dialog special flags
	BOOL m_bChangeFlag;
	BOOL m_bReadOnlyMode;
	BOOL m_bLockState;
	BOOL m_bForceClose;
	BOOL m_bUseEscape;
	BOOL m_bUseEnter;

	// Properties set flags
	BOOL m_bSetDlgTitle;
	BOOL m_bSetBkgrdColor;
	BOOL m_bSetTextColor;

	BOOL m_bSetMinSize;
	BOOL m_bSetMaxSize;

	BOOL m_bSetTopMost;
	BOOL m_bSetInitSound;

	// Lock state exception IDs
	CUIntArray m_arrLockExceptionIDList;

	// Dialog position
	CPoint m_ptDlgPosition;

	// Dialog alignment
	UINT m_nDlgAlign;

	// Dialog size
	CSize m_szDlgSize;
	CSize m_szDefaultSize;

	CPoint m_ptMinSize;
	CPoint m_ptMaxSize;

	// Dialog margin
	CPoint	 m_ptMarginTopLeft;
	CPoint	 m_ptMarginBottomRight;

	// Other properties
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
	virtual void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual void OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int	 PreDestroyDialog();

	// Member functions
	virtual CWnd* GetParentWnd();
	virtual void SetParentWnd(CWnd* pParentWnd);
	virtual BOOL IsParentWndAvailable();

	virtual BOOL AddDialogStyle(DWORD dwAddStyle);
	virtual BOOL RemoveDialogStyle(DWORD dwRemoveStyle);

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
	virtual UINT GetDialogAlign(void);
	virtual void SetDialogAlign(UINT nAlign);
	virtual void GetDialogPosition(LPPOINT lpPosition);
	virtual void SetDialogPosition(POINT ptPosition);

	virtual void MoveDialog(POINT ptPosition, LPRECT lpNewRect = NULL);
	virtual void MoveDialog(int dx, int dy, LPRECT lpNewRect = NULL);
	virtual void ResizeDialog(BOOL bCenterDialog);
	virtual void ResetDialogSize();

	virtual void GetDlgSize(LPSIZE lpDlgSize);
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
	virtual int DisplayMessageBox(UINT nPromptID, UINT nCaptionID = NULL, UINT nStyle = NULL);
	virtual int DisplayMessageBox(LPCTSTR lpszPrompt, UINT nCaptionID = NULL, UINT nStyle = NULL);
	virtual int DisplayMessageBox(UINT nPromptID, LPCTSTR lpszCaption, UINT nStyle = NULL);
	virtual int DisplayMessageBox(LPCTSTR lpszPrompt, LPCTSTR lpszCaption, UINT nStyle = NULL);

	// Dialog and items setup functions
	virtual void SetupLanguage();
	virtual void SetupComboBox(UINT nComboID, LANGTABLE_PTR pLanguage);
	virtual void UpdateItemText(UINT nCtrlID, LPCTSTR lpszNewCaption);
	virtual void UpdateItemText(UINT nCtrlID, UINT nNewCaptionID = NULL, LANGTABLE_PTR ptrLanguage = NULL);
	virtual void SetControlText(CWnd* pCtrlWnd, UINT nCtrlID, LANGTABLE_PTR ptrLanguage = NULL);
	virtual void EnableControl(UINT nCtrlID, BOOL bEnable);
	virtual void SetupDlgItemState();
	virtual void RefreshDlgItemState();

	// Data processing functions
	virtual BOOL GetChangeFlagValue();
	virtual void SetChangeFlagValue(BOOL bChangeFlag);
	virtual BOOL CheckDataChangeState();
	virtual BOOL CheckSettingChangeState();
	virtual BOOL IsForceClosingByRequest(void);

	// Request processing functions
	virtual LRESULT RequestCloseDialog(void);
};

#endif	// ifndef _STUPIDFRAMEWORK_SDIALOG_H_INCLUDED