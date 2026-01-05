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
	baseControlWndPtr_ = NULL;

	// Relative windows
	parentWndPtr_ = NULL;
	buddyWndPtr_ = NULL;

	// Control ID info
	typeId_ = Control_Base;
	templateId_ = 0;
	templateStringId_.empty();

	// Control attributes
	caption_.empty();
	isVisible_ = false;
	isEnabled_ = false;
	isFocused_ = false;

	// --- Control data values --- //

	// Boolean data
	isCheckedPtr_ = NULL;

	// Integer data
	valueIntPtr_ = NULL;
	reservedValueIntPtr_ = NULL;
	minValueIntPtr_ = NULL;
	maxValueIntPtr_ = NULL;

	// Float data
	valueDoublePtr_ = NULL;
	reservedValueDoublePtr_ = NULL;
	minValueDoublePtr_ = NULL;
	maxValueDoublePtr_ = NULL;

	// String data
	valueStringPtr_ = NULL;
	reservedValueStringPtr_ = NULL;

	// Integer array data
	valueIntListPtr_ = NULL;
	reservedValueIntListPtr_ = NULL;

	// String array data
	valueStringListPtr_ = NULL;
	reservedValueStringListPtr_ = NULL;

	// Time data
	valueTimePtr_ = NULL;

	// Custom data
	customDataPtr_ = NULL;
	dataSizePtr_ = NULL;
}

/**
 * @brief	Destructor
 */
SCtrlInfoWrap::~SCtrlInfoWrap()
{
	// Clean-up and destroy control data pointers
	//

	// Boolean data
	if (isCheckedPtr_ != NULL) {
		delete isCheckedPtr_;
		isCheckedPtr_ = NULL;
	}

	// Integer data
	if (valueIntPtr_ != NULL) {
		delete valueIntPtr_;
		valueIntPtr_ = NULL;
	}
	if (reservedValueIntPtr_ != NULL) {
		delete reservedValueIntPtr_;
		reservedValueIntPtr_ = NULL;
	}
	if (minValueIntPtr_ != NULL) {
		delete minValueIntPtr_;
		minValueIntPtr_ = NULL;
	}
	if (maxValueIntPtr_ != NULL) {
		delete maxValueIntPtr_;
		maxValueIntPtr_ = NULL;
	}

	// Float data
	if (valueDoublePtr_ != NULL) {
		delete valueDoublePtr_;
		valueDoublePtr_ = NULL;
	}
	if (reservedValueDoublePtr_ != NULL) {
		delete reservedValueDoublePtr_;
		reservedValueDoublePtr_ = NULL;
	}
	if (minValueDoublePtr_ != NULL) {
		delete minValueDoublePtr_;
		minValueDoublePtr_ = NULL;
	}
	if (maxValueDoublePtr_ != NULL) {
		delete maxValueDoublePtr_;
		maxValueDoublePtr_ = NULL;
	}

	// String data
	if (valueStringPtr_ != NULL) {
		delete valueStringPtr_;
		valueStringPtr_ = NULL;
	}
	if (reservedValueStringPtr_ != NULL) {
		delete reservedValueStringPtr_;
		reservedValueStringPtr_ = NULL;
	}

	// Integer array data
	if (valueIntListPtr_ != NULL) {
		// Cleanup array data
		if (!valueIntListPtr_->empty()) {
			valueIntListPtr_->clear();
		}
		delete valueIntListPtr_;
		valueIntListPtr_ = NULL;
	}
	if (reservedValueIntListPtr_ != NULL) {
		// Cleanup reserved array data
		if (!reservedValueIntListPtr_->empty()) {
			reservedValueIntListPtr_->clear();
		}
		delete reservedValueIntListPtr_;
		reservedValueIntListPtr_ = NULL;
	}

	// String array data
	if (valueStringListPtr_ != NULL) {
		// Cleanup array data
		if (!valueStringListPtr_->empty()) {
			valueStringListPtr_->clear();
		}
		delete valueStringListPtr_;
		valueStringListPtr_ = NULL;
	}
	if (reservedValueStringListPtr_ != NULL) {
		// Cleanup reserved array data
		if (!reservedValueStringListPtr_->empty()) {
			reservedValueStringListPtr_->clear();
		}
		delete reservedValueStringListPtr_;
		reservedValueStringListPtr_ = NULL;
	}

	// Time data
	if (valueTimePtr_ != NULL) {
		delete valueTimePtr_;
		valueTimePtr_ = NULL;
	}

	// Custom data
	if (customDataPtr_ != NULL) {
		// Note: Because this pointer is allocated using 'malloc', 
		// we need to use 'free' for de-allocation to free its memory block; 
		// we also cannot use 'delete' to free it, because it's a void pointer 
		free(customDataPtr_);
		customDataPtr_ = NULL;
	}
	if (dataSizePtr_ != NULL) {
		delete dataSizePtr_;
		dataSizePtr_ = NULL;
	}
}

/**
 * @brief	Initialize control info wrap object
 * @param	parentWndPtr - Parent window
 * @param	buddyWndPtr  - Buddy window
 * @param	controlId	 - Control ID
 * @param	controlId	 - Control type ID
 * @return	true/false
 */
bool SCtrlInfoWrap::initialize(CWnd* parentWndPtr, CWnd* buddyWndPtr, unsigned controlId, int typeId)
{
	ASSERT(parentWndPtr->GetSafeHwnd());
	if (parentWndPtr == NULL)
		return false;

	// Set base control pointer (maybe NULL)
	this->baseControlWndPtr_ = parentWndPtr->GetDlgItem(controlId);

	// Set relative windows
	this->parentWndPtr_ = parentWndPtr;
	this->buddyWndPtr_ = buddyWndPtr;

	// Set control ID info
	this->templateId_ = controlId;
	this->typeId_ = typeId;
	this->templateStringId_ = MAKEUNICODE(GET_NAME_ID(this->templateId_));

	// Set control attributes
	this->updateAttributes();

	return true;
}

/**
 * @brief	Update current control's attributes
 * @param	None
 * @return	None
 */
void SCtrlInfoWrap::updateAttributes(void)
{
	// Update control caption by type
	switch (this->typeId_)
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
			if (isBaseControlAvailable()) {
				const int captionLength = this->getBaseControl()->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(captionLength + 1);
				this->getBaseControl()->GetWindowText(tempBuff.data(), captionLength + 1);
				String captionString = tempBuff.data();
				this->setCaption(captionString);
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
			if (isBuddyAvailable()) {
				const int captionLength = this->getBuddy()->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(captionLength + 1);
				this->getBuddy()->GetWindowText(tempBuff.data(), captionLength + 1);
				String captionString = tempBuff.data();
				this->setCaption(captionString);
			}
		} break;

		// The remaining types of control have no caption implemented:
		// Skip by default
		default:
			break;
	}

	// Update control displaying state
	this->setVisibleState(false);		// Reset state
	this->setEnableState(false);		// Reset state
	this->setFocusedState(false);		// Reset state

	if (isBaseControlAvailable()) {

		// Update visible state
		this->setVisibleState(getBaseControl()->IsWindowVisible());

		// Update enable state
		this->setEnableState(getBaseControl()->IsWindowEnabled());

		// Update focus state
		if (isParentAvailable()) {
			CWnd* pFocusCtrl = getParent()->GetFocus();
			if ((pFocusCtrl != NULL) && (pFocusCtrl == getBaseControl())) {
				this->setFocusedState(true);
			}
		}
	}
}

/**
 * @brief	Get current control's checked state
 * @param	None
 * @return	true/false
 */
bool SCtrlInfoWrap::getCheck(void) const
{
	if (this->isCheckedPtr_ == NULL)
		return false;
	else
		return *(this->isCheckedPtr_);
}

/**
 * @brief	Get current control's integer data value
 * @param	value - Integer value (out)
 * @return	LONG_PTR
 */
LONG_PTR SCtrlInfoWrap::getInteger(void) const
{
	if (this->valueIntPtr_ == NULL)
		return INT_INVALID;
	else
		return *(this->valueIntPtr_);
}

void SCtrlInfoWrap::getInteger(_Out_ LONG_PTR& value) const
{
	if (this->valueIntPtr_ == NULL)
		value = INT_INVALID;
	else
		value = *(this->valueIntPtr_);
}

/**
 * @brief	Get current control's reserve integer data value
 * @param	value - Integer reserve value (out)
 * @return	LONG_PTR
 */
LONG_PTR SCtrlInfoWrap::getReserveInteger(void) const
{
	if (this->reservedValueIntPtr_ == NULL)
		return INT_INVALID;
	else
		return *(this->reservedValueIntPtr_);
}

void SCtrlInfoWrap::getReserveInteger(_Out_ LONG_PTR& value) const
{
	if (this->reservedValueIntPtr_ == NULL)
		value = INT_INVALID;
	else
		value = *(this->reservedValueIntPtr_);
}

/**
 * @brief	Get current control's min/max range integer data value
 * @param	minVal - Min range integer value (out)
 * @param	maxVal - Max range integer value (out)
 * @return	None
 */
void SCtrlInfoWrap::getMinMaxInt(_Out_ LONG_PTR& minVal, _Out_ LONG_PTR& maxVal) const
{
	// Min value
	if (this->minValueIntPtr_ == NULL)
		minVal = INT_INVALID;
	else
		minVal = *(this->minValueIntPtr_);

	// Max value
	if (this->maxValueIntPtr_ == NULL)
		maxVal = INT_INVALID;
	else
		maxVal = *(this->maxValueIntPtr_);
}

/**
 * @brief	Get current control's float data value
 * @param	valueDouble - Float value (out)
 * @return	DOUBLE
 */
DOUBLE SCtrlInfoWrap::getFloat(void) const
{
	if (this->valueDoublePtr_ == NULL)
		return FLOAT_INVALID;
	else
		return *(this->valueDoublePtr_);
}

void SCtrlInfoWrap::getFloat(_Out_ DOUBLE& valueDouble) const
{
	if (this->valueDoublePtr_ == NULL)
		valueDouble = FLOAT_INVALID;
	else
		valueDouble = *(this->valueDoublePtr_);
}

/**
 * @brief	Get current control's reserve float data value
 * @param	valueDouble - Float reserve value (out)
 * @return	DOUBLE
 */
DOUBLE SCtrlInfoWrap::getReserveFloat(void) const
{
	if (this->reservedValueDoublePtr_ == NULL)
		return FLOAT_INVALID;
	else
		return *(this->reservedValueDoublePtr_);
}

void SCtrlInfoWrap::getReserveFloat(_Out_ DOUBLE& valueDouble) const
{
	if (this->reservedValueDoublePtr_ == NULL)
		valueDouble = FLOAT_INVALID;
	else
		valueDouble = *(this->reservedValueDoublePtr_);
}

/**
 * @brief	Get current control's min/max range integer data value
 * @param	minVal - Min range integer value (out)
 * @param	maxVal - Max range integer value (out)
 * @return	None
 */
void SCtrlInfoWrap::getMinMaxFloat(_Out_ DOUBLE& minVal, _Out_ DOUBLE& maxVal) const
{
	// Min value
	if (this->minValueDoublePtr_ == NULL)
		minVal = FLOAT_INVALID;
	else
		minVal = *(this->minValueDoublePtr_);

	// Max value
	if (this->maxValueDoublePtr_ == NULL)
		maxVal = FLOAT_INVALID;
	else
		maxVal = *(this->maxValueDoublePtr_);
}

/**
 * @brief	Get current control's string data value
 * @param	value - String value (out)
 * @return	const wchar_t*
 */
const wchar_t* SCtrlInfoWrap::getString(void) const
{
	if (this->valueStringPtr_ == NULL)
		return Constant::String::Empty;
	else
		return *(this->valueStringPtr_);
}

void SCtrlInfoWrap::getString(_Out_ String& value) const
{
	if (this->valueStringPtr_ == NULL)
		value = Constant::String::Empty;
	else
		value = *(this->valueStringPtr_);
}

/**
 * @brief	Get current control's reserve string data value
 * @param	value - String reserve value (out)
 * @return	const wchar_t*
 */
const wchar_t* SCtrlInfoWrap::getReserveString(void) const
{
	if (this->reservedValueStringPtr_ == NULL)
		return Constant::String::Empty;
	else
		return *(this->reservedValueStringPtr_);
}

void SCtrlInfoWrap::getReserveString(_Out_ String& value) const
{
	if (this->reservedValueStringPtr_ == NULL)
		value = Constant::String::Empty;
	else
		value = *(this->reservedValueStringPtr_);
}

/**
 * @brief	Get current control's integer array data value
 * @param	valueList - Integer array value (out)
 * @return	None
 */
void SCtrlInfoWrap::getIntArray(_Out_ ULongArray& valueList) const
{
	if (this->valueIntListPtr_ == NULL) {
		valueList.clear();
	}
	else {
		valueList = *(this->valueIntListPtr_);
	}
}

/**
 * @brief	Get current control's reserve integer array data value
 * @param	valueList - Integer array value (out)
 * @return	None
 */
void SCtrlInfoWrap::getReserveIntArray(_Out_ ULongArray& valueList) const
{
	if (this->reservedValueIntListPtr_ == NULL) {
		valueList.clear();
	}
	else {
		valueList = *(this->reservedValueIntListPtr_);
	}
}

/**
 * @brief	Get current control's string array data value
 * @param	valueList - String array value (out)
 * @return	None
 */
void SCtrlInfoWrap::getStringArray(_Out_ StringArray& valueList) const
{
	if (this->valueStringListPtr_ == NULL) {
		valueList.clear();
	}
	else {
		valueList = *(this->valueStringListPtr_);
	}
}

/**
 * @brief	Get current control's reserve string array data value
 * @param	valueList - String array value (out)
 * @return	None
 */
void SCtrlInfoWrap::getReserveStringArray(_Out_ StringArray& valueList) const
{
	if (this->reservedValueStringListPtr_ == NULL) {
		valueList.clear();
	}
	else {
		valueList = *(this->reservedValueStringListPtr_);
	}
}

/**
 * @brief	Get current control's time data value
 * @param	timeValue - Time value (out)
 * @return	SYSTEMTIME
 */
SYSTEMTIME SCtrlInfoWrap::getTime(void) const
{
	if (this->valueTimePtr_ == NULL)
		return {0};
	else
		return *(this->valueTimePtr_);
}

void SCtrlInfoWrap::getTime(_Out_ SYSTEMTIME& timeValue) const
{
	if (this->valueTimePtr_ == NULL)
		timeValue = {0};
	else
		timeValue = *(this->valueTimePtr_);
}

/**
 * @brief	Set current control's check state
 * @param	isChecked - Checked state (BOOLEAN)
 * @return	None
 */
void SCtrlInfoWrap::setCheck(_In_ const bool& isChecked)
{
	if (this->isCheckedPtr_ == NULL)
		this->isCheckedPtr_ = new bool(isChecked);
	else {
		delete (this->isCheckedPtr_);
		this->isCheckedPtr_ = new bool(isChecked);
	}
}

/**
 * @brief	Set current control's integer data value
 * @param	value - Integer value (in)
 * @return	None
 */
void SCtrlInfoWrap::setInteger(_In_ const LONG_PTR& value)
{
	if (this->valueIntPtr_ == NULL)
		this->valueIntPtr_ = new LONG_PTR(value);
	else {
		delete (this->valueIntPtr_);
		this->valueIntPtr_ = new LONG_PTR(value);
	}
}

/**
 * @brief	Set current control's reserve integer data value
 * @param	value - Integer reserve value (in)
 * @return	None
 */
void SCtrlInfoWrap::setReserveInteger(_In_ const LONG_PTR& value)
{
	if (this->reservedValueIntPtr_ == NULL)
		this->reservedValueIntPtr_ = new LONG_PTR(value);
	else {
		delete (this->reservedValueIntPtr_);
		this->reservedValueIntPtr_ = new LONG_PTR(value);
	}
}

/**
 * @brief	Set current control's min/max range integer data value
 * @param	minVal - Min range integer value (in)
 * @param	maxVal - Max range integer value (in)
 * @return	None
 */
void SCtrlInfoWrap::setMinMaxInt(_In_ const LONG_PTR& minVal, _In_ const LONG_PTR& maxVal)
{
	// Min value
	if (this->minValueIntPtr_ == NULL)
		this->minValueIntPtr_ = new LONG_PTR(minVal);
	else {
		delete (this->minValueIntPtr_);
		this->minValueIntPtr_ = new LONG_PTR(minVal);
	}

	// Max value
	if (this->maxValueIntPtr_ == NULL)
		this->maxValueIntPtr_ = new LONG_PTR(maxVal);
	else {
		delete (this->maxValueIntPtr_);
		this->maxValueIntPtr_ = new LONG_PTR(maxVal);
	}
}

/**
 * @brief	Set current control's float data value
 * @param	valueDouble - Float value (in)
 * @return	None
 */
void SCtrlInfoWrap::setFloat(_In_ const DOUBLE& valueDouble)
{
	if (this->valueDoublePtr_ == NULL)
		this->valueDoublePtr_ = new DOUBLE(valueDouble);
	else {
		delete (this->valueDoublePtr_);
		this->valueDoublePtr_ = new DOUBLE(valueDouble);
	}
}

/**
 * @brief	Set current control's reserve float data value
 * @param	valueDouble - Float reserve value (in)
 * @return	None
 */
void SCtrlInfoWrap::setReserveFloat(_In_ const DOUBLE& valueDouble)
{
	if (this->reservedValueDoublePtr_ == NULL)
		this->reservedValueDoublePtr_ = new DOUBLE(valueDouble);
	else {
		delete (this->reservedValueDoublePtr_);
		this->reservedValueDoublePtr_ = new DOUBLE(valueDouble);
	}
}

/**
 * @brief	Set current control's min/max range float data value
 * @param	minVal - Min range integer value (in)
 * @param	maxVal - Max range integer value (in)
 * @return	None
 */
void SCtrlInfoWrap::setMinMaxFloat(_In_ const DOUBLE& minVal, _In_ const DOUBLE& maxVal)
{
	// Min value
	if (this->minValueDoublePtr_ == NULL)
		this->minValueDoublePtr_ = new DOUBLE(minVal);
	else {
		delete (this->minValueDoublePtr_);
		this->minValueDoublePtr_ = new DOUBLE(minVal);
	}

	// Max value
	if (this->maxValueDoublePtr_ == NULL)
		this->maxValueDoublePtr_ = new DOUBLE(maxVal);
	else {
		delete (this->maxValueDoublePtr_);
		this->maxValueDoublePtr_ = new DOUBLE(maxVal);
	}
}

/**
 * @brief	Set current control's string data value
 * @param	value - String value (in)
 * @return	None
 */
void SCtrlInfoWrap::setString(_In_ const wchar_t* value)
{
	if (this->valueStringPtr_ == NULL)
		this->valueStringPtr_ = new String(value);
	else {
		delete (this->valueStringPtr_);
		this->valueStringPtr_ = new String(value);
	}
}

/**
 * @brief	Set current control's reserve string data value
 * @param	value - String reserve value (in)
 * @return	None
 */
void SCtrlInfoWrap::setReserveString(_In_ const wchar_t* value)
{
	if (this->reservedValueStringPtr_ == NULL)
		this->reservedValueStringPtr_ = new String(value);
	else {
		delete (this->reservedValueStringPtr_);
		this->reservedValueStringPtr_ = new String(value);
	}
}

/**
 * @brief	Set current control's integer array data value
 * @param	valueList - Integer array value (in)
 * @return	None
 */
void SCtrlInfoWrap::setIntArray(_In_ const ULongArray& valueList)
{
	if (this->valueIntListPtr_ == NULL)
		this->valueIntListPtr_ = new ULongArray();
	
	if (this->valueIntListPtr_ != NULL) {
		this->valueIntListPtr_->clear();
		this->valueIntListPtr_->assign(valueList.begin(), valueList.end());
	}
}

/**
 * @brief	Set current control's reserve integer array data value
 * @param	valueList - Integer array value (in)
 * @return	None
 */
void SCtrlInfoWrap::setReserveIntArray(_In_ const ULongArray& valueList)
{
	if (this->reservedValueIntListPtr_ == NULL)
		this->reservedValueIntListPtr_ = new ULongArray();

	if (this->reservedValueIntListPtr_ != NULL) {
		this->reservedValueIntListPtr_->clear();
		this->reservedValueIntListPtr_->assign(valueList.begin(), valueList.end());
	}
}

/**
 * @brief	Set current control's string array data value
 * @param	valueList - String array value (in)
 * @return	None
 */
void SCtrlInfoWrap::setStringArray(_In_ const StringArray& valueList)
{
	if (this->valueStringListPtr_ == NULL)
		this->valueStringListPtr_ = new StringArray();

	if (this->valueStringListPtr_ != NULL) {
		this->valueStringListPtr_->clear();
		this->valueStringListPtr_->assign(valueList.begin(), valueList.end());
	}
}

/**
 * @brief	Set current control's reserve string array data value
 * @param	valueList - String array value (in)
 * @return	None
 */
void SCtrlInfoWrap::setReserveStringArray(_In_ const StringArray& valueList)
{
	if (this->reservedValueStringListPtr_ == NULL)
		this->reservedValueStringListPtr_ = new StringArray();

	if (this->reservedValueStringListPtr_ != NULL) {
		this->reservedValueStringListPtr_->clear();
		this->reservedValueStringListPtr_->assign(valueList.begin(), valueList.end());
	}
}

/**
 * @brief	Set current control's time data value
 * @param	timeValue - Time value (in)
 * @return	None
 */
void SCtrlInfoWrap::setTime(_In_ const SYSTEMTIME& timeValue)
{
	if (this->valueTimePtr_ == NULL)
		this->valueTimePtr_ = new SYSTEMTIME(timeValue);
	else {
		delete (this->valueTimePtr_);
		this->valueTimePtr_ = new SYSTEMTIME(timeValue);
	}
}

/**
 * @brief	Get current control's custom data pointer
 * @param	outputPtr   - Output data pointer
 * @param	dataSize - Data's total size in bytes (in/out)
 * @return	true/false
 */
template<typename DATA_TYPE>
bool SCtrlInfoWrap::getData(_Outptr_ DATA_TYPE* outputPtr, _Inout_opt_z_ SIZE_T& dataSize) const
{
	// If pointers are invalid
	if ((outputPtr == NULL) ||
		(this->customDataPtr_ == NULL) ||
		(this->dataSizePtr_ == NULL))
		return false;	// Fail to retrieve data

	// If size value is invalid
	SIZE_T szCurDataSize = *(this->dataSizePtr_);
	if ((dataSize <= 0) || (szCurDataSize <= 0))
		return false;	// Fail to retrieve data

	// If the output buffer size is insufficient
	if (dataSize < szCurDataSize) {
		// Re-allocate output data pointer
		// we can 
		delete[] outputPtr;
		outputPtr = new DATA_TYPE[szCurDataSize / sizeof(DATA_TYPE)];
		ASSERT(outputPtr != NULL);
		if (outputPtr == NULL) {
			throw std::bad_alloc();
			return false;	// Allocation failed
		}
	}

	// Copy data and update the output data size
	memcpy(outputPtr, this->customDataPtr_, szCurDataSize);
	dataSize = szCurDataSize;

	return true;	// Get data successfully
}

/**
 * @brief	Set current control's custom data pointer
 * @param	inputPtr	   - Input data pointer
 * @param	dataSize - Data's total size in bytes (in)
 * @return	None
 */
template<typename DATA_TYPE>
bool SCtrlInfoWrap::setData(_In_ const DATA_TYPE* inputPtr, _In_ const SIZE_T& dataSize)
{
	// If input data or size are invalid
	if ((inputPtr == NULL) || (dataSize <= 0))
		return false;	// Fail to set data

	// If the current data pointer is not empty
	// or its current total size in bytes is not large enough
	if ((this->customDataPtr_ != NULL) ||
		((this->dataSizePtr_ != NULL) && (*(this->dataSizePtr_) < dataSize))) {
		if (this->customDataPtr_ != NULL) {
			// Free the data pointer's existing memory
			free(this->customDataPtr_);
		}
	}

	// Allocate the data pointer if not available
	if (this->customDataPtr_ == NULL) {
		this->customDataPtr_ = malloc(dataSize);
		ASSERT(this->customDataPtr_ != NULL);
		if (this->customDataPtr_ == NULL) {
			throw std::bad_alloc();
			return false;	// Allocation failed
		}
	}

	// Allocate the data size pointer if not available
	if (this->dataSizePtr_ == NULL) {
		this->dataSizePtr_ = new SIZE_T;
		ASSERT(this->dataSizePtr_ != NULL);
		if (this->dataSizePtr_ == NULL) {
			throw std::bad_alloc();
			return false;	// Allocation failed
		}
	}

	// Copy data
	memcpy(this->customDataPtr_, inputPtr, dataSize);

	// Update the data's new total size in bytes
	*(this->dataSizePtr_) = dataSize;

	return true;	// Set data successfully
}

/**
 * @brief	Check if the current control's custom data pointer is empty
 * @param	None
 * @return	true/false
 */
bool SCtrlInfoWrap::isDataEmpty(void) const
{
	// If the data pointer is not available, then it's indeed empty
	if (this->customDataPtr_ == NULL)
		return true;

	// If the data size is not available or invalid,
	// the data can be considered empty
	if ((this->dataSizePtr_ == NULL) ||
		(this->dataSizePtr_ != NULL) && *(this->dataSizePtr_) <= 0)
		return true;

	return false;	// Not empty
}

/**
 * @brief	Get total size in bytes of the current control's custom data
 * @param	None
 * @return	SIZE_T
 */
SIZE_T SCtrlInfoWrap::getDataSize(void) const
{
	// If the data is empty, return 0
	if (isDataEmpty())
		return INT_NULL;

	// Otherwise, return the size
	if (this->dataSizePtr_ != NULL)
		return *(this->dataSizePtr_);

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
SMenu::SMenu(CWnd* /* parentWnd = NULL */) : CMenu()
{
	// User menu layout
	menuLayoutPtr_ = NULL;
}

/**
 * @brief	Destructor
 */
SMenu::~SMenu()
{
	// List of controls
	if (menuLayoutPtr_ != NULL) {
		delete menuLayoutPtr_;
		menuLayoutPtr_ = NULL;
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
SControlManager::SControlManager(CWnd* parentWnd /* = NULL */) : CObject()
{
	// List of control info wrappers
	controlInfoListPtr_ = NULL;

	// Parent window
	parentWndPtr_ = parentWnd;
}

/**
 * @brief	Destructor
 */
SControlManager::~SControlManager()
{
	// List of control info wrappers
	if (controlInfoListPtr_ != NULL) {
		this->deleteAll();
		delete controlInfoListPtr_;
		controlInfoListPtr_ = NULL;
	}
}

/**
 * @brief	Initialize control management
 * @param	None
 * @return	None
 */
bool SControlManager::initialize(void)
{
	// Initialize control info list pointer
	if (this->controlInfoListPtr_ == NULL) {
		this->controlInfoListPtr_ = new SCtrlInfoList();

		// Allocation failed
		if (this->controlInfoListPtr_ == NULL)
			return false;
	}

	// Empty array data
	controlInfoListPtr_->clear();

	return true;
}

/**
 * @brief	Delete all and clean-up control management
 * @param	None
 * @return	None
 */
bool SControlManager::deleteAll(void)
{
	// If data is not initialized
	if (this->controlInfoListPtr_ == NULL)
		return false;

	// Delete all control info wrapper pointers
	for (int index = 0; index < (this->controlInfoListPtr_->size()); index++) {
		SCtrlInfoWrap* pExControl = controlInfoListPtr_->at(index);
		if (pExControl != NULL) {
			delete pExControl;
			pExControl = NULL;
		}
	}

	// Empty array data
	controlInfoListPtr_->clear();

	return true;
}

/**
 * @brief	Add dialog/window control to management
 * @param	pControl - Dialog control item
 * @return	long long
 */
long long SControlManager::addControl(SCtrlInfoWrap* pControl)
{
	// Check for control pointer validity
	if (pControl == NULL) 
		return INT_INVALID;

	// If data is not initialized
	if (this->controlInfoListPtr_ == NULL)
		return INT_INVALID;

	// Search if control ID had already existed
	for (int index = 0; index < (this->controlInfoListPtr_->size()); index++) {
		SCtrlInfoWrap* pExControl = controlInfoListPtr_->at(index);
		if (pExControl == NULL) continue;
		if (pExControl->getTemplateID() == pControl->getTemplateID()) {
			// Return control index
			return index;
		}
	}

	// Add control to list and return list size
	this->controlInfoListPtr_->push_back(pControl);
	return (this->controlInfoListPtr_->size());
}

/**
 * @brief	Add dialog/window control to management
 * @param	controlId - Dialog control ID
 * @param	typeId - Control type ID
 * @return	long long
 */
long long SControlManager::addControl(unsigned controlId, unsigned typeId)
{
	// If parent window is not set, do nothing
	if (this->parentWndPtr_ == NULL)
		return INT_INVALID;

	// Get base control window pointer
	CWnd* pCtrlWnd = parentWndPtr_->GetDlgItem(controlId);
	if (pCtrlWnd == NULL)
		return INT_INVALID;

	// Initialize control info
	SCtrlInfoWrap* pControl = new SCtrlInfoWrap();
	pControl->initialize(parentWndPtr_, NULL, controlId, typeId);

	// Add control to management list
	size_t nRetIndex = this->addControl(pControl);
	if (nRetIndex != -1)
		return nRetIndex;

	// Failed to add
	delete pControl;
	return INT_INVALID;
}

/**
 * @brief	Remove dialog/window control from management
 * @param	controlId - Dialog control ID
 * @return	size_t
 */
long long SControlManager::removeControl(unsigned controlId)
{
	// If data is not initialized or is empty
	if ((controlInfoListPtr_ == NULL) || (this->isEmpty()))
		return INT_INVALID;

	// Search for control ID
	for (int index = 0; index < (controlInfoListPtr_->size()); index++) {
		SCtrlInfoWrap* pExControl = controlInfoListPtr_->at(index);
		if (pExControl == NULL) continue;
		if (pExControl->getTemplateID() == controlId) {
			delete pExControl;

			// Remove control from list
			controlInfoListPtr_->erase(controlInfoListPtr_->begin() + index);
			return (controlInfoListPtr_->size());
		}
	}

	// Control ID not found, return -1
	return INT_INVALID;
}

/**
 * @brief	Remove dialog/window control from management
 * @param	controlId - Dialog control ID
 * @return	None
 */
SCtrlInfoWrap* SControlManager::getControl(unsigned controlId)
{
	// If data is not initialized or is empty
	if ((this->controlInfoListPtr_ == NULL) || (this->isEmpty()))
		return NULL;

	// Search for control ID
	for (int index = 0; index < (this->controlInfoListPtr_->size()); index++) {
		SCtrlInfoWrap* pControl = controlInfoListPtr_->at(index);
		if (pControl == NULL) continue;
		if (pControl->getTemplateID() == controlId)
			return pControl;
	}

	return NULL;
}

/**
 * @brief	Set buddy relationship between 2 controls
 * @param	nBaseCtrlID  - Base control ID
 * @param	nBuddyCtrlID - Buddy control ID
 * @return	true/false
 */
bool SControlManager::setBuddy(unsigned nBaseCtrlID, unsigned nBuddyCtrlID)
{
	// Get base control from management list
	SCtrlInfoWrap* pBaseControl = getControl(nBaseCtrlID);
	if (pBaseControl == NULL)
		return false;

	// Get buddy control (natively)
	// No need to check NULL for parent window because if base control is found 
	// in management list, it means parent window had already been set
	CWnd* buddyWndPtr = parentWndPtr_->GetDlgItem(nBuddyCtrlID);
	if (buddyWndPtr == NULL)
		return false;

	// Set buddy relationship
	pBaseControl->setBuddy(buddyWndPtr);

	// Trigger updating control attributes
	// This will set buddy control caption as base control caption if available
	pBaseControl->updateAttributes();

	return true;
}

/**
 * @brief	Update data for specified control or all controls
 * @param	controlId - Control ID (NULL means all controls)
 * @return	None
 */
void SControlManager::updateData(unsigned controlId /* = NULL */)
{
	// If data is not initialized or is empty
	if ((this->controlInfoListPtr_ == NULL) || (this->isEmpty()))
		return;

	// If parent window is not available
	if (this->parentWndPtr_ == NULL)
		return;

	// Loop through control management list
	int nTriggerForceRetFlag = FLAG_OFF;
	for (int index = 0; index < (this->controlInfoListPtr_->size()); index++) {

		// If force return flag is ON, break the loop
		if (nTriggerForceRetFlag == FLAG_ON)
			break;

		// Get control wrapper pointer
		SCtrlInfoWrap* pCurControl = controlInfoListPtr_->at(index);
		if (pCurControl == NULL) continue;

		// Only update data for specified control
		if (controlId != NULL) {
			if (pCurControl->getTemplateID() != controlId) {
				// Skip this control
				continue;
			}
			else {
				// Trigger force return flag 
				// It will stop after updating for current control
				nTriggerForceRetFlag = FLAG_ON;
			}
		}

		// If base control is not available, skip updating
		if (!pCurControl->isBaseControlAvailable())
			continue;

		// Get base control pointer
		CWnd* pBaseControl = pCurControl->getBaseControl();

		// Update control attributes
		pCurControl->updateAttributes();

		// Update data for control by type
		switch (pCurControl->getType())
		{
			// Clickable and checkable controls
			case Button:
			case Check_Box:
			case Radio_Button:
			{
				// Update control's checked state
				bool isChecked = ((CButton*)pBaseControl)->GetCheck();
				pCurControl->setCheck(isChecked);
			} break;

			// Edit box
			case Edit_Control:
			{
				// Update control's text value
				const int textLength = ((CEdit*)pBaseControl)->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(textLength + 1);
				((CEdit*)pBaseControl)->GetWindowText(tempBuff.data(), textLength + 1);
				String tempText = tempBuff.data();
				pCurControl->setString(tempText);
			} break;

			// Combo-box
			case Combo_Box:
			{
				// Update control's current selection index
				size_t currenSelection = ((CComboBox*)pBaseControl)->GetCurSel();
				pCurControl->setInteger(currenSelection);
				// Update all item strings
				StringArray arrStringData;
				size_t count = ((CComboBox*)pBaseControl)->GetCount();
				arrStringData.reserve(count);
				for (size_t index = 0; index < count; index++) {
					wchar_t tempBuff[Constant::Max::StringLength] = {0};
					((CComboBox*)pBaseControl)->GetLBText(index, tempBuff);
					arrStringData.push_back(tempBuff);
				}
				pCurControl->setStringArray(arrStringData);
			} break;

			// List box
			case List_Box:
			{
				// Update control's current selection index
				size_t currenSelection = ((CListBox*)pBaseControl)->GetCurSel();
				pCurControl->setInteger(currenSelection);
				// Update all item strings
				StringArray arrStringData;
				size_t count = ((CListBox*)pBaseControl)->GetCount();
				arrStringData.reserve(count);
				for (size_t index = 0; index < count; index++) {
					wchar_t tempBuff[Constant::Max::StringLength] = {0};
					((CListBox*)pBaseControl)->GetText(index, tempBuff);
					arrStringData.push_back(tempBuff);
				}
				pCurControl->setStringArray(arrStringData);
			} break;

			// List control
			case List_Control:
			{
				// Update number of items and columns
				size_t itemCount = ((CListCtrl*)pBaseControl)->GetItemCount();
				pCurControl->setInteger(itemCount);
				size_t nColumnCount = 0;
				CHeaderCtrl* pHeaderCtrl = ((CListCtrl*)pBaseControl)->GetHeaderCtrl();
				if (pHeaderCtrl != NULL) {
					nColumnCount = pHeaderCtrl->GetItemCount();
				}
				pCurControl->setReserveInteger(nColumnCount);
				// Update control's data current selection index(es)
				ULongArray arrSelection;
				arrSelection.reserve(itemCount);
				for (size_t index = 0; index < itemCount; index++) {
					// Get selection index
					if ((((CListCtrl*)pBaseControl)->GetItemState(index, LVIS_SELECTED) & LVIS_SELECTED) == LVIS_SELECTED) {
						arrSelection.push_back(index);
					}
				}
				pCurControl->setIntArray(arrSelection);
				// Update all item strings
				StringArray arrStringData;
				arrStringData.reserve(itemCount);
				for (size_t index = 0; index < itemCount; index++) {
					for (size_t nColIndex = 0; nColIndex < nColumnCount; nColIndex++) {
						// Get item text
						String tempText = ((CListCtrl*)pBaseControl)->GetItemText(index, nColIndex).GetString();
						arrStringData.push_back(tempText);
					}
				}
				pCurControl->setStringArray(arrStringData);
			} break;

			// Tab control
			case Tab_Control:
			{
				// Update the number of tabs
				size_t tabCount = ((CTabCtrl*)pBaseControl)->GetItemCount();
				// Update the currently selected tab index
				size_t nCurSelTab = ((CTabCtrl*)pBaseControl)->GetCurSel();
				pCurControl->setInteger(nCurSelTab);
				// Update all tab's title
				TCITEM tabInfo;
				StringArray arrTabTitles;
				arrTabTitles.reserve(tabCount);
				for (size_t index = 0; index < tabCount; index++) {
					String tempText = Constant::String::Empty;
					bool returnFlag = ((CTabCtrl*)pBaseControl)->GetItem(index, &tabInfo);
					if (returnFlag == true && ((tabInfo.mask & TCIF_TEXT) != 0)) {
						tempText = tabInfo.pszText;
					}
					arrTabTitles.push_back(tempText);
				}
				pCurControl->setStringArray(arrTabTitles);
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
				pCurControl->setCaption(captionString);
			} break;

			// Scroll bars
			case Horizontal_Scroll_Bar:
			case Vertical_Scroll_Bar:
			{
				// Update control's current position
				size_t nCurPos = ((CScrollBar*)pBaseControl)->GetScrollPos();
				pCurControl->setInteger(nCurPos);
				// Update control's min/max range
				int nMin = NULL, nMax = NULL;
				((CScrollBar*)pBaseControl)->GetScrollRange(&nMin, &nMax);
				pCurControl->setMinMaxInt(nMin, nMax);
			} break;

			// Slider control
			case Slider_Control:
			{
				// Update control's current position
				size_t nCurPos = ((CSliderCtrl*)pBaseControl)->GetPos();
				pCurControl->setInteger(nCurPos);
				// Update control's min/max range
				size_t nMin = ((CSliderCtrl*)pBaseControl)->GetRangeMin();
				size_t nMax = ((CSliderCtrl*)pBaseControl)->GetRangeMax();
				pCurControl->setMinMaxInt(nMin, nMax);
			} break;

			// Progress bar
			case Progress_Control:
			{
				// Update control's current position
				size_t nCurPos = ((CProgressCtrl*)pBaseControl)->GetPos();
				pCurControl->setInteger(nCurPos);
				// Update control's min/max range
				int nMin = NULL, nMax = NULL;
				((CProgressCtrl*)pBaseControl)->GetRange(nMin, nMax);
				pCurControl->setMinMaxInt(nMin, nMax);
			} break;

			// Spin button control
			case Spin_Control:
			{
				// Update control's current position
				size_t nCurPos = ((CSpinButtonCtrl*)pBaseControl)->GetPos();
				pCurControl->setInteger(nCurPos);
				// Update control's min/max range
				int nMin = NULL, nMax = NULL;
				((CSpinButtonCtrl*)pBaseControl)->GetRange(nMin, nMax);
				pCurControl->setMinMaxInt(nMin, nMax);
			} break;

			// Hot key control
			case Hot_Key:
			{
				// Update control's current hotkey
				DWORD dwHotkey = ((CHotKeyCtrl*)pBaseControl)->GetHotKey();
				pCurControl->setInteger(LOWORD(dwHotkey));			// Virtual keycode
				pCurControl->setReserveInteger(HIWORD(dwHotkey));	// Modifier flags
			} break;

			// IP address control
			case IP_Address_Control:
			{
				// Update control's current IP address
				DWORD dwAddress = 0;
				byte byField0 = 0, byField1 = 0, byField2 = 0, byField3 = 0;
				int nNonBlankFieldNum = ((CIPAddressCtrl*)pBaseControl)->GetAddress(dwAddress);
				((CIPAddressCtrl*)pBaseControl)->GetAddress(byField0, byField1, byField2, byField3);
				pCurControl->setInteger(dwAddress);
				pCurControl->setReserveInteger(nNonBlankFieldNum);
				// Store each field value separately into an integer array
				ULongArray arrAddressFields;
				arrAddressFields.resize(4);
				arrAddressFields[0] = byField0;		// Field 0
				arrAddressFields[1] = byField1;		// Field 1
				arrAddressFields[2] = byField2;		// Field 2
				arrAddressFields[3] = byField3;		// Field 3
				pCurControl->setIntArray(arrAddressFields);
			} break;

			// Network address control
			case Network_Address_Control:
			{
				// Update control's current network address
				NC_ADDRESS ncAddress;
				NET_ADDRESS_INFO netAddressInfo;
				ncAddress.pAddrInfo = &netAddressInfo;
				HRESULT resourceHandle = ((CNetAddressCtrl*)pBaseControl)->GetAddress(&ncAddress);
				if (resourceHandle == S_OK) {
					// Save address and port info
					String addressString = ncAddress.pAddrInfo->NamedAddress.Address;
					String portString = ncAddress.pAddrInfo->NamedAddress.Port;
					pCurControl->setString(addressString);
					pCurControl->setReserveString(portString);
					// Numeric data
					pCurControl->setInteger(ncAddress.PortNumber);
					pCurControl->setReserveInteger(ncAddress.PrefixLength);
				}
			} break;

			// Date time picker
			case Date_Time_Picker:
			{
				// Update control's date/time value
				SYSTEMTIME timeTemp{};
				((CDateTimeCtrl*)pBaseControl)->GetTime(&timeTemp);
				pCurControl->setTime(timeTemp);
			} break;

			// Month calendar control
			case Month_Calendar_Control:
			{
				// Update control's current selected date
				SYSTEMTIME dateTemp{};
				((CMonthCalCtrl*)pBaseControl)->GetCurSel(&dateTemp);
				pCurControl->setTime(dateTemp);
			} break;
		}
	}
}

