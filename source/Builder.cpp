#include "Builder.h"

BuilderCache::BuilderCache(Project& project, ProjectSource& projectSource)
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
	ProjectSource cacheProjectSource = cacheProjectSourceNode.as<ProjectSource>();

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
	: _compiler(compiler), _project(project), _projectSource(ProjectSourceLoader::load(project)),
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
	flags += " " + ComposeStandard();

	for (const string& directory : _project.headerDirectories)
	{
		flags += " " + ComposeHeaderDirectory(directory);
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
		string command = _compiler.program + flags + " " + ComposeObjectBuilder(source) + " " + source;
		string log;
		log += "[";
		log += Term::yellowFG;
		log += _project.name;
		log += Term::reset;
		log += "] ";
		log += Term::greenFG;
		log += "compile ";
		log += Term::reset;
		log += fs::path(source).filename();
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

bool Builder::Link(const string& flags)
{
	string objects;

	for (const string& source : _project.sources)
	{
		objects += " build/object/" + _project.name + "/" + ComposeObjectFile(source);
	}

	for (const string& library : _project.libraries) 
	{
		for (const string& libraryDirectory: _project.libraryDirectories)
		{
			string libraryPath = libraryDirectory + "/" + ComposeLibraryFile(library);

			if (fs::exists(libraryPath))
			{
				objects += " " + libraryPath;
			}
		}
	}

	if (_project.type == "library")
	{
		string command = _compiler.program + flags + " " + ComposeLibraryBuilder(_project.name) + objects;
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
		string command = _compiler.program + flags + " " + ComposeExecutableBuilder(_project.name) + objects;
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

	string executablePath = "build/executable/" + ComposeExecutableFile(_project.name);
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

string Builder::ComposeHeaderDirectory(const string& directory) const
{
	return Text::ReplaceAll(_compiler.directory.header, "<directory>", directory);
}

string Builder::ComposeLibraryDirectory(const string& directory) const
{
	return Text::ReplaceAll(_compiler.directory.library, "<directory>", directory);
}

string Builder::ComposeStandard() const
{
	return Text::ReplaceAll(_compiler.standard, "<standard>", _project.standard);
}

string Builder::ComposeObjectFile(const string& source) const
{
	return Text::ReplaceAll(_compiler.file.object, "<source>", source);
}

string Builder::ComposeLibraryFile(const string& name) const
{
	return Text::ReplaceAll(_compiler.file.library, "<name>", name);
}

string Builder::ComposeExecutableFile(const string& name) const
{
	return Text::ReplaceAll(_compiler.file.executable, "<name>", name);
}

string Builder::ComposeObjectBuilder(const string& source) const
{
	string file = "build/object/" + _project.name + "/" + ComposeObjectFile(source);
	string directory = fs::path(file).parent_path();

	if (!fs::exists(directory))
	{
		fs::create_directories(directory);
	}

	return Text::ReplaceAll(_compiler.builder.object, "<file>", file);
}

string Builder::ComposeLibraryBuilder(const string& name) const
{
	string directory = "build/library/";
	string file = directory + ComposeLibraryFile(name);

	if (!fs::exists(directory))
	{
		fs::create_directories(directory);
	}

	return Text::ReplaceAll(_compiler.builder.library, "<file>", file);
}

string Builder::ComposeExecutableBuilder(const string& name) const
{
	string directory = "build/executable/";
	string file = directory + ComposeExecutableFile(_project.name);

	if (!fs::exists(directory))
	{
		fs::create_directories(directory);
	}

	return Text::ReplaceAll(_compiler.builder.executable, "<file>", file);
}
