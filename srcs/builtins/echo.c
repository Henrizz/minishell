#include "../../includes/minishell.h"
//TODO eliminate quotes in the string if any
//TODO handle escape characters
void	echo(char **str)
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
			if(str[i + 1] != NULL)
				printf(" ");
			i++;
		}
		if(ft_strncmp(str[1], "-n", len) != 0)
			printf("\n");
	}
}