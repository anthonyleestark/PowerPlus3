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
	baseControlWndPtr_ = parentWndPtr->GetDlgItem(controlId);

	// Set relative windows
	parentWndPtr_ = parentWndPtr;
	buddyWndPtr_ = buddyWndPtr;

	// Set control ID info
	templateId_ = controlId;
	typeId_ = typeId;
	templateStringId_ = MAKEUNICODE(GET_NAME_ID(templateId_));

	// Set control attributes
	updateAttributes();

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
	switch (typeId_)
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
				const int captionLength = getBaseControl()->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(captionLength + 1);
				getBaseControl()->GetWindowText(tempBuff.data(), captionLength + 1);
				String captionString = tempBuff.data();
				setCaption(captionString);
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
				const int captionLength = getBuddy()->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(captionLength + 1);
				getBuddy()->GetWindowText(tempBuff.data(), captionLength + 1);
				String captionString = tempBuff.data();
				setCaption(captionString);
			}
		} break;

		// The remaining types of control have no caption implemented:
		// Skip by default
		default:
			break;
	}

	// Update control displaying state
	setVisibleState(false);		// Reset state
	setEnableState(false);		// Reset state
	setFocusedState(false);		// Reset state

	if (isBaseControlAvailable()) {

		// Update visible state
		setVisibleState(getBaseControl()->IsWindowVisible());

		// Update enable state
		setEnableState(getBaseControl()->IsWindowEnabled());

		// Update focus state
		if (isParentAvailable()) {
			CWnd* focusedCtrlPtr = CWnd::GetFocus();
			if (focusedCtrlPtr
				&& (focusedCtrlPtr->GetParent() == parentWndPtr_)
				&& (focusedCtrlPtr == getBaseControl())) {
				setFocusedState(true);
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
	if (isCheckedPtr_ == NULL)
		return false;
	else
		return *(isCheckedPtr_);
}

/**
 * @brief	Get current control's integer data value
 * @param	value - Integer value (out)
 * @return	int64
 */
int64 SCtrlInfoWrap::getInteger(void) const
{
	if (valueIntPtr_ == NULL)
		return kInvalidInteger;
	else
		return *(valueIntPtr_);
}

void SCtrlInfoWrap::getInteger(_Out_ int64& value) const
{
	if (valueIntPtr_ == NULL)
		value = kInvalidInteger;
	else
		value = *(valueIntPtr_);
}

/**
 * @brief	Get current control's reserve integer data value
 * @param	value - Integer reserve value (out)
 * @return	int64
 */
int64 SCtrlInfoWrap::getReserveInteger(void) const
{
	if (reservedValueIntPtr_ == NULL)
		return kInvalidInteger;
	else
		return *(reservedValueIntPtr_);
}

void SCtrlInfoWrap::getReserveInteger(_Out_ int64& value) const
{
	if (reservedValueIntPtr_ == NULL)
		value = kInvalidInteger;
	else
		value = *(reservedValueIntPtr_);
}

/**
 * @brief	Get current control's min/max range integer data value
 * @param	minVal - Min range integer value (out)
 * @param	maxVal - Max range integer value (out)
 * @return	None
 */
void SCtrlInfoWrap::getMinMaxInt(_Out_ int64& minVal, _Out_ int64& maxVal) const
{
	// Min value
	if (minValueIntPtr_ == NULL)
		minVal = kInvalidInteger;
	else
		minVal = *(minValueIntPtr_);

	// Max value
	if (maxValueIntPtr_ == NULL)
		maxVal = kInvalidInteger;
	else
		maxVal = *(maxValueIntPtr_);
}

/**
 * @brief	Get current control's float data value
 * @param	valueDouble - Float value (out)
 * @return	double
 */
double SCtrlInfoWrap::getFloat(void) const
{
	if (valueDoublePtr_ == NULL)
		return kInvalidFloat;
	else
		return *(valueDoublePtr_);
}

void SCtrlInfoWrap::getFloat(_Out_ double& valueDouble) const
{
	if (valueDoublePtr_ == NULL)
		valueDouble = kInvalidFloat;
	else
		valueDouble = *(valueDoublePtr_);
}

/**
 * @brief	Get current control's reserve float data value
 * @param	valueDouble - Float reserve value (out)
 * @return	double
 */
double SCtrlInfoWrap::getReserveFloat(void) const
{
	if (reservedValueDoublePtr_ == NULL)
		return kInvalidFloat;
	else
		return *(reservedValueDoublePtr_);
}

void SCtrlInfoWrap::getReserveFloat(_Out_ double& valueDouble) const
{
	if (reservedValueDoublePtr_ == NULL)
		valueDouble = kInvalidFloat;
	else
		valueDouble = *(reservedValueDoublePtr_);
}

/**
 * @brief	Get current control's min/max range integer data value
 * @param	minVal - Min range integer value (out)
 * @param	maxVal - Max range integer value (out)
 * @return	None
 */
void SCtrlInfoWrap::getMinMaxFloat(_Out_ double& minVal, _Out_ double& maxVal) const
{
	// Min value
	if (minValueDoublePtr_ == NULL)
		minVal = kInvalidFloat;
	else
		minVal = *(minValueDoublePtr_);

	// Max value
	if (maxValueDoublePtr_ == NULL)
		maxVal = kInvalidFloat;
	else
		maxVal = *(maxValueDoublePtr_);
}

/**
 * @brief	Get current control's string data value
 * @param	value - String value (out)
 * @return	const wchar_t*
 */
const wchar_t* SCtrlInfoWrap::getString(void) const
{
	if (valueStringPtr_ == NULL)
		return Constant::String::Empty;
	else
		return *(valueStringPtr_);
}

void SCtrlInfoWrap::getString(_Out_ String& value) const
{
	if (valueStringPtr_ == NULL)
		value = Constant::String::Empty;
	else
		value = *(valueStringPtr_);
}

/**
 * @brief	Get current control's reserve string data value
 * @param	value - String reserve value (out)
 * @return	const wchar_t*
 */
const wchar_t* SCtrlInfoWrap::getReserveString(void) const
{
	if (reservedValueStringPtr_ == NULL)
		return Constant::String::Empty;
	else
		return *(reservedValueStringPtr_);
}

void SCtrlInfoWrap::getReserveString(_Out_ String& value) const
{
	if (reservedValueStringPtr_ == NULL)
		value = Constant::String::Empty;
	else
		value = *(reservedValueStringPtr_);
}

/**
 * @brief	Get current control's integer array data value
 * @param	valueList - Integer array value (out)
 * @return	None
 */
void SCtrlInfoWrap::getIntArray(_Out_ uint64_array& valueList) const
{
	if (valueIntListPtr_ == NULL) {
		valueList.clear();
	}
	else {
		valueList = *(valueIntListPtr_);
	}
}

/**
 * @brief	Get current control's reserve integer array data value
 * @param	valueList - Integer array value (out)
 * @return	None
 */
void SCtrlInfoWrap::getReserveIntArray(_Out_ uint64_array& valueList) const
{
	if (reservedValueIntListPtr_ == NULL) {
		valueList.clear();
	}
	else {
		valueList = *(reservedValueIntListPtr_);
	}
}

/**
 * @brief	Get current control's string array data value
 * @param	valueList - String array value (out)
 * @return	None
 */
void SCtrlInfoWrap::getStringArray(_Out_ StringArray& valueList) const
{
	if (valueStringListPtr_ == NULL) {
		valueList.clear();
	}
	else {
		valueList = *(valueStringListPtr_);
	}
}

/**
 * @brief	Get current control's reserve string array data value
 * @param	valueList - String array value (out)
 * @return	None
 */
void SCtrlInfoWrap::getReserveStringArray(_Out_ StringArray& valueList) const
{
	if (reservedValueStringListPtr_ == NULL) {
		valueList.clear();
	}
	else {
		valueList = *(reservedValueStringListPtr_);
	}
}

/**
 * @brief	Get current control's time data value
 * @param	timeValue - Time value (out)
 * @return	SYSTEMTIME
 */
SYSTEMTIME SCtrlInfoWrap::getTime(void) const
{
	if (valueTimePtr_ == NULL)
		return {0};
	else
		return *(valueTimePtr_);
}

void SCtrlInfoWrap::getTime(_Out_ SYSTEMTIME& timeValue) const
{
	if (valueTimePtr_ == NULL)
		timeValue = {0};
	else
		timeValue = *(valueTimePtr_);
}

/**
 * @brief	Set current control's check state
 * @param	isChecked - Checked state (BOOLEAN)
 * @return	None
 */
void SCtrlInfoWrap::setCheck(_In_ const bool& isChecked)
{
	if (isCheckedPtr_ == NULL)
		isCheckedPtr_ = new bool(isChecked);
	else {
		delete (isCheckedPtr_);
		isCheckedPtr_ = new bool(isChecked);
	}
}

/**
 * @brief	Set current control's integer data value
 * @param	value - Integer value (in)
 * @return	None
 */
void SCtrlInfoWrap::setInteger(_In_ const int64& value)
{
	if (valueIntPtr_ == NULL)
		valueIntPtr_ = new int64(value);
	else {
		delete (valueIntPtr_);
		valueIntPtr_ = new int64(value);
	}
}

/**
 * @brief	Set current control's reserve integer data value
 * @param	value - Integer reserve value (in)
 * @return	None
 */
void SCtrlInfoWrap::setReserveInteger(_In_ const int64& value)
{
	if (reservedValueIntPtr_ == NULL)
		reservedValueIntPtr_ = new int64(value);
	else {
		delete (reservedValueIntPtr_);
		reservedValueIntPtr_ = new int64(value);
	}
}

/**
 * @brief	Set current control's min/max range integer data value
 * @param	minVal - Min range integer value (in)
 * @param	maxVal - Max range integer value (in)
 * @return	None
 */
void SCtrlInfoWrap::setMinMaxInt(_In_ const int64& minVal, _In_ const int64& maxVal)
{
	// Min value
	if (minValueIntPtr_ == NULL)
		minValueIntPtr_ = new int64(minVal);
	else {
		delete (minValueIntPtr_);
		minValueIntPtr_ = new int64(minVal);
	}

	// Max value
	if (maxValueIntPtr_ == NULL)
		maxValueIntPtr_ = new int64(maxVal);
	else {
		delete (maxValueIntPtr_);
		maxValueIntPtr_ = new int64(maxVal);
	}
}

/**
 * @brief	Set current control's float data value
 * @param	valueDouble - Float value (in)
 * @return	None
 */
void SCtrlInfoWrap::setFloat(_In_ const double& valueDouble)
{
	if (valueDoublePtr_ == NULL)
		valueDoublePtr_ = new double(valueDouble);
	else {
		delete (valueDoublePtr_);
		valueDoublePtr_ = new double(valueDouble);
	}
}

/**
 * @brief	Set current control's reserve float data value
 * @param	valueDouble - Float reserve value (in)
 * @return	None
 */
void SCtrlInfoWrap::setReserveFloat(_In_ const double& valueDouble)
{
	if (reservedValueDoublePtr_ == NULL)
		reservedValueDoublePtr_ = new double(valueDouble);
	else {
		delete (reservedValueDoublePtr_);
		reservedValueDoublePtr_ = new double(valueDouble);
	}
}

/**
 * @brief	Set current control's min/max range float data value
 * @param	minVal - Min range integer value (in)
 * @param	maxVal - Max range integer value (in)
 * @return	None
 */
void SCtrlInfoWrap::setMinMaxFloat(_In_ const double& minVal, _In_ const double& maxVal)
{
	// Min value
	if (minValueDoublePtr_ == NULL)
		minValueDoublePtr_ = new double(minVal);
	else {
		delete (minValueDoublePtr_);
		minValueDoublePtr_ = new double(minVal);
	}

	// Max value
	if (maxValueDoublePtr_ == NULL)
		maxValueDoublePtr_ = new double(maxVal);
	else {
		delete (maxValueDoublePtr_);
		maxValueDoublePtr_ = new double(maxVal);
	}
}

/**
 * @brief	Set current control's string data value
 * @param	value - String value (in)
 * @return	None
 */
void SCtrlInfoWrap::setString(_In_ const wchar_t* value)
{
	if (valueStringPtr_ == NULL)
		valueStringPtr_ = new String(value);
	else {
		delete (valueStringPtr_);
		valueStringPtr_ = new String(value);
	}
}

/**
 * @brief	Set current control's reserve string data value
 * @param	value - String reserve value (in)
 * @return	None
 */
void SCtrlInfoWrap::setReserveString(_In_ const wchar_t* value)
{
	if (reservedValueStringPtr_ == NULL)
		reservedValueStringPtr_ = new String(value);
	else {
		delete (reservedValueStringPtr_);
		reservedValueStringPtr_ = new String(value);
	}
}

/**
 * @brief	Set current control's integer array data value
 * @param	valueList - Integer array value (in)
 * @return	None
 */
void SCtrlInfoWrap::setIntArray(_In_ const uint64_array& valueList)
{
	if (valueIntListPtr_ == NULL)
		valueIntListPtr_ = new uint64_array();
	
	if (valueIntListPtr_ != NULL) {
		valueIntListPtr_->clear();
		valueIntListPtr_->assign(valueList.begin(), valueList.end());
	}
}

/**
 * @brief	Set current control's reserve integer array data value
 * @param	valueList - Integer array value (in)
 * @return	None
 */
void SCtrlInfoWrap::setReserveIntArray(_In_ const uint64_array& valueList)
{
	if (reservedValueIntListPtr_ == NULL)
		reservedValueIntListPtr_ = new uint64_array();

	if (reservedValueIntListPtr_ != NULL) {
		reservedValueIntListPtr_->clear();
		reservedValueIntListPtr_->assign(valueList.begin(), valueList.end());
	}
}

/**
 * @brief	Set current control's string array data value
 * @param	valueList - String array value (in)
 * @return	None
 */
void SCtrlInfoWrap::setStringArray(_In_ const StringArray& valueList)
{
	if (valueStringListPtr_ == NULL)
		valueStringListPtr_ = new StringArray();

	if (valueStringListPtr_ != NULL) {
		valueStringListPtr_->clear();
		valueStringListPtr_->assign(valueList.begin(), valueList.end());
	}
}

/**
 * @brief	Set current control's reserve string array data value
 * @param	valueList - String array value (in)
 * @return	None
 */
void SCtrlInfoWrap::setReserveStringArray(_In_ const StringArray& valueList)
{
	if (reservedValueStringListPtr_ == NULL)
		reservedValueStringListPtr_ = new StringArray();

	if (reservedValueStringListPtr_ != NULL) {
		reservedValueStringListPtr_->clear();
		reservedValueStringListPtr_->assign(valueList.begin(), valueList.end());
	}
}

/**
 * @brief	Set current control's time data value
 * @param	timeValue - Time value (in)
 * @return	None
 */
void SCtrlInfoWrap::setTime(_In_ const SYSTEMTIME& timeValue)
{
	if (valueTimePtr_ == NULL)
		valueTimePtr_ = new SYSTEMTIME(timeValue);
	else {
		delete (valueTimePtr_);
		valueTimePtr_ = new SYSTEMTIME(timeValue);
	}
}

/**
 * @brief	Get current control's custom data pointer
 * @param	outputPtr   - Output data pointer
 * @param	dataSize - Data's total size in bytes (in/out)
 * @return	true/false
 */
template<typename DataType>
bool SCtrlInfoWrap::getData(_Outptr_ DataType*& outputPtr, _Inout_opt_z_ size_t& dataSize) const
{
	// If pointers are invalid
	if ((outputPtr == NULL) ||
		(customDataPtr_ == NULL) ||
		(dataSizePtr_ == NULL))
		return false;	// Fail to retrieve data

	// If size value is invalid
	size_t curDataSize = *(dataSizePtr_);
	if ((dataSize <= 0) || (curDataSize <= 0))
		return false;	// Fail to retrieve data

	// If the output buffer size is insufficient
	if (dataSize < curDataSize) {
		// Re-allocate output data pointer
		// we can 
		delete[] outputPtr;
		outputPtr = new DataType[curDataSize / sizeof(DataType)];
		ASSERT(outputPtr != NULL);
		if (outputPtr == NULL) {
			throw std::bad_alloc();
			return false;	// Allocation failed
		}
	}

	// Copy data and update the output data size
	memcpy(outputPtr, customDataPtr_, curDataSize);
	dataSize = curDataSize;

	return true;	// Get data successfully
}

/**
 * @brief	Set current control's custom data pointer
 * @param	inputPtr - Input data pointer
 * @param	dataSize - Data's total size in bytes (in)
 * @return	None
 */
template<typename DataType>
bool SCtrlInfoWrap::setData(_In_ const DataType* inputPtr, _In_ const size_t& dataSize)
{
	// If input data or size are invalid
	if ((inputPtr == NULL) || (dataSize <= 0))
		return false;	// Fail to set data

	// If the current data pointer is not empty
	// or its current total size in bytes is not large enough
	if ((customDataPtr_ != NULL) ||
		((dataSizePtr_ != NULL) && (*(dataSizePtr_) < dataSize))) {
		if (customDataPtr_ != NULL) {
			// Free the data pointer's existing memory
			free(customDataPtr_);
		}
	}

	// Allocate the data pointer if not available
	if (customDataPtr_ == NULL) {
		customDataPtr_ = malloc(dataSize);
		ASSERT(customDataPtr_ != NULL);
		if (customDataPtr_ == NULL) {
			throw std::bad_alloc();
			return false;	// Allocation failed
		}
	}

	// Allocate the data size pointer if not available
	if (dataSizePtr_ == NULL) {
		dataSizePtr_ = new size_t;
		ASSERT(dataSizePtr_ != NULL);
		if (dataSizePtr_ == NULL) {
			throw std::bad_alloc();
			return false;	// Allocation failed
		}
	}

	// Copy data
	memcpy(customDataPtr_, inputPtr, dataSize);

	// Update the data's new total size in bytes
	*(dataSizePtr_) = dataSize;

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
	if (customDataPtr_ == NULL)
		return true;

	// If the data size is not available or invalid,
	// the data can be considered empty
	if ((dataSizePtr_ == NULL) ||
		(dataSizePtr_ != NULL) && *(dataSizePtr_) <= 0)
		return true;

	return false;	// Not empty
}

/**
 * @brief	Get total size in bytes of the current control's custom data
 * @param	None
 * @return	size_t
 */
size_t SCtrlInfoWrap::getDataSize(void) const
{
	// If the data is empty, return 0
	if (isDataEmpty())
		return kNullInteger;

	// Otherwise, return the size
	if (dataSizePtr_ != NULL)
		return *(dataSizePtr_);

	return kNullInteger;	// Default, return 0
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
		deleteAll();
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
	if (controlInfoListPtr_ == NULL) {
		controlInfoListPtr_ = new SCtrlInfoList();

		// Allocation failed
		if (controlInfoListPtr_ == NULL)
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
	if (controlInfoListPtr_ == NULL)
		return false;

	// Delete all control info wrapper pointers
	for (int index = 0; index < (controlInfoListPtr_->size()); index++) {
		SCtrlInfoWrap* controlWrapPtr = controlInfoListPtr_->at(index);
		if (controlWrapPtr != NULL) {
			delete controlWrapPtr;
			controlWrapPtr = NULL;
		}
	}

	// Empty array data
	controlInfoListPtr_->clear();

	return true;
}

/**
 * @brief	Add dialog/window control to management
 * @param	pControl - Dialog control item
 * @return	int64
 */
int64 SControlManager::addControl(SCtrlInfoWrap* pControl)
{
	// Check for control pointer validity
	if (pControl == NULL) 
		return kInvalidInteger;

	// If data is not initialized
	if (controlInfoListPtr_ == NULL)
		return kInvalidInteger;

	// Search if control ID had already existed
	for (int index = 0; index < (controlInfoListPtr_->size()); index++) {
		SCtrlInfoWrap* controlWrapPtr = controlInfoListPtr_->at(index);
		if (controlWrapPtr == NULL) continue;
		if (controlWrapPtr->getTemplateID() == pControl->getTemplateID()) {
			// Return control index
			return index;
		}
	}

	// Add control to list and return list size
	controlInfoListPtr_->push_back(pControl);
	return (controlInfoListPtr_->size());
}

/**
 * @brief	Add dialog/window control to management
 * @param	controlId - Dialog control ID
 * @param	typeId - Control type ID
 * @return	int64
 */
int64 SControlManager::addControl(unsigned controlId, unsigned typeId)
{
	// If parent window is not set, do nothing
	if (parentWndPtr_ == NULL)
		return kInvalidInteger;

	// Get base control window pointer
	CWnd* controlWndPtr = parentWndPtr_->GetDlgItem(controlId);
	if (controlWndPtr == NULL)
		return kInvalidInteger;

	// Initialize control info
	SCtrlInfoWrap* controlWrapPtr = new SCtrlInfoWrap();
	controlWrapPtr->initialize(parentWndPtr_, NULL, controlId, typeId);

	// Add control to management list
	size_t retIndex = addControl(controlWrapPtr);
	if (retIndex != -1)
		return retIndex;

	// Failed to add
	delete controlWrapPtr;
	return kInvalidInteger;
}

/**
 * @brief	Remove dialog/window control from management
 * @param	controlId - Dialog control ID
 * @return	int64
 */
int64 SControlManager::removeControl(unsigned controlId)
{
	// If data is not initialized or is empty
	if ((controlInfoListPtr_ == NULL) || (isEmpty()))
		return kInvalidInteger;

	// Search for control ID
	for (int index = 0; index < (controlInfoListPtr_->size()); index++) {
		SCtrlInfoWrap* controlWrapPtr = controlInfoListPtr_->at(index);
		if (controlWrapPtr == NULL) continue;
		if (controlWrapPtr->getTemplateID() == controlId) {
			delete controlWrapPtr;

			// Remove control from list
			controlInfoListPtr_->erase(controlInfoListPtr_->begin() + index);
			return (controlInfoListPtr_->size());
		}
	}

	// Control ID not found, return -1
	return kInvalidInteger;
}

/**
 * @brief	Remove dialog/window control from management
 * @param	controlId - Dialog control ID
 * @return	None
 */
SCtrlInfoWrap* SControlManager::getControl(unsigned controlId)
{
	// If data is not initialized or is empty
	if ((controlInfoListPtr_ == NULL) || (isEmpty()))
		return NULL;

	// Search for control ID
	for (int index = 0; index < (controlInfoListPtr_->size()); index++) {
		SCtrlInfoWrap* controlWrapPtr = controlInfoListPtr_->at(index);
		if (controlWrapPtr == NULL) continue;
		if (controlWrapPtr->getTemplateID() == controlId)
			return controlWrapPtr;
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
	SCtrlInfoWrap* baseControlPtr = getControl(nBaseCtrlID);
	if (baseControlPtr == NULL)
		return false;

	// Get buddy control (natively)
	// No need to check NULL for parent window because if base control is found 
	// in management list, it means parent window had already been set
	CWnd* buddyWndPtr = parentWndPtr_->GetDlgItem(nBuddyCtrlID);
	if (buddyWndPtr == NULL)
		return false;

	// Set buddy relationship
	baseControlPtr->setBuddy(buddyWndPtr);

	// Trigger updating control attributes
	// This will set buddy control caption as base control caption if available
	baseControlPtr->updateAttributes();

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
	if ((controlInfoListPtr_ == NULL) || (isEmpty()))
		return;

	// If parent window is not available
	if (parentWndPtr_ == NULL)
		return;

	// Loop through control management list
	int triggerForceReturnFlag = FLAG_OFF;
	for (int index = 0; index < (controlInfoListPtr_->size()); index++) {

		// If force return flag is ON, break the loop
		if (triggerForceReturnFlag == FLAG_ON)
			break;

		// Get control wrapper pointer
		SCtrlInfoWrap* currentControlPtr = controlInfoListPtr_->at(index);
		if (currentControlPtr == NULL) continue;

		// Only update data for specified control
		if (controlId != NULL) {
			if (currentControlPtr->getTemplateID() != controlId) {
				// Skip this control
				continue;
			}
			else {
				// Trigger force return flag 
				// It will stop after updating for current control
				triggerForceReturnFlag = FLAG_ON;
			}
		}

		// If base control is not available, skip updating
		if (!currentControlPtr->isBaseControlAvailable())
			continue;

		// Get base control pointer
		CWnd* baseControlPtr = currentControlPtr->getBaseControl();

		// Update control attributes
		currentControlPtr->updateAttributes();

		// Update data for control by type
		switch (currentControlPtr->getType())
		{
			// Clickable and checkable controls
			case Button:
			case Check_Box:
			case Radio_Button:
			{
				// Update control's checked state
				bool isChecked = ((CButton*)baseControlPtr)->GetCheck();
				currentControlPtr->setCheck(isChecked);
			} break;

			// Edit box
			case Edit_Control:
			{
				// Update control's text value
				const int textLength = ((CEdit*)baseControlPtr)->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(textLength + 1);
				((CEdit*)baseControlPtr)->GetWindowText(tempBuff.data(), textLength + 1);
				String tempText = tempBuff.data();
				currentControlPtr->setString(tempText);
			} break;

			// Combo-box
			case Combo_Box:
			{
				// Update control's current selection index
				size_t currenSelection = ((CComboBox*)baseControlPtr)->GetCurSel();
				currentControlPtr->setInteger(currenSelection);
				// Update all item strings
				StringArray stringData;
				size_t count = ((CComboBox*)baseControlPtr)->GetCount();
				stringData.reserve(count);
				for (size_t index = 0; index < count; index++) {
					wchar_t tempBuff[Constant::Max::StringLength] = {0};
					((CComboBox*)baseControlPtr)->GetLBText(index, tempBuff);
					stringData.push_back(tempBuff);
				}
				currentControlPtr->setStringArray(stringData);
			} break;

			// List box
			case List_Box:
			{
				// Update control's current selection index
				size_t currenSelection = ((CListBox*)baseControlPtr)->GetCurSel();
				currentControlPtr->setInteger(currenSelection);
				// Update all item strings
				StringArray stringData;
				size_t count = ((CListBox*)baseControlPtr)->GetCount();
				stringData.reserve(count);
				for (size_t index = 0; index < count; index++) {
					wchar_t tempBuff[Constant::Max::StringLength] = {0};
					((CListBox*)baseControlPtr)->GetText(index, tempBuff);
					stringData.push_back(tempBuff);
				}
				currentControlPtr->setStringArray(stringData);
			} break;

			// List control
			case List_Control:
			{
				// Update number of items and columns
				size_t itemCount = ((CListCtrl*)baseControlPtr)->GetItemCount();
				currentControlPtr->setInteger(itemCount);
				size_t columnCount = 0;
				CHeaderCtrl* headerCtrlPtr = ((CListCtrl*)baseControlPtr)->GetHeaderCtrl();
				if (headerCtrlPtr != NULL) {
					columnCount = headerCtrlPtr->GetItemCount();
				}
				currentControlPtr->setReserveInteger(columnCount);
				// Update control's data current selection index(es)
				uint64_array selectionList;
				selectionList.reserve(itemCount);
				for (size_t index = 0; index < itemCount; index++) {
					// Get selection index
					if ((((CListCtrl*)baseControlPtr)->GetItemState(index, LVIS_SELECTED) & LVIS_SELECTED) == LVIS_SELECTED) {
						selectionList.push_back(index);
					}
				}
				currentControlPtr->setIntArray(selectionList);
				// Update all item strings
				StringArray stringData;
				stringData.reserve(itemCount);
				for (size_t index = 0; index < itemCount; index++) {
					for (size_t colIndex = 0; colIndex < columnCount; colIndex++) {
						// Get item text
						String tempText = ((CListCtrl*)baseControlPtr)->GetItemText(index, colIndex).GetString();
						stringData.push_back(tempText);
					}
				}
				currentControlPtr->setStringArray(stringData);
			} break;

			// Tab control
			case Tab_Control:
			{
				// Update the number of tabs
				size_t tabCount = ((CTabCtrl*)baseControlPtr)->GetItemCount();
				// Update the currently selected tab index
				size_t curSelTab = ((CTabCtrl*)baseControlPtr)->GetCurSel();
				currentControlPtr->setInteger(curSelTab);
				// Update all tab's title
				TCITEM tabInfo;
				StringArray tabTitleList;
				tabTitleList.reserve(tabCount);
				for (size_t index = 0; index < tabCount; index++) {
					String tempText = Constant::String::Empty;
					bool returnFlag = ((CTabCtrl*)baseControlPtr)->GetItem(index, &tabInfo);
					if (returnFlag == true && ((tabInfo.mask & TCIF_TEXT) != 0)) {
						tempText = tabInfo.pszText;
					}
					tabTitleList.push_back(tempText);
				}
				currentControlPtr->setStringArray(tabTitleList);
			} break;

			// Static text and decorating items
			case Static_Text:
			case Group_Box:
			case SysLink_Control:
			{
				// Update control's text label
				const int textLength = baseControlPtr->GetWindowTextLength();
				std::vector<wchar_t> tempBuff(textLength + 1);
				baseControlPtr->GetWindowText(tempBuff.data(), textLength + 1);
				String captionString = tempBuff.data();
				currentControlPtr->setCaption(captionString);
			} break;

			// Scroll bars
			case Horizontal_Scroll_Bar:
			case Vertical_Scroll_Bar:
			{
				// Update control's current position
				size_t currentPosition = ((CScrollBar*)baseControlPtr)->GetScrollPos();
				currentControlPtr->setInteger(currentPosition);
				// Update control's min/max range
				int minVal = NULL, maxVal = NULL;
				((CScrollBar*)baseControlPtr)->GetScrollRange(&minVal, &maxVal);
				currentControlPtr->setMinMaxInt(minVal, maxVal);
			} break;

			// Slider control
			case Slider_Control:
			{
				// Update control's current position
				size_t currentPosition = ((CSliderCtrl*)baseControlPtr)->GetPos();
				currentControlPtr->setInteger(currentPosition);
				// Update control's min/max range
				size_t minVal = ((CSliderCtrl*)baseControlPtr)->GetRangeMin();
				size_t maxVal = ((CSliderCtrl*)baseControlPtr)->GetRangeMax();
				currentControlPtr->setMinMaxInt(minVal, maxVal);
			} break;

			// Progress bar
			case Progress_Control:
			{
				// Update control's current position
				size_t currentPosition = ((CProgressCtrl*)baseControlPtr)->GetPos();
				currentControlPtr->setInteger(currentPosition);
				// Update control's min/max range
				int minVal = NULL, maxVal = NULL;
				((CProgressCtrl*)baseControlPtr)->GetRange(minVal, maxVal);
				currentControlPtr->setMinMaxInt(minVal, maxVal);
			} break;

			// Spin button control
			case Spin_Control:
			{
				// Update control's current position
				size_t currentPosition = ((CSpinButtonCtrl*)baseControlPtr)->GetPos();
				currentControlPtr->setInteger(currentPosition);
				// Update control's min/max range
				int minVal = NULL, maxVal = NULL;
				((CSpinButtonCtrl*)baseControlPtr)->GetRange(minVal, maxVal);
				currentControlPtr->setMinMaxInt(minVal, maxVal);
			} break;

			// Hot key control
			case Hot_Key:
			{
				// Update control's current hotkey
				DWORD hotkey = ((CHotKeyCtrl*)baseControlPtr)->GetHotKey();
				currentControlPtr->setInteger(LOWORD(hotkey));			// Virtual keycode
				currentControlPtr->setReserveInteger(HIWORD(hotkey));	// Modifier flags
			} break;

			// IP address control
			case IP_Address_Control:
			{
				// Update control's current IP address
				DWORD address = 0;
				byte field0 = 0, field1 = 0, field2 = 0, field3 = 0;
				int nonBlankFieldCount = ((CIPAddressCtrl*)baseControlPtr)->GetAddress(address);
				((CIPAddressCtrl*)baseControlPtr)->GetAddress(field0, field1, field2, field3);
				currentControlPtr->setInteger(address);
				currentControlPtr->setReserveInteger(nonBlankFieldCount);
				// Store each field value separately into an integer array
				uint64_array addressFieldArray;
				addressFieldArray.resize(4);
				addressFieldArray[0] = field0;		// Field 0
				addressFieldArray[1] = field1;		// Field 1
				addressFieldArray[2] = field2;		// Field 2
				addressFieldArray[3] = field3;		// Field 3
				currentControlPtr->setIntArray(addressFieldArray);
			} break;

			// Network address control
			case Network_Address_Control:
			{
				// Update control's current network address
				NC_ADDRESS address;
				NET_ADDRESS_INFO netAddressInfo;
				address.pAddrInfo = &netAddressInfo;
				HRESULT resourceHandle = ((CNetAddressCtrl*)baseControlPtr)->GetAddress(&address);
				if (resourceHandle == S_OK) {
					// Save address and port info
					String addressString = address.pAddrInfo->NamedAddress.Address;
					String portString = address.pAddrInfo->NamedAddress.Port;
					currentControlPtr->setString(addressString);
					currentControlPtr->setReserveString(portString);
					// Numeric data
					currentControlPtr->setInteger(address.PortNumber);
					currentControlPtr->setReserveInteger(address.PrefixLength);
				}
			} break;

			// Date time picker
			case Date_Time_Picker:
			{
				// Update control's date/time value
				SYSTEMTIME timeTemp{};
				((CDateTimeCtrl*)baseControlPtr)->GetTime(&timeTemp);
				currentControlPtr->setTime(timeTemp);
			} break;

			// Month calendar control
			case Month_Calendar_Control:
			{
				// Update control's current selected date
				SYSTEMTIME dateTemp{};
				((CMonthCalCtrl*)baseControlPtr)->GetCurSel(&dateTemp);
				currentControlPtr->setTime(dateTemp);
			} break;
		}
	}
}

