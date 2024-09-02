#include "../../includes/minishell.h"
//TODO eliminate quotes in the string if any
//TODO handle escape characters
void	echo(char **str, t_global *global)
{
	int	i;
	size_t	len;

	i = 1;
	if(str[i])
	{
		len = ft_strlen(str[1]);
		if(ft_strncmp(str[1], "-n", len) == 0)
			i++;
		while(str[i])
		{
			printf("%s", str[i]);
			//ft_putstr_fd(str[i], 1);
			if(str[i + 1] != NULL)
				printf(" ");
				//ft_putstr_fd(" ", 1);
			i++;
		}
		if(ft_strncmp(str[1], "-n", len) != 0)
			printf("\n");
			//ft_putstr_fd("\n", 1);
	}
	global->exit_status = 0;
}
