#include "Compiler.h"
#include "Platform.h"

vector<string> CompilerPath::Generate(const string& language, const string& name)
{
	string file = name + ".prau";

#ifdef PLATFORM_APPLE
	return {
		"/usr/local/share/prau/compiler/" + language + "/" + file,
		"asset/compiler/" + language + "/" + file,
	};
#endif

	return {};
}

Compiler CompilerLoader::Load(const string& path)
{
	YAML::Node node = YAML::LoadFile(path);
	Compiler compiler = node.as<Compiler>();

	return compiler;
}
