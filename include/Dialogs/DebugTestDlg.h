/**
 * @file		DebugTestDlg.h
 * @brief		Header file for DebugTest dialog
 * @author		AnthonyLeeStark
 * @date		2024.07.13
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/Global.h"
#include "Framework/SDialog.h"


// DebugTest dialog class
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
	String m_strBuffer;
	String m_strBufferBak;

	// Debug command history
	bool		 m_bCurDispHistory;
	size_t		 m_nHistoryCurIndex;
	StringArray  m_astrCommandHistory;

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
	virtual bool SendDebugCommand(void);

private:
	// Member functions
	CEdit* GetDebugEditView(void) const {
		return m_pDebugEditView;
	};
	bool InitDebugEditView(unsigned nCtrlID);
	bool CreateDebugViewFont(void);
	bool CreateDebugViewBrush(void);

	inline bool IsDebugEditViewValid(void) {
		return (GetDebugEditView() != NULL);
	};
	inline bool IsDebugEditViewFocus(void) {
		// Check DebugTest edit view validity
		if (!IsDebugEditViewValid()) return FALSE;

		// Check if it is focused
		HWND hCurFocusWnd = GetFocus()->GetSafeHwnd();
		return (hCurFocusWnd == GetDebugEditView()->GetSafeHwnd());
	};

	int GetCaretPosition(void);
	bool ShowDebugTestEditViewMenu(void);

	inline void BackupDebugViewBuffer(void) {
		m_strBufferBak = m_strBuffer;
	};
	int  FormatDebugCommand(String &debugCommand);
	void ClearViewBuffer(void);

	void AddLine(const wchar_t* lineString, bool bNewLine = TRUE);
	void UpdateDisplay(bool bSeekToEnd = FALSE, bool bNotifyParent = TRUE);

	size_t AddDebugCommandHistory(const wchar_t* commandString);
	void DispDebugCommandHistory(int nHistoryIndex);
	void ClearDebugCommandHistory(void) {
		m_astrCommandHistory.clear();
	};
	size_t GetDebugCommandHistoryCount(void) const {
		return m_astrCommandHistory.size();
	};
	bool IsDebugCommandHistoryEmpty(void) const {
		return m_astrCommandHistory.empty();
	};

	inline bool IsCurrentlyDispHistory(void) const {
		return m_bCurDispHistory;
	};
	inline void SetCurrentlyDispHistoryState(bool bState) {
		m_bCurDispHistory = bState;
	};
	inline size_t GetHistoryCurrentDispIndex(void) const {
		return m_nHistoryCurIndex;
	};
	inline void SetHistoryCurrentDispIndex(size_t nCurIndex) {
		m_nHistoryCurIndex = nCurIndex;
	};
};

