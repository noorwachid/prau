#include "builder.h"
#include "platform.h"
#include "starter.h"
#include "workspace.h"
#include "compiler/gcccompiler.h"
#include "compiler/clangcompiler.h"
#include "compiler/emscriptencompiler.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "no command were specified\n";
		return 0;
	}

	string command = argv[1];
	string mode = "release";
	string platform = Platform::get();
	string compilerName = Platform::pickDefaultCompiler();
	bool verbose = false;

	vector<string> forwardedArguments;

	for (int i = 2; i < argc; ++i) {
		string argument = argv[i];
		if (argument.size() > 1 && argument[0] == '-' && argument[1] == '-') {
			if (argument.size() == 2) {
				for (int j = i + 1; j < argc; ++j) {
					forwardedArguments.push_back(argv[j]);
				}
				break;
			}

			if (argument == "--mode" && i + 1 < argc) {
				mode = argv[i + 1];
				++i;
			}

			if (argument == "--platform" && i + 1 < argc) {
				platform = argv[i + 1];
				++i;
			}

			if (argument == "--compiler" && i + 1 < argc) {
				compilerName = argv[i + 1];
				++i;
			}

			if (argument == "--verbose") {
				verbose = true;
			}
		}
	}

	if (platform.empty()) {
		platform = Platform::get();
	}

	if (compilerName.empty()) {
		compilerName = Platform::pickDefaultCompiler();
	}

	Compiler* compiler = nullptr;

	if (compilerName == "gcc") {
		compiler = new GCCCompiler();
	} else if (compilerName == "clang") {
		compiler = new ClangCompiler();
	} else if (compilerName == "emscripten") {
		compiler = new EmscriptenCompiler();
	} else {
		cout << "unsupported compiler\n";
		return 0;
	}

	Workspace workspace(*compiler);

	try {
		if (command == "build") {
			workspace.build();
			return 0;
		}

		if (command == "run") {
			workspace.run();
			return 0;
		}

		if (command == "show") {
			workspace.show();
			return 0;
		}

		if (command == "show-graph") {
			workspace.showGraph();
			return 0;
		}

		cout << "unknown command\n";
		return 0;
	} catch (const std::exception& e) {
		cout << e.what() << "\n";
	}

	return 0;
}
