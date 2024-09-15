
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PowerPlus.inl
//		Description:	Define inline functions for the app
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.07.26:		Create new for version 3.1
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////
//
//	Define inline functions for output logging
//
////////////////////////////////////////////////////////

// Get app log count
_AFXWIN_INLINE int GetAppLogCount()
{
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	SLogging* pAppLog = pApp->GetAppEventLog();
	VERIFY(pAppLog != NULL);
	if (pAppLog == NULL) return 0;
	return pAppLog->GetLogCount();
}

// Get app log item
_AFXWIN_INLINE LOGITEM GetAppLogItem(int nIndex)
{
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	SLogging* pAppLog = pApp->GetAppEventLog();
	VERIFY(pAppLog != NULL);
	if (pAppLog == NULL) return LOGITEM();
	return pAppLog->GetLogItem(nIndex);
}

// Output button log
_AFXWIN_INLINE void OutputButtonLog(UINT nDialogID, UINT nButtonID, UINT nButtonEvent = LOG_EVENT_BUTTON_CLICKED)
{
	CStringA strDialogID(IDMAP_GET(nDialogID));
	CStringA strButtonID(IDMAP_GET(nButtonID));
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp->GetAppEventLogOption() == FALSE) return;
	SLogging* pAppLog = pApp->GetAppEventLog();
	VERIFY(pAppLog != NULL);
	if (pAppLog == NULL) return;
	return pAppLog->OutputDlgEventLog(nButtonEvent, strDialogID, strButtonID);
}

// Output combo log
_AFXWIN_INLINE void OutputComboLog(UINT nDialogID, UINT nComboID, int nSel, UINT nComboEvent = LOG_EVENT_COMBO_SELECTION)
{
	CStringA strDialogID(IDMAP_GET(nDialogID));
	CStringA strComboID(IDMAP_GET(nComboID));
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp->GetAppEventLogOption() == FALSE) return;
	SLogging* pAppLog = pApp->GetAppEventLog();
	VERIFY(pAppLog != NULL);
	if (pAppLog == NULL) return;
	return pAppLog->OutputDlgEventLog(nComboEvent, strDialogID, strComboID, nSel);
}

// Ouput checkbox log
_AFXWIN_INLINE void OutputCheckboxLog(UINT nDialogID, UINT nCheckboxID, UINT nCheckboxEvent)
{
	CStringA strDialogID(IDMAP_GET(nDialogID));
	CStringA strCheckboxID(IDMAP_GET(nCheckboxID));
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp->GetAppEventLogOption() == FALSE) return;
	SLogging* pAppLog = pApp->GetAppEventLog();
	VERIFY(pAppLog != NULL);
	if (pAppLog == NULL) return;
	return pAppLog->OutputDlgEventLog(nCheckboxEvent, strDialogID, strCheckboxID);
}

// Output menu log
_AFXWIN_INLINE void OutputMenuLog(UINT nMenuItemID, UINT nMenuItemEvent = LOG_EVENT_MENU_SELECTION)
{
	CStringA strMenuItemID(IDMAP_GET(nMenuItemID));
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp->GetAppEventLogOption() == FALSE) return;
	SLogging* pAppLog = pApp->GetAppEventLog();
	VERIFY(pAppLog != NULL);
	if (pAppLog == NULL) return;
	return pAppLog->OutputDlgEventLog(nMenuItemEvent, strMenuItemID);
}

// Output dialog log
_AFXWIN_INLINE void OutputDialogLog(UINT nDialogID, UINT nDialogEvent)
{
	CStringA strDialogID(IDMAP_GET(nDialogID));
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp->GetAppEventLogOption() == FALSE) return;
	SLogging* pAppLog = pApp->GetAppEventLog();
	VERIFY(pAppLog != NULL);
	if (pAppLog == NULL) return;
	return pAppLog->OutputDlgEventLog(nDialogEvent, strDialogID);
}

// Output notify icon log
_AFXWIN_INLINE void OutputNotifyIconLog(UINT nNotifyEvent)
{
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp->GetAppEventLogOption() == FALSE) return;
	SLogging* pAppLog = pApp->GetAppEventLog();
	VERIFY(pAppLog != NULL);
	if (pAppLog == NULL) return;
	return pAppLog->OutputDlgEventLog(nNotifyEvent);
}

// Output function log
_AFXWIN_INLINE void OutputFunctionLog(CStringA strFunctionName, CStringA strFunctionDetail)
{
	CStringA strFunctionLog;
	strFunctionLog.Format(IDS_FUNCTIONLOG_FORMAT, strFunctionName, strFunctionDetail);
	if (strFunctionLog.IsEmpty()) return;
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp->GetAppEventLogOption() == FALSE) return;
	SLogging* pAppLog = pApp->GetAppEventLog();
	VERIFY(pAppLog != NULL);
	if (pAppLog == NULL) return;
	return pAppLog->OutputLogString(strFunctionLog);
}

// Output app event log
_AFXWIN_INLINE void OutputAppEventLog(CStringA strOutputLog)
{
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp->GetAppEventLogOption() == FALSE) return;
	SLogging* pAppLog = pApp->GetAppEventLog();
	VERIFY(pAppLog != NULL);
	if (pAppLog == NULL) return;
	return pAppLog->OutputLogString(strOutputLog);
}

