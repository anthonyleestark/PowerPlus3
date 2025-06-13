/**
 * @file		AppConstants.h
 * @brief		Define application base enums and constants
 * @author		AnthonyLeeStark
 * @date		2025.06.04
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#ifndef _APP_CONSTANTS_INCLUDED
#define _APP_CONSTANTS_INCLUDED
#endif


// Using Unicode for strings
#ifndef _UNICODE
	#define _UNICODE
#endif


// Flag values
enum Flag {
	FLAG_OFF = (int)0x00,
	FLAG_ON	 = (int)0x01,
};


// Function results
enum Result {
	Success = (int)0x0000,
	Failure = (int)0xFFFF,
};


// Function and application modes
enum Mode {
	Init	= (int)0x000,
	View	= (int)0x001,
	Add		= (int)0x002,
	Update	= (int)0x004,
	Load	= (int)0x008,
	Save	= (int)0x010,
	Disable = (int)0x020,
};


// Using RGB for color values
#ifndef RGB
	#define RGB(r,g,b) ((unsigned long)(((unsigned char)(r)|((unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))
#endif


// Color values
enum Color {
	Red					= RGB(255,0,0),
	Green				= RGB(0,255,0),
	Yellow				= RGB(255,255,0),
	Blue				= RGB(0,0,255),
	White				= RGB(255,255,255),
	Black				= RGB(0,0,0),
	Pink				= RGB(230,190,200),
	Sakura_Pink			= RGB(250,224,216),
	Orange				= RGB(255,127,40),
	Ultramarine_Blue	= RGB(63,72,204),
	Indigo				= RGB(75,0,130),
	Violet				= RGB(238,130,238),
	Purple				= RGB(115,43,245),
	Scarlet				= RGB(255,36,0),
	Jade				= RGB(0,168,207),
	Emerald				= RGB(80,200,120),
	Gray				= RGB(200,200,200),
	Dark_Gray			= RGB(160,160,160),	
	Bright_Gray			= RGB(240,240,240),
};


// System icon IDs
struct SystemIcon {
	static constexpr int Application		= 32512;			// Default application icon
	static constexpr int Hand				= 32513;			// Error icon
	static constexpr int Question			= 32514;			// Question mark icon
	static constexpr int Exclamation		= 32515;			// Warning icon
	static constexpr int Asterisk			= 32516;			// Information icon
#if(WINVER >= 0x0400)
	static constexpr int WinLogo			= 32517;			// Windows logo icon
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0600)
	static constexpr int Shield				= 32518;			// Security shield icon
#endif /* WINVER >= 0x0600 */
	static constexpr int Warning			= Exclamation;		// Warning icon
	static constexpr int Error				= Hand;				// Error icon
	static constexpr int Information		= Asterisk;			// Information icon
};


// Constant definitions
struct Constant {

	// Minimum values
	struct Min {
		static constexpr int SnoozeTime					= 1;								// 1 minute
		static constexpr int StringLength				= 5;								// 5 characters
		static constexpr int PasswordLength				= 6;								// 6 characters

		static constexpr int TimeSpin					= 0;								// Equals to 00:00
	};

	// Default values
	struct Default {
		static constexpr int TimeSpin					= 0;								// Equals to 00:00
	};

	// Maximum values
	struct Max {
		static constexpr int SnoozeTime					= 30;								// 30 minutes
		static constexpr int DaysOfWeek					= 7;								// 7 days

		static constexpr int TokenNumber				= 50;								// 50 tokens
		static constexpr int TokenLength				= 512;								// 512 characters
		static constexpr int StringLength				= 1024;								// 2KB ~ 1024 characters
		static constexpr int TextLength					= 2097152;							// 2MB ~ 2097152 characters
		static constexpr int PasswordLength				= 30;								// 30 characters
		static constexpr int DisplayLogStringLength		= 20;								// 20 characters
		static constexpr int BackupFileNumber			= 100;								// 100 files
		static constexpr int LogFileSize				= 1048576;							// 1MB

		static constexpr int RetryTime					= 10;								// 10 times

		static constexpr int TimeSpin					= 1439;								// Equals to 23:59

		// Function timeout
		struct Timeout {
			static constexpr int WaitMessage			= 30000;							// 30 seconds
		};
	};

	// Special strings
	struct String {
		static constexpr const wchar_t* Empty						= L"";							// Empty string
		static constexpr const wchar_t* EndLine						= L"\n";						// "End line" string
		static constexpr const wchar_t* NewLine						= L"\r\n";						// "New line" string (with 'return' character)
		static constexpr const wchar_t* Null						= L"#NULL";						// Null/invalid string
		static constexpr const wchar_t* QuoteFormat					= L"\"%s\"";					// Quote string template format
	};


	// Special symbols
	struct Symbol {
		static constexpr const wchar_t* Dot							= L".";							// Dot (.) symbol
		static constexpr const wchar_t* Comma						= L",";							// Comma (,) symbol
		static constexpr const wchar_t* Colon						= L":";							// Colon (:) symbol
		static constexpr const wchar_t* Dash						= L"-";							// Dash (-) symbol
		static constexpr const wchar_t* Underscore					= L"_";							// Underscore (_) symbol
		static constexpr const wchar_t* Backslash					= L"\\";						// Backslash (typically used as file/folder path separator)

		static constexpr const wchar_t* Arrow						= L"->";						// Arrow symbol
		static constexpr const wchar_t* EqualArrow					= L"=>";						// Arrow symbol (with equal sign)
		static constexpr const wchar_t* Output						= L">>";						// Output sign
		static constexpr const wchar_t* Input						= L"<<";						// Input sign

		static constexpr const wchar_t* Space						= L" ";							// Space
		static constexpr const wchar_t* DoubleSpace					= L"  ";						// Double spaces
		static constexpr const wchar_t* Tab							= L"\t";						// Default indentation (tab character)
		static constexpr const wchar_t* JSON_Indent					= Tab;							// JSON indentation (tab character)
		static constexpr const wchar_t* YAML_Indent					= DoubleSpace;					// YAML indentation (double spaces)

		static constexpr const wchar_t* AnteMeridiem				= L"AM";						// Ante meridiem (AM)
		static constexpr const wchar_t* PostMeridiem				= L"PM";						// Post meridiem (PM)

		static constexpr const wchar_t* CurrentFolder				= L".\\";						// Current folder
		static constexpr const wchar_t* ParentFolder				= L"..\\";						// Parent folder
	};


	// Special characters
	struct Char {
		static constexpr const wchar_t Space						= L' ';							// Space character
		static constexpr const wchar_t Dot							= L'.';							// Dot (.) character
		static constexpr const wchar_t Comma						= L',';							// Comma (,) character
		static constexpr const wchar_t QuotaMark					= L'\"';						// 'Quotation mark' character
		static constexpr const wchar_t Tab							= L'\t';						// Indentation (tab character)

		static constexpr const wchar_t Colon						= L':';							// Colon symbol
		static constexpr const wchar_t Dash							= L'-';							// Dash symbol
		static constexpr const wchar_t Underscore					= L'_';							// Underscore symbol
		static constexpr const wchar_t Backslash					= L'\\';						// Backslash (typically used as file/folder path separator)

		static constexpr const wchar_t Return						= L'\r';						// 'Return' character
		static constexpr const wchar_t EndLine						= L'\n';						// 'Endline' character
		static constexpr const wchar_t EndString					= L'\0';						// Null-termination (end of string)
	};

	// Special string-based values
	struct Value {
		static constexpr const wchar_t* True						= L"Yes";						// Boolean value: True (Yes)
		static constexpr const wchar_t* False						= L"No";						// Boolean value: False (No)
		static constexpr const wchar_t* Null						= L"Null";						// Null value
		static constexpr const wchar_t* Unknown						= L"Unknown";					// Unknown value
		static constexpr const wchar_t* Undefined					= L"Undefined";					// Undefined value
	};

	// Special commands
	struct Command {
		static constexpr const wchar_t* RunAs						= L"runas";
		static constexpr const wchar_t* Open						= L"open";

		struct Registry {
			static constexpr const wchar_t* Delete					= L"reg delete %s /va /f";
			static constexpr const wchar_t* Export					= L"\"reg.exe export \"%s\" \"%s\"\" /y";
		};

		struct RunAsAdmin {
			static constexpr const wchar_t* Register				= L"schtasks /create /sc onlogon /tn %s /rl highest /tr \"%s\" /f";
			static constexpr const wchar_t* Unregister				= L"schtasks /delete /tn %s /f";
		};
	};
};
