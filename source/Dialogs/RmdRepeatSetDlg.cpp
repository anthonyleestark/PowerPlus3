/**
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
constexpr const int kDefaultWidth = 320;
constexpr const int kDefaultHeight = 240;


// Implement methods for CRmdRepeatSetDlg
IMPLEMENT_DYNAMIC(CRmdRepeatSetDlg, SDialog)


/**
 * @brief	Constructor
 */
CRmdRepeatSetDlg::CRmdRepeatSetDlg() : SDialog(IDD_RMDREPEATSET_DLG)
{
	// Dialog control
	snoozeIntervalEditPtr_ = NULL;
	snoozeIntervalSpinPtr_ = NULL;

	repeatEnableChkPtr_ = NULL;
	snoozeEnableChkPtr_ = NULL;
	activeMondayChkPtr_ = NULL;
	activeTuesdayChkPtr_ = NULL;
	activeWednesdayChkPtr_ = NULL;
	activeThursdayChkPtr_ = NULL;
	activeFridayChkPtr_ = NULL;
	activeSaturdayChkPtr_ = NULL;
	activeSundayChkPtr_ = NULL;

	// Member value
	snoozeInterval_ = 0;
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
	setupLanguage();
	setupDialogItemState();
	refreshDialogItemState();

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
void CRmdRepeatSetDlg::OnActivate(UINT state, CWnd* otherWndPtr, BOOL isMinimized)
{
	// Default
	SDialog::OnActivate(state, otherWndPtr, isMinimized);

	// If dialog is inactivated
	if (state == WA_INACTIVE) {
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
	LPNMUPDOWN upDownPtr = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// Get timespin position
	int position = upDownPtr->iPos;

	// Set snooze interval value
	setSnoozeInterval(position * 60);

	// Convert and set edit value
	setSnoozeIntervalEdit(position);

	*pResult = NULL;
}

/**
 * @brief	Handle clicked event for checkbox-es
 * @param	id - ID of checkbox
 * @return	None
 */
void CRmdRepeatSetDlg::OnCheckboxClicked(UINT /*id*/)
{
	refreshDialogItemState();
}

/**
 * @brief	Setup language for dialog controls
 * @param	None
 * @return	None
 */
void CRmdRepeatSetDlg::setupLanguage()
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		unsigned id = childWndPtr->GetDlgCtrlID();

		switch (id)
		{
		case IDC_RMDREPEATSET_DUMMY_BORDER:
		case IDC_RMDREPEATSET_DETAILS_STATIC:
		case IDC_RMDREPEATSET_ACTIVEDAYS_STATIC:
		case IDC_RMDREPEATSET_SNOOZE_INTERVAL_EDIT:
		case IDC_RMDREPEATSET_SNOOZE_INTERVAL_SPIN:
			// Skip these items
			break;

		default:
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void CRmdRepeatSetDlg::setupDialogItemState()
{
	// Initialize dialog items
	if (snoozeIntervalEditPtr_ == NULL) {
		snoozeIntervalEditPtr_ = (CEdit*)GetDlgItem(IDC_RMDREPEATSET_SNOOZE_INTERVAL_EDIT);
		if (snoozeIntervalEditPtr_ == NULL) {
			TRACE_ERROR("Error: Snooze interval edit control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (snoozeIntervalSpinPtr_ == NULL) {
		snoozeIntervalSpinPtr_ = (CSpinButtonCtrl*)GetDlgItem(IDC_RMDREPEATSET_SNOOZE_INTERVAL_SPIN);
		if (snoozeIntervalSpinPtr_ == NULL) {
			TRACE_ERROR("Error: Snooze interval spin control not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (repeatEnableChkPtr_ == NULL) {
		repeatEnableChkPtr_ = (CButton*)GetDlgItem(IDC_RMDREPEATSET_REPEAT_CHK);
		if (repeatEnableChkPtr_ == NULL) {
			TRACE_ERROR("Error: Repeat enable checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (snoozeEnableChkPtr_ == NULL) {
		snoozeEnableChkPtr_ = (CButton*)GetDlgItem(IDC_RMDREPEATSET_SNOOZE_CHK);
		if (snoozeEnableChkPtr_ == NULL) {
			TRACE_ERROR("Error: Snooze enable checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (activeMondayChkPtr_ == NULL) {
		activeMondayChkPtr_ = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_MONDAY_CHK);
		if (activeMondayChkPtr_ == NULL) {
			TRACE_ERROR("Error: Monday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (activeTuesdayChkPtr_ == NULL) {
		activeTuesdayChkPtr_ = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_TUESDAY_CHK);
		if (activeTuesdayChkPtr_ == NULL) {
			TRACE_ERROR("Error: Tuesday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (activeWednesdayChkPtr_ == NULL) {
		activeWednesdayChkPtr_ = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_WEDNESDAY_CHK);
		if (activeWednesdayChkPtr_ == NULL) {
			TRACE_ERROR("Error: Wednesday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (activeThursdayChkPtr_ == NULL) {
		activeThursdayChkPtr_ = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_THURSDAY_CHK);
		if (activeThursdayChkPtr_ == NULL) {
			TRACE_ERROR("Error: Thursday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (activeFridayChkPtr_ == NULL) {
		activeFridayChkPtr_ = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_FRIDAY_CHK);
		if (activeFridayChkPtr_ == NULL) {
			TRACE_ERROR("Error: Friday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (activeSaturdayChkPtr_ == NULL) {
		activeSaturdayChkPtr_ = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_SATURDAY_CHK);
		if (activeSaturdayChkPtr_ == NULL) {
			TRACE_ERROR("Error: Saturday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (activeSundayChkPtr_ == NULL) {
		activeSundayChkPtr_ = (CButton*)GetDlgItem(IDC_RMDREPEATSET_ACTIVE_SUNDAY_CHK);
		if (activeSundayChkPtr_ == NULL) {
			TRACE_ERROR("Error: Sunday active checkbox not found!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Setup properties
	int defaultSnoozeMin = PwrRepeatSet::kDefaultSnoozeInterval / 60;
	if (snoozeIntervalEditPtr_ != NULL) {
		snoozeIntervalEditPtr_->SetReadOnly(true);
	}
	if (snoozeIntervalSpinPtr_ != NULL) {
		if (snoozeIntervalEditPtr_ == NULL) return;

		// Set buddy: Snooze interval edit control
		snoozeIntervalSpinPtr_->SetBuddy(snoozeIntervalEditPtr_);

		// Set spin edit value
		snoozeIntervalSpinPtr_->SetRange(Constant::Min::SnoozeTime, Constant::Max::SnoozeTime);
		snoozeIntervalSpinPtr_->SetPos(defaultSnoozeMin);
		setSnoozeIntervalEdit(defaultSnoozeMin);

		// Set snooze interval value
		setSnoozeInterval(PwrRepeatSet::kDefaultSnoozeInterval);
	}

	// Default
	SDialog::setupDialogItemState();
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	isRecheckState - Recheck all item's state
 * @return	None
 */
void CRmdRepeatSetDlg::refreshDialogItemState(bool isRecheckState /* = false */)
{
	int repeatState = Constant::kNullInteger;
	int snoozeState = Constant::kNullInteger;

	// Update checkbox checked state
	if (repeatEnableChkPtr_ != NULL) {
		repeatState = repeatEnableChkPtr_->GetCheck();

		// Enable/disable all other sub-controls
		bool isEnabled = (repeatState == 1) ? true : false;
		if (snoozeEnableChkPtr_ != NULL) {
			snoozeEnableChkPtr_->EnableWindow(isEnabled);
		}
		if (activeMondayChkPtr_ != NULL) {
			activeMondayChkPtr_->EnableWindow(isEnabled);
		}
		if (activeTuesdayChkPtr_ != NULL) {
			activeTuesdayChkPtr_->EnableWindow(isEnabled);
		}
		if (activeWednesdayChkPtr_ != NULL) {
			activeWednesdayChkPtr_->EnableWindow(isEnabled);
		}
		if (activeThursdayChkPtr_ != NULL) {
			activeThursdayChkPtr_->EnableWindow(isEnabled);
		}
		if (activeFridayChkPtr_ != NULL) {
			activeFridayChkPtr_->EnableWindow(isEnabled);
		}
		if (activeSaturdayChkPtr_ != NULL) {
			activeSaturdayChkPtr_->EnableWindow(isEnabled);
		}
		if (activeSundayChkPtr_ != NULL) {
			activeSundayChkPtr_->EnableWindow(isEnabled);
		}

	}
	if (snoozeEnableChkPtr_ != NULL) {
		snoozeState = snoozeEnableChkPtr_->GetCheck();

		// Enable/disable snooze controls
		if ((snoozeIntervalEditPtr_ != NULL) && (snoozeIntervalSpinPtr_ != NULL)) {
			if ((repeatState == 1) && (snoozeState == 1)) {
				snoozeIntervalEditPtr_->EnableWindow(true);
				snoozeIntervalSpinPtr_->EnableWindow(true);
			}
			else {
				snoozeIntervalEditPtr_->EnableWindow(false);
				snoozeIntervalSpinPtr_->EnableWindow(false);
			}
		}
	}

	// Default
	SDialog::refreshDialogItemState(isRecheckState);
}

/**
 * @brief	Set value to snooze interval edit control
 * @param	value - Value to set (in minutes)
 * @return	None
 */
void CRmdRepeatSetDlg::setSnoozeIntervalEdit(int value)
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Get format string
	const wchar_t* formatString = getLanguageString(languageTablePtr, PWRRMD_REPEATSET_SNOOZEINTERVAL);
	if (IS_NULL_STRING(formatString)) return;

	// Check validity
	if (((value * 60) < PwrRepeatSet::kMinSnoozeInterval) ||
		((value * 60) > PwrRepeatSet::kMaxSnoozeInterval))
		return;

	// Show snooze interval value
	String snoozeIntervalString = StringUtils::stringFormat(formatString, value);
	if (snoozeIntervalEditPtr_ != NULL) {
		// Display
		snoozeIntervalEditPtr_->SetWindowText(snoozeIntervalString);
		if (snoozeIntervalSpinPtr_ != NULL) {
			// Update spin position
			if (snoozeIntervalSpinPtr_->GetPos() != value) {
				snoozeIntervalSpinPtr_->SetPos(value);
			}
		}
	}
}

/**
 * @brief	Update repeat set data from/to dialog controls
 * @param	reminderItem - Power Reminder item
 * @param	updateFlag		- Update data flag
 * @return	None
 */
void CRmdRepeatSetDlg::updateDialogData(PwrReminderItem& reminderItem, bool updateFlag)
{
	// Get repeat set data
	PwrRepeatSet& repeatData = reminderItem.getRepeatSetData();

	if (updateFlag == true) {

		/***************************************************************/
		/*															   */
		/*				Update data from dialog controls			   */
		/*															   */
		/***************************************************************/

		// Repeat enable
		int state = 0;
		if (repeatEnableChkPtr_ != NULL) {
			state = repeatEnableChkPtr_->GetCheck();
			repeatData.enableRepeat((state == 1) ? true : false);
		}

		// Snooze enable
		if (snoozeEnableChkPtr_ != NULL) {
			state = snoozeEnableChkPtr_->GetCheck();
			repeatData.enableSnoozing((state == 1) ? true : false);

			if (snoozeIntervalSpinPtr_ != NULL) {
				// Get snooze interval value (in seconds)
				repeatData.setSnoozeInterval(this->getSnoozeInterval());
			}
		}

		// Repeat active status for days of week
		// Note: Read and store data from Sunday first

		BYTE repeatDays = 0;
		{
			bool isSundayEnabled = false;
			if (activeSundayChkPtr_ != NULL) {
				state = activeSundayChkPtr_->GetCheck();
				isSundayEnabled = (state == 1) ? true : false;
			}
			repeatDays |= isSundayEnabled << DayOfWeek::Sunday;

			bool isMondayEnabled = false;
			if (activeMondayChkPtr_ != NULL) {
				state = activeMondayChkPtr_->GetCheck();
				isMondayEnabled = (state == 1) ? true : false;
			}
			repeatDays |= isMondayEnabled << DayOfWeek::Monday;

			bool isTuesdayEnabled = false;
			if (activeTuesdayChkPtr_ != NULL) {
				state = activeTuesdayChkPtr_->GetCheck();
				isTuesdayEnabled = (state == 1) ? true : false;
			}
			repeatDays |= isTuesdayEnabled << DayOfWeek::Tuesday;

			bool isWednesdayEnabled = false;
			if (activeWednesdayChkPtr_ != NULL) {
				state = activeWednesdayChkPtr_->GetCheck();
				isWednesdayEnabled = (state == 1) ? true : false;
			}
			repeatDays |= isWednesdayEnabled << DayOfWeek::Wednesday;

			bool isThursdayEnabled = false;
			if (activeThursdayChkPtr_ != NULL) {
				state = activeThursdayChkPtr_->GetCheck();
				isThursdayEnabled = (state == 1) ? true : false;
			}
			repeatDays |= isThursdayEnabled << DayOfWeek::Thursday;

			bool isFridayEnabled = false;
			if (activeFridayChkPtr_ != NULL) {
				state = activeFridayChkPtr_->GetCheck();
				isFridayEnabled = (state == 1) ? true : false;
			}
			repeatDays |= isFridayEnabled << DayOfWeek::Friday;

			bool isSaturdayEnabled = false;
			if (activeSaturdayChkPtr_ != NULL) {
				state = activeSaturdayChkPtr_->GetCheck();
				isSaturdayEnabled = (state == 1) ? true : false;
			}
			repeatDays |= isSaturdayEnabled << DayOfWeek::Saturday;
		}
		repeatData.setActiveDays(repeatDays);
	}
	else {

		/***************************************************************/
		/*															   */
		/*				  Bind data to dialog controls				   */
		/*															   */
		/***************************************************************/

		int state = 0;

		// Repeat enable
		if (repeatEnableChkPtr_ != NULL) {
			state = (repeatData.isRepeatEnabled()) ? FLAG_ON : FLAG_OFF;
			repeatEnableChkPtr_->SetCheck(state);
		}

		// Snooze enable
		if (snoozeEnableChkPtr_ != NULL) {
			state = (repeatData.isAllowSnoozing()) ? FLAG_ON : FLAG_OFF;
			snoozeEnableChkPtr_->SetCheck(state);
			
			if (snoozeIntervalSpinPtr_ != NULL) {
				// Set spin edit value (in minutes)
				int nSnoozeMinute = repeatData.getSnoozeInterval() / 60;
				snoozeIntervalSpinPtr_->SetPos(nSnoozeMinute);
				setSnoozeIntervalEdit(nSnoozeMinute);
				// Set snooze interval value (in seconds)
				setSnoozeInterval(repeatData.getSnoozeInterval());
			}
		}

		// Repeat active status for days of week
		// Note: Bind data from Monday first

		if (activeMondayChkPtr_ != NULL) {
			state = (repeatData.isDayActive(DayOfWeek::Monday)) ? FLAG_ON : FLAG_OFF;
			activeMondayChkPtr_->SetCheck(state);
		}
		if (activeTuesdayChkPtr_ != NULL) {
			state = (repeatData.isDayActive(DayOfWeek::Tuesday)) ? FLAG_ON : FLAG_OFF;
			activeTuesdayChkPtr_->SetCheck(state);
		}
		if (activeWednesdayChkPtr_ != NULL) {
			state = (repeatData.isDayActive(DayOfWeek::Wednesday)) ? FLAG_ON : FLAG_OFF;
			activeWednesdayChkPtr_->SetCheck(state);
		}
		if (activeThursdayChkPtr_ != NULL) {
			state = (repeatData.isDayActive(DayOfWeek::Thursday)) ? FLAG_ON : FLAG_OFF;
			activeThursdayChkPtr_->SetCheck(state);
		}
		if (activeFridayChkPtr_ != NULL) {
			state = (repeatData.isDayActive(DayOfWeek::Friday)) ? FLAG_ON : FLAG_OFF;
			activeFridayChkPtr_->SetCheck(state);
		}
		if (activeSaturdayChkPtr_ != NULL) {
			state = (repeatData.isDayActive(DayOfWeek::Saturday)) ? FLAG_ON : FLAG_OFF;
			activeSaturdayChkPtr_->SetCheck(state);
		}
		if (activeSundayChkPtr_ != NULL) {
			state = (repeatData.isDayActive(DayOfWeek::Sunday)) ? FLAG_ON : FLAG_OFF;
			activeSundayChkPtr_->SetCheck(state);
		}

		// Update data
		UpdateData(false);
		refreshDialogItemState();
	}
}

