#pragma once

#include "Common.h"
#include "yaml-cpp/yaml.h"

struct Project
{
	string name;
	string description;
	string type;
	string language;
	string standard;
	string license;
	string url;
	vector<string> authors;

	vector<string> sources;
	vector<string> headers;
	vector<string> headerDirectories;

	vector<string> libraries;
	vector<string> libraryDirectories;

	vector<string> assets;
	vector<string> dependencies;
};

struct ProjectPath
{
	static string Generate(const string& directory);
};

struct ProjectLoader
{
	static Project Load(const string& path);

	static bool Check(const Project& project);
};

namespace YAML
{
	template <>
	struct convert<Project>
	{
		static Node encode(const Project& project)
		{
			Node node;
			if (!project.name.empty())
				node["name"] = project.name;
			if (!project.description.empty())
				node["description"] = project.description;
			if (!project.type.empty())
				node["type"] = project.type;
			if (!project.language.empty())
				node["language"] = project.language;
			if (!project.standard.empty())
				node["standard"] = project.standard;

			if (!project.license.empty())
				node["license"] = project.license;
			if (!project.url.empty())
				node["url"] = project.url;
			if (!project.authors.empty())
				node["authors"] = project.authors;

			if (!project.sources.empty())
				node["sources"] = project.sources;
			if (!project.headers.empty())
				node["headers"] = project.headers;
			if (!project.headerDirectories.empty())
				node["header-directories"] = project.headerDirectories;
			if (!project.libraries.empty())
				node["librarys"] = project.libraries;
			if (!project.libraryDirectories.empty())
				node["library-directories"] = project.libraryDirectories;

			if (!project.assets.empty())
				node["assets"] = project.assets;
			if (!project.dependencies.empty())
				node["dependencies"] = project.dependencies;

			return node;
		}

		static bool decode(const Node& node, Project& project)
		{
			if (node["name"] && node["name"].IsScalar())
				project.name = node["name"].as<string>();
			if (node["description"] && node["description"].IsScalar())
				project.description = node["description"].as<string>();
			if (node["type"] && node["type"].IsScalar())
				project.type = node["type"].as<string>();
			if (node["language"] && node["language"].IsScalar())
				project.language = node["language"].as<string>();
			if (node["standard"] && node["standard"].IsScalar())
				project.standard = node["standard"].as<string>();
			if (node["license"] && node["license"].IsScalar())
				project.license = node["license"].as<string>();
			if (node["url"] && node["url"].IsScalar())
				project.url = node["url"].as<string>();
			if (node["authors"] && node["authors"].IsSequence())
				project.authors = node["authors"].as<vector<string>>();

			if (node["sources"] && node["sources"].IsSequence())
				project.sources = node["sources"].as<vector<string>>();
			if (node["headers"] && node["headers"].IsSequence())
				project.headers = node["headers"].as<vector<string>>();
			if (node["header-directories"] && node["header-directories"].IsSequence())
				project.headerDirectories = node["header-directories"].as<vector<string>>();
			if (node["libraries"] && node["libraries"].IsSequence())
				project.libraries = node["libraries"].as<vector<string>>();
			if (node["library-directories"] && node["library-directories"].IsSequence())
				project.libraryDirectories = node["library-directories"].as<vector<string>>();

			if (node["assets"] && node["assets"].IsSequence())
				project.assets = node["assets"].as<vector<string>>();
			if (node["dependencies"] && node["dependencies"].IsSequence())
				project.dependencies = node["dependencies"].as<vector<string>>();

			return true;
		}
	};
}
