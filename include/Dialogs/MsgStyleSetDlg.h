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
	enum class DataSetFlag { commonStyle, customStyle };

public:
	CRmdMsgStyleSetDlg();		// constructor
	~CRmdMsgStyleSetDlg();		// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSGSTYLESET_DLG };
#endif

private:
	// Background/Text color settings
	CButton*	backgroundColorPickBtnPtr_;
	CButton*	textColorPickBtnPtr_;

	// Font settings
	CComboBox*	fontNamePickComboPtr_;
	CComboBox*	fontSizePickComboPtr_;

	// Icon settings
	CComboBox*	iconIdPickComboPtr_;
	CEdit*		iconSizeEditPtr_;
	CButton*	iconPositionOnTopRadPtr_;
	CButton*	iconPositionOnLeftRadPtr_;
	CStatic*	iconPreviewStaticPtr_;

	// Timeout settings
	CEdit*			 timeoutEditPtr_;
	CSpinButtonCtrl* timeoutSpinPtr_;

	// Display position setting
	CComboBox* displayPosComboPtr_;

	// Margin settings
	CEdit* horizontalMarginEditPtr_;
	CEdit* verticalMarginEditPtr_;

	// Data
	RmdMsgStyleSet messageStyleData_;
	RmdMsgStyleSet tempMessageStyleData_;
	COLORREF	   messageBackgroundColor_;
	COLORREF	   messageTextColor_;
	DataSetFlag	   flagDataSet_;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

public:
	// Generated message handle functions
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnCancel();
	afx_msg void OnPreview();
	afx_msg void OnReset();
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Dialog and items setup functions
	virtual void setupLanguage(void);
	virtual void setupComboBox(unsigned comboId, LANGTABLE_PTR languageTablePtr);
	virtual void setupDialogItemState(void);
	virtual void refreshDialogItemState(bool isRecheckState = false);

	// Displaying functions
	void redrawIconPreview(LPDRAWITEMSTRUCT drawItemStructPtr);
	int  getEditValue(HWND editCtrlHandle, unsigned maxDigits);
	bool validateEditValue(HWND editCtrlHandle, int& inputVal, int minVal, int maxVal, bool showWarning = true);

public:
	// Data processing functions
	void updateDialogData(bool updateFlag);
	void getData(RmdMsgStyleSet& msgStyleData) const {
		msgStyleData.copy(messageStyleData_);
	}
	void setData(const RmdMsgStyleSet& msgStyleData) {
		messageStyleData_.copy(msgStyleData);
		tempMessageStyleData_.copy(messageStyleData_);
	}
	DataSetFlag getDataFlag(void) const {
		return flagDataSet_;
	}
	void setDataFlag(DataSetFlag flag) {
		flagDataSet_ = flag;
	}
	bool validateAndCorrect(int& inputVal, int minVal, int maxVal) {
		if (maxVal < minVal) std::swap(minVal, maxVal);
		if (inputVal >= minVal && inputVal <= maxVal) return true;
		if (inputVal < minVal) inputVal = minVal;
		else if (inputVal > maxVal) inputVal = maxVal;
		return false;
	}
};
