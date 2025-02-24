
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

#include "stdafx.h"

#include "PowerPlus.h"
#include "PowerPlusDlg.h"

#include "AboutDlg.h"
#include "MultiScheduleDlg.h"
#include "LogViewerDlg.h"
#include "HotkeySetDlg.h"
#include "PwrReminderDlg.h"
#include "ReminderMsgDlg.h"
#include "DebugTestDlg.h"

using namespace PairFuncs;
using namespace CoreFuncs;


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ProcessDebugCommand
//	Description:	Process app debug commands
//  Arguments:		lpszCommand - Debug command string
//  Return value:	BOOL
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ProcessDebugCommand(LPCTSTR lpszCommand, DWORD& dwErrorCode)
{
	// If debug command is empty, do nothing
	CString strDebugCommand(lpszCommand);
	if (strDebugCommand.IsEmpty()) {
		// Error: Debug command empty
		dwErrorCode = APP_ERROR_DBG_EMPTY;
		return FALSE;
	}

	// Get application class
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) {
		// Error: Get app failed
		dwErrorCode = APP_ERROR_DBG_GETAPP_FAILED;
		return FALSE;
	}

	// Format debug command
	strDebugCommand.MakeLower();

	// Debug log output result
	CString strOutputResult;

	// Return result
	BOOL bRet = TRUE;

	// No replying flag
	BOOL bNoReply = TRUE;

	// Invalid command flag
	BOOL bInvalidCmdFlag = FALSE;

	/*********************************************************************/
	/*																	 */
	/*					Process basic debug commands				     */
	/*																	 */
	/*********************************************************************/

	if (!_tcscmp(strDebugCommand, _T("exit"))) {
		// Exit application
		ExitApp(EXITCODE_DEBUGCOMMAND);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("reload"))) {
		// Reload settings
		ReloadSettings();
		strOutputResult.Format(_T("Settings reloaded!!!"));
		OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("setdefault"))) {
		// Set default settings
		SetDefaultConfig();
		strOutputResult.Format(_T("Reset default settings!!!"));
		OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("restartapp"))) {
		// Restart app (non-admin)
		RequestRestartApp(IDD_DEBUGTEST_DLG, FALSE);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("restartadmin"))) {
		// Restart app (as admin)
		RequestRestartApp(IDD_DEBUGTEST_DLG, TRUE);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("logviewer"))) {
		// Open Logviewer dialog
		OpenChildDialogEx(IDD_LOGVIEWER_DLG);
		strOutputResult.Format(_T("Logviewer opened!!!"));
		OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("hksupdate"))) {
		// Update HotkeySet settings
		PostMessage(SM_APP_UPDATE_HOTKEYSETDATA);
		strOutputResult.Format(_T("HotkeySet data updated!!!"));
		OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("rmdupdate"))) {
		// Update Power Reminder data
		PostMessage(SM_APP_UPDATE_PWRREMINDERDATA);
		strOutputResult.Format(_T("Power Reminder data updated!!!"));
		OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("bakconfig"))) {
		// Backup configuration
		SConfigBackup::AutoRegistryExport();
		strOutputResult.Format(_T("Config backed-up!!!"));
		OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("viewbakconfig"))) {
		// View backup configuration file
		OpenTextFileToView(FILENAME_BAKCONFIG, FILEEXT_REGFILE);
		strOutputResult.Format(_T("Opening backup config file..."));
		OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("viewhistory"))) {
		// View action history log file
		OpenTextFileToView(FILENAME_HISTORY_LOG, FILEEXT_LOGFILE, SUBFOLDER_LOG);
		strOutputResult.Format(_T("Opening action history log file..."));
		OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
		bNoReply = FALSE;	// Reset flag
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("clrscr"))) {
		// Clear debug view screen buffer
		HWND hDebugTestDlg = FindDebugTestDlg();
		if (hDebugTestDlg != NULL) {
			// Post clear screen message
			::PostMessage(hDebugTestDlg, SM_WND_DEBUGVIEWCLRSCR, NULL, NULL);
		}
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("close"))) {
		// Close DebugTest dialog if opening
		SDialog* pDebugTestDlg = pApp->GetDebugTestDlg();
		if (pDebugTestDlg != NULL) {
			pDebugTestDlg->ShowWindow(SW_HIDE);
		}
		return TRUE;
	}

	/*********************************************************************/
	/*																	 */
	/*			Process advanced/multi-token debug commands				 */
	/*																	 */
	/*********************************************************************/

	// Prepare copy-buffer data
	LPTSTR lpszCopyBuff = new TCHAR[MAX_BUFFER_LENGTH];
	if (lpszCopyBuff == NULL) {
		// Error: Copy-buffer allocation failed
		dwErrorCode = APP_ERROR_DBG_CPYBUFF_ALLOC_FAILED;
		return FALSE;
	}
	_tcscpy(lpszCopyBuff, strDebugCommand.operator LPCTSTR());

	// Prepare return-buffer data
	PBUFFER retBuff = new BUFFER[MAX_TOKEN_COUNT];
	if (retBuff == NULL) {
		// Error: Return-buffer allocation failed
		dwErrorCode = APP_ERROR_DBG_RETBUFF_ALLOC_FAILED;
		// Clean-up copy-buffer data
		delete[] lpszCopyBuff;
		return FALSE;
	}

	// Make token list (break string buffer)
	int nCount = GetTokenList(lpszCopyBuff, retBuff, _T(" =,;:-_"));

	// Validate number of tokens
	if (nCount <= 0) {
		// Error: Tokenization failed
		dwErrorCode = APP_ERROR_DBG_TOKENIZATION_FAILED;
		// Clean-up copy-buffer and return-buffer data
		delete[] lpszCopyBuff;
		delete[] retBuff;
		return FALSE;
	}

	int nTemp = INT_INVALID;
	CString strTemp = STRING_EMPTY;

	// Process debug commands by tokens
	if (!_tcscmp(retBuff[0].tcToken, _T("test"))) {
		// Debug command token test
		if (nCount > 1) {
			// Prepare for replying
			bNoReply = FALSE;	// Reset flag
			// Print token list
			int nTokenCount = (nCount - 1);
			OutputDebugLogFormat(_T("Token number: %d"), nTokenCount);
			for (int nTokenIndex = 0; nTokenIndex < nTokenCount; nTokenIndex++) {
				strTemp = retBuff[nTokenIndex + 1].tcToken;
				OutputDebugLogFormat(_T("Token[%d]: %s"), nTokenIndex, strTemp);
			}
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("dmytest"))) {
		// Set dummy test mode
		if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("on")))) {
			SetDummyTestMode(TRUE);
			OutputDebugLog(_T("DummyTest ON"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("off")))) {
			SetDummyTestMode(FALSE);
			OutputDebugLog(_T("DummyTest OFF"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("reset")))) {
			SetDummyTestMode(DEFAULT_DUMMYTEST);
			OutputDebugLog(_T("DummyTest reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("dmyteststs"))) {
		// Get dummy test mode
		int nRet = GetDummyTestMode();
		OutputDebugLogFormat(_T("DummyTest status = %d"), nRet);
		bNoReply = FALSE;	// Reset flag
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("debugmode"))) {
		// Set debug mode
		if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("on")))) {
			SetDebugMode(TRUE);
			OutputDebugLog(_T("DebugMode ON"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("off")))) {
			SetDebugMode(FALSE);
			OutputDebugLog(_T("DebugMode OFF"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("reset")))) {
			SetDebugMode(DEFAULT_DEBUGMODE);
			OutputDebugLog(_T("DebugMode reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("debugmodests"))) {
		// Get debug mode
		int nRet = GetDebugMode();
		OutputDebugLogFormat(_T("DebugMode status = %d"), nRet);
		bNoReply = FALSE;	// Reset flag
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("debuglog"))) {
		if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("output")))) {
			// Set debug log output target
			if ((nCount >= 3) && (!_tcscmp(retBuff[2].tcToken, _T("default")))) {
				SetDebugOutputTarget(DBOUT_DEFAULT);
				OutputDebugLog(_T("Debug log output target changed"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((nCount >= 3) && (!_tcscmp(retBuff[2].tcToken, _T("tofile")))) {
				SetDebugOutputTarget(DBOUT_DEBUGINFOFILE);
				OutputDebugLog(_T("Debug log output target changed"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((nCount >= 3) && (!_tcscmp(retBuff[2].tcToken, _T("todbtool")))) {
				SetDebugOutputTarget(DBOUT_DEBUGTESTTOOL);
				OutputDebugLog(_T("Debug log output target changed"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("reset")))) {
				SetDebugOutputTarget(DEFAULT_DEBUGOUTPUT);
				OutputDebugLog(_T("Debug log output target reset"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("getoutput")))) {
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
	else if (!_tcscmp(retBuff[0].tcToken, _T("testfeature"))) {
		// Set test feature enable
		if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("on")))) {
			SetTestFeatureEnable(TRUE);
			OutputDebugLog(_T("TestFeature ON"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("off")))) {
			SetTestFeatureEnable(FALSE);
			OutputDebugLog(_T("TestFeature OFF"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("reset")))) {
			SetTestFeatureEnable(DEFAULT_TESTFEATURE);
			OutputDebugLog(_T("TestFeature reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("testfeaturests"))) {
		// Get test feature enable
		int nRet = GetTestFeatureEnable();
		OutputDebugLogFormat(_T("TestFeature status = %d"), nRet);
		bNoReply = FALSE;	// Reset flag
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("logtest"))) {
		if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("trcerr")))) {
			// Trace error log test
			int nVal = _tstoi(retBuff[2].tcToken);
			if (nVal >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				bNoReply = FALSE;	// Reset flag
				if (nVal >= 50) {
					// Output waiting notification
					OutputDebugLog(_T("Wait for a while!!!"), DBOUT_DEBUGTESTTOOL);
					WaitMessage(SM_WND_DEBUGOUTPUTDISP); // wait for the notification displaying
				}
				for (int i = 0; i < nVal; i++) {
					// Write test trace error log
					TraceLogFormat("TraceErrorLog Test (%d)", i);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				OutputDebugLog(_T("TraceErrorLog test done!!!"), DBOUT_DEBUGTESTTOOL);
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("trcdebug")))) {
			// Trace debug log test
			int nVal = _tstoi(retBuff[2].tcToken);
			if (nVal >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				bNoReply = FALSE;	// Reset flag
				if (nVal >= 50) {
					// Output waiting notification
					OutputDebugLog(_T("Wait for a while!!!"), DBOUT_DEBUGTESTTOOL);
					WaitMessage(SM_WND_DEBUGOUTPUTDISP); // wait for the notification displaying
				}
				CString strFormat;
				for (int i = 0; i < nVal; i++) {
					// Write test trace debug log
					strFormat.Format(_T("TraceDebugLog Test (%d)"), i);
					WriteTraceDebugLogFile(strFormat);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				OutputDebugLog(_T("TraceDebugLog test done!!!"), DBOUT_DEBUGTESTTOOL);
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("debuginfo")))) {
			// Output debug info log test
			int nVal = _tstoi(retBuff[2].tcToken);
			if (nVal >= 1) {
				// Always reply,
				// but sometimes may be slowly responding
				bNoReply = FALSE;	// Reset flag
				if (nVal >= 50) {
					// Output waiting notification
					OutputDebugLog(_T("Wait for a while!!!"), DBOUT_DEBUGTESTTOOL);
					WaitMessage(SM_WND_DEBUGOUTPUTDISP); // wait for the notification displaying
				}
				CString strFormat;
				for (int i = 0; i < nVal; i++) {
					// Write test debug info log
					strFormat.Format(_T("DebugInfoLog Test (%d)"), i);
					OutputDebugLog(strFormat, DBOUT_DEBUGINFOFILE);
					Sleep(50); // wait for a blink
				}
				// Output notification when done
				OutputDebugLog(_T("DebugInfoLog test done!!!"), DBOUT_DEBUGTESTTOOL);
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
	else if (!_tcscmp(retBuff[0].tcToken, _T("errtest"))) {
		if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("code")))) {
			// Test show error message
			int nErrCode = _tstoi(retBuff[2].tcToken);
			if (nErrCode >= 0) {
				if (GetAppOption(OPTIONID_SHOWERRORMSG) == FALSE) {
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
	else if (!_tcscmp(retBuff[0].tcToken, _T("appeventlog"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("clear")))) {
			// Clear all app event log data in memory
			SLogging* pAppEventLog = ((CPowerPlusApp*)AfxGetApp())->GetAppEventLog();
			if (pAppEventLog != NULL) {
				pAppEventLog->Init();
				OutputDebugLog(_T("App event log data cleared"));
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("write")))) {
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
		else if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("del")))) {
			// Delete app event log files
			CString strKeyName = retBuff[2].tcToken;;
			if (!_tcscmp(retBuff[2].tcToken, _T("all"))) {
				strKeyName = _T("AppEventLog");
			}
			CFileFind Finder;
			CString strFileName = (CString)SUBFOLDER_LOG + SYMBOL_BACKSLASH + _T("*.log");
			BOOL bFindRet = Finder.FindFile(strFileName);
			if (!strKeyName.IsEmpty()) {
				int nDelFileCount = 0;
				while (bFindRet == TRUE) {
					bFindRet = Finder.FindNextFile();
					strFileName = Finder.GetFileName();
					if (strFileName.Find(strKeyName) != -1) {
						// Delete file
						CFile::Remove(Finder.GetFilePath());
						nDelFileCount++;	// Increase counter
					}
				}
				if (nDelFileCount > 0) {
					// Output number of deleted files
					strOutputResult.Format(_T("App event log file(s) deleted (Count=%d)"), nDelFileCount);
					OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
					bNoReply = FALSE;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("reminderdisp"))) {
		if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("id")))) {
			int nItemID = _tstoi(retBuff[2].tcToken);
			// Find and display reminder item by ID
			BOOL bFindRet = FALSE;
			PWRREMINDERITEM pwrTemp;
			int nItemNum = m_prdReminderData.GetItemNum();
			for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
				pwrTemp = m_prdReminderData.GetItemAt(nIndex);
				if (pwrTemp.nItemID == nItemID) {
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
	else if (!_tcscmp(retBuff[0].tcToken, _T("saveglobaldata"))) {
		if (nCount == 1) {
			// Save all global data variables
			pApp->SaveGlobalData(0xFF);
			strOutputResult.Format(_T("Global variables stored"));
			OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
			bNoReply = FALSE;	// Reset flag
		}
		else if (nCount == 2) {
			if (!_tcscmp(retBuff[1].tcToken, _T("all"))) {
				// Save all global variables
				pApp->SaveGlobalData(0xFF);
				strOutputResult.Format(_T("Global variables stored"));
				OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
				bNoReply = FALSE;	// Reset flag
			}
			else if (!_tcscmp(retBuff[1].tcToken, _T("dbtest"))) {
				// Save debugging/testing config (global variables)
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_DEBUGTEST);
				strOutputResult.Format(_T("Debug/test config stored"));
				OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
				bNoReply = FALSE;	// Reset flag
			}
			else if (!_tcscmp(retBuff[1].tcToken, _T("appflags"))) {
				// Save app flags (global variables)
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
				strOutputResult.Format(_T("Global app flags stored"));
				OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
				bNoReply = FALSE;	// Reset flag
			}
			else if (!_tcscmp(retBuff[1].tcToken, _T("features"))) {
				// Save special variables (global variables)
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				strOutputResult.Format(_T("Global special feature variables stored"));
				OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
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
	else if (!_tcscmp(retBuff[0].tcToken, _T("print"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("config")))) {
			// Print app config data
			PCONFIGDATA pcfgDataTemp = pApp->GetAppConfigData();
			if (pcfgDataTemp != NULL) {
				// Prepare for replying
				bNoReply = FALSE;	// Reset flag
				// Load app language package
				LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();
				// Format and print data
				CString strKeyName = STRING_EMPTY;
				CString strValue = STRING_EMPTY;
				// Left mouse button action
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_ACTIONLMB);
				int nActionStringID = GetPairedID(idplActionName, pcfgDataTemp->nLMBAction);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, GetLanguageString(ptrLanguage, nActionStringID));
				// Middle mouse button action
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_ACTIONMMB);
				nActionStringID = GetPairedID(idplActionName, pcfgDataTemp->nMMBAction);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, GetLanguageString(ptrLanguage, nActionStringID));
				// Right mouse button action
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_ACTIONRMB);
				nActionStringID = GetPairedID(idplActionName, pcfgDataTemp->nRMBAction);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, GetLanguageString(ptrLanguage, nActionStringID));
				// Right mouse button: Only show menu
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_RMBSHOWMENU);
				strValue = ((pcfgDataTemp->bRMBShowMenu) ? VALUE_TRUE : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Language setting
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_LANGUAGEID);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, MAKEUNICODE(GetLanguageName(pcfgDataTemp->nLanguageID)));
				// Show dialog at startup
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SHOWATSTARTUP);
				strValue = ((pcfgDataTemp->bShowDlgAtStartup) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Startup with Windows
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_STARTUPENABLE);
				strValue = ((pcfgDataTemp->bStartupEnabled) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Show confirm message before executing action
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_CONFIRMACTION);
				strValue = ((pcfgDataTemp->bConfirmAction) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Save action log
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SAVEACTIONLOG);
				strValue = ((pcfgDataTemp->bSaveHistoryLog) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Save app event log
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SAVEAPPEVENTLOG);
				strValue = ((pcfgDataTemp->bSaveAppEventLog) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Run with admin privileges
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_RUNASADMIN);
				strValue = ((pcfgDataTemp->bRunAsAdmin) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Show action error message
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SHOWERROR);
				strValue = ((pcfgDataTemp->bShowErrorMsg) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Show notify tip for schedule action
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SCHEDULENOTIFY);
				strValue = ((pcfgDataTemp->bNotifySchedule) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Allow canceling schedule when notify
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SCHEDALLOWCANCEL);
				strValue = ((pcfgDataTemp->bAllowCancelSchedule) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Enable background action hotkeys
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_ENBBKGRDHOTKEYS);
				strValue = ((pcfgDataTemp->bEnableBackgroundHotkey) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Allow background hotkeys on lockscreen
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_LOCKSTATEHOTKEY);
				strValue = ((pcfgDataTemp->bLockStateHotkey) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Enable Power Peminder feature
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_ENBPWRREMINDER);
				strValue = ((pcfgDataTemp->bEnablePowerReminder) ? VALUE_TRUE : VALUE_FALSE);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
			}
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("schedule")))) {
			// Print schedule data
			PSCHEDULEDATA pSchedDataTemp = pApp->GetAppScheduleData();
			if (pSchedDataTemp != NULL) {
				// Prepare for replying
				bNoReply = FALSE;	// Reset flag
				// Print default schedule
				CString strDefItemPrint;
				pSchedDataTemp->GetDefaultItem().Print(strDefItemPrint);
				strOutputResult.Format(_T("DefaultSchedule: %s"), strDefItemPrint);
				OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
				// Print extra item number
				int nExtraItemNum = pSchedDataTemp->GetExtraItemNum();
				strOutputResult.Format(_T("ScheduleExtraData: ItemNum = %d"), nExtraItemNum);
				OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
				// Print each item data
				for (int nExtraIndex = 0; nExtraIndex < nExtraItemNum; nExtraIndex++) {
					SCHEDULEITEM schExtraItem = pSchedDataTemp->GetItemAt(nExtraIndex);

					// Print item
					CString strItemPrint;
					schExtraItem.Print(strItemPrint);
					strOutputResult.Format(_T("Index=%d, %s"), nExtraIndex, strItemPrint);
					OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
				}
			}
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("hksetdata")))) {
			// Print HotkeySet data
			PHOTKEYSETDATA pHksDataTemp = pApp->GetAppHotkeySetData();
			if (pHksDataTemp != NULL) {
				// Prepare for replying
				bNoReply = FALSE;	// Reset flag
				// Print item number
				int nItemNum = pHksDataTemp->GetItemNum();
				strOutputResult.Format(_T("HotkeySetData: ItemNum = %d"), nItemNum);
				OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
				// Load app language package
				LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();
				// Print each item data
				for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
					HOTKEYSETITEM hksItem = pHksDataTemp->GetItemAt(nIndex);

					// Print item
					CString strItemPrint;
					hksItem.Print(strItemPrint);
					strOutputResult.Format(_T("Index=%d, %s"), nIndex, strItemPrint);
					OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
				}
			}
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("rmddata")))) {
			// Print Power Reminder data
			PPWRREMINDERDATA pRmdDataTemp = pApp->GetAppPwrReminderData();
			if (pRmdDataTemp != NULL) {
				// Prepare for replying
				bNoReply = FALSE;	// Reset flag
				// Print item number
				int nItemNum = pRmdDataTemp->GetItemNum();
				strOutputResult.Format(_T("PwrReminderData: ItemNum = %d"), nItemNum);
				OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
				// Load app language package
				LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();
				// Print each item data
				for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
					PWRREMINDERITEM pwrItem = pRmdDataTemp->GetItemAt(nIndex);

					// Print item
					CString strItemPrint;
					pwrItem.Print(strItemPrint);
					strOutputResult.Format(_T("Index=%d, %s"), nIndex, strItemPrint);
					OutputDebugLog(strOutputResult, DBOUT_DEBUGTESTTOOL);
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("rmdmsgset"))) {
		if ((nCount >= 3) && (!_tcscmp(retBuff[1].tcToken, _T("bkgclr")))) {
			if (nCount == 3) {
				// Set message background color by name
				CString strColorName = retBuff[2].tcToken;
				DWORD dwRetColorID = GetStringID(strplColorName, strColorName);
				if (dwRetColorID != INT_INVALID) {
					// Set background color
					SetReminderMsgBkgrdColor(dwRetColorID);
					pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
					OutputDebugLogFormat(_T("Message background color set: %s"), strColorName.MakeUpper());
					bNoReply = FALSE;	// Reset flag
				}
				else {
					// Invalid command
					bInvalidCmdFlag = TRUE;
				}
			}
			else if (((nCount > 3) && (nCount <= 6)) && (!_tcscmp(retBuff[2].tcToken, _T("rgb")))) {
				// Set message background color by RGB value
				int nRValue = (nCount >= 4) ? _tstoi(retBuff[3].tcToken) : 0;	// Red
				int nGValue = (nCount >= 5) ? _tstoi(retBuff[4].tcToken) : 0;	// Green
				int nBValue = (nCount >= 6) ? _tstoi(retBuff[5].tcToken) : 0;	// Blue
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
		else if ((nCount >= 3) && (!_tcscmp(retBuff[1].tcToken, _T("txtclr")))) {
			if (nCount == 3) {
				// Set message text color by name
				CString strColorName = retBuff[2].tcToken;
				DWORD dwRetColorID = GetStringID(strplColorName, strColorName);
				if (dwRetColorID != INT_INVALID) {
					// Set text color
					SetReminderMsgTextColor(dwRetColorID);
					pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
					OutputDebugLogFormat(_T("Message text color set: %s"), strColorName.MakeUpper());
					bNoReply = FALSE;	// Reset flag
				}
				else {
					// Invalid command
					bInvalidCmdFlag = TRUE;
				}
			}
			else if (((nCount > 3) && (nCount <= 6)) && (!_tcscmp(retBuff[2].tcToken, _T("rgb")))) {
				// Set message background color by RGB value
				int nRValue = (nCount >= 4) ? _tstoi(retBuff[3].tcToken) : 0;	// Red
				int nGValue = (nCount >= 5) ? _tstoi(retBuff[4].tcToken) : 0;	// Green
				int nBValue = (nCount >= 6) ? _tstoi(retBuff[5].tcToken) : 0;	// Blue
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
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("fontname")))) {
			// Set reminder message font name
			CString strFontName = retBuff[2].tcToken;
			// Because the input debug command had been converted to lowercase,
			// it needs to be re-formatted by capitalizing first character of each word
			UpperEachWord(strFontName, TRUE);
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
				OutputDebugLogFormat(_T("Message font name set: %s"), strFontName);
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("fontsize")))) {
			// Set reminder message font size
			int nFontSize = _tstoi(retBuff[2].tcToken);
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
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("timeout")))) {
			// Set reminder message auto-close interval (timeout)
			int nTimeout = _tstoi(retBuff[2].tcToken);
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
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("notimeout")))) {
			// No reminder message timeout (default 0)
			SetReminderMsgTimeout(INT_NULL);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message time-out disabled"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("iconid")))) {
			// Set message icon ID by name
			CString strIconName = retBuff[2].tcToken;
			DWORD dwRetIconID = GetStringID(strplMsgIconName, strIconName);
			if (dwRetIconID != INT_INVALID) {
				// Set icon ID
				SetReminderMsgIconID(dwRetIconID);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLogFormat(_T("Message icon ID set: %s (%d)"), strIconName.MakeUpper(), dwRetIconID);
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("noicon")))) {
			// No reminder message icon (default 0)
			SetReminderMsgIconID(IDI_MSGICON_NOICON);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message icon disabled"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("iconsize")))) {
			// Set reminder message icon size
			int nIconSize = _tstoi(retBuff[2].tcToken);
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
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("iconpos")))) {
			// Set reminder message icon position
			CString strIconPos = retBuff[2].tcToken;
			if (!_tcscmp(strIconPos, _T("left"))) {
				// Set icon position: Left
				SetReminderMsgIconPosition(MSGICONPOS_ONLEFT);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLog(_T("Message icon position set: Left"));
				bNoReply = FALSE;	// Reset flag
			}
			else if (!_tcscmp(strIconPos, _T("top"))) {
				// Set icon position: Top
				SetReminderMsgIconPosition(MSGICONPOS_ONTOP);
				pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
				OutputDebugLog(_T("Message icon position set: Top"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("hmargin")))) {
			// Set reminder message horizontal margin
			int nHMargin = _tstoi(retBuff[2].tcToken);
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
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("vmargin")))) {
			// Set reminder message vertical margin
			int nVMargin = _tstoi(retBuff[2].tcToken);
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
	else if (!_tcscmp(retBuff[0].tcToken, _T("rmdmsgreset"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("bkgclr")))) {
			// Reset message background color
			SetReminderMsgBkgrdColor(DEFAULT_MSGBKGRDCLR);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message background color reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("txtclr")))) {
			// Set message text color by name
			SetReminderMsgTextColor(DEFAULT_MSGTEXTCLR);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message text color reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("fontname")))) {
			// Set reminder message font name
			SetReminderMsgFontName(DEFAULT_MSGFONTNAME);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message font name reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("fontsize")))) {
			// Set reminder message font size
			SetReminderMsgFontSize(DEFAULT_MSGFONTSIZE);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message font size reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("timeout")))) {
			// Reset reminder message auto-close interval (time-out)
			SetReminderMsgTimeout(DEFAULT_MSGTIMEOUT);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message time-out reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconid")))) {
			// Reset reminder message icon ID
			SetReminderMsgIconID(DEFAULT_MSGICONID);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message icon ID reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconsize")))) {
			// Reset reminder message icon size
			SetReminderMsgIconSize(DEFAULT_MSGICONSIZE);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message icon size reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconpos")))) {
			// Reset reminder message icon position
			SetReminderMsgIconPosition(DEFAULT_MSGICONPOS);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLog(_T("Message icon position reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("hmargin")))) {
			// Reset reminder message horizontal margin
			SetReminderMsgHMargin(DEFAULT_MSGHMARGIN);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message horizontal margin reset)"));
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("vmargin")))) {
			// Reset reminder message vertical margin
			SetReminderMsgVMargin(DEFAULT_MSGVMARGIN);
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
			OutputDebugLogFormat(_T("Message vertical margin reset"));
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("rmdmsgget"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("bkgclr")))) {
			// Get reminder message background color
			DWORD dwBkgrdColor = GetReminderMsgBkgrdColor();
			int nBValue = (dwBkgrdColor & 0x00FF0000) >> 16;	// Blue
			int nGValue = (dwBkgrdColor & 0x0000FF00) >> 8;		// Green
			int nRValue = (dwBkgrdColor & 0x000000FF);			// Red
			OutputDebugLogFormat(_T("Message background color: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("txtclr")))) {
			// Get reminder message text color
			DWORD dwTextColor = GetReminderMsgTextColor();
			int nBValue = (dwTextColor & 0x00FF0000) >> 16;		// Blue
			int nGValue = (dwTextColor & 0x0000FF00) >> 8;		// Green
			int nRValue = (dwTextColor & 0x000000FF);			// Red
			OutputDebugLogFormat(_T("Message text color: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("fontname")))) {
			// Get reminder message font name
			CString strFontName;
			BOOL bRet = GetReminderMsgFontName(strFontName);
			OutputDebugLogFormat(_T("Message font name: %s"), strFontName);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("fontsize")))) {
			// Get reminder message font size
			int nFontSize = GetReminderMsgFontSize();
			OutputDebugLogFormat(_T("Message font size: %dpt"), nFontSize);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("timeout")))) {
			// Get reminder message auto-close interval (time-out)
			int nTimeout = GetReminderMsgTimeout();
			OutputDebugLogFormat(_T("Message time-out: %ds"), nTimeout);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconid")))) {
			// Get reminder message icon ID
			UINT nIconID = GetReminderMsgIconID();
			OutputDebugLogFormat(_T("Message icon ID: %d"), nIconID);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconsize")))) {
			// Get reminder message icon size
			int nIconSize = GetReminderMsgIconSize();
			OutputDebugLogFormat(_T("Message icon size: %dx%dpx"), nIconSize, nIconSize);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconpos")))) {
			// Get reminder message icon position
			BYTE byIconPosition = GetReminderMsgIconPosition();
			if (byIconPosition == MSGICONPOS_ONLEFT) {
				OutputDebugLog(_T("Message icon position: Left"));
				bNoReply = FALSE;	// Reset flag
			}
			else if (byIconPosition == MSGICONPOS_ONTOP) {
				OutputDebugLog(_T("Message icon position: Top"));
				bNoReply = FALSE;	// Reset flag
			}
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("hmargin")))) {
			// Get reminder message horizontal margin
			int nHMargin = GetReminderMsgHMargin();
			OutputDebugLogFormat(_T("Message horizontal margin: %dpx"), nHMargin);
			bNoReply = FALSE;	// Reset flag
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("vmargin")))) {
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
	else if (!_tcscmp(retBuff[0].tcToken, _T("runtimequeue"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("print")))) {
			// Print Power++ runtime queue list
			if (m_arrRuntimeQueue.IsEmpty()) {
				// Empty list
				OutputDebugLog(_T("Runtime queue empty!"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Print each item
				PWRRUNTIMEITEM pwrRuntimeItem;
				for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {
					// Get runtime item from queue
					pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);
					// Print runtime item info
					OutputDebugLogFormat(_T("Item%03d: CategoryID=%d, ItemID=%d, Display=%d, Skip=%d, Snooze=%d"), nIndex, pwrRuntimeItem.nCategory, 
						pwrRuntimeItem.nItemID, pwrRuntimeItem.nDisplayFlag, pwrRuntimeItem.nSkipFlag, pwrRuntimeItem.nSnoozeFlag);
					bNoReply = FALSE;	// Reset flag
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("rmdsnooze"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("prntqueue")))) {
			// Print Power Reminder snooze queue list
			if (m_arrRuntimeQueue.IsEmpty()) {
				// Empty list
				OutputDebugLog(_T("Reminder snooze queue empty!"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Print each item
				PWRRUNTIMEITEM pwrRuntimeItem;
				for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {
					// Get runtime item from queue
					pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);
					// Skip if it's not Power Reminder item
					if (pwrRuntimeItem.nCategory != FID_PWRREMINDERITEM) continue;
					// Print runtime item info
					OutputDebugLogFormat(_T("Item%03d: ID=%d, Snooze=%d, NextTrigger=%02d:%02d"), nIndex, pwrRuntimeItem.nItemID,
						pwrRuntimeItem.nSnoozeFlag, pwrRuntimeItem.stNextSnoozeTime.wHour, pwrRuntimeItem.stNextSnoozeTime.wMinute);
					bNoReply = FALSE;	// Reset flag
				}
			}
		}
		else if ((nCount >= 3) && (!_tcscmp(retBuff[1].tcToken, _T("interval")))) {
			if ((nCount == 4) && (!_tcscmp(retBuff[2].tcToken, _T("set")))) {
				// Set reminder message snooze interval
				int nInputInterval = _tstoi(retBuff[3].tcToken);
				if ((nInputInterval < 1) || (nInputInterval > 60)) {
					// Invalid argument
					OutputDebugLog(_T("Invalid value (Value range: 1 -> 60)"));
					bNoReply = FALSE;	// Reset flag
				}
				else {
					// Set snooze interval
					int nSnoozeInterval = nInputInterval * 60;
					SetReminderMsgSnoozeInterval(nSnoozeInterval);
					pApp->SaveGlobalData(DEF_GLBDATA_CATE_FEATURES);
					OutputDebugLogFormat(_T("Message snooze interval set: %ds"), nSnoozeInterval);
					bNoReply = FALSE;	// Reset flag
				}
			}
			else if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("get")))) {
				// Get reminder message snooze interval
				int nSnoozeInterval = GetReminderMsgSnoozeInterval();
				OutputDebugLogFormat(_T("Message snooze interval: %ds"), nSnoozeInterval);
				bNoReply = FALSE;	// Reset flag
			}
			else if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("reset")))) {
				// Reset reminder message snooze interval
				SetReminderMsgSnoozeInterval(DEFAULT_SNOOZETIME);
				OutputDebugLog(_T("Message snooze interval reset"));
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
	else if (!_tcscmp(retBuff[0].tcToken, _T("rmdruntime"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("prntdata")))) {
			// Print Power Reminder runtime data list
			if (m_arrRuntimeQueue.IsEmpty()) {
				// Empty list
				OutputDebugLog(_T("Reminder runtime data empty!"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Print each item
				PWRRUNTIMEITEM pwrRuntimeItem;
				for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {
					// Get runtime item from queue
					pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);
					// Skip if it's not Power Reminder item
					if (pwrRuntimeItem.nCategory != FID_PWRREMINDERITEM) continue;
					// Print runtime item info
					OutputDebugLogFormat(_T("Item%03d: ID=%d, Display=%d, Snooze=%d, NextTrigger=%02d:%02d"), nIndex, pwrRuntimeItem.nItemID,
						pwrRuntimeItem.nDisplayFlag, pwrRuntimeItem.nSnoozeFlag, pwrRuntimeItem.stNextSnoozeTime.wHour, pwrRuntimeItem.stNextSnoozeTime.wMinute);
					bNoReply = FALSE;	// Reset flag
				}
			}
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("upper"))) {
		if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("string")))) {
			// Upper each word
			CString strInput = retBuff[2].tcToken;
			UpperEachWord(strInput, TRUE);
			OutputDebugLogFormat(_T("Upper: %s"), strInput);
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("enum"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("fontnames")))) {
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
	else if (!_tcscmp(retBuff[0].tcToken, _T("getlastsysevttime"))) {
		// Get last system event time
		SYSTEMTIME stTimeTemp;
		CString strDateTimeFormat;
		strDateTimeFormat.LoadString(IDS_FORMAT_FULLDATETIME);
		// Get last system suspend time
		if (pApp->GetLastSysEventTime(SYSEVT_SUSPEND, stTimeTemp)) {
			// Format date time
			CString strLogTemp;
			CString strMiddayFlag = (stTimeTemp.wHour >= 12) ? _T("PM") : _T("AM");
			strLogTemp.Format(strDateTimeFormat, stTimeTemp.wYear, stTimeTemp.wMonth, stTimeTemp.wDay,
				stTimeTemp.wHour, stTimeTemp.wMinute, stTimeTemp.wSecond, stTimeTemp.wMilliseconds, strMiddayFlag);
			strOutputResult.Format(_T("Last System Suspend: %s"), strLogTemp);
			OutputDebugLog(strOutputResult);
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Get last system suspend time failed
			OutputDebugLog(_T("Get last system suspend time failed"));
			bNoReply = FALSE;	// Reset flag
		}
		// Get last system wakeup time
		if (pApp->GetLastSysEventTime(SYSEVT_WAKEUP, stTimeTemp)) {
			// Format date time
			CString strLogTemp;
			CString strMiddayFlag = (stTimeTemp.wHour >= 12) ? _T("PM") : _T("AM");
			strLogTemp.Format(strDateTimeFormat, stTimeTemp.wYear, stTimeTemp.wMonth, stTimeTemp.wDay,
				stTimeTemp.wHour, stTimeTemp.wMinute, stTimeTemp.wSecond, stTimeTemp.wMilliseconds, strMiddayFlag);
			strOutputResult.Format(_T("Last System Wakeup: %s"), strLogTemp);
			OutputDebugLog(strOutputResult);
			bNoReply = FALSE;	// Reset flag
		}
		else {
			// Get last system wakeup time failed
			OutputDebugLog(_T("Get last system wakeup time failed"));
			bNoReply = FALSE;	// Reset flag
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("powerbroadcast"))) {
		if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("param")))) {
			// Simulate system power event (WM_POWERBROADCAST message)
			int nParam = _tstoi(retBuff[2].tcToken);
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
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("skip")))) {
			// Skip system power event (WM_POWERBROADCAST message)
			int nSkipCount = _tstoi(retBuff[2].tcToken);
			if ((nSkipCount <= 0) || (nSkipCount > 3600)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 1 -> 3600)"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Set skip counter
				SetFlagValue(FLAGID_PWRBROADCASTSKIPCOUNT, nSkipCount);
				OutputDebugLogFormat(_T("Skip PowerBroadcast event: Counter=%d"), nSkipCount);
				bNoReply = FALSE;	// Reset flag
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("flagset"))) {
		if (!_tcscmp(retBuff[1].tcToken, _T("pwraction"))) {
			// Turn ON/OFF power action flag
			if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("on")))) {
				SetPwrActionFlag(FLAG_ON);
				OutputDebugLog(_T("Power action flag: ON"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("off")))) {
				SetPwrActionFlag(FLAG_OFF);
				OutputDebugLog(_T("Power action flag: OFF"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if (!_tcscmp(retBuff[1].tcToken, _T("syssuspend"))) {
			// Turn ON/OFF system suspend flag
			if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("on")))) {
				SetSystemSuspendFlag(FLAG_ON);
				OutputDebugLog(_T("System suspend flag: ON"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("off")))) {
				SetSystemSuspendFlag(FLAG_OFF);
				OutputDebugLog(_T("System suspend flag: OFF"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if (!_tcscmp(retBuff[1].tcToken, _T("sessionend"))) {
			// Turn ON/OFF session end flag
			if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("on")))) {
				SetSessionEndFlag(FLAG_ON);
				OutputDebugLog(_T("Session end flag: ON"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("off")))) {
				SetSessionEndFlag(FLAG_OFF);
				OutputDebugLog(_T("Session end flag: OFF"));
				bNoReply = FALSE;	// Reset flag
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if (!_tcscmp(retBuff[1].tcToken, _T("sessionlock"))) {
			// Turn ON/OFF session lock flag
			if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("on")))) {
				SetSessionLockFlag(FLAG_ON);
				OutputDebugLog(_T("Session lock flag: ON"));
				bNoReply = FALSE;	// Reset flag
			}
			else if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("off")))) {
				SetSessionLockFlag(FLAG_OFF);
				OutputDebugLog(_T("Session lock flag: OFF"));
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
		dwErrorCode = APP_ERROR_DBG_INVALID_COMMAND;
		bNoReply = FALSE;	// Reset flag
		bRet = FALSE;		// Return failed
	}

	// If command is executed but no reply
	if (bNoReply == TRUE) {
		OutputDebugLog(_T("Command sent, no reply!!!"));
		bNoReply = FALSE;	// Reset flag
		bRet = TRUE;		// Return successful
	}

	// Clean-up copy-buffer and return-buffer data
	delete[] lpszCopyBuff;
	delete[] retBuff;

	return bRet;
}

