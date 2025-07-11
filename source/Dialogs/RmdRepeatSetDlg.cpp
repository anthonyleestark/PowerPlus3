﻿/**
 * @file		RmdRepeatSetDlg.cpp
 * @brief		Source file for Reminder message repeat mode set dialog
 * @author		AnthonyLeeStark
 * @date		2024.10.02
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "MainApp/PowerPlus.h"
#include "Dialogs/RmdRepeatSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;


// Dialog default size
constexpr const int defaultWidth = 320;
constexpr const int defaultHeight = 240;


// Implement methods for CRmdRepeatSetDlg
IMPLEMENT_DYNAMIC(CRmdRepeatSetDlg, SDialog)


/**
 * @brief	Constructor
 */
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

	// Member value
	m_nSnoozeInterval = 0;
}

/**
 * @brief	Destructor
 */
CRmdRepeatSetDlg::~CRmdRepeatSetDlg()
{
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CRmdRepeatSetDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}


// CRmdRepeatSetDlg dialog message map
BEGIN_MESSAGE_MAP(CRmdRepeatSetDlg, SDialog)
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_RMDREPEATSET_SNOOZE_INTERVAL_SPIN, &CRmdRepeatSetDlg::OnSnoozeSpinChange)
	ON_COMMAND_RANGE(IDC_RMDREPEATSET_REPEAT_CHK, IDC_RMDREPEATSET_SNOOZE_CHK, &CRmdRepeatSetDlg::OnCheckboxClicked)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// CRmdRepeatSetDlg message handlers

/**
 * @brief	Initialize Reminder repeat set dialog
 * @param	None
 * @return	BOOL - Default
 */
BOOL CRmdRepeatSetDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Setup display
	SetupLanguage();
	SetupDialogItemState();
	RefreshDialogItemState();

	return true;
}

/**
 * @brief	Destroy dialog
 * @param	None
 * @return	None
 */
void CRmdRepeatSetDlg::OnDestroy()
{
	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	Default method for dialog activate message handling
 * @param	Default
 * @return	None
 */
void CRmdRepeatSetDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	// Default
	SDialog::OnActivate(nState, pWndOther, bMinimized);

	// If dialog is inactivated
	if (nState == WA_INACTIVE) {
		// Hide the dialog itself
		this->ShowWindow(SW_HIDE);
	}
}

/**
 * @brief	Update when snooze spin value changes
 * @param	pNMDHR  - Default of spin event handler
 * @param	pResult - Default of spin event handler
 * @return	None
 */
void CRmdRepeatSetDlg::OnSnoozeSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// Get timespin position
	int nPos = pNMUpDown->iPos;

	// Set snooze interval value
	SetSnoozeInterval(nPos * 60);

	// Convert and set edit value
	SetSnoozeIntervalEdit(nPos);

	*pResult = NULL;
}

/**
 * @brief	Handle clicked event for checkbox-es
 * @param	nID - ID of checkbox
 * @return	None
 */
void CRmdRepeatSetDlg::OnCheckboxClicked(UINT /*nID*/)
{
	RefreshDialogItemState();
}

/**
 * @brief	Setup language for dialog controls
 * @param	None
 * @return	None
 */
void CRmdRepeatSetDlg::SetupLanguage()
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		unsigned nID = pWndChild->GetDlgCtrlID();

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

	// Default
	SDialog::SetupLanguage();
}

/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void CRmdRepeatSetDlg::SetupDialogItemState()
{
	// Initialize dialog items
	if (m_pSnoozeIntervalEdit == NULL) {
		m_pSnoozeIntervalEdit = (CEdit*)GetDlgItem(IDC_RMDREPEATSET_SNOOZE_INTERVAL_EDIT);
		if (m_pSnoozeIntervalEdit == NULL) {
			TRACE_ERROR("Error: Snooze interval edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pSnoozeIntervalSpin == NULL) {
		m_pSnoozeIntervalSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_RMDREPEATSET_SNOOZE_INTERVAL_SPIN);
		if (m_pSnoozeIntervalSpin == NULL) {
			TRACE_ERROR("Error: Snooze interval spin control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pRepeatEnableChk == NULL) {
		m_pRepeatEnableChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_REPEAT_CHK);
		if (m_pRepeatEnableChk == NULL) {
			TRACE_ERROR("Error: Repeat enable checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pSnoozeEnableChk == NULL) {
		m_pSnoozeEnableChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_SNOOZE_CHK);
		if (m_pSnoozeEnableChk == NULL) {
			TRACE_ERROR("Error: Snooze enable checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pActiveMondayChk == NULL) {
		m_pActiveMondayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_MONDAY_CHK);
		if (m_pActiveMondayChk == NULL) {
			TRACE_ERROR("Error: Monday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pActiveTuesdayChk == NULL) {
		m_pActiveTuesdayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_TUESDAY_CHK);
		if (m_pActiveTuesdayChk == NULL) {
			TRACE_ERROR("Error: Tuesday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pActiveWednesdayChk == NULL) {
		m_pActiveWednesdayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_WEDNESDAY_CHK);
		if (m_pActiveWednesdayChk == NULL) {
			TRACE_ERROR("Error: Wednesday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pActiveThursdayChk == NULL) {
		m_pActiveThursdayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_THURSDAY_CHK);
		if (m_pActiveThursdayChk == NULL) {
			TRACE_ERROR("Error: Thursday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pActiveFridayChk == NULL) {
		m_pActiveFridayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_FRIDAY_CHK);
		if (m_pActiveFridayChk == NULL) {
			TRACE_ERROR("Error: Friday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pActiveSaturdayChk == NULL) {
		m_pActiveSaturdayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_SATURDAY_CHK);
		if (m_pActiveSaturdayChk == NULL) {
			TRACE_ERROR("Error: Saturday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pActiveSundayChk == NULL) {
		m_pActiveSundayChk = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_SUNDAY_CHK);
		if (m_pActiveSundayChk == NULL) {
			TRACE_ERROR("Error: Sunday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Setup properties
	int nDefaultSnoozeMin = PwrRepeatSet::defaultSnoozeInterval / 60;
	if (m_pSnoozeIntervalEdit != NULL) {
		m_pSnoozeIntervalEdit->SetReadOnly(true);
	}
	if (m_pSnoozeIntervalSpin != NULL) {
		if (m_pSnoozeIntervalEdit == NULL) return;

		// Set buddy: Snooze interval edit control
		m_pSnoozeIntervalSpin->SetBuddy(m_pSnoozeIntervalEdit);

		// Set spin edit value
		m_pSnoozeIntervalSpin->SetRange(Constant::Min::SnoozeTime, Constant::Max::SnoozeTime);
		m_pSnoozeIntervalSpin->SetPos(nDefaultSnoozeMin);
		SetSnoozeIntervalEdit(nDefaultSnoozeMin);

		// Set snooze interval value
		SetSnoozeInterval(PwrRepeatSet::defaultSnoozeInterval);
	}

	// Default
	SDialog::SetupDialogItemState();
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	bRecheckState - Recheck all item's state
 * @return	None
 */
void CRmdRepeatSetDlg::RefreshDialogItemState(bool bRecheckState /* = false */)
{
	int nRepeatState = INT_NULL;
	int nSnoozeState = INT_NULL;

	// Update checkbox checked state
	if (m_pRepeatEnableChk != NULL) {
		nRepeatState = m_pRepeatEnableChk->GetCheck();

		// Enable/disable all other sub-controls
		bool bEnable = (nRepeatState == 1) ? true : false;
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
				m_pSnoozeIntervalEdit->EnableWindow(true);
				m_pSnoozeIntervalSpin->EnableWindow(true);
			}
			else {
				m_pSnoozeIntervalEdit->EnableWindow(false);
				m_pSnoozeIntervalSpin->EnableWindow(false);
			}
		}
	}

	// Default
	SDialog::RefreshDialogItemState(bRecheckState);
}

/**
 * @brief	Set value to snooze interval edit control
 * @param	nValue - Value to set (in minutes)
 * @return	None
 */
void CRmdRepeatSetDlg::SetSnoozeIntervalEdit(int nValue)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Get format string
	const wchar_t* formatString = GetLanguageString(pAppLang, PWRRMD_REPEATSET_SNOOZEINTERVAL);
	if (IS_NULL_STRING(formatString)) return;

	// Check validity
	if (((nValue * 60) < PwrRepeatSet::minSnoozeInterval) ||
		((nValue * 60) > PwrRepeatSet::maxSnoozeInterval))
		return;

	// Show snooze interval value
	String snoozeIntervalString = StringUtils::StringFormat(formatString, nValue);
	if (m_pSnoozeIntervalEdit != NULL) {
		// Display
		m_pSnoozeIntervalEdit->SetWindowText(snoozeIntervalString);
		if (m_pSnoozeIntervalSpin != NULL) {
			// Update spin position
			if (m_pSnoozeIntervalSpin->GetPos() != nValue) {
				m_pSnoozeIntervalSpin->SetPos(nValue);
			}
		}
	}
}

/**
 * @brief	Update repeat set data from/to dialog controls
 * @param	pwrItemData - Power Reminder item
 * @param	bUpdate		- Update data flag
 * @return	None
 */
void CRmdRepeatSetDlg::UpdateDialogData(PwrReminderItem& pwrItemData, bool bUpdate)
{
	// Get repeat set data
	PwrRepeatSet& rpsRepeatData = pwrItemData.GetRepeatSetData();

	if (bUpdate == true) {

		/***************************************************************/
		/*															   */
		/*				Update data from dialog controls			   */
		/*															   */
		/***************************************************************/

		// Repeat enable
		int nState = 0;
		if (m_pRepeatEnableChk != NULL) {
			nState = m_pRepeatEnableChk->GetCheck();
			rpsRepeatData.EnableRepeat((nState == 1) ? true : false);
		}

		// Snooze enable
		if (m_pSnoozeEnableChk != NULL) {
			nState = m_pSnoozeEnableChk->GetCheck();
			rpsRepeatData.EnableSnoozing((nState == 1) ? true : false);

			if (m_pSnoozeIntervalSpin != NULL) {
				// Get snooze interval value (in seconds)
				rpsRepeatData.SetSnoozeInterval(this->GetSnoozeInterval());
			}
		}

		// Repeat active status for days of week
		// Note: Read and store data from Sunday first

		BYTE byRepeatDays = 0;
		{
			bool bSundayEnable = false;
			if (m_pActiveSundayChk != NULL) {
				nState = m_pActiveSundayChk->GetCheck();
				bSundayEnable = (nState == 1) ? true : false;
			}
			byRepeatDays |= bSundayEnable << DayOfWeek::Sunday;

			bool bMondayEnable = false;
			if (m_pActiveMondayChk != NULL) {
				nState = m_pActiveMondayChk->GetCheck();
				bMondayEnable = (nState == 1) ? true : false;
			}
			byRepeatDays |= bMondayEnable << DayOfWeek::Monday;

			bool bTuesdayEnable = false;
			if (m_pActiveTuesdayChk != NULL) {
				nState = m_pActiveTuesdayChk->GetCheck();
				bTuesdayEnable = (nState == 1) ? true : false;
			}
			byRepeatDays |= bTuesdayEnable << DayOfWeek::Tuesday;

			bool bWednesdayEnable = false;
			if (m_pActiveWednesdayChk != NULL) {
				nState = m_pActiveWednesdayChk->GetCheck();
				bWednesdayEnable = (nState == 1) ? true : false;
			}
			byRepeatDays |= bWednesdayEnable << DayOfWeek::Wednesday;

			bool bThursdayEnable = false;
			if (m_pActiveThursdayChk != NULL) {
				nState = m_pActiveThursdayChk->GetCheck();
				bThursdayEnable = (nState == 1) ? true : false;
			}
			byRepeatDays |= bThursdayEnable << DayOfWeek::Thursday;

			bool bFridayEnable = false;
			if (m_pActiveFridayChk != NULL) {
				nState = m_pActiveFridayChk->GetCheck();
				bFridayEnable = (nState == 1) ? true : false;
			}
			byRepeatDays |= bFridayEnable << DayOfWeek::Friday;

			bool bSaturdayEnable = false;
			if (m_pActiveSaturdayChk != NULL) {
				nState = m_pActiveSaturdayChk->GetCheck();
				bSaturdayEnable = (nState == 1) ? true : false;
			}
			byRepeatDays |= bSaturdayEnable << DayOfWeek::Saturday;
		}
		rpsRepeatData.SetActiveDays(byRepeatDays);
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
			nState = (rpsRepeatData.IsRepeatEnabled()) ? FLAG_ON : FLAG_OFF;
			m_pRepeatEnableChk->SetCheck(nState);
		}

		// Snooze enable
		if (m_pSnoozeEnableChk != NULL) {
			nState = (rpsRepeatData.IsAllowSnoozing()) ? FLAG_ON : FLAG_OFF;
			m_pSnoozeEnableChk->SetCheck(nState);
			
			if (m_pSnoozeIntervalSpin != NULL) {
				// Set spin edit value (in minutes)
				int nSnoozeMinute = rpsRepeatData.GetSnoozeInterval() / 60;
				m_pSnoozeIntervalSpin->SetPos(nSnoozeMinute);
				SetSnoozeIntervalEdit(nSnoozeMinute);
				// Set snooze interval value (in seconds)
				SetSnoozeInterval(rpsRepeatData.GetSnoozeInterval());
			}
		}

		// Repeat active status for days of week
		// Note: Bind data from Monday first

		if (m_pActiveMondayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(DayOfWeek::Monday)) ? FLAG_ON : FLAG_OFF;
			m_pActiveMondayChk->SetCheck(nState);
		}
		if (m_pActiveTuesdayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(DayOfWeek::Tuesday)) ? FLAG_ON : FLAG_OFF;
			m_pActiveTuesdayChk->SetCheck(nState);
		}
		if (m_pActiveWednesdayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(DayOfWeek::Wednesday)) ? FLAG_ON : FLAG_OFF;
			m_pActiveWednesdayChk->SetCheck(nState);
		}
		if (m_pActiveThursdayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(DayOfWeek::Thursday)) ? FLAG_ON : FLAG_OFF;
			m_pActiveThursdayChk->SetCheck(nState);
		}
		if (m_pActiveFridayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(DayOfWeek::Friday)) ? FLAG_ON : FLAG_OFF;
			m_pActiveFridayChk->SetCheck(nState);
		}
		if (m_pActiveSaturdayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(DayOfWeek::Saturday)) ? FLAG_ON : FLAG_OFF;
			m_pActiveSaturdayChk->SetCheck(nState);
		}
		if (m_pActiveSundayChk != NULL) {
			nState = (rpsRepeatData.IsDayActive(DayOfWeek::Sunday)) ? FLAG_ON : FLAG_OFF;
			m_pActiveSundayChk->SetCheck(nState);
		}

		// Update data
		UpdateData(false);
		RefreshDialogItemState();
	}
}

