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
	CEdit* debugViewPtr_;
	CEdit* debugCommandInputPtr_;

	// Font & brush
	CFont* debugScreenFontPtr_;
	CBrush* debugScreenBrushPtr_;

	// Buffer content
	String bufferString_;
	String backupBufferString_;
	String commandBuffer_;

	// Specific flags
	bool isCommandPrefixEnabled_;

	// Debug command history
	bool		 isCurrentlyDisplayHistory_;
	size_t		 currentHistoryIndex_;
	StringArray  commandHistoryList_;

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
	virtual void OnGetMinMaxInfo(MINMAXINFO* minMaxInfoPtr);
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* windowPtr, UINT nCtlColor);
	virtual void OnSize(UINT nType, int width, int height);
	afx_msg LRESULT OnDebugOutput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugCmdNoReply(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugViewClear(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowDialog(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* messagePtr);
	virtual bool SendDebugCommand(void);

private:
	// Get access to the DebugTest view pointer
	CEdit* GetDebugView(void) const {
		return debugViewPtr_;
	};

	// Get access to the Debug command input pointer
	CEdit* GetDebugCommandInput(void) const {
		return debugCommandInputPtr_;
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
		backupBufferString_ = bufferString_;
	};
	void ClearDebugViewBuffer(void);

	void AddLine(const wchar_t* lpszString, bool newLine = true);
	void UpdateDisplay(bool isSeekToEnd = false, bool notifyParent = true);

	size_t AddDebugCommandHistory(const wchar_t* commandString);
	void DispDebugCommandHistory(int historyIndex);
	void ClearDebugCommandHistory(void) {
		commandHistoryList_.clear();
	};
	size_t GetDebugCommandHistoryCount(void) const {
		return commandHistoryList_.size();
	};
	bool IsDebugCommandHistoryEmpty(void) const {
		return commandHistoryList_.empty();
	};

	bool IsCurrentlyDispHistory(void) const {
		return isCurrentlyDisplayHistory_;
	};
	void SetCurrentlyDispHistoryState(bool state) {
		isCurrentlyDisplayHistory_ = state;
	};
	size_t GetHistoryCurrentDispIndex(void) const {
		return currentHistoryIndex_;
	};
	void SetHistoryCurrentDispIndex(size_t currentIndex) {
		currentHistoryIndex_ = currentIndex;
	};
};

