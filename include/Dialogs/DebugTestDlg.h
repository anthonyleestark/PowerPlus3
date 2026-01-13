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
	CEdit* debugEditViewPtr_;
	CFont* debugViewFontPtr_;
	CBrush* debugViewBrushPtr_;

	// Buffer content
	String bufferString_;
	String backupBufferString_;

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
	afx_msg void OnDebugViewEditChange(void);
	afx_msg LRESULT OnDebugOutput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugCmdNoReply(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugViewClear(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowDialog(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG *messagePtr);
	virtual bool SendDebugCommand(void);

private:
	// Member functions
	CEdit* getDebugEditView(void) const {
		return debugEditViewPtr_;
	}
	bool initDebugEditView(unsigned controlId);
	bool createDebugViewFont(void);
	bool createDebugViewBrush(void);

	inline bool isDebugEditViewValid(void) {
		return (getDebugEditView() != NULL);
	}
	inline bool isDebugEditViewFocus(void) {
		// Check DebugTest edit view validity
		if (!isDebugEditViewValid()) return FALSE;

		// Check if it is focused
		HWND currentFocusedWndHandle = GetFocus()->GetSafeHwnd();
		return (currentFocusedWndHandle == getDebugEditView()->GetSafeHwnd());
	}

	int getCaretPosition(void);
	bool showDebugTestEditViewMenu(void);

	inline void backupDebugViewBuffer(void) {
		backupBufferString_ = bufferString_;
	}
	int  formatDebugCommand(String &debugCommand);
	void clearViewBuffer(void);

	void addLine(const wchar_t* lineString, bool newLine = TRUE);
	void updateDisplay(bool isSeekToEnd = FALSE, bool notifyParent = TRUE);

	size_t addDebugCommandHistory(const wchar_t* commandString);
	void dispDebugCommandHistory(int historyIndex);
	void clearDebugCommandHistory(void) {
		commandHistoryList_.clear();
	}
	size_t getDebugCommandHistoryCount(void) const {
		return commandHistoryList_.size();
	}
	bool isDebugCommandHistoryEmpty(void) const {
		return commandHistoryList_.empty();
	}

	inline bool isCurrentlyDispHistory(void) const {
		return isCurrentlyDisplayHistory_;
	}
	inline void setCurrentlyDispHistoryState(bool state) {
		isCurrentlyDisplayHistory_ = state;
	}
	inline size_t getHistoryCurrentDispIndex(void) const {
		return currentHistoryIndex_;
	}
	inline void setHistoryCurrentDispIndex(size_t currentIndex) {
		currentHistoryIndex_ = currentIndex;
	}
};

