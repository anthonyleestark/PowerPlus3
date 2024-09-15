
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Logging.cpp
//		Description:	Implement necessary methods to read/write app logs
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.22:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Logging.h"

///////////////////////////////////////////////////////

//// Implementations


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagLOGITEM
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagLOGITEM::tagLOGITEM()
{
	this->stTime = {};							// Log time
	this->byType = 0;							// Log type
	this->byCategory = 0;						// Log category
	this->strLogString = DEF_STRING_EMPTY;		// Log string
	this->strDetails = DEF_STRING_EMPTY;		// Log detail string
}

tagLOGITEM::tagLOGITEM(const tagLOGITEM& pItem)
{
	this->stTime = pItem.stTime;				// Log time
	this->byType = pItem.byType;				// Log type
	this->byCategory = pItem.byCategory;		// Log category
	this->strLogString = pItem.strLogString;	// Log string
	this->strDetails = pItem.strDetails;		// Log detail string
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatDateTime
//	Description:	Return a formated logitem date/time string
//  Arguments:		None
//  Return value:	CString - Formatted result
//
//////////////////////////////////////////////////////////////////////////

CString tagLOGITEM::FormatDateTime()
{
	CString strTimeFormat;
	CString strMiddayFlag = (stTime.wHour >= 12) ? _T("PM") : _T("AM");
	strTimeFormat.Format(IDS_STRFORMAT_FULLDATETIME, stTime.wYear, stTime.wMonth, stTime.wDay,
						stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, strMiddayFlag);

	return strTimeFormat;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatLogString
//	Description:	Return a formated log string
//  Arguments:		None
//  Return value:	CString - Formatted result
//
//////////////////////////////////////////////////////////////////////////

CString tagLOGITEM::FormatLogString()
{
	CString strLogFormat;
	strLogFormat.Format(IDS_STRFORMAT_LOGSTRING, FormatDateTime(), strLogString, strDetails);
	return strLogFormat;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SLogging/~SLogging
//	Description:	Constructor and destructor
//
//////////////////////////////////////////////////////////////////////////

SLogging::SLogging()
{
	m_nLogWriteMode = 0;
	m_nLogDataType = 0;
}

SLogging::~SLogging()
{
	// Clean up loglist
	m_arrLogData.RemoveAll();
	m_arrLogData.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Initialize log data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::Init()
{
	m_arrLogData.RemoveAll();
	m_arrLogData.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLogCount
//	Description:	Return the number of items of log list
//  Arguments:		None
//  Return value:	int - Number of log items
//
//////////////////////////////////////////////////////////////////////////

int SLogging::GetLogCount()
{
	return int(m_arrLogData.GetSize());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLogItem
//	Description:	Return a specific log item of log list
//  Arguments:		nIndex - Item index
//  Return value:	LOGITEM - Return log item
//
//////////////////////////////////////////////////////////////////////////

LOGITEM SLogging::GetLogItem(int nIndex)
{
	// Invalid index
	if (nIndex < 0)
		nIndex = 0;
	else if (nIndex >= GetLogCount())
		nIndex = GetLogCount() - 1;

	return m_arrLogData.GetAt(nIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetLogDataType
//	Description:	Get/set log data type
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

UINT SLogging::GetLogDataType()
{
	return m_nLogDataType;
}

void SLogging::SetLogDataType(UINT logDataType)
{
	m_nLogDataType = logDataType;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetLogWriteMode
//	Description:	Get/set log write mode
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

UINT SLogging::GetLogWriteMode()
{
	return m_nLogWriteMode;
}

void SLogging::SetLogWriteMode(UINT logWriteMode)
{
	m_nLogWriteMode = logWriteMode;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputLog
//	Description:	Add a log item into log list data
//  Arguments:		logItem			- Log item to write
//					nForceWriteMode - Force specific write mode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::OutputLog(LOGITEM& logItem, UINT nForceWriteMode /* = 0 */)
{
	UINT nLogWriteMode = this->GetLogWriteMode();
	if ((nForceWriteMode != 0) && (nForceWriteMode != nLogWriteMode)) {
		// Force log write mode
		nLogWriteMode = nForceWriteMode;
	}

	if (nLogWriteMode == LOG_WRITE_MODE_INSTANT) {
		// Write instantly mode
		WriteInstantLog(logItem);
	}
	else {
		// Write on-call mode -> Store log
		m_arrLogData.Add(logItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputLogString
//	Description:	Add a log string into log list data
//  Arguments:		lpszLogString - Log string
//					lpszLogDetail - Log detail string
//					byType		  - Log type
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::OutputLogString(LPCSTR lpszLogStringA, LPCSTR lpszLogDetailA /* = NULL */, BYTE byType /* = 0 */)
{
	// Convert to Unicode string
	LPCTSTR lpszLogStringW = CA2W(lpszLogStringA).m_psz;
	LPCTSTR lpszLogDetailW = CA2W(lpszLogDetailA).m_psz;
	OutputLogString(lpszLogStringW, lpszLogDetailW, byType);
}

void SLogging::OutputLogString(LPCTSTR lpszLogStringW, LPCTSTR lpszLogDetailW /* = NULL */, BYTE byType /* = 0 */)
{
	// Write instantly mode
	if (this->GetLogWriteMode() == LOG_WRITE_MODE_INSTANT) {
		WriteInstantLog(lpszLogStringW, lpszLogDetailW, byType);
	}
	else {
		// Get log time
		SYSTEMTIME stLogTime;
		stLogTime = GetCurSysTime();

		// Output log string
		LOGITEM logItem;
		logItem.byType = byType;
		logItem.stTime = stLogTime;
		logItem.strLogString = lpszLogStringW;
		logItem.strDetails = lpszLogDetailW;
		OutputLog(logItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteLog
//	Description:	Write log data list into the corresponding logfile
//  Arguments:		None
//  Return value:	BOOL - Result of log writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL SLogging::WriteLog()
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();

	// Quit if current log mode is write instantly mode
	if (this->GetLogWriteMode() == LOG_WRITE_MODE_INSTANT) {
		bResult = FALSE;
		return bResult;
	}

	CString strFileName;
	CString strFilePath;
	CString strCurFileName;
	CFile fLogFile;

	LOGITEM logItem;
	SYSTEMTIME stTemp;
	CString strLogFormat;

	for (int nIndex = 0; nIndex < GetLogCount(); nIndex++)
	{
		// Get log item
		logItem = GetLogItem(nIndex);
		stTemp = logItem.stTime;

		// Get filename according to type of logs
		switch (m_nLogDataType)
		{
		case LOGTYPE_APPEVENT_LOG:
			// Format app event log filename
			strFileName.Format(FILE_APPEVENT_LOG, stTemp.wYear, stTemp.wMonth, stTemp.wDay);
			strFilePath.Format(_T("%s\\%s"), DIR_SUBDIR_LOG, strFileName);
			if (strCurFileName.IsEmpty())
				strCurFileName = strFileName;

			// If a file with another name (previous day's log file) is opening,
			// write down all current log strings and close the file
			if ((fLogFile.m_hFile != CFile::hFileNull) && (strFileName != strCurFileName))
			{
				if (!strLogFormat.IsEmpty()) {
					// Write log strings to file
					fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
					fLogFile.Flush();

					strLogFormat.Empty();
				}

				fLogFile.Close();
				strCurFileName = strFileName;
			}
			break;
		case LOGTYPE_ACTION_LOG:
			// Action log
			strFileName = FILE_ACTION_LOG;
			strFilePath.Format(_T("%s\\%s"), DIR_SUBDIR_LOG, strFileName);
			break;
		default:
			// Wrong argument
			dwErrCode = DEF_APP_ERROR_WRONG_ARGUMENT;
			PostMessage(hWnd, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrCode, NULL);
			TRCFFMT(__FUNCTION__, "Invalid log type");
			break;
		}

		// Check if file is opening, if not, open it
		if (fLogFile.m_hFile == CFile::hFileNull)
		{
			bResult = fLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
			if (bResult == FALSE) {
				// Open file failed
				dwErrCode = GetLastError();
				PostMessage(hWnd, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrCode, NULL);
				TRCFFMT(__FUNCTION__, "Can not open/create log file");
				return bResult;
			}

			// Go to end of file
			fLogFile.SeekToEnd();
		}

		// Format output log strings
		strLogFormat += logItem.FormatLogString();
	}

	if (!strLogFormat.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteInstantLog
//	Description:	Write logitem instantly into logfile
//  Arguments:		logItem - Log item to write
//  Return value:	BOOL - Result of log writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL SLogging::WriteInstantLog(LOGITEM& logItem)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();

	// Quit if current log mode is not write instantly mode
	if (this->GetLogWriteMode() != LOG_WRITE_MODE_INSTANT) {
		bResult = FALSE;
		return bResult;
	}

	CString strFileName;
	CString strFilePath;
	CFile fLogFile;

	CString strLogFormat;

	// Get log time
	SYSTEMTIME stTimeTemp;
	stTimeTemp = logItem.stTime;

	// Get filename according to type of logs
	switch (m_nLogDataType)
	{
	case LOGTYPE_APPEVENT_LOG:
		// Format app event log filename
		strFileName.Format(FILE_APPEVENT_LOG, stTimeTemp.wYear, stTimeTemp.wMonth, stTimeTemp.wDay);
		break;
	case LOGTYPE_ACTION_LOG:
		// Action log
		strFileName = FILE_ACTION_LOG;
		break;
	default:
		// Wrong argument
		dwErrCode = DEF_APP_ERROR_WRONG_ARGUMENT;
		PostMessage(hWnd, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrCode, NULL);
		TRCFFMT(__FUNCTION__, "Invalid log type");
		return FALSE;
		break;
	}

	// Get file path
	strFilePath.Format(_T("%s\\%s"), DIR_SUBDIR_LOG, strFileName);

	// Check if file is opening, if not, open it
	if (fLogFile.m_hFile == CFile::hFileNull)
	{
		bResult = fLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == FALSE) {
			// Open file failed
			dwErrCode = GetLastError();
			PostMessage(hWnd, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrCode, NULL);
			TRCFFMT(__FUNCTION__, "Can not open/create log file");
			return bResult;
		}

		// Go to end of file
		fLogFile.SeekToEnd();
	}

	// Format output log strings
	strLogFormat = logItem.FormatLogString();

	if (!strLogFormat.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteInstantLog
//	Description:	Write log string instantly into logfile
//  Arguments:		lpszLogString - Log string
//  Return value:	BOOL - Result of log writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL SLogging::WriteInstantLog(LPCSTR lpszLogStringA, LPCSTR lpszLogDetailA /* = NULL */, BYTE byType /* = 0 */)
{
	// Convert to Unicode string
	LPCTSTR lpszLogStringW = CA2W(lpszLogStringA).m_psz;
	LPCTSTR lpszLogDetailW = CA2W(lpszLogDetailA).m_psz;
	return WriteInstantLog(lpszLogStringW, lpszLogDetailW, byType);
}

BOOL SLogging::WriteInstantLog(LPCTSTR lpszLogStringW, LPCTSTR lpszLogDetailW /* = NULL */, BYTE byType /* = 0 */)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();

	// Quit if current log mode is not write instantly mode
	if (this->GetLogWriteMode() != LOG_WRITE_MODE_INSTANT) {
		bResult = FALSE;
		TRCFFMT(__FUNCTION__, "Not write instantly mode");
		return bResult;
	}

	CString strFileName;
	CString strFilePath;
	CFile fLogFile;

	CString strLogFormat;

	// Get log time
	SYSTEMTIME stCurTime;
	GetLocalTime(&stCurTime);

	// Get filename according to type of logs
	switch (m_nLogDataType)
	{
	case LOGTYPE_APPEVENT_LOG:
		// Format app event log filename
		strFileName.Format(FILE_APPEVENT_LOG, stCurTime.wYear, stCurTime.wMonth, stCurTime.wDay);
		break;
	case LOGTYPE_ACTION_LOG:
		// Action log
		strFileName = FILE_ACTION_LOG;
		break;
	default:
		// Wrong argument
		dwErrCode = DEF_APP_ERROR_WRONG_ARGUMENT;
		PostMessage(hWnd, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrCode, NULL);
		TRCFFMT(__FUNCTION__, "Invalid log type");
		return FALSE;
		break;
	}

	// Get file path
	strFilePath.Format(_T("%s\\%s"), DIR_SUBDIR_LOG, strFileName);

	// Check if file is opening, if not, open it
	if (fLogFile.m_hFile == CFile::hFileNull)
	{
		bResult = fLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == FALSE) {
			// Open file failed
			dwErrCode = GetLastError();
			PostMessage(hWnd, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrCode, NULL);
			TRCFFMT(__FUNCTION__, "Can not open/create log file");
			return bResult;
		}

		// Go to end of file
		fLogFile.SeekToEnd();
	}

	// Format output log strings
	LOGITEM logItem;
	logItem.byType = byType;
	logItem.stTime = stCurTime;
	logItem.strLogString = lpszLogStringW;
	logItem.strDetails = lpszLogDetailW;
	strLogFormat = logItem.FormatLogString();

	if (!strLogFormat.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDlgEventLog
//	Description:	Output log for dialog item events
//  Arguments:		nEventID   - Item event ID
//					lpszDlgID  - Dialog string ID
//					lpszItemID - Item string ID
//					wParam	   - Additional param 1
//					lParam	   - Additional param 2
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::OutputDlgEventLog(UINT nEventID, LPCSTR lpszDlgID /* = NULL */, LPCSTR lpszItemID /* = NULL */, WPARAM wParam /* = NULL */, LPARAM lParam /* = NULL */)
{
	CStringA strLog;
	switch (nEventID)
	{
	// Button events
	case LOG_EVENT_BUTTON_CLICKED:
		if ((lpszDlgID == NULL) && (lpszItemID == NULL)) return;
		strLog.Format(IDS_BUTTONLOG_CLICKED_FORMAT, lpszDlgID, lpszItemID);
		break;
	case LOG_EVENT_BUTTON_ENABLED:
		if ((lpszDlgID == NULL) && (lpszItemID == NULL)) return;
		strLog.Format(IDS_BUTTONLOG_ENABLED_FORMAT, lpszDlgID, lpszItemID);
		break;
	case LOG_EVENT_BUTTON_DISABLED:
		if ((lpszDlgID == NULL) && (lpszItemID == NULL)) return;
		strLog.Format(IDS_BUTTONLOG_DISABLED_FORMAT, lpszDlgID, lpszItemID);
		break;

	// Combo-box events
	case LOG_EVENT_COMBO_SELECTION:
		if ((lpszDlgID == NULL) && (lpszItemID == NULL) && (wParam == NULL)) return;
		strLog.Format(IDS_COMBOLOG_SELECTION_FORMAT, lpszDlgID, lpszItemID, (int)wParam);
		break;
	case LOG_EVENT_COMBO_ENABLED:
		if ((lpszDlgID == NULL) && (lpszItemID == NULL)) return;
		strLog.Format(IDS_COMBOLOG_ENABLED_FORMAT, lpszDlgID, lpszItemID);
		break;
	case LOG_EVENT_COMBO_DISABLED:
		if ((lpszDlgID == NULL) && (lpszItemID == NULL)) return;
		strLog.Format(IDS_COMBOLOG_DISABLED_FORMAT, lpszDlgID, lpszItemID);
		break;

	// Checkbox events
	case LOG_EVENT_CHKBOX_CHECKED:
		if ((lpszDlgID == NULL) && (lpszItemID == NULL)) return;
		strLog.Format(IDS_CHKBOXLOG_CHECKED_FORMAT, lpszDlgID, lpszItemID);
		break;
	case LOG_EVENT_CHKBOX_UNCHECKED:
		if ((lpszDlgID == NULL) && (lpszItemID == NULL)) return;
		strLog.Format(IDS_CHKBOXLOG_UNCHECKED_FORMAT, lpszDlgID, lpszItemID);
		break;
	case LOG_EVENT_CHKBOX_ENABLED:
		if ((lpszDlgID == NULL) && (lpszItemID == NULL)) return;
		strLog.Format(IDS_CHKBOXLOG_ENABLED_FORMAT, lpszDlgID, lpszItemID);
		break;
	case LOG_EVENT_CHKBOX_DISABLED:
		if ((lpszDlgID == NULL) && (lpszItemID == NULL)) return;
		strLog.Format(IDS_CHKBOXLOG_DISABLED_FORMAT, lpszDlgID, lpszItemID);
		break;

	// Menu events
	case LOG_EVENT_MENU_SELECTION:
		if (lpszItemID == NULL) return;
		strLog.Format(IDS_MENULOG_SELECTION_FORMAT, lpszItemID);
		break;
	case LOG_EVENT_MENU_ENABLED:
		if (lpszItemID == NULL) return;
		strLog.Format(IDS_MENULOG_ENABLED_FORMAT, lpszItemID);
		break;
	case LOG_EVENT_MENU_DISABLED:
		if (lpszItemID == NULL) return;
		strLog.Format(IDS_MENULOG_DISABLED_FORMAT, lpszItemID);
		break;

	// Dialog events
	case LOG_EVENT_DLG_STARTUP:
		if (lpszDlgID == NULL) return;
		strLog.Format(IDS_DIALOGLOG_STARTUP_FORMAT, lpszDlgID);
		break;
	case LOG_EVENT_DLG_DESTROYED:
		if (lpszDlgID == NULL) return;
		strLog.Format(IDS_DIALOGLOG_DESTROYED_FORMAT, lpszDlgID);
		break;
	case LOG_EVENT_DLG_SHOWED:
		if (lpszDlgID == NULL) return;
		strLog.Format(IDS_DIALOGLOG_SHOWED_FORMAT, lpszDlgID);
		break;
	case LOG_EVENT_DLG_HIDDEN:
		if (lpszDlgID == NULL) return;
		strLog.Format(IDS_DIALOGLOG_HIDDEN_FORMAT, lpszDlgID);
		break;
	case LOG_EVENT_DLG_EXPANDED:
		if (lpszDlgID == NULL) return;
		strLog.Format(IDS_DIALOGLOG_EXPANDED_FORMAT, lpszDlgID);
		break;
	case LOG_EVENT_DLG_COLLAPSED:
		if (lpszDlgID == NULL) return;
		strLog.Format(IDS_DIALOGLOG_COLLAPSED_FORMAT, lpszDlgID);
		break;

	// Notify icon events
	case LOG_EVENT_NOTIFY_CREATED:
		strLog.Format(IDS_NOTIFYLOG_CREATED_FORMAT);
		break;
	case LOG_EVENT_NOTIFY_REMOVED:
		strLog.Format(IDS_NOTIFYLOG_REMOVED_FORMAT);
		break;
	case LOG_EVENT_NOTIFY_LMBCLICKED:
		strLog.Format(IDS_NOTIFYLOG_LMBCLICKED_FORMAT);
		break;
	case LOG_EVENT_NOTIFY_MMBCLICKED:
		strLog.Format(IDS_NOTIFYLOG_MMBCLICKED_FORMAT);
		break;
	case LOG_EVENT_NOTIFY_RMBCLICKED:
		strLog.Format(IDS_NOTIFYLOG_RMBCLICKED_FORMAT);
		break;
	case LOG_EVENT_NOTIFY_SHOWMENU:
		strLog.Format(IDS_NOTIFYLOG_SHOWMENU_FORMAT);
		break;
	case LOG_EVENT_NOTIFY_UPDATETIPTEXT:
		strLog.Format(IDS_NOTIFYLOG_UPDATETIPTEXT_FORMAT);
		break;
	}

	// Log string validity
	if (strLog.IsEmpty())
		return;

	// Output log string
	OutputLogString(strLog);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDataChangeLog
//	Description:	Output log for data change event
//  Arguments:		byDataType	   - Data type ID
//					byDataCategory - Data category ID
//					dwCtrlID	   - Parent control ID
//					dwPreVal	   - Previous value (integer)
//					dwAftVal	   - After value (integer)
//					lpszFlagName   - Additional flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::OutputDataChangeLog(BYTE byDataType, BYTE byDataCategory, DWORD dwCtrlID, DWORD dwPreVal, DWORD dwAftVal, LPCTSTR lpszFlagName)
{
	UINT nLogStringFormatID = DEF_INTEGER_INVALID;
	CString strDataCate = DEF_STRING_NULL;
	CString strPreVal = DEF_STRING_NULL;
	CString strAftVal = DEF_STRING_NULL;
	CString strCtrlName = DEF_STRING_NULL;
	CString strTemp;
	CString strLogFormat;
	SYSTEMTIME stTimeTemp;

	// Load language package
	LANGTABLE_PTR pLang = LoadLanguageTable(APP_LANGUAGE_ENGLISH);
	strTemp = GetLanguageString(pLang, dwCtrlID);
	strCtrlName = strTemp;
	if (strTemp.GetLength() > DEF_DATACHANGELOG_CTRLNAME_MAXLENGTH) {
		// Shorten control title if too long
		int nMaxLength = DEF_DATACHANGELOG_CTRLNAME_MAXLENGTH;
		strCtrlName.Format(_T("%s..."), strTemp.Left(nMaxLength));
	}
	if ((dwCtrlID == DEF_INTEGER_NULL) &&
		(lpszFlagName != NULL)) {
		// Use flag value instead
		strCtrlName = lpszFlagName;
	}

	switch (byDataCategory)
	{
	case DATACATE_APPCONFIG:
		strDataCate = _T("Config");
		break;
	case DATACATE_SCHEDULE:
		strDataCate = _T("Schedule");
		break;
	case DATACATE_HOTKEYSET:
		strDataCate = _T("HotkeySet");
		break;
	case DATACATE_PWRREMINDER:
		strDataCate = _T("PwrReminder");
		break;
	}

	switch (byDataType)
	{
	case DATATYPE_YESNO_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_YESNO_VALUE;
		strPreVal = (dwPreVal == TRUE) ? _T("Yes") : _T("No");
		strAftVal = (dwAftVal == TRUE) ? _T("Yes") : _T("No");
		strLogFormat.Format(nLogStringFormatID, strDataCate, strCtrlName, strPreVal, strAftVal);
		break;
	case DATATYPE_OPTION_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_OPTION_VALUE;
		strPreVal = GetLanguageString(pLang, dwPreVal);
		strAftVal = GetLanguageString(pLang, dwAftVal);
		strLogFormat.Format(nLogStringFormatID, strDataCate, strCtrlName, strPreVal, strAftVal);
		break;
	case DATATYPE_ITEMNUM_ADD:
		nLogStringFormatID = IDS_DATACHANGELOG_ITEMNUM_ADD;
		strLogFormat.Format(nLogStringFormatID, strDataCate, dwPreVal, dwAftVal);
		break;
	case DATATYPE_ITEMNUM_REMOVE:
		nLogStringFormatID = IDS_DATACHANGELOG_ITEMNUM_REMOVE;
		strLogFormat.Format(nLogStringFormatID, strDataCate, dwPreVal, dwAftVal);
		break;
	case DATATYPE_STRING_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_STRING_VALUE;
		strPreVal = GetLanguageString(pLang, dwPreVal);
		strAftVal = GetLanguageString(pLang, dwAftVal);
		strLogFormat.Format(nLogStringFormatID, strDataCate, strPreVal, strAftVal);
		break;
	case DATATYPE_TIME_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_TIME_VALUE;
		SpinPos2Time(stTimeTemp, dwPreVal);
		strPreVal.Format(_T("%02d:%02d"), stTimeTemp.wHour, stTimeTemp.wMinute);
		SpinPos2Time(stTimeTemp, dwAftVal);
		strAftVal.Format(_T("%02d:%02d"), stTimeTemp.wHour, stTimeTemp.wMinute);
		strLogFormat.Format(nLogStringFormatID, strDataCate, strPreVal, strAftVal);
		break;
	case DATATYPE_FLAG_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_FLAG_VALUE;
		strPreVal.Format(_T("%d"), dwPreVal);
		strAftVal.Format(_T("%d"), dwAftVal);
		strLogFormat.Format(nLogStringFormatID, strDataCate, strPreVal, strAftVal);
		break;
	case DATATYPE_SPEC_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_SPEC_VALUE;
		strPreVal.Format(_T("%d"), dwPreVal);
		strAftVal.Format(_T("%d"), dwAftVal);
		strLogFormat.Format(nLogStringFormatID, strDataCate, strCtrlName, strPreVal, strAftVal);
		break;
	}

	// If log string is empty, do nothing
	if (strLogFormat.IsEmpty())
		return;

	// Output log string
	OutputLogString(strLogFormat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDataChangeLog
//	Description:	Output log for data change event
//  Arguments:		byDataType	   - Data type ID
//					byDataCategory - Data category ID
//					dwCtrlID	   - Parent control ID
//					lpszPreVal	   - Previous value (string)
//					lpszAftVal	   - After value (string)
//					lpszFlagName   - Additional flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::OutputDataChangeLog(BYTE byDataType, BYTE byDataCategory, DWORD dwCtrlID, LPCTSTR lpszPreVal, LPCTSTR lpszAftVal, LPCTSTR lpszFlagName)
{
	UINT nLogStringFormatID = DEF_INTEGER_INVALID;
	CString strDataCate = DEF_STRING_NULL;
	DWORD dwPreVal = DEF_INTEGER_NULL;
	DWORD dwAftVal = DEF_INTEGER_NULL;
	CString strCtrlName = DEF_STRING_NULL;
	CString strTemp;
	CString strPreVal, strAftVal;
	CString strLogFormat;

	// Load language package
	LANGTABLE_PTR pLang = LoadLanguageTable(APP_LANGUAGE_ENGLISH);
	strTemp = GetLanguageString(pLang, dwCtrlID);
	strCtrlName = strTemp;
	if (strTemp.GetLength() > DEF_DATACHANGELOG_CTRLNAME_MAXLENGTH) {
		// Shorten control title if too long
		int nMaxLength = DEF_DATACHANGELOG_CTRLNAME_MAXLENGTH;
		strCtrlName.Format(_T("%s..."), strTemp.Left(nMaxLength));
	}
	if ((dwCtrlID == DEF_INTEGER_NULL) &&
		(lpszFlagName != NULL)) {
		// Use flag value instead
		strCtrlName = lpszFlagName;
	}

	switch (byDataCategory)
	{
	case DATACATE_APPCONFIG:
		strDataCate = _T("Config");
		break;
	case DATACATE_SCHEDULE:
		strDataCate = _T("Schedule");
		break;
	case DATACATE_HOTKEYSET:
		strDataCate = _T("HotkeySet");
		break;
	case DATACATE_PWRREMINDER:
		strDataCate = _T("PwrReminder");
		break;
	}

	switch (byDataType)
	{
	case DATATYPE_YESNO_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_YESNO_VALUE;
		strLogFormat.Format(nLogStringFormatID, strDataCate, strCtrlName, lpszPreVal, lpszAftVal);
		break;
	case DATATYPE_OPTION_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_OPTION_VALUE;
		strLogFormat.Format(nLogStringFormatID, strDataCate, strCtrlName, lpszPreVal, lpszAftVal);
		break;
	case DATATYPE_ITEMNUM_ADD:
		nLogStringFormatID = IDS_DATACHANGELOG_ITEMNUM_ADD;
		dwPreVal = _tstoi(lpszPreVal);
		dwAftVal = _tstoi(lpszAftVal);
		strLogFormat.Format(nLogStringFormatID, strDataCate, dwPreVal, dwAftVal);
		break;
	case DATATYPE_ITEMNUM_REMOVE:
		nLogStringFormatID = IDS_DATACHANGELOG_ITEMNUM_REMOVE;
		dwPreVal = _tstoi(lpszPreVal);
		dwAftVal = _tstoi(lpszAftVal);
		strLogFormat.Format(nLogStringFormatID, strDataCate, dwPreVal, dwAftVal);
		break;
	case DATATYPE_STRING_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_STRING_VALUE;
		strTemp = lpszPreVal;
		strPreVal = strTemp;
		if (strTemp.GetLength() > DEF_LOGDISP_STRING_MAXLENGTH) {
			strPreVal = strTemp.Left(DEF_LOGDISP_STRING_MAXLENGTH) + _T("...");
		}
		strTemp = lpszAftVal;
		strAftVal = strTemp;
		if (strTemp.GetLength() > DEF_LOGDISP_STRING_MAXLENGTH) {
			strAftVal = strTemp.Left(DEF_LOGDISP_STRING_MAXLENGTH) + _T("...");
		}
		strLogFormat.Format(nLogStringFormatID, strDataCate, strPreVal, strAftVal);
		break;
	case DATATYPE_TIME_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_TIME_VALUE;
		strLogFormat.Format(nLogStringFormatID, strDataCate, lpszPreVal, lpszAftVal);
		break;
	case DATATYPE_FLAG_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_FLAG_VALUE;
		strLogFormat.Format(nLogStringFormatID, strDataCate, lpszPreVal, lpszAftVal);
		break;
	case DATATYPE_SPEC_VALUE:
		nLogStringFormatID = IDS_DATACHANGELOG_SPEC_VALUE;
		strLogFormat.Format(nLogStringFormatID, strDataCate, strCtrlName, lpszPreVal, lpszAftVal);
		break;
	}

	// If log string is empty, do nothing
	if (strLogFormat.IsEmpty())
		return;

	// Output log string
	OutputLogString(strLogFormat);
}
