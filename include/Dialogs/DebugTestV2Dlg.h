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
	CEdit* getDebugView(void) const {
		return debugViewPtr_;
	}

	// Get access to the Debug command input pointer
	CEdit* getDebugCommandInput(void) const {
		return debugCommandInputPtr_;
	}

	// DebugView and command input initialization
	bool initDebugScreen(void);
	bool createDebugScreenFont(void);
	bool createDebugScreenBrush(void);

	// Check if the DebugScreen (DebugView + Debug command input) is valid
	bool isDebugScreenValid(void) {
		return (isDebugViewValid() && isDebugCommandInputValid());
	}
	bool isDebugViewValid(void) {
		return (getDebugView() != NULL);
	}
	bool isDebugCommandInputValid(void) {
		return (getDebugCommandInput() != NULL);
	}
	bool isDebugCommandInputFocused(void) {
		// Check DebugScreen validity
		if (!isDebugScreenValid() || !isDebugCommandInputValid()) return false;

		// Get focused control
		HWND currentFocusedWndHandle = GetFocus()->GetSafeHwnd();
		return (currentFocusedWndHandle == getDebugCommandInput()->GetSafeHwnd());
	}

	bool refreshDebugScreen(int flag);
	bool showDebugScreenContextMenu(void);

	int  formatDebugCommand(String& debugCommand) const;
	void clearDebugCommandInput(const wchar_t* commandBuff = Constant::String::Empty);
	void backupDebugViewBuffer(void) {
		backupBufferString_ = bufferString_;
	}
	void clearDebugViewBuffer(void);

	void addLine(const wchar_t* lineString, bool newLine = true);
	void updateDisplay(bool isSeekToEnd = false, bool notifyParent = true);

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

	bool isCurrentlyDispHistory(void) const {
		return isCurrentlyDisplayHistory_;
	}
	void setCurrentlyDispHistoryState(bool state) {
		isCurrentlyDisplayHistory_ = state;
	}
	size_t getHistoryCurrentDispIndex(void) const {
		return currentHistoryIndex_;
	}
	void setHistoryCurrentDispIndex(size_t currentIndex) {
		currentHistoryIndex_ = currentIndex;
	}
};

