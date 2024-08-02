#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"

int main(int argc, char **argv)
{
	int  fd; char *line; if (argc == 1)
		fd = 0; else
		{
			fd = open(argv[1], O_RDONLY);
			if (fd < 0)   return (1);
		}
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	if (fd != 0)
	{
		close(fd);
	}
	return (0);
}
