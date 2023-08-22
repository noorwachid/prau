#include "ProjectGraph.h"

ProjectGraph ProjectGraphLoader::load(Project& project)
{
	ProjectGraph projectSource;
	ProjectGraphLoader projectLoader(project, projectSource);

	return projectSource;
}

ProjectGraphLoader::ProjectGraphLoader(Project& project, ProjectGraph& projectGraph)
	: _project(project), _projectGraph(projectGraph)
{
	for (const string source : project.sources)
	{
		if (fs::exists(source))
		{
			Add(source);
		}
	}
}

void ProjectGraphLoader::Print()
{
	for (const auto& [source, detail] : _projectGraph.sources)
	{
		cout << source << ":\n";
		cout << "  modified: " << detail.modified << "\n";
		if (!detail.references.empty())
		{
			cout << "  references:\n";
			for (const string& unit : detail.references)
			{
				cout << "    - " << unit << "\n";
			}
		}
	}
}

void ProjectGraphLoader::Add(const string& source)
{
	if (_projectGraph.sources.count(source))
	{
		return;
	}

	ProjectGraph::Detail sourceDetail;

	chrono::time_point timepoint = fs::last_write_time(source);
	sourceDetail.modified = chrono::duration_cast<chrono::milliseconds>(timepoint.time_since_epoch()).count();

	size_t sourceSize = fs::file_size(source);
	ifstream sourceStream(source);
	string sourceContent(sourceSize, '\0');
	sourceStream.read(sourceContent.data(), sourceSize);

	SourceParser sourceParser;
	sourceParser.Parse(sourceContent);
	_projectGraph.sources[source] = sourceDetail;

	string directory = fs::path(source).parent_path().string();
	if (!directory.empty())
	{
		directory += "/";
	}

	for (const string& dependency : sourceParser.GetDependencies())
	{
		string sameDirectory = directory + dependency;
		if (fs::exists(sameDirectory))
		{
			Add(sameDirectory);
			_projectGraph.sources[sameDirectory].references.push_back(source);
			continue;
		}

		for (const string& directory : _project.headerDirectories)
		{
			string differentDirectory = directory + "/" + dependency;

			if (fs::exists(differentDirectory))
			{
				Add(differentDirectory);
				_projectGraph.sources[differentDirectory].references.push_back(source);
				break;
			}
		}
	}
}
