/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olreshet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 00:24:18 by olreshet          #+#    #+#             */
/*   Updated: 2024/08/04 01:19:43 by olreshet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "/home/olreshet/Desktop/do-ch-cc/get_next_line/get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void test_multiple_fd()
{
    int fd1, fd2, fd3;
    char *line1 = NULL, *line2 = NULL, *line3 = NULL;

    fd1 = open("file1.txt", O_RDONLY);
    fd2 = open("file2.txt", O_RDONLY);
    fd3 = open("file3.txt", O_RDONLY);

    if (fd1 == -1 || fd2 == -1 || fd3 == -1)
    {
        perror("Error opening one of the files");
        if (fd1 != -1) close(fd1);
        if (fd2 != -1) close(fd2);
        if (fd3 != -1) close(fd3);
        return;
    }

    printf("Testing multiple file descriptors\n");
    while ((line1 = get_next_line(fd1)) != NULL ||
           (line2 = get_next_line(fd2)) != NULL ||
           (line3 = get_next_line(fd3)) != NULL)
    {
        if (line1)
        {
            printf("File1: %s", line1);
            free(line1);
            line1 = NULL;
        }
        if (line2)
        {
            printf("File2: %s", line2);
            free(line2);
            line2 = NULL;
        }
        if (line3)
        {
            printf("File3: %s", line3);
            free(line3);
            line3 = NULL;
        }
    }

    close(fd1);
    close(fd2);
    close(fd3);
}

int main()
{
    // Test multiple file descriptors
    printf("\n--- Testing multiple file descriptors ---\n");
    test_multiple_fd();

    return (0);
}
