/**
 * @file		Serialization_defs.h
 * @brief		Define necessary macros, enums and constants for serialization and backup system
 * @author		AnthonyLeeStark
 * @date		2024.06.03
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once


// Application profile info
struct AppProfile {

	static constexpr const wchar_t* CompanyName									= _T("Stark Personal");
	static constexpr const wchar_t* ProductName									= _T("Power++ v3.2");
	static constexpr const wchar_t* ProductID									= _T("powerplusapp");

	struct LaunchInfo {
		static constexpr const wchar_t* LaunchTime								= _T("LaunchTime");
		static constexpr const wchar_t* LaunchCounter							= _T("LaunchCounter");
		static constexpr const wchar_t* Directory								= _T("Directory");
		static constexpr const wchar_t* FileName								= _T("FileName");
		static constexpr const wchar_t* ProductVersion							= _T("ProductVersion");
		static constexpr const wchar_t* UserName								= _T("UserName");
		static constexpr const wchar_t* DeviceName								= _T("DeviceName");
		static constexpr const wchar_t* OSVersion								= _T("OSVersion");
		static constexpr const wchar_t* OSBuildNumber							= _T("OSBuildNumber");
		static constexpr const wchar_t* OSPlatformID							= _T("OSPlatformID");
	};
};


// Registry info
struct Registry {
	struct RootKey {
		static constexpr const wchar_t* CurrentUser								= _T("HKEY_CURRENT_USER");
		static constexpr const wchar_t* LocalMachine							= _T("HKEY_LOCAL_MACHINE");
	};

	struct Path {
		static constexpr const wchar_t* Application								= _T("Software\\Stark Personal\\powerplusapp");
		static constexpr const wchar_t* AutoStart								= _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	};
};


// Data sections
struct Section {
	static constexpr const wchar_t* ConfigData									= _T("ConfigData");
	static constexpr const wchar_t* ScheduleData								= _T("ScheduleData");
	struct Schedule {
		static constexpr const wchar_t* DefautItem								= _T("DefaultSchedule");
		static constexpr const wchar_t* ItemFormat								= _T("ScheduleItem_%02d");
		static const wchar_t* Item(int id) {
			static wchar_t buffer[32] = L"\0";
			swprintf(buffer, 32, ItemFormat, id);
			return buffer;
		};
	};

	static constexpr const wchar_t* HotkeySetData								= _T("HotkeySetData");
	struct HotkeySet {
		static constexpr const wchar_t* ItemFormat								= _T("HotkeySetItem_%02d");
		static const wchar_t* Item(int id) {
			static wchar_t buffer[32] = L"\0";
			swprintf(buffer, 32, ItemFormat, id);
			return buffer;
		};
	};

	static constexpr const wchar_t* PwrReminderData								= _T("PwrReminderData");
	struct PwrReminder {
		static constexpr const wchar_t* CommonStyle								= _T("CommonStyle");
		static constexpr const wchar_t* ItemFormat								= _T("PwrReminderItem_%02d");
		static const wchar_t* Item(int id) {
			static wchar_t buffer[32] = L"\0";
			swprintf(buffer, 32, ItemFormat, id);
			return buffer;
		};
	};

	static constexpr const wchar_t* GlobalData									= _T("GlobalData");
	struct GlobalData
	{
		static constexpr const wchar_t* DebugTest								= _T("DebugTest");
		static constexpr const wchar_t* AppFlag									= _T("AppFlag");
		static constexpr const wchar_t* Feature									= _T("Feature");
		static constexpr const wchar_t* Other									= _T("Other");
	};

	static constexpr const wchar_t* LayoutInfo									= _T("LayoutInfo");
	struct LayoutInfo
	{
		static constexpr const wchar_t* LogViewerTable							= _T("LogViewerTable");
		static constexpr const wchar_t* LogDetailProps							= _T("LogDetailProps");
		static constexpr const wchar_t* LogDetailData							= _T("LogDetailData");
		static constexpr const wchar_t* MultiScheduleTable						= _T("MultiScheduleTable");
		static constexpr const wchar_t* HKeySetTable							= _T("HKeySetTable");
		static constexpr const wchar_t* PwrReminderTable						= _T("PwrReminderTable");
	};

	static constexpr const wchar_t* SystemEventTracking							= _T("SystemEventTracking");
};

// Data keys
struct Key
{
	struct ConfigData {
		static constexpr const wchar_t* LMBAction								= _T("LMBAction");
		static constexpr const wchar_t* MMBAction								= _T("MMBAction");
		static constexpr const wchar_t* RMBAction								= _T("RMBAction");
		static constexpr const wchar_t* RMBShowMenu								= _T("RMBShowMenu");
		static constexpr const wchar_t* LanguageID								= _T("LanguageID");
		static constexpr const wchar_t* ShowDlgAtStartup						= _T("ShowDlgAtStartup");
		static constexpr const wchar_t* StartupEnabled							= _T("StartupEnabled");
		static constexpr const wchar_t* ConfirmAction							= _T("ConfirmAction");
		static constexpr const wchar_t* SaveHistoryLog							= _T("SaveHistoryLog");
		static constexpr const wchar_t* SaveAppEventLog							= _T("SaveAppEventLog");
		static constexpr const wchar_t* RunAsAdmin								= _T("RunAsAdmin");
		static constexpr const wchar_t* ShowErrorMsg							= _T("ShowErrorMsg");
		static constexpr const wchar_t* NotifySchedule							= _T("NotifySchedule");
		static constexpr const wchar_t* AllowCancelSchedule						= _T("AllowCancelSchedule");
		static constexpr const wchar_t* EnableBackgroundHotkey					= _T("EnableBackgroundHotkey");
		static constexpr const wchar_t* LockStateHotkey							= _T("LockStateHotkey");
		static constexpr const wchar_t* EnablePowerReminder						= _T("EnablePowerReminder");
	};

	struct PwrRepeatSet {
		static constexpr const wchar_t* IsRepeated								= _T("IsRepeated");
		static constexpr const wchar_t* AllowSnooze								= _T("AllowSnooze");
		static constexpr const wchar_t* SnoozeInterval							= _T("SnoozeInterval");
		static constexpr const wchar_t* RepeatDays								= _T("RepeatDays");
	};

	struct ScheduleData
	{
		static constexpr const wchar_t* ExtraItemNum							= _T("ExtraItemNum");
	};

	struct ScheduleItem {
		static constexpr const wchar_t* ItemID									= _T("ItemID");
		static constexpr const wchar_t* IsEnabled								= _T("IsEnabled");
		static constexpr const wchar_t* ActionID								= _T("ActionID");
		static constexpr const wchar_t* Time									= _T("Time");
	};

	struct HotkeySetData
	{
		static constexpr const wchar_t* ItemNum									= _T("ItemNum");
	};

	struct HotkeySetItem {
		static constexpr const wchar_t* IsEnabled								= _T("IsEnabled");
		static constexpr const wchar_t* HKActionID								= _T("HKActionID");
		static constexpr const wchar_t* Modifiers								= _T("Modifiers");
		static constexpr const wchar_t* VirtualKey								= _T("VirtualKey");
	};

	struct PwrReminderData
	{
		static constexpr const wchar_t* ItemNum									= _T("ItemNum");
	};

	struct PwrReminderMsgStyle {
		static constexpr const wchar_t* BkgrdColor								= _T("BkgrdColor");
		static constexpr const wchar_t* TextColor								= _T("TextColor");
		static constexpr const wchar_t* FontName								= _T("FontName");
		static constexpr const wchar_t* FontSize								= _T("FontSize");
		static constexpr const wchar_t* Timeout									= _T("Timeout");
		static constexpr const wchar_t* IconID									= _T("IconID");
		static constexpr const wchar_t* IconSize								= _T("IconSize");
		static constexpr const wchar_t* IconPosition							= _T("IconPosition");
		static constexpr const wchar_t* DisplayPosition							= _T("DisplayPosition");
		static constexpr const wchar_t* HorizontalMargin						= _T("HorizontalMargin");
		static constexpr const wchar_t* VerticalMargin							= _T("VerticalMargin");
	};

	struct PwrReminderItem {
		static constexpr const wchar_t* IsEnabled								= _T("IsEnabled");
		static constexpr const wchar_t* ItemID									= _T("ItemID");
		static constexpr const wchar_t* Message									= _T("Message");
		static constexpr const wchar_t* EventID									= _T("EventID");
		static constexpr const wchar_t* Time									= _T("Time");
		static constexpr const wchar_t* MsgStyle								= _T("MsgStyle");
	};

	struct GlobalData
	{
		struct DebugTest {
			static constexpr const wchar_t* DummyTest							= _T("DummyTest");
			static constexpr const wchar_t* DebugMode							= _T("DebugMode");
			static constexpr const wchar_t* DebugOutput							= _T("DebugOutput");
			static constexpr const wchar_t* TestFeature							= _T("TestFeature");
		};

		struct AppFlag {
			static constexpr const wchar_t* PwrActionFlag						= _T("PwrActionFlag");
			static constexpr const wchar_t* SystemSuspendFlag					= _T("SystemSuspendFlag");
			static constexpr const wchar_t* SessionEndFlag						= _T("SessionEndFlag");
			static constexpr const wchar_t* SafeTermination						= _T("SafeTermination");
		};
	};

	struct LayoutInfo
	{
		static constexpr const wchar_t* GridColSizeFormat						= _T("GridColSize_%02d");
		static const wchar_t* GridColSize(int id) {
			static wchar_t buffer[32] = L"\0";
			swprintf(buffer, 32, GridColSizeFormat, id);
			return buffer;
		};
	};

	struct SystemEventTracking {
		static constexpr const wchar_t* LastSysSuspend							= _T("LastSysSuspend");
		static constexpr const wchar_t* LastSysWakeup							= _T("LastSysWakeup");
		static constexpr const wchar_t* LastSessionEnd							= _T("LastSessionEnd");
	};
};
