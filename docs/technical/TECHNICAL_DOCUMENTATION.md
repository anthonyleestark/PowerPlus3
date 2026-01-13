# PowerPlus3 Technical Documentation

## Table of Contents

1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Core Components](#core-components)
4. [Data Structures](#data-structures)
5. [Features and Functionality](#features-and-functionality)
6. [System Integration](#system-integration)
7. [Data Persistence](#data-persistence)
8. [Logging System](#logging-system)
9. [User Interface Framework](#user-interface-framework)
10. [Code Patterns and Conventions](#code-patterns-and-conventions)
11. [Migration Considerations](#migration-considerations)

---

## Overview

PowerPlus3 is a Windows desktop application built with C++ and MFC (Microsoft Foundation Classes) that provides power management functionality for Windows systems. The application allows users to:

- Execute power actions (shutdown, restart, sleep, hibernate, display off, sign out)
- Schedule power actions at specific times
- Configure hotkeys for quick power actions
- Set up power reminders with customizable messages
- Manage application settings and preferences

### Key Technologies

- **Language**: C++ (C++11/14 standards)
- **Framework**: MFC (Microsoft Foundation Classes)
- **UI Framework**: Windows API, MFC Dialog-based UI
- **Data Storage**: Windows Registry
- **Build System**: Visual Studio (MSBuild)
- **Target Platform**: Windows 7/8/10/11 (x64)

---

## Architecture

### Current Architecture (Windows-Only)

#### **Application Hierarchy**

```markdown
CPowerPlusApp (SWinApp)
    └── CPowerPlusDlg (SDialog)
        ├── CAboutDlg
        ├── CHelpDlg
        ├── CLogViewerDlg
        ├── CMultiScheduleDlg
        │   └── CEditScheduleDlg
        ├── CHotkeySetDlg
        └── CPwrReminderDlg
            ├── CRmdRepeatSetDlg
            ├── CRmdMsgStyleSetDlg
            └── CReminderMsgDlg
```

#### **Class Inheritance Structure**

1. **SWinApp** (extends `CWinAppEx`)
   - Base application class providing:
     - Language management
     - Event logging
     - Flag management
     - Application-wide settings
   - **Platform Dependency**: MFC (`CWinAppEx`)

2. **SDialog** (extends `CDialogEx`)
   - Base dialog class providing:
     - Dialog management system
     - Control management
     - Layout management
     - Tooltip support
     - Language support
     - Event logging
   - **Platform Dependency**: MFC (`CDialogEx`)

3. **CPowerPlusApp** (extends `SWinApp`)
   - Main application class
   - Manages application lifecycle
   - Handles data serialization
   - Manages keyboard hooks
   - Provides application-wide data access
   - **Platform Dependency**: Windows Registry, Windows API

4. **CPowerPlusDlg** (extends `SDialog`)
   - Main window dialog
   - Coordinates all features
   - Manages system events
   - Handles timers for scheduled actions
   - **Platform Dependency**: Windows messages, Windows timers

#### **Application Lifecycle**

1. **Initialization** (`CPowerPlusApp::InitInstance()`)
   - Check for existing instance (single instance enforcement)
   - Initialize common controls
   - Setup registry key
   - Create necessary folders
   - Setup keyboard hook
   - Load application data from registry
   - Initialize logging systems
   - Create main dialog

2. **Runtime**
   - Main dialog handles user interactions
   - Timers process scheduled actions
   - System event handlers process power/session events
   - Hotkey handlers process keyboard shortcuts
   - Background tasks execute scheduled actions

3. **Shutdown** (`CPowerPlusApp::ExitInstance()`)
   - Save application data
   - Unregister hotkeys
   - Remove keyboard hook
   - Clean up resources
   - Close log files

### Recommended Cross-Platform Architecture

#### **Layered Architecture Design**

For cross-platform migration, the application should be refactored into a layered architecture with clear separation between platform-independent and platform-specific code:

```markdown
┌────────────────────────────────────────────────────────────┐
│                    Presentation Layer                      │
│             (Platform-Specific UI Framework)               │
│    ┌──────────────┐  ┌──────────────┐  ┌──────────────┐    │
│    │   Windows    │  │    Linux     │  │    macOS     │    │
│    │  (Qt/WinUI)  │  │    (Qt)      │  │    (Qt)      │    │
│    └──────────────┘  └──────────────┘  └──────────────┘    │
└────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────────┐
│                    Application Layer                       │
│           (Use Cases, Services, Orchestration)             │
│                  - PowerActionService                      │
│                   - ScheduleService                        │
│                    - HotkeyService                         │
│                   - ReminderService                        │
│                 - ConfigurationService                     │
│                 (Platform-Independent)                     │
└────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────────┐
│                      Domain Layer                          │
│         (Business Logic, Entities, Value Objects)          │
│               - PowerAction (enum/class)                   │
│                    - ScheduleItem                          │
│                     - HotkeyItem                           │
│                    - ReminderItem                          │
│                    - ConfigData                            │
│                 (Platform-Independent)                     │
└────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────────┐
│                  Infrastructure Layer                      │
│            (Persistence, System Integration)               │
│  ┌──────────────────────────────────────────────────────┐  │
│  │           Platform Abstraction Interfaces            │  │
│  │                 - IPowerManager                      │  │
│  │              - IConfigurationStorage                 │  │
│  │                 - IHotkeyManager                     │  │
│  │                  - ISystemTray                       │  │
│  │                  - IFileSystem                       │  │
│  │                 - ITimerManager                      │  │
│  └──────────────────────────────────────────────────────┘  │
│                            │                               │
│        ┌───────────────────┼───────────────────┐           │
│        ▼                   ▼                   ▼           │
│  ┌─────────────┐    ┌──────────────┐    ┌───────────────┐  │
│  │   Windows   │    │    Linux     │    │     macOS     │  │
│  │  Platform   │    │   Platform   │    │   Platform    │  │
│  │  Implement  │    │  Implement   │    │   Implement   │  │
│  └─────────────┘    └──────────────┘    └───────────────┘  │
└────────────────────────────────────────────────────────────┘
```

#### **Platform Abstraction Layer (PAL)**

The Platform Abstraction Layer provides interfaces for all platform-specific functionality:

##### **1. Power Management Interface**

```cpp
// Platform-independent interface
class IPowerManager {
public:
    virtual ~IPowerManager() = default;
    
    enum class Action {
        DisplayOff,
        Sleep,
        Hibernate,
        Shutdown,
        Restart,
        SignOut
    };
    
    virtual Result<void, ErrorCode> executeAction(Action action) = 0;
    virtual bool isActionSupported(Action action) const = 0;
    virtual bool requiresPrivileges(Action action) const = 0;
};

// Windows implementation
class WindowsPowerManager : public IPowerManager {
public:
    Result<void, ErrorCode> executeAction(Action action) override;
    // ... Windows-specific implementation
};

// Linux implementation
class LinuxPowerManager : public IPowerManager {
public:
    Result<void, ErrorCode> executeAction(Action action) override;
    // ... Linux-specific implementation (systemd, logind)
};
```

##### **2. Configuration Storage Interface**

```cpp
// Platform-independent interface
class IConfigurationStorage {
public:
    virtual ~IConfigurationStorage() = default;
    
    virtual Result<ConfigData, ErrorCode> loadConfig() = 0;
    virtual Result<void, ErrorCode> saveConfig(const ConfigData& config) = 0;
    virtual Result<ScheduleData, ErrorCode> loadSchedule() = 0;
    virtual Result<void, ErrorCode> saveSchedule(const ScheduleData& schedule) = 0;
    // ... other data types
};

// Windows implementation (Registry)
class RegistryStorage : public IConfigurationStorage {
    // Uses Windows Registry
};

// Cross-platform implementation (JSON files)
class JsonFileStorage : public IConfigurationStorage {
    // Uses JSON files in platform-specific locations
    // Windows: %APPDATA%\PowerPlus3\
    // Linux: ~/.config/PowerPlus3/
    // macOS: ~/Library/Application Support/PowerPlus3/
};
```

##### **3. Hotkey Management Interface**

```cpp
// Platform-independent interface
class IHotkeyManager {
public:
    virtual ~IHotkeyManager() = default;
    
    struct KeyCombination {
        std::vector<ModifierKey> modifiers;
        KeyCode key;
    };
    
    virtual Result<void, ErrorCode> registerHotkey(
        unsigned id, 
        const KeyCombination& combo
    ) = 0;
    virtual Result<void, ErrorCode> unregisterHotkey(unsigned id) = 0;
    virtual bool isHotkeySupported() const = 0;
};

// Windows implementation
class WindowsHotkeyManager : public IHotkeyManager {
    // Uses RegisterHotKey() and keyboard hooks
};

// Linux implementation (X11/Wayland)
class LinuxHotkeyManager : public IHotkeyManager {
    // Uses X11 XGrabKey or Wayland protocols
};

// macOS implementation
class MacOSHotkeyManager : public IHotkeyManager {
    // Uses Carbon/Cocoa hotkey APIs
};
```

##### **4. System Tray Interface**

```cpp
// Platform-independent interface
class ISystemTray {
public:
    virtual ~ISystemTray() = default;
    
    virtual Result<void, ErrorCode> createIcon(
        const std::string& iconPath,
        const std::string& tooltip
    ) = 0;
    virtual Result<void, ErrorCode> showNotification(
        const std::string& title,
        const std::string& message
    ) = 0;
    virtual void setMenu(std::unique_ptr<IMenu> menu) = 0;
};

// Platform-specific implementations
class WindowsSystemTray : public ISystemTray { /* ... */ };
class LinuxSystemTray : public ISystemTray { /* ... */ };
class MacOSSystemTray : public ISystemTray { /* ... */ };
```

#### **Refactoring Strategy**

##### **Phase 1: Extract Platform-Independent Core**

1. **Identify Platform-Independent Code**
   - Data structures (ScheduleItem, HotkeyItem, etc.)
   - Business logic (scheduling algorithms, time calculations)
   - Domain models and value objects
   - Utility functions (string manipulation, time formatting)

2. **Create Platform Abstraction Interfaces**
   - Define interfaces for all platform-specific operations
   - Create abstract base classes
   - Document expected behavior

3. **Move Core Logic to Domain Layer**
   - Extract business logic from UI classes
   - Create service classes for feature orchestration
   - Remove direct Windows API calls from business logic

##### **Phase 2: Implement Platform Abstraction**

1. **Create Windows Implementation**
   - Implement all PAL interfaces using Windows APIs
   - Wrap existing Windows-specific code
   - Maintain backward compatibility

2. **Create Factory Pattern**
   - Platform detection at runtime
   - Factory creates appropriate implementations
   - Dependency injection for services

##### **Phase 3: UI Framework Abstraction**

1. **Abstract UI Components**
   - Create base classes for dialogs/views
   - Define common UI operations
   - Platform-specific implementations

2. **Choose Cross-Platform UI Framework**
   - **Qt**: Recommended for C++ cross-platform
   - **WxWidgets**: Alternative option
   - **Native**: Platform-specific UI (WinUI, GTK, Cocoa)

##### **Phase 4: Data Storage Migration**

1. **Abstract Storage Layer**
   - Create storage interface
   - Support multiple backends (Registry, JSON, SQLite)
   - Migration utilities for existing data

2. **Implement Cross-Platform Storage**
   - JSON file storage for cross-platform
   - Platform-specific locations
   - Data format migration tools

#### **Project Structure for Cross-Platform**

```markdown
PowerPlus3/
├── src/
│   ├── domain/              # Platform-independent domain logic
│   │   ├── entities/
│   │   ├── value_objects/
│   │   └── services/
│   ├── application/          # Application layer (use cases)
│   │   ├── services/
│   │   └── use_cases/
│   ├── infrastructure/      # Infrastructure implementations
│   │   ├── storage/
│   │   ├── logging/
│   │   └── platform/
│   │       ├── windows/
│   │       ├── linux/
│   │       └── macos/
│   └── presentation/        # UI layer
│       ├── windows/         # Windows UI (Qt/WinUI)
│       ├── linux/          # Linux UI (Qt/GTK)
│       └── macos/          # macOS UI (Qt/Cocoa)
├── include/
│   └── [same structure]
├── tests/
│   ├── unit/
│   ├── integration/
│   └── platform/
├── cmake/                   # CMake build files
└── CMakeLists.txt
```

#### **Technology Stack Recommendations**

##### **For Cross-Platform C++:**

1. **UI Framework: Qt 6**
   - Mature cross-platform framework
   - Excellent C++ support
   - Rich widget set
   - System integration APIs
   - Commercial and open-source licenses

2. **Build System: CMake**
   - Industry standard
   - Cross-platform support
   - Good IDE integration
   - Package management support

3. **Package Management: vcpkg or Conan**
   - Dependency management
   - Cross-platform package support
   - Version control

4. **Testing: Google Test / Catch2**
   - Cross-platform testing frameworks
   - Good mocking support
   - CI/CD integration

5. **Serialization: JSON (nlohmann/json)**
   - Human-readable format
   - Cross-platform compatible
   - Good C++ library support

6. **Logging: spdlog**
   - Fast, header-only
   - Cross-platform
   - Multiple sinks support

---

## Core Components

### 1. Application Core (`AppCore`)

#### 1.1 Data Classes

##### **ConfigData**

- Stores application configuration settings
- Includes: mouse button actions, language, startup options, logging flags
- Managed via `AppOptionID` enum

##### **ScheduleData**

- Manages scheduled power actions
- Contains default schedule item + extra items (up to 100)
- Each item has: ID, enabled state, action ID, time, repeat settings

##### **HotkeySetData**

- Manages hotkey configurations
- Stores hotkey items with: action ID, modifiers, virtual key code
- Supports background hotkeys (works when screen is locked)

##### **PwrReminderData**

- Manages power reminder configurations
- Contains reminder items with: message, event trigger, time, style settings
- Includes common style settings for all reminders

##### **HistoryInfoData**

- Stores action execution history
- Tracks: category, timestamp, item ID, action ID, result, error code

#### 1.2 Utility Classes

##### **StringUtils**

- String formatting and manipulation
- Resource string loading
- Path management
- Product version retrieval

##### **ClockTimeUtils**

- Clock time (HH:MM) operations
- Time conversion (system time ↔ clock time)
- Time matching and comparison
- Time offset calculations

##### **DateTimeUtils**

- Date/time operations
- System time conversion
- Date/time formatting

##### **PerformanceCounter**

- Performance measurement
- Elapsed time calculation

#### 1.3 System Event Tracking

##### **SystemEvent**

- Tracks system events (suspend, wake, session lock/unlock, session end)
- Stores event ID and timestamp

##### **SystemEventTracker**

- Manages collection of system events
- Provides event history tracking

### 2. Framework Components

#### 2.1 SWinApp (Base Application Class)

##### **Responsibilities:**

- Application-wide language management
- Application event logging
- Flag management (application-wide flags)
- Message box handling
- Application name and caption management

##### **Key Features:**

- Language table management (`LANGTABLE_PTR`)
- Event log initialization and output
- Flag manager integration
- Read-only mode support
- Force close handling

#### 2.2 SDialog (Base Dialog Class)

##### **Responsibilities:**

- Dialog lifecycle management
- Control management
- Layout management
- Tooltip support
- Language support
- Event logging

##### **Key Features:**

- Dialog registration system (tracks all open dialogs)
- Control manager (`SControlManager`) for UI controls
- Anchor point and alignment system
- Size management (min/max/default)
- Margin management
- Lock state management (disable controls)
- Read-only mode support

##### **Dialog Management System:**

- Each dialog registers itself on creation
- Parent-child relationship tracking
- Automatic cleanup on destruction
- Message routing between dialogs

### 3. Main Application Components

#### 3.1 CPowerPlusApp

##### **Data Management:**

- `ConfigData* appConfigDataPtr_` - Application configuration
- `ScheduleData* scheduleDataPtr_` - Schedule data
- `HotkeySetData* hotkeySetDataPtr_` - Hotkey data
- `PwrReminderData* reminderDataPtr_` - Reminder data
- `Logger* appHistoryLogPtr_` - History logger

##### **Key Functions:**

- `initAppData()` - Initialize all data structures
- `loadRegistryAppData()` - Load from registry
- `saveRegistryAppData()` - Save to registry
- `initAppHistoryLog()` - Initialize history logging
- `enableAutoStart()` - Manage Windows startup registration

##### **System Integration:**

- Keyboard hook (`SetWindowsHookEx(WH_KEYBOARD_LL)`)
- Device notification callback
- Message filter processing

#### 3.2 CPowerPlusDlg

##### **Data Management:**

- Local copies of all application data
- Runtime queue for scheduled actions
- Registered hotkey list
- Notify icon data

##### **Key Features:**

- Timer-based action scheduling
- Hotkey processing
- System event handling
- Power reminder execution
- Notify icon management

---

## Data Structures

### 1. ScheduleItem

```cpp
class ScheduleItem {
    unsigned itemId_;              // Unique item ID (10000-19999)
    BOOL isEnabled_;               // Enable/disable state
    unsigned actionId_;            // Power action ID
    ClockTime timeValue_;          // Schedule time (HH:MM)
    PwrRepeatSet repeatSetInfo_;   // Repeat configuration
};
```

#### **Repeat Configuration (PwrRepeatSet):**

- `isRepeated_` - Daily repeat flag
- `isSnoozingAllowed_` - Allow snoozing
- `snoozeInterval_` - Snooze interval (60-1800 seconds)
- `repeatDays_` - Bitmask for active days (7 bits, one per day)

### 2. HotkeySetItem

```cpp
class HotkeySetItem {
    bool isEnabled_;            // Enable/disable state
    unsigned hotkeyActionId_;   // Hotkey action ID (HKID enum)
    DWORD modifiers_;           // Modifier keys (MOD_ALT, MOD_CONTROL, MOD_WIN)
    DWORD virtualKey_;          // Virtual key code
};
```

#### **Hotkey IDs:**

- `displayOff` (0x1a01)
- `sleep` (0x1a02)
- `shutdown` (0x1a03)
- `restart` (0x1a04)
- `signOut` (0x1a05)
- `hibernate` (0x1a06)

### 3. PwrReminderItem

```cpp
class PwrReminderItem {
    BOOL isEnabled_;                   // Enable/disable state
    unsigned itemId_;                  // Unique item ID (10000-19999)
    String messageContent_;            // Reminder message text
    unsigned eventId_;                 // Event trigger ID
    ClockTime timeValue_;              // Event time (for "at set time" event)
    DWORD messageStyle_;               // Style (MessageBox or DialogBox)
    PwrRepeatSet repeatSetInfo_;       // Repeat configuration
    BOOL useCustomStyle_;              // Use custom style flag
    RmdMsgStyleSet msgStyleSetInfo_;   // Custom style settings
};
```

#### **Event IDs:**

- `atSetTime` (0x1b01) - At specific time
- `atAppStartup` (0x1b02) - When app starts
- `atSysWakeUp` (0x1b03) - When system wakes
- `beforePwrAction` (0x1b04) - Before power action
- `wakeAfterAction` (0x1b05) - After power action (wake)
- `atAppExit` (0x1b06) - When app exits

#### **Message Style Settings (RmdMsgStyleSet):**

- Background color, text color
- Font name, font size
- Icon ID, icon size, icon position
- Timeout value
- Display position
- Horizontal/vertical margins

### 4. PwrRuntimeItem

```cpp
class PwrRuntimeItem {
    int categoryId_;             // Feature category (powerAction, schedule, hotkeySet, pwrReminder)
    unsigned itemId_;            // Item ID
    int displayFlag_;            // Display flag (for reminders)
    int skipFlag_;               // Skip flag (for schedules)
    int snoozeFlag_;             // Snooze trigger flag
    ClockTime nextSnoozeTime_;   // Next snooze trigger time
};
```

Used for runtime state management of scheduled actions and reminders.

---

## Features and Functionality

### 1. Power Actions

#### **Supported Actions:**

- Display Off (`APP_ACTION_DISPLAYOFF`)
- Sleep (`APP_ACTION_SLEEP`)
- Shutdown (`APP_ACTION_SHUTDOWN`)
- Restart (`APP_ACTION_RESTART`)
- Sign Out (`APP_ACTION_SIGNOUT`)
- Hibernate (`APP_ACTION_HIBERNATE`)

#### **Execution Flow:**

1. User triggers action (button, hotkey, schedule, etc.)
2. `CPowerPlusDlg::executeAction()` called
3. Confirmation dialog shown (if enabled)
4. `AppCore::executePowerAction()` called
5. Windows API functions invoked:
   - Display Off: `PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2)`
   - Sleep: `SetSuspendState(false, false, false)`
   - Hibernate: `SetSuspendState(true, false, false)`
   - Shutdown/Restart/Sign Out: `ExitWindowsEx()` with appropriate flags
6. History logged
7. Event log written

#### **Privilege Requirements:**

- Shutdown/Restart/Sign Out require `SE_SHUTDOWN_NAME` privilege
- Application adjusts token privileges before execution

### 2. Action Scheduling

#### **Architecture:**

- Timer-based system (`SetTimer()` with 1-second interval)
- Timer ID: `TimerId::ActionSchedule`
- Processes every second to check for matching times

#### **Processing Flow:**

1. `OnTimer()` called every second
2. `processActionSchedule()` invoked
3. Gets current time
4. Iterates through default item + extra items
5. For each item:
   - Checks if enabled
   - Checks if repeat is enabled and day is active
   - Checks for notification trigger (30 seconds before)
   - Checks for action trigger (exact time match)
   - Executes action if triggered
   - Disables item if not repeating

#### **Time Matching:**

- Uses `ClockTimeUtils::isMatching()` with tolerance (default 0 seconds)
- Notification check uses -30 second tolerance

#### **Skip and Snooze:**

- Skip flag: Prevents execution once (set via notification)
- Snooze flag: Delays execution by snooze interval
- Managed via `PwrRuntimeItem` in runtime queue

#### **Repeat Configuration:**

- Daily repeat option
- Active days selection (bitmask)
- Snooze support with configurable interval

### 3. Hotkey System

#### **Registration:**

- Uses Windows `RegisterHotKey()` API
- Registered in `setupBackgroundHotkey()`
- Each hotkey gets unique ID (HotkeyID enum)
- Modifiers: `MOD_ALT`, `MOD_CONTROL`, `MOD_WIN`, `MOD_NOREPEAT`

#### **Processing:**

- `WM_HOTKEY` message handler in `WindowProc()`
- `processHotkey()` called with hotkey ID
- Maps hotkey ID to action ID
- Executes action via `executeAction()`

#### **Background Hotkeys (Lock Screen Support):**

- Low-level keyboard hook (`WH_KEYBOARD_LL`)
- Hook procedure: `CPowerPlusApp::KeyboardProc()`
- Only processes function keys (F1-F12) with modifiers
- Converts keycode to hotkey ID
- Posts message to main dialog
- `processLockStateHotkey()` handles lock screen hotkeys

#### **Limitations:**

- Windows `RegisterHotKey()` only works when app has focus
- Background hotkeys require keyboard hook
- Hook only processes function keys on lock screen

### 4. Power Reminder

#### **Architecture:**

- Timer-based system (`SetTimer()` with 1-second interval)
- Timer ID: `TimerId::PowerReminder`
- Processes "at set time" events

#### **Event Triggers:**

- **At Set Time**: Timer-based, checks every second
- **At App Startup**: Triggered in `OnInitDialog()`
- **At System Wake**: Triggered in `OnPowerBroadcastEvent()` (PBT_POWERRESUMEAUTOMATIC)
- **Before Power Action**: Triggered before `executeAction()`
- **Wake After Action**: Triggered after sleep/hibernate (via power broadcast)
- **At App Exit**: Triggered in `OnDestroy()`

#### **Display:**

- Two styles: MessageBox or DialogBox
- Custom style support (colors, fonts, icons, positioning)
- Timeout support (auto-close)
- Snooze support with configurable interval

#### **Processing Flow:**

1. Event triggered
2. `executePowerReminder()` called
3. Checks if reminder is enabled
4. Checks repeat configuration (if applicable)
5. Checks display flag (prevents duplicate displays)
6. `displayPwrReminder()` called
7. Creates and shows reminder dialog
8. User can: Close, Snooze (if enabled)
9. Display flag set to prevent duplicates

#### **Snooze:**

- Calculates next snooze time
- Stores in runtime queue
- Timer processes snooze times
- Re-displays reminder at snooze time

### 5. System Event Handling

#### **Power Broadcast Events:**

- `WM_POWERBROADCAST` message handler
- Handles: `PBT_APMSUSPEND`, `PBT_APMRESUMEAUTOMATIC`, `PBT_APMRESUMESUSPEND`
- Triggers power reminders
- Updates system event tracking

#### **Session Change Events:**

- `WM_WTSSESSION_CHANGE` message handler
- Handles: `WTS_SESSION_LOCK`, `WTS_SESSION_UNLOCK`, `WTS_SESSION_LOGOFF`
- Updates session lock flag
- Enables/disables lock screen hotkeys

#### **Query End Session:**

- `WM_QUERYENDSESSION` message handler
- Handles system shutdown/restart requests
- Can prevent shutdown (if needed)
- Triggers "at app exit" reminders

### 6. Notify Icon

#### **Management:**

- Created in `createNotifyIcon()`
- Uses `Shell_NotifyIcon()` API
- Shows/hides based on dialog visibility
- Context menu for quick actions

#### **Features:**

- Tooltip text
- Balloon notifications
- Context menu
- Double-click to show dialog

---

## System Integration

### 1. Windows Registry

#### **Registry Structure:**

```markdown
HKEY_CURRENT_USER\Software\[CompanyName]\[AppName]\
    ├── Config\           (Application configuration)
    ├── Schedule\         (Schedule data)
    │   ├── Default\      (Default schedule item)
    │   └── Extra\        (Extra schedule items)
    ├── HotkeySet\        (Hotkey data)
    ├── PowerReminder\    (Reminder data)
    │   ├── CommonStyle\  (Common style settings)
    │   └── Items\        (Reminder items)
    ├── Layout\           (Dialog layout info)
    ├── SystemEvent\      (System event tracking)
    └── Global\           (Global data)
```

#### **Serialization:**

- `AppRegistry` namespace handles all registry operations
- Functions: `getRegistryValueInt()`, `writeRegistryValueInt()`, etc.
- Nested section support
- Error handling and validation

### 2. Windows API Integration

#### **Power Management:**

- `SetSuspendState()` - Sleep/Hibernate
- `ExitWindowsEx()` - Shutdown/Restart/Sign Out
- `PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, ...)` - Display Off

#### **Hotkey Management:**

- `RegisterHotKey()` - Register hotkeys
- `UnregisterHotKey()` - Unregister hotkeys
- `SetWindowsHookEx(WH_KEYBOARD_LL)` - Low-level keyboard hook

#### **System Events:**

- `RegisterPowerSettingNotification()` - Power broadcast events
- `RegisterSessionNotification()` - Session change events

#### **Privilege Management:**

- `OpenProcessToken()` - Get process token
- `LookupPrivilegeValue()` - Get privilege value
- `AdjustTokenPrivileges()` - Enable privileges

### 3. Auto-Start Integration

#### **Methods:**

1. **Registry Method** (Current User):
   - `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`
   - Key: Application name
   - Value: Executable path

2. **Task Scheduler Method** (Admin):
   - Uses `schtasks` command
   - Creates task: "On logon", "Highest" privilege level
   - Supports run-as-admin option

#### **Implementation:**

- `enableAutoStart()` function
- Checks current user vs admin requirements
- Uses appropriate method based on privileges

---

## Data Persistence

### 1. Registry-Based Storage

#### **Advantages:**

- Integrated with Windows
- Automatic backup/restore
- User-specific storage
- No file management needed

#### **Data Types Stored:**

- Integer values (options, flags, IDs)
- String values (messages, paths)
- Structured data (serialized as multiple keys)

### 2. File-Based Storage

#### **Log Files:**

- Application event logs (monthly files)
- History logs (action history)
- Trace error logs (debug)
- Trace debug logs (debug)
- Debug info logs (debug)

#### **File Locations:**

- Base: Application directory
- Logs: `[AppDir]\Log\`
- Backup: `[AppDir]\Backup\`
- Help: Embedded in executable (resource)

#### **File Naming:**

- Event logs: `AppEventLog_YYYY_MM.log`
- History: `AppHistory.log`
- Trace: `TraceError.log`, `TraceDebug.log`, `DebugInfo.log`

### 3. Backup System

#### **Registry Export:**

- Uses `reg.exe export` command
- Exports entire application registry branch
- Saves to `.reg` file
- Timestamped filenames

#### **Implementation:**

- `BackupSystem::RegistryExport()`
- Executes registry export command
- Saves to backup directory

---

## Logging System

### 1. Logger Class

#### **Log Types:**

- `AppEvent` - Application events
- `AppHistory` - Action history
- `TraceError` - Error traces
- `TraceDebug` - Debug traces
- `DebugInfo` - Debug information

#### **Log Item Structure:**

```cpp
class LogItem {
    DateTime timeValue_;         // Timestamp
    DWORD processId_;            // Process ID
    uint16 categoryId_;          // Category ID
    String logString_;           // Log message
    LOGDETAILINFO detailInfo_;   // Additional details
};
```

#### **Log Detail Structure:**

```cpp
class LogDetail {
    uint16 categoryId_;         // Detail category
    int flag_;                  // Detail flag
    int detailValue_;           // Integer value
    String detailInfoString_;   // String value
    void* detailDataPtr_;       // Pointer data
    byte pointerType_;          // Pointer type
    size_t pointerSize_;        // Pointer size
};
```

#### **Output Formats:**

- YAML format (structured)
- Plain text format
- JSON format (for structured data)

### 2. Debug Logger

#### **Singleton Pattern:**

- Single instance for entire application
- Accessed via `DebugLogger::getDebugLogger()`

#### **Log Files:**

- `TraceError.log` - Error traces
- `TraceDebug.log` - Debug traces
- `DebugInfo.log` - Debug information

#### **Features:**

- Automatic file rotation
- Backup old log files
- Exception handling
- Thread-safe operations

### 3. Event Logging

#### **Event Categories:**

- Dialog events (init, show, hide, close)
- Control events (button click, checkbox change, etc.)
- Action events (power action execution)
- Schedule events (schedule execution)
- Hotkey events (hotkey execution)
- Reminder events (reminder display)

#### **Logging Functions:**

- `outputEventLog()` - General event logging
- `outputButtonLog()` - Button events
- `outputCheckBoxLog()` - Checkbox events
- `outputComboBoxLog()` - ComboBox events
- `outputScheduleEventLog()` - Schedule events
- `outputPwrReminderEventLog()` - Reminder events

---

## User Interface Framework

### 1. Dialog System

#### **Dialog Registration:**

- Each dialog registers itself on creation
- Maintains parent-child relationships
- Tracks all open dialogs
- Automatic cleanup on destruction

#### **Dialog Management:**

- `registerDialogManagement()` - Register dialog
- `updateDialogManagement()` - Update dialog state
- `unregisterDialogManagement()` - Unregister dialog

#### **Dialog Lifecycle:**

1. Construction
2. `OnInitDialog()` - Initialize
3. `registerDialogManagement()` - Register
4. Runtime (user interaction)
5. `PreDestroyDialog()` - Pre-destroy
6. `OnDestroy()` - Destroy
7. `unregisterDialogManagement()` - Unregister

### 2. Control Management

#### **SControlManager:**

- Manages UI controls
- Provides control access
- Handles control state
- Supports control grouping

#### **Control Operations:**

- `addControl()` - Add control
- `removeControl()` - Remove control
- `enableItem()` - Enable/disable control
- `showItem()` - Show/hide control
- `setControlText()` - Set control text

### 3. Layout Management

#### **Layout System:**

- Anchor point system
- Alignment system (left, right, top, bottom, center)
- Size management (min, max, default)
- Margin management

#### **Layout Persistence:**

- Saves layout info to registry
- Loads layout on dialog creation
- Supports per-dialog layouts

### 4. Grid Control

#### **Custom Grid Control:**

- Based on MFC grid control
- Supports: fixed cells, checkbox cells, normal cells
- Column formatting
- Row/column resizing
- Cell editing

#### **Grid Features:**

- Column styles (fixed, checkbox, normal)
- Header customization
- Cell content editing
- Checkbox support
- Data binding

### 5. Language System

#### **Language Tables:**

- `LANGTABLE_PTR` - Pointer to language table
- `MapTable` class - Manages language strings
- Resource-based strings
- Dynamic language switching

#### **Language Management:**

- `initAppLanguage()` - Initialize language
- `reloadAppLanguage()` - Reload language
- `getLanguageString()` - Get string by ID
- `setupLanguage()` - Setup dialog language

#### **Supported Languages:**

- English
- Vietnamese
- Simplified Chinese

---

## Code Patterns and Conventions

### 1. Naming Conventions

#### **Current State (Post-Refactoring):**

- Functions: `camelCase` (e.g., `executeAction()`, `processActionSchedule()`)
- Member variables: `camelCase_` with trailing underscore (e.g., `appConfigDataPtr_`, `scheduleData_`)
- Constants: `k` prefix + `PascalCase` (e.g., `kMaxItemNum`, `kDefaultItemID`)
- Classes: `PascalCase` (e.g., `CPowerPlusApp`, `ScheduleItem`)
- Enums: `PascalCase` (e.g., `AppOptionID`, `TimerId`)

#### **Previous State (Pre-Refactoring):**

- Functions: `PascalCase` (e.g., `ExecuteAction()`, `ProcessActionSchedule()`)
- Member variables: `m_` prefix + `PascalCase` (e.g., `m_pAppConfigData`, `m_schSchedule`)

### 2. Error Handling

#### **Error Codes:**

- Application-defined error codes (`APP_ERROR_*`)
- Windows error codes (`GetLastError()`)
- Return values: `bool` for success/failure, `DWORD` for error codes

#### **Error Handling Pattern:**

```cpp
bool result = false;
DWORD errorCode = APP_ERROR_SUCCESS;

// Perform operation
if (!operation()) {
    errorCode = GetLastError();
    TRACE_FORMAT("Error: Operation failed (Code: 0x%08X)", errorCode);
    return false;
}

return true;
```

### 3. Memory Management

#### **Smart Pointers:**

- Limited use of smart pointers
- Primarily raw pointers with manual management
- RAII principles followed where applicable

#### **Resource Management:**

- Constructor: Initialize resources
- Destructor: Clean up resources
- Copy constructors: Deep copy
- Assignment operators: Deep copy

### 4. Data Validation

#### **Input Validation:**

- Range checks for numeric values
- String length checks
- Enum value validation
- Null pointer checks

#### **Data Integrity:**

- `isEmpty()` checks
- `compare()` functions for data comparison
- `copy()` functions for safe copying
- Validation before serialization

### 5. Threading Model

#### **Single-Threaded:**

- Main UI thread only
- No explicit threading
- Timer-based background tasks
- Message-based communication

#### **Timer System:**

- `SetTimer()` for periodic tasks
- Timer IDs: `TimerId` enum
- 1-second intervals for scheduling
- Timer cleanup on dialog destruction

### 6. Message Handling

#### **MFC Message Map:**

- `BEGIN_MESSAGE_MAP()` / `END_MESSAGE_MAP()`
- `ON_COMMAND()` for menu/button commands
- `ON_MESSAGE()` for custom messages
- `ON_WM_*()` for Windows messages

#### **Custom Messages:**

- `WM_USER + offset` for application messages
- Message routing between dialogs
- Parent-child communication

---

## Migration Considerations

### 1. Cross-Platform Migration Strategy

#### **Current Platform Constraints:**

- **Windows-only**: MFC dependency, Windows Registry, Windows API
- **Architecture**: x64 only
- **OS Support**: Windows 7/8/10/11
- **Build System**: Visual Studio / MSBuild

#### **Target Platforms:**

1. **Windows** (Primary)
   - Maintain existing functionality
   - Modernize UI framework
   - Improve architecture

2. **Linux** (Secondary)
   - Desktop environments (GNOME, KDE, XFCE)
   - Systemd/logind integration
   - X11/Wayland support

3. **macOS** (Future)
   - Cocoa integration
   - macOS-specific power management
   - System Preferences integration

#### **Migration Approach: Strangler Fig Pattern**

Instead of a complete rewrite, use incremental migration:

1. **Build new architecture alongside old code**
2. **Gradually migrate features** from old to new
3. **Route functionality** through new architecture
4. **Remove old code** once migration is complete

#### **Migration Phases**

##### **Phase 1: Foundation (Platform-Independent Core)**

- Extract domain models (ScheduleItem, HotkeyItem, etc.)
- Create platform abstraction interfaces
- Implement Windows PAL (Platform Abstraction Layer)
- Set up cross-platform build system (CMake)
- Create unit tests for domain logic

##### **Phase 2: Core Features Migration**

- Power action execution (via PAL)
- Configuration management (abstract storage)
- Basic scheduling logic
- Data serialization (JSON format)

##### **Phase 3: UI Framework Migration**

- Choose UI framework (Qt recommended)
- Migrate dialogs to new framework
- Implement platform-specific UI components
- Maintain feature parity

##### **Phase 4: Advanced Features**

- Hotkey system (platform-specific implementations)
- System tray integration
- Reminder system
- System event handling

##### **Phase 5: Platform Expansion**

- Linux implementation
- macOS implementation (if needed)
- Platform-specific optimizations
- Testing and validation

### 2. Platform Abstraction Implementation

#### **Power Management Abstraction**

**Windows Implementation:**

```cpp
class WindowsPowerManager : public IPowerManager {
public:
    Result<void, ErrorCode> executeAction(Action action) override {
        switch (action) {
            case Action::Shutdown:
                return executeShutdown();
            case Action::Sleep:
                return executeSleep();
            // ... other actions
        }
    }
    
private:
    Result<void, ErrorCode> executeShutdown() {
        // Windows-specific: ExitWindowsEx()
        HANDLE token;
        if (!OpenProcessToken(GetCurrentProcess(), 
                              TOKEN_ADJUST_PRIVILEGES, &token)) {
            return Result<void, ErrorCode>::error(GetLastError());
        }
        // ... privilege adjustment and shutdown
    }
};
```

**Linux Implementation:**

```cpp
class LinuxPowerManager : public IPowerManager {
public:
    Result<void, ErrorCode> executeAction(Action action) override {
        switch (action) {
            case Action::Shutdown:
                return executeSystemdCommand("poweroff");
            case Action::Sleep:
                return executeSystemdCommand("suspend");
            // ... other actions
        }
    }
    
private:
    Result<void, ErrorCode> executeSystemdCommand(const std::string& cmd) {
        // Use systemd's logind D-Bus interface
        // or execute systemctl command
    }
};
```

#### **Configuration Storage Abstraction**

**Registry Storage (Windows):**

```cpp
class RegistryStorage : public IConfigurationStorage {
    Result<ConfigData, ErrorCode> loadConfig() override {
        // Read from Windows Registry
        // HKEY_CURRENT_USER\Software\...
    }
};
```

**JSON File Storage (Cross-Platform):**

```cpp
class JsonFileStorage : public IConfigurationStorage {
    std::filesystem::path getConfigPath() {
        #ifdef _WIN32
            return std::filesystem::path(std::getenv("APPDATA")) 
                   / "PowerPlus3" / "config.json";
        #elif __linux__
            return std::filesystem::path(std::getenv("HOME")) 
                   / ".config" / "PowerPlus3" / "config.json";
        #elif __APPLE__
            return std::filesystem::path(std::getenv("HOME")) 
                   / "Library" / "Application Support" / "PowerPlus3" 
                   / "config.json";
        #endif
    }
    
    Result<ConfigData, ErrorCode> loadConfig() override {
        auto path = getConfigPath();
        // Read and parse JSON file
    }
};
```

#### **Hotkey Management Abstraction**

**Windows Implementation:**

- Uses `RegisterHotKey()` API
- Low-level keyboard hook for background hotkeys
- Virtual key codes and modifiers

**Linux Implementation:**

- X11: `XGrabKey()` API
- Wayland: Compositor-specific protocols
- Key symbol mapping (X11 keysyms)

**macOS Implementation:**

- Carbon: `RegisterEventHotKey()`
- Cocoa: `NSEvent` monitoring
- Key code mapping

### 3. Refactoring Steps for Cross-Platform

#### **Step 1: Extract Domain Models**

**Current Code:**

```cpp
// Windows-specific code mixed with business logic
void CPowerPlusDlg::executeAction(unsigned actionId) {
    // Business logic
    if (actionId == APP_ACTION_SHUTDOWN) {
        // Windows API call
        ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);
    }
}
```

**Refactored Code:**

```cpp
// Domain layer (platform-independent)
class PowerActionService {
public:
    PowerActionService(std::unique_ptr<IPowerManager> powerManager)
        : powerManager_(std::move(powerManager)) {}
    
    Result<void, ErrorCode> executeAction(PowerAction action) {
        return powerManager_->executeAction(action);
    }
    
private:
    std::unique_ptr<IPowerManager> powerManager_;
};

// Application layer
class PowerActionUseCase {
public:
    PowerActionUseCase(PowerActionService& service) 
        : service_(service) {}
    
    Result<void, ErrorCode> execute(PowerAction action) {
        // Business logic (validation, logging, etc.)
        return service_.executeAction(action);
    }
};
```

#### **Step 2: Abstract System Integration**

**Current Code:**

```cpp
// Direct Windows API usage
void CPowerPlusApp::InitInstance() {
    SetRegistryKey("CompanyName");
    // ... registry operations
}
```

**Refactored Code:**

```cpp
// Abstract interface
class IConfigurationStorage {
    virtual Result<void, ErrorCode> initialize() = 0;
    virtual Result<ConfigData, ErrorCode> loadConfig() = 0;
};

// Factory pattern
class PlatformFactory {
public:
    static std::unique_ptr<IConfigurationStorage> createStorage() {
        #ifdef _WIN32
            return std::make_unique<RegistryStorage>();
        #else
            return std::make_unique<JsonFileStorage>();
        #endif
    }
    
    static std::unique_ptr<IPowerManager> createPowerManager() {
        #ifdef _WIN32
            return std::make_unique<WindowsPowerManager>();
        #elif __linux__
            return std::make_unique<LinuxPowerManager>();
        #elif __APPLE__
            return std::make_unique<MacOSPowerManager>();
        #endif
    }
};
```

#### **Step 3: UI Framework Migration**

##### **Current: MFC Dialogs**

```cpp
class CPowerPlusDlg : public CDialogEx {
    // MFC-specific code
};
```

##### **Target: Qt Framework**

```cpp
// Platform-independent base
class MainWindowBase : public QMainWindow {
    Q_OBJECT
public:
    virtual void setupUI() = 0;
    virtual void connectSignals() = 0;
};

// Platform-specific implementations if needed
class WindowsMainWindow : public MainWindowBase {
    // Windows-specific UI customizations
};

class LinuxMainWindow : public MainWindowBase {
    // Linux-specific UI customizations
};
```

### 4. Data Migration Strategy

#### **Registry to JSON Migration**

**Migration Tool:**

```cpp
class DataMigrationTool {
public:
    Result<void, ErrorCode> migrateFromRegistry() {
        // 1. Read from Windows Registry
        auto config = registryStorage_.loadConfig();
        auto schedule = registryStorage_.loadSchedule();
        
        // 2. Convert to JSON format
        nlohmann::json jsonConfig = toJson(config);
        nlohmann::json jsonSchedule = toJson(schedule);
        
        // 3. Save to JSON files
        jsonStorage_.saveConfig(jsonConfig);
        jsonStorage_.saveSchedule(jsonSchedule);
        
        // 4. Verify migration
        return verifyMigration();
    }
};
```

**Backward Compatibility:**

- Support loading from both Registry and JSON
- Automatic migration on first run
- Backup old data before migration

### 5. Build System Migration

#### **CMake Configuration**

**CMakeLists.txt Structure:**

```cmake
cmake_minimum_required(VERSION 3.20)
project(PowerPlus3 VERSION 3.3.0 LANGUAGES CXX)

# Platform detection
if(WIN32)
    set(PLATFORM_WINDOWS ON)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX ON)
elseif(APPLE)
    set(PLATFORM_MACOS ON)
endif()

# Dependencies
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(nlohmann_json REQUIRED)

# Platform-specific sources
if(PLATFORM_WINDOWS)
    add_subdirectory(src/infrastructure/platform/windows)
elseif(PLATFORM_LINUX)
    add_subdirectory(src/infrastructure/platform/linux)
elseif(PLATFORM_MACOS)
    add_subdirectory(src/infrastructure/platform/macos)
endif()

# Common sources
add_subdirectory(src/domain)
add_subdirectory(src/application)
add_subdirectory(src/infrastructure/storage)
```

### 6. Testing Strategy for Cross-Platform

#### **Unit Testing**

- Test domain logic independently
- Mock platform abstraction interfaces
- Platform-agnostic test cases

#### **Integration Testing**

- Test platform-specific implementations
- Verify PAL interface contracts
- Cross-platform behavior validation

#### **Platform-Specific Testing**

- Windows: Test Registry storage, Windows APIs
- Linux: Test systemd integration, X11/Wayland
- macOS: Test Cocoa integration, macOS APIs

### 7. Challenges and Solutions

#### **Challenge 1: Power Management Differences**

**Problem:**

- Windows: `ExitWindowsEx()`, `SetSuspendState()`
- Linux: systemd/logind D-Bus, `systemctl` commands
- macOS: `osascript`, `pmset`, Cocoa APIs

**Solution:**

- Abstract all power operations through `IPowerManager`
- Platform-specific implementations
- Feature detection for supported actions

#### **Challenge 2: Hotkey System**

**Problem:**

- Windows: `RegisterHotKey()`, keyboard hooks
- Linux: X11 `XGrabKey()`, Wayland protocols
- macOS: Carbon/Cocoa hotkey APIs

**Solution:**

- Abstract hotkey registration
- Key code mapping between platforms
- Graceful degradation if not supported

#### **Challenge 3: System Tray**

**Problem:**

- Windows: Shell_NotifyIcon
- Linux: Various implementations (libappindicator, StatusNotifier)
- macOS: NSStatusItem

**Solution:**

- Use Qt's system tray (cross-platform)
- Or implement platform-specific wrappers

#### **Challenge 4: Configuration Storage**

**Problem:**

- Windows: Registry (hierarchical, binary-safe)
- Linux/macOS: Files (JSON, INI, etc.)

**Solution:**

- JSON file storage for cross-platform
- Registry support for Windows (optional)
- Migration tool for existing data

### 8. Migration Timeline and Priorities

#### **Recommended Migration Order:**

1. **High Priority (Core Functionality)**
   - Domain models extraction
   - Power management abstraction
   - Configuration storage abstraction
   - Basic UI framework migration

2. **Medium Priority (Features)**
   - Scheduling system
   - Reminder system
   - Hotkey management
   - System tray integration

3. **Low Priority (Polish)**
   - Advanced UI features
   - Platform-specific optimizations
   - Additional platforms (macOS)

#### **Risk Mitigation:**

- Maintain Windows version during migration
- Feature parity validation
- Automated testing
- Gradual rollout
- User feedback collection

### 3. Code Modernization

#### **C++ Standards:**

- Current: C++11/14 features
- Recommended: C++17/20
- Use modern features: `std::optional`, `std::variant`, ranges

#### **Smart Pointers:**

- Replace raw pointers with smart pointers
- `std::unique_ptr` for ownership
- `std::shared_ptr` for shared ownership
- `std::weak_ptr` for non-owning references

#### **STL Containers:**

- Already using `std::vector`, `std::unordered_map`
- Consider: `std::array`, `std::string_view`, `std::span`

#### **Error Handling:**

- Consider `std::expected` (C++23) or similar
- Exception safety improvements
- Error propagation patterns

### 4. Testing Strategy

#### **Current State:**

- Limited testing infrastructure
- Manual testing primarily
- Debug logging for diagnostics

#### **Recommended:**

1. **Unit Testing**
   - Google Test / Catch2
   - Test core logic
   - Mock dependencies

2. **Integration Testing**
   - Test component interactions
   - Test data serialization
   - Test system integration

3. **UI Testing**
   - Automated UI tests
   - Dialog interaction tests
   - End-to-end scenarios

### 5. Documentation Improvements

#### **Current State:**

- Header file documentation
- Inline comments
- Limited external documentation

#### **Recommended:**

1. **API Documentation**
   - Doxygen/XML documentation
   - API reference
   - Usage examples

2. **Architecture Documentation**
   - Component diagrams
   - Sequence diagrams
   - Data flow diagrams

3. **User Documentation**
   - User manual
   - Feature guides
   - Troubleshooting guides

### 6. Build System Modernization

#### **Current:**

- Visual Studio project files
- MSBuild
- Windows-only

#### **Recommended:**

1. **CMake**
   - Cross-platform build system
   - Better dependency management
   - IDE-agnostic

2. **Package Management**
   - vcpkg / Conan
   - Dependency management
   - Version control

3. **CI/CD**
   - Automated builds
   - Automated testing
   - Automated deployment

---

## Conclusion

PowerPlus3 is a well-structured Windows application with a clear separation of concerns between UI, business logic, and system integration. The codebase follows MFC patterns and Windows API conventions, making it maintainable for Windows-specific development.

For migration to other platforms or modernization, the core business logic (power actions, scheduling, reminders) can be preserved while replacing the UI layer and system integration components. The data structures are well-defined and can be adapted to different storage backends.

The application demonstrates good practices in:

- Data serialization
- Event logging
- Error handling
- Resource management
- System integration

Areas for improvement include:

- Cross-platform support
- Modern C++ features
- Testing infrastructure
- Architecture decoupling
- Configuration abstraction

This documentation should serve as a comprehensive reference for understanding, maintaining, and migrating the PowerPlus3 application.
