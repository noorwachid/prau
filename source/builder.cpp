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

Builder::Builder(Compiler& compiler, Project& project)
	: _compiler(compiler), _project(project), _projectSource(ProjectGraphLoader::load(project)),
	  _cache(project, _projectSource) {
}

void Builder::build() {
	_cache.validate();

	if (_cache.isValid()) {
		cout << "no work to do\n";
		return;
	}

	string flags;
	flags += " " + _compiler.composeStandard(_project.standard);

	for (const string& directory : _project.headerDirectories) {
		flags += " " + _compiler.composeHeaderDirectory(directory);
	}

	vector<string> invalidSources = _cache.getInvalidSources();

	if (!compile(flags, !invalidSources.empty() ? invalidSources : _project.sources)) {
		return;
	}

	if (!link(flags)) {
		return;
	}

	_cache.invalidate();
}

bool Builder::compile(const string& flags, const vector<string>& sources) {
	for (const string& source : sources) {
		string objectFile = "build/object/" + _project.name + "/" + _compiler.composeObjectFile(source);
		string objectCommand = _compiler.composeObject(objectFile) + " " + source;
		string command = _compiler.getProgram() + flags + " " + objectCommand;
		string log;

		log = log + "[" + Term::yellowFG + _project.name + Term::reset + "] ";
		log = log + Term::greenFG + "compile " + Term::reset;
		log = log + fs::path(source).filename().string() + "\n";

		string objectDirectory = fs::path(objectFile).parent_path();

		if (!fs::exists(objectDirectory))
			fs::create_directories(objectDirectory);

		pid_t pid = fork();
		if (pid == 0) {
			system(command.c_str());

			cout << log;

			if (verbose)
				cout << (command + "\n");

			exit(0);
		}
	}

	while (wait(NULL) > 0)
		;

	return true;
}

bool Builder::link(const string& flags) {
	string objects;

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

		if (verbose)
			cout << (command + "\n");

		if (system(command.c_str()) != 0) {
			string log;
			log += "[";
			log += Term::yellowFG;
			log += _project.name;
			log += Term::reset;
			log += "] ";
			log += Term::greenFG;
			log += "failed";
			log += Term::reset;
			log += "\n";

			cout << log;

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

		if (verbose)
			cout << (command + "\n");

		if (system(command.c_str()) != 0) {
			return false;
		}
	}

	return true;
}

void Builder::run() {
	if (_project.type != "executable") {
		cout << "the project's type is not an executable\n";
		return;
	}

	_cache.validate();
	if (!_cache.isValid()) {
		build();
	}

	string executablePath = "build/executable/" + _compiler.composeExecutableFile(_project.name);
	if (!fs::exists(executablePath)) {
		cout << "no executable were found\n";
		return;
	}

	system(("./" + executablePath).c_str());
	return;
}

void Builder::clean() {
	if (!fs::exists(ProjectPath::generate("."))) {
		cout << "whoa there's no project file in this directory, careful dude\n";
		return;
	}

	if (!fs::exists("build")) {
		cout << "already clean\n";
		return;
	}

	fs::remove_all("build");
	cout << "cleaned up\n";
}
