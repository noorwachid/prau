#pragma once

#include "SourceLexer.h"

class SourceParser {
public:
	void parse(const std::string& content) {
		_lexer.tokenize(content);
		_index = 0;

		Parse();
	}

	const vector<string>& getDependencies() const {
		return _dependencies;
	}

private:
	bool check(size_t offset) {
		return _index + offset < _lexer.getTokenSize();
	}

	bool is(size_t offset, TokenType type) {
		return _lexer.getToken(_index + offset).type == type;
	}

	const Token& get(size_t offset) {
		return _lexer.getToken(_index + offset);
	}

	void advance(size_t offset) {
		_index += offset;
	}

	void parse() {
		while (check(0)) {
			if (check(1) && is(0, TokenType::macroInclude) && is(1, TokenType::path)) {
				_dependencies.push_back(get(1).value);
				advance(2);
			} else {
				advance(1);
			}
		}
	}

private:
	size_t _index = 0;
	SourceLexer _lexer;
	vector<string> _dependencies;
};
