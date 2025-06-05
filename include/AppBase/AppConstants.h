
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		AppMacros.h
//		Description:	Define application base macros
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2025.06.04:		Create new
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _APP_CONSTANTS_INCLUDED
#define _APP_CONSTANTS_INCLUDED
#endif


struct Constant {

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
		static constexpr const wchar_t Space						= L' ';								// Space character
		static constexpr const wchar_t Dot							= L'.';								// Dot (.) character
		static constexpr const wchar_t Comma						= L',';								// Comma (,) character
		static constexpr const wchar_t QuotaMark					= L'\"';							// 'Quotation mark' character
		static constexpr const wchar_t Tab							= L'\t';							// Indentation (tab character)

		static constexpr const wchar_t Colon						= L':';								// Colon symbol
		static constexpr const wchar_t Dash							= L'-';								// Dash symbol
		static constexpr const wchar_t Underscore					= L'_';								// Underscore symbol
		static constexpr const wchar_t Backslash					= L'\\';							// Backslash (typically used as file/folder path separator)

		static constexpr const wchar_t Return						= L'\r';							// 'Return' character
		static constexpr const wchar_t EndLine						= L'\n';							// 'Endline' character
		static constexpr const wchar_t EndString					= L'\0';							// Null-termination (end of string)
	};

	// Special string-based values
	struct Value {
		static constexpr const wchar_t* True						= L"Yes";							// Boolean value: True (Yes)
		static constexpr const wchar_t* False						= L"No";							// Boolean value: False (No)
		static constexpr const wchar_t* Null						= L"Null";							// Null value
		static constexpr const wchar_t* Unknown						= L"Unknown";						// Unknown value
		static constexpr const wchar_t* Undefined					= L"Undefined";						// Undefined value
	};
};
