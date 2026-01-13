/**
 * @file		PowerPlusDlgFunc.cpp
 * @brief		Define necessary functions using for main window
 * @author		AnthonyLeeStark
 * @date		2024.02.24
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"
#include "Dialogs/AboutDlg.h"
#include "Dialogs/MultiScheduleDlg.h"
#include "Dialogs/LogViewerDlg.h"
#include "Dialogs/HotkeySetDlg.h"
#include "Dialogs/PwrReminderDlg.h"
#include "Dialogs/ReminderMsgDlg.h"
#include "Dialogs/DebugTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MapTable;
using namespace Language;
using namespace AppCore;


/**
 * @brief	Process app debug commands
 * @param	commandString - Debug command string
 * @param	errorCode	  - Returned error code
 * @return	true/false
 */
bool CPowerPlusDlg::processDebugCommand(const wchar_t* commandString, DWORD& errorCode)
{
	String debugCommand(commandString);

	// If debug command is empty, do nothing
	if (debugCommand.isEmpty()) {
		// Error: Debug command empty
		errorCode = APP_ERROR_DBG_EMPTY;
		TRACE("Debug command is empty!!!");
		return false;
	}

	// Get application class
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) {
		// Error: Get app failed
		errorCode = APP_ERROR_DBG_GETAPP_FAILED;
		// Trace error
		TRACE_ERROR("Error: Get app pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Format debug command
	debugCommand.toLower();

	// Debug log output result
	String logOutputResult;

	// Return result
	bool returnFlag = true;

	// No replying flag
	bool isNoReply = true;
	bool isNoReplySilent = false;

	// Invalid command flag
	bool isInvalidCmdFlag = false;

	/*********************************************************************/
	/*																	 */
	/*					Process basic debug commands				     */
	/*																	 */
	/*********************************************************************/

	if (!_tcscmp(debugCommand, _T("exit"))) {
		// Exit application
		exitApp(ExitCode::FromDebugCommand);
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("reload"))) {
		// Reload settings
		reloadSettings();
		logOutputResult.format(_T("Settings reloaded!!!"));
		outputDebugLog(logOutputResult, DebugTestTool);
		isNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("setdefault"))) {
		// Set default settings
		setDefaultConfig();
		logOutputResult.format(_T("Reset default settings!!!"));
		outputDebugLog(logOutputResult, DebugTestTool);
		isNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("restartapp"))) {
		// Restart app (non-admin)
		requestRestartApp(IDD_DEBUGTEST_DLG, false);
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("restartadmin"))) {
		// Restart app (as admin)
		requestRestartApp(IDD_DEBUGTEST_DLG, true);
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("logviewer"))) {
		// Open Logviewer dialog
		openChildDialogEx(IDD_LOGVIEWER_DLG);
		logOutputResult.format(_T("Logviewer opened!!!"));
		outputDebugLog(logOutputResult, DebugTestTool);
		isNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("hksupdate"))) {
		// Update HotkeySet settings
		PostMessage(SM_APP_UPDATE_HOTKEYSETDATA);
		logOutputResult.format(_T("HotkeySet data updated!!!"));
		outputDebugLog(logOutputResult, DebugTestTool);
		isNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("rmdupdate"))) {
		// Update Power Reminder data
		PostMessage(SM_APP_UPDATE_PWRREMINDERDATA);
		logOutputResult.format(_T("Power Reminder data updated!!!"));
		outputDebugLog(logOutputResult, DebugTestTool);
		isNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("bakconfig"))) {
		// Backup configuration
		BackupSystem::RegistryExport();
		logOutputResult.format(_T("Config backed-up!!!"));
		outputDebugLog(logOutputResult, DebugTestTool);
		isNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("viewbakconfig"))) {
		// View backup configuration file
		openTextFileToView(Constant::File::Name::Backup_Config, Constant::File::Extension::Reg);
		logOutputResult.format(_T("Opening backup config file..."));
		outputDebugLog(logOutputResult, DebugTestTool);
		isNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("viewhistory"))) {
		// View action history log file
		openTextFileToView(Constant::File::Name::AppHistory, Constant::File::Extension::Log, Constant::Folder::Log);
		logOutputResult.format(_T("Opening action history log file..."));
		outputDebugLog(logOutputResult, DebugTestTool);
		isNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("clrscr"))) {
		// Clear debug view screen buffer
		HWND debugTestDlgHandle = AppCore::findDebugTestDlg();
		if (debugTestDlgHandle != NULL) {
			// Post clear screen message
			::PostMessage(debugTestDlgHandle, SM_WND_DEBUGVIEW_CLRSCR, NULL, NULL);
		}
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("close"))) {
		// Close DebugTest dialog if opening
		SDialog* debugTestDlgPtr = theAppPtr->getDebugTestDlg();
		if (debugTestDlgPtr != NULL) {
			debugTestDlgPtr->ShowWindow(SW_HIDE);
			debugTestDlgPtr->PostMessage(SM_APP_DEBUGCMD_NOREPLY);
		}
		return true;
	}

	/*********************************************************************/
	/*																	 */
	/*			Process advanced/multi-token debug commands				 */
	/*																	 */
	/*********************************************************************/

	// Make token list (break string buffer)
	TokenList tokenList = debugCommand.tokenize(_T("=,;:-_"));
	size_t tokenCount = tokenList.size();

	// Validate number of tokens
	if (tokenCount <= 0) {
		// Error: Tokenization failed
		errorCode = APP_ERROR_DBG_TOKENIZATION_FAILED;
		return false;
	}

	String tempString = Constant::String::Empty;

	// Process debug commands by tokens
	if (!_tcscmp(tokenList.at(0).c_str(), _T("test"))) {
		// Debug command token test
		if (tokenCount > 1) {
			// Prepare for replying
			isNoReply = false;	// Reset flag
			// Print token list
			int actualTokenCount = (tokenCount - 1);
			outputDebugLogFormat(_T("Token number: %d"), actualTokenCount);
			for (int tokenIndex = 0; tokenIndex < actualTokenCount; tokenIndex++) {
				tempString = tokenList.at(tokenIndex + 1);
				outputDebugLogFormat(_T("Token[%d]: %s"), tokenIndex, tempString.getString());
			}
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("dmytest"))) {
		// Set dummy test mode
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("on")))) {
			setDummyTestMode(true);
			outputDebugLog(_T("DummyTest ON"));
			isNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("off")))) {
			setDummyTestMode(false);
			outputDebugLog(_T("DummyTest OFF"));
			isNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
			setDummyTestMode(DEFAULT_DUMMYTEST);
			outputDebugLog(_T("DummyTest reset"));
			isNoReply = false;	// Reset flag
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("dmyteststs"))) {
		// Get dummy test mode
		int returnValue = getDummyTestMode();
		outputDebugLogFormat(_T("DummyTest status = %d"), returnValue);
		isNoReply = false;	// Reset flag
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("debugmode"))) {
		// Set debug mode
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("on")))) {
			setDebugMode(true);
			outputDebugLog(_T("DebugMode ON"));
			isNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("off")))) {
			setDebugMode(false);
			outputDebugLog(_T("DebugMode OFF"));
			isNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
			setDebugMode(DEFAULT_DEBUGMODE);
			outputDebugLog(_T("DebugMode reset"));
			isNoReply = false;	// Reset flag
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("debugmodests"))) {
		// Get debug mode
		int returnValue = getDebugMode();
		outputDebugLogFormat(_T("DebugMode status = %d"), returnValue);
		isNoReply = false;	// Reset flag
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("debuglog"))) {
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("output")))) {
			// Set debug log output target
			if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("default")))) {
				setDebugOutputTarget(DefaultOutput);
				outputDebugLog(_T("Debug log output target changed"));
				isNoReply = false;	// Reset flag
			}
			else if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("tofile")))) {
				setDebugOutputTarget(DebugInfoFile);
				outputDebugLog(_T("Debug log output target changed"));
				isNoReply = false;	// Reset flag
			}
			else if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("todbtool")))) {
				setDebugOutputTarget(DebugTestTool);
				outputDebugLog(_T("Debug log output target changed"));
				isNoReply = false;	// Reset flag
			}
			else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
				setDebugOutputTarget(DEFAULT_DEBUGOUTPUT);
				outputDebugLog(_T("Debug log output target reset"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("getoutput")))) {
			// Get debug log output target
			int returnValue = getDebugOutputTarget();
			outputDebugLogFormat(_T("DebugLogOutputTarget = %d"), returnValue);
			isNoReply = false;	// Reset flag
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("testfeature"))) {
		// Set test feature enable
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("on")))) {
			setTestFeatureEnable(true);
			outputDebugLog(_T("TestFeature ON"));
			isNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("off")))) {
			setTestFeatureEnable(false);
			outputDebugLog(_T("TestFeature OFF"));
			isNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
			setTestFeatureEnable(DEFAULT_TESTFEATURE);
			outputDebugLog(_T("TestFeature reset"));
			isNoReply = false;	// Reset flag
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("testfeaturests"))) {
		// Get test feature enable
		int returnValue = getTestFeatureEnable();
		outputDebugLogFormat(_T("TestFeature status = %d"), returnValue);
		isNoReply = false;	// Reset flag
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("logtest"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("trcerr")))) {
			// Trace error log test
			int value = _tstoi(tokenList.at(2).c_str());
			if (value >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				isNoReply = false;	// Reset flag
				if (value >= 50) {
					// Output waiting notification
					outputDebugLog(_T("Wait for a while!!!"), DebugTestTool);
					AppCore::waitMessage(SM_WND_DEBUGOUTPUT_DISP); // wait for the notification displaying
				}
				for (int i = 0; i < value; i++) {
					// Write test trace error log
					TRACE_FORMAT("[traceError Test] Loop time: %d", i);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				outputDebugLog(_T("TraceErrorLog test done!!!"), DebugTestTool);
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("trcdebug")))) {
			// Trace debug log test
			int value = _tstoi(tokenList.at(2).c_str());
			if (value >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				isNoReply = false;	// Reset flag
				if (value >= 50) {
					// Output waiting notification
					outputDebugLog(_T("Wait for a while!!!"), DebugTestTool);
					AppCore::waitMessage(SM_WND_DEBUGOUTPUT_DISP); // wait for the notification displaying
				}
				String formatString;
				for (int i = 0; i < value; i++) {
					// Write test trace debug log
					formatString.format(_T("[TraceDebugLog Test] Loop time: %d"), i);
					DebugLogger::getDebugLogger().writeTraceDebugLogFile(formatString);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				outputDebugLog(_T("TraceDebugLog test done!!!"), DebugTestTool);
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("debuginfo")))) {
			// Output debug info log test
			int value = _tstoi(tokenList.at(2).c_str());
			if (value >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				isNoReply = false;	// Reset flag
				if (value >= 50) {
					// Output waiting notification
					outputDebugLog(_T("Wait for a while!!!"), DebugTestTool);
					AppCore::waitMessage(SM_WND_DEBUGOUTPUT_DISP); // wait for the notification displaying
				}
				String formatString;
				for (int i = 0; i < value; i++) {
					// Write test debug info log
					formatString.format(_T("[OutputDebugInfo Test] Loop time: %d"), i);
					outputDebugLog(formatString, DebugTestTool);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				outputDebugLog(_T("DebugInfoLog test done!!!"), DebugTestTool);
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("errtest"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("code")))) {
			// Test show error message
			int errorCodeVal = _tstoi(tokenList.at(2).c_str());
			if (errorCodeVal >= 0) {
				if (getAppOption(AppOptionID::showErrorMessage) == false) {
					outputDebugLog(_T("Show error message OFF"));
					isNoReply = false;	// Reset flag
				}
				else {
					// Show error message
					showErrorMessage(errorCodeVal);
					outputDebugLog(_T("Error message showed!!!"));
					isNoReply = false;	// Reset flag
				}
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("appeventlog"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("clear")))) {
			// Clear all app event log data in memory
			Logger* appEventLoggerPtr = ((CPowerPlusApp*)AfxGetApp())->getAppEventLog();
			if (appEventLoggerPtr != NULL) {
				appEventLoggerPtr->init();
				outputDebugLog(_T("App event log data cleared"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("write")))) {
			// Force writing app event log data from memory to file
			Logger* appEventLoggerPtr = ((CPowerPlusApp*)AfxGetApp())->getAppEventLog();
			if (appEventLoggerPtr != NULL) {
				bool returnFlag = appEventLoggerPtr->write();
				if (returnFlag == true) {
					// Write log succeeded
					outputDebugLog(_T("App event log data written"));
					isNoReply = false;	// Reset flag
				}
				else {
					// Write log failed
					outputDebugLog(_T("Write app event log data failed"));
					isNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("del")))) {
			// Delete app event log files
			String keyName = tokenList.at(2).c_str();;
			if (!_tcscmp(tokenList.at(2).c_str(), _T("all"))) {
				keyName = _T("AppEventLog");
			}
			CFileFind Finder;
			String fileName = StringUtils::makeFilePath(StringUtils::getSubFolderPath(Constant::Folder::Log), _T("*"), Constant::File::Extension::Log);
			bool finderRetFlag = Finder.FindFile(fileName);
			if (!keyName.isEmpty()) {
				int delFileCount = 0;
				while (finderRetFlag == true) {
					finderRetFlag = Finder.FindNextFile();
					fileName = Finder.GetFileName().GetString();
					if (fileName.find(keyName) != Constant::InvalidInteger) {
						// Delete file
						CFile::Remove(Finder.GetFilePath());
						delFileCount++;	// Increase counter
					}
				}
				if (delFileCount > 0) {
					// Output number of deleted files
					logOutputResult.format(_T("App event log file(s) deleted (Count=%d)"), delFileCount);
					outputDebugLog(logOutputResult, DebugTestTool);
					isNoReply = false;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("reminderdisp"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("id")))) {
			int itemId = _tstoi(tokenList.at(2).c_str());
			// Find and display reminder item by ID
			bool finderRetFlag = false;
			PwrReminderItem tempItem;
			int itemNum = reminderData_.getItemNum();
			for (int index = 0; index < itemNum; index++) {
				tempItem = reminderData_.getItemAt(index);
				if (tempItem.getItemId() == static_cast<unsigned>(itemId)) {
					finderRetFlag = true;
					displayPwrReminder(tempItem);
					outputDebugLog(_T("Reminder item displayed!!!"));
					isNoReply = false;	// Reset flag
					break;
				}
			}
			if (finderRetFlag == false) {
				// Item not found
				outputDebugLog(_T("Reminder item not found"));
				isNoReply = false;	// Reset flag
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("saveglobaldata"))) {
		if (tokenCount == 1) {
			// Save all global data variables
			theAppPtr->saveGlobalData(0xFF);
			logOutputResult.format(_T("Global variables stored"));
			outputDebugLog(logOutputResult, DebugTestTool);
			isNoReply = false;	// Reset flag
		}
		else if (tokenCount == 2) {
			if (!_tcscmp(tokenList.at(1).c_str(), _T("all"))) {
				// Save all global variables
				theAppPtr->saveGlobalData(0xFF);
				logOutputResult.format(_T("Global variables stored"));
				outputDebugLog(logOutputResult, DebugTestTool);
				isNoReply = false;	// Reset flag
			}
			else if (!_tcscmp(tokenList.at(1).c_str(), _T("dbtest"))) {
				// Save debugging/testing config (global variables)
				theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_DEBUGTEST);
				logOutputResult.format(_T("Debug/test config stored"));
				outputDebugLog(logOutputResult, DebugTestTool);
				isNoReply = false;	// Reset flag
			}
			else if (!_tcscmp(tokenList.at(1).c_str(), _T("appflags"))) {
				// Save app flags (global variables)
				theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
				logOutputResult.format(_T("Global app flags stored"));
				outputDebugLog(logOutputResult, DebugTestTool);
				isNoReply = false;	// Reset flag
			}
			else if (!_tcscmp(tokenList.at(1).c_str(), _T("features"))) {
				// Save special variables (global variables)
				theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				logOutputResult.format(_T("Global special feature variables stored"));
				outputDebugLog(logOutputResult, DebugTestTool);
				isNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("print"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("config")))) {
			// Print app config data
			const ConfigData* tempConfigDataPtr = theAppPtr->getAppConfigData();
			if (tempConfigDataPtr != NULL) {
				// Prepare for replying
				isNoReply = false;	// Reset flag
				// Load app language package
				LANGTABLE_PTR languageTablePtr = theAppPtr->getAppLanguage();
				// Format and print data
				String valueString = Constant::String::Empty;
				// Left mouse button action
				int actionStringId = getPairedID(IDTable::ActionName, tempConfigDataPtr->leftMouseAction);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::LMBAction, getLanguageString(languageTablePtr, actionStringId));
				// Middle mouse button action
				actionStringId = getPairedID(IDTable::ActionName, tempConfigDataPtr->middleMouseAction);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::MMBAction, getLanguageString(languageTablePtr, actionStringId));
				// Right mouse button action
				actionStringId = getPairedID(IDTable::ActionName, tempConfigDataPtr->rightMouseAction);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::RMBAction, getLanguageString(languageTablePtr, actionStringId));
				// Right mouse button: Only show menu
				valueString = ((tempConfigDataPtr->rightMouseShowMenu) ? Constant::Value::True : _T("NO"));
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::RMBShowMenu, valueString.getString());
				// Language setting
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::LanguageID, getLanguageName(tempConfigDataPtr->languageID));
				// Show dialog at startup
				valueString = ((tempConfigDataPtr->showDialogAtStartup) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::ShowDlgAtStartup, valueString.getString());
				// Startup with Windows
				valueString = ((tempConfigDataPtr->enableAutoStart) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::StartupEnabled, valueString.getString());
				// Show confirm message before executing action
				valueString = ((tempConfigDataPtr->actionConfirmation) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::ConfirmAction, valueString.getString());
				// Save action log
				valueString = ((tempConfigDataPtr->saveActionHistory) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::SaveHistoryLog, valueString.getString());
				// Save app event log
				valueString = ((tempConfigDataPtr->saveAppEventLog) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::SaveAppEventLog, valueString.getString());
				// Run with admin privileges
				valueString = ((tempConfigDataPtr->runAsAdmin) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::RunAsAdmin, valueString.getString());
				// Show action error message
				valueString = ((tempConfigDataPtr->showErrorMessage) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::ShowErrorMsg, valueString.getString());
				// Show notify tip for schedule action
				valueString = ((tempConfigDataPtr->scheduleNotification) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::NotifySchedule, valueString.getString());
				// Allow canceling schedule when notify
				valueString = ((tempConfigDataPtr->allowScheduleCancellation) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::AllowCancelSchedule, valueString.getString());
				// Enable background action hotkeys
				valueString = ((tempConfigDataPtr->enableBackgroundHotkey) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::EnableBackgroundHotkey, valueString.getString());
				// Allow background hotkeys on lockscreen
				valueString = ((tempConfigDataPtr->allowLockscreenHotkey) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::LockStateHotkey, valueString.getString());
				// Enable Power Peminder feature
				valueString = ((tempConfigDataPtr->enablePowerReminder) ? Constant::Value::True : Constant::Value::False);
				outputDebugLogFormat(_T("%s=%s"), Key::ConfigData::EnablePowerReminder, valueString.getString());
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("schedule")))) {
			// Print schedule data
			const ScheduleData* tempScheduleDataPtr = theAppPtr->getAppScheduleData();
			if (tempScheduleDataPtr != NULL) {
				// Prepare for replying
				isNoReply = false;	// Reset flag
				// Print default schedule
				String defaultItemPrint;
				tempScheduleDataPtr->getDefaultItem().print(defaultItemPrint);
				logOutputResult.format(_T("DefaultSchedule: %s"), defaultItemPrint.getString());
				outputDebugLog(logOutputResult, DebugTestTool);
				// Print extra item number
				int extraItemNum = tempScheduleDataPtr->getExtraItemNum();
				logOutputResult.format(_T("ScheduleExtraData: ItemNum = %d"), extraItemNum);
				outputDebugLog(logOutputResult, DebugTestTool);
				// Print each item data
				for (int extraIndex = 0; extraIndex < extraItemNum; extraIndex++) {
					const ScheduleItem extraScheduleItem = tempScheduleDataPtr->getItemAt(extraIndex);

					// Print item
					String extraItemPrint;
					extraScheduleItem.print(extraItemPrint);
					logOutputResult.format(_T("Index=%d, %s"), extraIndex, extraItemPrint.getString());
					outputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("hksetdata")))) {
			// Print HotkeySet data
			const HotkeySetData* tempHotkeyDataPtr = theAppPtr->getAppHotkeySetData();
			if (tempHotkeyDataPtr != NULL) {
				// Prepare for replying
				isNoReply = false;	// Reset flag
				// Print item number
				int itemNum = tempHotkeyDataPtr->getItemNum();
				logOutputResult.format(_T("HotkeySetData: ItemNum = %d"), itemNum);
				outputDebugLog(logOutputResult, DebugTestTool);
				// Print each item data
				for (int index = 0; index < itemNum; index++) {
					const HotkeySetItem hotkeyItem = tempHotkeyDataPtr->getItemAt(index);

					// Print item
					String hotKeyItemPrint;
					hotkeyItem.print(hotKeyItemPrint);
					logOutputResult.format(_T("Index=%d, %s"), index, hotKeyItemPrint.getString());
					outputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("rmddata")))) {
			// Print Power Reminder data
			const PwrReminderData* tempReminderDataPtr = theAppPtr->getAppPwrReminderData();
			if (tempReminderDataPtr != NULL) {
				// Prepare for replying
				isNoReply = false;	// Reset flag
				// Print item number
				int itemNum = tempReminderDataPtr->getItemNum();
				logOutputResult.format(_T("PwrReminderData: ItemNum = %d"), itemNum);
				outputDebugLog(logOutputResult, DebugTestTool);
				// Print each item data
				for (int index = 0; index < itemNum; index++) {
					const PwrReminderItem reminderItem = tempReminderDataPtr->getItemAt(index);

					// Print item
					String reminderItemPrint;
					reminderItem.print(reminderItemPrint);
					logOutputResult.format(_T("Index=%d, %s"), index, reminderItemPrint.getString());
					outputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("resourceidmap")))) {
			// Print application resource ID map data
			const SResourceIDMap* resourceIdMapPtr = GET_RESOURCEID_MAP();
			if (resourceIdMapPtr != NULL) {
				// Prepare for replying
				isNoReply = false;	// Reset flag
				// Print number of entries
				size_t size = resourceIdMapPtr->getMapCount();
				logOutputResult.format(_T("Resource ID map count=%lld"), size);
				outputDebugLog(logOutputResult, DebugTestTool);
				// Print each resource ID map entry
				for (size_t index = 0; index < size; index++) {
					const RESOURCE_ID_MAP_ENTRY& resourceIDMapEntry = resourceIdMapPtr->getAt(index);
					logOutputResult.format(_T("Index=%lld: { ResourceID=%d, NameID=%s }"), index, resourceIDMapEntry.resourceID, MAKEUNICODE(resourceIDMapEntry.nameID));
					outputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdmsgset"))) {
		// Get Power Reminder data
		PwrReminderData* reminderDataPtr = theAppPtr->getAppPwrReminderData();
		if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("bkgclr")))) {
			if (tokenCount == 3) {
				// Set message background color by name
				String colorName = tokenList.at(2).c_str();
				DWORD retColorId = getStringID(StringTable::ColorName, colorName);
				if (retColorId != Constant::InvalidInteger) {
					// Set background color
					if (reminderDataPtr != NULL) {
						reminderDataPtr->getCommonStyle().setBkgrdColor(retColorId);
						theAppPtr->saveRegistryAppData(AppData::PowerReminder);
						outputDebugLogFormat(_T("Message background color set: %s"), colorName.toUpper().getString());
						isNoReply = false;	// Reset flag
					}
					else {
						outputDebugLogFormat(_T("Failed to set message background color!!!"));
						isNoReply = false;	// Reset flag
					}
				}
				else {
					// Invalid command
					isInvalidCmdFlag = true;
				}
			}
			else if (((tokenCount > 3) && (tokenCount <= 6)) && (!_tcscmp(tokenList.at(2).c_str(), _T("rgb")))) {
				// Set message background color by RGB value
				int colorRValue = (tokenCount >= 4) ? _tstoi(tokenList.at(3).c_str()) : Constant::InvalidInteger;		// Red
				int colorGValue = (tokenCount >= 5) ? _tstoi(tokenList.at(4).c_str()) : Constant::InvalidInteger;		// Green
				int colorBValue = (tokenCount >= 6) ? _tstoi(tokenList.at(5).c_str()) : Constant::InvalidInteger;		// Blue
				if (((colorRValue < 0) || (colorRValue > 255)) || ((colorGValue < 0) || (colorGValue > 255)) || ((colorBValue < 0) || (colorBValue > 255))) {
					// Invalid argument
					outputDebugLog(_T("Invalid value (Value range: 0 -> 255)"));
					isNoReply = false;	// Reset flag
				}
				else {
					// Set background color
					COLORREF colorRgb = RGB(colorRValue, colorGValue, colorBValue);
					if (reminderDataPtr != NULL) {
						reminderDataPtr->getCommonStyle().setBkgrdColor(colorRgb);
						theAppPtr->saveRegistryAppData(AppData::PowerReminder);
						outputDebugLogFormat(_T("Message background color set: RGB(%d,%d,%d)"), colorRValue, colorGValue, colorBValue);
						isNoReply = false;	// Reset flag
					}
					else {
						outputDebugLogFormat(_T("Failed to set message background color!!!"));
						isNoReply = false;	// Reset flag
					}
				}
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("txtclr")))) {
			if (tokenCount == 3) {
				// Set message text color by name
				String colorName = tokenList.at(2).c_str();
				DWORD retColorId = getStringID(StringTable::ColorName, colorName);
				if (retColorId != Constant::InvalidInteger) {
					// Set text color
					if (reminderDataPtr != NULL) {
						reminderDataPtr->getCommonStyle().setTextColor(retColorId);
						theAppPtr->saveRegistryAppData(AppData::PowerReminder);
						outputDebugLogFormat(_T("Message text color set: %s"), colorName.toUpper().getString());
						isNoReply = false;	// Reset flag
					}
					else {
						outputDebugLogFormat(_T("Failed to set message text color!!!"));
						isNoReply = false;	// Reset flag
					}
				}
				else {
					// Invalid command
					isInvalidCmdFlag = true;
				}
			}
			else if (((tokenCount > 3) && (tokenCount <= 6)) && (!_tcscmp(tokenList.at(2).c_str(), _T("rgb")))) {
				// Set message background color by RGB value
				int colorRValue = (tokenCount >= 4) ? _tstoi(tokenList.at(3).c_str()) : Constant::InvalidInteger;		// Red
				int colorGValue = (tokenCount >= 5) ? _tstoi(tokenList.at(4).c_str()) : Constant::InvalidInteger;		// Green
				int colorBValue = (tokenCount >= 6) ? _tstoi(tokenList.at(5).c_str()) : Constant::InvalidInteger;		// Blue
				if (((colorRValue < 0) || (colorRValue > 255)) || ((colorGValue < 0) || (colorGValue > 255)) || ((colorBValue < 0) || (colorBValue > 255))) {
					// Invalid argument
					outputDebugLog(_T("Invalid value (Value range: 0 -> 255)"));
					isNoReply = false;	// Reset flag
				}
				else {
					// Set text color
					COLORREF colorRgb = RGB(colorRValue, colorGValue, colorBValue);
					if (reminderDataPtr != NULL) {
						reminderDataPtr->getCommonStyle().setTextColor(colorRgb);
						theAppPtr->saveRegistryAppData(AppData::PowerReminder);
						outputDebugLogFormat(_T("Message text color set: RGB(%d,%d,%d)"), colorRValue, colorGValue, colorBValue);
						isNoReply = false;	// Reset flag
					}
					else {
						outputDebugLogFormat(_T("Failed to set message text color!!!"));
						isNoReply = false;	// Reset flag
					}
				}
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontname")))) {
			// Set reminder message font name
			String fontName = tokenList.at(2).c_str();
			// Because the input debug command had been converted to lowercase,
			// it needs to be re-formatted by capitalizing first character of each word
			fontName.trim().upperEachWord();
			// Validate font name
			bool returnFlag = AppCore::validateFontName(fontName);
			if (returnFlag != true) {
				// Invalid font name
				outputDebugLog(_T("Invalid font name"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Set message font
				if (reminderDataPtr != NULL) {
					reminderDataPtr->getCommonStyle().setFontName(fontName);
					theAppPtr->saveRegistryAppData(AppData::PowerReminder);
					outputDebugLogFormat(_T("Message font name set: %s"), fontName.getString());
					isNoReply = false;	// Reset flag
				}
				else {
					outputDebugLogFormat(_T("Failed to set message font name!!!"));
					isNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontsize")))) {
			// Set reminder message font size
			int fontSize = _tstoi(tokenList.at(2).c_str());
			if ((fontSize < RmdMsgStyleSet::kMinFontSize) || (fontSize > RmdMsgStyleSet::kMaxFontSize)) {
				// Invalid argument
				outputDebugLog(_T("Invalid value (Value range: 10 -> 100)"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Set font size
				if (reminderDataPtr != NULL) {
					reminderDataPtr->getCommonStyle().setFontSize(fontSize);
					theAppPtr->saveRegistryAppData(AppData::PowerReminder);
					outputDebugLogFormat(_T("Message font size set: %dpt"), fontSize);
					isNoReply = false;	// Reset flag
				}
				else {
					outputDebugLogFormat(_T("Failed to set message font size!!!"));
					isNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("timeout")))) {
			// Set reminder message auto-close interval (timeout)
			int timeoutValue = _tstoi(tokenList.at(2).c_str());
			if ((timeoutValue < RmdMsgStyleSet::kMinTimeOut) || (timeoutValue > RmdMsgStyleSet::kMaxTimeOut)) {
				// Invalid argument
				outputDebugLog(_T("Invalid value (Value range: 10 -> 1800)"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Set timeout
				if (reminderDataPtr != NULL) {
					reminderDataPtr->getCommonStyle().setTimeout(timeoutValue);
					theAppPtr->saveRegistryAppData(AppData::PowerReminder);
					outputDebugLogFormat(_T("Message time-out set: %ds"), timeoutValue);
					isNoReply = false;	// Reset flag
				}
				else {
					outputDebugLogFormat(_T("Failed to set message time-out!!!"));
					isNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("notimeout")))) {
			// No reminder message timeout (default 0)
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setTimeout(RmdMsgStyleSet::kDefaultTimeout);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLog(_T("Message time-out disabled"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to set message time-out!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconid")))) {
			// Set message icon ID by name
			String iconName = tokenList.at(2).c_str();
			DWORD retIconId = getStringID(StringTable::MsgIconName, iconName);
			if (retIconId != Constant::InvalidInteger) {
				// Set icon ID
				if (reminderDataPtr != NULL) {
					reminderDataPtr->getCommonStyle().setIconId(retIconId);
					theAppPtr->saveRegistryAppData(AppData::PowerReminder);
					outputDebugLogFormat(_T("Message icon ID set: %s (%d)"), iconName.toUpper().getString(), retIconId);
					isNoReply = false;	// Reset flag
				}
				else {
					outputDebugLogFormat(_T("Failed to set message icon ID!!!"));
					isNoReply = false;	// Reset flag
				}
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("noicon")))) {
			// No reminder message icon (default 0)
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setIconId(0);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLog(_T("Message icon disabled"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to set message icon ID!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconsize")))) {
			// Set reminder message icon size
			int iconSize = _tstoi(tokenList.at(2).c_str());
			if ((iconSize < RmdMsgStyleSet::kMinIconSize) || (iconSize > RmdMsgStyleSet::kMaxIconSize)) {
				// Invalid argument
				outputDebugLog(_T("Invalid value (Value range: 30 -> 100)"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Set icon size
				if (reminderDataPtr != NULL) {
					reminderDataPtr->getCommonStyle().setIconSize(iconSize);
					theAppPtr->saveRegistryAppData(AppData::PowerReminder);
					outputDebugLogFormat(_T("Message icon size set: %dx%dpx"), iconSize, iconSize);
					isNoReply = false;	// Reset flag
				}
				else {
					outputDebugLogFormat(_T("Failed to set message icon size!!!"));
					isNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconplacement")))) {
			// Set reminder message icon position
			String iconPosition = tokenList.at(2).c_str();
			if (!_tcscmp(iconPosition, _T("left"))) {
				// Set icon position: Icon on the Left
				if (reminderDataPtr != NULL) {
					reminderDataPtr->getCommonStyle().setIconPosition(RmdMsgStyleSet::IconOnTheLeft);
					theAppPtr->saveRegistryAppData(AppData::PowerReminder);
					outputDebugLog(_T("Message icon position set: Left"));
					isNoReply = false;	// Reset flag
				}
				else {
					outputDebugLogFormat(_T("Failed to set message icon position!!!"));
					isNoReply = false;	// Reset flag
				}
			}
			else if (!_tcscmp(iconPosition, _T("top"))) {
				// Set icon position: Icon on the Top
				if (reminderDataPtr != NULL) {
					reminderDataPtr->getCommonStyle().setIconPosition(RmdMsgStyleSet::IconOnTheTop);
					theAppPtr->saveRegistryAppData(AppData::PowerReminder);
					outputDebugLog(_T("Message icon position set: Top"));
					isNoReply = false;	// Reset flag
				}
				else {
					outputDebugLogFormat(_T("Failed to set message icon position!!!"));
					isNoReply = false;	// Reset flag
				}
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("hmargin")))) {
			// Set reminder message horizontal margin
			int marginHorizontal = _tstoi(tokenList.at(2).c_str());
			if ((marginHorizontal < RmdMsgStyleSet::kMinMarginVal) || (marginHorizontal > RmdMsgStyleSet::kMaxMarginVal)) {
				// Invalid argument
				outputDebugLog(_T("Invalid value (Value range: 10 -> 120)"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Set margin
				if (reminderDataPtr != NULL) {
					reminderDataPtr->getCommonStyle().setHorizontalMargin(marginHorizontal);
					theAppPtr->saveRegistryAppData(AppData::PowerReminder);
					outputDebugLogFormat(_T("Message horizontal margin set: %dpx"), marginHorizontal);
					isNoReply = false;	// Reset flag
				}
				else {
					outputDebugLogFormat(_T("Failed to set message horizontal margin!!!"));
					isNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("vmargin")))) {
			// Set reminder message vertical margin
			int marginVertical = _tstoi(tokenList.at(2).c_str());
			if ((marginVertical < RmdMsgStyleSet::kMinMarginVal) || (marginVertical > RmdMsgStyleSet::kMaxMarginVal)) {
				// Invalid argument
				outputDebugLog(_T("Invalid value (Value range: 10 -> 120)"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Set margin
				if (reminderDataPtr != NULL) {
					reminderDataPtr->getCommonStyle().setVerticalMargin(marginVertical);
					theAppPtr->saveRegistryAppData(AppData::PowerReminder);
					outputDebugLogFormat(_T("Message vertical margin set: %dpx"), marginVertical);
					isNoReply = false;	// Reset flag
				}
				else {
					outputDebugLogFormat(_T("Failed to set message vertical margin!!!"));
					isNoReply = false;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdmsgreset"))) {
		// Get Power Reminder data
		PwrReminderData* reminderDataPtr = theAppPtr->getAppPwrReminderData();
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("bkgclr")))) {
			// Reset message background color
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setBkgrdColor(RmdMsgStyleSet::kDefaultBkgrdColor);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLogFormat(_T("Message background color reset"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to reset message background color!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("txtclr")))) {
			// Set message text color by name
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setTextColor(RmdMsgStyleSet::kDefaultTextColor);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLogFormat(_T("Message text color reset"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to reset message text color!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontname")))) {
			// Set reminder message font name
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setFontName(RmdMsgStyleSet::kDefaultFontName);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLogFormat(_T("Message font name reset"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to reset message font name!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontsize")))) {
			// Set reminder message font size
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setFontSize(RmdMsgStyleSet::kDefaultFontSize);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLogFormat(_T("Message font size reset"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to reset message font size!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("timeout")))) {
			// Reset reminder message auto-close interval (time-out)
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setTimeout(RmdMsgStyleSet::kDefaultTimeout);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLog(_T("Message time-out reset"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to reset message time-out!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconid")))) {
			// Reset reminder message icon ID
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setIconId(RmdMsgStyleSet::kDefaultIconID);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLog(_T("Message icon ID reset"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to reset message icon ID!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconsize")))) {
			// Reset reminder message icon size
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setIconSize(RmdMsgStyleSet::kDefaultIconSize);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLog(_T("Message icon size reset"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to reset message icon size!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconpos")))) {
			// Reset reminder message icon position
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setIconPosition(RmdMsgStyleSet::kDefaultIconPosition);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLog(_T("Message icon position reset"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to reset message icon position!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("hmargin")))) {
			// Reset reminder message horizontal margin
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setHorizontalMargin(RmdMsgStyleSet::kDefaultHorizontalMargin);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLogFormat(_T("Message horizontal margin reset)"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to reset message horizontal margin!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("vmargin")))) {
			// Reset reminder message vertical margin
			if (reminderDataPtr != NULL) {
				reminderDataPtr->getCommonStyle().setVerticalMargin(RmdMsgStyleSet::kDefaultVerticalMargin);
				theAppPtr->saveRegistryAppData(AppData::PowerReminder);
				outputDebugLogFormat(_T("Message vertical margin reset"));
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to reset message vertical margin!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdmsgget"))) {
		// Get Power Reminder data
		PwrReminderData* reminderDataPtr = theAppPtr->getAppPwrReminderData();
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("bkgclr")))) {
			// Get reminder message background color
			if (reminderDataPtr != NULL) {
				DWORD backgroundColor = reminderDataPtr->getCommonStyle().getBkgrdColor();
				int colorBValue = (backgroundColor & 0x00FF0000) >> 16;		// Blue
				int colorGValue = (backgroundColor & 0x0000FF00) >> 8;		// Green
				int colorRValue = (backgroundColor & 0x000000FF);			// Red
				outputDebugLogFormat(_T("Message background color: RGB(%d,%d,%d)"), colorRValue, colorGValue, colorBValue);
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to get message background color!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("txtclr")))) {
			// Get reminder message text color
			if (reminderDataPtr != NULL) {
				DWORD textColor = reminderDataPtr->getCommonStyle().getTextColor();
				int colorBValue = (textColor & 0x00FF0000) >> 16;		// Blue
				int colorGValue = (textColor & 0x0000FF00) >> 8;		// Green
				int colorRValue = (textColor & 0x000000FF);				// Red
				outputDebugLogFormat(_T("Message text color: RGB(%d,%d,%d)"), colorRValue, colorGValue, colorBValue);
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to get message text color!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontname")))) {
			// Get reminder message font name
			if (reminderDataPtr != NULL) {
				String fontName = reminderDataPtr->getCommonStyle().getFontName();
				outputDebugLogFormat(_T("Message font name: %s"), fontName.getString());
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to get message font name!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontsize")))) {
			// Get reminder message font size
			if (reminderDataPtr != NULL) {
				int fontSize = reminderDataPtr->getCommonStyle().getFontSize();
				outputDebugLogFormat(_T("Message font size: %dpt"), fontSize);
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to get message font size!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("timeout")))) {
			// Get reminder message auto-close interval (time-out)
			if (reminderDataPtr != NULL) {
				int timeoutValue = reminderDataPtr->getCommonStyle().getTimeout();
				outputDebugLogFormat(_T("Message time-out: %ds"), timeoutValue);
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to get message time-out!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconid")))) {
			// Get reminder message icon ID
			if (reminderDataPtr != NULL) {
				unsigned iconId = reminderDataPtr->getCommonStyle().getIconId();
				outputDebugLogFormat(_T("Message icon ID: %d"), iconId);
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to get message icon ID!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconsize")))) {
			// Get reminder message icon size
			if (reminderDataPtr != NULL) {
				int iconSize = reminderDataPtr->getCommonStyle().getIconSize();
				outputDebugLogFormat(_T("Message icon size: %dx%dpx"), iconSize, iconSize);
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to get message icon size!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconpos")))) {
			// Get reminder message icon position
			if (reminderDataPtr != NULL) {
				BYTE iconPosition = reminderDataPtr->getCommonStyle().getIconPosition();
				if (iconPosition == RmdMsgStyleSet::IconOnTheLeft) {
					outputDebugLog(_T("Message icon position: Left"));
					isNoReply = false;	// Reset flag
				}
				else if (iconPosition == RmdMsgStyleSet::IconOnTheTop) {
					outputDebugLog(_T("Message icon position: Top"));
					isNoReply = false;	// Reset flag
				}
				else {
					outputDebugLog(_T("Message icon position: Unknown"));
					isNoReply = false;	// Reset flag
				}
			}
			else {
				outputDebugLogFormat(_T("Failed to get message icon position!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("hmargin")))) {
			// Get reminder message horizontal margin
			if (reminderDataPtr != NULL) {
				int marginHorizontal = reminderDataPtr->getCommonStyle().getHorizontalMargin();
				outputDebugLogFormat(_T("Message horizontal margin: %dpx"), marginHorizontal);
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to get message horizontal margin!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("vmargin")))) {
			// Get reminder message vertical margin
			if (reminderDataPtr != NULL) {
				int marginVertical = reminderDataPtr->getCommonStyle().getVerticalMargin();
				outputDebugLogFormat(_T("Message vertical margin: %dpx"), marginVertical);
				isNoReply = false;	// Reset flag
			}
			else {
				outputDebugLogFormat(_T("Failed to get message vertical margin!!!"));
				isNoReply = false;	// Reset flag
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("runtimequeue"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("print")))) {
			// Print Power++ runtime queue list
			if (runtimeQueue_.empty()) {
				// Empty list
				outputDebugLog(_T("Runtime queue empty!"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Print each item
				PwrRuntimeItem runtimeItem;
				for (int index = 0; index < runtimeQueue_.size(); index++) {
					// Get runtime item from queue
					runtimeItem = runtimeQueue_.at(index);
					// Print runtime item info
					outputDebugLogFormat(_T("Item%03d: CategoryID=%d, ItemID=%d, Display=%d, Skip=%d, Snooze=%d"), index, runtimeItem.getCategory(),
						runtimeItem.getItemId(), runtimeItem.getDisplayFlag(), runtimeItem.getSkipFlag(), runtimeItem.getSnoozeFlag());
					isNoReply = false;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdsnooze"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("prntqueue")))) {
			// Print Power Reminder snooze queue list
			if (runtimeQueue_.empty()) {
				// Empty list
				outputDebugLog(_T("Reminder snooze queue empty!"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Print each item
				PwrRuntimeItem runtimeItem;
				for (int index = 0; index < runtimeQueue_.size(); index++) {
					// Get runtime item from queue
					runtimeItem = runtimeQueue_.at(index);
					// Skip if it's not Power Reminder item
					if (runtimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;
					// Print runtime item info
					ClockTime timeNextSnooze = runtimeItem.getTime();
					outputDebugLogFormat(_T("Item%03d: ID=%d, Snooze=%d, NextTrigger=%02d:%02d"), index, runtimeItem.getItemId(),
						runtimeItem.getSnoozeFlag(), timeNextSnooze.hour(), timeNextSnooze.minute());
					isNoReply = false;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdruntime"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("prntdata")))) {
			// Print Power Reminder runtime data list
			if (runtimeQueue_.empty()) {
				// Empty list
				outputDebugLog(_T("Reminder runtime data empty!"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Print each item
				PwrRuntimeItem runtimeItem;
				for (int index = 0; index < runtimeQueue_.size(); index++) {
					// Get runtime item from queue
					runtimeItem = runtimeQueue_.at(index);
					// Skip if it's not Power Reminder item
					if (runtimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;
					// Print runtime item info
					ClockTime timeNextSnooze = runtimeItem.getTime();
					outputDebugLogFormat(_T("Item%03d: ID=%d, Display=%d, Snooze=%d, NextTrigger=%02d:%02d"), index, runtimeItem.getItemId(),
						runtimeItem.getDisplayFlag(), runtimeItem.getSnoozeFlag(), timeNextSnooze.hour(), timeNextSnooze.minute());
					isNoReply = false;	// Reset flag
				}
			}
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("upper"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("string")))) {
			// Upper each word
			String inputString = tokenList.at(2).c_str();
			inputString.trim().upperEachWord();
			outputDebugLogFormat(_T("Upper: %s"), inputString.getString());
			isNoReply = false;	// Reset flag
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("enum"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontnames")))) {
			// Enumerate all currently available fonts
			std::vector<std::wstring> fontNames;
			bool returnFlag = AppCore::enumFontNames(fontNames);
			if (returnFlag == false) {
				// Enumerate fonts failed
				outputDebugLog(_T("Enumerate fonts failed"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Print all font names
				BeginWaitCursor();
				{
					// Prepare for replying
					isNoReply = false;	// Reset flag
					for (auto it = fontNames.begin(); it != fontNames.end(); it++) {
						outputDebugLogFormat(_T("Font [%d]: %s"), (it - fontNames.begin()), (*it).c_str());
						Sleep(50);  // wait for a blink
					}
				}
				EndWaitCursor();
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("getlastsysevttime"))) {
		// Get last system event time
		DateTime dateTimeTemp;
		String dateTimeFormat = StringUtils::loadResourceString(IDS_FORMAT_FULLDATETIME);
		// Get last system suspend time
		if (theAppPtr->getLastSysEventTime(SystemEventID::SystemSuspend, dateTimeTemp)) {
			// Format date time
			const wchar_t* middayFlag = (dateTimeTemp.hour() >= 12) ? _T("PM") : _T("AM");
			String logTemp = StringUtils::stringFormat(dateTimeFormat, dateTimeTemp.year(), dateTimeTemp.month(), dateTimeTemp.day(),
				dateTimeTemp.hour(), dateTimeTemp.minute(), dateTimeTemp.second(), dateTimeTemp.millisecond(), middayFlag);
			logOutputResult.format(_T("Last System Suspend: %s"), logTemp.getString());
			outputDebugLog(logOutputResult);
			isNoReply = false;	// Reset flag
		}
		else {
			// Get last system suspend time failed
			outputDebugLog(_T("Get last system suspend time failed"));
			isNoReply = false;	// Reset flag
		}
		// Get last system wakeup time
		if (theAppPtr->getLastSysEventTime(SystemEventID::SystemWakeUp, dateTimeTemp)) {
			// Format date time
			const wchar_t* middayFlag = (dateTimeTemp.hour() >= 12) ? _T("PM") : _T("AM");
			String logTemp = StringUtils::stringFormat(dateTimeFormat, dateTimeTemp.year(), dateTimeTemp.month(), dateTimeTemp.day(),
				dateTimeTemp.hour(), dateTimeTemp.minute(), dateTimeTemp.second(), dateTimeTemp.millisecond(), middayFlag);
			logOutputResult.format(_T("Last System Wakeup: %s"), logTemp.getString());
			outputDebugLog(logOutputResult);
			isNoReply = false;	// Reset flag
		}
		else {
			// Get last system wakeup time failed
			outputDebugLog(_T("Get last system wakeup time failed"));
			isNoReply = false;	// Reset flag
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("powerbroadcast"))) {
		if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("param")))) {
			// Simulate system power event (WM_POWERBROADCAST message)
			int param = _tstoi(tokenList.at(2).c_str());
			if ((param == PBT_APMPOWERSTATUSCHANGE) ||
				(param == PBT_APMRESUMEAUTOMATIC) ||
				(param == PBT_APMRESUMESUSPEND) ||
				(param == PBT_APMSUSPEND) ||
				(param == PBT_POWERSETTINGCHANGE)) {
				// Post message
				PostMessage(WM_POWERBROADCAST, (WPARAM)param, NULL);
				outputDebugLogFormat(_T("Message posted: Param=%d"), param);
				isNoReply = false;	// Reset flag
			}
			else {
				// Invalid param
				outputDebugLog(_T("Invalid parameter"));
				isNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("skip")))) {
			// Skip system power event (WM_POWERBROADCAST message)
			int skipCount = _tstoi(tokenList.at(2).c_str());
			if ((skipCount <= 0) || (skipCount > 3600)) {
				// Invalid argument
				outputDebugLog(_T("Invalid value (Value range: 1 -> 3600)"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Set skip counter
				setFlagValue(AppFlagID::pwrBroadcastSkipCount, skipCount);
				outputDebugLogFormat(_T("Skip PowerBroadcast event: Counter=%d"), skipCount);
				isNoReply = false;	// Reset flag
			}
		}
		else {
			// Invalid command
			isInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("flagset"))) {
		if (!_tcscmp(tokenList.at(1).c_str(), _T("pwraction"))) {
			// Turn ON/OFF power action flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				setPwrActionFlag(FLAG_ON);
				outputDebugLog(_T("Power action flag: ON"));
				isNoReply = false;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				setPwrActionFlag(FLAG_OFF);
				outputDebugLog(_T("Power action flag: OFF"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("syssuspend"))) {
			// Turn ON/OFF system suspend flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				setSystemSuspendFlag(FLAG_ON);
				outputDebugLog(_T("System suspend flag: ON"));
				isNoReply = false;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				setSystemSuspendFlag(FLAG_OFF);
				outputDebugLog(_T("System suspend flag: OFF"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("sessionend"))) {
			// Turn ON/OFF session end flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				setSessionEndFlag(FLAG_ON);
				outputDebugLog(_T("Session end flag: ON"));
				isNoReply = false;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				setSessionEndFlag(FLAG_OFF);
				outputDebugLog(_T("Session end flag: OFF"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("sessionlock"))) {
			// Turn ON/OFF session lock flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				setSessionLockFlag(FLAG_ON);
				outputDebugLog(_T("Session lock flag: ON"));
				isNoReply = false;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				setSessionLockFlag(FLAG_OFF);
				outputDebugLog(_T("Session lock flag: OFF"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("safetermination"))) {
			// Turn ON/OFF previously safe termination trace flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				setSafeTerminationFlag(FLAG_ON);
				outputDebugLog(_T("Previously safe termination trace flag: ON"));
				isNoReply = false;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				setSafeTerminationFlag(FLAG_OFF);
				outputDebugLog(_T("Previously safe termination trace flag: OFF"));
				isNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				isInvalidCmdFlag = true;
			}
		}
	}
	else {
		// Invalid command
		isInvalidCmdFlag = true;
	}

	/*********************************************************************/
	/*																	 */
	/*					Post-processing and clean-up				     */
	/*																	 */
	/*********************************************************************/

	// If command is invalid
	if (isInvalidCmdFlag == true) {
		// Error: Invalid command
		errorCode = APP_ERROR_DBG_INVALID_COMMAND;
		isNoReply = false;		// Reset flag
		returnFlag = false;		// Return failed
	}

	// If command is executed but no reply
	if (isNoReply == true) {
		// Notify to the DebugTest dialog
		SDialog* debugTestDlgPtr = theAppPtr->getDebugTestDlg();
		if (debugTestDlgPtr != NULL) {
			debugTestDlgPtr->PostMessage(SM_APP_DEBUGCMD_NOREPLY);
		}
		if (isNoReplySilent != true) {
			outputDebugLog(_T("Command sent, no reply!!!"));
		}
		isNoReply = false;			// Reset flag
		isNoReplySilent = false;	// Reset flag
		returnFlag = true;			// Return successful
	}

	return returnFlag;
}

