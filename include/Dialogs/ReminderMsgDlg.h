
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		ReminderMsgDlg.h
//		Description:	Header file for Reminder Message dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2024.08.12:		Create new
//						<1> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REMINDERMSGDLG_H_INCLUDED
#define _REMINDERMSGDLG_H_INCLUDED

#include "AppCore\AppCore.h"
#include "AppCore\Global.h"
#include "Framework\SDialog.h"


////////////////////////////////////////////////////////
//
//	CReminderMsgDlg dialog used for Reminder Message
//
////////////////////////////////////////////////////////

class CReminderMsgDlg : public SDialog
{
	DECLARE_DYNAMIC(CReminderMsgDlg)

public:
	CReminderMsgDlg(CWnd *pParentWnd = NULL);	// constructor
	~CReminderMsgDlg();							// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMINDERMSG_DLG };
#endif

private:
	// Message string buffer
	CString m_strBuffer;

	// Message font
	CFont*	m_pMsgFont;
	CString	m_strMsgFontName;
	float	m_fMsgFontPoint;

	// Message icon
	HICON	m_hMsgIcon;
	BOOL	m_bDispIcon;
	BYTE	m_byIconPlacement;
	CSize	m_szIconSize;

	// Flags
	BOOL m_bTimerSet;
	BOOL m_bLockDlgSize;
	BOOL m_bLockFontSize;
	BOOL m_bAllowSnooze;
	int	 m_nSnoozeFlag;

	// Properties
	UINT m_nAutoCloseInterval;

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
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// Setup displaying content and properties

	// Display content
	virtual LPCTSTR GetDispMessage(void) const;
	virtual void SetDispMessage(LPCTSTR lpszDispMsg);
	
	// Auto-close message
	virtual UINT GetAutoCloseInterval(void) const;
	virtual void SetAutoCloseInterval(UINT nSeconds);

	// Dialog size
	virtual void SetSize(CSize szRegSize);
	virtual void SetSize(LONG lWidth, LONG lHeight);

	// Message font
	virtual void GetMsgFontName(CString& strFontName) const;
	virtual void GetMsgFontPoint(float& fFontPoint) const;
	virtual void SetMsgFont(LPCTSTR lpszFontName, float fFontPoint);

	// Icon and margin
	virtual void SetMsgIcon(UINT nIconID, int nIconSqrSize);
	virtual void SetMsgIconPlacement(BYTE byPlacement);

	// Other properties
	virtual BOOL GetAllowSnoozeMode(void) const;
	virtual void SetAllowSnoozeMode(BOOL bValue);

	// Flags
	virtual void GetSnoozeTriggerFlag(int& nValue) const;
	virtual void SetSnoozeTriggerFLag(int nValue);

protected:
	// Calculate icon position
	BOOL CalcMsgIconPosition(LPPOINT lpptIcon) const;

	// Convert text and client rectangle
	void ClientToText(LPRECT lpRect) const;
	void TextToClient(LPRECT lpRect) const;
};


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _REMINDERMSGDLG_ENABLE_INLINES
	#include "Dialogs.inl"
	#ifdef _REMINDERMSGDLG_INLINE_INCLUDED
		#pragma message("-- Dialogs inline library included (ReminderMsgDlg.h)")
	#else
		#pragma error("-- Linking error in ReminderMsgDlg.h: Unable to link to inline header!")
	#endif
	#undef _REMINDERMSGDLG_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in ReminderMsgDlg.h: Inline is not enabled!")
#endif

#endif	// ifndef _REMINDERMSGDLG_H_INCLUDED
