# Architecture Recommendations - PowerPlus3 Redesign

## Table of Contents

1. [Modern C++ Practices](#modern-c-practices)
2. [Design Patterns](#design-patterns)
3. [Error Handling Strategy](#error-handling-strategy)
4. [Memory Management](#memory-management)
5. [Concurrency Model](#concurrency-model)
6. [Testing Strategy](#testing-strategy)
7. [Build System](#build-system)
8. [Dependency Management](#dependency-management)
9. [Code Organization](#code-organization)
10. [Performance Considerations](#performance-considerations)

---

## Modern C++ Practices

### C++ Standard Version

**Recommendation**: Target **C++20** (minimum C++17)

**Rationale:**

- Better type safety (concepts, ranges)
- Improved error handling (std::expected in C++23, custom Result type for C++20)
- Modern concurrency features
- Better performance (constexpr improvements)
- Standard library enhancements

### Key Modern C++ Features to Adopt

#### 1. **Smart Pointers**

**Replace raw pointers with smart pointers:**

```cpp
// ❌ Old approach
class ScheduleService {
private:
    ScheduleData* scheduleDataPtr_;
    
public:
    ~ScheduleService() {
        if (scheduleDataPtr_ != NULL) {
            delete scheduleDataPtr_;
        }
    }
};

// ✅ New approach
class ScheduleService {
private:
    std::unique_ptr<ScheduleData> scheduleData_;
    
public:
    // No destructor needed - automatic cleanup
};
```

**Guidelines:**

- Use `std::unique_ptr` for exclusive ownership
- Use `std::shared_ptr` only when shared ownership is necessary
- Use `std::weak_ptr` to break circular dependencies
- Avoid raw pointers in public interfaces

#### 2. **RAII (Resource Acquisition Is Initialization)**

**Apply RAII to all resources:**

```cpp
// ✅ RAII wrapper for Windows handles
class HandleGuard {
public:
    explicit HandleGuard(HANDLE handle) : handle_(handle) {}
    ~HandleGuard() {
        if (handle_ != INVALID_HANDLE_VALUE) {
            CloseHandle(handle_);
        }
    }
    
    HANDLE get() const { return handle_; }
    HANDLE release() {
        HANDLE temp = handle_;
        handle_ = INVALID_HANDLE_VALUE;
        return temp;
    }
    
private:
    HANDLE handle_;
};
```

#### 3. **constexpr and consteval**

**Use compile-time evaluation where possible:**

```cpp
// ✅ Compile-time constants
class ScheduleData {
public:
    static constexpr int kMinItemId = 10000;
    static constexpr int kMaxItemId = 19999;
    static constexpr int kDefaultItemId = 0x00;
    
    // Compile-time validation
    static constexpr bool isValidItemId(int id) {
        return (id == kDefaultItemId) || 
               (id >= kMinItemId && id <= kMaxItemId);
    }
};
```

#### 4. **std::optional and std::variant**

**Use for optional values and type-safe unions:**

```cpp
// ✅ Optional return values
std::optional<ScheduleItem> findScheduleById(unsigned id) {
    auto it = schedules_.find(id);
    if (it != schedules_.end()) {
        return *it;
    }
    return std::nullopt;
}

// ✅ Type-safe union for action types
using ActionData = std::variant<
    PowerAction,
    ScheduleAction,
    HotkeyAction
>;
```

#### 5. **Custom Result Type (Error Handling)**

**Create a Result type for better error handling:**

```cpp
// ✅ Result type for error handling
template<typename T, typename E = ErrorCode>
class Result {
public:
    static Result success(T value) {
        return Result(std::move(value), std::nullopt);
    }
    
    static Result error(E error) {
        return Result(std::nullopt, error);
    }
    
    bool isSuccess() const { return value_.has_value(); }
    bool isError() const { return error_.has_value(); }
    
    T& value() { return value_.value(); }
    const T& value() const { return value_.value(); }
    
    E& error() { return error_.value(); }
    const E& error() const { return error_.value(); }
    
private:
    std::optional<T> value_;
    std::optional<E> error_;
    
    Result(std::optional<T> value, std::optional<E> error)
        : value_(std::move(value)), error_(std::move(error)) {}
};
```

#### 6. **Ranges and Algorithms**

**Use modern algorithms and ranges (C++20):**

```cpp
// ✅ Modern algorithm usage
auto enabledSchedules = schedules_ 
    | std::views::filter([](const auto& s) { return s.isEnabled(); })
    | std::views::transform([](const auto& s) { return s.getId(); })
    | std::ranges::to<std::vector>();
```

#### 7. **Concepts (C++20)**

**Use concepts for better type safety:**

```cpp
// ✅ Concept definition
template<typename T>
concept Repository = requires(T t, unsigned id) {
    { t.findById(id) } -> std::same_as<Result<ScheduleItem, ErrorCode>>;
    { t.save(std::declval<ScheduleItem>()) } -> std::same_as<Result<void, ErrorCode>>;
};

// ✅ Use concept
template<Repository Repo>
class ScheduleService {
    Repo repository_;
    // ...
};
```

---

## Design Patterns

### 1. Dependency Injection

**Use constructor injection for dependencies:**

```cpp
// ✅ Dependency injection
class ScheduleService {
public:
    ScheduleService(
        std::unique_ptr<IScheduleRepository> repository,
        std::unique_ptr<IPowerActionExecutor> executor,
        std::shared_ptr<ILogger> logger
    ) : repository_(std::move(repository)),
        executor_(std::move(executor)),
        logger_(std::move(logger)) {}
        
private:
    std::unique_ptr<IScheduleRepository> repository_;
    std::unique_ptr<IPowerActionExecutor> executor_;
    std::shared_ptr<ILogger> logger_;
};

// Usage
auto service = std::make_unique<ScheduleService>(
    std::make_unique<RegistryScheduleRepository>(),
    std::make_unique<WindowsPowerActionExecutor>(),
    std::make_shared<FileLogger>()
);
```

### 2. Factory Pattern

**Use factories for object creation:**

```cpp
// ✅ Factory for platform-specific implementations
class PlatformFactory {
public:
    static std::unique_ptr<IPowerManager> createPowerManager() {
        #ifdef _WIN32
            return std::make_unique<WindowsPowerManager>();
        #else
            return std::make_unique<DummyPowerManager>();
        #endif
    }
    
    static std::unique_ptr<IHotkeyRegistry> createHotkeyRegistry() {
        #ifdef _WIN32
            return std::make_unique<WindowsHotkeyRegistry>();
        #else
            return std::make_unique<DummyHotkeyRegistry>();
        #endif
    }
};
```

### 3. Builder Pattern

**Use builders for complex object construction:**

```cpp
// ✅ Builder for schedule items
class ScheduleItemBuilder {
public:
    ScheduleItemBuilder& withId(unsigned id) {
        item_.setId(id);
        return *this;
    }
    
    ScheduleItemBuilder& withAction(PowerAction action) {
        item_.setAction(action);
        return *this;
    }
    
    ScheduleItemBuilder& withTime(const ClockTime& time) {
        item_.setTime(time);
        return *this;
    }
    
    ScheduleItemBuilder& enabled(bool enabled) {
        item_.setEnabled(enabled);
        return *this;
    }
    
    ScheduleItem build() {
        return std::move(item_);
    }
    
private:
    ScheduleItem item_;
};

// Usage
auto schedule = ScheduleItemBuilder()
    .withId(10001)
    .withAction(PowerAction::Shutdown)
    .withTime(ClockTime(14, 30))
    .enabled(true)
    .build();
```

### 4. Command Pattern

**Use commands for action execution:**

```cpp
// ✅ Command interface
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual Result<void, ErrorCode> execute() = 0;
    virtual Result<void, ErrorCode> undo() = 0;
    virtual std::string getDescription() const = 0;
};

// ✅ Concrete commands
class ShutdownCommand : public ICommand {
public:
    ShutdownCommand(std::shared_ptr<IPowerManager> powerManager)
        : powerManager_(std::move(powerManager)) {}
        
    Result<void, ErrorCode> execute() override {
        return powerManager_->shutdown();
    }
    
    Result<void, ErrorCode> undo() override {
        // Shutdown cannot be undone
        return Result<void, ErrorCode>::error(ErrorCode::CannotUndo);
    }
    
    std::string getDescription() const override {
        return "Shutdown system";
    }
    
private:
    std::shared_ptr<IPowerManager> powerManager_;
};
```

### 5. Observer Pattern

**Use observers for event notification:**

```cpp
// ✅ Observer interface
template<typename T>
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onEvent(const T& event) = 0;
};

// ✅ Observable
template<typename T>
class Observable {
public:
    void subscribe(std::weak_ptr<IObserver<T>> observer) {
        observers_.push_back(observer);
    }
    
    void notify(const T& event) {
        observers_.erase(
            std::remove_if(observers_.begin(), observers_.end(),
                [&event](const auto& weak_obs) {
                    if (auto obs = weak_obs.lock()) {
                        obs->onEvent(event);
                        return false;
                    }
                    return true; // Remove expired observers
                }),
            observers_.end()
        );
    }
    
private:
    std::vector<std::weak_ptr<IObserver<T>>> observers_;
};
```

---

## Error Handling Strategy

### Error Code System

**Create a comprehensive error code system:**

```cpp
// ✅ Error code enum
enum class ErrorCode : uint32_t {
    Success = 0,
    
    // General errors
    InvalidArgument = 0x1000,
    NullPointer = 0x1001,
    OutOfRange = 0x1002,
    
    // Power action errors
    PowerActionFailed = 0x2000,
    InsufficientPrivileges = 0x2001,
    SystemNotSupported = 0x2002,
    
    // Schedule errors
    ScheduleNotFound = 0x3000,
    ScheduleAlreadyExists = 0x3001,
    InvalidScheduleTime = 0x3002,
    
    // Hotkey errors
    HotkeyAlreadyRegistered = 0x4000,
    HotkeyRegistrationFailed = 0x4001,
    InvalidHotkey = 0x4002,
    
    // Persistence errors
    RegistryAccessFailed = 0x5000,
    FileNotFound = 0x5001,
    SerializationFailed = 0x5002,
};

// ✅ Error code utilities
class ErrorCodeUtils {
public:
    static std::string toString(ErrorCode code);
    static std::string getUserMessage(ErrorCode code, LanguageId lang);
    static bool isRecoverable(ErrorCode code);
};
```

### Result Type Usage

**Use Result type consistently:**

```cpp
// ✅ Function returning Result
Result<ScheduleItem, ErrorCode> ScheduleService::createSchedule(
    const CreateScheduleRequest& request
) {
    // Validate input
    if (!request.isValid()) {
        return Result<ScheduleItem, ErrorCode>::error(
            ErrorCode::InvalidArgument
        );
    }
    
    // Create schedule
    ScheduleItem item = ScheduleItemBuilder()
        .withId(generateId())
        .withAction(request.action())
        .withTime(request.time())
        .enabled(true)
        .build();
    
    // Save to repository
    auto saveResult = repository_->save(item);
    if (saveResult.isError()) {
        return Result<ScheduleItem, ErrorCode>::error(
            saveResult.error()
        );
    }
    
    return Result<ScheduleItem, ErrorCode>::success(std::move(item));
}

// ✅ Usage with error handling
auto result = scheduleService->createSchedule(request);
if (result.isError()) {
    logger_->error("Failed to create schedule: {}", 
                   ErrorCodeUtils::toString(result.error()));
    showErrorMessage(result.error());
    return;
}

auto schedule = result.value();
logger_->info("Created schedule: {}", schedule.getId());
```

---

## Memory Management

### Smart Pointer Guidelines

**Use smart pointers appropriately:**

```cpp
// ✅ Use unique_ptr for exclusive ownership
class Service {
private:
    std::unique_ptr<Repository> repository_;
    
public:
    Service(std::unique_ptr<Repository> repo)
        : repository_(std::move(repo)) {}
};

// ✅ Use shared_ptr only when necessary
class Observable {
private:
    std::vector<std::shared_ptr<IObserver>> observers_;
    // Use shared_ptr only if observers need to outlive Observable
};

// ✅ Use weak_ptr to break cycles
class Parent {
private:
    std::vector<std::weak_ptr<Child>> children_;
    // Use weak_ptr to avoid circular references
};
```

### RAII for Resources

**Wrap all resources in RAII classes:**

```cpp
// ✅ Registry key RAII
class RegistryKey {
public:
    RegistryKey(HKEY parent, const std::wstring& subkey, REGSAM access) {
        LONG result = RegOpenKeyEx(parent, subkey.c_str(), 0, access, &key_);
        if (result != ERROR_SUCCESS) {
            throw RegistryException(result);
        }
    }
    
    ~RegistryKey() {
        if (key_ != nullptr) {
            RegCloseKey(key_);
        }
    }
    
    HKEY get() const { return key_; }
    
private:
    HKEY key_ = nullptr;
};
```

---

## Concurrency Model

### Thread Safety Strategy

**Guidelines:**

1. **Single-threaded UI**: All UI operations on main thread
2. **Background workers**: Use std::thread or std::async for background tasks
3. **Thread-safe logging**: Logger must be thread-safe
4. **Immutable data**: Prefer immutable data structures where possible

### Async Operations

**Use std::async for background tasks:**

```cpp
// ✅ Async schedule processing
class ScheduleProcessor {
public:
    void start() {
        stopFlag_ = false;
        processorThread_ = std::thread([this]() {
            processSchedules();
        });
    }
    
    void stop() {
        stopFlag_ = true;
        if (processorThread_.joinable()) {
            processorThread_.join();
        }
    }
    
private:
    void processSchedules() {
        while (!stopFlag_) {
            auto schedules = repository_->findDueSchedules();
            for (const auto& schedule : schedules) {
                executor_->execute(schedule.getAction());
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    std::atomic<bool> stopFlag_{false};
    std::thread processorThread_;
};
```

### Thread-Safe Logging

**Ensure logger is thread-safe:**

```cpp
// ✅ Thread-safe logger
class ThreadSafeLogger {
public:
    void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        // Log implementation
    }
    
private:
    std::mutex mutex_;
};
```

---

## Testing Strategy

### Unit Testing

**Test domain and application layers:**

```cpp
// ✅ Unit test example
TEST(ScheduleServiceTest, CreateSchedule_Success) {
    // Arrange
    auto mockRepository = std::make_unique<MockScheduleRepository>();
    auto mockExecutor = std::make_unique<MockPowerActionExecutor>();
    auto logger = std::make_shared<NullLogger>();
    
    ScheduleService service(
        std::move(mockRepository),
        std::move(mockExecutor),
        logger
    );
    
    CreateScheduleRequest request;
    request.setAction(PowerAction::Shutdown);
    request.setTime(ClockTime(14, 30));
    
    // Act
    auto result = service.createSchedule(request);
    
    // Assert
    ASSERT_TRUE(result.isSuccess());
    EXPECT_EQ(result.value().getAction(), PowerAction::Shutdown);
}
```

### Integration Testing

**Test infrastructure implementations:**

```cpp
// ✅ Integration test example
TEST(RegistryRepositoryTest, SaveAndLoadSchedule) {
    // Arrange
    RegistryScheduleRepository repository;
    ScheduleItem item = ScheduleItemBuilder()
        .withId(10001)
        .withAction(PowerAction::Shutdown)
        .withTime(ClockTime(14, 30))
        .enabled(true)
        .build();
    
    // Act
    auto saveResult = repository.save(item);
    ASSERT_TRUE(saveResult.isSuccess());
    
    auto loadResult = repository.findById(10001);
    
    // Assert
    ASSERT_TRUE(loadResult.isSuccess());
    EXPECT_EQ(loadResult.value().getId(), 10001);
    EXPECT_EQ(loadResult.value().getAction(), PowerAction::Shutdown);
}
```

### Test Framework

**Recommendation**: Use **Google Test** or **Catch2**

```cmake
# CMakeLists.txt
find_package(GTest REQUIRED)
enable_testing()

add_executable(tests
    tests/unit/schedule_service_test.cpp
    tests/integration/registry_repository_test.cpp
)

target_link_libraries(tests
    GTest::gtest
    GTest::gtest_main
    powerplus_domain
    powerplus_application
)
```

---

## Build System

### CMake Configuration

**Use modern CMake:**

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(PowerPlus3 VERSION 3.2.1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_PLUGINS "Build plugin system" ON)

# Source directories
set(DOMAIN_SOURCES
    src/domain/entities/schedule/schedule_item.cpp
    src/domain/entities/schedule/schedule_data.cpp
    # ...
)

set(APPLICATION_SOURCES
    src/application/services/schedule_service.cpp
    # ...
)

# Libraries
add_library(powerplus_domain STATIC ${DOMAIN_SOURCES})
add_library(powerplus_application STATIC ${APPLICATION_SOURCES})

# Executable
add_executable(powerplus
    src/main.cpp
    src/presentation/main_window/main_window.cpp
    # ...
)

target_link_libraries(powerplus
    powerplus_domain
    powerplus_application
    # Windows libraries
)
```

---

## Dependency Management

### Third-Party Libraries

**Recommendations:**

1. **Logging**: spdlog
2. **JSON**: nlohmann/json
3. **Testing**: Google Test
4. **Date/Time**: date (Howard Hinnant's date library) or std::chrono
5. **CLI**: CLI11 (if adding command-line interface)

### Package Management

**Options:**

1. **vcpkg**: Microsoft's C++ package manager
2. **Conan**: Cross-platform package manager
3. **CMake FetchContent**: For header-only libraries

**Example with vcpkg:**

```cmake
# vcpkg.json
{
  "name": "powerplus3",
  "version": "3.2.1.0",
  "dependencies": [
    "spdlog",
    "nlohmann-json",
    "gtest"
  ]
}
```

---

## Code Organization

### Header File Guidelines

**Use include guards or #pragma once:**

```cpp
// ✅ Modern approach
#pragma once

#include <memory>
#include <string>
#include <vector>

namespace domain::entities::schedule {
    class ScheduleItem {
        // ...
    };
}
```

**Forward declarations:**

```cpp
// ✅ Use forward declarations when possible
namespace domain::interfaces {
    class IScheduleRepository;  // Forward declaration
}

class ScheduleService {
private:
    std::unique_ptr<IScheduleRepository> repository_;  // No include needed
};
```

### Namespace Organization

**Use nested namespaces:**

```cpp
// ✅ Clear namespace hierarchy
namespace powerplus {
    namespace domain {
        namespace entities {
            namespace schedule {
                class ScheduleItem { /* ... */ };
            }
        }
        
        namespace interfaces {
            class IScheduleRepository { /* ... */ };
        }
    }
    
    namespace application {
        namespace services {
            class ScheduleService { /* ... */ };
        }
    }
}
```

---

## Performance Considerations

### Optimization Guidelines

1. **Profile First**: Use profiling tools (Visual Studio Profiler, perf, etc.)
2. **Measure**: Don't optimize without data
3. **Hot Paths**: Focus optimization on frequently executed code
4. **Memory**: Minimize allocations in hot paths
5. **Cache**: Consider cache locality

### Performance Best Practices

```cpp
// ✅ Avoid unnecessary copies
void processSchedules(const std::vector<ScheduleItem>& schedules) {
    // Pass by const reference
    for (const auto& schedule : schedules) {
        // Process schedule
    }
}

// ✅ Use move semantics
ScheduleItem createSchedule() {
    ScheduleItem item;
    // ... initialize
    return item;  // Move semantics (RVO)
}

// ✅ Reserve vector capacity
std::vector<ScheduleItem> schedules;
schedules.reserve(100);  // Avoid reallocations
```

---

## Cross-Platform Architecture

### Platform Abstraction Strategy

For cross-platform migration, implement a **Platform Abstraction Layer (PAL)** that isolates all platform-specific code behind interfaces.

#### **Core Principle: Dependency Inversion**

- **Domain and Application layers** depend on abstractions (interfaces)
- **Infrastructure layer** implements these abstractions
- **Platform-specific code** lives only in infrastructure/platform/

### Platform Abstraction Interfaces

#### **1. Power Management Interface**

```cpp
namespace infrastructure::platform {
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
        virtual std::string getActionName(Action action) const = 0;
    };
}
```

**Implementation Guidelines:**

- Windows: Use `ExitWindowsEx()`, `SetSuspendState()`, `PostMessage()`
- Linux: Use systemd/logind D-Bus or `systemctl` commands
- macOS: Use Cocoa APIs or `pmset` commands

#### **2. Configuration Storage Interface**

```cpp
namespace infrastructure::platform {
    class IConfigurationStorage {
    public:
        virtual ~IConfigurationStorage() = default;
        
        virtual Result<ConfigData, ErrorCode> loadConfig() = 0;
        virtual Result<void, ErrorCode> saveConfig(const ConfigData& config) = 0;
        virtual Result<ScheduleData, ErrorCode> loadSchedule() = 0;
        virtual Result<void, ErrorCode> saveSchedule(const ScheduleData& schedule) = 0;
        virtual Result<HotkeySetData, ErrorCode> loadHotkeys() = 0;
        virtual Result<void, ErrorCode> saveHotkeys(const HotkeySetData& hotkeys) = 0;
        virtual Result<PwrReminderData, ErrorCode> loadReminders() = 0;
        virtual Result<void, ErrorCode> saveReminders(const PwrReminderData& reminders) = 0;
        
        virtual bool supportsBackup() const = 0;
        virtual Result<void, ErrorCode> backup(const std::string& backupPath) = 0;
        virtual Result<void, ErrorCode> restore(const std::string& backupPath) = 0;
    };
}
```

**Storage Backend Options:**

- **RegistryStorage** (Windows only): Uses Windows Registry
- **JsonFileStorage** (Cross-platform): Uses JSON files
- **SqliteStorage** (Optional): Uses SQLite database

#### **3. Hotkey Management Interface**

```cpp
namespace infrastructure::platform {
    class IHotkeyManager {
    public:
        virtual ~IHotkeyManager() = default;
        
        enum class ModifierKey {
            Ctrl,
            Alt,
            Shift,
            Win,     // Windows key / Super key
            Meta     // Meta key (Linux/macOS)
        };
        
        struct KeyCombination {
            std::vector<ModifierKey> modifiers;
            unsigned keyCode;  // Platform-agnostic key code
        };
        
        virtual Result<void, ErrorCode> registerHotkey(
            unsigned id,
            const KeyCombination& combo
        ) = 0;
        virtual Result<void, ErrorCode> unregisterHotkey(unsigned id) = 0;
        virtual Result<void, ErrorCode> unregisterAll() = 0;
        virtual bool isHotkeySupported() const = 0;
        virtual bool supportsBackgroundHotkeys() const = 0;
    };
}
```

**Platform-Specific Notes:**

- **Windows**: `RegisterHotKey()` + keyboard hook for background
- **Linux**: X11 `XGrabKey()` or Wayland protocols (limited)
- **macOS**: Carbon/Cocoa hotkey APIs

#### **4. System Tray Interface**

```cpp
namespace infrastructure::platform {
    class ISystemTray {
    public:
        virtual ~ISystemTray() = default;
        
        virtual Result<void, ErrorCode> createIcon(
            const std::string& iconPath,
            const std::string& tooltip
        ) = 0;
        virtual Result<void, ErrorCode> updateIcon(const std::string& iconPath) = 0;
        virtual Result<void, ErrorCode> updateTooltip(const std::string& tooltip) = 0;
        virtual Result<void, ErrorCode> showNotification(
            const std::string& title,
            const std::string& message,
            int timeoutMs = 5000
        ) = 0;
        virtual void setMenu(std::unique_ptr<IMenu> menu) = 0;
        virtual void removeIcon() = 0;
        virtual bool isSupported() const = 0;
    };
}
```

**Platform-Specific Notes:**

- **Windows**: `Shell_NotifyIcon()` API
- **Linux**: StatusNotifier protocol or libappindicator
- **macOS**: `NSStatusItem` API

#### **5. File System Interface**

```cpp
namespace infrastructure::platform {
    class IFileSystem {
    public:
        virtual ~IFileSystem() = default;
        
        virtual std::filesystem::path getApplicationDataPath() const = 0;
        virtual std::filesystem::path getConfigPath() const = 0;
        virtual std::filesystem::path getLogPath() const = 0;
        virtual std::filesystem::path getTempPath() const = 0;
        
        virtual Result<void, ErrorCode> createDirectory(
            const std::filesystem::path& path
        ) = 0;
        virtual bool fileExists(const std::filesystem::path& path) const = 0;
        virtual Result<std::string, ErrorCode> readFile(
            const std::filesystem::path& path
        ) = 0;
        virtual Result<void, ErrorCode> writeFile(
            const std::filesystem::path& path,
            const std::string& content
        ) = 0;
    };
}
```

**Platform-Specific Paths:**

- **Windows**: `%APPDATA%\PowerPlus3\`
- **Linux**: `~/.config/PowerPlus3/` (XDG Base Directory)
- **macOS**: `~/Library/Application Support/PowerPlus3/`

#### **6. Timer Management Interface**

```cpp
namespace infrastructure::platform {
    class ITimerManager {
    public:
        virtual ~ITimerManager() = default;
        
        using TimerCallback = std::function<void()>;
        
        virtual unsigned createTimer(
            int intervalMs,
            TimerCallback callback,
            bool repeat = true
        ) = 0;
        virtual Result<void, ErrorCode> destroyTimer(unsigned timerId) = 0;
        virtual Result<void, ErrorCode> startTimer(unsigned timerId) = 0;
        virtual Result<void, ErrorCode> stopTimer(unsigned timerId) = 0;
    };
}
```

**Platform-Specific Notes:**

- **Windows**: `SetTimer()` / `KillTimer()` or `std::thread` + `std::chrono`
- **Linux/macOS**: `std::thread` + `std::chrono` or Qt's `QTimer`

### Platform Factory Pattern

```cpp
namespace infrastructure::platform {
    class PlatformFactory {
    public:
        // Power management
        static std::unique_ptr<IPowerManager> createPowerManager();
        
        // Configuration storage
        static std::unique_ptr<IConfigurationStorage> createStorage(
            StorageType type = StorageType::Auto
        );
        
        // Hotkey management
        static std::unique_ptr<IHotkeyManager> createHotkeyManager();
        
        // System tray
        static std::unique_ptr<ISystemTray> createSystemTray();
        
        // File system
        static std::unique_ptr<IFileSystem> createFileSystem();
        
        // Timer management
        static std::unique_ptr<ITimerManager> createTimerManager();
        
        // Platform detection
        static Platform getCurrentPlatform();
        static bool isWindows();
        static bool isLinux();
        static bool isMacOS();
        
    private:
        enum class Platform {
            Windows,
            Linux,
            MacOS,
            Unknown
        };
        
        static Platform currentPlatform_;
    };
}
```

### Cross-Platform Code Organization

#### **Directory Structure**

```markdown
src/infrastructure/platform/
├── abstractions/                       # Interface definitions
│   ├── power_manager_interface.h
│   ├── config_storage_interface.h
│   ├── hotkey_manager_interface.h
│   ├── system_tray_interface.h
│   ├── file_system_interface.h
│   └── timer_manager_interface.h
├── windows/                            # Windows implementations
│   ├── windows_power_manager.h/cpp
│   ├── registry_storage.h/cpp
│   ├── windows_hotkey_manager.h/cpp
│   ├── windows_system_tray.h/cpp
│   └── windows_file_system.h/cpp
├── linux/                              # Linux implementations
│   ├── linux_power_manager.h/cpp
│   ├── json_file_storage.h/cpp
│   ├── linux_hotkey_manager.h/cpp
│   ├── linux_system_tray.h/cpp
│   └── linux_file_system.h/cpp
├── macos/                              # macOS implementations (future)
│   └── [macOS-specific files]
└── factory/
    ├── platform_factory.h
    └── platform_factory.cpp
```

### Platform-Specific Considerations

#### **Windows-Specific**

1. **Registry Access**
   - Use `RegOpenKeyEx`, `RegQueryValueEx`, `RegSetValueEx`
   - Handle registry errors gracefully
   - Support both 32-bit and 64-bit registry views

2. **Privilege Management**
   - Adjust token privileges for shutdown/restart
   - Handle privilege elevation requests
   - Support run-as-admin mode

3. **Windows Messages**
   - Use `PostMessage` for display off
   - Handle `WM_POWERBROADCAST` for power events
   - Handle `WM_WTSSESSION_CHANGE` for session events

#### **Linux-Specific**

1. **Power Management**
   - Prefer D-Bus interface (systemd/logind)
   - Fallback to `systemctl` commands
   - Handle polkit authorization
   - Support different init systems (systemd, upstart, etc.)

2. **Display Server**
   - Support both X11 and Wayland
   - Detect display server at runtime
   - X11: Use `xset`, `xrandr` for display control
   - Wayland: Limited hotkey support

3. **Desktop Environment**
   - Detect desktop environment (GNOME, KDE, XFCE, etc.)
   - Adapt system tray implementation
   - Use appropriate notification system

#### **macOS-Specific** (Future)

1. **Power Management**
   - Use Cocoa APIs (`NSWorkspace`)
   - Support `pmset` commands
   - Handle macOS-specific sleep modes

2. **System Integration**
   - Use `NSStatusItem` for system tray
   - Integrate with System Preferences
   - Support macOS notification center

### Build System for Cross-Platform

#### **CMake Platform Detection**

```cmake
# Platform detection
if(WIN32)
    set(PLATFORM_WINDOWS ON)
    add_definitions(-DPLATFORM_WINDOWS)
    set(PLATFORM_NAME "Windows")
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX ON)
    add_definitions(-DPLATFORM_LINUX)
    set(PLATFORM_NAME "Linux")
    
    # Detect desktop environment
    find_program(XDG_CURRENT_DESKTOP xdg-current-desktop)
    if(XDG_CURRENT_DESKTOP)
        execute_process(
            COMMAND ${XDG_CURRENT_DESKTOP}
            OUTPUT_VARIABLE DESKTOP_ENV
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "Desktop environment: ${DESKTOP_ENV}")
    endif()
    
    # Detect display server
    if(DEFINED ENV{WAYLAND_DISPLAY})
        set(DISPLAY_SERVER "Wayland")
    else()
        set(DISPLAY_SERVER "X11")
    endif()
    
elseif(APPLE)
    set(PLATFORM_MACOS ON)
    add_definitions(-DPLATFORM_MACOS)
    set(PLATFORM_NAME "macOS")
endif()

message(STATUS "Building for platform: ${PLATFORM_NAME}")
```

#### **Conditional Compilation**

```cpp
// Platform-specific includes
#ifdef PLATFORM_WINDOWS
    #include <windows.h>
    #include <winuser.h>
#elif defined(PLATFORM_LINUX)
    #include <X11/Xlib.h>
    #include <systemd/sd-bus.h>
#elif defined(PLATFORM_MACOS)
    #include <Cocoa/Cocoa.h>
#endif

// Platform-specific code
Result<void, ErrorCode> executeShutdown() {
    #ifdef PLATFORM_WINDOWS
        // Windows implementation
        return executeWindowsShutdown();
    #elif defined(PLATFORM_LINUX)
        // Linux implementation
        return executeLinuxShutdown();
    #elif defined(PLATFORM_MACOS)
        // macOS implementation
        return executeMacOSShutdown();
    #else
        return Result<void, ErrorCode>::error(ERR_UNSUPPORTED_PLATFORM);
    #endif
}
```

### Testing Cross-Platform Code

#### **Unit Testing Strategy**

1. **Mock Platform Interfaces**
   - Create mock implementations of PAL interfaces
   - Test domain/application logic independently
   - Verify interface contracts

2. **Platform-Specific Tests**
   - Test each platform implementation separately
   - Use platform-specific test frameworks if needed
   - Integration tests for platform features

3. **Cross-Platform Test Suite**
   - Common test cases for all platforms
   - Platform-specific test cases
   - Feature parity validation

### Migration Best Practices

1. **Start with Windows Implementation**
   - Wrap existing Windows code in PAL interfaces
   - Maintain backward compatibility
   - Test thoroughly before adding other platforms

2. **Incremental Platform Addition**
   - Add one platform at a time
   - Maintain feature parity
   - Document platform-specific limitations

3. **Feature Detection**
   - Detect platform capabilities at runtime
   - Graceful degradation for unsupported features
   - Clear error messages for users

4. **Code Reuse**
   - Maximize platform-independent code
   - Share common utilities
   - Use standard library where possible

---

## Conclusion

These recommendations provide a foundation for modernizing PowerPlus3's architecture. Key principles:

1. **Modern C++**: Use C++20 features for better type safety and performance
2. **Smart Pointers**: Eliminate manual memory management
3. **RAII**: Wrap all resources
4. **Error Handling**: Use Result type for explicit error handling
5. **Testing**: Comprehensive test coverage
6. **Build System**: Modern CMake configuration
7. **Dependencies**: Use established libraries
8. **Performance**: Profile-driven optimization
9. **Cross-Platform**: Platform abstraction layer for multi-platform support
10. **Separation of Concerns**: Clear boundaries between platform-independent and platform-specific code

---

*This document should be updated as new best practices emerge and the project evolves.*
