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
	using Data = Logger*;
	enum ColumnID {
		DateTime = 0,								// Date/Time
		CategoryID,									// Event ID (category ID)
		Description,								// Additional description
	};

public:
	CLogViewerDlg(CWnd* parentWnd = nullptr);			// standard constructor
	virtual ~CLogViewerDlg();						// destructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGVIEWER_DLG };
#endif

private:
	// Member variables
	CGridCtrl* logViewerListPtr_;
	Data appEventLoggerPtr_;
	size_t logCount_;

	// Table format and properties
	int	columnCount_;
	GRIDCTRLCOLFORMAT* gridCtrlFormatInfoPtr_;
	Size* logViewerTableSizePtr_;

	// Other variables
	int currentMode_;
	int checkCount_;
	int curSelIndex_;

public:
	// Generated handlers
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	virtual LRESULT requestCloseDialog(void);
	afx_msg void OnRemoveAllBtn();
	afx_msg void OnDetailBtn();
	afx_msg void OnCloseBtn();
	afx_msg void OnSelectLogItem(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_RESOURCEID_MAP()

public:
	// Member functions
	void setupLanguage(void);
	void setupLogViewerList(LANGTABLE_PTR languageTablePtr);
	void drawLogViewerTable(void);
	BOOL loadAppEventLogData(void);
	void updateLogViewer(void);
	void displayLogDetails(int index);

	// Layout functions
	void updateLayoutInfo(void);
	void loadLayoutInfo(void);
	void saveLayoutInfo(void);
};
