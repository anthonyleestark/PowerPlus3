/**
 * @file		BaseTypes.cpp
 * @brief		Implementations of very basic and utility data types for application
 * @author		AnthonyLeeStark
 * @date		2025.06.04
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "AppBase/BaseTypes.h"


// Format string
String& String::FormatV(const wchar_t* formatStr, va_list vargs)
{
	if (!formatStr) _buffer.clear();
	else {
		wchar_t _tempBuff[1024];

		va_list _args;
		va_copy(_args, vargs);
		int len = std::vswprintf(_tempBuff, sizeof(_tempBuff) / sizeof(wchar_t), formatStr, _args);
		va_end(_args);

		if (len >= 0 && static_cast<size_t>(len) < sizeof(_tempBuff) / sizeof(wchar_t))
			_buffer = _tempBuff;
		else {
			size_t size = (len > 0) ? len + 1 : 4096;
			std::vector<wchar_t> dynamic(size);

			va_copy(_args, vargs);
			len = std::vswprintf(dynamic.data(), size, formatStr, _args);
			va_end(_args);

			if (len < 0) _buffer.clear();
			else
				_buffer.assign(dynamic.data(), static_cast<size_t>(len));
		}
	}

	return *this;
}

// Upper first character of each word
String& String::UpperEachWord(void)
{
	if (_buffer.length() <= 0) _buffer.clear();
	else {
		// Capitalize first character of each word
		for (int _index = 0; _index < _buffer.length(); _index++) {
			if (_buffer.at(_index) != Constant::Char::Space) {
				if (_index == 0	/* First character */ ||
					/* Not the first character and standing right next to a space */
					(_index > 0 && _buffer.at(_index - 1) == Constant::Char::Space)) {
					// Convert to uppercase
					_buffer[_index] = std::towupper(_buffer[_index]);
				}
			}
		}
	}
	
	return *this;
}

// Tokenization
TokenList String::Tokenize(const wchar_t* delimiters) const
{
	TokenList _tokens;
	_tokens.reserve(Constant::Max::TokenNumber);

	std::wstring _currentToken;
	_currentToken.reserve(Constant::Max::TokenLength);

	bool _inQuotes = false;
	const std::wstring _delimiters(delimiters);

	for (wchar_t _ch : _buffer)
	{
		// Handle quotation mark
		if (_ch == Constant::Char::QuotaMark) {

			_inQuotes = !_inQuotes;

			// If token number exceeds maximum limitation, stop processing
			if (_tokens.size() >= Constant::Max::TokenNumber)
				break;

			// Don't add quote to token
			continue;
		}

		// Check for end-of-line characters
		bool _isEndOfLine = (_ch == Constant::Char::EndLine || _ch == Constant::Char::Return);

		// Check for tab or space characters
		bool _isTabOrSpace = (_ch == Constant::Char::Tab || std::iswspace(_ch));

		// Check for delimiter characters (skip if inside quotes)
		bool _isDelimiter = !_inQuotes && (_isEndOfLine || _isTabOrSpace || _delimiters.find(_ch) != std::wstring::npos);

		// If we hit a delimiter or end-of-string
		if (_isDelimiter || _ch == Constant::Char::EndString) {

			if (!_currentToken.empty()) {

				// Add current token to list
				_tokens.push_back(_currentToken);

				// Start a new token
				_currentToken.clear();
				_currentToken.reserve(Constant::Max::TokenLength);
			}

			// If end of string or token number exceeds maximum limitation, stop processing
			if (_tokens.size() >= Constant::Max::TokenNumber || _ch == Constant::Char::EndString)
				break;

			continue;
		}

		// If end-of-line but inside quotes -> skip (do not add)
		if (_inQuotes && _isEndOfLine)
			continue;

		// Normal character, add to current token
		_currentToken.push_back(_ch);
	}

	// Add any remaining token to list
	if (!_currentToken.empty() && _tokens.size() < Constant::Max::TokenNumber)
		_tokens.push_back(_currentToken);

	return _tokens;
}

