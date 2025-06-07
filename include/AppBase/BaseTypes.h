
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		BaseTypes.h
//		Description:	Declare very basic and utility data types for application
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2025.06.04:		Create new
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _BASE_TYPES_INCLUDED
#define _BASE_TYPES_INCLUDED
#endif


// Include necessary dependencies
#ifndef _CPP_STANDARD_INCLUDED
	#include "AppCommon.h"
#endif
#ifndef _APP_MACROS_INCLUDED
	#include "AppMacros.h"
#endif
#ifndef _APP_CONSTANTS_INCLUDED
	#include "AppConstants.h"
#endif


// Using Unicode for strings
#ifndef _UNICODE
	#define	_UNICODE
#endif


// For tokenization
using TokenList = typename std::vector<std::wstring>;


// Using va_list for string formatted
#if !defined(_INC_STDARG) || !defined(_CSTDARG_)
	#include <cstdarg>
#endif


// Using wide-character classification/conversion
#if !defined(_INC_WCTYPE) || !defined(_CWCTYPE_)
	#include <cwctype>
#endif


// Basic string
// Wrapper of STL wstring
class String
{
private:
	// Using STL wstring as base buffer
	std::wstring _buffer;

public:
	// Construction
	explicit String() = default;
	String(const std::wstring& srcString) : _buffer(srcString) {};
	String(const wchar_t* srcBuff) : _buffer(srcBuff) {};

	// Copy constructor
	String(const String& srcString) {
		_buffer.assign(srcString._buffer);
	};

	// Copy assignment operator
	String& operator=(const String& srcString) {
		if (this != &srcString)
			_buffer.assign(srcString._buffer);
		return *this;
	};

	// Move constructor
	String(String&& srcString) noexcept : _buffer(std::move(srcString._buffer)) {};

	// Move assignment operator
	String& operator=(String&& srcString) noexcept {
		if (this != &srcString)
			_buffer = std::move(srcString._buffer);
		return *this;
	};

public:
	// Subscript operator
	constexpr wchar_t& operator[](const size_t index) {
		return _buffer.at(index);
	};
	constexpr const wchar_t& operator[](const size_t index) const {
		return _buffer.at(index);
	};

	// Concatenation operator
	String operator+(const String& other) const noexcept {
		return String(_buffer + other._buffer);
	};
	String& operator+=(const String& other) {
		if (this != &other) _buffer += other._buffer;
		return *this;
	};
	String operator+(const std::wstring& other) const noexcept {
		return String(_buffer + other);
	};
	String& operator+=(const std::wstring& other) {
		if (_buffer != other) _buffer += other;
		return *this;
	};
	String operator+(const wchar_t* other) const noexcept {
		return String(_buffer + other);
	};
	String& operator+=(const wchar_t* other) {
		_buffer += other;
		return *this;
	};

	// Equality operator
	constexpr bool operator==(const String& other) const noexcept {
		return _buffer == other._buffer;
	};
	constexpr bool operator==(const std::wstring& other) const noexcept {
		return _buffer == other;
	};
	constexpr bool operator==(const wchar_t* other) const noexcept {
		return _buffer == other;
	};

	// Inequality operator
	constexpr bool operator!=(const String& other) const noexcept {
		return _buffer != other._buffer;
	};
	constexpr bool operator!=(const std::wstring& other) const noexcept {
		return _buffer != other;
	};
	constexpr bool operator!=(const wchar_t* other) const noexcept {
		return _buffer != other;
	};

	// Comparison operator
	constexpr bool operator<(const String& other) noexcept {
		return _buffer < other._buffer;
	};
	constexpr bool operator<(const std::wstring& other) noexcept {
		return _buffer < other;
	};
	constexpr bool operator<(const wchar_t* other) noexcept {
		return _buffer < other;
	};
	constexpr bool operator>(const String& other) noexcept {
		return _buffer > other._buffer;
	};
	constexpr bool operator>(const std::wstring& other) noexcept {
		return _buffer > other;
	};
	constexpr bool operator>(const wchar_t* other) noexcept {
		return _buffer > other;
	};
	constexpr bool operator<=(const String& other) noexcept {
		return _buffer <= other._buffer;
	};
	constexpr bool operator<=(const std::wstring& other) noexcept {
		return _buffer <= other;
	};
	constexpr bool operator<=(const wchar_t* other) noexcept {
		return _buffer <= other;
	};
	constexpr bool operator>=(const String& other) noexcept {
		return _buffer >= other._buffer;
	};
	constexpr bool operator>=(const std::wstring& other) noexcept {
		return _buffer >= other;
	};
	constexpr bool operator>=(const wchar_t* other) noexcept {
		return _buffer >= other;
	};

	// Stream insertion operator
	friend std::wostream& operator<<(std::wostream& os, const String& str) {
		return os << str._buffer;
	};

	// Stream extraction operator
	friend std::wistream& operator>>(std::wistream& is, String& str) {
		return is >> str._buffer;
	};

	// Implicit conversion operator
	operator std::wstring() const noexcept {
		return _buffer;
	};
	operator const wchar_t*() const noexcept {
		return _buffer.c_str();
	};

	// Prefix increment operator (append a space character)
	String& operator++() noexcept {
		_buffer += Constant::Symbol::Space;
		return *this;
	};

	// Prefix decrement operator (remove the last character)
	String& operator--() noexcept {
		if (!_buffer.empty()) _buffer.pop_back();
		return *this;
	};

	// Postfix increment operator
	// Append a space character but return the original string
	String& operator++(int) noexcept {
		String _temp = *this;
		_buffer += Constant::Symbol::Space;
		return _temp;
	};

	// Prefix decrement operator
	// Remove the last character but return the original string
	String& operator--(int) noexcept {
		String _temp = *this;
		if (!_buffer.empty()) _buffer.pop_back();
		return _temp;
	};

public:
	// Validation and access attributes
	constexpr bool IsEmpty(void) const noexcept {
		return _buffer.empty();
	};
	constexpr size_t GetLength(void) const noexcept {
		return _buffer.length();
	};

public:
	// Get/set functions
	constexpr const wchar_t* GetString(void) const noexcept {
		return _buffer.c_str();
	};
	void SetString(const std::wstring& srcString) {
		this->_buffer.assign(srcString);
	};
	void SetString(const wchar_t* srcBuff) {
		this->_buffer = srcBuff;
	};

	// Access characters
	constexpr wchar_t& At(const size_t index) {
		return _buffer.at(index);
	};
	constexpr const wchar_t& At(const size_t index) const {
		return _buffer.at(index);
	};
	constexpr wchar_t& GetAt(const size_t index) {
		return _buffer.at(index);
	};
	constexpr const wchar_t& GetAt(const size_t index) const {
		return _buffer.at(index);
	};
	void SetAt(const size_t index, const wchar_t& ch) noexcept {
		if (_buffer.size() <= index) return;
		_buffer[index] = ch;
	};

	// Extract a part of the string
	String Left(int count) const {
		if (count <= 0) return String(Constant::String::Empty);
		else if (static_cast<size_t>(count) >= _buffer.length()) return String(_buffer);
		else return String(_buffer.substr(0, static_cast<size_t>(count)));
	};
	String Right(int count) const {
		if (count <= 0) return String(Constant::String::Empty);
		else if (static_cast<size_t>(count) >= _buffer.length()) return String(_buffer);
		else {
			size_t _start = _buffer.length() - static_cast<size_t>(count);
			return String(_buffer.substr(_start));
		}
	};
	String Mid(int start, int count = -1) const {
		if (start < 0 || static_cast<size_t>(start) >= _buffer.length()) return String(Constant::String::Empty);
		size_t _startPos = static_cast<size_t>(start); size_t _maxCount = _buffer.length() - _startPos;
		if (count < 0 || static_cast<size_t>(count) > _maxCount) count = static_cast<int>(_maxCount);
		return String(_buffer.substr(_startPos, static_cast<size_t>(count)));
	};

public:
	// Append another string
	String& Append(const String& other) {
		if (this != &other) _buffer += other._buffer;
		return *this;
	};
	String& Append(const std::wstring& other) {
		_buffer += other;
		return *this;
	};
	String& Append(const wchar_t* other) {
		_buffer += other;
		return *this;
	};

	// Append a character
	String& AppendChar(const wchar_t& ch) {
		_buffer.push_back(ch);
		return *this;
	};
	String& PushBack(const wchar_t& ch) {
		_buffer.push_back(ch);
		return *this;
	};

	// Remove all occurences of a character
	String& Remove(const wchar_t& ch) {
		_buffer.erase(std::remove(_buffer.begin(), _buffer.end(), ch), _buffer.end());
		return *this;
	};

	// Remove all occurences of a substring
	String& Remove(const String& subStr) {
		return Remove(static_cast<const wchar_t*>(subStr));
	};
	String& Remove(const std::wstring& subStr) {
		return Remove(subStr.c_str());
	};
	String& Remove(const wchar_t* subStr) {
		size_t _pos = 0;
		while ((_pos = _buffer.find(subStr, _pos)) != std::wstring::npos)
			_buffer.replace(_pos, wcslen(subStr), Constant::String::Empty);
		return *this;
	};

	// Remove all characters
	void Empty(void) noexcept {
		_buffer.clear();
	};

	// Replace all occurrences of a substring
	String& Replace(const String& substrOld, const String& substrNew) {
		return Replace(static_cast<const wchar_t*>(substrOld), static_cast<const wchar_t*>(substrNew));
	};
	String& Replace(const std::wstring& substrOld, const std::wstring& substrNew) {
		return Replace(substrOld.c_str(), substrNew.c_str());
	};
	String& Replace(const wchar_t* substrOld, const wchar_t* substrNew) {
		size_t pos = 0;
		while ((pos = _buffer.find(substrOld, pos)) != std::wstring::npos) {
			_buffer.replace(pos, wcslen(substrOld), substrNew);
			pos += wcslen(substrNew);
		}
		return *this;
	};

	// Delete one or many characters from the string
	int Delete(int index, int count = 1) {
		if (index < 0 || count <= 0 || static_cast<size_t>(index) >= _buffer.length()) return static_cast<int>(_buffer.length());
		size_t _count = std::min(static_cast<size_t>(count), _buffer.length() - static_cast<size_t>(index));
		return static_cast<int>(_buffer.erase(static_cast<size_t>(index), _count).length());
	};

	// Lowercase conversion
	String& ToLower(void) {
		std::transform(_buffer.begin(), _buffer.end(), _buffer.begin(), ::towlower);
		return *this;
	};

	// Uppercase conversion
	String& ToUpper(void) {
		std::transform(_buffer.begin(), _buffer.end(), _buffer.begin(), ::towupper);
		return *this;
	};

	// Remove all leading/trailing whitespaces
	String& Trim(void) noexcept {
		TrimLeft(); TrimRight(); return *this;
	};

	// Remove leading whitespaces
	String& TrimLeft(void) {
		_buffer.erase(_buffer.begin(), std::find_if(_buffer.begin(), _buffer.end(),
			[](wchar_t ch) { return !std::iswspace(ch); }));
		return *this;
	};

	// Remove trailing whitespaces
	String& TrimRight(void) {
		_buffer.erase(std::find_if(_buffer.rbegin(), _buffer.rend(),
			[](wchar_t ch) { return !std::iswspace(ch); }).base(), _buffer.end());
		return *this;
	};

	// Compare two string (case-sensitive)
	int Compare(const String& other) const noexcept {
		return Compare(static_cast<const wchar_t*>(other));
	};
	int Compare(const std::wstring& other) const noexcept {
		return Compare(other.c_str());
	};
	int Compare(const wchar_t* other) const noexcept {
		std::wstring _other(other);
		if (_buffer > _other) return static_cast<int>(_buffer.size() - _other.size());
		else if (_buffer < _other) return static_cast<int>(_other.size() - _buffer.size());
		else /* if (_buffer == _other) */ return 0;
	};

	// Compare two string (case-insensitive)
	int CompareNoCase(const wchar_t* other) const {
		String _temp = _buffer; _temp.ToLower(); std::wstring _other(other);
		std::transform(_other.begin(), _other.end(), _other.begin(), ::towlower);
		return _temp.Compare(_other);
	};

	// Find the first occurence of a character
	int Find(const wchar_t& ch, int start = 0) const {
		if (start < 0 || static_cast<size_t>(start) >= _buffer.length()) return -1;
		size_t _pos = _buffer.find(ch, static_cast<size_t>(start));
		return (_pos != std::wstring::npos) ? static_cast<int>(_pos) : -1;
	};

	// Find the first occurence of a substring
	int Find(const String& subStr, int start = 0) const {
		return Find(static_cast<const wchar_t*>(subStr), start);
	};
	int Find(const std::wstring& subStr, int start = 0) const {
		return Find(subStr.c_str(), start);
	};
	int Find(const wchar_t* subStr, int start = 0) const {
		if (!subStr || start < 0 || static_cast<size_t>(start) >= _buffer.length()) return -1;
		size_t _pos = _buffer.find(subStr, static_cast<size_t>(start));
		return (_pos != std::wstring::npos) ? static_cast<int>(_pos) : -1;
	};

	// Truncate the string to the new length
	String& Truncate(int newLength) {
		if (newLength <= 0) _buffer.clear();
		else if (static_cast<size_t>(newLength) < _buffer.length())
			_buffer.resize(static_cast<size_t>(newLength));
		return *this;
	};

	// Reverse the string
	String& Reverse() {
		std::reverse(_buffer.begin(), _buffer.end());
		return *this;
	};

	// Find the last match of a character
	int ReverseFind(const wchar_t& ch) const {
		String _temp = _buffer; _temp.Reverse();
		return _temp.Find(ch, 0);
	};

	// Format string
	String& Format(const wchar_t* formatStr, ...) {
		va_list args; va_start(args, formatStr);
		FormatV(formatStr, args); va_end(args);
		return *this;
	};
	String& FormatV(const wchar_t* formatStr, va_list vargs);

	// Upper first character of each word
	String& UpperEachWord(void);

	// Tokenization
	TokenList Tokenize(const wchar_t* delimiters) const;
};
