#pragma once

#include "Common.h"

enum struct TokenType
{
	macroInclude,
	path,
};

struct Token
{
	TokenType type;
	string value;
};
