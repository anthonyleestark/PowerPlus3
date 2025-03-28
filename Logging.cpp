
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Logging.cpp
//		Description:	Implement necessary methods to read/write app logs
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.22:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Logging.h"

///////////////////////////////////////////////////////

//// Implementations


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSizeByValue
//	Description:	Get size of data by data value
//  Arguments:		dataValue - Data value
//  Return value:	SIZE_T
//
//////////////////////////////////////////////////////////////////////////

template <typename DATA>
SIZE_T GetSizeByValue(DATA dataValue)
{
	return sizeof(dataValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSizeByType
//	Description:	Get size of data by data type
//  Arguments:		byDataType - Data type
//  Return value:	SIZE_T
//
//////////////////////////////////////////////////////////////////////////

SIZE_T GetSizeByType(BYTE byDataType)
{
	SIZE_T retSize = 0;
	switch (byDataType)
	{
	case DATA_TYPE_VOID:				// No type (unusable)
		retSize = INT_NULL;
		break;
	case DATA_TYPE_NUM_U1:				// Unsigned integer (1-byte)
	case DATA_TYPE_NUM_I1:				// Signed integer (1-byte)
	case DATA_TYPE_NUM_F1:				// Float number (1-byte)
		retSize = sizeof(UCHAR);
		break;
	case DATA_TYPE_NUM_U2:				// Unsigned integer (2-byte)
	case DATA_TYPE_NUM_I2:				// Signed integer (2-byte)
	case DATA_TYPE_NUM_F2:				// Float number (2-byte)
		retSize = sizeof(USHORT);
		break;
	case DATA_TYPE_NUM_U4:				// Unsigned integer (4-byte)
	case DATA_TYPE_NUM_I4:				// Signed integer (4-byte)
	case DATA_TYPE_NUM_F4:				// Float number (4-byte)
		retSize = sizeof(ULONG);
		break;
	case DATA_TYPE_NUM_U8:				// Unsigned integer (8-byte)
	case DATA_TYPE_NUM_I8:				// Signed integer (8-byte)
	case DATA_TYPE_NUM_F8:				// Float number (8-byte)
		retSize = sizeof(ULONGLONG);
		break;
	case DATA_TYPE_NUM_BOOLEAN:			// Boolean number (TRUE/FALSE)
		retSize = sizeof(BOOL);
		break;
	case DATA_TYPE_SYSTEMTIME:			// SYSTEMTIME struct
		retSize = sizeof(SYSTEMTIME);
		break;
	default:
		retSize = INT_NULL;
		break;
	}

	return retSize;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagLOGDETAIL
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagLOGDETAIL::tagLOGDETAIL()
{
	// Initialization
	this->usCategory = 0;										// Detail category
	this->uiDetailInfo = 0;										// Detail info (integer)
	this->strDetailInfo.Empty();								// Detail info (string)
	this->ptrDetailInfo = NULL;									// Detail info (pointer)
	this->byPointerType = DATA_TYPE_VOID;						// Detail info pointer data type
	this->szPointerSize = INT_NULL;								// Detail info pointer data size
}

tagLOGDETAIL::tagLOGDETAIL(const tagLOGDETAIL& pItem)
{
	// Copy data
	this->usCategory = pItem.usCategory;						// Detail category
	this->uiDetailInfo = pItem.uiDetailInfo;					// Detail info (integer)
	this->strDetailInfo = pItem.strDetailInfo;					// Detail info (string)
	this->PointerCopy(pItem);									// Detail info (pointer)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		Default
//  Return value:	tagLOGDETAIL&
//
//////////////////////////////////////////////////////////////////////////

tagLOGDETAIL& tagLOGDETAIL::operator=(const tagLOGDETAIL& pItem)
{
	// Copy data
	this->usCategory = pItem.usCategory;						// Detail category
	this->uiDetailInfo = pItem.uiDetailInfo;					// Detail info (integer)
	this->strDetailInfo = pItem.strDetailInfo;					// Detail info (string)
	this->PointerCopy(pItem);									// Detail info (pointer)

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Initialize/reset item data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGDETAIL::Init()
{
	// Initialization
	this->usCategory = 0;										// Detail category
	this->uiDetailInfo = 0;										// Detail info (integer)
	this->strDetailInfo.Empty();								// Detail info (string)
	this->ptrDetailInfo = NULL;									// Detail info (pointer)
	this->byPointerType = DATA_TYPE_VOID;						// Detail info pointer data type
	this->szPointerSize = INT_NULL;								// Detail info pointer data size
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGDETAIL::Copy(const tagLOGDETAIL& pItem)
{
	// Copy data
	this->usCategory = pItem.usCategory;						// Detail category
	this->uiDetailInfo = pItem.uiDetailInfo;					// Detail info (integer)
	this->strDetailInfo = pItem.strDetailInfo;					// Detail info (string)
	this->PointerCopy(pItem);									// Detail info (pointer)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PointerCopy
//	Description:	Copy detail info pointer
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGDETAIL::PointerCopy(const tagLOGDETAIL& pItem)
{
	// Copy pointer properties
	this->byPointerType = pItem.byPointerType;					// Detail info pointer data type
	this->szPointerSize = pItem.szPointerSize;					// Detail info pointer data size

	// Copy pointer data
	memcpy(this->ptrDetailInfo, pItem.ptrDetailInfo, pItem.szPointerSize);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGDETAIL::Compare(const tagLOGDETAIL& pItem) const
{
	BOOL bRet = FALSE;

	// Compare items
	bRet &= (this->usCategory == pItem.usCategory);				// Detail category
	bRet &= (this->uiDetailInfo == pItem.uiDetailInfo);			// Detail info (integer)
	bRet &= (this->strDetailInfo == pItem.strDetailInfo);		// Detail info (string)
	bRet &= this->PointerCompare(pItem);						// Detail info (pointer)

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PointerCompare
//	Description:	Compare detail info pointers
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGDETAIL::PointerCompare(const tagLOGDETAIL& pItem) const
{
	BOOL bRet = FALSE;

	// Compare properties
	bRet &= (this->byPointerType == pItem.byPointerType);		// Detail info pointer data type
	bRet &= (this->szPointerSize == pItem.szPointerSize);		// Detail info pointer data size

	// Only compare pointer values if properties are matching
	if (bRet != FALSE) {
		bRet &= memcmp(this->ptrDetailInfo, pItem.ptrDetailInfo, this->szPointerSize);
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if current detail info data is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGDETAIL::IsEmpty(void) const
{
	// Initialize empty detail info
	static const LOGDETAIL logDummyDetail;

	// Compare with that data and return result
	return this->Compare(logDummyDetail);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetPointerData
//	Description:	Set detail info pointer data
//  Arguments:		pDataBuff	- Data buffer (pointer)
//					byDataType	- Data type
//					szDataSize	- Data size
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGDETAIL::SetPointerData(PVOID pDataBuff, BYTE byDataType /* = DATA_TYPE_UNSPECIFIED */, SIZE_T szDataSize /* = 0 */)
{
	// If data type is void (unusable), do nothing
	if (byDataType == DATA_TYPE_VOID)
		return FALSE;

	// If both data type and size are not specified, do nothing
	if ((byDataType == DATA_TYPE_UNSPECIFIED) && (szDataSize == 0))
		return FALSE;

	// If size is not specified,
	if (szDataSize == 0) {
		// Get size by data type
		szDataSize = GetSizeByType(byDataType);

		// Get size failed, do nothing
		if (szDataSize == 0)
			return FALSE;
	}

	// Otherwise, set normally
	this->byPointerType = byDataType;
	this->szPointerSize = szDataSize;
	memcpy(this->ptrDetailInfo, pDataBuff, szDataSize);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagLOGITEM
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagLOGITEM::tagLOGITEM()
{
	// Initialization
	this->stTime = SYSTEMTIME_ZERO;								// Log time
	this->dwProcessID = INT_NULL;								// Process ID
	this->usCategory = LOG_MACRO_NONE;							// Log category
	this->strLogString = STRING_EMPTY;							// Log string
	this->arrDetailInfo.RemoveAll();							// Log detail info
}

tagLOGITEM::tagLOGITEM(const tagLOGITEM& pItem)
{
	// Copy data
	this->stTime = pItem.stTime;								// Log time
	this->dwProcessID = pItem.dwProcessID;						// Process ID
	this->usCategory = pItem.usCategory;						// Log category
	this->strLogString = pItem.strLogString;					// Log string
	this->arrDetailInfo.Copy(pItem.arrDetailInfo);				// Log detail info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		Default
//  Return value:	tagLOGITEM&
//
//////////////////////////////////////////////////////////////////////////

tagLOGITEM& tagLOGITEM::operator=(const tagLOGITEM& pItem)
{
	// Copy data
	this->stTime = pItem.stTime;								// Log time
	this->dwProcessID = pItem.dwProcessID;						// Process ID
	this->usCategory = pItem.usCategory;						// Log category
	this->strLogString = pItem.strLogString;					// Log string
	this->arrDetailInfo.Copy(pItem.arrDetailInfo);				// Log detail info

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another log item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGITEM::Copy(const tagLOGITEM& pItem)
{
	// Copy data
	this->stTime = pItem.stTime;								// Log time
	this->dwProcessID = pItem.dwProcessID;						// Process ID
	this->usCategory = pItem.usCategory;						// Log category
	this->strLogString = pItem.strLogString;					// Log string
	this->arrDetailInfo.Copy(pItem.arrDetailInfo);				// Log detail info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGITEM::Compare(const tagLOGITEM& pItem) const
{
	BOOL bRet = FALSE;

	// Compare item
	bRet &= (this->stTime.wHour == pItem.stTime.wHour);
	bRet &= (this->stTime.wMinute == pItem.stTime.wMinute);
	bRet &= (this->dwProcessID == pItem.dwProcessID);
	bRet &= (this->usCategory == pItem.usCategory);
	bRet &= (this->strLogString == pItem.strLogString);

	// Compare log detail info
	BOOL bDetailInfoCompare = TRUE;
	if (this->arrDetailInfo.GetSize() != pItem.arrDetailInfo.GetSize()) {
		bDetailInfoCompare = FALSE;
	}
	else {
		for (int nIndex = 0; nIndex < this->arrDetailInfo.GetSize(); nIndex++) {
			if (this->arrDetailInfo.GetAt(nIndex).Compare(pItem.arrDetailInfo.GetAt(nIndex)) != TRUE) {
				bDetailInfoCompare = FALSE;
				break;
			}
		}
	}
	bRet &= bDetailInfoCompare;

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if current log item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGITEM::IsEmpty(void) const
{
	// Initialize an empty item
	static const LOGITEM logDummyItem;

	// Compare with this item and return result
	return this->Compare(logDummyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveDetailInfo
//	Description:	Remove all log detail info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGITEM::RemoveDetailInfo(void)
{
	// Clean up log detail info data
	this->arrDetailInfo.RemoveAll();
	this->arrDetailInfo.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all log item data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGITEM::RemoveAll(void)
{
	// Reset data
	this->stTime = SYSTEMTIME_ZERO;								// Log time
	this->dwProcessID = INT_NULL;								// Process ID
	this->usCategory = LOG_MACRO_NONE;							// Log category
	this->strLogString = STRING_EMPTY;							// Log string

	// Clean up log detail info data
	this->RemoveDetailInfo();									// Log detail info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddDetail
//	Description:	Add log detail info data
//  Arguments:		logDetailInfo	- Log detail info data
//					usCategory		- Detail category
//					nDetailInfo		- Detail info (integer)
//					strDetailInfo	- Detail info (string)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGITEM::AddDetail(const LOGDETAIL& logDetailInfo)
{
	// Add detail info data
	this->arrDetailInfo.Add(logDetailInfo);
}

void tagLOGITEM::AddDetail(USHORT usCategory, UINT nDetailInfo)
{
	// Prepare detail info data
	LOGDETAIL logDetailInfo;
	logDetailInfo.usCategory = usCategory;
	logDetailInfo.uiDetailInfo = nDetailInfo;

	// Add detail info data
	this->AddDetail(logDetailInfo);
}

void tagLOGITEM::AddDetail(USHORT usCategory, LPCTSTR lpszDetailInfo)
{
	// Prepare detail info data
	LOGDETAIL logDetailInfo;
	logDetailInfo.usCategory = usCategory;
	logDetailInfo.strDetailInfo = lpszDetailInfo;

	// Add detail info data
	this->AddDetail(logDetailInfo);
}

void tagLOGITEM::AddDetail(USHORT usCategory, UINT nDetailInfo, LPCTSTR lpszDetailInfo)
{
	// Prepare detail info data
	LOGDETAIL logDetailInfo;
	logDetailInfo.usCategory = usCategory;
	logDetailInfo.uiDetailInfo = nDetailInfo;
	logDetailInfo.strDetailInfo = lpszDetailInfo;

	// Add detail info data
	this->AddDetail(logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatDateTime
//	Description:	Return a formatted logitem date/time string
//  Arguments:		None
//  Return value:	CString - Formatted result
//
//////////////////////////////////////////////////////////////////////////

CString tagLOGITEM::FormatDateTime(void) const
{
	CString strTimeFormat;
	CString strMiddayFlag = (stTime.wHour >= 12) ? SYMBOL_POST_MERIDIEM : SYMBOL_ANTE_MERIDIEM;
	strTimeFormat.Format(IDS_FORMAT_FULLDATETIME, stTime.wYear, stTime.wMonth, stTime.wDay,
						stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, strMiddayFlag);

	return strTimeFormat;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatOutput
//	Description:	Return formatted output log string for file writing
//  Arguments:		None
//  Return value:	CString - Formatted result
//
//////////////////////////////////////////////////////////////////////////

CString tagLOGITEM::FormatOutput(void) const
{
	// Create JSON data object
	JSONDATA jsonOutputData;

	CString strLogInfoIDTitle;
	CString strLogInfoValue;

	// Load default language table package
	LANGTABLE_PTR pDefLang = LoadLanguageTable(NULL);

	/*********************************************************************/
	/*																	 */
	/*			  Convert log item base info into JSON data				 */
	/*																	 */
	/*********************************************************************/

	// Log time
	strLogInfoIDTitle = GetString(strTableLogInfoIDTitle, BASELOG_INFO_TIME);
	jsonOutputData.AddString(strLogInfoIDTitle, this->FormatDateTime());

	// Process ID
	strLogInfoIDTitle = GetString(strTableLogInfoIDTitle, BASELOG_INFO_PID);
	jsonOutputData.AddInteger(strLogInfoIDTitle, this->dwProcessID);

	// Log category
	strLogInfoIDTitle = GetString(strTableLogInfoIDTitle, BASELOG_INFO_CATEGORY);
	strLogInfoValue = GetLanguageString(pDefLang, this->usCategory);
	jsonOutputData.AddString(strLogInfoIDTitle, strLogInfoValue);

	// Log description string
	strLogInfoIDTitle = GetString(strTableLogInfoIDTitle, BASELOG_INFO_DESCRIPTION);
	jsonOutputData.AddString(strLogInfoIDTitle, this->strLogString);

	/*********************************************************************/
	/*																	 */
	/*		  Convert log item detail info data into JSON data			 */
	/*																	 */
	/*********************************************************************/

	// Log detail info
	if (!this->arrDetailInfo.IsEmpty()) {

		// Create JSON detail data object
		JSONDATA jsonDetailData;

		CString strLogDetailTitle;

		// Set object name: Detail
		jsonDetailData.SetObjectName(GetString(strTableLogInfoIDTitle, BASELOG_INFO_DETAILS));

		// Convert data
		for (int nIndex = 0; nIndex < (this->arrDetailInfo.GetSize()); nIndex++) {

			// Get detail info item
			LOGDETAIL logDetail = this->arrDetailInfo.GetAt(nIndex);

			// Detail info category
			strLogDetailTitle = GetString(strTableLogInfoIDTitle, logDetail.usCategory);

			// Detail info value
			if (logDetail.uiDetailInfo != INT_NULL) {
				jsonDetailData.AddInteger(strLogDetailTitle, logDetail.uiDetailInfo);
			}
			else if (!logDetail.strDetailInfo.IsEmpty()) {
				jsonDetailData.AddString(strLogDetailTitle, logDetail.strDetailInfo);
			}
		}

		// Output detail JSON data
		jsonOutputData.AddChildObject(&jsonDetailData);
	}

	/*********************************************************************/
	/*																	 */
	/*			    Format JSON data into output string					 */
	/*																	 */
	/*********************************************************************/

	CString strLogFormat;
	jsonOutputData.Print(strLogFormat, 0, TRUE, TRUE);

	return strLogFormat;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	JSON
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

JSON::JSON()
{
	// Initialization
	this->m_strObjectName = STRING_EMPTY;			// JSON object name
	this->m_astrKeyList.RemoveAll();				// List of keys
	this->m_astrValueList.RemoveAll();				// List of values (string)
	this->m_nChildObjectCount = 0;					// Number of child objects
	this->m_apChildObjectList = NULL;				// List of child objects
}

JSON::JSON(const JSON& pObj)
{
	// Copy data
	this->m_strObjectName = pObj.m_strObjectName;	// JSON object name
	this->CopyArrayData(pObj);						// Property (array) data
	this->CopyPtrData(pObj);						// Child object (pointer) data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~JSON
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

JSON::~JSON()
{
	// Remove all data and clean-up
	this->RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		Default
//  Return value:	JSON&
//
//////////////////////////////////////////////////////////////////////////

JSON& JSON::operator=(const JSON& pObj)
{
	// Copy data
	this->m_strObjectName = pObj.m_strObjectName;	// JSON object name
	this->CopyArrayData(pObj);						// Property (array) data
	this->CopyPtrData(pObj);						// Child object (pointer) data

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another JSON object
//  Arguments:		pObj - Pointer of input object
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::Copy(const JSON& pObj)
{
	// Copy data
	this->m_strObjectName = pObj.m_strObjectName;	// JSON object name
	this->CopyArrayData(pObj);						// Property (array) data
	this->CopyPtrData(pObj);						// Child object (pointer) data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CopyArrayData
//	Description:	Copy object array data from another JSON object
//  Arguments:		pObj - Pointer of input object
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::CopyArrayData(const JSON& pObj)
{
	// Remove all existing array data
	this->m_astrKeyList.RemoveAll();
	this->m_astrValueList.RemoveAll();

	// Free destination array data memory
	this->m_astrKeyList.FreeExtra();
	this->m_astrValueList.FreeExtra();

	// Set destination array data size
	this->m_astrKeyList.SetSize(pObj.m_astrKeyList.GetSize());
	this->m_astrValueList.SetSize(pObj.m_astrValueList.GetSize());

	// Copy list of keys
	for (int nIndex = 0; nIndex < pObj.m_astrKeyList.GetSize(); nIndex++) {
		this->m_astrKeyList.SetAt(nIndex, pObj.m_astrKeyList.GetAt(nIndex));
	}

	// Copy list of values
	for (int nIndex = 0; nIndex < pObj.m_astrValueList.GetSize(); nIndex++) {
		this->m_astrValueList.SetAt(nIndex, pObj.m_astrValueList.GetAt(nIndex));
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CopyPtrData
//	Description:	Copy object pointer data from another JSON object
//  Arguments:		pObj - Pointer of input object
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::CopyPtrData(const JSON& pObj)
{
	// Number of child objects
	this->m_nChildObjectCount = pObj.m_nChildObjectCount;

	// List of child objects (pointer copy)
	if ((pObj.m_nChildObjectCount > 0) && (pObj.m_apChildObjectList != NULL)) {

		// Allocation and initialization
		this->m_apChildObjectList = new PJSONDATA[this->m_nChildObjectCount];
		if (this->m_apChildObjectList == NULL) {
			TRCLOG("Error: JSON child object array data allocation failed!!!");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}

		// Copy data
		for (int nCount = 0; nCount < this->m_nChildObjectCount; nCount++) {

			// Allocate memory
			this->m_apChildObjectList[nCount] = new JSONDATA;
			if (this->m_apChildObjectList[nCount] == NULL) {
				TRCFMT("Error: JSON new child object data allocation failed!!! (Index=%d)", nCount);
				TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
				continue;
			}

			// Get source data
			PJSONDATA pSrcData = pObj.m_apChildObjectList[nCount];
			if (pSrcData == NULL) {
				TRCFMT("Error: JSON child object is skipped when copying!!! (Index=%d)", nCount);
				TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
				continue;
			}

			std::auto_ptr<int>		auto_int_ptr;
			std::unique_ptr<int>	unique_int_ptr;
			std::shared_ptr<int>	share_int_ptr;
			std::weak_ptr<int>		weak_int_ptr;

			// Copy object data (do not use 'memcpy' in here)
			*(this->m_apChildObjectList[nCount]) = *pSrcData;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given object
//  Arguments:		pObj - Pointer of given object
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL JSON::Compare(const JSON& pObj) const
{
	BOOL bRet = FALSE;

	// Compare object name
	bRet &= (this->m_strObjectName == pObj.m_strObjectName);

	// Compare detail item info
	BOOL bRetCompareDetail = TRUE;
	if ((this->m_astrKeyList.GetSize() != pObj.m_astrKeyList.GetSize()) ||
		(this->m_astrValueList.GetSize() != pObj.m_astrValueList.GetSize())) {
		bRetCompareDetail = FALSE;
	}
	if (bRetCompareDetail != FALSE) {
		for (int nIndex = 0; nIndex < (this->m_astrKeyList.GetSize()); nIndex++) {
			if (this->m_astrKeyList.GetAt(nIndex) != pObj.m_astrKeyList.GetAt(nIndex)) {
				bRetCompareDetail = FALSE;
				break;
			}
		}
	}
	if (bRetCompareDetail != FALSE) {
		for (int nIndex = 0; nIndex < (this->m_astrValueList.GetSize()); nIndex++) {
			if (this->m_astrValueList.GetAt(nIndex) != pObj.m_astrValueList.GetAt(nIndex)) {
				bRetCompareDetail = FALSE;
				break;
			}
		}
	}
	bRet &= bRetCompareDetail;

	// Compare child objects
	if (this->m_apChildObjectList != NULL && pObj.m_apChildObjectList != NULL) {

		bRetCompareDetail = TRUE;

		// Compare child object numbers
		int nThisChildObjectCount = this->m_nChildObjectCount;
		int nOtherChildObjectCount = pObj.m_nChildObjectCount;
		if (nThisChildObjectCount != nOtherChildObjectCount) {
			bRetCompareDetail = FALSE;
		}
		else {
			// Compare each child object data
			for (int nCount = 0; nCount < nThisChildObjectCount; nCount++) {
				PJSONDATA pThisChildObject = this->m_apChildObjectList[nCount];
				PJSONDATA pOtherChildObject = pObj.m_apChildObjectList[nCount];
				if ((pThisChildObject != NULL) && (pOtherChildObject != NULL)) {
					bRetCompareDetail &= pThisChildObject->Compare(*pOtherChildObject);
				}
				else {
					bRetCompareDetail = FALSE;
					break;
				}
			}
		}
		bRet &= bRetCompareDetail;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if current JSON object is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL JSON::IsEmpty(void) const
{
	// Initialize an empty item
	static const JSONDATA jsonDummyItem;

	// Compare with this item and return result
	return this->Compare(jsonDummyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveProperty
//	Description:	Remove property by index
//  Arguments:		nIndex - Property index
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::RemoveProperty(INT_PTR nIndex)
{
	// Invalid index
	if ((nIndex < 0) || (nIndex >= this->m_astrKeyList.GetSize()))
		return;

	// Remove property by index
	this->m_astrKeyList.RemoveAt(nIndex);
	this->m_astrValueList.RemoveAt(nIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveProperty
//	Description:	Remove property by its key name
//  Arguments:		lpszKeyName - Key name
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::RemoveProperty(LPCTSTR lpszKeyName)
{
	// If property data is empty, do nothing
	if (this->m_astrKeyList.IsEmpty())
		return;

	// Search for key name
	int nFoundIndex = INT_INVALID;
	for (int nIndex = 0; nIndex < (this->m_astrKeyList.GetSize()); nIndex++) {
		if (this->m_astrKeyList.GetAt(nIndex) == lpszKeyName) {
			nFoundIndex = nIndex;
			break;
		}
	}

	// Remove property by index
	this->RemoveProperty(nFoundIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all JSON object data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::RemoveAll(void)
{
	// Reset data
	this->m_strObjectName.Empty();					// JSON object name
	this->m_astrKeyList.RemoveAll();					// List of keys
	this->m_astrValueList.RemoveAll();				// List of values (string)

	// Free extra memory
	this->m_astrKeyList.FreeExtra();					// List of keys
	this->m_astrValueList.FreeExtra();				// List of values (string)

	// Remove all child objects
	if ((this->m_nChildObjectCount > 0) && (this->m_apChildObjectList != NULL)) {
		for (int nCount = 0; nCount < this->m_nChildObjectCount; nCount++) {
			PJSONDATA pChildObj = this->m_apChildObjectList[nCount];
			if (pChildObj != NULL) {
				pChildObj->RemoveAll();
				delete pChildObj;
			}
		}
		delete[] (this->m_apChildObjectList);
		this->m_apChildObjectList = NULL;
		
		// Reset child object counter
		this->m_nChildObjectCount = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetObjectName
//	Description:	Set JSON object name
//  Arguments:		lpszObjectName - Object name
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::SetObjectName(LPCTSTR lpszObjectName)
{
	this->m_strObjectName = lpszObjectName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddString
//	Description:	Add a string-typed key-value pair, update value if
//					the given key name already existed
//  Arguments:		lpszKeyName - Key name
//					lpszValue	- String value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::AddString(LPCTSTR lpszKeyName, LPCTSTR lpszValue)
{
	// Search if key name already existed
	for (int nIndex = 0; nIndex < (this->m_astrKeyList.GetSize()); nIndex++) {
		const CString& strKeyName = this->m_astrKeyList.GetAt(nIndex);
		if (strKeyName == lpszKeyName) {
			// Replace existed value with new value
			this->m_astrValueList.SetAt(nIndex, lpszValue);
			return;
		}
	}

	// Add property
	this->m_astrKeyList.Add(lpszKeyName);
	this->m_astrValueList.Add(lpszValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddInteger
//	Description:	Add an integer-typed key-value pair, update value if
//					the given key name already existed
//  Arguments:		lpszKeyName - Item name
//					nValue		- Signed integer value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::AddInteger(LPCTSTR lpszKeyName, INT nValue)
{
	// Convert integer to string
	CString strValue;
	strValue.Format(_T("%d"), nValue);

	// Add property
	AddString(lpszKeyName, strValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddFloat
//	Description:	Add a float-typed key-value pair, update value if
//					the given key name already existed
//  Arguments:		lpszKeyName - Key name
//					dbValue		- Float value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::AddFloat(LPCTSTR lpszKeyName, DOUBLE dbValue)
{
	// Convert float number to string
	CString strValue;
	strValue.Format(_T("%f"), dbValue);

	// Add property
	AddString(lpszKeyName, strValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddChildOject
//	Description:	Add a child object
//  Arguments:		pSrc - Source item data (pointer)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::AddChildObject(JSON* pSrc)
{
	// Check for data validity
	if (pSrc == NULL)
		return;

	// Index to copy
	INT_PTR nIndex = 0;

	// Allocate child object array data memory if not yet allocated
	if (this->m_apChildObjectList == NULL) {
		this->m_apChildObjectList = new PJSONDATA;
		if (this->m_apChildObjectList == NULL) {
			TRCLOG("Error: JSON child object array data allocation failed!!!");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	else {
		nIndex = this->m_nChildObjectCount;
	}
	
	// Allocated destination child object memory
	this->m_apChildObjectList[nIndex] = new JSONDATA;
	if (this->m_apChildObjectList[nIndex] == NULL) {
		TRCFMT("Error: JSON new child object data allocation failed!!! (Index=%d)", nIndex);
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Copy child object data (do not use 'memcpy' in here)
	*(this->m_apChildObjectList[nIndex]) = *pSrc;

	// Increase child object counter
	this->m_nChildObjectCount++;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Print
//	Description:	Print JSON object data with indentation
//  Arguments:		strOutput  - Output printed result string
//					nIndent	   - Indentation
//					bSeparator - Whether to add a blank line as separator
//					bMultiline - Whether to print the data in multiple lines
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::Print(CString& strOutput, int nIndent, BOOL bSeparator, BOOL bMultiline /* = TRUE */)
{
	// Empty output result string
	strOutput.Empty();

	// Make indentation
	CString strIndent = STRING_EMPTY;
	for (int nTabCount = 1; nTabCount <= nIndent; nTabCount++) {
		// Add indent (tab character)
		strIndent.Append(SYMBOL_INDENT);
	}

	// Do not use indentation if printing in single line
	// This will make better visualization
	if (bMultiline != TRUE) {
		strIndent.Empty();
	}

	// Add indentation
	strOutput.Append(strIndent);

	CString strFormat = STRING_EMPTY;

	// Print object name (if set)
	if (!this->m_strObjectName.IsEmpty()) {
		strFormat.Format(_T("\"%s\": "), this->m_strObjectName);
		strOutput.Append(strFormat);
	}

	// Opening bracket
	strOutput.Append(_T("{ "));
	if (bMultiline == TRUE) {
		strOutput.Append(STRING_ENDLINE);
	}

	// Print list of properties
	INT_PTR nItemNum = this->m_astrKeyList.GetSize();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

		// Add indentation
		strOutput.Append(strIndent);

		// Get key and value
		CString strKeyName = this->m_astrKeyList.GetAt(nIndex);
		CString strValue = STRING_EMPTY;
		if (nIndex < this->m_astrValueList.GetSize()) {
			strValue = this->m_astrValueList.GetAt(nIndex);
		}

		// Format properties
		if ((nIndex == (nItemNum - 1)) &&
			((this->m_nChildObjectCount <= 0) || (this->m_apChildObjectList == NULL))) {

			// Last property (no other child object following) has no comma in the end
			strFormat.Format(_T("\t\"%s\": \"%s\" "), strKeyName, strValue);
			strOutput.Append(strFormat);
			if (bMultiline == TRUE) {
				strOutput.Append(STRING_ENDLINE);
			}
		}
		else {
			// Add comma character at the end of each property
			strFormat.Format(_T("\t\"%s\": \"%s\", "), strKeyName, strValue);
			strOutput.Append(strFormat);
			if (bMultiline == TRUE) {
				strOutput.Append(STRING_ENDLINE);
			}
		}
	}

	// Print child objects
	CString strSubItemOutput = STRING_EMPTY;
	if ((this->m_nChildObjectCount > 0) && (this->m_apChildObjectList != NULL)) {
		for (int nCount = 0; nCount < this->m_nChildObjectCount; nCount++) {
			PJSONDATA pSubItem = this->m_apChildObjectList[nCount];
			if (pSubItem != NULL) {
				pSubItem->Print(strSubItemOutput, nIndent + 1, FALSE, bMultiline);
				strOutput.Append(strSubItemOutput);
			}
		}
	}

	// Add indentation and closing bracket
	strOutput.Append(strIndent);
	strOutput.Append(_T("} "));
	if (bMultiline == TRUE) {
		strOutput.Append(STRING_ENDLINE);
	}

	// Add a blank line as separator
	if (bSeparator == TRUE) {
		strOutput.Append(STRING_ENDLINE);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PrintYAML
//	Description:	Print JSON object data in YAML format
//  Arguments:		strOutput  - Output printed result string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::PrintYAML(CString& strOutput)
{
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SLogging
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

SLogging::SLogging(BYTE byLogType)
{
	// Log data array
	m_arrLogData.RemoveAll();

	// Properties
	m_byLogType = byLogType;
	m_byWriteMode = LOG_WRITEMODE_NONE;
	m_nMaxSize = INT_INFINITE;
	m_strFilePath = STRING_EMPTY;
	m_pItemDefTemplate = NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~SLogging
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

SLogging::~SLogging()
{
	// Clean up log data
	m_arrLogData.RemoveAll();
	m_arrLogData.FreeExtra();

	// Clean up default item template
	if (m_pItemDefTemplate != NULL) {
		delete m_pItemDefTemplate;
		m_pItemDefTemplate = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Initialize log data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void SLogging::Init()
{
	// Initialize empty log data
	m_arrLogData.RemoveAll();
	m_arrLogData.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DeleteAll
//	Description:	Delete all log data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void SLogging::DeleteAll()
{
	// Clean up log data
	m_arrLogData.RemoveAll();
	m_arrLogData.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if current log data is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL SLogging::IsEmpty(void) const
{
	return m_arrLogData.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLogCount
//	Description:	Return the number of items of log data
//  Arguments:		None
//  Return value:	INT_PTR - Number of log items
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE INT_PTR SLogging::GetLogCount() const
{
	return m_arrLogData.GetSize();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLogItem
//	Description:	Return a specific log item of log list
//  Arguments:		nIndex - Item index
//  Return value:	LOGITEM - Return log item
//
//////////////////////////////////////////////////////////////////////////

LOGITEM& SLogging::GetLogItem(int nIndex)
{
	// If current log data is empty
	if (this->IsEmpty()) {
		// Return an empty dummy item
		static LOGITEM logDummyItem;
		return logDummyItem;
	}

	// Invalid index
	if (nIndex < 0) {
		// Return 1st item
		nIndex = 0;
	}
	else if (nIndex >= GetLogCount()) {
		// Return last item
		nIndex = (GetLogCount() - 1);
	}

	return m_arrLogData.GetAt(nIndex);
}

const LOGITEM& SLogging::GetLogItem(int nIndex) const
{
	// If current log data is empty
	if (this->IsEmpty()) {
		// Return an empty dummy item
		static const LOGITEM logDummyItem;
		return logDummyItem;
	}

	// Invalid index
	if (nIndex < 0) {
		// Return 1st item
		nIndex = 0;
	}
	else if (nIndex >= GetLogCount()) {
		// Return last item
		nIndex = (GetLogCount() - 1);
	}

	return m_arrLogData.GetAt(nIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetMaxSize
//	Description:	Get/set maximum log data size
//  Arguments:		nMaxSize - Max log data size
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE INT_PTR SLogging::GetMaxSize(void) const
{
	return m_nMaxSize;
}

void SLogging::SetMaxSize(INT_PTR nMaxSize)
{
	// Max size can only be larger than current log data size
	if (nMaxSize > (this->m_arrLogData.GetSize())) {
		m_nMaxSize = nMaxSize;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetWriteMode
//	Description:	Get/set log write mode
//  Arguments:		byWriteMode - Log write mode
//  Return value:	BYTE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BYTE SLogging::GetWriteMode(void) const
{
	return m_byWriteMode;
}

AFX_INLINE void SLogging::SetWriteMode(BYTE byWriteMode)
{
	m_byWriteMode = byWriteMode;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetFilePath
//	Description:	Get/set log output file path
//  Arguments:		strFilePath/lpszFilePath - Log file path
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE void SLogging::GetFilePath(CString& strFilePath)
{
	strFilePath = m_strFilePath;
}

AFX_INLINE void SLogging::SetFilePath(LPCTSTR lpszFilePath)
{
	m_strFilePath = lpszFilePath;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetDefaultTemplate
//	Description:	Get/set log default template
//  Arguments:		logItemTemplate - Log item template
//  Return value:	BYTE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE PLOGITEM SLogging::GetDefaultTemplate(void)
{
	return m_pItemDefTemplate;
}

void SLogging::SetDefaultTemplate(const LOGITEM& logItemTemplate)
{
	// Initialize default template
	if (m_pItemDefTemplate == NULL) {
		m_pItemDefTemplate = new LOGITEM;
		if (m_pItemDefTemplate == NULL) {
			TRCLOG("Default item initialization failed!!!");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Update template
	if (m_pItemDefTemplate != NULL) {
		if (logItemTemplate.IsEmpty()) return;
		m_pItemDefTemplate->Copy(logItemTemplate);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputItem
//	Description:	Add a log item into log data
//  Arguments:		logItem	- Log item to write
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::OutputItem(const LOGITEM& logItem)
{
	if (GetWriteMode() == LOG_WRITEMODE_INSTANT) {
		// Write instantly
		Write(logItem);
	}
	else {
		// If already reached max data size
		INT_PTR nMaxSize = GetMaxSize();
		if ((nMaxSize != INT_INFINITE) && (GetLogCount() >= nMaxSize)) {
			// Can not output log item --> Trace info
			TRCLOG("Output log item failed: Log data exceeded max size!!!");
			return;
		}

		// Store log data
		m_arrLogData.Add(logItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputLogString
//	Description:	Add a log string into log list data
//  Arguments:		lpszLogString - Log string
//					lpszLogDetail - Log detail string
//					byType		  - Log type
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::OutputString(LPCTSTR lpszLogString, BOOL bUseLastTemplate /* = TRUE */)
{
	if (GetWriteMode() == LOG_WRITEMODE_INSTANT) {
		// Write instantly
		Write(lpszLogString);
	}
	else {
		// Get log time
		SYSTEMTIME stLogTime;
		stLogTime = GetCurSysTime();

		// Prepare log item
		LOGITEM logItem;
		if (bUseLastTemplate == TRUE) {
			// Use last log item as template
			if (this->IsEmpty()) {
				// Can not output log string --> Trace info
				TRCLOG("Output log string failed: No item to use as template!!!");
				return;
			}
			else {
				// Copy template
				logItem.Copy(this->GetLogItem(GetLogCount() - 1));
			}
		}
		else {
			// Use default template
			if (this->GetDefaultTemplate() == NULL) {
				// Can not output log string --> Trace info
				TRCLOG("Output log string failed: Default template not set!!!");
				return;
			}
			else {
				// Copy template
				logItem.Copy(*(this->GetDefaultTemplate()));
			}
		}

		// Update log item data
		logItem.stTime = stLogTime;
		logItem.strLogString = lpszLogString;
		OutputItem(logItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Write
//	Description:	Write log data into the corresponding logfile
//  Arguments:		None
//  Return value:	BOOL - Result of log writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL SLogging::Write(void)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log is set as Read-only
	// or current log mode is write instantly mode
	if ((this->GetWriteMode() == LOG_WRITEMODE_NONE) ||
		(this->GetWriteMode() == LOG_WRITEMODE_INSTANT))
		return FALSE;

	CString strFileName;
	CString strFilePath;
	CString strCurFileName;
	CFile fLogFile;

	LOGITEM logItem;
	SYSTEMTIME stTemp;
	CString strLogFormat;

	// Setup performance counter for tracking
	PERFORMANCECOUNTER counter;
	counter.Start();

	for (int nIndex = 0; nIndex < GetLogCount(); nIndex++)
	{
		// Get log item
		logItem = GetLogItem(nIndex);
		stTemp = logItem.stTime;

		// Get filename according to type of logs
		switch (m_byLogType)
		{
		case LOGTYPE_APP_EVENT:
			// Format app event log filename
			strFileName.Format(FILENAME_APPEVENT_LOG, stTemp.wYear, stTemp.wMonth, stTemp.wDay);
			MakeFilePath(strFilePath, SUBFOLDER_LOG, strFileName, FILEEXT_LOGFILE);
			if (strCurFileName.IsEmpty()) {
				// Set current file name
				strCurFileName = strFileName;
			}

			// If a file with another name (previous day's log file) is opening,
			// write down all current log strings and close the file
			if ((fLogFile.m_hFile != CFile::hFileNull) && (strFileName != strCurFileName))
			{
				if (!strLogFormat.IsEmpty()) {

					// Write log strings to file
					fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
					fLogFile.Flush();

					strLogFormat.Empty();
				}

				// Close current file
				fLogFile.Close();

				// Set new current file name
				strCurFileName = strFileName;
			}
			break;
		case LOGTYPE_HISTORY_LOG:
			// App history log
			strFileName = FILENAME_HISTORY_LOG;
			MakeFilePath(strFilePath, SUBFOLDER_LOG, strFileName, FILEEXT_LOGFILE);
			break;
		default:
			// Wrong argument
			TRCLOG("Write log failed: Invalid log type!!!");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			// Show error message
			dwErrCode = APP_ERROR_WRONG_ARGUMENT;
			PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
			break;
		}

		// Check if file is opening, if not, open it
		if (fLogFile.m_hFile == CFile::hFileNull)
		{
			bResult = fLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
			if (bResult == FALSE) {

				// Open file failed
				dwErrCode = GetLastError();

				// Trace error
				TRCFMT("Write log failed: Can not open/create log file!!! (Code: 0x%08X)", dwErrCode);
				TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);

				// Show error message
				PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
				return bResult;
			}

			// Go to end of file
			fLogFile.SeekToEnd();
		}

		// Format output log strings
		strLogFormat += logItem.FormatOutput();
	}

	if (!strLogFormat.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	// Display performance counter
	counter.Stop();
	OutputDebugLogFormat(_T("Total write log time: %.4f (ms)"), counter.GetElapsedTime(TRUE));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Write
//	Description:	Write log item instantly into logfile
//  Arguments:		logItem		 - Log item to write
//					lpszFilePath - Output log file path
//  Return value:	BOOL - Result of log writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL SLogging::Write(const LOGITEM& logItem, LPCTSTR lpszFilePath /* = NULL */)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log mode is not write instantly mode
	if (this->GetWriteMode() != LOG_WRITEMODE_INSTANT)
		return FALSE;

	CString strFileName;
	CString strFilePath;
	CFile fLogFile;

	CString strLogFormat;

	// Get log time
	SYSTEMTIME stTimeTemp;
	stTimeTemp = logItem.stTime;

	// Get filename according to type of logs
	switch (m_byLogType)
	{
	case LOGTYPE_APP_EVENT:
		// Format app event log filename
		strFileName.Format(FILENAME_APPEVENT_LOG, stTimeTemp.wYear, stTimeTemp.wMonth, stTimeTemp.wDay);
		break;
	case LOGTYPE_HISTORY_LOG:
		// App history log
		strFileName = FILENAME_HISTORY_LOG;
		break;
	default:
		// Wrong argument
		TRCLOG("Error: Invalid log type");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
		PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
		return FALSE;
		break;
	}

	// Get file path
	MakeFilePath(strFilePath, SUBFOLDER_LOG, strFileName, FILEEXT_LOGFILE);

	// Check if file is opening, if not, open it
	if (fLogFile.m_hFile == CFile::hFileNull)
	{
		bResult = fLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == FALSE) {

			// Open file failed
			dwErrCode = GetLastError();

			// Trace error
			TRCFMT("Write log failed: Can not open/create log file!!! (Code: 0x%08X)", dwErrCode);
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);

			// Show error message
			dwErrCode = GetLastError();
			PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
			return bResult;
		}

		// Go to end of file
		fLogFile.SeekToEnd();
	}

	// Format output log strings
	strLogFormat = logItem.FormatOutput();

	if (!strLogFormat.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteInstantLog
//	Description:	Write log string instantly into logfile
//  Arguments:		lpszLogString - Log string
//  Return value:	BOOL - Result of log writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL SLogging::Write(LPCTSTR lpszLogString, LPCTSTR lpszFilePath /* = NULL */)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log mode is not write instantly mode
	if (this->GetWriteMode() != LOG_WRITEMODE_INSTANT)
		return FALSE;

	CString strFileName;
	CString strFilePath;
	CFile fLogFile;

	CString strLogFormat;

	// Get log time
	SYSTEMTIME stCurTime;
	GetLocalTime(&stCurTime);

	// Get filename according to type of logs
	switch (m_byLogType)
	{
	case LOGTYPE_APP_EVENT:
		// Format app event log filename
		strFileName.Format(FILENAME_APPEVENT_LOG, stCurTime.wYear, stCurTime.wMonth, stCurTime.wDay);
		break;
	case LOGTYPE_HISTORY_LOG:
		// App history log
		strFileName = FILENAME_HISTORY_LOG;
		break;
	default:
		// Wrong argument
		TRCLOG("Error: Invalid log type");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		// Show error message
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
		PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
		return FALSE;
		break;
	}

	// Get file path
	MakeFilePath(strFilePath, SUBFOLDER_LOG, strFileName, FILEEXT_LOGFILE);

	// Check if file is opening, if not, open it
	if (fLogFile.m_hFile == CFile::hFileNull)
	{
		bResult = fLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == FALSE) {

			// Open file failed
			dwErrCode = GetLastError();

			// Trace error
			TRCFMT("Write log failed: Can not open/create log file!!! (Code: 0x%08X)", dwErrCode);
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);

			// Show error message
			dwErrCode = GetLastError();
			PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
			return bResult;
		}

		// Go to end of file
		fLogFile.SeekToEnd();
	}

	// Format output log strings
	LOGITEM logItem;
	logItem.stTime = stCurTime;
	logItem.strLogString = lpszLogString;
	strLogFormat = logItem.FormatOutput();

	if (!strLogFormat.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	return TRUE;
}

