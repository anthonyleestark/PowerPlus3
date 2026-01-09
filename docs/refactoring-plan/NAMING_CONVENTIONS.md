# Naming Conventions - PowerPlus3 Project

This document describes the naming conventions used in the PowerPlus3 project (excluding the Components section).

---

## 1. Classes

### Rules
- **Format**: `PascalCase`
- Starts with an uppercase letter, each subsequent word also starts with an uppercase letter
- No special prefixes or suffixes

### Examples
```cpp
class ConfigData
class ScheduleItem
class HotkeySetItem
class PwrReminderItem
class StringUtils
class ClockTimeUtils
class PerformanceCounter
class FlagManager
```

### Notes
- Utility classes often end with `Utils` (StringUtils, ClockTimeUtils, DateTimeUtils)
- Data management classes often end with `Data` (ScheduleData, HotkeySetData, PwrReminderData)
- Item management classes often end with `Item` (ScheduleItem, HotkeySetItem, PwrReminderItem)

---

## 2. Structs

### Rules
- **Format**: `PascalCase` or `UPPER_CASE`
- Both formats can be used depending on context

### Examples
```cpp
// PascalCase
struct CONFIGDATAINFO
struct RESOURCE_ID_MAP_ENTRY
struct SystemEvent
struct AppProfile

// UPPER_CASE (commonly used for data-defining structs)
struct JSON_ENTRY
```

### Notes
- Configuration information structs often use `UPPER_CASE` (CONFIGDATAINFO)
- Entry/map-defining structs often use `UPPER_CASE` with underscores (RESOURCE_ID_MAP_ENTRY)
- Class-like structs often use `PascalCase` (SystemEvent)

---

## 3. Enums

### Rules
- **Format**: `PascalCase` or `UPPER_CASE`
- Enum values typically use `PascalCase` or `UPPER_CASE` depending on the enum type

### Examples
```cpp
// Enum class with PascalCase
enum class AppOptionID : int {
    invalid = -1,
    leftMouseAction = 0,
    middleMouseAction,
    // ...
};

// Regular enum with UPPER_CASE
enum Flag {
    FLAG_OFF = 0x00,
    FLAG_ON  = 0x01,
};

enum Result {
    Success = 0x0000,
    Failure = 0xFFFF,
};
```

### Notes
- Enum classes typically use `PascalCase` for both the enum name and values
- Regular enums typically use `UPPER_CASE` for values

---

## 4. Functions

### Rules
- **Format**: `PascalCase`
- Starts with an uppercase letter
- Function names should clearly describe their purpose

### Examples
```cpp
void GetData(CONFIGDATAINFO& data) const noexcept;
int GetAppOption(AppOptionID appOptionID) const noexcept;
void SetItemID(unsigned itemID) noexcept;
bool IsEnabled(void) const noexcept;
void EnableItem(bool enabled) noexcept;
void Copy(const ScheduleItem& other) noexcept;
bool Compare(const ScheduleItem& other) const noexcept;
```

### Naming Conventions
- **Getter**: Starts with `Get` (GetItemID, GetTime, GetAction)
- **Setter**: Starts with `Set` (SetItemID, SetTime, SetAction)
- **Boolean check**: Starts with `Is` or `Has` (IsEnabled, IsEmpty, HasValue)
- **Enable/Disable**: Starts with `Enable` or `Disable` (EnableItem, EnableRepeat)
- **Action verbs**: Use clear verbs (Copy, Compare, Add, Update, Remove, Delete)

---

## 5. Variables

### 5.1. Member Variables

#### Rules
- **Format**: `m_` + `camelCase`
- Prefix `m_` to distinguish member variables
- After the prefix, use camelCase for the variable name

#### Examples
```cpp
// Boolean flags
bool m_isEnabled;          // Enable/disable state
bool m_isRepeated;         // Repeat state
bool m_allowSnoozing;      // Allow snoozing
bool m_useCustomStyle;     // Use custom style

// Numeric values
unsigned m_itemID;         // Item ID
int m_actionID;            // Action ID
int m_snoozeInterval;      // Snooze interval
unsigned m_fontSize;       // Font size
unsigned m_timeout;        // Timeout

// String values
String m_messageContent;   // Message content
String m_fontName;         // Font name

// Object references
ClockTime m_timeValue;     // Time value
PwrRepeatSet m_repeatSetInfo;  // Repeat set information
RmdMsgStyleSet m_msgStyleSetInfo;  // Style set information

// Collections
ScheduleItemList m_extraScheduleItemList;  // Extra item list
HotkeySetItemList m_hotkeySetList;         // Hotkey set list
```

### 5.2. Global Variables

#### Rules
- **Format**: `g_` + Hungarian Notation (currently retained)
- Prefix `g_` to distinguish global variables
- Hungarian Notation is still used for global variables

#### Examples
```cpp
namespace Global {
    extern DateTime g_stAppLaunchTime;           // Application launch time
    extern unsigned g_uiAppLaunchTimeCounter;    // Launch time counter
};

extern FlagManager g_sharedFlagManager;         // Shared flag manager
```

### 5.3. Local Variables

#### Rules
- **Format**: `camelCase`
- No prefix
- Variable names should clearly describe their purpose

#### Examples
```cpp
// Simple variables
int index = 0;
bool ret = true;
unsigned itemID = 100;
String message = "Hello";

// Temporary variables
ScheduleItem item;
HotkeySetData* newData = new HotkeySetData;
int temp = GetPairedID(IDTable::ActionName, actionID);

// Loop variables
for (int index = 0; index < GetItemNum(); index++) {
    const Item& item = GetItemAt(index);
    // ...
}

// Return values
bool result = ExecutePowerAction(actionType, message, errCode);
unsigned retNextID = ScheduleData::minItemID;
```

### 5.4. Function Parameters

#### Rules
- **Format**: `camelCase`
- No prefix (unless needed to avoid conflicts)
- Parameter names should clearly describe their meaning

#### Examples
```cpp
void SetItemID(unsigned itemID) noexcept;
void EnableItem(bool enabled) noexcept;
void SetTime(const ClockTime& time) noexcept;
DWORD Add(const ScheduleItem& item);
void Update(const ScheduleItem& item);
bool ExecutePowerAction(unsigned actionType, unsigned message, DWORD& errCode);
```

---

## 6. Constants

### Rules
- **Format**: `UPPER_CASE` with underscores
- All uppercase letters, words separated by underscores

### Examples
```cpp
// Preprocessor defines
#define DEF_GLBDATA_CATE_NONE        0x00
#define DEFAULT_DUMMYTEST            FALSE
#define DEFAULT_DEBUGMODE            FALSE

// Static constexpr in class
static constexpr int minItemID = 10000;
static constexpr int maxItemID = 19999;
static constexpr int defaultItemID = 0x00;
static constexpr COLORREF defaultBkgrdColor = Color::Pink;
```

---

## 7. Namespaces

### Rules
- **Format**: `PascalCase`
- Starts with an uppercase letter

### Examples
```cpp
namespace Global {
    // ...
}

namespace MapTable {
    // ...
}

namespace Language {
    // ...
}

namespace AppCore {
    // ...
}
```

---

## 8. Typedefs and Type Aliases

### Rules
- **Format**: `PascalCase` or `UPPER_CASE` depending on context
- Typically `PascalCase` for type aliases
- `UPPER_CASE` for legacy typedefs (for compatibility)

### Examples
```cpp
// Type aliases (modern C++)
using AppOptionID = ConfigData::AppOptionID;
using AppFlagID = FlagManager::AppFlagID;
using ScheduleItemList = typename std::vector<ScheduleItem>;
using HotkeySetItemList = typename std::vector<HotkeySetItem>;
using TokenList = typename std::vector<std::wstring>;

// Typedefs (legacy)
typedef enum eGRIDCOLSTYLE {
    COLSTYLE_FIXED = 0,
    COLSTYLE_CHECKBOX,
    COLSTYLE_NORMAL,
} GRIDCOLSTYLE;
```

---

## 9. Macros

### Rules
- **Format**: `UPPER_CASE` with underscores
- All uppercase letters, words separated by underscores

### Examples
```cpp
#define DEF_GLBDATA_CATE_NONE        0x00
#define DEFAULT_DUMMYTEST            FALSE
#define DEFAULT_DEBUGMODE            FALSE
#define _APP_CONSTANTS_INCLUDED
```

---

## 10. Special Prefixes and Suffixes

### 10.1. Prefix for Member Variables
- `m_` - Member variable

### 10.2. Prefix for Global Variables
- `g_` - Global variable

### 10.3. Prefix for Static Variables
- No special prefix, but usually placed in a namespace or class

### 10.4. Suffix for Classes
- `Utils` - Utility classes (StringUtils, ClockTimeUtils)
- `Data` - Data management classes (ScheduleData, HotkeySetData)
- `Item` - Item classes (ScheduleItem, HotkeySetItem)
- `Manager` - Manager classes (FlagManager)

---

## 11. Legacy Hungarian Notation Prefixes - Analysis and Meaning

**Note**: These prefixes have been converted to LLVM naming conventions during refactoring, but may still appear in some parts of the code (especially global variables).

### 11.1. Prefixes by Data Type

| Prefix | Meaning | Old Example | New Example |
|--------|---------|-------------|-------------|
| `n` | Number/Integer | `nItemID`, `nActionID` | `itemID`, `actionID` |
| `b` | Boolean | `bEnabled`, `bIsEmpty` | `isEnabled`, `isEmpty` |
| `p` | Pointer | `pItem`, `pData` | `item`, `data` |
| `dw` | DWORD (unsigned long) | `dwErrorCode`, `dwModifiers` | `errorCode`, `modifiers` |
| `lpsz` | Long Pointer to String | `lpszName`, `lpszMessage` | `name`, `message` |
| `str` | String | `strMessage`, `strFontName` | `messageContent`, `fontName` |
| `h` | Handle (Windows) | `hWnd`, `hInstance` | `wnd`, `instance` |
| `u` / `ui` | Unsigned/Unsigned Int | `uiFontSize`, `uValue` | `fontSize`, `value` |
| `by` | Byte | `byRepeatDays`, `byIconPos` | `repeatDays`, `iconPosition` |
| `st` | Struct | `stTime`, `stTimestamp` | `timeValue`, `timestamp` |

### 11.2. Prefixes by Purpose

| Prefix | Meaning | Old Example | New Example |
|--------|---------|-------------|-------------|
| `arr` | Array | `arrSchedExtraItemList` | `extraScheduleItemList` |
| `sch` | Schedule | `schDefaultItem`, `schItem` | `defaultItem`, `item` |
| `rps` | Repeat Set | `rpsRepeatSet` | `repeatSetInfo` |
| `rmd` | Reminder | `rmdCommonStyle`, `rmdItemList` | `commonStyleSet`, `reminderItemList` |
| `hks` | Hotkey Set | `hksItem`, `hksTemp` | `item`, `temp` |
| `pwr` | Power | `pwrItem`, `pwrTemp` | `item`, `temp` |

### 11.3. Prefixes by Scope

| Prefix | Meaning | Example |
|--------|---------|---------|
| `m_` | Member variable | `m_itemID`, `m_isEnabled` |
| `g_` | Global variable | `g_stAppLaunchTime`, `g_sharedFlagManager` |
| `s_` | Static variable | (rarely used in this project) |

### 11.4. Other Special Prefixes

| Prefix | Meaning | Old Example | New Example |
|--------|---------|-------------|-------------|
| `ull` | Unsigned Long Long | `ullBeginTimestamp` | `beginTimestamp` |
| `li` | LARGE_INTEGER | `liStartTime`, `liFrequency` | `startTime`, `frequency` |
| `ovi` | OSVERSIONINFOEX | `oviOSVersion` | `osVersion` |
| `rc` | RECT | `rcBtnRect` | `btnRect` |

### 11.5. Detailed Analysis of Prefixes

#### `n` - Number/Integer
- **Meaning**: Integer (int, unsigned int, long, etc.)
- **Usage**: For numeric variables like ID, index, count, size
- **Examples**: `nItemID`, `nIndex`, `nCount`, `nSize`
- **Conversion**: Remove `n` prefix, keep descriptive name: `itemID`, `index`, `count`, `size`

#### `b` - Boolean
- **Meaning**: Boolean value (true/false)
- **Usage**: For flags, on/off states, enable/disable
- **Examples**: `bEnabled`, `bIsEmpty`, `bRet`, `bResult`
- **Conversion**: Replace with `is`/`has`/`allow`/`enable` prefix: `isEnabled`, `isEmpty`, `ret`, `result`

#### `p` - Pointer
- **Meaning**: Pointer
- **Usage**: For pointer variables
- **Examples**: `pItem`, `pData`, `pNew`, `pLang`
- **Conversion**: Remove `p` prefix, keep descriptive name: `item`, `data`, `newData`, `lang`

#### `dw` - DWORD
- **Meaning**: DWORD (unsigned long, 32-bit)
- **Usage**: For DWORD values in Windows API
- **Examples**: `dwErrorCode`, `dwModifiers`, `dwVirtualKey`
- **Conversion**: Remove `dw` prefix, keep descriptive name: `errorCode`, `modifiers`, `virtualKey`

#### `lpsz` - Long Pointer to String
- **Meaning**: Pointer to null-terminated string (Windows API style)
- **Usage**: For C-style string pointers
- **Examples**: `lpszName`, `lpszMessage`, `lpszAction`
- **Conversion**: Remove `lpsz` prefix, keep descriptive name: `name`, `message`, `action`

#### `str` - String
- **Meaning**: String object
- **Usage**: For string objects (CString, String, std::string)
- **Examples**: `strMessage`, `strFontName`, `strKeyStrokes`
- **Conversion**: Remove `str` prefix, keep descriptive name: `messageContent`, `fontName`, `keyStrokes`

#### `h` - Handle
- **Meaning**: Windows handle (HWND, HINSTANCE, HANDLE, etc.)
- **Usage**: For Windows API handles
- **Examples**: `hWnd`, `hInstance`, `hToken`, `hUxTheme`
- **Conversion**: Remove `h` prefix, keep descriptive name: `wnd`, `instance`, `token`, `uxTheme`

#### `u` / `ui` - Unsigned
- **Meaning**: Unsigned integer
- **Usage**: For unsigned integers
- **Examples**: `uiFontSize`, `uiTimeout`, `uiIconID`, `uExitWinExFlags`
- **Conversion**: Remove `ui`/`u` prefix, keep descriptive name: `fontSize`, `timeout`, `iconID`, `exitWinExFlags`

#### `by` - Byte
- **Meaning**: Byte (8-bit)
- **Usage**: For byte values
- **Examples**: `byRepeatDays`, `byIconPos`, `byDisplayPos`
- **Conversion**: Remove `by` prefix, keep descriptive name: `repeatDays`, `iconPosition`, `displayPosition`

#### `st` - Struct
- **Meaning**: Struct object
- **Usage**: For struct variables
- **Examples**: `stTime`, `stTimestamp`, `stNextSnoozeTime`
- **Conversion**: Remove `st` prefix, add descriptive suffix: `timeValue`, `timestamp`, `nextSnoozeTime`

#### `arr` - Array
- **Meaning**: Array
- **Usage**: For array or vector variables
- **Examples**: `arrSchedExtraItemList`, `arrHotkeySetList`, `arrRmdItemList`
- **Conversion**: Remove `arr` prefix, keep descriptive name: `extraScheduleItemList`, `hotkeySetList`, `reminderItemList`

#### `sch` - Schedule
- **Meaning**: Schedule-related
- **Usage**: For schedule-related variables
- **Examples**: `schDefaultItem`, `schItem`, `schTemp`
- **Conversion**: Remove `sch` prefix, keep descriptive name: `defaultItem`, `item`, `temp`

#### `rps` - Repeat Set
- **Meaning**: Repeat set
- **Usage**: For repeat set-related variables
- **Examples**: `rpsRepeatSet`
- **Conversion**: Remove `rps` prefix, add descriptive suffix: `repeatSetInfo`

#### `rmd` - Reminder
- **Meaning**: Reminder-related
- **Usage**: For reminder-related variables
- **Examples**: `rmdCommonStyle`, `rmdItemList`
- **Conversion**: Remove `rmd` prefix, keep descriptive name: `commonStyleSet`, `reminderItemList`

#### `hks` - Hotkey Set
- **Meaning**: Hotkey set-related
- **Usage**: For hotkey set-related variables
- **Examples**: `hksItem`, `hksTemp`
- **Conversion**: Remove `hks` prefix, keep descriptive name: `item`, `temp`

#### `pwr` - Power
- **Meaning**: Power management-related
- **Usage**: For power management-related variables
- **Examples**: `pwrItem`, `pwrTemp`
- **Conversion**: Remove `pwr` prefix, keep descriptive name: `item`, `temp`

---

## 12. Special Naming Rules

### 12.1. Boolean Variables
- Use prefixes `is`, `has`, `allow`, `enable` to clarify meaning
- Examples: `isEnabled`, `isEmpty`, `hasValue`, `allowSnoozing`, `enableDarkMode`

### 12.2. Collections
- Use suffix `List` for collections
- Examples: `extraScheduleItemList`, `hotkeySetList`, `reminderItemList`

### 12.3. Temporary Variables
- Use short names like `temp`, `item`, `data`, `ret`, `result`
- Examples: `ScheduleItem temp`, `bool ret`, `HotkeySetData* newData`

### 12.4. Return Values
- Use `result`, `ret`, `retValue` for return values
- Examples: `bool result`, `int ret`, `unsigned retNextID`

### 12.5. Error Codes
- Use `errorCode`, `errCode` for error codes
- Examples: `DWORD errorCode`, `DWORD& errCode`

---

## 13. Summary of Conversion Rules

### From Hungarian Notation to LLVM Naming Conventions

| Type | Hungarian Notation | LLVM Convention |
|------|--------------------|-----------------|
| Member variable | `m_nItemID` | `m_itemID` |
| Local variable | `nIndex` | `index` |
| Boolean | `bEnabled` | `isEnabled` |
| Pointer | `pItem` | `item` |
| String | `strMessage` | `messageContent` |
| DWORD | `dwErrorCode` | `errorCode` |
| Handle | `hWnd` | `wnd` |
| Array | `arrItemList` | `itemList` |

---

## 14. Important Notes

1. **Do not change function or class names**: Only change variable names
2. **Retain global variables**: Global variables still use Hungarian Notation with `g_` prefix
3. **Consistency**: Ensure consistency throughout the project
4. **Readability**: Variable names must be readable and clearly describe their purpose
5. **Avoid abbreviations**: Avoid unclear abbreviations

---

## 15. Complete Examples

### Before refactoring (Hungarian Notation):
```cpp
class ScheduleItem {
private:
    unsigned m_nItemID;
    BOOL m_bEnabled;
    unsigned m_nActionID;
    ClockTime m_stTime;
    PwrRepeatSet m_rpsRepeatSet;
    
public:
    void SetItemID(unsigned nItemID) {
        m_nItemID = nItemID;
    }
    
    bool IsEmpty() const {
        bool bRet = (m_nItemID == 0);
        bRet &= (m_bEnabled == false);
        return bRet;
    }
};
```

### After refactoring (LLVM Naming Conventions):
```cpp
class ScheduleItem {
private:
    unsigned m_itemID;
    BOOL m_isEnabled;
    unsigned m_actionID;
    ClockTime m_timeValue;
    PwrRepeatSet m_repeatSetInfo;
    
public:
    void SetItemID(unsigned itemID) {
        m_itemID = itemID;
    }
    
    bool IsEmpty() const {
        bool ret = (m_itemID == 0);
        ret &= (m_isEnabled == false);
        return ret;
    }
};
```

---

**This document was automatically generated based on analysis of the PowerPlus3 codebase.**
**Last updated**: 2025-01-XX