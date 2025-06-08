/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		DebugTestV2Dlg.h
//		Description:	Header file for DebugTestV2 dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2025.06.02:		Create new for version 3.2
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/Global.h"
#include "Framework/SDialog.h"


////////////////////////////////////////////////////////
//
//	CDebugTestV2Dlg dialog used for DebugTest dialog
//
////////////////////////////////////////////////////////

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
	CString m_strBuffer;
	CString m_strBackupBuffer;
	CString m_strCommandBuffer;

	// Specific flags
	BOOL m_bDispCommandPrefix;

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
	afx_msg LRESULT OnDebugOutput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugCmdNoReply(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDebugViewClear(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowDialog(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL SendDebugCommand(void);

private:
	// Member functions
	CEdit* GetDebugView(void) const;
	CEdit* GetDebugCommandInput(void) const;

	BOOL InitDebugScreen(void);
	BOOL CreateDebugScreenFont(void);
	BOOL CreateDebugScreenBrush(void);

	BOOL IsDebugScreenValid(void);
	BOOL IsDebugViewValid(void);
	BOOL IsDebugCommandInputValid(void);
	BOOL IsDebugCommandInputFocused(void);

	BOOL RefreshDebugScreen(int nFlag);
	BOOL ShowDebugScreenContextMenu(void);

	int  FormatDebugCommand(CString& strDebugCommand);
	void ClearDebugCommandInput(const wchar_t* commandBuff = Constant::String::Empty);
	void BackupDebugViewBuffer(void);
	void ClearDebugViewBuffer(void);

	void AddLine(const wchar_t* lpszString, BOOL bNewLine = TRUE);
	void UpdateDisplay(BOOL bSeekToEnd = FALSE, BOOL bNotifyParent = TRUE);

	size_t AddDebugCommandHistory(const wchar_t* commandString);
	void DispDebugCommandHistory(int nHistoryIndex);
	void ClearDebugCommandHistory(void);
	size_t GetDebugCommandHistoryCount(void) const;
	BOOL IsDebugCommandHistoryEmpty(void) const;

	BOOL IsCurrentlyDispHistory(void) const;
	void SetCurrentlyDispHistoryState(BOOL bState);
	size_t GetHistoryCurrentDispIndex(void) const;
	void SetHistoryCurrentDispIndex(size_t nCurIndex);
};


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _DEBUGTESTV2DLG_ENABLE_INLINES
	#include "Dialogs.inl"
	#ifdef _DEBUGTESTV2DLG_INLINE_INCLUDED
		#pragma message("-- Dialogs inline library included (DebugTestV2Dlg.h)")
	#else
		#pragma error("-- Linking error in DebugTestV2Dlg.h: Unable to link to inline header!")
	#endif
	#undef _DEBUGTESTV2DLG_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in DebugTestV2Dlg.h: Inline is not enabled!")
#endif
