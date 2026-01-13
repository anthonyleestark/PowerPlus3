/**
 * @file		SElements.h
 * @brief		Custom base class for dialog elements (header file)
 * @author		AnthonyLeeStark
 * @date		2024.12.24
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/IDManager.h"


// Define dialog/window control types
enum ControlType {
	Control_Base = 0x00,			// Base control type (NULL)

	Button,							// Push Button
	Check_Box,						// Check Box
	Edit_Control,					// Edit Control
	Combo_Box,						// Combo Box
	List_Box,						// List Box
	Group_Box,						// Group Box
	Radio_Button,					// Radio Button
	Static_Text,					// Static Text
	Picture_Control,				// Picture Control
	Horizontal_Scroll_Bar,			// Horizontal Scroll Bar
	Vertical_Scroll_Bar,			// Vertical Scroll Bar
	Slider_Control,					// Slider Control
	Spin_Control,					// Spin Control
	Progress_Control,				// Progress Control
	Hot_Key,						// Hot Key
	List_Control,					// List Control
	Tree_Control,					// Tree Control
	Tab_Control,					// Tab Control
	Animation_Control,				// Animation_Control
	Rich_Edit_Control,				// Rich Edit 2.0 Control
	Date_Time_Picker,				// Date Time Picker
	Month_Calendar_Control,			// Month Calendar Control
	IP_Address_Control,				// IP Address Control
	Extended_Combo_Box,				// Extended Combo Box
	Custom_Control,					// Custom Control
	SysLink_Control,				// SysLink Control
	Split_Button_Control,			// Split Button Control
	Network_Address_Control,		// Network Address Control
	Command_Button_Control,			// Command Button Control

	MFC_Button_Control,				// MFC Button Control
	MFC_ColorButton_Control,		// MFC ColorButton Control
	MFC_EditBrowse_Control,			// MFC EditBrowse Control
	MFC_VSListBox_Control,			// MFC VSListBox Control
	MFC_FontComboBox_Control,		// MFC FontComboBox Control
	MFC_MaskedEdit_Control,			// MFC MaskedEdit Control
	MFC_MenuButton_Control,			// MFC MenuButton Control
	MFC_PropertyGrid_Control,		// MFC PropertyGrid Control
	MFC_ShellList_Control,			// MFC ShellList Control
	MFC_ShellTree_Control,			// MFC ShellTree Control
	MFC_Link_Control,				// MFC Link Control
};


// Menu item info
struct USERMENUITEM
{
	unsigned itemId;				// Menu item ID
	unsigned itemType;				// Menu item type
	wchar_t* itemCaption;			// Menu item text
	unsigned parentId;				// Item parent ID
	wchar_t* parentCaption;			// Item parent caption
};


// User menu
struct USERMENU
{
	unsigned itemCount;				// Number of menu items
	USERMENUITEM* pMenuItemList;	// User menu pointer
};


// Custom class for dialog/window control info wrapper
class SCtrlInfoWrap : public CObject
{
	DECLARE_DYNAMIC(SCtrlInfoWrap)

public:
	// Construction
	SCtrlInfoWrap();								// default constructor
	virtual ~SCtrlInfoWrap();						// destructor

protected:
	// Base control window pointer
	CWnd*			baseControlWndPtr_;

	// Relative windows
	CWnd*			parentWndPtr_;
	CWnd*			buddyWndPtr_;

	// Control ID info
	int				typeId_;
	unsigned		templateId_;
	String			templateStringId_;

	// Control attributes
	String			caption_;
	bool			isVisible_;
	bool			isEnabled_;
	bool			isFocused_;

protected:
	// --- Control data values --- //

	// Boolean data
	bool*			isCheckedPtr_;

	// Integer data
	int64*			valueIntPtr_;
	int64*			reservedValueIntPtr_;
	int64*			minValueIntPtr_;
	int64*			maxValueIntPtr_;

	// Float data
	double*			valueDoublePtr_;
	double*			reservedValueDoublePtr_;
	double*			minValueDoublePtr_;
	double*			maxValueDoublePtr_;

	// String data
	String*			valueStringPtr_;
	String*			reservedValueStringPtr_;

	// Integer array data
	uint64_array*	valueIntListPtr_;
	uint64_array*	reservedValueIntListPtr_;

	// String array data
	StringArray*	valueStringListPtr_;
	StringArray*	reservedValueStringListPtr_;

	// Time data
	LPSYSTEMTIME	valueTimePtr_;

	// Custom data
	void*			customDataPtr_;			// Data pointer
	size_t*			dataSizePtr_;			// Total size in bytes

public:
	// Initialization
	virtual bool initialize(CWnd* parentWndPtr, CWnd* buddyWndPtr, unsigned controlId, int typeId);

	// Base control window pointer access
	virtual CWnd* getBaseControl(void) {
		return baseControlWndPtr_;
	}
	virtual bool isBaseControlAvailable(void) const {
		return ((baseControlWndPtr_ != NULL) && (baseControlWndPtr_->GetSafeHwnd() != NULL));
	}

	// Parent window functions
	virtual CWnd* getParent(void) {
		return parentWndPtr_;
	}
	virtual void setParent(CWnd* parentWndPtr) {
		parentWndPtr_ = parentWndPtr;
	}
	virtual bool isParentAvailable(void) const {
		return ((parentWndPtr_ != NULL) && (parentWndPtr_->GetSafeHwnd() != NULL));
	}

	// Buddy window functions
	virtual CWnd* getBuddy(void) {
		return buddyWndPtr_;
	}
	virtual void setBuddy(CWnd* buddyWndPtr) {
		buddyWndPtr_ = buddyWndPtr;
	}
	virtual bool isBuddyAvailable(void) const {
		return (buddyWndPtr_ != NULL);
	}

	// Get control ID info
	virtual int getType(void) const {
		return typeId_;
	}
	virtual unsigned getTemplateID(void) const {
		return templateId_;
	}
	virtual const wchar_t* getTemplateStringId(void) const {
		return templateStringId_;
	}
	virtual void getTemplateStringId(_Out_ String& templateID) const {
		templateID = templateStringId_;
	}

	// Get attributes
	virtual const wchar_t* getCaption(void) const {
		return templateStringId_;
	}
	virtual void getCaption(_Out_ String& caption) const {
		caption = caption_;
	}
	virtual bool isVisible(void) const {
		return isVisible_;
	}
	virtual bool isEnabled(void) const {
		return isEnabled_;
	}
	virtual bool isFocused(void) const {
		return isFocused_;
	}

	// Set control ID info
	virtual void setType(_In_ int typeId) {
		typeId_ = typeId;
	}
	virtual void setTemplateID(_In_ unsigned templateId) {
		templateId_ = templateId;
	}
	virtual void setTemplateStringId(_In_z_ const wchar_t* templateID) {
		templateStringId_ = templateID;
	}

	// Set attributes
	virtual void updateAttributes(void);
	virtual void setCaption(_In_z_ const wchar_t* caption) {
		caption_ = caption;
	}
	virtual void setVisibleState(_In_ bool isVisible) {
		isVisible_ = isVisible;
	}
	virtual void setEnableState(_In_ bool isEnabled) {
		isEnabled_ = isEnabled;
	}
	virtual void setFocusedState(_In_ bool isFocused) {
		isFocused_ = isFocused;
	}

	// Get boolean data values
	virtual bool	 getCheck(void) const;

	// Get integer data values
	virtual int64	 getInteger(void) const;
	virtual void	 getInteger(_Out_ int64& value) const;
	virtual int64	 getReserveInteger(void) const;
	virtual void	 getReserveInteger(_Out_ int64& value) const;
	virtual void	 getMinMaxInt(_Out_ int64& minVal, _Out_ int64& maxVal) const;

	// Get float data values
	virtual double	 getFloat(void) const;
	virtual void	 getFloat(_Out_ double& valueDouble) const;
	virtual double	 getReserveFloat(void) const;
	virtual void	 getReserveFloat(_Out_ double& valueDouble) const;
	virtual void	 getMinMaxFloat(_Out_ double& minVal, _Out_ double& maxVal) const;

	// Get string data values
	virtual const wchar_t* getString(void) const;
	virtual void	 getString(_Out_ String& value) const;
	virtual const wchar_t* getReserveString(void) const;
	virtual void	 getReserveString(_Out_ String& value) const;

	// Get array data values
	virtual void	 getIntArray(_Out_ uint64_array& valueList) const;
	virtual void	 getReserveIntArray(_Out_ uint64_array& valueList) const;
	virtual void	 getStringArray(_Out_ StringArray& valueList) const;
	virtual void	 getReserveStringArray(_Out_ StringArray& valueList) const;

	// Get time data value
	virtual SYSTEMTIME	getTime(void) const;
	virtual void		getTime(_Out_ SYSTEMTIME& timeValue) const;

	// Set boolean data values
	virtual void	setCheck(_In_ const bool& isChecked);

	// Set integer data values
	virtual void	setInteger(_In_ const int64& value);
	virtual void	setReserveInteger(_In_ const int64& value);
	virtual void	setMinMaxInt(_In_ const int64& minVal, _In_ const int64& maxVal);

	// Set float data values
	virtual void	setFloat(_In_ const double& valueDouble);
	virtual void	setReserveFloat(_In_ const double& valueDouble);
	virtual void	setMinMaxFloat(_In_ const double& minVal, _In_ const double& maxVal);

	// Set string data values
	virtual void	setString(_In_ const wchar_t* value);
	virtual void	setReserveString(_In_ const wchar_t* value);

	// Set array data values
	virtual void	setIntArray(_In_ const uint64_array& valueList);
	virtual void	setReserveIntArray(_In_ const uint64_array& valueList);
	virtual void	setStringArray(_In_ const StringArray& valueList);
	virtual void	setReserveStringArray(_In_ const StringArray& valueList);

	// Set time data values
	virtual void	setTime(_In_ const SYSTEMTIME& timeValue);

public:
	// Get/set custom data pointer
	template<typename DataType>
	bool getData(_Outptr_ DataType*& outputPtr, _Inout_opt_z_ size_t& dataSize) const;
	template<typename DataType>
	bool setData(_In_ const DataType* inputPtr, _In_ const size_t& dataSize);

	// Custom data size retrieving and validating
	virtual bool	isDataEmpty(void) const;
	virtual size_t	getDataSize(void) const;
};

// Define new typename
using SCtrlInfoList = typename std::vector<SCtrlInfoWrap*>;


// Custom base class for user menu
class SMenu : public CMenu
{
	DECLARE_DYNAMIC(SMenu)

public:
	// Construction
	SMenu(CWnd* parentWnd = NULL);			// constructor
	virtual ~SMenu();						// destructor

protected:
	// User menu layout
	USERMENU* menuLayoutPtr_;
};


// For dialog/window control management
class SControlManager : public CObject
{
	DECLARE_DYNAMIC(SControlManager)

public:
	// Construction
	SControlManager(CWnd* parentWnd = NULL);		// constructor
	virtual ~SControlManager();						// destructor

private:
	// List of control info wrappers
	SCtrlInfoList* controlInfoListPtr_;

	// Parent window
	CWnd* parentWndPtr_;

public:
	// Initialization and cleanup
	bool initialize(void);
	bool deleteAll(void);

	// Attributes
	size_t getCount(void) const {
		if (controlInfoListPtr_ == NULL) return 0;
		return controlInfoListPtr_->size();
	}
	bool isEmpty(void) const {
		if (controlInfoListPtr_ == NULL) return TRUE;
		return controlInfoListPtr_->empty();
	}

	// Parent window functions
	CWnd* getParent(void) {
		return parentWndPtr_;
	}
	void setParent(CWnd* parentWndPtr) {
		parentWndPtr_ = parentWndPtr;
	}
	bool isParentAvailable(void) const {
		return ((parentWndPtr_ != NULL) && (parentWndPtr_->GetSafeHwnd() != NULL));
	}

	// Add/remove control
	int64 addControl(SCtrlInfoWrap* pControl);
	int64 addControl(unsigned controlId, unsigned typeId);
	int64 removeControl(unsigned controlId);

	// Accessing elements
	SCtrlInfoWrap* getControl(unsigned controlId);
	bool setBuddy(unsigned nBaseCtrlID, unsigned buddyCtrlId);
	void updateData(unsigned controlId = NULL);
};

