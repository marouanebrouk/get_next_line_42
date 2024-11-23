/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrouk <mbrouk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:20 by mbrouk            #+#    #+#             */
/*   Updated: 2024/11/20 18:23:08 by mbrouk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*sep_remaining_and_line(char *newline)
{
	char	*str;
	int		i;

	i = 0;
	while (newline[i] && newline[i] != '\n')
		i++;
	if (newline[i] == '\0')
		return (NULL);
	str = ft_substr(newline, i + 1, ft_strlen(newline) - i);
	if (*str == '\0')
	{
		free(str);
		str = NULL;
	}
	newline[i + 1] = '\0';
	return (str);
}

static char	*r_buffer_build_newline(int fd, char *remaining, char *buffer)
{
	ssize_t	bytes_read;
	char	*tmp;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(remaining);
			return (NULL);
		}
		else if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		if (!remaining)
			remaining = ft_strdup("");
		tmp = remaining;
		remaining = ft_strjoin(tmp, buffer);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (remaining);
}

char	*get_next_line(int fd)
{
	static char	*remaining;
	char		*buffer;
	char		*newline;

	if (fd < 0 || BUFFER_SIZE < 0)
	{
		buffer = NULL;
		remaining = NULL;
		return (NULL);
	}
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	newline = r_buffer_build_newline(fd, remaining, buffer);
	free(buffer);
	if (!newline)
		return (NULL);
	remaining = sep_remaining_and_line(newline);
	return (newline);
}
/*
#include <stdio.h>
 int main()
 {
	int count = 1;
	char *line;
	int fd = open("texttest.txt", O_RDONLY);
	while((line = get_next_line(fd)))
	{
		printf("%d :%s",count++,line);
		free(line);
	}
}*/