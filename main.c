#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

void test_file(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        return;
    }

    printf("Testing file: %s\n", filename);
    char *line;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    printf("\n\n");
}

void test_invalid_fd()
{
    printf("Testing invalid file descriptor:\n");
    char *line = get_next_line(-1); // Invalid file descriptor
    if (!line)
        printf("Returned NULL as expected for invalid FD.\n");
    else
        printf("Error: Unexpected non-NULL value for invalid FD.\n");
    free(line);
    printf("\n");
}

void test_large_file()
{
    printf("Testing large dynamically generated input:\n");
    int fd = open("large_test.txt", O_CREAT | O_RDWR, 0644);
    if (fd < 0)
    {
        perror("Error creating large test file");
        return;
    }

    // Generate a file with 1,000,000 lines
    for (int i = 0; i < 1000000; i++)
        dprintf(fd, "Line %d\n", i);

    lseek(fd, 0, SEEK_SET); // Reset file pointer
    char *line;
    int count = 0;

    while ((line = get_next_line(fd)) != NULL && count < 10) // Only read first 10 lines
    {
        printf("%s", line);
        free(line);
        count++;
    }
    printf("... (truncated output for large file test)\n");
    close(fd);
    unlink("large_test.txt"); // Cleanup
    printf("\n");
}

int main()
{
    // Test cases
    test_file("empty_file.txt");          // Empty file
    test_file("single_line.txt");         // Single line file
    test_file("multi_line.txt");          // File with multiple lines
    test_file("no_newline.txt");          // File without a trailing newline
    test_invalid_fd();                    // Invalid FD
    test_large_file();                    // Large file test

    return 0;
}
