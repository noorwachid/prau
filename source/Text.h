#pragma once

#include "Common.h"

namespace Text
{
	bool IsSymbolPrefix(char c);

	bool IsSymbol(char c);

	bool IsWhitespace(char c);

	bool IsLowerCase(char c);

	bool IsUpperCase(char c);

	bool IsAlphabetic(char c);

	bool IsIntegral(char c);

	bool IsFloatingPoint(char c);

	string ToSymbol(const string& text);

	string ReplaceAll(const string& original, const string& from, const string& to);
}
