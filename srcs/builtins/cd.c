#include "../../includes/minishell.h"

//int chdir(const char *path);

void cd(char *path)
{
	//TODO too many arguments
	//TODO improve error display
	if(!chdir(path))
		return;
	else
	{
		printf("minishell: cd: %s: ", path);
		perror(strerror(errno));
		printf("\n");
	}
}

