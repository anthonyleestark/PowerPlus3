
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		SElements.h
//		Project name:	Stupid Framework
//		Description:	Custom base class for dialog elements (source file)
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.12.24:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SElements.h"
#include "SWinApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace PairFuncs;
using namespace CoreFuncs;


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for SCtrlInfoWrap
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(SCtrlInfoWrap, CObject)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SCtrlInfoWrap
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

SCtrlInfoWrap::SCtrlInfoWrap() : CObject()
{
	// Relative windows
	m_pParentWnd = NULL;
	m_pBuddyWnd = NULL;

	// Control properties
	m_nTypeID = CTRL_TYPE_BASE;
	m_nTemplateID = 0;
	m_strTemplateID.Empty();
	m_strCaption.Empty();

	// Control data values
	m_pbCheck = NULL;
	m_plValue = NULL;
	m_plMinValue = NULL;
	m_plMaxValue = NULL;
	m_pdbValue = NULL;
	m_pdbMinValue = NULL;
	m_pdbMaxValue = NULL;
	m_pstrValue = NULL;
	m_pauiValueList = NULL;
	m_pastrValueList = NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~SCtrlInfoWrap
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

SCtrlInfoWrap::~SCtrlInfoWrap()
{
	// Control data values
	if (m_pbCheck != NULL) {
		delete m_pbCheck;
		m_pbCheck = NULL;
	}
	if (m_plValue != NULL) {
		delete m_plValue;
		m_plValue = NULL;
	}
	if (m_plMinValue != NULL) {
		delete m_plMinValue;
		m_plMinValue = NULL;
	}
	if (m_plMaxValue != NULL) {
		delete m_plMaxValue;
		m_plMaxValue = NULL;
	}
	if (m_pdbValue != NULL) {
		delete m_pdbValue;
		m_pdbValue = NULL;
	}
	if (m_pdbMinValue != NULL) {
		delete m_pdbMinValue;
		m_pdbMinValue = NULL;
	}
	if (m_pdbMaxValue != NULL) {
		delete m_pdbMaxValue;
		m_pdbMaxValue = NULL;
	}
	if (m_pstrValue != NULL) {
		delete m_pstrValue;
		m_pstrValue = NULL;
	}
	if (m_pauiValueList != NULL) {
		// Cleanup array data
		if (!m_pauiValueList->IsEmpty()) {
			m_pauiValueList->RemoveAll();
			m_pauiValueList->FreeExtra();
		}
		delete m_pauiValueList;
		m_pauiValueList = NULL;
	}
	if (m_pastrValueList != NULL) {
		// Cleanup array data
		if (!m_pastrValueList->IsEmpty()) {
			m_pastrValueList->RemoveAll();
			m_pastrValueList->FreeExtra();
		}
		delete m_pastrValueList;
		m_pastrValueList = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Initialize
//	Description:	Initialize control info wrap object
//  Arguments:		pParentWnd - Parent window
//					pBuddyWnd  - Buddy window
//					nCtrlID	   - Control ID
//					nTypeID	   - Control type ID
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SCtrlInfoWrap::Initialize(CWnd* pParentWnd, CWnd* pBuddyWnd, UINT nCtrlID, INT nTypeID)
{
	ASSERT(pParentWnd->GetSafeHwnd());

	// Set attributes and properties
	this->m_pParentWnd = pParentWnd;
	this->m_nTemplateID = nCtrlID;
	this->m_nTypeID = nTypeID;

	// Set control template string ID
	this->m_strTemplateID = IDMAP_GET(this->m_nTemplateID);

	// Set control caption by type
	switch (this->m_nTypeID)
	{
		case CTRL_TYPE_BUTTON:
		case CTRL_TYPE_CHECKBOX:
		case CTRL_TYPE_RADIOBTN:
		case CTRL_TYPE_STATIC:
		case CTRL_TYPE_GROUPBOX:
		case CTRL_TYPE_SYSLINKCTRL:
		{
			// Get control window text
			CString strCaption;
			CWnd* pCtrlWnd = this->m_pParentWnd->GetDlgItem(this->m_nTemplateID);
			if (pCtrlWnd != NULL) {
				pCtrlWnd->GetWindowText(strCaption);
				this->m_strCaption = strCaption;
			}
		} break;
		case CTRL_TYPE_EDITBOX:
		case CTRL_TYPE_COMBOBOX:
		case CTRL_TYPE_LISTBOX:
		case CTRL_TYPE_SPINCTRL:
		case CTRL_TYPE_SLIDERCTRL:
		case CTRL_TYPE_PROGRESSBAR:
		case CTRL_TYPE_LISTCTRL:
		case CTRL_TYPE_TREECTRL:
		case CTRL_TYPE_RICHEDIT:
		{
			// Get buddy control caption
			CString strCaption;
			if (this->m_pBuddyWnd != NULL) {
				this->m_pBuddyWnd->GetWindowText(strCaption);
				this->m_strCaption = strCaption;
			}
		} break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetParent
//	Description:	Get current control's parent window
//  Arguments:		None
//  Return value:	CWnd*
//
//////////////////////////////////////////////////////////////////////////

CWnd* SCtrlInfoWrap::GetParent(void)
{
	return m_pParentWnd;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetParent
//	Description:	Set parent window for current control
//  Arguments:		pParentWnd - Parent window
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetParent(CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsParentAvailable
//	Description:	Check if parent window (pointer) is available
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL SCtrlInfoWrap::IsParentAvailable(void) const
{
	return (m_pParentWnd != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetBuddy
//	Description:	Get current control's buddy window
//  Arguments:		None
//  Return value:	CWnd*
//
//////////////////////////////////////////////////////////////////////////

CWnd* SCtrlInfoWrap::GetBuddy(void)
{
	return m_pBuddyWnd;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetParent
//	Description:	Set buddy window for current control
//  Arguments:		pBuddyWnd - Buddy window
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetBuddy(CWnd* pBuddyWnd)
{
	m_pBuddyWnd = pBuddyWnd;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsParentAvailable
//	Description:	Check if buddy window (pointer) is available
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL SCtrlInfoWrap::IsBuddyAvailable(void) const
{
	return (m_pBuddyWnd != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetType
//	Description:	Get current control's type
//  Arguments:		None
//  Return value:	INT
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE INT SCtrlInfoWrap::GetType(void) const
{
	return m_nTypeID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetTemplateID
//	Description:	Get current control's template ID
//  Arguments:		None
//  Return value:	UINT
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE UINT SCtrlInfoWrap::GetTemplateID(void) const
{
	return m_nTemplateID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetTemplateStringID
//	Description:	Get current control's template string ID
//  Arguments:		strTemplateID - Template string ID (out)
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE LPCTSTR SCtrlInfoWrap::GetTemplateStringID(void) const
{
	return m_strTemplateID;
}

AFX_INLINE void SCtrlInfoWrap::GetTemplateStringID(CString& strTemplateID) const
{
	strTemplateID = m_strTemplateID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetCaption
//	Description:	Get current control's caption
//  Arguments:		strCaption - Caption string (out)
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE LPCTSTR SCtrlInfoWrap::GetCaption(void) const
{
	return m_strTemplateID;
}

AFX_INLINE void SCtrlInfoWrap::GetCaption(CString& strCaption) const
{
	strCaption = m_strCaption;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetType
//	Description:	Set current control's type
//  Arguments:		nTypeID - Control type ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetType(INT nTypeID)
{
	m_nTypeID = nTypeID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetTemplateID
//	Description:	Set current control's template ID
//  Arguments:		nTemplateID - Template ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetTemplateID(UINT nTemplateID)
{
	m_nTemplateID = nTemplateID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetTemplateStringID
//	Description:	Set current control's template ID
//  Arguments:		lpszTemplateID - Template string ID (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetTemplateStringID(LPCTSTR lpszTemplateID)
{
	m_strTemplateID = lpszTemplateID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetCaption
//	Description:	Set current control's caption
//  Arguments:		lpszCaption - Caption string (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetCheck
//	Description:	Get current control's checked state
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SCtrlInfoWrap::GetCheck(void) const
{
	if (this->m_pbCheck == NULL)
		return FALSE;
	else
		return *(this->m_pbCheck);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetValueInt
//	Description:	Get current control's integer data value
//  Arguments:		lValue - Integer value (out)
//  Return value:	LONG_PTR
//
//////////////////////////////////////////////////////////////////////////

LONG_PTR SCtrlInfoWrap::GetValueInt(void) const
{
	if (this->m_plValue == NULL)
		return INT_INVALID;
	else
		return *(this->m_plValue);
}

void SCtrlInfoWrap::GetValueInt(LONG_PTR& lValue) const
{
	if (this->m_plValue == NULL)
		lValue = INT_INVALID;
	else
		lValue = *(this->m_plValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMinMaxInt
//	Description:	Get current control's min/max range integer data value
//  Arguments:		lMin - Min range integer value (out)
//					lMax - Max range integer value (out)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::GetMinMaxInt(LONG_PTR& lMin, LONG_PTR& lMax) const
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetValueFloat
//	Description:	Get current control's float data value
//  Arguments:		dbValue - Float value (out)
//  Return value:	DOUBLE
//
//////////////////////////////////////////////////////////////////////////

DOUBLE SCtrlInfoWrap::GetValueFloat(void) const
{
	if (this->m_pdbValue == NULL)
		return FLOAT_INVALID;
	else
		return *(this->m_pdbValue);
}

void SCtrlInfoWrap::GetValueFloat(DOUBLE& dbValue) const
{
	if (this->m_pdbValue == NULL)
		dbValue = FLOAT_INVALID;
	else
		dbValue = *(this->m_pdbValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetMinMaxInt
//	Description:	Get current control's min/max range integer data value
//  Arguments:		lMin - Min range integer value (out)
//					lMax - Max range integer value (out)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::GetMinMaxFloat(DOUBLE& dbMin, DOUBLE& dbMax) const
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetValueString
//	Description:	Get current control's string data value
//  Arguments:		strValue - String value (out)
//  Return value:	LPCTSTR
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR SCtrlInfoWrap::GetValueString(void) const
{
	if (this->m_pstrValue == NULL)
		return STRING_EMPTY;
	else
		return *(this->m_pstrValue);
}

void SCtrlInfoWrap::GetValueString(CString& strValue) const
{
	if (this->m_pstrValue == NULL)
		strValue = STRING_EMPTY;
	else
		strValue = *(this->m_pstrValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetValueIntArray
//	Description:	Get current control's integer array data value
//  Arguments:		auiValue - Integer array value (out)
//  Return value:	CUIntArray
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::GetValueIntArray(CUIntArray& auiValue) const
{
	if (this->m_pauiValueList == NULL) {
		auiValue.RemoveAll();
		auiValue.FreeExtra();
	}
	else {
		auiValue.Copy(*(this->m_pauiValueList));
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetValueStringArray
//	Description:	Get current control's string array data value
//  Arguments:		astrValue - String array value (out)
//  Return value:	CStringArray
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::GetValueStringArray(CStringArray& astrValue) const
{
	if (this->m_pastrValueList == NULL) {
		astrValue.RemoveAll();
		astrValue.FreeExtra();
	}
	else {
		astrValue.Copy(*(this->m_pastrValueList));
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetCheck
//	Description:	Set current control's check state
//  Arguments:		bCheck - Checked state (BOOLEAN)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetCheck(const BOOL& bCheck)
{
	if (this->m_pbCheck == NULL)
		this->m_pbCheck = new BOOL(bCheck);
	else {
		delete (this->m_pbCheck);
		this->m_pbCheck = new BOOL(bCheck);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetValueInt
//	Description:	Set current control's integer data value
//  Arguments:		lValue - Integer value (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetValueInt(const LONG_PTR& lValue)
{
	if (this->m_plValue == NULL)
		this->m_plValue = new LONG_PTR(lValue);
	else {
		delete (this->m_plValue);
		this->m_plValue = new LONG_PTR(lValue);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMinMaxInt
//	Description:	Set current control's min/max range integer data value
//  Arguments:		lMin - Min range integer value (in)
//					lMax - Max range integer value (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetMinMaxInt(const LONG_PTR& lMin, const LONG_PTR& lMax)
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetValueFloat
//	Description:	Set current control's float data value
//  Arguments:		dbValue - Float value (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetValueFloat(const DOUBLE& dbValue)
{
	if (this->m_pdbValue == NULL)
		this->m_pdbValue = new DOUBLE(dbValue);
	else {
		delete (this->m_pdbValue);
		this->m_pdbValue = new DOUBLE(dbValue);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMinMaxFloat
//	Description:	Set current control's min/max range float data value
//  Arguments:		lMin - Min range integer value (in)
//					lMax - Max range integer value (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetMinMaxFloat(const DOUBLE& dbMin, const DOUBLE& dbMax)
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetValueString
//	Description:	Set current control's string data value
//  Arguments:		lpszValue - String value (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetValueString(LPCTSTR lpszValue)
{
	if (this->m_pstrValue == NULL)
		this->m_pstrValue = new CString(lpszValue);
	else {
		delete (this->m_pstrValue);
		this->m_pstrValue = new CString(lpszValue);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetValueIntArray
//	Description:	Set current control's integer array data value
//  Arguments:		auiValue - Integer array value (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetValueIntArray(const CUIntArray& auiValue)
{
	if (this->m_pauiValueList == NULL)
		this->m_pauiValueList = new CUIntArray();
	
	if (this->m_pauiValueList != NULL) {
		this->m_pauiValueList->RemoveAll();
		this->m_pauiValueList->FreeExtra();
		this->m_pauiValueList->Copy(auiValue);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetValueStringArray
//	Description:	Set current control's string array data value
//  Arguments:		astrValue - String array value (in)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SCtrlInfoWrap::SetValueStringArray(const CStringArray& astrValue)
{
	if (this->m_pastrValueList == NULL)
		this->m_pastrValueList = new CStringArray();

	if (this->m_pastrValueList != NULL) {
		this->m_pastrValueList->RemoveAll();
		this->m_pastrValueList->FreeExtra();
		this->m_pastrValueList->Copy(astrValue);
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for SMenu
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(SMenu, CMenu)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SMenu
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

SMenu::SMenu(CWnd* pParentWnd /* = NULL */) : CMenu()
{
	// User menu layout
	m_pMenuLayout = NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~SMenu
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SControlManager
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

SControlManager::SControlManager(CWnd* pParentWnd /* = NULL */) : CObject()
{
	// List of control info wrappers
	m_pCtrlInfoArray = NULL;

	// Parent window
	m_pParentWnd = pParentWnd;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~SControlManager
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

SControlManager::~SControlManager()
{
	// List of control info wrappers
	if (m_pCtrlInfoArray != NULL) {
		this->DeleteAll();
		delete m_pCtrlInfoArray;
		m_pCtrlInfoArray = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Initialize
//	Description:	Initialize control management
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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
	m_pCtrlInfoArray->RemoveAll();
	m_pCtrlInfoArray->FreeExtra();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DeleteAll
//	Description:	Delete all and clean-up control management
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL SControlManager::DeleteAll(void)
{
	// If data is not initialized
	if (this->m_pCtrlInfoArray == NULL)
		return FALSE;

	// Delete all control info wrapper pointers
	for (int nIndex = 0; nIndex < (this->m_pCtrlInfoArray->GetCount()); nIndex++) {
		SCtrlInfoWrap* pExControl = m_pCtrlInfoArray->ElementAt(nIndex);
		if (pExControl != NULL) {
			delete pExControl;
			pExControl = NULL;
		}
	}

	// Empty array data
	m_pCtrlInfoArray->RemoveAll();
	m_pCtrlInfoArray->FreeExtra();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetCount
//	Description:	Get number of controls in management
//  Arguments:		None
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

inline INT_PTR SControlManager::GetCount(void) const
{
	return m_pCtrlInfoArray->GetCount();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if control management list is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

inline BOOL SControlManager::IsEmpty(void) const
{
	return m_pCtrlInfoArray->IsEmpty();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetParent
//	Description:	Get current control's parent window
//  Arguments:		None
//  Return value:	CWnd*
//
//////////////////////////////////////////////////////////////////////////

CWnd* SControlManager::GetParent(void)
{
	return m_pParentWnd;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetParent
//	Description:	Set parent window for current control
//  Arguments:		pParentWnd - Parent window
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SControlManager::SetParent(CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsParentAvailable
//	Description:	Check if parent window (pointer) is available
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SControlManager::IsParentAvailable(void) const
{
	return (m_pParentWnd != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddControl
//	Description:	Add dialog/window control to management
//  Arguments:		pControl - Dialog control item
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

INT_PTR SControlManager::AddControl(SCtrlInfoWrap* pControl)
{
	// Check for control pointer validity
	if (pControl == NULL) 
		return INT_INVALID;

	// If data is not initialized
	if (this->m_pCtrlInfoArray == NULL)
		return INT_INVALID;

	// Search if control ID had already existed
	for (int nIndex = 0; nIndex < (this->m_pCtrlInfoArray->GetCount()); nIndex++) {
		SCtrlInfoWrap* pExControl = m_pCtrlInfoArray->ElementAt(nIndex);
		if (pExControl == NULL) continue;
		if (pExControl->GetTemplateID() == pControl->GetTemplateID()) {
			// Return control index
			return nIndex;
		}
	}

	// Add control to list and return list size
	return (this->m_pCtrlInfoArray->Add(pControl));
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddControl
//	Description:	Add dialog/window control to management
//  Arguments:		nCtrlID - Dialog control ID
//					nTypeID - Control type ID
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

INT_PTR SControlManager::AddControl(UINT nCtrlID, UINT nTypeID)
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
	INT_PTR nRetIndex = this->AddControl(pControl);
	if (nRetIndex != -1)
		return nRetIndex;

	// Failed to add
	delete pControl;
	return INT_INVALID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveControl
//	Description:	Remove dialog/window control from management
//  Arguments:		nCtrlID - Dialog control ID
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

INT_PTR SControlManager::RemoveControl(UINT nCtrlID)
{
	// If data is not initialized or is empty
	if ((this->m_pCtrlInfoArray == NULL) || (this->IsEmpty()))
		return INT_INVALID;

	// Search for control ID
	for (int nIndex = 0; nIndex < (this->m_pCtrlInfoArray->GetCount()); nIndex++) {
		SCtrlInfoWrap* pExControl = this->m_pCtrlInfoArray->GetAt(nIndex);
		if (pExControl == NULL) continue;
		if (pExControl->GetTemplateID() == nCtrlID) {
			delete pExControl;

			// Remove control from list
			this->m_pCtrlInfoArray->RemoveAt(nIndex);
			this->m_pCtrlInfoArray->FreeExtra();
			return (this->m_pCtrlInfoArray->GetUpperBound());
		}
	}

	// Control ID not found, return -1
	return INT_INVALID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveControl
//	Description:	Remove dialog/window control from management
//  Arguments:		nCtrlID - Dialog control ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

SCtrlInfoWrap* SControlManager::GetControl(UINT nCtrlID)
{
	// If data is not initialized or is empty
	if ((this->m_pCtrlInfoArray == NULL) || (this->IsEmpty()))
		return NULL;

	// Search for control ID
	for (int nIndex = 0; nIndex < (this->m_pCtrlInfoArray->GetCount()); nIndex++) {
		SCtrlInfoWrap* pControl = m_pCtrlInfoArray->GetAt(nIndex);
		if (pControl == NULL) continue;
		if (pControl->GetTemplateID() == nCtrlID)
			return pControl;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetBuddy
//	Description:	Set buddy relationship between 2 controls
//  Arguments:		nBaseCtrlID  - Base control ID
//					nBuddyCtrlID - Buddy control ID
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

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

	// Set buddy control caption as base control caption
	switch (pBaseControl->GetType())
	{
		case CTRL_TYPE_EDITBOX:
		case CTRL_TYPE_COMBOBOX:
		case CTRL_TYPE_LISTBOX:
		case CTRL_TYPE_SPINCTRL:
		case CTRL_TYPE_SLIDERCTRL:
		case CTRL_TYPE_PROGRESSBAR:
		case CTRL_TYPE_LISTCTRL:
		case CTRL_TYPE_TREECTRL:
		case CTRL_TYPE_RICHEDIT:
		{
			// Get buddy control caption
			CString strCaption;
			pBuddyWnd->GetWindowText(strCaption);
			pBaseControl->SetCaption(strCaption);
		} break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateData
//	Description:	Update data for specified control or all controls
//  Arguments:		nCtrlID - Control ID (NULL means all controls)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SControlManager::UpdateData(UINT nCtrlID /* = NULL */)
{
	// If data is not initialized or is empty
	if ((this->m_pCtrlInfoArray == NULL) || (this->IsEmpty()))
		return;

	// If parent window is not available
	if (m_pParentWnd == NULL)
		return;

	// Loop through control management list
	INT nTriggerForceRetFlag = FLAG_OFF;
	for (int nIndex = 0; nIndex < (this->m_pCtrlInfoArray->GetCount()); nIndex++) {
		// Get control wrapper pointer
		SCtrlInfoWrap* pCurControl = m_pCtrlInfoArray->GetAt(nIndex);
		if (pCurControl == NULL) continue;
		if (nCtrlID != NULL) {
			if (pCurControl->GetTemplateID() != nCtrlID) {
				// Skip control
				continue;
			}
			else {
				// Trigger force return flag 
				// It will stop after updating for current control
				nTriggerForceRetFlag = FLAG_ON;
			}
		}

		// Get base control pointer
		CWnd* pBaseControl = m_pParentWnd->GetDlgItem(pCurControl->GetTemplateID());
		if (pBaseControl != NULL) {

			// Update data for control by type
			switch (pCurControl->GetType())
			{
				case CTRL_TYPE_CHECKBOX:
				case CTRL_TYPE_RADIOBTN:
				{
					// Update control's checked state
					BOOL bCheck = ((CButton*)pBaseControl)->GetCheck();
					pCurControl->SetCheck(bCheck);
				} break;

				case CTRL_TYPE_EDITBOX:
				{
					// Update control's text value
					CString strTemp;
					((CEdit*)pBaseControl)->GetWindowText(strTemp);
					pCurControl->SetValueString(strTemp);
				} break;

				case CTRL_TYPE_COMBOBOX:
				{
					// Update control's current selection index
					INT_PTR nCurSel = ((CComboBox*)pBaseControl)->GetCurSel();
					pCurControl->SetValueInt(nCurSel);
					// Update all item strings
					CStringArray arrStringData;
					arrStringData.RemoveAll();
					INT_PTR nCount = ((CComboBox*)pBaseControl)->GetCount();
					for (INT_PTR nIndex = 0; nIndex < nCount; nIndex++) {
						CString strTemp;
						((CComboBox*)pBaseControl)->GetLBText(nIndex, strTemp);
						arrStringData.Add(strTemp);
					}
					pCurControl->SetValueStringArray(arrStringData);
				} break;

				case CTRL_TYPE_LISTBOX:
				{
					// Update control's current selection index
					INT_PTR nCurSel = ((CListBox*)pBaseControl)->GetCurSel();
					pCurControl->SetValueInt(nCurSel);
					// Update all item strings
					CStringArray arrStringData;
					arrStringData.RemoveAll();
					INT_PTR nCount = ((CListBox*)pBaseControl)->GetCount();
					for (INT_PTR nIndex = 0; nIndex < nCount; nIndex++) {
						CString strTemp;
						((CListBox*)pBaseControl)->GetText(nIndex, strTemp);
						arrStringData.Add(strTemp);
					}
					pCurControl->SetValueStringArray(arrStringData);
				} break;

				case CTRL_TYPE_SLIDERCTRL:
				{
					// Update control's current position
					INT_PTR nCurPos = ((CSliderCtrl*)pBaseControl)->GetPos();
					pCurControl->SetValueInt(nCurPos);
					// Update control's min/max range
					INT_PTR nMin = ((CSliderCtrl*)pBaseControl)->GetRangeMin();
					INT_PTR nMax = ((CSliderCtrl*)pBaseControl)->GetRangeMax();
					pCurControl->SetMinMaxInt(nMin, nMax);
				} break;

				case CTRL_TYPE_PROGRESSBAR:
				{
					// Update control's current position
					INT_PTR nCurPos = ((CProgressCtrl*)pBaseControl)->GetPos();
					pCurControl->SetValueInt(nCurPos);
					// Update control's min/max range
					INT nMin = NULL, nMax = NULL;
					((CProgressCtrl*)pBaseControl)->GetRange(nMin, nMax);
					pCurControl->SetMinMaxInt(nMin, nMax);
				} break;

				case CTRL_TYPE_SPINCTRL:
				{
					// Update control's current position
					INT_PTR nCurPos = ((CSpinButtonCtrl*)pBaseControl)->GetPos();
					pCurControl->SetValueInt(nCurPos);
					// Update control's min/max range
					INT nMin = NULL, nMax = NULL;
					((CSpinButtonCtrl*)pBaseControl)->GetRange(nMin, nMax);
					pCurControl->SetMinMaxInt(nMin, nMax);
				} break;
			}
		}

		// If force return flag is ON
		if (nTriggerForceRetFlag == FLAG_ON)
			return;
	}
}

