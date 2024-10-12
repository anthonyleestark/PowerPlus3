
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		LogViewerDlg.h
//		Description:	Header file for LogViewer dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.24:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LOGVIEWERDLG_H_INCLUDED
#define _LOGVIEWERDLG_H_INCLUDED

#include "Core.h"
#include "Config.h"
#include "Logging.h"
#include "IDMapping.h"
#include "SDialog.h"

////////////////////////////////////////////////////////
//
//	CLogViewrDlg dialog used for LogViewer function
//
////////////////////////////////////////////////////////

class CLogViewerDlg : public SDialog
{
	DECLARE_DYNAMIC(CLogViewerDlg)

public:
	CLogViewerDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLogViewerDlg();				  // destructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGVIEWER_DLG };
#endif

private:
	CListBox* m_pLogViewerList;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnSelectLogItem();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	DECLARE_CLASS_IDMAP()

public:
	void SetupLanguage();
	void UpdateLogViewer();
	void DisplayLogDetails(int nIndex);
};

#endif	// ifndef _LOGVIEWERDLG_H_INCLUDED