# Power++ v3.x
Based on Power++ v2.0 STL, upgraded since Apr-2024.  
[Click here](https://github.com/anthonyleestark/PowerPlus) to see the original source code of version v2.0

## Changes and updates
### Version: 3.0 (v3.0.1.0) - Release Date: June 2024  

**Improvements and corrections:**
- Fixed a bug that action schedule did not executed correctly sometimes
- Fixed a bug that action history time value is not correct
- Fixed a bug that confirmation message does not show up when switched to desktop
- Redesigned action schedule features and fixed some bugs
- Improved language switching function
- View change log function for Help dialog
- Refactored and updated source code from top to bottom so that everything will work the same

**New feature(s):**
- Support middle mouse button action
- Allow saving app event logs
- Support Logviewer function to view app event logs
- Reload settings feature
- Allow running with admin privileges at startup
- Allow notifying and canceling schedule
- System background hotkey feature
- Restart with admin privileges feature

### Version: 3.1 (v3.1.1.0) - Release Date: Oct 2024  

**Improvements and corrections:**
- Improvement: Changed caption of some dialog items
- Improvement: Bypass action confirmation for background hotkey feature
- Fixed a bug that multiple instances of a child dialog can be running at the same time
- Fixed a bug that some dialog item IDs are not displayed in app event logs
- Fixed a bug that some dialog item enable/disable event logs are recorded continuously
- Fixed a bug that action history content is empty or incorrect sometimes
- Fixed a bug that unable to view action log file from notify menu
- Improved HotkeySet function: Data serialization and register/unregister hotkey processing
- Improved hotkey validation: Not allow adding existed system key shortcuts
- Improved logging function: Output data change logs
- Improved trace and debug logging function

**New features:**
- Power Reminder feature
- Restart application feature
- Support Chinese language interface
- Debug command functions

### Version: 3.2 (v3.2.1.0) - In development  

## Build & usage
- Download the source code and open the solution/project file (.sln/.vcxproj) with Visual Studio.
- Recommend building and running with Visual Studio version 2019 or later.
- After compiling and building, run the program by opening the **PowerPlus3.exe** executable file
