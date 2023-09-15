#include "Text.h"

namespace Text {
	bool isSymbolPrefix(char c) {
		return isAlphabetic(c) || c == '_';
	}

	bool isSymbol(char c) {
		return isAlphabetic(c) || isIntegral(c) || c == '_';
	}

	bool isWhitespace(char c) {
		return c == ' ' || c == '\t';
	}

	bool isLowerCase(char c) {
		return c >= 'a' && c <= 'z';
	}

	bool isUpperCase(char c) {
		return c >= 'A' && c <= 'Z';
	}

	bool isAlphabetic(char c) {
		return isLowerCase(c) || isUpperCase(c);
	}

	bool isIntegral(char c) {
		return c >= '0' && c <= '9';
	}

	bool isFloatingPoint(char c) {
		return isIntegral(c) || c == '.';
	}

	string toSymbol(const string& text) {
		string symbol;
		for (char c : text) {
			if (isSymbol(c)) {
				symbol += c;
			}
		}

		return symbol;
	}

	string replaceAll(const string& original, const string& from, const string& to) {
		string results;

		string::const_iterator end = original.end();
		string::const_iterator current = original.begin();
		string::const_iterator next = std::search(current, end, from.begin(), from.end());

		while (next != end) {
			results.append(current, next);
			results.append(to);
			current = next + from.size();
			next = std::search(current, end, from.begin(), from.end());
		}

		results.append(current, next);

		return results;
	}
}
