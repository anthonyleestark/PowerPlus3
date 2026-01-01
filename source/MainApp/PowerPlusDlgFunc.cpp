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
	bool bNoReply = true;
	bool bNoReplySilent = false;

	// Invalid command flag
	bool bInvalidCmdFlag = false;

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
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("setdefault"))) {
		// Set default settings
		setDefaultConfig();
		logOutputResult.format(_T("Reset default settings!!!"));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = false;	// Reset flag
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
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("hksupdate"))) {
		// Update HotkeySet settings
		PostMessage(SM_APP_UPDATE_HOTKEYSETDATA);
		logOutputResult.format(_T("HotkeySet data updated!!!"));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("rmdupdate"))) {
		// Update Power Reminder data
		PostMessage(SM_APP_UPDATE_PWRREMINDERDATA);
		logOutputResult.format(_T("Power Reminder data updated!!!"));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("bakconfig"))) {
		// Backup configuration
		BackupSystem::RegistryExport();
		logOutputResult.format(_T("Config backed-up!!!"));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("viewbakconfig"))) {
		// View backup configuration file
		openTextFileToView(Constant::File::Name::Backup_Config, Constant::File::Extension::Reg);
		logOutputResult.format(_T("Opening backup config file..."));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = false;	// Reset flag
		return true;
	}
	else if (!_tcscmp(debugCommand, _T("viewhistory"))) {
		// View action history log file
		openTextFileToView(Constant::File::Name::AppHistory, Constant::File::Extension::Log, Constant::Folder::Log);
		logOutputResult.format(_T("Opening action history log file..."));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = false;	// Reset flag
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
		SDialog* pDebugTestDlg = theAppPtr->getDebugTestDlg();
		if (pDebugTestDlg != NULL) {
			pDebugTestDlg->ShowWindow(SW_HIDE);
			pDebugTestDlg->PostMessage(SM_APP_DEBUGCMD_NOREPLY);
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

	String strTemp = Constant::String::Empty;

	// Process debug commands by tokens
	if (!_tcscmp(tokenList.at(0).c_str(), _T("test"))) {
		// Debug command token test
		if (tokenCount > 1) {
			// Prepare for replying
			bNoReply = false;	// Reset flag
			// Print token list
			int nTokenCount = (tokenCount - 1);
			OutputDebugLogFormat(_T("Token number: %d"), nTokenCount);
			for (int nTokenIndex = 0; nTokenIndex < nTokenCount; nTokenIndex++) {
				strTemp = tokenList.at(nTokenIndex + 1);
				OutputDebugLogFormat(_T("Token[%d]: %s"), nTokenIndex, strTemp.getString());
			}
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("dmytest"))) {
		// Set dummy test mode
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("on")))) {
			setDummyTestMode(true);
			OutputDebugLog(_T("DummyTest ON"));
			bNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("off")))) {
			setDummyTestMode(false);
			OutputDebugLog(_T("DummyTest OFF"));
			bNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
			setDummyTestMode(DEFAULT_DUMMYTEST);
			OutputDebugLog(_T("DummyTest reset"));
			bNoReply = false;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("dmyteststs"))) {
		// Get dummy test mode
		int returnValue = getDummyTestMode();
		OutputDebugLogFormat(_T("DummyTest status = %d"), returnValue);
		bNoReply = false;	// Reset flag
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("debugmode"))) {
		// Set debug mode
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("on")))) {
			setDebugMode(true);
			OutputDebugLog(_T("DebugMode ON"));
			bNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("off")))) {
			setDebugMode(false);
			OutputDebugLog(_T("DebugMode OFF"));
			bNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
			setDebugMode(DEFAULT_DEBUGMODE);
			OutputDebugLog(_T("DebugMode reset"));
			bNoReply = false;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("debugmodests"))) {
		// Get debug mode
		int returnValue = getDebugMode();
		OutputDebugLogFormat(_T("DebugMode status = %d"), returnValue);
		bNoReply = false;	// Reset flag
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("debuglog"))) {
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("output")))) {
			// Set debug log output target
			if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("default")))) {
				setDebugOutputTarget(DefaultOutput);
				OutputDebugLog(_T("Debug log output target changed"));
				bNoReply = false;	// Reset flag
			}
			else if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("tofile")))) {
				setDebugOutputTarget(DebugInfoFile);
				OutputDebugLog(_T("Debug log output target changed"));
				bNoReply = false;	// Reset flag
			}
			else if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("todbtool")))) {
				setDebugOutputTarget(DebugTestTool);
				OutputDebugLog(_T("Debug log output target changed"));
				bNoReply = false;	// Reset flag
			}
			else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
				setDebugOutputTarget(DEFAULT_DEBUGOUTPUT);
				OutputDebugLog(_T("Debug log output target reset"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("getoutput")))) {
			// Get debug log output target
			int returnValue = getDebugOutputTarget();
			OutputDebugLogFormat(_T("DebugLogOutputTarget = %d"), returnValue);
			bNoReply = false;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("testfeature"))) {
		// Set test feature enable
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("on")))) {
			setTestFeatureEnable(true);
			OutputDebugLog(_T("TestFeature ON"));
			bNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("off")))) {
			setTestFeatureEnable(false);
			OutputDebugLog(_T("TestFeature OFF"));
			bNoReply = false;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
			setTestFeatureEnable(DEFAULT_TESTFEATURE);
			OutputDebugLog(_T("TestFeature reset"));
			bNoReply = false;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("testfeaturests"))) {
		// Get test feature enable
		int returnValue = getTestFeatureEnable();
		OutputDebugLogFormat(_T("TestFeature status = %d"), returnValue);
		bNoReply = false;	// Reset flag
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("logtest"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("trcerr")))) {
			// Trace error log test
			int nVal = _tstoi(tokenList.at(2).c_str());
			if (nVal >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				bNoReply = false;	// Reset flag
				if (nVal >= 50) {
					// Output waiting notification
					OutputDebugLog(_T("Wait for a while!!!"), DebugTestTool);
					AppCore::waitMessage(SM_WND_DEBUGOUTPUT_DISP); // wait for the notification displaying
				}
				for (int i = 0; i < nVal; i++) {
					// Write test trace error log
					TRACE_FORMAT("[TraceError Test] Loop time: %d", i);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				OutputDebugLog(_T("TraceErrorLog test done!!!"), DebugTestTool);
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("trcdebug")))) {
			// Trace debug log test
			int nVal = _tstoi(tokenList.at(2).c_str());
			if (nVal >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				bNoReply = false;	// Reset flag
				if (nVal >= 50) {
					// Output waiting notification
					OutputDebugLog(_T("Wait for a while!!!"), DebugTestTool);
					AppCore::waitMessage(SM_WND_DEBUGOUTPUT_DISP); // wait for the notification displaying
				}
				String strFormat;
				for (int i = 0; i < nVal; i++) {
					// Write test trace debug log
					strFormat.format(_T("[TraceDebugLog Test] Loop time: %d"), i);
					DebugLogging::GetDebugLogger().WriteTraceDebugLogFile(strFormat);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				OutputDebugLog(_T("TraceDebugLog test done!!!"), DebugTestTool);
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("debuginfo")))) {
			// Output debug info log test
			int nVal = _tstoi(tokenList.at(2).c_str());
			if (nVal >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				bNoReply = false;	// Reset flag
				if (nVal >= 50) {
					// Output waiting notification
					OutputDebugLog(_T("Wait for a while!!!"), DebugTestTool);
					AppCore::waitMessage(SM_WND_DEBUGOUTPUT_DISP); // wait for the notification displaying
				}
				String strFormat;
				for (int i = 0; i < nVal; i++) {
					// Write test debug info log
					strFormat.format(_T("[OutputDebugInfo Test] Loop time: %d"), i);
					OutputDebugLog(strFormat, DebugTestTool);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				OutputDebugLog(_T("DebugInfoLog test done!!!"), DebugTestTool);
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("errtest"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("code")))) {
			// Test show error message
			int nErrCode = _tstoi(tokenList.at(2).c_str());
			if (nErrCode >= 0) {
				if (getAppOption(AppOptionID::showErrorMessage) == false) {
					OutputDebugLog(_T("Show error message OFF"));
					bNoReply = false;	// Reset flag
				}
				else {
					// Show error message
					showErrorMessage(nErrCode);
					OutputDebugLog(_T("Error message showed!!!"));
					bNoReply = false;	// Reset flag
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("appeventlog"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("clear")))) {
			// Clear all app event log data in memory
			SLogging* pAppEventLog = ((CPowerPlusApp*)AfxGetApp())->getAppEventLog();
			if (pAppEventLog != NULL) {
				pAppEventLog->Init();
				OutputDebugLog(_T("App event log data cleared"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("write")))) {
			// Force writing app event log data from memory to file
			SLogging* pAppEventLog = ((CPowerPlusApp*)AfxGetApp())->getAppEventLog();
			if (pAppEventLog != NULL) {
				bool returnFlag = pAppEventLog->Write();
				if (returnFlag == true) {
					// Write log succeeded
					OutputDebugLog(_T("App event log data written"));
					bNoReply = false;	// Reset flag
				}
				else {
					// Write log failed
					OutputDebugLog(_T("Write app event log data failed"));
					bNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("del")))) {
			// Delete app event log files
			String strKeyName = tokenList.at(2).c_str();;
			if (!_tcscmp(tokenList.at(2).c_str(), _T("all"))) {
				strKeyName = _T("AppEventLog");
			}
			CFileFind Finder;
			String fileName = StringUtils::makeFilePath(StringUtils::getSubFolderPath(Constant::Folder::Log), _T("*"), Constant::File::Extension::Log);
			bool bFindRet = Finder.FindFile(fileName);
			if (!strKeyName.isEmpty()) {
				int nDelFileCount = 0;
				while (bFindRet == true) {
					bFindRet = Finder.FindNextFile();
					fileName = Finder.GetFileName().GetString();
					if (fileName.find(strKeyName) != INT_INVALID) {
						// Delete file
						CFile::Remove(Finder.GetFilePath());
						nDelFileCount++;	// Increase counter
					}
				}
				if (nDelFileCount > 0) {
					// Output number of deleted files
					logOutputResult.format(_T("App event log file(s) deleted (Count=%d)"), nDelFileCount);
					OutputDebugLog(logOutputResult, DebugTestTool);
					bNoReply = false;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("reminderdisp"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("id")))) {
			int itemId = _tstoi(tokenList.at(2).c_str());
			// Find and display reminder item by ID
			bool bFindRet = false;
			PwrReminderItem pwrTemp;
			int itemNum = reminderData_.getItemNum();
			for (int index = 0; index < itemNum; index++) {
				pwrTemp = reminderData_.getItemAt(index);
				if (pwrTemp.getItemId() == static_cast<unsigned>(itemId)) {
					bFindRet = true;
					displayPwrReminder(pwrTemp);
					OutputDebugLog(_T("Reminder item displayed!!!"));
					bNoReply = false;	// Reset flag
					break;
				}
			}
			if (bFindRet == false) {
				// Item not found
				OutputDebugLog(_T("Reminder item not found"));
				bNoReply = false;	// Reset flag
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("saveglobaldata"))) {
		if (tokenCount == 1) {
			// Save all global data variables
			theAppPtr->saveGlobalData(0xFF);
			logOutputResult.format(_T("Global variables stored"));
			OutputDebugLog(logOutputResult, DebugTestTool);
			bNoReply = false;	// Reset flag
		}
		else if (tokenCount == 2) {
			if (!_tcscmp(tokenList.at(1).c_str(), _T("all"))) {
				// Save all global variables
				theAppPtr->saveGlobalData(0xFF);
				logOutputResult.format(_T("Global variables stored"));
				OutputDebugLog(logOutputResult, DebugTestTool);
				bNoReply = false;	// Reset flag
			}
			else if (!_tcscmp(tokenList.at(1).c_str(), _T("dbtest"))) {
				// Save debugging/testing config (global variables)
				theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_DEBUGTEST);
				logOutputResult.format(_T("Debug/test config stored"));
				OutputDebugLog(logOutputResult, DebugTestTool);
				bNoReply = false;	// Reset flag
			}
			else if (!_tcscmp(tokenList.at(1).c_str(), _T("appflags"))) {
				// Save app flags (global variables)
				theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
				logOutputResult.format(_T("Global app flags stored"));
				OutputDebugLog(logOutputResult, DebugTestTool);
				bNoReply = false;	// Reset flag
			}
			else if (!_tcscmp(tokenList.at(1).c_str(), _T("features"))) {
				// Save special variables (global variables)
				theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				logOutputResult.format(_T("Global special feature variables stored"));
				OutputDebugLog(logOutputResult, DebugTestTool);
				bNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("print"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("config")))) {
			// Print app config data
			const ConfigData* pcfgDataTemp = theAppPtr->getAppConfigData();
			if (pcfgDataTemp != NULL) {
				// Prepare for replying
				bNoReply = false;	// Reset flag
				// Load app language package
				LANGTABLE_PTR languageTablePtr = theAppPtr->getAppLanguage();
				// Format and print data
				String valueString = Constant::String::Empty;
				// Left mouse button action
				int actionStringId = GetPairedID(IDTable::ActionName, pcfgDataTemp->leftMouseAction);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::LMBAction, getLanguageString(languageTablePtr, actionStringId));
				// Middle mouse button action
				actionStringId = GetPairedID(IDTable::ActionName, pcfgDataTemp->middleMouseAction);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::MMBAction, getLanguageString(languageTablePtr, actionStringId));
				// Right mouse button action
				actionStringId = GetPairedID(IDTable::ActionName, pcfgDataTemp->rightMouseAction);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::RMBAction, getLanguageString(languageTablePtr, actionStringId));
				// Right mouse button: Only show menu
				valueString = ((pcfgDataTemp->rightMouseShowMenu) ? Constant::Value::True : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::RMBShowMenu, valueString.getString());
				// Language setting
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::LanguageID, getLanguageName(pcfgDataTemp->languageID));
				// Show dialog at startup
				valueString = ((pcfgDataTemp->showDialogAtStartup) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::ShowDlgAtStartup, valueString.getString());
				// Startup with Windows
				valueString = ((pcfgDataTemp->enableAutoStart) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::StartupEnabled, valueString.getString());
				// Show confirm message before executing action
				valueString = ((pcfgDataTemp->actionConfirmation) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::ConfirmAction, valueString.getString());
				// Save action log
				valueString = ((pcfgDataTemp->saveActionHistory) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::SaveHistoryLog, valueString.getString());
				// Save app event log
				valueString = ((pcfgDataTemp->saveAppEventLog) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::SaveAppEventLog, valueString.getString());
				// Run with admin privileges
				valueString = ((pcfgDataTemp->runAsAdmin) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::RunAsAdmin, valueString.getString());
				// Show action error message
				valueString = ((pcfgDataTemp->showErrorMessage) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::ShowErrorMsg, valueString.getString());
				// Show notify tip for schedule action
				valueString = ((pcfgDataTemp->scheduleNotification) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::NotifySchedule, valueString.getString());
				// Allow canceling schedule when notify
				valueString = ((pcfgDataTemp->allowScheduleCancellation) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::AllowCancelSchedule, valueString.getString());
				// Enable background action hotkeys
				valueString = ((pcfgDataTemp->enableBackgroundHotkey) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::EnableBackgroundHotkey, valueString.getString());
				// Allow background hotkeys on lockscreen
				valueString = ((pcfgDataTemp->allowLockscreenHotkey) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::LockStateHotkey, valueString.getString());
				// Enable Power Peminder feature
				valueString = ((pcfgDataTemp->enablePowerReminder) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::EnablePowerReminder, valueString.getString());
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("schedule")))) {
			// Print schedule data
			const ScheduleData* pSchedDataTemp = theAppPtr->getAppScheduleData();
			if (pSchedDataTemp != NULL) {
				// Prepare for replying
				bNoReply = false;	// Reset flag
				// Print default schedule
				String defaultItemPrint;
				pSchedDataTemp->getDefaultItem().print(defaultItemPrint);
				logOutputResult.format(_T("DefaultSchedule: %s"), defaultItemPrint.getString());
				OutputDebugLog(logOutputResult, DebugTestTool);
				// Print extra item number
				int extraItemNum = pSchedDataTemp->getExtraItemNum();
				logOutputResult.format(_T("ScheduleExtraData: ItemNum = %d"), extraItemNum);
				OutputDebugLog(logOutputResult, DebugTestTool);
				// Print each item data
				for (int extraIndex = 0; extraIndex < extraItemNum; extraIndex++) {
					const ScheduleItem extraScheduleItem = pSchedDataTemp->getItemAt(extraIndex);

					// Print item
					String extraItemPrint;
					extraScheduleItem.print(extraItemPrint);
					logOutputResult.format(_T("Index=%d, %s"), extraIndex, extraItemPrint.getString());
					OutputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("hksetdata")))) {
			// Print HotkeySet data
			const HotkeySetData* pHksDataTemp = theAppPtr->getAppHotkeySetData();
			if (pHksDataTemp != NULL) {
				// Prepare for replying
				bNoReply = false;	// Reset flag
				// Print item number
				int itemNum = pHksDataTemp->getItemNum();
				logOutputResult.format(_T("HotkeySetData: ItemNum = %d"), itemNum);
				OutputDebugLog(logOutputResult, DebugTestTool);
				// Print each item data
				for (int index = 0; index < itemNum; index++) {
					const HotkeySetItem hotkeyItem = pHksDataTemp->getItemAt(index);

					// Print item
					String hotKeyItemPrint;
					hotkeyItem.print(hotKeyItemPrint);
					logOutputResult.format(_T("Index=%d, %s"), index, hotKeyItemPrint.getString());
					OutputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("rmddata")))) {
			// Print Power Reminder data
			const PwrReminderData* pRmdDataTemp = theAppPtr->getAppPwrReminderData();
			if (pRmdDataTemp != NULL) {
				// Prepare for replying
				bNoReply = false;	// Reset flag
				// Print item number
				int itemNum = pRmdDataTemp->getItemNum();
				logOutputResult.format(_T("PwrReminderData: ItemNum = %d"), itemNum);
				OutputDebugLog(logOutputResult, DebugTestTool);
				// Print each item data
				for (int index = 0; index < itemNum; index++) {
					const PwrReminderItem reminderItem = pRmdDataTemp->getItemAt(index);

					// Print item
					String reminderItemPrint;
					reminderItem.print(reminderItemPrint);
					logOutputResult.format(_T("Index=%d, %s"), index, reminderItemPrint.getString());
					OutputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("resourceidmap")))) {
			// Print application resource ID map data
			const SResourceIDMap* pResourceIDMap = GET_RESOURCEID_MAP();
			if (pResourceIDMap != NULL) {
				// Prepare for replying
				bNoReply = false;	// Reset flag
				// Print number of entries
				size_t nSize = pResourceIDMap->getMapCount();
				logOutputResult.format(_T("Resource ID map count=%lld"), nSize);
				OutputDebugLog(logOutputResult, DebugTestTool);
				// Print each resource ID map entry
				for (size_t index = 0; index < nSize; index++) {
					const RESOURCE_ID_MAP_ENTRY& resourceIDMapEntry = pResourceIDMap->getAt(index);
					logOutputResult.format(_T("Index=%lld: { ResourceID=%d, NameID=%s }"), index, resourceIDMapEntry.resourceID, MAKEUNICODE(resourceIDMapEntry.nameID));
					OutputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdmsgset"))) {
		// Get Power Reminder data
		PwrReminderData* pRmdData = theAppPtr->getAppPwrReminderData();
		if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("bkgclr")))) {
			if (tokenCount == 3) {
				// Set message background color by name
				String colorName = tokenList.at(2).c_str();
				DWORD dwRetColorID = GetStringID(StringTable::ColorName, colorName);
				if (dwRetColorID != INT_INVALID) {
					// Set background color
					if (pRmdData != NULL) {
						pRmdData->getCommonStyle().setBkgrdColor(dwRetColorID);
						theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
						OutputDebugLogFormat(_T("Message background color set: %s"), colorName.toUpper().getString());
						bNoReply = false;	// Reset flag
					}
					else {
						OutputDebugLogFormat(_T("Failed to set message background color!!!"));
						bNoReply = false;	// Reset flag
					}
				}
				else {
					// Invalid command
					bInvalidCmdFlag = true;
				}
			}
			else if (((tokenCount > 3) && (tokenCount <= 6)) && (!_tcscmp(tokenList.at(2).c_str(), _T("rgb")))) {
				// Set message background color by RGB value
				int nRValue = (tokenCount >= 4) ? _tstoi(tokenList.at(3).c_str()) : INT_INVALID;		// Red
				int nGValue = (tokenCount >= 5) ? _tstoi(tokenList.at(4).c_str()) : INT_INVALID;		// Green
				int nBValue = (tokenCount >= 6) ? _tstoi(tokenList.at(5).c_str()) : INT_INVALID;		// Blue
				if (((nRValue < 0) || (nRValue > 255)) || ((nGValue < 0) || (nGValue > 255)) || ((nBValue < 0) || (nBValue > 255))) {
					// Invalid argument
					OutputDebugLog(_T("Invalid value (Value range: 0 -> 255)"));
					bNoReply = false;	// Reset flag
				}
				else {
					// Set background color
					COLORREF clrRGB = RGB(nRValue, nGValue, nBValue);
					if (pRmdData != NULL) {
						pRmdData->getCommonStyle().setBkgrdColor(clrRGB);
						theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
						OutputDebugLogFormat(_T("Message background color set: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
						bNoReply = false;	// Reset flag
					}
					else {
						OutputDebugLogFormat(_T("Failed to set message background color!!!"));
						bNoReply = false;	// Reset flag
					}
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("txtclr")))) {
			if (tokenCount == 3) {
				// Set message text color by name
				String colorName = tokenList.at(2).c_str();
				DWORD dwRetColorID = GetStringID(StringTable::ColorName, colorName);
				if (dwRetColorID != INT_INVALID) {
					// Set text color
					if (pRmdData != NULL) {
						pRmdData->getCommonStyle().setTextColor(dwRetColorID);
						theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
						OutputDebugLogFormat(_T("Message text color set: %s"), colorName.toUpper().getString());
						bNoReply = false;	// Reset flag
					}
					else {
						OutputDebugLogFormat(_T("Failed to set message text color!!!"));
						bNoReply = false;	// Reset flag
					}
				}
				else {
					// Invalid command
					bInvalidCmdFlag = true;
				}
			}
			else if (((tokenCount > 3) && (tokenCount <= 6)) && (!_tcscmp(tokenList.at(2).c_str(), _T("rgb")))) {
				// Set message background color by RGB value
				int nRValue = (tokenCount >= 4) ? _tstoi(tokenList.at(3).c_str()) : INT_INVALID;		// Red
				int nGValue = (tokenCount >= 5) ? _tstoi(tokenList.at(4).c_str()) : INT_INVALID;		// Green
				int nBValue = (tokenCount >= 6) ? _tstoi(tokenList.at(5).c_str()) : INT_INVALID;		// Blue
				if (((nRValue < 0) || (nRValue > 255)) || ((nGValue < 0) || (nGValue > 255)) || ((nBValue < 0) || (nBValue > 255))) {
					// Invalid argument
					OutputDebugLog(_T("Invalid value (Value range: 0 -> 255)"));
					bNoReply = false;	// Reset flag
				}
				else {
					// Set text color
					COLORREF clrRGB = RGB(nRValue, nGValue, nBValue);
					if (pRmdData != NULL) {
						pRmdData->getCommonStyle().setTextColor(clrRGB);
						theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
						OutputDebugLogFormat(_T("Message text color set: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
						bNoReply = false;	// Reset flag
					}
					else {
						OutputDebugLogFormat(_T("Failed to set message text color!!!"));
						bNoReply = false;	// Reset flag
					}
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontname")))) {
			// Set reminder message font name
			String strFontName = tokenList.at(2).c_str();
			// Because the input debug command had been converted to lowercase,
			// it needs to be re-formatted by capitalizing first character of each word
			strFontName.trim().upperEachWord();
			// Validate font name
			bool returnFlag = AppCore::validateFontName(strFontName);
			if (returnFlag != true) {
				// Invalid font name
				OutputDebugLog(_T("Invalid font name"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Set message font
				if (pRmdData != NULL) {
					pRmdData->getCommonStyle().setFontName(strFontName);
					theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
					OutputDebugLogFormat(_T("Message font name set: %s"), strFontName.getString());
					bNoReply = false;	// Reset flag
				}
				else {
					OutputDebugLogFormat(_T("Failed to set message font name!!!"));
					bNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontsize")))) {
			// Set reminder message font size
			int nFontSize = _tstoi(tokenList.at(2).c_str());
			if ((nFontSize < RmdMsgStyleSet::minFontSize) || (nFontSize > RmdMsgStyleSet::maxFontSize)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 100)"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Set font size
				if (pRmdData != NULL) {
					pRmdData->getCommonStyle().setFontSize(nFontSize);
					theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
					OutputDebugLogFormat(_T("Message font size set: %dpt"), nFontSize);
					bNoReply = false;	// Reset flag
				}
				else {
					OutputDebugLogFormat(_T("Failed to set message font size!!!"));
					bNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("timeout")))) {
			// Set reminder message auto-close interval (timeout)
			int timeoutValue = _tstoi(tokenList.at(2).c_str());
			if ((timeoutValue < RmdMsgStyleSet::minTimeOut) || (timeoutValue > RmdMsgStyleSet::maxTimeOut)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 1800)"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Set timeout
				if (pRmdData != NULL) {
					pRmdData->getCommonStyle().setTimeout(timeoutValue);
					theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
					OutputDebugLogFormat(_T("Message time-out set: %ds"), timeoutValue);
					bNoReply = false;	// Reset flag
				}
				else {
					OutputDebugLogFormat(_T("Failed to set message time-out!!!"));
					bNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("notimeout")))) {
			// No reminder message timeout (default 0)
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setTimeout(RmdMsgStyleSet::defaultTimeout);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLog(_T("Message time-out disabled"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to set message time-out!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconid")))) {
			// Set message icon ID by name
			String iconName = tokenList.at(2).c_str();
			DWORD dwRetIconID = GetStringID(StringTable::MsgIconName, iconName);
			if (dwRetIconID != INT_INVALID) {
				// Set icon ID
				if (pRmdData != NULL) {
					pRmdData->getCommonStyle().setIconId(dwRetIconID);
					theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
					OutputDebugLogFormat(_T("Message icon ID set: %s (%d)"), iconName.toUpper().getString(), dwRetIconID);
					bNoReply = false;	// Reset flag
				}
				else {
					OutputDebugLogFormat(_T("Failed to set message icon ID!!!"));
					bNoReply = false;	// Reset flag
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("noicon")))) {
			// No reminder message icon (default 0)
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setIconId(0);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLog(_T("Message icon disabled"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to set message icon ID!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconsize")))) {
			// Set reminder message icon size
			int nIconSize = _tstoi(tokenList.at(2).c_str());
			if ((nIconSize < RmdMsgStyleSet::minIconSize) || (nIconSize > RmdMsgStyleSet::maxIconSize)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 30 -> 100)"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Set icon size
				if (pRmdData != NULL) {
					pRmdData->getCommonStyle().setIconSize(nIconSize);
					theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
					OutputDebugLogFormat(_T("Message icon size set: %dx%dpx"), nIconSize, nIconSize);
					bNoReply = false;	// Reset flag
				}
				else {
					OutputDebugLogFormat(_T("Failed to set message icon size!!!"));
					bNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconplacement")))) {
			// Set reminder message icon position
			String iconPosition = tokenList.at(2).c_str();
			if (!_tcscmp(iconPosition, _T("left"))) {
				// Set icon position: Icon on the Left
				if (pRmdData != NULL) {
					pRmdData->getCommonStyle().setIconPosition(RmdMsgStyleSet::IconOnTheLeft);
					theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
					OutputDebugLog(_T("Message icon position set: Left"));
					bNoReply = false;	// Reset flag
				}
				else {
					OutputDebugLogFormat(_T("Failed to set message icon position!!!"));
					bNoReply = false;	// Reset flag
				}
			}
			else if (!_tcscmp(iconPosition, _T("top"))) {
				// Set icon position: Icon on the Top
				if (pRmdData != NULL) {
					pRmdData->getCommonStyle().setIconPosition(RmdMsgStyleSet::IconOnTheTop);
					theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
					OutputDebugLog(_T("Message icon position set: Top"));
					bNoReply = false;	// Reset flag
				}
				else {
					OutputDebugLogFormat(_T("Failed to set message icon position!!!"));
					bNoReply = false;	// Reset flag
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("hmargin")))) {
			// Set reminder message horizontal margin
			int nHMargin = _tstoi(tokenList.at(2).c_str());
			if ((nHMargin < RmdMsgStyleSet::minMarginVal) || (nHMargin > RmdMsgStyleSet::maxMarginVal)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 120)"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Set margin
				if (pRmdData != NULL) {
					pRmdData->getCommonStyle().setHorizontalMargin(nHMargin);
					theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
					OutputDebugLogFormat(_T("Message horizontal margin set: %dpx"), nHMargin);
					bNoReply = false;	// Reset flag
				}
				else {
					OutputDebugLogFormat(_T("Failed to set message horizontal margin!!!"));
					bNoReply = false;	// Reset flag
				}
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("vmargin")))) {
			// Set reminder message vertical margin
			int nVMargin = _tstoi(tokenList.at(2).c_str());
			if ((nVMargin < RmdMsgStyleSet::minMarginVal) || (nVMargin > RmdMsgStyleSet::maxMarginVal)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 120)"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Set margin
				if (pRmdData != NULL) {
					pRmdData->getCommonStyle().setVerticalMargin(nVMargin);
					theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
					OutputDebugLogFormat(_T("Message vertical margin set: %dpx"), nVMargin);
					bNoReply = false;	// Reset flag
				}
				else {
					OutputDebugLogFormat(_T("Failed to set message vertical margin!!!"));
					bNoReply = false;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdmsgreset"))) {
		// Get Power Reminder data
		PwrReminderData* pRmdData = theAppPtr->getAppPwrReminderData();
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("bkgclr")))) {
			// Reset message background color
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setBkgrdColor(RmdMsgStyleSet::defaultBkgrdColor);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLogFormat(_T("Message background color reset"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to reset message background color!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("txtclr")))) {
			// Set message text color by name
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setTextColor(RmdMsgStyleSet::defaultTextColor);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLogFormat(_T("Message text color reset"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to reset message text color!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontname")))) {
			// Set reminder message font name
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setFontName(RmdMsgStyleSet::defaultFontName);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLogFormat(_T("Message font name reset"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to reset message font name!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontsize")))) {
			// Set reminder message font size
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setFontSize(RmdMsgStyleSet::defaultFontSize);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLogFormat(_T("Message font size reset"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to reset message font size!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("timeout")))) {
			// Reset reminder message auto-close interval (time-out)
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setTimeout(RmdMsgStyleSet::defaultTimeout);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLog(_T("Message time-out reset"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to reset message time-out!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconid")))) {
			// Reset reminder message icon ID
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setIconId(RmdMsgStyleSet::defaultIconID);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLog(_T("Message icon ID reset"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to reset message icon ID!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconsize")))) {
			// Reset reminder message icon size
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setIconSize(RmdMsgStyleSet::defaultIconSize);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLog(_T("Message icon size reset"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to reset message icon size!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconpos")))) {
			// Reset reminder message icon position
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setIconPosition(RmdMsgStyleSet::defaultIconPosition);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLog(_T("Message icon position reset"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to reset message icon position!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("hmargin")))) {
			// Reset reminder message horizontal margin
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setHorizontalMargin(RmdMsgStyleSet::defaultHorizontalMargin);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLogFormat(_T("Message horizontal margin reset)"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to reset message horizontal margin!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("vmargin")))) {
			// Reset reminder message vertical margin
			if (pRmdData != NULL) {
				pRmdData->getCommonStyle().setVerticalMargin(RmdMsgStyleSet::defaultVerticalMargin);
				theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
				OutputDebugLogFormat(_T("Message vertical margin reset"));
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to reset message vertical margin!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdmsgget"))) {
		// Get Power Reminder data
		PwrReminderData* pRmdData = theAppPtr->getAppPwrReminderData();
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("bkgclr")))) {
			// Get reminder message background color
			if (pRmdData != NULL) {
				DWORD dwBkgrdColor = pRmdData->getCommonStyle().getBkgrdColor();
				int nBValue = (dwBkgrdColor & 0x00FF0000) >> 16;	// Blue
				int nGValue = (dwBkgrdColor & 0x0000FF00) >> 8;		// Green
				int nRValue = (dwBkgrdColor & 0x000000FF);			// Red
				OutputDebugLogFormat(_T("Message background color: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to get message background color!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("txtclr")))) {
			// Get reminder message text color
			if (pRmdData != NULL) {
				DWORD dwTextColor = pRmdData->getCommonStyle().getTextColor();
				int nBValue = (dwTextColor & 0x00FF0000) >> 16;		// Blue
				int nGValue = (dwTextColor & 0x0000FF00) >> 8;		// Green
				int nRValue = (dwTextColor & 0x000000FF);			// Red
				OutputDebugLogFormat(_T("Message text color: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to get message text color!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontname")))) {
			// Get reminder message font name
			if (pRmdData != NULL) {
				String fontName = pRmdData->getCommonStyle().getFontName();
				OutputDebugLogFormat(_T("Message font name: %s"), fontName.getString());
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to get message font name!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontsize")))) {
			// Get reminder message font size
			if (pRmdData != NULL) {
				int nFontSize = pRmdData->getCommonStyle().getFontSize();
				OutputDebugLogFormat(_T("Message font size: %dpt"), nFontSize);
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to get message font size!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("timeout")))) {
			// Get reminder message auto-close interval (time-out)
			if (pRmdData != NULL) {
				int timeoutValue = pRmdData->getCommonStyle().getTimeout();
				OutputDebugLogFormat(_T("Message time-out: %ds"), timeoutValue);
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to get message time-out!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconid")))) {
			// Get reminder message icon ID
			if (pRmdData != NULL) {
				unsigned nIconID = pRmdData->getCommonStyle().getIconId();
				OutputDebugLogFormat(_T("Message icon ID: %d"), nIconID);
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to get message icon ID!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconsize")))) {
			// Get reminder message icon size
			if (pRmdData != NULL) {
				int nIconSize = pRmdData->getCommonStyle().getIconSize();
				OutputDebugLogFormat(_T("Message icon size: %dx%dpx"), nIconSize, nIconSize);
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to get message icon size!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconpos")))) {
			// Get reminder message icon position
			if (pRmdData != NULL) {
				BYTE byIconPosition = pRmdData->getCommonStyle().getIconPosition();
				if (byIconPosition == RmdMsgStyleSet::IconOnTheLeft) {
					OutputDebugLog(_T("Message icon position: Left"));
					bNoReply = false;	// Reset flag
				}
				else if (byIconPosition == RmdMsgStyleSet::IconOnTheTop) {
					OutputDebugLog(_T("Message icon position: Top"));
					bNoReply = false;	// Reset flag
				}
				else {
					OutputDebugLog(_T("Message icon position: Unknown"));
					bNoReply = false;	// Reset flag
				}
			}
			else {
				OutputDebugLogFormat(_T("Failed to get message icon position!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("hmargin")))) {
			// Get reminder message horizontal margin
			if (pRmdData != NULL) {
				int nHMargin = pRmdData->getCommonStyle().getHorizontalMargin();
				OutputDebugLogFormat(_T("Message horizontal margin: %dpx"), nHMargin);
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to get message horizontal margin!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("vmargin")))) {
			// Get reminder message vertical margin
			if (pRmdData != NULL) {
				int nVMargin = pRmdData->getCommonStyle().getVerticalMargin();
				OutputDebugLogFormat(_T("Message vertical margin: %dpx"), nVMargin);
				bNoReply = false;	// Reset flag
			}
			else {
				OutputDebugLogFormat(_T("Failed to get message vertical margin!!!"));
				bNoReply = false;	// Reset flag
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("runtimequeue"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("print")))) {
			// Print Power++ runtime queue list
			if (runtimeQueue_.empty()) {
				// Empty list
				OutputDebugLog(_T("Runtime queue empty!"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Print each item
				PwrRuntimeItem pwrRuntimeItem;
				for (int index = 0; index < runtimeQueue_.size(); index++) {
					// Get runtime item from queue
					pwrRuntimeItem = runtimeQueue_.at(index);
					// Print runtime item info
					OutputDebugLogFormat(_T("Item%03d: CategoryID=%d, ItemID=%d, Display=%d, Skip=%d, Snooze=%d"), index, pwrRuntimeItem.getCategory(),
						pwrRuntimeItem.getItemId(), pwrRuntimeItem.getDisplayFlag(), pwrRuntimeItem.getSkipFlag(), pwrRuntimeItem.getSnoozeFlag());
					bNoReply = false;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdsnooze"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("prntqueue")))) {
			// Print Power Reminder snooze queue list
			if (runtimeQueue_.empty()) {
				// Empty list
				OutputDebugLog(_T("Reminder snooze queue empty!"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Print each item
				PwrRuntimeItem pwrRuntimeItem;
				for (int index = 0; index < runtimeQueue_.size(); index++) {
					// Get runtime item from queue
					pwrRuntimeItem = runtimeQueue_.at(index);
					// Skip if it's not Power Reminder item
					if (pwrRuntimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;
					// Print runtime item info
					ClockTime timeNextSnooze = pwrRuntimeItem.getTime();
					OutputDebugLogFormat(_T("Item%03d: ID=%d, Snooze=%d, NextTrigger=%02d:%02d"), index, pwrRuntimeItem.getItemId(),
						pwrRuntimeItem.getSnoozeFlag(), timeNextSnooze.hour(), timeNextSnooze.minute());
					bNoReply = false;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdruntime"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("prntdata")))) {
			// Print Power Reminder runtime data list
			if (runtimeQueue_.empty()) {
				// Empty list
				OutputDebugLog(_T("Reminder runtime data empty!"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Print each item
				PwrRuntimeItem pwrRuntimeItem;
				for (int index = 0; index < runtimeQueue_.size(); index++) {
					// Get runtime item from queue
					pwrRuntimeItem = runtimeQueue_.at(index);
					// Skip if it's not Power Reminder item
					if (pwrRuntimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;
					// Print runtime item info
					ClockTime timeNextSnooze = pwrRuntimeItem.getTime();
					OutputDebugLogFormat(_T("Item%03d: ID=%d, Display=%d, Snooze=%d, NextTrigger=%02d:%02d"), index, pwrRuntimeItem.getItemId(),
						pwrRuntimeItem.getDisplayFlag(), pwrRuntimeItem.getSnoozeFlag(), timeNextSnooze.hour(), timeNextSnooze.minute());
					bNoReply = false;	// Reset flag
				}
			}
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("upper"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("string")))) {
			// Upper each word
			String strInput = tokenList.at(2).c_str();
			strInput.trim().upperEachWord();
			OutputDebugLogFormat(_T("Upper: %s"), strInput.getString());
			bNoReply = false;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("enum"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontnames")))) {
			// Enumerate all currently available fonts
			std::vector<std::wstring> fontNames;
			bool returnFlag = AppCore::enumFontNames(fontNames);
			if (returnFlag == false) {
				// Enumerate fonts failed
				OutputDebugLog(_T("Enumerate fonts failed"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Print all font names
				BeginWaitCursor();
				{
					// Prepare for replying
					bNoReply = false;	// Reset flag
					for (auto it = fontNames.begin(); it != fontNames.end(); it++) {
						OutputDebugLogFormat(_T("Font [%d]: %s"), (it - fontNames.begin()), (*it).c_str());
						Sleep(50);  // wait for a blink
					}
				}
				EndWaitCursor();
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
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
			OutputDebugLog(logOutputResult);
			bNoReply = false;	// Reset flag
		}
		else {
			// Get last system suspend time failed
			OutputDebugLog(_T("Get last system suspend time failed"));
			bNoReply = false;	// Reset flag
		}
		// Get last system wakeup time
		if (theAppPtr->getLastSysEventTime(SystemEventID::SystemWakeUp, dateTimeTemp)) {
			// Format date time
			const wchar_t* middayFlag = (dateTimeTemp.hour() >= 12) ? _T("PM") : _T("AM");
			String logTemp = StringUtils::stringFormat(dateTimeFormat, dateTimeTemp.year(), dateTimeTemp.month(), dateTimeTemp.day(),
				dateTimeTemp.hour(), dateTimeTemp.minute(), dateTimeTemp.second(), dateTimeTemp.millisecond(), middayFlag);
			logOutputResult.format(_T("Last System Wakeup: %s"), logTemp.getString());
			OutputDebugLog(logOutputResult);
			bNoReply = false;	// Reset flag
		}
		else {
			// Get last system wakeup time failed
			OutputDebugLog(_T("Get last system wakeup time failed"));
			bNoReply = false;	// Reset flag
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("powerbroadcast"))) {
		if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("param")))) {
			// Simulate system power event (WM_POWERBROADCAST message)
			int nParam = _tstoi(tokenList.at(2).c_str());
			if ((nParam == PBT_APMPOWERSTATUSCHANGE) ||
				(nParam == PBT_APMRESUMEAUTOMATIC) ||
				(nParam == PBT_APMRESUMESUSPEND) ||
				(nParam == PBT_APMSUSPEND) ||
				(nParam == PBT_POWERSETTINGCHANGE)) {
				// Post message
				PostMessage(WM_POWERBROADCAST, (WPARAM)nParam, NULL);
				OutputDebugLogFormat(_T("Message posted: Param=%d"), nParam);
				bNoReply = false;	// Reset flag
			}
			else {
				// Invalid param
				OutputDebugLog(_T("Invalid parameter"));
				bNoReply = false;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("skip")))) {
			// Skip system power event (WM_POWERBROADCAST message)
			int nSkipCount = _tstoi(tokenList.at(2).c_str());
			if ((nSkipCount <= 0) || (nSkipCount > 3600)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 1 -> 3600)"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Set skip counter
				setFlagValue(AppFlagID::pwrBroadcastSkipCount, nSkipCount);
				OutputDebugLogFormat(_T("Skip PowerBroadcast event: Counter=%d"), nSkipCount);
				bNoReply = false;	// Reset flag
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = true;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("flagset"))) {
		if (!_tcscmp(tokenList.at(1).c_str(), _T("pwraction"))) {
			// Turn ON/OFF power action flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				setPwrActionFlag(FLAG_ON);
				OutputDebugLog(_T("Power action flag: ON"));
				bNoReply = false;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				setPwrActionFlag(FLAG_OFF);
				OutputDebugLog(_T("Power action flag: OFF"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("syssuspend"))) {
			// Turn ON/OFF system suspend flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				setSystemSuspendFlag(FLAG_ON);
				OutputDebugLog(_T("System suspend flag: ON"));
				bNoReply = false;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				setSystemSuspendFlag(FLAG_OFF);
				OutputDebugLog(_T("System suspend flag: OFF"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("sessionend"))) {
			// Turn ON/OFF session end flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				setSessionEndFlag(FLAG_ON);
				OutputDebugLog(_T("Session end flag: ON"));
				bNoReply = false;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				setSessionEndFlag(FLAG_OFF);
				OutputDebugLog(_T("Session end flag: OFF"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("sessionlock"))) {
			// Turn ON/OFF session lock flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				setSessionLockFlag(FLAG_ON);
				OutputDebugLog(_T("Session lock flag: ON"));
				bNoReply = false;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				setSessionLockFlag(FLAG_OFF);
				OutputDebugLog(_T("Session lock flag: OFF"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("safetermination"))) {
			// Turn ON/OFF previously safe termination trace flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				setSafeTerminationFlag(FLAG_ON);
				OutputDebugLog(_T("Previously safe termination trace flag: ON"));
				bNoReply = false;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				setSafeTerminationFlag(FLAG_OFF);
				OutputDebugLog(_T("Previously safe termination trace flag: OFF"));
				bNoReply = false;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = true;
			}
		}
	}
	else {
		// Invalid command
		bInvalidCmdFlag = true;
	}

	/*********************************************************************/
	/*																	 */
	/*					Post-processing and clean-up				     */
	/*																	 */
	/*********************************************************************/

	// If command is invalid
	if (bInvalidCmdFlag == true) {
		// Error: Invalid command
		errorCode = APP_ERROR_DBG_INVALID_COMMAND;
		bNoReply = false;	// Reset flag
		returnFlag = false;		// Return failed
	}

	// If command is executed but no reply
	if (bNoReply == true) {
		// Notify to the DebugTest dialog
		SDialog* pDebugTestDlg = theAppPtr->getDebugTestDlg();
		if (pDebugTestDlg != NULL) {
			pDebugTestDlg->PostMessage(SM_APP_DEBUGCMD_NOREPLY);
		}
		if (bNoReplySilent != true) {
			OutputDebugLog(_T("Command sent, no reply!!!"));
		}
		bNoReply = false;			// Reset flag
		bNoReplySilent = false;		// Reset flag
		returnFlag = true;				// Return successful
	}

	return returnFlag;
}

