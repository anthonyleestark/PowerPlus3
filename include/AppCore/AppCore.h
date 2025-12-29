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
		this->copy(other);
	};

public:
	// Data processing
	void copy(const ConfigData& other) noexcept;
	constexpr bool compare(const ConfigData& other) const noexcept;
	void setDefaultData(void) noexcept {
		static const ConfigData defaultConfig;
		this->copy(defaultConfig);
	}

	// Access data
	void getData(CONFIGDATAINFO& data) const noexcept;
	int getAppOption(AppOptionID appOptionID) const noexcept;
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
	bool isFlagPresent(AppFlagID flagID) const {
		auto it = m_mapUniqueFlags.find(flagID);
		return (it != m_mapUniqueFlags.end());
	};

	// Get application flag value by ID
	int getFlagValue(AppFlagID flagID) const {
		auto it = m_mapUniqueFlags.find(flagID);
		if (it != m_mapUniqueFlags.end()) return it->second;
		else return FLAG_OFF;
	};

	// Set application flag value by ID
	void setFlagValue(AppFlagID flagID, int value) {
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
		this->copy(other);
	};

	// Operator
	PwrRepeatSet& operator=(const PwrRepeatSet& other) {
		this->copy(other);
		return *this;
	}

public:
	// Data processing
	void copy(const PwrRepeatSet& other) noexcept;
	constexpr bool compare(const PwrRepeatSet& other) const noexcept;

public:
	// Get attributes
	constexpr bool isRepeatEnabled(void) const noexcept {
		return m_isRepeated;
	};
	constexpr bool isAllowSnoozing(void) const noexcept {
		return m_allowSnoozing;
	};
	constexpr int getSnoozeInterval(void) const noexcept {
		return m_snoozeInterval;
	};
	constexpr byte getActiveDays(void) const noexcept {
		return m_repeatDays;
	};
	constexpr bool isDayActive(DayOfWeek dayOfWeek) const noexcept {
		if ((dayOfWeek < DayOfWeek::Sunday) || (dayOfWeek > DayOfWeek::Saturday)) return false;
		return ((m_repeatDays & (1 << dayOfWeek)) >> dayOfWeek);
	};

	// Set attributes
	void enableRepeat(bool enabled) noexcept {
		m_isRepeated = enabled;
	};
	void enableSnoozing(bool enabled) noexcept {
		m_allowSnoozing = enabled;
	};
	void setSnoozeInterval(int value) noexcept {
		m_snoozeInterval = value;
	};
	void setActiveDays(byte activeDays) noexcept {
		m_repeatDays = activeDays;
	};
	void setDayActive(DayOfWeek dayOfWeek, bool active) noexcept {
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
		this->copy(other);
	};

	// Copy assignment operator
	ScheduleItem& operator=(const ScheduleItem& other) {
		this->copy(other);
		return *this;
	};

public:
	// Data processing
	void copy(const ScheduleItem& other) noexcept;
	constexpr bool compare(const ScheduleItem& other) const noexcept;
	bool isEmpty(void) const noexcept {
		static const ScheduleItem schDummyItem;
		return this->compare(schDummyItem);
	};

public:
	// Get/set attributes
	constexpr unsigned getItemId(void) const noexcept {
		return m_itemID;
	};
	void setItemId(unsigned itemID) noexcept {
		m_itemID = itemID;
	};
	constexpr bool isEnabled(void) const noexcept {
		return m_isEnabled;
	};
	void enableItem(bool enabled) noexcept {
		m_isEnabled = enabled;
	};
	constexpr unsigned getAction(void) const noexcept {
		return m_actionID;
	};
	void setAction(unsigned actionID) noexcept {
		m_actionID = actionID;
	};
	ClockTime getTime(void) const noexcept {
		return m_timeValue;
	};
	void setTime(const ClockTime& time) noexcept {
		m_timeValue = time;
	};

	// Get RepeatSet data
	constexpr bool isRepeatEnabled(void) const noexcept {
		return (m_repeatSetInfo.isRepeatEnabled());
	};

	constexpr bool isAllowSnoozing(void) const noexcept {
		if (isRepeatEnabled() != true) return false;
		if (m_repeatSetInfo.isAllowSnoozing() != true) return false;
		return true;
	};
	constexpr bool isDayActive(DayOfWeek dayOfWeek) const noexcept {
		return (m_repeatSetInfo.isDayActive(dayOfWeek));
	};
	constexpr byte getActiveDays(void) const noexcept {
		return (m_repeatSetInfo.getActiveDays());
	};

	// Set RepeatSet data
	void enableRepeat(bool bEnabled) noexcept {
		m_repeatSetInfo.enableRepeat(bEnabled);
	};
	void enableSnoozing(bool bEnabled) noexcept {
		m_repeatSetInfo.enableSnoozing(bEnabled);
	};
	void setSnoozeInterval(int nValue) noexcept {
		m_repeatSetInfo.setSnoozeInterval(nValue);
	};
	void setActiveDays(byte byActiveDays) noexcept {
		m_repeatSetInfo.setActiveDays(byActiveDays);
	};
	void setDayActive(DayOfWeek dayOfWeek, bool bActive) noexcept {
		m_repeatSetInfo.setDayActive(dayOfWeek, bActive);
	};

	// Print item data
	void print(String& outputString) const;
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
		this->copy(other);
	};

	// Copy assignment operator
	ScheduleData& operator=(const ScheduleData& other) {
		this->copy(other);
		return *this;
	};

public:
	// Data processing
	void init(void) {
		m_defaultItem = ScheduleItem(ScheduleData::defaultItemID);
		m_extraScheduleItemList.clear();
	};
	void copy(const ScheduleData& other);
	void setDefaultData(void) {
		this->init();
	};

	// Update items
	DWORD add(const ScheduleItem& item);
	DWORD update(const ScheduleItem& item);

	// Access items
	const ScheduleItem& getDefaultItem(void) const noexcept {
		return m_defaultItem;
	};
	ScheduleItem& getDefaultItem(void) noexcept {
		return m_defaultItem;
	};
	const ScheduleItem& getItemAt(int index) const {
		ASSERT((index >= 0) && (index < getExtraItemNum()));
		if ((index >= 0) && (index < getExtraItemNum()))
			return m_extraScheduleItemList.at(index);

		AfxThrowInvalidArgException();
	};
	ScheduleItem& getItemAt(int index) {
		ASSERT((index >= 0) && (index < getExtraItemNum()));
		if ((index >= 0) && (index < getExtraItemNum()))
			return m_extraScheduleItemList.at(index);

		AfxThrowInvalidArgException();
	};

	// Item processing
	void adjust(void);
	unsigned getNextId(void) const;
	void remove(int index);
	void removeAll(void) {
		for (int index = 0; index < getExtraItemNum(); index++)
			remove(index);
	};

	// Get attributes
	constexpr size_t getExtraItemNum(void) const noexcept {
		return m_extraScheduleItemList.size();
	};
	bool isDefaultEmpty(void) const noexcept {
		return m_defaultItem.isEmpty();
	};
	constexpr bool isEmpty(int index) const noexcept {
		if ((index < 0) || (index >= getExtraItemNum())) return true;
		const ScheduleItem& item = getItemAt(index);
		return item.isEmpty();
	};

	// Check if extra data is empty
	constexpr bool isExtraEmpty(void) const noexcept;
	bool isAllEmpty(void) const noexcept {
		return (isDefaultEmpty() && isExtraEmpty());
	};

	// Clean-up
	void deleteItem(int nIndex);
	void deleteExtra(void) noexcept {
		m_extraScheduleItemList.clear();
	};
	void deleteAll(void) noexcept {
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
		this->copy(other);
	};

	// Copy assignment operator
	HotkeySetItem& operator=(const HotkeySetItem& other) {
		this->copy(other);
		return *this;
	};

public:
	// Data processing
	void copy(const HotkeySetItem& other) noexcept;
	constexpr bool isEmpty(void) const noexcept {
		bool isEmpty = (m_modifiers == 0);
		isEmpty &= (m_virtualKey == 0);
		return isEmpty;
	};
	constexpr bool compare(const HotkeySetItem& other) const noexcept {
		bool ret = (this->m_hotkeyActionID == other.m_hotkeyActionID);
		ret &= this->compareKeycode(other);
		return ret;
	};
	constexpr bool compareKeycode(const HotkeySetItem& other) const noexcept {
		return compareKeycode(other.m_modifiers, other.m_virtualKey);
	};

	// Print item data
	void print(String& outputString) const;
	void printKeyStrokes(String& outputString) const;

public:
	// Get/set attributes
	constexpr bool isEnabled(void) const noexcept {
		return m_isEnabled;
	};
	void enableItem(bool enabled) noexcept {
		m_isEnabled = enabled;
	};
	constexpr unsigned getActionId(void) const noexcept {
		return m_hotkeyActionID;
	};
	void setActionId(unsigned hkActionID) noexcept {
		m_hotkeyActionID = hkActionID;
	};
	constexpr void getKeyCode(DWORD& modifiers, DWORD& virtualKey) const noexcept {
		modifiers = m_modifiers; virtualKey = m_virtualKey;
	};
	void setKeyCode(DWORD modifiers, DWORD virtualKey) noexcept {
		m_modifiers = modifiers; m_virtualKey = virtualKey;
	}

	// Compare given keycode with item keystroke
	constexpr bool compareKeycode(DWORD modifiers, DWORD virtualKey) const noexcept {
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
		this->copy(other);
	};

	// Operator
	HotkeySetData& operator=(const HotkeySetData& other) {
		this->copy(other);
		return *this;
	};

public:
	// Data processing
	void init(void) noexcept {
		m_hotkeySetList.clear();
	};
	void copy(const HotkeySetData& other) noexcept;
	void setDefaultData(void);

	// Update items
	void add(const Item& item);
	void update(const Item& item);

	// Access items
	const Item& getItemAt(int index) const {
		ASSERT((index >= 0) && (index < getItemNum()));
		if ((index >= 0) && (index < getItemNum()))
			return m_hotkeySetList.at(index);

		AfxThrowInvalidArgException();
	};
	Item& getItemAt(int index)	{
		ASSERT((index >= 0) && (index < getItemNum()));
		if ((index >= 0) && (index < getItemNum()))
			return m_hotkeySetList.at(index);

		AfxThrowInvalidArgException();
	};

	// Item processing
	void adjust(void);
	void remove(int index);
	void removeAll(void) {
		for (int index = 0; index < getItemNum(); index++)
			remove(index);
	};

	// Get attributes
	constexpr size_t getItemNum(void) const noexcept {
		return m_hotkeySetList.size();
	};
	constexpr bool isEmpty(int index) const noexcept {
		if ((index < 0) || (index >= getItemNum())) return true;
		const Item& item = getItemAt(index);
		return item.isEmpty();
	};
	bool isAllEmpty(void) const noexcept;

	// Clean-up
	void deleteItem(int index);
	void deleteAll(void) noexcept {
		m_hotkeySetList.clear();
	};

	// Print item keystrokes by ID
	void printKeyStrokes(unsigned hkID, String& outputString) const;
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
		this->copy(other);
	};

	// Copy assignment operator
	RmdMsgStyleSet& operator=(const RmdMsgStyleSet& other) {
		this->copy(other);
		return *this;
	};

public:
	// Member functions
	void copy(const RmdMsgStyleSet& other) noexcept;
	bool compare(const RmdMsgStyleSet& other) const noexcept;

public:
	// Get/set functions
	constexpr COLORREF getBkgrdColor(void) const noexcept {
		return m_colorBkgrd;
	};
	void setBkgrdColor(COLORREF color) noexcept {
		m_colorBkgrd = color;
	};
	constexpr COLORREF getTextColor(void) const noexcept {
		return m_colorText;
	};
	void setTextColor(COLORREF color) noexcept {
		m_colorText = color;
	};
	String getFontName(void) const noexcept {
		return m_fontName;
	};
	void setFontName(const wchar_t* fontName) noexcept {
		m_fontName = fontName;
	};
	constexpr unsigned getFontSize(void) const noexcept {
		return m_fontSize;
	};
	void setFontSize(unsigned fontSize) noexcept {
		m_fontSize = fontSize;
	};
	constexpr unsigned getTimeout(void) const noexcept {
		return m_timeout;
	};
	void setTimeout(unsigned timeout) noexcept {
		m_timeout = timeout;
	};
	constexpr unsigned getIconId(void) const noexcept {
		return m_iconID;
	};
	void setIconId(unsigned uiIconID) noexcept {
		m_iconID = uiIconID;
	};
	constexpr int getIconSize(void) const noexcept {
		return m_iconSize;
	};
	void setIconSize(int nIconSize) noexcept {
		m_iconSize = nIconSize;
	};
	constexpr byte getIconPosition(void) const noexcept {
		return m_iconPosition;
	};
	void setIconPosition(byte byIconPos) noexcept {
		m_iconPosition = byIconPos;
	};
	constexpr byte getDisplayPosition(void) const noexcept {
		return m_displayPosition;
	};
	void setDisplayPosition(byte byDisplayPos) noexcept {
		m_displayPosition = byDisplayPos;
	};
	constexpr unsigned getHorizontalMargin(void) const noexcept {
		return m_marginHorizontal;
	};
	void setHorizontalMargin(unsigned uiHMargin) noexcept {
		m_marginHorizontal = uiHMargin;
	};
	constexpr unsigned getVerticalMargin(void) const noexcept {
		return m_marginVertical;
	};
	void setVerticalMargin(unsigned uiVMargin) noexcept {
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
		this->copy(other);
	};

	// Copy assignment operator
	PwrReminderItem& operator=(const PwrReminderItem& other) {
		this->copy(other);
		return *this;
	};

public:
	// Data processing
	void copy(const PwrReminderItem& other) noexcept;
	bool compare(const PwrReminderItem& other) const noexcept;
	bool isEmpty(void) const noexcept {
		static const PwrReminderItem pwrDummyItem;
		return this->compare(pwrDummyItem);
	};

	// Access data
	const PwrRepeatSet& getRepeatSetData(void) const noexcept {
		return this->m_repeatSetInfo;
	};
	PwrRepeatSet& getRepeatSetData(void) noexcept {
		return this->m_repeatSetInfo;
	};
	void resetRepeatInfo(void) noexcept {
		const PwrRepeatSet emptyData = PwrRepeatSet();
		this->m_repeatSetInfo.copy(emptyData);
	};
	const RmdMsgStyleSet& getMessageStyleData(void) const noexcept {
		return this->m_msgStyleSetInfo;
	};
	RmdMsgStyleSet& getMessageStyleData(void) noexcept {
		return this->m_msgStyleSetInfo;
	};
	void resetMessageStyleInfo(void) noexcept {
		const RmdMsgStyleSet emptyData = RmdMsgStyleSet();
		this->m_msgStyleSetInfo.copy(emptyData);
	};

public:
	// Get/set attributes
	constexpr bool isEnabled(void) const noexcept {
		return m_isEnabled;
	};
	void enableItem(bool bEnabled) noexcept {
		m_isEnabled = bEnabled;
	};
	constexpr unsigned getItemId(void) const noexcept {
		return m_itemID;
	};
	void setItemId(unsigned nItemID) noexcept {
		m_itemID = nItemID;
	};
	constexpr const wchar_t* getMessage(void) const noexcept {
		return m_messageContent.getString();
	};
	void setMessage(const wchar_t* message) noexcept {
		m_messageContent = message;
	};
	constexpr unsigned getEventId(void) const noexcept {
		return m_eventID;
	};
	void setEventId(unsigned nEventID) noexcept {
		m_eventID = nEventID;
	};
	ClockTime getTime(void) const noexcept {
		return m_timeValue;
	};
	void setTime(const ClockTime& stTime) noexcept {
		m_timeValue = stTime;
	};
	constexpr DWORD getMessageStyle(void) const noexcept {
		return m_messageStyle;
	};
	void setMessageStyle(DWORD nMsgStyleID) noexcept {
		m_messageStyle = nMsgStyleID;
	};
	constexpr bool isCustomStyleEnabled(void) const noexcept {
		return m_useCustomStyle;
	};
	void enableCustomStyle(bool bEnabled) noexcept {
		m_useCustomStyle = bEnabled;
	};

	// Get RepeatSet data
	constexpr bool isRepeatEnabled(void) const noexcept {
		return m_repeatSetInfo.isRepeatEnabled();
	};
	constexpr bool isDayActive(DayOfWeek dayOfWeek) const noexcept {
		return m_repeatSetInfo.isDayActive(dayOfWeek);
	};
	bool isAllowSnoozing(void) const noexcept;
	constexpr int getSnoozeInterval(void) const noexcept {
		return m_repeatSetInfo.getSnoozeInterval();
	};
	constexpr byte getActiveDays(void) const noexcept {
		return m_repeatSetInfo.getActiveDays();
	};

	// Set RepeatSet data
	void enableRepeat(bool bEnabled) noexcept {
		m_repeatSetInfo.enableRepeat(bEnabled);
	};
	void enableSnoozing(bool bEnabled) noexcept {
		m_repeatSetInfo.enableSnoozing(bEnabled);
	};
	void setSnoozeInterval(int nValue) noexcept {
		m_repeatSetInfo.setSnoozeInterval(nValue);
	};
	void setActiveDays(byte byActiveDays) noexcept {
		m_repeatSetInfo.setActiveDays(byActiveDays);
	};
	void setDayActive(DayOfWeek dayOfWeek, bool bActive) noexcept {
		m_repeatSetInfo.setDayActive(dayOfWeek, bActive);
	};

	// Print item data
	void print(String& outputString) const;
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
		this->copy(other);
	};

	// Copy assignment operator
	PwrReminderData& operator=(const PwrReminderData& other) {
		this->copy(other);
		return *this;
	};

public:
	// Member functions
	void init(void) noexcept;
	void copy(const PwrReminderData& other) noexcept;
	void setDefaultData(void) noexcept {
		this->init();
	};

	// Update items
	void add(const PwrReminderItem& pItem);
	void update(const PwrReminderItem& pItem);

	// Access data
	const RmdMsgStyleSet& getCommonStyle(void) const noexcept {
		return m_commonStyleSet;
	};
	RmdMsgStyleSet& getCommonStyle(void) noexcept {
		return m_commonStyleSet;
	};

	// Access items
	const PwrReminderItem& getItemAt(int nIndex) const {
		ASSERT((nIndex >= 0) && (nIndex < getItemNum()));
		if ((nIndex >= 0) && (nIndex < getItemNum()))
			return m_reminderItemList.at(nIndex);

		AfxThrowInvalidArgException();
	};
	PwrReminderItem& getItemAt(int nIndex) {
		ASSERT((nIndex >= 0) && (nIndex < getItemNum()));
		if ((nIndex >= 0) && (nIndex < getItemNum()))
			return m_reminderItemList.at(nIndex);

		AfxThrowInvalidArgException();
	};

	// Item processing
	void adjust(void);
	unsigned getNextId(void) const noexcept;
	void remove(int nIndex);
	void removeAll(void) {
		for (int nIndex = 0; nIndex < getItemNum(); nIndex++)
			remove(nIndex);
	};

	// Get attributes
	size_t getItemNum(void) const noexcept {
		return m_reminderItemList.size();
	};
	bool isEmpty(int nIndex) const noexcept {
		if ((nIndex < 0) || (nIndex >= getItemNum())) return true;
		const PwrReminderItem& pwrItem = getItemAt(nIndex);
		return pwrItem.isEmpty();
	};
	bool isAllEmpty(void) const noexcept;

	// Clean-up
	void deleteItem(int nIndex);
	void deleteAll(void) noexcept {
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
		this->copy(other);
	};

	// Copy assignment operator
	PwrRuntimeItem& operator=(const PwrRuntimeItem& other) {
		this->copy(other);
		return *this;
	};

	// Data processing
	void copy(const PwrRuntimeItem& other) noexcept;
	void calcNextSnoozeTime(int nInterval) noexcept;

public:
	// Get/set attributes
	constexpr int getCategory(void) const noexcept {
		return m_categoryID;
	};
	void setCategory(int nValue) noexcept {
		m_categoryID = nValue;
	};
	constexpr unsigned getItemId(void) const noexcept {
		return m_itemID;
	};
	void setItemId(unsigned nValue) noexcept {
		m_itemID = nValue;
	};
	constexpr int getDisplayFlag(void) const noexcept {
		return m_displayFlag;
	};
	void setDisplayFlag(int nValue) noexcept {
		m_displayFlag = nValue;
	};
	constexpr int getSkipFlag(void) const noexcept {
		return m_skipFlag;
	};
	void setSkipFlag(int nValue) noexcept {
		m_skipFlag = nValue;
	};
	constexpr int getSnoozeFlag(void) const noexcept {
		return m_snoozeFlag;
	};
	void setSnoozeFlag(int nValue) noexcept {
		m_snoozeFlag = nValue;
	};
	ClockTime getTime(void) const noexcept {
		return m_nextSnoozeTime;
	};
	void setTime(const ClockTime& stTime) noexcept {
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
		this->copy(other);
	};

	// Copy assignment operator
	HistoryInfoData& operator=(const HistoryInfoData& other) {
		this->copy(other);
		return *this;
	};

	// Member functions
	void copy(const HistoryInfoData& other) noexcept;
	void init(unsigned nCategoryID) noexcept;
	void removeAll(void) noexcept {
		const HistoryInfoData emptyItem;
		this->copy(emptyItem);
	};

public:
	// Get/set properties
	constexpr bool isInit(void) const noexcept {
		return m_initState;
	};
	constexpr unsigned getCategoryId(void) const noexcept {
		return m_categoryID;
	};
	void setCategoryId(unsigned nCategoryID) noexcept {
		m_categoryID = nCategoryID;
	};
	DateTime getTime(void) const noexcept {
		return m_timestamp;
	};
	void setTime(const DateTime& stTime) noexcept {
		m_timestamp = stTime;
	};
	constexpr unsigned getItemId(void) const noexcept {
		return m_itemID;
	};
	void setItemId(unsigned nItemID) noexcept {
		m_itemID = nItemID;
	};
	constexpr unsigned getActionId(void) const noexcept {
		return m_actionID;
	};
	void setActionId(unsigned nActionID) noexcept {
		m_actionID = nActionID;
	};
	constexpr bool isSuccess(void) const noexcept {
		return m_actionResult;
	};
	void setResult(bool bResult) noexcept {
		m_actionResult = bResult;
	};
	constexpr DWORD getErrorCode(void) const noexcept {
		return m_errorCode;
	};
	void setErrorCode(DWORD dwErrorCode) noexcept {
		m_errorCode = dwErrorCode;
	};
	void getDescription(String& strDescription) const noexcept {
		strDescription = m_description;
	};
	const wchar_t* getDescription(void) const noexcept {
		return m_description.getString();
	};
	void setDescription(const wchar_t* description) noexcept {
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
	EventID getEventId(void) const noexcept {
		return m_eventID;
	};
	DateTime getTimestamp(void) const noexcept {
		return m_timestamp;
	};
	void setTimestamp(DateTime eventTimestamp) noexcept {
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
	constexpr bool isEmpty(void) const noexcept {
		return (m_trackingData.empty());
	};
	constexpr size_t getTrackedCount(void) const noexcept {
		return (m_trackingData.size());
	};

	// Add system event info
	void addEvent(const SystemEvent& eventInfo) {
		m_trackingData.push_back(eventInfo);
	};

	// Remove all tracking data of specific event ID
	void removeAll(SystemEventID eventID) {
		m_trackingData.erase(std::remove_if(m_trackingData.begin(), m_trackingData.end(),
			[eventID](const SystemEvent& eventInfo) { return (eventInfo.getEventId() == eventID); }), m_trackingData.end());
	};

	// Remove all event tracking data
	void removeAll(void) noexcept {
		m_trackingData.clear();
	};

	// Access items
	const SystemEvent& getAt(int nIndex) const {
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
	static String stringFormat(unsigned formatTemplateID, ...);
	static String stringFormat(const wchar_t* formatTemplate, ...);

	// Load resource string/text data
	static String loadResourceString(unsigned resourceStringID);
	static bool	loadResourceString(String& resultStr, unsigned resourceStringID);
	static String loadResourceTextData(unsigned resourceFileID);

	// Make/acquire paths
	static String getApplicationPath(bool includeExeName);
	static String getSubFolderPath(const wchar_t* subFolderName);
	static String makeFilePath(const wchar_t* directory, const wchar_t* fileName, const wchar_t* extension);

	// Get product version
	static String getProductVersion(bool isFullVersion);
	static bool getProductVersion(String& fullVersion, String& shortVersion);

	// Get system and user info
	static bool getDeviceName(String& deviceName);
	static bool getCurrentUserName(String& userName);

	// Print character list
	static int printCharList(const wchar_t* srcStr, String& outputStr);
};


// For clock-time processing and validation
class ClockTimeUtils
{
public:
	// Get current clock-time from system
	static ClockTime getCurrentClockTime(void);

	// Conversion
	static ClockTime fromSystemTime(SYSTEMTIME sysTime);
	static SYSTEMTIME toSystemTime(const ClockTime& clockTime);

	static bool inputText2Time(ClockTime& clockTime, const wchar_t* inputText);
	static bool inputText2TimeBase(ClockTime& clockTime, const wchar_t* inputText);
	static void spinPos2Time(ClockTime& clockTime, int spinPos);
	static void time2SpinPos(const ClockTime& clockTime, int& spinPos);

	// Calculate time offset (increasing/descreasing) in seconds
	static void calculateOffset(ClockTime& clockTime, int offInSecs);

	// Check time matching (in seconds)
	static bool isMatching(ClockTime thisTime, ClockTime otherTime, int offInSecs = 0);

	// Format for displaying/printing
	static String format(LANGTABLE_PTR pLang, unsigned nFormatID, const ClockTime& clockTime);
	static String format(LANGTABLE_PTR pLang, const wchar_t* formatString, const ClockTime& clockTime);
};


// For date/time processing and validation
class DateTimeUtils
{
public:
	// Get current date/time from system
	static DateTime getCurrentDateTime(void);

	// Conversion
	static DateTime fromSystemTime(SYSTEMTIME sysTime);
	static SYSTEMTIME toSystemTime(const DateTime& dateTime);

	// Format for displaying/printing
	static String format(LANGTABLE_PTR pLang, unsigned nFormatID, const DateTime& dateTime);
	static String format(LANGTABLE_PTR pLang, const wchar_t* formatString, const DateTime& dateTime);
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
	void start(void);
	void stop(void);
	double getElapsedTime(bool) const noexcept;
};


// Core methods for application
// These functions will be used elsewhere in the program
namespace AppCore
{
	// Power action execution functions (main core)
	bool executePowerAction(unsigned nActionType, unsigned nMessage, DWORD& dwErrCode);
	bool executePowerActionDummy(unsigned nActionType, unsigned nMessage, DWORD& dwErrCode);

	// Message and notification functions
	LRESULT	waitMessage(unsigned nMsg, int nTimeout = Constant::Max::Timeout::WaitMessage);
	void	showErrorMessage(HWND hMsgOwnerWnd, unsigned nLanguageID, DWORD dwErrorCode, LPARAM lParam = NULL);

	// Convert combo-box selection into option ID
	inline unsigned sel2Opt(unsigned nOptionMacro, unsigned nSelection) {
		VERIFY(nOptionMacro > 0x00 && nOptionMacro < UINT_MAX);
		VERIFY(nSelection >= 0 && nSelection < UINT_MAX);
		return ((nOptionMacro << 8) + (nSelection + 1));
	};

	// Convert option ID into combo-box selection
	inline unsigned opt2Sel(unsigned nOptionMacro, unsigned nCurOption) {
		VERIFY(nOptionMacro > 0x00 && nOptionMacro < UINT_MAX);
		VERIFY(nCurOption >= 0 && nCurOption < UINT_MAX);
		return (nCurOption - (nOptionMacro << 8) - 1);
	};

	// Data/control/window functions
	HWND findDebugTestDlg(void);
	void setFixedCellStyle(CGridCtrl* pGridCtrl, int nRow, int nCol);
	bool setDarkMode(CWnd* pWnd, bool bEnableDarkMode);
	void drawButton(CButton*& pButton, unsigned nIconID, const wchar_t* buttonTitle = Constant::String::Empty);

	// Get Windows OS version
	unsigned getWindowsOSVersion(void);

	// File and media
	void playSound(bool bSoundEnable, unsigned nTypeOfSound);
	bool fileViewStd(FILETYPE eFileType, const wchar_t* filePath);
	bool openWebURL(const wchar_t* webUrl);

	// Applications and instances
	LRESULT runApp(const wchar_t* appPath, bool bRunAsAdmin = false, bool bShowFlag = true);
	LRESULT executeCommand(const wchar_t* commandString, bool bRunAsAdmin = true, bool bShowFlag = true);
	bool	createAppProcess(const wchar_t* appPath, wchar_t* commandLine, unsigned nStyle, DWORD& dwErrorCode);

	// Font name validation
	bool enumFontNames(std::vector<std::wstring>& fontNames);
	bool validateFontName(const wchar_t* fontName);
};

