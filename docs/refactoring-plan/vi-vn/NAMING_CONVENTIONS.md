# Naming Conventions - PowerPlus3 Project

Tài liệu này mô tả các quy tắc naming convention được sử dụng trong project PowerPlus3 (trừ phần Components).

---

## 1. Classes (Lớp)

### Quy tắc
- **Format**: `PascalCase`
- Bắt đầu bằng chữ cái in hoa, mỗi từ tiếp theo cũng bắt đầu bằng chữ cái in hoa
- Không sử dụng prefix hoặc suffix đặc biệt

### Ví dụ
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

### Lưu ý
- Các class utility thường kết thúc bằng `Utils` (StringUtils, ClockTimeUtils, DateTimeUtils)
- Các class quản lý dữ liệu thường kết thúc bằng `Data` (ScheduleData, HotkeySetData, PwrReminderData)
- Các class quản lý item thường kết thúc bằng `Item` (ScheduleItem, HotkeySetItem, PwrReminderItem)

---

## 2. Structs (Cấu trúc)

### Quy tắc
- **Format**: `PascalCase` hoặc `UPPER_CASE`
- Có thể sử dụng cả hai format tùy theo context

### Ví dụ
```cpp
// PascalCase
struct CONFIGDATAINFO
struct RESOURCE_ID_MAP_ENTRY
struct SystemEvent
struct AppProfile

// UPPER_CASE (thường dùng cho các struct định nghĩa dữ liệu)
struct JSON_ENTRY
```

### Lưu ý
- Các struct định nghĩa thông tin cấu hình thường dùng `UPPER_CASE` (CONFIGDATAINFO)
- Các struct định nghĩa entry/map thường dùng `UPPER_CASE` với underscore (RESOURCE_ID_MAP_ENTRY)
- Các struct class-like thường dùng `PascalCase` (SystemEvent)

---

## 3. Enums (Liệt kê)

### Quy tắc
- **Format**: `PascalCase` hoặc `UPPER_CASE`
- Enum values thường dùng `PascalCase` hoặc `UPPER_CASE` tùy theo enum type

### Ví dụ
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

### Lưu ý
- Enum class thường dùng `PascalCase` cho cả tên enum và values
- Enum thông thường thường dùng `UPPER_CASE` cho values

---

## 4. Functions (Hàm)

### Quy tắc
- **Format**: `PascalCase`
- Bắt đầu bằng chữ cái in hoa
- Tên hàm nên mô tả rõ ràng chức năng

### Ví dụ
```cpp
void GetData(CONFIGDATAINFO& data) const noexcept;
int GetAppOption(AppOptionID appOptionID) const noexcept;
void SetItemID(unsigned itemID) noexcept;
bool IsEnabled(void) const noexcept;
void EnableItem(bool enabled) noexcept;
void Copy(const ScheduleItem& other) noexcept;
bool Compare(const ScheduleItem& other) const noexcept;
```

### Quy ước đặt tên
- **Getter**: Bắt đầu bằng `Get` (GetItemID, GetTime, GetAction)
- **Setter**: Bắt đầu bằng `Set` (SetItemID, SetTime, SetAction)
- **Boolean check**: Bắt đầu bằng `Is` hoặc `Has` (IsEnabled, IsEmpty, HasValue)
- **Enable/Disable**: Bắt đầu bằng `Enable` hoặc `Disable` (EnableItem, EnableRepeat)
- **Action verbs**: Sử dụng động từ rõ ràng (Copy, Compare, Add, Update, Remove, Delete)

---

## 5. Variables (Biến)

### 5.1. Member Variables (Biến thành viên)

#### Quy tắc
- **Format**: `m_` + `camelCase`
- Prefix `m_` để phân biệt member variable
- Sau prefix là tên biến theo camelCase

#### Ví dụ
```cpp
// Boolean flags
bool m_isEnabled;          // Trạng thái enable/disable
bool m_isRepeated;         // Trạng thái lặp lại
bool m_allowSnoozing;      // Cho phép snooze
bool m_useCustomStyle;     // Sử dụng style tùy chỉnh

// Numeric values
unsigned m_itemID;         // ID của item
int m_actionID;            // ID của action
int m_snoozeInterval;      // Khoảng thời gian snooze
unsigned m_fontSize;       // Kích thước font
unsigned m_timeout;        // Thời gian timeout

// String values
String m_messageContent;   // Nội dung message
String m_fontName;         // Tên font

// Object references
ClockTime m_timeValue;     // Giá trị thời gian
PwrRepeatSet m_repeatSetInfo;  // Thông tin repeat set
RmdMsgStyleSet m_msgStyleSetInfo;  // Thông tin style set

// Collections
ScheduleItemList m_extraScheduleItemList;  // Danh sách item bổ sung
HotkeySetItemList m_hotkeySetList;         // Danh sách hotkey set
```

### 5.2. Global Variables (Biến toàn cục)

#### Quy tắc
- **Format**: `g_` + Hungarian Notation (đang được giữ nguyên)
- Prefix `g_` để phân biệt global variable
- Vẫn sử dụng Hungarian Notation cho global variables

#### Ví dụ
```cpp
namespace Global {
    extern DateTime g_stAppLaunchTime;           // Thời gian khởi động app
    extern unsigned g_uiAppLaunchTimeCounter;    // Bộ đếm thời gian khởi động
};

extern FlagManager g_sharedFlagManager;         // Flag manager dùng chung
```

### 5.3. Local Variables (Biến cục bộ)

#### Quy tắc
- **Format**: `camelCase`
- Không sử dụng prefix
- Tên biến nên mô tả rõ ràng mục đích sử dụng

#### Ví dụ
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

### 5.4. Function Parameters (Tham số hàm)

#### Quy tắc
- **Format**: `camelCase`
- Không sử dụng prefix (trừ khi cần thiết để tránh conflict)
- Tên tham số nên mô tả rõ ràng ý nghĩa

#### Ví dụ
```cpp
void SetItemID(unsigned itemID) noexcept;
void EnableItem(bool enabled) noexcept;
void SetTime(const ClockTime& time) noexcept;
DWORD Add(const ScheduleItem& item);
void Update(const ScheduleItem& item);
bool ExecutePowerAction(unsigned actionType, unsigned message, DWORD& errCode);
```

---

## 6. Constants (Hằng số)

### Quy tắc
- **Format**: `UPPER_CASE` với underscore
- Tất cả chữ cái in hoa, các từ cách nhau bởi underscore

### Ví dụ
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

---

## 7. Namespaces (Không gian tên)

### Quy tắc
- **Format**: `PascalCase`
- Bắt đầu bằng chữ cái in hoa

### Ví dụ
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

## 8. Typedefs và Type Aliases

### Quy tắc
- **Format**: `PascalCase` hoặc `UPPER_CASE` tùy theo context
- Thường sử dụng `PascalCase` cho type aliases
- Sử dụng `UPPER_CASE` cho typedefs cũ (để tương thích)

### Ví dụ
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

## 9. Macros (Macro)

### Quy tắc
- **Format**: `UPPER_CASE` với underscore
- Tất cả chữ cái in hoa, các từ cách nhau bởi underscore

### Ví dụ
```cpp
#define DEF_GLBDATA_CATE_NONE        0x00
#define DEFAULT_DUMMYTEST            FALSE
#define DEFAULT_DEBUGMODE            FALSE
#define _APP_CONSTANTS_INCLUDED
```

---

## 10. Prefix và Suffix Đặc Biệt

### 10.1. Prefix cho Member Variables
- `m_` - Member variable (biến thành viên)

### 10.2. Prefix cho Global Variables
- `g_` - Global variable (biến toàn cục)

### 10.3. Prefix cho Static Variables
- Không có prefix đặc biệt, nhưng thường được đặt trong namespace hoặc class

### 10.4. Suffix cho Classes
- `Utils` - Utility classes (StringUtils, ClockTimeUtils)
- `Data` - Data management classes (ScheduleData, HotkeySetData)
- `Item` - Item classes (ScheduleItem, HotkeySetItem)
- `Manager` - Manager classes (FlagManager)

---

## 11. Hungarian Notation Prefixes (Cũ) - Phân Tích và Ý Nghĩa

**Lưu ý**: Các prefix này đã được chuyển đổi sang LLVM naming conventions trong quá trình refactoring, nhưng vẫn có thể xuất hiện trong một số phần của code (đặc biệt là global variables).

### 11.1. Prefix theo Kiểu Dữ Liệu

| Prefix | Ý nghĩa | Ví dụ cũ | Ví dụ mới |
|--------|---------|----------|-----------|
| `n` | Number/Integer (số nguyên) | `nItemID`, `nActionID` | `itemID`, `actionID` |
| `b` | Boolean (true/false) | `bEnabled`, `bIsEmpty` | `isEnabled`, `isEmpty` |
| `p` | Pointer (con trỏ) | `pItem`, `pData` | `item`, `data` |
| `dw` | DWORD (unsigned long) | `dwErrorCode`, `dwModifiers` | `errorCode`, `modifiers` |
| `lpsz` | Long Pointer to String (con trỏ chuỗi) | `lpszName`, `lpszMessage` | `name`, `message` |
| `str` | String (chuỗi) | `strMessage`, `strFontName` | `messageContent`, `fontName` |
| `h` | Handle (handle Windows) | `hWnd`, `hInstance` | `wnd`, `instance` |
| `u` / `ui` | Unsigned/Unsigned Int | `uiFontSize`, `uValue` | `fontSize`, `value` |
| `by` | Byte | `byRepeatDays`, `byIconPos` | `repeatDays`, `iconPosition` |
| `st` | Struct | `stTime`, `stTimestamp` | `timeValue`, `timestamp` |

### 11.2. Prefix theo Mục Đích Sử Dụng

| Prefix | Ý nghĩa | Ví dụ cũ | Ví dụ mới |
|--------|---------|----------|-----------|
| `arr` | Array (mảng) | `arrSchedExtraItemList` | `extraScheduleItemList` |
| `sch` | Schedule (lịch trình) | `schDefaultItem`, `schItem` | `defaultItem`, `item` |
| `rps` | Repeat Set (bộ lặp) | `rpsRepeatSet` | `repeatSetInfo` |
| `rmd` | Reminder (nhắc nhở) | `rmdCommonStyle`, `rmdItemList` | `commonStyleSet`, `reminderItemList` |
| `hks` | Hotkey Set (bộ phím tắt) | `hksItem`, `hksTemp` | `item`, `temp` |
| `pwr` | Power (năng lượng) | `pwrItem`, `pwrTemp` | `item`, `temp` |

### 11.3. Prefix theo Phạm Vi

| Prefix | Ý nghĩa | Ví dụ |
|--------|---------|-------|
| `m_` | Member variable | `m_itemID`, `m_isEnabled` |
| `g_` | Global variable | `g_stAppLaunchTime`, `g_sharedFlagManager` |
| `s_` | Static variable | (ít sử dụng trong project này) |

### 11.4. Prefix Đặc Biệt Khác

| Prefix | Ý nghĩa | Ví dụ cũ | Ví dụ mới |
|--------|---------|----------|-----------|
| `ull` | Unsigned Long Long | `ullBeginTimestamp` | `beginTimestamp` |
| `li` | LARGE_INTEGER | `liStartTime`, `liFrequency` | `startTime`, `frequency` |
| `ovi` | OSVERSIONINFOEX | `oviOSVersion` | `osVersion` |
| `rc` | RECT | `rcBtnRect` | `btnRect` |

### 11.5. Phân Tích Chi Tiết Các Prefix

#### `n` - Number/Integer
- **Ý nghĩa**: Chỉ số nguyên (int, unsigned int, long, etc.)
- **Sử dụng**: Cho các biến số như ID, index, count, size
- **Ví dụ**: `nItemID`, `nIndex`, `nCount`, `nSize`
- **Chuyển đổi**: Bỏ prefix `n`, giữ lại tên mô tả: `itemID`, `index`, `count`, `size`

#### `b` - Boolean
- **Ý nghĩa**: Giá trị boolean (true/false)
- **Sử dụng**: Cho các flag, trạng thái on/off, enable/disable
- **Ví dụ**: `bEnabled`, `bIsEmpty`, `bRet`, `bResult`
- **Chuyển đổi**: Thay bằng `is`/`has`/`allow`/`enable` prefix: `isEnabled`, `isEmpty`, `ret`, `result`

#### `p` - Pointer
- **Ý nghĩa**: Con trỏ (pointer)
- **Sử dụng**: Cho các biến con trỏ
- **Ví dụ**: `pItem`, `pData`, `pNew`, `pLang`
- **Chuyển đổi**: Bỏ prefix `p`, giữ lại tên mô tả: `item`, `data`, `newData`, `lang`

#### `dw` - DWORD
- **Ý nghĩa**: DWORD (unsigned long, 32-bit)
- **Sử dụng**: Cho các giá trị DWORD trong Windows API
- **Ví dụ**: `dwErrorCode`, `dwModifiers`, `dwVirtualKey`
- **Chuyển đổi**: Bỏ prefix `dw`, giữ lại tên mô tả: `errorCode`, `modifiers`, `virtualKey`

#### `lpsz` - Long Pointer to String
- **Ý nghĩa**: Con trỏ đến chuỗi null-terminated (Windows API style)
- **Sử dụng**: Cho các con trỏ chuỗi C-style
- **Ví dụ**: `lpszName`, `lpszMessage`, `lpszAction`
- **Chuyển đổi**: Bỏ prefix `lpsz`, giữ lại tên mô tả: `name`, `message`, `action`

#### `str` - String
- **Ý nghĩa**: Chuỗi (string object)
- **Sử dụng**: Cho các biến string object (CString, String, std::string)
- **Ví dụ**: `strMessage`, `strFontName`, `strKeyStrokes`
- **Chuyển đổi**: Bỏ prefix `str`, giữ lại tên mô tả: `messageContent`, `fontName`, `keyStrokes`

#### `h` - Handle
- **Ý nghĩa**: Handle Windows (HWND, HINSTANCE, HANDLE, etc.)
- **Sử dụng**: Cho các handle Windows API
- **Ví dụ**: `hWnd`, `hInstance`, `hToken`, `hUxTheme`
- **Chuyển đổi**: Bỏ prefix `h`, giữ lại tên mô tả: `wnd`, `instance`, `token`, `uxTheme`

#### `u` / `ui` - Unsigned
- **Ý nghĩa**: Unsigned integer
- **Sử dụng**: Cho các số nguyên không dấu
- **Ví dụ**: `uiFontSize`, `uiTimeout`, `uiIconID`, `uExitWinExFlags`
- **Chuyển đổi**: Bỏ prefix `ui`/`u`, giữ lại tên mô tả: `fontSize`, `timeout`, `iconID`, `exitWinExFlags`

#### `by` - Byte
- **Ý nghĩa**: Byte (8-bit)
- **Sử dụng**: Cho các giá trị byte
- **Ví dụ**: `byRepeatDays`, `byIconPos`, `byDisplayPos`
- **Chuyển đổi**: Bỏ prefix `by`, giữ lại tên mô tả: `repeatDays`, `iconPosition`, `displayPosition`

#### `st` - Struct
- **Ý nghĩa**: Struct object
- **Sử dụng**: Cho các biến struct
- **Ví dụ**: `stTime`, `stTimestamp`, `stNextSnoozeTime`
- **Chuyển đổi**: Bỏ prefix `st`, thêm suffix mô tả: `timeValue`, `timestamp`, `nextSnoozeTime`

#### `arr` - Array
- **Ý nghĩa**: Mảng (array)
- **Sử dụng**: Cho các biến mảng hoặc vector
- **Ví dụ**: `arrSchedExtraItemList`, `arrHotkeySetList`, `arrRmdItemList`
- **Chuyển đổi**: Bỏ prefix `arr`, giữ lại tên mô tả: `extraScheduleItemList`, `hotkeySetList`, `reminderItemList`

#### `sch` - Schedule
- **Ý nghĩa**: Schedule (lịch trình)
- **Sử dụng**: Cho các biến liên quan đến schedule
- **Ví dụ**: `schDefaultItem`, `schItem`, `schTemp`
- **Chuyển đổi**: Bỏ prefix `sch`, giữ lại tên mô tả: `defaultItem`, `item`, `temp`

#### `rps` - Repeat Set
- **Ý nghĩa**: Repeat Set (bộ lặp)
- **Sử dụng**: Cho các biến liên quan đến repeat set
- **Ví dụ**: `rpsRepeatSet`
- **Chuyển đổi**: Bỏ prefix `rps`, thêm suffix mô tả: `repeatSetInfo`

#### `rmd` - Reminder
- **Ý nghĩa**: Reminder (nhắc nhở)
- **Sử dụng**: Cho các biến liên quan đến reminder
- **Ví dụ**: `rmdCommonStyle`, `rmdItemList`
- **Chuyển đổi**: Bỏ prefix `rmd`, giữ lại tên mô tả: `commonStyleSet`, `reminderItemList`

#### `hks` - Hotkey Set
- **Ý nghĩa**: Hotkey Set (bộ phím tắt)
- **Sử dụng**: Cho các biến liên quan đến hotkey set
- **Ví dụ**: `hksItem`, `hksTemp`
- **Chuyển đổi**: Bỏ prefix `hks`, giữ lại tên mô tả: `item`, `temp`

#### `pwr` - Power
- **Ý nghĩa**: Power (năng lượng)
- **Sử dụng**: Cho các biến liên quan đến power management
- **Ví dụ**: `pwrItem`, `pwrTemp`
- **Chuyển đổi**: Bỏ prefix `pwr`, giữ lại tên mô tả: `item`, `temp`

---

## 12. Quy Tắc Đặt Tên Đặc Biệt

### 12.1. Boolean Variables
- Sử dụng prefix `is`, `has`, `allow`, `enable` để làm rõ ý nghĩa
- Ví dụ: `isEnabled`, `isEmpty`, `hasValue`, `allowSnoozing`, `enableDarkMode`

### 12.2. Collections
- Sử dụng suffix `List` cho các collection
- Ví dụ: `extraScheduleItemList`, `hotkeySetList`, `reminderItemList`

### 12.3. Temporary Variables
- Sử dụng tên ngắn gọn như `temp`, `item`, `data`, `ret`, `result`
- Ví dụ: `ScheduleItem temp`, `bool ret`, `HotkeySetData* newData`

### 12.4. Return Values
- Sử dụng `result`, `ret`, `retValue` cho giá trị trả về
- Ví dụ: `bool result`, `int ret`, `unsigned retNextID`

### 12.5. Error Codes
- Sử dụng `errorCode`, `errCode` cho mã lỗi
- Ví dụ: `DWORD errorCode`, `DWORD& errCode`

---

## 13. Tóm Tắt Quy Tắc Chuyển Đổi

### Từ Hungarian Notation sang LLVM Naming Conventions

| Loại | Hungarian Notation | LLVM Convention |
|------|-------------------|-----------------|
| Member variable | `m_nItemID` | `m_itemID` |
| Local variable | `nIndex` | `index` |
| Boolean | `bEnabled` | `isEnabled` |
| Pointer | `pItem` | `item` |
| String | `strMessage` | `messageContent` |
| DWORD | `dwErrorCode` | `errorCode` |
| Handle | `hWnd` | `wnd` |
| Array | `arrItemList` | `itemList` |

---

## 14. Lưu Ý Quan Trọng

1. **Không thay đổi tên hàm và class**: Chỉ thay đổi tên biến
2. **Giữ nguyên global variables**: Global variables vẫn sử dụng Hungarian Notation với prefix `g_`
3. **Consistency**: Đảm bảo tính nhất quán trong toàn bộ project
4. **Readability**: Tên biến phải dễ đọc và mô tả rõ ràng mục đích
5. **Avoid abbreviations**: Tránh viết tắt không rõ ràng

---

## 15. Ví Dụ Hoàn Chỉnh

### Trước khi refactor (Hungarian Notation):
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

### Sau khi refactor (LLVM Naming Conventions):
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

**Tài liệu này được tạo tự động dựa trên phân tích codebase PowerPlus3.**
**Ngày cập nhật**: 2025-01-XX




