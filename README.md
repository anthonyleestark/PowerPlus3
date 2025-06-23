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

### Version: 3.1 (v3.1.1.0) - Release Date: Sep 2024  

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

### Version: 3.2 (v3.2.1.0) - Release Date: June 2025  

**Improvements and corrections:**
 - Improvement: Change some default option states
 - Improvement: Re-design UI for HotkeySet and LogViewer dialogs
 - Improvement: DebugView dialog context-menu
 - Improvement: Save layout changes for dialog Grid tables
 - Improvement: Save event and history logging output in YAML format
 - Improvement: Embed help files and changelog files into executable file
 - Adjustment: Write the application event log to one file per month instead of one file per day
 - Fixed a bug where unexpected false error messages are sometimes displayed when executing Power Actions
 - Fixed a bug where Power Reminder message is not displayed when restarting computer (or after shutdown)
 - Fixed a bug where Trace and Debug logging access incorrect folder path when auto-start with Windows
 - Fixed a bug where Access denied error when Trace and Debug logging write logs continuously
 - Fixed a bug where data loading errors are displayed when launching application for the first time
 - Fixed a bug where Notify icon is lost after Windows taskbar crashes and being recreated
 - Fixed a bug where feature dialogs are not closed when exitting application via Notify menu
 - Fixed a bug where Reminder messages are not closed when exitting application via Notify menu
 - Fixed a bug where multiple Reminder messages block each other when displaying
 - Refactored and updated source code from top to bottom so that everything will work the same

**New feature(s):**
 - Power Reminder: Message snoozing, RepeatSet and message style customization
 - Schedule: Supports Multiple schedule actions
 - HotkeySet: Allows using background Hotkeys when the screen is locked

## Screenshots
<p align="center">
  <img src="https://github.com/user-attachments/assets/3f8795a5-3598-4c0d-b161-ffa142820a70" alt="Main window (Collapsed)" width="480"/><br>
  <em>Main window (Collapsed)</em>
</p>
<p align="center">
	<img src="https://github.com/user-attachments/assets/0a01768e-4fc0-4c5a-b80b-ba62ca50aa8c" alt="Main window (Expanded)" width=480 /><br>
	<em>Main window (Expanded)</em>
</p>
<p align="center">
	<img src="https://github.com/user-attachments/assets/e77be3c9-878b-47fe-b896-ebf0248fb376" alt="Schedule dialog" width="480"/><br>
	<em>Schedule dialog</em>
</p>
<p align="center">
	<img src="https://github.com/user-attachments/assets/3c08d0f6-e7f4-44f4-975b-79dcb8d747e5" alt="HotkeySet dialog" width="480"/><br>
	<em>HotkeySet dialog</em>
</p>
<p align="center">
	<img src="https://github.com/user-attachments/assets/a98e5086-8062-4cc4-a6e1-913e826803ee" alt="Power Reminder dialog" width="640"/><br>
	<em>Power Reminder dialog</em>
</p>
<p align="center">
	<img src="https://github.com/user-attachments/assets/aac17b5a-94a4-4ddc-b12d-7b9c9749e046" alt="Reminder message (Dialog Box style)" width="480"/><br>
	<em>Reminder message (Dialog Box style)</em>
</p>

## Build & usage
- Download the source code and open the solution/project file (.sln/.vcxproj) with Visual Studio.
- Recommend building and running with Visual Studio version 2019 or later.
- After compiling and building, run the program by opening the **power++.exe** executable file

## Releases
Click [here](https://github.com/anthonyleestark/PowerPlus3/releases/download/v3.2.1.0/PowerPlus3.zip) to download the latest version.

## License
This repository is open-source and available under the [BSD-3-Clause license](https://opensource.org/license/bsd-3-clause).
