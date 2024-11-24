#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int pipe_fds[2]; // Array to hold pipe file descriptors
    char buffer[100];
    int initial_read_count;

    // Create a pipe
    if (pipe(pipe_fds) < 0)
    {
        printf("Pipe creation failed\n");
        exit(1);
    }

    // Record the initial read count
    initial_read_count = getreadcount();

    // Fork the process
    int pid = fork();

    if (pid < 0)
    {
        printf("Fork failed\n");
        exit(1);
    }

    if (pid == 0)
    {
        // Child process: Writes to the pipe
        printf("Child process: Writing to pipe...\n");
        char *message = "Hello from child!\n";
        write(pipe_fds[1], message, strlen(message));
        printf("Child process: Message written\n");
        close(pipe_fds[1]); // Close the write end of the pipe
        printf("The initial read count is : %d\n",initial_read_count);
        exit(0);
    }
    else
    {
        // Parent process: Reads from the pipe
        // printf("Parent process: Waiting for child to complete...\n");
        wait(0);

        printf("Parent process: Reading from pipe...\n");
        int bytes_read = read(pipe_fds[0], buffer, sizeof(buffer));
        if (bytes_read < 0)
        {
            printf("Parent: Read failed\n");
            close(pipe_fds[0]); // Close the read end of the pipe
            exit(1);
        }
        buffer[bytes_read] = '\0'; // Null-terminate the buffer

        printf("Parent read: %s\n", buffer);

        // Get the updated read count
        int parent_read_count = getreadcount();
        printf("Parent read count: %d\n", parent_read_count);

        close(pipe_fds[0]); // Close the read end of the pipe
    }

    exit(0);
}
