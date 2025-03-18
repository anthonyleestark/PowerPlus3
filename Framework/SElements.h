
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

#ifndef _STUPIDFRAMEWORK_SELEMENTS_H_INCLUDED
#define _STUPIDFRAMEWORK_SELEMENTS_H_INCLUDED

#include "stdafx.h"

#include "Core.h"
#include "IDMapping.h"
#include "Logging.h"


//////////////////////////////////////////////////////////////////////////
//
//	Define dialog/window control types
//
//////////////////////////////////////////////////////////////////////////

#define CTRL_TYPE_BASE				0x01
#define CTRL_TYPE_BUTTON			((CTRL_TYPE_BASE<<8)+0x01)
#define CTRL_TYPE_CHECKBOX			((CTRL_TYPE_BASE<<8)+0x02)
#define CTRL_TYPE_RADIOBTN			((CTRL_TYPE_BASE<<8)+0x03)
#define CTRL_TYPE_EDITBOX			((CTRL_TYPE_BASE<<8)+0x04)
#define CTRL_TYPE_COMBOBOX			((CTRL_TYPE_BASE<<8)+0x05)
#define CTRL_TYPE_LISTBOX			((CTRL_TYPE_BASE<<8)+0x06)
#define CTRL_TYPE_STATIC			((CTRL_TYPE_BASE<<8)+0x07)
#define CTRL_TYPE_PICTURE			((CTRL_TYPE_BASE<<8)+0x08)
#define CTRL_TYPE_GROUPBOX			((CTRL_TYPE_BASE<<8)+0x09)
#define CTRL_TYPE_HSCROLL			((CTRL_TYPE_BASE<<8)+0x0A)
#define CTRL_TYPE_VSCROLL			((CTRL_TYPE_BASE<<8)+0x0B)
#define CTRL_TYPE_SLIDERCTRL		((CTRL_TYPE_BASE<<8)+0x0C)
#define CTRL_TYPE_SPINCTRL			((CTRL_TYPE_BASE<<8)+0x0D)
#define CTRL_TYPE_PROGRESSBAR		((CTRL_TYPE_BASE<<8)+0x0E)
#define CTRL_TYPE_HOTKEY			((CTRL_TYPE_BASE<<8)+0x0F)
#define CTRL_TYPE_LISTCTRL			((CTRL_TYPE_BASE<<8)+0x10)
#define CTRL_TYPE_TREECTRL			((CTRL_TYPE_BASE<<8)+0x11)
#define CTRL_TYPE_TABCTRL			((CTRL_TYPE_BASE<<8)+0x12)
#define CTRL_TYPE_ANIMATION			((CTRL_TYPE_BASE<<8)+0x13)
#define CTRL_TYPE_RICHEDIT			((CTRL_TYPE_BASE<<8)+0x14)
#define CTRL_TYPE_DATETIME			((CTRL_TYPE_BASE<<8)+0x15)
#define CTRL_TYPE_MCALENDAR			((CTRL_TYPE_BASE<<8)+0x16)
#define CTRL_TYPE_IPADDRESS			((CTRL_TYPE_BASE<<8)+0x17)
#define CTRL_TYPE_EXTCOMBOBOX		((CTRL_TYPE_BASE<<8)+0x18)
#define CTRL_TYPE_CUSTOMCTRL		((CTRL_TYPE_BASE<<8)+0x19)
#define CTRL_TYPE_SYSLINKCTRL		((CTRL_TYPE_BASE<<8)+0x1A)
#define CTRL_TYPE_SPLITBUTTON		((CTRL_TYPE_BASE<<8)+0x1B)
#define CTRL_TYPE_NETADDRESS		((CTRL_TYPE_BASE<<8)+0x1B)
#define CTRL_TYPE_CMDBUTTON			((CTRL_TYPE_BASE<<8)+0x1C)


/////////////////////////////////////////////////////////////////
//
//	Class name:	 SCtrlInfoWrap
//  Description: Custom class for dialog/window control info wrapper
//  Base class:	 CObject
//
/////////////////////////////////////////////////////////////////

class SCtrlInfoWrap : public CObject
{
	DECLARE_DYNAMIC(SCtrlInfoWrap)

public:
	// Construction
	SCtrlInfoWrap();								// default constructor
	virtual ~SCtrlInfoWrap();						// destructor

protected:
	// Relative windows
	CWnd*			m_pParentWnd;
	CWnd*			m_pBuddyWnd;

	// Control properties
	INT				m_nTypeID;
	UINT			m_nTemplateID;
	CString			m_strTemplateID;

	CString			m_strCaption;

	// Control data values
	BOOL*			m_pbCheck;

	LONG_PTR*		m_plValue;
	LONG_PTR*		m_plReserveValue;
	LONG_PTR*		m_plMinValue;
	LONG_PTR*		m_plMaxValue;

	DOUBLE*			m_pdbValue;
	DOUBLE*			m_pdbReserveValue;
	DOUBLE*			m_pdbMinValue;
	DOUBLE*			m_pdbMaxValue;

	CString*		m_pstrValue;
	CString*		m_pstrReserveValue;

	CUIntArray*		m_pauiValueList;
	CStringArray*	m_pastrValueList;

public:
	// Initialization
	virtual BOOL	Initialize(CWnd* pParentWnd, CWnd* pBuddyWnd, UINT nCtrlID, INT nTypeID);

	// Parent window functions
	virtual CWnd*	GetParent(void);
	virtual void	SetParent(CWnd* pParentWnd);
	virtual BOOL	IsParentAvailable(void) const;

	// Buddy window functions
	virtual CWnd*	GetBuddy(void);
	virtual void	SetBuddy(CWnd* pBuddyWnd);
	virtual BOOL	IsBuddyAvailable(void) const;

	// Get properties
	virtual INT		GetType(void) const;
	virtual UINT	GetTemplateID(void) const;
	virtual LPCTSTR GetTemplateStringID(void) const;
	virtual void	GetTemplateStringID(CString& strTemplateID) const;

	virtual LPCTSTR GetCaption(void) const;
	virtual void	GetCaption(CString& strCaption) const;

	// Set properties
	virtual void	SetType(INT nTypeID);
	virtual void	SetTemplateID(UINT nTemplateID);
	virtual void	SetTemplateStringID(LPCTSTR lpszTemplateID);
	virtual void	SetCaption(LPCTSTR lpszCaption);

	// Get boolean data values
	virtual BOOL	 GetCheck(void) const;

	// Get integer data values
	virtual LONG_PTR GetValueInt(void) const;
	virtual void	 GetValueInt(LONG_PTR& lValue) const;
	virtual LONG_PTR GetReserveValueInt(void) const;
	virtual void	 GetReserveValueInt(LONG_PTR& lValue) const;
	virtual void	 GetMinMaxInt(LONG_PTR& lMin, LONG_PTR& lMax) const;

	// Get float data values
	virtual DOUBLE	 GetValueFloat(void) const;
	virtual void	 GetValueFloat(DOUBLE& dbValue) const;
	virtual DOUBLE	 GetReserveValueFloat(void) const;
	virtual void	 GetReserveValueFloat(DOUBLE& dbValue) const;
	virtual void	 GetMinMaxFloat(DOUBLE& dbMin, DOUBLE& dbMax) const;

	// Get string data values
	virtual LPCTSTR	 GetValueString(void) const;
	virtual void	 GetValueString(CString& strValue) const;
	virtual LPCTSTR	 GetReserveValueString(void) const;
	virtual void	 GetReserveValueString(CString& strValue) const;

	// Get array data values
	virtual void	 GetValueIntArray(CUIntArray& auiValue) const;
	virtual void	 GetValueStringArray(CStringArray& astrValue) const;

	// Set boolean data values
	virtual void	SetCheck(const BOOL& bCheck);

	// Set integer data values
	virtual void	SetValueInt(const LONG_PTR& lValue);
	virtual void	SetReserveValueInt(const LONG_PTR& lValue);
	virtual void	SetMinMaxInt(const LONG_PTR& lMin, const LONG_PTR& lMax);

	// Set float data values
	virtual void	SetValueFloat(const DOUBLE& dbValue);
	virtual void	SetReserveValueFloat(const DOUBLE& dbValue);
	virtual void	SetMinMaxFloat(const DOUBLE& dbMin, const DOUBLE& dbMax);

	// Set string data values
	virtual void	SetValueString(LPCTSTR lpszValue);
	virtual void	SetReserveValueString(LPCTSTR lpszValue);

	// Set array data values
	virtual void	SetValueIntArray(const CUIntArray& auiValue);
	virtual void	SetValueStringArray(const CStringArray& astrValue);
};

/////////////////////////////////////////////////////////////////
//
//	Class name:	 SMenu
//  Description: Custom base class for user menu
//  Base class:	 CMenu
//
/////////////////////////////////////////////////////////////////

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
//	Data type name:	SCtrlInfoList
//  Description:	Store list of dialog/window control info wrappers
//  Derivered from: MFC CArray class
//
//////////////////////////////////////////////////////////////////////////

typedef CArray<SCtrlInfoWrap*, SCtrlInfoWrap*> SCtrlInfoList;

/////////////////////////////////////////////////////////////////
//
//	Class name:	 SControlManager
//  Description: Class for dialog/window control management
//  Base class:	 CObject
//
/////////////////////////////////////////////////////////////////

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
	INT_PTR GetCount(void) const;
	BOOL	IsEmpty(void) const;

	// Parent window functions
	CWnd*	GetParent(void);
	void	SetParent(CWnd* pParentWnd);
	BOOL	IsParentAvailable(void) const;

	// Add/remove control
	INT_PTR AddControl(SCtrlInfoWrap* pControl);
	INT_PTR AddControl(UINT nCtrlID, UINT nTypeID);
	INT_PTR RemoveControl(UINT nCtrlID);

	// Accessing elements
	SCtrlInfoWrap* GetControl(UINT nCtrlID);
	BOOL SetBuddy(UINT nBaseCtrlID, UINT nBuddyCtrlID);
	void UpdateData(UINT nCtrlID = NULL);
};

#endif	// ifndef _STUPIDFRAMEWORK_SELEMENTS_H_INCLUDED
