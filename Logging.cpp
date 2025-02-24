
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
	this->stTime = {0};											// Log time
	this->dwProcessID = 0;										// Process ID
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
	this->stTime = {0};											// Log time
	this->dwProcessID = 0;										// Process ID
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
	strLogInfoIDTitle = GetPairedString(strplLogInfoIDTitle, BASELOG_INFO_TIME);
	jsonOutputData.AddStringItem(strLogInfoIDTitle, this->FormatDateTime());

	// Process ID
	strLogInfoIDTitle = GetPairedString(strplLogInfoIDTitle, BASELOG_INFO_PID);
	jsonOutputData.AddIntItem(strLogInfoIDTitle, this->dwProcessID);

	// Log category
	strLogInfoIDTitle = GetPairedString(strplLogInfoIDTitle, BASELOG_INFO_CATEGORY);
	strLogInfoValue = GetLanguageString(pDefLang, this->usCategory);
	jsonOutputData.AddStringItem(strLogInfoIDTitle, strLogInfoValue);

	// Log description string
	strLogInfoIDTitle = GetPairedString(strplLogInfoIDTitle, BASELOG_INFO_DESCRIPTION);
	jsonOutputData.AddStringItem(strLogInfoIDTitle, this->strLogString);

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

		// Set group name: Detail
		jsonDetailData.SetGroupName(GetPairedString(strplLogInfoIDTitle, BASELOG_INFO_DETAILS));

		// Convert data
		for (int nIndex = 0; nIndex < (this->arrDetailInfo.GetSize()); nIndex++) {

			// Get detail info item
			LOGDETAIL logDetail = this->arrDetailInfo.GetAt(nIndex);

			// Detail info category
			strLogDetailTitle = GetPairedString(strplLogInfoIDTitle, logDetail.usCategory);

			// Detail info value
			if (logDetail.uiDetailInfo != INT_NULL) {
				jsonDetailData.AddIntItem(strLogDetailTitle, logDetail.uiDetailInfo);
			}
			else if (!logDetail.strDetailInfo.IsEmpty()) {
				jsonDetailData.AddStringItem(strLogDetailTitle, logDetail.strDetailInfo);
			}
		}

		// Output detail JSON data
		jsonOutputData.AddSubItem(&jsonDetailData);
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
//	Function name:	tagJSONDATA
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

tagJSONDATA::tagJSONDATA()
{
	// Initialization
	this->strGroupName = STRING_EMPTY;				// JSON group name
	this->astrItemName.RemoveAll();					// List of item names
	this->astrItemValue.RemoveAll();				// List of item values (string)
	this->nSubItemNum = 0;							// Number of nested sub-items
	this->apSubItemData = NULL;						// List of nested sub-items
}

tagJSONDATA::tagJSONDATA(const tagJSONDATA& pItem)
{
	// Copy data
	this->strGroupName = pItem.strGroupName;		// JSON group name
	this->CopyArrayData(pItem);						// Item array data
	this->CopyPtrData(pItem);						// Item pointer data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~tagJSONDATA
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

tagJSONDATA::~tagJSONDATA()
{
	// Remove all data and clean-up
	this->RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		Default
//  Return value:	tagJSONDATA&
//
//////////////////////////////////////////////////////////////////////////

tagJSONDATA& tagJSONDATA::operator=(const tagJSONDATA& pItem)
{
	// Copy data
	this->strGroupName = pItem.strGroupName;		// JSON group name
	this->CopyArrayData(pItem);						// Item array data
	this->CopyPtrData(pItem);						// Item pointer data

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another JSON item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::Copy(const tagJSONDATA& pItem)
{
	// Copy data
	this->strGroupName = pItem.strGroupName;		// JSON group name
	this->CopyArrayData(pItem);						// Item array data
	this->CopyPtrData(pItem);						// Item pointer data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CopyArrayData
//	Description:	Copy item array data from another JSON item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::CopyArrayData(const tagJSONDATA& pItem)
{
	// Remove all existing array data
	this->astrItemName.RemoveAll();
	this->astrItemValue.RemoveAll();

	// Free destination array data memory
	this->astrItemName.FreeExtra();
	this->astrItemValue.FreeExtra();

	// Set destination array data size
	this->astrItemName.SetSize(pItem.astrItemName.GetSize());
	this->astrItemValue.SetSize(pItem.astrItemValue.GetSize());

	// Copy list of item names
	for (int nIndex = 0; nIndex < pItem.astrItemName.GetSize(); nIndex++) {
		this->astrItemName.SetAt(nIndex, pItem.astrItemName.GetAt(nIndex));
	}

	// Copy list of item values
	for (int nIndex = 0; nIndex < pItem.astrItemValue.GetSize(); nIndex++) {
		this->astrItemValue.SetAt(nIndex, pItem.astrItemValue.GetAt(nIndex));
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CopyPtrData
//	Description:	Copy item pointer data from another JSON item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::CopyPtrData(const tagJSONDATA& pItem)
{
	// Number of nested sub-items
	this->nSubItemNum = pItem.nSubItemNum;

	// List of nested sub-items (pointer copy)
	if ((pItem.nSubItemNum > 0) && (pItem.apSubItemData != NULL)) {

		// Allocation and initialization
		this->apSubItemData = new PJSONDATA[this->nSubItemNum];
		if (this->apSubItemData == NULL) {
			TRCLOG("Error: JSON sub-item array data allocation failed!!!");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}

		// Copy data
		for (int nCount = 0; nCount < this->nSubItemNum; nCount++) {

			// Allocate memory
			this->apSubItemData[nCount] = new JSONDATA;
			if (this->apSubItemData[nCount] == NULL) {
				TRCFMT("Error: JSON new sub-item data allocation failed!!! (Index=%d)", nCount);
				TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
				continue;
			}

			// Get source data
			PJSONDATA pSrcData = pItem.apSubItemData[nCount];
			if (pSrcData == NULL) {
				TRCFMT("Error: JSON sub-item is skipped when copying!!! (Index=%d)", nCount);
				TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
				continue;
			}

			// Copy item data (do not use 'memcpy' in here)
			*(this->apSubItemData[nCount]) = *pSrcData;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagJSONDATA::Compare(const tagJSONDATA& pItem) const
{
	BOOL bRet = FALSE;

	// Compare group name
	bRet &= (this->strGroupName == pItem.strGroupName);

	// Compare detail item info
	BOOL bRetCompareDetail = TRUE;
	if ((this->astrItemName.GetSize() != pItem.astrItemName.GetSize()) ||
		(this->astrItemValue.GetSize() != pItem.astrItemValue.GetSize())) {
		bRetCompareDetail = FALSE;
	}
	if (bRetCompareDetail != FALSE) {
		for (int nIndex = 0; nIndex < (this->astrItemName.GetSize()); nIndex++) {
			if (this->astrItemName.GetAt(nIndex) != pItem.astrItemName.GetAt(nIndex)) {
				bRetCompareDetail = FALSE;
				break;
			}
		}
	}
	if (bRetCompareDetail != FALSE) {
		for (int nIndex = 0; nIndex < (this->astrItemValue.GetSize()); nIndex++) {
			if (this->astrItemValue.GetAt(nIndex) != pItem.astrItemValue.GetAt(nIndex)) {
				bRetCompareDetail = FALSE;
				break;
			}
		}
	}
	bRet &= bRetCompareDetail;

	// Compare nested sub-items
	if (this->apSubItemData != NULL && pItem.apSubItemData != NULL) {

		bRetCompareDetail = TRUE;

		// Compare nested sub-item numbers
		int nThisSubItemNum = this->nSubItemNum;
		int nOtherSubItemNum = pItem.nSubItemNum;
		if (nThisSubItemNum != nOtherSubItemNum) {
			bRetCompareDetail = FALSE;
		}
		else {
			// Compare each item data
			for (int nCount = 0; nCount < nThisSubItemNum; nCount++) {
				PJSONDATA pThisSubItem = this->apSubItemData[nCount];
				PJSONDATA pOtherSubItem = pItem.apSubItemData[nCount];
				if ((pThisSubItem != NULL) && (pOtherSubItem != NULL)) {
					bRetCompareDetail &= pThisSubItem->Compare(*pOtherSubItem);
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
//	Description:	Check if current JSON item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagJSONDATA::IsEmpty(void) const
{
	// Initialize an empty item
	static const JSONDATA jsonDummyItem;

	// Compare with this item and return result
	return this->Compare(jsonDummyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveItem
//	Description:	Remove detail item by index
//  Arguments:		nIndex - Item index
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::RemoveItem(int nIndex)
{
	// Invalid index
	if ((nIndex < 0) || (nIndex >= this->astrItemName.GetSize()))
		return;

	// Remove item by index
	this->astrItemName.RemoveAt(nIndex);
	this->astrItemValue.RemoveAt(nIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveItem
//	Description:	Remove detail item by name
//  Arguments:		lpszItemName - Item name
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::RemoveItem(LPCTSTR lpszItemName)
{
	// If data is empty, do nothing
	if (this->astrItemName.IsEmpty())
		return;

	// Search for name
	int nFoundIndex = INT_INVALID;
	for (int nIndex = 0; nIndex < (this->astrItemName.GetSize()); nIndex++) {
		if (this->astrItemName.GetAt(nIndex) == lpszItemName) {
			nFoundIndex = nIndex;
			break;
		}
	}

	// Remove item by index
	this->RemoveItem(nFoundIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all JSON item data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::RemoveAll(void)
{
	// Reset data
	this->strGroupName.Empty();						// JSON group name
	this->astrItemName.RemoveAll();					// List of item names
	this->astrItemValue.RemoveAll();				// List of item values (string)

	// Free extra memory
	this->astrItemName.FreeExtra();					// List of item names
	this->astrItemValue.FreeExtra();				// List of item values (string)

	// Remove all nested sub-items
	if ((this->nSubItemNum > 0) && (this->apSubItemData != NULL)) {
		for (int nCount = 0; nCount < this->nSubItemNum; nCount++) {
			PJSONDATA pSubItem = this->apSubItemData[nCount];
			if (pSubItem != NULL) {
				pSubItem->RemoveAll();
				delete pSubItem;
			}
		}
		delete[] (this->apSubItemData);
		this->apSubItemData = NULL;
		
		// Reset item counter
		this->nSubItemNum = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetGroupName
//	Description:	Set JSON data group name
//  Arguments:		lpszGroupName - Group name
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::SetGroupName(LPCTSTR lpszGroupName)
{
	this->strGroupName = lpszGroupName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddStringItem
//	Description:	Add string-typed item
//  Arguments:		lpszItemName - Item name
//					lpszValue	 - String value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::AddStringItem(LPCTSTR lpszItemName, LPCTSTR lpszValue)
{
	// Search if item name existed
	for (int nIndex = 0; nIndex < (this->astrItemName.GetSize()); nIndex++) {
		const CString& strItemName = this->astrItemName.GetAt(nIndex);
		if (strItemName == lpszItemName) {
			// Replace item value with new value
			this->astrItemValue.SetAt(nIndex, lpszValue);
			return;
		}
	}

	// Add item
	this->astrItemName.Add(lpszItemName);
	this->astrItemValue.Add(lpszValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddIntItem
//	Description:	Add integer-typed item
//  Arguments:		lpszItemName - Item name
//					nValue		 - Unsigned integer value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::AddIntItem(LPCTSTR lpszItemName, UINT nValue)
{
	// Convert integer to string
	CString strValue;
	strValue.Format(_T("%d"), nValue);

	// Add item
	AddStringItem(lpszItemName, strValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddFloatItem
//	Description:	Add float-typed item
//  Arguments:		lpszItemName - Item name
//					dbValue		 - Float value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::AddFloatItem(LPCTSTR lpszItemName, DOUBLE dbValue)
{
	// Convert integer to string
	CString strValue;
	strValue.Format(_T("%f"), dbValue);

	// Add item
	AddStringItem(lpszItemName, strValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddSubItem
//	Description:	Add nested sub-item
//  Arguments:		pSrc - Source item data (pointer)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::AddSubItem(tagJSONDATA* pSrc)
{
	// Check for data validity
	if (pSrc == NULL)
		return;

	// Index to copy
	INT_PTR nIndex = 0;

	// Allocate array data memory if not yet allocated
	if (this->apSubItemData == NULL) {
		this->apSubItemData = new PJSONDATA;
		if (this->apSubItemData == NULL) {
			TRCLOG("Error: JSON sub-item array data allocation failed!!!");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	else {
		nIndex = this->nSubItemNum;
	}
	
	// Allocated destination sub-item memory
	this->apSubItemData[nIndex] = new JSONDATA;
	if (this->apSubItemData[nIndex] == NULL) {
		TRCFMT("Error: JSON new sub-item data allocation failed!!! (Index=%d)", nIndex);
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Copy data (do not use 'memcpy' in here)
	*(this->apSubItemData[nIndex]) = *pSrc;

	// Increase sub-item counter
	this->nSubItemNum++;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Print
//	Description:	Print item data with indentation
//  Arguments:		strOutput  - Output printed result string
//					nIndent	   - Indentation
//					bSeparator - Whether to add a blank line as separator
//					bMultiline - Whether to print the data in multiple lines
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagJSONDATA::Print(CString& strOutput, int nIndent, BOOL bSeparator, BOOL bMultiline /* = TRUE */)
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

	// Print group name
	if (!this->strGroupName.IsEmpty()) {
		strFormat.Format(_T("\"%s\": "), this->strGroupName);
		strOutput.Append(strFormat);
	}

	// Opening bracket
	strOutput.Append(_T("{ "));
	if (bMultiline == TRUE) {
		strOutput.Append(STRING_ENDLINE);
	}

	// Print detail items
	INT_PTR nItemNum = this->astrItemName.GetSize();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		// Add indentation
		strOutput.Append(strIndent);

		// Get item name and value
		CString strItemName = this->astrItemName.GetAt(nIndex);
		CString strItemValue = STRING_EMPTY;
		if (nIndex < this->astrItemValue.GetSize()) {
			strItemValue = this->astrItemValue.GetAt(nIndex);
		}

		// Format detail item
		if (nIndex < (nItemNum - 1)) {
			// Add comma character at the end of each item
			strFormat.Format(_T("\t\"%s\": \"%s\", "), strItemName, strItemValue);
			strOutput.Append(strFormat);
			if (bMultiline == TRUE) {
				strOutput.Append(STRING_ENDLINE);
			}
		}
		else {
			// Last item has no comma in the end
			strFormat.Format(_T("\t\"%s\": \"%s\" "), strItemName, strItemValue);
			strOutput.Append(strFormat);
			if (bMultiline == TRUE) {
				strOutput.Append(STRING_ENDLINE);
			}
		}
	}

	// Print nested sub-items
	CString strSubItemOutput = STRING_EMPTY;
	if ((this->nSubItemNum > 0) && (this->apSubItemData != NULL)) {
		for (int nCount = 0; nCount < this->nSubItemNum; nCount++) {
			PJSONDATA pSubItem = this->apSubItemData[nCount];
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

	// Add a blank line as seperator
	if (bSeparator == TRUE) {
		strOutput.Append(STRING_ENDLINE);
	}
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

void SLogging::Init()
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

void SLogging::DeleteAll()
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

void SLogging::SetWriteMode(BYTE byWriteMode)
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

void SLogging::GetFilePath(CString& strFilePath)
{
	strFilePath = m_strFilePath;
}

void SLogging::SetFilePath(LPCTSTR lpszFilePath)
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

PLOGITEM SLogging::GetDefaultTemplate(void)
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

BOOL SLogging::Write()
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

