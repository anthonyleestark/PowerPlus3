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
	int		leftMouseAction;										// Left mouse button action
	int		middleMouseAction;										// Middle mouse button action
	int		rightMouseAction;										// Right mouse button action
	BOOL	rightMouseShowMenu;										// Right mouse button: Only show menu

	// Display setting
	int		languageID;												// Language setting

	// System settings
	BOOL	showDialogAtStartup;									// Show dialog at startup
	BOOL	enableAutoStart;										// Startup with Windows
	BOOL	actionConfirmation;										// Show confirm message before executing action
	BOOL	saveActionHistory;										// Save app history log
	BOOL	saveAppEventLog;										// Save app event log
	BOOL	runAsAdmin;												// Run with admin privileges
	BOOL	showErrorMessage;										// Show action error message
	BOOL	scheduleNotification;									// Show notify tip for schedule action
	BOOL	allowScheduleCancellation;								// Allow canceling schedule when notify
	BOOL	enableBackgroundHotkey;									// Enable background action hotkeys
	BOOL	allowLockscreenHotkey;									// Allow background hotkeys on lockscreen
	BOOL	enablePowerReminder;									// Enable Power Peminder feature
};


// For application config data management
class ConfigData : public CONFIGDATAINFO
{
public:
	enum class AppOptionID : int {
		invalid = -1,												// *** Invalid option ***
		leftMouseAction = 0,										// Left mouse button action
		middleMouseAction,											// Middle mouse button action
		rightMouseAction,											// Right mouse button action
		rightMouseShowMenu,											// Right mouse button: Only show menu
		languageID,													// Language setting
		curDispLanguage,											// Currently displaying language
		showDialogAtStartup,										// Show dialog at startup
		enableAutoStart,											// Startup with Windows
		actionConfirmation,											// Show confirm message before doing action
		saveAppEventLog,											// Save app event log
		saveActionHistory,											// Save action history log
		runAsAdmin,													// Run with admin privileges
		showErrorMessage,											// Show action error message
		scheduleNotification,										// Show notify tip for schedule action
		allowScheduleCancellation,									// Allow canceling schedule when notify
		enableBackgroundHotkey,										// Enable background action hotkeys
		allowLockscreenHotkey,										// Allow background hotkeys on lockscreen
		enablePowerReminder,										// Enable Power Peminder feature
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
	void GetData(CONFIGDATAINFO& data) const noexcept;
	int GetAppOption(AppOptionID appOptionID) const noexcept;
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
	bool IsFlagPresent(AppFlagID flagID) const {
		auto it = m_mapUniqueFlags.find(flagID);
		return (it != m_mapUniqueFlags.end());
	};

	// Get application flag value by ID
	int GetFlagValue(AppFlagID flagID) const {
		auto it = m_mapUniqueFlags.find(flagID);
		if (it != m_mapUniqueFlags.end()) return it->second;
		else return FLAG_OFF;
	};

	// Set application flag value by ID
	void SetFlagValue(AppFlagID flagID, int value) {
		m_mapUniqueFlags[flagID] = value;
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
	BOOL m_isRepeated;												// Repeat daily
	BOOL m_allowSnoozing;											// Allow snoozing mode
	int	 m_snoozeInterval;											// Snooze interval
	byte m_repeatDays;												// Days of week (for repeating)

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
		return m_isRepeated;
	};
	constexpr bool IsAllowSnoozing(void) const noexcept {
		return m_allowSnoozing;
	};
	constexpr int GetSnoozeInterval(void) const noexcept {
		return m_snoozeInterval;
	};
	constexpr byte GetActiveDays(void) const noexcept {
		return m_repeatDays;
	};
	constexpr bool IsDayActive(DayOfWeek dayOfWeek) const noexcept {
		if ((dayOfWeek < DayOfWeek::Sunday) || (dayOfWeek > DayOfWeek::Saturday)) return false;
		return ((m_repeatDays & (1 << dayOfWeek)) >> dayOfWeek);
	};

	// Set attributes
	void EnableRepeat(bool enabled) noexcept {
		m_isRepeated = enabled;
	};
	void EnableSnoozing(bool enabled) noexcept {
		m_allowSnoozing = enabled;
	};
	void SetSnoozeInterval(int value) noexcept {
		m_snoozeInterval = value;
	};
	void SetActiveDays(byte activeDays) noexcept {
		m_repeatDays = activeDays;
	};
	void SetDayActive(DayOfWeek dayOfWeek, bool active) noexcept {
		if ((dayOfWeek < DayOfWeek::Sunday) || (dayOfWeek > DayOfWeek::Saturday)) return;
		m_repeatDays |= active << dayOfWeek;
	};
};

// Define new typenames for RepeatSet data
using PPwrRepeatSet = PwrRepeatSet*;


// Store and manage schedule item data
class ScheduleItem
{
private:
	// Attributes
	unsigned	 m_itemID;											// Item ID
	BOOL		 m_isEnabled;										// Enable/disable state
	unsigned	 m_actionID;										// Schedule action ID
	ClockTime	 m_timeValue;										// Schedule time
	PwrRepeatSet m_repeatSetInfo;									// Repeat set data

public:
	// Constructor
	ScheduleItem();
	ScheduleItem(unsigned itemID);
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
		return m_itemID;
	};
	void SetItemID(unsigned itemID) noexcept {
		m_itemID = itemID;
	};
	constexpr bool IsEnabled(void) const noexcept {
		return m_isEnabled;
	};
	void EnableItem(bool enabled) noexcept {
		m_isEnabled = enabled;
	};
	constexpr unsigned GetAction(void) const noexcept {
		return m_actionID;
	};
	void SetAction(unsigned actionID) noexcept {
		m_actionID = actionID;
	};
	ClockTime GetTime(void) const noexcept {
		return m_timeValue;
	};
	void SetTime(const ClockTime& time) noexcept {
		m_timeValue = time;
	};

	// Get RepeatSet data
	constexpr bool IsRepeatEnabled(void) const noexcept {
		return (m_repeatSetInfo.IsRepeatEnabled());
	};

	constexpr bool IsAllowSnoozing(void) const noexcept {
		if (IsRepeatEnabled() != true) return false;
		if (m_repeatSetInfo.IsAllowSnoozing() != true) return false;
		return true;
	};
	constexpr bool IsDayActive(DayOfWeek dayOfWeek) const noexcept {
		return (m_repeatSetInfo.IsDayActive(dayOfWeek));
	};
	constexpr byte GetActiveDays(void) const noexcept {
		return (m_repeatSetInfo.GetActiveDays());
	};

	// Set RepeatSet data
	void EnableRepeat(bool bEnabled) noexcept {
		m_repeatSetInfo.EnableRepeat(bEnabled);
	};
	void EnableSnoozing(bool bEnabled) noexcept {
		m_repeatSetInfo.EnableSnoozing(bEnabled);
	};
	void SetSnoozeInterval(int nValue) noexcept {
		m_repeatSetInfo.SetSnoozeInterval(nValue);
	};
	void SetActiveDays(byte byActiveDays) noexcept {
		m_repeatSetInfo.SetActiveDays(byActiveDays);
	};
	void SetDayActive(DayOfWeek dayOfWeek, bool bActive) noexcept {
		m_repeatSetInfo.SetDayActive(dayOfWeek, bActive);
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
	ScheduleItem	 m_defaultItem;
	ScheduleItemList m_extraScheduleItemList;

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
		m_defaultItem = ScheduleItem(ScheduleData::defaultItemID);
		m_extraScheduleItemList.clear();
	};
	void Copy(const ScheduleData& other);
	void SetDefaultData(void) {
		this->Init();
	};

	// Update items
	DWORD Add(const ScheduleItem& item);
	DWORD Update(const ScheduleItem& item);

	// Access items
	const ScheduleItem& GetDefaultItem(void) const noexcept {
		return m_defaultItem;
	};
	ScheduleItem& GetDefaultItem(void) noexcept {
		return m_defaultItem;
	};
	const ScheduleItem& GetItemAt(int index) const {
		ASSERT((index >= 0) && (index < GetExtraItemNum()));
		if ((index >= 0) && (index < GetExtraItemNum()))
			return m_extraScheduleItemList.at(index);

		AfxThrowInvalidArgException();
	};
	ScheduleItem& GetItemAt(int index) {
		ASSERT((index >= 0) && (index < GetExtraItemNum()));
		if ((index >= 0) && (index < GetExtraItemNum()))
			return m_extraScheduleItemList.at(index);

		AfxThrowInvalidArgException();
	};

	// Item processing
	void Adjust(void);
	unsigned GetNextID(void) const;
	void Remove(int index);
	void RemoveAll(void) {
		for (int index = 0; index < GetExtraItemNum(); index++)
			Remove(index);
	};

	// Get attributes
	constexpr size_t GetExtraItemNum(void) const noexcept {
		return m_extraScheduleItemList.size();
	};
	bool IsDefaultEmpty(void) const noexcept {
		return m_defaultItem.IsEmpty();
	};
	constexpr bool IsEmpty(int index) const noexcept {
		if ((index < 0) || (index >= GetExtraItemNum())) return true;
		const ScheduleItem& item = GetItemAt(index);
		return item.IsEmpty();
	};

	// Check if extra data is empty
	constexpr bool IsExtraEmpty(void) const noexcept;
	bool IsAllEmpty(void) const noexcept {
		return (IsDefaultEmpty() && IsExtraEmpty());
	};

	// Clean-up
	void Delete(int nIndex);
	void DeleteExtra(void) noexcept {
		m_extraScheduleItemList.clear();
	};
	void DeleteAll(void) noexcept {
		m_defaultItem = ScheduleItem(ScheduleData::defaultItemID);
		m_extraScheduleItemList.clear();
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
	bool		m_isEnabled;										// Hotkey enabled/disabled
	unsigned	m_hotkeyActionID;									// Hotkey action ID
	DWORD		m_modifiers;										// Modifier keys
	DWORD		m_virtualKey;										// Virtual key code

public:
	// Constructor
	HotkeySetItem();
	HotkeySetItem(unsigned hkActionID);
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
		bool isEmpty = (m_modifiers == 0);
		isEmpty &= (m_virtualKey == 0);
		return isEmpty;
	};
	constexpr bool Compare(const HotkeySetItem& other) const noexcept {
		bool ret = (this->m_hotkeyActionID == other.m_hotkeyActionID);
		ret &= this->CompareKeycode(other);
		return ret;
	};
	constexpr bool CompareKeycode(const HotkeySetItem& other) const noexcept {
		return CompareKeycode(other.m_modifiers, other.m_virtualKey);
	};

	// Print item data
	void Print(String& outputString) const;
	void PrintKeyStrokes(String& outputString) const;

public:
	// Get/set attributes
	constexpr bool IsEnabled(void) const noexcept {
		return m_isEnabled;
	};
	void EnableItem(bool enabled) noexcept {
		m_isEnabled = enabled;
	};
	constexpr unsigned GetActionID(void) const noexcept {
		return m_hotkeyActionID;
	};
	void SetActionID(unsigned hkActionID) noexcept {
		m_hotkeyActionID = hkActionID;
	};
	constexpr void GetKeyCode(DWORD& modifiers, DWORD& virtualKey) const noexcept {
		modifiers = m_modifiers; virtualKey = m_virtualKey;
	};
	void SetKeyCode(DWORD modifiers, DWORD virtualKey) noexcept {
		m_modifiers = modifiers; m_virtualKey = virtualKey;
	}

	// Compare given keycode with item keystroke
	constexpr bool CompareKeycode(DWORD modifiers, DWORD virtualKey) const noexcept {
		return ((m_modifiers == modifiers) && (m_virtualKey == virtualKey));
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
	DataList m_hotkeySetList;

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
		m_hotkeySetList.clear();
	};
	void Copy(const HotkeySetData& other) noexcept;
	void SetDefaultData(void);

	// Update items
	void Add(const Item& item);
	void Update(const Item& item);

	// Access items
	const Item& GetItemAt(int index) const {
		ASSERT((index >= 0) && (index < GetItemNum()));
		if ((index >= 0) && (index < GetItemNum()))
			return m_hotkeySetList.at(index);

		AfxThrowInvalidArgException();
	};
	Item& GetItemAt(int index)	{
		ASSERT((index >= 0) && (index < GetItemNum()));
		if ((index >= 0) && (index < GetItemNum()))
			return m_hotkeySetList.at(index);

		AfxThrowInvalidArgException();
	};

	// Item processing
	void Adjust(void);
	void Remove(int index);
	void RemoveAll(void) {
		for (int index = 0; index < GetItemNum(); index++)
			Remove(index);
	};

	// Get attributes
	constexpr size_t GetItemNum(void) const noexcept {
		return m_hotkeySetList.size();
	};
	constexpr bool IsEmpty(int index) const noexcept {
		if ((index < 0) || (index >= GetItemNum())) return true;
		const Item& item = GetItemAt(index);
		return item.IsEmpty();
	};
	bool IsAllEmpty(void) const noexcept;

	// Clean-up
	void Delete(int index);
	void DeleteAll(void) noexcept {
		m_hotkeySetList.clear();
	};

	// Print item keystrokes by ID
	void PrintKeyStrokes(unsigned hkID, String& outputString) const;
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
	COLORREF	m_colorBkgrd;								// Background color
	COLORREF	m_colorText;								// Text color
	String		m_fontName;									// Font name
	unsigned	m_fontSize;									// Font size
	unsigned	m_timeout;									// Timeout (auto-close) interval
	unsigned	m_iconID;									// Message icon ID
	int			m_iconSize;									// Message icon size
	byte		m_iconPosition;								// Message icon position
	byte		m_displayPosition;							// Message display position
	unsigned	m_marginHorizontal;							// Display area horizontal margin
	unsigned	m_marginVertical;							// Display area vertical margin

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
		return m_fontName;
	};
	void SetFontName(const wchar_t* fontName) noexcept {
		m_fontName = fontName;
	};
	constexpr unsigned GetFontSize(void) const noexcept {
		return m_fontSize;
	};
	void SetFontSize(unsigned fontSize) noexcept {
		m_fontSize = fontSize;
	};
	constexpr unsigned GetTimeout(void) const noexcept {
		return m_timeout;
	};
	void SetTimeout(unsigned timeout) noexcept {
		m_timeout = timeout;
	};
	constexpr unsigned GetIconID(void) const noexcept {
		return m_iconID;
	};
	void SetIconID(unsigned uiIconID) noexcept {
		m_iconID = uiIconID;
	};
	constexpr int GetIconSize(void) const noexcept {
		return m_iconSize;
	};
	void SetIconSize(int nIconSize) noexcept {
		m_iconSize = nIconSize;
	};
	constexpr byte GetIconPosition(void) const noexcept {
		return m_iconPosition;
	};
	void SetIconPosition(byte byIconPos) noexcept {
		m_iconPosition = byIconPos;
	};
	constexpr byte GetDisplayPosition(void) const noexcept {
		return m_displayPosition;
	};
	void SetDisplayPosition(byte byDisplayPos) noexcept {
		m_displayPosition = byDisplayPos;
	};
	constexpr unsigned GetHorizontalMargin(void) const noexcept {
		return m_marginHorizontal;
	};
	void SetHorizontalMargin(unsigned uiHMargin) noexcept {
		m_marginHorizontal = uiHMargin;
	};
	constexpr unsigned GetVerticalMargin(void) const noexcept {
		return m_marginVertical;
	};
	void SetVerticalMargin(unsigned uiVMargin) noexcept {
		m_marginVertical = uiVMargin;
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
	BOOL			m_isEnabled;									// Enable state
	unsigned		m_itemID;										// Item ID
	String			m_messageContent;								// Message content
	unsigned		m_eventID;										// Event ID
	ClockTime		m_timeValue;									// Event time
	DWORD			m_messageStyle;									// Reminder style
	PwrRepeatSet	m_repeatSetInfo;								// Repeat set data
	BOOL			m_useCustomStyle;								// Use message custom style
	RmdMsgStyleSet	m_msgStyleSetInfo;								// Reminder message style set

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
		return this->m_repeatSetInfo;
	};
	PwrRepeatSet& GetRepeatSetData(void) noexcept {
		return this->m_repeatSetInfo;
	};
	void ResetRepeatInfo(void) noexcept {
		const PwrRepeatSet emptyData = PwrRepeatSet();
		this->m_repeatSetInfo.Copy(emptyData);
	};
	const RmdMsgStyleSet& GetMessageStyleData(void) const noexcept {
		return this->m_msgStyleSetInfo;
	};
	RmdMsgStyleSet& GetMessageStyleData(void) noexcept {
		return this->m_msgStyleSetInfo;
	};
	void ResetMessageStyleInfo(void) noexcept {
		const RmdMsgStyleSet emptyData = RmdMsgStyleSet();
		this->m_msgStyleSetInfo.Copy(emptyData);
	};

public:
	// Get/set attributes
	constexpr bool IsEnabled(void) const noexcept {
		return m_isEnabled;
	};
	void EnableItem(bool bEnabled) noexcept {
		m_isEnabled = bEnabled;
	};
	constexpr unsigned GetItemID(void) const noexcept {
		return m_itemID;
	};
	void SetItemID(unsigned nItemID) noexcept {
		m_itemID = nItemID;
	};
	constexpr const wchar_t* GetMessage(void) const noexcept {
		return m_messageContent.GetString();
	};
	void SetMessage(const wchar_t* message) noexcept {
		m_messageContent = message;
	};
	constexpr unsigned GetEventID(void) const noexcept {
		return m_eventID;
	};
	void SetEventID(unsigned nEventID) noexcept {
		m_eventID = nEventID;
	};
	ClockTime GetTime(void) const noexcept {
		return m_timeValue;
	};
	void SetTime(const ClockTime& stTime) noexcept {
		m_timeValue = stTime;
	};
	constexpr DWORD GetMessageStyle(void) const noexcept {
		return m_messageStyle;
	};
	void SetMessageStyle(DWORD nMsgStyleID) noexcept {
		m_messageStyle = nMsgStyleID;
	};
	constexpr bool IsCustomStyleEnabled(void) const noexcept {
		return m_useCustomStyle;
	};
	void EnableCustomStyle(bool bEnabled) noexcept {
		m_useCustomStyle = bEnabled;
	};

	// Get RepeatSet data
	constexpr bool IsRepeatEnabled(void) const noexcept {
		return m_repeatSetInfo.IsRepeatEnabled();
	};
	constexpr bool IsDayActive(DayOfWeek dayOfWeek) const noexcept {
		return m_repeatSetInfo.IsDayActive(dayOfWeek);
	};
	bool IsAllowSnoozing(void) const noexcept;
	constexpr int GetSnoozeInterval(void) const noexcept {
		return m_repeatSetInfo.GetSnoozeInterval();
	};
	constexpr byte GetActiveDays(void) const noexcept {
		return m_repeatSetInfo.GetActiveDays();
	};

	// Set RepeatSet data
	void EnableRepeat(bool bEnabled) noexcept {
		m_repeatSetInfo.EnableRepeat(bEnabled);
	};
	void EnableSnoozing(bool bEnabled) noexcept {
		m_repeatSetInfo.EnableSnoozing(bEnabled);
	};
	void SetSnoozeInterval(int nValue) noexcept {
		m_repeatSetInfo.SetSnoozeInterval(nValue);
	};
	void SetActiveDays(byte byActiveDays) noexcept {
		m_repeatSetInfo.SetActiveDays(byActiveDays);
	};
	void SetDayActive(DayOfWeek dayOfWeek, bool bActive) noexcept {
		m_repeatSetInfo.SetDayActive(dayOfWeek, bActive);
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
	PwrReminderItemList	m_reminderItemList;							// List of reminder items
	RmdMsgStyleSet		m_commonStyleSet;							// Common message style set

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
		return m_commonStyleSet;
	};
	RmdMsgStyleSet& GetCommonStyle(void) noexcept {
		return m_commonStyleSet;
	};

	// Access items
	const PwrReminderItem& GetItemAt(int nIndex) const {
		ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
		if ((nIndex >= 0) && (nIndex < GetItemNum()))
			return m_reminderItemList.at(nIndex);

		AfxThrowInvalidArgException();
	};
	PwrReminderItem& GetItemAt(int nIndex) {
		ASSERT((nIndex >= 0) && (nIndex < GetItemNum()));
		if ((nIndex >= 0) && (nIndex < GetItemNum()))
			return m_reminderItemList.at(nIndex);

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
		return m_reminderItemList.size();
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
		m_reminderItemList.clear();
		m_commonStyleSet = RmdMsgStyleSet();
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
	int			m_categoryID;										// Item category
	unsigned	m_itemID;											// Power Reminder item ID
	int			m_displayFlag;										// Item displaying flag
	int			m_skipFlag;											// Item skip flag
	int			m_snoozeFlag;										// Item snooze trigger flag
	ClockTime	m_nextSnoozeTime;									// Next snooze trigger time

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
		return m_categoryID;
	};
	void SetCategory(int nValue) noexcept {
		m_categoryID = nValue;
	};
	constexpr unsigned GetItemID(void) const noexcept {
		return m_itemID;
	};
	void SetItemID(unsigned nValue) noexcept {
		m_itemID = nValue;
	};
	constexpr int GetDisplayFlag(void) const noexcept {
		return m_displayFlag;
	};
	void SetDisplayFlag(int nValue) noexcept {
		m_displayFlag = nValue;
	};
	constexpr int GetSkipFlag(void) const noexcept {
		return m_skipFlag;
	};
	void SetSkipFlag(int nValue) noexcept {
		m_skipFlag = nValue;
	};
	constexpr int GetSnoozeFlag(void) const noexcept {
		return m_snoozeFlag;
	};
	void SetSnoozeFlag(int nValue) noexcept {
		m_snoozeFlag = nValue;
	};
	ClockTime GetTime(void) const noexcept {
		return m_nextSnoozeTime;
	};
	void SetTime(const ClockTime& stTime) noexcept {
		m_nextSnoozeTime = stTime;
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
	bool		m_initState;										// Init state flag
	unsigned	m_categoryID;										// Category ID
	DateTime	m_timestamp;										// Timestamp of history
	unsigned	m_itemID;											// Item ID
	unsigned	m_actionID;											// Action ID
	bool		m_actionResult;										// Action result
	DWORD		m_errorCode;										// Returned error code
	String		m_description;										// History description (attached info)

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
		return m_initState;
	};
	constexpr unsigned GetCategoryID(void) const noexcept {
		return m_categoryID;
	};
	void SetCategoryID(unsigned nCategoryID) noexcept {
		m_categoryID = nCategoryID;
	};
	DateTime GetTime(void) const noexcept {
		return m_timestamp;
	};
	void SetTime(const DateTime& stTime) noexcept {
		m_timestamp = stTime;
	};
	constexpr unsigned GetItemID(void) const noexcept {
		return m_itemID;
	};
	void SetItemID(unsigned nItemID) noexcept {
		m_itemID = nItemID;
	};
	constexpr unsigned GetActionID(void) const noexcept {
		return m_actionID;
	};
	void SetActionID(unsigned nActionID) noexcept {
		m_actionID = nActionID;
	};
	constexpr bool IsSuccess(void) const noexcept {
		return m_actionResult;
	};
	void SetResult(bool bResult) noexcept {
		m_actionResult = bResult;
	};
	constexpr DWORD GetErrorCode(void) const noexcept {
		return m_errorCode;
	};
	void SetErrorCode(DWORD dwErrorCode) noexcept {
		m_errorCode = dwErrorCode;
	};
	void GetDescription(String& strDescription) const noexcept {
		strDescription = m_description;
	};
	const wchar_t* GetDescription(void) const noexcept {
		return m_description.GetString();
	};
	void SetDescription(const wchar_t* description) noexcept {
		m_description = description;
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
	EventID	 m_eventID;												// System event ID
	DateTime m_timestamp;											// Event timestamp

public:
	// Construction
	SystemEvent(EventID eventID);
	SystemEvent(const SystemEvent& other);

	// Copy assignment operator
	SystemEvent& operator=(const SystemEvent& other);

public:
	// Get/set functions
	EventID GetEventID(void) const noexcept {
		return m_eventID;
	};
	DateTime GetTimestamp(void) const noexcept {
		return m_timestamp;
	};
	void SetTimestamp(DateTime eventTimestamp) noexcept {
		m_timestamp = eventTimestamp;
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
	SystemEventData m_trackingData;								// System event tracking data

public:
	// Construction
	SystemEventTracker();
	SystemEventTracker(const SystemEventTracker& other);

	// Copy assignment operator
	SystemEventTracker& operator=(const SystemEventTracker& other);

public:
	// Validation
	constexpr bool IsEmpty(void) const noexcept {
		return (m_trackingData.empty());
	};
	constexpr size_t GetTrackedCount(void) const noexcept {
		return (m_trackingData.size());
	};

	// Add system event info
	void AddEvent(const SystemEvent& eventInfo) {
		m_trackingData.push_back(eventInfo);
	};

	// Remove all tracking data of specific event ID
	void RemoveAll(SystemEventID eventID) {
		m_trackingData.erase(std::remove_if(m_trackingData.begin(), m_trackingData.end(),
			[eventID](const SystemEvent& eventInfo) { return (eventInfo.GetEventID() == eventID); }), m_trackingData.end());
	};

	// Remove all event tracking data
	void RemoveAll(void) noexcept {
		m_trackingData.clear();
	};

	// Access items
	const SystemEvent& GetAt(int nIndex) const {
		return (m_trackingData.at(nIndex));
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
	bool request;											// Request to restart
	bool adminCheck;										// Check if already running as admin
	bool showMsgIfNotAdmin;									// If not admin, not show check message
	bool doNothingIfAdmin;									// If already running as admin, do nothing
	bool showMsgWhenDeny;									// Show message when denied
	bool resetFlag;										// Reset flag when denied
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
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_endTime;
	LARGE_INTEGER m_frequency;

	// Counting flag
	bool m_isRunning;

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

