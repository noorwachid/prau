#include "builder.h"
#include <unistd.h>

BuilderCache::BuilderCache(Project& project, ProjectGraph& projectSource)
	: _project(project), _projectSource(projectSource) {
}

void BuilderCache::invalidate() {
	ofstream fileStream(getFile());
	YAML::Node node;
	node = _projectSource;
	fileStream << node;
}

void BuilderCache::validate() {
	_valid = true;

	string file = getFile();
	string directory = "build/cache";

	if (!fs::exists(directory)) {
		fs::create_directories(directory);
	}

	if (!fs::exists(file)) {
		_valid = false;
		return;
	}

	YAML::Node cacheProjectSourceNode = YAML::LoadFile(file);
	ProjectGraph cacheProjectSource = cacheProjectSourceNode.as<ProjectGraph>();

	for (const auto [source, detail] : _projectSource.sources) {
		if (cacheProjectSource.sources.count(source)) {
			if (cacheProjectSource.sources[source].modified != detail.modified) {
				addInvalidSource(source);
			}
		} else {
			addInvalidSource(source);
		}
	}

	if (!_invalidSources.empty()) {
		_valid = false;
	}
}

bool BuilderCache::isValid() const {
	return _valid;
}

void BuilderCache::addInvalidSource(const string& source) {
	if (_projectSource.sources.count(source)) {
		for (const string& reference : _projectSource.sources[source].references) {
			addInvalidSource(reference);
		}
	}

	if (isSourceFile(source) && _invalidSources.count(source) == 0) {
		_invalidSources.insert(source);
		return;
	}
}

bool BuilderCache::isSourceFile(const string& source) const {
	string extension = fs::path(source).extension().string();

	return extension == ".cpp";
}

string BuilderCache::getFile() const {
	return "build/cache/" + _project.name + ".prau";
}

vector<string> BuilderCache::getInvalidSources() const {
	return vector<string>(_invalidSources.begin(), _invalidSources.end());
}

Builder::Builder(Compiler& compiler, Project& project, const vector<BuildDependency>& dependencies)
	: _compiler(compiler), _project(project), _projectGraph(ProjectGraphLoader::load(project)),
	  _cache(project, _projectGraph), _dependencies(dependencies) {
}

void Builder::setVerbose(bool verbose) {
	_verbose = verbose;
}

void Builder::setMode(const string& mode) {
	_mode = mode;
}

bool Builder::requireRebuilding() {
	_cache.validate();

	return !_cache.isValid();
}

BuildResult Builder::build() {
	BuildResult result;

	string flags;
	flags += " " + _compiler.composeStandard(_project.standard);

	for (const string& directory : _project.headerPaths) {
		flags += " " + _compiler.composeHeaderDirectory(directory);
	}

	_cache.validate();

	if (_cache.isValid()) {
		for (const BuildDependency& dependency: _dependencies) {
			if (!dependency.headerOnly && dependency.recompiling) {
				result.recompiling = true;
			}
		}

		if (result.recompiling) {
			if (!link(flags)) {
				return result;
			}

			return result;
		}

		string log = string("[") + Term::yellowFG + _project.name + Term::reset + "] no work to do\n";
		cout << log;

		return result;
	}

	vector<string> invalidSources = _cache.getInvalidSources();

	if (!compile(flags, !invalidSources.empty() ? invalidSources : _project.sources)) {
		return result;
	}

	if (!link(flags)) {
		return result;
	}

	_cache.invalidate();

	result.recompiling = true;

	return result;
}

bool Builder::compile(const string& flags, const vector<string>& sources) {
	for (const string& source : sources) {
		string objectFile = "build/object/" + _project.name + "/" + _compiler.composeObjectFile(source);
		string objectCommand = _compiler.composeObject(objectFile) + " " + source;
		string command = _compiler.getProgram() + flags + " " + _compiler.composeMode(_mode) + " " + objectCommand;
		string log;

		log = log + "[" + Term::yellowFG + _project.name + Term::reset + "] ";
		log = log + Term::greenFG + "compile " + Term::reset;
		log = log + fs::path(source).filename().string() + "\n";

		string objectDirectory = fs::path(objectFile).parent_path();

		if (!fs::exists(objectDirectory))
			fs::create_directories(objectDirectory);

		pid_t pid = fork();

		if (pid == 0) {
			cout << log;

			if (_verbose)
				cout << (command + "\n");

			int result = system(command.c_str());

			exit(0);
		}
	}

	while (wait(NULL) > 0);

	return true;
}

bool Builder::link(const string& flags) {
	if (_project.sources.empty()) {
		return true;
	}

	string objects;

	for (const BuildDependency& dependency: _dependencies) {
		if (!dependency.headerOnly) {
			objects += " build/library/" + _compiler.composeLibraryFile(dependency.project);
		}
	}

	for (const string& source : _project.sources) {
		objects += " build/object/" + _project.name + "/" + _compiler.composeObjectFile(source);
	}

	if (_project.type == "library") {
		if (!fs::exists("build/library"))
			fs::create_directories("build/library");

		string libraryFile = "build/library/" + _compiler.composeLibraryFile(_project.name);
		string command = _compiler.getProgram() + flags + " " + _compiler.composeLibrary(libraryFile) + objects;
		string log;
		log += "[";
		log += Term::yellowFG;
		log += _project.name;
		log += Term::reset;
		log += "] ";
		log += Term::greenFG;
		log += "link ";
		log += Term::reset;
		log += to_string(_project.sources.size());
		log += _project.sources.size() > 1 ? " objects" : " object";
		log += "\n";

		cout << log;

		if (_verbose)
			cout << (command + "\n");

		if (system(command.c_str()) != 0) {
			return false;
		}
	}

	if (_project.type == "executable") {
		if (!fs::exists("build/executable"))
			fs::create_directories("build/executable");

		string executableFile = "build/executable/" + _compiler.composeExecutableFile(_project.name);
		string command = _compiler.getProgram() + flags + " " + _compiler.composeExecutable(executableFile) + objects;
		string log;
		log += "[";
		log += Term::yellowFG;
		log += _project.name;
		log += Term::reset;
		log += "] ";
		log += Term::greenFG;
		log += "link ";
		log += Term::reset;
		log += to_string(_project.sources.size());
		log += _project.sources.size() > 1 ? " objects" : " object";
		log += "\n";

		cout << log;

		if (_verbose)
			cout << (command + "\n");

		if (system(command.c_str()) != 0) {
			return false;
		}
	}

	return true;
}
