#include "ProjectGraph.h"

ProjectSource ProjectSourceLoader::load(Project& project)
{
	ProjectSource projectSource;
	ProjectSourceLoader projectLoader(project, projectSource);

	return projectSource;
}

ProjectSourceLoader::ProjectSourceLoader(Project& project, ProjectSource& projectSource)
	: project(project), projectSource(projectSource)
{
	for (const string source : project.sources)
	{
		if (fs::exists(source))
		{
			Add(source);
		}
	}
}

void ProjectSourceLoader::Print()
{
	for (const auto& [source, detail] : projectSource.sources)
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

void ProjectSourceLoader::Add(const string& source)
{
	if (projectSource.sources.count(source))
	{
		return;
	}

	ProjectSource::Detail sourceDetail;

	chrono::time_point timepoint = fs::last_write_time(source);
	sourceDetail.modified = chrono::duration_cast<chrono::milliseconds>(timepoint.time_since_epoch()).count();

	size_t sourceSize = fs::file_size(source);
	ifstream sourceStream(source);
	string sourceContent(sourceSize, '\0');
	sourceStream.read(sourceContent.data(), sourceSize);

	SourceParser sourceParser;
	sourceParser.Parse(sourceContent);
	projectSource.sources[source] = sourceDetail;

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
			projectSource.sources[sameDirectory].references.push_back(source);
			continue;
		}

		for (const string& directory : project.headerDirectories)
		{
			string differentDirectory = directory + "/" + dependency;

			if (fs::exists(differentDirectory))
			{
				Add(differentDirectory);
				projectSource.sources[differentDirectory].references.push_back(source);
				break;
			}
		}
	}
}
