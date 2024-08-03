#include "/home/olreshet/Desktop/do-ch-cc/get_next_line/get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void test_read_file(const char *filename, int buffer_size)
{
    int fd;
    char *line;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return;
    }
    printf("Testing file: %s with BUFFER_SIZE=%d\n", filename, buffer_size);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
}

void test_multiple_files(int buffer_size)
{
    char *files[] = {"file1.txt", "file2.txt", "file3.txt"};
    int num_files = sizeof(files) / sizeof(files[0]);

    for (int i = 0; i < num_files; i++)
    {
        test_read_file(files[i], buffer_size);
    }
}

void test_edge_cases()
{
    test_read_file("/dev/null", 1024); // empty file
    test_read_file("newline_only.txt", 1); // file with only newlines
}

void benchmark_large_file(const char *filename, int buffer_size)
{
    int fd;
    char *line;
    clock_t start, end;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return;
    }

    start = clock();
    while ((line = get_next_line(fd)) != NULL)
    {
        free(line);
    }
    end = clock();
    close(fd);

    printf("Time taken to read %s with BUFFER_SIZE=%d: %f seconds\n", filename, buffer_size, (double)(end - start) / CLOCKS_PER_SEC);
}

void test_error_handling()
{
    char *line;

    printf("Testing error handling with invalid fd\n");
    line = get_next_line(42);
    if (line == NULL)
        printf("Passed: get_next_line returned NULL for invalid fd\n");
    else
    {
        printf("Failed: get_next_line did not return NULL for invalid fd\n");
        free(line);
    }
}

void test_various_buffer_sizes(const char *filename)
{
    int buffer_sizes[] = {1, 4, 8, 1024, 2048, 4096};
    int num_sizes = sizeof(buffer_sizes) / sizeof(buffer_sizes[0]);

    for (int i = 0; i < num_sizes; i++)
    {
        test_read_file(filename, buffer_sizes[i]);
    }
}

void test_stdin()
{
    char *line;

    printf("Testing reading from stdin\n");
    printf("Enter text (type 'STOP' to end):\n");
    while ((line = get_next_line(STDIN_FILENO)) != NULL)
    {
        if (strcmp(line, "STOP\n") == 0 || strcmp(line, "STOP\r\n") == 0)
        {
            free(line);
            break;
        }
        printf("%s", line);
        free(line);
    }
}

void test_multiple_fd()
{
    int fd1, fd2, fd3;
    char *line1, *line2, *line3;

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
    line1 = get_next_line(fd1);
    line2 = get_next_line(fd2);
    line3 = get_next_line(fd3);
    if (line1) printf("File1: %s", line1);
    if (line2) printf("File2: %s", line2);
    if (line3) printf("File3: %s", line3);
    free(line1);
    free(line2);
    free(line3);

    close(fd1);
    close(fd2);
    close(fd3);
}

void create_test_files()
{
    system("echo 'file1 line1\nfile1 line2' > file1.txt");
    system("echo 'file2 line1\nfile2 line2' > file2.txt");
    system("echo 'file3 line1\nfile3 line2' > file3.txt");
    system("echo '\n\n\n' > newline_only.txt");
    system("base64 /dev/urandom | head -c 2000000 > large_file.txt");
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <filename1> <filename2> ...\n", argv[0]);
        return (1);
    }

    create_test_files();

    // Test with multiple files
    for (int i = 1; i < argc; i++)
    {
        test_read_file(argv[i], 1024);
    }

    // Test edge cases
    printf("\n--- Testing edge cases ---\n");
    test_edge_cases();

    // Test error handling
    printf("\n--- Testing error handling ---\n");
    test_error_handling();

    // Test various buffer sizes
    printf("\n--- Testing various buffer sizes ---\n");
    for (int i = 1; i < argc; i++)
    {
        test_various_buffer_sizes(argv[i]);
    }

    // Test reading from stdin
    printf("\n--- Testing reading from stdin ---\n");
    test_stdin();

    // Test multiple file descriptors
    printf("\n--- Testing multiple file descriptors ---\n");
    test_multiple_fd();

    // Benchmark performance with a large file
    printf("\n--- Benchmarking performance ---\n");
    benchmark_large_file("large_file.txt", 1024);

    return (0);
}

