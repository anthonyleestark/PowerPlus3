
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

#include "AppCore\AppCore.h"
#include "AppCore\Global.h"
#include "Framework\SDialog.h"


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
	CFont* m_pDebugViewFont;
	CBrush* m_pDebugViewBrush;

	// Buffer content
	CString m_strBuffer;
	CString m_strBufferBak;

	// Debug command history
	BOOL		 m_bCurDispHistory;
	INT_PTR		 m_nHistoryCurIndex;
	CStringArray m_astrCommandHistory;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

public:
	// Generated message handle functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	virtual void OnDestroy();
	virtual void OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo);
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnSize(UINT nType, int nWidth, int nHeight);
	afx_msg void OnDebugViewEditChange(void);
	afx_msg LRESULT OnDebugOutput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugCmdNoReply(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugViewClear(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowDialog(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual BOOL SendDebugCommand(void);

private:
	// Member functions
	CEdit* GetDebugEditView(void) const;
	BOOL InitDebugEditView(UINT nCtrlID);
	BOOL CreateDebugViewFont(void);
	BOOL CreateDebugViewBrush(void);

	inline BOOL IsDebugEditViewValid(void);
	inline BOOL IsDebugEditViewFocus(void);

	INT_PTR GetCaretPosition(void);
	BOOL	ShowDebugTestEditViewMenu(void);

	inline void BackupDebugViewBuffer(void);
	int  FormatDebugCommand(CString &strDebugCommand);
	void ClearViewBuffer(void);

	void AddLine(LPCTSTR lpszString, BOOL bNewLine = TRUE);
	void UpdateDisplay(BOOL bSeekToEnd = FALSE, BOOL bNotifyParent = TRUE);

	INT_PTR AddDebugCommandHistory(LPCTSTR lpszCommand);
	void DispDebugCommandHistory(int nHistoryIndex);
	void ClearDebugCommandHistory(void);
	INT_PTR GetDebugCommandHistoryCount(void) const;
	BOOL IsDebugCommandHistoryEmpty(void) const;

	inline BOOL IsCurrentlyDispHistory(void) const;
	inline void SetCurrentlyDispHistoryState(BOOL bState);
	inline INT_PTR GetHistoryCurrentDispIndex(void) const;
	inline void SetHistoryCurrentDispIndex(INT_PTR nCurIndex);
};


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _DEBUGTESTDLG_ENABLE_INLINES
	#include "Dialogs.inl"
	#ifdef _DEBUGTESTDLG_INLINE_INCLUDED
		#pragma message("-- Dialogs inline library included (DebugTestDlg.h)")
	#else
		#pragma error("-- Linking error in DebugTestDlg.h: Unable to link to inline header!")
	#endif
	#undef _DEBUGTESTDLG_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in DebugTestDlg.h: Inline is not enabled!")
#endif

#endif	// ifndef _DEBUGTESTDLG_H_INCLUDED
