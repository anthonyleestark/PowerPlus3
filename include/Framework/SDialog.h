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
	explicit SDialog(unsigned templateId, CWnd* parentWnd = NULL);		// custom constructor
	explicit SDialog(const wchar_t* templateName, CWnd* parentWnd = NULL);	// custom constructor
	virtual ~SDialog();														// destructor

protected:
	// Dialog resource ID mapping
	static const size_t PASCAL UpdateThisResourceIDMap() {
		return GET_RESOURCEID_MAP()->getMapCount();
	}
	virtual const size_t UpdateResourceIDMap() {
		return UpdateThisResourceIDMap();
	}

	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

protected:
	// Tooltip control
	CToolTipCtrl* tooltipCtrlPtr_;

	// Dialog control management
	SControlManager* controlManagerPtr_;

	// Dialog flag manager
	FlagManager flagManager_;

	// Dialog special flags
	unsigned descendantCount_;

	// Lock state exception ID list
	UIntArray* lockExceptionIdList_;

	// Dialog anchor point
	Point anchorPoint_;

	// Dialog alignment
	unsigned alignment_;

	// Dialog size
	Size registeredSize_;
	Size defaultSize_;

	// Dialog min/max info
	Size minSize_;
	Size maxSize_;

	// Dialog client display margin
	Rect clientMargin_;

	// Other properties
	String  caption_;
	HICON	defaultIcon_;
	String	messageCaption_;

	// Color and graphic objects
	CBrush*  backgroudBrush_;
	COLORREF backgroundColor_;
	COLORREF textColor_;

public:
	// Message handlers
	afx_msg void OnOK();
	afx_msg void OnCancel();

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT state, CWnd* otherWndPtr, BOOL isMinimized);
	afx_msg void OnMouseMove(UINT flags, CPoint point);
	afx_msg LRESULT OnChildDialogInit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChildDialogDestroy(WPARAM wParam, LPARAM lParam);
	virtual void OnGetMinMaxInfo(MINMAXINFO* minMaxInfoPtr);
	virtual BOOL PreTranslateMessage(MSG* messagePtr);
	virtual int	 PreDestroyDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	// Modal processing
	virtual INT_PTR DoModal(void);

	// Parent window functions
	CWnd* getParent(void) {
		return m_pParentWnd;
	}
	void setParent(CWnd* parentWnd) {
		m_pParentWnd = parentWnd;
	}
	bool isParentAvailable(void) const {
		return ((m_pParentWnd != NULL) && (m_pParentWnd->GetSafeHwnd() != NULL));
	}
	bool notifyParent(unsigned message, WPARAM wParam, LPARAM lParam);
	LRESULT sendMessageToParent(unsigned message, WPARAM wParam, LPARAM lParam);

	// Tooltip control functions
	CToolTipCtrl* getToolTipCtrl(void) {
		return tooltipCtrlPtr_;
	}
	bool isTooltipAvailable(void) const {
		return ((tooltipCtrlPtr_ != NULL) && (tooltipCtrlPtr_->GetSafeHwnd() != NULL));
	}

	// Dialog control management functions
	virtual int registerDialogManagement(void);
	virtual void updateDialogManagement(void);
	virtual bool unregisterDialogManagement(void);
	SControlManager* getControlManager(void) {
		return controlManagerPtr_;
	}

	// Dialog style functions
	bool addStyle(DWORD style);
	bool removeStyle(DWORD style);

	// Dialog properties and information
	unsigned getDialogId(void) const {
		return m_nIDHelp;
	}
	bool isReadOnlyMode(void) const {
		return flagManager_.getFlagValue(AppFlagID::dialogReadOnlyMode);
	}
	void setReadOnlyMode(bool flagValue) {
		flagManager_.setFlagValue(AppFlagID::dialogReadOnlyMode, flagValue);
	}
	bool getLockState(void) const {
		return flagManager_.getFlagValue(AppFlagID::dialogLockState);
	}
	void setLockState(bool flagValue) {
		flagManager_.setFlagValue(AppFlagID::dialogLockState, flagValue);
	}
	void setUseEnter(bool flagValue) {
		flagManager_.setFlagValue(AppFlagID::dialogUseEnterKey, flagValue);
	}
	void setUseEscape(bool flagValue) {
		flagManager_.setFlagValue(AppFlagID::dialogUseEscapeKey, flagValue);
	}

	// Lock state exception ID list
	virtual void addLockStateException(unsigned id);
	virtual void removeLockStateException(unsigned id);
	virtual void resetLockStateExceptionList(void);

	// Dialog align and position
	unsigned getAlignment(void) const {
		return alignment_;
	}
	void setAlignment(unsigned value) {
		alignment_ = value;
	}
	void getAnchorPoint(Point& anchorPoint) const {
		anchorPoint = anchorPoint_;
	}
	void setAnchorPoint(const Point& anchorPoint) {
		anchorPoint_ = anchorPoint;

		// Move dialog
		move(anchorPoint_);
	}
	void setDialogPosition(const Point& anchorPoint, unsigned alignment) {
		setAlignment(alignment);
		setAnchorPoint(anchorPoint);
	}

	// Move and resize dialog
	void move(const Point& position, Rect* newRect = nullptr);
	void move(long dx, long dy, Rect* newRect = nullptr);
	void resize(bool isCentered);
	void resetSize(void);

	// Get/set dialog size functions
	void getSize(Size& regSize) const {
		// If size is not set, return default
		if (registeredSize_.isEmpty()) regSize = defaultSize_;

		// Return dialog size
		regSize = registeredSize_;
	}
	void setSize(const Size& regSize) {
		registeredSize_ = regSize;
	}
	void setSize(long width, long height)	{
		Size dialogSize(width, height);
		this->setSize(dialogSize);
	}
	void setMinSize(long minWidth, long minHeight) {
		minSize_ = Size(minWidth, minHeight);
	}
	void setMaxSize(long maxWidth, long maxHeight) {
		maxSize_ = Size(maxWidth, maxHeight);
	}

	// Other properties
	void setTopMost(bool flagValue) {
		setFlagValue(AppFlagID::dialogSetTopMost, flagValue);;
	}
	void setInitSound(bool flagValue) {
		setFlagValue(AppFlagID::dialogSetInitSound, flagValue);
	}

	// Dialog margins and display area
	void setLeftMargin(long margin) {
		clientMargin_._left = margin;
	}
	void setTopMargin(long margin) {
		clientMargin_._top = margin;
	}
	void setRightMargin(long margin) {
		clientMargin_._right = margin;
	}
	void setBottomMargin(long margin) {
		clientMargin_._bottom = margin;
	}
	void setCenterMargin(const Point& margin) {
		clientMargin_.setTopLeft(margin);
		clientMargin_.setBottomRight(margin);
	}
	void setCenterMargin(long horizontalMargin, long verticalMargin) {
		clientMargin_.setTopLeft(horizontalMargin, verticalMargin);
		clientMargin_.setBottomRight(horizontalMargin, verticalMargin);
	}
	void getDisplayArea(Rect& displayAreaRect) const {
		RECT rcTemp; this->GetClientRect(&rcTemp);
		displayAreaRect = Rect(rcTemp.left, rcTemp.top, rcTemp.right, rcTemp.bottom);

		// Calculate display area with margin
		displayAreaRect._left += clientMargin_.left();
		displayAreaRect._top += clientMargin_.top();
		displayAreaRect._right -= clientMargin_.right();
		displayAreaRect._bottom -= clientMargin_.bottom();
	}
	void setDisplayArea(const Rect& newDispArea, bool isResized, bool isCentered);
	void getMargin(Rect& dialogMargin) const {
		dialogMargin = clientMargin_;
	}

	// Dialog caption get/set functions
	void setCaptionFromResource(unsigned resourceStringId);
	void setCaptionFromLanguage(unsigned langStringId);
	const wchar_t* getCaption(void) const {
		return caption_;
	}
	void getCaption(String& caption) const {
		caption = caption_;
	}
	void setCaption(const wchar_t* caption)	{
		caption_ = caption;

		// If dialog is already initialized, trigger updating title
		if (IsWindow(this->m_hWnd)) {
			this->SetWindowText(caption_);
		}
	}

	// Dialog icon functions
	void setIcon(unsigned iconResourceId) {
		::DeleteObject(defaultIcon_);
		defaultIcon_ = AfxGetApp()->LoadIcon(iconResourceId);
	}
	void setIcon(HICON hIcon, bool isBig) {
		CDialogEx::SetIcon(hIcon, isBig);
	}

	// Dialog color functions
	bool createBrush(void);
	void getBackgroundColor(COLORREF& colorBackground) const {
		colorBackground = backgroundColor_;
	}
	void setBackgroundColor(COLORREF colorBackground) {
		backgroundColor_ = colorBackground;
		setFlagValue(AppFlagID::dialogSetBackgroundColor, true);
	}
	void getTextColor(COLORREF& colorText) const {
		colorText = textColor_;
	}
	void setTextColor(COLORREF colorText) {
		textColor_ = colorText;
		setFlagValue(AppFlagID::dialogSetTextColor, true);
	}

	// MessageBox functions
	int displayMessageBox(unsigned promptId, unsigned captionId = NULL, unsigned style = NULL);
	int displayMessageBox(const wchar_t* prompt, const wchar_t* caption = NULL, unsigned style = NULL);
	void registerMessageBoxCaption(unsigned captionId);
	void registerMessageBoxCaption(const wchar_t* caption) {
		messageCaption_ = caption;
	}
	void getMessageBoxCaption(String& regMsgBoxCaption) const {
		regMsgBoxCaption = messageCaption_;
	}

	// Dialog event logging function
	void outputEventLog(USHORT eventId, const wchar_t* description = NULL, LOGDETAILINFO* detailInfoPtr = NULL);

	// Dialog control event logging functions
	void outputButtonLog(USHORT eventId, unsigned buttonId);
	void outputCheckBoxLog(USHORT eventId, unsigned checkboxId);
	void outputRadButtonLog(USHORT eventId, unsigned radButtonId);
	void outputComboBoxLog(USHORT eventId, unsigned comboId);
	void outputEditBoxLog(USHORT eventId, unsigned editId);
	void outputListBoxLog(USHORT eventId, unsigned listBoxId);
	void outputSpinCtrlLog(USHORT eventId, unsigned spinCtrlId);
	void outputMenuLog(USHORT eventId, unsigned menuItemId);

	// Dialog and items setup functions
	virtual void setupLanguage(void);
	virtual void setupComboBox(unsigned comboId, LANGTABLE_PTR langTablePtr);
	void setButtonIcon(unsigned buttonId, unsigned iconId, bool reupdateTitle = false);
	void updateItemText(unsigned controlId, const wchar_t* newCaption);
	void updateItemText(unsigned controlId, unsigned newCaptionId = NULL, LANGTABLE_PTR langTablePtr = NULL);
	void setControlText(CWnd* controlPtr, unsigned controlId, LANGTABLE_PTR langTablePtr = NULL);

	void moveItemGroup(const UIntArray& controlIdGroup, const Point& newPosition);
	void moveItemGroup(const UIntArray& controlIdGroup, int direction, int distance);

	virtual void setupDialogItemState(void);
	virtual void refreshDialogItemState(bool recheckState = false);

	void showItem(CWnd* itemPtr, bool isVisible);
	void enableItem(CWnd* itemPtr, bool isEnabled);
	void showItem(unsigned itemId, bool isVisible) {
		showItem(GetDlgItem(itemId), isVisible);
	}
	void enableItem(unsigned itemId, bool isEnabled) {
		enableItem(GetDlgItem(itemId), isEnabled);
	}

	// Layout functions
	// TODO: Override these functions for custom actions
	virtual void updateLayoutInfo(void) {}
	virtual void loadLayoutInfo(void) {}
	virtual void saveLayoutInfo(void) {}

	// Data processing functions
	virtual void updateDialogData(bool saveAndValidate = true);
	virtual int  getAppOption(AppOptionID optionId, bool isTemp = false) const;

	// Dialog flag management
	virtual int  getFlagValue(AppFlagID flagId) const;
	virtual void setFlagValue(AppFlagID flagId, int value);
	FlagManager& getAppFlagManager(void) {
		return ((SWinApp*)AfxGetApp())->getAppFlagManager();
	}
	const FlagManager& getAppFlagManager(void) const {
		return ((SWinApp*)AfxGetApp())->getAppFlagManager();
	}
	FlagManager& getDialogFlagManager(void) {
		return flagManager_;
	}
	const FlagManager& getDialogFlagManager(void) const {
		return flagManager_;
	}

	// Directly access flag values
	bool getChangeFlagValue(void) const {
		return flagManager_.getFlagValue(AppFlagID::dialogDataChanged);
	}
	void setChangeFlagValue(bool value) {
		flagManager_.setFlagValue(AppFlagID::dialogDataChanged, value);
	}
	virtual bool checkDataChangeState(void) {
		return true;
	}
	virtual bool checkSettingChangeState(void) {
		return true;
	}
	bool isForceClosingByRequest(void) const {
		return flagManager_.getFlagValue(AppFlagID::dialogForceClosing);
	}
	int getReturnFlag(void) const {
		return flagManager_.getFlagValue(AppFlagID::dialogReturnFlag);
	}
	void setReturnFlag(int value) {
		flagManager_.setFlagValue(AppFlagID::dialogReturnFlag, value);
	}

	// Request processing functions
	virtual LRESULT requestCloseDialog(void);

	// Descendant dialog functions
	virtual void openChildDialogEx(unsigned dialogId) { (dialogId); }
	virtual void openChildDialogEx(SDialog* dialogPtr) { (dialogPtr); }
	unsigned getDescendantCount(void) const {
		return descendantCount_;
	}
};

