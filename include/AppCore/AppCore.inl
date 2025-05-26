
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		AppCore.inl
//		Description:	Implement definition of inline functions for the application core
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.07.26:		Create new for version 3.1
//						<1> 2025.25.26:		Rename from PowerPlus.inl to AppCore.inl
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _APPCORE_INL_INCLUDED
#define _APPCORE_INL_INCLUDED


///////////////////////////////////////////////////////
//// Implementations

// Check if repeat option is enabled
inline BOOL PwrRepeatSet::IsRepeatEnabled(void) const {
	return m_bRepeat;
}

// Check if snooze option is enabled
inline BOOL PwrRepeatSet::IsAllowSnoozing(void) const {
	return m_bAllowSnooze;
}

// Get snooze interval data
inline INT PwrRepeatSet::GetSnoozeInterval(void) const {
	return m_nSnoozeInterval;
}

// Get repeat days data
inline BYTE PwrRepeatSet::GetActiveDays(void) const {
	return m_byRepeatDays;
}

// Check if day of week is active
inline BOOL PwrRepeatSet::IsDayActive(DAYOFWEEK dayOfWeek) const
{
	// Invalid day of week
	if ((dayOfWeek < SUNDAY) || (dayOfWeek > SATURDAY))
		return FALSE;

	return ((m_byRepeatDays & (1 << dayOfWeek)) >> dayOfWeek);
}

// Set repeat enable state
inline void PwrRepeatSet::EnableRepeat(BOOL bEnabled) {
	m_bRepeat = bEnabled;
}

// Set allow snoozing state
inline void PwrRepeatSet::EnableSnoozing(BOOL bEnabled) {
	m_bAllowSnooze = bEnabled;
}

// Set snooze interval data
inline void PwrRepeatSet::SetSnoozeInterval(INT nValue) {
	m_nSnoozeInterval = nValue;
}

// Set repeat days data
inline void PwrRepeatSet::SetActiveDays(BYTE byActiveDays) {
	m_byRepeatDays = byActiveDays;
}

// Set active state for specific day of week
inline void PwrRepeatSet::SetDayActive(DAYOFWEEK dayOfWeek, BOOL bActive)
{
	// Invalid day of week
	if ((dayOfWeek < SUNDAY) || (dayOfWeek > SATURDAY))
		return;

	m_byRepeatDays |= bActive << dayOfWeek;
}

// Get schedule item ID
inline UINT ScheduleItem::GetItemID(void) const {
	return m_nItemID;
}

// Set schedule item ID
inline void ScheduleItem::SetItemID(UINT nItemID) {
	m_nItemID = nItemID;
}

// Check if item is enabled
inline BOOL ScheduleItem::IsEnabled(void) const {
	return m_bEnabled;
}

// Set/change item active state
inline void ScheduleItem::EnableItem(BOOL bEnabled) {
	m_bEnabled = bEnabled;
}

// Get schedule item action ID
inline UINT ScheduleItem::GetAction(void) const {
	return m_nActionID;
}

// Set schedule item action ID
inline void ScheduleItem::SetAction(UINT nActionID) {
	m_nActionID = nActionID;
}

// Get schedule item time data
inline SYSTEMTIME ScheduleItem::GetTime(void) const {
	return m_stTime;
}

// Set schedule item time data
inline void ScheduleItem::SetTime(const SYSTEMTIME& stTime) {
	m_stTime = stTime;
}

// Check if repeat is enabled
inline BOOL ScheduleItem::IsRepeatEnabled(void) const {
	return (m_rpsRepeatSet.IsRepeatEnabled());
}

// Check if item snooze mode is available
inline BOOL ScheduleItem::IsAllowSnoozing(void) const
{
	// If repeat option is currently OFF
	if (IsRepeatEnabled() != TRUE) {
		// Not allow snooze mode
		return FALSE;
	}
	// If allow snooze option is OFF
	if (m_rpsRepeatSet.IsAllowSnoozing() != TRUE) {
		// Not allow snooze mode
		return FALSE;
	}

	return TRUE;
}

// Check if day of week is active
inline BOOL ScheduleItem::IsDayActive(DAYOFWEEK dayOfWeek) const {
	return (m_rpsRepeatSet.IsDayActive(dayOfWeek));
}

// Get repeat active days
inline BYTE ScheduleItem::GetActiveDays(void) const {
	return (m_rpsRepeatSet.GetActiveDays());
}

// Set repeat enable state
inline void ScheduleItem::EnableRepeat(BOOL bEnabled) {
	m_rpsRepeatSet.EnableRepeat(bEnabled);
}

// Set allow snoozing state
inline void ScheduleItem::EnableSnoozing(BOOL bEnabled) {
	m_rpsRepeatSet.EnableSnoozing(bEnabled);
}

// Set snooze interval data
inline void ScheduleItem::SetSnoozeInterval(INT nValue) {
	m_rpsRepeatSet.SetSnoozeInterval(nValue);
}

// Set repeat days data
inline void ScheduleItem::SetActiveDays(BYTE byActiveDays) {
	m_rpsRepeatSet.SetActiveDays(byActiveDays);
}

// Set active state for specific day of week
inline void ScheduleItem::SetDayActive(DAYOFWEEK dayOfWeek, BOOL bActive) {
	m_rpsRepeatSet.SetDayActive(dayOfWeek, bActive);
}

// Get the default Action Schedule item
inline const SCHEDULEITEM& ScheduleData::GetDefaultItem(void) const {
	return m_schDefaultItem;
}

// Get the default Action Schedule item
inline SCHEDULEITEM& ScheduleData::GetDefaultItem(void) {
	return m_schDefaultItem;
}

// Get the Action Schedule item at index
inline const SCHEDULEITEM& ScheduleData::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < GetExtraItemNum()));
	if ((nIndex >= 0) && (nIndex < GetExtraItemNum()))
		return m_arrSchedExtraItemList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Get the Action Schedule item at index
inline SCHEDULEITEM& ScheduleData::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < GetExtraItemNum()));
	if ((nIndex >= 0) && (nIndex < GetExtraItemNum()))
		return m_arrSchedExtraItemList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Get number of extra items
inline INT_PTR ScheduleData::GetExtraItemNum(void) const {
	return m_arrSchedExtraItemList.GetSize();
}

// Check if default item is empty
inline BOOL ScheduleData::IsDefaultEmpty(void) const {
	return m_schDefaultItem.IsEmpty();
}

// Check if item at index is empty
inline BOOL ScheduleData::IsEmpty(int nIndex) const
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= GetExtraItemNum()))
		return TRUE;

	// Check if item is empty
	const SCHEDULEITEM& schItem = GetItemAt(nIndex);
	return schItem.IsEmpty();
}

// Check if all item are empty
inline BOOL ScheduleData::IsAllEmpty(void) const {
	return (IsDefaultEmpty() && IsExtraEmpty());
}

// Check if HotkeySet item is empty
inline BOOL HotkeySetItem::IsEmpty(void) const
{
	BOOL bIsEmpty = FALSE;

	// Check if keystroke value is empty
	bIsEmpty &= (m_dwCtrlKeyCode == 0);
	bIsEmpty &= (m_dwFuncKeyCode == 0);

	return bIsEmpty;
}

// Check if item is enabled
inline BOOL HotkeySetItem::IsEnabled(void) const {
	return m_bEnabled;
}

// Set item enable state
inline void HotkeySetItem::EnableItem(BOOL bEnabled) {
	m_bEnabled = bEnabled;
}

// Get Hotkey action ID
inline UINT HotkeySetItem::GetActionID(void) const {
	return m_nHKActionID;
}

// Set Hotkey action ID
inline void HotkeySetItem::SetActionID(UINT nHKActionID) {
	m_nHKActionID = nHKActionID;
}

// Get item keycode data
inline void HotkeySetItem::GetKeyCode(DWORD& dwCtrlKey, DWORD& dwFuncKey) const
{
	dwCtrlKey = m_dwCtrlKeyCode;
	dwFuncKey = m_dwFuncKeyCode;
}

// Set item keycode data
inline void HotkeySetItem::SetKeyCode(DWORD dwCtrlKey, DWORD dwFuncKey)
{
	m_dwCtrlKeyCode = dwCtrlKey;
	m_dwFuncKeyCode = dwFuncKey;
}

// Init HotkeySet data (NULL)
inline void HotkeySetData::Init() {
	m_arrHotkeySetList.RemoveAll();
}

// Get number of items
inline INT_PTR HotkeySetData::GetItemNum(void) const {
	return m_arrHotkeySetList.GetSize();
}

// Check if item at index is empty
inline BOOL HotkeySetData::IsEmpty(int nIndex) const
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= GetItemNum()))
		return TRUE;

	// Get item data
	const HOTKEYSETITEM& hksItem = GetItemAt(nIndex);

	// Check if item keystroke value is empty
	return hksItem.IsEmpty();
}

// Get the hotkeyset item at index
inline const HOTKEYSETITEM& HotkeySetData::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrHotkeySetList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Get the hotkeyset item at index
inline HOTKEYSETITEM& HotkeySetData::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrHotkeySetList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Delete all HotkeySet data
inline void HotkeySetData::DeleteAll(void)
{
	// Reset data
	m_arrHotkeySetList.RemoveAll();
	m_arrHotkeySetList.FreeExtra();
}

// Get access to item RepeatSet info data
inline PWRREPEATSET& PwrReminderItem::GetRepeatSetData(void) {
	return this->m_rpsRepeatSet;
}

// Reset RepeatSet info data
inline void PwrReminderItem::ResetRepeatInfo(void)
{
	// Create a new default RepeatSet info data
	const PwrRepeatSet emptyData = PWRREPEATSET();

	// Copy and overwrite current data
	this->m_rpsRepeatSet.Copy(emptyData);
}

// Get access to item Message Style info data
inline RMDMSGSTYLESET& PwrReminderItem::GetMessageStyleData(void) {
	return this->m_rmsMsgStyleSet;
}

// Reset Message Style info data
inline void PwrReminderItem::ResetMessageStyleInfo(void)
{
	// Create a new default Message Style info data
	const RmdMsgStyleSet emptyData = RMDMSGSTYLESET();

	// Copy and overwrite current data
	this->m_rmsMsgStyleSet.Copy(emptyData);
}

// Check if item is enabled
inline BOOL PwrReminderItem::IsEnabled(void) const {
	return m_bEnabled;
}

// Set/change item enable state
inline void PwrReminderItem::EnableItem(BOOL bEnabled) {
	m_bEnabled = bEnabled;
}

// Get Power Reminder item ID
inline UINT PwrReminderItem::GetItemID(void) const {
	return m_nItemID;
}

// Set Power Reminder item ID
inline void PwrReminderItem::SetItemID(UINT nItemID) {
	m_nItemID = nItemID;
}

// Get Power Reminder item message content
inline LPCTSTR PwrReminderItem::GetMessage(void) const {
	return m_strMessage.GetString();
}

// Set Power Reminder item message content
inline void PwrReminderItem::SetMessage(LPCTSTR lpszMessage) {
	m_strMessage = lpszMessage;
}

// Get Power Reminder item event ID
inline UINT PwrReminderItem::GetEventID(void) const {
	return m_nEventID;
}

// Set Power Reminder item event ID
inline void PwrReminderItem::SetEventID(UINT nEventID) {
	m_nEventID = nEventID;
}

// Get Power Reminder item time data
inline SYSTEMTIME PwrReminderItem::GetTime(void) const {
	return m_stTime;
}

// Set Power Reminder item time data
inline void PwrReminderItem::SetTime(const SYSTEMTIME& stTime) {
	m_stTime = stTime;
}

// Get Power Reminder item message style ID
inline DWORD PwrReminderItem::GetMessageStyle(void) const {
	return m_dwMsgStyle;
}

// Set Power Reminder item message style ID
inline void PwrReminderItem::SetMessageStyle(DWORD nMsgStyleID) {
	m_dwMsgStyle = nMsgStyleID;
}

// Check if item message custom style is enabled
inline BOOL PwrReminderItem::IsCustomStyleEnabled(void) const {
	return m_bUseCustomStyle;
}

// Set/change item message custom style enable state
inline void PwrReminderItem::EnableCustomStyle(BOOL bEnabled) {
	m_bUseCustomStyle = bEnabled;
}

// Check if item repeat mode is enabled
inline BOOL PwrReminderItem::IsRepeatEnabled(void) const {
	return m_rpsRepeatSet.IsRepeatEnabled();
}

// Check if day of week is active
inline BOOL PwrReminderItem::IsDayActive(DAYOFWEEK dayOfWeek) const {
	return m_rpsRepeatSet.IsDayActive(dayOfWeek);
}

// Get item snooze interval value
inline INT PwrReminderItem::GetSnoozeInterval(void) const {
	return m_rpsRepeatSet.GetSnoozeInterval();
}

// Get repeat active days
inline BYTE PwrReminderItem::GetActiveDays(void) const {
	return m_rpsRepeatSet.GetActiveDays();
}

// Set repeat enable state
inline void PwrReminderItem::EnableRepeat(BOOL bEnabled) {
	m_rpsRepeatSet.EnableRepeat(bEnabled);
}

// Set allow snoozing state
inline void PwrReminderItem::EnableSnoozing(BOOL bEnabled) {
	m_rpsRepeatSet.EnableSnoozing(bEnabled);
}

// Set snooze interval data
inline void PwrReminderItem::SetSnoozeInterval(INT nValue) {
	m_rpsRepeatSet.SetSnoozeInterval(nValue);
}

// Set repeat days data
inline void PwrReminderItem::SetActiveDays(BYTE byActiveDays) {
	m_rpsRepeatSet.SetActiveDays(byActiveDays);
}

// Set active state for specific day of week
inline void PwrReminderItem::SetDayActive(DAYOFWEEK dayOfWeek, BOOL bActive) {
	m_rpsRepeatSet.SetDayActive(dayOfWeek, bActive);
}

// Get number of items
inline INT_PTR PwrReminderData::GetItemNum(void) const {
	return m_arrRmdItemList.GetSize();
}

// Get the Power Reminder item at index
inline PWRREMINDERITEM& PwrReminderData::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrRmdItemList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Get the Power Reminder item at index
inline const PWRREMINDERITEM& PwrReminderData::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrRmdItemList.GetAt(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Check if item at index is empty
inline BOOL PwrReminderData::IsEmpty(int nIndex) const
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= GetItemNum()))
		return TRUE;

	// Check if item is empty
	const PWRREMINDERITEM& pwrItem = GetItemAt(nIndex);
	return pwrItem.IsEmpty();
}

// Delete all Power Reminder data
inline void PwrReminderData::DeleteAll(void)
{
	// Reset data
	m_arrRmdItemList.RemoveAll();
	m_arrRmdItemList.FreeExtra();
	m_rmdCommonStyle = RMDMSGSTYLESET();
}

// Calculate next snooze time
inline void PwrRuntimeItem::CalcNextSnoozeTime(int nInterval)
{
	// Calculate time with offset
	AppCore::CalcTimeOffset(m_stNextSnoozeTime, nInterval);
}

// Get item category
inline INT PwrRuntimeItem::GetCategory(void) const {
	return m_nCategory;
}

// Set item category
inline void PwrRuntimeItem::SetCategory(INT nValue) {
	m_nCategory = nValue;
}

// Get item ID
inline UINT PwrRuntimeItem::GetItemID(void) const {
	return m_nItemID;
}

// Set item ID
inline void PwrRuntimeItem::SetItemID(UINT nValue) {
	m_nItemID = nValue;
}

// Get item displaying flag
inline INT PwrRuntimeItem::GetDisplayFlag(void) const {
	return m_nDisplayFlag;
}

// Set item displaying flag
inline void PwrRuntimeItem::SetDisplayFlag(INT nValue) {
	m_nDisplayFlag = nValue;
}

// Get item skip flag
inline INT PwrRuntimeItem::GetSkipFlag(void) const {
	return m_nSkipFlag;
}

// Set item skip flag
inline void PwrRuntimeItem::SetSkipFlag(INT nValue) {
	m_nSkipFlag = nValue;
}

// Get item snooze flag
inline INT PwrRuntimeItem::GetSnoozeFlag(void) const {
	return m_nSnoozeFlag;
}

// Set item snooze flag
inline void PwrRuntimeItem::SetSnoozeFlag(INT nValue) {
	m_nSnoozeFlag = nValue;
}

// Get next snooze trigger time
inline SYSTEMTIME PwrRuntimeItem::GetTime(void) const {
	return m_stNextSnoozeTime;
}

// Set next snooze trigger time
inline void PwrRuntimeItem::SetTime(const SYSTEMTIME& stTime) {
	m_stNextSnoozeTime = stTime;
}

// Remove all history info data
inline void HistoryInfoData::RemoveAll(void)
{
	// Create an empty item
	const HistoryInfoData emptyItem;

	// Copy and overwrite current data
	this->Copy(emptyItem);
}

// Get registry key name
inline void tagREGISTRYKEY::GetKeyName(CString& strKeyName) const {
	strKeyName = this->strKeyName;
}

// Get registry key name
inline LPCTSTR tagREGISTRYKEY::GetKeyName(void) const {
	return this->strKeyName.GetString();
}

// Set registry key name
inline void tagREGISTRYKEY::SetKeyName(UINT nResourceID)
{
	// Set key name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	this->SetKeyName(strKeyName);
}

// Set registry key name
inline void tagREGISTRYKEY::SetKeyName(LPCTSTR lpszKeyName) {
	this->strKeyName = lpszKeyName;
}

// Get registry key value type
inline UINT tagREGISTRYKEY::GetValueType(void) const {
	return this->nValueType;
}

// Set registry key value type
inline void tagREGISTRYKEY::SetValueType(UINT nValueType) {
	this->nValueType = nValueType;
}

// Get root key
inline HKEY RegistryInfo::GetRootKey(void) const {
	return m_hRootKey;
}

// Get registry key info data
inline const REGISTRYKEY& RegistryInfo::GetRegistryKey(void) const {
	return m_regKeyInfo;
}

// Get root key name
inline LPCTSTR RegistryInfo::GetRootKeyName(void) const {
	return m_strRootKey;
}

// Get Profile key name part
inline LPCTSTR RegistryInfo::GetProfileName(void) const {
	return m_strProfileName;
}

// Get application name part
inline LPCTSTR RegistryInfo::GetAppName(void) const {
	return m_strAppName;
}

// Set registry info root key
inline void RegistryInfo::SetRootKey(HKEY hRootKey) {
	m_hRootKey = hRootKey;
}

// Set registry info root key name
inline void RegistryInfo::SetRootKeyName(UINT nResourceID)
{
	// Set root key name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	SetRootKeyName(strKeyName);
}

// Set registry info root key name
inline void RegistryInfo::SetRootKeyName(LPCTSTR lpszRootkeyName) {
	m_strRootKey = lpszRootkeyName;
}

// Set registry info sub-key path
inline void RegistryInfo::SetSubkeyPath(UINT nResourceID)
{
	// Add sub-key path (by resource ID)
	CString strSubKeyName;
	VERIFY(strSubKeyName.LoadString(nResourceID));
	SetSubkeyPath(strSubKeyName);
}

// Set registry info sub-key path
inline void RegistryInfo::SetSubkeyPath(LPCTSTR lpszSubKeyName) {
	m_astrSubkeyPath.Add(lpszSubKeyName);
}

// Set registry info sub-key path
inline void RegistryInfo::SetSubkeyPath(CStringArray& astrSubkeyPath) {
	m_astrSubkeyPath.Copy(astrSubkeyPath);
}


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetProfileName
//	Description:	Set registry info profile key name
//  Arguments:		nResourceID - Profile key name resource string ID (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

inline void RegistryInfo::SetProfileName(UINT nResourceID)
{
	// Set profile key name (by resource ID)
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	SetProfileName(strKeyName);
}

inline void RegistryInfo::SetProfileName(LPCTSTR lpszProfileName)
{
	// Set profile key name (string)
	m_strProfileName = lpszProfileName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppName
//	Description:	Set registry info app name
//  Arguments:		nResourceID - App name resource string ID (in)
//					lpszAppName - App name (string) (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

inline void RegistryInfo::SetAppName(UINT nResourceID)
{
	// Set app name (by resource ID)
	CString strAppName;
	VERIFY(strAppName.LoadString(nResourceID));
	SetAppName(strAppName);
}

inline void RegistryInfo::SetAppName(LPCTSTR lpszAppName)
{
	// Set app name (string)
	m_strAppName = lpszAppName;
}

// Add section name (by resource ID)
inline void RegistryInfo::SetSectionName(UINT nResourceID)
{
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nResourceID));
	m_astrSectionArray.Add(strKeyName);
}

// Add section name (string)
inline void RegistryInfo::SetSectionName(LPCTSTR lpszSectionName) {
	m_astrSectionArray.Add(lpszSectionName);
}

// Set section name array
inline void RegistryInfo::SetSectionName(CStringArray& astrSectionArray) {
	m_astrSectionArray.Copy(astrSectionArray);
}

// Check if data is empty
inline BOOL Substring::IsEmpty(void) const {
	return (m_strLeft.IsEmpty() && m_strMid.IsEmpty() && m_strRight.IsEmpty());
}

// Trim spaces for left part
inline void Substring::TrimLeft(void) {
	m_strLeft.Trim();
}

// Trim spaces for middle part
inline void Substring::TrimMid(void) {
	m_strMid.Trim();
}

// Trim spaces for all parts
inline void Substring::TrimAll(void)
{
	m_strLeft.Trim();
	m_strMid.Trim();
	m_strRight.Trim();
}

// Trim spaces for right part
inline void Substring::TrimRight(void) {
	m_strRight.Trim();
}

// Get left part
inline LPCTSTR Substring::Left(void) const {
	return (m_strLeft.GetString());
}

// Get middle part
inline LPCTSTR Substring::Mid(void) const {
	return (m_strMid.GetString());
}

// Get right part
inline LPCTSTR Substring::Right(void) const {
	return (m_strRight.GetString());
}

// Set left part
inline void Substring::SetLeft(LPCTSTR lpszSrc) {
	m_strLeft = lpszSrc;
}

// Set middle part
inline void Substring::SetMid(LPCTSTR lpszSrc) {
	m_strMid = lpszSrc;
}

// Set right part
inline void Substring::SetRight(LPCTSTR lpszSrc) {
	m_strRight = lpszSrc;
}

// Convert combo-box selection into option ID
inline UINT AppCore::Sel2Opt(UINT nOptionMacro, UINT nSelection)
{
	VERIFY(nOptionMacro > 0x00 && nOptionMacro < UINT_MAX);
	VERIFY(nSelection >= 0 && nSelection < UINT_MAX);
	return ((nOptionMacro << 8) + (nSelection + 1));
}

// Convert option ID into combo-box selection
inline UINT AppCore::Opt2Sel(UINT nOptionMacro, UINT nCurOption)
{
	VERIFY(nOptionMacro > 0x00 && nOptionMacro < UINT_MAX);
	VERIFY(nCurOption >= 0 && nCurOption < UINT_MAX);
	return (nCurOption - (nOptionMacro << 8) - 1);
}

// Get current time with milliseconds and return SYSTEMTIME
inline void AppCore::GetCurSysTime(SYSTEMTIME& stTime) {
	stTime = GetCurSysTime();
}

#endif		// ifndef _APPCORE_INL_INCLUDED
