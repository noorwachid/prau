#pragma once

#include "Common.h"
#include "yaml-cpp/yaml.h"

struct Compiler
{
	string program;
	string standard;
	vector<string> flags;

	struct
	{
		string header;
		string library;
	} directory;

	struct
	{
		string object;
		string library;
		string executable;
	} file;

	struct
	{
		string object;
		string library;
		string executable;
	} builder;
};

struct CompilerPath
{
	static vector<string> Generate(const string& language, const string& name);
};

struct CompilerLoader
{
	static Compiler Load(const string& path);
};

namespace YAML
{
	template <>
	struct convert<Compiler>
	{
		static Node encode(const Compiler& package)
		{
			Node node;
			node["program"] = package.program;
			node["standard"] = package.standard;

			if (!package.flags.empty())
				node["flags"] = package.flags;

			node["directory"]["header"] = package.directory.header;
			node["directory"]["library"] = package.directory.library;

			node["file"]["object"] = package.file.object;
			node["file"]["library"] = package.file.library;
			node["file"]["executable"] = package.file.executable;

			node["builder"]["object"] = package.builder.object;
			node["builder"]["library"] = package.builder.library;
			node["builder"]["executable"] = package.builder.executable;

			return node;
		}

		static bool decode(const Node& node, Compiler& compiler)
		{
			if (node["program"] && node["program"].IsScalar())
				compiler.program = node["program"].as<string>();
			if (node["standard"] && node["standard"].IsScalar())
				compiler.standard = node["standard"].as<string>();
			if (node["flags"] && node["flags"].IsSequence())
				compiler.flags = node["flags"].as<vector<string>>();

			if (node["directory"] && node["directory"].IsMap())
			{
				YAML::Node subnode = node["directory"];
				if (subnode["header"] && subnode["header"].IsScalar())
					compiler.directory.header = subnode["header"].as<string>();
				if (subnode["library"] && subnode["library"].IsScalar())
					compiler.directory.library = subnode["library"].as<string>();
			}

			if (node["file"] && node["file"].IsMap())
			{
				YAML::Node subnode = node["file"];
				if (subnode["object"] && subnode["object"].IsScalar())
					compiler.file.object = subnode["object"].as<string>();
				if (subnode["library"] && subnode["library"].IsScalar())
					compiler.file.library = subnode["library"].as<string>();
				if (subnode["executable"] && subnode["executable"].IsScalar())
					compiler.file.executable = subnode["executable"].as<string>();
			}

			if (node["builder"] && node["builder"].IsMap())
			{
				YAML::Node subnode = node["builder"];
				if (subnode["object"] && subnode["object"].IsScalar())
					compiler.builder.object = subnode["object"].as<string>();
				if (subnode["library"] && subnode["library"].IsScalar())
					compiler.builder.library = subnode["library"].as<string>();
				if (subnode["executable"] && subnode["executable"].IsScalar())
					compiler.builder.executable = subnode["executable"].as<string>();
			}

			return true;
		}
	};
}
