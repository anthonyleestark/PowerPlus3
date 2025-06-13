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
	BOOL		 m_bCurDispHistory;
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
	virtual BOOL SendDebugCommand(void);

private:
	// Member functions
	CEdit* GetDebugEditView(void) const;
	BOOL InitDebugEditView(UINT nCtrlID);
	BOOL CreateDebugViewFont(void);
	BOOL CreateDebugViewBrush(void);

	inline BOOL IsDebugEditViewValid(void);
	inline BOOL IsDebugEditViewFocus(void);

	int GetCaretPosition(void);
	BOOL ShowDebugTestEditViewMenu(void);

	inline void BackupDebugViewBuffer(void);
	int  FormatDebugCommand(String &debugCommand);
	void ClearViewBuffer(void);

	void AddLine(const wchar_t* lineString, BOOL bNewLine = TRUE);
	void UpdateDisplay(BOOL bSeekToEnd = FALSE, BOOL bNotifyParent = TRUE);

	size_t AddDebugCommandHistory(const wchar_t* commandString);
	void DispDebugCommandHistory(int nHistoryIndex);
	void ClearDebugCommandHistory(void);
	size_t GetDebugCommandHistoryCount(void) const;
	BOOL IsDebugCommandHistoryEmpty(void) const;

	inline BOOL IsCurrentlyDispHistory(void) const;
	inline void SetCurrentlyDispHistoryState(BOOL bState);
	inline size_t GetHistoryCurrentDispIndex(void) const;
	inline void SetHistoryCurrentDispIndex(size_t nCurIndex);
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
