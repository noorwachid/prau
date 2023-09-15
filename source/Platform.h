#pragma once

#include "Common.h"

#ifdef _WIN64
#define PLATFORM_WINDOWS
#elif _WIN32
#define PLATFORM_WINDOWS
#elif __APPLE__
#define PLATFORM_APPLE
#elif __linux
#define PLATFORM_LINUX
#elif __unix
#define PLATFORM_UNIX
#elif __emscripten__
#define PLATFORM_WEB
#endif

namespace Platform {
	string get();
}
