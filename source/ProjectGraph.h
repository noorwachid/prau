#pragma once

#include "Project.h"
#include "SourceParser.h"

struct ProjectSource
{
	struct Detail
	{
		size_t modified;
		vector<string> references;
	};

	unordered_map<string, Detail> sources;
};

namespace YAML
{
	template <>
	struct convert<ProjectSource>
	{
		static Node encode(const ProjectSource& projectSource)
		{
			Node node;

			for (const auto& [source, detail] : projectSource.sources)
			{
				YAML::Node subnode = node[source];
				subnode["modified"] = detail.modified;

				if (!detail.references.empty())
					subnode["references"] = detail.references;
			}

			return node;
		}

		static bool decode(const Node& node, ProjectSource& projectSource)
		{
			if (node.IsMap())
			{
				for (const auto& subnode : node)
				{
					if (subnode.second.IsMap())
					{
						ProjectSource::Detail& detail = projectSource.sources[subnode.first.as<string>()];
						if (subnode.second["modified"] && subnode.second["modified"].IsScalar())
							detail.modified = std::stoull(subnode.second["modified"].as<string>());
						if (subnode.second["references"] && subnode.second["references"].IsSequence())
							detail.references = subnode.second["references"].as<vector<string>>();
					}
				}
			}

			return true;
		}
	};
}

class ProjectSourceLoader
{
public:
	static ProjectSource load(Project& project);

private:
	Project& project;
	ProjectSource& projectSource;

	ProjectSourceLoader(Project& project, ProjectSource& projectSource);

	void Add(const string& source);

	void Print();
};
