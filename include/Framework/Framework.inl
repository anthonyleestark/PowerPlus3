
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Framework.inl
//		Description:	Implement definition of inline functions for Stupid Framework classes
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2025.05.26:		Create new for version 3.2
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//// Implementations

#ifdef _SELEMENTS_ENABLE_INLINES

#ifndef _SELEMENTS_INLINE_INCLUDED
#define _SELEMENTS_INLINE_INCLUDED
#endif

// Get access to base control pointer
inline CWnd* SCtrlInfoWrap::GetBaseControl(void) {
	return m_pBaseControl;
}

// Check if the base control window (pointer) is available
inline BOOL SCtrlInfoWrap::IsBaseControlAvailable(void) const {
	return ((m_pBaseControl != NULL) && (m_pBaseControl->GetSafeHwnd() != NULL));
}

// Get current control's parent window
inline CWnd* SCtrlInfoWrap::GetParent(void) {
	return m_pParentWnd;
}

// Set parent window for current control
inline void SCtrlInfoWrap::SetParent(CWnd* pParentWnd) {
	m_pParentWnd = pParentWnd;
}

// Check if parent window (pointer) is available
inline BOOL SCtrlInfoWrap::IsParentAvailable(void) const {
	return ((m_pParentWnd != NULL) && (m_pParentWnd->GetSafeHwnd() != NULL));
}

// Get current control's buddy window
inline CWnd* SCtrlInfoWrap::GetBuddy(void) {
	return m_pBuddyWnd;
}

// Set buddy window for current control
inline void SCtrlInfoWrap::SetBuddy(CWnd* pBuddyWnd) {
	m_pBuddyWnd = pBuddyWnd;
}

// Check if buddy window (pointer) is available
inline BOOL SCtrlInfoWrap::IsBuddyAvailable(void) const {
	return (m_pBuddyWnd != NULL);
}

// Get current control's type
inline INT SCtrlInfoWrap::GetType(void) const {
	return m_nTypeID;
}

// Get current control's template ID
inline UINT SCtrlInfoWrap::GetTemplateID(void) const {
	return m_nTemplateID;
}

// Get current control's template string ID
inline LPCTSTR SCtrlInfoWrap::GetTemplateStringID(void) const {
	return m_strTemplateID;
}

// Get current control's template string ID
inline void SCtrlInfoWrap::GetTemplateStringID(_Out_ CString& strTemplateID) const {
	strTemplateID = m_strTemplateID;
}

// Get current control's caption
inline LPCTSTR SCtrlInfoWrap::GetCaption(void) const {
	return m_strTemplateID;
}

// Get current control's caption
inline void SCtrlInfoWrap::GetCaption(_Out_ CString& strCaption) const {
	strCaption = m_strCaption;
}

// Get current control's visible state
inline BOOL SCtrlInfoWrap::IsVisible(void) const {
	return m_bVisible;
}

// Get current control's enable state
inline BOOL SCtrlInfoWrap::IsEnabled(void) const {
	return m_bEnabled;
}

// Get current control's focus state
inline BOOL SCtrlInfoWrap::IsFocused(void) const {
	return m_bFocused;
}

// Set current control's type
inline void SCtrlInfoWrap::SetType(_In_ INT nTypeID) {
	m_nTypeID = nTypeID;
}

// Set current control's template ID
inline void SCtrlInfoWrap::SetTemplateID(_In_ UINT nTemplateID) {
	m_nTemplateID = nTemplateID;
}

// Set current control's template ID
inline void SCtrlInfoWrap::SetTemplateStringID(_In_z_ LPCTSTR lpszTemplateID) {
	m_strTemplateID = lpszTemplateID;
}

// Set current control's caption
inline void SCtrlInfoWrap::SetCaption(_In_z_ LPCTSTR lpszCaption) {
	m_strCaption = lpszCaption;
}

// Set current control's visible state
inline void SCtrlInfoWrap::SetVisibleState(_In_ BOOL bVisible) {
	m_bVisible = bVisible;
}

// Set current control's enable state
inline void SCtrlInfoWrap::SetEnableState(_In_ BOOL bEnabled) {
	m_bEnabled = bEnabled;
}

// Set current control's focus state
inline void SCtrlInfoWrap::SetFocusedState(_In_ BOOL bFocused) {
	m_bFocused = bFocused;
}

// Get number of controls in management
inline size_t SControlManager::GetCount(void) const
{
	// If data is not initialized
	if (m_pCtrlInfoArray == NULL)
		return 0;

	return m_pCtrlInfoArray->size();
}

// Check if control management list is empty
inline BOOL SControlManager::IsEmpty(void) const 
{
	// If data is not initialized
	if (m_pCtrlInfoArray == NULL)
		return TRUE;

	return m_pCtrlInfoArray->empty();
}

// Get current control's parent window
inline CWnd* SControlManager::GetParent(void) {
	return m_pParentWnd;
}

// Set parent window for current control
inline void SControlManager::SetParent(CWnd* pParentWnd) {
	m_pParentWnd = pParentWnd;
}

// Check if parent window (pointer) is available
inline BOOL SControlManager::IsParentAvailable(void) const {
	return ((m_pParentWnd != NULL) && (m_pParentWnd->GetSafeHwnd() != NULL));
}

#endif		// ifdef _SELEMENTS_ENABLE_INLINES

///////////////////////////////////////////////////////

#ifdef _SWINAPP_ENABLE_INLINES

#ifndef _SWINAPP_INLINE_INCLUDED
#define _SWINAPP_INLINE_INCLUDED
#endif

// Get application current language package pointer
inline LANGTABLE_PTR SWinApp::GetAppLanguage(void)
{
	// Check validity
	VERIFY(m_pAppLangPtr != NULL);
	if (m_pAppLangPtr == NULL)
		return NULL;

	return m_pAppLangPtr;
}

// Get application current language option
inline UINT_PTR SWinApp::GetAppLanguageOption(BOOL bCurDispLang /* = FALSE */) const
{
	// Return currently displaying language
	if (bCurDispLang == TRUE)
		return m_nCurDispLang;

	// Return current setting language
	return m_nCurSetLang;
}

// Set application current language option
inline void SWinApp::SetAppLanguageOption(UINT nSetLanguage) {
	m_nCurSetLang = nSetLanguage;
}

// Get application name
inline LPCTSTR SWinApp::GetAppName(void) const {
	return m_pszAppName;
}

// Get application name
inline void SWinApp::GetAppName(CString& strAppName) const {
	strAppName = m_pszAppName;
}

// Set application name
inline void SWinApp::SetAppName(LPCTSTR lpszAppName)
{
	// First, free default allocated string
	free((void*)m_pszAppName);

	// Assign new application name string
	m_pszAppName = _tcsdup(lpszAppName);
}

// Get application window common caption
inline LPCTSTR SWinApp::GetAppWindowCaption(void) const {
	return m_strWindowCaption.GetString();
}

// Get application window common caption
inline void SWinApp::GetAppWindowCaption(CString& strWindowCaption) const {
	strWindowCaption = m_strWindowCaption;
}

// Register message box caption string
inline void SWinApp::RegisterMessageBoxCaption(LPCTSTR lpszCaption) {
	m_strMessageCaption = lpszCaption;
}

// Get registered message box caption string
inline void SWinApp::GetRegisterdMsgBoxCaption(CString& strRegMsgBoxCap) const {
	strRegMsgBoxCap = m_strMessageCaption;
}

// Get application event logging pointer
inline SLogging* SWinApp::GetAppEventLog(void)
{
	// Check validity
	VERIFY(m_pAppEventLog != NULL);
	return m_pAppEventLog;
}

// Get application flag manager (for writing)
inline FlagManager& SWinApp::GetAppFlagManager(void) {
	return m_flagManager;
}

// Get application flag manager (for read-only)
inline const FlagManager& SWinApp::GetAppFlagManager(void) const {
	return m_flagManager;
}

// Get change flag value
inline BOOL SWinApp::GetChangeFlagValue(void) const {
	return m_bChangeFlag;
}

// Set change flag value
inline void SWinApp::SetChangeFlagValue(BOOL bChangeFlag) {
	m_bChangeFlag = bChangeFlag;
}

// Check if data changed
inline BOOL SWinApp::CheckDataChangeState(void) {
	return TRUE;
}

// Check if settings changed
inline BOOL SWinApp::CheckSettingChangeState(void) {
	return TRUE;
}

// Get read-only mode
inline BOOL SWinApp::GetReadOnlyMode(void) const {
	return m_bReadOnlyMode;
}

// Set read-only mode
inline void SWinApp::SetReadOnlyMode(BOOL bReadOnly) {
	m_bReadOnlyMode = bReadOnly;
}

// Check application force closing flag
inline BOOL SWinApp::IsForceClosingByRequest(void) const {
	return m_bForceClose;
}

// Post error message to application
inline void SWinApp::PostErrorMessage(DWORD dwErrorCode, LPARAM lParam /* = NULL */) {
	PostMessage(NULL, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrorCode, lParam);
}

// Post error message to specified window handle
inline void SWinApp::PostErrorMessage(HWND hRcvWnd, DWORD dwErrorCode, LPARAM lParam /* = NULL */) {
	PostMessage(hRcvWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrorCode, lParam);
}

// Post error message to specified window pointer
inline void SWinApp::PostErrorMessage(CWnd* pRcvWnd, DWORD dwErrorCode, LPARAM lParam /* = NULL */) {
	pRcvWnd->PostMessage(SM_APP_ERROR_MESSAGE, (WPARAM)dwErrorCode, lParam);
}

#endif		// ifdef _SWINAPP_ENABLE_INLINES

///////////////////////////////////////////////////////

#ifdef _SDIALOG_ENABLE_INLINES

#ifndef _SDIALOG_INLINE_INCLUDED
#define _SDIALOG_INLINE_INCLUDED
#endif

// Base functions for implementing dialog resource ID map
inline const size_t SDialog::UpdateResourceIDMap() {
	return UpdateThisResourceIDMap();
}

// Base functions for implementing dialog resource ID map
inline const size_t PASCAL SDialog::UpdateThisResourceIDMap() {
	return GET_RESOURCEID_MAP()->GetMapCount();
}

// Get current dialog's parent window
inline CWnd* SDialog::GetParentWnd(void) {
	return m_pParentWnd;
}

// Set parent window for current dialog
inline void SDialog::SetParentWnd(CWnd* pParentWnd) {
	m_pParentWnd = pParentWnd;
}

// Check if parent window (pointer) is available
inline BOOL SDialog::IsParentWndAvailable(void) const {
	return ((m_pParentWnd != NULL) && (m_pParentWnd->GetSafeHwnd() != NULL));
}

// Get current dialog's tooltip control window
inline CToolTipCtrl* SDialog::GetToolTipCtrl(void) {
	return m_pToolTip;
}

// Check if tooltip control window (pointer) is available
inline BOOL SDialog::IsToolTipCtrlAvailable(void) const {
	return ((m_pToolTip != NULL) && (m_pToolTip->GetSafeHwnd() != NULL));
}

// Get dialog control manager pointer
inline SControlManager* SDialog::GetControlManager(void) {
	return m_pCtrlManager;
}

// Get dialog ID
inline UINT SDialog::GetDialogID(void) const {
	return m_nIDHelp;
}

// Get/set read-only mode
inline BOOL SDialog::GetReadOnlyMode(void) const {
	return m_flagManager.GetFlagValue(AppFlagID::dialogReadOnlyMode);
}

inline void SDialog::SetReadOnlyMode(BOOL bReadOnly) {
	m_flagManager.SetFlagValue(AppFlagID::dialogReadOnlyMode, bReadOnly);
}

// Get/set current item lock state
inline BOOL SDialog::GetLockState(void) const {
	return m_flagManager.GetFlagValue(AppFlagID::dialogLockState);
}

inline void SDialog::SetLockState(BOOL bIsLocked) {
	m_flagManager.SetFlagValue(AppFlagID::dialogLockState, bIsLocked);
}

// Set use Enter button flag
inline void SDialog::SetUseEnter(BOOL bUseEnter) {
	m_flagManager.SetFlagValue(AppFlagID::dialogUseEnterKey, bUseEnter);
}

// Set use Escape button flag
inline void SDialog::SetUseEscape(BOOL bUseEscape) {
	m_flagManager.SetFlagValue(AppFlagID::dialogUseEscapeKey, bUseEscape);
}

// Get dialog alignment flags
inline UINT SDialog::GetAlignment(void) const {
	return m_nAlignment;
}

// Set dialog alignment flags
inline void SDialog::SetAlignment(UINT nAlignment) {
	m_nAlignment = nAlignment;
}

// Get dialog anchor point
inline void SDialog::GetAnchorPoint(LPPOINT lpAnchorPoint) const {
	ASSERT(lpAnchorPoint != NULL);
	if (lpAnchorPoint != NULL) {
		lpAnchorPoint->x = m_ptAnchorPoint.x;
		lpAnchorPoint->y = m_ptAnchorPoint.y;
	}
}

// Set dialog anchor point
inline void SDialog::SetAnchorPoint(POINT ptAnchorPoint)
{
	// Set position
	m_ptAnchorPoint.x = ptAnchorPoint.x;
	m_ptAnchorPoint.y = ptAnchorPoint.y;

	// Move dialog
	MoveDialog(m_ptAnchorPoint);
}

// Set dialog position (combine of anchor point and alignment flags)
inline void SDialog::SetDialogPosition(POINT ptAnchorPoint, UINT nAlignment) {
	SetAlignment(nAlignment);
	SetAnchorPoint(ptAnchorPoint);
}

// Get dialog size
inline void SDialog::GetSize(LPSIZE lpRegSize) const
{
	// If size is not set, return default
	if ((m_szRegisterSize.cx <= 0) && (m_szRegisterSize.cy <= 0)) {
		lpRegSize->cx = m_szDefaultSize.cx;
		lpRegSize->cy = m_szDefaultSize.cy;
	}

	// Return dialog size
	lpRegSize->cx = m_szRegisterSize.cx;
	lpRegSize->cy = m_szRegisterSize.cy;
}

// Set dialog size
inline void SDialog::SetSize(SIZE szRegSize) {
	m_szRegisterSize.cx = szRegSize.cx;
	m_szRegisterSize.cy = szRegSize.cy;
}

// Set dialog size
inline void SDialog::SetSize(LONG lWidth, LONG lHeight)
{
	// Create dialog size data
	CSize szDialogSize(lWidth, lHeight);

	// Set dialog size
	this->SetSize(szDialogSize);
}

// Set dialog min size
inline void SDialog::SetMinSize(LONG lMinWidth, LONG lMinHeight) {
	m_szMinSize = CSize(lMinWidth, lMinHeight);
}

// Set dialog max size
inline void SDialog::SetMaxSize(LONG lMaxWidth, LONG lMaxHeight) {
	m_szMaxSize = CSize(lMaxWidth, lMaxHeight);
}

// Set dialog top-most flag
inline void SDialog::SetTopMost(BOOL bTopMost) {
	SetFlagValue(AppFlagID::dialogSetTopMost, bTopMost);;
}

// Set dialog init sound enable flag
inline void SDialog::SetInitSound(BOOL bInitSound) {
	SetFlagValue(AppFlagID::dialogSetInitSound, bInitSound);
}

// Set content display margin: left
inline void SDialog::SetLeftMargin(LONG lMargin) {
	m_rcClientMargin.left = lMargin;
}

// Set content display margin: top
inline void SDialog::SetTopMargin(LONG lMargin) {
	m_rcClientMargin.top = lMargin;
}

// Set content display margin: right
inline void SDialog::SetRightMargin(LONG lMargin) {
	m_rcClientMargin.right = lMargin;
}

// Set content display margin: bottom
inline void SDialog::SetBottomMargin(LONG lMargin) {
	m_rcClientMargin.bottom = lMargin;
}

// Set content display center margin
inline void SDialog::SetCenterMargin(POINT ptMargin)
{
	// Top left
	m_rcClientMargin.left = ptMargin.x;
	m_rcClientMargin.top = ptMargin.y;

	// Bottom right
	m_rcClientMargin.right = ptMargin.x;
	m_rcClientMargin.bottom = ptMargin.y;
}

// Set content display center margin
inline void SDialog::SetCenterMargin(LONG lHMargin, LONG lVMargin)
{
	// Top left
	m_rcClientMargin.left = lHMargin;
	m_rcClientMargin.top = lVMargin;

	// Bottom right
	m_rcClientMargin.right = lHMargin;
	m_rcClientMargin.bottom = lVMargin;
}

// Get content display area
inline void SDialog::GetDisplayArea(LPRECT lpDispAreaRect) const
{
	// Get client rect
	this->GetClientRect(lpDispAreaRect);

	// Calculate display area with margin
	lpDispAreaRect->left += m_rcClientMargin.left;
	lpDispAreaRect->top += m_rcClientMargin.top;
	lpDispAreaRect->right -= m_rcClientMargin.right;
	lpDispAreaRect->bottom -= m_rcClientMargin.bottom;
}

// Get content display margin
inline void SDialog::GetMargin(LPRECT lpDialogMargin) const
{
	// Top left
	lpDialogMargin->left = m_rcClientMargin.left;
	lpDialogMargin->top = m_rcClientMargin.top;

	// Bottom right
	lpDialogMargin->right = m_rcClientMargin.right;
	lpDialogMargin->bottom = m_rcClientMargin.bottom;
}

// Get dialog caption
inline LPCTSTR SDialog::GetCaption(void) const {
	return m_strCaption;
}

// Get dialog caption
inline void SDialog::GetCaption(CString& strCaption) const {
	strCaption = m_strCaption;
}

// Set dialog caption
inline void SDialog::SetCaption(LPCTSTR lpszCaption)
{
	// Set caption
	m_strCaption = lpszCaption;

	// If dialog is already initialized, trigger updating title
	if (IsWindow(this->m_hWnd)) {
		this->SetWindowText(m_strCaption);
	}
}

// Set icon by resource ID
inline void SDialog::SetIcon(UINT nIconResourceID)
{
	// Reset icon
	::DeleteObject(m_hDefaultIcon);
	m_hDefaultIcon = AfxGetApp()->LoadIcon(nIconResourceID);
}

// Set icon (same as CDialogEx base method)
inline void SDialog::SetIcon(HICON hIcon, BOOL bBigIcon)
{
	// Use default method
	CDialogEx::SetIcon(hIcon, bBigIcon);
}

// Get background color
inline void SDialog::GetBkgrdColor(COLORREF& clBkgrdColor) const {
	clBkgrdColor = m_clBkgrdColor;
}

// Set background color
inline void SDialog::SetBkgrdColor(COLORREF clBkgrdColor)
{
	m_clBkgrdColor = clBkgrdColor;
	SetFlagValue(AppFlagID::dialogSetBackgroundColor, TRUE);
}

// Get text color
inline void SDialog::GetTextColor(COLORREF& clTextColor) const {
	clTextColor = m_clTextColor;
}

// Set text color
inline void SDialog::SetTextColor(COLORREF clTextColor)
{
	m_clTextColor = clTextColor;
	SetFlagValue(AppFlagID::dialogSetTextColor, TRUE);
}

// Register message box caption
inline void SDialog::RegisterMessageBoxCaption(LPCTSTR lpszCaption) {
	m_strMsgCaption = lpszCaption;
}

// Get registered message box caption string
inline void SDialog::GetMessageBoxCaption(CString& strRegMsgBoxCap) const {
	strRegMsgBoxCap = m_strMsgCaption;
}

// Show/hide dialog item with corresponding ID
inline void SDialog::ShowItem(UINT nDlgItemID, BOOL bVisible) {
	ShowItem(GetDlgItem(nDlgItemID), bVisible);
}

// Enable/disable dialog item with corresponding ID
inline void SDialog::EnableItem(UINT nDlgItemID, BOOL bEnabled) {
	EnableItem(GetDlgItem(nDlgItemID), bEnabled);
}

// Update layout info data
// TODO: Override this function for custom actions
inline void SDialog::UpdateLayoutInfo(void) {}

// Load layout info data
// TODO: Override this function for custom actions
inline void SDialog::LoadLayoutInfo(void) {}

// Save layout info data
// TODO: Override this function for custom actions
inline void SDialog::SaveLayoutInfo(void) {}

// Get application flag manager (for writing)
inline FlagManager& SDialog::GetAppFlagManager(void) {
	return ((SWinApp*)AfxGetApp())->GetAppFlagManager();
}

// Get application flag manager (for read-only)
inline const FlagManager& SDialog::GetAppFlagManager(void) const {
	return ((SWinApp*)AfxGetApp())->GetAppFlagManager();
}

// Get dialog-owned flag manager (for writing)
inline FlagManager& SDialog::GetDialogFlagManager(void) {
	return m_flagManager;
}

// Get dialog-owned flag manager (for read-only)
inline const FlagManager& SDialog::GetDialogFlagManager(void) const {
	return m_flagManager;
}

// Get change flag value
inline BOOL SDialog::GetChangeFlagValue(void) const {
	return m_flagManager.GetFlagValue(AppFlagID::dialogDataChanged);
}

// Set change flag value
inline void SDialog::SetChangeFlagValue(BOOL bValue) {
	m_flagManager.SetFlagValue(AppFlagID::dialogDataChanged, bValue);
}

// Check if data changed
inline BOOL SDialog::CheckDataChangeState(void) {
	return TRUE;
}

// Check if settings changed
inline BOOL SDialog::CheckSettingChangeState(void) {
	return TRUE;
}

// Check dialog force closing flag
inline BOOL SDialog::IsForceClosingByRequest(void) const {
	return m_flagManager.GetFlagValue(AppFlagID::dialogForceClosing);
}

// Get dialog returned flag
inline int SDialog::GetReturnFlag(void) const {
	return m_flagManager.GetFlagValue(AppFlagID::dialogReturnFlag);
}

// Set dialog returned flag
inline void SDialog::SetReturnFlag(int nValue) {
	m_flagManager.SetFlagValue(AppFlagID::dialogReturnFlag, nValue);
}

// Open a child dialog with corresponding ID
// TODO: Override this function for custom actions
inline void SDialog::OpenChildDialogEx(UINT nDialogID) { (nDialogID); }

// Open a child dialog via its pointer
// TODO: Override this function for custom actions
inline void SDialog::OpenChildDialogEx(SDialog* pChildDialog) { (pChildDialog); }

// Get the number of descendant dialogs
inline UINT SDialog::GetDescendantCount(void) const {
	return m_nDescendantCount;
}

#endif		// ifdef _SDIALOG_ENABLE_INLINES
