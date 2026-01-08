# Google Naming Convention Conversion Guide - Phase 1

This document describes the rules for converting from the current naming conventions of the PowerPlus3 project to the **Google C++ Style Guide** naming conventions.

**Purpose**: Reference document for Phase 1 of the project refactoring process.

---

## Conversion Overview

| Type                  | Current                | Google Style                  | Notes                  |
|-----------------------|------------------------|-------------------------------|------------------------|
| **Classes**           | `PascalCase`           | `PascalCase`                  | ✅ No change           |
| **Functions**         | `PascalCase`           | `camelCase`                   | ⚠️ Change             |
| **Member Variables**  | `m_` + `camelCase`     | `camelCase_` (trailing underscore) | ⚠️ Major change       |
| **Local Variables**   | `camelCase`            | `camelCase`                   | ✅ No change           |
| **Function Parameters**| `camelCase`            | `camelCase`                   | ✅ No change           |
| **Constants**         | `UPPER_CASE`           | `kConstantName`               | ⚠️ Change             |
| **Namespaces**        | `PascalCase`           | `snake_case`                  | ⚠️ Change             |
| **Enums**             | `PascalCase` (class) / `UPPER_CASE` (values) | `PascalCase` (class) / `kEnumValue` (values) | ⚠️ Partial change     |
| **Macros**            | `UPPER_CASE`           | `UPPER_CASE`                  | ✅ No change           |
| **Global Variables**  | `g_` + Hungarian       | `g_` + `camelCase_`           | ⚠️ Partial change     |

---

## 1. Classes

### Conversion Rules
- **Current**: `PascalCase`
- **Google Style**: `PascalCase`
- **Action**: ✅ **No change** - Keep as is

### Examples
```cpp
// Before and after (unchanged)
class ConfigData { };
class ScheduleItem { };
class HotkeySetItem { };
class PwrReminderItem { };
class StringUtils { };
class ClockTimeUtils { };
class PerformanceCounter { };
class FlagManager { };
```

---

## 2. Functions

### Conversion Rules
- **Current**: `PascalCase`
- **Google Style**: `camelCase`
- **Action**: ⚠️ **Change** - Convert from PascalCase to camelCase

### Conversion Table

| Current              | Google Style             | Notes                  |
|----------------------|--------------------------|------------------------|
| `GetData`            | `getData`                | Getter functions       |
| `GetAppOption`       | `getAppOption`           | Getter functions       |
| `SetItemID`          | `setItemId`              | Setter functions       |
| `IsEnabled`          | `isEnabled`              | Boolean check functions|
| `EnableItem`         | `enableItem`             | Action functions       |
| `Copy`               | `copy`                   | Action functions       |
| `Compare`            | `compare`                | Action functions       |
| `ExecutePowerAction` | `executePowerAction`     | Action functions       |

### Complete Examples

#### Before (Current)
```cpp
void GetData(CONFIGDATAINFO& data) const noexcept;
int GetAppOption(AppOptionID appOptionID) const noexcept;
void SetItemID(unsigned itemID) noexcept;
bool IsEnabled(void) const noexcept;
void EnableItem(bool enabled) noexcept;
void Copy(const ScheduleItem& other) noexcept;
bool Compare(const ScheduleItem& other) const noexcept;
```

#### After (Google Style)
```cpp
void getData(CONFIGDATAINFO& data) const noexcept;
int getAppOption(AppOptionID appOptionId) const noexcept;
void setItemId(unsigned itemId) noexcept;
bool isEnabled(void) const noexcept;
void enableItem(bool enabled) noexcept;
void copy(const ScheduleItem& other) noexcept;
bool compare(const ScheduleItem& other) const noexcept;
```

### Detailed Rules
1. **PascalCase → camelCase**:
   - `GetData` → `getData`
   - `SetItemID` → `setItemId`
   - `IsEnabled` → `isEnabled`
2. **Preserve naming conventions**:
   - Getter: `get` + name
   - Setter: `set` + name
   - Boolean check: `is`/`has` + name
   - Action verbs: verb in camelCase

**Note**: Function names change, but **parameters** remain camelCase (see section 5).

---

## 3. Member Variables

### Conversion Rules
- **Current**: `m_` + `camelCase`
- **Google Style**: `camelCase_` (trailing underscore)
- **Action**: ⚠️ **Major change** - Remove `m_` prefix, keep camelCase, add trailing underscore

### Conversion Table

| Current                      | Google Style                  | Notes                  |
|------------------------------|-------------------------------|------------------------|
| `m_isEnabled`                | `isEnabled_`                  | Boolean flags          |
| `m_isRepeated`               | `isRepeated_`                 | Boolean flags          |
| `m_allowSnoozing`            | `allowSnoozing_`              | Boolean flags          |
| `m_useCustomStyle`           | `useCustomStyle_`             | Boolean flags          |
| `m_itemID`                   | `itemId_`                     | Numeric values (ID → Id)|
| `m_actionID`                 | `actionId_`                   | Numeric values         |
| `m_snoozeInterval`           | `snoozeInterval_`             | Numeric values         |
| `m_fontSize`                 | `fontSize_`                   | Numeric values         |
| `m_timeout`                  | `timeout_`                    | Numeric values         |
| `m_messageContent`           | `messageContent_`             | String values          |
| `m_fontName`                 | `fontName_`                   | String values          |
| `m_timeValue`                | `timeValue_`                  | Object references      |
| `m_repeatSetInfo`            | `repeatSetInfo_`              | Object references      |
| `m_msgStyleSetInfo`          | `msgStyleSetInfo_`            | Object references      |
| `m_extraScheduleItemList`    | `extraScheduleItemList_`      | Collections            |
| `m_hotkeySetList`            | `hotkeySetList_`              | Collections            |

### Complete Examples

#### Before (Current)
```cpp
class ScheduleItem {
private:
    unsigned m_itemID;
    bool m_isEnabled;
    unsigned m_actionID;
    ClockTime m_timeValue;
    PwrRepeatSet m_repeatSetInfo;
    ScheduleItemList m_extraScheduleItemList;
};
```

#### After (Google Style)
```cpp
class ScheduleItem {
private:
    unsigned itemId_;
    bool isEnabled_;
    unsigned actionId_;
    ClockTime timeValue_;
    PwrRepeatSet repeatSetInfo_;
    ScheduleItemList extraScheduleItemList_;
};
```

### Detailed Rules
1. **Remove `m_` prefix**: Completely drop the `m_` prefix
2. **Keep camelCase**: Do not switch to snake_case
3. **Add trailing underscore**: Append `_` at the end
4. **Standardize ID → Id**: Optionally convert `ID` to `Id` for consistency
5. **Preserve meaning**: Do not alter the meaning of the name

---

## 4. Local Variables

### Conversion Rules
- **Current**: `camelCase`
- **Google Style**: `camelCase`
- **Action**: ✅ **No change** - Keep as is

### Examples
```cpp
// Before and after (unchanged)
int index = 0;
bool ret = true;
unsigned itemID = 100;
String message = "Hello";
ScheduleItem item;
HotkeySetData* newData = new HotkeySetData;
int temp = GetPairedID(IDTable::ActionName, actionID);
bool result = ExecutePowerAction(actionType, message, errCode);
unsigned retNextID = ScheduleData::minItemID;
```

**Note**: Local variables remain camelCase, no changes required.

---

## 5. Function Parameters

### Conversion Rules
- **Current**: `camelCase`
- **Google Style**: `camelCase`
- **Action**: ✅ **No change** - Keep as is

### Examples
```cpp
// Before and after (unchanged)
void setItemId(unsigned itemId) noexcept;
void enableItem(bool enabled) noexcept;
void setTime(const ClockTime& time) noexcept;
DWORD add(const ScheduleItem& item);
void update(const ScheduleItem& item);
bool executePowerAction(unsigned actionType, unsigned message, DWORD& errCode);
int getAppOption(AppOptionID appOptionId) const noexcept;
```

**Note**: Function parameters remain camelCase, no changes required.

---

## 6. Constants

### Conversion Rules
- **Current**: `UPPER_CASE` with underscores
- **Google Style**: `kConstantName` (k prefix + PascalCase)
- **Action**: ⚠️ **Change** - Convert from UPPER_CASE to kConstantName

### Conversion Table

| Current                     | Google Style                 | Notes                          |
|-----------------------------|------------------------------|--------------------------------|
| `DEF_GLBDATA_CATE_NONE`     | `kDefGlbdataCateNone`        | Preprocessor → constexpr       |
| `DEFAULT_DUMMYTEST`         | `kDefaultDummyTest`          | Preprocessor → constexpr       |
| `DEFAULT_DEBUGMODE`         | `kDefaultDebugMode`          | Preprocessor → constexpr       |
| `minItemID`                 | `kMinItemId`                 | Static constexpr               |
| `maxItemID`                 | `kMaxItemId`                 | Static constexpr               |
| `defaultItemID`             | `kDefaultItemId`             | Static constexpr               |
| `defaultBkgrdColor`         | `kDefaultBkgrdColor`          | Static constexpr               |

### Complete Examples

#### Before (Current)
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

#### After (Google Style)
```cpp
// constexpr constants (replace preprocessor defines)
constexpr int kDefGlbdataCateNone = 0x00;
constexpr bool kDefaultDummyTest = false;
constexpr bool kDefaultDebugMode = false;

// Static constexpr in class
static constexpr int kMinItemId = 10000;
static constexpr int kMaxItemId = 19999;
static constexpr int kDefaultItemId = 0x00;
static constexpr COLORREF kDefaultBkgrdColor = Color::Pink;
```

### Detailed Rules
1. **Add `k` prefix**: All constants start with `k`
2. **UPPER_CASE → PascalCase**:
   - `DEF_GLBDATA_CATE_NONE` → `kDefGlbdataCateNone`
   - `minItemID` → `kMinItemId`
3. **Prefer constexpr**: Replace `#define` with `constexpr` where possible
4. **Preserve meaning**: Do not change the meaning of the constant

**Special Note**:
- Macro guards (`#ifndef`, `#define`) remain `UPPER_CASE`
- Enum values may stay as is or switch to `kEnumValue` format (see section 7)

---

## 7. Enums

### Conversion Rules
- **Enum Class**: `PascalCase` (keep unchanged)
- **Enum Values**:
  - **Current**: `PascalCase` (enum class) or `UPPER_CASE` (regular enum)
  - **Google Style**: `kEnumValue` (k prefix + PascalCase) or keep `PascalCase` for enum class
- **Action**: ⚠️ **Partial change**

### Complete Examples

#### Before (Current)
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

#### After (Google Style)
```cpp
// Enum class with k prefix (or keep PascalCase)
enum class AppOptionID : int {
    kInvalid = -1,
    kLeftMouseAction = 0,
    kMiddleMouseAction,
    // ...
};

// Regular enum - prefer converting to enum class
enum class Flag {
    kOff = 0x00,
    kOn  = 0x01,
};

enum class Result {
    kSuccess = 0x0000,
    kFailure = 0xFFFF,
};
```

### Detailed Rules
1. **Enum Class**: Keep `PascalCase` or add `k` prefix to values
2. **Regular Enum**: Prefer converting to `enum class` and add `k` prefix
3. **Option**: Team may choose to keep `PascalCase` for enum class values without `k`

**Note**: Google Style Guide allows both approaches for enum values in enum class:
- `PascalCase` (no k prefix): `Invalid`, `LeftMouseAction`
- `kPascalCase` (with k prefix): `kInvalid`, `kLeftMouseAction`

The team can decide on one approach and apply it consistently.

---

## 8. Namespaces

### Conversion Rules
- **Current**: `PascalCase`
- **Google Style**: `snake_case`
- **Action**: ⚠️ **Change** - Convert from PascalCase to snake_case

### Conversion Table

| Current     | Google Style   | Notes       |
|-------------|----------------|-------------|
| `Global`    | `global`       | snake_case  |
| `MapTable`  | `map_table`    | snake_case  |
| `Language`  | `language`     | snake_case  |
| `AppCore`   | `app_core`     | snake_case  |

### Complete Examples

#### Before (Current)
```cpp
namespace Global {
    extern DateTime g_stAppLaunchTime;
    extern unsigned g_uiAppLaunchTimeCounter;
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

#### After (Google Style)
```cpp
namespace global {
    extern DateTime g_appLaunchTime_;
    extern unsigned g_appLaunchTimeCounter_;
}

namespace map_table {
    // ...
}

namespace language {
    // ...
}

namespace app_core {
    // ...
}
```

### Detailed Rules
1. **PascalCase → snake_case**: All namespaces converted to snake_case
2. **Use underscores**: Separate words with underscores
3. **Global variables inside namespaces**: Also convert to camelCase_ (see section 9)

---

## 9. Global Variables

### Conversion Rules
- **Current**: `g_` + Hungarian Notation
- **Google Style**: `g_` + `camelCase_` (trailing underscore)
- **Action**: ⚠️ **Partial change** - Keep `g_` prefix, remove Hungarian Notation, convert to camelCase and add trailing underscore

### Conversion Table

| Current                       | Google Style                    | Notes                |
|-------------------------------|---------------------------------|----------------------|
| `g_stAppLaunchTime`           | `g_appLaunchTime_`              | DateTime object      |
| `g_uiAppLaunchTimeCounter`    | `g_appLaunchTimeCounter_`       | Unsigned counter     |
| `g_sharedFlagManager`         | `g_sharedFlagManager_`          | Manager object       |

### Complete Examples

#### Before (Current)
```cpp
namespace Global {
    extern DateTime g_stAppLaunchTime;
    extern unsigned g_uiAppLaunchTimeCounter;
}

extern FlagManager g_sharedFlagManager;
```

#### After (Google Style)
```cpp
namespace global {
    extern DateTime g_appLaunchTime_;
    extern unsigned g_appLaunchTimeCounter_;
}

extern FlagManager g_sharedFlagManager_;
```

### Detailed Rules
1. **Keep `g_` prefix**: Retain `g_` to identify global variables
2. **Remove Hungarian Notation**: Drop prefixes like `st_`, `ui_`, etc.
3. **Convert to camelCase**: Use camelCase (not snake_case)
4. **Add trailing underscore**: Append `_` for consistency with member variables

---

## 10. Structs

### Conversion Rules
- **Current**: `PascalCase` or `UPPER_CASE`
- **Google Style**: `PascalCase` (same as classes)
- **Action**: ⚠️ **Partial change** - Convert UPPER_CASE to PascalCase

### Conversion Table

| Current                   | Google Style              | Notes       |
|---------------------------|---------------------------|-------------|
| `CONFIGDATAINFO`          | `ConfigDataInfo`          | PascalCase  |
| `RESOURCE_ID_MAP_ENTRY`   | `ResourceIdMapEntry`      | PascalCase  |
| `SystemEvent`             | `SystemEvent`             | Unchanged   |
| `AppProfile`              | `AppProfile`              | Unchanged   |
| `JSON_ENTRY`              | `JsonEntry`               | PascalCase  |

### Complete Examples

#### Before (Current)
```cpp
struct CONFIGDATAINFO {
    // ...
};

struct RESOURCE_ID_MAP_ENTRY {
    // ...
};

struct SystemEvent {
    // ...
};
```

#### After (Google Style)
```cpp
struct ConfigDataInfo {
    // ...
};

struct ResourceIdMapEntry {
    // ...
};

struct SystemEvent {
    // ...
};
```

### Detailed Rules
1. **UPPER_CASE → PascalCase**: All structs use PascalCase
2. **Member variables**: Also convert to camelCase_ (see section 3)

---

## 11. Typedefs and Type Aliases

### Conversion Rules
- **Current**: `PascalCase` or `UPPER_CASE`
- **Google Style**: `PascalCase` (same as classes)
- **Action**: ⚠️ **Partial change** - Convert UPPER_CASE to PascalCase

### Complete Examples

#### Before (Current)
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

#### After (Google Style)
```cpp
// Type aliases (modern C++)
using AppOptionID = ConfigData::AppOptionID;
using AppFlagID = FlagManager::AppFlagID;
using ScheduleItemList = typename std::vector<ScheduleItem>;
using HotkeySetItemList = typename std::vector<HotkeySetItem>;
using TokenList = typename std::vector<std::wstring>;

// Legacy typedefs - prefer converting to enum class
enum class GridColStyle {
    kFixed = 0,
    kCheckbox,
    kNormal,
};
```

### Detailed Rules
1. **Type aliases**: Keep PascalCase
2. **Legacy typedefs**: Prefer converting to enum class or type alias

---

## 12. Macros

### Conversion Rules
- **Current**: `UPPER_CASE` with underscores
- **Google Style**: `UPPER_CASE` with underscores
- **Action**: ✅ **No change** - Keep as is

### Examples
```cpp
// Before and after (unchanged)
#define DEF_GLBDATA_CATE_NONE        0x00
#define DEFAULT_DUMMYTEST            FALSE
#define DEFAULT_DEBUGMODE            FALSE
#define _APP_CONSTANTS_INCLUDED
```

**Note**: Although unchanged, prefer replacing with `constexpr` where possible (see section 6).

---

## 13. Summary of Conversion Rules

### Quick Summary Table

| Type                  | Current                  | Google Style                  | Action                |
|-----------------------|--------------------------|-------------------------------|-----------------------|
| Classes               | `PascalCase`             | `PascalCase`                  | ✅ No change           |
| Functions             | `PascalCase`             | `camelCase`                   | ⚠️ Change             |
| Member Variables      | `m_camelCase`            | `camelCase_`                  | ⚠️ Major change       |
| Local Variables       | `camelCase`              | `camelCase`                   | ✅ No change           |
| Parameters            | `camelCase`              | `camelCase`                   | ✅ No change           |
| Constants             | `UPPER_CASE`             | `kConstantName`               | ⚠️ Change             |
| Namespaces            | `PascalCase`             | `snake_case`                  | ⚠️ Change             |
| Enums                 | `PascalCase`/`UPPER_CASE`| `PascalCase`/`kEnumValue`     | ⚠️ Partial change     |
| Global Variables      | `g_` + Hungarian         | `g_` + `camelCase_`           | ⚠️ Partial change     |
| Structs               | `PascalCase`/`UPPER_CASE`| `PascalCase`                  | ⚠️ Partial change     |
| Macros                | `UPPER_CASE`             | `UPPER_CASE`                  | ✅ No change           |

---

## 14. Complete Before-and-After Examples

### Example 1: Class with Member Variables and Functions

#### Before (Current)
```cpp
class ScheduleItem {
private:
    unsigned m_itemID;
    bool m_isEnabled;
    unsigned m_actionID;
    ClockTime m_timeValue;
    PwrRepeatSet m_repeatSetInfo;
    ScheduleItemList m_extraScheduleItemList;
    
public:
    void SetItemID(unsigned itemID) noexcept {
        m_itemID = itemID;
    }
    
    bool IsEmpty() const {
        bool ret = (m_itemID == 0);
        ret &= (m_isEnabled == false);
        return ret;
    }
};
```

#### After (Google Style)
```cpp
class ScheduleItem {
private:
    unsigned itemId_;
    bool isEnabled_;
    unsigned actionId_;
    ClockTime timeValue_;
    PwrRepeatSet repeatSetInfo_;
    ScheduleItemList extraScheduleItemList_;
    
public:
    void setItemId(unsigned itemId) noexcept {
        itemId_ = itemId;
    }
    
    bool isEmpty() const {
        bool ret = (itemId_ == 0);
        ret &= (isEnabled_ == false);
        return ret;
    }
};
```

### Example 2: Function with Parameters and Local Variables

#### Before (Current)
```cpp
bool ExecutePowerAction(unsigned actionType, unsigned message, DWORD& errCode) {
    bool result = false;
    unsigned itemID = GetCurrentItemID();
    
    if (actionType == PowerAction::Shutdown) {
        result = DoShutdown(itemID, message, errCode);
    }
    
    return result;
}
```

#### After (Google Style)
```cpp
bool executePowerAction(unsigned actionType, unsigned message, DWORD& errCode) {
    bool result = false;
    unsigned itemId = getCurrentItemId();
    
    if (actionType == PowerAction::kShutdown) {
        result = doShutdown(itemId, message, errCode);
    }
    
    return result;
}
```

### Example 3: Global Variables and Namespaces

#### Before (Current)
```cpp
namespace Global {
    extern DateTime g_stAppLaunchTime;
    extern unsigned g_uiAppLaunchTimeCounter;
}

static inline const DateTime& GetAppLaunchTime(void) {
    return Global::g_stAppLaunchTime;
}
```

#### After (Google Style)
```cpp
namespace global {
    extern DateTime g_appLaunchTime_;
    extern unsigned g_appLaunchTimeCounter_;
}

static inline const DateTime& getAppLaunchTime(void) {
    return global::g_appLaunchTime_;
}
```

---

## 15. Priorities and Important Notes

### 15.1. Conversion Priority Order

1. **Phase 1.1**: Functions (`PascalCase` → `camelCase`)
2. **Phase 1.2**: Member variables (`m_camelCase` → `camelCase_`)
3. **Phase 1.3**: Constants (`UPPER_CASE` → `kConstantName`)
4. **Phase 1.4**: Namespaces (`PascalCase` → `snake_case`)
5. **Phase 1.5**: Global variables (`g_` + Hungarian → `g_` + `camelCase_`)
6. **Phase 1.6**: Structs and Enums (if needed)

### 15.2. Important Notes

1. **Do not change class names**: Only change function, variable, parameter, and constant names
2. **Consistency**: Ensure consistency across the entire project
3. **Readability**: Names must be readable and clearly describe their purpose
4. **Avoid abbreviations**: Avoid unclear abbreviations
5. **Incremental refactoring**: Convert one file at a time, test thoroughly before continuing
6. **Version control**: Commit each phase separately for easy rollback if needed
7. **Name improvement**: You are not required to keep original names — feel free to improve names for clarity (see section 15.4)

### 15.3. Improving Names During Conversion

**Important principle**: During the naming convention conversion, **you are not required to keep the original names**. You may freely improve function, variable, and constant names to make the code clearer and more understandable, as long as you follow the new naming convention.

#### Name Improvement Examples

**Functions**:
- `GetData()` → `getData()` (only case change)
- `GetData()` → `getConfigData()` (improved for clarity — **encouraged**)
- `SetItemID()` → `setItemId()` (only case change)
- `SetItemID()` → `setScheduleItemId()` (improved for clarity — **encouraged**)

**Member Variables**:
- `m_itemID` → `itemId_` (only format change)
- `m_itemID` → `scheduleItemId_` (improved for clarity — **encouraged**)
- `m_timeValue` → `timeValue_` (only format change)
- `m_timeValue` → `scheduledTime_` (improved for clarity — **encouraged**)

**Constants**:
- `DEF_GLBDATA_CATE_NONE` → `kDefGlbdataCateNone` (only format change)
- `DEF_GLBDATA_CATE_NONE` → `kGlobalDataCategoryNone` (improved for clarity — **encouraged**)
- `minItemID` → `kMinItemId` (only format change)
- `minItemID` → `kMinScheduleItemId` (improved for clarity — **encouraged**)

#### Name Improvement Principles

1. **Prioritize clarity**: New name should be clearer than the old one
2. **Avoid unclear abbreviations**: Remove ambiguous abbreviations
3. **Consistency**: Ensure new names are consistent with the rest of the codebase
4. **Accurate description**: Name must accurately reflect purpose or meaning
5. **Document changes**: Note significant name changes in commit messages

#### When to Improve Names

- ✅ **Should improve** when:
  - Current name is unclear or confusing
  - Name contains unclear abbreviations
  - Name does not accurately describe purpose
  - Improvement makes code easier to read and understand

- ⚠️ **Consider carefully** when:
  - Name is used in many places (requires updating many references)
  - Name is already familiar to the team (may cause confusion)
  - Change provides no clear benefit

- ❌ **Should not** when:
  - Name is already very clear and appropriate
  - Change would cause large breaking changes with no corresponding benefit

### 15.4. Special Cases

1. **Windows API**: Keep Windows API naming (HWND, DWORD, etc.)
2. **Third-party libraries**: Keep naming of external libraries
3. **Legacy code**: May retain some legacy code if refactoring is too costly
4. **Macro guards**: Keep `UPPER_CASE` for include guards

---

## 16. Conversion Checklist

### Checklist for Each File

- [ ] Functions: `PascalCase` → `camelCase` (may improve names for clarity)
- [ ] Member variables: `m_camelCase` → `camelCase_` (may improve names for clarity)
- [ ] Local variables: Keep `camelCase` or improve if needed
- [ ] Function parameters: Keep `camelCase` or improve if needed
- [ ] Constants: `UPPER_CASE` → `kConstantName` (may improve names for clarity)
- [ ] Namespaces: `PascalCase` → `snake_case` (if present)
- [ ] Global variables: `g_` + Hungarian → `g_` + `camelCase_` (may improve names for clarity)
- [ ] Structs: `UPPER_CASE` → `PascalCase` (if present)
- [ ] Enums: Convert values if needed
- [ ] Compile and test: Ensure code still works correctly
- [ ] Review: Code review before commit
- [ ] Document: Note significant name changes in commit message

---

## 17. Reference Documents

- [Google C++ Style Guide - Naming](https://google.github.io/styleguide/cppguide.html#Naming)
- [NAMING_CONVENTIONS.md](./NAMING_CONVENTIONS.md) - Current project naming conventions document

---

**This document was created for Phase 1 of the PowerPlus3 project refactoring.**
**Created**: 2025-01-XX
**Version**: 1.0