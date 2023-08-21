#include "Starter.h"
#include "Builder.h"

string GetCompilerPath()
{
	string language = "cpp";
	string name = "clang";
	vector<string> paths = CompilerPath::Generate(language, name);

	for (const string& path : paths)
	{
		if (fs::exists(path))
		{
			return path;
		}
	}

	cout << "compiler file for '" << name << "' were not found\n";
	return "";
}

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

void InformCompiler()
{
	string compilerPath = GetCompilerPath();
	if (compilerPath.empty())
	{
		return;
	}

	Compiler compiler = CompilerLoader::Load(compilerPath);
	YAML::Node node;
	node = compiler;
	std::cout << node << "\n";
}

void InformProject()
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

void InformProjectDependency()
{
	string projectPath = GetProjectPath();
	if (projectPath.empty())
	{
		return;
	}

	Project project = ProjectLoader::Load(projectPath);
	YAML::Node projectNode;
	projectNode.push_back(project);

	for (const string& dependency : project.dependencies) 
	{
		string dependencyProjectPath = ProjectPath::Generate(dependency);

		if (fs::exists(dependencyProjectPath))
		{
			Project dependencyProject = ProjectLoader::Load(dependencyProjectPath);
			projectNode.push_back(dependencyProject);
		}
	}

	cout << projectNode << "\n";
}

void InformProjectSource()
{
	string projectPath = GetProjectPath();
	if (projectPath.empty())
	{
		return;
	}

	Project project = ProjectLoader::Load(projectPath);
	ProjectSource projectSource = ProjectSourceLoader::load(project);
	YAML::Node projectSourceNode;
	projectSourceNode = projectSource;

	cout << projectSourceNode << "\n";
}

void Build()
{
	string compilerPath = GetCompilerPath();
	if (compilerPath.empty())
	{
		return;
	}

	string projectPath = GetProjectPath();
	if (projectPath.empty())
	{
		return;
	}

	Compiler compiler = CompilerLoader::Load(compilerPath);
	Project project = ProjectLoader::Load(projectPath);

	for (const string& dependency : project.dependencies) 
	{
		string dependencyProjectPath = ProjectPath::Generate(dependency);

		if (fs::exists(dependencyProjectPath))
		{
			Project dependencyProject = ProjectLoader::Load(dependencyProjectPath);
			project.libraries.push_back(dependencyProject.name);

			for (size_t i = 0; i < dependencyProject.headerDirectories.size(); ++i) 
			{
				const string& headerDirectory = dependencyProject.headerDirectories[i];
				project.headerDirectories.push_back(dependency + "/" + headerDirectory);
				dependencyProject.headerDirectories[i] = dependency + "/" + headerDirectory;
			}

			for (size_t i = 0; i < dependencyProject.sources.size(); ++i) 
			{
				const string& source = dependencyProject.sources[i];
				dependencyProject.sources[i] = dependency + "/" + source;
			}

			Builder builder(compiler, dependencyProject);
			builder.Build();

			project.libraries.push_back(dependencyProject.name);
		}
	}

	project.libraryDirectories.push_back("build/library");

	Builder builder(compiler, project);
	builder.Build();
}

void Clean()
{
	string compilerPath = GetCompilerPath();
	if (compilerPath.empty())
	{
		return;
	}

	string projectPath = GetProjectPath();
	if (projectPath.empty())
	{
		return;
	}

	Compiler compiler = CompilerLoader::Load(compilerPath);
	Project project = ProjectLoader::Load(projectPath);
	Builder builder(compiler, project);
	builder.Clean();
}

void Run()
{
	string compilerPath = GetCompilerPath();
	if (compilerPath.empty())
	{
		return;
	}

	string projectPath = GetProjectPath();
	if (projectPath.empty())
	{
		return;
	}

	Compiler compiler = CompilerLoader::Load(compilerPath);
	Project project = ProjectLoader::Load(projectPath);
	Builder builder(compiler, project);

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
		if (command == "inform-project")
		{
			InformProject();
			return 0;
		}

		if (command == "inform-project-dependency")
		{
			InformProjectDependency();
			return 0;
		}


		if (command == "inform-project-source")
		{
			InformProjectSource();
			return 0;
		}

		if (command == "inform-compiler")
		{
			InformCompiler();
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
