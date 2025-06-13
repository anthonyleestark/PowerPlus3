/**
 * @file		BaseTypes.h
 * @brief		Declare very basic and utility data types for application
 * @author		AnthonyLeeStark
 * @date		2025.06.04
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

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

	// Postfix decrement operator
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
		String _temp = _buffer; _temp.Reverse(); int _pos = _temp.Find(ch, 0);
		return (_pos != -1) ? static_cast<int>(_buffer.length() - _pos) : -1;
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


// Use STL chrono for time
#ifndef _CHRONO_
	#include <chrono>
#endif


// Base class for other time classes
// Only intended to be inherited without creating instances
class __TimeBase
{
protected:
	// Using aliases for std::chrono classes
	using __TimePoint = std::chrono::system_clock::time_point;
	using __Duration = std::chrono::system_clock::duration;
	using __Date = std::chrono::year_month_day;
	using __Year = std::chrono::year;
	using __Years = std::chrono::years;
	using __Month = std::chrono::month;
	using __Months = std::chrono::months;
	using __Day = std::chrono::day;
	using __Days = std::chrono::days;
	using __Weekday = std::chrono::weekday;
	using __Hours = std::chrono::hours;
	using __Minutes = std::chrono::minutes;
	using __Seconds = std::chrono::seconds;
	using __Milliseconds = std::chrono::milliseconds;

protected:
	// Using STL Chrono time_point as base value
	__TimePoint _timePoint;

protected:
	// Constructor
	__TimeBase() : _timePoint(__TimePoint{}) {};
	explicit __TimeBase(__TimePoint src) : _timePoint(src) {};

public:
	virtual ~__TimeBase() = default;
	virtual __TimePoint GetTimePoint(void) const noexcept = 0;

protected:
	// Internal validations
	static constexpr bool _isValidHour(int hour) noexcept {
		return (hour >= 0 && hour <= 23);
	};
	static constexpr bool _isValidMinute(int minute) noexcept {
		return (minute >= 0 && minute <= 59);
	};
	static constexpr bool _isValidSecond(int second) noexcept {
		return (second >= 0 && second <= 59);
	};
	static constexpr bool _isValidMillisecs(int millisecs) noexcept {
		return (millisecs >= 0 && millisecs <= 999);
	};

public:
	// Special validation
	constexpr bool IsEmpty() const noexcept {
		return _timePoint == __TimePoint{};
	};
};


// For time span calculation
class TimeSpan : public __TimeBase
{
public:
	// Construction
	explicit TimeSpan() = default;

	// Copy constructor
	TimeSpan(const TimeSpan& other) : __TimeBase(other._timePoint) {};

	// Copy assignment operator
	TimeSpan& operator=(const TimeSpan& other) {
		if (this != &other) {
			_timePoint = other._timePoint;
		}
		return *this;
	};

	// Move constructor
	TimeSpan(TimeSpan&& other) noexcept : __TimeBase(std::move(other._timePoint)) {};

	// Move assignment operator
	TimeSpan& operator=(TimeSpan&& other) noexcept {
		if (this != &other) {
			_timePoint = std::move(other._timePoint);
		}
		return *this;
	};

public:
	// Conversion constructor
	TimeSpan(__TimePoint timepoint) : __TimeBase(timepoint) {};
	TimeSpan(int days, int hours = 0, int mins = 0, int secs = 0, int millisecs = 0) noexcept {
		_timePoint = __TimePoint{ duration_cast<__TimePoint::duration>(__Days{ days } +
				__Hours{ hours } + __Minutes { mins } + __Seconds { secs } + __Milliseconds{ millisecs }) };
	};

public:
	// Get time point (conversion to std::chrono::system_clock::time_point)
	__TimePoint GetTimePoint(void) const noexcept override {
		return _timePoint;
	};

public:
	// Addition operator
	TimeSpan operator+(const TimeSpan& other) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() + other._timePoint.time_since_epoch()));
	};
	TimeSpan& operator+=(const TimeSpan& other) {
		if (this != &other) _timePoint += other._timePoint.time_since_epoch();
		return *this;
	};
	TimeSpan operator+(int seconds) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() + __Seconds{ seconds }));
	};
	TimeSpan& operator+=(int seconds) {
		_timePoint += __Seconds{ seconds };
		return *this;
	};

	// Subtraction operator
	TimeSpan operator-(const TimeSpan& other) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() - other._timePoint.time_since_epoch()));
	};
	TimeSpan& operator-=(const TimeSpan& other) {
		if (this != &other) _timePoint -= other._timePoint.time_since_epoch();
		return *this;
	};
	TimeSpan operator-(int seconds) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() - __Seconds{ seconds }));
	};
	TimeSpan& operator-=(int seconds) {
		_timePoint -= __Seconds{ seconds };
		return *this;
	};

	// Equality operator
	constexpr bool operator==(const TimeSpan& other) const noexcept {
		return _timePoint == other._timePoint;
	};
	constexpr bool operator==(int64_t seconds) const noexcept {
		return TotalSeconds() == seconds;
	};

	// Inequality operator
	constexpr bool operator!=(const TimeSpan& other) const noexcept {
		return _timePoint != other._timePoint;
	};
	constexpr bool operator!=(int64_t seconds) const noexcept {
		return TotalSeconds() != seconds;
	};

	// Comparison operator
	constexpr bool operator<(const TimeSpan& other) const noexcept {
		return _timePoint < other._timePoint;
	};
	constexpr bool operator<(int64_t seconds) const noexcept {
		return TotalSeconds() < seconds;
	};
	constexpr bool operator>(const TimeSpan& other) const noexcept {
		return _timePoint > other._timePoint;
	};
	constexpr bool operator>(int64_t seconds) const noexcept {
		return TotalSeconds() > seconds;
	};
	constexpr bool operator<=(const TimeSpan& other) const noexcept {
		return _timePoint <= other._timePoint;
	};
	constexpr bool operator<=(int64_t seconds) const noexcept {
		return TotalSeconds() <= seconds;
	};
	constexpr bool operator>=(const TimeSpan& other) const noexcept {
		return _timePoint >= other._timePoint;
	};
	constexpr bool operator>=(int64_t seconds) const noexcept {
		return TotalSeconds() >= seconds;
	};

	// Prefix increment operator (increase by 1 second)
	TimeSpan& operator++() noexcept {
		_timePoint += __Seconds{1};
		return *this;
	};

	// Prefix decrement operator (decrease by 1 second)
	TimeSpan& operator--() noexcept {
		_timePoint -= __Seconds{1};
		return *this;
	};

	// Postfix increment operator (increase by 1 second)
	TimeSpan& operator++(int) noexcept {
		TimeSpan _original = *this;
		_timePoint += __Seconds{1};
		return _original;
	};

	// Postfix decrement operator (decrease by 1 second)
	TimeSpan& operator--(int) noexcept {
		TimeSpan _original = *this;
		_timePoint -= __Seconds{1};
		return _original;
	};

public:
	// Total durations
	constexpr int64_t TotalHours() const noexcept {
		return std::chrono::duration_cast<__Hours>(_timePoint.time_since_epoch()).count();
	};
	constexpr int64_t TotalMinutes() const noexcept {
		return std::chrono::duration_cast<__Minutes>(_timePoint.time_since_epoch()).count();
	};
	constexpr int64_t TotalSeconds() const noexcept {
		return std::chrono::duration_cast<__Seconds>(_timePoint.time_since_epoch()).count();
	};
	constexpr int64_t TotalMilliseconds() const noexcept {
		return std::chrono::duration_cast<__Milliseconds>(_timePoint.time_since_epoch()).count();
	};

public:
	// Access data
	constexpr int Days() const noexcept {
		return std::chrono::duration_cast<__Days>(_timePoint.time_since_epoch()).count();
	};
	constexpr int Hours() const noexcept {
		return static_cast<int>(TotalHours() % 24);
	};
	constexpr int Minutes() const noexcept {
		return static_cast<int>(TotalMinutes() % 60);
	};
	constexpr int Seconds() const noexcept {
		return static_cast<int>(TotalSeconds() % 60);
	};
	constexpr int Milliseconds() const noexcept {
		return static_cast<int>(TotalMilliseconds() % 1000);
	};

public:
	// Arithmetic
	TimeSpan& IncreaseDays(int days) noexcept {
		_timePoint += __Days{ days };
		return *this;
	};
	TimeSpan& DecreaseDays(int days) noexcept {
		_timePoint -= __Days{ days };
		return *this;
	};
	TimeSpan& IncreaseHours(int hours) noexcept {
		_timePoint += __Hours{ hours };
		return *this;
	};
	TimeSpan& DecreaseHours(int hours) noexcept {
		_timePoint -= __Hours{ hours };
		return *this;
	};
	TimeSpan& IncreaseMinutes(int mins) noexcept {
		_timePoint += __Minutes{ mins };
		return *this;
	};
	TimeSpan& DecreaseMinutes(int mins) noexcept {
		_timePoint -= __Minutes{ mins };
		return *this;
	};
	TimeSpan& IncreaseSeconds(int secs) noexcept {
		_timePoint += __Seconds{ secs };
		return *this;
	};
	TimeSpan& DecreaseSeconds(int secs) noexcept {
		_timePoint -= __Seconds{ secs };
		return *this;
	};
	TimeSpan& IncreaseMillisecs(int millisecs) noexcept {
		_timePoint += __Milliseconds{ millisecs };
		return *this;
	};
	TimeSpan& DecreaseMillisecs(int millisecs) noexcept {
		_timePoint -= __Milliseconds{ millisecs };
		return *this;
	};
};


// Manage Clock-time-only data
class ClockTime : public __TimeBase
{
public:
	// Construction
	explicit ClockTime() = default;

	// Copy constructor
	ClockTime(const ClockTime& other) : __TimeBase(other._timePoint) {};

	// Copy assignment operator
	ClockTime& operator=(const ClockTime& other) {
		if (this != &other) {
			_timePoint = other._timePoint;
		}
		return *this;
	};

	// Move constructor
	ClockTime(ClockTime&& other) noexcept : __TimeBase(std::move(other._timePoint)) {};

	// Move assignment operator
	ClockTime& operator=(ClockTime&& other) noexcept {
		if (this != &other) {
			_timePoint = std::move(other._timePoint);
		}
		return *this;
	};

public:
	// Conversion constructor
	ClockTime(__TimePoint timepoint) {
		_timePoint = std::chrono::sys_time{ timepoint - std::chrono::floor<__Days>(timepoint) };
	};
	ClockTime(int hour, int min, int sec, int millisec = 0) noexcept {
		auto _timeVal = __Hours(hour) + __Minutes(min) + __Seconds(sec) + __Milliseconds(millisec);
		_timePoint = std::chrono::sys_time{ _timeVal };
	};

public:
	// Get time point (conversion to std::chrono::system_clock::time_point)
	__TimePoint GetTimePoint(void) const noexcept override {
		return _timePoint;
	};

private:
	// Internal getters
	constexpr __Hours _getHours() const noexcept {
		return std::chrono::duration_cast<__Hours>(_timePoint.time_since_epoch() % __Days(1));
	};
	constexpr __Minutes _getMinutes() const noexcept {
		return std::chrono::duration_cast<__Minutes>(_timePoint.time_since_epoch() % __Hours(1));
	};
	constexpr __Seconds _getSeconds() const noexcept {
		return std::chrono::duration_cast<__Seconds>(_timePoint.time_since_epoch() % __Minutes(1));
	};
	constexpr __Milliseconds _getMillisecs() const noexcept {
		return std::chrono::duration_cast<__Milliseconds>(_timePoint.time_since_epoch() % __Seconds(1));
	};
	constexpr __Hours _toHours() const noexcept {
		return std::chrono::floor<__Hours>(_timePoint - std::chrono::floor<__Days>(_timePoint));
	};
	constexpr __Minutes _toMinutes() const noexcept {
		return std::chrono::floor<__Minutes>(_timePoint - std::chrono::floor<__Days>(_timePoint));
	};
	constexpr __Seconds _toSeconds() const noexcept {
		return std::chrono::floor<__Seconds>(_timePoint - std::chrono::floor<__Days>(_timePoint));
	};
	constexpr __Milliseconds _toMillisecs() const noexcept {
		return std::chrono::floor<__Milliseconds>(_timePoint - std::chrono::floor<__Days>(_timePoint));
	};

public:
	// Addition operator
	TimeSpan operator+(const ClockTime& other) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() + other._timePoint.time_since_epoch()));
	};
	ClockTime& operator+=(const ClockTime& other) {
		if (this != &other) _timePoint += other._timePoint.time_since_epoch();
		return *this;
	};
	TimeSpan operator+(const TimeSpan& timeSpan) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() + timeSpan.GetTimePoint().time_since_epoch()));
	};
	ClockTime& operator+=(const TimeSpan& timeSpan) {
		_timePoint += timeSpan.GetTimePoint().time_since_epoch();
		return *this;
	};
	TimeSpan operator+(int seconds) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() + __Seconds{ seconds }));
	};
	ClockTime& operator+=(int seconds) {
		_timePoint += __Seconds{ seconds };
		return *this;
	};

	// Subtraction operator
	TimeSpan operator-(const ClockTime& other) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() - other._timePoint.time_since_epoch()));
	};
	ClockTime& operator-=(const ClockTime& other) {
		if (this != &other) _timePoint -= other._timePoint.time_since_epoch();
		return *this;
	};
	TimeSpan operator-(const TimeSpan& timeSpan) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() - timeSpan.GetTimePoint().time_since_epoch()));
	};
	ClockTime& operator-=(const TimeSpan& timeSpan) {
		_timePoint -= timeSpan.GetTimePoint().time_since_epoch();
		return *this;
	};
	TimeSpan operator-(int seconds) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() - __Seconds{ seconds }));
	};
	ClockTime& operator-=(int seconds) {
		_timePoint -= __Seconds{ seconds };
		return *this;
	};

	// Equality operator
	constexpr bool operator==(const ClockTime& other) const noexcept {
		return _timePoint == other._timePoint;
	};

	// Inequality operator
	constexpr bool operator!=(const ClockTime& other) const noexcept {
		return _timePoint != other._timePoint;
	};

	// Comparison operator
	constexpr bool operator<(const ClockTime& other) const noexcept {
		return _timePoint < other._timePoint;
	};
	constexpr bool operator>(const ClockTime& other) const noexcept {
		return _timePoint > other._timePoint;
	};
	constexpr bool operator<=(const ClockTime& other) const noexcept {
		return _timePoint <= other._timePoint;
	};
	constexpr bool operator>=(const ClockTime& other) const noexcept {
		return _timePoint >= other._timePoint;
	};

	// Prefix increment operator (increase by 1 second)
	ClockTime& operator++() noexcept {
		_timePoint += __Seconds{ 1 };
		return *this;
	};

	// Prefix decrement operator (decrease by 1 second)
	ClockTime& operator--() noexcept {
		_timePoint -= __Seconds{ 1 };
		return *this;
	};

	// Postfix increment operator (increase by 1 second)
	ClockTime& operator++(int) noexcept {
		ClockTime _original = *this;
		_timePoint += __Seconds{ 1 };
		return _original;
	};

	// Postfix decrement operator (decrease by 1 second)
	ClockTime& operator--(int) noexcept {
		ClockTime _original = *this;
		_timePoint -= __Seconds{ 1 };
		return _original;
	};

public:
	// Access data
	constexpr int Hour(void) const noexcept {
		return _getHours().count();
	};
	constexpr int Minute(void) const noexcept {
		return _getMinutes().count();
	};
	constexpr int Second(void) const noexcept {
		return static_cast<int>(_getSeconds().count());
	};
	constexpr int Millisecond(void) const noexcept {
		return static_cast<int>(_getMillisecs().count());
	};

	// Modify data
	void SetHour(int hour) noexcept {
		ClockTime _original = *this;
		_timePoint = std::chrono::sys_time{ __Hours(hour) + _original._getMinutes() + _original._getSeconds() + _original._getMillisecs() };
	};
	void SetMinute(int minute) noexcept {
		ClockTime _original = *this;
		_timePoint = std::chrono::sys_time{ _original._getHours() + __Minutes(minute) + _original._getSeconds() + _original._getMillisecs() };
	};
	void SetSecond(int second) noexcept {
		ClockTime _original = *this;
		_timePoint = std::chrono::sys_time{ _original._getHours() + _original._getMinutes() + __Seconds(second) + _original._getMillisecs() };
	};
	void SetMillisecs(int millisecs) noexcept {
		ClockTime _original = *this;
		_timePoint = std::chrono::sys_time{ _original._getHours() + _original._getMinutes() + _original._getSeconds() + __Milliseconds(millisecs) };
	};

public:
	// Special validations
	constexpr bool IsValidTime(int hour, int minute, int second, int millisecs = 0) const noexcept {
		return (_isValidHour(hour) && _isValidMinute(minute) && _isValidSecond(second) && _isValidMillisecs(millisecs));
	};

public:
	// Arithmetic
	ClockTime& IncreaseHours(int hours) noexcept {
		_timePoint += __Hours{ hours };
		return *this;
	};
	ClockTime& DecreaseHours(int hours) noexcept {
		_timePoint -= __Hours{ hours };
		return *this;
	};
	ClockTime& IncreaseMinutes(int mins) noexcept {
		_timePoint += __Minutes{ mins };
		return *this;
	};
	ClockTime& DecreaseMinutes(int mins) noexcept {
		_timePoint -= __Minutes{ mins };
		return *this;
	};
	ClockTime& IncreaseSeconds(int secs) noexcept {
		_timePoint += __Seconds{ secs };
		return *this;
	};
	ClockTime& DecreaseSeconds(int secs) noexcept {
		_timePoint -= __Seconds{ secs };
		return *this;
	};
	ClockTime& IncreaseMillisecs(int millisecs) noexcept {
		_timePoint += __Milliseconds{ millisecs };
		return *this;
	};
	ClockTime& DecreaseMillisecs(int millisecs) noexcept {
		_timePoint -= __Milliseconds{ millisecs };
		return *this;
	};

	// Comparison
	int CompareToHours(const ClockTime& other) const noexcept {
		return static_cast<int>((_toHours() - other._toHours()).count());
	};
	int CompareToMinutes(const ClockTime& other) const noexcept {
		return static_cast<int>((_toMinutes() - other._toMinutes()).count());
	};
	int CompareToSeconds(const ClockTime& other) const noexcept {
		return static_cast<int>((_toSeconds() - other._toSeconds()).count());
	};
	int CompareToMillisecs(const ClockTime& other) const noexcept {
		return static_cast<int>((_toMillisecs() - other._toMillisecs()).count());
	};
};


// Manage Date/Time data
class DateTime : public __TimeBase
{
public:
	enum __DayOfWeek { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };

public:
	// Construction
	explicit DateTime() = default;

	// Copy constructor
	DateTime(const DateTime& other) : __TimeBase(other._timePoint) {};

	// Copy assignment operator
	DateTime& operator=(const DateTime& other) {
		if (this != &other) {
			_timePoint = other._timePoint;
		}
		return *this;
	};

	// Move constructor
	DateTime(DateTime&& other) noexcept : __TimeBase(std::move(other._timePoint)) {};

	// Move assignment operator
	DateTime& operator=(DateTime&& other) noexcept {
		if (this != &other) {
			_timePoint = std::move(other._timePoint);
		}
		return *this;
	};

public:
	// Conversion constructor
	DateTime(__TimePoint timepoint) : __TimeBase(timepoint) {};
	DateTime(int year, unsigned int month, unsigned int day, int hour, int min, int sec, int millisec = 0) noexcept {
		auto _dateVal = __Date{ __Year{ year }, __Month{ month }, __Day{ day } };
		auto _timeVal = __Hours(hour) + __Minutes(min) + __Seconds(sec) + __Milliseconds(millisec);
		_timePoint = std::chrono::sys_days{ _dateVal } + _timeVal;
	};

public:
	// Get time point (conversion to std::chrono::system_clock::time_point)
	__TimePoint GetTimePoint(void) const noexcept override {
		return _timePoint;
	};

	// Get clock-time data
	ClockTime GetClockTime(void) const noexcept {
		return ClockTime(Hour(), Minute(), Second(), Millisecond());
	};

public:
	// Addition operator
	TimeSpan operator+(const DateTime& other) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() + other._timePoint.time_since_epoch()));
	};
	DateTime& operator+=(const DateTime& other) {
		if (this != &other) _timePoint += other._timePoint.time_since_epoch();
		return *this;
	};
	TimeSpan operator+(const TimeSpan& timeSpan) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() + timeSpan.GetTimePoint().time_since_epoch()));
	};
	DateTime& operator+=(const TimeSpan& timeSpan) {
		_timePoint += timeSpan.GetTimePoint().time_since_epoch();
		return *this;
	};
	TimeSpan operator+(int seconds) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() + __Seconds{ seconds }));
	};
	DateTime& operator+=(int seconds) {
		_timePoint += __Seconds{ seconds };
		return *this;
	};

	// Subtraction operator
	TimeSpan operator-(const DateTime& other) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() - other._timePoint.time_since_epoch()));
	};
	DateTime& operator-=(const DateTime& other) {
		if (this != &other) _timePoint -= other._timePoint.time_since_epoch();
		return *this;
	};
	TimeSpan operator-(const TimeSpan& timeSpan) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() - timeSpan.GetTimePoint().time_since_epoch()));
	};
	DateTime& operator-=(const TimeSpan& timeSpan) {
		_timePoint -= timeSpan.GetTimePoint().time_since_epoch();
		return *this;
	};
	TimeSpan operator-(int seconds) const noexcept {
		return TimeSpan(__TimePoint(_timePoint.time_since_epoch() - __Seconds{ seconds }));
	};
	DateTime& operator-=(int seconds) {
		_timePoint -= __Seconds{ seconds };
		return *this;
	};

	// Equality operator
	constexpr bool operator==(const DateTime& other) const noexcept {
		return _timePoint == other._timePoint;
	};

	// Inequality operator
	constexpr bool operator!=(const DateTime& other) const noexcept {
		return _timePoint != other._timePoint;
	};

	// Comparison operator
	constexpr bool operator<(const DateTime& other) const noexcept {
		return _timePoint < other._timePoint;
	};
	constexpr bool operator>(const DateTime& other) const noexcept {
		return _timePoint > other._timePoint;
	};
	constexpr bool operator<=(const DateTime& other) const noexcept {
		return _timePoint <= other._timePoint;
	};
	constexpr bool operator>=(const DateTime& other) const noexcept {
		return _timePoint >= other._timePoint;
	};

	// Prefix increment operator (increase by 1 second)
	DateTime& operator++() noexcept {
		_timePoint += __Seconds{ 1 };
		return *this;
	};

	// Prefix decrement operator (decrease by 1 second)
	DateTime& operator--() noexcept {
		_timePoint -= __Seconds{ 1 };
		return *this;
	};

	// Postfix increment operator (increase by 1 second)
	DateTime& operator++(int) noexcept {
		DateTime _original = *this;
		_timePoint += __Seconds{ 1 };
		return _original;
	};

	// Postfix decrement operator (decrease by 1 second)
	DateTime& operator--(int) noexcept {
		DateTime _original = *this;
		_timePoint -= __Seconds{ 1 };
		return _original;
	};

private:
	// Internal getters
	constexpr __Date _dateVal() const noexcept {
		return __Date{ std::chrono::floor<__Days>(_timePoint) };
	};
	constexpr __Year _yearVal() const noexcept {
		return __Date{ std::chrono::floor<__Days>(_timePoint) }.year();
	};
	constexpr __Month _monthVal() const noexcept {
		return __Date{ std::chrono::floor<__Days>(_timePoint) }.month();
	};
	constexpr __Day _dayVal() const noexcept {
		return __Date{ std::chrono::floor<__Days>(_timePoint) }.day();
	};
	constexpr __Weekday _weekdayVal() const noexcept {
		return __Weekday{ std::chrono::floor<__Days>(_timePoint) };
	};
	constexpr __Hours _getHours() const noexcept {
		return std::chrono::duration_cast<__Hours>(_timePoint.time_since_epoch() % __Days(1));
	};
	constexpr __Minutes _getMinutes() const noexcept {
		return std::chrono::duration_cast<__Minutes>(_timePoint.time_since_epoch() % __Hours(1));
	};
	constexpr __Seconds _getSeconds() const noexcept {
		return std::chrono::duration_cast<__Seconds>(_timePoint.time_since_epoch() % __Minutes(1));
	};
	constexpr __Milliseconds _getMillisecs() const noexcept {
		return std::chrono::duration_cast<__Milliseconds>(_timePoint.time_since_epoch() % __Seconds(1));
	};

public:
	// Access data
	constexpr int Year(void) const noexcept {
		return static_cast<int>(_yearVal());
	};
	constexpr unsigned Month(void) const noexcept {
		return static_cast<unsigned>(_monthVal());
	};
	constexpr unsigned Day(void) const noexcept {
		return static_cast<unsigned>(_dayVal());
	};
	constexpr int Hour(void) const noexcept {
		return _getHours().count();
	};
	constexpr int Minute(void) const noexcept {
		return _getMinutes().count();
	};
	constexpr int Second(void) const noexcept {
		return static_cast<int>(_getSeconds().count());
	};
	constexpr int Millisecond(void) const noexcept {
		return static_cast<int>(_getMillisecs().count());
	};
	constexpr int DayOfWeek(void) const noexcept {
		return static_cast<unsigned>(_weekdayVal().c_encoding());
	};

	// Modify data
	void SetDate(int year, unsigned int month, unsigned int day) noexcept {
		DateTime _original = *this;
		auto _newDateVal = __Date{ __Year(year), __Month(month), __Day(day) };
		_timePoint = std::chrono::sys_days{ _newDateVal } + (_original._timePoint - std::chrono::sys_days{ _original._dateVal() });
	};
	void SetClockTime(int hour, int minute, int second, int millisecs = 0) noexcept {
		DateTime _original = *this;
		auto _newTimeVal = __Hours(hour) + __Minutes(minute) + __Seconds(second) + __Milliseconds(millisecs);
		_timePoint = std::chrono::sys_days{ _original._dateVal() } + _newTimeVal;
	};
	void SetClockTime(const ClockTime& clockTime) noexcept {
		DateTime _original = *this;
		auto _newTimeVal = clockTime.GetTimePoint() - std::chrono::floor<__Days>(clockTime.GetTimePoint());
		_timePoint = std::chrono::sys_days{ _original._dateVal() } + _newTimeVal;
	};
	void SetYear(int year) noexcept {
		DateTime _original = *this;
		auto _newDateVal = __Date{ __Year(year), _original._monthVal(), _original._dayVal() };
		_timePoint = std::chrono::sys_days{ _newDateVal } + (_original._timePoint - std::chrono::sys_days{ _original._dateVal() });
	};
	void SetMonth(int month) noexcept {
		DateTime _original = *this;
		auto _newDateVal = __Date{ _original._yearVal(), __Month(month), _original._dayVal() };
		_timePoint = std::chrono::sys_days{ _newDateVal } + (_original._timePoint - std::chrono::sys_days{ _original._dateVal() });
	};
	void SetDay(int day) noexcept {
		DateTime _original = *this;
		auto _newDateVal = __Date{ _original._yearVal(), _original._monthVal(), __Day(day) };
		_timePoint = std::chrono::sys_days{ _newDateVal } + (_original._timePoint - std::chrono::sys_days{ _original._dateVal() });
	};
	void SetHour(int hour) noexcept {
		DateTime _original = *this;
		auto _newTimeVal = __Hours(hour) + _original._getMinutes() + _original._getSeconds() + _original._getMillisecs();
		_timePoint = std::chrono::sys_days{ _original._dateVal() } + _newTimeVal;
	};
	void SetMinute(int minute) noexcept {
		DateTime _original = *this;
		auto _newTimeVal = _original._getHours() + __Minutes(minute) + _original._getSeconds() + _original._getMillisecs();
		_timePoint = std::chrono::sys_days{ _original._dateVal() } + _newTimeVal;
	};
	void SetSecond(int second) noexcept {
		DateTime _original = *this;
		auto _newTimeVal = _original._getHours() + _original._getMinutes() + __Seconds(second) + _original._getMillisecs();
		_timePoint = std::chrono::sys_days{ _original._dateVal() } + _newTimeVal;
	};
	void SetMillisecs(int millisecs) noexcept {
		DateTime _original = *this;
		auto _newTimeVal = _original._getHours() + _original._getMinutes() + _original._getSeconds() + __Milliseconds(millisecs);
		_timePoint = std::chrono::sys_days{ _original._dateVal() } + _newTimeVal;
	};

public:
	// Special validations
	constexpr bool IsLeapYear(void) const noexcept {
		return _yearVal().is_leap();
	};
	constexpr bool IsValidDate(int year, unsigned int month, unsigned int day) const noexcept {
		return __Date{ __Year(year), __Month(month), __Day(day) }.ok();
	};
	constexpr bool IsValidTime(int hour, int minute, int second, int millisecs = 0) const noexcept {
		return (_isValidHour(hour) && _isValidMinute(minute) && _isValidSecond(second) && _isValidMillisecs(millisecs));
	};
	constexpr bool IsToday(void) const noexcept {
		return _dateVal() == DateTime(std::chrono::system_clock::now())._dateVal();
	};
	constexpr bool IsInTheFuture(void) const noexcept {
		return _timePoint > std::chrono::system_clock::now();
	};
	constexpr bool IsInThePast(void) const noexcept {
		return _timePoint < std::chrono::system_clock::now();
	};

public:
	// Arithmetic
	DateTime& IncreaseDays(int days) noexcept {
		_timePoint += __Days{ days };
		return *this;
	};
	DateTime& DecreaseDays(int days) noexcept {
		_timePoint -= __Days{ days };
		return *this;
	};
	DateTime& IncreaseHours(int hours) noexcept {
		_timePoint += __Hours{ hours };
		return *this;
	};
	DateTime& DecreaseHours(int hours) noexcept {
		_timePoint -= __Hours{ hours };
		return *this;
	};
	DateTime& IncreaseMinutes(int mins) noexcept {
		_timePoint += __Minutes{ mins };
		return *this;
	};
	DateTime& DecreaseMinutes(int mins) noexcept {
		_timePoint -= __Minutes{ mins };
		return *this;
	};
	DateTime& IncreaseSeconds(int secs) noexcept {
		_timePoint += __Seconds{ secs };
		return *this;
	};
	DateTime& DecreaseSeconds(int secs) noexcept {
		_timePoint -= __Seconds{ secs };
		return *this;
	};
	DateTime& IncreaseMillisecs(int millisecs) noexcept {
		_timePoint += __Milliseconds{ millisecs };
		return *this;
	};
	DateTime& DecreaseMillisecs(int millisecs) noexcept {
		_timePoint -= __Milliseconds{ millisecs };
		return *this;
	};
};

// For global usage
using DayOfWeek = DateTime::__DayOfWeek;
