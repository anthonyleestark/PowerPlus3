/**
 * @file		SDialog.h
 * @brief		Custom base class for dialogs (header file)
 * @author		AnthonyLeeStark
 * @date		2024.07.06
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/IDManager.h"
#include "AppCore/Logging.h"
#include "Components/GridCtrl/GridCtrl.h"
#include "Components/GridCtrl/GridCellCheck.h"
#include "SElements.h"
#include "SWinApp.h"


// Flags for SetDialogAlign

#define SDA_LEFTALIGN						0x0001L
#define SDA_RIGHTALIGN						0x0002L
#define SDA_TOPALIGN						0x0004L
#define SDA_BOTTOMALIGN						0x0008L

#define SDA_HCENTERALIGN					0x0010L
#define SDA_VCENTERALIGN					0x0020L

#define SDA_LEFTBUTTON						0x0100L
#define SDA_RIGHTBUTTON						0x0200L


// Flags for moving functions

#define MOVDIR_HORIZONTAL					0x0001L
#define MOVDIR_VERTICAL						0x0002L


// Flags and default values for dialog properties

#define POINT_NULL							Point(0,0)
#define SIZE_NULL							Size(0,0)
#define SIZE_UNDEFINED						Size(-1,-1)
#define MARGIN_NULL							Rect(0,0,0,0)
#define MARGIN_DEFAULT						Rect(10,10,10,10)


// Define custom message for dialogs

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


// Custom base class for dialogs
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
	explicit SDialog(unsigned nIDTemplate, CWnd* pParentWnd = NULL);			// custom constructor
	explicit SDialog(const wchar_t* templateName, CWnd* pParentWnd = NULL);	// custom constructor
	virtual ~SDialog();														// destructor

protected:
	// Dialog resource ID mapping
	static const size_t PASCAL UpdateThisResourceIDMap() {
		return GET_RESOURCEID_MAP()->GetMapCount();
	};
	virtual const size_t UpdateResourceIDMap() {
		return UpdateThisResourceIDMap();
	};

	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

protected:
	// Tooltip control
	CToolTipCtrl* m_pToolTip;

	// Dialog control management
	SControlManager* m_pCtrlManager;

	// Dialog flag manager
	FlagManager m_flagManager;

	// Dialog special flags
	unsigned m_nDescendantCount;

	// Lock state exception ID list
	UIntArray* m_paLockExceptionIDList;

	// Dialog anchor point
	Point m_ptAnchorPoint;

	// Dialog alignment
	unsigned m_nAlignment;

	// Dialog size
	Size m_szRegisterSize;
	Size m_szDefaultSize;

	// Dialog min/max info
	Size m_szMinSize;
	Size m_szMaxSize;

	// Dialog client display margin
	Rect m_rcClientMargin;

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
	virtual CWnd* GetParentWnd(void) {
		return m_pParentWnd;
	};
	virtual void SetParentWnd(CWnd* pParentWnd) {
		m_pParentWnd = pParentWnd;
	};
	virtual bool IsParentWndAvailable(void) const {
		return ((m_pParentWnd != NULL) && (m_pParentWnd->GetSafeHwnd() != NULL));
	};
	virtual bool NotifyParent(unsigned nMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT SendMessageToParent(unsigned nMsg, WPARAM wParam, LPARAM lParam);

	// Tooltip control functions
	virtual CToolTipCtrl* GetToolTipCtrl(void) {
		return m_pToolTip;
	};
	virtual bool IsToolTipCtrlAvailable(void) const {
		return ((m_pToolTip != NULL) && (m_pToolTip->GetSafeHwnd() != NULL));
	};

	// Dialog control management functions
	virtual int RegisterDialogManagement(void);
	virtual void UpdateDialogManagement(void);
	virtual bool UnregisterDialogManagement(void);
	virtual SControlManager* GetControlManager(void) {
		return m_pCtrlManager;
	};

	// Dialog style functions
	virtual bool AddStyle(DWORD dwAddStyle);
	virtual bool RemoveStyle(DWORD dwRemoveStyle);

	// Dialog properties and information
	virtual unsigned GetDialogID(void) const {
		return m_nIDHelp;
	};
	virtual bool GetReadOnlyMode(void) const {
		return m_flagManager.GetFlagValue(AppFlagID::dialogReadOnlyMode);
	};
	virtual void SetReadOnlyMode(bool bReadOnly) {
		m_flagManager.SetFlagValue(AppFlagID::dialogReadOnlyMode, bReadOnly);
	};
	virtual bool GetLockState(void) const {
		return m_flagManager.GetFlagValue(AppFlagID::dialogLockState);
	};
	virtual void SetLockState(bool bIsLocked) {
		m_flagManager.SetFlagValue(AppFlagID::dialogLockState, bIsLocked);
	};
	virtual void SetUseEnter(bool bUseEnter) {
		m_flagManager.SetFlagValue(AppFlagID::dialogUseEnterKey, bUseEnter);
	};
	virtual void SetUseEscape(bool bUseEscape) {
		m_flagManager.SetFlagValue(AppFlagID::dialogUseEscapeKey, bUseEscape);
	};

	// Lock state exception ID list
	virtual void AddLockStateException(unsigned nID);
	virtual void RemoveLockStateException(unsigned nID);
	virtual void ResetLockStateExceptionList(void);

	// Dialog align and position
	virtual unsigned GetAlignment(void) const {
		return m_nAlignment;
	};
	virtual void SetAlignment(unsigned nAlignment) {
		m_nAlignment = nAlignment;
	};
	virtual void GetAnchorPoint(Point& anchorPoint) const {
		anchorPoint = m_ptAnchorPoint;
	};
	virtual void SetAnchorPoint(const Point& anchorPoint) {
		m_ptAnchorPoint = anchorPoint;

		// Move dialog
		MoveDialog(m_ptAnchorPoint);
	};
	virtual void SetDialogPosition(const Point& anchorPoint, unsigned nAlignment) {
		SetAlignment(nAlignment);
		SetAnchorPoint(anchorPoint);
	};

	// Move and resize dialog
	virtual void MoveDialog(const Point& position, Rect* newRect = nullptr);
	virtual void MoveDialog(long dx, long dy, Rect* newRect = nullptr);
	virtual void ResizeDialog(bool bCenterDialog);
	virtual void ResetDialogSize(void);

	// Get/set dialog size functions
	virtual void GetSize(Size& regSize) const {
		// If size is not set, return default
		if (m_szRegisterSize.IsEmpty()) regSize = m_szDefaultSize;

		// Return dialog size
		regSize = m_szRegisterSize;
	};
	virtual void SetSize(const Size& regSize) {
		m_szRegisterSize = regSize;
	};
	virtual void SetSize(long lWidth, long lHeight)	{
		Size dialogSize(lWidth, lHeight);
		this->SetSize(dialogSize);
	};
	virtual void SetMinSize(long lMinWidth, long lMinHeight) {
		m_szMinSize = Size(lMinWidth, lMinHeight);
	};
	virtual void SetMaxSize(long lMaxWidth, long lMaxHeight) {
		m_szMaxSize = Size(lMaxWidth, lMaxHeight);
	};

	// Other properties
	virtual void SetTopMost(bool bTopMost) {
		SetFlagValue(AppFlagID::dialogSetTopMost, bTopMost);;
	};
	virtual void SetInitSound(bool bInitSound) {
		SetFlagValue(AppFlagID::dialogSetInitSound, bInitSound);
	};

	// Dialog margins and display area
	virtual void SetLeftMargin(long lMargin) {
		m_rcClientMargin._left = lMargin;
	};
	virtual void SetTopMargin(long lMargin) {
		m_rcClientMargin._top = lMargin;
	};
	virtual void SetRightMargin(long lMargin) {
		m_rcClientMargin._right = lMargin;
	};
	virtual void SetBottomMargin(long lMargin) {
		m_rcClientMargin._bottom = lMargin;
	};
	virtual void SetCenterMargin(const Point& ptMargin) {
		m_rcClientMargin.SetTopLeft(ptMargin);
		m_rcClientMargin.SetBottomRight(ptMargin);
	};
	virtual void SetCenterMargin(long lHMargin, long lVMargin) {
		m_rcClientMargin.SetTopLeft(lHMargin, lVMargin);
		m_rcClientMargin.SetBottomRight(lHMargin, lVMargin);
	};
	virtual void GetDisplayArea(Rect& displayAreaRect) const {
		RECT rcTemp; this->GetClientRect(&rcTemp);
		displayAreaRect = Rect(rcTemp.left, rcTemp.top, rcTemp.right, rcTemp.bottom);

		// Calculate display area with margin
		displayAreaRect._left += m_rcClientMargin.Left();
		displayAreaRect._top += m_rcClientMargin.Top();
		displayAreaRect._right -= m_rcClientMargin.Right();
		displayAreaRect._bottom -= m_rcClientMargin.Bottom();
	};
	virtual void SetDisplayArea(const Rect& newDispArea, bool bResizeDialog, bool bCenter);
	virtual void GetMargin(Rect& dialogMargin) const {
		dialogMargin = m_rcClientMargin;
	};

	// Dialog caption get/set functions
	virtual void SetCaptionFromResource(unsigned nResourceStringID);
	virtual void SetCaptionFromLanguage(unsigned nLangStringID);
	virtual const wchar_t* GetCaption(void) const {
		return m_strCaption;
	};
	virtual void GetCaption(String& caption) const {
		caption = m_strCaption;
	};
	virtual void SetCaption(const wchar_t* caption)	{
		m_strCaption = caption;

		// If dialog is already initialized, trigger updating title
		if (IsWindow(this->m_hWnd)) {
			this->SetWindowText(m_strCaption);
		}
	};

	// Dialog icon functions
	virtual void SetIcon(unsigned nIconResourceID) {
		::DeleteObject(m_hDefaultIcon);
		m_hDefaultIcon = AfxGetApp()->LoadIcon(nIconResourceID);
	};
	virtual void SetIcon(HICON hIcon, bool bBigIcon) {
		CDialogEx::SetIcon(hIcon, bBigIcon);
	};

	// Dialog color functions
	virtual bool CreateBrush(void);
	virtual void GetBkgrdColor(COLORREF& clBkgrdColor) const {
		clBkgrdColor = m_clBkgrdColor;
	};
	virtual void SetBkgrdColor(COLORREF clBkgrdColor) {
		m_clBkgrdColor = clBkgrdColor;
		SetFlagValue(AppFlagID::dialogSetBackgroundColor, true);
	};
	virtual void GetTextColor(COLORREF& clTextColor) const {
		clTextColor = m_clTextColor;
	};
	virtual void SetTextColor(COLORREF clTextColor) {
		m_clTextColor = clTextColor;
		SetFlagValue(AppFlagID::dialogSetTextColor, true);
	};

	// MessageBox functions
	virtual int DisplayMessageBox(unsigned nPromptID, unsigned nCaptionID = NULL, unsigned nStyle = NULL);
	virtual int DisplayMessageBox(const wchar_t* prompt, const wchar_t* caption = NULL, unsigned nStyle = NULL);
	virtual void RegisterMessageBoxCaption(unsigned nCaptionID);
	virtual void RegisterMessageBoxCaption(const wchar_t* caption) {
		m_strMsgCaption = caption;
	};
	virtual void GetMessageBoxCaption(String& regMsgBoxCaption) const {
		regMsgBoxCaption = m_strMsgCaption;
	};

	// Dialog event logging function
	virtual void OutputEventLog(USHORT usEvent, const wchar_t* description = NULL, LOGDETAILINFO* pDetailInfo = NULL);

	// Dialog control event logging functions
	virtual void OutputButtonLog(USHORT usEvent, unsigned nButtonID);
	virtual void OutputCheckBoxLog(USHORT usEvent, unsigned nCheckboxID);
	virtual void OutputRadButtonLog(USHORT usEvent, unsigned nRadButtonID);
	virtual void OutputComboBoxLog(USHORT usEvent, unsigned nComboID);
	virtual void OutputEditBoxLog(USHORT usEvent, unsigned nEditID);
	virtual void OutputListBoxLog(USHORT usEvent, unsigned nListBoxID);
	virtual void OutputSpinCtrlLog(USHORT usEvent, unsigned nSpinCtrlID);
	virtual void OutputMenuLog(USHORT usEvent, unsigned nMenuItemID);

	// Dialog and items setup functions
	virtual void SetupLanguage(void);
	virtual void SetupComboBox(unsigned nComboID, LANGTABLE_PTR pLanguage);
	virtual void SetButtonIcon(unsigned nButtonID, unsigned nIconID, bool bReUpdateTitle = false);
	virtual void UpdateItemText(unsigned nCtrlID, const wchar_t* newCaption);
	virtual void UpdateItemText(unsigned nCtrlID, unsigned nNewCaptionID = NULL, LANGTABLE_PTR ptrLanguage = NULL);
	virtual void SetControlText(CWnd* pCtrlWnd, unsigned nCtrlID, LANGTABLE_PTR ptrLanguage = NULL);

	virtual void MoveItemGroup(const UIntArray& arrCtrlIDGroup, const Point& newPosition);
	virtual void MoveItemGroup(const UIntArray& arrCtrlIDGroup, int nDirection, int nDistance);

	virtual void SetupDialogItemState(void);
	virtual void RefreshDialogItemState(bool bRecheckState = false);

	virtual void ShowItem(CWnd* pDlgItemWnd, bool bVisible);
	virtual void EnableItem(CWnd* pDlgItemWnd, bool bEnabled);
	virtual void ShowItem(unsigned nDlgItemID, bool bVisible) {
		ShowItem(GetDlgItem(nDlgItemID), bVisible);
	};
	virtual void EnableItem(unsigned nDlgItemID, bool bEnabled) {
		EnableItem(GetDlgItem(nDlgItemID), bEnabled);
	};

	// Layout functions
	// TODO: Override these functions for custom actions
	virtual void UpdateLayoutInfo(void) {};
	virtual void LoadLayoutInfo(void) {};
	virtual void SaveLayoutInfo(void) {};

	// Data processing functions
	virtual void UpdateDialogData(bool bSaveAndValidate = true);
	virtual int  GetAppOption(AppOptionID eAppOptionID, bool bTemp = false) const;

	// Dialog flag management
	virtual int  GetFlagValue(AppFlagID eFlagID) const;
	virtual void SetFlagValue(AppFlagID eFlagID, int nValue);
	virtual FlagManager& GetAppFlagManager(void) {
		return ((SWinApp*)AfxGetApp())->GetAppFlagManager();
	};
	virtual const FlagManager& GetAppFlagManager(void) const {
		return ((SWinApp*)AfxGetApp())->GetAppFlagManager();
	};
	virtual FlagManager& GetDialogFlagManager(void) {
		return m_flagManager;
	};
	virtual const FlagManager& GetDialogFlagManager(void) const {
		return m_flagManager;
	};

	// Directly access flag values
	virtual bool GetChangeFlagValue(void) const {
		return m_flagManager.GetFlagValue(AppFlagID::dialogDataChanged);
	};
	virtual void SetChangeFlagValue(bool bValue) {
		m_flagManager.SetFlagValue(AppFlagID::dialogDataChanged, bValue);
	};
	virtual bool CheckDataChangeState(void) {
		return true;
	};
	virtual bool CheckSettingChangeState(void) {
		return true;
	};
	virtual bool IsForceClosingByRequest(void) const {
		return m_flagManager.GetFlagValue(AppFlagID::dialogForceClosing);
	};
	virtual int GetReturnFlag(void) const {
		return m_flagManager.GetFlagValue(AppFlagID::dialogReturnFlag);
	};
	virtual void SetReturnFlag(int nValue) {
		m_flagManager.SetFlagValue(AppFlagID::dialogReturnFlag, nValue);
	};

	// Request processing functions
	virtual LRESULT RequestCloseDialog(void);

	// Descendant dialog functions
	virtual void OpenChildDialogEx(unsigned nDialogID) { (nDialogID); };
	virtual void OpenChildDialogEx(SDialog* pChildDialog) { (pChildDialog); };
	virtual unsigned GetDescendantCount(void) const {
		return m_nDescendantCount;
	};
};

