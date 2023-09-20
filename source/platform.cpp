#include "platform.h"

namespace Platform {
	string get() {
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

	string pickDefaultCompiler() {
#ifdef _WIN64
		return "";
#elif _WIN32
		return "";
#elif __APPLE__
		return "clang";
#elif __linux
		return "gcc";
#elif __unix
		return "gcc";
#elif __emscripten__
		return "emscripten";
#else
		return "";
#endif
	}
}
