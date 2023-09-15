#pragma once

#include "Common.h"

namespace Text {
	bool isSymbolPrefix(char c);

	bool isSymbol(char c);

	bool isWhitespace(char c);

	bool isLowerCase(char c);

	bool isUpperCase(char c);

	bool isAlphabetic(char c);

	bool isIntegral(char c);

	bool isFloatingPoint(char c);

	string toSymbol(const string& text);

	string replaceAll(const string& original, const string& from, const string& to);
}
