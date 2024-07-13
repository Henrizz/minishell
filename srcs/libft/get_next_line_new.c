/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_new.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 14:41:49 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/07/06 17:51:04 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	new_line_check(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_excerpt_line(char *temp)
{
	int		i;
	char	*line;

	i = 0;
	if (!temp)
		return (NULL);
	while (temp[i] != '\n' && temp[i] != '\0')
		i++;
	line = (char *)ft_calloc((i + 1 + (new_line_check(temp))), sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (temp[i] != 0 && temp[i] != '\n')
	{
		line[i] = temp[i];
		i++;
	}
	if (temp[i] == '\n')
	{
		line[i] = temp[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*keep_rest(char *temp)
{
	int		i;
	int		j;
	char	*new_temp;

	i = 0;
	j = 0;
	if (temp == NULL)
		return (NULL);
	while (temp[i] != '\n' && temp[i] != '\0')
		i++;
	if (temp[i] == '\0')
		return (free(temp), NULL);
	new_temp = (char *)ft_calloc((ft_strlen(temp) - i + 1), sizeof(char));
	if (!new_temp)
		return (free(temp), NULL);
	i++;
	while (temp[i] != '\0')
		new_temp[j++] = temp[i++];
	new_temp[j] = '\0';
	free(temp);
	if (new_temp[0] == '\0')
		return (free(new_temp), NULL);
	return (new_temp);
}

char	*get_next_line_new(int fd)
{
	static char	*temp;
	char		*line;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (temp)
			free (temp);
		temp = NULL;
		return (NULL);
	}
	buffer = (char *)ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (NULL);
	temp = ft_read_and_check(fd, temp, buffer);
	line = ft_excerpt_line(temp);
	temp = keep_rest(temp);
	if (!line)
	{
		free(temp);
		temp = NULL;
		return (NULL);
	}
	return (line);
}

char	*ft_read_and_check(int fd, char *temp, char *buffer)
{
	int		bytes_read;
	char	*holder;

	bytes_read = 1;
	while (new_line_check(buffer) == 0 && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		if (temp == NULL)
			temp = ft_strdup(buffer);
		else
		{
			holder = ft_strdup(temp);
			free(temp);
			temp = ft_strjoin(holder, buffer);
			free(holder);
		}
	}
	free(buffer);
	if (bytes_read == -1)
		return (free(temp), NULL);
	return (temp);
}
