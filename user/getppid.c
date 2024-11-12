#include "user/user.h"
#include "kernel/types.h"

int main()
{

    int pid = fork();

    if(pid==0)
    {
        // Child process

        printf("I am child process %d and my parent is %d\n",getpid(),getppid());
    }

    else{
        wait(0);
        printf("I am a parent process with %d and my parent is %d\n",getpid(),getppid());
    }

    // printf("current pid is: %d\n", getpid());
    // printf("parent pid is: %d\n", getppid());
    exit(0);
}
