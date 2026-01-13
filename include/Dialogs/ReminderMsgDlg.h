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
	CReminderMsgDlg(CWnd *parentWnd = NULL);	// constructor
	~CReminderMsgDlg();							// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMINDERMSG_DLG };
#endif

private:
	// Message string buffer
	String bufferString_;

	// Message font & icon
	CFont*	messageFontPtr_;
	HICON	messageIconHandle_;
	Size	iconSize_;

	// Message style set
	RmdMsgStyleSet messageStyleData_;

	// Flags
	bool isTimerSet_;
	bool isDisplayIcon_;
	bool isLockDialogSize_;
	bool isLockFontSize_;
	bool isSnoozingAllowed_;
	int	 snoozeFlag_;

	// Properties
	unsigned autoCloseInterval_;

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
	afx_msg void OnTimer(UINT_PTR eventId);
	afx_msg int OnCreate(LPCREATESTRUCT createStructPtr);
	afx_msg void OnSysCommand(UINT id, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* windowPtr, UINT nCtlColor);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	// Get/set display content
	inline const wchar_t* getDispMessage(void) const {
		return bufferString_.getString();
	}
	inline void setDispMessage(const wchar_t* dispMsg) {
		bufferString_ = dispMsg;
	}

	// Get/set message style
	inline void getMessageStyle(RmdMsgStyleSet& messageStyle) const {
		messageStyle.copy(messageStyleData_);
	}
	inline void setMessageStyle(const RmdMsgStyleSet& messageStyle) {
		messageStyleData_.copy(messageStyle);
	}
	
	// Auto-close message
	inline unsigned getAutoCloseInterval(void) const {
		return autoCloseInterval_;
	}
	inline void setAutoCloseInterval(unsigned nSeconds) {
		autoCloseInterval_ = nSeconds;
	}

	// Dialog size
	void setSize(Size regSize) {
		SDialog::setSize(regSize);
		isLockDialogSize_ = true;
	}
	void setSize(long width, long height) {
		SDialog::setSize(width, height);
		isLockDialogSize_ = true;
	}

	// Other properties
	inline bool getAllowSnoozeMode(void) const {
		return isSnoozingAllowed_;
	}
	inline void setAllowSnoozeMode(bool value) {
		isSnoozingAllowed_ = value;
	}

	// Flags
	inline void getSnoozeTriggerFlag(int& value) const {
		value = snoozeFlag_;
	}
	inline void setSnoozeTriggerFLag(int value) {
		snoozeFlag_ = value;
	}

protected:
	// Initialize message style
	bool initMessageStyle(void);

	// Calculate icon position
	bool calcMsgIconPosition(Point& iconPosition) const;

	// Move the dialog to specific display position
	void moveToDisplayPosition(MsgDispPosition displayPosition);

	// Convert text and client rectangle
	void clientToText(Rect& rect) const;
	void textToClient(Rect& rect) const;
};

