/**
 * @file		Resource.h
 * @brief		Microsoft Visual C++ generated include file
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */


//************************************* VersionInfo **************************************

#define VERSION_PRODUCT_NAME							L"Power++\0"
#define VERSION_FILE_DESCRIPTION 						L"Power++ v3.2\0"
#define VERSION_COMPANY_NAME							L"Stark Personal\0"

#define VERSION_INTERNAL_VALUE 							L"3.2\0"
#define VERSION_PRODUCT_VALUE 							L"3.2.1\0"
#define VERSION_DIGITAL_VALUE 							3, 2, 1, 0

//****************************************************************************************


//********************************* Basic resource IDs ***********************************

#define IDM_ABOUTBOX		           					100

#define IDI_ICON_APP    	            		 		102
#define IDI_ICON_NOTIFY         	       				103
#define IDI_ICON_ABOUT_LARGE							104
#define IDI_ICON_ABOUT_MID								105
#define IDI_ICON_ABOUT_SMALL							106
#define IDI_ICON_HELP_LARGE								107
#define IDI_ICON_HELP_MID								108
#define IDI_ICON_HELP_SMALL								109
#define IDI_ICON_DEFAULT_LARGE							110
#define IDI_ICON_DEFAULT_MID							111
#define IDI_ICON_DEFAULT_SMALL							112
#define IDI_ICON_OK_LARGE								113
#define IDI_ICON_OK_MID									114
#define IDI_ICON_OK_SMALL								115
#define IDI_ICON_CANCEL_LARGE							116
#define IDI_ICON_CANCEL_MID								117
#define IDI_ICON_CANCEL_SMALL							118
#define IDI_ICON_RELOAD_LARGE							119
#define IDI_ICON_RELOAD_MID								120
#define IDI_ICON_RELOAD_SMALL							121
#define IDI_ICON_ARROW_UP_LARGE							122
#define IDI_ICON_ARROW_UP_MID							123
#define IDI_ICON_ARROW_UP_SMALL							124
#define IDI_ICON_ARROW_DOWN_LARGE						125
#define IDI_ICON_ARROW_DOWN_MID							126
#define IDI_ICON_ARROW_DOWN_SMALL						127
#define IDI_ICON_ADD_LARGE								128
#define IDI_ICON_ADD_MID								129
#define IDI_ICON_ADD_SMALL								130
#define IDI_ICON_SAVE_LARGE								131
#define IDI_ICON_SAVE_MID								132
#define IDI_ICON_SAVE_SMALL								133
#define IDI_ICON_EDIT_LARGE								134
#define IDI_ICON_EDIT_MID								135
#define IDI_ICON_EDIT_SMALL								136
#define IDI_ICON_REMOVE_LARGE							137
#define IDI_ICON_REMOVE_MID								138
#define IDI_ICON_REMOVE_SMALL							139
#define IDI_ICON_DELETE_LARGE							140
#define IDI_ICON_DELETE_MID								141
#define IDI_ICON_DELETE_SMALL							142
#define IDI_ICON_DETAIL_LARGE							143
#define IDI_ICON_DETAIL_MID								144
#define IDI_ICON_DETAIL_SMALL							145
#define IDI_ICON_REPEAT									146

#define IDR_MENU_NOTIFY_DEFAULT     		  			200
#define IDR_MENU_DEBUGTEST_CONTEXT						201

#define IDR_FILE_HELP_ENG								300
#define IDR_FILE_CHANGELOG_ENG							301
#define IDR_FILE_HELP_VIE								302
#define IDR_FILE_CHANGELOG_VIE							303
#define IDR_FILE_HELP_CHS								304
#define IDR_FILE_CHANGELOG_CHS							305

//****************************************************************************************

//************************************ Dialog IDs ****************************************

#define IDD_POWERPLUS_DIALOG        		    		1000
#define IDD_ABOUT_DLG       	        	    		1001
#define IDD_HELP_DLG	    		            		1002
#define IDD_MULTISCHEDULE_DLG							1003
#define IDD_EDITSCHEDULE_DLG  	    		        	1004
#define IDD_LOGVIEWER_DLG           		    		1005
#define IDD_LOGDETAIL_DLG           		    		1006
#define IDD_HOTKEYSET_DLG	       	    	    		1007
#define IDD_PWRREMINDER_DLG								1008
#define IDD_REMINDERMSG_DLG								1009
#define IDD_CUSTOM_MESSAGE_DLG							1010
#define IDD_RMDREPEATSET_DLG							1011
#define IDD_MSGSTYLESET_DLG								1012
#define IDD_DEBUGTEST_DLG								1013
#define IDD_DEBUGTESTV2_DLG								1014

//****************************************************************************************

//******************************** Resource ID groups ************************************
//

#define RESOURCEID_CONTROL								10000
#define RESOURCEID_MENUITEM								20000
#define RESOURCEID_STRING								30000

//****************************************************************************************

//************************ Cntrol resource IDs for Main dialog ***************************
//

#define CONTROLID_MAIN_DLG								(RESOURCEID_CONTROL+100)
#define IDC_MAIN_TITLE          		        		(CONTROLID_MAIN_DLG+1)
#define IDC_LEFTMOUSE_TITLE	    	    	    		(CONTROLID_MAIN_DLG+2)
#define IDC_MIDMOUSE_TITLE  	            			(CONTROLID_MAIN_DLG+3)
#define IDC_RIGHTMOUSE_TITLE    	    	    		(CONTROLID_MAIN_DLG+4)
#define IDC_LMBACTION_LIST      		      			(CONTROLID_MAIN_DLG+5)
#define IDC_MMBACTION_LIST   	        	  			(CONTROLID_MAIN_DLG+6)
#define IDC_RMBACTION_LIST 		      					(CONTROLID_MAIN_DLG+7)
#define IDC_ENABLERMBMENU_CHK   	    	  		 	(CONTROLID_MAIN_DLG+8)
#define IDC_APPLY_BTN               	    			(CONTROLID_MAIN_DLG+9)
#define IDC_RELOAD_BTN                					(CONTROLID_MAIN_DLG+10)
#define IDC_EXIT_BTN	                				(CONTROLID_MAIN_DLG+11)
#define IDC_EXPAND_BTN   	    	           			(CONTROLID_MAIN_DLG+12)
#define IDC_COLLAPSE_BTN    	            			(CONTROLID_MAIN_DLG+13)
#define IDC_ABOUT_BTN               	    			(CONTROLID_MAIN_DLG+14)
#define IDC_HELP_BTN                    				(CONTROLID_MAIN_DLG+15)
#define IDC_DEFAULT_BTN 	                			(CONTROLID_MAIN_DLG+16)
#define IDC_FRAMEWND        	            			(CONTROLID_MAIN_DLG+17)
#define IDC_SYSTEM_TITLE        	        			(CONTROLID_MAIN_DLG+18)
#define IDC_SHOWATSTARTUP_CHK       	    			(CONTROLID_MAIN_DLG+19)
#define IDC_ENABLEAUTOSTART_CHK         				(CONTROLID_MAIN_DLG+20)
#define IDC_DISPLAYTIP_CHK              				(CONTROLID_MAIN_DLG+21)
#define IDC_CONFIRMACTION_CHK   	        			(CONTROLID_MAIN_DLG+22)
#define IDC_SAVEHISTORYLOG_CHK       	    			(CONTROLID_MAIN_DLG+23)
#define IDC_SAVEAPPEVENTLOG_CHK         	   			(CONTROLID_MAIN_DLG+24)
#define IDC_RUNASADMIN_CHK								(CONTROLID_MAIN_DLG+25)
#define IDC_SHOWERROR_CHK       	        			(CONTROLID_MAIN_DLG+26)
#define IDC_ENABLESOUND_CHK         	    			(CONTROLID_MAIN_DLG+27)
#define IDC_SCHEDNOTIFY_CHK             				(CONTROLID_MAIN_DLG+28)
#define IDC_SCHEDALLOWCANCEL_CHK 	   	    			(CONTROLID_MAIN_DLG+29)
#define IDC_ENBBKGRDHOTKEYS_CHK         				(CONTROLID_MAIN_DLG+30)
#define IDC_ENBPWRREMINDER_CHK    		   				(CONTROLID_MAIN_DLG+31)
#define IDC_LANGUAGE_TITLE              				(CONTROLID_MAIN_DLG+32)
#define IDC_LANGUAGE_LIST              		 			(CONTROLID_MAIN_DLG+33)
#define IDC_VIEWACTIONLOG_BTN           				(CONTROLID_MAIN_DLG+34)
#define IDC_VIEWBAKCFG_BTN								(CONTROLID_MAIN_DLG+35)
#define IDC_BACKUPCFG_BTN       	        			(CONTROLID_MAIN_DLG+36)
#define IDC_SCHEDULE_BTN            	  				(CONTROLID_MAIN_DLG+37)
#define IDC_LOGVIEWER_BTN               				(CONTROLID_MAIN_DLG+38)
#define IDC_HOTKEYSET_BTN              					(CONTROLID_MAIN_DLG+39)
#define IDC_PWRREMINDER_BTN								(CONTROLID_MAIN_DLG+40)
#define IDC_COLLAPSE_BTNPOS     	        			(CONTROLID_MAIN_DLG+41)
#define IDC_EXPAND_BTNPOS           	    			(CONTROLID_MAIN_DLG+42)

//****************************************************************************************

//*********************** Control resource IDs for About dialog **************************
//

#define CONTROLID_ABOUT_DLG								(RESOURCEID_CONTROL+200)
#define IDC_APPNAME_LABEL               				(CONTROLID_ABOUT_DLG+1)
#define IDC_COPYRIGHT_LABEL             				(CONTROLID_ABOUT_DLG+2)
#define IDC_AUTH_LABEL       	    	       			(CONTROLID_ABOUT_DLG+3)
#define IDC_VIEW_FACEBOOK_PROFILE_LINK	    	        (CONTROLID_ABOUT_DLG+4)
#define IDC_APPINFO_LABEL               				(CONTROLID_ABOUT_DLG+5)
#define IDC_ABOUT_CLOSE_BTN								(CONTROLID_ABOUT_DLG+6)

//****************************************************************************************

//*********************** Control resource IDs for Help dialog ***************************
//

#define CONTROLID_HELP_DLG								(RESOURCEID_CONTROL+300)
#define IDC_HELPINFO_EDITBOX   	            			(CONTROLID_HELP_DLG+1)
#define IDC_HELP_CLOSE_BTN								(CONTROLID_HELP_DLG+2)
#define IDC_HELP_SWITCHVIEWMODE_BTN						(CONTROLID_HELP_DLG+3)

//****************************************************************************************

//****************** Control resource IDs for MultiSchedule dialog ***********************
//

#define CONTROLID_MULTISCHEDULE_DLG						(RESOURCEID_CONTROL+400)
#define IDC_MULTISCHEDULE_ITEM_LISTBOX					(CONTROLID_MULTISCHEDULE_DLG+1)
#define IDC_MULTISCHEDULE_ADD_BTN						(CONTROLID_MULTISCHEDULE_DLG+2)
#define IDC_MULTISCHEDULE_EDIT_BTN						(CONTROLID_MULTISCHEDULE_DLG+3)
#define IDC_MULTISCHEDULE_REMOVE_BTN					(CONTROLID_MULTISCHEDULE_DLG+4)
#define IDC_MULTISCHEDULE_REMOVEALL_BTN					(CONTROLID_MULTISCHEDULE_DLG+5)
#define IDC_MULTISCHEDULE_CHECKALL_BTN					(CONTROLID_MULTISCHEDULE_DLG+6)
#define IDC_MULTISCHEDULE_UNCHECKALL_BTN				(CONTROLID_MULTISCHEDULE_DLG+7)
#define IDC_MULTISCHEDULE_VIEWDETAILS_BTN				(CONTROLID_MULTISCHEDULE_DLG+8)
#define IDC_MULTISCHEDULE_SETDEFAULT_BTN				(CONTROLID_MULTISCHEDULE_DLG+9)
#define IDC_MULTISCHEDULE_APPLY_BTN          			(CONTROLID_MULTISCHEDULE_DLG+10)
#define IDC_MULTISCHEDULE_CANCEL_BTN					(CONTROLID_MULTISCHEDULE_DLG+11)

//****************************************************************************************

//******************* Control resource IDs for EditSchedule dialog ***********************
//

#define CONTROLID_EDITSCHEDULE_DLG						(RESOURCEID_CONTROL+500)
#define IDC_EDITSCHEDULE_DETAIL_STATIC					(CONTROLID_EDITSCHEDULE_DLG+1)
#define IDC_EDITSCHEDULE_SUBDETAIL_STATIC				(CONTROLID_EDITSCHEDULE_DLG+2)
#define IDC_EDITSCHEDULE_ENABLE_CHK         			(CONTROLID_EDITSCHEDULE_DLG+3)
#define IDC_EDITSCHEDULE_ACTION_LABEL       			(CONTROLID_EDITSCHEDULE_DLG+4)
#define IDC_EDITSCHEDULE_ACTION_LIST 		       		(CONTROLID_EDITSCHEDULE_DLG+5)
#define IDC_EDITSCHEDULE_TIME_LABEL         			(CONTROLID_EDITSCHEDULE_DLG+6)
#define IDC_EDITSCHEDULE_TIME_EDITBOX          			(CONTROLID_EDITSCHEDULE_DLG+7)
#define IDC_EDITSCHEDULE_TIME_SPIN         		 		(CONTROLID_EDITSCHEDULE_DLG+8)
#define IDC_EDITSCHEDULE_REPEATDAILY_CHK    			(CONTROLID_EDITSCHEDULE_DLG+9)
#define IDC_EDITSCHEDULE_ACTIVEDAYS_LISTBOX				(CONTROLID_EDITSCHEDULE_DLG+10)
#define IDC_EDITSCHEDULE_APPLY_BTN          			(CONTROLID_EDITSCHEDULE_DLG+11)
#define IDC_EDITSCHEDULE_CANCEL_BTN						(CONTROLID_EDITSCHEDULE_DLG+12)

//****************************************************************************************

//********************* Control resource IDs for LogViewer dialog ************************
//

#define CONTROLID_LOGVIEWER_DLG							(RESOURCEID_CONTROL+600)
#define IDC_LOGVIEWER_LOGDATA_LISTBOX              		(CONTROLID_LOGVIEWER_DLG+1)
#define IDC_LOGVIEWER_REMOVEALL_BTN          			(CONTROLID_LOGVIEWER_DLG+2)
#define IDC_LOGVIEWER_DETAILS_BTN          				(CONTROLID_LOGVIEWER_DLG+3)
#define IDC_LOGVIEWER_CLOSE_BTN        					(CONTROLID_LOGVIEWER_DLG+4)

//****************************************************************************************

// ******************* Control resource IDs for LogDetail dialog *************************
//

#define CONTROLID_LOGDETAIL_DLG							(RESOURCEID_CONTROL+700)
#define IDC_LOGDETAIL_PREV_BTN        					(CONTROLID_LOGDETAIL_DLG+1)
#define IDC_LOGDETAIL_NEXT_BTN        					(CONTROLID_LOGDETAIL_DLG+2)
#define IDC_LOGDETAIL_CLOSE_BTN        					(CONTROLID_LOGDETAIL_DLG+3)

//****************************************************************************************

//******************* Control resource IDs for HotkeySet dialog **************************
//

#define CONTROLID_HOTKEYSET_DLG							(RESOURCEID_CONTROL+800)
#define IDC_HOTKEYSET_ITEM_LISTBOX              		(CONTROLID_HOTKEYSET_DLG+1)
#define IDC_HOTKEYSET_ADD_BTN           				(CONTROLID_HOTKEYSET_DLG+2)
#define IDC_HOTKEYSET_REMOVE_BTN        				(CONTROLID_HOTKEYSET_DLG+3)
#define IDC_HOTKEYSET_REMOVEALL_BTN     				(CONTROLID_HOTKEYSET_DLG+4)
#define IDC_HOTKEYSET_CHECKALL_BTN     					(CONTROLID_HOTKEYSET_DLG+5)
#define IDC_HOTKEYSET_UNCHECKALL_BTN     				(CONTROLID_HOTKEYSET_DLG+6)
#define IDC_HOTKEYSET_APPLY_BTN         				(CONTROLID_HOTKEYSET_DLG+7)
#define IDC_HOTKEYSET_CANCEL_BTN        				(CONTROLID_HOTKEYSET_DLG+8)
#define IDC_HOTKEYSET_DETAIL_STATIC         			(CONTROLID_HOTKEYSET_DLG+9)
#define IDC_HOTKEYSET_ACTION_TITLE      				(CONTROLID_HOTKEYSET_DLG+10)
#define IDC_HOTKEYSET_ACTION_LIST       				(CONTROLID_HOTKEYSET_DLG+11)
#define IDC_HOTKEYSET_CONTROLKEY_STATIC     			(CONTROLID_HOTKEYSET_DLG+12)
#define IDC_HOTKEYSET_CTRLKEY_CHK          				(CONTROLID_HOTKEYSET_DLG+13)
#define IDC_HOTKEYSET_ALTKEY_CHK          				(CONTROLID_HOTKEYSET_DLG+14)
#define IDC_HOTKEYSET_WINKEY_CHK       					(CONTROLID_HOTKEYSET_DLG+15)
#define IDC_HOTKEYSET_FUNCKEY_TITLE     				(CONTROLID_HOTKEYSET_DLG+16)
#define IDC_HOTKEYSET_FUNCKEY_LIST      				(CONTROLID_HOTKEYSET_DLG+17)
#define IDC_HOTKEYSET_EXPORT_BTN						(CONTROLID_HOTKEYSET_DLG+18)

//****************************************************************************************

//****************** Control resource IDs for PwrReminder dialog *************************
//

#define CONTROLID_PWRREMINDER_DLG						(RESOURCEID_CONTROL+900)
#define IDC_PWRREMINDER_ITEM_LISTBOX					(CONTROLID_PWRREMINDER_DLG+1)
#define IDC_PWRREMINDER_ADD_BTN							(CONTROLID_PWRREMINDER_DLG+2)
#define IDC_PWRREMINDER_EDIT_BTN						(CONTROLID_PWRREMINDER_DLG+3)
#define IDC_PWRREMINDER_REMOVE_BTN						(CONTROLID_PWRREMINDER_DLG+4)
#define IDC_PWRREMINDER_REMOVEALL_BTN					(CONTROLID_PWRREMINDER_DLG+5)
#define IDC_PWRREMINDER_CHECKALL_BTN					(CONTROLID_PWRREMINDER_DLG+6)
#define IDC_PWRREMINDER_UNCHECKALL_BTN					(CONTROLID_PWRREMINDER_DLG+7)
#define IDC_PWRREMINDER_PREVIEW_BTN						(CONTROLID_PWRREMINDER_DLG+8)
#define IDC_PWRREMINDER_APPLY_BTN						(CONTROLID_PWRREMINDER_DLG+9)
#define IDC_PWRREMINDER_CANCEL_BTN						(CONTROLID_PWRREMINDER_DLG+10)
#define IDC_PWRREMINDER_DETAIL_STATIC					(CONTROLID_PWRREMINDER_DLG+11)
#define IDC_PWRREMINDER_MSGSTRING_TITLE					(CONTROLID_PWRREMINDER_DLG+12)
#define IDC_PWRREMINDER_MSGSTRING_EDITBOX				(CONTROLID_PWRREMINDER_DLG+13)
#define IDC_PWRREMINDER_MSGSTRING_COUNTER				(CONTROLID_PWRREMINDER_DLG+14)
#define IDC_PWRREMINDER_EVENT_TITLE						(CONTROLID_PWRREMINDER_DLG+15)
#define IDC_PWRREMINDER_EVENT_SETTIME_RADBTN			(CONTROLID_PWRREMINDER_DLG+16)
#define IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX			(CONTROLID_PWRREMINDER_DLG+17)
#define IDC_PWRREMINDER_EVENT_SETTIME_SPIN				(CONTROLID_PWRREMINDER_DLG+18)
#define IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN			(CONTROLID_PWRREMINDER_DLG+19)
#define IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN			(CONTROLID_PWRREMINDER_DLG+20)
#define IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN		(CONTROLID_PWRREMINDER_DLG+21)
#define IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN		(CONTROLID_PWRREMINDER_DLG+22)
#define IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN			(CONTROLID_PWRREMINDER_DLG+23)
#define IDC_PWRREMINDER_EVENT_REPEATSET_BTN				(CONTROLID_PWRREMINDER_DLG+24)
#define IDC_PWRREMINDER_MSGSTYLE_TITLE					(CONTROLID_PWRREMINDER_DLG+25)
#define IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN			(CONTROLID_PWRREMINDER_DLG+26)
#define IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN			(CONTROLID_PWRREMINDER_DLG+27)
#define IDC_PWRREMINDER_MSGSTYLE_COMBO					(CONTROLID_PWRREMINDER_DLG+28)

//****************************************************************************************

//******************** Control resource IDs for ReminderMsg dialog ***********************
//

#define CONTROLID_REMINDERMSG_DLG						(RESOURCEID_CONTROL+1000)
#define IDC_REMINDERMSG_MSGTEXT_STATIC					(CONTROLID_REMINDERMSG_DLG+1)

//****************************************************************************************

//******************* Control resource IDs for RmdRepeatSet dialog ***********************
//

#define CONTROLID_RMDREPEATSET_DLG						(RESOURCEID_CONTROL+1100)
#define IDC_RMDREPEATSET_REPEAT_CHK						(CONTROLID_RMDREPEATSET_DLG+1)
#define IDC_RMDREPEATSET_DETAILS_STATIC					(CONTROLID_RMDREPEATSET_DLG+2)
#define IDC_RMDREPEATSET_SNOOZE_CHK						(CONTROLID_RMDREPEATSET_DLG+3)
#define IDC_RMDREPEATSET_SNOOZE_INTERVAL_EDIT			(CONTROLID_RMDREPEATSET_DLG+4)
#define IDC_RMDREPEATSET_SNOOZE_INTERVAL_SPIN			(CONTROLID_RMDREPEATSET_DLG+5)
#define IDC_RMDREPEATSET_ACTIVEDAYS_STATIC				(CONTROLID_RMDREPEATSET_DLG+6)
#define IDC_RMDREPEATSET_ACTIVE_MONDAY_CHK				(CONTROLID_RMDREPEATSET_DLG+7)
#define IDC_RMDREPEATSET_ACTIVE_TUESDAY_CHK				(CONTROLID_RMDREPEATSET_DLG+8)
#define IDC_RMDREPEATSET_ACTIVE_WEDNESDAY_CHK			(CONTROLID_RMDREPEATSET_DLG+9)
#define IDC_RMDREPEATSET_ACTIVE_THURSDAY_CHK			(CONTROLID_RMDREPEATSET_DLG+10)
#define IDC_RMDREPEATSET_ACTIVE_FRIDAY_CHK				(CONTROLID_RMDREPEATSET_DLG+11)
#define IDC_RMDREPEATSET_ACTIVE_SATURDAY_CHK			(CONTROLID_RMDREPEATSET_DLG+12)
#define IDC_RMDREPEATSET_ACTIVE_SUNDAY_CHK				(CONTROLID_RMDREPEATSET_DLG+13)
#define IDC_RMDREPEATSET_DUMMY_BORDER					(CONTROLID_RMDREPEATSET_DLG+14)

//****************************************************************************************

//******************** Control resource IDs for DebugTest dialog *************************
//

#define CONTROLID_DEBUGTEST_DLG							(RESOURCEID_CONTROL+1200)
#define IDC_DEBUGTEST_EDITVIEW							(CONTROLID_DEBUGTEST_DLG+1)
#define IDC_DEBUGTESTV2_VIEW							(CONTROLID_DEBUGTEST_DLG+2)
#define IDC_DEBUGTESTV2_INPUT							(CONTROLID_DEBUGTEST_DLG+3)

//****************************************************************************************

//************************ Item resource IDs for notify menu *****************************
//

#define MENUID_NOTIFY_DLG								(RESOURCEID_MENUITEM+100)
#define IDM_NOTIFY_OPENDLG_ABOUT   	            		(MENUID_NOTIFY_DLG+1)
#define IDM_NOTIFY_OPENDLG_HELP       		        	(MENUID_NOTIFY_DLG+2)
#define IDM_NOTIFY_OPENDLG_SCHEDULE    	        		(MENUID_NOTIFY_DLG+3)
#define IDM_NOTIFY_OPENDLG_LOGVIEWER					(MENUID_NOTIFY_DLG+4)
#define IDM_NOTIFY_OPENDLG_HOTKEYSET					(MENUID_NOTIFY_DLG+5)
#define IDM_NOTIFY_OPENDLG_PWRREMINDER					(MENUID_NOTIFY_DLG+6)
#define IDM_NOTIFY_VIEW_BAKCONFIG						(MENUID_NOTIFY_DLG+7)
#define IDM_NOTIFY_VIEW_ACTIONLOG      	        		(MENUID_NOTIFY_DLG+8)
#define IDM_NOTIFY_ACTIONS_TITLE          		    	12
#define IDM_NOTIFY_ACTION_SHUTDOWN         	    		(MENUID_NOTIFY_DLG+9)
#define IDM_NOTIFY_ACTION_SLEEP							(MENUID_NOTIFY_DLG+10)
#define IDM_NOTIFY_ACTION_RESTART             			(MENUID_NOTIFY_DLG+11)
#define IDM_NOTIFY_ACTION_SIGNOUT             			(MENUID_NOTIFY_DLG+12)
#define IDM_NOTIFY_ACTION_STANDBY             			(MENUID_NOTIFY_DLG+13)
#define IDM_NOTIFY_ACTION_DISPLAYOFF          			(MENUID_NOTIFY_DLG+14)
#define IDM_NOTIFY_ACTION_HIBERNATE           			(MENUID_NOTIFY_DLG+15)
#define IDM_NOTIFY_ACTION_SCHEDULE						(MENUID_NOTIFY_DLG+16)
#define IDM_NOTIFY_RESTART_APP                			(MENUID_NOTIFY_DLG+17)
#define IDM_NOTIFY_BACKUP_CONFIG               			(MENUID_NOTIFY_DLG+18)
#define IDM_NOTIFY_RESTART_ASADMIN						(MENUID_NOTIFY_DLG+19)
#define IDM_NOTIFY_SHOW_WINDOW                 			(MENUID_NOTIFY_DLG+20)
#define	IDM_NOTIFY_EXIT_APP								(MENUID_NOTIFY_DLG+21)

//****************************************************************************************

//*********************** Item resource IDs for DebugTest menu ***************************

#define MENUID_DEBUGTEST_DLG							(RESOURCEID_MENUITEM+200)
#define IDM_DEBUGTEST_COPY                 				(MENUID_DEBUGTEST_DLG+1)
#define IDM_DEBUGTEST_COPY_ALL                 			(MENUID_DEBUGTEST_DLG+2)
#define IDM_DEBUGTEST_PASTE								(MENUID_DEBUGTEST_DLG+3)
#define IDM_DEBUGTEST_SELECT_ALL						(MENUID_DEBUGTEST_DLG+4)
#define IDM_DEBUGTEST_DISP_PREVCOMMAND					(MENUID_DEBUGTEST_DLG+5)
#define IDM_DEBUGTEST_DISP_NEXTCOMMAND					(MENUID_DEBUGTEST_DLG+6)
#define IDM_DEBUGTEST_CLEAR_BUFFER                 		(MENUID_DEBUGTEST_DLG+7)
#define IDM_DEBUGTEST_DEFAULT_SIZE						(MENUID_DEBUGTEST_DLG+8)
#define IDM_DEBUGTEST_STATUS_LINE						(MENUID_DEBUGTEST_DLG+9)
#define IDM_DEBUGTEST_CLOSE								(MENUID_DEBUGTEST_DLG+10)

//****************************************************************************************

//////////////////////////////////////////////////////////////////////////////////
//
// Define string resource IDs
// Note: These are the rules on how to numbering the string resource groups
//		  1. From 0 - 99		: Not used
//		  2. From 100 - 999		: Common and basic strings
//		  3. From 1000 - 9999	: Other resource strings
//
//////////////////////////////////////////////////////////////////////////////////


// Define string resource IDs for application basic info
//

#define STRINGID_APP_BASIC								(RESOURCEID_STRING+100)
#define IDS_APP_WINDOW_CAPTION							(STRINGID_APP_BASIC+1)
#define IDS_APP_SYSMENU_ABOUT            				(STRINGID_APP_BASIC+2)
#define IDS_APP_DEBUGTESTDLG_TITLE						(STRINGID_APP_BASIC+3)


// Define string resource IDs for others
//

#define STRINGID_OTHER									(RESOURCEID_STRING+1000)
#define IDS_FORMAT_FULLDATETIME							(STRINGID_OTHER+1)
#define IDS_FORMAT_LOGSTRING							(STRINGID_OTHER+2)
#define IDS_FORMAT_SHORTDATE							(STRINGID_OTHER+3)
#define IDS_FORMAT_SHORTTIME							(STRINGID_OTHER+4)

