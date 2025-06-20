/**
 * @file		MsgStyleSetDlg.h
 * @brief		Header file for Reminder message style set dialog
 * @author		AnthonyLeeStark
 * @date		2025.06.17
 *
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/Global.h"
#include "Framework/SDialog.h"


 // RmdMsgStyleSet dialog class
class CRmdMsgStyleSetDlg : public SDialog
{
	DECLARE_DYNAMIC(CRmdMsgStyleSetDlg)

public:
	CRmdMsgStyleSetDlg();		// constructor
	~CRmdMsgStyleSetDlg();		// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSGSTYLESET_DLG };
#endif

private:
	// Background/Text color settings
	CButton*	m_pBkgrdColorPickBtn;
	CButton*	m_pTextColorPickBtn;

	// Font settings
	CComboBox*	m_pFontNamePickCombo;
	CComboBox*	m_pFontSizePickCombo;

	// Icon settings
	CComboBox*	m_pIconIDPickCombo;
	CEdit*		m_pIconSizeEdit;
	CButton*	m_pIconPositionOnTopRad;
	CButton*	m_pIconPositionOnLeftRad;
	CStatic*	m_pIconPreviewStatic;

	// Timeout settings
	CEdit*			 m_pTimeoutEdit;
	CSpinButtonCtrl* m_pTimeoutSpin;

	// Display position setting
	CComboBox* m_pDisplayPosCombo;

	// Margin settings
	CEdit* m_pHorizontalMarginEdit;
	CEdit* m_pVerticalMarginEdit;

	// Data
	RmdMsgStyleSet m_rmsMsgStyleData;
	RmdMsgStyleSet m_rmsMsgStyleTemp;
	COLORREF	   m_clrMsgBackground;
	COLORREF	   m_clrMsgText;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

public:
	// Generated message handle functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnApply();
	afx_msg void OnCancel();
	afx_msg void OnPreview();
	afx_msg void OnReset();
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Dialog and items setup functions
	virtual void SetupLanguage(void);
	virtual void SetupComboBox(unsigned nComboID, LANGTABLE_PTR pLanguage);
	virtual void SetupDialogItemState(void);
	virtual void RefreshDialogItemState(bool bRecheckState = false);

public:
	// Data processing functions
	void UpdateDialogData(bool bUpdate);
	void GetData(RmdMsgStyleSet& msgStyleData) const {
		msgStyleData.Copy(m_rmsMsgStyleData);
	};
	void SetData(const RmdMsgStyleSet& msgStyleData) {
		m_rmsMsgStyleData.Copy(msgStyleData);
		m_rmsMsgStyleTemp.Copy(m_rmsMsgStyleData);
	};
};
