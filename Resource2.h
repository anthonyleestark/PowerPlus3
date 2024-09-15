
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Resource2.h
//		Description:	User-definition resource IDs
//		Owner:			AnthonyLeeStark
//		
//		History:		<1> 2024.01.27:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RESOURCE2_H_INCLUDED
#define _RESOURCE2_H_INCLUDED


////////////////////////////////////////////////////////////

#define BTN_HELPDLG_VIEWMODE_HELPFILE							120001
#define BTN_HELPDLG_VIEWMODE_CHANGELOG							120002
#define BTN_HKSETDLG_ADD_MODEADD								120003
#define BTN_HKSETDLG_ADD_MODEUPDATE								120004
#define BTN_HKSETDLG_CHECKALL									120005
#define BTN_HKSETDLG_UNCHECKALL									120006

#define LVCOLUMN_EMPTY_TITLE									120010
#define LVCOLUMN_HOTKEYSET_ACTION								120011
#define LVCOLUMN_HOTKEYSET_KEYSTROKES							120012
#define GRIDCOLUMN_PWRREMINDER_INDEX							120013
#define GRIDCOLUMN_PWRREMINDER_STATE							120014
#define GRIDCOLUMN_PWRREMINDER_ITEMID							120015
#define GRIDCOLUMN_PWRREMINDER_MESSAGE							120016
#define GRIDCOLUMN_PWRREMINDER_EVENTID							120017
#define GRIDCOLUMN_PWRREMINDER_STYLE							120018
#define GRIDCOLUMN_PWRREMINDER_REPEAT							120019

#define FORMAT_TIME_BEFOREMIDDAY								120020
#define FORMAT_TIME_AFTERMIDDAY									120021

#define ACTION_NAME_DISPLAYOFF									120101
#define ACTION_NAME_SLEEP										120102
#define ACTION_NAME_SHUTDOWN									120103
#define ACTION_NAME_RESTART										120104
#define ACTION_NAME_SIGNOUT										120105
#define ACTION_NAME_HIBERNATE									120106
#define ACTION_NAME_SHOWMENU									120107

#define MESSAGE_ACTION_DISPLAYOFF								120201
#define MESSAGE_ACTION_SLEEP									120202
#define MESSAGE_ACTION_SHUTDOWN									120203
#define MESSAGE_ACTION_RESTART									120204
#define MESSAGE_ACTION_SIGNOUT									120205
#define MESSAGE_ACTION_HIBERNATE								120206
#define MESSAGE_SCHEDNOTIFY_DISPLAYOFF							120207
#define MESSAGE_SCHEDNOTIFY_SLEEP								120208
#define MESSAGE_SCHEDNOTIFY_SHUTDOWN							120209
#define MESSAGE_SCHEDNOTIFY_RESTART								120210
#define MESSAGE_SCHEDNOTIFY_SIGNOUT								120211
#define MESSAGE_SCHEDNOTIFY_HIBERNATE							120212

#define COMBOBOX_ACTION_DISPLAYOFF								120301
#define COMBOBOX_ACTION_SLEEP									120302
#define COMBOBOX_ACTION_SHUTDOWN								120303
#define COMBOBOX_ACTION_RESTART									120304
#define COMBOBOX_ACTION_SIGNOUT									120305
#define COMBOBOX_ACTION_HIBERNATE								120306
#define COMBOBOX_ACTION_SHOWMENU								120307
#define COMBOBOX_LANGUAGE_ENGLISH								120308
#define COMBOBOX_LANGUAGE_VIETNAMESE							120309
#define COMBOBOX_LANGUAGE_SIMPCHINESE							120310
#define COMBOBOX_MSGSTYLE_MESSAGEBOX							120311
#define COMBOBOX_MSGSTYLE_DIALOGBOX								120312

#define NOTIFY_TIP_TEMPLATE										120401
#define NOTIFY_TIP_DISPLAYOFF									120402
#define NOTIFY_TIP_SLEEP										120403
#define NOTIFY_TIP_SHUTDOWN										120404
#define NOTIFY_TIP_RESTART										120405
#define NOTIFY_TIP_SIGNOUT										120406
#define NOTIFY_TIP_HIBERNATE									120407
#define NOTIFY_TIP_SHOWMENU										120408

#define BALLOON_TIP_TEMPLATE									120501
#define BALLOON_TIP_DISPLAYOFF									120502
#define BALLOON_TIP_SLEEP										120503
#define BALLOON_TIP_SHUTDOWN									120504
#define BALLOON_TIP_RESTART										120505
#define BALLOON_TIP_SIGNOUT										120506
#define BALLOON_TIP_HIBERNATE									120507

#define ERROR_HELPDLG_NOHELPFILE								120601
#define ERROR_HELPDLG_NOCHANGELOGFILE							120602

#define PWRRMD_EVENT_AT_SETTIME									120701
#define PWRRMD_EVENT_AT_APPSTARTUP								120702
#define PWRRMD_EVENT_AT_SYSWAKEUP								120703
#define PWRRMD_EVENT_AT_BFRPWRACTION							120704
#define PWRRMD_EVENT_AT_PWRACTIONWAKE							120705
#define PWRRMD_EVENT_AT_APPEXIT									120706
#define PWRRMD_STYLE_MESSAGEBOX									120710
#define PWRRMD_STYLE_DIALOG										120711

#define MSGBOX_CONFIG_RELOAD_CAPTION							140701
#define MSGBOX_CONFIG_CHANGED_CONTENT							140702
#define MSGBOX_CONFIG_NOTCHANGED_CONTENT						140703
#define MSGBOX_SCHEDULE_CHANGED_CAPTION							140704
#define MSGBOX_SCHEDULE_CHANGED_CONTENT							140705
#define MSGBOX_SCHEDULE_NOTIFY									140706
#define MSGBOX_SCHEDULE_ALLOWCANCEL								140707
#define MSGBOX_SCHEDULE_CANCELED								140708
#define MSGBOX_HOTKEYSET_CAPTION								140709
#define MSGBOX_HOTKEYSET_REG_FAILED								140710
#define MSGBOX_HOTKEYSET_UNREG_FAILED							140711
#define MSGBOX_HOTKEYSET_CHANGED_CONTENT						140712
#define MSGBOX_HOTKEYSET_REMOVE_ITEM							140713
#define MSGBOX_HOTKEYSET_REMOVEALL_ITEMS						140714
#define MSGBOX_HOTKEYSET_INVALIDITEM_ACTIONID					140715
#define MSGBOX_HOTKEYSET_INVALIDITEM_CTRLKEY					140716
#define MSGBOX_HOTKEYSET_INVALIDITEM_FUNCKEY					140717
#define MSGBOX_HOTKEYSET_INVALIDITEM_EMPTY						140718
#define MSGBOX_HOTKEYSET_EXISTED_HOTKEY							140719
#define MSGBOX_PWRREMINDER_CAPTION								140720
#define MSGBOX_PWRREMINDER_DISPLAY_FAILED						140721
#define MSGBOX_PWRREMINDER_UPDATE_FAILED						140722
#define MSGBOX_PWRREMINDER_CHANGED_CONTENT						140723
#define MSGBOX_PWRREMINDER_REMOVE_ITEM							140724
#define MSGBOX_PWRREMINDER_REMOVEALL_ITEMS						140725
#define MSGBOX_PWRREMINDER_CONFIRM_EXITMODE						140726
#define MSGBOX_PWRREMINDER_INVALIDITEM_ITEMID					140727
#define MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_EMPTY			140728
#define MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_OUTOFLIMIT		140729
#define MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_INVALIDCHAR		140730
#define MSGBOX_PWRREMINDER_INVALIDITEM_EVENTID					140731
#define MSGBOX_PWRREMINDER_INVALIDITEM_TIMEVALUE				140732
#define MSGBOX_PWRREMINDER_INVALIDITEM_STYLEID					140733

#define MSGBOX_OTHER_NOTRUNASADMIN								140801
#define MSGBOX_OTHER_REQUEST_RESTARTASADMIN						140802
#define MSGBOX_OTHER_RUNASADMIN_NEXTTIME						140803
#define MSGBOX_OTHER_ALREADY_ADMIN								140804
#define MSGBOX_OTHER_FUNCTION_NOT_AVAILABLE						140805

#define MSGBOX_ERROR_CAPTION									150099
#define MSGBOX_ERROR_SUCCESS									150100
#define MSGBOX_ERROR_FAILED										150101
#define MSGBOX_ERROR_WRONG_ARGUMENT								150102
#define MSGBOX_ERROR_INVALID_FUNCTION							150103
#define MSGBOX_ERROR_FILE_NOT_FOUND								150104
#define MSGBOX_ERROR_PATH_NOT_FOUND								150105
#define MSGBOX_ERROR_ACCESS_DENIED								150106
#define MSGBOX_ERROR_INVALID_HANDLE								150107
#define MSGBOX_ERROR_INVALID_DATA								150108
#define MSGBOX_ERROR_NO_MORE_FILES								150109
#define MSGBOX_ERROR_FILE_EXISTS								150110
#define MSGBOX_ERROR_CANNOT_MAKE								150111
#define MSGBOX_ERROR_INVALID_PARAMETER							150112
#define MSGBOX_ERROR_OPEN_FAILED								150113
#define MSGBOX_ERROR_BUFFER_OVERFLOW							150114
#define MSGBOX_ERROR_INVALID_NAME								150115
#define MSGBOX_ERROR_DIR_NOT_EMPTY								150116
#define MSGBOX_ERROR_FAIL_SHUTDOWN								150117
#define MSGBOX_ERROR_FAIL_RESTART								150118
#define MSGBOX_ERROR_INVALID_ADDRESS							150119
#define MSGBOX_ERROR_APP_INIT_FAILURE							150120
#define MSGBOX_ERROR_CANNOT_LOAD_REGISTRY						150121
#define MSGBOX_ERROR_REGISTRY_QUOTA_LIMIT						150122
#define MSGBOX_ERROR_SYSTEM_SHUTDOWN							150123
#define MSGBOX_ERROR_HIBERNATED									150124
#define MSGBOX_ERROR_RESUME_HIBERNATION							150125
#define MSGBOX_ERROR_WAKE_SYSTEM								150126
#define MSGBOX_ERROR_BACKUP_REG_FAILED							150127
#define MSGBOX_ERROR_LOAD_CFG_FAILED							150128
#define MSGBOX_ERROR_SAVE_CFG_FAILED							150129
#define MSGBOX_ERROR_LOAD_SCHED_FAILED							150130
#define MSGBOX_ERROR_SAVE_SCHED_FAILED							150131
#define MSGBOX_ERROR_LOAD_HKEYSET_FAILED						150132
#define MSGBOX_ERROR_SAVE_HKEYSET_FAILED						150133
#define MSGBOX_ERROR_LOAD_PWRRMD_FAILED							150134
#define MSGBOX_ERROR_SAVE_PWRRMD_FAILED							150135
#define MSGBOX_ERROR_WRITE_LOG_FAILED							150136
#define MSGBOX_ERROR_OUTPUT_LOG_FAILED							150137
#define MSGBOX_ERROR_UNKNOWN									150199

#define HKEYSET_EXISTED_ALT_F4									180000
#define HKEYSET_EXISTED_ALT_F8									180001
#define HKEYSET_EXISTED_CTRL_F3									180010
#define HKEYSET_EXISTED_CTRL_F4									180011
#define HKEYSET_EXISTED_CTRL_F5									180012
#define HKEYSET_EXISTED_CTRL_WIN_F4								180013

////////////////////////////////////////////////////////////

#endif	// ifndef _RESOURCE2_H_INCLUDED