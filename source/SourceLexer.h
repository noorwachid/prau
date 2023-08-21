#pragma once

#include "SourceToken.h"
#include "Text.h"

class SourceLexer
{
public:
	void Tokenize(const string& content)
	{
		_content = content;
		_index = 0;

		Tokenize();
	}

	size_t GetTokenSize() const
	{
		return _tokens.size();
	}

	const Token& GetToken(size_t offset) const
	{
		return _tokens[offset];
	}

private:
	bool Check(size_t offset)
	{
		return _index + offset < _content.size();
	}

	bool Is(size_t offset, char c)
	{
		return _content[_index + offset] == c;
	}

	char Get(size_t offset)
	{
		return _content[_index + offset];
	}

	void Advance(size_t offset)
	{
		_index += offset;
	}

	void Tokenize()
	{
		while (Check(0))
		{
			switch (Get(0))
			{
				case '#':
					TokenizeMacro();
					break;

				default:
					Advance(1);
					break;
			}
		}
	}

	void TokenizeWhitespace()
	{
		while (Check(0) && (Text::IsWhitespace(Get(0)) || Is(0, '\n')))
		{
			Advance(1);
		}
	}

	void TokenizeMacro()
	{
		Advance(1);
		TokenizeWhitespace();

		string symbol = CollectSymbol();

		if (symbol == "include")
		{
			AddToken(TokenType::macroInclude, '#' + symbol);
			TokenizeWhitespace();

			if (Is(0, '"'))
			{
				return ParsePath('"', TokenType::path);
			}

			if (Is(0, '<'))
			{
				return ParsePath('>', TokenType::path);
			}
			return;
		}
	}

	string CollectSymbol()
	{
		size_t begin = _index;
		size_t n = 0;

		while (Check(0) && Text::IsSymbol(Get(0)))
		{
			Advance(1);
			++n;
		}

		return _content.substr(begin, n);
	}

	void ParsePath(char closing, TokenType type)
	{
		size_t begin = _index + 1;
		size_t n = 0;

		Advance(1);

		while (Check(0) && Get(0) != closing)
		{
			if (Is(0, '\\'))
			{
				++n;
				Advance(1);
				continue;
			}

			Advance(1);
			++n;
		}

		string string = _content.substr(begin, n);
		Advance(1);
		++n;

		AddToken(type, string);
	}

	void AddToken(TokenType type, const string& value)
	{
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
