#include "../../includes/minishell.h"
//TODO eliminate quotes in the string if any
void	echo(char **argv)
{
	int	i;
	size_t	len;

	i = 1;
	if(argv[i])
	{
		len = ft_strlen(argv[1]);
		if(ft_strncmp(argv[1], "-n", len) == 0)
			i++;
		while(argv[i])
			{
				printf("%s", argv[i]); // OK
				if(argv[i + 1] != NULL)
					printf(" "); // OK
				i++;
			}
		if(ft_strncmp(argv[1], "-n", len) != 0)
			printf("\n"); // OK
	}
}