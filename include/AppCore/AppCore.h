/**
 * @file		AppCore.h
 * @brief		Define and implement core data types, macros and methods which will be used elsewhere in program
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppBase/AppBase.h"
#include "Components/GridCtrl/GridCtrl.h"
#include "Components/GridCtrl/GridCellCheck.h"
#include "Language.h"


// Grid table column style
typedef enum eGRIDCOLSTYLE {
	COLSTYLE_FIXED = 0,					// Fixed cell
	COLSTYLE_CHECKBOX,					// Checkbox cell
	COLSTYLE_NORMAL,					// Normal cell
} GRIDCOLSTYLE;


// File types (view file mode)
typedef enum eFILETYPE {
	FILETYPE_TEXT = 0,					// Text files
	FILETYPE_IMAGE,						// Image files
} FILETYPE;


//	Define data types for program, these data types will be used elsewhere in the program


// Store application settings and configurations
struct CONFIGDATAINFO
{
	// Main settings
	int		nLMBAction;												// Left mouse button action
	int		nMMBAction;												// Middle mouse button action
	int		nRMBAction;												// Right mouse button action
	BOOL	bRMBShowMenu;											// Right mouse button: Only show menu

	// Display setting
	int		nLanguageID;											// Language setting

	// System settings
	BOOL	bShowDlgAtStartup;										// Show dialog at startup
	BOOL	bStartupEnabled;										// Startup with Windows
	BOOL	bConfirmAction;											// Show confirm message before executing action
	BOOL	bSaveHistoryLog;										// Save app history log
	BOOL	bSaveAppEventLog;										// Save app event log
	BOOL	bRunAsAdmin;											// Run with admin privileges
	BOOL	bShowErrorMsg;											// Show action error message
	BOOL	bNotifySchedule;										// Show notify tip for schedule action
	BOOL	bAllowCancelSchedule;									// Allow canceling schedule when notify
	BOOL	bEnableBackgroundHotkey;								// Enable background action hotkeys
	BOOL	bLockStateHotkey;										// Allow background hotkeys on lockscreen
	BOOL	bEnablePowerReminder;									// Enable Power Peminder feature
};


// For application config data management
class ConfigData : public CONFIGDATAINFO
{
public:
	enum AppOptionID {
		invalid = -1,												// *** Invalid option ***
		leftMouseAction = 0,										// Left mouse button action
		middleMouseAction,											// Middle mouse button action
		rightMouseAction,											// Right mouse button action
		rightMouseShowMenu,											// Right mouse button: Only show menu
		languageID,													// Language setting
		curDispLanguage,											// Currently displaying language
		showDlgAtStartup,											// Show dialog at startup
		startupEnabled,												// Startup with Windows
		confirmBeforeExecuting,										// Show confirm message before doing action
		saveAppEventLog,											// Save app event log
		saveAppHistoryLog,											// Save action history log
		runAsAdmin,													// Run with admin privileges
		showErrorMessage,											// Show action error message
		notifySchedule,												// Show notify tip for schedule action
		allowCancelingSchedule,										// Allow canceling schedule when notify
		backgroundHotkeyEnabled,									// Enable background action hotkeys
		lockStateHotkeyEnabled,										// Allow background hotkeys on lockscreen
		pwrReminderEnabled,											// Enable Power Peminder feature
		defaultScheduleActiveState,									// Default schedule active state
		defaultScheduleActionID,									// Default schedule action ID
		defaultScheduleRepeat										// Default schedule repeat option
	};

public:
	// Construction
	ConfigData();
	ConfigData(const ConfigData& other) {
		this->Copy(other);
	};

public:
	// Data processing
	void Copy(const ConfigData& other) noexcept;
	constexpr bool Compare(const ConfigData& other) const noexcept;
	void SetDefaultData(void) noexcept {
		static const ConfigData defaultConfig;
		this->Copy(defaultConfig);
	}

	// Access data
	void GetData(CONFIGDATAINFO& pData) const noexcept;
	int GetAppOption(AppOptionID eAppOptionID) const noexcept;
};

// Define new global typenames for the enum attributes of Application config data
using AppOptionID = ConfigData::AppOptionID;


// For application flag data management
class FlagManager final
{
public:
	enum AppFlagID {
	// Application-wide tracing flags: Globally managed
		pwrActionFlag,												// Power action trace flag
		systemSuspendFlag,											// System suspended trace flag
		sessionEndFlag,												// Session ended trace flag
		safeTerminationFlag,										// Previously safe termination trace flag
		sessionLockFlag,											// Session lock trace flag

	// Application DebugTest flags: Globally managed
		dummyTestMode,												// Dummy test mode enabled flag
		debugMode,													// Debug mode enabled flag
		debugOutputTarget,											// Debug log output target flag
		testFeatureEnabled,											// Test feature enable flag

	// Application-base flags: Application managed
		appDataChanged,												// Application data/setting change flag
		appReadOnlyMode,											// Application read-only mode
		appForceClosing,											// Force closing application by request
		appExitCode,												// Application exit code

	// Dialog-base properties/flags: Dialog managed
		dialogDataChanged,											// Dialog data/setting change flag
		dialogReturnFlag,											// Dialog returned flag
		dialogExpanded,												// Dialog expanded/collapsed
		dialogReadOnlyMode,											// Dialog read-only mode
		dialogLockState,											// Dialog item lock state
		dialogForceClosing,											// Force closing dialog by request
		dialogUseEscapeKey,											// Use Escape key
		dialogUseEnterKey,											// Use Enter key
		dialogSetBackgroundColor,									// Dialog background color is set
		dialogSetTextColor,											// Dialog text color is set
		dialogSetMinSize,											// Dialog minimum size is set
		dialogSetMaxSize,											// Dialog maximum size is set
		dialogSetTopMost,											// Dialog top-most position is set
		dialogSetInitSound,											// Dialog initialize sound is set

	// Application main window runtime flags: Application managed
		notifyIconShowed,											// Notify icon showing flag
		hotkeyRegistered,											// Hotkey registered
		restartAsAdmin,												// Restart as admin flag
		pwrBroadcastSkipCount,										// Power Broadcase event skip counter
		wtsSessionNotifyRegistered,									// WTS Session Change State Notification registered
	};
	enum ManagerID {
		dialogFlagManager,											// Dialog-owned flag manager
		applicationFlagManager,										// Application flag manager
		globalFlagManager,											// Global/shared flag manager
	};
	enum ManagementMethod {
		dialogSelfManaged,											// For flags managed directly by the dialog itself
		applicationManaged,											// For flags managed by the broader application logic
		globallyManaged,											// For flags managed centrally by a global/shared manager
	};

private:
	// Define private typenames/aliases
	using UniqueFlagMap = typename std::unordered_map<AppFlagID, int>;

private:
	// Attributes
	UniqueFlagMap m_mapUniqueFlags;

public:
	// Constructor
	FlagManager(void) = default;

private:
	// No copyable
	FlagManager(const FlagManager&) = delete;
	FlagManager& operator=(const FlagManager&) = delete;

	// No movable
	FlagManager(const FlagManager&&) = delete;
	FlagManager& operator=(const FlagManager&&) = delete;

public:
	// Check if a flag value exists
	bool IsFlagPresent(AppFlagID eFlagID) const {
		auto it = m_mapUniqueFlags.find(eFlagID);
		return (it != m_mapUniqueFlags.end());
	};

	// Get application flag value by ID
	int GetFlagValue(AppFlagID eFlagID) const {
		auto it = m_mapUniqueFlags.find(eFlagID);
		if (it != m_mapUniqueFlags.end()) return it->second;
		else return FLAG_OFF;
	};

	// Set application flag value by ID
	void SetFlagValue(AppFlagID eFlagID, int nValue) {
		m_mapUniqueFlags[eFlagID] = nValue;
	};
};

// Define new global typenames for the enum attributes of Application flag data
using AppFlagID = FlagManager::AppFlagID;
using FlagManagerID = FlagManager::ManagerID;
using FlagManagementMethod = FlagManager::ManagementMethod;


// Store and manage data of a Power++ item repeat set
class PwrRepeatSet
{
public:
	// Define constant values
	static constexpr int minSnoozeInterval = 60;					// Min snooze interval: 1 minutes
	static constexpr int defaultSnoozeInterval = 600;				// Default snooze interval: 10 minutes
	static constexpr int maxSnoozeInterval = 1800;					// Max snooze interval: 30 minutes
	static constexpr int defaultActiveDays = 0b01111111;			// Default repeat: All days of weekss

private:
	// Attributes
	BOOL m_bRepeat;													// Repeat daily
	BOOL m_bAllowSnooze;											// Allow snoozing mode
	int	 m_nSnoozeInterval;											// Snooze interval
	byte m_byRepeatDays;											// Days of week (for repeating)

public:
	// Constructor
	PwrRepeatSet();
	PwrRepeatSet(const PwrRepeatSet& other) {
		this->Copy(other);
	};

	// Operator
	PwrRepeatSet& operator=(const PwrRepeatSet& other) {
		this->Copy(other);
		return *this;
	}

public:
	// Data processing
	void Copy(const PwrRepeatSet& other) noexcept;
	constexpr bool Compare(const PwrRepeatSet& other) const noexcept;

public:
	// Get attributes
	constexpr bool IsRepeatEnabled(void) const noexcept {
		return m_bRepeat;
	};
	constexpr bool IsAllowSnoozing(void) const noexcept {
		return m_bAllowSnooze;
	};
	constexpr int GetSnoozeInterval(void) const noexcept {
		return m_nSnoozeInterval;
	};
	constexpr byte GetActiveDays(void) const noexcept {
		return m_byRepeatDays;
	};
	constexpr bool IsDayActive(DayOfWeek dayOfWeek) const noexcept {
		if ((dayOfWeek < DayOfWeek::Sunday) || (dayOfWeek > DayOfWeek::Saturday)) return false;
		return ((m_byRepeatDays & (1 << dayOfWeek)) >> dayOfWeek);
	};

	// Set attributes
	void EnableRepeat(bool bEnabled) noexcept {
		m_bRepeat = bEnabled;
	};
	void EnableSnoozing(bool bEnabled) noexcept {
		m_bAllowSnooze = bEnabled;
	};
	void SetSnoozeInterval(int nValue) noexcept {
		m_nSnoozeInterval = nValue;
	};
	void SetActiveDays(byte byActiveDays) noexcept {
		m_byRepeatDays = byActiveDays;
	};
	void SetDayActive(DayOfWeek dayOfWeek, bool bActive) noexcept {
		if ((dayOfWeek < DayOfWeek::Sunday) || (dayOfWeek > DayOfWeek::Saturday)) return;
		m_byRepeatDays |= bActive << dayOfWeek;
	};
};

// Define new typenames for RepeatSet data
using PPwrRepeatSet = PwrRepeatSet*;


// Store and manage schedule item data
class ScheduleItem
{
private:
	// Attributes
	unsigned	 m_nItemID;											// Item ID
	BOOL		 m_bEnabled;										// Enable/disable state
	unsigned	 m_nActionID;										// Schedule action ID
	ClockTime	 m_stTime;											// Schedule time
	PwrRepeatSet m_rpsRepeatSet;									// Repeat set data

public:
	// Constructor
	ScheduleItem();
	ScheduleItem(unsigned nItemID);
	ScheduleItem(const ScheduleItem& other)	{
		this->Copy(other);
	};

	// Copy assignment operator
	ScheduleItem& operator=(const ScheduleItem& other) {
		this->Copy(other);
		return *this;
	};

public:
	// Data processing
	void Copy(const ScheduleItem& other) noexcept;
	constexpr bool Compare(const ScheduleItem& other) const noexcept;
	bool IsEmpty(void) const noexcept {
		static const ScheduleItem schDummyItem;
		return this->Compare(schDummyItem);
	};

public:
	// Get/set attributes
	constexpr unsigned GetItemID(void) const noexcept {
		return m_nItemID;
	};
	void SetItemID(unsigned nItemID) noexcept {
		m_nItemID = nItemID;
	};
	constexpr bool IsEnabled(void) const noexcept {
		return m_bEnabled;
	};
	void EnableItem(bool bEnabled) noexcept {
		m_bEnabled = bEnabled;
	};
	constexpr unsigned GetAction(void) const noexcept {
		return m_nActionID;
	};
	void SetAction(unsigned nActionID) noexcept {
		m_nActionID = nActionID;
	};
	ClockTime GetTime(void) const noexcept {
		return m_stTime;
	};
	void SetTime(const ClockTime& stTime) noexcept {
		m_stTime = stTime;
	};

	// Get RepeatSet data
	constexpr bool IsRepeatEnabled(void) const noexcept {
		return (m_rpsRepeatSet.IsRepeatEnabled());
	};

	constexpr bool IsAllowSnoozing(void) const noexcept {
		if (IsRepeatEnabled() != true) return false;
		if (m_rpsRepeatSet.IsAllowSnoozing() != true) return false;
		return true;
	};
	constexpr bool IsDayActive(DayOfWeek dayOfWeek) const noexcept {
		return (m_rpsRepeatSet.IsDayActive(dayOfWeek));
	};
	constexpr byte GetActiveDays(void) const noexcept {
		return (m_rpsRepeatSet.GetActiveDays());
	};

	// Set RepeatSet data
	void EnableRepeat(bool bEnabled) noexcept {
		m_rpsRepeatSet.EnableRepeat(bEnabled);
	};
	void EnableSnoozing(bool bEnabled) noexcept {
		m_rpsRepeatSet.EnableSnoozing(bEnabled);
	};
	void SetSnoozeInterval(int nValue) noexcept {
		m_rpsRepeatSet.SetSnoozeInterval(nValue);
	};
	void SetActiveDays(byte byActiveDays) noexcept {
		m_rpsRepeatSet.SetActiveDays(byActiveDays);
	};
	void SetDayActive(DayOfWeek dayOfWeek, bool bActive) noexcept {
		m_rpsRepeatSet.SetDayActive(dayOfWeek, bActive);
	};

	// Print item data
	void Print(String& outputString) const;
};

// Define new typenames for Schedule item data
using PScheduleItem = ScheduleItem*;
using ScheduleItemList = typename std::vector<ScheduleItem>;


// Store and manage application's Action Schedule data settings
class ScheduleData
{
public:
	// Define constant values
	static constexpr int defaultItemNum = 1;						// Default item number: 1
	static constexpr int maxItemNum = 100;							// Max item number: 100
	static constexpr int defaultItemID = 0x00;						// Default item ID: 0
	static constexpr int minItemID = 10000;							// Min item ID: 10000
	static constexpr int maxItemID = 19999;							// Max item ID: 19999
	static constexpr int defaultActionID = APP_ACTION_DISPLAYOFF;	// Default action (for new item): Turn off display

	enum Error {
		Success = 0,												// Success (no error)
		ItemIsDefault,												// Item is default (can not remove/delete)
		ItemIsEmpty,												// Schedule data is empty
		MaxItemReached,												// Schedule data item number reaches maximum limit
		ItemDuplicated,												// Item data is duplicated (can not add)
		TimeDuplicated,												// Item time value is duplicated (can not add)
	};

private:
	// Attributes
	ScheduleItem	 m_schDefaultItem;
	ScheduleItemList m_arrSchedExtraItemList;

public:
	// Constructor
	ScheduleData();
	ScheduleData(const ScheduleData& other) {
		this->Copy(other);
	};

	// Copy assignment operator
	ScheduleData& operator=(const ScheduleData& other) {
		this->Copy(other);
		return *this;
	};

public:
	// Data processing
	void Init(void) {
		m_schDefaultItem = ScheduleItem(ScheduleData::defaultItemID);
		m_arrSchedExtraItemList.clear();
	};
	void Copy(const ScheduleData& other);
	void SetDefaultData(void) {
		this->Init();
	};

	// Update items
	DWORD Add(const ScheduleItem& pItem);
	DWORD Update(const ScheduleItem& pItem);

	// Access items
	const ScheduleItem& GetDefaultItem(void) const noexcept {
		return m_schDefaultItem;
	};
	ScheduleItem& GetDefaultItem(void) noexcept {
		return m_schDefaultItem;
	};
	const ScheduleItem& GetItemAt(int nIndex) const {
		ASSERT((nIndex >= 0) && (nIndex < GetExtraItemNum()));
		if ((nIndex >= 0) && (nIndex < GetExtraItemNum()))
			return m_arrSchedExtraItemList.at(nIndex);

		AfxThrowInvalidArgException();
	};
	ScheduleItem& GetItemAt(int nIndex) {
		ASSERT((nIndex >= 0) && (nIndex < GetExtraItemNum()));
		if ((nIndex >= 0) && (nIndex < GetExtraItemNum()))
			return m_arrSchedExtraItemList.at(nIndex);

		AfxThrowInvalidArgException();
	};

	// Item processing
	void Adjust(void);
	unsigned GetNextID(void) const;
	void Remove(int nIndex);
	void RemoveAll(void) {
		for (int nIndex = 0; nIndex < GetExtraItemNum(); nIndex++)
			Remove(nIndex);
	};

	// Get attributes
	constexpr size_t GetExtraItemNum(void) const noexcept {
		return m_arrSchedExtraItemList.size();
	};
	bool IsDefaultEmpty(void) const noexcept {
		return m_schDefaultItem.IsEmpty();
	};
	constexpr bool IsEmpty(int nIndex) const noexcept {
		if ((nIndex < 0) || (nIndex >= GetExtraItemNum())) return true;
		const ScheduleItem& schItem = GetItemAt(nIndex);
		return schItem.IsEmpty();
	};

	// Check if extra data is empty
	constexpr bool IsExtraEmpty(void) const noexcept;
	bool IsAllEmpty(void) const noexcept {
		return (IsDefaultEmpty() && IsExtraEmpty());
	};

	// Clean-up
	void Delete(int nIndex);
	void DeleteExtra(void) noexcept {
		m_arrSchedExtraItemList.clear();
	};
	void DeleteAll(void) noexcept {
		m_schDefaultItem = ScheduleItem(ScheduleData::defaultItemID);
		m_arrSchedExtraItemList.clear();
	};
};


// Store and manage data of a HotkeySet item
class HotkeySetItem
{
public:
	enum HKID {
		displayOff = 0x1a01,										// Turn off display
		sleep,														// Sleep
		shutdown,													// Shut down
		restart,													// Restart
		signOut,													// Sign out
		hibernate,													// Hibernate
	};

private:
	// Attributes
	bool		m_bEnabled;											// Hotkey enabled/disabled
	unsigned	m_nHKActionID;										// Hotkey action ID
	DWORD		m_dwModifiers;										// Modifier keys
	DWORD		m_dwVirtualKey;										// Virtual key code

public:
	// Constructor
	HotkeySetItem();
	HotkeySetItem(unsigned nHKActionID);
	HotkeySetItem(const HotkeySetItem& other) {
		this->Copy(other);
	};

	// Copy assignment operator
	HotkeySetItem& operator=(const HotkeySetItem& other) {
		this->Copy(other);
		return *this;
	};

public:
	// Data processing
	void Copy(const HotkeySetItem& other) noexcept;
	constexpr bool IsEmpty(void) const noexcept {
		bool bIsEmpty = (m_dwModifiers == 0);
		bIsEmpty &= (m_dwVirtualKey == 0);
		return bIsEmpty;
	};
	constexpr bool Compare(const HotkeySetItem& other) const noexcept {
		bool bRet = (this->m_nHKActionID == other.m_nHKActionID);
		bRet &= this->CompareKeycode(other);
		return bRet;
	};
	constexpr bool CompareKeycode(const HotkeySetItem& other) const noexcept {
		return CompareKeycode(other.m_dwModifiers, other.m_dwVirtualKey);
	};

	// Print item data
	void Print(String& outputString) const;
	void PrintKeyStrokes(String& outputString) const;

public:
	// Get/set attributes
	constexpr bool IsEnabled(void) const noexcept {
		return m_bEnabled;
	};
	void EnableItem(bool bEnabled) noexcept {
		m_bEnabled = bEnabled;
	};
	constexpr unsigned GetActionID(void) const noexcept {
		return m_nHKActionID;
	};
	void SetActionID(unsigned nHKActionID) noexcept {
		m_nHKActionID = nHKActionID;
	};
	constexpr void GetKeyCode(DWORD& dwModifiers, DWORD& dwVirtualKey) const noexcept {
		dwModifiers = m_dwModifiers; dwVirtualKey = m_dwVirtualKey;
	};
	void SetKeyCode(DWORD dwModifiers, DWORD dwVirtualKey) noexcept {
		m_dwModifiers = dwModifiers; m_dwVirtualKey = dwVirtualKey;
	}

	// Compare given keycode with item keystroke
	constexpr bool CompareKeycode(DWORD dwModifiers, DWORD dwVirtualKey) const noexcept {
		return ((m_dwModifiers == dwModifiers) && (m_dwVirtualKey == dwVirtualKey));
	};
};

// Define new typenames for HotkeySet item data
using PHotkeySetItem = HotkeySetItem*;
using HotkeySetItemList = typename std::vector<HotkeySetItem>;

// Define new global typenames for the enum attributes of HotkeySet items
using HotkeyID = HotkeySetItem::HKID;


// Store and manage application's HotkeySet data settings
class HotkeySetData
{
public:
	using Item = HotkeySetItem;
	using HKID = Item::HKID;
	using DataList = HotkeySetItemList;

private:
	// Attributes
	DataList m_arrHotkeySetList;

public:
	// Constructor
	HotkeySetData() = default;
	HotkeySetData(const HotkeySetData& other) {
		this->Copy(other);
	};

	// Operator
	HotkeySetData& operator=(const HotkeySetData& other) {
		this->Copy(other);
		return *this;
	};

public:
	// Data processing
	void Init(void) noexcept {
		m_arrHotkeySetList.clear();
	};
	void Copy(const HotkeySetData& other) noexcept;
	void SetDefaultData(void);

	// Update items
	void Add(const Item& pItem);
	void Update(const Item& pItem);

	// Access items
	const Item& GetItemAt(int nIndex) const {
		ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
		if ((nIndex >= 0) && (nIndex < GetItemNum()))
			return m_arrHotkeySetList.at(nIndex);

		AfxThrowInvalidArgException();
	};
	Item& GetItemAt(int nIndex)	{
		ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
		if ((nIndex >= 0) && (nIndex < GetItemNum()))
			return m_arrHotkeySetList.at(nIndex);

		AfxThrowInvalidArgException();
	};

	// Item processing
	void Adjust(void);
	void Remove(int nIndex);
	void RemoveAll(void) {
		for (int nIndex = 0; nIndex < GetItemNum(); nIndex++)
			Remove(nIndex);
	};

	// Get attributes
	constexpr size_t GetItemNum(void) const noexcept {
		return m_arrHotkeySetList.size();
	};
	constexpr bool IsEmpty(int nIndex) const noexcept {
		if ((nIndex < 0) || (nIndex >= GetItemNum())) return true;
		const Item& hksItem = GetItemAt(nIndex);
		return hksItem.IsEmpty();
	};
	bool IsAllEmpty(void) const noexcept;

	// Clean-up
	void Delete(int nIndex);
	void DeleteAll(void) noexcept {
		m_arrHotkeySetList.clear();
	};

	// Print item keystrokes by ID
	void PrintKeyStrokes(unsigned nHKID, String& outputString) const;
};


// Store data of Reminder message style
class RmdMsgStyleSet
{
public:
	enum IconPosition {
		IconOnTheTop = 0,
		IconOnTheLeft,
	};
	enum DisplayPosition {
		AtCenter = 0,
		OnTopLeft,
		OnTopRight,
		OnBottomLeft,
		OnBottomRight,
	};

public:
	// Define default style values
	static constexpr COLORREF defaultBkgrdColor = Color::Pink;
	static constexpr COLORREF defaultTextColor = Color::Red;
	static constexpr const wchar_t* defaultFontName = _T("Arial");
	static constexpr int defaultFontSize = 20;
	static constexpr int defaultTimeout = 0;
	static constexpr int defaultIconID = SystemIcon::Information;
	static constexpr int defaultIconSize = 50;
	static constexpr int defaultIconPosition = IconOnTheTop;
	static constexpr int defaultDisplayPosition = AtCenter;
	static constexpr int defaultHorizontalMargin = 50;
	static constexpr int defaultVerticalMargin = 50;

public:
	// Define constant values
	static constexpr int minFontSize = 10;
	static constexpr int maxFontSize = 100;
	static constexpr int minTimeOut = 10;
	static constexpr int maxTimeOut = 1800;
	static constexpr int minIconSize = 30;
	static constexpr int maxIconSize = 100;
	static constexpr int minMarginVal = 10;
	static constexpr int maxMarginVal = 120;

private:
	// Attributes
	COLORREF	m_colorBkgrd;									// Background color
	COLORREF	m_colorText;									// Text color
	String		m_strFontName;									// Font name
	unsigned	m_uiFontSize;									// Font size
	unsigned	m_uiTimeout;									// Timeout (auto-close) interval
	unsigned	m_uiIconID;										// Message icon ID
	int			m_nIconSize;									// Message icon size
	byte		m_byIconPos;									// Message icon position
	byte		m_byDisplayPos;									// Message display position
	unsigned	m_uiHMargin;									// Display area horizontal margin
	unsigned	m_uiVMargin;									// Display area vertical margin

public:
	// Constructor
	RmdMsgStyleSet();
	RmdMsgStyleSet(const RmdMsgStyleSet& other) {
		this->Copy(other);
	};

	// Copy assignment operator
	RmdMsgStyleSet& operator=(const RmdMsgStyleSet& other) {
		this->Copy(other);
		return *this;
	};

public:
	// Member functions
	void Copy(const RmdMsgStyleSet& other) noexcept;
	bool Compare(const RmdMsgStyleSet& other) const noexcept;

public:
	// Get/set functions
	constexpr COLORREF GetBkgrdColor(void) const noexcept {
		return m_colorBkgrd;
	};
	void SetBkgrdColor(COLORREF color) noexcept {
		m_colorBkgrd = color;
	};
	constexpr COLORREF GetTextColor(void) const noexcept {
		return m_colorText;
	};
	void SetTextColor(COLORREF color) noexcept {
		m_colorText = color;
	};
	String GetFontName(void) const noexcept {
		return m_strFontName;
	};
	void SetFontName(const wchar_t* fontName) noexcept {
		m_strFontName = fontName;
	};
	constexpr unsigned GetFontSize(void) const noexcept {
		return m_uiFontSize;
	};
	void SetFontSize(unsigned uiFontSize) noexcept {
		m_uiFontSize = uiFontSize;
	};
	constexpr unsigned GetTimeout(void) const noexcept {
		return m_uiTimeout;
	};
	void SetTimeout(unsigned uiTimeout) noexcept {
		m_uiTimeout = uiTimeout;
	};
	constexpr unsigned GetIconID(void) const noexcept {
		return m_uiIconID;
	};
	void SetIconID(unsigned uiIconID) noexcept {
		m_uiIconID = uiIconID;
	};
	constexpr int GetIconSize(void) const noexcept {
		return m_nIconSize;
	};
	void SetIconSize(int nIconSize) noexcept {
		m_nIconSize = nIconSize;
	};
	constexpr byte GetIconPosition(void) const noexcept {
		return m_byIconPos;
	};
	void SetIconPosition(byte byIconPos) noexcept {
		m_byIconPos = byIconPos;
	};
	constexpr byte GetDisplayPosition(void) const noexcept {
		return m_byDisplayPos;
	};
	void SetDisplayPosition(byte byDisplayPos) noexcept {
		m_byDisplayPos = byDisplayPos;
	};
	constexpr unsigned GetHorizontalMargin(void) const noexcept {
		return m_uiHMargin;
	};
	void SetHorizontalMargin(unsigned uiHMargin) noexcept {
		m_uiHMargin = uiHMargin;
	};
	constexpr unsigned GetVerticalMargin(void) const noexcept {
		return m_uiVMargin;
	};
	void SetVerticalMargin(unsigned uiVMargin) noexcept {
		m_uiVMargin = uiVMargin;
	};
};

// Define new typenames for Reminder message style data
using PRmdMsgStyleSet = RmdMsgStyleSet*;

// Define new global typenames for the enum attributes of Reminder message style data
using MsgIconPosition = RmdMsgStyleSet::IconPosition;
using MsgDispPosition = RmdMsgStyleSet::DisplayPosition;


// Store and manage data of a Power Reminder item
class PwrReminderItem
{
public:
	enum Event {
		atSetTime = 0x1b01,											// At set time
		atAppStartup,												// At app startup
		atSysWakeUp,												// At system wake
		beforePwrAction,											// Before power action
		wakeAfterAction,											// Wake after action
		atAppExit,													// Before app exit
	};
	enum Style {
		messageBox = 0x1c01,										// Message Box
		dialogBox,													// Dialog Box
	};

private:
	// Attributes
	BOOL			m_bEnabled;										// Enable state
	unsigned		m_nItemID;										// Item ID
	String			m_strMessage;									// Message content
	unsigned		m_nEventID;										// Event ID
	ClockTime		m_stTime;										// Event time
	DWORD			m_dwMsgStyle;									// Reminder style
	PwrRepeatSet	m_rpsRepeatSet;									// Repeat set data
	BOOL			m_bUseCustomStyle;								// Use message custom style
	RmdMsgStyleSet	m_rmsMsgStyleSet;								// Reminder message style set

public:
	// Constructor
	PwrReminderItem();
	PwrReminderItem(const PwrReminderItem& other) {
		this->Copy(other);
	};

	// Copy assignment operator
	PwrReminderItem& operator=(const PwrReminderItem& other) {
		this->Copy(other);
		return *this;
	};

public:
	// Data processing
	void Copy(const PwrReminderItem& other) noexcept;
	bool Compare(const PwrReminderItem& other) const noexcept;
	bool IsEmpty(void) const noexcept {
		static const PwrReminderItem pwrDummyItem;
		return this->Compare(pwrDummyItem);
	};

	// Access data
	const PwrRepeatSet& GetRepeatSetData(void) const noexcept {
		return this->m_rpsRepeatSet;
	};
	PwrRepeatSet& GetRepeatSetData(void) noexcept {
		return this->m_rpsRepeatSet;
	};
	void ResetRepeatInfo(void) noexcept {
		const PwrRepeatSet emptyData = PwrRepeatSet();
		this->m_rpsRepeatSet.Copy(emptyData);
	};
	const RmdMsgStyleSet& GetMessageStyleData(void) const noexcept {
		return this->m_rmsMsgStyleSet;
	};
	RmdMsgStyleSet& GetMessageStyleData(void) noexcept {
		return this->m_rmsMsgStyleSet;
	};
	void ResetMessageStyleInfo(void) noexcept {
		const RmdMsgStyleSet emptyData = RmdMsgStyleSet();
		this->m_rmsMsgStyleSet.Copy(emptyData);
	};

public:
	// Get/set attributes
	constexpr bool IsEnabled(void) const noexcept {
		return m_bEnabled;
	};
	void EnableItem(bool bEnabled) noexcept {
		m_bEnabled = bEnabled;
	};
	constexpr unsigned GetItemID(void) const noexcept {
		return m_nItemID;
	};
	void SetItemID(unsigned nItemID) noexcept {
		m_nItemID = nItemID;
	};
	constexpr const wchar_t* GetMessage(void) const noexcept {
		return m_strMessage.GetString();
	};
	void SetMessage(const wchar_t* message) noexcept {
		m_strMessage = message;
	};
	constexpr unsigned GetEventID(void) const noexcept {
		return m_nEventID;
	};
	void SetEventID(unsigned nEventID) noexcept {
		m_nEventID = nEventID;
	};
	ClockTime GetTime(void) const noexcept {
		return m_stTime;
	};
	void SetTime(const ClockTime& stTime) noexcept {
		m_stTime = stTime;
	};
	constexpr DWORD GetMessageStyle(void) const noexcept {
		return m_dwMsgStyle;
	};
	void SetMessageStyle(DWORD nMsgStyleID) noexcept {
		m_dwMsgStyle = nMsgStyleID;
	};
	constexpr bool IsCustomStyleEnabled(void) const noexcept {
		return m_bUseCustomStyle;
	};
	void EnableCustomStyle(bool bEnabled) noexcept {
		m_bUseCustomStyle = bEnabled;
	};

	// Get RepeatSet data
	constexpr bool IsRepeatEnabled(void) const noexcept {
		return m_rpsRepeatSet.IsRepeatEnabled();
	};
	constexpr bool IsDayActive(DayOfWeek dayOfWeek) const noexcept {
		return m_rpsRepeatSet.IsDayActive(dayOfWeek);
	};
	bool IsAllowSnoozing(void) const noexcept;
	constexpr int GetSnoozeInterval(void) const noexcept {
		return m_rpsRepeatSet.GetSnoozeInterval();
	};
	constexpr byte GetActiveDays(void) const noexcept {
		return m_rpsRepeatSet.GetActiveDays();
	};

	// Set RepeatSet data
	void EnableRepeat(bool bEnabled) noexcept {
		m_rpsRepeatSet.EnableRepeat(bEnabled);
	};
	void EnableSnoozing(bool bEnabled) noexcept {
		m_rpsRepeatSet.EnableSnoozing(bEnabled);
	};
	void SetSnoozeInterval(int nValue) noexcept {
		m_rpsRepeatSet.SetSnoozeInterval(nValue);
	};
	void SetActiveDays(byte byActiveDays) noexcept {
		m_rpsRepeatSet.SetActiveDays(byActiveDays);
	};
	void SetDayActive(DayOfWeek dayOfWeek, bool bActive) noexcept {
		m_rpsRepeatSet.SetDayActive(dayOfWeek, bActive);
	};

	// Print item data
	void Print(String& outputString) const;
};

// Define new typenames for Power Reminder Item data
using PPwrReminderItem = PwrReminderItem*;
using PwrReminderItemList = typename std::vector<PwrReminderItem>;

// Define new global typenames for the enum attributes of Power Reminder items
using PwrReminderEvent = PwrReminderItem::Event;
using PwrReminderStyle = PwrReminderItem::Style;


// Store and manage application's Power Reminder settings
class PwrReminderData
{
public:
	// Define constant values
	static constexpr int maxItemNum = 100;							// Max item number: 100
	static constexpr int minItemID = 10000;							// Min item ID: 10000
	static constexpr int maxItemID = 19999;							// Max item ID: 19999
	static constexpr int previewTimeout = 10;						// Default time-out for preview: 10s

private:
	// Attributes
	PwrReminderItemList	m_arrRmdItemList;							// List of reminder items
	RmdMsgStyleSet		m_rmdCommonStyle;							// Common message style set

public:
	// Constructor
	PwrReminderData();
	PwrReminderData(const PwrReminderData& other) {
		this->Copy(other);
	};

	// Copy assignment operator
	PwrReminderData& operator=(const PwrReminderData& other) {
		this->Copy(other);
		return *this;
	};

public:
	// Member functions
	void Init(void) noexcept;
	void Copy(const PwrReminderData& other) noexcept;
	void SetDefaultData(void) noexcept {
		this->Init();
	};

	// Update items
	void Add(const PwrReminderItem& pItem);
	void Update(const PwrReminderItem& pItem);

	// Access data
	const RmdMsgStyleSet& GetCommonStyle(void) const noexcept {
		return m_rmdCommonStyle;
	};
	RmdMsgStyleSet& GetCommonStyle(void) noexcept {
		return m_rmdCommonStyle;
	};

	// Access items
	const PwrReminderItem& GetItemAt(int nIndex) const {
		ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
		if ((nIndex >= 0) && (nIndex < GetItemNum()))
			return m_arrRmdItemList.at(nIndex);

		AfxThrowInvalidArgException();
	};
	PwrReminderItem& GetItemAt(int nIndex) {
		ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
		if ((nIndex >= 0) && (nIndex < GetItemNum()))
			return m_arrRmdItemList.at(nIndex);

		AfxThrowInvalidArgException();
	};

	// Item processing
	void Adjust(void);
	unsigned GetNextID(void) const noexcept;
	void Remove(int nIndex);
	void RemoveAll(void) {
		for (int nIndex = 0; nIndex < GetItemNum(); nIndex++)
			Remove(nIndex);
	};

	// Get attributes
	size_t GetItemNum(void) const noexcept {
		return m_arrRmdItemList.size();
	};
	bool IsEmpty(int nIndex) const noexcept {
		if ((nIndex < 0) || (nIndex >= GetItemNum())) return true;
		const PwrReminderItem& pwrItem = GetItemAt(nIndex);
		return pwrItem.IsEmpty();
	};
	bool IsAllEmpty(void) const noexcept;

	// Clean-up
	void Delete(int nIndex);
	void DeleteAll(void) noexcept {
		// Reset data
		m_arrRmdItemList.clear();
		m_rmdCommonStyle = RmdMsgStyleSet();
	};
};


// Store and manage information of application feature runtime item
class PwrRuntimeItem
{
public:
	enum FeatureCategoryID {
		powerAction = 0,											// Power action
		schedule,													// Action schedule
		hotkeySet,													// HotkeySet
		pwrReminder,												// Power Reminder
	};

private:
	// Attributes
	int			m_nCategory;										// Item category
	unsigned	m_nItemID;											// Power Reminder item ID
	int			m_nDisplayFlag;										// Item displaying flag
	int			m_nSkipFlag;										// Item skip flag
	int			m_nSnoozeFlag;										// Item snooze trigger flag
	ClockTime	m_stNextSnoozeTime;									// Next snooze trigger time

public:
	// Constructor
	PwrRuntimeItem();
	PwrRuntimeItem(const PwrRuntimeItem& other)	{
		this->Copy(other);
	};

	// Copy assignment operator
	PwrRuntimeItem& operator=(const PwrRuntimeItem& other) {
		this->Copy(other);
		return *this;
	};

	// Data processing
	void Copy(const PwrRuntimeItem& other) noexcept;
	void CalcNextSnoozeTime(int nInterval) noexcept;

public:
	// Get/set attributes
	constexpr int GetCategory(void) const noexcept {
		return m_nCategory;
	};
	void SetCategory(int nValue) noexcept {
		m_nCategory = nValue;
	};
	constexpr unsigned GetItemID(void) const noexcept {
		return m_nItemID;
	};
	void SetItemID(unsigned nValue) noexcept {
		m_nItemID = nValue;
	};
	constexpr int GetDisplayFlag(void) const noexcept {
		return m_nDisplayFlag;
	};
	void SetDisplayFlag(int nValue) noexcept {
		m_nDisplayFlag = nValue;
	};
	constexpr int GetSkipFlag(void) const noexcept {
		return m_nSkipFlag;
	};
	void SetSkipFlag(int nValue) noexcept {
		m_nSkipFlag = nValue;
	};
	constexpr int GetSnoozeFlag(void) const noexcept {
		return m_nSnoozeFlag;
	};
	void SetSnoozeFlag(int nValue) noexcept {
		m_nSnoozeFlag = nValue;
	};
	ClockTime GetTime(void) const noexcept {
		return m_stNextSnoozeTime;
	};
	void SetTime(const ClockTime& stTime) noexcept {
		m_stNextSnoozeTime = stTime;
	};
};

// Define new typenames for runtime info item data
using PPwrRuntimeItem = PwrRuntimeItem*;
using PwrRuntimeQueue = typename std::vector<PwrRuntimeItem>;

// Define new global typenames for the enum attributes of runtime info items
using PwrFeatureID = PwrRuntimeItem::FeatureCategoryID;


// Store application action history info data
class HistoryInfoData
{
private:
	// Attributes
	bool		m_bInitState;										// Init state flag
	unsigned	m_nCategoryID;										// Category ID
	DateTime	m_stTimestamp;										// Timestamp of history
	unsigned	m_nItemID;											// Item ID
	unsigned	m_nActionID;										// Action ID
	bool		m_bActionResult;									// Action result
	DWORD		m_dwErrorCode;										// Returned error code
	String		m_strDescription;									// History description (attached info)

public:
	// Constructor
	HistoryInfoData();
	HistoryInfoData(const HistoryInfoData& other) {
		this->Copy(other);
	};

	// Copy assignment operator
	HistoryInfoData& operator=(const HistoryInfoData& other) {
		this->Copy(other);
		return *this;
	};

	// Member functions
	void Copy(const HistoryInfoData& other) noexcept;
	void Init(unsigned nCategoryID) noexcept;
	void RemoveAll(void) noexcept {
		const HistoryInfoData emptyItem;
		this->Copy(emptyItem);
	};

public:
	// Get/set properties
	constexpr bool IsInit(void) const noexcept {
		return m_bInitState;
	};
	constexpr unsigned GetCategoryID(void) const noexcept {
		return m_nCategoryID;
	};
	void SetCategoryID(unsigned nCategoryID) noexcept {
		m_nCategoryID = nCategoryID;
	};
	DateTime GetTime(void) const noexcept {
		return m_stTimestamp;
	};
	void SetTime(const DateTime& stTime) noexcept {
		m_stTimestamp = stTime;
	};
	constexpr unsigned GetItemID(void) const noexcept {
		return m_nItemID;
	};
	void SetItemID(unsigned nItemID) noexcept {
		m_nItemID = nItemID;
	};
	constexpr unsigned GetActionID(void) const noexcept {
		return m_nActionID;
	};
	void SetActionID(unsigned nActionID) noexcept {
		m_nActionID = nActionID;
	};
	constexpr bool IsSuccess(void) const noexcept {
		return m_bActionResult;
	};
	void SetResult(bool bResult) noexcept {
		m_bActionResult = bResult;
	};
	constexpr DWORD GetErrorCode(void) const noexcept {
		return m_dwErrorCode;
	};
	void SetErrorCode(DWORD dwErrorCode) noexcept {
		m_dwErrorCode = dwErrorCode;
	};
	void GetDescription(String& strDescription) const noexcept {
		strDescription = m_strDescription;
	};
	const wchar_t* GetDescription(void) const noexcept {
		return m_strDescription.GetString();
	};
	void SetDescription(const wchar_t* description) noexcept {
		m_strDescription = description;
	};
};

// Define new typenames for History info data
using PHistoryInfoData = HistoryInfoData*;


// Store data of System Event tracking info
class SystemEvent
{
public:
	enum EventID {
		SystemSuspend = 0,											// System suspend event
		SystemWakeUp,												// System wakeup event
		SessionEnded,												// Session end event
		SessionLocked,												// Session locked event
		SessionUnlocked,											// Session unlocked event
	};

private:
	// Attributes
	EventID	 m_sysEventID;											// System event ID
	DateTime m_timeStamp;											// Event timestamp

public:
	// Construction
	SystemEvent(EventID eventID);
	SystemEvent(const SystemEvent& other);

	// Copy assignment operator
	SystemEvent& operator=(const SystemEvent& other);

public:
	// Get/set functions
	EventID GetEventID(void) const noexcept {
		return m_sysEventID;
	};
	DateTime GetTimestamp(void) const noexcept {
		return m_timeStamp;
	};
	void SetTimestamp(DateTime eventTimestamp) noexcept {
		m_timeStamp = eventTimestamp;
	};
};

// Define new typenames for Power System Event info data
using SystemEventData = typename std::vector<SystemEvent>;

// Define new global typenames for the enum attributes of System Event info
using SystemEventID = SystemEvent::EventID;


// Store and manage System Event tracking data
class SystemEventTracker
{
private:
	// Attributes
	SystemEventData m_arrTrackingData;								// System event tracking data

public:
	// Construction
	SystemEventTracker();
	SystemEventTracker(const SystemEventTracker& other);

	// Copy assignment operator
	SystemEventTracker& operator=(const SystemEventTracker& other);

public:
	// Validation
	constexpr bool IsEmpty(void) const noexcept {
		return (m_arrTrackingData.empty());
	};
	constexpr size_t GetTrackedCount(void) const noexcept {
		return (m_arrTrackingData.size());
	};

	// Add system event info
	void AddEvent(const SystemEvent& eventInfo) {
		m_arrTrackingData.push_back(eventInfo);
	};

	// Remove all tracking data of specific event ID
	void RemoveAll(SystemEventID eventID) {
		m_arrTrackingData.erase(std::remove_if(m_arrTrackingData.begin(), m_arrTrackingData.end(),
			[eventID](const SystemEvent& eventInfo) { return (eventInfo.GetEventID() == eventID); }), m_arrTrackingData.end());
	};

	// Remove all event tracking data
	void RemoveAll(void) noexcept {
		m_arrTrackingData.clear();
	};

	// Access items
	const SystemEvent& GetAt(int nIndex) const {
		return (m_arrTrackingData.at(nIndex));
	};
};


// Store grid control/view column format info
typedef struct tagGRIDCTRLCOLFMT 
{
	// Member variables
	int			nColID;										// Column ID
	unsigned	nHeaderTitleID;								// Header title string ID
	int			nWidth;										// Column width
	unsigned	nColStyle;									// Column style
	bool		bCenter;									// Align center
} GRIDCTRLCOLFORMAT, *PGRIDCTRLCOLFORMAT;


// For request to restart app as admin
typedef struct tagRESTARTREQ 
{
	// Member variables
	bool bRequest;											// Request to restart
	bool bAdminCheck;										// Check if already running as admin
	bool bNotAdminShowMsg;									// If not admin, not show check message
	bool bIsAdminDoNothing;									// If already running as admin, do nothing
	bool bShowMsgWhenDeny;									// Show message when denied
	bool bResetFlag;										// Reset flag when denied
} RESTARTREQ, *PRESTARTREQ;


// For string processing and validation
class StringUtils
{
public:
	enum class ValidationError {
		None = 0,
		EmptyString,
		TooShort,
		TooLong,
		InvalidCharacters,
		InvalidFormat,
		ContainsWhitespace,
		ContainsProhibitedSymbols,
		NotAlphanumeric,
		EncodingError,
		InjectionDetected,
		CustomRuleFailed
	};

public:
	// Format string
	static String StringFormat(unsigned formatTemplateID, ...);
	static String StringFormat(const wchar_t* formatTemplate, ...);

	// Load resource string/text data
	static String LoadResourceString(unsigned resourceStringID);
	static bool	LoadResourceString(String& resultStr, unsigned resourceStringID);
	static String LoadResourceTextData(unsigned resourceFileID);

	// Make/acquire paths
	static String GetApplicationPath(bool includeExeName);
	static String GetSubFolderPath(const wchar_t* subFolderName);
	static String MakeFilePath(const wchar_t* directory, const wchar_t* fileName, const wchar_t* extension);

	// Get product version
	static String GetProductVersion(bool isFullVersion);
	static bool GetProductVersion(String& fullVersion, String& shortVersion);

	// Get system and user info
	static bool GetDeviceName(String& deviceName);
	static bool GetCurrentUserName(String& userName);

	// Print character list
	static int PrintCharList(const wchar_t* srcStr, String& outputStr);
};


// For clock-time processing and validation
class ClockTimeUtils
{
public:
	// Get current clock-time from system
	static ClockTime GetCurrentClockTime(void);

	// Conversion
	static ClockTime FromSystemTime(SYSTEMTIME sysTime);
	static SYSTEMTIME ToSystemTime(const ClockTime& clockTime);

	static bool InputText2Time(ClockTime& clockTime, const wchar_t* inputText);
	static bool InputText2TimeBase(ClockTime& clockTime, const wchar_t* inputText);
	static void SpinPos2Time(ClockTime& clockTime, int spinPos);
	static void Time2SpinPos(const ClockTime& clockTime, int& spinPos);

	// Calculate time offset (increasing/descreasing) in seconds
	static void CalculateOffset(ClockTime& clockTime, int offInSecs);

	// Check time matching (in seconds)
	static bool IsMatching(ClockTime thisTime, ClockTime otherTime, int offInSecs = 0);

	// Format for displaying/printing
	static String Format(LANGTABLE_PTR pLang, unsigned nFormatID, const ClockTime& clockTime);
	static String Format(LANGTABLE_PTR pLang, const wchar_t* formatString, const ClockTime& clockTime);
};


// For date/time processing and validation
class DateTimeUtils
{
public:
	// Get current date/time from system
	static DateTime GetCurrentDateTime(void);

	// Conversion
	static DateTime FromSystemTime(SYSTEMTIME sysTime);
	static SYSTEMTIME ToSystemTime(const DateTime& dateTime);

	// Format for displaying/printing
	static String Format(LANGTABLE_PTR pLang, unsigned nFormatID, const DateTime& dateTime);
	static String Format(LANGTABLE_PTR pLang, const wchar_t* formatString, const DateTime& dateTime);
};


// For querrying performance counter of functions
class PerformanceCounter
{
private:
	// Attributes
	LARGE_INTEGER m_liStartTime;
	LARGE_INTEGER m_liEndTime;
	LARGE_INTEGER m_liFrequency;

	// Counting flag
	bool m_bIsRunning;

public:
	// Construction
	PerformanceCounter();
	~PerformanceCounter();

	// Member functions
	void Start(void);
	void Stop(void);
	double GetElapsedTime(bool) const noexcept;
};


// Core methods for application
// These functions will be used elsewhere in the program
namespace AppCore
{
	// Power action execution functions (main core)
	bool ExecutePowerAction(unsigned nActionType, unsigned nMessage, DWORD& dwErrCode);
	bool ExecutePowerActionDummy(unsigned nActionType, unsigned nMessage, DWORD& dwErrCode);

	// Message and notification functions
	LRESULT	WaitMessage(unsigned nMsg, int nTimeout = Constant::Max::Timeout::WaitMessage);
	void	ShowErrorMessage(HWND hMsgOwnerWnd, unsigned nLanguageID, DWORD dwErrorCode, LPARAM lParam = NULL);

	// Convert combo-box selection into option ID
	inline unsigned Sel2Opt(unsigned nOptionMacro, unsigned nSelection) {
		VERIFY(nOptionMacro > 0x00 && nOptionMacro < UINT_MAX);
		VERIFY(nSelection >= 0 && nSelection < UINT_MAX);
		return ((nOptionMacro << 8) + (nSelection + 1));
	};

	// Convert option ID into combo-box selection
	inline unsigned Opt2Sel(unsigned nOptionMacro, unsigned nCurOption) {
		VERIFY(nOptionMacro > 0x00 && nOptionMacro < UINT_MAX);
		VERIFY(nCurOption >= 0 && nCurOption < UINT_MAX);
		return (nCurOption - (nOptionMacro << 8) - 1);
	};

	// Data/control/window functions
	HWND FindDebugTestDlg(void);
	void SetFixedCellStyle(CGridCtrl* pGridCtrl, int nRow, int nCol);
	bool SetDarkMode(CWnd* pWnd, bool bEnableDarkMode);
	void DrawButton(CButton*& pButton, unsigned nIconID, const wchar_t* buttonTitle = Constant::String::Empty);

	// Get Windows OS version
	unsigned GetWindowsOSVersion(void);

	// File and media
	void PlaySound(bool bSoundEnable, unsigned nTypeOfSound);
	bool FileViewStd(FILETYPE eFileType, const wchar_t* filePath);
	bool OpenWebURL(const wchar_t* webUrl);

	// Applications and instances
	LRESULT RunApp(const wchar_t* appPath, bool bRunAsAdmin = false, bool bShowFlag = true);
	LRESULT ExecuteCommand(const wchar_t* commandString, bool bRunAsAdmin = true, bool bShowFlag = true);
	bool	CreateAppProcess(const wchar_t* appPath, wchar_t* commandLine, unsigned nStyle, DWORD& dwErrorCode);

	// Font name validation
	bool EnumFontNames(std::vector<std::wstring>& fontNames);
	bool ValidateFontName(const wchar_t* fontName);
};

