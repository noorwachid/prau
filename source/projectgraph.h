#pragma once

#include "project.h"
#include "sourceparser.h"

struct ProjectGraph {
	struct Detail {
		size_t modified;
		vector<string> references;
	};

	unordered_map<string, Detail> sources;
};

namespace YAML {
	template <>
	struct convert<ProjectGraph> {
		static Node encode(const ProjectGraph& projectSource) {
			Node node;

			for (const auto& [source, detail] : projectSource.sources) {
				YAML::Node subnode = node[source];
				subnode["modified"] = detail.modified;

				if (!detail.references.empty())
					subnode["references"] = detail.references;
			}

			return node;
		}

		static bool decode(const Node& node, ProjectGraph& projectSource) {
			if (node.IsMap()) {
				for (const auto& subnode : node) {
					if (subnode.second.IsMap()) {
						ProjectGraph::Detail& detail = projectSource.sources[subnode.first.as<string>()];
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

class ProjectGraphLoader {
public:
	static ProjectGraph load(Project& project);

private:
	ProjectGraphLoader(Project& project, ProjectGraph& projectGraph);

	void add(const string& source);

	void print();

private:
	Project& _project;
	ProjectGraph& _projectGraph;
};
