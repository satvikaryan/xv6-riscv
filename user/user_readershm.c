#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/shm.h"

#define MEMORY_KEY 1000  
#define MEMORY_SIZE 2048 

int main()
{
    int memory_id;
    char *shared_data;
    char message[200];

    
    memory_id = shmget(MEMORY_KEY, MEMORY_SIZE, 0); 
    if (memory_id < 0)
    {
        printf("Error: Unable to access shared memory.\n");
        exit(1);
    }
    printf("Shared memory accessed successfully. Memory ID: %d\n", memory_id);

    
    shared_data = (char *)shmat(memory_id);
    if (shared_data == (char *)-1)
    {
        printf("Error: Could not attach to shared memory.\n");
        exit(1);
    }
    printf("Attached to shared memory segment.\n");

   
    printf("Reading content from shared memory...\n");
    int i = 0;
    while (shared_data[i] != '#' && i < MEMORY_SIZE - 1)
    { 
        message[i] = shared_data[i];
        i++;
    }
    message[i] = '\0'; 

    if (i == 0)
    {
        printf("No data found in shared memory.\n");
    }
    else
    {
        printf("Message read: %s\n", message);
    }

    
    if (shmdt(memory_id) < 0)
    {
        printf("Error: Failed to detach from shared memory.\n");
        exit(1);
    }
    printf("Detached from shared memory.\n");

    
    if (shmctl(memory_id, IPC_RMID) < 0)
    {
        printf("Error: Unable to delete shared memory segment.\n");
        exit(1);
    }
    printf("Shared memory deleted successfully.\n");

    exit(0);
}
