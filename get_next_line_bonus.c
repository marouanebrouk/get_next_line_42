/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrouk <mbrouk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:55:02 by mbrouk            #+#    #+#             */
/*   Updated: 2024/11/20 16:01:44 by mbrouk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*sep_remaining_and_line(char *newline)
{
	char	*str;
	size_t	i;

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

static char	*r_buffer_build_newline(int fd, char *remaining[fd], char *buffer)
{
	ssize_t	bytes_read;
	char	*tmp;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(remaining[fd]);
			return (NULL);
		}
		else if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		if (!remaining[fd])
			remaining[fd] = ft_strdup("");
		tmp = remaining[fd];
		remaining[fd] = ft_strjoin(tmp, buffer);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (remaining[fd]);
}

char	*get_next_line(int fd)
{
	static char	*remaining[1024];
	char		*buffer;
	char		*newline;

	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(buffer);
		free(remaining[fd]);
		buffer = NULL;
		remaining[fd] = NULL;
		return (NULL);
	}
	if (!buffer)
		return (NULL);
	newline = r_buffer_build_newline(fd, remaining, buffer);
	free(buffer);
	buffer = NULL;
	if (!newline)
		return (NULL);
	remaining[fd] = sep_remaining_and_line(newline);
	return (newline);
}

// int main (int ac, char **av)
// {
// 	ac = ac;
// 	char *str1;
// 	char *str2;
// 	char *str3;
// 	int fd1 = open(av[1],O_RDONLY);
// 	int fd2 = open(av[2],O_RDONLY);
// 	int fd3 = open(av[3],O_RDONLY);
// while (1)
// {
// 	str1 = get_next_line(fd1);
// 	str2 = get_next_line(fd2);
// 	str3 = get_next_line(fd3);

// 	if(str1)
// 		printf("%s",str1);
// 	if(str2)
// 		printf("%s",str2);
// 	if(str3)
// 		printf("%s",str3);
// 	if (!str1 || !str2 || !str3)
// 		break;
// }
// }
