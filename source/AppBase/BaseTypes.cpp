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

// Using PI constants
#ifndef M_PI
	#ifdef _HAS_CXX20
		#if !defined(_NUMBERS_)
			#include <numbers>
		#endif
		constexpr double M_PI = std::numbers::pi;
		constexpr double M_PI_2 = std::numbers::pi / 2;
		constexpr double M_PI_4 = std::numbers::pi / 4;
	#else
		#define M_PI       3.14159265358979323846   // pi
		#define M_PI_2     1.57079632679489661923   // pi/2
		#define M_PI_4     0.785398163397448309616  // pi/4
	#endif
#endif

// Rectangle rotation around center
Rect Rect::Rotate(Rotation rotation) const noexcept
{
	double _angleRadians = 0.0;

	switch (rotation) {
		case Rotation::Clockwise_90:         _angleRadians = -M_PI / 2.0;		break;
		case Rotation::Clockwise_180:        _angleRadians = -M_PI;				break;
		case Rotation::Clockwise_270:        _angleRadians = -3 * M_PI / 2.0;	break;
		case Rotation::CounterClockwise_90:  _angleRadians = M_PI / 2.0;		break;
		case Rotation::CounterClockwise_180: _angleRadians = M_PI;				break;
		case Rotation::CounterClockwise_270: _angleRadians = 3 * M_PI / 2.0;	break;
	}

	// Rotate around center
	return RotateAround(Center(), _angleRadians);
}

// Rotate point around a pivot
// Helper for Rect::RotatedAround() function
Vector2D RotatePointAround(const Vector2D& point, const Vector2D& pivot, double angle) noexcept
{
	double _sin = std::sin(angle);
	double _cos = std::cos(angle);

	// Translate to origin
	Vector2D _translated = point - pivot;

	// Rotate
	double _xNew = _translated._x * _cos - _translated._y * _sin;
	double _yNew = _translated._x * _sin + _translated._y * _cos;

	// Translate back
	return Vector2D(_xNew + pivot._x, _yNew + pivot._y);
}

// Rectangle rotation around a pivot (any point)
Rect Rect::RotateAround(const Vector2D& pivot, double angleRadians) const noexcept
{
	// Get the 4 corners (respecting inversion)
	std::array<Vector2D, 4> _corners = {
		Vector2D(_leftVal(),  _topVal()),
		Vector2D(_rightVal(), _topVal()),
		Vector2D(_rightVal(), _bottomVal()),
		Vector2D(_leftVal(),  _bottomVal())
	};

	// Rotate each corner around the pivot
	for (auto& _point : _corners)
		_point = RotatePointAround(_point, pivot, angleRadians);

	// Compute the new bounding box
	double _minX = _corners[0]._x, _maxX = _corners[0]._x;
	double _minY = _corners[0]._y, _maxY = _corners[0]._y;
	for (int i = 1; i < 4; ++i) {
		_minX = std::min(_minX, _corners[i]._x);
		_maxX = std::max(_maxX, _corners[i]._x);
		_minY = std::min(_minY, _corners[i]._y);
		_maxY = std::max(_maxY, _corners[i]._y);
	}

	return Rect(_minX, _minY, _maxX, _maxY);
}
