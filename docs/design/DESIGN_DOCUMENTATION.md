# PowerPlus3 Design Documentation

## Table of Contents

1. [Project Overview](#project-overview)
2. [Architecture](#architecture)
3. [Core Components](#core-components)
4. [Data Models](#data-models)
5. [Features](#features)
6. [UI Framework](#ui-framework)
7. [Persistence Layer](#persistence-layer)
8. [Logging System](#logging-system)
9. [System Integration](#system-integration)
10. [Development Guidelines](#development-guidelines)

---

## Project Overview

### Purpose

PowerPlus3 (Power++) is a Windows-only, offline utility application written in C++ using Microsoft Foundation Classes (MFC). The application provides comprehensive power management capabilities, allowing users to control system power states through multiple interaction methods including mouse actions, keyboard shortcuts, scheduled tasks, and system tray notifications.

### Key Capabilities

- **Power Actions**: Execute system power operations (shutdown, restart, sleep, hibernate, sign out, display off)
- **Scheduled Actions**: Automatically execute power actions at specified times with repeat options
- **Hotkey Support**: Register global keyboard shortcuts for quick power action execution
- **Power Reminders**: Display customizable reminder messages at specific events or times
- **System Tray Integration**: Minimize to system tray with context menu access
- **Multi-language Support**: English, Vietnamese, and Simplified Chinese interfaces
- **Comprehensive Logging**: Track application events, action history, and system events

### Technology Stack

- **Language**: C++ (C++11/14 standard)
- **Framework**: Microsoft Foundation Classes (MFC)
- **Platform**: Windows (Win32 API)
- **Build System**: Visual Studio (2019 or later recommended)
- **Storage**: Windows Registry for configuration persistence

---

## Architecture

### High-Level Architecture

The application follows a layered architecture with clear separation of concerns:

```
┌─────────────────────────────────────────┐
│           Application Layer             │
│  (CPowerPlusApp, CPowerPlusDlg)         │
└─────────────────────────────────────────┘
                    │
┌─────────────────────────────────────────┐
│           Framework Layer               │
│  (SWinApp, SDialog, SElements)          │
└─────────────────────────────────────────┘
                    │
┌─────────────────────────────────────────┐
│           Core Layer                    │
│  (AppCore, Data Models, Utilities)      │
└─────────────────────────────────────────┘
                    │
┌─────────────────────────────────────────┐
│           Component Layer               │
│  (GridCtrl, Custom Controls)            │
└─────────────────────────────────────────┘
                    │
┌─────────────────────────────────────────┐
│           Platform Layer                │
│  (Win32 API, Registry, System Calls)    │
└─────────────────────────────────────────┘
```

### Module Organization

#### 1. **AppBase** (`include/AppBase/`, `source/AppBase/`)
- Base types and constants
- Application macros and definitions
- Resource definitions
- Windows application base classes

#### 2. **AppCore** (`include/AppCore/`, `source/AppCore/`)
- Core data structures and models
- Business logic for power actions
- Data serialization interfaces
- Language management
- Logging infrastructure
- ID management system

#### 3. **Framework** (`include/Framework/`, `source/Framework/`)
- Custom base classes for dialogs (`SDialog`)
- Custom base classes for applications (`SWinApp`)
- UI element management (`SElements`)
- Dialog control management

#### 4. **Components** (`include/Components/`, `source/Components/`)
- Grid control implementation (`GridCtrl`)
- Custom cell types (checkbox cells, etc.)
- In-place editing support
- Title tips and tooltips

#### 5. **Dialogs** (`include/Dialogs/`, `source/Dialogs/`)
- Feature-specific dialog implementations:
  - `AboutDlg`: About dialog
  - `HelpDlg`: Help and documentation viewer
  - `MultiScheduleDlg`: Schedule management
  - `HotkeySetDlg`: Hotkey configuration
  - `PwrReminderDlg`: Power reminder management
  - `LogViewerDlg`: Log file viewer
  - `ReminderMsgDlg`: Reminder message display
  - `RmdRepeatSetDlg`: Repeat settings for reminders
  - `MsgStyleSetDlg`: Message style customization

#### 6. **MainApp** (`include/MainApp/`, `source/MainApp/`)
- Main application class (`CPowerPlusApp`)
- Main dialog class (`CPowerPlusDlg`)
- Application entry point and initialization

---

## Core Components

### Application Class (`CPowerPlusApp`)

The main application class inherits from `SWinApp` and manages:

- **Application Lifecycle**: Initialization and cleanup
- **Data Management**: Configuration, schedule, hotkey, and reminder data
- **System Integration**: Keyboard hooks, power broadcast events, session notifications
- **Logging**: Application event and history logging
- **Registry Operations**: Loading and saving application data
- **Auto-start Management**: Windows startup integration

**Key Responsibilities:**
- Initialize application data structures
- Load/save configuration from/to Windows Registry
- Manage global keyboard hooks for background hotkeys
- Handle system power events and session changes
- Provide access to application-wide data and settings

### Main Dialog Class (`CPowerPlusDlg`)

The main window dialog inherits from `SDialog` and provides:

- **User Interface**: Main application window with collapsible/expandable design
- **Action Execution**: Coordinate power action execution
- **Feature Management**: Schedule processing, hotkey handling, reminder display
- **System Tray Integration**: Notify icon creation and management
- **Child Dialog Management**: Open and manage feature dialogs
- **Settings Management**: Apply, reload, and backup configuration

**Key Responsibilities:**
- Display and manage main application UI
- Execute power actions based on user input (mouse, hotkey, schedule)
- Process scheduled actions and reminders
- Manage system tray icon and context menu
- Coordinate between different application features

### Core Action Execution (`AppCore::executePowerAction`)

The core power action execution function:

- **Action Types**: Monitor actions (display off) and power actions (shutdown, restart, sleep, hibernate, sign out)
- **Error Handling**: Comprehensive error code reporting
- **System Integration**: Direct Win32 API calls for power management
- **Dummy Mode**: Test mode that simulates actions without actual execution

---

## Data Models

### Configuration Data (`ConfigData`)

Stores application settings and preferences:

```cpp
struct CONFIGDATAINFO {
    int  leftMouseAction;            // Left mouse button action
    int  middleMouseAction;          // Middle mouse button action
    int  rightMouseAction;           // Right mouse button action
    BOOL rightMouseShowMenu;         // Right mouse: show menu only
    int  languageID;                 // Language setting
    BOOL showDialogAtStartup;        // Show dialog at startup
    BOOL enableAutoStart;            // Startup with Windows
    BOOL actionConfirmation;         // Confirm before action
    BOOL saveActionHistory;          // Save action history log
    BOOL saveAppEventLog;            // Save app event log
    BOOL runAsAdmin;                 // Run with admin privileges
    BOOL showErrorMessage;           // Show error messages
    BOOL scheduleNotification;       // Notify for scheduled actions
    BOOL allowScheduleCancellation;  // Allow canceling schedules
    BOOL enableBackgroundHotkey;     // Enable background hotkeys
    BOOL allowLockscreenHotkey;      // Allow hotkeys on lockscreen
    BOOL enablePowerReminder;        // Enable Power Reminder feature
};
```

### Schedule Data (`ScheduleData`)

Manages scheduled power actions:

- **Default Item**: One default schedule item (ID: 0x00)
- **Extra Items**: Up to 100 additional schedule items (ID: 10000-19999)
- **Schedule Item** (`ScheduleItem`):
  - Item ID
  - Enabled/disabled state
  - Action ID (which power action to execute)
  - Time value (`ClockTime`)
  - Repeat settings (`PwrRepeatSet`)

**Repeat Settings** (`PwrRepeatSet`):
- Daily repeat enabled/disabled
- Snoozing allowed/disabled
- Snooze interval (60-1800 seconds)
- Active days of week (bitmask)

### Hotkey Set Data (`HotkeySetData`)

Manages keyboard shortcuts for power actions:

- **Hotkey Items** (`HotkeySetItem`):
  - Enabled/disabled state
  - Action ID
  - Modifier keys (Ctrl, Alt, Shift, Win)
  - Virtual key code

**Supported Actions:**
- Display Off (0x1a01)
- Sleep (0x1a02)
- Shutdown (0x1a03)
- Restart (0x1a04)
- Sign Out (0x1a05)
- Hibernate (0x1a06)

### Power Reminder Data (`PwrReminderData`)

Manages reminder messages:

- **Reminder Items** (`PwrReminderItem`):
  - Item ID (10000-19999)
  - Enabled/disabled state
  - Message content
  - Event ID (at set time, at app startup, at system wake, before power action, wake after action, at app exit)
  - Time value (for "at set time" events)
  - Message style (Message Box or Dialog Box)
  - Repeat settings
  - Custom style settings (`RmdMsgStyleSet`)

**Message Style Settings** (`RmdMsgStyleSet`):
- Background color
- Text color
- Font name and size
- Timeout value
- Icon ID and size
- Icon position (top/left)
- Display position (center, corners)
- Margins

### History Info Data (`HistoryInfoData`)

Tracks action execution history:

- Category ID (power action, schedule, hotkey, reminder)
- Timestamp
- Item ID
- Action ID
- Result (success/failure)
- Error code
- Description

### Runtime Queue (`PwrRuntimeQueue`)

In-memory queue for managing active features:

- **Runtime Items** (`PwrRuntimeItem`):
  - Category ID
  - Item ID
  - Display flag
  - Skip flag
  - Snooze flag
  - Next snooze time

Used for tracking active schedules and reminders that need periodic processing.

---

## Features

### 1. Power Actions

**Supported Actions:**
- **Display Off**: Turn off monitor display
- **Sleep**: Put system into sleep mode
- **Shutdown**: Shutdown the system
- **Restart**: Restart the system
- **Sign Out**: Log out current user
- **Hibernate**: Put system into hibernate mode

**Execution Methods:**
- Mouse button clicks (left, middle, right)
- Keyboard hotkeys (global shortcuts)
- Scheduled execution
- System tray menu
- Programmatic execution

**Confirmation:**
- Optional confirmation dialog before execution
- Can be bypassed for background hotkeys

### 2. Action Scheduling

**Capabilities:**
- Multiple scheduled actions (up to 100 items)
- Time-based scheduling (hour:minute)
- Daily repeat options
- Day-of-week filtering
- Snooze functionality
- Notification before execution
- Cancellation support

**Schedule Processing:**
- Timer-based checking (1-second interval)
- Queue management for active schedules
- Skip and snooze flag handling
- Automatic execution at scheduled time

### 3. Hotkey Management

**Features:**
- Global keyboard shortcuts (work system-wide)
- Background hotkey support (work when screen is locked)
- Modifier key combinations (Ctrl, Alt, Shift, Win)
- Hotkey validation (prevents conflicts with system shortcuts)
- Per-action hotkey assignment

**Registration:**
- Windows `RegisterHotKey` API
- Keyboard hook for background hotkeys
- Session lock state detection

### 4. Power Reminders

**Trigger Events:**
- At set time
- At application startup
- At system wake up
- Before power action execution
- After wake from action
- At application exit

**Display Styles:**
- Message Box (standard Windows message box)
- Dialog Box (custom styled dialog)

**Customization:**
- Background and text colors
- Font name and size
- Icon selection and size
- Icon position
- Display position on screen
- Timeout/auto-close
- Margins

**Repeat Options:**
- Daily repeat
- Day-of-week selection
- Snooze functionality with configurable interval

### 5. System Tray Integration

**Features:**
- Minimize to system tray
- Notify icon with tooltip
- Context menu with quick actions
- Balloon notifications
- Taskbar recreation handling

**Menu Options:**
- Quick power actions
- Open main window
- Open feature dialogs (Schedule, HotkeySet, Power Reminder)
- View logs
- Settings
- About
- Exit

### 6. Multi-language Support

**Supported Languages:**
- English
- Vietnamese
- Simplified Chinese

**Implementation:**
- Language table system (`Language.h`)
- Resource string management
- Runtime language switching
- Persistent language preference

### 7. Logging System

**Log Types:**
- **Application Event Log**: Application lifecycle events, dialog events, control interactions
- **Action History Log**: Power action execution history with timestamps and results
- **Trace Log**: Detailed execution traces (debug builds)
- **Debug Log**: Debug information and diagnostics

**Log Formats:**
- YAML format for event and history logs
- Text format for trace and debug logs
- Monthly rotation for event logs
- Daily rotation for history logs

**Log Locations:**
- Application data directory
- Subdirectories: `Log/Event/`, `Log/History/`, `Log/Trace/`, `Log/Debug/`

### 8. Configuration Management

**Storage:**
- Windows Registry (HKEY_CURRENT_USER)
- Structured registry sections:
  - Config: Application settings
  - Schedule: Schedule data
  - HotkeySet: Hotkey configurations
  - PwrReminder: Reminder settings
  - Layout: Dialog layout information
  - Global: Global application data

**Operations:**
- Load configuration on startup
- Save configuration on apply
- Backup/restore functionality
- Reload settings without restart
- Default configuration reset

---

## UI Framework

### Dialog Base Class (`SDialog`)

Custom base class extending MFC's `CDialogEx` with enhanced features:

**Key Features:**
- **Control Management**: Automatic control registration and state management
- **Flag Management**: Dialog-level flag system for state tracking
- **Layout Management**: Size, position, alignment, and margin control
- **Language Support**: Built-in language switching
- **Event Logging**: Automatic dialog event logging
- **Tooltip Support**: Integrated tooltip control
- **Read-only Mode**: Support for read-only dialog states
- **Lock State**: Control enable/disable states
- **Child Dialog Tracking**: Manage descendant dialogs

**Dialog Properties:**
- Minimum/maximum size constraints
- Alignment options (left, right, top, bottom, center)
- Anchor point positioning
- Background and text colors
- Top-most window support
- Initialization sound

### Application Base Class (`SWinApp`)

Custom base class extending MFC's `CWinAppEx`:

**Key Features:**
- **Language Management**: Application-wide language support
- **Flag Management**: Application-level flag system
- **Logging**: Application event logging
- **Exit Code Management**: Structured exit code handling
- **Message Processing**: Enhanced message filtering

### Grid Control (`CGridCtrl`)

Custom grid control for data display and editing:

**Features:**
- Multiple cell types (fixed, checkbox, normal, editable)
- In-place editing
- Column formatting and styling
- Row and column resizing
- Title tips
- Custom cell rendering

**Used In:**
- Schedule dialog (schedule item list)
- HotkeySet dialog (hotkey list)
- Power Reminder dialog (reminder item list)
- Log Viewer dialog (log entry list)

---

## Persistence Layer

### Registry Structure

```
HKEY_CURRENT_USER
└── Software
    └── [Application Name]
        ├── Config
        │   ├── LeftMouseAction
        │   ├── MiddleMouseAction
        │   ├── RightMouseAction
        │   ├── LanguageID
        │   └── ... (other config options)
        ├── Schedule
        │   ├── Default
        │   │   ├── Enabled
        │   │   ├── ActionID
        │   │   ├── TimeValue
        │   │   └── ... (repeat settings)
        │   └── Extra
        │       ├── ItemNum
        │       └── Item[0..N]
        │           └── ... (item data)
        ├── HotkeySet
        │   ├── ItemNum
        │   └── Item[0..N]
        │       ├── Enabled
        │       ├── ActionID
        │       ├── Modifiers
        │       └── VirtualKey
        ├── PwrReminder
        │   ├── CommonStyle
        │   │   ├── BackgroundColor
        │   │   ├── TextColor
        │   │   ├── FontName
        │   │   └── ... (style settings)
        │   ├── ItemNum
        │   └── Item[0..N]
        │       ├── Enabled
        │       ├── ItemID
        │       ├── Message
        │       ├── EventID
        │       └── ... (item data)
        ├── Layout
        │   └── [DialogName]
        │       ├── Width
        │       ├── Height
        │       └── ... (layout data)
        └── Global
            ├── AppFlags
            ├── SystemEvents
            └── ... (global data)
```

### Serialization (`Serialization.h`)

**Namespace: `AppRegistry`**

Provides functions for reading/writing registry values:

- **Base Functions**: Generic registry read/write operations
- **Config Functions**: Application configuration data
- **Schedule Functions**: Schedule item data
- **HotkeySet Functions**: Hotkey configuration data
- **PwrReminder Functions**: Reminder data and style settings
- **Layout Functions**: Dialog layout information
- **Global Data Functions**: Application-wide data

**Data Types:**
- Integer values
- String values
- Structured data (serialized as integers or strings)

### Backup System (`BackupSystem`)

**Capabilities:**
- Export registry configuration to file
- Import configuration from file
- Backup before major changes
- Restore from backup

---

## Logging System

### Logger Class (`Logger`)

Manages log file creation and writing:

**Features:**
- Automatic file rotation (daily/monthly)
- Thread-safe logging
- Multiple log levels
- Formatted output
- File path management

### Log Categories

#### 1. Application Event Log
- **Purpose**: Track application lifecycle and user interactions
- **Format**: YAML
- **Rotation**: Monthly
- **Location**: `Log/Event/`
- **Events**: Dialog init/show/hide, button clicks, checkbox changes, menu selections, etc.

#### 2. Action History Log
- **Purpose**: Record power action executions
- **Format**: YAML
- **Rotation**: Daily
- **Location**: `Log/History/`
- **Content**: Action type, timestamp, result, error code, description

#### 3. Trace Log
- **Purpose**: Detailed execution traces
- **Format**: Text
- **Rotation**: Daily
- **Location**: `Log/Trace/`
- **Usage**: Debug builds, development

#### 4. Debug Log
- **Purpose**: Debug information and diagnostics
- **Format**: Text
- **Rotation**: Daily
- **Location**: `Log/Debug/`
- **Usage**: Debug builds, troubleshooting

### Log Event IDs

Structured event ID system for categorizing log entries:
- Dialog events
- Control events (button, checkbox, combo, etc.)
- Menu events
- Action events
- Schedule events
- Hotkey events
- Reminder events
- System events

---

## System Integration

### Windows API Integration

#### Power Management
- `ExitWindowsEx`: Shutdown, restart, sign out
- `SetSuspendState`: Sleep and hibernate
- `SendMessage(WM_SYSCOMMAND, SC_MONITORPOWER)`: Display off

#### System Events
- `WM_POWERBROADCAST`: Power state changes (suspend, resume)
- `WM_QUERYENDSESSION`: System shutdown notification
- `WM_ENDSESSION`: Session end notification
- `WM_WTSSESSION_CHANGE`: Session lock/unlock events

#### Hotkey Management
- `RegisterHotKey`: Register global hotkeys
- `UnregisterHotKey`: Unregister hotkeys
- `SetWindowsHookEx(WH_KEYBOARD_LL)`: Low-level keyboard hook for background hotkeys

#### System Tray
- `Shell_NotifyIcon`: Notify icon operations (add, modify, delete)
- `RegisterWindowMessage("TaskbarCreated")`: Handle taskbar recreation

#### Registry
- `RegCreateKeyEx`: Create registry keys
- `RegSetValueEx`: Write registry values
- `RegQueryValueEx`: Read registry values
- `RegDeleteKey`: Delete registry keys

### Auto-start Integration

**Registry Location:**
- `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`

**Features:**
- Enable/disable auto-start
- Run as administrator option
- Status checking

### Session Management

**WTS Session Notifications:**
- Register for session change notifications
- Handle session lock/unlock events
- Support background hotkeys on lockscreen

---

## Development Guidelines

### Code Style

- **Naming Conventions**:
  - Types: `PascalCase` (e.g., `ConfigData`, `ScheduleItem`)
  - Functions: `CamelCase` (e.g., `executeAction`, `getAppData`)
  - Variables: `snake_case` (e.g., `appConfigData_`, `notifyIconHandle_`)
  - Constants: `kConstantName` (e.g., `kDefaultItemNum`)
  - Macros: `UPPER_CASE` (e.g., `APP_ACTION_SHUTDOWN`)

- **File Organization**:
  - Header files (`.h`) in `include/` directory
  - Source files (`.cpp`) in `source/` directory
  - One class per file pair (`.h`/`.cpp`)

- **Documentation**:
  - File headers with purpose, author, date, copyright
  - Function documentation with `@brief`, `@param`, `@return`
  - Inline comments for complex logic

### Architecture Principles

1. **Separation of Concerns**: Clear boundaries between UI, business logic, and data persistence
2. **Single Responsibility**: Each class has a focused purpose
3. **Data Independence**: Data models are independent of UI and platform code
4. **Explicit Ownership**: Clear ownership of resources (pointers, handles)
5. **Error Handling**: Comprehensive error checking and reporting

### Best Practices

1. **Memory Management**:
   - Use smart pointers where appropriate
   - Explicit cleanup in destructors
   - Check for NULL before deletion

2. **Error Handling**:
   - Return error codes for operations
   - Log errors appropriately
   - Show user-friendly error messages

3. **Resource Management**:
   - Register resources (hotkeys, hooks) on creation
   - Unregister on destruction
   - Handle resource cleanup in error paths

4. **Thread Safety**:
   - Logging operations are thread-safe
   - UI operations on main thread only
   - Message-based communication between threads

5. **Configuration Management**:
   - Load configuration on startup
   - Save on explicit apply
   - Validate before saving
   - Provide backup/restore

### Testing Considerations

- **Dummy Test Mode**: Test power actions without actual execution
- **Debug Commands**: Built-in debug command system for testing
- **Logging**: Comprehensive logging for troubleshooting
- **Error Reporting**: Detailed error codes and messages

### Extension Points

1. **New Power Actions**: Add to `AppMacros.h` and implement in `AppCore::executePowerAction`
2. **New Dialogs**: Inherit from `SDialog` and follow dialog management pattern
3. **New Data Types**: Create data class following existing patterns (copy, compare, get/set)
4. **New Languages**: Add language table to `Language.h` and resource strings
5. **New Export Formats**: Extend serialization layer (currently Registry-only)

---

## Version History

### Version 3.2 (v3.2.1.0) - June 2025
- Power Reminder enhancements (snoozing, repeat, style customization)
- Multiple schedule actions support
- Background hotkeys on lockscreen
- YAML log format
- Monthly event log rotation
- Embedded help files

### Version 3.1 (v3.1.1.0) - September 2024
- Power Reminder feature
- Chinese language support
- Debug command functions
- Improved hotkey validation
- Enhanced logging

### Version 3.0 (v3.0.1.0) - June 2024
- Middle mouse button support
- App event logging
- Log viewer function
- Reload settings feature
- Background hotkey feature
- Schedule notification and cancellation

---

## References

- **Source Code**: `Sources/` directory
- **Documentation**: `Sources/docs/` directory
- **Resources**: `Sources/res/` directory
- **Project Files**: `Sources/project/` directory

---

*This documentation reflects the current state of PowerPlus3 as of version 3.2.1.0. For the most up-to-date information, refer to the source code and inline documentation.*

