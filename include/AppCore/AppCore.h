
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Core.h
//		Description:	Define and implement core data types, macros and methods which will be used elsewhere in program
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//						<3> 2024.07.06:		Update to version 3.1
//						<4> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AppBase/AppBase.h"
#include "Components/GridCtrl/GridCtrl.h"
#include "Components/GridCtrl/GridCellCheck.h"
#include "Language.h"

/////////////////////////////////////

//// Declarations


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define enum datas for program, these datas will be used elsewhere in the programs
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////// ********************
// 
// Grid table column style
//
//////////////////// ********************

typedef enum eGRIDCOLSTYLE {
	COLSTYLE_FIXED = 0,					// Fixed cell
	COLSTYLE_CHECKBOX,					// Checkbox cell
	COLSTYLE_NORMAL,					// Normal cell
} GRIDCOLSTYLE;


//////////////////// ********************
// 
// File types (view file mode)
//
//////////////////// ********************

typedef enum eFILETYPE {
	FILETYPE_TEXT = 0,					// Text files
	FILETYPE_IMAGE,						// Image files
} FILETYPE;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define data types for program, these data types will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	CONFIGDATA
//  Description:	Store application settings and configurations
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

struct CONFIGDATA
{
	// Main settings
	INT		nLMBAction;												// Left mouse button action
	INT		nMMBAction;												// Middle mouse button action
	INT		nRMBAction;												// Right mouse button action
	BOOL	bRMBShowMenu;											// Right mouse button: Only show menu

	// Display setting
	INT		nLanguageID;											// Language setting

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

// Define typename
using PCONFIGDATA = CONFIGDATA*;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		ConfigData
//  Description:	Class for application config data management
//  Derivered from:	CONFIGDATA
//
//////////////////////////////////////////////////////////////////////////

class ConfigData : public CONFIGDATA
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
	ConfigData();													// Default constructor
	ConfigData(const CONFIGDATA&);									// Copy constructor

public:
	// Data processing
	void Copy(const CONFIGDATA&);									// Copy data
	BOOL Compare(const CONFIGDATA&) const;							// Compare data
	void SetDefaultData(void);										// Set default data

	// Access data
	void GetData(CONFIGDATA&) const;								// Get a copy of config data
	int  GetAppOption(AppOptionID) const;							// Get application option data by ID
};

// Define new global typenames for the enum attributes of Application config data
using AppOptionID = ConfigData::AppOptionID;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		FlagManager
//  Description:	Class for application flag data management
//
//////////////////////////////////////////////////////////////////////////

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
	using UniqueFlagMap = typename std::unordered_map<AppFlagID, INT>;

private:
	// Attributes
	UniqueFlagMap m_mapUniqueFlags;

public:
	// Constructor
	FlagManager(void) = default;									// Default constructor

private:
	// No copyable
	FlagManager(const FlagManager&) = delete;
	FlagManager& operator=(const FlagManager&) = delete;

	// No movable
	FlagManager(const FlagManager&&) = delete;
	FlagManager& operator=(const FlagManager&&) = delete;

public:
	// Member functions
	BOOL IsFlagPresent(AppFlagID) const;							// Check if a flag value exists
	int  GetFlagValue(AppFlagID) const;								// Get flag value by ID
	void SetFlagValue(AppFlagID, int);								// Set flag value by ID
};

// Define new global typenames for the enum attributes of Application flag data
using AppFlagID = FlagManager::AppFlagID;
using FlagManagerID = FlagManager::ManagerID;
using FlagManagementMethod = FlagManager::ManagementMethod;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		PwrRepeatSet
//  Description:	Store and manage data of a Power++ item repeat set
//
//////////////////////////////////////////////////////////////////////////

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
	INT	 m_nSnoozeInterval;											// Snooze interval
	BYTE m_byRepeatDays;											// Days of week (for repeating)

public:
	// Constructor
	PwrRepeatSet();													// Default constructor
	PwrRepeatSet(const PwrRepeatSet&);								// Copy constructor

	// Operator
	PwrRepeatSet& operator=(const PwrRepeatSet&);					// Copy assignment operator

public:
	// Data processing
	void Copy(const PwrRepeatSet&);									// Copy data
	BOOL Compare(const PwrRepeatSet&) const;						// Compare data

public:
	// Get attributes
	BOOL IsRepeatEnabled(void) const;								// Check if repeat option is enabled
	BOOL IsAllowSnoozing(void) const;								// Check if snooze option is enabled
	INT	 GetSnoozeInterval(void) const;								// Get snooze interval data
	BYTE GetActiveDays(void) const;									// Get repeat days data
	BOOL IsDayActive(DayOfWeek) const;								// Check if day of week is active

	// Set attributes
	void EnableRepeat(BOOL);										// Set repeat enable state
	void EnableSnoozing(BOOL);										// Set allow snoozing state
	void SetSnoozeInterval(INT);									// Set snooze interval data
	void SetActiveDays(BYTE);										// Set repeat days data
	void SetDayActive(DayOfWeek, BOOL);								// Set active state for specific day of week
};

// Define new typenames for RepeatSet data
using PPwrRepeatSet = PwrRepeatSet*;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		ScheduleItem
//  Description:	Store schedule item settings
//
//////////////////////////////////////////////////////////////////////////

class ScheduleItem
{
private:
	// Attributes
	UINT		 m_nItemID;											// Item ID
	BOOL		 m_bEnabled;										// Enable/disable state
	UINT		 m_nActionID;										// Schedule action ID
	ClockTime	 m_stTime;											// Schedule time
	PwrRepeatSet m_rpsRepeatSet;									// Repeat set data

public:
	// Constructor
	ScheduleItem();													// Default constructor
	ScheduleItem(UINT nItemID);										// Overloaded constructor
	ScheduleItem(const ScheduleItem&);								// Copy constructor

	// Operator
	ScheduleItem& operator=(const ScheduleItem&);					// Copy assignment operator

public:
	// Data processing
	void Copy(const ScheduleItem&);									// Copy data
	BOOL Compare(const ScheduleItem&) const;						// Compare items
	BOOL IsEmpty(void) const;										// Check if item data is empty

public:
	// Get/set attributes
	UINT GetItemID(void) const;										// Get schedule item ID
	void SetItemID(UINT);											// Set schedule item ID
	BOOL IsEnabled(void) const;										// Check if item is enabled
	void EnableItem(BOOL);											// Set item active state
	UINT GetAction(void) const;										// Get item action ID
	void SetAction(UINT);											// Set item action ID
	ClockTime GetTime(void) const;									// Get schedule item time data
	void SetTime(const ClockTime&);									// Set schedule item time data

	// Get RepeatSet data
	BOOL IsRepeatEnabled(void) const;								// Check if repeat is enabled
	BOOL IsAllowSnoozing(void) const;								// Check if item snooze mode is available
	BOOL IsDayActive(DayOfWeek) const;								// Check if day of week is active
	BYTE GetActiveDays(void) const;									// Get repeat active days

	// Set RepeatSet data
	void EnableRepeat(BOOL);										// Set repeat enable state
	void EnableSnoozing(BOOL);										// Set allow snoozing state
	void SetSnoozeInterval(INT);									// Set snooze interval data
	void SetActiveDays(BYTE);										// Set repeat days data
	void SetDayActive(DayOfWeek, BOOL);								// Set active state for specific day of week

	void Print(String&) const;										// Print item data
};

// Define new typenames for Schedule item data
using PScheduleItem = ScheduleItem*;
using ScheduleItemList = typename std::vector<ScheduleItem>;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		ScheduleData
//  Description:	Store app Action Schedule data settings
//
//////////////////////////////////////////////////////////////////////////

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
	ScheduleItem	 m_schDefaultItem;								// Default schedule item
	ScheduleItemList m_arrSchedExtraItemList;						// List of extra schedule items

public:
	// Constructor
	ScheduleData();													// Default constructor
	ScheduleData(const ScheduleData&);								// Copy constructor

	// Operator
	ScheduleData& operator=(const ScheduleData&);					// Copy assignment operator

public:
	// Data processing
	void Init(void);												// Init data
	void Copy(const ScheduleData&);									// Copy data
	void SetDefaultData(void);										// Set default data

	// Update items
	DWORD Add(const ScheduleItem&);									// Add item
	DWORD Update(const ScheduleItem&);								// Update item

	// Access items
	const ScheduleItem& GetDefaultItem(void) const;					// Get default item (constant)
	ScheduleItem& GetDefaultItem(void);								// Get default item
	const ScheduleItem& GetItemAt(int) const;						// Get item at index (constant)
	ScheduleItem& GetItemAt(int);									// Get item at index

	// Item processing
	void Remove(int);												// Remove item at index
	void RemoveAll(void);											// Remove all item
	void Adjust();													// Adjust data validity
	UINT GetNextID();												// Get next item ID (to add new item)

	// Get attributes
	size_t GetExtraItemNum(void) const;							// Get number of extra items
	BOOL IsDefaultEmpty(void) const;								// Check if default item is empty
	BOOL IsEmpty(int) const;										// Check if item at index is empty
	BOOL IsExtraEmpty(void) const;									// Check if extra data is empty
	BOOL IsAllEmpty(void) const;									// Check if all items are empty

	// Clean-up
	void Delete(int);												// Delete item at index
	void DeleteExtra(void);											// Delete all extra items
	void DeleteAll(void);											// Delete all data
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		HotkeySetItem
//  Description:	Store data of a HotkeySet item
//
//////////////////////////////////////////////////////////////////////////

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
	BOOL	m_bEnabled;												// Hotkey enabled/disabled
	UINT	m_nHKActionID;											// Hotkey action ID
	DWORD	m_dwModifiers;											// Modifier keys
	DWORD	m_dwVirtualKey;											// Virtual key code

public:
	// Constructor
	HotkeySetItem();												// Default constructor
	HotkeySetItem(UINT nHKActionID);								// Overloaded constructor
	HotkeySetItem(const HotkeySetItem&);							// Copy constructor

	// Operator
	HotkeySetItem& operator=(const HotkeySetItem&);					// Copy assignment operator

public:
	// Data processing
	void Copy(const HotkeySetItem&);								// Copy item
	BOOL IsEmpty(void) const;										// Check if item is empty
	BOOL Compare(const HotkeySetItem&) const;						// Compare items
	BOOL CompareKeycode(const HotkeySetItem&) const;				// Compare item keycodes
	void Print(String&) const;										// Print item data
	void PrintKeyStrokes(String&) const;							// Print item keystrokes

public:
	// Get/set attributes
	BOOL IsEnabled(void) const;										// Check if item is enabled
	void EnableItem(BOOL);											// Set item enable state
	UINT GetActionID(void) const;									// Get Hotkey action ID
	void SetActionID(UINT);											// Set Hotkey action ID
	void GetKeyCode(DWORD&, DWORD&) const;							// Get item keycode data
	void SetKeyCode(DWORD, DWORD);									// Set item keycode data
	BOOL CompareKeycode(DWORD, DWORD) const;						// Compare given keycode with item keystroke
};

// Define new typenames for HotkeySet item data
using PHotkeySetItem = HotkeySetItem*;
using HotkeySetItemList = typename std::vector<HotkeySetItem>;

// Define new global typenames for the enum attributes of HotkeySet items
using HotkeyID = HotkeySetItem::HKID;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		HotkeySetData
//  Description:	Store app Hotkeyset data settings
//
//////////////////////////////////////////////////////////////////////////

class HotkeySetData
{
public:
	using Item = HotkeySetItem;
	using HKID = Item::HKID;
	using DataList = HotkeySetItemList;

private:
	// Attributes
	DataList m_arrHotkeySetList;									// Pointer to HotkeySet list

public:
	// Constructor
	HotkeySetData();												// Default constructor
	HotkeySetData(const HotkeySetData&);							// Copy constructor

	// Operator
	HotkeySetData& operator=(const HotkeySetData&);					// Copy assignment operator

public:
	// Data processing
	void Init(void);												// Init data
	void Copy(const HotkeySetData&);								// Copy data
	void SetDefaultData(void);										// Set default data

	// Update items
	void Add(const Item&);											// Add item
	void Update(const Item&);										// Update item

	// Access items
	const Item& GetItemAt(int) const;								// Get item at index (const)
	Item& GetItemAt(int);											// Get item at index

	// Item processing
	void Remove(int);												// Remove item at index
	void RemoveAll(void);											// Remove all item
	void Adjust(void);												// Adjust data validity

	// Get attributes
	size_t GetItemNum(void) const;									// Get number of items
	BOOL IsEmpty(int) const;										// Check if item at index is empty
	BOOL IsAllEmpty() const;										// Check if all items are empty

	// Clean-up
	void Delete(int);												// Delete item at index
	void DeleteAll(void);											// Delete all data
	void PrintKeyStrokes(UINT, String&) const;						// Print item keystrokes by ID
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		RmdMsgStyleSet
//  Description:	Store data of a Reminder message style set
//
//////////////////////////////////////////////////////////////////////////

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

private:
	// Attributes
	COLORREF	m_colorBkgrd;										// Background color
	COLORREF	m_colorText;										// Text color
	String		m_strFontName;										// Font name
	UINT		m_uiFontSize;										// Font size
	UINT		m_uiTimeout;										// Timeout (auto-close) interval
	UINT		m_uiIconID;											// Message icon ID
	INT			m_nIconSize;										// Message icon size
	BYTE		m_byIconPos;										// Message icon position
	BYTE		m_byDisplayPos;										// Message display position
	UINT		m_uiHMargin;										// Display area horizontal margin
	UINT		m_uiVMargin;										// Display area vertical margin

public:
	// Constructor
	RmdMsgStyleSet();												// Default constructor
	RmdMsgStyleSet(const RmdMsgStyleSet&);							// Copy constructor

	// Operator
	RmdMsgStyleSet& operator=(const RmdMsgStyleSet&);				// Copy assignment operator

public:
	// Member functions
	void Copy(const RmdMsgStyleSet&);								// Copy data
	BOOL Compare(const RmdMsgStyleSet&) const;						// Compare data

public:
	// Get/set functions
	COLORREF GetBkgrdColor(void) const;								// Get background color
	void SetBkgrdColor(COLORREF);									// Set background color
	COLORREF GetTextColor(void) const;								// Get text color
	void SetTextColor(COLORREF);									// Set text color
	String GetFontName(void) const;									// Get font name
	void SetFontName(const wchar_t*);								// Set font name
	UINT GetFontSize(void) const;									// Get font size
	void SetFontSize(UINT);											// Set font size
	UINT GetTimeout(void) const;									// Get timeout interval
	void SetTimeout(UINT);											// Set timeout interval
	UINT GetIconID(void) const;										// Get message icon ID
	void SetIconID(UINT);											// Set message icon ID
	INT GetIconSize(void) const;									// Get message icon size
	void SetIconSize(INT);											// Set message icon size
	BYTE GetIconPosition(void) const;								// Get message icon position
	void SetIconPosition(BYTE);										// Set message icon position
	BYTE GetDisplayPosition(void) const;							// Get message display position
	void SetDisplayPosition(BYTE);									// Set message display position
	UINT GetHorizontalMargin(void) const;							// Get horizontal margin
	void SetHorizontalMargin(UINT);									// Set horizontal margin
	UINT GetVerticalMargin(void) const;								// Get vertical margin
	void SetVerticalMargin(UINT);									// Set vertical margin
};

// Define new typenames for Reminder message style data
using PRmdMsgStyleSet = RmdMsgStyleSet*;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		PwrReminderItem
//  Description:	Store data of a Power Reminder item
//
//////////////////////////////////////////////////////////////////////////

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
	UINT			m_nItemID;										// Item ID
	String			m_strMessage;									// Message content
	UINT			m_nEventID;										// Event ID
	ClockTime		m_stTime;										// Event time
	DWORD			m_dwMsgStyle;									// Reminder style
	PwrRepeatSet	m_rpsRepeatSet;									// Repeat set data
	BOOL			m_bUseCustomStyle;								// Use message custom style
	RmdMsgStyleSet	m_rmsMsgStyleSet;								// Reminder message style set

public:
	// Constructor
	PwrReminderItem();												// Default constructor
	PwrReminderItem(const PwrReminderItem&);						// Copy constructor

	// Operator
	PwrReminderItem& operator=(const PwrReminderItem&);				// Copy assignment operator

public:
	// Data processing
	void Copy(const PwrReminderItem&);								// Copy item
	BOOL IsEmpty(void) const;										// Check if item is empty
	BOOL Compare(const PwrReminderItem&) const;						// Compare items

	// Access data
	PwrRepeatSet& GetRepeatSetData(void);							// Get access to item RepeatSet info data
	void ResetRepeatInfo(void);										// Reset RepeatSet info data
	RmdMsgStyleSet& GetMessageStyleData(void);						// Get access to item Message Style info data
	void ResetMessageStyleInfo(void);								// Reset Message Style info data

public:
	// Get/set attributes
	BOOL IsEnabled(void) const;										// Check if item is enabled
	void EnableItem(BOOL);											// Set item enable state
	UINT GetItemID(void) const;										// Get Power Reminder item ID
	void SetItemID(UINT);											// Set Power Reminder item ID
	const wchar_t* GetMessage(void) const;							// Get item message content
	void SetMessage(const wchar_t*);								// Set item message content
	UINT GetEventID(void) const;									// Get Power Reminder item event ID
	void SetEventID(UINT);											// Set Power Reminder item event ID
	ClockTime GetTime(void) const;									// Get Power Reminder item time data
	void SetTime(const ClockTime&);									// Set Power Reminder item time data
	DWORD GetMessageStyle(void) const;								// Get item message style ID
	void SetMessageStyle(DWORD);									// Set item message style ID
	BOOL IsCustomStyleEnabled(void) const;							// Check if item message custom style is enabled
	void EnableCustomStyle(BOOL);									// Set item message custom style enable state

	// Get RepeatSet data
	BOOL IsRepeatEnabled(void) const;								// Check if item repeat mode is enabled
	BOOL IsDayActive(DayOfWeek dayOfWeek) const;					// Check if day of week is active
	BOOL IsAllowSnoozing(void) const;								// Check if item snooze mode is available
	INT  GetSnoozeInterval(void) const;								// Get item snooze interval value
	BYTE GetActiveDays(void) const;									// Get repeat active days

	// Set RepeatSet data
	void EnableRepeat(BOOL);										// Set repeat enable state
	void EnableSnoozing(BOOL);										// Set allow snoozing state
	void SetSnoozeInterval(INT);									// Set snooze interval data
	void SetActiveDays(BYTE);										// Set repeat days data
	void SetDayActive(DayOfWeek, BOOL);								// Set active state for specific day of week

	void Print(String&) const;										// Print item data
};

// Define new typenames for Power Reminder Item data
using PPwrReminderItem = PwrReminderItem*;
using PwrReminderItemList = typename std::vector<PwrReminderItem>;

// Define new global typenames for the enum attributes of Power Reminder items
using PwrReminderEvent = PwrReminderItem::Event;
using PwrReminderStyle = PwrReminderItem::Style;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		PwrReminderData
//  Description:	Store app Power Reminder data settings
//
//////////////////////////////////////////////////////////////////////////

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
	PwrReminderData();												// Default constructor
	PwrReminderData(const PwrReminderData&);						// Copy constructor

	// Operator
	PwrReminderData& operator=(const PwrReminderData&);				// Copy assignment operator

public:
	// Member functions
	void Init(void);												// Init data
	void Copy(const PwrReminderData&);								// Copy data
	void SetDefaultData(void);										// Set default data

	// Update items
	void Add(const PwrReminderItem&);								// Add item
	void Update(const PwrReminderItem&);							// Update item

	// Access items
	const PwrReminderItem& GetItemAt(int) const;					// Get item at index (constant)
	PwrReminderItem& GetItemAt(int);								// Get item at index

	// Item processing
	void Remove(int);												// Remove item at index
	void RemoveAll(void);											// Remove all item
	void Adjust(void);												// Adjust data validity
	UINT GetNextID(void);											// Get next item ID (to add new item)

	// Get attributes
	size_t GetItemNum(void) const;									// Get number of items
	BOOL IsEmpty(int) const;										// Check if item at index is empty
	BOOL IsAllEmpty(void) const;									// Check if all items are empty

	// Clean-up
	void Delete(int);												// Delete item at index
	void DeleteAll(void);											// Delete all data
};

//////////////////////////////////////////////////////////////////////////
//
//  Class name:		PwrRuntimeItem
//  Description:	Store data of a runtime info item
//
//////////////////////////////////////////////////////////////////////////

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
	INT			m_nCategory;										// Item category
	UINT		m_nItemID;											// Power Reminder item ID
	INT			m_nDisplayFlag;										// Item displaying flag
	INT			m_nSkipFlag;										// Item skip flag
	INT			m_nSnoozeFlag;										// Item snooze trigger flag
	ClockTime	m_stNextSnoozeTime;									// Next snooze trigger time

public:
	// Constructor
	PwrRuntimeItem();												// Default constructor
	PwrRuntimeItem(const PwrRuntimeItem&);							// Copy constructor

	// Operator
	PwrRuntimeItem& operator=(const PwrRuntimeItem&);				// Copy assignment operator

	// Data processing
	void Copy(const PwrRuntimeItem&);								// Copy data
	void CalcNextSnoozeTime(int nInterval);							// Calculate next snooze time

public:
	// Get/set attributes
	INT			GetCategory(void) const;							// Get item category
	void		SetCategory(INT);									// Set item category
	UINT		GetItemID(void) const;								// Get item ID
	void		SetItemID(UINT);									// Set item ID
	INT			GetDisplayFlag(void) const;							// Get item displaying flag
	void		SetDisplayFlag(INT);								// Set item displaying flag
	INT			GetSkipFlag(void) const;							// Get item skip flag
	void		SetSkipFlag(INT);									// Set item skip flag
	INT			GetSnoozeFlag(void) const;							// Get item snooze trigger flag
	void		SetSnoozeFlag(INT);									// Set item snooze trigger flag
	ClockTime	GetTime(void) const;								// Get next snooze trigger time
	void		SetTime(const ClockTime&);							// Set next snooze trigger time
};

// Define new typenames for runtime info item data
using PPwrRuntimeItem = PwrRuntimeItem*;
using PwrRuntimeQueue = typename std::vector<PwrRuntimeItem>;

// Define new global typenames for the enum attributes of runtime info items
using PwrFeatureID = PwrRuntimeItem::FeatureCategoryID;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		HistoryInfoData
//  Description:	Manage app action history info data
//
//////////////////////////////////////////////////////////////////////////

class HistoryInfoData
{
private:
	// Attributes
	BOOL		m_bInitState;										// Init state flag
	UINT		m_nCategoryID;										// Category ID
	DateTime	m_stTimestamp;										// Timestamp of history
	UINT		m_nItemID;											// Item ID
	UINT		m_nActionID;										// Action ID
	BOOL		m_bActionResult;									// Action result
	DWORD		m_dwErrorCode;										// Returned error code
	String		m_strDescription;									// History description (attached info)

public:
	// Constructor
	HistoryInfoData();												// Default constructor
	HistoryInfoData(const HistoryInfoData&);						// Copy constructor

	// Operator
	HistoryInfoData& operator=(const HistoryInfoData&);				// Copy assignment operator

	// Member functions
	void Copy(const HistoryInfoData&);								// Copy data
	void Init(UINT nCategoryID);									// Initialization
	void RemoveAll(void);											// Remove all data

public:
	// Get/set properties
	BOOL IsInit(void) const;										// Check if data is initialized
	UINT GetCategoryID(void) const;									// Get category ID
	void SetCategoryID(UINT);										// Set category ID
	DateTime GetTime(void) const;									// Get timestamp
	void SetTime(const DateTime&);									// Set timestamp
	UINT GetItemID(void) const;										// Get item ID
	void SetItemID(UINT);											// Set item ID
	UINT GetActionID(void) const;									// Get action ID
	void SetActionID(UINT);											// Set action ID
	BOOL IsSuccess(void) const;										// Check if action is successful
	void SetResult(BOOL);											// Set action result
	DWORD GetErrorCode(void) const;									// Get error code
	void SetErrorCode(DWORD);										// Set error code
	void GetDescription(String&) const;							// Get description
	const wchar_t* GetDescription(void) const;						// Get description
	void SetDescription(const wchar_t*);							// Set description
};

// Define new typenames for History info data
using PHistoryInfoData = HistoryInfoData*;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		SystemEvent
//  Description:	Store data of System Event info
//
//////////////////////////////////////////////////////////////////////////

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
	SystemEvent(EventID);											// Constructor (with event ID)
	SystemEvent(const SystemEvent&);								// Copy constructor

	// Operators
	SystemEvent& operator=(const SystemEvent&);						// Copy assignment operator

public:
	// Get/set functions
	EventID GetEventID(void) const;									// Get system event ID
	DateTime GetTimestamp(void) const;								// Get system event timestamp
	void SetTimestamp(DateTime);									// Set system event timestamp
};

// Define new typenames for Power System Event info data
using SystemEventData = typename std::vector<SystemEvent>;

// Define new global typenames for the enum attributes of System Event info
using SystemEventID = SystemEvent::EventID;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		SystemEventTracker
//  Description:	Store and manage System Event tracking data
//
//////////////////////////////////////////////////////////////////////////

class SystemEventTracker
{
private:
	// Attributes
	SystemEventData m_arrTrackingData;								// System event tracking data

public:
	// Construction
	SystemEventTracker();											// Constructor (with event ID)
	SystemEventTracker(const SystemEventTracker&);					// Copy constructor

	// Operators
	SystemEventTracker& operator=(const SystemEventTracker&);		// Copy assignment operator

public:
	// Validation
	BOOL IsEmpty(void) const;										// Check if tracking data is empty
	size_t GetTrackedCount(void) const;								// Get number of tracked events

	// Update data
	void AddEvent(const SystemEvent&);								// Add system event info
	void RemoveAll(SystemEventID);									// Remove all tracking data of specific event ID
	void RemoveAll(void);											// Remove all event tracking data

	// Access items
	const SystemEvent& GetAt(int) const;							// Get system event info data by index
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		RegistryValue
//  Description:	Store data values of a registry key 
//
//////////////////////////////////////////////////////////////////////////

class RegistryValue
{
public:
	enum Type {
		NONE = 0,											// Undefined/invalid type/no data
		STRING,												// String value
		DWORD_32,											// DWORD (32-bit) value
		QWORD_64,											// QWORD (64-bit) value
		MULTI_STRING,										// Multi-string value
	};

private:
	// Data values
	String*			m_pstrValue;							// String value
	DWORD*			m_pdwValue;								// DWORD (32-bit) value
	QWORD*			m_pqwValue;								// QWORD (64-bit) value
	StringArray*	m_pastrValue;							// Multi-string value

public:
	// Construction
	RegistryValue();										// Default constructor
	RegistryValue(const RegistryValue&);					// Copy constructor	
	~RegistryValue();										// Default destructor

	// Operator
	RegistryValue& operator=(const RegistryValue&);			// Copy assignment operator

	// Member functions
	BOOL Init(UINT);										// Initialize with type
	void Copy(const RegistryValue&);						// Copy data
	void Reset(void);										// Remove data
	void Refactor(void);									// Refactor data and optimize
	BOOL IsEmpty(void) const;								// Check if data is empty
	BOOL IsType(UINT) const;								// Check if current type is matching

public:
	// Get/set values
	UINT GetType(void) const;								// Get value type
	void GetString(String&) const;							// Get String value
	const wchar_t* GetString(void) const;					// Get String value
	void SetString(const wchar_t*);							// Set String value
	DWORD GetDWord(void) const;								// Get DWORD (32-bit) value
	void SetDWord(DWORD);									// Set DWORD (32-bit) value
	QWORD GetQWord(void) const;								// Get QWORD (64-bit) value
	void SetQWord(QWORD);									// Set QWORD (64-bit) value
	void GetMultiString(StringArray&) const;				// Get Multi-string value
	void SetMultiString(StringArray&);						// Set Multi-string value
	BOOL AddString(const wchar_t*);							// Add string value to Multi-string data
};

// Define new typenames for Registry value data
using REGISTRYVALUE = RegistryValue;
using PREGISTRYVALUE = RegistryValue*;

// Define new global typenames for the enum attributes of RegistryValue
using RegValueType = RegistryValue::Type;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		RegistryKey
//  Description:	Store data of a registry key info
//
//////////////////////////////////////////////////////////////////////////

class RegistryKey
{
private:
	// Member variables
	String			m_strKeyName;							// Key name (string)
	REGISTRYVALUE	m_regValue;								// Registry value data

public:
	// Constructor
	RegistryKey();											// Default constructor
	RegistryKey(const RegistryKey&);						// Copy constructor

	// Operator
	RegistryKey& operator=(const RegistryKey&);				// Copy assignment operator

	// Member functions
	void Copy(const RegistryKey&);							// Copy data
	void Clear(void);										// Remove all data
	BOOL IsEmpty(void) const;								// Check if registry key is empty

public:
	// Get/set properties
	void GetName(String&) const;							// Get key name
	const wchar_t* GetName(void) const;						// Get key name
	void SetName(const wchar_t*);							// Set key name
	UINT GetType(void) const;								// Get value type
	void SetType(UINT);										// Set value type

	// Get/set value
	void GetString(String&) const;							// Get String value
	const wchar_t* GetString(void) const;					// Get String value
	void SetString(const wchar_t*);							// Set String value
	DWORD GetDWord(void) const;								// Get DWORD (32-bit) value
	void SetDWord(DWORD);									// Set DWORD (32-bit) value
	QWORD GetQWord(void) const;								// Get QWORD (64-bit) value
	void SetQWord(QWORD);									// Set QWORD (64-bit) value
	void GetMultiString(StringArray&) const;				// Get Multi-string value
	void SetMultiString(StringArray&);						// Set Multi-string value
	BOOL AddString(const wchar_t*);							// Add string value to Multi-string data
};

// Define new typenames for Registry key info data
using PRegistryKey = RegistryKey*;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		RegistryInfo
//  Description:	Store data of a registry key info
//
//////////////////////////////////////////////////////////////////////////

class RegistryInfo
{
public:
	enum RegistryPathType {
		fullPath = 0,										// Full path
		rootKeyOnly,										// Root key only
		includingSubKeyPath,								// Including sub-key path
		includingCompanyName,								// Including company name
		includingProductName,								// Including product name
		includingSectionName,								// Including section name
		includingKeyName,									// Including key name
	};

private:
	// Attributes
	HKEY			m_hRootKey;								// Root key (HKEY)
	String			m_strRootKey;							// Root key (string)
	StringArray		m_astrSubkeyPath;						// Subkey path (string array)
	String			m_strCompanyName;						// Company name (string)
	String			m_strProductName;						// Product name (string)
	StringArray		m_astrSectionArray;						// Section array (string)
	RegistryKey		m_regKeyInfo;							// Registry key info

public:
	// Constructor
	RegistryInfo();											// Default constructor
	RegistryInfo(const RegistryInfo&);						// Copy constructor

	// Operator
	RegistryInfo& operator=(const RegistryInfo&);			// Copy assignment operator

	// Member functions
	void Copy(const RegistryInfo&);							// Copy data
	void RemoveAll(void);									// Remove all data

public:
	// Get data
	HKEY GetRootKey(void) const;							// Get root key
	const RegistryKey& GetRegistryKey(void) const;			// Get registry key info data

	// Get attributes
	const wchar_t* GetRootKeyName(void) const;				// Get root key name
	void GetSubkeyPath(StringArray&) const;					// Get subkey path array
	const wchar_t* GetCompanyName(void) const;				// Get company name
	const wchar_t* GetProductName(void) const;				// Get product name
	void GetSectionName(StringArray&) const;				// Get section name array

	// Set attributes
	void SetRootKey(HKEY);									// Set root key
	void SetRootKeyName(const wchar_t*);					// Set root key name
	void SetSubkeyPath(const wchar_t*);						// Set subkey path
	void SetSubkeyPath(StringArray&);						// Set subkey path (string array)
	void SetCompanyName(const wchar_t*);					// Set company name
	void SetProductName(const wchar_t*);					// Set product name
	void SetSectionName(const wchar_t*);					// Set section name
	void SetSectionName(StringArray&);						// Set section array (string array)
};

// Define new typenames for Registry info class
using PRegistryInfo = RegistryInfo*;

// Define new global typenames for the enum attributes of RegistryValue
using RegistryPathType = RegistryInfo::RegistryPathType;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	GRIDCTRLCOLFORMAT
//  Description:	Store grid control/view column format info
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagGRIDCTRLCOLFMT 
{
	// Member variables
	INT		nColID;											// Column ID
	UINT	nHeaderTitleID;									// Header title string ID
	INT		nWidth;											// Column width
	UINT	nColStyle;										// Column style
	BOOL	bCenter;										// Align center
} GRIDCTRLCOLFORMAT, *PGRIDCTRLCOLFORMAT;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	RESTARTREQ
//  Description:	Using for request to restart app as admin
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagRESTARTREQ 
{
	// Member variables
	BOOL bRequest;											// Request to restart
	BOOL bAdminCheck;										// Check if already running as admin
	BOOL bNotAdminShowMsg;									// If not admin, not show check message
	BOOL bIsAdminDoNothing;									// If already running as admin, do nothing
	BOOL bShowMsgWhenDeny;									// Show message when denied
	BOOL bResetFlag;										// Reset flag when denied
} RESTARTREQ, *PRESTARTREQ;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		Substring
//  Description:	Using for getting substrings
//
//////////////////////////////////////////////////////////////////////////

class Substring
{
private:
	// Member variables
	String m_strLeft;										// Left part
	String m_strMid;										// Middle part
	String m_strRight;										// Right part

public:
	// Constructor
	Substring();											// Default constructor
	Substring(const Substring&);							// Copy constructor

	// Operator
	Substring& operator=(const Substring&);					// Copy assignment operator

	// Data processing
	void Copy(const Substring&);							// Copy data
	void RemoveAll(void);									// Remove all data
	BOOL IsEmpty(void) const;								// Check if data is empty
	void TrimLeft(void);									// Trim spaces for left part
	void TrimMid(void);										// Trim spaces for middle part
	void TrimRight(void);									// Trim spaces for right part
	void TrimAll(void);										// Trim spaces for all parts

	// Get substrings
	const wchar_t* Left(void) const;						// Get left part
	const wchar_t* Mid(void) const;							// Get middle part
	const wchar_t* Right(void) const;						// Get right part

	// Set substrings
	void SetLeft(const wchar_t*);							// Set left part
	void SetMid(const wchar_t*);							// Set middle part
	void SetRight(const wchar_t*);							// Set right part
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		StringUtils
//  Description:	Using for string processing and validation
//
//////////////////////////////////////////////////////////////////////////

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
	static String StringFormat(UINT formatTemplateID, ...);
	static String StringFormat(const wchar_t* formatTemplate, ...);

	// Load resource string/text data
	static String LoadResourceString(UINT resourceStringID);
	static bool	LoadResourceString(String& resultStr, UINT resourceStringID);
	static String LoadResourceTextData(UINT resourceFileID);

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

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		ClockTimeUtils
//  Description:	Using for clock-time processing and validation
//
//////////////////////////////////////////////////////////////////////////

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
	static String Format(LANGTABLE_PTR pLang, UINT nFormatID, const ClockTime& clockTime);
	static String Format(LANGTABLE_PTR pLang, const wchar_t* formatString, const ClockTime& clockTime);
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		DateTimeUtils
//  Description:	Using for date/time processing and validation
//
//////////////////////////////////////////////////////////////////////////

class DateTimeUtils
{
public:
	// Get current date/time from system
	static DateTime GetCurrentDateTime(void);

	// Conversion
	static DateTime FromSystemTime(SYSTEMTIME sysTime);
	static SYSTEMTIME ToSystemTime(const DateTime& dateTime);

	// Format for displaying/printing
	static String Format(LANGTABLE_PTR pLang, UINT nFormatID, const DateTime& dateTime);
	static String Format(LANGTABLE_PTR pLang, const wchar_t* formatString, const DateTime& dateTime);
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		PerformanceCounter
//  Description:	Using for querrying performance counter of functions
//
//////////////////////////////////////////////////////////////////////////

class PerformanceCounter
{
private:
	// Attributes
	LARGE_INTEGER m_liStartTime;							// Start time
	LARGE_INTEGER m_liEndTime;								// End time
	LARGE_INTEGER m_liFrequency;							// Performance frequency

	// Counting flag
	BOOL m_bIsRunning;										// Counter is running

public:
	// Constructor
	PerformanceCounter();									// Default constructor
	~PerformanceCounter();									// Destructor

	// Member functions
	void Start(void);										// Start performance counter
	void Stop(void);										// Stop performance counter
	double GetElapsedTime(BOOL) const;						// Get function execution elapsed time
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define core methods for program, these functions will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace AppCore
{
	// Power action execution functions (main core)
	BOOL ExecutePowerAction(UINT nActionType, UINT nMessage, DWORD& dwErrCode);
	BOOL ExecutePowerActionDummy(UINT nActionType, UINT nMessage, DWORD& dwErrCode);

	// Trace logging functions
	void TraceError(const char* traceLogA);
	void TraceError(const wchar_t* traceLogW);
	void TraceErrorFormat(const char* traceLogFormatA, ...);
	void TraceErrorFormat(const wchar_t* traceLogFormatW, ...);
	void TraceDebugInfo(const char* funcName, const char* fileName, int lineIndex);

	// Debug logging functions
	void OutputDebugLog(const wchar_t* debugLog, int forceStyle = -1);
	void OutputDebugLogFormat(const wchar_t* debugLogFormat, ...);
	void OutputDebugStringFormat(const wchar_t* debugStringFormat, ...);

	// Trace/debug file logging functions
	BOOL BackupOldLogFile(String& filePath, const wchar_t* logFileName);
	void WriteTraceErrorLogFile(const wchar_t* logStringW);
	void WriteTraceDebugLogFile(const wchar_t* logStringW);
	void WriteDebugInfoLogFile(const wchar_t* logStringW);
	void WriteTraceNDebugLogFileBase(const wchar_t* fileName, const wchar_t* logStringW);

	// Message and notification functions
	LRESULT	WaitMessage(UINT nMsg, int nTimeout = Constant::Max::Timeout::WaitMessage);
	void	ShowErrorMessage(HWND hMsgOwnerWnd, UINT nLanguageID, DWORD dwErrorCode, LPARAM lParam = NULL);

	// Data converting functions
	UINT Sel2Opt(UINT nOptionMacro, UINT nSelection);
	UINT Opt2Sel(UINT nOptionMacro, UINT nCurOption);

	// Data/control/window processing functions
	int	 GetListCurSel(CListCtrl& pListCtrl);
	HWND FindDebugTestDlg(void);
	void SetFixedCellStyle(CGridCtrl* pGridCtrl, int nRow, int nCol);
	void DrawGridTableRow(CGridCtrl* pGridCtrl, int nRow, int nRowNum, int nColNum, GRIDCTRLCOLFORMAT* apGrdColFormat);

	// Additional functions
	UINT	GetWindowsOSVersion(void);

	BOOL	AddRegistryKey(const RegistryInfo& regInfo);
	const wchar_t* MakeRegistryPath(const RegistryInfo& regInfo, UINT nRegPathType = RegistryPathType::fullPath, BOOL bIncRootKey = TRUE);

	void	PlaySound(BOOL bSoundEnable, UINT nTypeOfSound);
	BOOL	FileViewStd(FILETYPE eFileType, const wchar_t* filePath);
	BOOL	OpenWebURL(const wchar_t* webUrl);

	LRESULT RunApp(const wchar_t* appPath, BOOL bRunAsAdmin = FALSE, BOOL bShowFlag = TRUE);
	LRESULT ExecuteCommand(const wchar_t* commandString, BOOL bRunAsAdmin = TRUE, BOOL bShowFlag = TRUE);
	BOOL	CreateAppProcess(const wchar_t* appPath, wchar_t* commandLine, UINT nStyle, DWORD& dwErrorCode);

	BOOL	SetDarkMode(CWnd* pWnd, BOOL bEnableDarkMode);
	void	DrawButton(CButton*& pButton, UINT nIconID, const wchar_t* buttonTitle = Constant::String::Empty);
	bool	EnumFontNames(std::vector<std::wstring>& fontNames);
	bool	ValidateFontName(const wchar_t* fontName);
};


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _APPCORE_ENABLE_INLINES
	#include "AppCore.inl"
	#ifdef _APPCORE_INLINE_INCLUDED
		#pragma message("-- AppCore inline library included")
	#else
		#pragma error("-- Linking error in AppCore.h: Unable to link to inline header!")
	#endif
	#undef _APPCORE_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in AppCore.h: Inline is not enabled!")
#endif
