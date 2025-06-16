/**
 * @file		ReminderMsgDlg.h
 * @brief		Header file for Reminder Message dialog
 * @author		AnthonyLeeStark
 * @date		2024.08.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/Global.h"
#include "Framework/SDialog.h"


// Reminder message dialog class
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
	String m_strBuffer;

	// Message font & icon
	CFont*	m_pMsgFont;
	HICON	m_hMsgIcon;
	Size	m_szIconSize;

	// Message style set
	RmdMsgStyleSet m_rmdMsgStyleSet;

	// Flags
	bool m_bTimerSet;
	bool m_bDispIcon;
	bool m_bLockDlgSize;
	bool m_bLockFontSize;
	bool m_bAllowSnooze;
	int	 m_nSnoozeFlag;

	// Properties
	unsigned m_nAutoCloseInterval;

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

public:
	// Get/set display content
	virtual const wchar_t* GetDispMessage(void) const {
		return m_strBuffer.GetString();
	};
	virtual void SetDispMessage(const wchar_t* dispMsg) {
		m_strBuffer = dispMsg;
	};

	// Get/set message style
	virtual void GetMessageStyle(RmdMsgStyleSet& rmdMsgStyle) const {
		rmdMsgStyle.Copy(m_rmdMsgStyleSet);
	};
	virtual void SetMessageStyle(const RmdMsgStyleSet& rmdMsgStyle) {
		m_rmdMsgStyleSet.Copy(rmdMsgStyle);
	};
	
	// Auto-close message
	virtual unsigned GetAutoCloseInterval(void) const {
		return m_nAutoCloseInterval;
	};
	virtual void SetAutoCloseInterval(unsigned nSeconds) {
		m_nAutoCloseInterval = nSeconds;
	};

	// Dialog size
	virtual void SetSize(Size regSize) {
		SDialog::SetSize(regSize);
		m_bLockDlgSize = true;
	};
	virtual void SetSize(long lWidth, long lHeight) {
		SDialog::SetSize(lWidth, lHeight);
		m_bLockDlgSize = true;
	};

	// Other properties
	virtual bool GetAllowSnoozeMode(void) const {
		return m_bAllowSnooze;
	};
	virtual void SetAllowSnoozeMode(bool bValue) {
		m_bAllowSnooze = bValue;
	};

	// Flags
	virtual void GetSnoozeTriggerFlag(int& nValue) const {
		nValue = m_nSnoozeFlag;
	};
	virtual void SetSnoozeTriggerFLag(int nValue) {
		m_nSnoozeFlag = nValue;
	};

protected:
	// Initialize message style
	bool InitMessageStyle(void);

	// Calculate icon position
	bool CalcMsgIconPosition(Point& iconPosition) const;

	// Convert text and client rectangle
	void ClientToText(Rect& rect) const;
	void TextToClient(Rect& rect) const;
};

