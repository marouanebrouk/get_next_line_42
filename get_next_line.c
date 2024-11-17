#include "get_next_line.h"

static char *ft_strchr(char *s, int c)
{
    int     i;

    i = 0;
    while(s[i])
    {
        if (s[i] == c)
            return ((char *)(s + i));
        i++;
    }
    if(c == '\0')
        return ((char *)(s + i));
    return (NULL);
}

static char	*r_buffer_build_line(int fd, char *remaining, char *buffer)
{
	ssize_t	b_read;
	char	*tmp;

	b_read = 1;
	while (b_read > 0)
	{
		b_read = read(fd, buffer, BUFFER_SIZE);
		if (b_read == -1)
		{
			free(remaining);
			return (NULL);
		}
		else if (b_read == 0)
            break ;
                            
        buffer[b_read] = '\0'; 

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

static char *sep_line_and_remaining(char *line)  
{
    char    *str;
    ssize_t    i;
    
    i = 0;
    
    while (line[i] && line[i] != '\n')
        i++;

    if (line[i] == '\0' || line[1]=='\0')
        return (NULL);
                            
    str = ft_substr(line, i + 1, ft_strlen(line) - i);
    if (*str == '\0')
    {
        free(str);
        str = NULL;
    }
    line[i + 1] = '\0';
    return (str);
}

char    *get_next_line(int fd)
{
    static char *remaining;
    char        *buffer;
    char        *line;
    
    buffer = (char *)malloc((BUFFER_SIZE + 1));

    if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
    {
        free(buffer);
        free(remaining);
        remaining = NULL;
        buffer = NULL;
        return (NULL);
    }
    if (!buffer)
        return (NULL);
          
    line = r_buffer_build_line(fd, remaining, buffer);
    free(buffer);
    buffer = NULL;
    if (!line)
        return (NULL);
    remaining = sep_line_and_remaining(line);
    return (line); 
}
/*
int main()
{
    int fd = open("text.txt",O_RDONLY);
    char *ster;
    while((ster = get_next_line(fd)))
        printf("%s",ster);
}*/