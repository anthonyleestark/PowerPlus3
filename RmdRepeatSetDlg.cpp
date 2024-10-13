
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		RmdRepeatSetDlg.cpp
//		Description:	Source file for Reminder message repeat mode set dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2024.10.02:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PowerPlus.h"
#include "RmdRepeatSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_WIDTH	320
#define DEFAULT_HEIGHT	240

using namespace CoreFuncs;


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CRmdRepeatSetDlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CRmdRepeatSetDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CRmdRepeatSetDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CRmdRepeatSetDlg::CRmdRepeatSetDlg() : SDialog(IDD_RMDREPEATSET_DLG)
{
	// Dialog control
	m_pSnoozeIntervalEdit = NULL;
	m_pSnoozeIntervalSpin = NULL;

	m_pRepeatEnableChk = NULL;
	m_pSnoozeEnableChk = NULL;
	m_pActiveMondayChk = NULL;
	m_pActiveTuesdayChk = NULL;
	m_pActiveWednesdayChk = NULL;
	m_pActiveThursdayChk = NULL;
	m_pActiveFridayChk = NULL;
	m_pActiveSaturdayChk = NULL;
	m_pActiveSundayChk = NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CRmdRepeatSetDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CRmdRepeatSetDlg::~CRmdRepeatSetDlg()
{
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void CRmdRepeatSetDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


//////////////////////////////////////////////////////////////////////////
//
//	CRmdRepeatSetDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CRmdRepeatSetDlg, SDialog)
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_RMDREPEATSET_SNOOZE_INTERVAL_SPIN, &CRmdRepeatSetDlg::OnSnoozeSpinChange)
	ON_COMMAND_RANGE(IDC_RMDREPEATSET_REPEAT_CHK, IDC_RMDREPEATSET_SNOOZE_CHK, &CRmdRepeatSetDlg::OnCheckboxClicked)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// CRmdRepeatSetDlg message handlers

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize Reminder repeat set dialog
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CRmdRepeatSetDlg::OnInitDialog()
{
	SDialog::OnInitDialog();

	// Setup display
	SetupLanguage();
	SetupDlgItemState();
	RefreshDlgItemState();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDestroy
//	Description:	Destroy dialog
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CRmdRepeatSetDlg::OnDestroy()
{
	EndDialog(IDCANCEL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnActivate
//	Description:	Default method for dialog activate message handling
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CRmdRepeatSetDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	SDialog::OnActivate(nState, pWndOther, bMinimized);

	// If dialog is inactivated
	if (nState == WA_INACTIVE) {
		// Hide the dialog itself
		this->ShowWindow(SW_HIDE);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSnoozeSpinChange
//	Description:	Update when snooze spin value changes
//  Arguments:		pNMDHR  - Default of spin event handler
//					pResult - Default of spin event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CRmdRepeatSetDlg::OnSnoozeSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// Get timespin position and convert to time value
	int nPos = pNMUpDown->iPos;
	SetSnoozeIntervalEdit(nPos);

	*pResult = NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCheckboxClicked
//	Description:	Handle clicked event for checkbox-es
//  Arguments:		nID - ID of checkbox
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CRmdRepeatSetDlg::OnCheckboxClicked(UINT nID)
{
	RefreshDlgItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog controls
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CRmdRepeatSetDlg::SetupLanguage()
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		UINT nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDC_RMDREPEATSET_DUMMY_BORDER:
		case IDC_RMDREPEATSET_DETAILS_STATIC:
		case IDC_RMDREPEATSET_ACTIVEDAYS_STATIC:
		case IDC_RMDREPEATSET_SNOOZE_INTERVAL_EDIT:
		case IDC_RMDREPEATSET_SNOOZE_INTERVAL_SPIN:
			// Skip these items
			break;
		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupDlgItemState
//	Description:	Setup properties and values for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CRmdRepeatSetDlg::SetupDlgItemState()
{
	// Initialize dialog items
	if (m_pSnoozeIntervalEdit == NULL) {
		m_pSnoozeIntervalEdit = (CEdit*)GetDlgItem(IDC_RMDREPEATSET_SNOOZE_INTERVAL_EDIT);
		if (m_pSnoozeIntervalEdit == NULL) {
			TRCFFMT(__FUNCTION__, "Snooze interval edit control not found");
			return;
		}
	}
	if (m_pSnoozeIntervalSpin == NULL) {
		m_pSnoozeIntervalSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_RMDREPEATSET_SNOOZE_INTERVAL_SPIN);
		if (m_pSnoozeIntervalSpin == NULL) {
			TRCFFMT(__FUNCTION__, "Snooze interval spin control not found");
			return;
		}
	}
	if (m_pRepeatEnableChk == NULL) {
		m_pRepeatEnableChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_REPEAT_CHK);
		if (m_pRepeatEnableChk == NULL) {
			TRCFFMT(__FUNCTION__, "Repeat enable checkbox not found");
			return;
		}
	}
	if (m_pSnoozeEnableChk == NULL) {
		m_pSnoozeEnableChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_SNOOZE_CHK);
		if (m_pSnoozeEnableChk == NULL) {
			TRCFFMT(__FUNCTION__, "Snooze enable checkbox not found");
			return;
		}
	}
	if (m_pActiveMondayChk == NULL) {
		m_pActiveMondayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_MONDAY_CHK);
		if (m_pActiveMondayChk == NULL) {
			TRCFFMT(__FUNCTION__, "Monday active checkbox not found");
			return;
		}
	}
	if (m_pActiveTuesdayChk == NULL) {
		m_pActiveTuesdayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_TUESDAY_CHK);
		if (m_pActiveTuesdayChk == NULL) {
			TRCFFMT(__FUNCTION__, "Tuesday active checkbox not found");
			return;
		}
	}
	if (m_pActiveWednesdayChk == NULL) {
		m_pActiveWednesdayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_WEDNESDAY_CHK);
		if (m_pActiveWednesdayChk == NULL) {
			TRCFFMT(__FUNCTION__, "Wednesday active checkbox not found");
			return;
		}
	}
	if (m_pActiveThursdayChk == NULL) {
		m_pActiveThursdayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_THURSDAY_CHK);
		if (m_pActiveThursdayChk == NULL) {
			TRCFFMT(__FUNCTION__, "Thursday active checkbox not found");
			return;
		}
	}
	if (m_pActiveFridayChk == NULL) {
		m_pActiveFridayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_FRIDAY_CHK);
		if (m_pActiveFridayChk == NULL) {
			TRCFFMT(__FUNCTION__, "Friday active checkbox not found");
			return;
		}
	}
	if (m_pActiveSaturdayChk == NULL) {
		m_pActiveSaturdayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_SATURDAY_CHK);
		if (m_pActiveSaturdayChk == NULL) {
			TRCFFMT(__FUNCTION__, "Saturday active checkbox not found");
			return;
		}
	}
	if (m_pActiveSundayChk == NULL) {
		m_pActiveSundayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_SUNDAY_CHK);
		if (m_pActiveSundayChk == NULL) {
			TRCFFMT(__FUNCTION__, "Sunday active checkbox not found");
			return;
		}
	}

	// Setup properties
	int nDefaultSnoozeMin = DEF_PWRREMINDER_DEFAULT_SNOOZE / 60;
	if (m_pSnoozeIntervalEdit != NULL) {
		m_pSnoozeIntervalEdit->SetReadOnly(TRUE);
	}
	if (m_pSnoozeIntervalSpin != NULL) {
		if (m_pSnoozeIntervalEdit == NULL) return;

		// Set buddy: Snooze interval edit control
		m_pSnoozeIntervalSpin->SetBuddy(m_pSnoozeIntervalEdit);
		m_pSnoozeIntervalSpin->SetRange(DEF_SPINCTRL_SNOOZEMINPOS, DEF_SPINCTRL_SNOOZEMAXPOS);
		m_pSnoozeIntervalSpin->SetPos(nDefaultSnoozeMin);
		SetSnoozeIntervalEdit(nDefaultSnoozeMin);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RefreshDlgItemState
//	Description:	Refresh and update state for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CRmdRepeatSetDlg::RefreshDlgItemState()
{
	int nRepeatState = DEF_INTEGER_NULL;
	int nSnoozeState = DEF_INTEGER_NULL;

	// Update checkbox checked state
	if (m_pRepeatEnableChk != NULL) {
		nRepeatState = m_pRepeatEnableChk->GetCheck();

		// Enable/disable all other sub-controls
		BOOL bEnable = (nRepeatState == 1) ? TRUE : FALSE;
		if (m_pSnoozeEnableChk != NULL) {
			m_pSnoozeEnableChk->EnableWindow(bEnable);
		}
		if (m_pActiveMondayChk != NULL) {
			m_pActiveMondayChk->EnableWindow(bEnable);
		}
		if (m_pActiveTuesdayChk != NULL) {
			m_pActiveTuesdayChk->EnableWindow(bEnable);
		}
		if (m_pActiveWednesdayChk != NULL) {
			m_pActiveWednesdayChk->EnableWindow(bEnable);
		}
		if (m_pActiveThursdayChk != NULL) {
			m_pActiveThursdayChk->EnableWindow(bEnable);
		}
		if (m_pActiveFridayChk != NULL) {
			m_pActiveFridayChk->EnableWindow(bEnable);
		}
		if (m_pActiveSaturdayChk != NULL) {
			m_pActiveSaturdayChk->EnableWindow(bEnable);
		}
		if (m_pActiveSundayChk != NULL) {
			m_pActiveSundayChk->EnableWindow(bEnable);
		}

	}
	if (m_pSnoozeEnableChk != NULL) {
		nSnoozeState = m_pSnoozeEnableChk->GetCheck();

		// Enable/disable snooze controls
		if ((m_pSnoozeIntervalEdit != NULL) && (m_pSnoozeIntervalSpin != NULL)) {
			if ((nRepeatState == 1) && (nSnoozeState == 1)) {
				m_pSnoozeIntervalEdit->EnableWindow(TRUE);
				m_pSnoozeIntervalSpin->EnableWindow(TRUE);
			}
			else {
				m_pSnoozeIntervalEdit->EnableWindow(FALSE);
				m_pSnoozeIntervalSpin->EnableWindow(FALSE);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetSnoozeIntervalEdit
//	Description:	Set value to snooze interval edit control
//  Arguments:		nValue - Value to set (in minutes)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CRmdRepeatSetDlg::SetSnoozeIntervalEdit(int nValue)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Get format string
	CString strFormat = GetLanguageString(pAppLang, PWRRMD_REPEATSET_SNOOZEINTERVAL);
	if (!_tcscmp(strFormat, DEF_STRING_NULL)) return;

	// Check validity
	if (((nValue * 60) < DEF_PWRREMINDER_MIN_SNOOZE) ||
		((nValue * 60) > DEF_PWRREMINDER_MAX_SNOOZE))
		return;

	// Show snooze interval value
	CString strSnoozeInterval;
	strSnoozeInterval.Format(strFormat, nValue);
	if (m_pSnoozeIntervalEdit != NULL) {
		// Display
		m_pSnoozeIntervalEdit->SetWindowText(strSnoozeInterval);
		if (m_pSnoozeIntervalSpin != NULL) {
			// Update spin position
			if (m_pSnoozeIntervalSpin->GetPos() != nValue) {
				m_pSnoozeIntervalSpin->SetPos(nValue);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateDialogData
//	Description:	Update repeat set data from/to dialog controls
//  Arguments:		pwrItemData - Power Reminder item
//					bUpdate		- Update data flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CRmdRepeatSetDlg::UpdateDialogData(PWRREMINDERITEM& pwrItemData, BOOL bUpdate)
{
	// Get repeat set data
	RMDREPEATSET& rpsRepeatData = pwrItemData.rpsRepeatSet;

	if (bUpdate == TRUE) {

		/***************************************************************/
		/*															   */
		/*				Update data from dialog controls			   */
		/*															   */
		/***************************************************************/

		// Repeat enable
		int nState = 0;
		if (m_pRepeatEnableChk != NULL) {
			nState = m_pRepeatEnableChk->GetCheck();
			rpsRepeatData.bRepeat = (nState == 1) ? TRUE : FALSE;
		}

		// Snooze enable
		if (m_pSnoozeEnableChk != NULL) {
			nState = m_pSnoozeEnableChk->GetCheck();
			rpsRepeatData.bAllowSnooze = (nState == 1) ? TRUE : FALSE;

			if (m_pSnoozeIntervalSpin != NULL) {
				int nSnoozeMinute = m_pSnoozeIntervalSpin->GetPos();
				rpsRepeatData.nSnoozeInterval = nSnoozeMinute * 60;
			}
		}

		// Repeat active status for days of week
		// Note: Read and store data from Sunday first

		BYTE byRepeatDays = 0;
		{
			BOOL bSundayEnable = FALSE;
			if (m_pActiveSundayChk != NULL) {
				nState = m_pActiveSundayChk->GetCheck();
				bSundayEnable = (nState == 1) ? TRUE : FALSE;
			}
			byRepeatDays |= bSundayEnable << SUNDAY;

			BOOL bMondayEnable = FALSE;
			if (m_pActiveMondayChk != NULL) {
				nState = m_pActiveMondayChk->GetCheck();
				bMondayEnable = (nState == 1) ? TRUE : FALSE;
			}
			byRepeatDays |= bMondayEnable << MONDAY;

			BOOL bTuesdayEnable = FALSE;
			if (m_pActiveTuesdayChk != NULL) {
				nState = m_pActiveTuesdayChk->GetCheck();
				bTuesdayEnable = (nState == 1) ? TRUE : FALSE;
			}
			byRepeatDays |= bTuesdayEnable << TUESDAY;

			BOOL bWednesdayEnable = FALSE;
			if (m_pActiveWednesdayChk != NULL) {
				nState = m_pActiveWednesdayChk->GetCheck();
				bWednesdayEnable = (nState == 1) ? TRUE : FALSE;
			}
			byRepeatDays |= bWednesdayEnable << WEDNESDAY;

			BOOL bThursdayEnable = FALSE;
			if (m_pActiveThursdayChk != NULL) {
				nState = m_pActiveThursdayChk->GetCheck();
				bThursdayEnable = (nState == 1) ? TRUE : FALSE;
			}
			byRepeatDays |= bThursdayEnable << THURSDAY;

			BOOL bFridayEnable = FALSE;
			if (m_pActiveFridayChk != NULL) {
				nState = m_pActiveFridayChk->GetCheck();
				bFridayEnable = (nState == 1) ? TRUE : FALSE;
			}
			byRepeatDays |= bFridayEnable << FRIDAY;

			BOOL bSaturdayEnable = FALSE;
			if (m_pActiveSaturdayChk != NULL) {
				nState = m_pActiveSaturdayChk->GetCheck();
				bSaturdayEnable = (nState == 1) ? TRUE : FALSE;
			}
			byRepeatDays |= bSaturdayEnable << SATURDAY;
		}
		rpsRepeatData.byRepeatDays = byRepeatDays;
	}
	else {

		/***************************************************************/
		/*															   */
		/*				  Bind data to dialog controls				   */
		/*															   */
		/***************************************************************/

		int nState = 0;

		// Repeat enable
		if (m_pRepeatEnableChk != NULL) {
			nState = (rpsRepeatData.bRepeat) ? 1 : 0;
			m_pRepeatEnableChk->SetCheck(nState);
		}

		// Snooze enable
		if (m_pSnoozeEnableChk != NULL) {
			nState = (rpsRepeatData.bAllowSnooze) ? 1 : 0;
			m_pSnoozeEnableChk->SetCheck(nState);
			
			if (m_pSnoozeIntervalSpin != NULL) {
				int nSnoozeMinute = rpsRepeatData.nSnoozeInterval / 60;
				m_pSnoozeIntervalSpin->SetPos(nSnoozeMinute);
				SetSnoozeIntervalEdit(nSnoozeMinute);
			}
		}

		// Repeat active status for days of week
		// Note: Bind data from Monday first

		if (m_pActiveMondayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(MONDAY)) ? 1 : 0;
			m_pActiveMondayChk->SetCheck(nState);
		}
		if (m_pActiveTuesdayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(TUESDAY)) ? 1 : 0;
			m_pActiveTuesdayChk->SetCheck(nState);
		}
		if (m_pActiveWednesdayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(WEDNESDAY)) ? 1 : 0;
			m_pActiveWednesdayChk->SetCheck(nState);
		}
		if (m_pActiveThursdayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(THURSDAY)) ? 1 : 0;
			m_pActiveThursdayChk->SetCheck(nState);
		}
		if (m_pActiveFridayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(FRIDAY)) ? 1 : 0;
			m_pActiveFridayChk->SetCheck(nState);
		}
		if (m_pActiveSaturdayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(SATURDAY)) ? 1 : 0;
			m_pActiveSaturdayChk->SetCheck(nState);
		}
		if (m_pActiveSundayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(SUNDAY)) ? 1 : 0;
			m_pActiveSundayChk->SetCheck(nState);
		}

		// Update data
		UpdateData(FALSE);
		RefreshDlgItemState();
	}
}
