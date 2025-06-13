/**
 * @file		AppCore.inl
 * @brief		Implement definition of inline functions for the application core
 * @author		AnthonyLeeStark
 * @date		2024.07.26
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once


///////////////////////////////////////////////////////
//// Implementations

#ifdef _APPCORE_ENABLE_INLINES

#ifndef _APPCORE_INLINE_INCLUDED
#define _APPCORE_INLINE_INCLUDED
#endif

///////////////////////////////////////////////////////

// Check if a flag value exists
inline int FlagManager::IsFlagPresent(AppFlagID eFlagID) const {
	auto it = m_mapUniqueFlags.find(eFlagID);
	return (it != m_mapUniqueFlags.end());
}

// Get application flag value
inline int FlagManager::GetFlagValue(AppFlagID eFlagID) const {
	auto it = m_mapUniqueFlags.find(eFlagID);
	if (it != m_mapUniqueFlags.end()) return it->second;
	else return FLAG_OFF;
}

// Get application flag value
inline void FlagManager::SetFlagValue(AppFlagID eFlagID, int nValue) {
	m_mapUniqueFlags[eFlagID] = nValue;
}

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
inline BOOL PwrRepeatSet::IsDayActive(DayOfWeek dayOfWeek) const
{
	// Invalid day of week
	if ((dayOfWeek < DayOfWeek::Sunday) || (dayOfWeek > DayOfWeek::Saturday))
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
inline void PwrRepeatSet::SetDayActive(DayOfWeek dayOfWeek, BOOL bActive)
{
	// Invalid day of week
	if ((dayOfWeek < DayOfWeek::Sunday) || (dayOfWeek > DayOfWeek::Saturday))
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
inline ClockTime ScheduleItem::GetTime(void) const {
	return m_stTime;
}

// Set schedule item time data
inline void ScheduleItem::SetTime(const ClockTime& stTime) {
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
inline BOOL ScheduleItem::IsDayActive(DayOfWeek dayOfWeek) const {
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
inline void ScheduleItem::SetDayActive(DayOfWeek dayOfWeek, BOOL bActive) {
	m_rpsRepeatSet.SetDayActive(dayOfWeek, bActive);
}

// Get the default Action Schedule item
inline const ScheduleItem& ScheduleData::GetDefaultItem(void) const {
	return m_schDefaultItem;
}

// Get the default Action Schedule item
inline ScheduleItem& ScheduleData::GetDefaultItem(void) {
	return m_schDefaultItem;
}

// Get the Action Schedule item at index
inline const ScheduleItem& ScheduleData::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < GetExtraItemNum()));
	if ((nIndex >= 0) && (nIndex < GetExtraItemNum()))
		return m_arrSchedExtraItemList.at(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Get the Action Schedule item at index
inline ScheduleItem& ScheduleData::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < GetExtraItemNum()));
	if ((nIndex >= 0) && (nIndex < GetExtraItemNum()))
		return m_arrSchedExtraItemList.at(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Get number of extra items
inline size_t ScheduleData::GetExtraItemNum(void) const {
	return m_arrSchedExtraItemList.size();
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
	const ScheduleItem& schItem = GetItemAt(nIndex);
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
	bIsEmpty &= (m_dwModifiers == 0);
	bIsEmpty &= (m_dwVirtualKey == 0);

	return bIsEmpty;
}

// Compare with another given item
inline BOOL HotkeySetItem::Compare(const HotkeySetItem& pItem) const
{
	BOOL bRet = TRUE;

	// Compare item
	bRet &= (this->m_nHKActionID == pItem.m_nHKActionID);
	bRet &= this->CompareKeycode(pItem);

	return bRet;
}

// Compare item keycode with another given item
inline BOOL HotkeySetItem::CompareKeycode(const HotkeySetItem& pItem) const {
	return CompareKeycode(pItem.m_dwModifiers, pItem.m_dwVirtualKey);
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
inline void HotkeySetItem::GetKeyCode(DWORD& dwModifiers, DWORD& dwVirtualKey) const
{
	dwModifiers = m_dwModifiers;
	dwVirtualKey = m_dwVirtualKey;
}

// Set item keycode data
inline void HotkeySetItem::SetKeyCode(DWORD dwModifiers, DWORD dwVirtualKey)
{
	m_dwModifiers = dwModifiers;
	m_dwVirtualKey = dwVirtualKey;
}

// Compare given keycode with item keystroke
inline BOOL HotkeySetItem::CompareKeycode(DWORD dwModifiers, DWORD dwVirtualKey) const {
	return ((m_dwModifiers == dwModifiers) && (m_dwVirtualKey == dwVirtualKey));
}

// Init HotkeySet data (NULL)
inline void HotkeySetData::Init() {
	m_arrHotkeySetList.clear();
}

// Get number of items
inline size_t HotkeySetData::GetItemNum(void) const {
	return m_arrHotkeySetList.size();
}

// Check if item at index is empty
inline BOOL HotkeySetData::IsEmpty(int nIndex) const
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= GetItemNum()))
		return TRUE;

	// Get item data
	const Item& hksItem = GetItemAt(nIndex);

	// Check if item keystroke value is empty
	return hksItem.IsEmpty();
}

// Get the hotkeyset item at index
inline const HotkeySetItem& HotkeySetData::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrHotkeySetList.at(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Get the hotkeyset item at index
inline HotkeySetItem& HotkeySetData::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrHotkeySetList.at(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Delete all HotkeySet data
inline void HotkeySetData::DeleteAll(void)
{
	// Reset data
	m_arrHotkeySetList.clear();
}

// Get background color
inline COLORREF RmdMsgStyleSet::GetBkgrdColor(void) const {
	return m_colorBkgrd;
}

// Set background color
inline void RmdMsgStyleSet::SetBkgrdColor(COLORREF color) {
	m_colorBkgrd = color;
}

// Get text color
inline COLORREF RmdMsgStyleSet::GetTextColor(void) const {
	return m_colorText;
}

// Set text color
inline void RmdMsgStyleSet::SetTextColor(COLORREF color) {
	m_colorText = color;
}

// Get font name
inline String RmdMsgStyleSet::GetFontName(void) const {
	return m_strFontName;
}

// Set font name
inline void RmdMsgStyleSet::SetFontName(const wchar_t* fontName) {
	m_strFontName = fontName;
}

// Get font size
inline UINT RmdMsgStyleSet::GetFontSize(void) const {
	return m_uiFontSize;
}

// Set font size
inline void RmdMsgStyleSet::SetFontSize(UINT uiFontSize) {
	m_uiFontSize = uiFontSize;
}

// Get timeout interval
inline UINT RmdMsgStyleSet::GetTimeout(void) const {
	return m_uiTimeout;
}

// Set timeout interval
inline void RmdMsgStyleSet::SetTimeout(UINT uiTimeout) {
	m_uiTimeout = uiTimeout;
}

// Get message icon ID
inline UINT RmdMsgStyleSet::GetIconID(void) const {
	return m_uiIconID;
}

// Set message icon ID
inline void RmdMsgStyleSet::SetIconID(UINT uiIconID) {
	m_uiIconID = uiIconID;
}

// Get message icon size
inline INT RmdMsgStyleSet::GetIconSize(void) const {
	return m_nIconSize;
}

// Set message icon size
inline void RmdMsgStyleSet::SetIconSize(INT nIconSize) {
	m_nIconSize = nIconSize;
}

// Get message icon position
inline BYTE RmdMsgStyleSet::GetIconPosition(void) const {
	return m_byIconPos;
}

// Set message icon position
inline void RmdMsgStyleSet::SetIconPosition(BYTE byIconPos) {
	m_byIconPos = byIconPos;
}

// Get message display position
inline BYTE RmdMsgStyleSet::GetDisplayPosition(void) const {
	return m_byDisplayPos;
}

// Set message display position
inline void RmdMsgStyleSet::SetDisplayPosition(BYTE byDisplayPos) {
	m_byDisplayPos = byDisplayPos;
}

// Get message display horizontal margin
inline UINT RmdMsgStyleSet::GetHorizontalMargin(void) const {
	return m_uiHMargin;
}

// Set message display horizontal margin
inline void RmdMsgStyleSet::SetHorizontalMargin(UINT uiHMargin) {
	m_uiHMargin = uiHMargin;
}

// Get message display vertical margin
inline UINT RmdMsgStyleSet::GetVerticalMargin(void) const {
	return m_uiVMargin;
}

// Set message display vertical margin
inline void RmdMsgStyleSet::SetVerticalMargin(UINT uiVMargin) {
	m_uiVMargin = uiVMargin;
}

// Get access to item RepeatSet info data
inline PwrRepeatSet& PwrReminderItem::GetRepeatSetData(void) {
	return this->m_rpsRepeatSet;
}

// Get access to item RepeatSet info data
inline const PwrRepeatSet& PwrReminderItem::GetRepeatSetData(void) const {
	return this->m_rpsRepeatSet;
}

// Reset RepeatSet info data
inline void PwrReminderItem::ResetRepeatInfo(void)
{
	// Create a new default RepeatSet info data
	const PwrRepeatSet emptyData = PwrRepeatSet();

	// Copy and overwrite current data
	this->m_rpsRepeatSet.Copy(emptyData);
}

// Get access to item Message Style info data
inline RmdMsgStyleSet& PwrReminderItem::GetMessageStyleData(void) {
	return this->m_rmsMsgStyleSet;
}

// Get access to item Message Style info data
inline const RmdMsgStyleSet& PwrReminderItem::GetMessageStyleData(void) const {
	return this->m_rmsMsgStyleSet;
}

// Reset Message Style info data
inline void PwrReminderItem::ResetMessageStyleInfo(void)
{
	// Create a new default Message Style info data
	const RmdMsgStyleSet emptyData = RmdMsgStyleSet();

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
inline const wchar_t* PwrReminderItem::GetMessage(void) const {
	return m_strMessage.GetString();
}

// Set Power Reminder item message content
inline void PwrReminderItem::SetMessage(const wchar_t* message) {
	m_strMessage = message;
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
inline ClockTime PwrReminderItem::GetTime(void) const {
	return m_stTime;
}

// Set Power Reminder item time data
inline void PwrReminderItem::SetTime(const ClockTime& stTime) {
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
inline BOOL PwrReminderItem::IsDayActive(DayOfWeek dayOfWeek) const {
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
inline void PwrReminderItem::SetDayActive(DayOfWeek dayOfWeek, BOOL bActive) {
	m_rpsRepeatSet.SetDayActive(dayOfWeek, bActive);
}

// Get number of items
inline size_t PwrReminderData::GetItemNum(void) const {
	return m_arrRmdItemList.size();
}

// Get Power Reminder common style data
inline RmdMsgStyleSet& PwrReminderData::GetCommonStyle(void) {
	return m_rmdCommonStyle;
}

// Get Power Reminder common style data
inline const RmdMsgStyleSet& PwrReminderData::GetCommonStyle(void) const {
	return m_rmdCommonStyle;
}

// Get the Power Reminder item at index
inline PwrReminderItem& PwrReminderData::GetItemAt(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrRmdItemList.at(nIndex);

	// Invalid argument
	AfxThrowInvalidArgException();
}

// Get the Power Reminder item at index
inline const PwrReminderItem& PwrReminderData::GetItemAt(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
	if ((nIndex >= 0) && (nIndex < GetItemNum()))
		return m_arrRmdItemList.at(nIndex);

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
	const PwrReminderItem& pwrItem = GetItemAt(nIndex);
	return pwrItem.IsEmpty();
}

// Delete all Power Reminder data
inline void PwrReminderData::DeleteAll(void)
{
	// Reset data
	m_arrRmdItemList.clear();
	m_rmdCommonStyle = RmdMsgStyleSet();
}

// Calculate next snooze time
inline void PwrRuntimeItem::CalcNextSnoozeTime(int nInterval)
{
	// Calculate time with offset
	ClockTimeUtils::CalculateOffset(m_stNextSnoozeTime, nInterval);
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
inline ClockTime PwrRuntimeItem::GetTime(void) const {
	return m_stNextSnoozeTime;
}

// Set next snooze trigger time
inline void PwrRuntimeItem::SetTime(const ClockTime& stTime) {
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

// Check if data is initialized
inline BOOL HistoryInfoData::IsInit(void) const {
	return m_bInitState;
}

// Get category ID
inline UINT HistoryInfoData::GetCategoryID(void) const {
	return m_nCategoryID;
}

// Set category ID
inline void HistoryInfoData::SetCategoryID(UINT nCategoryID) {
	m_nCategoryID = nCategoryID;
}

// Get timestamp of history
inline DateTime HistoryInfoData::GetTime(void) const {
	return m_stTimestamp;
}

// Set timestamp of history
inline void HistoryInfoData::SetTime(const DateTime& stTime) {
	m_stTimestamp = stTime;
}

// Get item ID
inline UINT HistoryInfoData::GetItemID(void) const {
	return m_nItemID;
}

// Set item ID
inline void HistoryInfoData::SetItemID(UINT nItemID) {
	m_nItemID = nItemID;
}

// Get action ID
inline UINT HistoryInfoData::GetActionID(void) const {
	return m_nActionID;
}

// Set action ID
inline void HistoryInfoData::SetActionID(UINT nActionID) {
	m_nActionID = nActionID;
}

// Get action result
inline BOOL HistoryInfoData::IsSuccess(void) const {
	return m_bActionResult;
}

// Set action result
inline void HistoryInfoData::SetResult(BOOL bResult) {
	m_bActionResult = bResult;
}

// Get error code
inline DWORD HistoryInfoData::GetErrorCode(void) const {
	return m_dwErrorCode;
}

// Set error code
inline void HistoryInfoData::SetErrorCode(DWORD dwErrorCode) {
	m_dwErrorCode = dwErrorCode;
}

// Get history description (attached info)
inline const wchar_t* HistoryInfoData::GetDescription(void) const {
	return m_strDescription.GetString();
}

// Get history description (attached info)
inline void HistoryInfoData::GetDescription(String& strDescription) const {
	strDescription = m_strDescription;
}

// Set history description (attached info)
inline void HistoryInfoData::SetDescription(const wchar_t* description) {
	m_strDescription = description;
}

// Get system event ID
inline SystemEventID SystemEvent::GetEventID(void) const {
	return m_sysEventID;
}

// Get system event timestamp
inline DateTime SystemEvent::GetTimestamp(void) const {
	return m_timeStamp;
}

// Set system event timestamp
inline void SystemEvent::SetTimestamp(DateTime eventTimestamp) {
	m_timeStamp = eventTimestamp;
}

// Check if tracking data is empty
inline BOOL SystemEventTracker::IsEmpty(void) const {
	return (m_arrTrackingData.empty());
}

// Get number of tracked events
inline size_t SystemEventTracker::GetTrackedCount(void) const {
	return (m_arrTrackingData.size());
}

// Add system event info
inline void SystemEventTracker::AddEvent(const SystemEvent& eventInfo) {
	m_arrTrackingData.push_back(eventInfo);
}

// Remove all tracking data of specific event ID
inline void SystemEventTracker::RemoveAll(SystemEventID eventID) {
	m_arrTrackingData.erase(std::remove_if(m_arrTrackingData.begin(), m_arrTrackingData.end(),
		[eventID](const SystemEvent& eventInfo) { return (eventInfo.GetEventID() == eventID); }), m_arrTrackingData.end());
}

// Remove all event tracking data
inline void SystemEventTracker::RemoveAll(void) {
	m_arrTrackingData.clear();
}

// Get system event info data by index
inline const SystemEvent& SystemEventTracker::GetAt(int nIndex) const {
	return (m_arrTrackingData.at(nIndex));
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
inline const wchar_t* Substring::Left(void) const {
	return (m_strLeft.GetString());
}

// Get middle part
inline const wchar_t* Substring::Mid(void) const {
	return (m_strMid.GetString());
}

// Get right part
inline const wchar_t* Substring::Right(void) const {
	return (m_strRight.GetString());
}

// Set left part
inline void Substring::SetLeft(const wchar_t* srcStr) {
	m_strLeft = srcStr;
}

// Set middle part
inline void Substring::SetMid(const wchar_t* srcStr) {
	m_strMid = srcStr;
}

// Set right part
inline void Substring::SetRight(const wchar_t* srcStr) {
	m_strRight = srcStr;
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

#endif		// ifdef _APPCORE_ENABLE_INLINES
