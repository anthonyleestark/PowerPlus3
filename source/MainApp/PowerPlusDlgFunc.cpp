
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PowerPlusDlg.cpp
//		Description:	Define necessary functions using for main window
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.24:		Create new
//						<1> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ProcessDebugCommand
//	Description:	Process app debug commands
//  Arguments:		commandString - Debug command string
//					errorCode	  - Returned error code
//  Return value:	BOOL
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ProcessDebugCommand(const wchar_t* commandString, DWORD& errorCode)
{
	String debugCommand(commandString);

	// If debug command is empty, do nothing
	if (debugCommand.IsEmpty()) {
		// Error: Debug command empty
		errorCode = APP_ERROR_DBG_EMPTY;
		TRACE("Debug command is empty!!!");
		return FALSE;
	}

	// Get application class
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) {
		// Error: Get app failed
		errorCode = APP_ERROR_DBG_GETAPP_FAILED;
		// Trace error
		TRACE_ERROR("Error: Get app pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Format debug command
	debugCommand.ToLower();

	// Debug log output result
	String logOutputResult;

	// Return result
	BOOL bRet = TRUE;

	// No replying flag
	BOOL bNoReply = TRUE;
	BOOL bNoReplySilent = FALSE;

	// Invalid command flag
	BOOL bInvalidCmdFlag = FALSE;

	/*********************************************************************/
	/*																	 */
	/*					Process basic debug commands				     */
	/*																	 */
	/*********************************************************************/

	if (!_tcscmp(debugCommand, _T("exit"))) {
		// Exit application
		ExitApp(ExitCode::FromDebugCommand);
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("reload"))) {
		// Reload settings
		ReloadSettings();
		logOutputResult.Format(_T("Settings reloaded!!!"));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("setdefault"))) {
		// Set default settings
		SetDefaultConfig();
		logOutputResult.Format(_T("Reset default settings!!!"));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("restartapp"))) {
		// Restart app (non-admin)
		RequestRestartApp(IDD_DEBUGTEST_DLG, FALSE);
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("restartadmin"))) {
		// Restart app (as admin)
		RequestRestartApp(IDD_DEBUGTEST_DLG, TRUE);
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("logviewer"))) {
		// Open Logviewer dialog
		OpenChildDialogEx(IDD_LOGVIEWER_DLG);
		logOutputResult.Format(_T("Logviewer opened!!!"));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("hksupdate"))) {
		// Update HotkeySet settings
		PostMessage(SM_APP_UPDATE_HOTKEYSETDATA);
		logOutputResult.Format(_T("HotkeySet data updated!!!"));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("rmdupdate"))) {
		// Update Power Reminder data
		PostMessage(SM_APP_UPDATE_PWRREMINDERDATA);
		logOutputResult.Format(_T("Power Reminder data updated!!!"));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("bakconfig"))) {
		// Backup configuration
		BackupSystem::RegistryExport();
		logOutputResult.Format(_T("Config backed-up!!!"));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("viewbakconfig"))) {
		// View backup configuration file
		OpenTextFileToView(FILENAME_BAKCONFIG, FILEEXT_REGFILE);
		logOutputResult.Format(_T("Opening backup config file..."));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("viewhistory"))) {
		// View action history log file
		OpenTextFileToView(FILENAME_HISTORY_LOG, FILEEXT_LOGFILE, SUBFOLDER_LOG);
		logOutputResult.Format(_T("Opening action history log file..."));
		OutputDebugLog(logOutputResult, DebugTestTool);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("clrscr"))) {
		// Clear debug view screen buffer
		HWND hDebugTestDlg = FindDebugTestDlg();
		if (hDebugTestDlg != NULL) {
			// Post clear screen message
			::PostMessage(hDebugTestDlg, SM_WND_DEBUGVIEW_CLRSCR, NULL, NULL);
		}
		return TRUE;
	}
	else if (!_tcscmp(debugCommand, _T("close"))) {
		// Close DebugTest dialog if opening
		SDialog* pDebugTestDlg = pApp->GetDebugTestDlg();
		if (pDebugTestDlg != NULL) {
			pDebugTestDlg->ShowWindow(SW_HIDE);
			pDebugTestDlg->PostMessage(SM_APP_DEBUGCMD_NOREPLY);
		}
		return TRUE;
	}

	/*********************************************************************/
	/*																	 */
	/*			Process advanced/multi-token debug commands				 */
	/*																	 */
	/*********************************************************************/

	// Make token list (break string buffer)
	TokenList tokenList = debugCommand.Tokenize(_T("=,;:-_"));
	size_t tokenCount = tokenList.size();

	// Validate number of tokens
	if (tokenCount <= 0) {
		// Error: Tokenization failed
		errorCode = APP_ERROR_DBG_TOKENIZATION_FAILED;
		return FALSE;
	}

	String strTemp = Constant::String::Empty;

	// Process debug commands by tokens
	if (!_tcscmp(tokenList.at(0).c_str(), _T("test"))) {
		// Debug command token test
		if (tokenCount > 1) {
			// Prepare for replying
			bNoReply = FALSE;	// Reset flag
			// Print token list
			int nTokenCount = (tokenCount - 1);
			OutputDebugLogFormat(_T("Token number: %d"), nTokenCount);
			for (int nTokenIndex = 0; nTokenIndex < nTokenCount; nTokenIndex++) {
				strTemp = tokenList.at(nTokenIndex + 1);
				OutputDebugLogFormat(_T("Token[%d]: %s"), nTokenIndex, strTemp.GetString());
			}
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("dmytest"))) {
		// Set dummy test mode
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("on")))) {
			SetDummyTestMode(TRUE);
			OutputDebugLog(_T("DummyTest ON"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("off")))) {
			SetDummyTestMode(FALSE);
			OutputDebugLog(_T("DummyTest OFF"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
			SetDummyTestMode(DEFAULT_DUMMYTEST);
			OutputDebugLog(_T("DummyTest reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("dmyteststs"))) {
		// Get dummy test mode
		int nRet = GetDummyTestMode();
		OutputDebugLogFormat(_T("DummyTest status = %d"), nRet);
		bNoReply = FALSE;	// Reset flag
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("debugmode"))) {
		// Set debug mode
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("on")))) {
			SetDebugMode(TRUE);
			OutputDebugLog(_T("DebugMode ON"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("off")))) {
			SetDebugMode(FALSE);
			OutputDebugLog(_T("DebugMode OFF"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
			SetDebugMode(DEFAULT_DEBUGMODE);
			OutputDebugLog(_T("DebugMode reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("debugmodests"))) {
		// Get debug mode
		int nRet = GetDebugMode();
		OutputDebugLogFormat(_T("DebugMode status = %d"), nRet);
		bNoReply = FALSE;	// Reset flag
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("debuglog"))) {
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("output")))) {
			// Set debug log output target
			if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("default")))) {
				SetDebugOutputTarget(DefaultOutput);
				OutputDebugLog(_T("Debug log output target changed"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("tofile")))) {
				SetDebugOutputTarget(DebugInfoFile);
				OutputDebugLog(_T("Debug log output target changed"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("todbtool")))) {
				SetDebugOutputTarget(DebugTestTool);
				OutputDebugLog(_T("Debug log output target changed"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
				SetDebugOutputTarget(DEFAULT_DEBUGOUTPUT);
				OutputDebugLog(_T("Debug log output target reset"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("getoutput")))) {
			// Get debug log output target
			int nRet = GetDebugOutputTarget();
			OutputDebugLogFormat(_T("DebugLogOutputTarget = %d"), nRet);
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("testfeature"))) {
		// Set test feature enable
		if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("on")))) {
			SetTestFeatureEnable(TRUE);
			OutputDebugLog(_T("TestFeature ON"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("off")))) {
			SetTestFeatureEnable(FALSE);
			OutputDebugLog(_T("TestFeature OFF"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount >= 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("reset")))) {
			SetTestFeatureEnable(DEFAULT_TESTFEATURE);
			OutputDebugLog(_T("TestFeature reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("testfeaturests"))) {
		// Get test feature enable
		int nRet = GetTestFeatureEnable();
		OutputDebugLogFormat(_T("TestFeature status = %d"), nRet);
		bNoReply = FALSE;	// Reset flag
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("logtest"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("trcerr")))) {
			// Trace error log test
			int nVal = _tstoi(tokenList.at(2).c_str());
			if (nVal >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				bNoReply = FALSE;	// Reset flag
				if (nVal >= 50) {
					// Output waiting notification
					OutputDebugLog(_T("Wait for a while!!!"), DebugTestTool);
					WaitMessage(SM_WND_DEBUGOUTPUT_DISP); // wait for the notification displaying
				}
				for (int i = 0; i < nVal; i++) {
					// Write test trace error log
					TraceErrorFormat("[TraceError Test] Loop time: %d", i);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				OutputDebugLog(_T("TraceErrorLog test done!!!"), DebugTestTool);
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("trcdebug")))) {
			// Trace debug log test
			int nVal = _tstoi(tokenList.at(2).c_str());
			if (nVal >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				bNoReply = FALSE;	// Reset flag
				if (nVal >= 50) {
					// Output waiting notification
					OutputDebugLog(_T("Wait for a while!!!"), DebugTestTool);
					WaitMessage(SM_WND_DEBUGOUTPUT_DISP); // wait for the notification displaying
				}
				String strFormat;
				for (int i = 0; i < nVal; i++) {
					// Write test trace debug log
					strFormat.Format(_T("[TraceDebugLog Test] Loop time: %d"), i);
					WriteTraceDebugLogFile(strFormat);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				OutputDebugLog(_T("TraceDebugLog test done!!!"), DebugTestTool);
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("debuginfo")))) {
			// Output debug info log test
			int nVal = _tstoi(tokenList.at(2).c_str());
			if (nVal >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				bNoReply = FALSE;	// Reset flag
				if (nVal >= 50) {
					// Output waiting notification
					OutputDebugLog(_T("Wait for a while!!!"), DebugTestTool);
					WaitMessage(SM_WND_DEBUGOUTPUT_DISP); // wait for the notification displaying
				}
				String strFormat;
				for (int i = 0; i < nVal; i++) {
					// Write test debug info log
					strFormat.Format(_T("[OutputDebugInfo Test] Loop time: %d"), i);
					OutputDebugLog(strFormat, DebugTestTool);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				OutputDebugLog(_T("DebugInfoLog test done!!!"), DebugTestTool);
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("errtest"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("code")))) {
			// Test show error message
			int nErrCode = _tstoi(tokenList.at(2).c_str());
			if (nErrCode >= 0) {
				if (GetAppOption(AppOptionID::showErrorMessage) == FALSE) {
					OutputDebugLog(_T("Show error message OFF"));
					bNoReply = FALSE;	// Reset flag
				}
				else {
					// Show error message
					ShowErrorMessage(nErrCode);
					OutputDebugLog(_T("Error message showed!!!"));
					bNoReply = FALSE;	// Reset flag
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("appeventlog"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("clear")))) {
			// Clear all app event log data in memory
			SLogging* pAppEventLog = ((CPowerPlusApp*)AfxGetApp())->GetAppEventLog();
			if (pAppEventLog != NULL) {
				pAppEventLog->Init();
				OutputDebugLog(_T("App event log data cleared"));
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("write")))) {
			// Force writing app event log data from memory to file
			SLogging* pAppEventLog = ((CPowerPlusApp*)AfxGetApp())->GetAppEventLog();
			if (pAppEventLog != NULL) {
				BOOL bRet = pAppEventLog->Write();
				if (bRet == TRUE) {
					// Write log succeeded
					OutputDebugLog(_T("App event log data written"));
					bNoReply = FALSE;	// Reset flag
				}
				else {
					// Write log failed
					OutputDebugLog(_T("Write app event log data failed"));
					bNoReply = FALSE;	// Reset flag
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
			String fileName = StringUtils::MakeFilePath(StringUtils::GetSubFolderPath(SUBFOLDER_LOG), _T("*"), FILEEXT_LOGFILE);
			BOOL bFindRet = Finder.FindFile(fileName);
			if (!strKeyName.IsEmpty()) {
				int nDelFileCount = 0;
				while (bFindRet == TRUE) {
					bFindRet = Finder.FindNextFile();
					fileName = Finder.GetFileName().GetString();
					if (fileName.Find(strKeyName) != INT_INVALID) {
						// Delete file
						CFile::Remove(Finder.GetFilePath());
						nDelFileCount++;	// Increase counter
					}
				}
				if (nDelFileCount > 0) {
					// Output number of deleted files
					logOutputResult.Format(_T("App event log file(s) deleted (Count=%d)"), nDelFileCount);
					OutputDebugLog(logOutputResult, DebugTestTool);
					bNoReply = FALSE;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("reminderdisp"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("id")))) {
			int nItemID = _tstoi(tokenList.at(2).c_str());
			// Find and display reminder item by ID
			BOOL bFindRet = FALSE;
			PwrReminderItem pwrTemp;
			int nItemNum = m_prdReminderData.GetItemNum();
			for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
				pwrTemp = m_prdReminderData.GetItemAt(nIndex);
				if (pwrTemp.GetItemID() == static_cast<UINT>(nItemID)) {
					bFindRet = TRUE;
					DisplayPwrReminder(pwrTemp);
					OutputDebugLog(_T("Reminder item displayed!!!"));
					bNoReply = FALSE;	// Reset flag
					break;
				}
			}
			if (bFindRet == FALSE) {
				// Item not found
				OutputDebugLog(_T("Reminder item not found"));
				bNoReply = FALSE;	// Reset flag
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("saveglobaldata"))) {
		if (tokenCount == 1) {
			// Save all global data variables
			pApp->SaveGlobalData(0xFF);
			logOutputResult.Format(_T("Global variables stored"));
			OutputDebugLog(logOutputResult, DebugTestTool);
			bNoReply = FALSE;	// Reset flag
		}
		else if (tokenCount == 2) {
			if (!_tcscmp(tokenList.at(1).c_str(), _T("all"))) {
				// Save all global variables
				pApp->SaveGlobalData(0xFF);
				logOutputResult.Format(_T("Global variables stored"));
				OutputDebugLog(logOutputResult, DebugTestTool);
				bNoReply = FALSE;	// Reset flag
			}
			else if (!_tcscmp(tokenList.at(1).c_str(), _T("dbtest"))) {
				// Save debugging/testing config (global variables)
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_DEBUGTEST);
				logOutputResult.Format(_T("Debug/test config stored"));
				OutputDebugLog(logOutputResult, DebugTestTool);
				bNoReply = FALSE;	// Reset flag
			}
			else if (!_tcscmp(tokenList.at(1).c_str(), _T("appflags"))) {
				// Save app flags (global variables)
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
				logOutputResult.Format(_T("Global app flags stored"));
				OutputDebugLog(logOutputResult, DebugTestTool);
				bNoReply = FALSE;	// Reset flag
			}
			else if (!_tcscmp(tokenList.at(1).c_str(), _T("features"))) {
				// Save special variables (global variables)
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				logOutputResult.Format(_T("Global special feature variables stored"));
				OutputDebugLog(logOutputResult, DebugTestTool);
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("print"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("config")))) {
			// Print app config data
			PCONFIGDATA pcfgDataTemp = pApp->GetAppConfigData();
			if (pcfgDataTemp != NULL) {
				// Prepare for replying
				bNoReply = FALSE;	// Reset flag
				// Load app language package
				LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();
				// Format and print data
				String strValue = Constant::String::Empty;
				// Left mouse button action
				int nActionStringID = GetPairedID(IDTable::ActionName, pcfgDataTemp->nLMBAction);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::LMBAction, GetLanguageString(ptrLanguage, nActionStringID));
				// Middle mouse button action
				nActionStringID = GetPairedID(IDTable::ActionName, pcfgDataTemp->nMMBAction);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::MMBAction, GetLanguageString(ptrLanguage, nActionStringID));
				// Right mouse button action
				nActionStringID = GetPairedID(IDTable::ActionName, pcfgDataTemp->nRMBAction);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::RMBAction, GetLanguageString(ptrLanguage, nActionStringID));
				// Right mouse button: Only show menu
				strValue = ((pcfgDataTemp->bRMBShowMenu) ? Constant::Value::True : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::RMBShowMenu, strValue.GetString());
				// Language setting
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::LanguageID, GetLanguageName(pcfgDataTemp->nLanguageID));
				// Show dialog at startup
				strValue = ((pcfgDataTemp->bShowDlgAtStartup) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::ShowDlgAtStartup, strValue.GetString());
				// Startup with Windows
				strValue = ((pcfgDataTemp->bStartupEnabled) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::StartupEnabled, strValue.GetString());
				// Show confirm message before executing action
				strValue = ((pcfgDataTemp->bConfirmAction) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::ConfirmAction, strValue.GetString());
				// Save action log
				strValue = ((pcfgDataTemp->bSaveHistoryLog) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::SaveHistoryLog, strValue.GetString());
				// Save app event log
				strValue = ((pcfgDataTemp->bSaveAppEventLog) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::SaveAppEventLog, strValue.GetString());
				// Run with admin privileges
				strValue = ((pcfgDataTemp->bRunAsAdmin) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::RunAsAdmin, strValue.GetString());
				// Show action error message
				strValue = ((pcfgDataTemp->bShowErrorMsg) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::ShowErrorMsg, strValue.GetString());
				// Show notify tip for schedule action
				strValue = ((pcfgDataTemp->bNotifySchedule) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::NotifySchedule, strValue.GetString());
				// Allow canceling schedule when notify
				strValue = ((pcfgDataTemp->bAllowCancelSchedule) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::AllowCancelSchedule, strValue.GetString());
				// Enable background action hotkeys
				strValue = ((pcfgDataTemp->bEnableBackgroundHotkey) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::EnableBackgroundHotkey, strValue.GetString());
				// Allow background hotkeys on lockscreen
				strValue = ((pcfgDataTemp->bLockStateHotkey) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::LockStateHotkey, strValue.GetString());
				// Enable Power Peminder feature
				strValue = ((pcfgDataTemp->bEnablePowerReminder) ? Constant::Value::True : Constant::Value::False);
				OutputDebugLogFormat(_T("%s=%s"), Key::ConfigData::EnablePowerReminder, strValue.GetString());
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("schedule")))) {
			// Print schedule data
			ScheduleData* pSchedDataTemp = pApp->GetAppScheduleData();
			if (pSchedDataTemp != NULL) {
				// Prepare for replying
				bNoReply = FALSE;	// Reset flag
				// Print default schedule
				String defaultItemPrint;
				pSchedDataTemp->GetDefaultItem().Print(defaultItemPrint);
				logOutputResult.Format(_T("DefaultSchedule: %s"), defaultItemPrint.GetString());
				OutputDebugLog(logOutputResult, DebugTestTool);
				// Print extra item number
				int nExtraItemNum = pSchedDataTemp->GetExtraItemNum();
				logOutputResult.Format(_T("ScheduleExtraData: ItemNum = %d"), nExtraItemNum);
				OutputDebugLog(logOutputResult, DebugTestTool);
				// Print each item data
				for (int nExtraIndex = 0; nExtraIndex < nExtraItemNum; nExtraIndex++) {
					ScheduleItem schExtraItem = pSchedDataTemp->GetItemAt(nExtraIndex);

					// Print item
					String extraItemPrint;
					schExtraItem.Print(extraItemPrint);
					logOutputResult.Format(_T("Index=%d, %s"), nExtraIndex, extraItemPrint.GetString());
					OutputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("hksetdata")))) {
			// Print HotkeySet data
			HotkeySetData* pHksDataTemp = pApp->GetAppHotkeySetData();
			if (pHksDataTemp != NULL) {
				// Prepare for replying
				bNoReply = FALSE;	// Reset flag
				// Print item number
				int nItemNum = pHksDataTemp->GetItemNum();
				logOutputResult.Format(_T("HotkeySetData: ItemNum = %d"), nItemNum);
				OutputDebugLog(logOutputResult, DebugTestTool);
				// Print each item data
				for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
					HotkeySetItem hksItem = pHksDataTemp->GetItemAt(nIndex);

					// Print item
					String hotKeyItemPrint;
					hksItem.Print(hotKeyItemPrint);
					logOutputResult.Format(_T("Index=%d, %s"), nIndex, hotKeyItemPrint.GetString());
					OutputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("rmddata")))) {
			// Print Power Reminder data
			PwrReminderData* pRmdDataTemp = pApp->GetAppPwrReminderData();
			if (pRmdDataTemp != NULL) {
				// Prepare for replying
				bNoReply = FALSE;	// Reset flag
				// Print item number
				int nItemNum = pRmdDataTemp->GetItemNum();
				logOutputResult.Format(_T("PwrReminderData: ItemNum = %d"), nItemNum);
				OutputDebugLog(logOutputResult, DebugTestTool);
				// Print each item data
				for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
					PwrReminderItem pwrItem = pRmdDataTemp->GetItemAt(nIndex);

					// Print item
					String reminderItemPrint;
					pwrItem.Print(reminderItemPrint);
					logOutputResult.Format(_T("Index=%d, %s"), nIndex, reminderItemPrint.GetString());
					OutputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("resourceidmap")))) {
			// Print application resource ID map data
			SResourceIDMap* pResourceIDMap = GET_RESOURCEID_MAP();
			if (pResourceIDMap != NULL) {
				// Prepare for replying
				bNoReply = FALSE;	// Reset flag
				// Print number of entries
				size_t nSize = pResourceIDMap->GetMapCount();
				logOutputResult.Format(_T("Resource ID map count=%lld"), nSize);
				OutputDebugLog(logOutputResult, DebugTestTool);
				// Print each resource ID map entry
				for (size_t nIndex = 0; nIndex < nSize; nIndex++) {
					const RESOURCE_ID_MAP_ENTRY& resourceIDMapEntry = pResourceIDMap->GetAt(nIndex);
					logOutputResult.Format(_T("Index=%lld: { ResourceID=%d, NameID=%s }"), nIndex, resourceIDMapEntry.dwResourceID, MAKEUNICODE(resourceIDMapEntry.strNameID));
					OutputDebugLog(logOutputResult, DebugTestTool);
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdmsgset"))) {
		if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("bkgclr")))) {
			if (tokenCount == 3) {
				// Set message background color by name
				String colorName = tokenList.at(2).c_str();
				DWORD dwRetColorID = GetStringID(StringTable::ColorName, colorName);
				if (dwRetColorID != INT_INVALID) {
					// Set background color
					SetReminderMsgBkgrdColor(dwRetColorID);
					pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
					OutputDebugLogFormat(_T("Message background color set: %s"), colorName.ToUpper().GetString());
					bNoReply = FALSE;	// Reset flag
				}
				else {
					// Invalid command
					bInvalidCmdFlag = TRUE;
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
					bNoReply = FALSE;	// Reset flag
				}
				else {
					// Set background color
					COLORREF clrRGB = RGB(nRValue, nGValue, nBValue);
					SetReminderMsgBkgrdColor(clrRGB);
					pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
					OutputDebugLogFormat(_T("Message background color set: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
					bNoReply = FALSE;	// Reset flag
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((tokenCount >= 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("txtclr")))) {
			if (tokenCount == 3) {
				// Set message text color by name
				String colorName = tokenList.at(2).c_str();
				DWORD dwRetColorID = GetStringID(StringTable::ColorName, colorName);
				if (dwRetColorID != INT_INVALID) {
					// Set text color
					SetReminderMsgTextColor(dwRetColorID);
					pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
					OutputDebugLogFormat(_T("Message text color set: %s"), colorName.ToUpper().GetString());
					bNoReply = FALSE;	// Reset flag
				}
				else {
					// Invalid command
					bInvalidCmdFlag = TRUE;
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
					bNoReply = FALSE;	// Reset flag
				}
				else {
					// Set text color
					COLORREF clrRGB = RGB(nRValue, nGValue, nBValue);
					SetReminderMsgTextColor(clrRGB);
					pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
					OutputDebugLogFormat(_T("Message text color set: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
					bNoReply = FALSE;	// Reset flag
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontname")))) {
			// Set reminder message font name
			String strFontName = tokenList.at(2).c_str();
			// Because the input debug command had been converted to lowercase,
			// it needs to be re-formatted by capitalizing first character of each word
			strFontName.Trim().UpperEachWord();
			// Validate font name
			BOOL bRet = ValidateFontName(strFontName);
			if (bRet != TRUE) {
				// Invalid font name
				OutputDebugLog(_T("Invalid font name"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Set message font
				SetReminderMsgFontName(strFontName);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLogFormat(_T("Message font name set: %s"), strFontName.GetString());
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontsize")))) {
			// Set reminder message font size
			int nFontSize = _tstoi(tokenList.at(2).c_str());
			if ((nFontSize < 10) || (nFontSize > 100)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 100)"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Set font size
				SetReminderMsgFontSize(nFontSize);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLogFormat(_T("Message font size set: %dpt"), nFontSize);
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("timeout")))) {
			// Set reminder message auto-close interval (timeout)
			int nTimeout = _tstoi(tokenList.at(2).c_str());
			if ((nTimeout < 10) || (nTimeout > 1800)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 1800)"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Set timeout
				SetReminderMsgTimeout(nTimeout);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLogFormat(_T("Message time-out set: %ds"), nTimeout);
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("notimeout")))) {
			// No reminder message timeout (default 0)
			SetReminderMsgTimeout(INT_NULL);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message time-out disabled"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconid")))) {
			// Set message icon ID by name
			String iconName = tokenList.at(2).c_str();
			DWORD dwRetIconID = GetStringID(StringTable::MsgIconName, iconName);
			if (dwRetIconID != INT_INVALID) {
				// Set icon ID
				SetReminderMsgIconID(dwRetIconID);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLogFormat(_T("Message icon ID set: %s (%d)"), iconName.ToUpper().GetString(), dwRetIconID);
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("noicon")))) {
			// No reminder message icon (default 0)
			SetReminderMsgIconID(0);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message icon disabled"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconsize")))) {
			// Set reminder message icon size
			int nIconSize = _tstoi(tokenList.at(2).c_str());
			if ((nIconSize < 30) || (nIconSize > 100)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 30 -> 100)"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Set icon size
				SetReminderMsgIconSize(nIconSize);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLogFormat(_T("Message icon size set: %dx%dpx"), nIconSize, nIconSize);
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconplacement")))) {
			// Set reminder message icon position
			String iconPosition = tokenList.at(2).c_str();
			if (!_tcscmp(iconPosition, _T("left"))) {
				// Set icon position: Icon on the Left
				SetReminderMsgIconPosition(RmdMsgStyleSet::IconOnTheLeft);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLog(_T("Message icon position set: Left"));
				bNoReply = FALSE;	// Reset flag
			}
			else if (!_tcscmp(iconPosition, _T("top"))) {
				// Set icon position: Icon on the Top
				SetReminderMsgIconPosition(RmdMsgStyleSet::IconOnTheTop);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLog(_T("Message icon position set: Top"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("hmargin")))) {
			// Set reminder message horizontal margin
			int nHMargin = _tstoi(tokenList.at(2).c_str());
			if ((nHMargin < 10) || (nHMargin > 120)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 120)"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Set margin
				SetReminderMsgHMargin(nHMargin);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLogFormat(_T("Message horizontal margin set: %dpx"), nHMargin);
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("vmargin")))) {
			// Set reminder message vertical margin
			int nVMargin = _tstoi(tokenList.at(2).c_str());
			if ((nVMargin < 10) || (nVMargin > 120)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 120)"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Set margin
				SetReminderMsgVMargin(nVMargin);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLogFormat(_T("Message vertical margin set: %dpx"), nVMargin);
				bNoReply = FALSE;	// Reset flag
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdmsgreset"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("bkgclr")))) {
			// Reset message background color
			SetReminderMsgBkgrdColor(RmdMsgStyleSet::defaultBkgrdColor);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message background color reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("txtclr")))) {
			// Set message text color by name
			SetReminderMsgTextColor(RmdMsgStyleSet::defaultTextColor);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message text color reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontname")))) {
			// Set reminder message font name
			SetReminderMsgFontName(RmdMsgStyleSet::defaultFontName);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message font name reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontsize")))) {
			// Set reminder message font size
			SetReminderMsgFontSize(RmdMsgStyleSet::defaultFontSize);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message font size reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("timeout")))) {
			// Reset reminder message auto-close interval (time-out)
			SetReminderMsgTimeout(RmdMsgStyleSet::defaultTimeout);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message time-out reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconid")))) {
			// Reset reminder message icon ID
			SetReminderMsgIconID(RmdMsgStyleSet::defaultIconID);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message icon ID reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconsize")))) {
			// Reset reminder message icon size
			SetReminderMsgIconSize(RmdMsgStyleSet::defaultIconSize);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message icon size reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconpos")))) {
			// Reset reminder message icon position
			SetReminderMsgIconPosition(RmdMsgStyleSet::defaultIconPosition);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message icon position reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("hmargin")))) {
			// Reset reminder message horizontal margin
			SetReminderMsgHMargin(RmdMsgStyleSet::defaultHorizontalMargin);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message horizontal margin reset)"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("vmargin")))) {
			// Reset reminder message vertical margin
			SetReminderMsgVMargin(RmdMsgStyleSet::defaultVerticalMargin);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message vertical margin reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdmsgget"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("bkgclr")))) {
			// Get reminder message background color
			DWORD dwBkgrdColor = GetReminderMsgBkgrdColor();
			int nBValue = (dwBkgrdColor & 0x00FF0000) >> 16;	// Blue
			int nGValue = (dwBkgrdColor & 0x0000FF00) >> 8;		// Green
			int nRValue = (dwBkgrdColor & 0x000000FF);			// Red
			OutputDebugLogFormat(_T("Message background color: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("txtclr")))) {
			// Get reminder message text color
			DWORD dwTextColor = GetReminderMsgTextColor();
			int nBValue = (dwTextColor & 0x00FF0000) >> 16;		// Blue
			int nGValue = (dwTextColor & 0x0000FF00) >> 8;		// Green
			int nRValue = (dwTextColor & 0x000000FF);			// Red
			OutputDebugLogFormat(_T("Message text color: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontname")))) {
			// Get reminder message font name
			String fontName;
			GetReminderMsgFontName(fontName);
			OutputDebugLogFormat(_T("Message font name: %s"), fontName.GetString());
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontsize")))) {
			// Get reminder message font size
			int nFontSize = GetReminderMsgFontSize();
			OutputDebugLogFormat(_T("Message font size: %dpt"), nFontSize);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("timeout")))) {
			// Get reminder message auto-close interval (time-out)
			int nTimeout = GetReminderMsgTimeout();
			OutputDebugLogFormat(_T("Message time-out: %ds"), nTimeout);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconid")))) {
			// Get reminder message icon ID
			UINT nIconID = GetReminderMsgIconID();
			OutputDebugLogFormat(_T("Message icon ID: %d"), nIconID);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconsize")))) {
			// Get reminder message icon size
			int nIconSize = GetReminderMsgIconSize();
			OutputDebugLogFormat(_T("Message icon size: %dx%dpx"), nIconSize, nIconSize);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("iconpos")))) {
			// Get reminder message icon position
			BYTE byIconPlacement = GetReminderMsgIconPosition();
			if (byIconPlacement == RmdMsgStyleSet::IconOnTheLeft) {
				OutputDebugLog(_T("Message icon position: Left"));
				bNoReply = FALSE;	// Reset flag
			}
			else if (byIconPlacement == RmdMsgStyleSet::IconOnTheTop) {
				OutputDebugLog(_T("Message icon position: Top"));
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("hmargin")))) {
			// Get reminder message horizontal margin
			int nHMargin = GetReminderMsgHMargin();
			OutputDebugLogFormat(_T("Message horizontal margin: %dpx"), nHMargin);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("vmargin")))) {
			// Get reminder message vertical margin
			int nVMargin = GetReminderMsgVMargin();
			OutputDebugLogFormat(_T("Message vertical margin: %dpx"), nVMargin);
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("runtimequeue"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("print")))) {
			// Print Power++ runtime queue list
			if (m_arrRuntimeQueue.empty()) {
				// Empty list
				OutputDebugLog(_T("Runtime queue empty!"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Print each item
				PwrRuntimeItem pwrRuntimeItem;
				for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {
					// Get runtime item from queue
					pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);
					// Print runtime item info
					OutputDebugLogFormat(_T("Item%03d: CategoryID=%d, ItemID=%d, Display=%d, Skip=%d, Snooze=%d"), nIndex, pwrRuntimeItem.GetCategory(),
						pwrRuntimeItem.GetItemID(), pwrRuntimeItem.GetDisplayFlag(), pwrRuntimeItem.GetSkipFlag(), pwrRuntimeItem.GetSnoozeFlag());
					bNoReply = FALSE;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdsnooze"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("prntqueue")))) {
			// Print Power Reminder snooze queue list
			if (m_arrRuntimeQueue.empty()) {
				// Empty list
				OutputDebugLog(_T("Reminder snooze queue empty!"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Print each item
				PwrRuntimeItem pwrRuntimeItem;
				for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {
					// Get runtime item from queue
					pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);
					// Skip if it's not Power Reminder item
					if (pwrRuntimeItem.GetCategory() != PwrFeatureID::pwrReminder) continue;
					// Print runtime item info
					ClockTime timeNextSnooze = pwrRuntimeItem.GetTime();
					OutputDebugLogFormat(_T("Item%03d: ID=%d, Snooze=%d, NextTrigger=%02d:%02d"), nIndex, pwrRuntimeItem.GetItemID(),
						pwrRuntimeItem.GetSnoozeFlag(), timeNextSnooze.Hour(), timeNextSnooze.Minute());
					bNoReply = FALSE;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("rmdruntime"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("prntdata")))) {
			// Print Power Reminder runtime data list
			if (m_arrRuntimeQueue.empty()) {
				// Empty list
				OutputDebugLog(_T("Reminder runtime data empty!"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Print each item
				PwrRuntimeItem pwrRuntimeItem;
				for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {
					// Get runtime item from queue
					pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);
					// Skip if it's not Power Reminder item
					if (pwrRuntimeItem.GetCategory() != PwrFeatureID::pwrReminder) continue;
					// Print runtime item info
					ClockTime timeNextSnooze = pwrRuntimeItem.GetTime();
					OutputDebugLogFormat(_T("Item%03d: ID=%d, Display=%d, Snooze=%d, NextTrigger=%02d:%02d"), nIndex, pwrRuntimeItem.GetItemID(),
						pwrRuntimeItem.GetDisplayFlag(), pwrRuntimeItem.GetSnoozeFlag(), timeNextSnooze.Hour(), timeNextSnooze.Minute());
					bNoReply = FALSE;	// Reset flag
				}
			}
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("upper"))) {
		if ((tokenCount > 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("string")))) {
			// Upper each word
			String strInput = tokenList.at(2).c_str();
			strInput.Trim().UpperEachWord();
			OutputDebugLogFormat(_T("Upper: %s"), strInput.GetString());
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("enum"))) {
		if ((tokenCount == 2) && (!_tcscmp(tokenList.at(1).c_str(), _T("fontnames")))) {
			// Enumerate all currently available fonts
			std::vector<std::wstring> fontNames;
			BOOL bRet = EnumFontNames(fontNames);
			if (bRet == FALSE) {
				// Enumerate fonts failed
				OutputDebugLog(_T("Enumerate fonts failed"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Print all font names
				BeginWaitCursor();
				{
					// Prepare for replying
					bNoReply = FALSE;	// Reset flag
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
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("getlastsysevttime"))) {
		// Get last system event time
		DateTime dateTimeTemp;
		String dateTimeFormat = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
		// Get last system suspend time
		if (pApp->GetLastSysEventTime(SystemEventID::SystemSuspend, dateTimeTemp)) {
			// Format date time
			const wchar_t* middayFlag = (dateTimeTemp.Hour() >= 12) ? _T("PM") : _T("AM");
			String logTemp = StringUtils::StringFormat(dateTimeFormat, dateTimeTemp.Year(), dateTimeTemp.Month(), dateTimeTemp.Day(),
				dateTimeTemp.Hour(), dateTimeTemp.Minute(), dateTimeTemp.Second(), dateTimeTemp.Millisecond(), middayFlag);
			logOutputResult.Format(_T("Last System Suspend: %s"), logTemp.GetString());
			OutputDebugLog(logOutputResult);
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Get last system suspend time failed
			OutputDebugLog(_T("Get last system suspend time failed"));
			bNoReply = FALSE;	// Reset flag
		}
		// Get last system wakeup time
		if (pApp->GetLastSysEventTime(SystemEventID::SystemWakeUp, dateTimeTemp)) {
			// Format date time
			const wchar_t* middayFlag = (dateTimeTemp.Hour() >= 12) ? _T("PM") : _T("AM");
			String logTemp = StringUtils::StringFormat(dateTimeFormat, dateTimeTemp.Year(), dateTimeTemp.Month(), dateTimeTemp.Day(),
				dateTimeTemp.Hour(), dateTimeTemp.Minute(), dateTimeTemp.Second(), dateTimeTemp.Millisecond(), middayFlag);
			logOutputResult.Format(_T("Last System Wakeup: %s"), logTemp.GetString());
			OutputDebugLog(logOutputResult);
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Get last system wakeup time failed
			OutputDebugLog(_T("Get last system wakeup time failed"));
			bNoReply = FALSE;	// Reset flag
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
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid param
				OutputDebugLog(_T("Invalid parameter"));
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(1).c_str(), _T("skip")))) {
			// Skip system power event (WM_POWERBROADCAST message)
			int nSkipCount = _tstoi(tokenList.at(2).c_str());
			if ((nSkipCount <= 0) || (nSkipCount > 3600)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 1 -> 3600)"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Set skip counter
				SetFlagValue(AppFlagID::pwrBroadcastSkipCount, nSkipCount);
				OutputDebugLogFormat(_T("Skip PowerBroadcast event: Counter=%d"), nSkipCount);
				bNoReply = FALSE;	// Reset flag
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(tokenList.at(0).c_str(), _T("flagset"))) {
		if (!_tcscmp(tokenList.at(1).c_str(), _T("pwraction"))) {
			// Turn ON/OFF power action flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				SetPwrActionFlag(FLAG_ON);
				OutputDebugLog(_T("Power action flag: ON"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				SetPwrActionFlag(FLAG_OFF);
				OutputDebugLog(_T("Power action flag: OFF"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("syssuspend"))) {
			// Turn ON/OFF system suspend flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				SetSystemSuspendFlag(FLAG_ON);
				OutputDebugLog(_T("System suspend flag: ON"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				SetSystemSuspendFlag(FLAG_OFF);
				OutputDebugLog(_T("System suspend flag: OFF"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("sessionend"))) {
			// Turn ON/OFF session end flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				SetSessionEndFlag(FLAG_ON);
				OutputDebugLog(_T("Session end flag: ON"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				SetSessionEndFlag(FLAG_OFF);
				OutputDebugLog(_T("Session end flag: OFF"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("sessionlock"))) {
			// Turn ON/OFF session lock flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				SetSessionLockFlag(FLAG_ON);
				OutputDebugLog(_T("Session lock flag: ON"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				SetSessionLockFlag(FLAG_OFF);
				OutputDebugLog(_T("Session lock flag: OFF"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if (!_tcscmp(tokenList.at(1).c_str(), _T("safetermination"))) {
			// Turn ON/OFF previously safe termination trace flag
			if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("on")))) {
				SetSafeTerminationFlag(FLAG_ON);
				OutputDebugLog(_T("Previously safe termination trace flag: ON"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((tokenCount == 3) && (!_tcscmp(tokenList.at(2).c_str(), _T("off")))) {
				SetSafeTerminationFlag(FLAG_OFF);
				OutputDebugLog(_T("Previously safe termination trace flag: OFF"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
	}
	else {
		// Invalid command
		bInvalidCmdFlag = TRUE;
	}

	/*********************************************************************/
	/*																	 */
	/*					Post-processing and clean-up				     */
	/*																	 */
	/*********************************************************************/

	// If command is invalid
	if (bInvalidCmdFlag == TRUE) {
		// Error: Invalid command
		errorCode = APP_ERROR_DBG_INVALID_COMMAND;
		bNoReply = FALSE;	// Reset flag
		bRet = FALSE;		// Return failed
	}

	// If command is executed but no reply
	if (bNoReply == TRUE) {
		// Notify to the DebugTest dialog
		SDialog* pDebugTestDlg = pApp->GetDebugTestDlg();
		if (pDebugTestDlg != NULL) {
			pDebugTestDlg->PostMessage(SM_APP_DEBUGCMD_NOREPLY);
		}
		if (bNoReplySilent != TRUE) {
			OutputDebugLog(_T("Command sent, no reply!!!"));
		}
		bNoReply = FALSE;			// Reset flag
		bNoReplySilent = FALSE;		// Reset flag
		bRet = TRUE;				// Return successful
	}

	return bRet;
}

