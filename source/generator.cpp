#include "generator.h"

Generator::Generator(Workspace& workspace): _workspace(workspace) {
}

void Generator::generate(const string& target) {
	if (target == "clangd" || target == "clangdeeznuts") {
		return generateClangD();
	}

	if (target == "cmake") {
		return generateCMake();
	}

	if (target == "sh") {
		return generateSh();
	}
}

void Generator::generateClangD() {
	ofstream config(".clangd");
	config << "CompileFlags:\n";
	config << "  Add:\n";

	Project& project = _workspace._projects[""];

	config << "    - " << _workspace._compiler.composeStandard(project.standard) << "\n";

	for (const string& headerPath: project.headerPaths) {
		config << "    - " << _workspace._compiler.composeHeaderDirectory(fs::absolute(headerPath).string()) << "\n";
	}
}

void Generator::generateCMake() {
	Project& project = _workspace._projects[""];

	ofstream config("CMakeLists.txt");
	config << "cmake_minimum_required(VERSION 3.3)\n";
	config << "set(CMAKE_CXX_STANDARD " << project.standard << ")\n";
	config << "project(" << project.name << ")\n";

	if (project.type == "executable") {
		config << "add_executable(" << project.name << "\n";
		for (const string& source: project.sources) {
			config << "\t" << source << "\n";
		}
		config << ")\n";
	}

	if (project.type == "library") {
		config << "add_library(" << project.name << " SHARED\n";
		for (const string& source: project.sources) {
			config << "\t" << source << "\n";
		}
		config << ")\n";
	}

	if (!project.headerPaths.empty()) {
		config << "target_include_directories(" << project.name << " PUBLIC\n";
		for (const string& headerPath: project.headerPaths) {
			config << "\t" << headerPath << "\n";
		}
		config << ")\n";
	}
}

void Generator::generateSh() {
	Project& project = _workspace._projects[""];

	ofstream config("build.sh");

	config << _workspace._compiler.getProgram() << " \\\n";

	config << "\t" << _workspace._compiler.composeStandard(project.standard) << " \\\n";
	config << "\t" << _workspace._compiler.composeMode(_workspace._options["mode"]) << " \\\n";

	for (const string& headerPath: project.headerPaths) {
		config << "\t" << _workspace._compiler.composeHeaderDirectory(headerPath) << " \\\n";
	}

	for (const string& source: project.sources) {
		config << "\t" << source << " \\\n";
	}

	if (project.type == "executable") {
		config << "\t" << _workspace._compiler.composeExecutable(_workspace._compiler.composeExecutableFile(project.name));
	}

	if (project.type == "library") {
		config << "\t" << _workspace._compiler.composeLibrary(_workspace._compiler.composeLibraryFile(project.name));
	}
}
