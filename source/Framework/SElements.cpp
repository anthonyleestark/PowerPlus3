/**
 * @file		SElements.h
 * @brief		Custom base class for dialog elements (source file)
 * @author		AnthonyLeeStark
 * @date		2024.12.24
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "Framework/SElements.h"
#include "Framework/SWinApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MapTable;
using namespace AppCore;


// Implement methods for SCtrlInfoWrap
IMPLEMENT_DYNAMIC(SCtrlInfoWrap, CObject)


/**
 * @brief	Constructor
 */
SCtrlInfoWrap::SCtrlInfoWrap() : CObject()
{
	// Base control window pointer
	m_pBaseControl = NULL;

	// Relative windows
	m_pParentWnd = NULL;
	m_pBuddyWnd = NULL;

	// Control ID info
	m_nTypeID = Control_Base;
	m_nTemplateID = 0;
	m_strTemplateID.Empty();

	// Control attributes
	m_strCaption.Empty();
	m_bVisible = FALSE;
	m_bEnabled = FALSE;
	m_bFocused = FALSE;

	// --- Control data values --- //

	// Boolean data
	m_pbCheck = NULL;

	// Integer data
	m_plValue = NULL;
	m_plReserveValue = NULL;
	m_plMinValue = NULL;
	m_plMaxValue = NULL;

	// Float data
	m_pdbValue = NULL;
	m_pdbReserveValue = NULL;
	m_pdbMinValue = NULL;
	m_pdbMaxValue = NULL;

	// String data
	m_pstrValue = NULL;
	m_pstrReserveValue = NULL;

	// Integer array data
	m_paulValueList = NULL;
	m_paulReserveValueList = NULL;

	// String array data
	m_pastrValueList = NULL;
	m_pastrReserveValueList = NULL;

	// Time data
	m_pstTimeValue = NULL;

	// Custom data
	m_ptrCustomData = NULL;
	m_pszDataSize = NULL;
}

/**
 * @brief	Destructor
 */
SCtrlInfoWrap::~SCtrlInfoWrap()
{
	// Clean-up and destroy control data pointers
	//

	// Boolean data
	if (m_pbCheck != NULL) {
		delete m_pbCheck;
		m_pbCheck = NULL;
	}

	// Integer data
	if (m_plValue != NULL) {
		delete m_plValue;
		m_plValue = NULL;
	}
	if (m_plReserveValue != NULL) {
		delete m_plReserveValue;
		m_plReserveValue = NULL;
	}
	if (m_plMinValue != NULL) {
		delete m_plMinValue;
		m_plMinValue = NULL;
	}
	if (m_plMaxValue != NULL) {
		delete m_plMaxValue;
		m_plMaxValue = NULL;
	}

	// Float data
	if (m_pdbValue != NULL) {
		delete m_pdbValue;
		m_pdbValue = NULL;
	}
	if (m_pdbReserveValue != NULL) {
		delete m_pdbReserveValue;
		m_pdbReserveValue = NULL;
	}
	if (m_pdbMinValue != NULL) {
		delete m_pdbMinValue;
		m_pdbMinValue = NULL;
	}
	if (m_pdbMaxValue != NULL) {
		delete m_pdbMaxValue;
		m_pdbMaxValue = NULL;
	}

	// String data
	if (m_pstrValue != NULL) {
		delete m_pstrValue;
		m_pstrValue = NULL;
	}
	if (m_pstrReserveValue != NULL) {
		delete m_pstrReserveValue;
		m_pstrReserveValue = NULL;
	}

	// Integer array data
	if (m_paulValueList != NULL) {
		// Cleanup array data
		if (!m_paulValueList->empty()) {
			m_paulValueList->clear();
		}
		delete m_paulValueList;
		m_paulValueList = NULL;
	}
	if (m_paulReserveValueList != NULL) {
		// Cleanup reserved array data
		if (!m_paulReserveValueList->empty()) {
			m_paulReserveValueList->clear();
		}
		delete m_paulReserveValueList;
		m_paulReserveValueList = NULL;
	}

	// String array data
	if (m_pastrValueList != NULL) {
		// Cleanup array data
		if (!m_pastrValueList->empty()) {
			m_pastrValueList->clear();
		}
		delete m_pastrValueList;
		m_pastrValueList = NULL;
	}
	if (m_pastrReserveValueList != NULL) {
		// Cleanup reserved array data
		if (!m_pastrReserveValueList->empty()) {
			m_pastrReserveValueList->clear();
		}
		delete m_pastrReserveValueList;
		m_pastrReserveValueList = NULL;
	}

	// Time data
	if (m_pstTimeValue != NULL) {
		delete m_pstTimeValue;
		m_pstTimeValue = NULL;
	}

	// Custom data
	if (m_ptrCustomData != NULL) {
		// Note: Because this pointer is allocated using 'malloc', 
		// we need to use 'free' for de-allocation to free its memory block; 
		// we also cannot use 'delete' to free it, because it's a void pointer 
		free(m_ptrCustomData);
		m_ptrCustomData = NULL;
	}
	if (m_pszDataSize != NULL) {
		delete m_pszDataSize;
		m_pszDataSize = NULL;
	}
}

/**
 * @brief	Initialize control info wrap object
 * @param	pParentWnd - Parent window
 * @param	pBuddyWnd  - Buddy window
 * @param	nCtrlID	   - Control ID
 * @param	nTypeID	   - Control type ID
 * @return	TRUE/FALSE
 */
BOOL SCtrlInfoWrap::Initialize(CWnd* pParentWnd, CWnd* pBuddyWnd, UINT nCtrlID, INT nTypeID)
{
	ASSERT(pParentWnd->GetSafeHwnd());
	if (pParentWnd == NULL)
		return FALSE;

	// Set base control pointer (maybe NULL)
	this->m_pBaseControl = pParentWnd->GetDlgItem(nCtrlID);

	// Set relative windows
	this->m_pParentWnd = pParentWnd;
	this->m_pBuddyWnd = pBuddyWnd;

	// Set control ID info
	this->m_nTemplateID = nCtrlID;
	this->m_nTypeID = nTypeID;
	this->m_strTemplateID = MAKEUNICODE(GET_NAME_ID(this->m_nTemplateID));

	// Set control attributes
	this->UpdateAttributes();

	return TRUE;
}

/**
 * @brief	Update current control's attributes
 * @param	None
 * @return	None
 */
void SCtrlInfoWrap::UpdateAttributes(void)
{
	// Update control caption by type
	switch (this->m_nTypeID)
	{
		// These types of control have its own caption implemented:
		case Button:
		case Check_Box:
		case Radio_Button:
		case Split_Button_Control:
		case Static_Text:
		case Group_Box:
		case SysLink_Control:
		{
			// Get base control window text itself;
			if (IsBaseControlAvailable()) {
				const int captionLength = this->GetBaseControl()->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(captionLength + 1);
				this->GetBaseControl()->GetWindowText(tempBuff.data(), captionLength + 1);
				String captionString = tempBuff.data();
				this->SetCaption(captionString);
			}
		} break;

		// These types of control have no implemented caption
		// but we can use captions of their buddies
		case Edit_Control:
		case Combo_Box:
		case List_Box:
		case Spin_Control:
		case Slider_Control:
		case Progress_Control:
		case List_Control:
		case Tree_Control:
		case Rich_Edit_Control:
		case Hot_Key:
		case IP_Address_Control:
		case Network_Address_Control:
		case Date_Time_Picker:
		case Month_Calendar_Control:
		{
			// Get buddy control's caption
			if (IsBuddyAvailable()) {
				const int captionLength = this->GetBuddy()->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(captionLength + 1);
				this->GetBuddy()->GetWindowText(tempBuff.data(), captionLength + 1);
				String captionString = tempBuff.data();
				this->SetCaption(captionString);
			}
		} break;

		// The remaining types of control have no caption implemented:
		// Skip by default
		default:
			break;
	}

	// Update control displaying state
	this->SetVisibleState(FALSE);		// Reset state
	this->SetEnableState(FALSE);		// Reset state
	this->SetFocusedState(FALSE);		// Reset state

	if (IsBaseControlAvailable()) {

		// Update visible state
		this->SetVisibleState(GetBaseControl()->IsWindowVisible());

		// Update enable state
		this->SetEnableState(GetBaseControl()->IsWindowEnabled());

		// Update focus state
		if (IsParentAvailable()) {
			CWnd* pFocusCtrl = GetParent()->GetFocus();
			if ((pFocusCtrl != NULL) && (pFocusCtrl == GetBaseControl())) {
				this->SetFocusedState(TRUE);
			}
		}
	}
}

/**
 * @brief	Get current control's checked state
 * @param	None
 * @return	TRUE/FALSE
 */
BOOL SCtrlInfoWrap::GetCheck(void) const
{
	if (this->m_pbCheck == NULL)
		return FALSE;
	else
		return *(this->m_pbCheck);
}

/**
 * @brief	Get current control's integer data value
 * @param	lValue - Integer value (out)
 * @return	LONG_PTR
 */
LONG_PTR SCtrlInfoWrap::GetInteger(void) const
{
	if (this->m_plValue == NULL)
		return INT_INVALID;
	else
		return *(this->m_plValue);
}

void SCtrlInfoWrap::GetInteger(_Out_ LONG_PTR& lValue) const
{
	if (this->m_plValue == NULL)
		lValue = INT_INVALID;
	else
		lValue = *(this->m_plValue);
}

/**
 * @brief	Get current control's reserve integer data value
 * @param	lValue - Integer reserve value (out)
 * @return	LONG_PTR
 */
LONG_PTR SCtrlInfoWrap::GetReserveInteger(void) const
{
	if (this->m_plReserveValue == NULL)
		return INT_INVALID;
	else
		return *(this->m_plReserveValue);
}

void SCtrlInfoWrap::GetReserveInteger(_Out_ LONG_PTR& lValue) const
{
	if (this->m_plReserveValue == NULL)
		lValue = INT_INVALID;
	else
		lValue = *(this->m_plReserveValue);
}

/**
 * @brief	Get current control's min/max range integer data value
 * @param	lMin - Min range integer value (out)
 * @param	lMax - Max range integer value (out)
 * @return	None
 */
void SCtrlInfoWrap::GetMinMaxInt(_Out_ LONG_PTR& lMin, _Out_ LONG_PTR& lMax) const
{
	// Min value
	if (this->m_plMinValue == NULL)
		lMin = INT_INVALID;
	else
		lMin = *(this->m_plMinValue);

	// Max value
	if (this->m_plMaxValue == NULL)
		lMax = INT_INVALID;
	else
		lMax = *(this->m_plMaxValue);
}

/**
 * @brief	Get current control's float data value
 * @param	dbValue - Float value (out)
 * @return	DOUBLE
 */
DOUBLE SCtrlInfoWrap::GetFloat(void) const
{
	if (this->m_pdbValue == NULL)
		return FLOAT_INVALID;
	else
		return *(this->m_pdbValue);
}

void SCtrlInfoWrap::GetFloat(_Out_ DOUBLE& dbValue) const
{
	if (this->m_pdbValue == NULL)
		dbValue = FLOAT_INVALID;
	else
		dbValue = *(this->m_pdbValue);
}

/**
 * @brief	Get current control's reserve float data value
 * @param	dbValue - Float reserve value (out)
 * @return	DOUBLE
 */
DOUBLE SCtrlInfoWrap::GetReserveFloat(void) const
{
	if (this->m_pdbReserveValue == NULL)
		return FLOAT_INVALID;
	else
		return *(this->m_pdbReserveValue);
}

void SCtrlInfoWrap::GetReserveFloat(_Out_ DOUBLE& dbValue) const
{
	if (this->m_pdbReserveValue == NULL)
		dbValue = FLOAT_INVALID;
	else
		dbValue = *(this->m_pdbReserveValue);
}

/**
 * @brief	Get current control's min/max range integer data value
 * @param	lMin - Min range integer value (out)
 * @param	lMax - Max range integer value (out)
 * @return	None
 */
void SCtrlInfoWrap::GetMinMaxFloat(_Out_ DOUBLE& dbMin, _Out_ DOUBLE& dbMax) const
{
	// Min value
	if (this->m_pdbMinValue == NULL)
		dbMin = FLOAT_INVALID;
	else
		dbMin = *(this->m_pdbMinValue);

	// Max value
	if (this->m_pdbMaxValue == NULL)
		dbMax = FLOAT_INVALID;
	else
		dbMax = *(this->m_pdbMaxValue);
}

/**
 * @brief	Get current control's string data value
 * @param	value - String value (out)
 * @return	const wchar_t*
 */
const wchar_t* SCtrlInfoWrap::GetString(void) const
{
	if (this->m_pstrValue == NULL)
		return Constant::String::Empty;
	else
		return *(this->m_pstrValue);
}

void SCtrlInfoWrap::GetString(_Out_ String& value) const
{
	if (this->m_pstrValue == NULL)
		value = Constant::String::Empty;
	else
		value = *(this->m_pstrValue);
}

/**
 * @brief	Get current control's reserve string data value
 * @param	value - String reserve value (out)
 * @return	const wchar_t*
 */
const wchar_t* SCtrlInfoWrap::GetReserveString(void) const
{
	if (this->m_pstrReserveValue == NULL)
		return Constant::String::Empty;
	else
		return *(this->m_pstrReserveValue);
}

void SCtrlInfoWrap::GetReserveString(_Out_ String& value) const
{
	if (this->m_pstrReserveValue == NULL)
		value = Constant::String::Empty;
	else
		value = *(this->m_pstrReserveValue);
}

/**
 * @brief	Get current control's integer array data value
 * @param	aulValue - Integer array value (out)
 * @return	None
 */
void SCtrlInfoWrap::GetIntArray(_Out_ ULongArray& aulValue) const
{
	if (this->m_paulValueList == NULL) {
		aulValue.clear();
	}
	else {
		aulValue = *(this->m_paulValueList);
	}
}

/**
 * @brief	Get current control's reserve integer array data value
 * @param	aulValue - Integer array value (out)
 * @return	None
 */
void SCtrlInfoWrap::GetReserveIntArray(_Out_ ULongArray& aulValue) const
{
	if (this->m_paulReserveValueList == NULL) {
		aulValue.clear();
	}
	else {
		aulValue = *(this->m_paulReserveValueList);
	}
}

/**
 * @brief	Get current control's string array data value
 * @param	astrValue - String array value (out)
 * @return	None
 */
void SCtrlInfoWrap::GetStringArray(_Out_ StringArray& astrValue) const
{
	if (this->m_pastrValueList == NULL) {
		astrValue.clear();
	}
	else {
		astrValue = *(this->m_pastrValueList);
	}
}

/**
 * @brief	Get current control's reserve string array data value
 * @param	astrValue - String array value (out)
 * @return	None
 */
void SCtrlInfoWrap::GetReserveStringArray(_Out_ StringArray& astrValue) const
{
	if (this->m_pastrReserveValueList == NULL) {
		astrValue.clear();
	}
	else {
		astrValue = *(this->m_pastrReserveValueList);
	}
}

/**
 * @brief	Get current control's time data value
 * @param	timeValue - Time value (out)
 * @return	SYSTEMTIME
 */
SYSTEMTIME SCtrlInfoWrap::GetTime(void) const
{
	if (this->m_pstTimeValue == NULL)
		return SYSTEMTIME_ZERO;
	else
		return *(this->m_pstTimeValue);
}

void SCtrlInfoWrap::GetTime(_Out_ SYSTEMTIME& timeValue) const
{
	if (this->m_pstTimeValue == NULL)
		timeValue = SYSTEMTIME_ZERO;
	else
		timeValue = *(this->m_pstTimeValue);
}

/**
 * @brief	Set current control's check state
 * @param	bCheck - Checked state (BOOLEAN)
 * @return	None
 */
void SCtrlInfoWrap::SetCheck(_In_ const BOOL& bCheck)
{
	if (this->m_pbCheck == NULL)
		this->m_pbCheck = new BOOL(bCheck);
	else {
		delete (this->m_pbCheck);
		this->m_pbCheck = new BOOL(bCheck);
	}
}

/**
 * @brief	Set current control's integer data value
 * @param	lValue - Integer value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetInteger(_In_ const LONG_PTR& lValue)
{
	if (this->m_plValue == NULL)
		this->m_plValue = new LONG_PTR(lValue);
	else {
		delete (this->m_plValue);
		this->m_plValue = new LONG_PTR(lValue);
	}
}

/**
 * @brief	Set current control's reserve integer data value
 * @param	lValue - Integer reserve value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetReserveInteger(_In_ const LONG_PTR& lValue)
{
	if (this->m_plReserveValue == NULL)
		this->m_plReserveValue = new LONG_PTR(lValue);
	else {
		delete (this->m_plReserveValue);
		this->m_plReserveValue = new LONG_PTR(lValue);
	}
}

/**
 * @brief	Set current control's min/max range integer data value
 * @param	lMin - Min range integer value (in)
 * @param	lMax - Max range integer value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetMinMaxInt(_In_ const LONG_PTR& lMin, _In_ const LONG_PTR& lMax)
{
	// Min value
	if (this->m_plMinValue == NULL)
		this->m_plMinValue = new LONG_PTR(lMin);
	else {
		delete (this->m_plMinValue);
		this->m_plMinValue = new LONG_PTR(lMin);
	}

	// Max value
	if (this->m_plMaxValue == NULL)
		this->m_plMaxValue = new LONG_PTR(lMax);
	else {
		delete (this->m_plMaxValue);
		this->m_plMaxValue = new LONG_PTR(lMax);
	}
}

/**
 * @brief	Set current control's float data value
 * @param	dbValue - Float value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetFloat(_In_ const DOUBLE& dbValue)
{
	if (this->m_pdbValue == NULL)
		this->m_pdbValue = new DOUBLE(dbValue);
	else {
		delete (this->m_pdbValue);
		this->m_pdbValue = new DOUBLE(dbValue);
	}
}

/**
 * @brief	Set current control's reserve float data value
 * @param	dbValue - Float reserve value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetReserveFloat(_In_ const DOUBLE& dbValue)
{
	if (this->m_pdbReserveValue == NULL)
		this->m_pdbReserveValue = new DOUBLE(dbValue);
	else {
		delete (this->m_pdbReserveValue);
		this->m_pdbReserveValue = new DOUBLE(dbValue);
	}
}

/**
 * @brief	Set current control's min/max range float data value
 * @param	lMin - Min range integer value (in)
 * @param	lMax - Max range integer value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetMinMaxFloat(_In_ const DOUBLE& dbMin, _In_ const DOUBLE& dbMax)
{
	// Min value
	if (this->m_pdbMinValue == NULL)
		this->m_pdbMinValue = new DOUBLE(dbMin);
	else {
		delete (this->m_pdbMinValue);
		this->m_pdbMinValue = new DOUBLE(dbMin);
	}

	// Max value
	if (this->m_pdbMaxValue == NULL)
		this->m_pdbMaxValue = new DOUBLE(dbMax);
	else {
		delete (this->m_pdbMaxValue);
		this->m_pdbMaxValue = new DOUBLE(dbMax);
	}
}

/**
 * @brief	Set current control's string data value
 * @param	value - String value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetString(_In_ const wchar_t* value)
{
	if (this->m_pstrValue == NULL)
		this->m_pstrValue = new String(value);
	else {
		delete (this->m_pstrValue);
		this->m_pstrValue = new String(value);
	}
}

/**
 * @brief	Set current control's reserve string data value
 * @param	value - String reserve value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetReserveString(_In_ const wchar_t* value)
{
	if (this->m_pstrReserveValue == NULL)
		this->m_pstrReserveValue = new String(value);
	else {
		delete (this->m_pstrReserveValue);
		this->m_pstrReserveValue = new String(value);
	}
}

/**
 * @brief	Set current control's integer array data value
 * @param	aulValue - Integer array value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetIntArray(_In_ const ULongArray& aulValue)
{
	if (this->m_paulValueList == NULL)
		this->m_paulValueList = new ULongArray();
	
	if (this->m_paulValueList != NULL) {
		this->m_paulValueList->clear();
		this->m_paulValueList->assign(aulValue.begin(), aulValue.end());
	}
}

/**
 * @brief	Set current control's reserve integer array data value
 * @param	aulValue - Integer array value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetReserveIntArray(_In_ const ULongArray& aulValue)
{
	if (this->m_paulReserveValueList == NULL)
		this->m_paulReserveValueList = new ULongArray();

	if (this->m_paulReserveValueList != NULL) {
		this->m_paulReserveValueList->clear();
		this->m_paulReserveValueList->assign(aulValue.begin(), aulValue.end());
	}
}

/**
 * @brief	Set current control's string array data value
 * @param	astrValue - String array value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetStringArray(_In_ const StringArray& astrValue)
{
	if (this->m_pastrValueList == NULL)
		this->m_pastrValueList = new StringArray();

	if (this->m_pastrValueList != NULL) {
		this->m_pastrValueList->clear();
		this->m_pastrValueList->assign(astrValue.begin(), astrValue.end());
	}
}

/**
 * @brief	Set current control's reserve string array data value
 * @param	astrValue - String array value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetReserveStringArray(_In_ const StringArray& astrValue)
{
	if (this->m_pastrReserveValueList == NULL)
		this->m_pastrReserveValueList = new StringArray();

	if (this->m_pastrReserveValueList != NULL) {
		this->m_pastrReserveValueList->clear();
		this->m_pastrReserveValueList->assign(astrValue.begin(), astrValue.end());
	}
}

/**
 * @brief	Set current control's time data value
 * @param	timeValue - Time value (in)
 * @return	None
 */
void SCtrlInfoWrap::SetTime(_In_ const SYSTEMTIME& timeValue)
{
	if (this->m_pstTimeValue == NULL)
		this->m_pstTimeValue = new SYSTEMTIME(timeValue);
	else {
		delete (this->m_pstTimeValue);
		this->m_pstTimeValue = new SYSTEMTIME(timeValue);
	}
}

/**
 * @brief	Get current control's custom data pointer
 * @param	lpOutput   - Output data pointer
 * @param	szDataSize - Data's total size in bytes (in/out)
 * @return	TRUE/FALSE
 */
template<typename DATA_TYPE>
BOOL SCtrlInfoWrap::GetData(_Outptr_ DATA_TYPE* lpOutput, _Inout_opt_z_ SIZE_T& szDataSize) const
{
	// If pointers are invalid
	if ((lpOutput == NULL) ||
		(this->m_ptrCustomData == NULL) ||
		(this->m_pszDataSize == NULL))
		return FALSE;	// Fail to retrieve data

	// If size value is invalid
	SIZE_T szCurDataSize = *(this->m_pszDataSize);
	if ((szDataSize <= 0) || (szCurDataSize <= 0))
		return FALSE;	// Fail to retrieve data

	// If the output buffer size is insufficient
	if (szDataSize < szCurDataSize) {
		// Re-allocate output data pointer
		// we can 
		delete[] lpOutput;
		lpOutput = new DATA_TYPE[szCurDataSize / sizeof(DATA_TYPE)];
		ASSERT(lpOutput != NULL);
		if (lpOutput == NULL) {
			throw std::bad_alloc();
			return FALSE;	// Allocation failed
		}
	}

	// Copy data and update the output data size
	memcpy(lpOutput, this->m_ptrCustomData, szCurDataSize);
	szDataSize = szCurDataSize;

	return TRUE;	// Get data successfully
}

/**
 * @brief	Set current control's custom data pointer
 * @param	lpInput	   - Input data pointer
 * @param	szDataSize - Data's total size in bytes (in)
 * @return	None
 */
template<typename DATA_TYPE>
BOOL SCtrlInfoWrap::SetData(_In_ const DATA_TYPE* lpInput, _In_ const SIZE_T& szDataSize)
{
	// If input data or size are invalid
	if ((lpInput == NULL) || (szDataSize <= 0))
		return FALSE;	// Fail to set data

	// If the current data pointer is not empty
	// or its current total size in bytes is not large enough
	if ((this->m_ptrCustomData != NULL) ||
		((this->m_pszDataSize != NULL) && (*(this->m_pszDataSize) < szDataSize))) {
		if (this->m_ptrCustomData != NULL) {
			// Free the data pointer's existing memory
			free(this->m_ptrCustomData);
		}
	}

	// Allocate the data pointer if not available
	if (this->m_ptrCustomData == NULL) {
		this->m_ptrCustomData = malloc(szDataSize);
		ASSERT(this->m_ptrCustomData != NULL);
		if (this->m_ptrCustomData == NULL) {
			throw std::bad_alloc();
			return FALSE;	// Allocation failed
		}
	}

	// Allocate the data size pointer if not available
	if (this->m_pszDataSize == NULL) {
		this->m_pszDataSize = new SIZE_T;
		ASSERT(this->m_pszDataSize != NULL);
		if (this->m_pszDataSize == NULL) {
			throw std::bad_alloc();
			return FALSE;	// Allocation failed
		}
	}

	// Copy data
	memcpy(this->m_ptrCustomData, lpInput, szDataSize);

	// Update the data's new total size in bytes
	*(this->m_pszDataSize) = szDataSize;

	return TRUE;	// Set data successfully
}

/**
 * @brief	Check if the current control's custom data pointer is empty
 * @param	None
 * @return	TRUE/FALSE
 */
BOOL SCtrlInfoWrap::IsDataEmpty(void) const
{
	// If the data pointer is not available, then it's indeed empty
	if (this->m_ptrCustomData == NULL)
		return TRUE;

	// If the data size is not available or invalid,
	// the data can be considered empty
	if ((this->m_pszDataSize == NULL) ||
		(this->m_pszDataSize != NULL) && *(this->m_pszDataSize) <= 0)
		return TRUE;

	return FALSE;	// Not empty
}

/**
 * @brief	Get total size in bytes of the current control's custom data
 * @param	None
 * @return	SIZE_T
 */
SIZE_T SCtrlInfoWrap::GetDataSize(void) const
{
	// If the data is empty, return 0
	if (IsDataEmpty())
		return INT_NULL;

	// Otherwise, return the size
	if (this->m_pszDataSize != NULL)
		return *(this->m_pszDataSize);

	return INT_NULL;	// Default, return 0
}


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for SMenu
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(SMenu, CMenu)

/**
 * @brief	Constructor
 */
SMenu::SMenu(CWnd* /* pParentWnd = NULL */) : CMenu()
{
	// User menu layout
	m_pMenuLayout = NULL;
}

/**
 * @brief	Destructor
 */
SMenu::~SMenu()
{
	// List of controls
	if (m_pMenuLayout != NULL) {
		delete m_pMenuLayout;
		m_pMenuLayout = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for SControlManager
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(SControlManager, CObject)

/**
 * @brief	Constructor
 */
SControlManager::SControlManager(CWnd* pParentWnd /* = NULL */) : CObject()
{
	// List of control info wrappers
	m_pCtrlInfoArray = NULL;

	// Parent window
	m_pParentWnd = pParentWnd;
}

/**
 * @brief	Destructor
 */
SControlManager::~SControlManager()
{
	// List of control info wrappers
	if (m_pCtrlInfoArray != NULL) {
		this->DeleteAll();
		delete m_pCtrlInfoArray;
		m_pCtrlInfoArray = NULL;
	}
}

/**
 * @brief	Initialize control management
 * @param	None
 * @return	None
 */
BOOL SControlManager::Initialize(void)
{
	// Initialize control info list pointer
	if (this->m_pCtrlInfoArray == NULL) {
		this->m_pCtrlInfoArray = new SCtrlInfoList();

		// Allocation failed
		if (this->m_pCtrlInfoArray == NULL)
			return FALSE;
	}

	// Empty array data
	m_pCtrlInfoArray->clear();

	return TRUE;
}

/**
 * @brief	Delete all and clean-up control management
 * @param	None
 * @return	None
 */
BOOL SControlManager::DeleteAll(void)
{
	// If data is not initialized
	if (this->m_pCtrlInfoArray == NULL)
		return FALSE;

	// Delete all control info wrapper pointers
	for (int nIndex = 0; nIndex < (this->m_pCtrlInfoArray->size()); nIndex++) {
		SCtrlInfoWrap* pExControl = m_pCtrlInfoArray->at(nIndex);
		if (pExControl != NULL) {
			delete pExControl;
			pExControl = NULL;
		}
	}

	// Empty array data
	m_pCtrlInfoArray->clear();

	return TRUE;
}

/**
 * @brief	Add dialog/window control to management
 * @param	pControl - Dialog control item
 * @return	long long
 */
long long SControlManager::AddControl(SCtrlInfoWrap* pControl)
{
	// Check for control pointer validity
	if (pControl == NULL) 
		return INT_INVALID;

	// If data is not initialized
	if (this->m_pCtrlInfoArray == NULL)
		return INT_INVALID;

	// Search if control ID had already existed
	for (int nIndex = 0; nIndex < (this->m_pCtrlInfoArray->size()); nIndex++) {
		SCtrlInfoWrap* pExControl = m_pCtrlInfoArray->at(nIndex);
		if (pExControl == NULL) continue;
		if (pExControl->GetTemplateID() == pControl->GetTemplateID()) {
			// Return control index
			return nIndex;
		}
	}

	// Add control to list and return list size
	this->m_pCtrlInfoArray->push_back(pControl);
	return (this->m_pCtrlInfoArray->size());
}

/**
 * @brief	Add dialog/window control to management
 * @param	nCtrlID - Dialog control ID
 * @param	nTypeID - Control type ID
 * @return	long long
 */
long long SControlManager::AddControl(UINT nCtrlID, UINT nTypeID)
{
	// If parent window is not set, do nothing
	if (this->m_pParentWnd == NULL)
		return INT_INVALID;

	// Get base control window pointer
	CWnd* pCtrlWnd = m_pParentWnd->GetDlgItem(nCtrlID);
	if (pCtrlWnd == NULL)
		return INT_INVALID;

	// Initialize control info
	SCtrlInfoWrap* pControl = new SCtrlInfoWrap();
	pControl->Initialize(m_pParentWnd, NULL, nCtrlID, nTypeID);

	// Add control to management list
	size_t nRetIndex = this->AddControl(pControl);
	if (nRetIndex != -1)
		return nRetIndex;

	// Failed to add
	delete pControl;
	return INT_INVALID;
}

/**
 * @brief	Remove dialog/window control from management
 * @param	nCtrlID - Dialog control ID
 * @return	size_t
 */
long long SControlManager::RemoveControl(UINT nCtrlID)
{
	// If data is not initialized or is empty
	if ((m_pCtrlInfoArray == NULL) || (this->IsEmpty()))
		return INT_INVALID;

	// Search for control ID
	for (int nIndex = 0; nIndex < (m_pCtrlInfoArray->size()); nIndex++) {
		SCtrlInfoWrap* pExControl = m_pCtrlInfoArray->at(nIndex);
		if (pExControl == NULL) continue;
		if (pExControl->GetTemplateID() == nCtrlID) {
			delete pExControl;

			// Remove control from list
			m_pCtrlInfoArray->erase(m_pCtrlInfoArray->begin() + nIndex);
			return (m_pCtrlInfoArray->size());
		}
	}

	// Control ID not found, return -1
	return INT_INVALID;
}

/**
 * @brief	Remove dialog/window control from management
 * @param	nCtrlID - Dialog control ID
 * @return	None
 */
SCtrlInfoWrap* SControlManager::GetControl(UINT nCtrlID)
{
	// If data is not initialized or is empty
	if ((this->m_pCtrlInfoArray == NULL) || (this->IsEmpty()))
		return NULL;

	// Search for control ID
	for (int nIndex = 0; nIndex < (this->m_pCtrlInfoArray->size()); nIndex++) {
		SCtrlInfoWrap* pControl = m_pCtrlInfoArray->at(nIndex);
		if (pControl == NULL) continue;
		if (pControl->GetTemplateID() == nCtrlID)
			return pControl;
	}

	return NULL;
}

/**
 * @brief	Set buddy relationship between 2 controls
 * @param	nBaseCtrlID  - Base control ID
 * @param	nBuddyCtrlID - Buddy control ID
 * @return	TRUE/FALSE
 */
BOOL SControlManager::SetBuddy(UINT nBaseCtrlID, UINT nBuddyCtrlID)
{
	// Get base control from management list
	SCtrlInfoWrap* pBaseControl = GetControl(nBaseCtrlID);
	if (pBaseControl == NULL)
		return FALSE;

	// Get buddy control (natively)
	// No need to check NULL for parent window because if base control is found 
	// in management list, it means parent window had already been set
	CWnd* pBuddyWnd = m_pParentWnd->GetDlgItem(nBuddyCtrlID);
	if (pBuddyWnd == NULL)
		return FALSE;

	// Set buddy relationship
	pBaseControl->SetBuddy(pBuddyWnd);

	// Trigger updating control attributes
	// This will set buddy control caption as base control caption if available
	pBaseControl->UpdateAttributes();

	return TRUE;
}

/**
 * @brief	Update data for specified control or all controls
 * @param	nCtrlID - Control ID (NULL means all controls)
 * @return	None
 */
void SControlManager::UpdateData(UINT nCtrlID /* = NULL */)
{
	// If data is not initialized or is empty
	if ((this->m_pCtrlInfoArray == NULL) || (this->IsEmpty()))
		return;

	// If parent window is not available
	if (this->m_pParentWnd == NULL)
		return;

	// Loop through control management list
	INT nTriggerForceRetFlag = FLAG_OFF;
	for (int nIndex = 0; nIndex < (this->m_pCtrlInfoArray->size()); nIndex++) {

		// If force return flag is ON, break the loop
		if (nTriggerForceRetFlag == FLAG_ON)
			break;

		// Get control wrapper pointer
		SCtrlInfoWrap* pCurControl = m_pCtrlInfoArray->at(nIndex);
		if (pCurControl == NULL) continue;

		// Only update data for specified control
		if (nCtrlID != NULL) {
			if (pCurControl->GetTemplateID() != nCtrlID) {
				// Skip this control
				continue;
			}
			else {
				// Trigger force return flag 
				// It will stop after updating for current control
				nTriggerForceRetFlag = FLAG_ON;
			}
		}

		// Get base control pointer
		CWnd* pBaseControl = pCurControl->GetBaseControl();

		// If base control is not available, skip updating
		if (!pCurControl->IsBaseControlAvailable())
			continue;

		// Update control attributes
		pCurControl->UpdateAttributes();

		// Update data for control by type
		switch (pCurControl->GetType())
		{
			// Clickable and checkable controls
			case Button:
			case Check_Box:
			case Radio_Button:
			{
				// Update control's checked state
				BOOL bCheck = ((CButton*)pBaseControl)->GetCheck();
				pCurControl->SetCheck(bCheck);
			} break;

			// Edit box
			case Edit_Control:
			{
				// Update control's text value
				const int textLength = ((CEdit*)pBaseControl)->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(textLength + 1);
				((CEdit*)pBaseControl)->GetWindowText(tempBuff.data(), textLength + 1);
				String tempText = tempBuff.data();
				pCurControl->SetString(tempText);
			} break;

			// Combo-box
			case Combo_Box:
			{
				// Update control's current selection index
				size_t nCurSel = ((CComboBox*)pBaseControl)->GetCurSel();
				pCurControl->SetInteger(nCurSel);
				// Update all item strings
				StringArray arrStringData;
				size_t nCount = ((CComboBox*)pBaseControl)->GetCount();
				arrStringData.reserve(nCount);
				for (size_t nIndex = 0; nIndex < nCount; nIndex++) {
					wchar_t tempBuff[Constant::Max::StringLength] = {0};
					((CComboBox*)pBaseControl)->GetLBText(nIndex, tempBuff);
					arrStringData.push_back(tempBuff);
				}
				pCurControl->SetStringArray(arrStringData);
			} break;

			// List box
			case List_Box:
			{
				// Update control's current selection index
				size_t nCurSel = ((CListBox*)pBaseControl)->GetCurSel();
				pCurControl->SetInteger(nCurSel);
				// Update all item strings
				StringArray arrStringData;
				size_t nCount = ((CListBox*)pBaseControl)->GetCount();
				arrStringData.reserve(nCount);
				for (size_t nIndex = 0; nIndex < nCount; nIndex++) {
					wchar_t tempBuff[Constant::Max::StringLength] = {0};
					((CListBox*)pBaseControl)->GetText(nIndex, tempBuff);
					arrStringData.push_back(tempBuff);
				}
				pCurControl->SetStringArray(arrStringData);
			} break;

			// List control
			case List_Control:
			{
				// Update number of items and columns
				size_t nItemCount = ((CListCtrl*)pBaseControl)->GetItemCount();
				pCurControl->SetInteger(nItemCount);
				size_t nColumnCount = 0;
				CHeaderCtrl* pHeaderCtrl = ((CListCtrl*)pBaseControl)->GetHeaderCtrl();
				if (pHeaderCtrl != NULL) {
					nColumnCount = pHeaderCtrl->GetItemCount();
				}
				pCurControl->SetReserveInteger(nColumnCount);
				// Update control's data current selection index(es)
				ULongArray arrSelection;
				arrSelection.reserve(nItemCount);
				for (size_t nIndex = 0; nIndex < nItemCount; nIndex++) {
					// Get selection index
					if ((((CListCtrl*)pBaseControl)->GetItemState(nIndex, LVIS_SELECTED) & LVIS_SELECTED) == LVIS_SELECTED) {
						arrSelection.push_back(nIndex);
					}
				}
				pCurControl->SetIntArray(arrSelection);
				// Update all item strings
				StringArray arrStringData;
				arrStringData.reserve(nItemCount);
				for (size_t nIndex = 0; nIndex < nItemCount; nIndex++) {
					for (size_t nColIndex = 0; nColIndex < nColumnCount; nColIndex++) {
						// Get item text
						const wchar_t* tempText = ((CListCtrl*)pBaseControl)->GetItemText(nIndex, nColIndex);
						arrStringData.push_back(tempText);
					}
				}
				pCurControl->SetStringArray(arrStringData);
			} break;

			// Tab control
			case Tab_Control:
			{
				// Update the number of tabs
				size_t nTabCount = ((CTabCtrl*)pBaseControl)->GetItemCount();
				// Update the currently selected tab index
				size_t nCurSelTab = ((CTabCtrl*)pBaseControl)->GetCurSel();
				pCurControl->SetInteger(nCurSelTab);
				// Update all tab's title
				TCITEM tabInfo;
				StringArray arrTabTitles;
				arrTabTitles.reserve(nTabCount);
				for (size_t nIndex = 0; nIndex < nTabCount; nIndex++) {
					String tempText = Constant::String::Empty;
					BOOL bRet = ((CTabCtrl*)pBaseControl)->GetItem(nIndex, &tabInfo);
					if (bRet == TRUE && ((tabInfo.mask & TCIF_TEXT) != 0)) {
						tempText = tabInfo.pszText;
					}
					arrTabTitles.push_back(tempText);
				}
				pCurControl->SetStringArray(arrTabTitles);
			} break;

			// Static text and decorating items
			case Static_Text:
			case Group_Box:
			case SysLink_Control:
			{
				// Update control's text label
				const int textLength = pBaseControl->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(textLength + 1);
				pBaseControl->GetWindowText(tempBuff.data(), textLength + 1);
				String captionString = tempBuff.data();
				pCurControl->SetCaption(captionString);
			} break;

			// Scroll bars
			case Horizontal_Scroll_Bar:
			case Vertical_Scroll_Bar:
			{
				// Update control's current position
				size_t nCurPos = ((CScrollBar*)pBaseControl)->GetScrollPos();
				pCurControl->SetInteger(nCurPos);
				// Update control's min/max range
				INT nMin = NULL, nMax = NULL;
				((CScrollBar*)pBaseControl)->GetScrollRange(&nMin, &nMax);
				pCurControl->SetMinMaxInt(nMin, nMax);
			} break;

			// Slider control
			case Slider_Control:
			{
				// Update control's current position
				size_t nCurPos = ((CSliderCtrl*)pBaseControl)->GetPos();
				pCurControl->SetInteger(nCurPos);
				// Update control's min/max range
				size_t nMin = ((CSliderCtrl*)pBaseControl)->GetRangeMin();
				size_t nMax = ((CSliderCtrl*)pBaseControl)->GetRangeMax();
				pCurControl->SetMinMaxInt(nMin, nMax);
			} break;

			// Progress bar
			case Progress_Control:
			{
				// Update control's current position
				size_t nCurPos = ((CProgressCtrl*)pBaseControl)->GetPos();
				pCurControl->SetInteger(nCurPos);
				// Update control's min/max range
				INT nMin = NULL, nMax = NULL;
				((CProgressCtrl*)pBaseControl)->GetRange(nMin, nMax);
				pCurControl->SetMinMaxInt(nMin, nMax);
			} break;

			// Spin button control
			case Spin_Control:
			{
				// Update control's current position
				size_t nCurPos = ((CSpinButtonCtrl*)pBaseControl)->GetPos();
				pCurControl->SetInteger(nCurPos);
				// Update control's min/max range
				INT nMin = NULL, nMax = NULL;
				((CSpinButtonCtrl*)pBaseControl)->GetRange(nMin, nMax);
				pCurControl->SetMinMaxInt(nMin, nMax);
			} break;

			// Hot key control
			case Hot_Key:
			{
				// Update control's current hotkey
				DWORD dwHotkey = ((CHotKeyCtrl*)pBaseControl)->GetHotKey();
				pCurControl->SetInteger(LOWORD(dwHotkey));			// Virtual keycode
				pCurControl->SetReserveInteger(HIWORD(dwHotkey));	// Modifier flags
			} break;

			// IP address control
			case IP_Address_Control:
			{
				// Update control's current IP address
				DWORD dwAddress = 0;
				BYTE byField0 = 0, byField1 = 0, byField2 = 0, byField3 = 0;
				INT nNonBlankFieldNum = ((CIPAddressCtrl*)pBaseControl)->GetAddress(dwAddress);
				((CIPAddressCtrl*)pBaseControl)->GetAddress(byField0, byField1, byField2, byField3);
				pCurControl->SetInteger(dwAddress);
				pCurControl->SetReserveInteger(nNonBlankFieldNum);
				// Store each field value separately into an integer array
				ULongArray arrAddressFields;
				arrAddressFields.resize(4);
				arrAddressFields[0] = byField0;		// Field 0
				arrAddressFields[1] = byField1;		// Field 1
				arrAddressFields[2] = byField2;		// Field 2
				arrAddressFields[3] = byField3;		// Field 3
				pCurControl->SetIntArray(arrAddressFields);
			} break;

			// Network address control
			case Network_Address_Control:
			{
				// Update control's current network address
				NC_ADDRESS ncAddress;
				NET_ADDRESS_INFO netAddressInfo;
				ncAddress.pAddrInfo = &netAddressInfo;
				HRESULT hRes = ((CNetAddressCtrl*)pBaseControl)->GetAddress(&ncAddress);
				if (hRes == S_OK) {
					// Save address and port info
					String addressString = ncAddress.pAddrInfo->NamedAddress.Address;
					String portString = ncAddress.pAddrInfo->NamedAddress.Port;
					pCurControl->SetString(addressString);
					pCurControl->SetReserveString(portString);
					// Numeric data
					pCurControl->SetInteger(ncAddress.PortNumber);
					pCurControl->SetReserveInteger(ncAddress.PrefixLength);
				}
			} break;

			// Date time picker
			case Date_Time_Picker:
			{
				// Update control's date/time value
				SYSTEMTIME timeTemp = SYSTEMTIME_ZERO;
				((CDateTimeCtrl*)pBaseControl)->GetTime(&timeTemp);
				pCurControl->SetTime(timeTemp);
			} break;

			// Month calendar control
			case Month_Calendar_Control:
			{
				// Update control's current selected date
				SYSTEMTIME dateTemp = SYSTEMTIME_ZERO;
				((CMonthCalCtrl*)pBaseControl)->GetCurSel(&dateTemp);
				pCurControl->SetTime(dateTemp);
			} break;
		}
	}
}

