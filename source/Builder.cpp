#include "Builder.h"

BuilderCache::BuilderCache(Project& project, ProjectGraph& projectSource)
	: _project(project), _projectSource(projectSource)
{
}

void BuilderCache::Invalidate()
{
	ofstream fileStream(GetFile());
	YAML::Node node;
	node = _projectSource;
	fileStream << node;
}

void BuilderCache::Validate()
{
	_valid = true;

	string file = GetFile();
	string directory = "build/cache";

	if (!fs::exists(directory))
	{
		fs::create_directories(directory);
	}

	if (!fs::exists(file))
	{
		_valid = false;
		return;
	}

	YAML::Node cacheProjectSourceNode = YAML::LoadFile(file);
	ProjectGraph cacheProjectSource = cacheProjectSourceNode.as<ProjectGraph>();

	for (const auto [source, detail] : _projectSource.sources)
	{
		if (cacheProjectSource.sources.count(source))
		{
			if (cacheProjectSource.sources[source].modified != detail.modified)
			{
				AddInvalidSource(source);
			}
		}
		else
		{
			AddInvalidSource(source);
		}
	}

	if (!_invalidSources.empty())
	{
		_valid = false;
	}
}

bool BuilderCache::IsValid() const
{
	return _valid;
}

void BuilderCache::AddInvalidSource(const string& source)
{
	if (_projectSource.sources.count(source))
	{
		for (const string& reference : _projectSource.sources[source].references)
		{
			AddInvalidSource(reference);
		}
	}

	if (IsSourceFile(source) && _invalidSources.count(source) == 0)
	{
		_invalidSources.insert(source);
		return;
	}
}

bool BuilderCache::IsSourceFile(const string& source) const
{
	string extension = fs::path(source).extension().string();

	return extension == ".cpp";
}

string BuilderCache::GetFile() const
{
	return "build/cache/" + _project.name + ".prau";
}

vector<string> BuilderCache::GetInvalidSources() const
{
	return vector<string>(_invalidSources.begin(), _invalidSources.end());
}

Builder::Builder(Compiler& compiler, Project& project)
	: _compiler(compiler), _project(project), _projectSource(ProjectGraphLoader::load(project)),
	  _cache(project, _projectSource)
{
}

void Builder::Build()
{
	_cache.Validate();

	if (_cache.IsValid())
	{
		cout << "no work to do\n";
		return;
	}

	string flags;
	flags += " " + _compiler.ComposeStandard(_project.standard);

	for (const string& directory : _project.headerDirectories)
	{
		flags += " " + _compiler.ComposeHeaderDirectory(directory);
	}

	vector<string> invalidSources = _cache.GetInvalidSources();

	if (!Compile(flags, !invalidSources.empty() ? invalidSources : _project.sources))
	{
		return;
	}

	if (!Link(flags))
	{
		return;
	}

	_cache.Invalidate();
}

bool Builder::Compile(const string& flags, const vector<string>& sources)
{
	for (const string& source : sources)
	{
		string objectFile = "build/object/" + _project.name + "/" + _compiler.ComposeObjectFile(source);
		string objectCommand = _compiler.ComposeObject(objectFile) + " " + source;
		string command = _compiler.GetProgram() + flags + " " + objectCommand;
		string log;

		log = log + "[" + Term::yellowFG + _project.name + Term::reset + "] ";
		log = log + Term::greenFG + "compile " + Term::reset;
		log = log + fs::path(source).filename().string() + "\n";

		cout << log;

		if (verbose)
			cout << (command + "\n");

		string objectDirectory = fs::path(objectFile).parent_path();

		if (!fs::exists(objectDirectory))
			fs::create_directories(objectDirectory);

		if (system(command.c_str()) != 0)
		{
			return false;
		}
	}

	return true;
}

bool Builder::Link(const string& flags)
{
	string objects;

	for (const string& source : _project.sources)
	{
		objects += " build/object/" + _project.name + "/" + _compiler.ComposeObjectFile(source);
	}

	if (_project.type == "library")
	{
		if (!fs::exists("build/library"))
			fs::create_directories("build/library");

		string libraryFile = "build/library/" + _compiler.ComposeLibraryFile(_project.name);
		string command = _compiler.GetProgram() + flags + " " + _compiler.ComposeLibrary(libraryFile) + objects;
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

		if (system(command.c_str()) != 0)
		{
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

	if (_project.type == "executable")
	{
		if (!fs::exists("build/executable"))
			fs::create_directories("build/executable");

		string executableFile = "build/executable/" + _compiler.ComposeExecutableFile(_project.name);
		string command = _compiler.GetProgram() + flags + " " + _compiler.ComposeExecutable(executableFile) + objects;
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

		if (system(command.c_str()) != 0)
		{
			return false;
		}
	}

	return true;
}

void Builder::Run()
{
	if (_project.type != "executable")
	{
		cout << "the project's type is not an executable\n";
		return;
	}

	_cache.Validate();
	if (!_cache.IsValid())
	{
		Build();
	}

	string executablePath = "build/executable/" + _compiler.ComposeExecutableFile(_project.name);
	if (!fs::exists(executablePath))
	{
		cout << "no executable were found\n";
		return;
	}

	system(("./" + executablePath).c_str());
	return;
}

void Builder::Clean()
{
	if (!fs::exists(ProjectPath::Generate(".")))
	{
		cout << "whoa there's no project file in this directory, careful dude\n";
		return;
	}

	if (!fs::exists("build"))
	{
		cout << "already clean\n";
		return;
	}

	fs::remove_all("build");
	cout << "cleaned up\n";
}

