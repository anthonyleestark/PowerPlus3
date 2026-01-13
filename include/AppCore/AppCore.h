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
		copy(other);
	}

public:
	// Data processing
	void copy(const ConfigData& other) noexcept;
	constexpr bool compare(const ConfigData& other) const noexcept;
	void setDefaultData(void) noexcept {
		static const ConfigData defaultConfig;
		copy(defaultConfig);
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
	UniqueFlagMap uniqueFlagMap_;

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
		auto it = uniqueFlagMap_.find(flagID);
		return (it != uniqueFlagMap_.end());
	}

	// Get application flag value by ID
	int getFlagValue(AppFlagID flagID) const {
		auto it = uniqueFlagMap_.find(flagID);
		if (it != uniqueFlagMap_.end()) return it->second;
		else return FLAG_OFF;
	}

	// Set application flag value by ID
	void setFlagValue(AppFlagID flagID, int value) {
		uniqueFlagMap_[flagID] = value;
	}
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
	static constexpr int kMinSnoozeInterval = 60;					// Min snooze interval: 1 minutes
	static constexpr int kDefaultSnoozeInterval = 600;				// Default snooze interval: 10 minutes
	static constexpr int kMaxSnoozeInterval = 1800;					// Max snooze interval: 30 minutes
	static constexpr int kDefaultActiveDays = 0b01111111;			// Default repeat: All days of weekss

private:
	// Attributes
	BOOL isRepeated_;												// Repeat daily
	BOOL isSnoozingAllowed_;										// Allow snoozing mode
	int	 snoozeInterval_;											// Snooze interval
	byte repeatDays_;												// Days of week (for repeating)

public:
	// Constructor
	PwrRepeatSet();
	PwrRepeatSet(const PwrRepeatSet& other) {
		copy(other);
	}

	// Operator
	PwrRepeatSet& operator=(const PwrRepeatSet& other) {
		copy(other);
		return *this;
	}

public:
	// Data processing
	void copy(const PwrRepeatSet& other) noexcept;
	constexpr bool compare(const PwrRepeatSet& other) const noexcept;

public:
	// Get attributes
	constexpr bool isRepeatEnabled(void) const noexcept {
		return isRepeated_;
	}
	constexpr bool isAllowSnoozing(void) const noexcept {
		return isSnoozingAllowed_;
	}
	constexpr int getSnoozeInterval(void) const noexcept {
		return snoozeInterval_;
	}
	constexpr byte getActiveDays(void) const noexcept {
		return repeatDays_;
	}
	constexpr bool isDayActive(DayOfWeek dayOfWeek) const noexcept {
		if ((dayOfWeek < DayOfWeek::Sunday) || (dayOfWeek > DayOfWeek::Saturday)) return false;
		return ((repeatDays_ & (1 << dayOfWeek)) >> dayOfWeek);
	}

	// Set attributes
	void enableRepeat(bool enabled) noexcept {
		isRepeated_ = enabled;
	}
	void enableSnoozing(bool enabled) noexcept {
		isSnoozingAllowed_ = enabled;
	}
	void setSnoozeInterval(int value) noexcept {
		snoozeInterval_ = value;
	}
	void setActiveDays(byte activeDays) noexcept {
		repeatDays_ = activeDays;
	}
	void setDayActive(DayOfWeek dayOfWeek, bool active) noexcept {
		if ((dayOfWeek < DayOfWeek::Sunday) || (dayOfWeek > DayOfWeek::Saturday)) return;
		repeatDays_ |= active << dayOfWeek;
	}
};

// Define new typenames for RepeatSet data
using PPwrRepeatSet = PwrRepeatSet*;


// Store and manage schedule item data
class ScheduleItem
{
private:
	// Attributes
	unsigned	 itemId_;											// Item ID
	BOOL		 isEnabled_;										// Enable/disable state
	unsigned	 actionId_;											// Schedule action ID
	ClockTime	 timeValue_;										// Schedule time
	PwrRepeatSet repeatSetInfo_;									// Repeat set data

public:
	// Constructor
	ScheduleItem();
	ScheduleItem(unsigned itemID);
	ScheduleItem(const ScheduleItem& other)	{
		copy(other);
	}

	// Copy assignment operator
	ScheduleItem& operator=(const ScheduleItem& other) {
		copy(other);
		return *this;
	}

public:
	// Data processing
	void copy(const ScheduleItem& other) noexcept;
	constexpr bool compare(const ScheduleItem& other) const noexcept;
	bool isEmpty(void) const noexcept {
		static const ScheduleItem schDummyItem;
		return compare(schDummyItem);
	}

public:
	// Get/set attributes
	constexpr unsigned getItemId(void) const noexcept {
		return itemId_;
	}
	void setItemId(unsigned itemID) noexcept {
		itemId_ = itemID;
	}
	constexpr bool isEnabled(void) const noexcept {
		return isEnabled_;
	}
	void enableItem(bool enabled) noexcept {
		isEnabled_ = enabled;
	}
	constexpr unsigned getAction(void) const noexcept {
		return actionId_;
	}
	void setAction(unsigned actionID) noexcept {
		actionId_ = actionID;
	}
	ClockTime getTime(void) const noexcept {
		return timeValue_;
	}
	void setTime(const ClockTime& time) noexcept {
		timeValue_ = time;
	}

	// Get RepeatSet data
	constexpr bool isRepeatEnabled(void) const noexcept {
		return (repeatSetInfo_.isRepeatEnabled());
	}

	constexpr bool isAllowSnoozing(void) const noexcept {
		if (isRepeatEnabled() != true) return false;
		if (repeatSetInfo_.isAllowSnoozing() != true) return false;
		return true;
	}
	constexpr bool isDayActive(DayOfWeek dayOfWeek) const noexcept {
		return (repeatSetInfo_.isDayActive(dayOfWeek));
	}
	constexpr byte getActiveDays(void) const noexcept {
		return (repeatSetInfo_.getActiveDays());
	}

	// Set RepeatSet data
	void enableRepeat(bool isEnabled) noexcept {
		repeatSetInfo_.enableRepeat(isEnabled);
	}
	void enableSnoozing(bool isEnabled) noexcept {
		repeatSetInfo_.enableSnoozing(isEnabled);
	}
	void setSnoozeInterval(int value) noexcept {
		repeatSetInfo_.setSnoozeInterval(value);
	}
	void setActiveDays(byte activeDays) noexcept {
		repeatSetInfo_.setActiveDays(activeDays);
	}
	void setDayActive(DayOfWeek dayOfWeek, bool isActive) noexcept {
		repeatSetInfo_.setDayActive(dayOfWeek, isActive);
	}

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
	static constexpr int kDefaultItemNum = 1;						// Default item number: 1
	static constexpr int kMaxItemNum = 100;							// Max item number: 100
	static constexpr int kDefaultItemID = 0x00;						// Default item ID: 0
	static constexpr int kMinItemID = 10000;						// Min item ID: 10000
	static constexpr int kMaxItemID = 19999;						// Max item ID: 19999
	static constexpr int kDefaultActionID = APP_ACTION_DISPLAYOFF;	// Default action (for new item): Turn off display

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
	ScheduleItem	 defaultItem_;
	ScheduleItemList extraItemList_;

public:
	// Constructor
	ScheduleData();
	ScheduleData(const ScheduleData& other) {
		copy(other);
	}

	// Copy assignment operator
	ScheduleData& operator=(const ScheduleData& other) {
		copy(other);
		return *this;
	}

public:
	// Data processing
	void init(void) {
		defaultItem_ = ScheduleItem(ScheduleData::kDefaultItemID);
		extraItemList_.clear();
	}
	void copy(const ScheduleData& other);
	void setDefaultData(void) {
		init();
	}

	// Update items
	DWORD add(const ScheduleItem& item);
	DWORD update(const ScheduleItem& item);

	// Access items
	const ScheduleItem& getDefaultItem(void) const noexcept {
		return defaultItem_;
	}
	ScheduleItem& getDefaultItem(void) noexcept {
		return defaultItem_;
	}
	const ScheduleItem& getItemAt(int index) const {
		ASSERT((index >= 0) && (index < getExtraItemNum()));
		if ((index >= 0) && (index < getExtraItemNum()))
			return extraItemList_.at(index);

		AfxThrowInvalidArgException();
	}
	ScheduleItem& getItemAt(int index) {
		ASSERT((index >= 0) && (index < getExtraItemNum()));
		if ((index >= 0) && (index < getExtraItemNum()))
			return extraItemList_.at(index);

		AfxThrowInvalidArgException();
	}

	// Item processing
	void adjust(void);
	unsigned getNextId(void) const;
	void remove(int index);
	void removeAll(void) {
		for (int index = 0; index < getExtraItemNum(); index++)
			remove(index);
	}

	// Get attributes
	constexpr size_t getExtraItemNum(void) const noexcept {
		return extraItemList_.size();
	}
	bool isDefaultEmpty(void) const noexcept {
		return defaultItem_.isEmpty();
	}
	constexpr bool isEmpty(int index) const noexcept {
		if ((index < 0) || (index >= getExtraItemNum())) return true;
		const ScheduleItem& item = getItemAt(index);
		return item.isEmpty();
	}

	// Check if extra data is empty
	constexpr bool isExtraEmpty(void) const noexcept;
	bool isAllEmpty(void) const noexcept {
		return (isDefaultEmpty() && isExtraEmpty());
	}

	// Clean-up
	void deleteItem(int index);
	void deleteExtra(void) noexcept {
		extraItemList_.clear();
	}
	void deleteAll(void) noexcept {
		defaultItem_ = ScheduleItem(ScheduleData::kDefaultItemID);
		extraItemList_.clear();
	}
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
	bool		isEnabled_;											// Hotkey enabled/disabled
	unsigned	hotkeyActionId_;									// Hotkey action ID
	DWORD		modifiers_;											// Modifier keys
	DWORD		virtualKey_;										// Virtual key code

public:
	// Constructor
	HotkeySetItem();
	HotkeySetItem(unsigned hotkeyActionId);
	HotkeySetItem(const HotkeySetItem& other) {
		copy(other);
	}

	// Copy assignment operator
	HotkeySetItem& operator=(const HotkeySetItem& other) {
		copy(other);
		return *this;
	}

public:
	// Data processing
	void copy(const HotkeySetItem& other) noexcept;
	constexpr bool isEmpty(void) const noexcept {
		bool isEmpty = (modifiers_ == 0);
		isEmpty &= (virtualKey_ == 0);
		return isEmpty;
	}
	constexpr bool compare(const HotkeySetItem& other) const noexcept {
		bool ret = (hotkeyActionId_ == other.hotkeyActionId_);
		ret &= compareKeycode(other);
		return ret;
	}
	constexpr bool compareKeycode(const HotkeySetItem& other) const noexcept {
		return compareKeycode(other.modifiers_, other.virtualKey_);
	}

	// Print item data
	void print(String& outputString) const;
	void printKeyStrokes(String& outputString) const;

public:
	// Get/set attributes
	constexpr bool isEnabled(void) const noexcept {
		return isEnabled_;
	}
	void enableItem(bool enabled) noexcept {
		isEnabled_ = enabled;
	}
	constexpr unsigned getActionId(void) const noexcept {
		return hotkeyActionId_;
	}
	void setActionId(unsigned hotkeyActionId) noexcept {
		hotkeyActionId_ = hotkeyActionId;
	}
	constexpr void getKeyCode(DWORD& modifiers, DWORD& virtualKey) const noexcept {
		modifiers = modifiers_; virtualKey = virtualKey_;
	}
	void setKeyCode(DWORD modifiers, DWORD virtualKey) noexcept {
		modifiers_ = modifiers; virtualKey_ = virtualKey;
	}

	// Compare given keycode with item keystroke
	constexpr bool compareKeycode(DWORD modifiers, DWORD virtualKey) const noexcept {
		return ((modifiers_ == modifiers) && (virtualKey_ == virtualKey));
	}
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
	DataList hotkeySetList_;

public:
	// Constructor
	HotkeySetData() = default;
	HotkeySetData(const HotkeySetData& other) {
		copy(other);
	}

	// Operator
	HotkeySetData& operator=(const HotkeySetData& other) {
		copy(other);
		return *this;
	}

public:
	// Data processing
	void init(void) noexcept {
		hotkeySetList_.clear();
	}
	void copy(const HotkeySetData& other) noexcept;
	void setDefaultData(void);

	// Update items
	void add(const Item& item);
	void update(const Item& item);

	// Access items
	const Item& getItemAt(int index) const {
		ASSERT((index >= 0) && (index < getItemNum()));
		if ((index >= 0) && (index < getItemNum()))
			return hotkeySetList_.at(index);

		AfxThrowInvalidArgException();
	}
	Item& getItemAt(int index)	{
		ASSERT((index >= 0) && (index < getItemNum()));
		if ((index >= 0) && (index < getItemNum()))
			return hotkeySetList_.at(index);

		AfxThrowInvalidArgException();
	}

	// Item processing
	void adjust(void);
	void remove(int index);
	void removeAll(void) {
		for (int index = 0; index < getItemNum(); index++)
			remove(index);
	}

	// Get attributes
	constexpr size_t getItemNum(void) const noexcept {
		return hotkeySetList_.size();
	}
	constexpr bool isEmpty(int index) const noexcept {
		if ((index < 0) || (index >= getItemNum())) return true;
		const Item& item = getItemAt(index);
		return item.isEmpty();
	}
	bool isAllEmpty(void) const noexcept;

	// Clean-up
	void deleteItem(int index);
	void deleteAll(void) noexcept {
		hotkeySetList_.clear();
	}

	// Print item keystrokes by ID
	void printKeyStrokes(unsigned hotkeyId, String& outputString) const;
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
	static constexpr COLORREF kDefaultBkgrdColor = Color::Pink;
	static constexpr COLORREF kDefaultTextColor = Color::Red;
	static constexpr const wchar_t* kDefaultFontName = _T("Arial");
	static constexpr int kDefaultFontSize = 20;
	static constexpr int kDefaultTimeout = 0;
	static constexpr int kDefaultIconID = SystemIcon::Information;
	static constexpr int kDefaultIconSize = 50;
	static constexpr int kDefaultIconPosition = IconOnTheTop;
	static constexpr int kDefaultDisplayPosition = AtCenter;
	static constexpr int kDefaultHorizontalMargin = 50;
	static constexpr int kDefaultVerticalMargin = 50;

public:
	// Define constant values
	static constexpr int kMinFontSize = 10;
	static constexpr int kMaxFontSize = 100;
	static constexpr int kMinTimeOut = 10;
	static constexpr int kMaxTimeOut = 1800;
	static constexpr int kMinIconSize = 30;
	static constexpr int kMaxIconSize = 100;
	static constexpr int kMinMarginVal = 10;
	static constexpr int kMaxMarginVal = 120;

private:
	// Attributes
	COLORREF	colorBackground_;							// Background color
	COLORREF	colorText_;									// Text color
	String		fontName_;									// Font name
	unsigned	fontSize_;									// Font size
	unsigned	timeoutValue_;								// Timeout (auto-close) interval
	unsigned	iconId_;									// Message icon ID
	int			iconSize_;									// Message icon size
	byte		iconPosition_;								// Message icon position
	byte		displayPosition_;							// Message display position
	unsigned	marginHorizontal_;							// Display area horizontal margin
	unsigned	marginVertical_;							// Display area vertical margin

public:
	// Constructor
	RmdMsgStyleSet();
	RmdMsgStyleSet(const RmdMsgStyleSet& other) {
		copy(other);
	}

	// Copy assignment operator
	RmdMsgStyleSet& operator=(const RmdMsgStyleSet& other) {
		copy(other);
		return *this;
	}

public:
	// Member functions
	void copy(const RmdMsgStyleSet& other) noexcept;
	bool compare(const RmdMsgStyleSet& other) const noexcept;

public:
	// Get/set functions
	constexpr COLORREF getBkgrdColor(void) const noexcept {
		return colorBackground_;
	}
	void setBkgrdColor(COLORREF color) noexcept {
		colorBackground_ = color;
	}
	constexpr COLORREF getTextColor(void) const noexcept {
		return colorText_;
	}
	void setTextColor(COLORREF color) noexcept {
		colorText_ = color;
	}
	String getFontName(void) const noexcept {
		return fontName_;
	}
	void setFontName(const wchar_t* fontName) noexcept {
		fontName_ = fontName;
	}
	constexpr unsigned getFontSize(void) const noexcept {
		return fontSize_;
	}
	void setFontSize(unsigned fontSize) noexcept {
		fontSize_ = fontSize;
	}
	constexpr unsigned getTimeout(void) const noexcept {
		return timeoutValue_;
	}
	void setTimeout(unsigned timeout) noexcept {
		timeoutValue_ = timeout;
	}
	constexpr unsigned getIconId(void) const noexcept {
		return iconId_;
	}
	void setIconId(unsigned iconId) noexcept {
		iconId_ = iconId;
	}
	constexpr int getIconSize(void) const noexcept {
		return iconSize_;
	}
	void setIconSize(int iconSize) noexcept {
		iconSize_ = iconSize;
	}
	constexpr byte getIconPosition(void) const noexcept {
		return iconPosition_;
	}
	void setIconPosition(byte iconPosition) noexcept {
		iconPosition_ = iconPosition;
	}
	constexpr byte getDisplayPosition(void) const noexcept {
		return displayPosition_;
	}
	void setDisplayPosition(byte displayPosition) noexcept {
		displayPosition_ = displayPosition;
	}
	constexpr unsigned getHorizontalMargin(void) const noexcept {
		return marginHorizontal_;
	}
	void setHorizontalMargin(unsigned marginHorizontal) noexcept {
		marginHorizontal_ = marginHorizontal;
	}
	constexpr unsigned getVerticalMargin(void) const noexcept {
		return marginVertical_;
	}
	void setVerticalMargin(unsigned marginVertical) noexcept {
		marginVertical_ = marginVertical;
	}
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
	BOOL			isEnabled_;										// Enable state
	unsigned		itemId_;										// Item ID
	String			messageContent_;								// Message content
	unsigned		eventId_;										// Event ID
	ClockTime		timeValue_;										// Event time
	DWORD			messageStyle_;									// Reminder style
	PwrRepeatSet	repeatSetInfo_;									// Repeat set data
	BOOL			useCustomStyle_;								// Use message custom style
	RmdMsgStyleSet	msgStyleSetInfo_;								// Reminder message style set

public:
	// Constructor
	PwrReminderItem();
	PwrReminderItem(const PwrReminderItem& other) {
		copy(other);
	}

	// Copy assignment operator
	PwrReminderItem& operator=(const PwrReminderItem& other) {
		copy(other);
		return *this;
	}

public:
	// Data processing
	void copy(const PwrReminderItem& other) noexcept;
	bool compare(const PwrReminderItem& other) const noexcept;
	bool isEmpty(void) const noexcept {
		static const PwrReminderItem pwrDummyItem;
		return compare(pwrDummyItem);
	}

	// Access data
	const PwrRepeatSet& getRepeatSetData(void) const noexcept {
		return repeatSetInfo_;
	}
	PwrRepeatSet& getRepeatSetData(void) noexcept {
		return repeatSetInfo_;
	}
	void resetRepeatInfo(void) noexcept {
		const PwrRepeatSet emptyData = PwrRepeatSet();
		repeatSetInfo_.copy(emptyData);
	}
	const RmdMsgStyleSet& getMessageStyleData(void) const noexcept {
		return msgStyleSetInfo_;
	}
	RmdMsgStyleSet& getMessageStyleData(void) noexcept {
		return msgStyleSetInfo_;
	}
	void resetMessageStyleInfo(void) noexcept {
		const RmdMsgStyleSet emptyData = RmdMsgStyleSet();
		msgStyleSetInfo_.copy(emptyData);
	}

public:
	// Get/set attributes
	constexpr bool isEnabled(void) const noexcept {
		return isEnabled_;
	}
	void enableItem(bool isEnabled) noexcept {
		isEnabled_ = isEnabled;
	}
	constexpr unsigned getItemId(void) const noexcept {
		return itemId_;
	}
	void setItemId(unsigned itemId) noexcept {
		itemId_ = itemId;
	}
	constexpr const wchar_t* getMessage(void) const noexcept {
		return messageContent_.getString();
	}
	void setMessage(const wchar_t* message) noexcept {
		messageContent_ = message;
	}
	constexpr unsigned getEventId(void) const noexcept {
		return eventId_;
	}
	void setEventId(unsigned eventId) noexcept {
		eventId_ = eventId;
	}
	ClockTime getTime(void) const noexcept {
		return timeValue_;
	}
	void setTime(const ClockTime& timeValue) noexcept {
		timeValue_ = timeValue;
	}
	constexpr DWORD getMessageStyle(void) const noexcept {
		return messageStyle_;
	}
	void setMessageStyle(DWORD messageStyle) noexcept {
		messageStyle_ = messageStyle;
	}
	constexpr bool isCustomStyleEnabled(void) const noexcept {
		return useCustomStyle_;
	}
	void enableCustomStyle(bool isEnabled) noexcept {
		useCustomStyle_ = isEnabled;
	}

	// Get RepeatSet data
	constexpr bool isRepeatEnabled(void) const noexcept {
		return repeatSetInfo_.isRepeatEnabled();
	}
	constexpr bool isDayActive(DayOfWeek dayOfWeek) const noexcept {
		return repeatSetInfo_.isDayActive(dayOfWeek);
	}
	bool isAllowSnoozing(void) const noexcept;
	constexpr int getSnoozeInterval(void) const noexcept {
		return repeatSetInfo_.getSnoozeInterval();
	}
	constexpr byte getActiveDays(void) const noexcept {
		return repeatSetInfo_.getActiveDays();
	}

	// Set RepeatSet data
	void enableRepeat(bool isEnabled) noexcept {
		repeatSetInfo_.enableRepeat(isEnabled);
	}
	void enableSnoozing(bool isEnabled) noexcept {
		repeatSetInfo_.enableSnoozing(isEnabled);
	}
	void setSnoozeInterval(int value) noexcept {
		repeatSetInfo_.setSnoozeInterval(value);
	}
	void setActiveDays(byte activeDays) noexcept {
		repeatSetInfo_.setActiveDays(activeDays);
	}
	void setDayActive(DayOfWeek dayOfWeek, bool isActive) noexcept {
		repeatSetInfo_.setDayActive(dayOfWeek, isActive);
	}

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
	static constexpr int kMaxItemNum = 100;							// Max item number: 100
	static constexpr int kMinItemID = 10000;						// Min item ID: 10000
	static constexpr int kMaxItemID = 19999;						// Max item ID: 19999
	static constexpr int kPreviewTimeout = 10;						// Default time-out for preview: 10s

private:
	// Attributes
	PwrReminderItemList	reminderItemList_;							// List of reminder items
	RmdMsgStyleSet		commonStyleSet_;							// Common message style set

public:
	// Constructor
	PwrReminderData();
	PwrReminderData(const PwrReminderData& other) {
		copy(other);
	}

	// Copy assignment operator
	PwrReminderData& operator=(const PwrReminderData& other) {
		copy(other);
		return *this;
	}

public:
	// Member functions
	void init(void) noexcept;
	void copy(const PwrReminderData& other) noexcept;
	void setDefaultData(void) noexcept {
		init();
	}

	// Update items
	void add(const PwrReminderItem& reminderItem);
	void update(const PwrReminderItem& reminderItem);

	// Access data
	const RmdMsgStyleSet& getCommonStyle(void) const noexcept {
		return commonStyleSet_;
	}
	RmdMsgStyleSet& getCommonStyle(void) noexcept {
		return commonStyleSet_;
	}

	// Access items
	const PwrReminderItem& getItemAt(int index) const {
		ASSERT((index >= 0) && (index < getItemNum()));
		if ((index >= 0) && (index < getItemNum()))
			return reminderItemList_.at(index);

		AfxThrowInvalidArgException();
	}
	PwrReminderItem& getItemAt(int index) {
		ASSERT((index >= 0) && (index < getItemNum()));
		if ((index >= 0) && (index < getItemNum()))
			return reminderItemList_.at(index);

		AfxThrowInvalidArgException();
	}

	// Item processing
	void adjust(void);
	unsigned getNextId(void) const noexcept;
	void remove(int index);
	void removeAll(void) {
		for (int index = 0; index < getItemNum(); index++)
			remove(index);
	}

	// Get attributes
	size_t getItemNum(void) const noexcept {
		return reminderItemList_.size();
	}
	bool isEmpty(int index) const noexcept {
		if ((index < 0) || (index >= getItemNum())) return true;
		const PwrReminderItem& reminderItem = getItemAt(index);
		return reminderItem.isEmpty();
	}
	bool isAllEmpty(void) const noexcept;

	// Clean-up
	void deleteItem(int index);
	void deleteAll(void) noexcept {
		// Reset data
		reminderItemList_.clear();
		commonStyleSet_ = RmdMsgStyleSet();
	}
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
	int			categoryId_;										// Item category
	unsigned	itemId_;											// Power Reminder item ID
	int			displayFlag_;										// Item displaying flag
	int			skipFlag_;											// Item skip flag
	int			snoozeFlag_;										// Item snooze trigger flag
	ClockTime	nextSnoozeTime_;									// Next snooze trigger time

public:
	// Constructor
	PwrRuntimeItem();
	PwrRuntimeItem(const PwrRuntimeItem& other)	{
		copy(other);
	}

	// Copy assignment operator
	PwrRuntimeItem& operator=(const PwrRuntimeItem& other) {
		copy(other);
		return *this;
	}

	// Data processing
	void copy(const PwrRuntimeItem& other) noexcept;
	void calcNextSnoozeTime(int interval) noexcept;

public:
	// Get/set attributes
	constexpr int getCategory(void) const noexcept {
		return categoryId_;
	}
	void setCategory(int value) noexcept {
		categoryId_ = value;
	}
	constexpr unsigned getItemId(void) const noexcept {
		return itemId_;
	}
	void setItemId(unsigned value) noexcept {
		itemId_ = value;
	}
	constexpr int getDisplayFlag(void) const noexcept {
		return displayFlag_;
	}
	void setDisplayFlag(int value) noexcept {
		displayFlag_ = value;
	}
	constexpr int getSkipFlag(void) const noexcept {
		return skipFlag_;
	}
	void setSkipFlag(int value) noexcept {
		skipFlag_ = value;
	}
	constexpr int getSnoozeFlag(void) const noexcept {
		return snoozeFlag_;
	}
	void setSnoozeFlag(int value) noexcept {
		snoozeFlag_ = value;
	}
	ClockTime getTime(void) const noexcept {
		return nextSnoozeTime_;
	}
	void setTime(const ClockTime& timeValue) noexcept {
		nextSnoozeTime_ = timeValue;
	}
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
	bool		isInitiated_;										// Init state flag
	unsigned	categoryId_;										// Category ID
	DateTime	timestampValue_;									// Timestamp of history
	unsigned	itemId_;											// Item ID
	unsigned	actionId_;											// Action ID
	bool		actionResult_;										// Action result
	DWORD		errorCode_;											// Returned error code
	String		description_;										// History description (attached info)

public:
	// Constructor
	HistoryInfoData();
	HistoryInfoData(const HistoryInfoData& other) {
		copy(other);
	}

	// Copy assignment operator
	HistoryInfoData& operator=(const HistoryInfoData& other) {
		copy(other);
		return *this;
	}

	// Member functions
	void copy(const HistoryInfoData& other) noexcept;
	void init(unsigned categoryId) noexcept;
	void removeAll(void) noexcept {
		const HistoryInfoData emptyItem;
		copy(emptyItem);
	}

public:
	// Get/set properties
	constexpr bool isInit(void) const noexcept {
		return isInitiated_;
	}
	constexpr unsigned getCategoryId(void) const noexcept {
		return categoryId_;
	}
	void setCategoryId(unsigned categoryId) noexcept {
		categoryId_ = categoryId;
	}
	DateTime getTime(void) const noexcept {
		return timestampValue_;
	}
	void setTime(const DateTime& timeValue) noexcept {
		timestampValue_ = timeValue;
	}
	constexpr unsigned getItemId(void) const noexcept {
		return itemId_;
	}
	void setItemId(unsigned itemId) noexcept {
		itemId_ = itemId;
	}
	constexpr unsigned getActionId(void) const noexcept {
		return actionId_;
	}
	void setActionId(unsigned actionId) noexcept {
		actionId_ = actionId;
	}
	constexpr bool isSuccess(void) const noexcept {
		return actionResult_;
	}
	void setResult(bool result) noexcept {
		actionResult_ = result;
	};
	constexpr DWORD getErrorCode(void) const noexcept {
		return errorCode_;
	}
	void setErrorCode(DWORD errorCode) noexcept {
		errorCode_ = errorCode;
	}
	void getDescription(String& description) const noexcept {
		description = description_;
	}
	const wchar_t* getDescription(void) const noexcept {
		return description_.getString();
	}
	void setDescription(const wchar_t* description) noexcept {
		description_ = description;
	}
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
	EventID	 eventId_;												// System event ID
	DateTime timestampValue_;										// Event timestamp

public:
	// Construction
	SystemEvent(EventID eventID);
	SystemEvent(const SystemEvent& other);

	// Copy assignment operator
	SystemEvent& operator=(const SystemEvent& other);

public:
	// Get/set functions
	EventID getEventId(void) const noexcept {
		return eventId_;
	}
	DateTime getTimestamp(void) const noexcept {
		return timestampValue_;
	}
	void setTimestamp(DateTime eventTimestamp) noexcept {
		timestampValue_ = eventTimestamp;
	}
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
	SystemEventData trackingData_;								// System event tracking data

public:
	// Construction
	SystemEventTracker();
	SystemEventTracker(const SystemEventTracker& other);

	// Copy assignment operator
	SystemEventTracker& operator=(const SystemEventTracker& other);

public:
	// Validation
	constexpr bool isEmpty(void) const noexcept {
		return (trackingData_.empty());
	}
	constexpr size_t getTrackedCount(void) const noexcept {
		return (trackingData_.size());
	}

	// Add system event info
	void addEvent(const SystemEvent& eventInfo) {
		trackingData_.push_back(eventInfo);
	}

	// Remove all tracking data of specific event ID
	void removeAll(SystemEventID eventID) {
		trackingData_.erase(std::remove_if(trackingData_.begin(), trackingData_.end(),
			[eventID](const SystemEvent& eventInfo) { return (eventInfo.getEventId() == eventID); }), trackingData_.end());
	}

	// Remove all event tracking data
	void removeAll(void) noexcept {
		trackingData_.clear();
	}

	// Access items
	const SystemEvent& getAt(int index) const {
		return (trackingData_.at(index));
	}
};


// Store grid control/view column format info
typedef struct tagGRIDCTRLCOLFMT 
{
	// Member variables
	int			columnId;									// Column ID
	unsigned	headerTitleId;								// Header title string ID
	int			width;										// Column width
	unsigned	columnStyle;								// Column style
	bool		isCentered;									// Align center
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
	bool resetFlag;											// Reset flag when denied
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
	static String format(LANGTABLE_PTR languageTablePtr, unsigned formatId, const ClockTime& clockTime);
	static String format(LANGTABLE_PTR languageTablePtr, const wchar_t* formatString, const ClockTime& clockTime);
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
	static String format(LANGTABLE_PTR languageTablePtr, unsigned formatId, const DateTime& dateTime);
	static String format(LANGTABLE_PTR languageTablePtr, const wchar_t* formatString, const DateTime& dateTime);
};


// For querrying performance counter of functions
class PerformanceCounter
{
private:
	// Attributes
	LARGE_INTEGER startTime_;
	LARGE_INTEGER endTime_;
	LARGE_INTEGER frequency_;

	// Counting flag
	bool isRunning_;

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
	bool executePowerAction(unsigned actionType, unsigned message, DWORD& errorCode);
	bool executePowerActionDummy(unsigned actionType, unsigned message, DWORD& errorCode);

	// Message and notification functions
	LRESULT	waitMessage(unsigned message, int timeout = Constant::Max::Timeout::WaitMessage);
	void	showErrorMessage(HWND msgOwnerWnd, unsigned languageId, DWORD errorCode, LPARAM lParam = NULL);

	// Convert combo-box selection into option ID
	inline unsigned sel2Opt(unsigned optionMacro, unsigned selection) {
		VERIFY(optionMacro > 0x00 && optionMacro < UINT_MAX);
		VERIFY(selection >= 0 && selection < UINT_MAX);
		return ((optionMacro << 8) + (selection + 1));
	}

	// Convert option ID into combo-box selection
	inline unsigned opt2Sel(unsigned optionMacro, unsigned currentOption) {
		VERIFY(optionMacro > 0x00 && optionMacro < UINT_MAX);
		VERIFY(currentOption >= 0 && currentOption < UINT_MAX);
		return (currentOption - (optionMacro << 8) - 1);
	}

	// Data/control/window functions
	HWND findDebugTestDlg(void);
	void setFixedCellStyle(CGridCtrl* gridCtrlPtr, int row, int col);
	bool setDarkMode(CWnd* wndPtr, bool enableDarkMode);
	void drawButton(CButton*& buttonPtr, unsigned iconID, const wchar_t* buttonTitle = Constant::String::Empty);

	// Get Windows OS version
	unsigned getWindowsOSVersion(void);

	// File and media
	void playSound(bool isSoundEnabled, unsigned typeOfSound);
	bool fileViewStd(FILETYPE fileType, const wchar_t* filePath);
	bool openWebURL(const wchar_t* webUrl);

	// Applications and instances
	LRESULT runApp(const wchar_t* appPath, bool runAsAdmin = false, bool showFlag = true);
	LRESULT executeCommand(const wchar_t* commandString, bool runAsAdmin = true, bool showFlag = true);
	bool	createAppProcess(const wchar_t* appPath, wchar_t* commandLine, unsigned style, DWORD& errorCode);

	// Font name validation
	bool enumFontNames(std::vector<std::wstring>& fontNameList);
	bool validateFontName(const wchar_t* fontName);
};

