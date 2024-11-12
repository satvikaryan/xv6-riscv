#include "user/user.h"
#include "kernel/types.h"

int main() {
    int pid = getpid();
    printf("My process ID is %d\n", pid);

    int child_pid = fork();
    if (child_pid == 0) {
        printf("Child process with PID %d running.\n", getpid());
        sleep(10);
        printf("Child process with PID %d exiting.\n", getpid());
        exit(0);
    } else {
        printf("Killing child process with PID %d\n", child_pid);
        kilpid(child_pid);
        wait(0);
        printf("Child process killed.\n");
    }
    exit(0);
}
