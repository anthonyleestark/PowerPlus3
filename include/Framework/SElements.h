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
	unsigned	nItemID;				// Menu item ID
	unsigned	nItemType;				// Menu item type
	LPTSTR	lpszItemCaption;		// Menu item text
	unsigned	nParentID;				// Item parent ID
	LPTSTR	lpszParentCaption;		// Item parent caption
};


// User menu
struct USERMENU
{
	unsigned nItemCount;				// Number of menu items
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
	CWnd*			m_pBaseControl;

	// Relative windows
	CWnd*			m_pParentWnd;
	CWnd*			m_pBuddyWnd;

	// Control ID info
	int				m_nTypeID;
	unsigned		m_nTemplateID;
	String			m_strTemplateID;

	// Control attributes
	String			m_strCaption;
	bool			m_bVisible;
	bool			m_bEnabled;
	bool			m_bFocused;

protected:
	// --- Control data values --- //

	// Boolean data
	bool*			m_pbCheck;

	// Integer data
	PLONG_PTR		m_plValue;
	PLONG_PTR		m_plReserveValue;
	PLONG_PTR		m_plMinValue;
	PLONG_PTR		m_plMaxValue;

	// Float data
	double*			m_pdbValue;
	double*			m_pdbReserveValue;
	double*			m_pdbMinValue;
	double*			m_pdbMaxValue;

	// String data
	String*			m_pstrValue;
	String*			m_pstrReserveValue;

	// Integer array data
	ULongArray*		m_paulValueList;
	ULongArray*		m_paulReserveValueList;

	// String array data
	StringArray*	m_pastrValueList;
	StringArray*	m_pastrReserveValueList;

	// Time data
	LPSYSTEMTIME	m_pstTimeValue;

	// Custom data
	LPVOID			m_ptrCustomData;		// Data pointer
	PSIZE_T			m_pszDataSize;			// Total size in bytes

public:
	// Initialization
	virtual bool	Initialize(CWnd* pParentWnd, CWnd* pBuddyWnd, unsigned nCtrlID, int nTypeID);

	// Base control window pointer access
	virtual CWnd* GetBaseControl(void) {
		return m_pBaseControl;
	};
	virtual bool IsBaseControlAvailable(void) const {
		return ((m_pBaseControl != NULL) && (m_pBaseControl->GetSafeHwnd() != NULL));
	};

	// Parent window functions
	virtual CWnd* GetParent(void) {
		return m_pParentWnd;
	};
	virtual void SetParent(CWnd* pParentWnd) {
		m_pParentWnd = pParentWnd;
	};
	virtual bool IsParentAvailable(void) const {
		return ((m_pParentWnd != NULL) && (m_pParentWnd->GetSafeHwnd() != NULL));
	};

	// Buddy window functions
	virtual CWnd* GetBuddy(void) {
		return m_pBuddyWnd;
	};
	virtual void SetBuddy(CWnd* pBuddyWnd) {
		m_pBuddyWnd = pBuddyWnd;
	};
	virtual bool IsBuddyAvailable(void) const {
		return (m_pBuddyWnd != NULL);
	};

	// Get control ID info
	virtual int GetType(void) const {
		return m_nTypeID;
	};
	virtual unsigned GetTemplateID(void) const {
		return m_nTemplateID;
	};
	virtual const wchar_t* GetTemplateStringID(void) const {
		return m_strTemplateID;
	};
	virtual void GetTemplateStringID(_Out_ String& templateID) const {
		templateID = m_strTemplateID;
	};

	// Get attributes
	virtual const wchar_t* GetCaption(void) const {
		return m_strTemplateID;
	};
	virtual void GetCaption(_Out_ String& caption) const {
		caption = m_strCaption;
	};
	virtual bool IsVisible(void) const {
		return m_bVisible;
	};
	virtual bool IsEnabled(void) const {
		return m_bEnabled;
	};
	virtual bool IsFocused(void) const {
		return m_bFocused;
	};

	// Set control ID info
	virtual void SetType(_In_ int nTypeID) {
		m_nTypeID = nTypeID;
	};
	virtual void SetTemplateID(_In_ unsigned nTemplateID) {
		m_nTemplateID = nTemplateID;
	};
	virtual void SetTemplateStringID(_In_z_ const wchar_t* templateID) {
		m_strTemplateID = templateID;
	};

	// Set attributes
	virtual void UpdateAttributes(void);
	virtual void SetCaption(_In_z_ const wchar_t* caption) {
		m_strCaption = caption;
	};
	virtual void SetVisibleState(_In_ bool bVisible) {
		m_bVisible = bVisible;
	};
	virtual void SetEnableState(_In_ bool bEnabled) {
		m_bEnabled = bEnabled;
	};
	virtual void SetFocusedState(_In_ bool bFocused) {
		m_bFocused = bFocused;
	};

	// Get boolean data values
	virtual bool	 GetCheck(void) const;

	// Get integer data values
	virtual LONG_PTR GetInteger(void) const;
	virtual void	 GetInteger(_Out_ LONG_PTR& lValue) const;
	virtual LONG_PTR GetReserveInteger(void) const;
	virtual void	 GetReserveInteger(_Out_ LONG_PTR& lValue) const;
	virtual void	 GetMinMaxInt(_Out_ LONG_PTR& lMin, _Out_ LONG_PTR& lMax) const;

	// Get float data values
	virtual DOUBLE	 GetFloat(void) const;
	virtual void	 GetFloat(_Out_ DOUBLE& dbValue) const;
	virtual DOUBLE	 GetReserveFloat(void) const;
	virtual void	 GetReserveFloat(_Out_ DOUBLE& dbValue) const;
	virtual void	 GetMinMaxFloat(_Out_ DOUBLE& dbMin, _Out_ DOUBLE& dbMax) const;

	// Get string data values
	virtual const wchar_t* GetString(void) const;
	virtual void	 GetString(_Out_ String& value) const;
	virtual const wchar_t* GetReserveString(void) const;
	virtual void	 GetReserveString(_Out_ String& value) const;

	// Get array data values
	virtual void	 GetIntArray(_Out_ ULongArray& aulValue) const;
	virtual void	 GetReserveIntArray(_Out_ ULongArray& aulValue) const;
	virtual void	 GetStringArray(_Out_ StringArray& astrValue) const;
	virtual void	 GetReserveStringArray(_Out_ StringArray& astrValue) const;

	// Get time data value
	virtual SYSTEMTIME	GetTime(void) const;
	virtual void		GetTime(_Out_ SYSTEMTIME& timeValue) const;

	// Set boolean data values
	virtual void	SetCheck(_In_ const bool& bCheck);

	// Set integer data values
	virtual void	SetInteger(_In_ const LONG_PTR& lValue);
	virtual void	SetReserveInteger(_In_ const LONG_PTR& lValue);
	virtual void	SetMinMaxInt(_In_ const LONG_PTR& lMin, _In_ const LONG_PTR& lMax);

	// Set float data values
	virtual void	SetFloat(_In_ const DOUBLE& dbValue);
	virtual void	SetReserveFloat(_In_ const DOUBLE& dbValue);
	virtual void	SetMinMaxFloat(_In_ const DOUBLE& dbMin, _In_ const DOUBLE& dbMax);

	// Set string data values
	virtual void	SetString(_In_ const wchar_t* value);
	virtual void	SetReserveString(_In_ const wchar_t* value);

	// Set array data values
	virtual void	SetIntArray(_In_ const ULongArray& auiValue);
	virtual void	SetReserveIntArray(_In_ const ULongArray& auiValue);
	virtual void	SetStringArray(_In_ const StringArray& astrValue);
	virtual void	SetReserveStringArray(_In_ const StringArray& astrValue);

	// Set time data values
	virtual void	SetTime(_In_ const SYSTEMTIME& timeValue);

public:
	// Get/set custom data pointer
	template<typename DATA_TYPE>
	bool GetData(_Outptr_ DATA_TYPE* lpOutput, _Inout_opt_z_ SIZE_T& szDataSize) const;
	template<typename DATA_TYPE>
	bool SetData(_In_ const DATA_TYPE* lpInput, _In_ const SIZE_T& szDataSize);

	// Custom data size retrieving and validating
	virtual bool	IsDataEmpty(void) const;
	virtual SIZE_T	GetDataSize(void) const;
};

// Define new typename
using SCtrlInfoList = typename std::vector<SCtrlInfoWrap*>;


// Custom base class for user menu
class SMenu : public CMenu
{
	DECLARE_DYNAMIC(SMenu)

public:
	// Construction
	SMenu(CWnd* pParentWnd = NULL);			// constructor
	virtual ~SMenu();						// destructor

protected:
	// User menu layout
	USERMENU* m_pMenuLayout;
};


// For dialog/window control management
class SControlManager : public CObject
{
	DECLARE_DYNAMIC(SControlManager)

public:
	// Construction
	SControlManager(CWnd* pParentWnd = NULL);		// constructor
	virtual ~SControlManager();						// destructor

private:
	// List of control info wrappers
	SCtrlInfoList* m_pCtrlInfoArray;

	// Parent window
	CWnd* m_pParentWnd;

public:
	// Initialization and cleanup
	bool Initialize(void);
	bool DeleteAll(void);

	// Attributes
	size_t GetCount(void) const {
		if (m_pCtrlInfoArray == NULL) return 0;
		return m_pCtrlInfoArray->size();
	};
	bool IsEmpty(void) const {
		if (m_pCtrlInfoArray == NULL) return TRUE;
		return m_pCtrlInfoArray->empty();
	};

	// Parent window functions
	CWnd* GetParent(void) {
		return m_pParentWnd;
	};
	void SetParent(CWnd* pParentWnd) {
		m_pParentWnd = pParentWnd;
	};
	bool IsParentAvailable(void) const {
		return ((m_pParentWnd != NULL) && (m_pParentWnd->GetSafeHwnd() != NULL));
	};

	// Add/remove control
	long long AddControl(SCtrlInfoWrap* pControl);
	long long AddControl(unsigned nCtrlID, unsigned nTypeID);
	long long RemoveControl(unsigned nCtrlID);

	// Accessing elements
	SCtrlInfoWrap* GetControl(unsigned nCtrlID);
	bool SetBuddy(unsigned nBaseCtrlID, unsigned nBuddyCtrlID);
	void UpdateData(unsigned nCtrlID = NULL);
};

