#pragma once

#include "SourceLexer.h"

class SourceParser
{
public:
	void Parse(const std::string& content)
	{
		_lexer.Tokenize(content);
		_index = 0;

		Parse();
	}

	const vector<string>& GetDependencies() const
	{
		return _dependencies;
	}

private:
	bool Check(size_t offset)
	{
		return _index + offset < _lexer.GetTokenSize();
	}

	bool Is(size_t offset, TokenType type)
	{
		return _lexer.GetToken(_index + offset).type == type;
	}

	const Token& Get(size_t offset)
	{
		return _lexer.GetToken(_index + offset);
	}

	void Advance(size_t offset)
	{
		_index += offset;
	}

	void Parse()
	{
		while (Check(0))
		{
			if (Check(1) && Is(0, TokenType::macroInclude) && Is(1, TokenType::path))
			{
				_dependencies.push_back(Get(1).value);
				Advance(2);
			}
			else
			{
				Advance(1);
			}
		}
	}

private:
	size_t _index = 0;
	SourceLexer _lexer;
	vector<string> _dependencies;
};
