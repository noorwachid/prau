clang++ -std=c++17 -O2 -o prau -Idependency/yaml/include \
	source/main.cpp \
	source/text.cpp \
	source/platform.cpp \
	source/project.cpp \
	source/projectgraph.cpp \
	source/builder.cpp \
	source/starter.cpp \
	source/workspace.cpp \
	source/compiler/gcccompiler.cpp \
	source/compiler/clangcompiler.cpp \
	source/compiler/emscriptencompiler.cpp \
	dependency/yaml/src/binary.cpp \
	dependency/yaml/src/convert.cpp \
	dependency/yaml/src/depthguard.cpp \
	dependency/yaml/src/directives.cpp \
	dependency/yaml/src/emit.cpp \
	dependency/yaml/src/emitfromevents.cpp \
	dependency/yaml/src/emitter.cpp \
	dependency/yaml/src/emitterstate.cpp \
	dependency/yaml/src/emitterutils.cpp \
	dependency/yaml/src/exceptions.cpp \
	dependency/yaml/src/exp.cpp \
	dependency/yaml/src/memory.cpp \
	dependency/yaml/src/node.cpp \
	dependency/yaml/src/node_data.cpp \
	dependency/yaml/src/nodebuilder.cpp \
	dependency/yaml/src/nodeevents.cpp \
	dependency/yaml/src/null.cpp \
	dependency/yaml/src/ostream_wrapper.cpp \
	dependency/yaml/src/parse.cpp \
	dependency/yaml/src/parser.cpp \
	dependency/yaml/src/regex_yaml.cpp \
	dependency/yaml/src/scanner.cpp \
	dependency/yaml/src/scanscalar.cpp \
	dependency/yaml/src/scantag.cpp \
	dependency/yaml/src/scantoken.cpp \
	dependency/yaml/src/simplekey.cpp \
	dependency/yaml/src/singledocparser.cpp \
	dependency/yaml/src/stream.cpp \
	dependency/yaml/src/tag.cpp