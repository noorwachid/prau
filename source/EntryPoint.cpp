#include "Starter.h"
#include "Builder.h"

string GetProjectPath()
{
	string projectPath = ProjectPath::Generate(".");

	if (!fs::exists(projectPath))
	{
		cout << "project file were not found\n";
		return "";
	}

	return projectPath;
}

void InfoProject()
{
	string projectPath = GetProjectPath();
	if (projectPath.empty())
	{
		return;
	}

	Project project = ProjectLoader::Load(projectPath);
	YAML::Node projectNode;
	projectNode = project;
	cout << projectNode << "\n";
}

void InfoProjectGraph()
{
	string projectPath = GetProjectPath();
	if (projectPath.empty())
	{
		return;
	}

	Project project = ProjectLoader::Load(projectPath);
	ProjectGraph projectSource = ProjectGraphLoader::load(project);
	YAML::Node projectSourceNode;
	projectSourceNode = projectSource;

	cout << projectSourceNode << "\n";
}

void Build()
{
	string projectPath = GetProjectPath();
	if (projectPath.empty())
	{
		return;
	}

	Compiler* compiler = new ClangCompiler();
	Project project = ProjectLoader::Load(projectPath);
	Builder builder(*compiler, project);
	builder.Build();
}

void Clean()
{
	string projectPath = GetProjectPath();
	if (projectPath.empty())
	{
		return;
	}

	Compiler* compiler = new ClangCompiler();
	Project project = ProjectLoader::Load(projectPath);
	Builder builder(*compiler, project);
	builder.Clean();
}

void Run()
{
	string projectPath = GetProjectPath();
	if (projectPath.empty())
	{
		return;
	}

	Compiler* compiler = new ClangCompiler();
	Project project = ProjectLoader::Load(projectPath);
	Builder builder(*compiler, project);

	builder.Run();
}

void Create(const string& directory)
{
	if (fs::exists(directory))
	{
		cout << directory << " already exists\n";
		return;
	}

	Starter starter;
	starter.Generate(directory);
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "no command were specified\n";
		return 0;
	}

	string command = argv[1];

	try
	{
		if (command == "info-project")
		{
			InfoProject();
			return 0;
		}

		if (command == "info-project-graph")
		{
			InfoProjectGraph();
			return 0;
		}

		if (command == "build")
		{
			Build();
			return 0;
		}

		if (command == "run")
		{
			Run();
			return 0;
		}

		if (command == "create")
		{
			if (argc < 3)
			{
				cout << "please specify the new directory\n";
				return 0;
			}

			Create(argv[2]);
			return 0;
		}

		if (command == "clean")
		{
			Clean();
			return 0;
		}

		cout << "unknown command\n";
		return 0;
	}
	catch (const std::exception& e)
	{
		cout << e.what() << "\n";
	}

	return 0;
}
