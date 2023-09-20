#include "platform.h"
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
	unordered_map<string, string> options;

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
				options["mode"] = argv[i + 1];
				++i;
			}

			if (argument == "--platform" && i + 1 < argc) {
				options["platform"] = argv[i + 1];
				++i;
			}

			if (argument == "--compiler" && i + 1 < argc) {
				options["compiler"] = argv[i + 1];
				++i;
			}

			if (argument == "--verbose") {
				options["verbose"] = "1";
			}
		}
	}

	if (options.count("mode") == 0) {
		options["mode"] = "release";
	}

	if (options.count("platform") == 0) {
		options["platform"] =  Platform::get();
	}

	if (options.count("compiler") == 0) {
		options["compiler"] = Platform::pickDefaultCompiler();
	}

	Compiler* compiler = nullptr;

	if (options["compiler"] == "gcc") {
		compiler = new GCCCompiler();
	} else if (options["compiler"] == "clang") {
		compiler = new ClangCompiler();
	} else if (options["compiler"] == "emscripten") {
		compiler = new EmscriptenCompiler();
	} else {
		cout << "unsupported compiler\n";
		return 0;
	}

	Workspace workspace(*compiler, options);

	try {
		if (command == "build") {
			workspace.bootstrap();
			workspace.build();
			return 0;
		}

		if (command == "clean") {
			workspace.clean();
			return 0;
		}

		if (command == "run") {
			workspace.bootstrap();
			workspace.run(forwardedArguments);
			return 0;
		}

		if (command == "init") {
			workspace.init();
			return 0;
		}

		if (command == "show") {
			workspace.bootstrap();
			workspace.show();
			return 0;
		}

		cout << "unknown command\n";
		return 0;
	} catch (const std::exception& e) {
		cout << e.what() << "\n";
	}

	return 0;
}
