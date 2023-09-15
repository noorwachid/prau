#pragma once

#include "SourceToken.h"
#include "Text.h"

class SourceLexer {
public:
	void tokenize(const string& content) {
		_content = content;
		_index = 0;

		tokenize();
	}

	size_t getTokenSize() const {
		return _tokens.size();
	}

	const Token& getToken(size_t offset) const {
		return _tokens[offset];
	}

private:
	bool check(size_t offset) {
		return _index + offset < _content.size();
	}

	bool is(size_t offset, char c) {
		return _content[_index + offset] == c;
	}

	char get(size_t offset) {
		return _content[_index + offset];
	}

	void advance(size_t offset) {
		_index += offset;
	}

	void tokenize() {
		while (check(0)) {
			switch (get(0)) {
				case '#':
					tokenizeMacro();
					break;

				default:
					advance(1);
					break;
			}
		}
	}

	void tokenizeWhitespace() {
		while (check(0) && (Text::isWhitespace(get(0)) || is(0, '\n'))) {
			advance(1);
		}
	}

	void tokenizeMacro() {
		advance(1);
		tokenizeWhitespace();

		string symbol = collectSymbol();

		if (symbol == "include") {
			addToken(TokenType::macroInclude, '#' + symbol);
			tokenizeWhitespace();

			if (is(0, '"')) {
				return parsePath('"', TokenType::path);
			}

			if (is(0, '<')) {
				return parsePath('>', TokenType::path);
			}
			return;
		}
	}

	string collectSymbol() {
		size_t begin = _index;
		size_t n = 0;

		while (check(0) && Text::isSymbol(get(0))) {
			advance(1);
			++n;
		}

		return _content.substr(begin, n);
	}

	void parsePath(char closing, TokenType type) {
		size_t begin = _index + 1;
		size_t n = 0;

		advance(1);

		while (check(0) && get(0) != closing) {
			if (is(0, '\\')) {
				++n;
				advance(1);
				continue;
			}

			advance(1);
			++n;
		}

		string string = _content.substr(begin, n);
		advance(1);
		++n;

		addToken(type, string);
	}

	void addToken(TokenType type, const string& value) {
		Token token;
		token.type = type;
		token.value = value;
		_tokens.push_back(token);
	}

private:
	string _content;
	size_t _index = 0;
	vector<Token> _tokens;
};
