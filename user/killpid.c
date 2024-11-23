#include "user/user.h"
#include "kernel/types.h"
#include "kernel/param.h"

int main()
{
    int pid = fork();

    if (pid < 0)
    {
        // Fork failed
        printf("Fork failed\n");
        exit(1);
    }

    if (pid == 0)
    {
        // Child process
        printf("Child process: PID = %d, PPID = %d\n", getpid(), getppid());

        while (1)
        {
            // Busy loop to keep the child alive until it is killed
        }
    }
    else
    {
        // Parent process
        sleep(20); // Sleep for a moment to let the child run

        printf("Parent process: PID = %d, PPID = %d, killing child PID = %d\n", getpid(), getppid(), pid);

        // Use killpid to terminate the child
        if (killpid(pid) == 0)
        {
            printf("Successfully killed child process %d\n", pid);
        }
        else
        {
            printf("Failed to kill child process %d\n", pid);
        }

        wait(0); // Wait for the child process to ensure cleanup
    }

    exit(0);
}
