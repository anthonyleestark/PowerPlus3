
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		LogViewerDlg.h
//		Description:	Header file for LogViewer dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.24:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LOGVIEWERDLG_H_INCLUDED
#define _LOGVIEWERDLG_H_INCLUDED

#include "AppCore\AppCore.h"
#include "AppCore\Config.h"
#include "AppCore\MapTable.h"
#include "AppCore\Logging.h"
#include "AppCore\IDMapping.h"
#include "Framework\SDialog.h"

////////////////////////////////////////////////////////
//
//	CLogViewrDlg dialog used for LogViewer function
//
////////////////////////////////////////////////////////

class CLogViewerDlg : public SDialog
{
	DECLARE_DYNAMIC(CLogViewerDlg)

public:
	using Item = typename const LogItem&;
	using Data = typename SLogging*;
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
	INT_PTR	m_nLogCount;

	// Table format and properties
	int	m_nColNum;
	GRIDCTRLCOLFORMAT* m_apGrdColFormat;
	CSize* m_pszTableFrameSize;

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

#endif	// ifndef _LOGVIEWERDLG_H_INCLUDED
