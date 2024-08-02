/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olreshet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 17:47:23 by olreshet          #+#    #+#             */
/*   Updated: 2024/08/02 06:57:26 by yourLogin        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	{
		free (buffered_content);
		return (NULL);
	}
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
		}
	}
	free (buffer);
	return (buffered_content);
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
	{
		free(buffer);
		return (NULL);
	}
	rest_of_str = malloc(j - i + 1);
	if (!rest_of_str)
	{
		free (buffer);
		return (NULL);
	}
	j = 0;
	while (buffer[i] != '\0')
		rest_of_str[j++] = buffer[i++];
	rest_of_str[j] = '\0';
	free (buffer);
	return (rest_of_str);
}

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (buffer)
		{
			free(buffer);
			buffer = NULL;
		}
		return (NULL);
	}
	buffer = read_file(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_get_line(buffer);
	if (!line)
	{
		free(buffer);
		buffer = NULL;
	}
	else
		buffer = ft_clear_buffer(buffer, line);
	return (line);
}
