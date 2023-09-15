#pragma once

#include "common.h"

enum struct TokenType {
	macroInclude,
	path,
};

struct Token {
	TokenType type;
	string value;
};
