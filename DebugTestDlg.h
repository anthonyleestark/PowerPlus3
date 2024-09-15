
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		DebugTestDlg.h
//		Description:	Header file for DebugTest dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2024.07.13:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUGTESTDLG_H_INCLUDED
#define _DEBUGTESTDLG_H_INCLUDED

#include "Core.h"
#include "Global.h"
#include "SDialog.h"


////////////////////////////////////////////////////////
//
//	CDebugTestDlg dialog used for DebugTest dialog
//
////////////////////////////////////////////////////////

class CDebugTestDlg : public SDialog
{
	DECLARE_DYNAMIC(CDebugTestDlg)

public:
	CDebugTestDlg();	// constructor
	~CDebugTestDlg();	// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEBUGTEST_DLG };
#endif

private:
	// Edit view
	CEdit* m_pDebugEditView;
	CString m_strBuffer;
	CString m_strBufferBak;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

public:
	// Generated message handle functions
	virtual BOOL OnInitDialog();
	virtual void OnDestroy();
	virtual void OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo);
	virtual void OnSize(UINT nType, int nWidth, int nHeight);
	afx_msg void OnDebugViewChange(void);
	afx_msg LRESULT OnDebugOutput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugViewClear(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual void SendDebugCommand(HWND hRcvWnd);

private:
	// Member functions
	void BackupDebugViewBuffer(void);
	void FormatDebugCommand(CString &strDebugCommand);
	void ClearViewBuffer(void);
	void AddLine(LPCTSTR lpszString);
	void UpdateDisplay(BOOL bSeekToEnd = FALSE);
};

#endif	// ifndef _DEBUGTESTDLG_H_INCLUDED