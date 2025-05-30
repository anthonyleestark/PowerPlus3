﻿
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Dialogs.inl
//		Description:	Implement definition of inline functions for Dialog classes
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2025.05.26:		Create new for version 3.2
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// This file is allowed to be included in multiple translation units
#ifndef _DIALOGS_INL_INCLUDED
#define _DIALOGS_INL_INCLUDED
#endif


///////////////////////////////////////////////////////
//// Implementations

#ifdef _HELPDLG_ENABLE_INLINES

#ifndef _HELPDLG_INLINE_INCLUDED
#define _HELPDLG_INLINE_INCLUDED
#endif

// Get view mode
inline UINT CHelpDlg::GetViewMode() const {
	return m_nViewMode;
}

// Set view mode
inline void CHelpDlg::SetViewMode(UINT nViewMode) {
	m_nViewMode = nViewMode;
}

#endif		// ifdef _HELPDLG_ENABLE_INLINES

///////////////////////////////////////////////////////

#ifdef _MULTISCHEDULEDLG_ENABLE_INLINES

#ifndef _MULTISCHEDULEDLG_INLINE_INCLUDED
#define _MULTISCHEDULEDLG_INLINE_INCLUDED
#endif

// Get data table total item number (including default item)
inline int CMultiScheduleDlg::GetTotalItemNum() const {
	return GetExtraItemNum() + DEF_SCHEDULE_DEFAULT_ITEMNUM;
}

// Get data table extra item number only
inline int CMultiScheduleDlg::GetExtraItemNum() const {
	return m_schScheduleTemp.GetExtraItemNum();
}

#endif		// ifdef _MULTISCHEDULEDLG_ENABLE_INLINES

///////////////////////////////////////////////////////

#ifdef _HOTKEYSETDLG_ENABLE_INLINES

#ifndef _HOTKEYSETDLG_INLINE_INCLUDED
#define _HOTKEYSETDLG_INLINE_INCLUDED
#endif

// Get data table item number
inline int CHotkeySetDlg::GetItemNum() const {
	return m_hksHotkeySetTemp.GetItemNum();
}

// Return list control current selected item index
inline int CHotkeySetDlg::GetListCurSel(void) const {
	return m_nCurSelIndex;
}

// Update list control current selected item index
inline void CHotkeySetDlg::SetListCurSel(int nSelIndex) {
	m_nCurSelIndex = nSelIndex;
}

#endif		// ifdef _HOTKEYSETDLG_ENABLE_INLINES

///////////////////////////////////////////////////////

#ifdef _PWRREMINDERDLG_ENABLE_INLINES

#ifndef _PWRREMINDERDLG_INLINE_INCLUDED
#define _PWRREMINDERDLG_INLINE_INCLUDED
#endif

// Get data table item number
inline int CPwrReminderDlg::GetItemNum() const {
	return m_pwrReminderDataTemp.GetItemNum();
}

inline int CPwrReminderDlg::GetCurMode() const {
	return m_nCurMode;
}

#endif		// ifdef _PWRREMINDERDLG_ENABLE_INLINES

///////////////////////////////////////////////////////

#ifdef _REMINDERMSGDLG_ENABLE_INLINES

#ifndef _REMINDERMSGDLG_INLINE_INCLUDED
#define _REMINDERMSGDLG_INLINE_INCLUDED
#endif

// Get display message
inline LPCTSTR CReminderMsgDlg::GetDispMessage(void) const {
	return m_strBuffer.GetString();
}
// Set display message
inline void CReminderMsgDlg::SetDispMessage(LPCTSTR lpszDispMsg) {
	m_strBuffer = lpszDispMsg;
}

// Get auto closing message interval
inline UINT CReminderMsgDlg::GetAutoCloseInterval(void) const {
	return m_nAutoCloseInterval;
}

// Set auto closing message interval
inline void CReminderMsgDlg::SetAutoCloseInterval(UINT nSeconds) {
	m_nAutoCloseInterval = nSeconds;
}

// Set dialog size
inline void CReminderMsgDlg::SetSize(CSize szRegSize)
{
	// Set dialog size
	SDialog::SetSize(szRegSize);

	// Set lock size flag
	m_bLockDlgSize = TRUE;
}

// Set dialog size
inline void CReminderMsgDlg::SetSize(LONG lWidth, LONG lHeight)
{
	// Set dialog size
	SDialog::SetSize(lWidth, lHeight);

	// Set lock size flag
	m_bLockDlgSize = TRUE;
}

// Get message font name
inline void CReminderMsgDlg::GetMsgFontName(CString& strFontName) const
{
	// Do not return if font is invalid
	if (m_pMsgFont == NULL)
		return;

	// Return font name
	strFontName = m_strMsgFontName;
}

// Get message font point
inline void CReminderMsgDlg::GetMsgFontPoint(float& fFontPoint) const
{
	// Do not return if font is invalid
	if (m_pMsgFont == NULL)
		return;

	// Return font point
	fFontPoint = m_fMsgFontPoint;
}

// Set message icon placement
inline void CReminderMsgDlg::SetMsgIconPosition(BYTE byPosition) {
	m_byIconPosition = byPosition;
}

// Get allow snoozing mode enable status
inline BOOL CReminderMsgDlg::GetAllowSnoozeMode(void) const {
	return m_bAllowSnooze;
}

// Set allow snoozing mode enable status
inline void CReminderMsgDlg::SetAllowSnoozeMode(BOOL bValue) {
	m_bAllowSnooze = bValue;
}

// Get snooze mode trigger flag value
inline void CReminderMsgDlg::GetSnoozeTriggerFlag(int& nValue) const {
	nValue = m_nSnoozeFlag;
}

// Set snooze mode trigger flag value
inline void CReminderMsgDlg::SetSnoozeTriggerFLag(int nValue) {
	m_nSnoozeFlag = nValue;
}

#endif		// ifdef _REMINDERMSGDLG_ENABLE_INLINES

///////////////////////////////////////////////////////

#ifdef _RMDREPEATSETDLG_ENABLE_INLINES

#ifndef _RMDREPEATSETDLG_INLINE_INCLUDED
#define _RMDREPEATSETDLG_INLINE_INCLUDED
#endif

// Get snooze interval current value
inline int CRmdRepeatSetDlg::GetSnoozeInterval() const {
	return m_nSnoozeInterval;
}

// Set snooze interval current value
inline void CRmdRepeatSetDlg::SetSnoozeInterval(int nValue) {
	m_nSnoozeInterval = nValue;
}

#endif		// ifdef _RMDREPEATSETDLG_ENABLE_INLINES

///////////////////////////////////////////////////////

#ifdef _DEBUGTESTDLG_ENABLE_INLINES

#ifndef _DEBUGTESTDLG_INLINE_INCLUDED
#define _DEBUGTESTDLG_INLINE_INCLUDED
#endif

// Get access to the DebugTest edit view pointer
inline CEdit* CDebugTestDlg::GetDebugEditView(void) const {
	return m_pDebugEditView;
}

// Check if the DebugTest edit view (pointer) is valid
inline BOOL CDebugTestDlg::IsDebugEditViewValid(void) {
	return (GetDebugEditView() != NULL);
}

// Check if the DebugTest edit view is focused
inline BOOL CDebugTestDlg::IsDebugEditViewFocus(void)
{
	// Check DebugTest edit view validity
	if (!IsDebugEditViewValid())
		return FALSE;

	// Check if it is focused
	HWND hCurFocusWnd = GetFocus()->GetSafeHwnd();
	return (hCurFocusWnd == GetDebugEditView()->GetSafeHwnd());
}

// Backup debug view screen buffer content
inline void CDebugTestDlg::BackupDebugViewBuffer(void) {
	m_strBufferBak = m_strBuffer;
}

// Clear current debug command history
inline void CDebugTestDlg::ClearDebugCommandHistory(void) {
	m_astrCommandHistory.clear();
}

// Get debug command history item count
inline size_t CDebugTestDlg::GetDebugCommandHistoryCount(void) const {
	return m_astrCommandHistory.size();
}

// Check if current command history is empty or not
inline BOOL CDebugTestDlg::IsDebugCommandHistoryEmpty(void) const {
	return m_astrCommandHistory.empty();
}

// Check if currently displaying command history or not
inline BOOL CDebugTestDlg::IsCurrentlyDispHistory(void) const {
	return m_bCurDispHistory;
}

// Set currently displaying command history state
inline void CDebugTestDlg::SetCurrentlyDispHistoryState(BOOL bState) {
	m_bCurDispHistory = bState;
}

// Get command history current displaying index
inline size_t CDebugTestDlg::GetHistoryCurrentDispIndex(void) const {
	return m_nHistoryCurIndex;
}

// Set command history current displaying index
inline void CDebugTestDlg::SetHistoryCurrentDispIndex(size_t nCurIndex) {
	m_nHistoryCurIndex = nCurIndex;
}

#endif		// ifdef _DEBUGTESTDLG_ENABLE_INLINES
