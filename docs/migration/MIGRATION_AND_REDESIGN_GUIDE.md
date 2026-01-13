# Migration and Redesign Guide - PowerPlus3

## Table of Contents

1. [Overview](#overview)
2. [Migration Strategy](#migration-strategy)
3. [Recommended New Architecture](#recommended-new-architecture)
4. [Project Structure Redesign](#project-structure-redesign)
5. [Architecture Patterns](#architecture-patterns)
6. [Extension Points](#extension-points)
7. [Rebranding Considerations](#rebranding-considerations)
8. [Migration Phases](#migration-phases)
9. [Risk Assessment](#risk-assessment)
10. [Implementation Roadmap](#implementation-roadmap)

---

## Overview

### Purpose

This document provides guidance for **redesigning, rebuilding, extending, and rebranding** the PowerPlus3 application. Unlike incremental refactoring, this guide focuses on architectural transformation that enables:

- **Modern C++ practices** (C++17/20/23)
- **Better separation of concerns** (data-first architecture)
- **Enhanced extensibility** (plugin system, modular features)
- **Cross-platform potential** (abstraction layers)
- **Improved maintainability** (clear module boundaries)
- **Rebranding flexibility** (configurable branding elements)

### Scope

This guide addresses:

- ✅ Complete architectural redesign
- ✅ New project structure recommendations
- ✅ Migration path from current codebase
- ✅ Extension mechanisms for new features
- ✅ Rebranding strategy and implementation
- ❌ Incremental refactoring (see `REFACTORING_PLAN.md`)

### Target Outcomes

1. **Clean Architecture**: Clear separation between domain logic, application logic, and infrastructure
2. **Modular Design**: Features can be added/removed without affecting core
3. **Testability**: Business logic can be tested independently
4. **Extensibility**: Plugin system for custom actions and features
5. **Maintainability**: Reduced coupling, increased cohesion
6. **Future-Proof**: Ready for potential cross-platform expansion

---

## Migration Strategy

### Approach: Strangler Fig Pattern

Instead of a big-bang rewrite, use the **Strangler Fig Pattern**:

1. **Build new architecture alongside old code**
2. **Gradually migrate features** from old to new
3. **Route functionality** through new architecture
4. **Remove old code** once migration is complete

### Migration Principles

#### 1. **Data-First Migration**

- Start with data models (domain layer)
- Ensure data structures are platform-independent
- Create clear interfaces for data access

#### 2. **Feature-by-Feature Migration**

- Migrate one complete feature at a time
- Maintain feature parity during migration
- Test thoroughly before moving to next feature

#### 3. **Backward Compatibility**

- Maintain registry format compatibility (or provide migration tool)
- Support loading old configuration formats
- Provide data migration utilities

#### 4. **Incremental Rollout**

- New architecture runs alongside old code initially
- Switch features to new architecture one by one
- Keep old code as fallback until new code is proven

### Migration Phases Overview

```markdown
Phase 1: Foundation Setup
  ├─ New project structure
  ├─ Core domain models
  └─ Basic infrastructure

Phase 2: Core Features Migration
  ├─ Power actions
  ├─ Configuration management
  └─ Basic UI framework

Phase 3: Advanced Features Migration
  ├─ Scheduling system
  ├─ Hotkey management
  └─ Reminder system

Phase 4: UI and Integration
  ├─ Modern UI framework
  ├─ System integration
  └─ Logging and monitoring

Phase 5: Extension and Rebranding
  ├─ Plugin system
  ├─ Branding framework
  └─ Final cleanup
```

---

## Recommended New Architecture

### High-Level Architecture

```markdown
┌─────────────────────────────────────────────────────────────┐
│                    Presentation Layer                       │
│         (UI Framework, Dialogs, Views, ViewModels)          │
│  - Platform-specific UI (MFC, WinUI, or future frameworks)  │
│            - View models (presentation logic)               │
│                 - User input handling                       │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    Application Layer                        │
│            (Use Cases, Services, Orchestration)             │
│             - Power action execution service                │
│              - Schedule management service                  │
│              - Hotkey registration service                  │
│               - Reminder display service                    │
│                - Configuration service                      │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                      Domain Layer                           │
│         (Business Logic, Entities, Value Objects)           │
│                 - Power action types                        │
│                 - Schedule entities                         │
│                  - Hotkey entities                          │
│                 - Reminder entities                         │
│               - Configuration entities                      │
│                  - Domain services                          │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                  Infrastructure Layer                       │
│    (Persistence, System Integration, External Services)     │
│              - Repository implementations                   │
│                 - Windows API wrappers                      │
│                   - Registry access                         │
│                - File system operations                     │
│                - Logging infrastructure                     │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    Platform Abstraction                     │
│            (Platform-specific implementations)              │
│                - Windows power management                   │
│                    - Windows registry                       │
│                   - Windows system tray                     │
│                - Windows hotkey registration                │
└─────────────────────────────────────────────────────────────┘
```

### Architecture Principles

#### 1. **Dependency Inversion**

- High-level modules don't depend on low-level modules
- Both depend on abstractions (interfaces)
- Infrastructure depends on domain, not vice versa

#### 2. **Single Responsibility**

- Each class/module has one reason to change
- Clear separation of concerns

#### 3. **Open/Closed Principle**

- Open for extension (new features via plugins)
- Closed for modification (core remains stable)

#### 4. **Interface Segregation**

- Clients depend only on interfaces they use
- Small, focused interfaces

#### 5. **Dependency Injection**

- Dependencies provided from outside
- Enables testing and flexibility

### Layer Responsibilities

#### Domain Layer (Core Business Logic)

- **Entities**: Core business objects (Schedule, Hotkey, Reminder, etc.)
- **Value Objects**: Immutable objects (ClockTime, DateTime, etc.)
- **Domain Services**: Business logic that doesn't belong to a single entity
- **Interfaces**: Repository and service interfaces (abstractions)

**Rules:**

- ✅ NO platform dependencies (no Win32, no MFC)
- ✅ NO UI dependencies
- ✅ Pure business logic
- ✅ Testable in isolation

#### Application Layer (Use Cases)

- **Services**: Orchestrate domain objects to fulfill use cases
- **DTOs**: Data transfer objects for cross-layer communication
- **Application Services**: Coordinate between domain and infrastructure

**Rules:**

- ✅ Depends on domain layer only
- ✅ Uses domain interfaces (not implementations)
- ✅ Contains use case orchestration logic
- ✅ NO direct platform/UI access

#### Infrastructure Layer (Technical Implementation)

- **Repositories**: Concrete implementations of domain interfaces
- **Platform Adapters**: Windows API wrappers
- **Persistence**: Registry, file system access
- **External Services**: Logging, system integration

**Rules:**

- ✅ Implements domain interfaces
- ✅ Contains all platform-specific code
- ✅ Can be swapped without affecting domain/application

#### Presentation Layer (User Interface)

- **Views**: UI components (dialogs, windows)
- **ViewModels**: Presentation logic (if using MVVM)
- **Controllers**: Handle user input and coordinate with application layer

**Rules:**

- ✅ Depends on application layer
- ✅ NO direct domain access (goes through application layer)
- ✅ Platform-specific UI code

---

## Project Structure Redesign

### Recommended Directory Structure

```markdown
PowerPlus3/
├── src/
│   ├── domain/                                       # Domain layer (business logic)
│   │   ├── entities/                                 # Core business entities
│   │   │   ├── schedule/
│   │   │   │   ├── schedule_item.h
│   │   │   │   ├── schedule_item.cpp
│   │   │   │   ├── schedule_data.h
│   │   │   │   └── schedule_data.cpp
│   │   │   ├── hotkey/
│   │   │   │   ├── hotkey_item.h
│   │   │   │   └── hotkey_item.cpp
│   │   │   ├── reminder/
│   │   │   │   ├── reminder_item.h
│   │   │   │   └── reminder_item.cpp
│   │   │   └── config/
│   │   │       ├── app_config.h
│   │   │       └── app_config.cpp
│   │   ├── value_objects/                            # Immutable value objects
│   │   │   ├── clock_time.h
│   │   │   ├── clock_time.cpp
│   │   │   ├── date_time.h
│   │   │   └── date_time.cpp
│   │   ├── services/                                 # Domain services
│   │   │   ├── schedule_service.h
│   │   │   └── schedule_service.cpp
│   │   └── interfaces/                               # Repository/service interfaces
│   │       ├── schedule_repository.h
│   │       ├── config_repository.h
│   │       └── power_action_executor.h
│   │
│   ├── application/                                  # Application layer (use cases)
│   │   ├── services/                                 # Application services
│   │   │   ├── power_action_service.h
│   │   │   ├── power_action_service.cpp
│   │   │   ├── schedule_service.h
│   │   │   ├── schedule_service.cpp
│   │   │   ├── hotkey_service.h
│   │   │   ├── hotkey_service.cpp
│   │   │   ├── reminder_service.h
│   │   │   ├── reminder_service.cpp
│   │   │   └── config_service.h
│   │   │   └── config_service.cpp
│   │   ├── dto/                                      # Data transfer objects
│   │   │   ├── schedule_dto.h
│   │   │   ├── hotkey_dto.h
│   │   │   └── reminder_dto.h
│   │   └── use_cases/                                # Specific use cases
│   │       ├── execute_power_action.h
│   │       ├── execute_power_action.cpp
│   │       ├── create_schedule.h
│   │       └── create_schedule.cpp
│   │
│   ├── infrastructure/                               # Infrastructure layer
│   │   ├── persistence/                              # Data persistence
│   │   │   ├── registry/
│   │   │   │   ├── registry_repository.h
│   │   │   │   ├── registry_repository.cpp
│   │   │   │   ├── registry_config_repository.h
│   │   │   │   └── registry_config_repository.cpp
│   │   │   └── file/
│   │   │       ├── file_logger.h
│   │   │       └── file_logger.cpp
│   │   ├── platform/                                 # Platform-specific code
│   │   │   ├── windows/
│   │   │   │   ├── windows_power_manager.h
│   │   │   │   ├── windows_power_manager.cpp
│   │   │   │   ├── windows_hotkey_registry.h
│   │   │   │   ├── windows_hotkey_registry.cpp
│   │   │   │   ├── windows_system_tray.h
│   │   │   │   └── windows_system_tray.cpp
│   │   │   └── abstractions/                         # Platform abstraction interfaces
│   │   │       ├── power_manager_interface.h
│   │   │       ├── hotkey_registry_interface.h
│   │   │       └── system_tray_interface.h
│   │   ├── logging/                                  # Logging infrastructure
│   │   │   ├── logger.h
│   │   │   ├── logger.cpp
│   │   │   ├── event_logger.h
│   │   │   └── event_logger.cpp
│   │   └── serialization/                            # Data serialization
│   │       ├── config_serializer.h
│   │       └── config_serializer.cpp
│   │
│   ├── presentation/                                 # Presentation layer (UI)
│   │   ├── framework/                                # UI framework base
│   │   │   ├── base_dialog.h
│   │   │   ├── base_dialog.cpp
│   │   │   ├── base_window.h
│   │   │   └── base_window.cpp
│   │   ├── views/                                    # UI views
│   │   │   ├── main_window/
│   │   │   │   ├── main_window.h
│   │   │   │   └── main_window.cpp
│   │   │   ├── schedule_dialog/
│   │   │   │   ├── schedule_dialog.h
│   │   │   │   └── schedule_dialog.cpp
│   │   │   ├── hotkey_dialog/
│   │   │   │   ├── hotkey_dialog.h
│   │   │   │   └── hotkey_dialog.cpp
│   │   │   └── reminder_dialog/
│   │   │       ├── reminder_dialog.h
│   │   │       └── reminder_dialog.cpp
│   │   ├── view_models/                              # View models (if using MVVM)
│   │   │   ├── main_window_view_model.h
│   │   │   └── main_window_view_model.cpp
│   │   └── controls/                                 # Custom UI controls
│   │       ├── grid_control.h
│   │       └── grid_control.cpp
│   │
│   ├── plugins/                                      # Plugin system (optional)
│   │   ├── plugin_interface.h
│   │   ├── plugin_manager.h
│   │   └── plugin_manager.cpp
│   │
│   └── common/                                       # Shared utilities
│       ├── types/                                    # Common types
│       │   ├── result.h                              # Result<T, E> type
│       │   ├── optional.h                            # Enhanced optional
│       │   └── string_utils.h
│       ├── errors/                                   # Error handling
│       │   ├── error_code.h
│       │   └── error_code.cpp
│       └── utils/                                    # Utility functions
│           ├── string_utils.h
│           └── string_utils.cpp
│
├── tests/                                            # Test code
│   ├── unit/                                         # Unit tests
│   │   ├── domain/
│   │   ├── application/
│   │   └── infrastructure/
│   ├── integration/                                  # Integration tests
│   └── e2e/                                          # End-to-end tests
│
├── tools/                                            # Development tools
│   ├── migration/                                    # Migration utilities
│   │   ├── config_migrator.h
│   │   └── config_migrator.cpp
│   └── codegen/                                      # Code generation
│
├── resources/                                        # Resources
│   ├── icons/
│   ├── images/
│   ├── strings/                                      # Localization strings
│   │   ├── en/
│   │   ├── vi/
│   │   └── zh-CN/
│   └── branding/                                     # Branding assets
│       ├── logos/
│       ├── themes/
│       └── config.json                               # Branding configuration
│
├── docs/                                             # Documentation
│   ├── design/
│   ├── migration/
│   └── api/
│
├── third_party/                                      # Third-party libraries
│
├── CMakeLists.txt                                    # CMake build configuration
├── conanfile.txt                                     # Conan dependencies (optional)
└── README.md
```

### Key Structural Improvements

#### 1. **Clear Layer Separation**

- Each layer in its own directory
- Dependencies flow in one direction (presentation → application → domain)
- Infrastructure implements domain interfaces

#### 2. **Feature-Based Organization**

- Features grouped by domain concept (schedule, hotkey, reminder)
- Related code co-located
- Easier to understand and maintain

#### 3. **Testability**

- Tests mirror source structure
- Easy to find and write tests
- Clear separation enables unit testing

#### 4. **Extensibility**

- Plugin system directory
- Clear extension points
- New features can be added without modifying core

#### 5. **Branding Separation**

- Branding assets in dedicated directory
- Configuration-driven branding
- Easy to rebrand without code changes

---

## Architecture Patterns

### 1. Repository Pattern

**Purpose**: Abstract data access, enable testing, support multiple storage backends

```cpp
// Domain interface
namespace domain::interfaces {
    class IScheduleRepository {
    public:
        virtual ~IScheduleRepository() = default;
        virtual Result<ScheduleItem, ErrorCode> findById(unsigned id) = 0;
        virtual Result<void, ErrorCode> save(const ScheduleItem& item) = 0;
        virtual Result<std::vector<ScheduleItem>, ErrorCode> findAll() = 0;
        virtual Result<void, ErrorCode> remove(unsigned id) = 0;
    };
}

// Infrastructure implementation
namespace infrastructure::persistence::registry {
    class RegistryScheduleRepository : public domain::interfaces::IScheduleRepository {
    public:
        Result<ScheduleItem, ErrorCode> findById(unsigned id) override;
        Result<void, ErrorCode> save(const ScheduleItem& item) override;
        Result<std::vector<ScheduleItem>, ErrorCode> findAll() override;
        Result<void, ErrorCode> remove(unsigned id) override;
    private:
        // Registry access implementation
    };
}
```

### 2. Service Pattern

**Purpose**: Encapsulate business logic, coordinate between domain objects

```cpp
// Application service
namespace application::services {
    class ScheduleService {
    public:
        ScheduleService(
            std::unique_ptr<domain::interfaces::IScheduleRepository> repository,
            std::unique_ptr<domain::interfaces::IPowerActionExecutor> executor
        );
        
        Result<ScheduleItem, ErrorCode> createSchedule(
            const CreateScheduleRequest& request
        );
        
        Result<void, ErrorCode> executeScheduledAction(unsigned scheduleId);
        
    private:
        std::unique_ptr<domain::interfaces::IScheduleRepository> repository_;
        std::unique_ptr<domain::interfaces::IPowerActionExecutor> executor_;
    };
}
```

### 3. Strategy Pattern

**Purpose**: Support multiple power action execution strategies (real, dummy, test)

```cpp
// Domain interface
namespace domain::interfaces {
    class IPowerActionExecutor {
    public:
        virtual ~IPowerActionExecutor() = default;
        virtual Result<void, ErrorCode> execute(PowerAction action) = 0;
    };
}

// Implementations
namespace infrastructure::platform::windows {
    class WindowsPowerActionExecutor : public domain::interfaces::IPowerActionExecutor {
        Result<void, ErrorCode> execute(PowerAction action) override;
    };
    
    class DummyPowerActionExecutor : public domain::interfaces::IPowerActionExecutor {
        Result<void, ErrorCode> execute(PowerAction action) override;
    };
}
```

### 4. Observer Pattern

**Purpose**: Decouple schedule/reminder processing from UI updates

```cpp
// Domain interface
namespace domain::interfaces {
    template<typename T>
    class IObserver {
    public:
        virtual ~IObserver() = default;
        virtual void onEvent(const T& event) = 0;
    };
}

// Application service with observer support
namespace application::services {
    class ScheduleService {
    public:
        void subscribe(std::shared_ptr<IObserver<ScheduleEvent>> observer);
        void notify(const ScheduleEvent& event);
    private:
        std::vector<std::weak_ptr<IObserver<ScheduleEvent>>> observers_;
    };
}
```

### 5. Factory Pattern

**Purpose**: Create platform-specific implementations based on configuration

```cpp
namespace infrastructure::platform {
    class PlatformFactory {
    public:
        static std::unique_ptr<IPowerManager> createPowerManager();
        static std::unique_ptr<IHotkeyRegistry> createHotkeyRegistry();
        static std::unique_ptr<ISystemTray> createSystemTray();
    };
}
```

### 6. Plugin Pattern

**Purpose**: Enable extensibility for custom actions and features

```cpp
// Plugin interface
namespace plugins {
    class IPlugin {
    public:
        virtual ~IPlugin() = default;
        virtual std::string getName() const = 0;
        virtual std::string getVersion() const = 0;
        virtual void initialize() = 0;
        virtual void shutdown() = 0;
    };
    
    class IPowerActionPlugin : public IPlugin {
    public:
        virtual std::vector<PowerAction> getSupportedActions() const = 0;
        virtual Result<void, ErrorCode> execute(PowerAction action) = 0;
    };
}

// Plugin manager
namespace plugins {
    class PluginManager {
    public:
        void loadPlugin(const std::string& pluginPath);
        void unloadPlugin(const std::string& pluginName);
        std::vector<std::shared_ptr<IPlugin>> getPlugins() const;
    };
}
```

---

## Extension Points

### 1. Custom Power Actions

**Current**: Hard-coded action types in `AppMacros.h`

**New Approach**: Plugin-based action system

```cpp
// Domain interface
namespace domain::interfaces {
    class ICustomPowerAction {
    public:
        virtual ~ICustomPowerAction() = default;
        virtual std::string getId() const = 0;
        virtual std::string getName() const = 0;
        virtual std::string getDescription() const = 0;
        virtual Result<void, ErrorCode> execute() = 0;
    };
}

// Plugin implementation example
namespace plugins::custom_actions {
    class CustomShutdownAction : public domain::interfaces::ICustomPowerAction {
    public:
        std::string getId() const override { return "custom.shutdown"; }
        std::string getName() const override { return "Custom Shutdown"; }
        Result<void, ErrorCode> execute() override;
    };
}
```

### 2. Custom Schedule Triggers

**Current**: Only time-based triggers

**New Approach**: Extensible trigger system

```cpp
namespace domain::interfaces {
    class IScheduleTrigger {
    public:
        virtual ~IScheduleTrigger() = default;
        virtual bool shouldTrigger(const ClockTime& currentTime) const = 0;
        virtual std::string getType() const = 0;
    };
}

// Built-in triggers
namespace domain::triggers {
    class TimeTrigger : public IScheduleTrigger { /* ... */ };
    class SystemEventTrigger : public IScheduleTrigger { /* ... */ };
}

// Custom trigger plugin
namespace plugins::triggers {
    class NetworkStatusTrigger : public IScheduleTrigger { /* ... */ };
}
```

### 3. Custom Reminder Styles

**Current**: Fixed message box and dialog box styles

**New Approach**: Pluggable reminder renderers

```cpp
namespace domain::interfaces {
    class IReminderRenderer {
    public:
        virtual ~IReminderRenderer() = default;
        virtual std::string getStyleId() const = 0;
        virtual Result<void, ErrorCode> render(const ReminderItem& item) = 0;
    };
}

// Built-in renderers
namespace presentation::reminders {
    class MessageBoxRenderer : public IReminderRenderer { /* ... */ };
    class DialogBoxRenderer : public IReminderRenderer { /* ... */ };
}

// Custom renderer plugin
namespace plugins::reminders {
    class ToastNotificationRenderer : public IReminderRenderer { /* ... */ };
}
```

### 4. Custom Storage Backends

**Current**: Registry-only storage

**New Approach**: Repository pattern with multiple implementations

```cpp
// Support multiple storage backends
namespace infrastructure::persistence {
    class RegistryConfigRepository : public IConfigRepository { /* ... */ };
    class JsonFileConfigRepository : public IConfigRepository { /* ... */ };
    class SqliteConfigRepository : public IConfigRepository { /* ... */ };
}
```

### 5. Custom UI Themes

**Current**: Fixed UI appearance

**New Approach**: Theme system with plugin support

```cpp
namespace presentation::theming {
    class ITheme {
    public:
        virtual ~ITheme() = default;
        virtual std::string getId() const = 0;
        virtual Color getBackgroundColor() const = 0;
        virtual Color getTextColor() const = 0;
        virtual std::string getFontName() const = 0;
        // ... more theme properties
    };
    
    class ThemeManager {
    public:
        void loadTheme(const std::string& themeId);
        void registerTheme(std::unique_ptr<ITheme> theme);
    };
}
```

---

## Rebranding Considerations

### 1. Branding Configuration

**Approach**: Externalize all branding elements to configuration files

```json
// resources/branding/config.json
{
  "application": {
    "name": "PowerPlus3",
    "displayName": "Power++",
    "version": "3.2.1.0",
    "vendor": "YourCompany",
    "copyright": "Copyright (c) 2025 YourCompany"
  },
  "ui": {
    "theme": "default",
    "iconSet": "default",
    "colorScheme": {
      "primary": "#0078D4",
      "secondary": "#106EBE",
      "accent": "#00BCF2"
    }
  },
  "resources": {
    "logo": "resources/branding/logos/logo.png",
    "icon": "resources/branding/icons/app.ico",
    "splashScreen": "resources/branding/images/splash.png"
  },
  "strings": {
    "aboutText": "resources/branding/strings/about.txt",
    "helpUrl": "https://yourcompany.com/help"
  }
}
```

### 2. Branding Service

**Implementation**: Centralized branding access

```cpp
namespace application::services {
    class BrandingService {
    public:
        static BrandingService& instance();
        
        std::string getApplicationName() const;
        std::string getDisplayName() const;
        std::string getVersion() const;
        std::string getVendor() const;
        
        std::string getLogoPath() const;
        std::string getIconPath() const;
        
        Color getPrimaryColor() const;
        Color getSecondaryColor() const;
        
        void loadBrandingConfig(const std::string& configPath);
        
    private:
        BrandingConfig config_;
    };
}
```

### 3. Resource Management

**Approach**: Resource loader with branding override support

```cpp
namespace infrastructure::resources {
    class ResourceLoader {
    public:
        std::string loadString(const std::string& key) const;
        std::vector<uint8_t> loadBinary(const std::string& path) const;
        HICON loadIcon(const std::string& iconId) const;
        
        // Check branding override first, then default
        std::string resolvePath(const std::string& relativePath) const;
    };
}
```

### 4. String Localization with Branding

**Approach**: Layered string resolution

```text
Priority order:
1. Branding-specific strings (resources/branding/strings/{lang}/)
2. Application strings (resources/strings/{lang}/)
3. Default/fallback strings
```

### 5. Rebranding Checklist

- [ ] Application name and display name
- [ ] Version information
- [ ] Copyright and vendor information
- [ ] Application icons (all sizes)
- [ ] Logo and splash screen
- [ ] UI color scheme
- [ ] About dialog content
- [ ] Help documentation
- [ ] Registry keys and paths
- [ ] Log file names and paths
- [ ] Installer branding
- [ ] License information

---

## Migration Phases

### Phase 1: Foundation Setup (Weeks 1-4)

**Goals:**

- Set up new project structure
- Create core domain models
- Establish build system
- Set up testing framework

**Tasks:**

1. Create new directory structure
2. Set up CMake/build configuration
3. Create core domain entities (ScheduleItem, HotkeyItem, etc.)
4. Create value objects (ClockTime, DateTime)
5. Define domain interfaces (repositories, services)
6. Set up unit testing framework
7. Create basic CI/CD pipeline

**Deliverables:**

- New project structure
- Core domain models (compilable but not integrated)
- Basic test framework
- Build system configuration

### Phase 2: Core Features Migration (Weeks 5-12)

**Goals:**

- Migrate power action execution
- Migrate configuration management
- Create basic UI framework

**Tasks:**

1. Implement power action domain models
2. Create power action executor interface
3. Implement Windows power action executor
4. Migrate configuration data models
5. Create configuration repository interface
6. Implement registry configuration repository
7. Create application services (PowerActionService, ConfigService)
8. Create basic UI framework (base dialog, main window)
9. Integrate new architecture with existing UI (gradual)

**Deliverables:**

- Working power action execution (new architecture)
- Configuration management (new architecture)
- Basic UI framework
- Integration with existing codebase

### Phase 3: Advanced Features Migration (Weeks 13-24)

**Goals:**

- Migrate scheduling system
- Migrate hotkey management
- Migrate reminder system

**Tasks:**

1. Migrate schedule domain models
2. Create schedule repository
3. Implement schedule service
4. Migrate hotkey domain models
5. Create hotkey registry interface
6. Implement Windows hotkey registry
7. Migrate reminder domain models
8. Create reminder service
9. Implement reminder renderers
10. Update UI to use new services

**Deliverables:**

- Complete scheduling system (new architecture)
- Complete hotkey management (new architecture)
- Complete reminder system (new architecture)
- All features working with new architecture

### Phase 4: UI and Integration (Weeks 25-32)

**Goals:**

- Modernize UI framework
- Complete system integration
- Enhance logging

**Tasks:**

1. Refactor UI to use new architecture
2. Implement view models (if using MVVM)
3. Complete system tray integration
4. Enhance logging infrastructure
5. Implement event logging
6. Performance optimization
7. UI/UX improvements

**Deliverables:**

- Modernized UI
- Complete system integration
- Enhanced logging system
- Performance optimizations

### Phase 5: Extension and Rebranding (Weeks 33-40)

**Goals:**

- Implement plugin system
- Create branding framework
- Final cleanup

**Tasks:**

1. Design plugin interface
2. Implement plugin manager
3. Create plugin loading mechanism
4. Implement branding service
5. Create branding configuration system
6. Migrate all branding elements
7. Remove old code
8. Final testing and documentation

**Deliverables:**

- Working plugin system
- Complete branding framework
- Clean codebase (old code removed)
- Complete documentation

---

## Risk Assessment

### High Risk

#### 1. **Data Migration**

- **Risk**: Loss of user configuration during migration
- **Mitigation**:
  - Create comprehensive migration tool
  - Support loading old registry format
  - Provide backup/restore functionality
  - Extensive testing with real user data

#### 2. **Feature Regression**

- **Risk**: Missing features or broken functionality
- **Mitigation**:
  - Maintain feature parity checklist
  - Comprehensive testing before each phase
  - Keep old code as fallback
  - Gradual migration (feature by feature)

#### 3. **Performance Degradation**

- **Risk**: New architecture slower than old code
- **Mitigation**:
  - Performance benchmarks before/after
  - Profile critical paths
  - Optimize hot paths
  - Maintain performance requirements

### Medium Risk

#### 4. **Complexity Increase**

- **Risk**: New architecture too complex
- **Mitigation**:
  - Keep architecture simple
  - Document design decisions
  - Code reviews
  - Training for team members

#### 5. **Timeline Overrun**

- **Risk**: Migration takes longer than planned
- **Mitigation**:
  - Phased approach (can stop at any phase)
  - Prioritize critical features
  - Regular progress reviews
  - Adjust scope if needed

### Low Risk

#### 6. **Team Resistance**

- **Risk**: Team prefers old code
- **Mitigation**:
  - Clear communication of benefits
  - Training and documentation
  - Gradual transition
  - Showcase improvements

---

## Implementation Roadmap

### Year 1: Foundation and Core Migration

#### **Q1: Foundation**

- Phase 1: Foundation Setup
- Begin Phase 2: Core Features Migration

#### **Q2: Core Features**

- Complete Phase 2: Core Features Migration
- Begin Phase 3: Advanced Features Migration

#### **Q3: Advanced Features**

- Continue Phase 3: Advanced Features Migration

#### **Q4: Integration**

- Complete Phase 3: Advanced Features Migration
- Begin Phase 4: UI and Integration

### Year 2: Polish and Extension

#### **Q1: UI Completion**

- Complete Phase 4: UI and Integration
- Begin Phase 5: Extension and Rebranding

#### **Q2: Extension System**

- Complete plugin system
- Begin branding framework

#### **Q3: Branding**

- Complete branding framework
- Rebranding implementation

#### **Q4: Finalization**

- Remove old code
- Complete documentation
- Release new version

---

## Migration Tools and Utilities

### 1. Configuration Migrator

```cpp
namespace tools::migration {
    class ConfigMigrator {
    public:
        Result<void, ErrorCode> migrateFromRegistry(
            const std::string& registryPath,
            const std::string& outputPath
        );
        
        Result<void, ErrorCode> migrateToNewFormat(
            const std::string& oldConfigPath,
            const std::string& newConfigPath
        );
        
        bool validateMigration(const std::string& configPath);
    };
}
```

### 2. Data Validator

```cpp
namespace tools::migration {
    class DataValidator {
    public:
        ValidationResult validateScheduleData(const ScheduleData& data);
        ValidationResult validateHotkeyData(const HotkeySetData& data);
        ValidationResult validateReminderData(const ReminderData& data);
    };
}
```

### 3. Compatibility Layer

```cpp
namespace infrastructure::compatibility {
    // Temporary compatibility layer during migration
    class LegacyConfigLoader {
    public:
        Result<ConfigData, ErrorCode> loadFromOldRegistry();
        Result<void, ErrorCode> saveToOldRegistry(const ConfigData& data);
    };
}
```

---

## Cross-Platform Migration Strategy

### Overview

This section provides detailed guidance for migrating PowerPlus3 from a Windows-only application to a cross-platform solution supporting Windows, Linux, and potentially macOS.

### Target Platforms

1. **Windows** (Primary - Maintain existing functionality)
   - Windows 7/8/10/11 (x64)
   - Modernize UI framework while maintaining compatibility
   - Preserve Registry-based storage (with JSON fallback)

2. **Linux** (Secondary - Desktop environments)
   - GNOME, KDE, XFCE support
   - Systemd/logind integration for power management
   - X11 and Wayland display server support
   - JSON-based configuration storage

3. **macOS** (Future - Optional)
   - Cocoa integration
   - macOS-specific power management APIs
   - System Preferences integration

### Migration Approach: Strangler Fig Pattern

Instead of a complete rewrite, use incremental migration:

1. **Build new architecture alongside old code**
2. **Gradually migrate features** from old to new
3. **Route functionality** through new architecture
4. **Remove old code** once migration is complete

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

### **Platform Abstraction Layer (PAL)**

The Platform Abstraction Layer provides interfaces for all platform-specific functionality:

#### **1. Power Management Interface**

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

#### **2. Configuration Storage Interface**

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

#### **3. Hotkey Management Interface**

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

#### **4. System Tray Interface**

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

### **Cross-Platform Refactoring Strategy**

#### **Phase 1: Extract Platform-Independent Core**

**Goal**: Separate business logic from platform-specific code

1. **Identify Platform-Independent Code**
   - Data structures (ScheduleItem, HotkeyItem, ReminderItem, ConfigData)
   - Business logic (scheduling algorithms, time calculations, repeat logic)
   - Domain models and value objects (ClockTime, DateTime, PwrRepeatSet)
   - Utility functions (string manipulation, time formatting, validation)

2. **Create Platform Abstraction Interfaces**
   - Define interfaces for all platform-specific operations
   - Create abstract base classes with pure virtual methods
   - Document expected behavior and contracts
   - Define error handling patterns

3. **Move Core Logic to Domain Layer**
   - Extract business logic from UI classes (CPowerPlusDlg, etc.)
   - Create service classes for feature orchestration
   - Remove direct Windows API calls from business logic
   - Remove MFC dependencies from domain layer

**Deliverables:**

- Platform-independent domain models
- Platform abstraction interfaces defined
- Business logic extracted to domain layer

#### **Phase 2: Implement Platform Abstraction Layer (PAL)**

**Goal**: Create platform-specific implementations while maintaining Windows compatibility

1. **Create Windows Implementation**
   - Implement all PAL interfaces using Windows APIs
   - Wrap existing Windows-specific code
   - Maintain backward compatibility with Registry storage
   - Support both Registry and JSON storage (transition period)

2. **Create Factory Pattern**
   - Platform detection at compile-time and runtime
   - Factory creates appropriate implementations
   - Dependency injection for services
   - Configuration-based platform selection

**Example Factory Implementation:**

```cpp
namespace infrastructure::platform {
    class PlatformFactory {
    public:
        static std::unique_ptr<IPowerManager> createPowerManager() {
            #ifdef _WIN32
                return std::make_unique<WindowsPowerManager>();
            #elif __linux__
                return std::make_unique<LinuxPowerManager>();
            #elif __APPLE__
                return std::make_unique<MacOSPowerManager>();
            #else
                #error "Unsupported platform"
            #endif
        }
        
        static std::unique_ptr<IConfigurationStorage> createStorage(
            StorageType type = StorageType::Auto
        ) {
            #ifdef _WIN32
                if (type == StorageType::Registry || type == StorageType::Auto) {
                    return std::make_unique<RegistryStorage>();
                }
            #endif
            // Default to JSON for cross-platform
            return std::make_unique<JsonFileStorage>();
        }
    };
}
```

**Deliverables:**

- Windows PAL implementations
- Factory pattern for platform detection
- Backward compatibility maintained

#### **Phase 3: UI Framework Migration**

**Goal**: Abstract UI layer for cross-platform support

1. **Abstract UI Components**
   - Create base classes for dialogs/views
   - Define common UI operations (show, hide, enable, disable)
   - Platform-specific implementations
   - View model pattern (optional, for MVVM)

2. **Choose Cross-Platform UI Framework**
   - **Qt 6** (Recommended):
     - Mature cross-platform framework
     - Excellent C++ support
     - Rich widget set
     - System integration APIs
     - Commercial and open-source licenses
   - **WxWidgets** (Alternative):
     - Native look and feel
     - Smaller footprint
     - Good Windows integration
   - **Native** (Platform-specific):
     - WinUI 3 for Windows
     - GTK for Linux
     - Cocoa for macOS
     - More platform-specific code required

**UI Migration Strategy:**

```cpp
// Platform-independent base
class MainWindowBase {
public:
    virtual void setupUI() = 0;
    virtual void connectSignals() = 0;
    virtual void updateScheduleList(const std::vector<ScheduleItem>& items) = 0;
};

// Qt implementation
class QtMainWindow : public QMainWindow, public MainWindowBase {
    // Qt-specific implementation
};

// Windows-specific (if using WinUI)
class WinUIMainWindow : public MainWindowBase {
    // WinUI-specific implementation
};
```

**Deliverables:**

- UI abstraction layer
- Cross-platform UI framework integrated
- All dialogs migrated to new framework

#### **Phase 4: Data Storage Migration**

**Goal**: Support multiple storage backends with migration path

1. **Abstract Storage Layer**
   - Create storage interface (IConfigurationStorage)
   - Support multiple backends (Registry, JSON, SQLite)
   - Migration utilities for existing data
   - Data validation and integrity checks

2. **Implement Cross-Platform Storage**
   - JSON file storage for cross-platform compatibility
   - Platform-specific storage locations:
     - Windows: `%APPDATA%\PowerPlus3\`
     - Linux: `~/.config/PowerPlus3/`
     - macOS: `~/Library/Application Support/PowerPlus3/`
   - Data format migration tools
   - Backup and restore functionality

**Storage Migration Tool:**

```cpp
namespace tools::migration {
    class StorageMigrator {
    public:
        // Migrate from Registry to JSON
        Result<void, ErrorCode> migrateRegistryToJson(
            const std::string& registryPath,
            const std::string& jsonPath
        );
        
        // Validate migrated data
        bool validateMigration(
            const std::string& oldPath,
            const std::string& newPath
        );
        
        // Backup before migration
        Result<void, ErrorCode> backupData(const std::string& source);
    };
}
```

**Deliverables:**

- Multiple storage backend support
- Migration tools
- Data validation utilities

#### **Phase 5: Linux Platform Implementation**

**Goal**: Implement Linux-specific functionality

1. **Power Management (Linux)**
   - Systemd/logind D-Bus interface
   - `systemctl` command execution (fallback)
   - Privilege handling (polkit)
   - Action support matrix:
     - Shutdown: ✅ (systemd)
     - Restart: ✅ (systemd)
     - Sleep: ✅ (logind)
     - Hibernate: ✅ (logind, if enabled)
     - Display Off: ✅ (xset, xrandr)
     - Sign Out: ⚠️ (session manager dependent)

2. **Hotkey Management (Linux)**
   - X11: `XGrabKey()` API
   - Wayland: Compositor-specific protocols (limited)
   - Key symbol mapping (X11 keysyms to virtual keys)
   - Background hotkey support (X11 only)

3. **System Tray (Linux)**
   - StatusNotifier protocol (modern)
   - Legacy libappindicator (fallback)
   - Desktop environment detection

4. **Configuration Storage (Linux)**
   - JSON files in `~/.config/PowerPlus3/`
   - XDG Base Directory Specification compliance
   - File permissions and security

**Deliverables:**

- Complete Linux implementation
- Linux-specific testing
- Documentation for Linux users

#### **Phase 6: Testing and Validation**

**Goal**: Ensure cross-platform compatibility and feature parity

1. **Unit Testing**
   - Test domain logic independently
   - Mock platform abstraction interfaces
   - Platform-agnostic test cases

2. **Integration Testing**
   - Test platform-specific implementations
   - Verify PAL interface contracts
   - Cross-platform behavior validation

3. **Platform-Specific Testing**
   - Windows: Test Registry storage, Windows APIs
   - Linux: Test systemd integration, X11/Wayland
   - macOS: Test Cocoa integration, macOS APIs (if implemented)

4. **Feature Parity Validation**
   - Compare functionality across platforms
   - Document platform-specific limitations
   - User acceptance testing

**Deliverables:**

- Comprehensive test suite
- Platform-specific test results
- Feature parity documentation

### **Project Structure for Cross-Platform**

```markdown
PowerPlus3/
├── src/
│   ├── domain/                          # Platform-independent domain logic
│   │   ├── entities/                    # Core business entities
│   │   │   ├── schedule/
│   │   │   │   ├── schedule_item.h
│   │   │   │   ├── schedule_item.cpp
│   │   │   │   ├── schedule_data.h
│   │   │   │   └── schedule_data.cpp
│   │   │   ├── hotkey/
│   │   │   │   ├── hotkey_item.h
│   │   │   │   └── hotkey_item.cpp
│   │   │   ├── reminder/
│   │   │   │   ├── reminder_item.h
│   │   │   │   └── reminder_item.cpp
│   │   │   └── config/
│   │   │       ├── app_config.h
│   │   │       └── app_config.cpp
│   │   ├── value_objects/               # Immutable value objects
│   │   │   ├── clock_time.h
│   │   │   ├── clock_time.cpp
│   │   │   ├── date_time.h
│   │   │   └── date_time.cpp
│   │   ├── services/                    # Domain services
│   │   │   ├── schedule_service.h
│   │   │   └── schedule_service.cpp
│   │   └── interfaces/                 # Repository/service interfaces
│   │       ├── schedule_repository.h
│   │       ├── config_repository.h
│   │       ├── power_manager_interface.h
│   │       ├── hotkey_manager_interface.h
│   │       └── system_tray_interface.h
│   │
│   ├── application/                     # Application layer (use cases)
│   │   ├── services/                    # Application services
│   │   │   ├── power_action_service.h
│   │   │   ├── power_action_service.cpp
│   │   │   ├── schedule_service.h
│   │   │   ├── schedule_service.cpp
│   │   │   ├── hotkey_service.h
│   │   │   ├── hotkey_service.cpp
│   │   │   ├── reminder_service.h
│   │   │   ├── reminder_service.cpp
│   │   │   ├── config_service.h
│   │   │   └── config_service.cpp
│   │   ├── dto/                         # Data transfer objects
│   │   │   ├── schedule_dto.h
│   │   │   ├── hotkey_dto.h
│   │   │   └── reminder_dto.h
│   │   └── use_cases/                  # Specific use cases
│   │       ├── execute_power_action.h
│   │       ├── execute_power_action.cpp
│   │       ├── create_schedule.h
│   │       └── create_schedule.cpp
│   │
│   ├── infrastructure/                  # Infrastructure layer
│   │   ├── persistence/                 # Data persistence
│   │   │   ├── registry/                 # Windows Registry (Windows only)
│   │   │   │   ├── registry_repository.h
│   │   │   │   ├── registry_repository.cpp
│   │   │   │   ├── registry_config_repository.h
│   │   │   │   └── registry_config_repository.cpp
│   │   │   ├── json/                    # JSON file storage (cross-platform)
│   │   │   │   ├── json_config_repository.h
│   │   │   │   ├── json_config_repository.cpp
│   │   │   │   ├── json_schedule_repository.h
│   │   │   │   └── json_schedule_repository.cpp
│   │   │   └── sqlite/                  # SQLite storage (optional)
│   │   │       ├── sqlite_repository.h
│   │   │       └── sqlite_repository.cpp
│   │   ├── platform/                    # Platform-specific code
│   │   │   ├── abstractions/            # Platform abstraction interfaces
│   │   │   │   ├── power_manager_interface.h
│   │   │   │   ├── hotkey_manager_interface.h
│   │   │   │   ├── system_tray_interface.h
│   │   │   │   ├── file_system_interface.h
│   │   │   │   └── timer_manager_interface.h
│   │   │   ├── windows/                 # Windows implementations
│   │   │   │   ├── windows_power_manager.h
│   │   │   │   ├── windows_power_manager.cpp
│   │   │   │   ├── windows_hotkey_manager.h
│   │   │   │   ├── windows_hotkey_manager.cpp
│   │   │   │   ├── windows_system_tray.h
│   │   │   │   ├── windows_system_tray.cpp
│   │   │   │   └── windows_file_system.h
│   │   │   ├── linux/                  # Linux implementations
│   │   │   │   ├── linux_power_manager.h
│   │   │   │   ├── linux_power_manager.cpp
│   │   │   │   ├── linux_hotkey_manager.h
│   │   │   │   ├── linux_hotkey_manager.cpp
│   │   │   │   ├── linux_system_tray.h
│   │   │   │   ├── linux_system_tray.cpp
│   │   │   │   └── linux_file_system.h
│   │   │   └── macos/                  # macOS implementations (future)
│   │   │       ├── macos_power_manager.h
│   │   │       └── macos_power_manager.cpp
│   │   ├── logging/                     # Logging infrastructure
│   │   │   ├── logger.h
│   │   │   ├── logger.cpp
│   │   │   ├── event_logger.h
│   │   │   └── event_logger.cpp
│   │   ├── serialization/              # Data serialization
│   │   │   ├── config_serializer.h
│   │   │   ├── config_serializer.cpp
│   │   │   ├── json_serializer.h
│   │   │   └── json_serializer.cpp
│   │   └── factory/                    # Platform factory
│   │       ├── platform_factory.h
│   │       └── platform_factory.cpp
│   │
│   ├── presentation/                    # Presentation layer (UI)
│   │   ├── framework/                   # UI framework base
│   │   │   ├── base_dialog.h
│   │   │   ├── base_dialog.cpp
│   │   │   ├── base_window.h
│   │   │   └── base_window.cpp
│   │   ├── qt/                         # Qt UI implementation
│   │   │   ├── main_window/
│   │   │   │   ├── main_window.h
│   │   │   │   └── main_window.cpp
│   │   │   ├── schedule_dialog/
│   │   │   │   ├── schedule_dialog.h
│   │   │   │   └── schedule_dialog.cpp
│   │   │   └── hotkey_dialog/
│   │   │       ├── hotkey_dialog.h
│   │   │       └── hotkey_dialog.cpp
│   │   ├── windows/                    # Windows-specific UI (optional)
│   │   │   └── [WinUI implementation if needed]
│   │   ├── view_models/                # View models (if using MVVM)
│   │   │   ├── main_window_view_model.h
│   │   │   └── main_window_view_model.cpp
│   │   └── controls/                   # Custom UI controls
│   │       ├── grid_control.h
│   │       └── grid_control.cpp
│   │
│   ├── plugins/                        # Plugin system (optional)
│   │   ├── plugin_interface.h
│   │   ├── plugin_manager.h
│   │   └── plugin_manager.cpp
│   │
│   └── common/                         # Shared utilities
│       ├── types/                      # Common types
│       │   ├── result.h                # Result<T, E> type
│       │   ├── optional.h              # Enhanced optional
│       │   └── error_code.h
│       ├── errors/                     # Error handling
│       │   ├── error_code.h
│       │   └── error_code.cpp
│       └── utils/                     # Utility functions
│           ├── string_utils.h
│           └── string_utils.cpp
│
├── include/                            # Public headers
│   └── [mirror of src/ structure]
│
├── tests/                              # Test code
│   ├── unit/                           # Unit tests
│   │   ├── domain/
│   │   ├── application/
│   │   └── infrastructure/
│   ├── integration/                    # Integration tests
│   ├── platform/                       # Platform-specific tests
│   │   ├── windows/
│   │   ├── linux/
│   │   └── macos/
│   └── e2e/                           # End-to-end tests
│
├── tools/                              # Development tools
│   ├── migration/                     # Migration utilities
│   │   ├── config_migrator.h
│   │   ├── config_migrator.cpp
│   │   ├── registry_to_json.h
│   │   └── registry_to_json.cpp
│   └── codegen/                       # Code generation
│
├── resources/                          # Resources
│   ├── icons/
│   ├── images/
│   ├── strings/                        # Localization strings
│   │   ├── en/
│   │   ├── vi/
│   │   └── zh-CN/
│   └── branding/                      # Branding assets
│       ├── logos/
│       ├── themes/
│       └── config.json                # Branding configuration
│
├── docs/                              # Documentation
│   ├── design/
│   ├── migration/
│   └── api/
│
├── third_party/                       # Third-party libraries
│
├── cmake/                             # CMake build files
│   ├── FindQt6.cmake
│   ├── PlatformDetection.cmake
│   └── CompilerOptions.cmake
│
├── CMakeLists.txt                     # CMake build configuration
├── conanfile.txt                      # Conan dependencies (optional)
└── README.md
```

### **Technology Stack Recommendations**

#### **For Cross-Platform C++:**

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

### **Cross-Platform Implementation Details**

#### **1. Power Management Implementation**

**Windows:**

```cpp
class WindowsPowerManager : public IPowerManager {
public:
    Result<void, ErrorCode> executeAction(Action action) override {
        switch (action) {
            case Action::Shutdown:
            case Action::Restart:
            case Action::SignOut:
                return executeExitWindows(action);
            case Action::Sleep:
                return executeSleep();
            case Action::Hibernate:
                return executeHibernate();
            case Action::DisplayOff:
                return executeDisplayOff();
        }
    }
    
private:
    Result<void, ErrorCode> executeExitWindows(Action action) {
        // Adjust privileges, call ExitWindowsEx()
    }
    
    Result<void, ErrorCode> executeSleep() {
        // Call SetSuspendState(false, false, false)
    }
};
```

**Linux:**

```cpp
class LinuxPowerManager : public IPowerManager {
public:
    Result<void, ErrorCode> executeAction(Action action) override {
        switch (action) {
            case Action::Shutdown:
                return executeSystemdCommand("poweroff");
            case Action::Restart:
                return executeSystemdCommand("reboot");
            case Action::Sleep:
                return executeLogindSuspend();
            case Action::Hibernate:
                return executeLogindHibernate();
            case Action::DisplayOff:
                return executeX11DisplayOff();
        }
    }
    
private:
    Result<void, ErrorCode> executeSystemdCommand(const std::string& cmd) {
        // Use systemd D-Bus interface or systemctl command
    }
    
    Result<void, ErrorCode> executeLogindSuspend() {
        // Use logind D-Bus interface
    }
};
```

#### **2. Hotkey Management Implementation**

**Windows:**

```cpp
class WindowsHotkeyManager : public IHotkeyManager {
public:
    Result<void, ErrorCode> registerHotkey(
        unsigned id, 
        const KeyCombination& combo
    ) override {
        DWORD modifiers = convertModifiers(combo.modifiers);
        DWORD vk = convertKeyCode(combo.key);
        modifiers |= MOD_NOREPEAT;
        
        if (!RegisterHotKey(windowHandle_, id, modifiers, vk)) {
            return Result<void, ErrorCode>::error(GetLastError());
        }
        return Result<void, ErrorCode>::success();
    }
};
```

**Linux (X11):**

```cpp
class LinuxHotkeyManager : public IHotkeyManager {
public:
    Result<void, ErrorCode> registerHotkey(
        unsigned id, 
        const KeyCombination& combo
    ) override {
        Display* display = XOpenDisplay(NULL);
        if (!display) {
            return Result<void, ErrorCode>::error(ERR_NO_DISPLAY);
        }
        
        KeySym keysym = convertKeyCodeToX11(combo.key);
        KeyCode keycode = XKeysymToKeycode(display, keysym);
        unsigned int modifiers = convertModifiersToX11(combo.modifiers);
        
        XGrabKey(display, keycode, modifiers, rootWindow_, 
                 False, GrabModeAsync, GrabModeAsync);
        
        return Result<void, ErrorCode>::success();
    }
};
```

#### **3. Configuration Storage Implementation**

**Windows Registry:**

```cpp
class RegistryStorage : public IConfigurationStorage {
public:
    Result<ConfigData, ErrorCode> loadConfig() override {
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, registryPath_, 0, 
                        KEY_READ, &hKey) != ERROR_SUCCESS) {
            return Result<ConfigData, ErrorCode>::error(ERR_REG_OPEN);
        }
        
        ConfigData config;
        // Read registry values...
        
        RegCloseKey(hKey);
        return Result<ConfigData, ErrorCode>::success(config);
    }
};
```

**JSON File (Cross-Platform):**

```cpp
class JsonFileStorage : public IConfigurationStorage {
public:
    Result<ConfigData, ErrorCode> loadConfig() override {
        auto configPath = getConfigFilePath();
        
        std::ifstream file(configPath);
        if (!file.is_open()) {
            return Result<ConfigData, ErrorCode>::error(ERR_FILE_NOT_FOUND);
        }
        
        nlohmann::json json;
        file >> json;
        
        ConfigData config;
        config.fromJson(json);
        
        return Result<ConfigData, ErrorCode>::success(config);
    }
    
private:
    std::filesystem::path getConfigFilePath() {
        #ifdef _WIN32
            auto appData = std::getenv("APPDATA");
            return std::filesystem::path(appData) / "PowerPlus3" / "config.json";
        #elif __linux__
            auto home = std::getenv("HOME");
            return std::filesystem::path(home) / ".config" / "PowerPlus3" / "config.json";
        #elif __APPLE__
            auto home = std::getenv("HOME");
            return std::filesystem::path(home) / "Library" / "Application Support" 
                   / "PowerPlus3" / "config.json";
        #endif
    }
};
```

### **Migration Challenges and Solutions**

#### **Challenge 1: Power Management Differences**

**Problem:**

- Windows: Direct API calls (`ExitWindowsEx`, `SetSuspendState`)
- Linux: Systemd/logind D-Bus interface or command-line tools
- macOS: Cocoa APIs or `pmset` commands

**Solution:**

- Abstract all power operations through `IPowerManager`
- Platform-specific implementations
- Feature detection for supported actions
- Graceful degradation for unsupported actions

#### **Challenge 2: Hotkey System Variations**

**Problem:**

- Windows: `RegisterHotKey()` API, keyboard hooks
- Linux: X11 `XGrabKey()`, Wayland protocols (limited)
- macOS: Carbon/Cocoa hotkey APIs

**Solution:**

- Abstract hotkey registration
- Key code mapping between platforms
- Background hotkey support varies by platform
- Document platform limitations

#### **Challenge 3: System Tray Implementation**

**Problem:**

- Windows: `Shell_NotifyIcon` API
- Linux: StatusNotifier protocol, libappindicator
- macOS: `NSStatusItem` API

**Solution:**

- Use Qt's `QSystemTrayIcon` (cross-platform)
- Or implement platform-specific wrappers
- Feature detection for availability

#### **Challenge 4: Configuration Storage**

**Problem:**

- Windows: Registry (hierarchical, binary-safe)
- Linux/macOS: Files (JSON, INI, etc.)

**Solution:**

- JSON file storage for cross-platform compatibility
- Registry support for Windows (optional, for backward compatibility)
- Migration tool for existing Registry data
- Platform-specific storage locations

### **Build System Configuration**

#### **CMake Cross-Platform Setup**

```cmake
cmake_minimum_required(VERSION 3.20)
project(PowerPlus3 VERSION 3.3.0 LANGUAGES CXX)

# Platform detection
if(WIN32)
    set(PLATFORM_WINDOWS ON)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX ON)
    add_definitions(-DPLATFORM_LINUX)
elseif(APPLE)
    set(PLATFORM_MACOS ON)
    add_definitions(-DPLATFORM_MACOS)
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

# Platform-specific sources
if(PLATFORM_WINDOWS)
    add_subdirectory(src/infrastructure/platform/windows)
    add_subdirectory(src/infrastructure/persistence/registry)
elseif(PLATFORM_LINUX)
    add_subdirectory(src/infrastructure/platform/linux)
    # Find X11 for hotkey support
    find_package(X11 REQUIRED)
elseif(PLATFORM_MACOS)
    add_subdirectory(src/infrastructure/platform/macos)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
endif()

# Common sources
add_subdirectory(src/domain)
add_subdirectory(src/application)
add_subdirectory(src/infrastructure/persistence/json)
add_subdirectory(src/presentation/qt)

# Tests
enable_testing()
add_subdirectory(tests)
```

---

## Success Criteria

### Phase 1 Success Criteria

- [ ] New project structure created
- [ ] Core domain models compile and pass tests
- [ ] Build system works
- [ ] Test framework operational

### Phase 2 Success Criteria

- [ ] Power actions work with new architecture
- [ ] Configuration management works
- [ ] Basic UI framework functional
- [ ] Integration with existing code successful

### Phase 3 Success Criteria

- [ ] All features migrated to new architecture
- [ ] Feature parity maintained
- [ ] All tests pass
- [ ] Performance acceptable

### Phase 4 Success Criteria

- [ ] UI modernized
- [ ] System integration complete
- [ ] Logging enhanced
- [ ] User experience improved

### Phase 5 Success Criteria

- [ ] Plugin system working
- [ ] Branding framework complete
- [ ] Old code removed
- [ ] Documentation complete
- [ ] Ready for release

---

## Conclusion

This migration and redesign guide provides a comprehensive roadmap for transforming PowerPlus3 into a modern, extensible, and maintainable application. The phased approach minimizes risk while enabling continuous delivery of value.

**Key Takeaways:**

1. **Start with domain**: Build solid foundation with clean domain models
2. **Migrate incrementally**: One feature at a time, maintain compatibility
3. **Test thoroughly**: Comprehensive testing at each phase
4. **Document everything**: Keep documentation up-to-date
5. **Plan for extension**: Design for future growth and customization

**Next Steps:**

1. Review and approve this guide
2. Set up development environment
3. Begin Phase 1: Foundation Setup
4. Establish regular review meetings
5. Track progress against milestones

---

*This document is a living document and should be updated as the migration progresses and new insights are gained.*
