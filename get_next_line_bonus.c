/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olreshet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 23:06:16 by olreshet          #+#    #+#             */
/*   Updated: 2024/08/07 09:43:27 by olreshet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#define MAX_FD 1024

static char	*ft_free_buffers(char *read_buffer, char *buffered_content)
{
	free (read_buffer);
	free (buffered_content);
	return (NULL);
}

char	*read_file(int fd, char *buffered_content)
{
	char	*buffer;
	int		byte_read;

	if (!buffered_content)
		buffered_content = ft_strdup("");
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (free(buffered_content), NULL);
	byte_read = 1;
	while (byte_read > 0 && !ft_strchr(buffered_content, '\n'))
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
			return (ft_free_buffers(buffer, buffered_content));
		if (byte_read > 0)
		{
			buffer[byte_read] = '\0';
			buffered_content = ft_strjoin(buffered_content, buffer);
			if (!buffered_content)
				return (free(buffer), NULL);
		}
	}
	return (free(buffer), buffered_content);
}

static char	*ft_get_line(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (buffer[i] == '\0')
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	line = malloc((i + 1) * sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] != '\0')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*ft_clear_buffer(char *buffer, char *line)
{
	int		i;
	int		j;
	char	*rest_of_str;

	i = ft_strlen(line);
	j = ft_strlen(buffer);
	if (j - 1 <= 0)
		return (free(buffer), NULL);
	rest_of_str = malloc(j - i + 1);
	j = 0;
	while (buffer[i] != '\0')
		rest_of_str[j++] = buffer[i++];
	rest_of_str[j] = '\0';
	return (free(buffer), rest_of_str);
}

char	*get_next_line(int fd)
{
	static char	*buffer[MAX_FD];
	char		*line;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
	{
		if (fd >= 0 && fd < MAX_FD && buffer[fd])
		{
			free(buffer[fd]);
			buffer[fd] = NULL;
		}
		return (NULL);
	}
	buffer[fd] = read_file(fd, buffer[fd]);
	if (!buffer[fd])
		return (NULL);
	line = ft_get_line(buffer[fd]);
	if (!line)
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
	}
	else
		buffer[fd] = ft_clear_buffer(buffer[fd], line);
	return (line);
}
