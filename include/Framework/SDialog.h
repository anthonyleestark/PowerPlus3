
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

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/IDManager.h"
#include "AppCore/Logging.h"
#include "Components/GridCtrl/GridCtrl.h"
#include "Components/GridCtrl/GridCellCheck.h"
#include "SElements.h"
#include "SWinApp.h"


//////////////////////////////////////////////////////////////////////////
//
//	Define flags for dialog properties
//
//////////////////////////////////////////////////////////////////////////


// Flags for SetDialogAlign
//

#define SDA_LEFTALIGN						0x0001L
#define SDA_RIGHTALIGN						0x0002L
#define SDA_TOPALIGN						0x0004L
#define SDA_BOTTOMALIGN						0x0008L

#define SDA_HCENTERALIGN					0x0010L
#define SDA_VCENTERALIGN					0x0020L

#define SDA_LEFTBUTTON						0x0100L
#define SDA_RIGHTBUTTON						0x0200L


// Flags for moving functions
//

#define MOVDIR_HORIZONTAL					0x0001L
#define MOVDIR_VERTICAL						0x0002L


// Flags and default values for dialog properties
//

#define POINT_NULL							CPoint(0,0)
#define SIZE_NULL							CSize(0,0)
#define SIZE_UNDEFINED						CSize(-1,-1)
#define MARGIN_NULL							CRect(0,0,0,0)
#define MARGIN_DEFAULT						CRect(10,10,10,10)


//////////////////////////////////////////////////////////////////////////
//
//	Define custom message for dialogs
//
//////////////////////////////////////////////////////////////////////////


#define SCM_NOTIFY_DIALOG_EVENT				(WM_USER+2000)

#define SCM_NOTIFY_DIALOG_INIT				(SCM_NOTIFY_DIALOG_EVENT+1)
#define SCM_NOTIFY_DIALOG_SHOWED			(SCM_NOTIFY_DIALOG_EVENT+2)
#define SCM_NOTIFY_DIALOG_HIDDEN			(SCM_NOTIFY_DIALOG_EVENT+3)
#define SCM_NOTIFY_DIALOG_EXPAND			(SCM_NOTIFY_DIALOG_EVENT+4)
#define SCM_NOTIFY_DIALOG_COLLAPSE			(SCM_NOTIFY_DIALOG_EVENT+5)
#define SCM_NOTIFY_DIALOG_ACTIVE			(SCM_NOTIFY_DIALOG_EVENT+6)
#define SCM_NOTIFY_DIALOG_INACTIVE			(SCM_NOTIFY_DIALOG_EVENT+7)
#define SCM_NOTIFY_DIALOG_RESIZE			(SCM_NOTIFY_DIALOG_EVENT+8)
#define SCM_NOTIFY_DIALOG_MOVE				(SCM_NOTIFY_DIALOG_EVENT+9)
#define SCM_NOTIFY_DIALOG_CLOSE				(SCM_NOTIFY_DIALOG_EVENT+10)
#define SCM_NOTIFY_DIALOG_DESTROY			(SCM_NOTIFY_DIALOG_EVENT+11)


//////////////////////////////////////////////////////////////////////////
//
//	Class name:	 SDialog
//  Description: Custom base class for dialogs
//  Base class:	 CDialogEx
//
//////////////////////////////////////////////////////////////////////////

class SDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SDialog)

public:
	using ExitCode = SWinApp::ExitCode;
	enum ReturnFlag {
		Invalid = -1,
		OK,
		Cancel,
		Update,
		Close,
	};

public:
	// Construction
	SDialog();																// default constructor
	explicit SDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);			// custom constructor
	explicit SDialog(const wchar_t* templateName, CWnd* pParentWnd = NULL);	// custom constructor
	virtual ~SDialog();														// destructor

protected:
	// Dialog resource ID mapping
	static const size_t PASCAL UpdateThisResourceIDMap();
	virtual const size_t UpdateResourceIDMap();

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	// Tooltip control
	CToolTipCtrl* m_pToolTip;

	// Dialog control management
	SControlManager* m_pCtrlManager;

	// Dialog flag manager
	FlagManager m_flagManager;

	// Dialog special flags
	UINT m_nDescendantCount;

	// Lock state exception ID list
	UIntArray* m_paLockExceptionIDList;

	// Dialog anchor point
	CPoint m_ptAnchorPoint;

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
	String  m_strCaption;
	HICON	m_hDefaultIcon;
	String	m_strMsgCaption;

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
	afx_msg LRESULT OnChildDialogInit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChildDialogDestroy(WPARAM wParam, LPARAM lParam);
	virtual void OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int	 PreDestroyDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	// Modal processing
	virtual INT_PTR DoModal(void);

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
	virtual int RegisterDialogManagement(void);
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
	virtual void SetLockState(BOOL bIsLocked);
	virtual void SetUseEnter(BOOL bUseEnter);
	virtual void SetUseEscape(BOOL bUseEscape);

	// Lock state exception ID list
	virtual void AddLockStateException(UINT nID);
	virtual void RemoveLockStateException(UINT nID);
	virtual void ResetLockStateExceptionList(void);

	// Dialog align and position
	virtual UINT GetAlignment(void) const;
	virtual void SetAlignment(UINT nAlignment);
	virtual void GetAnchorPoint(LPPOINT lpAnchorPoint) const;
	virtual void SetAnchorPoint(POINT ptAnchorPoint);
	virtual void SetDialogPosition(POINT ptAnchorPoint, UINT nAlignment);

	// Move and resize dialog
	virtual void MoveDialog(POINT ptPosition, LPRECT lpNewRect = NULL);
	virtual void MoveDialog(LONG dx, LONG dy, LPRECT lpNewRect = NULL);
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
	virtual const wchar_t* GetCaption(void) const;
	virtual void GetCaption(String& caption) const;
	virtual void SetCaption(const wchar_t* caption);
	virtual void SetCaptionFromResource(UINT nResourceStringID);
	virtual void SetCaptionFromLanguage(UINT nLangStringID);

	// Dialog icon functions
	virtual void SetIcon(UINT nIconResourceID);
	virtual void SetIcon(HICON hIcon, BOOL bBigIcon);

	// Dialog color functions
	virtual void GetBkgrdColor(COLORREF& clBkgrdColor) const;
	virtual void SetBkgrdColor(COLORREF clBkgrdColor);
	virtual void GetTextColor(COLORREF& clTextColor) const;
	virtual void SetTextColor(COLORREF clTextColor);
	virtual BOOL CreateBrush(void);

	// MessageBox functions
	virtual void RegisterMessageBoxCaption(UINT nCaptionID);
	virtual void RegisterMessageBoxCaption(const wchar_t* caption);
	virtual void GetMessageBoxCaption(String& regMsgBoxCaption) const;
	virtual int DisplayMessageBox(UINT nPromptID, UINT nCaptionID = NULL, UINT nStyle = NULL);
	virtual int DisplayMessageBox(const wchar_t* prompt, const wchar_t* caption = NULL, UINT nStyle = NULL);

	// Dialog event logging function
	virtual void OutputEventLog(USHORT usEvent, const wchar_t* description = NULL, LOGDETAILINFO* pDetailInfo = NULL);

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
	virtual void SetButtonIcon(UINT nButtonID, UINT nIconID, BOOL bReUpdateTitle = FALSE);
	virtual void UpdateItemText(UINT nCtrlID, const wchar_t* newCaption);
	virtual void UpdateItemText(UINT nCtrlID, UINT nNewCaptionID = NULL, LANGTABLE_PTR ptrLanguage = NULL);
	virtual void SetControlText(CWnd* pCtrlWnd, UINT nCtrlID, LANGTABLE_PTR ptrLanguage = NULL);
	virtual void MoveItemGroup(const UIntArray& arrCtrlIDGroup, POINT ptNewPosition);
	virtual void MoveItemGroup(const UIntArray& arrCtrlIDGroup, int nDirection, int nDistance);
	virtual void ShowItem(UINT nDlgItemID, BOOL bVisible);
	virtual void ShowItem(CWnd* pDlgItemWnd, BOOL bVisible);
	virtual void EnableItem(UINT nDlgItemID, BOOL bEnabled);
	virtual void EnableItem(CWnd* pDlgItemWnd, BOOL bEnabled);
	virtual void SetupDialogItemState(void);
	virtual void RefreshDialogItemState(BOOL bRecheckState = FALSE);

	// Layout functions
	virtual void UpdateLayoutInfo(void);
	virtual void LoadLayoutInfo(void);
	virtual void SaveLayoutInfo(void);

	// Data processing functions
	virtual void UpdateDialogData(BOOL bSaveAndValidate = TRUE);
	virtual int  GetAppOption(AppOptionID eAppOptionID, BOOL bTemp = FALSE) const;
	virtual int  GetFlagValue(AppFlagID eFlagID) const;
	virtual void SetFlagValue(AppFlagID eFlagID, int nValue);
	virtual FlagManager& GetAppFlagManager(void);
	virtual const FlagManager& GetAppFlagManager(void) const;
	virtual FlagManager& GetDialogFlagManager(void);
	virtual const FlagManager& GetDialogFlagManager(void) const;

	// Directly access flag values
	virtual BOOL GetChangeFlagValue(void) const;
	virtual void SetChangeFlagValue(BOOL bValue);
	virtual BOOL CheckDataChangeState(void);
	virtual BOOL CheckSettingChangeState(void);
	virtual BOOL IsForceClosingByRequest(void) const;
	virtual int  GetReturnFlag(void) const;
	virtual void SetReturnFlag(int nRetFlag);

	// Request processing functions
	virtual LRESULT RequestCloseDialog(void);

	// Descendant dialog functions
	virtual void OpenChildDialogEx(UINT nDialogID);
	virtual void OpenChildDialogEx(SDialog* pChildDialog);
	virtual UINT GetDescendantCount(void) const;
};


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _SDIALOG_ENABLE_INLINES
	#include "Framework.inl"
	#ifdef _SDIALOG_INLINE_INCLUDED
		#pragma message("-- Framework inline library included (SDialog.h)")
	#else
		#pragma error("-- Linking error in SDialog.h: Unable to link to inline header!")
	#endif
	#undef _SDIALOG_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in SDialog.h: Inline is not enabled!")
#endif
