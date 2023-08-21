#include "Text.h"

namespace Text
{
	bool IsSymbolPrefix(char c)
	{
		return IsAlphabetic(c) || c == '_';
	}

	bool IsSymbol(char c)
	{
		return IsAlphabetic(c) || IsIntegral(c) || c == '_';
	}

	bool IsWhitespace(char c)
	{
		return c == ' ' || c == '\t';
	}

	bool IsLowerCase(char c)
	{
		return c >= 'a' && c <= 'z';
	}

	bool IsUpperCase(char c)
	{
		return c >= 'A' && c <= 'Z';
	}

	bool IsAlphabetic(char c)
	{
		return IsLowerCase(c) || IsUpperCase(c);
	}

	bool IsIntegral(char c)
	{
		return c >= '0' && c <= '9';
	}

	bool IsFloatingPoint(char c)
	{
		return IsIntegral(c) || c == '.';
	}

	string ToSymbol(const string& text)
	{
		string symbol;
		for (char c : text)
		{
			if (IsSymbol(c))
			{
				symbol += c;
			}
		}

		return symbol;
	}

	string ReplaceAll(const string& original, const string& from, const string& to)
	{
		string results;

		string::const_iterator end = original.end();
		string::const_iterator current = original.begin();
		string::const_iterator next = std::search(current, end, from.begin(), from.end());

		while (next != end)
		{
			results.append(current, next);
			results.append(to);
			current = next + from.size();
			next = std::search(current, end, from.begin(), from.end());
		}

		results.append(current, next);

		return results;
	}
}
