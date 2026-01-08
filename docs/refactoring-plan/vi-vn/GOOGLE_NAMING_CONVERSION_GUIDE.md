# Google Naming Convention Conversion Guide - Phase 1

Tài liệu này mô tả quy tắc chuyển đổi từ naming convention hiện tại của project PowerPlus3 sang **Google C++ Style Guide** naming conventions.

**Mục đích**: Tài liệu tham chiếu cho Phase 1 của quá trình refactoring project.

---

## Tổng Quan Chuyển Đổi

| Loại | Hiện Tại | Google Style | Ghi Chú |
|------|----------|--------------|---------|
| **Classes** | `PascalCase` | `PascalCase` | ✅ Giữ nguyên |
| **Functions** | `PascalCase` | `camelCase` | ⚠️ Thay đổi |
| **Member Variables** | `m_` + `camelCase` | `camelCase_` (trailing underscore) | ⚠️ Thay đổi lớn |
| **Local Variables** | `camelCase` | `camelCase` | ✅ Giữ nguyên |
| **Function Parameters** | `camelCase` | `camelCase` | ✅ Giữ nguyên |
| **Constants** | `UPPER_CASE` | `kConstantName` | ⚠️ Thay đổi |
| **Namespaces** | `PascalCase` | `snake_case` | ⚠️ Thay đổi |
| **Enums** | `PascalCase` (class) / `UPPER_CASE` (values) | `PascalCase` (class) / `kEnumValue` (values) | ⚠️ Thay đổi một phần |
| **Macros** | `UPPER_CASE` | `UPPER_CASE` | ✅ Giữ nguyên |
| **Global Variables** | `g_` + Hungarian | `g_` + `camelCase_` | ⚠️ Thay đổi một phần |

---

## 1. Classes (Lớp)

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `PascalCase`
- **Google Style**: `PascalCase`
- **Hành động**: ✅ **Giữ nguyên** - Không cần thay đổi

### Ví Dụ
```cpp
// Trước và sau (giữ nguyên)
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

## 2. Functions (Hàm)

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `PascalCase`
- **Google Style**: `camelCase`
- **Hành động**: ⚠️ **Thay đổi** - Chuyển từ PascalCase sang camelCase

### Bảng Chuyển Đổi

| Hiện Tại | Google Style | Ghi Chú |
|----------|--------------|---------|
| `GetData` | `getData` | Getter functions |
| `GetAppOption` | `getAppOption` | Getter functions |
| `SetItemID` | `setItemId` | Setter functions |
| `IsEnabled` | `isEnabled` | Boolean check functions |
| `EnableItem` | `enableItem` | Action functions |
| `Copy` | `copy` | Action functions |
| `Compare` | `compare` | Action functions |
| `ExecutePowerAction` | `executePowerAction` | Action functions |

### Ví Dụ Hoàn Chỉnh

#### Trước (Hiện Tại)
```cpp
void GetData(CONFIGDATAINFO& data) const noexcept;
int GetAppOption(AppOptionID appOptionID) const noexcept;
void SetItemID(unsigned itemID) noexcept;
bool IsEnabled(void) const noexcept;
void EnableItem(bool enabled) noexcept;
void Copy(const ScheduleItem& other) noexcept;
bool Compare(const ScheduleItem& other) const noexcept;
```

#### Sau (Google Style)
```cpp
void getData(CONFIGDATAINFO& data) const noexcept;
int getAppOption(AppOptionID appOptionId) const noexcept;
void setItemId(unsigned itemId) noexcept;
bool isEnabled(void) const noexcept;
void enableItem(bool enabled) noexcept;
void copy(const ScheduleItem& other) noexcept;
bool compare(const ScheduleItem& other) const noexcept;
```

### Quy Tắc Chi Tiết
1. **Chuyển PascalCase → camelCase**: 
   - `GetData` → `getData`
   - `SetItemID` → `setItemId`
   - `IsEnabled` → `isEnabled`
2. **Giữ nguyên quy ước đặt tên**: 
   - Getter: `get` + tên
   - Setter: `set` + tên
   - Boolean check: `is`/`has` + tên
   - Action verbs: động từ ở dạng camelCase

**Lưu ý**: Tên hàm thay đổi, nhưng **parameters** giữ nguyên camelCase (xem mục 5).

---

## 3. Member Variables (Biến Thành Viên)

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `m_` + `camelCase`
- **Google Style**: `camelCase_` (trailing underscore)
- **Hành động**: ⚠️ **Thay đổi lớn** - Bỏ prefix `m_`, giữ nguyên camelCase và thêm trailing underscore

### Bảng Chuyển Đổi

| Hiện Tại | Google Style | Ghi Chú |
|----------|--------------|---------|
| `m_isEnabled` | `isEnabled_` | Boolean flags |
| `m_isRepeated` | `isRepeated_` | Boolean flags |
| `m_allowSnoozing` | `allowSnoozing_` | Boolean flags |
| `m_useCustomStyle` | `useCustomStyle_` | Boolean flags |
| `m_itemID` | `itemId_` | Numeric values (ID → Id) |
| `m_actionID` | `actionId_` | Numeric values (ID → Id) |
| `m_snoozeInterval` | `snoozeInterval_` | Numeric values |
| `m_fontSize` | `fontSize_` | Numeric values |
| `m_timeout` | `timeout_` | Numeric values |
| `m_messageContent` | `messageContent_` | String values |
| `m_fontName` | `fontName_` | String values |
| `m_timeValue` | `timeValue_` | Object references |
| `m_repeatSetInfo` | `repeatSetInfo_` | Object references |
| `m_msgStyleSetInfo` | `msgStyleSetInfo_` | Object references |
| `m_extraScheduleItemList` | `extraScheduleItemList_` | Collections |
| `m_hotkeySetList` | `hotkeySetList_` | Collections |

### Ví Dụ Hoàn Chỉnh

#### Trước (Hiện Tại)
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

#### Sau (Google Style)
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

### Quy Tắc Chi Tiết
1. **Bỏ prefix `m_`**: Loại bỏ hoàn toàn prefix `m_`
2. **Giữ nguyên camelCase**: Không chuyển sang snake_case
3. **Thêm trailing underscore**: Thêm `_` ở cuối tên biến
4. **Chuẩn hóa ID → Id**: Có thể chuyển `ID` thành `Id` để nhất quán (tùy chọn)
5. **Giữ nguyên ý nghĩa**: Không thay đổi ý nghĩa của tên biến

---

## 4. Local Variables (Biến Cục Bộ)

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `camelCase`
- **Google Style**: `camelCase`
- **Hành động**: ✅ **Giữ nguyên** - Không cần thay đổi

### Ví Dụ
```cpp
// Trước và sau (giữ nguyên)
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

**Lưu ý**: Local variables giữ nguyên camelCase, không cần thay đổi.

---

## 5. Function Parameters (Tham Số Hàm)

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `camelCase`
- **Google Style**: `camelCase`
- **Hành động**: ✅ **Giữ nguyên** - Không cần thay đổi

### Ví Dụ
```cpp
// Trước và sau (giữ nguyên)
void setItemId(unsigned itemId) noexcept;
void enableItem(bool enabled) noexcept;
void setTime(const ClockTime& time) noexcept;
DWORD add(const ScheduleItem& item);
void update(const ScheduleItem& item);
bool executePowerAction(unsigned actionType, unsigned message, DWORD& errCode);
int getAppOption(AppOptionID appOptionId) const noexcept;
```

**Lưu ý**: Function parameters giữ nguyên camelCase, không cần thay đổi.

---

## 6. Constants (Hằng Số)

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `UPPER_CASE` với underscore
- **Google Style**: `kConstantName` (k prefix + PascalCase)
- **Hành động**: ⚠️ **Thay đổi** - Chuyển từ UPPER_CASE sang kConstantName

### Bảng Chuyển Đổi

| Hiện Tại | Google Style | Ghi Chú |
|----------|--------------|---------|
| `DEF_GLBDATA_CATE_NONE` | `kDefGlbdataCateNone` | Preprocessor defines → constexpr |
| `DEFAULT_DUMMYTEST` | `kDefaultDummyTest` | Preprocessor defines → constexpr |
| `DEFAULT_DEBUGMODE` | `kDefaultDebugMode` | Preprocessor defines → constexpr |
| `minItemID` | `kMinItemId` | Static constexpr |
| `maxItemID` | `kMaxItemId` | Static constexpr |
| `defaultItemID` | `kDefaultItemId` | Static constexpr |
| `defaultBkgrdColor` | `kDefaultBkgrdColor` | Static constexpr |

### Ví Dụ Hoàn Chỉnh

#### Trước (Hiện Tại)
```cpp
// Preprocessor defines
#define DEF_GLBDATA_CATE_NONE        0x00
#define DEFAULT_DUMMYTEST            FALSE
#define DEFAULT_DEBUGMODE            FALSE

// Static constexpr trong class
static constexpr int minItemID = 10000;
static constexpr int maxItemID = 19999;
static constexpr int defaultItemID = 0x00;
static constexpr COLORREF defaultBkgrdColor = Color::Pink;
```

#### Sau (Google Style)
```cpp
// constexpr constants (thay thế preprocessor defines)
constexpr int kDefGlbdataCateNone = 0x00;
constexpr bool kDefaultDummyTest = false;
constexpr bool kDefaultDebugMode = false;

// Static constexpr trong class
static constexpr int kMinItemId = 10000;
static constexpr int kMaxItemId = 19999;
static constexpr int kDefaultItemId = 0x00;
static constexpr COLORREF kDefaultBkgrdColor = Color::Pink;
```

### Quy Tắc Chi Tiết
1. **Thêm prefix `k`**: Tất cả constants bắt đầu bằng `k`
2. **Chuyển UPPER_CASE → PascalCase**: 
   - `DEF_GLBDATA_CATE_NONE` → `kDefGlbdataCateNone`
   - `minItemID` → `kMinItemId`
3. **Ưu tiên constexpr**: Thay thế `#define` bằng `constexpr` khi có thể
4. **Giữ nguyên ý nghĩa**: Không thay đổi ý nghĩa của constant

**Lưu ý Đặc Biệt**: 
- Các macro guards (`#ifndef`, `#define`) vẫn giữ nguyên `UPPER_CASE`
- Các enum values có thể giữ nguyên hoặc chuyển sang `kEnumValue` format (xem mục 7)

---

## 7. Enums (Liệt Kê)

### Quy Tắc Chuyển Đổi
- **Enum Class**: `PascalCase` (giữ nguyên)
- **Enum Values**: 
  - **Hiện tại**: `PascalCase` (enum class) hoặc `UPPER_CASE` (enum thông thường)
  - **Google Style**: `kEnumValue` (k prefix + PascalCase) hoặc giữ nguyên `PascalCase` cho enum class
- **Hành động**: ⚠️ **Thay đổi một phần**

### Ví Dụ Hoàn Chỉnh

#### Trước (Hiện Tại)
```cpp
// Enum class với PascalCase
enum class AppOptionID : int {
    invalid = -1,
    leftMouseAction = 0,
    middleMouseAction,
    // ...
};

// Enum thông thường với UPPER_CASE
enum Flag {
    FLAG_OFF = 0x00,
    FLAG_ON  = 0x01,
};

enum Result {
    Success = 0x0000,
    Failure = 0xFFFF,
};
```

#### Sau (Google Style)
```cpp
// Enum class với k prefix (hoặc giữ nguyên PascalCase)
enum class AppOptionID : int {
    kInvalid = -1,
    kLeftMouseAction = 0,
    kMiddleMouseAction,
    // ...
};

// Enum thông thường - chuyển sang enum class nếu có thể
enum class Flag {
    kOff = 0x00,
    kOn  = 0x01,
};

enum class Result {
    kSuccess = 0x0000,
    kFailure = 0xFFFF,
};
```

### Quy Tắc Chi Tiết
1. **Enum Class**: Giữ nguyên `PascalCase` hoặc thêm `k` prefix cho values
2. **Enum thông thường**: Ưu tiên chuyển sang `enum class` và thêm `k` prefix
3. **Tùy chọn**: Có thể giữ nguyên `PascalCase` cho enum class values nếu team quyết định

**Lưu ý**: Google Style Guide cho phép cả hai cách cho enum values trong enum class:
- `PascalCase` (không có k prefix): `Invalid`, `LeftMouseAction`
- `kPascalCase` (có k prefix): `kInvalid`, `kLeftMouseAction`

Team có thể quyết định chọn một trong hai cách và áp dụng nhất quán.

---

## 8. Namespaces (Không Gian Tên)

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `PascalCase`
- **Google Style**: `snake_case`
- **Hành động**: ⚠️ **Thay đổi** - Chuyển từ PascalCase sang snake_case

### Bảng Chuyển Đổi

| Hiện Tại | Google Style | Ghi Chú |
|----------|--------------|---------|
| `Global` | `global` | snake_case |
| `MapTable` | `map_table` | snake_case |
| `Language` | `language` | snake_case |
| `AppCore` | `app_core` | snake_case |

### Ví Dụ Hoàn Chỉnh

#### Trước (Hiện Tại)
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

#### Sau (Google Style)
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

### Quy Tắc Chi Tiết
1. **Chuyển PascalCase → snake_case**: Tất cả namespaces chuyển sang snake_case
2. **Dùng underscore**: Namespaces dùng underscore để phân tách các từ
3. **Global variables trong namespace**: Cũng cần chuyển sang camelCase_ (xem mục 9)

---

## 9. Global Variables (Biến Toàn Cục)

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `g_` + Hungarian Notation
- **Google Style**: `g_` + `camelCase_` (trailing underscore)
- **Hành động**: ⚠️ **Thay đổi một phần** - Giữ prefix `g_`, bỏ Hungarian Notation, chuyển sang camelCase và thêm trailing underscore

### Bảng Chuyển Đổi

| Hiện Tại | Google Style | Ghi Chú |
|----------|--------------|---------|
| `g_stAppLaunchTime` | `g_appLaunchTime_` | DateTime object |
| `g_uiAppLaunchTimeCounter` | `g_appLaunchTimeCounter_` | Unsigned counter |
| `g_sharedFlagManager` | `g_sharedFlagManager_` | Manager object |

### Ví Dụ Hoàn Chỉnh

#### Trước (Hiện Tại)
```cpp
namespace Global {
    extern DateTime g_stAppLaunchTime;
    extern unsigned g_uiAppLaunchTimeCounter;
}

extern FlagManager g_sharedFlagManager;
```

#### Sau (Google Style)
```cpp
namespace global {
    extern DateTime g_appLaunchTime_;
    extern unsigned g_appLaunchTimeCounter_;
}

extern FlagManager g_sharedFlagManager_;
```

### Quy Tắc Chi Tiết
1. **Giữ prefix `g_`**: Giữ nguyên prefix `g_` để phân biệt global variables
2. **Bỏ Hungarian Notation**: Loại bỏ các prefix như `st_`, `ui_`, `n_`, `b_`, etc.
3. **Chuyển sang camelCase**: Chuyển tên biến sang camelCase (không phải snake_case)
4. **Thêm trailing underscore**: Thêm `_` ở cuối để nhất quán với member variables

---

## 10. Structs (Cấu Trúc)

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `PascalCase` hoặc `UPPER_CASE`
- **Google Style**: `PascalCase` (giống classes)
- **Hành động**: ⚠️ **Thay đổi một phần** - Chuyển UPPER_CASE sang PascalCase

### Bảng Chuyển Đổi

| Hiện Tại | Google Style | Ghi Chú |
|----------|--------------|---------|
| `CONFIGDATAINFO` | `ConfigDataInfo` | PascalCase |
| `RESOURCE_ID_MAP_ENTRY` | `ResourceIdMapEntry` | PascalCase |
| `SystemEvent` | `SystemEvent` | Giữ nguyên |
| `AppProfile` | `AppProfile` | Giữ nguyên |
| `JSON_ENTRY` | `JsonEntry` | PascalCase |

### Ví Dụ Hoàn Chỉnh

#### Trước (Hiện Tại)
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

#### Sau (Google Style)
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

### Quy Tắc Chi Tiết
1. **Chuyển UPPER_CASE → PascalCase**: Tất cả structs dùng PascalCase
2. **Member variables**: Cũng cần chuyển sang camelCase_ (xem mục 3)

---

## 11. Typedefs và Type Aliases

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `PascalCase` hoặc `UPPER_CASE`
- **Google Style**: `PascalCase` (giống classes)
- **Hành động**: ⚠️ **Thay đổi một phần** - Chuyển UPPER_CASE sang PascalCase

### Ví Dụ Hoàn Chỉnh

#### Trước (Hiện Tại)
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

#### Sau (Google Style)
```cpp
// Type aliases (modern C++)
using AppOptionID = ConfigData::AppOptionID;
using AppFlagID = FlagManager::AppFlagID;
using ScheduleItemList = typename std::vector<ScheduleItem>;
using HotkeySetItemList = typename std::vector<HotkeySetItem>;
using TokenList = typename std::vector<std::wstring>;

// Typedefs (legacy) - chuyển sang enum class nếu có thể
enum class GridColStyle {
    kFixed = 0,
    kCheckbox,
    kNormal,
};
```

### Quy Tắc Chi Tiết
1. **Type aliases**: Giữ nguyên PascalCase
2. **Legacy typedefs**: Ưu tiên chuyển sang enum class hoặc type alias

---

## 12. Macros (Macro)

### Quy Tắc Chuyển Đổi
- **Hiện tại**: `UPPER_CASE` với underscore
- **Google Style**: `UPPER_CASE` với underscore
- **Hành động**: ✅ **Giữ nguyên** - Không cần thay đổi

### Ví Dụ
```cpp
// Trước và sau (giữ nguyên)
#define DEF_GLBDATA_CATE_NONE        0x00
#define DEFAULT_DUMMYTEST            FALSE
#define DEFAULT_DEBUGMODE            FALSE
#define _APP_CONSTANTS_INCLUDED
```

**Lưu ý**: Mặc dù giữ nguyên, nhưng nên ưu tiên thay thế bằng `constexpr` khi có thể (xem mục 6).

---

## 13. Tóm Tắt Quy Tắc Chuyển Đổi

### Bảng Tóm Tắt Nhanh

| Loại | Hiện Tại | Google Style | Hành Động |
|------|----------|--------------|-----------|
| Classes | `PascalCase` | `PascalCase` | ✅ Giữ nguyên |
| Functions | `PascalCase` | `camelCase` | ⚠️ Thay đổi |
| Member Variables | `m_camelCase` | `camelCase_` | ⚠️ Thay đổi lớn |
| Local Variables | `camelCase` | `camelCase` | ✅ Giữ nguyên |
| Parameters | `camelCase` | `camelCase` | ✅ Giữ nguyên |
| Constants | `UPPER_CASE` | `kConstantName` | ⚠️ Thay đổi |
| Namespaces | `PascalCase` | `snake_case` | ⚠️ Thay đổi |
| Enums | `PascalCase`/`UPPER_CASE` | `PascalCase`/`kEnumValue` | ⚠️ Thay đổi một phần |
| Global Variables | `g_` + Hungarian | `g_` + `camelCase_` | ⚠️ Thay đổi một phần |
| Structs | `PascalCase`/`UPPER_CASE` | `PascalCase` | ⚠️ Thay đổi một phần |
| Macros | `UPPER_CASE` | `UPPER_CASE` | ✅ Giữ nguyên |

---

## 14. Ví Dụ Hoàn Chỉnh Trước và Sau

### Ví Dụ 1: Class với Member Variables và Functions

#### Trước (Hiện Tại)
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

#### Sau (Google Style)
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

### Ví Dụ 2: Function với Parameters và Local Variables

#### Trước (Hiện Tại)
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

#### Sau (Google Style)
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

### Ví Dụ 3: Global Variables và Namespaces

#### Trước (Hiện Tại)
```cpp
namespace Global {
    extern DateTime g_stAppLaunchTime;
    extern unsigned g_uiAppLaunchTimeCounter;
}

static inline const DateTime& GetAppLaunchTime(void) {
    return Global::g_stAppLaunchTime;
}
```

#### Sau (Google Style)
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

## 15. Quy Tắc Ưu Tiên và Lưu Ý

### 15.1. Thứ Tự Ưu Tiên Chuyển Đổi

1. **Phase 1.1**: Functions (`PascalCase` → `camelCase`)
2. **Phase 1.2**: Member variables (`m_camelCase` → `camelCase_`)
3. **Phase 1.3**: Constants (`UPPER_CASE` → `kConstantName`)
4. **Phase 1.4**: Namespaces (`PascalCase` → `snake_case`)
5. **Phase 1.5**: Global variables (`g_` + Hungarian → `g_` + `camelCase_`)
6. **Phase 1.6**: Structs và Enums (nếu cần)

### 15.2. Lưu Ý Quan Trọng

1. **Không thay đổi tên class**: Chỉ thay đổi tên hàm, biến, parameters, constants
2. **Consistency**: Đảm bảo tính nhất quán trong toàn bộ project
3. **Readability**: Tên biến phải dễ đọc và mô tả rõ ràng mục đích
4. **Avoid abbreviations**: Tránh viết tắt không rõ ràng
5. **Incremental refactoring**: Chuyển đổi từng file một, test kỹ trước khi tiếp tục
6. **Version control**: Commit từng phase riêng biệt để dễ rollback nếu cần
7. **Cải thiện tên**: Không bắt buộc phải giữ nguyên tên gốc - có thể tự do cải thiện tên để rõ ràng hơn (xem mục 15.4)

### 15.3. Cải Thiện Tên Trong Quá Trình Chuyển Đổi

**Nguyên tắc quan trọng**: Trong quá trình chuyển đổi naming convention, **không bắt buộc phải giữ nguyên tên gốc**. Bạn có thể tự do cải thiện tên functions, variables, và constants để làm cho code rõ ràng và dễ hiểu hơn, miễn là tuân thủ naming convention mới.

#### Ví Dụ Cải Thiện Tên

**Functions**:
- `GetData()` → `getData()` (chỉ chuyển case) 
- `GetData()` → `getConfigData()` (cải thiện để rõ ràng hơn - **khuyến khích**)
- `SetItemID()` → `setItemId()` (chỉ chuyển case)
- `SetItemID()` → `setScheduleItemId()` (cải thiện để rõ ràng hơn - **khuyến khích**)

**Member Variables**:
- `m_itemID` → `itemId_` (chỉ chuyển format)
- `m_itemID` → `scheduleItemId_` (cải thiện để rõ ràng hơn - **khuyến khích**)
- `m_timeValue` → `timeValue_` (chỉ chuyển format)
- `m_timeValue` → `scheduledTime_` (cải thiện để rõ ràng hơn - **khuyến khích**)

**Constants**:
- `DEF_GLBDATA_CATE_NONE` → `kDefGlbdataCateNone` (chỉ chuyển format)
- `DEF_GLBDATA_CATE_NONE` → `kGlobalDataCategoryNone` (cải thiện để rõ ràng hơn - **khuyến khích**)
- `minItemID` → `kMinItemId` (chỉ chuyển format)
- `minItemID` → `kMinScheduleItemId` (cải thiện để rõ ràng hơn - **khuyến khích**)

#### Nguyên Tắc Cải Thiện Tên

1. **Ưu tiên tính rõ ràng**: Tên mới nên mô tả rõ ràng hơn tên cũ
2. **Tránh viết tắt không rõ ràng**: Loại bỏ các viết tắt khó hiểu
3. **Nhất quán**: Đảm bảo tên mới nhất quán với phần còn lại của codebase
4. **Mô tả đúng mục đích**: Tên phải phản ánh chính xác chức năng hoặc ý nghĩa
5. **Document changes**: Ghi chú lại các thay đổi tên quan trọng trong commit messages

#### Khi Nào Nên Cải Thiện Tên

- ✅ **Nên cải thiện** khi:
  - Tên hiện tại không rõ ràng hoặc dễ gây nhầm lẫn
  - Tên có viết tắt không rõ ràng
  - Tên không mô tả đúng mục đích
  - Có thể làm cho code dễ đọc và dễ hiểu hơn

- ⚠️ **Cân nhắc** khi:
  - Tên đã được sử dụng ở nhiều nơi (cần update nhiều references)
  - Tên đã quen thuộc với team (có thể gây confusion)
  - Thay đổi tên không mang lại lợi ích rõ ràng

- ❌ **Không nên** khi:
  - Tên đã rất rõ ràng và phù hợp
  - Thay đổi sẽ gây breaking changes lớn mà không có lợi ích tương ứng

### 15.4. Các Trường Hợp Đặc Biệt

1. **Windows API**: Giữ nguyên naming của Windows API (HWND, DWORD, etc.)
2. **Third-party libraries**: Giữ nguyên naming của thư viện bên thứ ba
3. **Legacy code**: Có thể giữ nguyên một số phần legacy code nếu refactor quá tốn kém
4. **Macro guards**: Giữ nguyên `UPPER_CASE` cho include guards

---

## 16. Checklist Chuyển Đổi

### Checklist cho Mỗi File

- [ ] Functions: `PascalCase` → `camelCase` (có thể cải thiện tên để rõ ràng hơn)
- [ ] Member variables: `m_camelCase` → `camelCase_` (có thể cải thiện tên để rõ ràng hơn)
- [ ] Local variables: Giữ nguyên `camelCase` hoặc cải thiện nếu cần
- [ ] Function parameters: Giữ nguyên `camelCase` hoặc cải thiện nếu cần
- [ ] Constants: `UPPER_CASE` → `kConstantName` (có thể cải thiện tên để rõ ràng hơn)
- [ ] Namespaces: `PascalCase` → `snake_case` (nếu có)
- [ ] Global variables: `g_` + Hungarian → `g_` + `camelCase_` (có thể cải thiện tên để rõ ràng hơn)
- [ ] Structs: `UPPER_CASE` → `PascalCase` (nếu có)
- [ ] Enums: Chuyển đổi values nếu cần
- [ ] Compile và test: Đảm bảo code vẫn hoạt động đúng
- [ ] Review: Code review trước khi commit
- [ ] Document: Ghi chú các thay đổi tên quan trọng trong commit message

---

## 17. Tài Liệu Tham Khảo

- [Google C++ Style Guide - Naming](https://google.github.io/styleguide/cppguide.html#Naming)
- [NAMING_CONVENTIONS.md](./NAMING_CONVENTIONS.md) - Tài liệu naming convention hiện tại của project

---

**Tài liệu này được tạo cho Phase 1 của quá trình refactoring PowerPlus3 project.**
**Ngày tạo**: 2025-01-XX
**Phiên bản**: 1.0
