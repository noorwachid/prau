#include "Platform.h"

namespace Platform
{
	string Get()
	{
#ifdef _WIN64
		return "windows";
#elif _WIN32
		return "windows";
#elif __APPLE__
		return "darwin";
#elif __linux
		return "linux";
#elif __unix
		return "unix";
#elif __emscripten__
		return "web";
#else
		return "unknown";
#endif
	}
}
