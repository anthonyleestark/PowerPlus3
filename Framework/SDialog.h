
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		SDialog.h
//		Project name:	Stupid Framework
//		Description:	Custom base class for dialogs (header file)
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.07.06:		Create new
//						<1> 2024.12.18:		Update to version 3.2
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

#include "SElements.h"


//////////////////////////////////////////////////////////////////////////
//
//	Define flags for dialog properties
//
//////////////////////////////////////////////////////////////////////////


// Flags for SetDialogAlign
//

#define SDA_LEFTALIGN					0x0001L
#define SDA_RIGHTALIGN					0x0002L
#define SDA_TOPALIGN					0x0004L
#define SDA_BOTTOMALIGN					0x0008L

#define SDA_HCENTERALIGN				0x0010L
#define SDA_VCENTERALIGN				0x0020L

#define SDA_LEFTBUTTON					0x0100L
#define SDA_RIGHTBUTTON					0x0200L


// Flags for moving functions
//

#define MOVDIR_HORIZONTAL				0x0001L
#define MOVDIR_VERTICAL					0x0002L


// Flags and default values for dialog properties
//

#define POINT_NULL						CPoint(0,0)
#define SIZE_NULL						CSize(0,0)
#define SIZE_UNDEFINED					CSize(-1,-1)
#define MARGIN_NULL						CRect(0,0,0,0)
#define MARGIN_DEFAULT					CRect(10,10,10,10)


//////////////////////////////////////////////////////////////////////////
//
//	Define custom message for dialogs
//
//////////////////////////////////////////////////////////////////////////


#define SCM_DIALOG						(WM_USER+2000)

#define SCM_NOTIFY_DIALOGINIT			(SCM_DIALOG+1)
#define SCM_NOTIFY_DIALOGSHOWED			(SCM_DIALOG+2)
#define SCM_NOTIFY_DIALOGHIDDEN			(SCM_DIALOG+3)
#define SCM_NOTIFY_DIALOGEXPAND			(SCM_DIALOG+4)
#define SCM_NOTIFY_DIALOGCOLLAPSE		(SCM_DIALOG+5)
#define SCM_NOTIFY_DIALOGACTIVE			(SCM_DIALOG+6)
#define SCM_NOTIFY_DIALOGINACTIVE		(SCM_DIALOG+7)
#define SCM_NOTIFY_DIALOGCLOSE			(SCM_DIALOG+8)
#define SCM_NOTIFY_DIALOGDESTROY		(SCM_DIALOG+9)


/////////////////////////////////////////////////////////////////
//
//	Class name:	 SDialog
//  Description: Custom base class for dialogs
//  Base class:	 CDialogEx
//
/////////////////////////////////////////////////////////////////

class SDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SDialog)

public:
	// Construction
	SDialog();																// default constructor
	explicit SDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);			// custom constructor
	explicit SDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);	// custom constructor
	virtual ~SDialog();														// destructor

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	DECLARE_MESSAGE_MAP()

protected:
	// Tooltip control
	CToolTipCtrl* m_pToolTip;

	// Dialog control management
	SControlManager* m_pCtrlManager;

	// Dialog special flags
	BOOL m_bChangeFlag;
	BOOL m_bReadOnlyMode;
	BOOL m_bLockState;
	BOOL m_bForceClose;
	BOOL m_bUseEscape;
	BOOL m_bUseEnter;

	// Properties set flags
	BOOL m_bBkgrdColorSet;
	BOOL m_bTextColorSet;
	BOOL m_bTopMostSet;
	BOOL m_bInitSoundSet;

	// Lock state exception ID list
	CUIntArray* m_paLockExceptionIDList;

	// Dialog position
	CPoint m_ptPosition;

	// Dialog alignment
	UINT m_nAlignment;

	// Dialog size
	CSize m_szRegisterSize;
	CSize m_szDefaultSize;

	// Dialog min/max info
	CSize m_szMinSize;
	CSize m_szMaxSize;

	// Dialog client display margin
	CRect	 m_rcClientMargin;

	// Other properties
	CString  m_strCaption;
	HICON	 m_hDefaultIcon;
	CString	 m_strMsgCaption;

	// Color and graphic objects
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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int	 PreDestroyDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	// Parent window functions
	virtual CWnd* GetParentWnd(void);
	virtual void SetParentWnd(CWnd* pParentWnd);
	virtual BOOL IsParentWndAvailable(void) const;
	virtual BOOL NotifyParent(UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT SendMessageToParent(UINT nMsg, WPARAM wParam, LPARAM lParam);

	// Tooltip control functions
	virtual CToolTipCtrl* GetToolTipCtrl(void);
	virtual BOOL IsToolTipCtrlAvailable(void) const;

	// Dialog control management functions
	virtual SControlManager* GetControlManager(void);
	virtual INT_PTR RegisterDialogManagement(void);
	virtual void UpdateDialogManagement(void);
	virtual BOOL UnregisterDialogManagement(void);

	// Dialog style functions
	virtual BOOL AddStyle(DWORD dwAddStyle);
	virtual BOOL RemoveStyle(DWORD dwRemoveStyle);

	// Dialog properties and information
	virtual UINT GetDialogID(void) const;
	virtual BOOL GetReadOnlyMode(void) const;
	virtual void SetReadOnlyMode(BOOL bReadOnly);
	virtual BOOL GetLockState(void) const;
	virtual void SetLockState(BOOL bIsLock);
	virtual void SetUseEnter(BOOL bUseEnter);
	virtual void SetUseEscape(BOOL bUseEscape);

	// Lock state exception ID list
	virtual void AddLockStateException(UINT nID);
	virtual void RemoveLockStateException(UINT nID);
	virtual void ResetLockStateExceptionList(void);

	// Dialog align and position
	virtual UINT GetAlignment(void) const;
	virtual void SetAlignment(UINT nAlignment);
	virtual void GetPosition(LPPOINT lpPosition) const;
	virtual void SetPosition(POINT ptPosition);

	// Move and resize dialog
	virtual void MoveDialog(POINT ptPosition, LPRECT lpNewRect = NULL);
	virtual void MoveDialog(int dx, int dy, LPRECT lpNewRect = NULL);
	virtual void ResizeDialog(BOOL bCenterDialog);
	virtual void ResetDialogSize(void);

	// Get/set dialog size functions
	virtual void GetSize(LPSIZE lpRegSize) const;
	virtual void SetSize(SIZE szRegSize);
	virtual void SetSize(LONG lWidth, LONG lHeight);
	virtual void SetMinSize(LONG lMinWidth, LONG lMinHeight);
	virtual void SetMaxSize(LONG lMaxWidth, LONG lMaxHeight);

	// Other properties
	virtual void SetTopMost(BOOL bTopMost);
	virtual void SetInitSound(BOOL bInitSound);

	// Dialog margins and display area
	virtual void SetLeftMargin(LONG lMargin);
	virtual void SetTopMargin(LONG lMargin);
	virtual void SetRightMargin(LONG lMargin);
	virtual void SetBottomMargin(LONG lMargin);
	virtual void SetCenterMargin(POINT ptMargin);
	virtual void SetCenterMargin(LONG lHMargin, LONG lVMargin);
	virtual void GetDisplayArea(LPRECT lpDispAreaRect) const;
	virtual void SetDisplayArea(RECT rcNewDispArea, BOOL bResizeDialog, BOOL bCenter);
	virtual void GetMargin(LPRECT lpDialogMargin) const;

	// Dialog caption get/set functions
	virtual LPCTSTR GetCaption(void) const;
	virtual void GetCaption(CString& strCaption) const;
	virtual void SetCaption(LPCTSTR lpszCaption);
	virtual void SetCaptionFromResource(UINT nResourceStringID);
	virtual void SetCaptionFromLanguage(UINT nLangStringID);

	// Dialog icon functions
	virtual void SetIcon(UINT nIconResourceID);
	virtual void SetIcon(HICON hIcon, BOOL bBigIcon);

	// Dialog color get/set functions
	virtual void GetBkgrdColor(COLORREF& clBkgrdColor) const;
	virtual void SetBkgrdColor(COLORREF clBkgrdColor);
	virtual void GetTextColor(COLORREF& clTextColor) const;
	virtual void SetTextColor(COLORREF clTextColor);

	// MessageBox functions
	virtual void RegisterMessageBoxCaption(UINT nCaptionID);
	virtual void RegisterMessageBoxCaption(LPCTSTR lpszCaption);
	virtual void GetMessageBoxCaption(CString& strRegMsgBoxCap) const;
	virtual int DisplayMessageBox(UINT nPromptID, UINT nCaptionID = NULL, UINT nStyle = NULL);
	virtual int DisplayMessageBox(LPCTSTR lpszPrompt, LPCTSTR lpszCaption = NULL, UINT nStyle = NULL);

	// Dialog event logging function
	virtual void OutputEventLog(USHORT usEvent, LPCTSTR lpszDescription = NULL, LOGDETAILINFO* pDetailInfo = NULL);

	// Dialog control event logging functions
	virtual void OutputButtonLog(USHORT usEvent, UINT nButtonID);
	virtual void OutputCheckBoxLog(USHORT usEvent, UINT nCheckboxID);
	virtual void OutputRadButtonLog(USHORT usEvent, UINT nRadButtonID);
	virtual void OutputComboBoxLog(USHORT usEvent, UINT nComboID);
	virtual void OutputEditBoxLog(USHORT usEvent, UINT nEditID);
	virtual void OutputListBoxLog(USHORT usEvent, UINT nListBoxID);
	virtual void OutputSpinCtrlLog(USHORT usEvent, UINT nSpinCtrlID);
	virtual void OutputMenuLog(USHORT usEvent, UINT nMenuItemID);

	// Dialog and items setup functions
	virtual void SetupLanguage(void);
	virtual void SetupComboBox(UINT nComboID, LANGTABLE_PTR pLanguage);
	virtual void UpdateItemText(UINT nCtrlID, LPCTSTR lpszNewCaption);
	virtual void UpdateItemText(UINT nCtrlID, UINT nNewCaptionID = NULL, LANGTABLE_PTR ptrLanguage = NULL);
	virtual void SetControlText(CWnd* pCtrlWnd, UINT nCtrlID, LANGTABLE_PTR ptrLanguage = NULL);
	virtual void MoveItemGroup(const CUIntArray& arrCtrlIDGroup, POINT ptNewPosition);
	virtual void MoveItemGroup(const CUIntArray& arrCtrlIDGroup, int nDirection, int nDistance);
	virtual void EnableControl(UINT nCtrlID, BOOL bEnable);
	virtual void SetupDlgItemState(void);
	virtual void RefreshDlgItemState(void);

	// Layout functions
	virtual void UpdateLayoutInfo(void);
	virtual void LoadLayoutInfo(void);
	virtual void SaveLayoutInfo(void);

	// Data processing functions
	virtual void UpdateDialogData(BOOL bSaveAndValidate = TRUE);
	virtual int  GetAppOption(APPOPTIONID eAppOptionID, BOOL bTemp = FALSE) const;
	virtual int  GetFlagValue(APPFLAGID eFlagID) const;
	virtual void SetFlagValue(APPFLAGID eFlagID, int nValue);
	virtual BOOL GetChangeFlagValue(void) const;
	virtual void SetChangeFlagValue(BOOL bChangeFlag);
	virtual BOOL CheckDataChangeState(void);
	virtual BOOL CheckSettingChangeState(void);
	virtual BOOL IsForceClosingByRequest(void) const;

	// Request processing functions
	virtual LRESULT RequestCloseDialog(void);
};

#endif	// ifndef _STUPIDFRAMEWORK_SDIALOG_H_INCLUDED
