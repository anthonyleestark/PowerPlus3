
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		SElements.h
//		Project name:	Stupid Framework
//		Description:	Custom base class for dialog elements (header file)
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.12.24:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"

#include "AppCore/AppCore.h"
#include "AppCore/IDManager.h"


//////////////////////////////////////////////////////////////////////////
//
//	Define dialog/window control types
//
//////////////////////////////////////////////////////////////////////////

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


//////////////////////////////////////////////////////////////////////////
//
//	Class name:	 SCtrlInfoWrap
//  Description: Custom class for dialog/window control info wrapper
//  Base class:	 CObject
//
//////////////////////////////////////////////////////////////////////////

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
	INT				m_nTypeID;
	UINT			m_nTemplateID;
	CString			m_strTemplateID;

	// Control attributes
	CString			m_strCaption;
	BOOL			m_bVisible;
	BOOL			m_bEnabled;
	BOOL			m_bFocused;

protected:
	// --- Control data values --- //

	// Boolean data
	PBOOL			m_pbCheck;

	// Integer data
	PLONG_PTR		m_plValue;
	PLONG_PTR		m_plReserveValue;
	PLONG_PTR		m_plMinValue;
	PLONG_PTR		m_plMaxValue;

	// Float data
	DOUBLE*			m_pdbValue;
	DOUBLE*			m_pdbReserveValue;
	DOUBLE*			m_pdbMinValue;
	DOUBLE*			m_pdbMaxValue;

	// String data
	CString*		m_pstrValue;
	CString*		m_pstrReserveValue;

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
	virtual BOOL	Initialize(CWnd* pParentWnd, CWnd* pBuddyWnd, UINT nCtrlID, INT nTypeID);

	// Base control window pointer access
	virtual CWnd*	GetBaseControl(void);
	virtual BOOL	IsBaseControlAvailable(void) const;

	// Parent window functions
	virtual CWnd*	GetParent(void);
	virtual void	SetParent(CWnd* pParentWnd);
	virtual BOOL	IsParentAvailable(void) const;

	// Buddy window functions
	virtual CWnd*	GetBuddy(void);
	virtual void	SetBuddy(CWnd* pBuddyWnd);
	virtual BOOL	IsBuddyAvailable(void) const;

	// Get control ID info
	virtual INT		GetType(void) const;
	virtual UINT	GetTemplateID(void) const;
	virtual LPCTSTR GetTemplateStringID(void) const;
	virtual void	GetTemplateStringID(_Out_ CString& strTemplateID) const;

	// Get attributes
	virtual LPCTSTR GetCaption(void) const;
	virtual void	GetCaption(_Out_ CString& strCaption) const;
	virtual BOOL	IsVisible(void) const;
	virtual BOOL	IsEnabled(void) const;
	virtual BOOL	IsFocused(void) const;

	// Set control ID info
	virtual void	SetType(_In_ INT nTypeID);
	virtual void	SetTemplateID(_In_ UINT nTemplateID);
	virtual void	SetTemplateStringID(_In_z_ LPCTSTR lpszTemplateID);

	// Set attributes
	virtual void	SetCaption(_In_z_ LPCTSTR lpszCaption);
	virtual void	SetVisibleState(_In_ BOOL bVisible);
	virtual void	SetEnableState(_In_ BOOL bEnabled);
	virtual void	SetFocusedState(_In_ BOOL bFocused);
	virtual void	UpdateAttributes(void);

	// Get boolean data values
	virtual BOOL	 GetCheck(void) const;

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
	virtual LPCTSTR	 GetString(void) const;
	virtual void	 GetString(_Out_ CString& strValue) const;
	virtual LPCTSTR	 GetReserveString(void) const;
	virtual void	 GetReserveString(_Out_ CString& strValue) const;

	// Get array data values
	virtual void	 GetIntArray(_Out_ ULongArray& aulValue) const;
	virtual void	 GetReserveIntArray(_Out_ ULongArray& aulValue) const;
	virtual void	 GetStringArray(_Out_ StringArray& astrValue) const;
	virtual void	 GetReserveStringArray(_Out_ StringArray& astrValue) const;

	// Get time data value
	virtual SYSTEMTIME	GetTime(void) const;
	virtual void		GetTime(_Out_ SYSTEMTIME& timeValue) const;

	// Set boolean data values
	virtual void	SetCheck(_In_ const BOOL& bCheck);

	// Set integer data values
	virtual void	SetInteger(_In_ const LONG_PTR& lValue);
	virtual void	SetReserveInteger(_In_ const LONG_PTR& lValue);
	virtual void	SetMinMaxInt(_In_ const LONG_PTR& lMin, _In_ const LONG_PTR& lMax);

	// Set float data values
	virtual void	SetFloat(_In_ const DOUBLE& dbValue);
	virtual void	SetReserveFloat(_In_ const DOUBLE& dbValue);
	virtual void	SetMinMaxFloat(_In_ const DOUBLE& dbMin, _In_ const DOUBLE& dbMax);

	// Set string data values
	virtual void	SetString(_In_ LPCTSTR lpszValue);
	virtual void	SetReserveString(_In_ LPCTSTR lpszValue);

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
	BOOL GetData(_Outptr_ DATA_TYPE* lpOutput, _Inout_opt_z_ SIZE_T& szDataSize) const;
	template<typename DATA_TYPE>
	BOOL SetData(_In_ const DATA_TYPE* lpInput, _In_ const SIZE_T& szDataSize);

	// Custom data size retrieving and validating
	virtual BOOL	IsDataEmpty(void) const;
	virtual SIZE_T	GetDataSize(void) const;
};

// Define new typename
using SCtrlInfoList = typename std::vector<SCtrlInfoWrap*>;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:	 SMenu
//  Description: Custom base class for user menu
//  Base class:	 CMenu
//
//////////////////////////////////////////////////////////////////////////

class SMenu : public CMenu
{
	DECLARE_DYNAMIC(SMenu)

public:
	// Construction
	SMenu(CWnd* pParentWnd = NULL);			// constructor
	virtual ~SMenu();						// destructor

protected:
	// User menu layout
	PUSERMENU	m_pMenuLayout;
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:	 SControlManager
//  Description: Class for dialog/window control management
//  Base class:	 CObject
//
//////////////////////////////////////////////////////////////////////////

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
	BOOL Initialize(void);
	BOOL DeleteAll(void);

	// Attributes
	size_t  GetCount(void) const;
	BOOL	IsEmpty(void) const;

	// Parent window functions
	CWnd*	GetParent(void);
	void	SetParent(CWnd* pParentWnd);
	BOOL	IsParentAvailable(void) const;

	// Add/remove control
	size_t AddControl(SCtrlInfoWrap* pControl);
	size_t AddControl(UINT nCtrlID, UINT nTypeID);
	size_t RemoveControl(UINT nCtrlID);

	// Accessing elements
	SCtrlInfoWrap* GetControl(UINT nCtrlID);
	BOOL SetBuddy(UINT nBaseCtrlID, UINT nBuddyCtrlID);
	void UpdateData(UINT nCtrlID = NULL);
};


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _SELEMENTS_ENABLE_INLINES
	#include "Framework.inl"
	#ifdef _SELEMENTS_INLINE_INCLUDED
		#pragma message("-- Framework inline library included (SElements.h)")
	#else
		#pragma error("-- Linking error in SElements.h: Unable to link to inline header!")
	#endif
	#undef _SELEMENTS_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in SElements.h: Inline is not enabled!")
#endif
