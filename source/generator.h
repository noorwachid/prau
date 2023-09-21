#pragma once

#include "workspace.h"

class Generator {
public:
	Generator(Workspace& workspace);

	void generate(const string& target);

	void generateClangD();

	void generateCMake();

	void generateSh();

private:
	Workspace& _workspace;
};
