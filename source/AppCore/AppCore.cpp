/**
 * @file		AppCore.cpp
 * @brief		Implement core methods which will be used elsewhere in program
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "AppCore/AppCore.h"
#include "AppCore/Global.h"
#include "AppCore/MapTable.h"
#include "AppCore/Logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/**
 * @brief	Constructor
 */
ConfigData::ConfigData()
{
	// Main settings
	this->leftMouseAction = APP_ACTION_DISPLAYOFF;						// left mouse button action
	this->middleMouseAction = APP_ACTION_SLEEP;							// Middle mouse button action
	this->rightMouseAction = APP_ACTION_SHOWMENU;						// Right mouse button action
	this->rightMouseShowMenu = true;									// Right mouse button: Only show menu

	// Display setting
	this->languageID = APP_LANGUAGE_ENGLISH;							// Language setting

	// System settings
	this->showDialogAtStartup = true;									// Show dialog at startup
	this->enableAutoStart = true;										// Startup with Windows
	this->actionConfirmation = true;									// Show confirm message before doing action
	this->saveActionHistory = false;									// Save app history log
	this->saveAppEventLog = true;										// Save app event log
	this->runAsAdmin = false;											// Run with admin privileges
	this->showErrorMessage = true;										// Show action error message
	this->scheduleNotification = true;									// Show notify tip for schedule action
	this->allowScheduleCancellation = false;							// Allow canceling schedule when notify
	this->enableBackgroundHotkey = false;								// Enable background action hotkeys
	this->allowLockscreenHotkey = true;									// Allow background hotkeys on lockscreen
	this->enablePowerReminder = true;									// Enable Power Peminder feature
}


/**
 * @brief	Copy data from another config data
 * @param	other - Pointer of input data
 * @return	None
 */
void ConfigData::copy(const ConfigData& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Main settings
	this->leftMouseAction = other.leftMouseAction;							// left mouse button action
	this->middleMouseAction = other.middleMouseAction;						// Middle mouse button action
	this->rightMouseAction = other.rightMouseAction;						// Right mouse button action
	this->rightMouseShowMenu = other.rightMouseShowMenu;					// Right mouse button: Only show menu

	// Display setting
	this->languageID = other.languageID;									// Language setting

	// System advanced settings
	this->showDialogAtStartup = other.showDialogAtStartup;					// Show dialog at startup
	this->enableAutoStart = other.enableAutoStart;							// Startup with Windows
	this->actionConfirmation = other.actionConfirmation;					// Show confirm message before doing action
	this->saveActionHistory = other.saveActionHistory;						// Save app history log
	this->saveAppEventLog = other.saveAppEventLog;							// Save app event log
	this->runAsAdmin = other.runAsAdmin;									// Run with admin privileges
	this->showErrorMessage = other.showErrorMessage;						// Show action error message
	this->scheduleNotification = other.scheduleNotification;				// Show notify tip for schedule action
	this->allowScheduleCancellation = other.allowScheduleCancellation;		// Allow canceling schedule when notify
	this->enableBackgroundHotkey = other.enableBackgroundHotkey;			// Enable background action hotkeys
	this->allowLockscreenHotkey = other.allowLockscreenHotkey;				// Allow background hotkeys on lockscreen
	this->enablePowerReminder = other.enablePowerReminder;					// Enable Power Peminder feature
}


/**
 * @brief	Compare with another given data
 * @param	other - Pointer of given data
 * @return	true/false
 */
constexpr bool ConfigData::compare(const ConfigData& other) const noexcept
{
	bool ret = true;

	// Compare Main settings
	ret &= (this->leftMouseAction == other.leftMouseAction);						// left mouse button action
	ret &= (this->middleMouseAction == other.middleMouseAction);					// Middle mouse button action
	ret &= (this->rightMouseAction == other.rightMouseAction);						// Right mouse button action
	ret &= (this->rightMouseShowMenu == other.rightMouseShowMenu);					// Right mouse button: Only show menu

	// Compare Display setting
	ret &= (this->languageID == other.languageID);									// Language setting

	// Compare System advanced settings
	ret &= (this->showDialogAtStartup == other.showDialogAtStartup);				// Show dialog at startup
	ret &= (this->enableAutoStart == other.enableAutoStart);						// Startup with Windows
	ret &= (this->actionConfirmation == other.actionConfirmation);					// Show confirm message before doing action
	ret &= (this->saveActionHistory == other.saveActionHistory);					// Save app history log
	ret &= (this->saveAppEventLog == other.saveAppEventLog);						// Save app event log
	ret &= (this->runAsAdmin == other.runAsAdmin);									// Run with admin privileges
	ret &= (this->showErrorMessage == other.showErrorMessage);						// Show action error message
	ret &= (this->scheduleNotification == other.scheduleNotification);				// Show notify tip for schedule action
	ret &= (this->allowScheduleCancellation == other.allowScheduleCancellation);	// Allow canceling schedule when notify
	ret &= (this->enableBackgroundHotkey == other.enableBackgroundHotkey);			// Enable background action hotkeys
	ret &= (this->allowLockscreenHotkey == other.allowLockscreenHotkey);			// Allow background hotkeys on lockscreen
	ret &= (this->enablePowerReminder == other.enablePowerReminder);				// Enable Power Peminder feature

	return ret;
}


/**
 * @brief	Get a clone copy of config data
 * @param	pData - Output config data (out)
 * @return	None
 */
 void ConfigData::getData(CONFIGDATAINFO& data) const noexcept
{
	// Main settings
	data.leftMouseAction = this->leftMouseAction;								// left mouse button action
	data.middleMouseAction = this->middleMouseAction;							// Middle mouse button action
	data.rightMouseAction = this->rightMouseAction;								// Right mouse button action
	data.rightMouseShowMenu = this->rightMouseShowMenu;							// Right mouse button: Only show menu

	// Display setting
	data.languageID = this->languageID;											// Language setting

	// System advanced settings
	data.showDialogAtStartup = this->showDialogAtStartup;						// Show dialog at startup
	data.enableAutoStart = this->enableAutoStart;								// Startup with Windows
	data.actionConfirmation = this->actionConfirmation;							// Show confirm message before doing action
	data.saveActionHistory = this->saveActionHistory;							// Save app history log
	data.saveAppEventLog = this->saveAppEventLog;								// Save app event log
	data.runAsAdmin = this->runAsAdmin;											// Run with admin privileges
	data.showErrorMessage = this->showErrorMessage;								// Show action error message
	data.scheduleNotification = this->scheduleNotification;						// Show notify tip for schedule action
	data.allowScheduleCancellation = this->allowScheduleCancellation;			// Allow canceling schedule when notify
	data.enableBackgroundHotkey = this->enableBackgroundHotkey;					// Enable background action hotkeys
	data.allowLockscreenHotkey = this->allowLockscreenHotkey;					// Allow background hotkeys on lockscreen
	data.enablePowerReminder = this->enablePowerReminder;						// Enable Power Peminder feature
}


/**
 * @brief	Get application option data by ID
 * @param	eAppOptionID - Option ID
 * @return	int - Option value
 */
 int ConfigData::getAppOption(AppOptionID appOptionID) const noexcept
{
	int result = INT_INVALID;

	switch (appOptionID)
	{
	case AppOptionID::invalid:
		result = INT_INVALID;
		break;
	case AppOptionID::leftMouseAction:
		result = this->leftMouseAction;
		break;
	case AppOptionID::middleMouseAction:
		result = this->middleMouseAction;
		break;
	case AppOptionID::rightMouseAction:
		result = this->rightMouseAction;
		break;
	case AppOptionID::rightMouseShowMenu:
		result = this->rightMouseShowMenu;
		break;
	case AppOptionID::languageID:
		result = this->languageID;
		break;
	case AppOptionID::showDialogAtStartup:
		result = this->showDialogAtStartup;
		break;
	case AppOptionID::enableAutoStart:
		result = this->enableAutoStart;
		break;
	case AppOptionID::actionConfirmation:
		result = this->actionConfirmation;
		break;
	case AppOptionID::saveAppEventLog:
		result = this->saveAppEventLog;
		break;
	case AppOptionID::saveActionHistory:
		result = this->saveActionHistory;
		break;
	case AppOptionID::runAsAdmin:
		result = this->runAsAdmin;
		break;
	case AppOptionID::showErrorMessage:
		result = this->showErrorMessage;
		break;
	case AppOptionID::scheduleNotification:
		result = this->scheduleNotification;
		break;
	case AppOptionID::allowScheduleCancellation:
		result = this->allowScheduleCancellation;
		break;
	case AppOptionID::enableBackgroundHotkey:
		result = this->enableBackgroundHotkey;
		break;
	case AppOptionID::allowLockscreenHotkey:
		result = this->allowLockscreenHotkey;
		break;
	case AppOptionID::enablePowerReminder:
		result = this->enablePowerReminder;
		break;
	}

	return result;
}


/**
 * @brief	Constructor
 */
 PwrRepeatSet::PwrRepeatSet()
{
	// Init data
	isRepeated_ = false;									// Repeat daily
	isSnoozingAllowed_ = true;								// Allow snoozing mode
	snoozeInterval_ = defaultSnoozeInterval;				// Snooze interval
	repeatDays_ = defaultActiveDays;						// Default repeat: All days of week
}


/**
 * @brief	Copy data from another Repeat set data
 * @param	other - Pointer of input item
 * @return	None
 */
void PwrRepeatSet::copy(const PwrRepeatSet& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	isRepeated_ = other.isRepeated_;							// Repeat daily
	isSnoozingAllowed_ = other.isSnoozingAllowed_;				// Allow snoozing mode
	snoozeInterval_ = other.snoozeInterval_;					// Snooze interval
	repeatDays_ = other.repeatDays_;							// Days of week (for repeating)
}


/**
 * @brief	Compare data with another Repeat set data
 * @param	other - Pointer of input item
 * @return	true/false
 */
constexpr bool PwrRepeatSet::compare(const PwrRepeatSet& other) const noexcept
{
	bool retCompare = true;

	// Compare data
	retCompare &= (this->isRepeated_ == other.isRepeated_);						// Repeat daily
	retCompare &= (this->isSnoozingAllowed_ == other.isSnoozingAllowed_);		// Allow snoozing mode
	retCompare &= (this->snoozeInterval_ == other.snoozeInterval_);				// Snooze interval
	retCompare &= (this->repeatDays_ == other.repeatDays_);						// Days of week (for repeating)

	return retCompare;
}


/**
 * @brief	Constructor
 */
ScheduleItem::ScheduleItem()
{
	// Initialize
	itemId_ = ScheduleData::minItemID;				// Item ID
	isEnabled_ = false;								// Enable/disable status
	actionId_ = APP_ACTION_NOTHING;					// Schedule action ID
	timeValue_ = ClockTime();						// Schedule time
	repeatSetInfo_ = PwrRepeatSet();				// Repeat set data
}

ScheduleItem::ScheduleItem(unsigned itemID)
{
	// Initialize
	itemId_ = itemID;								// Item ID
	isEnabled_ = false;								// Enable/disable status
	actionId_ = APP_ACTION_NOTHING;					// Schedule action
	timeValue_ = ClockTime();						// Schedule time
	repeatSetInfo_ = PwrRepeatSet();				// Repeat set data
}


/**
 * @brief	Copy data from another schedule item
 * @param	other - Pointer of input item
 * @return	None
 */
void ScheduleItem::copy(const ScheduleItem& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	itemId_ = other.itemId_;						// Item ID
	isEnabled_ = other.isEnabled_;					// Enable/disable status
	actionId_ = other.actionId_;					// Schedule action ID
	timeValue_ = other.timeValue_;					// Schedule time
	repeatSetInfo_.copy(other.repeatSetInfo_);		// Repeat set data
}


/**
 * @brief	Compare with another given item
 * @param	other - Pointer of given item
 * @return	true/false
 */
constexpr bool ScheduleItem::compare(const ScheduleItem& other) const noexcept
{
	bool ret = true;

	// Compare item (do not compare item ID)
	ret &= (this->isEnabled_ == other.isEnabled_);
	ret &= (this->actionId_ == other.actionId_);
	ret &= (this->timeValue_.hour() == other.timeValue_.hour());
	ret &= (this->timeValue_.minute() == other.timeValue_.minute());
	ret &= (this->repeatSetInfo_.compare(other.repeatSetInfo_));

	return ret;
}


/**
 * @brief	Print schedule item
 * @param	outputString - Output printed string
 * @return	None
 */
void ScheduleItem::print(String& outputString) const
{
	// Use table, language and core functions
	using namespace MapTable;
	using namespace Language;
	using namespace AppCore;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format schedule data
	const wchar_t* enableState = (isEnabled_ == true) ? Constant::Value::True : Constant::Value::False;							// Enable/disable state
	unsigned actionStringID = GetPairedID(IDTable::ActionName, actionId_);
	const wchar_t* actionName = GetLanguageString(ptrLanguage, actionStringID);													// Schedule action
	const wchar_t* timeFormat = ClockTimeUtils::format(ptrLanguage, IDS_FORMAT_SHORTTIME, timeValue_).getString();				// Schedule time
	const wchar_t* repeatState = (repeatSetInfo_.isRepeatEnabled() == true) ? Constant::Value::True : Constant::Value::False;	// Repeat daily

	// Print item
	outputString.format(_T("Active=(%s), ItemID=%d, Action=(%s), Time=(%s), Repeat=(%s)"),
					enableState, itemId_, actionName, timeFormat, repeatState);
}


/**
 * @brief	Constructor
 */
ScheduleData::ScheduleData()
{
	// Initialize
	defaultItem_ = ScheduleItem(ScheduleData::defaultItemID);
	extraItemList_.clear();
}


/**
 * @brief	Copy data from another Action Schedule data
 * @param	other - Pointer of input data
 * @return	None
 */
void ScheduleData::copy(const ScheduleData& other)
{
	// Do not copy itself
	if (this == &other) return;

	// Remove existing data
	this->deleteAll();

	// Copy default item
	this->defaultItem_.copy(other.defaultItem_);

	// Copy extra data
	for (int index = 0; index < other.getExtraItemNum(); index++) {
		ScheduleItem item = other.extraItemList_.at(index);
		this->extraItemList_.push_back(item);
	}
}


/**
 * @brief	Add an Action Schedule item
 * @param	pItem - Pointer of input item
 * @return	DWORD - Error code
 */
DWORD ScheduleData::add(const ScheduleItem& item)
{
	// If item is empty, can not update
	if (item.isEmpty())
		return Error::ItemIsEmpty;

	// If default item is currently empty
	if (defaultItem_.isEmpty()) {
		// Make item as default
		ScheduleItem defaultTemp(item);
		defaultTemp.setItemId(ScheduleData::defaultItemID);
		defaultItem_.copy(defaultTemp);
		return Error::Success;
	}

	// If extra schedule data is currently empty
	if (extraItemList_.empty()) {
		// Just add the item
		extraItemList_.push_back(item);
		return Error::Success;
	}

	// If number of items exceeded limit
	if (getExtraItemNum() >= ScheduleData::maxItemNum)
		return Error::MaxItemReached;

	// Check if item is duplicated, if yes, do not add
	for (int index = 0; index < getExtraItemNum(); index++) {
		ScheduleItem itemTemp = getItemAt(index);
		if (itemTemp.compare(item) == true) {
			// All data is duplicated
			return Error::ItemDuplicated;
		}
		else if (ClockTimeUtils::isMatching(itemTemp.getTime(), item.getTime())) {
			// Time value is duplicated
			// Can not execute multiple action at the same time
			return Error::TimeDuplicated;
		}
	}

	// Create new temporary data
	ScheduleData* newData = new ScheduleData;
	newData->extraItemList_.clear();

	// Copy old data to new one
	newData->defaultItem_.copy(this->defaultItem_);
	for (int index = 0; index < this->getExtraItemNum(); index++) {
		ScheduleItem scheduleItem = this->getItemAt(index);
		newData->extraItemList_.push_back(scheduleItem);
	}

	// Add new item and copy back to old data
	newData->extraItemList_.push_back(item);
	this->copy(*newData);

	// Delete data
	newData->deleteAll();
	if (newData != NULL) {
		delete newData;
		newData = NULL;
	}

	return Error::Success;
}


/**
 * @brief	Update a Action Schedule item
 * @param	pItem - Pointer of input item
 * @return	DWORD - Error code
 */
DWORD ScheduleData::update(const ScheduleItem& item)
{
	// If default item or extra schedule data is currently empty
	if (getDefaultItem().isEmpty() || isAllEmpty()) {
		// Just add item
		return add(item);
	}

	// If item is empty, can not update
	if (item.isEmpty())
		return Error::ItemIsEmpty;

	// If item ID is matching with default item
	if (item.getItemId() == ScheduleData::defaultItemID) {
		// Update default item
		getDefaultItem().copy(item);
		return Error::Success;
	}

	// Find extra item with matching ID
	int retItemIndex = INT_INVALID;
	for (int index = 0; index < getExtraItemNum(); index++) {
		if (getItemAt(index).getItemId() == item.getItemId()) {
			retItemIndex = index;
			break;
		}
	}

	// Update item if found
	if (retItemIndex != INT_INVALID) {
		ScheduleItem& temp = getItemAt(retItemIndex);
		temp.copy(item);
		return Error::Success;
	}
	// Otherwise,
	else {
		// Just add new
		return add(item);
	}
}


/**
 * @brief	Remove a schedule item by index
 * @param	nAtIndex - Index of item to remove
 * @return	None
 */
void ScheduleData::remove(int atIndex)
{
	// Check index validity
	if ((atIndex < 0) || (atIndex >= getExtraItemNum()))
		return;

	// Get item data
	ScheduleItem& item = getItemAt(atIndex);

	// Reset item value
	item.copy(ScheduleItem());
}


/**
 * @brief	Adjust Action Schedule data validity
 * @param	None
 * @return	None
 */
void ScheduleData::adjust(void)
{
	// If default item is empty but extra data is not
	if (isDefaultEmpty() && isExtraEmpty() == false) {
		// Make first extra item default
		defaultItem_.copy(getItemAt(0));
		defaultItem_.setItemId(ScheduleData::defaultItemID);

		// Remove that extra item
		deleteItem(0);
	}

	// Check and remove empty extra items
	for (int index = (getExtraItemNum() - 1); index >= 0; index--) {
		ScheduleItem temp = getItemAt(index);
		if (!temp.isEmpty()) continue;

		// Remove item
		deleteItem(index);
	}
}


/**
 * @brief	Get next item ID (to add new item)
 * @param	None
 * @return	unsigned
 */
unsigned ScheduleData::getNextId(void) const
{
	// Get currently max ID
	unsigned retNextID = ScheduleData::minItemID;
	for (int index = 0; index < getExtraItemNum(); index++) {
		ScheduleItem item = getItemAt(index);
		if (item.getItemId() > retNextID) {
			retNextID = item.getItemId();
		}
	}

	// Increase value
	retNextID++;

	return retNextID;
}


/**
 * @brief	Check if extra data is empty
 * @param	None
 * @return	bool - Result of all item empty
 */
constexpr bool ScheduleData::isExtraEmpty(void) const noexcept
{
	// If there's no item, return true
	if (extraItemList_.empty())
		return true;

	// Check each item
	bool extraEmpty = true;
	for (int index = 0; index < getExtraItemNum(); index++) {
		if (isEmpty(index) == false) {
			extraEmpty = false;
			break;
		}
	}

	return extraEmpty;
}


/**
 * @brief	Delete a reminder item by index
 * @param	nAtIndex - Index of item to delete
 * @return	None
 */
void ScheduleData::deleteItem(int atIndex)
{
	// Check index validity
	if ((atIndex < 0) || (atIndex >= getExtraItemNum()))
		return;

	// Create new temporary data
	ScheduleData* newData = new ScheduleData;
	newData->extraItemList_.clear();

	// Copy old data to new one (except the AtIndex item)
	newData->defaultItem_.copy(this->defaultItem_);
	for (int index = 0; index < this->getExtraItemNum(); index++) {
		if (index == atIndex) continue;
		newData->extraItemList_.push_back(this->extraItemList_.at(index));
	}

	// Copy back to old data
	this->copy(*newData);

	// Delete temporary data
	newData->deleteAll();
	if (newData != NULL) {
		delete newData;
		newData = NULL;
	}
}


/**
 * @brief	Constructor
 */
HotkeySetItem::HotkeySetItem()
{
	// Initialize
	isEnabled_ = false;							// Hotkey enabled/disabled
	hotkeyActionId_ = 0;						// Hotkey action ID
	modifiers_ = 0;								// Modifier keys
	virtualKey_ = 0;							// Virtual key code
}

HotkeySetItem::HotkeySetItem(unsigned hkActionID)
{
	// Initialize
	isEnabled_ = false;							// Hotkey enabled/disabled
	hotkeyActionId_ = hkActionID;				// Hotkey action ID
	modifiers_ = 0;								// Modifier keys
	virtualKey_ = 0;							// Virtual key code
}


/**
 * @brief	Copy data from another Hotkeyset item
 * @param	other - Pointer of input item
 * @return	None
 */
void HotkeySetItem::copy(const HotkeySetItem& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	isEnabled_ = other.isEnabled_;				// Hotkey enabled/disabled
	hotkeyActionId_ = other.hotkeyActionId_;	// Hotkey action ID
	modifiers_ = other.modifiers_;				// Modifier keys
	virtualKey_ = other.virtualKey_;			// Virtual key code
}


/**
 * @brief	Print HotkeySet item
 * @param	outputString - Output printed string
 * @return	None
 */
void HotkeySetItem::print(String& outputString) const
{
	// Use table and language functions
	using namespace MapTable;
	using namespace Language;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	const wchar_t* enable = (isEnabled_ == true) ? _T("Enabled") : _T("Disabled");
	unsigned actionNameID = GetPairedID(IDTable::ActionName, GetPairedID(IDTable::HKActionID, hotkeyActionId_));
	const wchar_t* action = GetLanguageString(ptrLanguage, actionNameID);
	String keyStrokesStr = Constant::String::Empty;
	printKeyStrokes(keyStrokesStr);

	// Print item
	outputString.format(_T("State=(%s), Action=(%s), Keystrokes=(%s)"),  enable, action, keyStrokesStr.getString());
}


/**
 * @brief	Print HotkeySet item keystrokes data
 * @param	outputString - Output printed keystrokes string
 * @return	None
 */
void HotkeySetItem::printKeyStrokes(String& outputString) const
{
	// Use table and language functions
	using namespace MapTable;
	using namespace Language;

	// Format keystrokes
	String keyStrokes = Constant::String::Empty;
	if (modifiers_ & MOD_CONTROL)	keyStrokes += _T("Ctrl + ");
	if (modifiers_ & MOD_ALT)		keyStrokes += _T("Alt + ");
	if (modifiers_ & MOD_WIN)		keyStrokes += _T("Win + ");
	keyStrokes += GetString(StringTable::FunctionKeys, virtualKey_);

	// Output string
	outputString.empty();
	outputString = keyStrokes;
}


/**
 * @brief	Copy data from another HotkeySet data
 * @param	other - Pointer of input data
 * @return	None
 */
void HotkeySetData::copy(const HotkeySetData& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Remove existing data
	this->deleteAll();

	// Copy data
	for (int index = 0; index < other.getItemNum(); index++) {
		const Item& item = other.hotkeySetList_.at(index);
		this->hotkeySetList_.push_back(item);
	}
}


/**
 * @brief	Set default for HotkeySet data
 * @param	pcfgData - Pointer of HotkeySet data
 * @return	None
 */
void HotkeySetData::setDefaultData(void)
{
	// Re-initialize data
	this->init();

	// Create default data
	hotkeySetList_.reserve(6);
	hotkeySetList_.push_back(Item(HKID::displayOff));
	hotkeySetList_.push_back(Item(HKID::sleep));
	hotkeySetList_.push_back(Item(HKID::shutdown));
	hotkeySetList_.push_back(Item(HKID::restart));
	hotkeySetList_.push_back(Item(HKID::signOut));
	hotkeySetList_.push_back(Item(HKID::hibernate));
}


/**
 * @brief	Add a hotkeyset item
 * @param	pItem - Pointer of input item
 * @return	None
 */
void HotkeySetData::add(const Item& item)
{
	// If data list is current empty
	if (hotkeySetList_.empty()) {
		// Just add item
		hotkeySetList_.push_back(item);
		return;
	}

	// Check if item exists, if yes, do not add
	for (int index = 0; index < getItemNum(); index++) {
		const Item& itemTemp = getItemAt(index);
		if (itemTemp.compare(item) == true)
			return;
	}

	// Create new temporary data
	HotkeySetData* newData = new HotkeySetData;
	newData->hotkeySetList_.clear();
	
	// Copy old data to new one
	for (int index = 0; index < this->getItemNum(); index++) {
		const Item& hotkeyItem = this->getItemAt(index);
		newData->hotkeySetList_.push_back(hotkeyItem);
	}

	// Add new item and copy back to old data
	newData->hotkeySetList_.push_back(item);
	this->copy(*newData);
	
	// Delete temporary data
	newData->deleteAll();
	if (newData != NULL) {
		delete newData;
		newData = NULL;
	}
}


/**
 * @brief	Update a hotkeyset item
 * @param	pItem - Pointer of input item
 * @return	None
 */
void HotkeySetData::update(const Item& item)
{
	// If data list is current empty
	if (hotkeySetList_.empty()) {
		// Just add item
		add(item);
		return;
	}

	// Check if item with same action ID or keystrokes exists
	int dupActionIndex = INT_INVALID;
	int dupKeyIndex = INT_INVALID;

	for (int index = 0; index < getItemNum(); index++) {
		const Item& temp = getItemAt(index);
		if (temp.getActionId() == item.getActionId()) {
			// Duplicate action ID found
			dupActionIndex = index;
		}
		if (temp.compareKeycode(item) == true) {
			// Duplicate keystrokes found
			dupKeyIndex = index;
		}
	}

	// If same item existed (same action ID and same keystrokes), update its state
	if (dupActionIndex == dupKeyIndex) {
		Item& temp = getItemAt(dupActionIndex);
		temp.enableItem(item.isEnabled());
		return;
	}

	// Delete existed duplicate keystrokes
	if (dupKeyIndex != INT_INVALID) {
		remove(dupKeyIndex);
	}

	// If item with same action ID existed, update its data
	if (dupActionIndex != INT_INVALID) {
		Item& temp = getItemAt(dupActionIndex);
		temp.copy(item);
	}
	// Otherwise, add new
	else {
		add(item);
	}
}


/**
 * @brief	Remove a hotkeyset item by index
 * @param	nAtIndex - Index of item to remove
 * @return	None
 */
void HotkeySetData::remove(int atIndex)
{
	// Check index validity
	if ((atIndex < 0) || (atIndex >= getItemNum()))
		return;

	// Get item data
	Item& item = getItemAt(atIndex);

	// Reset item value
	item.enableItem(false);
	item.setKeyCode(NULL, NULL);
}


/**
 * @brief	Adjust HotkeySet data validity
 * @param	None
 * @return	None
 */
void HotkeySetData::adjust(void)
{
	DWORD ctrlKey, funcKey;
	for (int index = 0; index < getItemNum(); index++) {

		// Get hotkeyset item keycode
		Item& item = getItemAt(index);
		item.getKeyCode(ctrlKey, funcKey);

		// Not enable hotkeyset item if no keystroke data
		if ((ctrlKey == 0) || (funcKey == 0)) {
			item.enableItem(false);
			item.setKeyCode(NULL, NULL);
		}
	}
}


/**
 * @brief	Check if all item are empty
 * @param	None
 * @return	bool - Result of all item empty
 */
bool HotkeySetData::isAllEmpty(void) const noexcept
{
	// If there's no item, return true
	if (hotkeySetList_.empty())
		return true;

	// Check each item
	bool allEmpty = true;
	for (int index = 0; index < getItemNum(); index++) {
		if (isEmpty(index) == false) {
			allEmpty = false;
			break;
		}
	}

	return allEmpty;
}


/**
 * @brief	Delete a hotkeyset item by index
 * @param	nAtIndex - Index of item to delete
 * @return	None
 */
void HotkeySetData::deleteItem(int atIndex)
{
	// Check index validity
	if ((atIndex < 0) || (atIndex >= getItemNum()))
		return;

	// Create new temporary data
	HotkeySetData* newData = new HotkeySetData;
	newData->hotkeySetList_.clear();

	// Copy old data to new one (except the AtIndex item)
	for (int index = 0; index < this->getItemNum(); index++) {
		if (index == atIndex) continue;
		newData->hotkeySetList_.push_back(this->getItemAt(index));
	}

	// Copy back to old data
	this->copy(*newData);

	// Delete temporary data
	newData->deleteAll();
	if (newData != NULL) {
		delete newData;
		newData = NULL;
	}
}


/**
 * @brief	Print HotkeySet item keystrokes data by ID
 * @param	nHKID		 - Item hotkey ID
 * @param	outputString - Output printed keystrokes string
 * @return	None
 */
void HotkeySetData::printKeyStrokes(unsigned hkID, String& outputString) const
{
	// Search for hotkey ID and get keystrokes string
	String keyStrokesStr = Constant::String::Empty;
	for (int index = 0; index < this->getItemNum(); index++) {
		Item item = this->getItemAt(index);
		if (item.getActionId() == hkID) {
			item.printKeyStrokes(keyStrokesStr);
			break;
		}
	}

	// Output string
	outputString.empty();
	outputString = keyStrokesStr;
}


/**
 * @brief	Constructor
 */
RmdMsgStyleSet::RmdMsgStyleSet()
{
	// Init data
	colorBackground_ = defaultBkgrdColor;						// Background color
	colorText_ = defaultTextColor;								// Text color
	fontName_ = defaultFontName;								// Font name
	fontSize_ = defaultFontSize;								// Font size
	timeoutValue_ = defaultTimeout;								// Timeout (auto-close) interval
	iconId_ = defaultIconID;									// Message icon ID
	iconSize_ = defaultIconSize;								// Message icon size
	iconPosition_ = defaultIconPosition;						// Message icon position
	displayPosition_ = defaultDisplayPosition;					// Message display position
	marginHorizontal_ = defaultHorizontalMargin;				// Display area horizontal margin
	marginVertical_ = defaultVerticalMargin;					// Display area vertical margin
}


/**
 * @brief	Copy data from another Reminder message style set data
 * @param	other - Pointer of input item
 * @return	None
 */
void RmdMsgStyleSet::copy(const RmdMsgStyleSet& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	colorBackground_ = other.colorBackground_;				// Background color
	colorText_ = other.colorText_;							// Text color
	fontName_ = other.fontName_;							// Font name
	fontSize_ = other.fontSize_;							// Font size
	timeoutValue_ = other.timeoutValue_;					// Timeout (auto-close) interval
	iconId_ = other.iconId_;								// Message icon ID
	iconSize_ = other.iconSize_;							// Message icon size
	iconPosition_ = other.iconPosition_;					// Message icon position
	displayPosition_ = other.displayPosition_;				// Message display position
	marginHorizontal_ = other.marginHorizontal_;			// Display area horizontal margin
	marginVertical_ = other.marginVertical_;				// Display area vertical margin
}


/**
 * @brief	Compare data with another Repeat set data
 * @param	other - Pointer of input item
 * @return	true/false
 */
bool RmdMsgStyleSet::compare(const RmdMsgStyleSet& other) const noexcept
{
	bool retCompare = true;

	// Compare data
	retCompare &= (this->colorBackground_ == other.colorBackground_);		// Background color
	retCompare &= (this->colorText_ == other.colorText_);					// Text color
	retCompare &= (this->fontName_ == other.fontName_);						// Font name
	retCompare &= (this->fontSize_ == other.fontSize_);						// Font size
	retCompare &= (this->timeoutValue_ == other.timeoutValue_);				// Timeout (auto-close) interval
	retCompare &= (this->iconId_ == other.iconId_);							// Message icon ID
	retCompare &= (this->iconSize_ == other.iconSize_);						// Message icon size
	retCompare &= (this->iconPosition_ == other.iconPosition_);				// Message icon position
	retCompare &= (this->displayPosition_ == other.displayPosition_);		// Message display position
	retCompare &= (this->marginHorizontal_ == other.marginHorizontal_);		// Display area horizontal margin
	retCompare &= (this->marginVertical_ == other.marginVertical_);			// Display area vertical margin

	return retCompare;
}


/**
 * @brief	Constructor
 */
PwrReminderItem::PwrReminderItem()
{
	// Init data
	isEnabled_ = false;										// Enable state
	itemId_ = PwrReminderData::minItemID;					// Item ID
	messageContent_ = Constant::String::Empty;				// Message content
	eventId_ = Event::atSetTime;							// Event ID
	timeValue_ = ClockTime();								// Event time
	messageStyle_ = Style::messageBox;						// Reminder style
	repeatSetInfo_ = PwrRepeatSet();						// Repeat set
	useCustomStyle_ = false;								// Use message custom style
	msgStyleSetInfo_ = RmdMsgStyleSet();					// Reminder message style set
}


/**
 * @brief	Copy data from another Power Reminder item
 * @param	other - Pointer of input item
 * @return	None
 */
void PwrReminderItem::copy(const PwrReminderItem& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	isEnabled_ = other.isEnabled_;						// Enable state
	itemId_ = other.itemId_;							// Item ID
	messageContent_ = other.messageContent_;			// Message content
	eventId_ = other.eventId_;							// Event ID
	timeValue_ = other.timeValue_;						// Event time
	messageStyle_ = other.messageStyle_;				// Reminder style
	repeatSetInfo_.copy(other.repeatSetInfo_);			// Repeat set
	useCustomStyle_ = other.useCustomStyle_;			// Use message custom style
	msgStyleSetInfo_.copy(other.msgStyleSetInfo_);		// Reminder message style set
}


/**
 * @brief	Compare with another given item
 * @param	other - Pointer of given item
 * @return	true/false
 */
bool PwrReminderItem::compare(const PwrReminderItem& other) const noexcept
{
	bool bRet = true;

	// Compare item (do not compare item ID)
	bRet &= (this->messageContent_ == other.messageContent_);
	bRet &= (this->eventId_ == other.eventId_);
	bRet &= (this->timeValue_.hour() == other.timeValue_.hour());
	bRet &= (this->timeValue_.minute() == other.timeValue_.minute());
	bRet &= (this->messageStyle_ == other.messageStyle_);
	bRet &= (this->repeatSetInfo_.compare(other.repeatSetInfo_));
	bRet &= (this->useCustomStyle_ == other.useCustomStyle_);
	bRet &= (this->msgStyleSetInfo_.compare(other.msgStyleSetInfo_));

	return bRet;
}


/**
 * @brief	Check if item snooze mode is available
 * @param	None
 * @return	true/false
 */
bool PwrReminderItem::isAllowSnoozing(void) const noexcept
{
	// If current eventID is not at settime
	if (eventId_ != Event::atSetTime) {
		// Not allow snooze mode
		return false;
	}
	// If repeat option is currently OFF
	if (isRepeatEnabled() != true) {
		// Not allow snooze mode
		return false;
	}
	// If allow snooze option is OFF
	if (repeatSetInfo_.isAllowSnoozing() != true) {
		// Not allow snooze mode
		return false;
	}

	return true;
}


/**
 * @brief	Print reminder item
 * @param	outputString - Output printed string
 * @return	None
 */
void PwrReminderItem::print(String& outputString) const
{
	// Use table, language and core functions
	using namespace MapTable;
	using namespace Language;
	using namespace AppCore;

	// Get language table
	LANGTABLE_PTR ptrLanguage = LoadLanguageTable(NULL);

	// Format item data
	const wchar_t* enableStr = (isEnabled_ == true) ? _T("Enabled") : _T("Disabled");
	String messageStr = messageContent_;
	if (messageStr.getLength() > (Constant::Max::DisplayLogStringLength + 3)) {
		messageStr = messageContent_.left(Constant::Max::DisplayLogStringLength) + _T("...");
	}
	int temp = GetPairedID(IDTable::PwrReminderEvent, eventId_);
	String eventStr = GetLanguageString(ptrLanguage, temp);
	if (eventId_ == Event::atSetTime) {
		// Format time string
		String formatString = eventStr;
		eventStr = ClockTimeUtils::format(ptrLanguage, formatString, timeValue_);
	}
	temp = GetPairedID(IDTable::PwrReminderStyle, messageStyle_);
	const wchar_t* styleStr = GetLanguageString(ptrLanguage, temp);

	// Print item
	outputString.format(_T("State=(%s), ItemID=%d, Msg=(%s), Event=(%s), Style=(%s), Repeat=%d"),
		enableStr, itemId_, messageStr.getString(), eventStr.getString(), styleStr, repeatSetInfo_.isRepeatEnabled());
}


/**
 * @brief	Constructor
 */
PwrReminderData::PwrReminderData()
{
	// Initialize
	reminderItemList_.clear();
	commonStyleSet_ = RmdMsgStyleSet();
}


/**
 * @brief	Init Power Reminder data (NULL)
 * @param	None
 * @return	None
 */
void PwrReminderData::init() noexcept
{
	// Initialize
	reminderItemList_.clear();
	commonStyleSet_ = RmdMsgStyleSet();
}


/**
 * @brief	Copy data from another Power Reminder data
 * @param	other - Pointer of input data
 * @return	None
 */
 void PwrReminderData::copy(const PwrReminderData& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Remove existing data
	this->deleteAll();

	// Copy reminder data
	for (int index = 0; index < other.getItemNum(); index++) {
		PwrReminderItem item = other.reminderItemList_.at(index);
		this->reminderItemList_.push_back(item);
	}

	// Copy common message style data
	this->commonStyleSet_.copy(other.commonStyleSet_);
}


/**
 * @brief	Add a Power Reminder item
 * @param	pItem - Pointer of input item
 * @return	None
 */
 void PwrReminderData::add(const PwrReminderItem& item)
{
	// If data list is current empty
	if (reminderItemList_.empty()) {
		// Just add item
		reminderItemList_.push_back(item);
		return;
	}

	// Check if item exists, if yes, do not add
	for (int index = 0; index < getItemNum(); index++) {
		PwrReminderItem itemTemp = getItemAt(index);
		if (itemTemp.compare(item) == true)
			return;
	}

	// Create new temporary data
	PwrReminderData* newData = new PwrReminderData;
	newData->reminderItemList_.clear();

	// Copy common message style data
	newData->commonStyleSet_.copy(this->commonStyleSet_);

	// Copy old data to new one
	for (int index = 0; index < this->getItemNum(); index++) {
		PwrReminderItem reminderItem = this->getItemAt(index);
		newData->reminderItemList_.push_back(reminderItem);
	}

	// Add new item and copy back to old data
	newData->reminderItemList_.push_back(item);
	this->copy(*newData);

	// Delete data
	newData->deleteAll();
	if (newData != NULL) {
		delete newData;
		newData = NULL;
	}
}


/**
 * @brief	Update a Power Reminder item
 * @param	pItem - Pointer of input item
 * @return	None
 */
 void PwrReminderData::update(const PwrReminderItem& item)
{
	// If data list is current empty
	if (reminderItemList_.empty()) {
		// Just add item
		add(item);
		return;
	}

	// Find item index
	int retItemIndex = INT_INVALID;
	for (int index = 0; index < getItemNum(); index++) {
		if (getItemAt(index).getItemId() == item.getItemId()) {
			retItemIndex = index;
			break;
		}
	}

	// Update item if found
	if (retItemIndex != INT_INVALID) {
		PwrReminderItem& temp = getItemAt(retItemIndex);
		temp.copy(item);
	}
	// Otherwise, add new
	else {
		add(item);
	}
}


/**
 * @brief	Remove a reminder item by index
 * @param	nAtIndex - Index of item to remove
 * @return	None
 */
 void PwrReminderData::remove(int atIndex)
{
	// Check index validity
	if ((atIndex < 0) || (atIndex >= getItemNum()))
		return;

	// Get item data
	PwrReminderItem& item = getItemAt(atIndex);

	// Reset item value
	item.copy(PwrReminderItem());
}


/**
 * @brief	Adjust Power Reminder data validity
 * @param	None
 * @return	None
 */
 void PwrReminderData::adjust(void)
{
	// Remove garbage items
	for (int index = (getItemNum() - 1); index >= 0; index--) {
		// Get item
		PwrReminderItem temp = getItemAt(index);
		if (!temp.isEmpty()) continue;

		// Remove empty if item
		deleteItem(index);
	}
}


/**
 * @brief	Get next item ID (to add new item)
 * @param	None
 * @return	unsigned
 */
 unsigned PwrReminderData::getNextId(void) const noexcept
{
	// Get max ID
	unsigned retNextID = PwrReminderData::minItemID;
	for (int index = 0; index < getItemNum(); index++) {
		PwrReminderItem item = getItemAt(index);
		if (item.getItemId() > retNextID) {
			retNextID = item.getItemId();
		}
	}

	// Increase value
	retNextID++;

	return retNextID;
}


/**
 * @brief	Check if all item are empty
 * @param	None
 * @return	bool - Result of all item empty
 */
 bool PwrReminderData::isAllEmpty() const noexcept
{
	// If there's no item, return true
	if (reminderItemList_.empty())
		return true;

	// Check each item
	bool allEmpty = true;
	for (int index = 0; index < getItemNum(); index++) {
		if (isEmpty(index) == false) {
			allEmpty = false;
			break;
		}
	}

	return allEmpty;
}


/**
 * @brief	Delete a reminder item by index
 * @param	nAtIndex - Index of item to delete
 * @return	None
 */
 void PwrReminderData::deleteItem(int atIndex)
{
	// Check index validity
	if ((atIndex < 0) || (atIndex >= getItemNum()))
		return;

	// Create new temporary data
	PwrReminderData* newData = new PwrReminderData;
	newData->reminderItemList_.clear();

	// Copy common message style data
	newData->commonStyleSet_.copy(this->commonStyleSet_);

	// Copy old data to new one (except the AtIndex item)
	for (int index = 0; index < this->getItemNum(); index++) {
		if (index == atIndex) continue;
		newData->reminderItemList_.push_back(this->reminderItemList_.at(index));
	}

	// Copy back to old data
	this->copy(*newData);

	// Delete temporary data
	newData->deleteAll();
	if (newData != NULL) {
		delete newData;
		newData = NULL;
	}
}


/**
 * @brief	Constructor
 */
 PwrRuntimeItem::PwrRuntimeItem()
{
	// Init data
	categoryId_ = INT_INVALID;						// Item category
	itemId_ = INT_NULL;								// Power Reminder item ID
	displayFlag_ = FLAG_OFF;						// Item displaying flag
	skipFlag_ = FLAG_OFF;							// Item skip flag
	snoozeFlag_ = FLAG_OFF;							// Item snooze trigger flag
	nextSnoozeTime_ = ClockTime();					// Next snooze trigger time
}


/**
 * @brief	Copy data from another Power++ runtime info item
 * @param	other - Pointer of input item
 * @return	None
 */
 void PwrRuntimeItem::copy(const PwrRuntimeItem& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	categoryId_ = other.categoryId_;					// Item category
	itemId_ = other.itemId_;							// Power Reminder item ID
	displayFlag_ = other.displayFlag_;					// Item displaying flag
	skipFlag_ = other.skipFlag_;						// Item skip flag
	snoozeFlag_ = other.snoozeFlag_;					// Item snooze trigger flag
	nextSnoozeTime_ = other.nextSnoozeTime_;			// Next snooze trigger time
}


/**
 * @brief	Calculate next snooze trigger time
 * @param	nInterval - Snooze interval
 * @return	None
 */
void PwrRuntimeItem::calcNextSnoozeTime(int interval) noexcept
{
	ClockTimeUtils::calculateOffset(nextSnoozeTime_, interval);
}


/**
 * @brief	Constructor
 */
 HistoryInfoData::HistoryInfoData()
{
	// Init data
	isInitiated_ = false;							// Init state
	categoryId_ = INT_NULL;							// Category ID
	timestampValue_ = DateTime();					// Timestamp of history
	itemId_ = INT_NULL;								// Item ID
	actionId_ = INT_NULL;							// History action ID
	actionResult_ = true;							// Action result
	errorCode_ = APP_ERROR_SUCCESS;					// Returned error code
	description_ = Constant::String::Empty;			// History description (attached info)
}


/**
 * @brief	Copy data from another action history data
 * @param	other - Pointer of input data
 * @return	None
 */
void HistoryInfoData::copy(const HistoryInfoData& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	isInitiated_ = other.isInitiated_;					// Init state
	categoryId_ = other.categoryId_;					// Category ID
	timestampValue_ = other.timestampValue_;			// Timestamp of history
	itemId_ = other.itemId_;							// Item ID
	actionId_ = other.actionId_;						// History action ID
	actionResult_ = other.actionResult_;				// Action result
	errorCode_ = other.errorCode_;						// Returned error code
	description_ = other.description_;					// History description (attached info)
}


/**
 * @brief	Initialization
 * @param	nCategoryID - Category ID
 * @return	None
 */
void HistoryInfoData::init(unsigned categoryID) noexcept
{
	// Reset data
	removeAll();
	isInitiated_ = true;										// Init state
	categoryId_ = categoryID;									// Category ID
	timestampValue_ = DateTimeUtils::getCurrentDateTime();		// Timestamp of history
	actionResult_ = true;										// Action result
	errorCode_ = APP_ERROR_SUCCESS;								// Returned error code
	description_ = Constant::String::Empty;						// History description (attached info)
}


/**
 * @brief	Constructor
 */
SystemEvent::SystemEvent(EventID eventID)
{
	// Initialize
	eventId_ = eventID;								// System event ID
	timestampValue_ = DateTime();					// Event timestamp
}

SystemEvent::SystemEvent(const SystemEvent& other)
{
	// Copy data
	eventId_ = other.eventId_;						// System event ID
	timestampValue_ = other.timestampValue_;		// Event timestamp
}


/**
 * @brief	Copy assignment operator
 * @param	other - Pointer of input item
 * @return	None
 */
SystemEvent& SystemEvent::operator=(const SystemEvent& other)
{
	// Copy data
	eventId_ = other.eventId_;						// System event ID
	timestampValue_ = other.timestampValue_;		// Event timestamp

	return *this;
}


/**
 * @brief	Constructor
 */
SystemEventTracker::SystemEventTracker()
{
	// Initialize
	trackingData_.clear();							// System event tracking data
}

SystemEventTracker::SystemEventTracker(const SystemEventTracker& other)
{
	// Copy data
	trackingData_ = other.trackingData_;			// System event tracking data
}


/**
 * @brief	Copy assignment operator
 * @param	other - Pointer of input data
 * @return	None
 */
SystemEventTracker& SystemEventTracker::operator=(const SystemEventTracker& other)
{
	// Copy data
	trackingData_ = other.trackingData_;			// System event tracking data

	return *this;
}


/**
 * @brief	Format string (same as default MFC Format function)
 * @param	formatTemplateID  - ID of resource format template string
 * @param	...				  - Same as default MFC Format function
 * @return	String - Returned formatted string
 */
String StringUtils::stringFormat(unsigned formatTemplateID, ...)
{
	// Load resource format template string
	String templateString = StringUtils::loadResourceString(formatTemplateID);
	if (templateString.isEmpty())
		return Constant::String::Empty;

	// Template string validation
	ATLASSERT(AtlIsValidString(templateString));

	// Result string
	String resultString;

	// Format string
	va_list argList;
	va_start(argList, formatTemplateID);
	resultString.formatV(templateString, argList);
	va_end(argList);

	return resultString;
}


/**
 * @brief	Format string (same as default MFC Format function)
 * @param	formatTemplate - Format template string
 * @param	...			   - Same as default MFC Format function
 * @return	String	- Returned formatted string
 */
String StringUtils::stringFormat(const wchar_t* formatTemplate, ...)
{
	// Template string validation
	ATLASSERT(AtlIsValidString(formatTemplate));

	// Result string
	String resultString;

	// Format string
	va_list argList;
	va_start(argList, formatTemplate);
	resultString.formatV(formatTemplate, argList);
	va_end(argList);

	return resultString;
}


/**
 * @brief	Load resource ID and return the string
 * @param	nResStringID - ID of resource string
 * @return	String	- Returned resource string
 */
String StringUtils::loadResourceString(unsigned resourceStringID)
{
	// Output result
	String resultString;

	// Get resource handle
	HINSTANCE hResInstance = AfxGetResourceHandle();
	if (hResInstance == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get resource handle failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return resultString;
	}

	// Load resource string
	wchar_t _tempBuffer[Constant::Max::StringLength] = {0};
	int _length = ::LoadStringW(hResInstance, resourceStringID, _tempBuffer, static_cast<int>(std::size(_tempBuffer)));
	if (_length <= 0)
		resultString = Constant::String::Null;
	else
		resultString.setString(_tempBuffer);

	return resultString;
}


/**
 * @brief	Load resource ID and return the string
 * @param	strResult	 - Returned resource string
 * @param	nResStringID - ID of resource string
 * @return	true/false
 */
bool StringUtils::loadResourceString(String& resultStr, unsigned resourceStringID)
{
	// Output result
	resultStr.empty();

	// Get resource handle
	HINSTANCE hResInstance = AfxGetResourceHandle();
	if (hResInstance == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get resource handle failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Load resource string
	wchar_t _tempBuffer[Constant::Max::StringLength] = {0};
	int _length = ::LoadStringW(hResInstance, resourceStringID, _tempBuffer, static_cast<int>(std::size(_tempBuffer)));
	if (_length <= 0) {
		resultStr = Constant::String::Null;
		return false;
	}
	else
		resultStr.setString(_tempBuffer);

	return true;
}


/**
 * @brief	Load resource text file by ID and return the text data
 * @param	resourceFileID - ID of the file in resource
 * @return	true/false
 */
String StringUtils::loadResourceTextData(unsigned resourceFileID)
{
	// Get resource handle
	HINSTANCE hResInstance = AfxGetResourceHandle();
	if (hResInstance == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get resource handle failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return Constant::String::Empty;
	}

	// Find resource file by ID
	HRSRC hRes = FindResource(hResInstance, MAKEINTRESOURCE(resourceFileID), RT_RCDATA);
	if (hRes != NULL) {

		// Load resource data
		HGLOBAL hData = LoadResource(hResInstance, hRes);
		if (hData != NULL) {

			// Convert resource data to text data
			const size_t dataSize = SizeofResource(hResInstance, hRes);
			const wchar_t* dataBuffer = static_cast<const wchar_t*>(LockResource(hData));
			if (dataBuffer) {
				const size_t wcharCount = dataSize / sizeof(wchar_t);
				std::vector<wchar_t> textBuffer(dataBuffer, dataBuffer + wcharCount);
				textBuffer.push_back(Constant::Char::EndString);
				return String(textBuffer.data());
			}
		}
	}

	return Constant::String::Empty;
}


/**
 * @brief	Get application executable file path
 * @param	includeExeName - Including executable file name
 * @return	String - Return application path
 */
String StringUtils::getApplicationPath(bool includeExeName)
{
	// Get the application's module handle
	HMODULE hModule = GetModuleHandle(NULL);

	// Get the full path of the executable file of the module
	wchar_t appPathBuffer[MAX_PATH];
	if (!GetModuleFileName(hModule, appPathBuffer, MAX_PATH)) {
		// Trace error
		TRACE_FORMAT("Error: Get module file name failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return Constant::String::Empty;
	}

	// Full path result
	String retAppPath(appPathBuffer);

	// If not including the executable file name
	if (!includeExeName) {
		// Remove the executable file name (and the last '\' as well)
		int nPos = retAppPath.reverseFind(Constant::Char::Backslash);
		if (nPos != INT_INVALID) {
			String strTemp = retAppPath.left(nPos);
			retAppPath = strTemp;
		}
	}

	return retAppPath;
}


/**
 * @brief	Get full sub-folder path
 * @param	lpszSubFolderName - Subfolder name
 * @return	String
 */
String StringUtils::getSubFolderPath(const wchar_t* subFolderName)
{
	// Get application executable file path
	String appPath = getApplicationPath(false);

	// Initialize result string
	String retSubFolderPath;

	// Make sub-folder path
	retSubFolderPath = appPath;
	if (subFolderName) {
		retSubFolderPath.append(Constant::Symbol::Backslash);
		retSubFolderPath.append(subFolderName);
	}

	return retSubFolderPath;
}


/**
 * @brief	Make file path by given part names
 * @param	directory - Directory path
 * @param	fileName  - File name
 * @param	extension - File extension
 * @return	String
 */
String StringUtils::makeFilePath(const wchar_t* directory, const wchar_t* fileName, const wchar_t* extension)
{
	// Format file path
	String strFilePath;

	// Directory path, it may or may not be specified
	// If not specified, it means targeted file is in the same folder with executable file
	if (directory) {
		// Add directory path
		strFilePath.append(directory);
		strFilePath.append(Constant::Symbol::Backslash);
	}

	// File name must be specified
	if (fileName) {
		strFilePath.append(fileName);
	}
	else {
		strFilePath.empty();
		return strFilePath;
	}

	// File extension, it may or may not be specified
	// If not specified, it means targeted file has no extension
	if (extension) {
		// Add file extension
		strFilePath.append(extension);
	}

	return strFilePath;
}


/**
 * @brief	Get app executable file product version info
 * @param	isFullVersion - Full product version number (x.x.x.x)
 * @param	or short version number (x.x)
 * @return	String - Product version string
 */
String StringUtils::getProductVersion(bool isFullVersion)
{
	// Get product file name
	String productFileName = StringUtils::makeFilePath(NULL, Constant::File::Name::App_Executable, Constant::File::Extension::Exe);
	if (productFileName.isEmpty()) {
		// Trace error
		TRACE_ERROR("Error: Make file path failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return Constant::String::Empty;
	}

	// Get file version info size
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(productFileName, &dwHandle);
	if (dwSize <= 0) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info size failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return Constant::String::Empty;
	}

	// Get file verision info structure
	BYTE* pVersionInfo = new BYTE[dwSize];
	if (!GetFileVersionInfo(productFileName, dwHandle, dwSize, pVersionInfo)) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return Constant::String::Empty;
	}

	// Querry version value
	unsigned uLen;
	VS_FIXEDFILEINFO* lpFfi;
	if (!VerQueryValue(pVersionInfo, Constant::Symbol::Backslash, (LPVOID*)&lpFfi, &uLen)) {
		// Trace error
		TRACE_FORMAT("Error: Querry version value failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return Constant::String::Empty;
	}

	// Result product version
	String productVersion;

	// Get product version successfully
	DWORD dwProductVersionMS = lpFfi->dwProductVersionMS;
	DWORD dwProductVersionLS = lpFfi->dwProductVersionLS;
	if (isFullVersion) {
		// Get full product version number (x.x.x.x)
		productVersion.format(_T("%d.%d.%d.%d"),
			HIWORD(dwProductVersionMS),
			LOWORD(dwProductVersionMS),
			HIWORD(dwProductVersionLS),
			LOWORD(dwProductVersionLS));
	}
	else {
		// Get short product version number (x.x)
		productVersion.format(_T("%d.%d"),
			HIWORD(dwProductVersionMS),
			LOWORD(dwProductVersionMS));
	}

	delete[] pVersionInfo;

	return productVersion;
}


/**
 * @brief	Get app executable file product version info
 * @param	fullVersion  - Full product version number (x.x.x.x)
 * @param	shortVersion - Short product version number (x.x)
 * @return	true/false
 */
bool StringUtils::getProductVersion(String& fullVersion, String& shortVersion)
{
	// Get product file name
	String productFileName = StringUtils::makeFilePath(NULL, Constant::File::Name::App_Executable, Constant::File::Extension::Exe);
	if (productFileName.isEmpty()) {
		// Trace error
		TRACE_ERROR("Error: Make file path failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get product version info size
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(productFileName, &dwHandle);
	if (dwSize <= 0) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info size failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get product version info structure
	BYTE* pVersionInfo = new BYTE[dwSize];
	if (!GetFileVersionInfo(productFileName, dwHandle, dwSize, pVersionInfo)) {
		// Trace error
		TRACE_FORMAT("Error: Get file version info structure failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return false;
	}

	// Querry version value
	unsigned uLen;
	VS_FIXEDFILEINFO* lpFfi;
	if (!VerQueryValue(pVersionInfo, Constant::Symbol::Backslash, (LPVOID*)&lpFfi, &uLen)) {
		// Trace error
		TRACE_FORMAT("Error: Querry version value failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		delete[] pVersionInfo;
		return false;
	}

	// Empty destination product version strings
	fullVersion.empty();
	shortVersion.empty();

	// Get product version successfully
	DWORD dwProductVersionMS = lpFfi->dwProductVersionMS;
	DWORD dwProductVersionLS = lpFfi->dwProductVersionLS;

	// Get full product version number (x.x.x.x)
	fullVersion.format(_T("%d.%d.%d.%d"),
		HIWORD(dwProductVersionMS),
		LOWORD(dwProductVersionMS),
		HIWORD(dwProductVersionLS),
		LOWORD(dwProductVersionLS));

	// Get short product version number (x.x)
	shortVersion.format(_T("%d.%d"),
		HIWORD(dwProductVersionMS),
		LOWORD(dwProductVersionMS));

	delete[] pVersionInfo;

	return true;
}


/**
 * @brief	Get the computer device name
 * @param	deviceName - Device name (out)
 * @return	true/false
 */
bool StringUtils::getDeviceName(String& deviceName)
{
	// Empty the output string
	deviceName.empty();

	// Get the computer device name
	wchar_t deviceNameBuffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwNameLength = sizeof(deviceNameBuffer) / sizeof(wchar_t);
	if (!GetComputerName(deviceNameBuffer, &dwNameLength)) {
		// Trace error
		TRACE_FORMAT("Error: Get computer name failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Return the computer name
	deviceName.setString(deviceNameBuffer);
	return true;
}


/**
 * @brief	Get the current Windows user name
 * @param	userName - User name (out)
 * @return	true/false
 */
bool StringUtils::getCurrentUserName(String& userName)
{
	// Empty the output string
	userName.empty();

	// Get the current user name
	wchar_t userNameBuffer[UNLEN + 1];
	DWORD dwNameLength = sizeof(userNameBuffer) / sizeof(wchar_t);
	if (!GetUserName(userNameBuffer, &dwNameLength)) {
		// Trace error
		TRACE_FORMAT("Error: Get user name failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Return the user name
	userName.setString(userNameBuffer);
	return true;
}


/**
 * @brief	Print list of characters of given string
 * @param	srcStr	  - Given string
 * @param	outputStr - Result string
 * @return	int - Number of characters
 */
int StringUtils::printCharList(const wchar_t* srcStr, String& outputStr)
{
	// Invalid source string
	if (!srcStr)
		return INT_INVALID;

	// Get source string
	String _srcStr(srcStr);

	// Prepare output string
	outputStr.empty();
	outputStr.append(_T("{ "));

	// Print character list
	String replaceStr = Constant::String::Empty;
	int nSrcLength = wcslen(srcStr);
	for (int nIndex = 0; nIndex < nSrcLength; nIndex++) {
		wchar_t ch = _srcStr.at(nIndex);
		switch (ch)
		{
		case Constant::Char::Tab:
			replaceStr = _T("#TAB");
			outputStr.append(replaceStr);
			break;
		case Constant::Char::Return:
			replaceStr = _T("#RET");
			outputStr.append(replaceStr);
			break;
		case Constant::Char::EndLine:
			replaceStr = _T("#ENDL");
			outputStr.append(replaceStr);
			break;
		default:
			outputStr.appendChar(ch);
			break;
		}

		// Add separator
		if (nIndex < nSrcLength - 1) {
			outputStr.append(_T(", "));
		}
	}

	// End result
	outputStr.append(_T(" }"));

	return nSrcLength;
}


/**
 * @brief	Get current local clock-time (including milliseconds)
 * @param	None
 * @return	ClockTime - Return clock-time data
 */
ClockTime ClockTimeUtils::getCurrentClockTime(void)
{
	SYSTEMTIME _tempSysTime{};
	::GetLocalTime(&_tempSysTime);

	return fromSystemTime(_tempSysTime);
}


/**
 * @brief	Convert Windows-based SYSTEMTIME to ClockTime data
 * @param	sysTime - Windows-based SYSTEMTIME data
 * @return	ClockTime - Return clock-time data
 */
ClockTime ClockTimeUtils::fromSystemTime(SYSTEMTIME sysTime)
{
	int hour = static_cast<int>(sysTime.wHour);
	int minute = static_cast<int>(sysTime.wMinute);
	int second = static_cast<int>(sysTime.wSecond);
	int millisecs = static_cast<int>(sysTime.wMilliseconds);

	return ClockTime(hour, minute, second, millisecs);
}


/**
 * @brief	Convert ClockTime data to Windows-based SYSTEMTIME data
 * @param	clockTime - Clock-time data
 * @return	SYSTEMTIME - Return Windows-based SYSTEMTIME data
 */
SYSTEMTIME ClockTimeUtils::toSystemTime(const ClockTime& clockTime)
{
	SYSTEMTIME _sysTime{};
	_sysTime.wHour = static_cast<unsigned short>(clockTime.hour());
	_sysTime.wMinute = static_cast<unsigned short>(clockTime.minute());
	_sysTime.wSecond = static_cast<unsigned short>(clockTime.second());
	_sysTime.wMilliseconds = static_cast<unsigned short>(clockTime.millisecond());

	return _sysTime;
}


/**
 * @brief	Convert editbox input text into valid time value
 * @param	stTime		- Return time data (ref-value)
 * @param	inputText	- Input text
 * @return	bool - Result of converting process
 */
bool ClockTimeUtils::inputText2Time(ClockTime& clockTime, const wchar_t* inputText)
{
	// Check input text validity
	int length = wcslen(inputText);
	if ((length == 0) || (length > 4))
		return false;

	String timeString(inputText);

	int hour = INT_INVALID;
	int minute = INT_INVALID;

	// Break the time value into combinations of digits
	int left1Digit = _tstoi(timeString.left(1));
	int left2Digits = _tstoi(timeString.left(2));
	int	right1Digit = _tstoi(timeString.right(1));
	int right2Digits = _tstoi(timeString.right(2));

	// Convert
	switch (length)
	{
	case 1:
		// Ex: 3 -> 03:00, 9 -> 09:00, ...
		hour = _tstoi(timeString);			// The given time value will be the hour value
		minute = 0;							// The minute value will be zero (0)
		break;

	case 2:
		if ((left1Digit == 0) ||													// Ex: 08 -> 00:08
			((left1Digit > 2) || ((left1Digit == 2) && (right1Digit > 3)))) {		// Ex: 35 -> 03:05, 24 -> 02:04, ...
			hour = left1Digit;														// The first half will be the hour value
			minute = right1Digit;													// The remaining will be the minute value
		}
		else {
			// Ex: 13 -> 13:00, 18 -> 18:00, ...
			hour = _tstoi(timeString);				// All digits will be the hour value
			minute = 0;								// The minute value will be zero (0)
		} break;

	case 3:
		if ((left1Digit == 0) ||								// Ex: 034 -> 00:34, ...
			((left1Digit > 2) || (left2Digits >= 24)) ||		// Ex: 320 -> 03:20, 250 -> 02:50, ...
			((right2Digits > 0) && (right2Digits < 60))) {		// Ex: 225 -> 02:25, 132 -> 01:32, ...
			hour = left1Digit;									// The first digit will be the hour value
			minute = right2Digits;								// The remaining will be the minute value
		}
		else {
			// Ex: 180 -> 18:00, 1530 -> 15:30, ...
			hour = left2Digits;						// The first 2 digits will be the hour value
			minute = right1Digit;					// The remaining will be the minute value
		} break;

	case 4:
		// Ex: 1235 -> 12:35, 1840 -> 18:40, ...
		hour = left2Digits;						// The first half will be the hour value
		minute = right2Digits;					// The remaining will be the minute value
		break;
	}

	// If the minute value is larger than 60
	if (minute >= 60) {
		hour++;				// The hour value increases by 1
		minute -= 60;		// The minute value decreases by 60
	}

	// If the hour value exceeds 24, return invalid
	if (hour >= 24)
		return false;

	// Only return if both the hour and minute values are valid
	if ((hour > INT_INVALID) && (minute > INT_INVALID)) {
		clockTime.setHour(hour);
		clockTime.setMinute(minute);
	}

	return true;
}


/**
 * @brief	Convert editbox input text into valid time value
 * @param	stTime		- Return time data (ref-value)
 * @param	inputText	- Input text
 * @return	bool - Result of converting process
 * @note	Old/base function (no longer used)
 */
bool ClockTimeUtils::inputText2TimeBase(ClockTime& clockTime, const wchar_t* inputText)
{
	// Check input text validity
	int length = wcslen(inputText);
	if ((length == 0) || (length > 4))
		return false;

	// Get input text length
	unsigned time = _tstoi(inputText);
	unsigned timeTemp = time;
	length = 0;
	do {
		length++;
		timeTemp /= 10;
	} while (timeTemp != 0);

	// Convert
	int hour = -1, minute = -1;
	switch (length)
	{
	case 1: 	// Ex: 3, 9, ...
		hour = time;
		minute = 0;
		break;

	case 2:
		if ((time / 10 > 2) ||
			((time / 10 == 2) && (time % 10 > 3))) {	// Ex: 35, 24, ...
			hour = time / 10;
			minute = time % 10;
		}
		else {	// Ex: 13, 18, ...
			hour = time;
			minute = 0;
		} break;

	case 3:
		if ((time / 100 > 2) || (time / 10 >= 24)) {	// Ex: 320, 240
			hour = time / 100;
			minute = time % 100;
		}
		else {	// Ex: 320, 245
			hour = time / 10;
			minute = time % 10;
		} break;

	case 4:
		hour = time / 100;
		minute = time % 100;
		break;
	}

	// Validate
	if ((hour >= 24) || (minute >= 60))
		return false;

	if ((hour > -1) && (minute > -1)) {
		clockTime.setHour(hour);
		clockTime.setMinute(minute);
	}

	return true;
}


/**
 * @brief	Convert timespin position to time value
 * @param	stTime  - Return time data (ref-value)
 * @param	nPos	- Input spin position
 * @return	None
 */
void ClockTimeUtils::spinPos2Time(ClockTime& clockTime, int pos)
{
	// Invalid input position
	if (pos < Constant::Min::TimeSpin)
		pos = Constant::Min::TimeSpin;
	else if (pos > Constant::Max::TimeSpin)
		pos = Constant::Max::TimeSpin;

	// Convert
	int hour = pos / 60;
	int minute = pos - (hour * 60);

	// Validate
	if ((hour != INT_INVALID) && (minute != INT_INVALID)) {
		clockTime.setHour(hour);
		clockTime.setMinute(minute);
	}
}


/**
 * @brief	Convert time value to timespin position
 * @param	stTime  - Return time data
 * @param	nPos	- Input spin position (ref-value)
 * @return	None
 */
void ClockTimeUtils::time2SpinPos(const ClockTime& clockTime, int& pos)
{
	// Convert
	pos = (clockTime.hour() * 60) + clockTime.minute();

	// Invalid result
	if (pos < Constant::Min::TimeSpin)
		pos = Constant::Min::TimeSpin;
	else if (pos > Constant::Max::TimeSpin)
		pos = Constant::Max::TimeSpin;
}


/**
 * @brief	Calculate time with given offset in seconds
 * @param	clockTime - Clock-time data (in/out)
 * @param	offInSecs - Offset value (in seconds)
 * @return	None
 */
void ClockTimeUtils::calculateOffset(ClockTime& clockTime, int offInSecs)
{
	if (offInSecs < 0)
		clockTime.decreaseSeconds(abs(offInSecs));
	else
		clockTime.increaseSeconds(offInSecs);
}


/**
 * @brief	Clock-time comparison with allowable offset in seconds
 * @param	thisTime  - This clock-time data
 * @param	otherTime - The other clock-time data to compare
 * @param	offInSecs - Offset value (in seconds, 0 by default)
 * @return	true/false - Clock-time values are matching or
 * @return	different within the allowable offset
 */
bool ClockTimeUtils::isMatching(ClockTime thisTime, ClockTime otherTime, int offInSecs /* = 0 */)
{
	TimeSpan _diff = thisTime - otherTime;
	int _diffInSecs = static_cast<int>(_diff.totalSeconds());
	if (offInSecs == 0)
		return (_diffInSecs == 0);
	else if (offInSecs > 0)
		return (_diffInSecs >= 0 && _diffInSecs <= offInSecs);
	else /* if (offInSecs < 0) */		return (_diffInSecs >= offInSecs && _diffInSecs <= 0);
}


/**
 * @brief	Format clock-time value for displaying or printing
 * @param	pLang	  - Language table pointer
 * @param	nFormatID - Format string ID
 * @param	clockTime - Given clock-time data
 * @return	String - Format clock-time string
 */
String ClockTimeUtils::format(LANGTABLE_PTR lang, unsigned formatID, const ClockTime& clockTime)
{
	// Load format string
	String formatString = StringUtils::loadResourceString(formatID);
	return format(lang, formatString, clockTime);
}


/**
 * @brief	Format clock-time value for displaying or printing
 * @param	pLang		 - Language table pointer
 * @param	formatString - Format string
 * @param	clockTime	 - Given clock-time data
 * @return	String - Format clock-time string
 */
String ClockTimeUtils::format(LANGTABLE_PTR lang, const wchar_t* formatString, const ClockTime& clockTime)
{
	// Format time string
	unsigned timePeriod = (clockTime.hour() < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	const wchar_t* timePeriodFormat = Language::GetLanguageString(lang, timePeriod);
	int hourVal = (clockTime.hour() > 12) ? (clockTime.hour() - 12) : clockTime.hour();
	int minuteVal = clockTime.minute();

	return StringUtils::stringFormat(formatString, hourVal, minuteVal, timePeriodFormat);
}


/**
 * @brief	Get current local date/time (including milliseconds)
 * @param	None
 * @return	DateTime - Return date/time data
 */
DateTime DateTimeUtils::getCurrentDateTime(void)
{
	SYSTEMTIME _tempSysTime{};
	::GetLocalTime(&_tempSysTime);

	return fromSystemTime(_tempSysTime);
}


/**
 * @brief	Convert Windows-based SYSTEMTIME to DateTime data
 * @param	sysTime - Windows-based SYSTEMTIME data
 * @return	DateTime - Return date/time data
 */
DateTime DateTimeUtils::fromSystemTime(SYSTEMTIME sysTime)
{
	int year = static_cast<int>(sysTime.wYear);
	unsigned int month = static_cast<unsigned int>(sysTime.wMonth);
	unsigned int day = static_cast<unsigned int>(sysTime.wDay);
	int hour = static_cast<int>(sysTime.wHour);
	int minute = static_cast<int>(sysTime.wMinute);
	int second = static_cast<int>(sysTime.wSecond);
	int millisecs = static_cast<int>(sysTime.wMilliseconds);

	return DateTime(year, month, day, hour, minute, second, millisecs);
}


/**
 * @brief	Convert DateTime data to Windows-based SYSTEMTIME data
 * @param	dateTime - Date/time data
 * @return	SYSTEMTIME - Return Windows-based SYSTEMTIME data
 */
SYSTEMTIME DateTimeUtils::toSystemTime(const DateTime& dateTime)
{
	SYSTEMTIME _sysTime{};
	_sysTime.wYear = static_cast<unsigned short>(dateTime.year());
	_sysTime.wMonth = static_cast<unsigned short>(dateTime.month());
	_sysTime.wDay = static_cast<unsigned short>(dateTime.day());
	_sysTime.wDayOfWeek = static_cast<unsigned short>(dateTime.dayOfWeek());
	_sysTime.wHour = static_cast<unsigned short>(dateTime.hour());
	_sysTime.wMinute = static_cast<unsigned short>(dateTime.minute());
	_sysTime.wSecond = static_cast<unsigned short>(dateTime.second());
	_sysTime.wMilliseconds = static_cast<unsigned short>(dateTime.millisecond());

	return _sysTime;
}


/**
 * @brief	Format date/time value for displaying or printing
 * @param	pLang	  - Language table pointer
 * @param	nFormatID - Format string ID
 * @param	dateTime  - Given date/time data
 * @return	String - Format date/time string
 */
String DateTimeUtils::format(LANGTABLE_PTR lang, unsigned formatID, const DateTime& dateTime)
{
	// Load format string
	String formatString = StringUtils::loadResourceString(formatID);
	return format(lang, formatString, dateTime);
}


/**
 * @brief	Format date/time value for displaying or printing
 * @param	pLang		 - Language table pointer
 * @param	formatString - Format string
 * @param	dateTime	 - Given date/time data
 * @return	String - Format time string
 */
String DateTimeUtils::format(LANGTABLE_PTR lang, const wchar_t* formatString, const DateTime& dateTime)
{
	// Format time string
	unsigned timePeriod = (dateTime.hour() < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	const wchar_t* timePeriodFormat = Language::GetLanguageString(lang, timePeriod);
	int hourVal = (dateTime.hour() > 12) ? (dateTime.hour() - 12) : dateTime.hour();
	int minuteVal = dateTime.minute();

	return StringUtils::stringFormat(formatString, hourVal, minuteVal, timePeriodFormat);
}


/**
 * @brief	Constructor
 */
PerformanceCounter::PerformanceCounter()
{
	// Initialization
	this->isRunning_ = false;
	QueryPerformanceFrequency(&frequency_);
}


/**
 * @brief	Destructor
 */
PerformanceCounter::~PerformanceCounter()
{
	// Stop counting
		this->stop();
}


/**
 * @brief	Start performance counter
 * @param	None
 * @return	None
 */
void PerformanceCounter::start(void)
{
	// Start performance counter
	if (!isRunning_) {
		QueryPerformanceCounter(&startTime_);
		this->isRunning_ = true;
	}
}


/**
 * @brief	Stop performance counter
 * @param	None
 * @return	None
 */
void PerformanceCounter::stop(void)
{
	// Stop performance counter
	if (isRunning_) {
		QueryPerformanceCounter(&endTime_);
		this->isRunning_ = false;
	}
}


/**
 * @brief	Get function execution elapsed time
 * @param	None
 * @return	None
 */
double PerformanceCounter::getElapsedTime(bool toMillisecs) const noexcept
{
	// Get elapsed time
	double retCounter = static_cast<double>(endTime_.QuadPart - startTime_.QuadPart) / frequency_.QuadPart;
	if (toMillisecs == true) {
		retCounter *= 1000;
	}
	return retCounter;
}


//////////////////////////////////////////////////////////////////////////
//
//	Define core methods for program
//
//////////////////////////////////////////////////////////////////////////


/**
 * @brief	Get the title name of current language
 * @param	nCurLanguage	- Current language ID
 * @param	bGetDescription - Get language package description
 * @return	const wchar_t* - Language name
 */
const wchar_t* Language::GetLanguageName(unsigned currentLanguage, bool getDescription /* = false */)
{
	// Load language table package
	LANGTABLE_PTR langTable = LoadLanguageTable(currentLanguage);
	if (langTable == NULL)
		return Constant::Value::Unknown;

	// Get language package info
	unsigned infoTargetID = (getDescription) ? LANGPACKINFO_DESCRIPTIONFULL : LANGPACKINFO_LANGNAMEID;
	const wchar_t* infoString = GetLanguageString(langTable, infoTargetID);
	if (IS_NULL_STRING(infoString))
		infoString = Constant::Value::Unknown;

	return infoString;
}


/**
 * @brief	Load language table by specified language option
 * @param	nCurLanguage   - Current language ID
 * @return	LANGTABLE_PTR - Language package pointer
 */
LANGTABLE_PTR Language::LoadLanguageTable(unsigned currentLanguage)
{
	LANGTABLE_PTR langTable = NULL;

	switch (currentLanguage)
	{
	case APP_LANGUAGE_ENGLISH:
		// Language: English (United States)
		langTable = &langtable_en_US;
		break;

	case APP_LANGUAGE_VIETNAMESE:
		// Language: Vietnamese (Vietnam)
		langTable = &langtable_vi_VN;
		break;

	case APP_LANGUAGE_SIMPCHINESE:
		// Language: Simplified Chinese (China mainland)
		langTable = &langtable_zh_CH;
		break;

	default:
		// Default language: English
		langTable = &langtable_en_US;
		break;
	}

	return langTable;
}


/**
 * @brief	Find and return language string by ID
 * @param	languageTablePtr - Language package pointer
 * @param	id				 - Language string ID
 * @param	pszResult		 - Result string (reference-type)
 * @return	const wchar_t*	 - Language string
 */
const wchar_t* Language::GetLanguageString(LANGTABLE_PTR languageTablePtr, unsigned id)
{
	// Return NULL string if language table is empty
	if ((languageTablePtr == NULL) || (languageTablePtr->empty()))
		return Constant::String::Null;

	// Find and return corresponding language string paired with specified ID
	for (int index = 0; index < languageTablePtr->size(); index++) {
		LANGTEXT langText = languageTablePtr->at(index);

		if (langText.id == id)
			return langText.langString;
	}

	return Constant::String::Null;
}


/**
 * @brief	Main power action function
 * @param	nActionType - Type of action
 * @param	nMessage	- Action message
 * @param	dwErrorCode - Return error code (ref-value)
 * @return	bool - Result of action execution
 */
bool AppCore::executePowerAction(unsigned actionType, unsigned message, DWORD& errorCode)
{
	bool ret = true;

	// Execute Power Actions in here
	switch (actionType)
	{
		// Monitor typed action
		case APP_ACTIONTYPE_MONITOR:
			// Turn off display
			PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, (WPARAM)message, (LPARAM)2);
			errorCode = APP_ERROR_SUCCESS;
			break;

		// Power typed actions
		case APP_ACTIONTYPE_POWER:
		{
			switch (message)
			{
				case APP_MESSAGE_SHUTDOWN:
				case APP_MESSAGE_REBOOT:
				case APP_MESSAGE_SIGNOUT:
				{
					// Force action
					unsigned exitWinExFlags = message;
					exitWinExFlags |= EWX_FORCE;

					HANDLE token;
					TOKEN_PRIVILEGES privileges{};

					// Get process token
					if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
						// Get error code
						errorCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, OpenProcessToken failed!!! (Code: 0x%08X)", errorCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}

					// Lookup privilege value
					if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid)) {
						// Get error code
						errorCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, LookupPrivilegeValue failed!!! (Code: 0x%08X)", errorCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}

					// Adjust token privileges
					privileges.PrivilegeCount = 1;
					privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
					if (!AdjustTokenPrivileges(token, false, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0)) {
						// Adjust token privileges failed
						errorCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, AdjustTokenPrivileges failed!!! (Code: 0x%08X)", errorCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}

					// Exit Windows
					if (!ExitWindowsEx(exitWinExFlags, 0)) {
						// Get exit Windows error
						errorCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, ExitWindowsEx failed!!! (Code: 0x%08X)", errorCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}

				} break;

				case APP_MESSAGE_SLEEP:
					// Sleep mode
					if (!SetSuspendState(false, false, false)) {		// Stand by (sleep)
						errorCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, SetSuspendState failed!!! (Code: 0x%08X)", errorCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}
					break;

				case APP_MESSAGE_HIBERNATE:
					// Hibernate mode
					if (!SetSuspendState(true, false, false)) {			// Hibernate
						errorCode = GetLastError();
						TRACE_FORMAT("Error: Cannot execute action, SetSuspendState failed!!! (Code: 0x%08X)", errorCode);
						TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
						return false;
					}
					break;
			}
		} break;

	default:
		// Wrong argument
		errorCode = APP_ERROR_WRONG_ARGUMENT;
		TRACE_FORMAT("Error: Cannot execute action, wrong argument!!! (Code: 0x%08X)", errorCode);
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		ret = false;
		break;
	}

	return ret;
}


/**
 * @brief	Dummy power action function (use for testing)
 * @param	nActionType - Type of action
 * @param	nMessage	- Action message
 * @param	dwErrorCode - Return error code (ref-value)
 * @return	bool - Result of action execution
 */
bool AppCore::executePowerActionDummy(unsigned actionType, unsigned message, DWORD& errorCode)
{
	// Get action execution time
	DateTime currentDateTime = DateTimeUtils::getCurrentDateTime();

	// Get action name
	String actionInfoString;
	if (actionType == APP_ACTIONTYPE_MONITOR) {
		actionInfoString.setString(_T("Turn off display"));
		errorCode = ERROR_SUCCESS;
	}
	else if (actionType == APP_ACTIONTYPE_POWER && 
			(message != APP_MESSAGE_SLEEP && message != APP_MESSAGE_HIBERNATE)) {
		switch (message)
		{
		case APP_MESSAGE_SHUTDOWN:
			// Shutdown
			actionInfoString.setString(_T("Shutdown"));
			errorCode = ERROR_SUCCESS;
			break;

		case APP_MESSAGE_REBOOT:
			// Restart
			actionInfoString.setString(_T("Restart"));
			errorCode = ERROR_SUCCESS;
			break;

		case APP_MESSAGE_SIGNOUT:
			// Sign out
			actionInfoString.setString(_T("Sign out"));
			errorCode = ERROR_SUCCESS;
			break;
		}
	}
	else if (actionType == APP_ACTIONTYPE_POWER && 
			(message == APP_MESSAGE_SLEEP || message == APP_MESSAGE_HIBERNATE)) {
		switch (message)
		{
		case APP_MESSAGE_SLEEP:
			// Sleep
			actionInfoString.setString(_T("Sleep"));
			errorCode = ERROR_SUCCESS;
			break;

		case APP_MESSAGE_HIBERNATE:
			// Hibernate
			actionInfoString.setString(_T("Hibernate"));
			errorCode = ERROR_SUCCESS;
			break;
		}
	}
	else {
		// Wrong argument
		actionInfoString.setString(_T("Invalid"));
		errorCode = APP_ERROR_WRONG_ARGUMENT;
	}

	// Time format
	String timeFormatStr;
	const wchar_t* timePeriod = (currentDateTime.hour() < 12) ? Constant::Symbol::AnteMeridiem : Constant::Symbol::PostMeridiem;
	String templateFormatStr = StringUtils::loadResourceString(IDS_FORMAT_FULLDATETIME);
	timeFormatStr.format(templateFormatStr, currentDateTime.year(), currentDateTime.month(), currentDateTime.day(),
						currentDateTime.hour(), currentDateTime.minute(), currentDateTime.second(), currentDateTime.millisecond(), timePeriod);

	// Message format
	String messageFormatStr;
	messageFormatStr.format(_T("[ExecutePowerAction]\nAction: %s\nTime: %s"), actionInfoString.getString(), timeFormatStr.getString());

	// Show dummy test message
	HWND hWnd = GET_HANDLE_MAINWND();
	MessageBox(hWnd, messageFormatStr, _T("DummyTest"), MB_OK | MB_ICONINFORMATION);
	return true;
}


/**
 * @brief	Create a loop and wait for specified message
 * @param	message	 - Message to wait for
 * @param	nTimeout - Timeout (tick-count)
 * @return	LRESULT
 * @note	Be careful when using this function, it may cause the program to be not responding
 */
LRESULT	AppCore::waitMessage(unsigned message, int timeout /* = DEF_WAITMESSAGE_TIMEOUT */)
{
	LRESULT result = Result::Success;

	// Get begin timestamp (for timeout counter)
	ULONGLONG beginTimestamp = GetTickCount64();

	// Wait for message
	while (1) {
		MSG msg = {0};
		if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {

			// If received specified message, break the loop
			if (msg.message == message) {

				// Success
				result = Result::Success;
				break;
			}
		}
		// Check for timeout
		if (GetTickCount64() - beginTimestamp >= timeout) {

			// Timeout --> Failed
			result = Result::Failure;
			break;
		}
	}

	return result;
}


/**
 * @brief	Show error message by error code
 * @param	hMsgOwnerWnd - Handle of Message Box's owner window
 * @param	nLanguageID	 - Language option ID
 * @param	dwErrorCode	 - Error code
 * @param	lParam		 - Additional attached param (description string)
 * @return	None
 */
void AppCore::showErrorMessage(HWND msgOwnerWnd, unsigned languageId, DWORD errorCode, LPARAM lParam /* = NULL */)
{
	// Use table and language functions
	using namespace MapTable;
	using namespace Language;

	// Get application-defined error code from system-defined error code
	DWORD appErrCode = GetPairedID(IDTable::ErrorCode, errorCode, true);
	if (appErrCode != INT_INVALID) {
		// Replace with application-defined error code
		errorCode = appErrCode;
	}

	// Get error message string ID
	int errMsgID = GetPairedID(IDTable::ErrorMessage, errorCode);

	// Invalid error message ID
	if (errMsgID == INT_INVALID) {
		// Show unknown error message
		errMsgID = MSGBOX_ERROR_UNKNOWN;
	}

	// If error message ID is NULL, do nothing
	if (errMsgID == INT_NULL)
		return;

	// Load language package
	LANGTABLE_PTR appLang = LoadLanguageTable(languageId);
	if (appLang == NULL) 
		return;

	// Get language strings
	String errorMessage = GetLanguageString(appLang, errMsgID);
	const wchar_t* errorCaption = GetLanguageString(appLang, MSGBOX_ERROR_CAPTION);

	// In case of unknown error, attach the error code
	if (errMsgID == MSGBOX_ERROR_UNKNOWN) {
		String tempStr;
		tempStr.format(errorMessage, errorCode);
		errorMessage = tempStr;
	}

	// Get attached param
	String descriptionStr = Constant::String::Null;
	if (lParam != NULL) {
		// Convert to description string
		descriptionStr = LPARAM_TO_STRING(lParam);
	}

	// Attach additional description if available
	if (IS_NOT_NULL_STRING(descriptionStr)) {
		errorMessage.append(Constant::String::NewLine);
		errorMessage.append(descriptionStr);
	}

	// Show error message
	MessageBox(msgOwnerWnd, errorMessage, errorCaption, MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);

	// Notify application class about error message showing
	WPARAM appMsgParam = (WPARAM)errorCode;
	LPARAM appMsgParam2 = MAKE_LPARAM_STRING(errorMessage);
	PostMessage(NULL, SM_APP_SHOW_ERROR_MSG, appMsgParam, appMsgParam2);
}


/**
 * @brief	Find and return DebugTest dialog handle
 * @param	None
 * @return	HWND
 */
HWND AppCore::findDebugTestDlg()
{
	String debugDlgTitle = StringUtils::loadResourceString(IDS_APP_DEBUGTESTDLG_TITLE);
	if (debugDlgTitle.isEmpty()) return NULL;
	return ::FindWindow(NULL, debugDlgTitle);
}


/**
 * @brief	Set fixed cell style (header row, base column)
 * @param	pGridCtrl	- Grid control table pointer
 * @param	nRow & nCol - Cell position (row & column)
 * @return	None
 */
void AppCore::setFixedCellStyle(CGridCtrl* gridCtrlPtr, int row, int col)
{
	// Check control validity
	if (gridCtrlPtr == NULL) return;

	// Set base style
	CGridCellBase* headerCell = (CGridCellBase*)gridCtrlPtr->GetCell(row, col);
	if (headerCell == NULL) return;
	headerCell->SetFormat(headerCell->GetFormat() | DT_CENTER);
	headerCell->SetMargin(0);
	headerCell->SetBackClr(Color::Gray);
	headerCell->SetTextClr(Color::Black);
}


/**
 * @brief	Set dark mode for dialog with specified handle
 * @param	pWnd			- Pointer of window
 * @param	bEnableDarkMode - Enable/disable dark mode
 * @return	bool - Result of dark mode setting process
 */
bool AppCore::setDarkMode(CWnd* wndPtr, bool enableDarkMode)
{
	// Load theme library
	HMODULE uxTheme = LoadLibraryEx(_T("uxtheme.dll"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (uxTheme == NULL) {
		// Trace error
		TRACE_FORMAT("Error: Load UXTheme library failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get function pointer
	using fnAllowDarkMode = bool (WINAPI*)(HWND hWND, bool bAllow);
	static const fnAllowDarkMode AllowDarkModeForWindow = (fnAllowDarkMode)GetProcAddress(uxTheme, MAKEINTRESOURCEA(133));
	if (AllowDarkModeForWindow == NULL) {
		// Trace error
		TRACE_FORMAT("Error: Get AllowDarkModeForWindow function address failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Set dark mode for each dialog item
	CWnd* wndChild = NULL;
	for (wndChild = wndPtr->GetTopWindow(); wndChild != NULL; wndChild = wndChild->GetWindow(GW_HWNDNEXT))
	{
		HWND childWnd = wndChild->GetSafeHwnd();
		AllowDarkModeForWindow(childWnd, enableDarkMode);
		SetWindowTheme(childWnd, _T("DarkMode_Explorer"), NULL);
		SendMessage(childWnd, WM_THEMECHANGED, 0, 0);
	}

	return true;
}


/**
 * @brief	Create button with icon
 * @param	pBtn		 - Pointer of button item
 * @param	nIconID		 - ID of button icon
 * @param	lpszBtnTitle - Title of button
 * @return	None
 */
void AppCore::drawButton(CButton*& buttonPtr, unsigned iconId, const wchar_t* buttonTitle /* = Constant::String::Empty */)
{
	// Check validity
	if (buttonPtr == NULL)
		return;

	// Load icon
	HICON btnIcon = AfxGetApp()->LoadIcon(iconId);
	if (btnIcon == NULL)
		return;

	// Button rect
	CRect btnRect;
	buttonPtr->GetWindowRect(&btnRect);

	// Button title
	String buttonTitleString;
	buttonTitleString.setString(buttonTitle);
	if (!buttonTitleString.isEmpty()) {
		wchar_t _tempBuffer[Constant::Max::StringLength] = { 0 };
		buttonPtr->GetWindowText(_tempBuffer, Constant::Max::StringLength);
		buttonTitleString.setString(_tempBuffer);
	}

	// Update button
	buttonPtr->SetButtonStyle(BS_ICON);
	buttonPtr->SetIcon(btnIcon);
	buttonPtr->UpdateWindow();
}


/////////////////////////////////////////////////////////////////////////////
// Additional functions


/**
 * @brief	Get Windows OS build version
 * @param	None
 * @return	unsigned - Windows version macro
 */
unsigned AppCore::getWindowsOSVersion(void)
{
	// Init info data
	OSVERSIONINFOEX osVersion{};
	osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	// Get function address
	NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
	if (RtlGetVersion == NULL) {
		// Trace error
		TRACE_FORMAT("Error: Get RtlGetVersion function address failed!!! (Code: 0x%08X)", GetLastError());
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return INT_NULL;
	}

	// Get Window OS version
	RtlGetVersion(&osVersion);

	// Return Windows OS version macro
	unsigned retWinVer = WINDOWS_VERSION_NONE;
	if (osVersion.dwBuildNumber >= OS_BUILDNUMBER_W11_EARLIEST) {
		// Is Windows 11
		retWinVer = WINDOWS_VERSION_11;
	}
	else if (osVersion.dwBuildNumber >= OS_BUILDNUMBER_W10_ORIGINAL) {
		// Is Windows 10
		retWinVer = WINDOWS_VERSION_10;
	}
	else if ((osVersion.dwBuildNumber >= OS_BUILDNUMBER_W8_EARLIEST) &&
		(osVersion.dwBuildNumber <= OS_BUILDNUMBER_W8_LATEST)) {
		// Is Windows 8/8.1
		retWinVer = WINDOWS_VERSION_8;
	}
	else if ((osVersion.dwBuildNumber >= OS_BUILDNUMBER_W7_EARLIEST) &&
		(osVersion.dwBuildNumber <= OS_BUILDNUMBER_W7_LATEST)) {
		// Is Windows 7
		retWinVer = WINDOWS_VERSION_7;
	}
	else {
		// Unknown version
		retWinVer = WINDOWS_VERSION_UNKNOWN;
	}

	return retWinVer;
}


/**
 * @brief	Play "BEEP" sound when sound is enabled
 * @param	bSoundEnable - Enable sound
 * @param	nTypeOfSound - Type of sound
 * @return	None
 */
void AppCore::playSound(bool isSoundEnabled, unsigned typeOfSound)
{
	// If sound is not enabled, do nothing
	if (!isSoundEnabled)
		return;

	// Play sound by type here
	switch (typeOfSound)
	{
	case APP_SOUND_ERROR:
		::PlaySound(_T("SystemExclamination"), NULL, SND_ASYNC);
		break;
	case APP_SOUND_SUCCESS:
		::PlaySound(_T("SystemExit"), NULL, SND_ASYNC);
		break;
	}
}


/**
 * @brief	Open a file to view using external standard fileviewer
 * @param	eFileType - File type
 * @param	filePath  - Path of file
 * @return	bool - Result of file opening process
 */
bool AppCore::fileViewStd(FILETYPE fileType, const wchar_t* filePath)
{
	String appPath = Constant::String::Empty;

	switch (fileType) 
	{
	case FILETYPE_TEXT:
		appPath = Constant::Path::Notepad;
		break;
	case FILETYPE_IMAGE:
		break;
	default:
		return false;
	}

	// Run a file viewer instance
	HWND wnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	HINSTANCE instance = ShellExecute(wnd, Constant::Command::Open, appPath, filePath, NULL, SW_SHOW);
	return (instance != NULL);
}


/**
 * @brief	Open web URL using default web browser
 * @param	webUrl - String of web URL
 * @return	bool - Result of web URL opening process
 */
bool AppCore::openWebURL(const wchar_t* webUrl)
{
	// Run a web browser instance
	HINSTANCE hInstance = ShellExecute(0, 0, webUrl, NULL, NULL, SW_NORMAL);
	return (hInstance != NULL);
}


/**
 * @brief	Run an application by specified path
 * @param	appPath		- Path of excutive file
 * @param	bRunAsAdmin	- Run as admin flag
 * @param	bShowFlag	- Show window flag
 * @return	LRESULT - Result of app launching process
 */
LRESULT AppCore::runApp(const wchar_t* appPath, bool runAsAdmin /* = false */, bool showFlag /* = true */)
{
	// Param set
	String runAsFlag = (runAsAdmin) ? Constant::Command::RunAs : Constant::Command::Open;
	int showFlagValue = (showFlag) ? SW_SHOW : SW_HIDE;

	// Run an executable instance
	HINSTANCE instance = ShellExecute(NULL, runAsFlag, appPath, 0, 0, showFlagValue);
	return (LRESULT)(instance != NULL);
}


/**
 * @brief	Execute CMD command
 * @param	commandString - Command string
 * @param	bRunAsAdmin	  - Run as admin flag
 * @param	bShowFlag	  - Show window flag
 * @return	LRESULT - Result of command execution process
 */
LRESULT AppCore::executeCommand(const wchar_t* commandString, bool runAsAdmin /* = true */, bool showFlag /* = true */)
{
	// Format input command
	String commandFormat = StringUtils::stringFormat(_T("/C %s"), commandString);

	// Flag param set
	String runAsFlag = (runAsAdmin) ? Constant::Command::RunAs : Constant::Command::Open;
	int showFlagValue = (showFlag) ? SW_SHOW : SW_HIDE;

	// Excute command
	HINSTANCE instance = ShellExecute(NULL, runAsFlag, Constant::Path::SystemCMD, commandFormat, 0, showFlagValue);
	return (LRESULT)(instance != NULL);
}


/**
 * @brief	Create app process
 * @param	appPath		 - App executable file path
 * @param	commandLine	 - Command line
 * @param	nStyle		 - App process style
 * @param	dwErrorCode	 - Returned error code
 * @return	bool
 */
bool AppCore::createAppProcess(const wchar_t* appPath, wchar_t* commandLine, unsigned style, DWORD& errorCode)
{
	// Startup info
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);

	// Process info
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

	// Create process
	bool result = CreateProcess(appPath, commandLine, (LPSECURITY_ATTRIBUTES)NULL,
		(LPSECURITY_ATTRIBUTES)NULL, false, (DWORD)style, NULL, NULL, &startupInfo, &processInfo);

	if (result == false) {
		// Get error code
		errorCode = GetLastError();
		TRACE_FORMAT("Error: Create app process failed!!! (Code: 0x%08X)", errorCode);
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return result;
	}

	// Wait for process exitting
	WaitForSingleObject(processInfo.hProcess, INFINITE);

	// Check whether our command succeeded?
	GetExitCodeProcess(processInfo.hProcess, &errorCode);

	// Avoid memory leak by closing process handle
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	return result;
}


/**
 * @brief	Callback function used with the EnumFontFamiliesEx
 * @param	Default (see MSDN "EnumFontFamProc callback function")
 * @return	true/false
 */
static bool CALLBACK EnumFontFamiliesExProc(ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* /*lpntme*/, DWORD /*FontType*/, LPARAM lParam) 
{
	using wstring_vector = typename std::vector<std::wstring>;
	wstring_vector* fontNames = reinterpret_cast<wstring_vector*>(lParam);
	fontNames->push_back(lpelfe->elfLogFont.lfFaceName);
	return true;
}


/**
 * @brief	Enumerate all currently available fonts
 * @param	fontNames - Array to contain enumerated fonts
 * @return	true/false
 */
bool AppCore::enumFontNames(std::vector<std::wstring>& fontNameList)
{
	// Define temp font
	LOGFONT logFont = {0};
	logFont.lfCharSet = DEFAULT_CHARSET;

	// Get font families
	HDC dc = GetDC(NULL);
	EnumFontFamiliesEx(dc, &logFont, (FONTENUMPROC)EnumFontFamiliesExProc, (LPARAM)&fontNameList, 0);
	ReleaseDC(NULL, dc);

	// Remove duplicated font names
	std::sort(fontNameList.begin(), fontNameList.end());
	fontNameList.erase(std::unique(fontNameList.begin(), fontNameList.end()), fontNameList.end());

	return (!fontNameList.empty());
}


/**
 * @brief	Check if an input string is a valid font name
 * @param	fontName - Input font name
 * @return	true/false
 */
bool AppCore::validateFontName(const wchar_t* fontName)
{
	// Array to get returned font names
	std::vector<std::wstring> fontNameList;

	// Enumerate all currently available fonts
	bool ret = enumFontNames(fontNameList);
	if (!ret) {
		// Trace error
		TRACE_ERROR("Error: Enumerate fonts failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Find for input font name within the acquired font families
	ret = (std::find(fontNameList.begin(), fontNameList.end(), fontName) != fontNameList.end());
	if (ret)
		return ret;

	// For easier comparison, convert all to lowercase
	std::wstring lowerInput(fontName);
	std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::towlower);
	std::vector<std::wstring> fontLowNames;
	for (auto it = fontNameList.begin(); it != fontNameList.end(); it++) {
		std::transform(it->begin(), it->end(), it->begin(), ::towlower);
		fontLowNames.push_back((*it));
	}

	// Find for input font name within the lower font name array
	ret = (std::find(fontLowNames.begin(), fontLowNames.end(), lowerInput) != fontNameList.end());

	return ret;
}

