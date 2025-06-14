/**
 * @file		DebugTestV2Dlg.h
 * @brief		Header file for DebugTestV2 dialog
 * @author		AnthonyLeeStark
 * @date		2025.06.02
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/Global.h"
#include "Framework/SDialog.h"


// DebugTestV2 dialog class
class CDebugTestV2Dlg : public SDialog
{
	DECLARE_DYNAMIC(CDebugTestV2Dlg)

public:
	enum Refresh {
		ScreenSize			= (int)0x001,
	};

public:
	CDebugTestV2Dlg();		// constructor
	~CDebugTestV2Dlg();		// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEBUGTESTV2_DLG };
#endif

private:
	// DebugScreen
	CEdit* m_pDebugView;
	CEdit* m_pDebugCommandInput;

	// Font & brush
	CFont* m_pDebugScreenFont;
	CBrush* m_pDebugScreenBrush;

	// Buffer content
	String m_strBuffer;
	String m_strBackupBuffer;
	String m_strCommandBuffer;

	// Specific flags
	bool m_bDispCommandPrefix;

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
	afx_msg LRESULT OnDebugOutput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugCmdNoReply(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugViewClear(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowDialog(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual bool SendDebugCommand(void);

private:
	// Get access to the DebugTest view pointer
	CEdit* GetDebugView(void) const {
		return m_pDebugView;
	};

	// Get access to the Debug command input pointer
	CEdit* GetDebugCommandInput(void) const {
		return m_pDebugCommandInput;
	};

	// DebugView and command input initialization
	bool InitDebugScreen(void);
	bool CreateDebugScreenFont(void);
	bool CreateDebugScreenBrush(void);

	// Check if the DebugScreen (DebugView + Debug command input) is valid
	bool IsDebugScreenValid(void) {
		return (IsDebugViewValid() && IsDebugCommandInputValid());
	};
	bool IsDebugViewValid(void) {
		return (GetDebugView() != NULL);
	};
	bool IsDebugCommandInputValid(void) {
		return (GetDebugCommandInput() != NULL);
	};
	bool IsDebugCommandInputFocused(void) {
		// Check DebugScreen validity
		if (!IsDebugScreenValid() || !IsDebugCommandInputValid()) return false;

		// Get focused control
		HWND hCurFocusWnd = GetFocus()->GetSafeHwnd();
		return (hCurFocusWnd == GetDebugCommandInput()->GetSafeHwnd());
	};

	bool RefreshDebugScreen(int nFlag);
	bool ShowDebugScreenContextMenu(void);

	int  FormatDebugCommand(String& debugCommand) const;
	void ClearDebugCommandInput(const wchar_t* commandBuff = Constant::String::Empty);
	void BackupDebugViewBuffer(void) {
		m_strBackupBuffer = m_strBuffer;
	};
	void ClearDebugViewBuffer(void);

	void AddLine(const wchar_t* lpszString, bool bNewLine = true);
	void UpdateDisplay(bool bSeekToEnd = false, bool bNotifyParent = true);

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

	bool IsCurrentlyDispHistory(void) const {
		return m_bCurDispHistory;
	};
	void SetCurrentlyDispHistoryState(bool bState) {
		m_bCurDispHistory = bState;
	};
	size_t GetHistoryCurrentDispIndex(void) const {
		return m_nHistoryCurIndex;
	};
	void SetHistoryCurrentDispIndex(size_t nCurIndex) {
		m_nHistoryCurIndex = nCurIndex;
	};
};

