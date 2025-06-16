/**
 * @file		LogViewerDlg.h
 * @brief		Header file for LogViewer dialog
 * @author		AnthonyLeeStark
 * @date		2024.02.24
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/MapTable.h"
#include "AppCore/Logging.h"
#include "AppCore/IDManager.h"
#include "AppCore/Serialization.h"
#include "Framework/SDialog.h"


// LogViewer dialog class
class CLogViewerDlg : public SDialog
{
	DECLARE_DYNAMIC(CLogViewerDlg)

public:
	using Item = const LogItem&;
	using Data = SLogging*;
	enum ColumnID {
		DateTime = 0,								// Date/Time
		CategoryID,									// Event ID (category ID)
		Description,								// Additional description
	};

public:
	CLogViewerDlg(CWnd* pParent = nullptr);			// standard constructor
	virtual ~CLogViewerDlg();						// destructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGVIEWER_DLG };
#endif

private:
	// Member variables
	CGridCtrl* m_pLogViewerList;
	Data m_ptrAppEventLog;
	size_t m_nLogCount;

	// Table format and properties
	int	m_nColNum;
	GRIDCTRLCOLFORMAT* m_apGrdColFormat;
	Size* m_pszTableFrameSize;

	// Other variables
	int m_nCurMode;
	int m_nCheckCount;
	int m_nCurSelIndex;

public:
	// Generated handlers
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	virtual LRESULT RequestCloseDialog(void);
	afx_msg void OnRemoveAllBtn();
	afx_msg void OnDetailBtn();
	afx_msg void OnCloseBtn();
	afx_msg void OnSelectLogItem(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_RESOURCEID_MAP()

public:
	// Member functions
	void SetupLanguage(void);
	void SetupLogViewerList(LANGTABLE_PTR ptrLanguage);
	void DrawLogViewerTable(void);
	BOOL LoadAppEventLogData(void);
	void UpdateLogViewer(void);
	void DisplayLogDetails(int nIndex);

	// Layout functions
	void UpdateLayoutInfo(void);
	void LoadLayoutInfo(void);
	void SaveLayoutInfo(void);
};
