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


// Using math for calculations
#if !defined(_CMATH_)
	#define _USE_MATH_DEFINES
	#include <cmath>
#endif


// Using std::array
#if !defined(_ARRAY_)
	#include <array>
#endif


// For throwing exception
#if !defined(_STDEXCEPT_) || !defined(_EXCEPTION_)
	#include <stdexcept>
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


// Manage point/coordinates
class Point final
{
public:
	double _x = 0;
	double _y = 0;

public:
	// Construction
	constexpr explicit Point() noexcept = default;
	constexpr Point(double x, double y) noexcept : _x(x), _y(y) {};

	// Copy constructor
	constexpr Point(const Point& other) = default;

	// Copy assignment operator
	Point& operator=(const Point& other) = default;

	// Move constructor
	constexpr Point(Point&& other) noexcept = default;

	// Move assignment operator
	Point& operator=(Point&& other) noexcept = default;

public:
	// Access data
	constexpr double GetX(void) const noexcept { return _x; };
	void SetX(double x) noexcept { _x = x; };
	constexpr double GetY(void) const noexcept { return _y; };
	void SetY(double y) noexcept { _y = y; };

public:
	// For floating-point precision comparisons
	static constexpr double EPSILON = 1e-9;
	static bool NearlyEqual(double a, double b, double epsilon = EPSILON) {
		return std::abs(a - b) < epsilon;
	};

	// Arithmetic Operators
	constexpr Point operator+(const Point& other) const noexcept {
		return Point(_x + other._x, _y + other._y);
	};
	constexpr Point operator-(const Point& other) const noexcept {
		return Point(_x - other._x, _y - other._y);
	};
	constexpr Point operator*(double scalar) const noexcept {
		return Point(_x * scalar, _y * scalar);
	};
	Point operator/(double scalar) const {
		if (!NearlyEqual(scalar, 0)) return Point(_x / scalar, _y / scalar);
		throw std::invalid_argument("Division by zero in Point::operator/");
	};

	// Compound Assignment Operators
	Point& operator+=(const Point& other) noexcept {
		if (this != &other) { _x += other._x; _y += other._y; }
		return *this;
	};
	Point& operator-=(const Point& other) noexcept {
		if (this != &other) { _x -= other._x; _y -= other._y; }
		return *this;
	};
	Point& operator*=(double scalar) noexcept {
		_x *= scalar; _y *= scalar;
		return *this;
	};
	Point& operator/=(double scalar) {
		if (!NearlyEqual(scalar, 0)) { _x /= scalar; _y /= scalar; return *this; }
		throw std::invalid_argument("Division by zero in Point::operator/=");
	};

	// Comparison Operators
	bool operator==(const Point& other) const noexcept {
		return NearlyEqual(_x, other._x) && NearlyEqual(_y, other._y);
	};
	bool operator!=(const Point& other) const noexcept {
		return !(*this == other);
	};
	bool operator<(const Point& other) const noexcept {
		return (_x < other._x) || (NearlyEqual(_x, other._x) && _y < other._y);
	};
	bool operator<=(const Point& other) const noexcept {
		return (*this < other) || (*this == other);
	};
	bool operator>(const Point& other) const noexcept {
		return (_x > other._x) || (NearlyEqual(_x, other._x) && _y > other._y);
	};
	bool operator>=(const Point& other) const noexcept {
		return !(*this < other);
	};

	// Stream Operators
	friend std::ostream& operator<<(std::ostream& os, const Point& pt) {
		return os << "(" << pt._x << ", " << pt._y << ")";
	};
	friend std::wostream& operator<<(std::wostream& wos, const Point& pt) {
		return wos << L"(" << pt._x << L", " << pt._y << L")";
	};

// Geometric/Vector Math Operations
public:
	// Distance between points
	double DistanceTo(const Point& other) const noexcept {
		double dx = _x - other._x; double dy = _y - other._y;
		return std::sqrt(dx * dx + dy * dy);
	};

	// Length / magnitude (treats the point as a vector from the origin (0, 0))
	double Magnitude(void) const noexcept {
		return std::sqrt(_x * _x + _y * _y);
	};

	// Dot products
	constexpr double Dot(const Point& other) const noexcept {
		return _x * other._x + _y * other._y;
	};

	// Cross product (2D)
	// Used for determining orientation, area of parallelogram, etc.
	constexpr double Cross(const Point& other) const noexcept {
		return _x * other._y - _y * other._x;
	};

	// Angle between two vectors
	double AngleWith(const Point& other) const noexcept {
		double _dotProd = this->Dot(other);
		double _magnitudes = this->Magnitude() * other.Magnitude();
		return std::acos(_dotProd / _magnitudes); // in radians
	}

// Utility Operations
public:
	// Normalization (unit vector)
	Point Normalized() const noexcept {
		double _magitude = this->Magnitude();
		return (_magitude == 0) ? Point(0, 0) : Point(_x / _magitude, _y / _magitude);
	};

	// Midpoint between two points
	Point Midpoint(const Point& other) const noexcept {
		return Point((_x + other._x) / 2, (_y + other._y) / 2);
	};

	// Manhattan distance
	double ManhattanDistanceTo(const Point& other) const noexcept {
		return std::abs(_x - other._x) + std::abs(_y - other._y);
	};
};


// Manage Vector2D (line)
class Vector2D
{
public:
	double _x = 0;
	double _y = 0;

public:
	// Construction
	constexpr explicit Vector2D() noexcept = default;
	constexpr Vector2D(double x, double y) noexcept : _x(x), _y(y) {};

	// Copy constructor
	constexpr Vector2D(const Vector2D& other) = default;

	// Copy assignment operator
	Vector2D& operator=(const Vector2D& other) = default;

	// Move constructor
	constexpr Vector2D(Vector2D&& other) noexcept = default;

	// Move assignment operator
	Vector2D& operator=(Vector2D&& other) noexcept = default;

public:
	// Access data
	constexpr double GetX(void) const noexcept { return _x; };
	void SetX(double x) noexcept { _x = x; };
	constexpr double GetY(void) const noexcept { return _y; };
	void SetY(double y) noexcept { _y = y; };
	Point GetPoint(void) const noexcept {
		return Point(_x, _y);
	};
	void SetPoint(const Point& point) noexcept {
		_x = point._x; _y = point._y;
	};

public:
	// For floating-point precision comparisons
	static constexpr double EPSILON = 1e-9;
	static bool NearlyEqual(double a, double b, double epsilon = EPSILON) {
		return std::abs(a - b) < epsilon;
	};

	// Arithmetic Operators
	constexpr Vector2D operator+(const Vector2D& other) const noexcept {
		return Vector2D(_x + other._x, _y + other._y);
	};
	constexpr Vector2D operator-(const Vector2D& other) const noexcept {
		return Vector2D(_x - other._x, _y - other._y);
	};
	constexpr Vector2D operator*(double scalar) const noexcept {
		return Vector2D(_x * scalar, _y * scalar);
	};
	Vector2D operator/(double scalar) const {
		if (!NearlyEqual(scalar, 0)) return Vector2D(_x / scalar, _y / scalar);
		throw std::invalid_argument("Division by zero in Vector2D::operator/");
	};

	// Compound Assignment Operators
	Vector2D& operator+=(const Vector2D& other) noexcept {
		if (this != &other) { _x += other._x; _y += other._y; }
		return *this;
	};
	Vector2D& operator-=(const Vector2D& other) noexcept {
		if (this != &other) { _x -= other._x; _y -= other._y; }
		return *this;
	};
	Vector2D& operator*=(double scalar) noexcept {
		_x *= scalar; _y *= scalar;
		return *this;
	};
	Vector2D& operator/=(double scalar) {
		if (!NearlyEqual(scalar, 0)) { _x /= scalar; _y /= scalar; return *this; }
		throw std::invalid_argument("Division by zero in Vector2D::operator/=");
	};

	// Comparison operators
	bool operator==(const Vector2D& other) const noexcept {
		return NearlyEqual(_x, other._x) && NearlyEqual(_y, other._y);
	};
	bool operator!=(const Vector2D& other) const noexcept {
		return !(*this == other);
	};
	bool operator<(const Vector2D& other) const noexcept {
		return (_x < other._x) || (NearlyEqual(_x, other._x) && _y < other._y);
	};
	bool operator<=(const Vector2D& other) const noexcept {
		return (*this < other) || (*this == other);
	};
	bool operator>(const Vector2D& other) const noexcept {
		return (_x > other._x) || (NearlyEqual(_x, other._x) && _y > other._y);
	};
	bool operator>=(const Vector2D& other) const noexcept {
		return !(*this < other);
	};

	// Stream Operators
	friend std::ostream& operator<<(std::ostream& os, const Vector2D& line) {
		return os << "(" << line._x << ", " << line._y << ")";
	};
	friend std::wostream& operator<<(std::wostream& wos, const Vector2D& line) {
		return wos << L"(" << line._x << L", " << line._y << L")";
	};

public:
	// Magnitude (Length)
	double Length() const noexcept {
		return std::sqrt(_x * _x + _y * _y);
	};

	// Squared length (no sqrt)
	constexpr double LengthSquared() const noexcept {
		return _x * _x + _y * _y;
	};

	// Normalized vector (unit length)
	Vector2D Normalized() const {
		double _length = Length();
		if (!NearlyEqual(_length, 0.0)) return Vector2D(_x / _length, _y / _length);
		throw std::runtime_error("Cannot normalize a zero-length vector");
	};

	// Dot product
	constexpr double Dot(const Vector2D& other) const noexcept {
		return _x * other._x + _y * other._y;
	};

	// Cross product (2D scalar cross)
	constexpr double Cross(const Vector2D& other) const noexcept {
		return _x * other._y - _y * other._x;
	};

	// Angle between vectors (in radians)
	double AngleTo(const Vector2D& other) const {
		double _dot = Dot(other);
		double _length1 = Length(); double _length2 = other.Length();
		if (NearlyEqual(_length1, 0.0) || NearlyEqual(_length2, 0.0))
			throw std::runtime_error("Cannot compute angle with zero-length vector");
		double _cosTheta = _dot / (_length1 * _length2);
		return std::acos(std::clamp(_cosTheta, -1.0, 1.0));
	};

	// Is zero
	bool IsZero() const noexcept {
		return NearlyEqual(_x, 0.0) && NearlyEqual(_y, 0.0);
	};
};


// Manage size
class Size
{
public:
	double _width = 0;
	double _height = 0;

public:
	// Construction
	constexpr Size() noexcept = default;
	constexpr Size(double width, double height) noexcept : _width(width), _height(height) {};

	// Copy constructor
	constexpr Size(const Size& other) = default;

	// Copy assignment operator
	Size& operator=(const Size& other) = default;

	// Move constructor
	constexpr Size(Size&& other) noexcept = default;

	// Move assignment operator
	Size& operator=(Size&& other) noexcept = default;

public:
	// Access data
	constexpr double Width(void) const noexcept { return _width; };
	void SetWidth(double width) noexcept { _width = width; };
	constexpr double Height(void) const noexcept { return _height; };
	void SetHeight(double height) noexcept { _height = height; };

public:
	// For floating-point precision comparisons
	static constexpr double EPSILON = 1e-9;
	static bool NearlyEqual(double a, double b, double epsilon = EPSILON) noexcept {
		return std::abs(a - b) < epsilon;
	};

	// Arithmetic with Size
	constexpr Size operator+(const Size& other) const noexcept {
		return Size(_width + other._width, _height + other._height);
	};
	constexpr Size& operator+=(const Size& other) noexcept {
		_width += other._width; _height += other._height;
		return *this;
	};
	constexpr Size operator-(const Size& other) const noexcept {
		return Size(_width - other._width, _height - other._height);
	};
	constexpr Size& operator-=(const Size& other) noexcept {
		_width -= other._width; _height -= other._height;
		return *this;
	};

	// Scaling with scalar
	constexpr Size operator*(double scalar) const noexcept {
		return Size(_width * scalar, _height * scalar);
	};
	constexpr Size& operator*=(double scalar) noexcept {
		_width *= scalar; _height *= scalar;
		return *this;
	};
	Size operator/(double scalar) const {
		if (!NearlyEqual(scalar, 0.0)) return Size(_width / scalar, _height / scalar);
		throw std::runtime_error("Division by zero in Size::operator/");
	};
	Size& operator/=(double scalar) {
		if (NearlyEqual(scalar, 0.0)) {	_width /= scalar; _height /= scalar; return *this; }
		throw std::runtime_error("Division by zero in Size::operator/=");
	};

	// Comparison operators
	bool operator==(const Size& other) const noexcept {
		return NearlyEqual(_width, other._width) && NearlyEqual(_height, other._height);
	};
	bool operator!=(const Size& other) const noexcept {
		return !(*this == other);
	};
	bool operator<(const Size& other) const noexcept {
		if (!NearlyEqual(_width, other._width)) return _width < other._width;
		return _height < other._height;
	};
	bool operator<=(const Size& other) const noexcept {
		return (*this < other) || (*this == other);
	};
	bool operator>(const Size& other) const noexcept {
		return (_width > other._width) || (NearlyEqual(_width, other._width) && _height > other._height);
	};
	bool operator>=(const Size& other) const noexcept {
		return !(*this < other);
	};

	// Stream Operators
	friend std::ostream& operator<<(std::ostream& os, const Size& size) {
		return os << "(" << size._width << ", " << size._height << ")";
	};
	friend std::wostream& operator<<(std::wostream& wos, const Size& size) {
		return wos << L"(" << size._width << L", " << size._height << L")";
	};

public:
	// Area
	double Area(void) const noexcept {
		return std::abs(_width * _height);
	};

	// Is zero/empty
	bool IsZero(void) const noexcept {
		return NearlyEqual(_width, 0.0) && NearlyEqual(_height, 0.0);
	};
	constexpr bool IsEmpty(void) const noexcept {
		return _width <= 0.0 || _height <= 0.0;
	};

	// Normalized size (positive width and height)
	Size Normalized(void) const noexcept {
		return Size(std::abs(_width), std::abs(_height));
	};

	// Conversion to Vector2D
	constexpr Vector2D ToVector() const noexcept {
		return Vector2D(_width, _height);
	};
};


// Manage rectangle
// Allowing inverted rectangles
class Rect
{
public:
	enum class Rotation {
		Clockwise_90, Clockwise_180, Clockwise_270,
		CounterClockwise_90, CounterClockwise_180, CounterClockwise_270,
	};

public:
	double _left = 0;
	double _top = 0;
	double _right = 0;
	double _bottom = 0;

public:
	// Construction
	constexpr explicit Rect() noexcept = default;
	constexpr Rect(double left, double top, double right, double bottom) : _left(left), _top(top), _right(right), _bottom(bottom) {};
	constexpr Rect(const Point& topLeft, const Point& bottomRight) : _left(topLeft._x), _top(topLeft._y), _right(bottomRight._x), _bottom(bottomRight._y) {};
	constexpr Rect(const Vector2D& position, const Size& size)
		: _left(position._x), _top(position._y), _right(position._x + size._width), _bottom(position._y + size._height) {};

	// Copy constructor
	constexpr Rect(const Rect& other) = default;

	// Copy assignment operator
	Rect& operator=(const Rect& other) = default;

	// Move constructor
	constexpr Rect(Rect&& other) noexcept = default;

	// Move assignment operator
	Rect& operator=(Rect&& other) noexcept = default;

public:
	// Create rectangle
	static constexpr Rect FromPositionSize(const Vector2D& position, const Size& size) {
		return Rect(position, size);
	};
	static constexpr Rect FromEdges(double left, double top, double right, double bottom) {
		return Rect(left, top, right, bottom);
	};
	static constexpr Rect FromEdges(const Point& topLeft, const Point& bottomRight) {
		return Rect(topLeft, bottomRight);
	};

public:
	// Access data
	constexpr double Left(void) const noexcept { return _left; };
	constexpr double Top(void) const noexcept { return _top; };
	constexpr double Right(void) const noexcept { return _right; };
	constexpr double Bottom(void) const noexcept { return _bottom; };

	Point TopLeft(void) const noexcept { return Point(_left, _top); };
	Point TopRight(void) const noexcept { return Point(_right, _top); };
	Point BottomLeft(void) const noexcept { return Point(_left, _bottom); };
	Point BottomRight(void) const noexcept { return Point(_right, _bottom); };

	// Modify data
	void SetLeft(double left) noexcept { _left = left; };
	void SetTop(double top) noexcept { _top = top; };
	void SetRight(double right) noexcept { _right = right; };
	void SetBottom(double bottom) noexcept { _bottom = bottom; };

	void SetTopLeft(const Point& topLeft) noexcept { _left = topLeft._x; _top = topLeft._y; };
	void SetTopLeft(double x, double y) noexcept { _left = x; _top = y; };
	void SetBottomRight(const Point& bottomRight) noexcept { _right = bottomRight._x; _bottom = bottomRight._y; };
	void SetBottomRight(double x, double y) noexcept { _right = x; _bottom = y; };

private:
	// Internal getters
	constexpr double _leftVal(void) const noexcept {
		return std::min(_left, _right);
	};
	constexpr double _rightVal(void) const noexcept {
		return std::max(_left, _right);
	};
	constexpr double _topVal(void) const noexcept {
		return std::min(_top, _bottom);
	};
	constexpr double _bottomVal(void) const noexcept {
		return std::max(_top, _bottom);
	};

public:
	// Comparison operators
	static constexpr double EPSILON = 1e-9;
	static bool NearlyEqual(double a, double b, double epsilon = EPSILON) {
		return std::abs(a - b) < epsilon;
	};
	bool operator==(const Rect& other) const noexcept {
		return NearlyEqual(_leftVal(), other._leftVal()) && NearlyEqual(_topVal(), other._topVal()) &&
			NearlyEqual(_rightVal(), other._rightVal()) && NearlyEqual(_bottomVal(), other._bottomVal());
	};
	bool operator!=(const Rect& other) const noexcept {
		return !(*this == other);
	};
	bool operator<(const Rect& other) const noexcept {
		if (!NearlyEqual(_leftVal(), other._leftVal())) return _leftVal() < other._leftVal();
		if (!NearlyEqual(_topVal(), other._topVal())) return _topVal() < other._topVal();
		if (!NearlyEqual(_rightVal(), other._rightVal())) return _rightVal() < other._rightVal();
		return _bottomVal() < other._bottomVal();
	};
	bool operator<=(const Rect& other) const noexcept {
		return (*this < other || *this == other);
	};
	bool operator>(const Rect& other) const noexcept {
		return !(*this <= other);
	};
	bool operator>=(const Rect& other) const noexcept {
		return !(*this < other);
	};

	// Arithmetic Operators
	Rect operator+(const Point& offset) const noexcept {
		return Rect(_left + offset._x, _top + offset._y, _right + offset._x, _bottom + offset._y);
	};
	Rect& operator+=(const Point& offset) noexcept {
		_left += offset._x; _top += offset._y; _right += offset._x; _bottom += offset._y;
		return *this;
	};
	Rect operator-(const Point& offset) const noexcept {
		return Rect(_left - offset._x, _top - offset._y, _right - offset._x, _bottom - offset._y);
	};
	Rect& operator-=(const Point& offset) noexcept {
		_left -= offset._x; _top -= offset._y;
		_right -= offset._x; _bottom -= offset._y;
		return *this;
	};
	Rect operator+(const Vector2D& offset) const noexcept {
		return Rect(_left + offset._x, _top + offset._y, _right + offset._x, _bottom + offset._y);
	};
	Rect& operator+=(const Vector2D& offset) noexcept {
		_left += offset._x; _top += offset._y; _right += offset._x; _bottom += offset._y;
		return *this;
	};

public:
	// Width and Height of the rectangle
	double Width(void) const noexcept {
		return std::abs(_right - _left);
	};
	double Height(void) const noexcept {
		return std::abs(_bottom - _top);
	};

	// Size of rectangle
	Size GetSize(void) const noexcept {
		return Size(Width(), Height());
	};

	// Position of rectangle
	constexpr Vector2D GetPosition() const noexcept {
		return Vector2D(_leftVal(), _topVal());
	};

	// Is a square
	bool IsSquare(void) const noexcept {
		return NearlyEqual(Width(), Height());
	};

	// Area of the rectangle
	double Area(void) const noexcept {
		return Width() * Height();
	}

	// Perimeter of the rectangle
	double Perimeter(void) const noexcept {
		return (Width() * 2.0 + Height() * 2.0);
	};

	// Center position of the rectangle
	constexpr Vector2D Center(void) const noexcept {
		return Vector2D((_leftVal() + _rightVal()) / 2.0, (_topVal() + _bottomVal()) / 2.0);
	};

	// Diagonal of the rectangle
	double Diagonal(void) const noexcept {
		return std::sqrt(Width() * Width() + Height() * Height());
	};

	// For inverted rectangles
	constexpr bool IsInverted(void) const noexcept {
		return (_right < _left || _bottom < _top);
	};
	Rect& Normalize(void) noexcept {
		if (_right < _left) std::swap(_right, _left);
		if (_bottom < _top) std::swap(_bottom, _top);
		return *this;
	};

	// Contains a point
	constexpr bool Contains(double x, double y) const noexcept {
		return x >= _leftVal() && x <= _rightVal() && y >= _topVal() && y <= _bottomVal();
	};
	constexpr bool Contains(const Point& point) const noexcept {
		return Contains(point._x, point._y);
	};

	// Intersects with another rectangle
	constexpr bool Intersects(const Rect& other) const noexcept {
		return !(_rightVal() < other._leftVal() || _leftVal() > other._rightVal() || _bottomVal() < other._topVal() || _topVal() > other._bottomVal());
	};

	// Intersection rectangle (returns empty Rect if no intersection)
	Rect Intersection(const Rect& other) const noexcept {
		if (!Intersects(other)) return Rect();
		return Rect(std::max(_leftVal(), other._leftVal()), std::max(_topVal(), other._topVal()),
			std::min(_rightVal(), other._rightVal()), std::min(_bottomVal(), other._bottomVal()));
	};

	// Union rectangle
	Rect Unite(const Rect& other) const noexcept {
		return Rect(std::min(_leftVal(), other._leftVal()), std::min(_topVal(), other._topVal()),
			std::max(_rightVal(), other._rightVal()), std::max(_bottomVal(), other._bottomVal()));
	};

	// Move/Offset the rectangle
	Rect& Offset(double deltaX, double deltaY = 0) noexcept {
		_left += deltaX; _right += deltaX; _top += deltaY; _bottom += deltaY;
		return *this;
	};
	Rect& Offset(const Point& point) noexcept {
		return Offset(point._x, point._y);
	};
	Rect& Offset(const Vector2D& vector) noexcept {
		return Offset(vector._x, vector._y);
	};

	// Empty the rectangle
	void Empty(void) noexcept {
		_left = _top = _right = _bottom = 0.0;
	};

	// Set new size (preserves whether it's inverted or not)
	void SetSize(const Size& newSize) noexcept {
		if (_right >= _left) { _right = _left + newSize._width; }
		else { _right = _left - newSize._width; }
		if (_bottom >= _top) { _bottom = _top + newSize._height; }
		else { _bottom = _top - newSize._height; }
	};

	// Flip horizontally: mirror across vertical axis
	Rect& FlipHorizontally() noexcept {
		std::swap(_left, _right);
		return *this;
	};

	// Flip vertically: mirror across horizontal axis
	Rect& FlipVertically() noexcept {
		std::swap(_top, _bottom);
		return *this;
	};

	// Rotation around center
	// This method only works like width/height swapping
	Rect Rotate(Rotation rotation) const noexcept;

	// Rotatition around center with an arbitary angle
	// This returns a new axis-aligned bounding rectangle,
	// which often has different width and height compared to the original rectangle
	Rect Rotate(double angleRadians) const noexcept {
		return RotateAround(Center(), angleRadians);
	};

	// Rotation around a pivot (any point)
	// This returns a new axis-aligned bounding rectangle,
	// which often has different width and height compared to the original rectangle
	Rect RotateAround(const Vector2D& pivot, double angleRadians) const noexcept;
};
