#include "../../includes/minishell.h"

//int chdir(const char *path);

void cd(char *path)
{
	if(!chdir(path))
		return;
	else
		printf("minishell: cd: %s: %s\n", path, strerror(errno)); //channel 2
}